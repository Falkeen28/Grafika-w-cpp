#include "ekran.h"
#include <QPainter>
#include <QMouseEvent>

Ekran::Ekran(QWidget *parent):QWidget{parent}
{
    im = QImage(600, 900, QImage::Format_RGB32);
    obraz = QImage(600, 900, QImage::Format_RGB32);
    obraz1 = QImage(600, 900, QImage::Format_RGB32);
    obraz2 = QImage(600, 900, QImage::Format_RGB32);

    im.fill(0);
    obraz.load("/Users/truemonster/Desktop/PGK/Teksturowanie_morfing/kotek.jpg");
    obraz1.load("/Users/truemonster/Desktop/PGK/Teksturowanie_morfing/kotek.jpg");
    obraz2.load("/Users/truemonster/Desktop/PGK/Teksturowanie_morfing/piesek.jpg");

    update();
}
void Ekran::paintEvent(QPaintEvent *)
{
    QPainter p(this);
    p.drawImage(0, 0, obraz);
    p.drawImage(450, 0, im);
}
void Ekran::slot_czysc()
{
    czysc();
    update();
}
void Ekran::czysc()
{
    im.fill(0);
    obraz.load("/Users/truemonster/Desktop/PGK/teksturowanie/kotek.jpg");
    punkty.clear();
    punkty2.clear();
    wynik.clear();
    obrazekSTATUS = false;
    stopien = 0;
}
void Ekran::zmianaobrazka()
{
    if(obrazekSTATUS)
    {
        qDebug("Obrazek1");
        obrazekSTATUS = false;
        obraz = obraz1;
    }
    else
    {
        qDebug("Obrazek2");
        obrazekSTATUS = true;
        obraz = obraz2;
    }
    update();
}
void Ekran::zmiana_suwak(int n)
{
    stopien = n;
    qDebug("%d", stopien);
    if (punkty.size() == 3 && punkty2.size() == 3)
    {
        Points(stopien);
        morphing(stopien);
    }
    update();
}
void Ekran::mousePressEvent(QMouseEvent *event)
{
    if (event->buttons() & Qt::LeftButton)
    {
        // Pobieramy wspolrzedne punktu klikniecia
        // Sa to wspolrzedne wzgledem glownego okna
        int x = event->pos().x();
        int y = event->pos().y();
        if (x < 450)
        {
            if(!obrazekSTATUS)
            {
                if(punkty.size() < 3)
                {
                    qDebug("x %d y %d", x, y);
                    punkty.push_back(QPointF(x, y));
                }
                if(punkty.size() == 3)
                {
                    obrys(punkty);
                }
            }
            else
            {
                if(punkty2.size() < 3)
                {
                    qDebug("x %d y %d", x, y);
                    punkty2.push_back(QPointF(x, y));
                }
                if(punkty2.size() == 3)
                {
                    obrys(punkty2);
                }
            }
            if (punkty.size() == 3 && punkty2.size() == 3)
            {
                Points(stopien);
                morphing(stopien);
            }
        }
    }
    update();
}
void Ekran::mouseMoveEvent(QMouseEvent *)
{

}
void Ekran::mouseReleaseEvent(QMouseEvent *)
{

}
