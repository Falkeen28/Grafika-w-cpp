#include "ekran.h"
#include <QPainter>
#include <QMouseEvent>

Ekran::Ekran(QWidget *parent):QWidget{parent}
{
    im = QImage(900, 600, QImage::Format_RGB32);
    temp = QImage(900, 600, QImage::Format_RGB32);
    im2 = QImage(900, 600, QImage::Format_RGB32);
    imG = QImage(900, 600, QImage::Format_RGB32);
    im.fill(0);
    im2.load("/Users/truemonster/Desktop/PGK/teksturowanie/kotek.jpg");
}
void Ekran::paintEvent(QPaintEvent *)
{
    QPainter p(this);
    p.drawImage(0, 0, im);
    p.drawImage(450, 0, im2);
}
void Ekran::co_rysuje0()
{
    co_rysowac = 0;
    qDebug("dodawanie %d", co_rysowac);
}
void Ekran::co_rysuje1()
{
    co_rysowac = 1;
    qDebug("przesuwanie %d", co_rysowac);
}
void Ekran::co_rysuje2()
{
    im.fill(0);
    im2.load("/Users/truemonster/Desktop/PGK/teksturowanie/kotek.jpg");
    narysowane = true;
    teksturowanie();
    obrys(punkty);
    obrys(punkty2);
    update();
}
void Ekran::interpolacja_on()
{
    if(interpolacja_status)
    {
        interpolacja_status = false;
        qDebug("interpolacja WYL");
        if(punkty.size() == 3 && punkty2.size() == 3 )
            co_rysuje2();
    }
    else
    {
        interpolacja_status = true;
        qDebug("interpolacja WL");
        if(punkty.size() == 3 && punkty2.size() == 3 )
            co_rysuje2();
    }
}
void Ekran::slot_czysc()
{
    czysc();
    update();
}
void Ekran::czysc()
{
    im.fill(0);
    im2.load("/Users/truemonster/Desktop/PGK/teksturowanie/kotek.jpg");
    punkty.clear();
    punkty2.clear();
    narysowane = false;
}
void Ekran::mousePressEvent(QMouseEvent *event)
{
    if (event->buttons() & Qt::LeftButton && co_rysowac == 0)
    {
        // Pobieramy wspolrzedne punktu klikniecia
        // Sa to wspolrzedne wzgledem glownego okna
        int x = event->pos().x();
        int y = event->pos().y();
        if (x < (im.width()) / 2)
        {
            if(punkty.size() < 3)
            {
                qDebug("x %d y %d", x, y);
                punkty.push_back(QPointF(x, y));
                temp = im;
            }
        }
        else
        {
            if(punkty2.size() < 3)
            {
                qDebug("x %d y %d", x, y);
                punkty2.push_back(QPointF(x, y));
                imG = im2;
            }
        }
        // Odswiezamy komponent
        update();
    }
    else if (event->buttons() & Qt::LeftButton && co_rysowac == 1)
    {
        // Pobieramy wspolrzedne punktu klikniecia
        // Sa to wspolrzedne wzgledem glownego okna
        int x = event->pos().x();
        int y = event->pos().y();
        qDebug("x %d y %d", x, y);
        int index = co_blizej(punkty, event->pos().x(), event->pos().y());
        int index2 = co_blizej(punkty2, event->pos().x(), event->pos().y());
        if (x < (im.width()) / 2)
        {
            if (index > -1)
            {
                punkty[index].setX(x);
                punkty[index].setY(y);
            }
        }
        else
        {
            if (index2 > -1)
            {
                punkty2[index2].setX(x);
                punkty2[index2].setY(y);
            }
        }
        // Odswiezamy komponent
        update();
    }

    for (int i = 0; i < punkty.size(); i++)
        drawPixel(punkty[i].x(), punkty[i].y(), 0, 0, 255);

    for (int i = 0; i < punkty2.size(); i++)
        drawPixel(punkty2[i].x(), punkty2[i].y(), 0, 0, 255);

    update();
}
void Ekran::mouseMoveEvent(QMouseEvent *event)
{
    // Pobieramy wspolrzedne punktu klikniecia
    // Sa to wspolrzedne wzgledem glownego okna
    int x = event->pos().x();
    int y = event->pos().y();
    qDebug("x %d y %d", x, y);
    if (x < (im.width()) / 2)
    {
        if (index_temp == -1 && co_rysowac == 1)
            index_temp = co_blizej(punkty, event->pos().x(), event->pos().y());
        if (index_temp > -1 && co_rysowac == 1)
        {
            im.fill(0);
            punkty[index_temp].setX(x);
            punkty[index_temp].setY(y);
            if (narysowane)
                co_rysuje2();
            for (int i = 0; i < punkty.size(); i++)
                drawPixel(punkty[i].x(), punkty[i].y(), 0, 0, 255);
        }
    }
    else
    {
        if (index_temp2 == -1 && co_rysowac == 1)
            index_temp2 = co_blizej(punkty2, event->pos().x(), event->pos().y());
        if (index_temp2 > -1 && co_rysowac == 1)
        {
            im2.load("/Users/truemonster/Desktop/PGK/teksturowanie/kotek.jpg");
            punkty2[index_temp2].setX(x);
            punkty2[index_temp2].setY(y);
            if (narysowane)
                co_rysuje2();
            for (int i = 0; i < punkty2.size(); i++)
                drawPixel(punkty2[i].x(), punkty2[i].y(), 0, 0, 255);
        }
    }
    // odświeżanie okna
    update();
}
void Ekran::mouseReleaseEvent(QMouseEvent *)
{
    index_temp = -1;
    index_temp2 = -1;
    // odświeżanie okna
    update();
}
