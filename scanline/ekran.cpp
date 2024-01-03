#include "ekran.h"
#include <QPainter>
#include <QMouseEvent>

Ekran::Ekran(QWidget *parent) : QWidget{parent}
{
    im = QImage (1200, 1200, QImage::Format_RGB32);
    im.fill(0);
    im2 = QImage (1200, 1200, QImage::Format_RGB32);
    im2.fill(0);
}
void Ekran::co_rysuje0()
{
    co_rysowac = 0;
    qDebug("dodawanie %d",co_rysowac);
}
void Ekran::co_rysuje1()
{
    co_rysowac = 1;
    qDebug("przesuwanie %d",co_rysowac);
}
void Ekran::co_rysuje2()
{
    narysowane = true;
    scanline(punkty);
    for (int i = 0; i < punkty.size(); i++)
        drawPixel(punkty[i].x(), punkty[i].y(), 0, 0, 255);
    update();
}
void Ekran::bezeir1()
{
    bezir = false;
    qDebug("bezsklejania %d",bezir);
}
void Ekran::bezeir2()
{
    bezir = true;
    qDebug("ze sklejaniem %d",bezir);
}
void Ekran::slot_czysc()
{
    czysc();
    update();
}
void Ekran::Suwak(int n)
{
    qDebug("%d",n);
    suwaczek = n;
}
void Ekran::czysc()
{
    im.fill(0);
    punkty.clear();
    narysowane = false;
}
void Ekran::paintEvent(QPaintEvent *)
{
    QPainter p(this);
    p.fillRect(0, 0, width(), height(), Qt::yellow);
    p.drawImage(0, 0, im);
}
void Ekran::mousePressEvent(QMouseEvent *event)
{
    if (event->buttons() & Qt::LeftButton && co_rysowac == 0)
    {
        // Pobieramy wspolrzedne punktu klikniecia
        // Sa to wspolrzedne wzgledem glownego okna
        int x = event->pos().x();
        int y = event->pos().y();
        qDebug("x %d y %d", x, y);
        punkty.push_back(QPointF(x,y));
        im2 = im;
        // Odswiezamy komponent
        update();
    }
    else if(event->buttons() & Qt::LeftButton && co_rysowac == 1)
    {
        // Pobieramy wspolrzedne punktu klikniecia
        // Sa to wspolrzedne wzgledem glownego okna
        int x = event->pos().x();
        int y = event->pos().y();
        qDebug("x %d y %d", x, y);
        int index = co_blizej(punkty, event->pos().x(), event->pos().y());
        if(index > -1)
        {
            punkty[index].setX(x);
            punkty[index].setY(y);
        }
        // Odswiezamy komponent
        update();
    }
    else if(event->buttons() & Qt::RightButton)
    {
        int index = co_blizej(punkty, event->pos().x(), event->pos().y());
        if(index > -1)
        {
            punkty.remove(index);
            im.fill(0);
            if(narysowane)
                co_rysuje2();
        }
    }
    for (int i = 0; i < punkty.size(); i++)
        drawPixel(punkty[i].x(), punkty[i].y(), 0, 0, 255);
    update();
}
void Ekran::mouseMoveEvent(QMouseEvent *event)
{
    // Pobieramy wspolrzedne punktu klikniecia
    // Sa to wspolrzedne wzgledem glownego okna
    int x = event->pos().x();
    int y = event->pos().y();
    qDebug("x %d y %d", x, y);
    if(index_temp == -1 && co_rysowac == 1)
        index_temp = co_blizej(punkty, event->pos().x(), event->pos().y());
    if(index_temp > -1 && co_rysowac == 1)
    {
        im.fill(0);
        punkty[index_temp].setX(x);
        punkty[index_temp].setY(y);
        if(narysowane)
            co_rysuje2();
        for (int i = 0; i < punkty.size(); i++)
            drawPixel(punkty[i].x(), punkty[i].y(), 0, 0, 255);
    }
    // odświeżanie okna
    update();
}
void Ekran::mouseReleaseEvent(QMouseEvent *)
{
    index_temp = -1;
    // odświeżanie okna
    update();
}
