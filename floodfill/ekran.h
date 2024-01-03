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
    int co_rysowac = 0;
    /*
     * flaga co_rysować
     * gdy 0 to linia
     * gdy 1 to kolo
     * gdy 2 to elipsa
     * gdy 3 to dodawanie
     * gdy 4 to przesuwanie
     * fdy 5 to floodfill
     */
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
    QStack<QPointF> stack;
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
    void drawCircle(int x0, int y0, int x1, int y1)
    {
        int R = sqrt(((x1 - x0) * (x1 - x0)) + ((y1 - y0) * (y1 - y0)));
        for (int x = 0; x <= (R / sqrt(2)); x++)
        {
            double y = (double)sqrt(R * R - x * x);
            drawPixel(x + x0, (int)floor(y + 0.5) + y0, 255, 192, 203);
            drawPixel(x + x0, -(int)floor(y + 0.5) + y0, 255, 192, 203);
            drawPixel(-x + x0, (int)floor(y + 0.5) + y0, 255, 192, 203);
            drawPixel(-x + x0, -(int)floor(y + 0.5) + y0, 255, 192, 203);
            // po odwróceniu wspołrzędnych
            drawPixel((int)floor(y + 0.5) + x0, x + y0, 255, 192, 203);
            drawPixel(-(int)floor(y + 0.5) + x0, x + y0, 255, 192, 203);
            drawPixel((int)floor(y + 0.5) + x0, -x + y0, 255, 192, 203);
            drawPixel(-(int)floor(y + 0.5) + x0, -x + y0, 255, 192, 203);
        }
        update();
    }
    void drawElipse(int x0, int y0, int x1, int y1, int suwaczek)
    {
        // suwaczek = ilosc linii w elipsie
        int e = abs(x1 - x0);
        int f = abs(y1 - y0);
        int py, px;
        double kat;
        int pxTemp = e;
        int pyTemp = 0;
        // Rysowanie linii między kolejnymi wierzchołkami
        for (int i = 1; i <= suwaczek; ++i)
        {
            // obliczenie kąta miedzy punktami
            kat = i * 2 * M_PI / suwaczek;
            px = e * cos(kat); // ruszanie pierwszej przekatnej wzgledem x
            py = f * sin(kat); // ruszanie drugiej przekatnej wzgledem y
            drawLine(px + x0, py + y0, pxTemp + x0, pyTemp + y0);
            pxTemp = px;
            pyTemp = py;
        }
    }
    void drawBezier(QVector<QPointF> &punkty, int suwaczek)
    {
        QPointF temp1, temp2;
        for (int j = 0; j < punkty.size() - 3; j += 3)
        {
            for (int i = 0; i < suwaczek; i++)
            {
                float tx = (float)i / (float)suwaczek;
                float ty = (float)i / (float)suwaczek + 1/(float)suwaczek;

                temp1 = pow(1-tx,3)*punkty[0+j] + 3*pow(1-tx,2)*tx*punkty[1+j] + 3*(1-tx)*pow(tx,2)*punkty[2+j] + pow(tx,3)*punkty[3+j];
                temp2 = pow(1-ty,3)*punkty[0+j] + 3*pow(1-ty,2)*ty*punkty[1+j] + 3*(1-ty)*pow(ty,2)*punkty[2+j] + pow(ty,3)*punkty[3+j];

                drawLine(temp1.x(), temp1.y(), temp2.x(), temp2.y());
                temp2 = temp1;
            }
            for(int i = 0; i < punkty.size(); i++)
            {
                drawPixel(punkty[i].x(), punkty[i].y(), 0,0,255);
            }
        }
    }
    void drawBezierSpline(QVector<QPointF> &punkty, int suwaczek)
    {
        QPointF temp1, temp2;
        for (int j = 0; j < punkty.size() - 3; j++)
        {
            for (int i = 0; i < suwaczek; i++)
            {
                float tx = (float)i / (float)suwaczek;
                float ty = (float)i / (float)suwaczek + 1/(float)suwaczek;

                temp1 = ((float)1/6)*(-pow((tx),3) + 3*pow(tx,2) - 3*tx + 1)*punkty[0+j] + ((float)1/6)*(3*pow(tx,3) - 6 * pow(tx,2) + 4)*punkty[1+j]
                        + ((float)1/6)*(-3*pow(tx,3) + 3*pow(tx,2) + 3*tx + 1)*punkty[2+j] + ((float)1/6)*pow(tx,3)*punkty[3+j];

                temp2 = ((float)1/6)*(-pow((ty),3) + 3*pow(ty,2) - 3*ty + 1)*punkty[0+j] + ((float)1/6)*(3*pow(ty,3) - 6 * pow(ty,2) + 4)*punkty[1+j]
                        + ((float)1/6)*(-3*pow(ty,3) + 3*pow(ty,2) + 3*ty + 1)*punkty[2+j] + ((float)1/6)*pow(ty,3)*punkty[3+j];

                drawLine(temp1.x(), temp1.y(), temp2.x(), temp2.y());
                temp2 = temp1;
            }
            for(int i = 0; i < punkty.size(); i++)
            {
                drawPixel(punkty[i].x(), punkty[i].y(), 0,0,255);
            }
        }
    }
    void flood_fill (int x0, int y0, QRgb rgb_wybrane)
    {
        int x,y;
        QColor Kolor = QColorDialog::getColor(Qt::white, this, "Wybierz kolor");
        if(!Kolor.isValid()) return;

        QRgb RGB = Kolor.rgb();
        if(RGB == rgb_wybrane) return;

        stack.push(QPointF(x0,y0));
        while (!stack.isEmpty())
        {
            x = stack.top().x();
            y = stack.top().y();
            stack.pop();
            if(im.pixel(x,y) == rgb_wybrane)
            {
                drawPixel(x, y, qBlue(RGB), qGreen(RGB), qRed(RGB));
                if (y>=0 && y<im.height())
                {
                    if (x+1 >= 0 && x+1 < im.width())
                        stack.push(QPointF(x+1,y));

                    if (x-1 >= 0 && x-1 < im.width())
                        stack.push(QPointF(x-1,y));
                }
                if (x>=0 && x<im.width())
                {
                    if (y+1 >= 0 && y+1 < im.height())
                        stack.push(QPointF(x,y+1));

                    if (y-1 >= 0 && y-1 < im.height())
                        stack.push(QPointF(x,y-1));
                }
            }
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
    void co_rysuje3();
    void co_rysuje4();
    void co_rysuje5();
    void bezeir1();
    void bezeir2();

private:
    QImage im,im2,im3,im4;
};

#endif // EKRAN_H
