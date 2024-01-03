#ifndef EKRAN_H
#define EKRAN_H

#include <QWidget>
#include <QPainter>

class Ekran : public QWidget
{
    Q_OBJECT
public:
    explicit Ekran(QWidget *parent = nullptr);
    /*
     * flaga co_rysować
     * gdy 0 dodawanie
     * gdy 1 przesuwanie
     */
    bool narysowane = false;
    bool bezir = false;
    int index_temp = -1;
    int suwaczek = 10;
    int startX = 0;
    int startY = 0;
    int finishX = 0;
    int finishY = 0;
    int tempx = -1, tempy = -1;
    bool isPressedL = false;
    bool isPressedR = false;
    void czysc();
    QVector<QPointF> punkty;
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
            uchar *pix = im.scanLine(y); // dostęp do piksela?
            //            pix[4*x]=255; // blue
            //            pix[4*x+1]=192; //green
            //            pix[4*x+2]=203; //red

            pix[4 * x] = B;     // blue
            pix[4 * x + 1] = G; // green
            pix[4 * x + 2] = R; // red
        }
    }
    void drawLine(int x0, int y0, int x1, int y1)
    {
        double m = 0;
        if (x1 == x0 && y1 == y0)
        {
            drawPixel(x1, y1, 255, 192, 203);
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
                    drawPixel(x, (int)floor(y + 0.5), 255, 192, 203);
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
                    drawPixel((int)floor(x + 0.5), y, 255, 192, 203);
                }
            }
        }
    }
    void scanline(QVector<QPointF> &P)
    {
        QVector <int> pktX;
        double tempx;
        int y1,y2,x1,x2;
        int yMAX = P[0].y(), yMIN = P[0].y();
        for(int i = 1; i < P.size(); i++)
        {
            if(P[i].y() > yMAX) yMAX = P[i].y();
            if(P[i].y() < yMIN) yMIN = P[i].y();
        }
        for(int y = yMIN; y <= yMAX; y++)
        {
            for(int i = 0; i < P.size(); i++)
            {
                y1 = P[i].y();
                y2 = P[(i+1)%P.size()].y();
                if(y1 > y2)
                {
                    std::swap(y1,y2);
                    x1 = P[(i+1)%P.size()].x();
                    x2 = P[i].x();
                }
                else
                {
                    x1 = P[i].x();
                    x2 = P[(i+1)%P.size()].x();
                }
                if(y >= y1 && y < y2 && y1 != y2)
                {
                    tempx = x1 + (double)(y - y1)*(x2 - x1)/(y2 - y1);
                    pktX.push_back((int)floor(tempx + 0.5));
                }
            }
            std::sort(pktX.begin(), pktX.end());
            for (int i = 0; i < pktX.size()-1; i+=2)
                drawLine(pktX[i], y, pktX[i+1], y);
            pktX.clear();
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
    void Suwak(int);
    void co_rysuje0();
    void co_rysuje1();
    void co_rysuje2();
    void bezeir1();
    void bezeir2();

private:
    QImage im;
    QImage im2;
};

#endif // EKRAN_H
