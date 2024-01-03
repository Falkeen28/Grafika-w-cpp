#ifndef EKRAN_H
#define EKRAN_H

#include <QWidget>
#include <QPainter>

class Ekran : public QWidget
{
    Q_OBJECT
public:
    explicit Ekran(QWidget *parent = nullptr);
    void czysc();
    int stopien = 0;
    bool obrazekSTATUS = false;
    QVector<QPointF> punkty;
    QVector<QPointF> punkty2;
    QVector<QPointF> wynik;
    void drawPixel(int x, int y, int red, int green, int blue, int warstwa)
    {
        int width = im.width();
        int height = im.height();

        if ((x >= 0) && (x <= width) && (y >= 0) && (y <= height))
        {
            if (warstwa == 1)
            {
                uchar *pix = obraz.scanLine(y); // dostęp do piksela
                pix[4 * x] = blue;              // blue
                pix[4 * x + 1] = green;         // green
                pix[4 * x + 2] = red;           // red
            }
            else if (warstwa == 2)
            {
                uchar *pix = im.scanLine(y); // dostęp do piksela
                pix[4 * x] = blue;           // blue
                pix[4 * x + 1] = green;      // green
                pix[4 * x + 2] = red;
            }
        }
    }
    void drawLine(int x0, int y0, int x1, int y1)
    {
        double m = 0;
        if (x1 == x0 && y1 == y0)
        {
            drawPixel(x1, y1, 255, 0, 0, 1);
        }
        else
        {
            if (abs(x1 - x0) >= abs(y1 - y0))
            {
                if (x0 > x1)
                {
                    std::swap(y0, y1);
                    std::swap(x0, x1);
                }
                m = (double)(y1 - y0) / (x1 - x0);
                for (int x = x0; x <= x1; x++)
                {
                    int y = m * (x - x0) + y0;
                    drawPixel(x, (int)floor(y + 0.5), 255, 0, 0, 1);
                }
            }
            else
            {
                if (y0 > y1)
                {
                    std::swap(x0, x1);
                    std::swap(y0, y1);
                }
                m = (double)(x1 - x0) / (y1 - y0);
                for (int y = y0; y <= y1; y++)
                {
                    int x = m * (y - y0) + x0;
                    drawPixel((int)floor(x + 0.5), y, 255, 0, 0, 1);
                }
            }
        }
    }
    void drawLine_wynikowa(int x0, int y0, int x1, int y1)
    {
        double m = 0;
        if (x1 == x0 && y1 == y0)
        {
            drawPixel(x1, y1, 255, 0, 0,2);
        }
        else
        {
            if (abs(x1 - x0) >= abs(y1 - y0))
            {
                if (x0 > x1)
                {
                    std::swap(y0, y1);
                    std::swap(x0, x1);
                }
                m = (double)(y1 - y0) / (x1 - x0);
                for (int x = x0; x <= x1; x++)
                {
                    int y = m * (x - x0) + y0;
                    drawPixel(x, (int)floor(y + 0.5), 255, 0, 0,2);
                }
            }
            else
            {
                if (y0 > y1)
                {
                    std::swap(x0, x1);
                    std::swap(y0, y1);
                }
                m = (double)(x1 - x0) / (y1 - y0);
                for (int y = y0; y <= y1; y++)
                {
                    int x = m * (y - y0) + x0;
                    drawPixel((int)floor(x + 0.5), y, 255, 0, 0,2);
                }
            }
        }
    }
    void obrys(QVector<QPointF> &P)
    {
        for (int i = 0; i < P.size(); i++)
            drawLine(P[i].x(), P[i].y(), P[(i + 1) % P.size()].x(), P[(i + 1) % P.size()].y());

        for (int i = 0; i < P.size(); i++)
            for (int j = -2; j <= 2; j++)
                for (int k = -2; k <= 2; k++)
                    drawPixel(P[i].x() + j, P[i].y() + k, 255, 0, 0, 1);
    }
    void Points(int stopien)
    {
        wynik.clear();
        im.fill(0);

        for (int i = 0; i < 3; i++)
        {
            float x, y;
//            obliczanie punktów trójkąta pośredniego z morphingu
            x = (1.0f - stopien / 100.0f) * punkty[i].x() + (stopien / 100.0f) * punkty2[i].x();
            y = (1.0f - stopien / 100.0f) * punkty[i].y() + (stopien / 100.0f) * punkty2[i].y();
            wynik.push_back(QPoint(x, y));
        }
    }
    void morphing(int stopien)
    {
//         Znajdź najmniejszą i największą wartość y spośród punktów w wektorze wynik
        int y_min = wynik[0].y();
        int y_max = wynik[0].y();

//         znajdz y max i min dla punktów z wektora wynik
        for (int i = 1; i < wynik.size(); i++)
        {
            int y = wynik[i].y();
            y_min = std::min(y_min, y);
            y_max = std::max(y_max, y);
        }

//         Iteruj po wartościach y od y_min do y_max
        for (int y = y_min; y <= y_max; y++)
        {
//             Inicjalizuj najmniejszą i największą wartość x dla danego y
            int min_x = im.height() + 1;
            int max_x = 0;

//             Dla każdej pary kolejnych punktów z wektora wynik
            for (int i = 0; i < wynik.size(); i++)
            {
                int p1 = i;
//                 Dla każdej pary punktów 0,1 1,2 2,0
                int p2 = (i + 1) % wynik.size();

//                 Sprawdzenie, czy y przecina się z linią między punktami p1 i p2
                if ((wynik[p1].y() <= y && y < wynik[p2].y()) || (wynik[p2].y() <= y && y < wynik[p1].y()))
                {
//                     Obliczenie x na podstawie równania linii
                    int x = static_cast<int>(wynik[p1].x() + (y - wynik[p1].y()) * (wynik[p2].x() - wynik[p1].x()) / (wynik[p2].y() - wynik[p1].y()));

//                     Aktualizacja najmniejszej i największej wartości x
                    min_x = std::min(min_x, x);
                    max_x = std::max(max_x, x);
                }
            }

//             Jeśli min_x jest mniejsze niż max_x, kontynuuj
            if (min_x < max_x)
            {
//                 Obliczenie wartości alpha na podstawie stopnia
                double alpha = stopien / 100.0;

                QPointF& A = wynik[0];
                QPointF& B = wynik[1];
                QPointF& C = wynik[2];

//                 Obliczenie wyznacznika dla macierzy A, B, C
                double wyznacznik = (B.x() - A.x()) * (C.y() - A.y()) - (B.y() - A.y()) * (C.x() - A.x());

//                 Iteruj po wartościach x od min_x do max_x
                for (int x = min_x; x < max_x; x++)
                {
//                     Obliczenie wartości v i w na podstawie współrzędnych x i y
                    double v = ((x - A.x()) * (C.y() - A.y()) - (y - A.y()) * (C.x() - A.x())) / wyznacznik;
                    double w = ((B.x() - A.x()) * (y - A.y()) - (B.y() - A.y()) * (x - A.x())) / wyznacznik;
                    double u = 1 - v - w;

                    QPointF& P = punkty[0];
                    QPointF& Q = punkty[1];
                    QPointF& R = punkty[2];

//                     Obliczenie współrzędnych punktu wynikowego na podstawie wartości u, v i w
                    double punkty_x = u * P.x() + v * Q.x() + w * R.x();
                    double punkty_y = u * P.y() + v * Q.y() + w * R.y();

//                     Uzyskanie wskaźnika na piksel w obrazie obraz1 na podstawie obliczonych współrzędnych punktu
                    uchar* pix1 = obraz1.scanLine(static_cast<int>(punkty_y)) + 4 * static_cast<int>(punkty_x);

                    QPointF& Pprim = punkty2[0];
                    QPointF& Qprim = punkty2[1];
                    QPointF& Rprim = punkty2[2];

//                     Obliczenie współrzędnych punktu wynikowego w obrazie obraz2 na podstawie wartości u, v i w
                    double punkty2_x = u * Pprim.x() + v * Qprim.x() + w * Rprim.x();
                    double punkty2_y = u * Pprim.y() + v * Qprim.y() + w * Rprim.y();

//                     Uzyskanie wskaźnika na piksel w obrazie obraz2 na podstawie obliczonych współrzędnych punktu
                    uchar* pix2 = obraz2.scanLine(static_cast<int>(punkty2_y)) + 4 * static_cast<int>(punkty2_x);

//                     Obliczenie nowych wartości składowych RGB dla danego piksela na podstawie wartości alpha, pix1 i pix2
                    int red = static_cast<int>(alpha * pix2[2] + (1 - alpha) * pix1[2]);
                    int green = static_cast<int>(alpha * pix2[1] + (1 - alpha) * pix1[1]);
                    int blue = static_cast<int>(alpha * pix2[0] + (1 - alpha) * pix1[0]);

//                     Narysuj piksel o obliczonych wartościach RGB na obrazie wynikowym
                    drawPixel(x, y, red, green, blue, 2);
                }
            }
        }

//         Narysuj linie łączące punkty z wektora wynik na obrazie wynikowym
        for (int i = 0; i < 3; i++)
            drawLine_wynikowa(wynik[i].x(), wynik[i].y(), wynik[(i + 1) % 3].x(), wynik[(i + 1) % 3].y());
    }



protected:
    void paintEvent(QPaintEvent *event);
    void mouseMoveEvent(QMouseEvent *);
    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *);
signals:
public slots:
    void slot_czysc();
    void zmianaobrazka();
    void zmiana_suwak(int i);

private:
    QImage obraz;
    QImage obraz1;
    QImage obraz2;
    QImage im;
};

#endif // EKRAN_H
