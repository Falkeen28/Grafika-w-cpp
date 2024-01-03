#ifndef EKRAN_H
#define EKRAN_H

#include <QWidget>
#include <QPainter>
#include <vector>
#include <QStack>
#include <QColorDialog>

class Ekran : public QWidget
{
    Q_OBJECT
public:
    explicit Ekran(QWidget *parent = nullptr);
    double Tx   = 0.0;
    double Ty   = 0.0;
    double alfa = 0.0;
    double Sx   = 1.0;
    double Sy   = 1.0;
    double Shx  = 0.0;
    double Shy  = 0.0;
    bool interpolacja_status = false;
    void czysc()
    {
        im.fill(0);
    }
    void drawPixel(int x, int y, int B, int G, int R)
    {
        int width = im.width();
        int height = im.height();
        if ((x >= 0) && (x <= width) && (y >= 0) && (y <= height))
        {
            uchar *pix = im.scanLine(y); // dostęp do piksela?
            pix[4 * x] = B;              // blue
            pix[4 * x + 1] = G;          // green
            pix[4 * x + 2] = R;          // red
        }
    }
    void mnozenie(double A[3][3], double B[3][3], double C[3][3])
    {
        for (int i = 0; i < 3; i++)
        {
            for (int j = 0; j < 3; j++)
            {
                double sum = 0;
                for (int k = 0; k < 3; k++)
                {
                    sum += A[i][k] * B[k][j];
                }
                C[i][j] = sum;
            }
        }
    }
    void operacje2D(double x, double y, double alfa, double Sx, double Sy, double Shx, double Shy)
    {
        double T2[3][3] = {{1, 0, -320},
                           {0, 1, -320},
                           {0, 0, 1}};
        // T2 = obrazek wraca na srodek

        double T1[3][3] = {{1, 0, 250},
                           {0, 1, 250},
                           {0, 0, 1}};
        // T1 = T(250, 250) - Srodek obrazka na 0,0

        // macierz translacji Txy-1 o x i y
        double Txy[3][3] = {{1, 0, -x},
                            {0, 1, -y},
                            {0, 0, 1}};

        // macierz rotacji R^-1 o kat alfa
        double a = alfa * (M_PI / 180); // zamiana na radiany
        double R[3][3] = {{cos(a), sin(a), 0},
                          {-sin(a), cos(a), 0},
                          {0, 0, 1}};

        // macierz skalowania
        double S[3][3] = {{1.0 / Sx, 0, 0},
                          {0, 1.0 / Sy, 0},
                          {0, 0, 1}};

        // macierze pochylenia
        double Sh_x[3][3] = {{1, -Shx, 0},
                             {0, 1, 0},
                             {0, 0, 1}};

        double Sh_y[3][3] = {{1, 0, 0},
                             {-Shy, 1, 0},
                             {0, 0, 1}};

        // zlozona maciez glowna (Poniewaz wszystkie sa juz odwrocone to kolejnosc mnozenia wyglaga tak: M = ((((T1 * R) * S) * Sh_xy) * Txy) * T2
        double M[3][3];

        mnozenie(T1, R, M);
        mnozenie(M, S, M);
        mnozenie(M, Sh_y, M);
        mnozenie(M, Sh_x, M);
        mnozenie(M, Txy, M);
        mnozenie(M, T2, M);

        // iteracja po wszystkich pikselach obrazka im2 i przemnozenie ich przez macierz M^-1
        for (int i = 0; i < im.height(); i++)
        {
            for (int j = 0; j < im.width(); j++)
            {
                double P[3][3] = {{(double)i}, {(double)j}, {1}};
                double Pprim[3][3];

                mnozenie(M,P,Pprim);

                QColor color;
                if (Pprim[0][0] >= 0 && Pprim[0][0] < 500 && Pprim[1][0] >= 0 && Pprim[1][0] < 490)
                {

                    if (interpolacja_status == true)
                        color = InterpolacjaDwuliniowa(&im2, Pprim[0][0], Pprim[1][0]);

                    else
                        color = im2.pixelColor(Pprim[0][0], Pprim[1][0]);
                }
                else
                    color = QColor(0, 0, 0);

                drawPixel(i, j, color.blue(), color.green(), color.red());
            }
        }
    }
    QColor InterpolacjaDwuliniowa(QImage* im, double x, double y)
    {
        if (x >= 0 && x <= 500 && y >= 0 && y <= 500)
        {
            int x0 = static_cast<int>(x);
            int x1 = x0 + 1;
            int y0 = static_cast<int>(y);
            int y1 = y0 + 1;

            double dx = x - x0;
            double dy = y - y0;

            const uchar* bits = im->constBits();

            int red = static_cast<int>((1 - dx) * (1 - dy) * bits[4 * (x0 + 500 * y0) + 2]
                      + dx * (1 - dy) * bits[4 * (x1 + 500 * y0) + 2]
                      + (1 - dx) * dy * bits[4 * (x0 + 500 * y1) + 2]
                      + dx * dy * bits[4 * (x1 + 500 * y1) + 2]);

            int green = static_cast<int>((1 - dx) * (1 - dy) * bits[4 * (x0 + 500 * y0) + 1]
                        + dx * (1 - dy) * bits[4 * (x1 + 500 * y0) + 1]
                        + (1 - dx) * dy * bits[4 * (x0 + 500 * y1) + 1]
                        + dx * dy * bits[4 * (x1 + 500 * y1) + 1]);

            int blue = static_cast<int>((1 - dx) * (1 - dy) * bits[4 * (x0 + 500 * y0) + 0]
                       + dx * (1 - dy) * bits[4 * (x1 + 500 * y0) + 0]
                       + (1 - dx) * dy * bits[4 * (x0 + 500 * y1) + 0]
                       + dx * dy * bits[4 * (x1 + 500 * y1) + 0]);

            return QColor(red, green, blue);
        }
        else
        {
            return QColor(0, 0, 0); // kolor tła
        }
    }

protected:
    void paintEvent(QPaintEvent *event);
signals:

public slots:
    void setRotation(int value);
    void setXTranslation(int value);
    void setYTranslation(int value);
    void setXScaling(int value);
    void setYScaling(int value);
    void setXShearing(int value);
    void setYShearing(int value);
    void reset();
    void interpolacja_on();

public slots:

private:
    QImage im, im2, temp;
};

#endif // EKRAN_H
