#ifndef EKRAN_H
#define EKRAN_H

#include <QWidget>
#include <QPainter>

class Ekran : public QWidget
{
    Q_OBJECT
public:
    explicit Ekran(QWidget *parent = nullptr);
    int co_rysowac = 0;
    /*
     * flaga co_rysować
     * gdy 0 dodawanie
     * gdy 1 przesuwanie
     */
    bool narysowane = false;
    bool interpolacja_status = true;
    int index_temp = -1;
    int index_temp2 = -1;
    void czysc();
    QColor color;
    QVector<QPointF> punkty;
    QVector<QPointF> punkty2;
    int co_blizej(QVector<QPointF> &punkty, int x, int y)
    {
        int index = -1;
        float odl = 20;
        for (int i = 0; i < punkty.size(); i++)
        {
            if (sqrt((punkty[i].x() - x) * (punkty[i].x() - x) + (punkty[i].y() - y) * (punkty[i].y() - y)) < odl)
            {
                odl = sqrt((punkty[i].x() - x) * (punkty[i].x() - x) + (punkty[i].y() - y) * (punkty[i].y() - y));
                index = i;
            }
        }
        return index;
    }
    void drawPixel(int x, int y, int B, int G, int R)
    {
        int width = im.width();
        int height = im.height();
        if ((x >= 0) && (x <= width) && (y >= 0) && (y <= height))
        {
            if ((x >= 0) && (x <= 450) && (y >= 0) && (y <= height))
            {
                uchar *pix = im.scanLine(y); // dostęp do piksela
                pix[4 * x] = B;              // blue
                pix[4 * x + 1] = G;          // green
                pix[4 * x + 2] = R;          // red
            }
            else
            {
                uchar *pix2 = im2.scanLine(y); // dostęp do piksela
                pix2[4 * x] = B;               // blue
                pix2[4 * x + 1] = G;           // green
                pix2[4 * x + 2] = R;           // red
            }
        }
    }
    void drawLine(int x0, int y0, int x1, int y1)
    {
        double m = 0;
        if (x1 == x0 && y1 == y0)
        {
            drawPixel(x1, y1, 255, 255, 255);
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
                    drawPixel(x, (int)floor(y + 0.5), 255, 255, 255);
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
                    drawPixel((int)floor(x + 0.5), y, 255, 255, 255);
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
                    drawPixel(P[i].x() + j, P[i].y() + k, 0, 0, 255);
    }
    void teksturowanie()
    {
        // Znalezienie najniższego i najwyższego punktu trojkąta
        int najnizszy = punkty[0].y();
        int najwyzej = punkty[0].y();
        for (int i = 1; i < punkty.size(); i++)
        {
            int y = punkty[i].y();
            if (y < najnizszy)
                najnizszy = y;
            if (y > najwyzej)
                najwyzej = y;
        }

        // Znalezienie najbardziej na lewo i prawo punktu trojkata
        int najbardziejNaLewo = punkty[0].x();
        int najbardziejNaPrawo = punkty[0].x();
        for (int i = 1; i < punkty.size(); i++)
        {
            int x = punkty[i].x();
            if (x < najbardziejNaLewo)
                najbardziejNaLewo = x;
            if (x > najbardziejNaPrawo)
                najbardziejNaPrawo = x;
        }

        // Wyznaczenie wz (wyznacznik) na podstawie punktow w trojkacie
        double wz = (punkty[0].y() - punkty[1].y()) * (punkty[2].x() - punkty[1].x()) + (punkty[1].x() - punkty[0].x()) * (punkty[2].y() - punkty[1].y());

        // Pętla po pikselach wewnątrz ograniczonego obszaru
        for (int y = najnizszy; y < najwyzej; y++)
        {
            for (int x = najbardziejNaLewo; x < najbardziejNaPrawo; x++)
            {
                // Wyznaczenie współrzędnych barycentrycznych
                double w = ((punkty[1].y() - punkty[2].y()) * (x - punkty[2].x()) + (punkty[2].x() - punkty[1].x()) * (y - punkty[2].y())) / wz;
                double v = ((punkty[2].y() - punkty[0].y()) * (x - punkty[2].x()) + (punkty[0].x() - punkty[2].x()) * (y - punkty[2].y())) / wz;
                double u = 1.0 - w - v;

                // Sprawdzenie czy punkt leży w trojkącie
                if (u >= 0 && u < 1 && v >= 0 && v < 1 && w >= 0 && w < 1)
                {
                    // Obliczenie współrzędnych punktu p prim
                    double Ppx = u * punkty2[2].x() + v * punkty2[1].x() + w * punkty2[0].x();
                    double Ppy = u * punkty2[2].y() + v * punkty2[1].y() + w * punkty2[0].y();

                    // Pobranie koloru z im2
                    if (interpolacja_status == true)
                        color = InterpolacjaDwuliniowa(&im2, Ppx - 450, Ppy);
                    else
                        color = im2.pixelColor(Ppx - 450, Ppy);

                    // Wstawienie koloru do im
                    drawPixel(x, y, color.blue(), color.green(), color.red());
                    update();
                }
            }
        }
    }
    QColor InterpolacjaDwuliniowa(QImage* im, double x, double y)
    {
        if (x >= 0 && x <= 450 && y >= 0 && y <= 600)
        {
            int x0 = static_cast<int>(x);
            int x1 = x0 + 1;
            int y0 = static_cast<int>(y);
            int y1 = y0 + 1;

            double dx = x - x0;
            double dy = y - y0;

            const uchar* bits = im->constBits();

            int red = static_cast<int>((1 - dx) * (1 - dy) * bits[4 * (x0 + 450 * y0) + 2]
                      + dx * (1 - dy) * bits[4 * (x1 + 450 * y0) + 2]
                      + (1 - dx) * dy * bits[4 * (x0 + 450 * y1) + 2]
                      + dx * dy * bits[4 * (x1 + 450 * y1) + 2]);

            int green = static_cast<int>((1 - dx) * (1 - dy) * bits[4 * (x0 + 450 * y0) + 1]
                        + dx * (1 - dy) * bits[4 * (x1 + 450 * y0) + 1]
                        + (1 - dx) * dy * bits[4 * (x0 + 450 * y1) + 1]
                        + dx * dy * bits[4 * (x1 + 450 * y1) + 1]);

            int blue = static_cast<int>((1 - dx) * (1 - dy) * bits[4 * (x0 + 450 * y0) + 0]
                       + dx * (1 - dy) * bits[4 * (x1 + 450 * y0) + 0]
                       + (1 - dx) * dy * bits[4 * (x0 + 450 * y1) + 0]
                       + dx * dy * bits[4 * (x1 + 450 * y1) + 0]);

            return QColor(red, green, blue);
        }
        else
        {
            return QColor(0, 0, 0); // kolor tła
        }
    }

protected:
    void paintEvent(QPaintEvent *event);
    void mouseMoveEvent(QMouseEvent *);
    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
signals:
public slots:
    void slot_czysc();
    void co_rysuje0();
    void co_rysuje1();
    void co_rysuje2();
    void interpolacja_on();

private:
    QImage im, im2, imG, temp;
};

#endif // EKRAN_H
