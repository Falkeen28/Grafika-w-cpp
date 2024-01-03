#ifndef EKRAN_H
#define EKRAN_H

#include <QWidget>

class Ekran : public QWidget
{
    Q_OBJECT
public:
    explicit Ekran(QWidget *parent = nullptr);

    int wysokosc = 640;
    int szerokosc = 640;
    struct trzyD
    {
        int x;
        int y;
        int z;
    };
    QVector<trzyD> punkty;
    QVector<QPoint> punkty2D;
    int alphaX = 0, alphaY = 0, alphaZ = 0;
    double sX = 1, sY = 1, sZ = 1;
    double tX = 0, tY = 0, tZ = 0;
    bool czyWidoczna = false;
    bool czyTekstura = false;
    void czysc()
    {
        im.fill(0);
    }
    void drawPixel(int x, int y, int B, int G, int R)
    {
        int width = im.width();
        int height = im.height();
        if ((x >= 0) && (x < width) && (y >= 0) && (y < height))
        {
            uchar *pix = im.scanLine(y); // dostęp do piksela
            pix[4 * x] = B;              // blue
            pix[4 * x + 1] = G;          // green
            pix[4 * x + 2] = R;          // red
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
            if (std::abs(x1 - x0) >= std::abs(y1 - y0))
            {
                if (x0 > x1)
                {
                    std::swap(y0, y1);
                    std::swap(x0, x1);
                }
                m = static_cast<double>(y1 - y0) / (x1 - x0);
                for (int x = x0; x <= x1; x++)
                {
                    int y = static_cast<int>(m * (x - x0) + y0);
                    drawPixel(x, y, 255, 255, 255);
                }
            }
            else
            {
                if (y0 > y1)
                {
                    std::swap(x0, x1);
                    std::swap(y0, y1);
                }
                m = static_cast<double>(x1 - x0) / (y1 - y0);
                for (int y = y0; y <= y1; y++)
                {
                    int x = static_cast<int>(m * (y - y0) + x0);
                    drawPixel(x, y, 255, 255, 255);
                }
            }
        }
    }
    void szkielet(const QVector<QPoint> &punkty)
    {
        drawLine(punkty[0].x(), punkty[0].y(), punkty[1].x(), punkty[1].y());
        drawLine(punkty[0].x(), punkty[0].y(), punkty[2].x(), punkty[2].y());
        drawLine(punkty[0].x(), punkty[0].y(), punkty[4].x(), punkty[4].y());
        drawLine(punkty[1].x(), punkty[1].y(), punkty[3].x(), punkty[3].y());
        drawLine(punkty[1].x(), punkty[1].y(), punkty[5].x(), punkty[5].y());
        drawLine(punkty[2].x(), punkty[2].y(), punkty[3].x(), punkty[3].y());
        drawLine(punkty[2].x(), punkty[2].y(), punkty[6].x(), punkty[6].y());
        drawLine(punkty[3].x(), punkty[3].y(), punkty[7].x(), punkty[7].y());
        drawLine(punkty[5].x(), punkty[5].y(), punkty[4].x(), punkty[4].y());
        drawLine(punkty[5].x(), punkty[5].y(), punkty[7].x(), punkty[7].y());
        drawLine(punkty[4].x(), punkty[4].y(), punkty[6].x(), punkty[6].y());
        drawLine(punkty[6].x(), punkty[6].y(), punkty[7].x(), punkty[7].y());
    }
    bool isWallVisible(const trzyD &p0, const trzyD &p1, const trzyD &p2, const trzyD &p3)
    {
//        obliczamy środkowy punkt danej ściany
        float centerX = (p0.x + p1.x + p2.x + p3.x) / 4.0;
        float centerY = (p0.y + p1.y + p2.y + p3.y) / 4.0;
        float centerZ = (p0.z + p1.z + p2.z + p3.z) / 4.0;

//        obliczamy wektor z centralnego punktu
        double wektor[3] = {im.width() / 2.0 - centerX, (im.height() / 2 + static_cast<int>(tY)) - centerY, -110 - centerZ};

//         Oblicz iloczyny krzyżowe dwóch wektorów ściany
        float w1[3] = {static_cast<float>(p2.x - p0.x), static_cast<float>(p2.y - p0.y), static_cast<float>(p2.z - p0.z)};
        float w2[3] = {static_cast<float>(p1.x - p0.x), static_cast<float>(p1.y - p0.y), static_cast<float>(p1.z - p0.z)};
        float w3[3] = {w1[1] * w2[2] - w2[1] * w1[2], w2[0] * w1[2] - w1[0] * w2[2], w1[0] * w2[1] - w2[0] * w1[1]};

//         Oblicz iloczyn skalarny wektora punktu widzenia i iloczynu krzyżowego
        float wynik = wektor[0] * w3[0] + wektor[1] * w3[1] + wektor[2] * w3[2];

//        zwraca prawde kiedy ściana jest widoczna
        return wynik > 0;
    }
    void teksturowanie(QPoint p1, QPoint p2, QPoint p3)
    {
        QPoint org_p1 = {0, 0};
        QPoint org_p2 = {0, obraz.height() - 1};
        QPoint org_p3 = {obraz.width() - 1, obraz.height() - 1};
        QColor color;
        QVector<QPoint> points;
        points.append(p1);
        points.append(p2);
        points.append(p3);
        int najnizszy = points[0].y();
        int najwyzej = points[0].y();
        int najbardziejNaLewo = points[0].x();
        int najbardziejNaPrawo = points[0].x();

        for (int i = 1; i < points.size(); i++)
        {
            if (points[i].y() < najnizszy)
                najnizszy = points[i].y();
            if (points[i].y() > najwyzej)
                najwyzej = points[i].y();
            if (points[i].x() < najbardziejNaLewo)
                najbardziejNaLewo = points[i].x();
            if (points[i].x() > najbardziejNaPrawo)
                najbardziejNaPrawo = points[i].x();
        }

        double wz = (points[0].y() - points[1].y()) * (points[2].x() - points[1].x()) + (points[1].x() - points[0].x()) * (points[2].y() - points[1].y());
        for (int y = najnizszy; y < najwyzej; y++)
        {
            for (int x = najbardziejNaLewo; x < najbardziejNaPrawo; x++)
            {
//                 wspolrzednych barycentrycznych
                double w = ((points[1].y() - points[2].y()) * (x - points[2].x()) + (points[2].x() - points[1].x()) * (y - points[2].y())) / wz;
                double v = ((points[2].y() - points[0].y()) * (x - points[2].x()) + (points[0].x() - points[2].x()) * (y - points[2].y())) / wz;
                double u = 1.0 - w - v;

//                 sprawdzenie czy punkt lezy w trojkacie
                if (u >= 0 && u <= 1 && v >= 0 && v <= 1 && w >= 0 && w <= 1)
                {
                    int Punkt_x = u * org_p1.x() + v * org_p2.x() + w * org_p3.x();
                    int Punkt_y = u * org_p1.y() + v * org_p2.y() + w * org_p3.y();

                    if (Punkt_x >= 0 && Punkt_x < obraz.width() && Punkt_y >= 0 && Punkt_y < obraz.height())
                    {
                        color = obraz.pixelColor(Punkt_x, Punkt_y);
                        if (x >= 0 && x < width() && y >= 0 && y < height())
                            im.setPixelColor(x, y, color);
                    }
                }
            }
        }
    }
    void dwaDtrzyD(const QVector<trzyD> &pkt)
    {
        /*
            Ściany:
            Przednia - 0,2,4,6
            Lewa - 1,0,5,4
            Prawa - 2,3,6,7
            Tylnia - 3,1,7,5
            Gorna - 1,3,0,2
            Dolna - 4,6,5,7
        */
//        sprawdzanie widoczności kazdej ze ścian
        trzyD p0 = pkt[0];
        trzyD p1 = pkt[2];
        trzyD p2 = pkt[4];
        trzyD p3 = pkt[6];
        bool isVisiblePrzod = isWallVisible(p0, p1, p2, p3);

        p0 = pkt[3];
        p1 = pkt[1];
        p2 = pkt[7];
        p3 = pkt[5];
        bool isVisibleTyl = isWallVisible(p0, p1, p2, p3);

        p0 = pkt[1];
        p1 = pkt[0];
        p2 = pkt[5];
        p3 = pkt[4];
        bool isVisibleLewa = isWallVisible(p0, p1, p2, p3);

        p0 = pkt[2];
        p1 = pkt[3];
        p2 = pkt[6];
        p3 = pkt[7];
        bool isVisiblePrawa = isWallVisible(p0, p1, p2, p3);

        p0 = pkt[1];
        p1 = pkt[3];
        p2 = pkt[0];
        p3 = pkt[2];
        bool isVisibleGora = isWallVisible(p0, p1, p2, p3);

        p0 = pkt[4];
        p1 = pkt[6];
        p2 = pkt[5];
        p3 = pkt[7];
        bool isVisibleDol = isWallVisible(p0, p1, p2, p3);

        qDebug("\n\n\n");
        qDebug("Ściana Przednia: %s", isVisiblePrzod ? "Widać" : "Nie widać");
        qDebug("Ściana Tylnia: %s", isVisibleTyl ? "Widać" : "Nie widać");
        qDebug("Ściana Lewa: %s", isVisibleLewa ? "Widać" : "Nie widać");
        qDebug("Ściana Prawa: %s", isVisiblePrawa ? "Widać" : "Nie widać");
        qDebug("Ściana Górna: %s", isVisibleGora ? "Widać" : "Nie widać");
        qDebug("Ściana Dolna: %s", isVisibleDol ? "Widać" : "Nie widać");

        QVector<QPoint> punkty2D;
        im.fill(0);

        for (int i = 0; i < pkt.size(); i++)
        {
            double x = pkt[i].x - width() / 2.0;
            double y = pkt[i].y - height() / 2.0;
            double z = pkt[i].z;

            int f = 110;
            double projX = x / (z + f);
            double projY = y / (z + f);
            projX *= 110;
            projY *= 110;
            punkty2D.append(QPoint(projX + width() / 2.0, projY + height() / 2.0));
        }
        if (czyWidoczna)
        {
            if (isVisiblePrzod)
            {
                if (czyTekstura)
                {
//                    Przednia - 0,2,4,6
                    teksturowanie(punkty2D[0], punkty2D[2], punkty2D[6]);
                    teksturowanie(punkty2D[6], punkty2D[4], punkty2D[0]);
                }
                drawLine(punkty2D[4].x(), punkty2D[4].y(), punkty2D[6].x(), punkty2D[6].y());
                drawLine(punkty2D[2].x(), punkty2D[2].y(), punkty2D[6].x(), punkty2D[6].y());
                drawLine(punkty2D[0].x(), punkty2D[0].y(), punkty2D[2].x(), punkty2D[2].y());
                drawLine(punkty2D[0].x(), punkty2D[0].y(), punkty2D[4].x(), punkty2D[4].y());
            }
            if (isVisibleTyl)
            {
                if (czyTekstura)
                {
//                    Tylnia - 3,1,7,5
                    teksturowanie(punkty2D[1], punkty2D[3], punkty2D[7]);
                    teksturowanie(punkty2D[7], punkty2D[5], punkty2D[1]);
                }

                drawLine(punkty2D[1].x(), punkty2D[1].y(), punkty2D[3].x(), punkty2D[3].y());
                drawLine(punkty2D[5].x(), punkty2D[5].y(), punkty2D[1].x(), punkty2D[1].y());
                drawLine(punkty2D[5].x(), punkty2D[5].y(), punkty2D[7].x(), punkty2D[7].y());
                drawLine(punkty2D[3].x(), punkty2D[3].y(), punkty2D[7].x(), punkty2D[7].y());
            }
            if (isVisiblePrawa)
            {
                if (czyTekstura)
                {
//                    Prawa - 2,3,6,7
                    teksturowanie(punkty2D[7], punkty2D[6], punkty2D[2]);
                    teksturowanie(punkty2D[2], punkty2D[3], punkty2D[7]);
                }
                drawLine(punkty2D[3].x(), punkty2D[3].y(), punkty2D[7].x(), punkty2D[7].y());
                drawLine(punkty2D[2].x(), punkty2D[2].y(), punkty2D[3].x(), punkty2D[3].y());
                drawLine(punkty2D[2].x(), punkty2D[2].y(), punkty2D[6].x(), punkty2D[6].y());
                drawLine(punkty2D[6].x(), punkty2D[6].y(), punkty2D[7].x(), punkty2D[7].y());
            }
            if (isVisibleLewa)
            {
                if (czyTekstura)
                {
//                    Lewa - 1,0,5,4
                    teksturowanie(punkty2D[0], punkty2D[1], punkty2D[5]);
                    teksturowanie(punkty2D[5], punkty2D[4], punkty2D[0]);
                }
                drawLine(punkty2D[1].x(), punkty2D[1].y(), punkty2D[0].x(), punkty2D[0].y());
                drawLine(punkty2D[1].x(), punkty2D[1].y(), punkty2D[5].x(), punkty2D[5].y());
                drawLine(punkty2D[5].x(), punkty2D[5].y(), punkty2D[4].x(), punkty2D[4].y());
                drawLine(punkty2D[0].x(), punkty2D[0].y(), punkty2D[4].x(), punkty2D[4].y());
            }
            if (isVisibleGora)
            {
                if (czyTekstura)
                {
//                    Gorna - 1,3,0,2
                    teksturowanie(punkty2D[2], punkty2D[0], punkty2D[1]);
                    teksturowanie(punkty2D[1], punkty2D[3], punkty2D[2]);
                }
                drawLine(punkty2D[1].x(), punkty2D[1].y(), punkty2D[0].x(), punkty2D[0].y());
                drawLine(punkty2D[1].x(), punkty2D[1].y(), punkty2D[3].x(), punkty2D[3].y());
                drawLine(punkty2D[3].x(), punkty2D[3].y(), punkty2D[2].x(), punkty2D[2].y());
                drawLine(punkty2D[0].x(), punkty2D[0].y(), punkty2D[2].x(), punkty2D[2].y());
            }
            if (isVisibleDol)
            {
                if (czyTekstura)
                {
//                    Dolna - 4,6,5,7
                    teksturowanie(punkty2D[7], punkty2D[5], punkty2D[4]);
                    teksturowanie(punkty2D[4], punkty2D[6], punkty2D[7]);
                }
                drawLine(punkty2D[5].x(), punkty2D[5].y(), punkty2D[4].x(), punkty2D[4].y());
                drawLine(punkty2D[6].x(), punkty2D[6].y(), punkty2D[4].x(), punkty2D[4].y());
                drawLine(punkty2D[6].x(), punkty2D[6].y(), punkty2D[7].x(), punkty2D[7].y());
                drawLine(punkty2D[5].x(), punkty2D[5].y(), punkty2D[7].x(), punkty2D[7].y());
            }
        }
        else
        {
            szkielet(punkty2D);
        }
        update();
    }
    void liczMacierze()
    {
//         Przesuwamy obiekt na środki
        double srodekX = (im.width() / 2.0) + tX;
        double srodekY = (im.height() / 2.0) + tY;
        double srodekZ = tZ;

//         Translacja
//         Macierz translacji wzdłuż osi x, y i z
        double T[4][4] = {{1, 0, 0, tX},
                          {0, 1, 0, tY},
                          {0, 0, 1, tZ},
                          {0, 0, 0, 1}};
//         Wynik = T * {{x}{y}{z}{1}}

//         Skalowanie względem punktu (0,0,0)
//         Macierz skalowania
        double S[4][4] = {{sX, 0, 0, 0},
                          {0, sY, 0, 0},
                          {0, 0, sZ, 0},
                          {0, 0, 0, 1}};
//         Wynik S * {{x}{y}{z}{1}}

//         Rotacja odpowiednio dla X,Y,Z
//         Macierz rotacji wokół osi x
        double Rx[4][4] = {{1, 0, 0, 0},
                           {0, cos(alphaX * (M_PI / 180.0)), -sin(alphaX * (M_PI / 180.0)), 0},
                           {0, sin(alphaX * (M_PI / 180.0)), cos(alphaX * (M_PI / 180.0)), 0},
                           {0, 0, 0, 1}};
//         Macierz rotacji wokół osi y
        double Ry[4][4] = {{cos(alphaY * (M_PI / 180.0)), 0, sin(alphaY * (M_PI / 180.0)), 0},
                           {0, 1, 0, 0},
                           {-sin(alphaY * (M_PI / 180.0)), 0, cos(alphaY * (M_PI / 180.0)), 0},
                           {0, 0, 0, 1}};
//         Macierz rotacji wokół osi z
        double Rz[4][4] = {{cos(alphaZ * (M_PI / 180.0)), -sin(alphaZ * (M_PI / 180.0)), 0, 0},
                           {sin(alphaZ * (M_PI / 180.0)), cos(alphaZ * (M_PI / 180.0)), 0, 0},
                           {0, 0, 1, 0},
                           {0, 0, 0, 1}};

        QVector<trzyD> nowePunkty;
//         Tablica przechowująca przesunięte współrzędne punktu
        double punkt[4] = {0.0, 0.0, 0.0, 1.0};
//         Dwie tablice potrzebne do obliczeń
        int tab1[4];
        int tab2[4];

//         mnozenie tych macierzy w kolejnosci M = Rz * Ry * Rx * S * T
        for (int i = 0; i < 8; i++)
        {
            punkt[0] = punkty[i].x - srodekX;
            punkt[1] = punkty[i].y - srodekY;
            punkt[2] = punkty[i].z - srodekZ;

            for (int x = 0; x < 4; x++)
            {
//                 Przesunięcie
                tab1[x] = 0;
                for (int y = 0; y < 4; ++y)
                {
                    tab1[x] += punkt[y] * T[x][y];
                }
            }

            for (int x = 0; x < 4; x++)
            {
//                 Skalowanie
                tab2[x] = 0;
                for (int y = 0; y < 4; ++y)
                {
                    tab2[x] += tab1[y] * S[x][y];
                }
            }

            for (int x = 0; x < 4; x++)
            {
//                 Rotacja
                tab1[x] = 0;
                for (int y = 0; y < 4; ++y)
                {
                    tab1[x] += tab2[y] * Rx[x][y];
                }
            }

            for (int x = 0; x < 4; x++)
            {
//                 Rotacja
                tab2[x] = 0;
                for (int y = 0; y < 4; ++y)
                {
                    tab2[x] += tab1[y] * Ry[x][y];
                }
            }

            for (int x = 0; x < 4; x++)
            {
//                 Rotacja
                tab1[x] = 0;
                for (int y = 0; y < 4; ++y)
                {
                    tab1[x] += tab2[y] * Rz[x][y];
                }
            }
            nowePunkty.append({tab1[0] + (int)srodekX, tab1[1] + (int)srodekY, tab1[2] + (int)srodekZ});
        }
        dwaDtrzyD(nowePunkty);
    }

protected:
    void paintEvent(QPaintEvent *event);
    void resizeEvent(QResizeEvent *);

signals:

public slots:
    void setXTranslation(int value);
    void setYTranslation(int value);
    void setZTranslation(int value);

    void setXScaling(int value);
    void setYScaling(int value);
    void setZScaling(int value);

    void rotX(int value);
    void rotY(int value);
    void rotZ(int value);

    void reset();
    void zmianaWidocznosci();
    void teksturaTryb();

public slots:

private:
    QImage im, obraz;
};

#endif // EKRAN_H
