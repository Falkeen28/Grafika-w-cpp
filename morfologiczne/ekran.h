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
    int r = 1;
    void czysc();
    QVector<QPointF> punkty;
    QStack<QPointF> stack;
    QColor color;
    enum modelBarw
    {
        red,
        green,
        blue,
        hue,
        sat,
        val
    };
    enum RGB
    {
        RED,
        GREEN,
        BLUE
    };
    enum SWITCH
    {
        NORMAL,
        ALPHA,
        DARKEN,
        OVERLAY,
        SCREEN,
        MULTIPLY
    };
    enum WARSTWA
    {
        WARSTWA1,
        WARSTWA2,
        KOLOR,
        CZARNY_BIALY,
        WYNIKOWA,
    };
    void drawPixel(int x, int y, int B, int G, int R)
    {
        int width = im.width();
        int height = im.height();
        if ((x >= 0) && (x <= width) && (y >= 0) && (y <= height))
        {
            uchar *pix = im.scanLine(y); // dostęp do piksela?
            pix[4 * x] = B;     // blue
            pix[4 * x + 1] = G; // green
            pix[4 * x + 2] = R; // red
        }
    }
    void drawPixel_oryginal(int x, int y, int B, int G, int R)
    {
        int width = im.width();
        int height = im.height();
        if ((x >= 0) && (x <= width) && (y >= 0) && (y <= height))
        {
            uchar *pix = im_blackwhite_oryginal.scanLine(y); // dostęp do piksela
            pix[4 * x] = B;
            pix[4 * x + 1] = G;
            pix[4 * x + 2] = R;
        }
    }
    int getColor(int x, int y, int WARSTWA, int RGB)
    {
        int width = im.width();
        int height = im.height();
        if ((x >= 0) && (x <= width) && (y >= 0) && (y <= height))
        {
            if (WARSTWA == KOLOR)
            {
                uchar *pix = im_kolor.scanLine(y); // dostęp do piksela
                if (RGB == RED)
                {
                    return pix[4 * x + 2];
                }
                else if (RGB == GREEN)
                {
                    return pix[4 * x + 1];
                }
                else if (RGB == BLUE)
                {
                    return pix[4 * x];
                }
            }
            if (WARSTWA == CZARNY_BIALY)
            {
                uchar *pix = im_blackwhite_oryginal.scanLine(y); // dostęp do piksela
                if (RGB == RED)
                {
                    return pix[4 * x + 2];
                }
                else if (RGB == GREEN)
                {
                    return pix[4 * x + 1];
                }
                else if (RGB == BLUE)
                {
                    return pix[4 * x];
                }
            }
            if (WARSTWA == WYNIKOWA)
            {
                uchar *pix = im.scanLine(y); // dostęp do piksela
                if (RGB == RED)
                {
                    return pix[4 * x + 2];
                }
                else if (RGB == GREEN)
                {
                    return pix[4 * x + 1];
                }
                else if (RGB == BLUE)
                {
                    return pix[4 * x];
                }
            }
        }
        return 0;
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
                    if(H_prim >= 0 && H_prim <= 1)
                    {
                        // Przypisanie wartości składowej czerwonej
                        r = V;
                        // Przypisanie wartości składowej zielonej
                        g = X + V - C;
                        // Przypisanie wartości składowej niebieskiej
                        b = V - C;
                    }
                        // Odcień znajduje się w sektorze żółtym (60°-120° lub 1-2), gdzie dominuje żółć (yellow).
                    else if(H_prim >= 1 && H_prim <= 2)
                    {
                        // Przypisanie wartości składowej czerwonej
                        r = X + V - C;
                        // Przypisanie wartości składowej zielonej
                        g = V;
                        // Przypisanie wartości składowej niebieskiej
                        b = V - C;
                    }
                        // Odcień znajduje się w sektorze zielonym (120°-180° lub 2-3), gdzie dominuje zieleń (green).
                    else if(H_prim >= 2 && H_prim <= 3)
                    {
                        // Przypisanie wartości składowej czerwonej
                        r = V - C;
                        // Przypisanie wartości składowej zielonej
                        g = V;
                        // Przypisanie wartości składowej niebieskiej
                        b = X + V - C;
                    }
                        // Odcień znajduje się w sektorze cyjanowym (180°-240° lub 3-4), gdzie dominuje cyjan (cyan).
                    else if(H_prim >= 3 && H_prim <= 4)
                    {
                        // Przypisanie wartości składowej czerwonej
                        r = V - C;
                        // Przypisanie wartości składowej zielonej
                        g = X + V - C;
                        // Przypisanie wartości składowej niebieskiej
                        b = V;
                    }
                        // Odcień znajduje się w sektorze niebieskim (240°-300° lub 4-5), gdzie dominuje niebieski (blue).
                    else if(H_prim >= 4 && H_prim <= 5)
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
    void blend()
    {

    }
    void color_to_blackwhite()
    {
       for (int y = 0; y < im.height(); y++)
        {
            for (int x = 0; x < im.width(); x++)
            {
                int red = getColor(x, y, KOLOR, RED);
                int green = getColor(x, y, KOLOR, GREEN);
                int blue = getColor(x, y, KOLOR, BLUE);
                int value = (red + green + blue) / 3;

                if (value > 127)
                    value = 255;
                else
                    value = 0;

                drawPixel(x, y, value, value, value);
                drawPixel_oryginal(x, y, value, value, value);
            }
        }
    }
    void morfologia(int r, bool co_to)
    {
        for (int y = 0; y < im.height(); y++)
        {
            for (int x = 0; x < im.width(); x++)
            {
                int wartosc = getColor(x, y, CZARNY_BIALY, BLUE);

                if (wartosc == 0 && co_to == true)
                {
                    for (int i = -r; i <= r; i++)
                    {
                        for (int j = -r; j <= r; j++)
                        {
                            drawPixel(x + i, y + j, 0, 0, 0);
                        }
                    }
                }
                else if(wartosc == 255 && co_to == false)
                {
                    for (int i = -r; i <= r; i++)
                    {
                        for (int j = -r; j <= r; j++)
                        {
                            drawPixel(x + i, y + j, 255, 255, 255);
                        }
                    }
                }
            }
        }
    }
    void copy_warstwa()
    {
        for (int y = 0; y < im.height(); y++)
        {
            for (int x = 0; x < im.width(); x++)
            {
                int wartosc = getColor(x, y, WYNIKOWA, BLUE);
                drawPixel_oryginal(x, y, wartosc, wartosc, wartosc);
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
    void promien(int i);
    void dylatacja_przycisk();
    void erozja_przycisk();
    void domkniecie_przycisk();
    void otwarcie_przycisk();
    void blackTOwhite();
    void reset();
public slots:

private:
    QImage im;
    QImage im_blackwhite_oryginal;
    QImage im_kolor;
};

#endif // EKRAN_H
