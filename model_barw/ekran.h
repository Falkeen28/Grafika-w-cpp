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
    /*
     * flaga co_rysować
     * gówno
     */
    int co_rysowac = 0;
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
    enum modelBarw
    {
        red,
        green,
        blue,
        hue,
        sat,
        val
    };
    QVector<QPointF> punkty;
    QStack<QPointF> stack;
    QColor color;
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
            uchar *pix = im.scanLine(y);
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
                float ty = (float)i / (float)suwaczek + 1 / (float)suwaczek;

                temp1 = pow(1 - tx, 3) * punkty[0 + j] + 3 * pow(1 - tx, 2) * tx * punkty[1 + j] + 3 * (1 - tx) * pow(tx, 2) * punkty[2 + j] + pow(tx, 3) * punkty[3 + j];
                temp2 = pow(1 - ty, 3) * punkty[0 + j] + 3 * pow(1 - ty, 2) * ty * punkty[1 + j] + 3 * (1 - ty) * pow(ty, 2) * punkty[2 + j] + pow(ty, 3) * punkty[3 + j];

                drawLine(temp1.x(), temp1.y(), temp2.x(), temp2.y());
                temp2 = temp1;
            }
            for (int i = 0; i < punkty.size(); i++)
            {
                drawPixel(punkty[i].x(), punkty[i].y(), 0, 0, 255);
            }
        }
    }
    void drawBezierSpline(QVector<QPointF> &punkty, int suwaczek)
    {
        QPointF temp1, temp2;
        for (int j = 0; j < punkty.size() - 3; j++)
        {
            for (int i = 0; i <= suwaczek; i++)
            {
                float tx = (float)i / (float)suwaczek;
                float ty = (float)i / (float)suwaczek + 1 / (float)suwaczek;

                temp1 = ((float)1 / 6) * (-pow((tx), 3) + 3 * pow(tx, 2) - 3 * tx + 1) * punkty[0 + j] + ((float)1 / 6) * (3 * pow(tx, 3) - 6 * pow(tx, 2) + 4) * punkty[1 + j] + ((float)1 / 6) * (-3 * pow(tx, 3) + 3 * pow(tx, 2) + 3 * tx + 1) * punkty[2 + j] + ((float)1 / 6) * pow(tx, 3) * punkty[3 + j];

                temp2 = ((float)1 / 6) * (-pow((ty), 3) + 3 * pow(ty, 2) - 3 * ty + 1) * punkty[0 + j] + ((float)1 / 6) * (3 * pow(ty, 3) - 6 * pow(ty, 2) + 4) * punkty[1 + j] + ((float)1 / 6) * (-3 * pow(ty, 3) + 3 * pow(ty, 2) + 3 * ty + 1) * punkty[2 + j] + ((float)1 / 6) * pow(ty, 3) * punkty[3 + j];

                drawLine(temp1.x(), temp1.y(), temp2.x(), temp2.y());
                temp2 = temp1;
            }
            for (int i = 0; i < punkty.size(); i++)
            {
                drawPixel(punkty[i].x(), punkty[i].y(), 0, 0, 255);
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
    void scanline(QVector<QPointF> &P)
    {
        QVector<int> pktX;
        double tempx;
        int y1, y2, x1, x2;
        int yMAX = P[0].y(), yMIN = P[0].y();
        if (P.size() < 2)
            return;
        for (int i = 1; i < P.size(); i++)
        {
            if (P[i].y() > yMAX)
                yMAX = P[i].y();
            if (P[i].y() < yMIN)
                yMIN = P[i].y();
        }
        for (int y = yMIN; y <= yMAX; y++)
        {
            for (int i = 0; i < P.size(); i++)
            {
                y1 = P[i].y();
                y2 = P[(i + 1) % P.size()].y();
                if (y1 > y2)
                {
                    std::swap(y1, y2);
                    x1 = P[(i + 1) % P.size()].x();
                    x2 = P[i].x();
                }
                else
                {
                    x1 = P[i].x();
                    x2 = P[(i + 1) % P.size()].x();
                }
                if (y >= y1 && y < y2 && y1 != y2)
                {
                    tempx = x1 + (double)(y - y1) * (x2 - x1) / (y2 - y1);
                    pktX.push_back((int)floor(tempx + 0.5));
                }
            }
            std::sort(pktX.begin(), pktX.end());
            for (int i = 0; i < pktX.size() - 1; i += 2)
                drawLine(pktX[i], y, pktX[i + 1], y);
            pktX.clear();
        }
    }
    void barwy(int n, int tryb)
    {
        float S,V,C,X,H_prim;
        float r, g, b;
        int tempx, tempy;
        if (tryb == red)
        {
            for (int y = 0; y < im.height(); y++)
            {
                //green
                tempy = (y * 255) / (im.height());
                for (int x = 0; x < im.width(); x++)
                {
                    //blue
                    tempx = (x * 255) / (im.width());
                    drawPixel(x,y, tempx, tempy, n);
                }
            }
        }
        else if (tryb == green)
        {
            for (int y = 0; y < im.height(); y++)
            {
                //red
                tempy = (y * 255) / (im.height());
                for (int x = 0; x < im.width(); x++)
                {
                    //blue
                    tempx = (x * 255) / (im.width());
                    drawPixel(x,y, tempx, n, tempy);
                }
            }
        }
        else if (tryb == blue)
        {
            for (int y = 0; y < im.height(); y++)
            {
                //red
                tempy = (y * 255) / (im.height());
                for (int x = 0; x < im.width(); x++)
                {
                    //green
                    tempx = (x * 255) / (im.width());
                    drawPixel(x,y, n, tempx, tempy);
                }
            }
        }
        else if (tryb == hue || tryb == sat || tryb == val)
        {
            for (int y = 0; y < im.height(); y++)
            {
                for (int x = 0; x < im.width(); x++)
                {
                    if(tryb == hue)
                    {
                        // Obliczanie nasycenia na podstawie pozycji x
                        S = (float)x / (im.width() - 1);
                        // Obliczanie jasności na podstawie pozycji y
                        V = (float)y / (im.height() - 1);
                        // Obliczanie nasycenia odcienia
                        C = S * V;
                        // Przeliczenie odcienia na zakres [0,6)
                        H_prim = n / 60.0f;
                        // Obliczanie wartości pomocniczej X
                        X = C * (1 - fabs(fmod(H_prim, 2.0f) - 1));
                    }
                    if(tryb == sat)
                    {
                        // Przeskalowanie nasycenia z przedziału [0, 255] na [0, 1]
                        S = (float)n / 255;
                        // Obliczanie jasności na podstawie pozycji y
                        V = (float)y / (im.height() - 1);
                        // Obliczanie nasycenia odcienia
                        C = S * V;
                        // Przeliczenie odcienia na zakres [0,6)
                        H_prim = ((float)x / (im.width() - 1)) * 6;
                        // Obliczanie wartości pomocniczej X
                        X = C * (1 - fabs(fmod(H_prim, 2.0f) - 1));
                    }
                    if (tryb == val)
                    {
                        // Obliczanie nasycenia na podstawie pozycji y
                        S = (float)y / (im.height() - 1);
                        // Przeskalowanie jasności z przedziału [0, 255] na [0, 1]
                        V = (float)n / 255;
                        // Obliczanie nasycenia odcienia
                        C = S * V;
                        // Przeliczenie odcienia na zakres [0,6)
                        H_prim = ((float)x / (im.width() - 1)) * 6;
                        // Obliczanie wartości pomocniczej X
                        X = C * (1 - fabs(fmod(H_prim, 2.0f) - 1));
                    }
                        // Odcień znajduje się w sektorze czerwonym (0°-60° lub 0-1), gdzie dominuje czerwień (red).
                    if(H_prim > 0 && H_prim <= 1)
                    {
                        // Przypisanie wartości składowej czerwonej
                        r = V;
                        // Przypisanie wartości składowej zielonej
                        g = X + V - C;
                        // Przypisanie wartości składowej niebieskiej
                        b = V - C;
                    }
                        // Odcień znajduje się w sektorze żółtym (60°-120° lub 1-2), gdzie dominuje żółć (yellow).
                    else if(H_prim > 1 && H_prim <= 2)
                    {
                        // Przypisanie wartości składowej czerwonej
                        r = X + V - C;
                        // Przypisanie wartości składowej zielonej
                        g = V;
                        // Przypisanie wartości składowej niebieskiej
                        b = V - C;
                    }
                        // Odcień znajduje się w sektorze zielonym (120°-180° lub 2-3), gdzie dominuje zieleń (green).
                    else if(H_prim > 2 && H_prim <= 3)
                    {
                        // Przypisanie wartości składowej czerwonej
                        r = V - C;
                        // Przypisanie wartości składowej zielonej
                        g = V;
                        // Przypisanie wartości składowej niebieskiej
                        b = X + V - C;
                    }
                        // Odcień znajduje się w sektorze cyjanowym (180°-240° lub 3-4), gdzie dominuje cyjan (cyan).
                    else if(H_prim > 3 && H_prim <= 4)
                    {
                        // Przypisanie wartości składowej czerwonej
                        r = V - C;
                        // Przypisanie wartości składowej zielonej
                        g = X + V - C;
                        // Przypisanie wartości składowej niebieskiej
                        b = V;
                    }
                        // Odcień znajduje się w sektorze niebieskim (240°-300° lub 4-5), gdzie dominuje niebieski (blue).
                    else if(H_prim > 4 && H_prim <= 5)
                    {
                        // Przypisanie wartości składowej czerwonej
                        r = X + V - C;
                        // Przypisanie wartości składowej zielonej
                        g = V - C;
                        // Przypisanie wartości składowej niebieskiej
                        b = V;
                    }
                        // Odcień znajduje się w sektorze magenty (300°-360° lub 5-6), gdzie dominuje magenta (magenta).
                    else
                    {
                        // Przypisanie wartości składowej czerwonej
                        r = V;
                        // Przypisanie wartości składowej zielonej
                        g = V - C;
                        // Przypisanie wartości składowej niebieskiej
                        b = X + V - C;
                    }
                    // Rysowanie piksela na obrazie
                    drawPixel(x, y, r * 255, g * 255, b * 255);
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
void bezeir1();
void bezeir2();
void red_wartosc(int);
void green_wartosc(int);
void blue_wartosc(int);
void hue_wartosc(int);
void saturation_wartosc(int);
void value_wartosc(int);

private:
QImage im;
QImage im2;
};

#endif // EKRAN_H
