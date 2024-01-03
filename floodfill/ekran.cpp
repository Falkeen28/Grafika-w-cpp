#include "ekran.h"
#include <QPainter>
#include <QMouseEvent>

Ekran::Ekran(QWidget *parent) : QWidget{parent}
{
    im = QImage (1200, 1200, QImage::Format_RGB32);
    im.fill(0);
    im2 = QImage (1200, 1200, QImage::Format_RGB32);
    im2.fill(0);
    im3 = QImage (1200, 1200, QImage::Format_RGB32);
    im3.fill(0);
}
void Ekran::co_rysuje0()
{
    co_rysowac = 0;
    qDebug("linia %d",co_rysowac);
}
void Ekran::co_rysuje1()
{
    co_rysowac = 1;
    qDebug("kolo %d",co_rysowac);
}
void Ekran::co_rysuje2()
{
    co_rysowac = 2;
    qDebug("Elipsa %d",co_rysowac);
}
void Ekran::co_rysuje3()
{
    co_rysowac = 3;
    qDebug("dodawanie %d",co_rysowac);
}
void Ekran::co_rysuje4()
{
    co_rysowac = 4;
    qDebug("przesuwanie %d",co_rysowac);
}
void Ekran::co_rysuje5()
{
    co_rysowac = 5;
    qDebug("floodfill %d",co_rysowac);
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
    // Funkcja czysci (zamalowuje na bialo) obszar rysowania
    // definicja znajduje sie ponizej
    czysc();
    // Funkcja "update()" powoduje ponowne "namalowanie" calego komponentu
    // Wywoluje funkcje "paintEvent"
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
        //start
        startX = x;
        startY = y;
        qDebug("x %d y %d", finishX, finishY);
        im2 = im;
        im3 = im;
        // Odswiezamy komponent
        update();
    }
    else if(event->buttons() & Qt::LeftButton && co_rysowac == 1)
    {
        isPressedL = true;
        // Pobieramy wspolrzedne punktu klikniecia
        // Sa to wspolrzedne wzgledem glownego okna
        int x = event->pos().x();
        int y = event->pos().y();
        //start
        startX = x;
        startY = y;
        qDebug("x %d y %d", finishX, finishY);
        im2 = im;
        im3 = im;
        // Odswiezamy komponent
        update();
    }
    else if (event->buttons() & Qt::LeftButton && co_rysowac == 2)
    {
        // Pobieramy wspolrzedne punktu klikniecia
        // Sa to wspolrzedne wzgledem glownego okna
        int x = event->pos().x();
        int y = event->pos().y();
        //start
        startX = x;
        startY = y;
        qDebug("x %d y %d", finishX, finishY);
        im2 = im;
        im3 = im;
        // Odswiezamy komponent
        update();
    }
    else if (event->buttons() & Qt::LeftButton && co_rysowac == 3)
    {
        // Pobieramy wspolrzedne punktu klikniecia
        // Sa to wspolrzedne wzgledem glownego okna
        int x = event->pos().x();
        int y = event->pos().y();
        qDebug("x %d y %d", x, y);
        punkty.push_back(QPointF(x,y));
        if( punkty.size() >= 4)
        {
            if (!bezir)
                drawBezier(punkty, suwaczek);
            else
                drawBezierSpline(punkty, suwaczek);
        }
        im2 = im;
        // Odswiezamy komponent
        update();
    }
    else if(event->buttons() & Qt::LeftButton && co_rysowac == 5)
    {
        // Pobieramy wspolrzedne punktu klikniecia
        // Sa to wspolrzedne wzgledem glownego okna
        int x = event->pos().x();
        int y = event->pos().y();
        qDebug("x %d y %d", x, y);
        QRgb rgb = im.pixel(x,y);
        flood_fill(x, y, rgb);
        im2 = im;
        // Odswiezamy komponent
        update();
    }
    else if(event->buttons() & Qt::LeftButton && co_rysowac == 1)
    {
        isPressedL = true;
        // Pobieramy wspolrzedne punktu klikniecia
        // Sa to wspolrzedne wzgledem glownego okna
        int x = event->pos().x();
        int y = event->pos().y();
        //start
        startX = x;
        startY = y;
        qDebug("x %d y %d", finishX, finishY);
        im2 = im;
        im3 = im;
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
            if (!bezir)
                drawBezier(punkty, suwaczek);
            else
                drawBezierSpline(punkty, suwaczek);
        }
    }
    for (int i = 0; i < punkty.size(); i++)
    {
        drawPixel(punkty[i].x(), punkty[i].y(), 0, 0, 255);
    }
    update();
}
void Ekran::mouseMoveEvent(QMouseEvent *event)
{
    im = im2;
    if (event->buttons() && co_rysowac == 0)
    {
        int x = event->pos().x();
        int y = event->pos().y();
        //koniec
        finishX = x;
        finishY = y;
        drawLine(startX, startY, finishX, finishY);
        // odświeżanie okna
        update();
    }
    else if(event->buttons() && co_rysowac == 1)
    {
        int x = event->pos().x();
        int y = event->pos().y();
        //koniec
        finishX = x;
        finishY = y;
        drawCircle(startX, startY, finishX, finishY);
        // odświeżanie okna
        update();
    }
    else if (event->buttons() && co_rysowac == 2)
    {
        int x = event->pos().x();
        int y = event->pos().y();
        //koniec
        finishX = x;
        finishY = y;;
        drawElipse(startX, startY, finishX, finishY, suwaczek);
        // odświeżanie okna
        update();
    }
    else if (event->buttons() & Qt::LeftButton && co_rysowac == 4)
    {
        // Pobieramy wspolrzedne punktu klikniecia
        // Sa to wspolrzedne wzgledem glownego okna
        int x = event->pos().x();
        int y = event->pos().y();
        qDebug("x %d y %d", x, y);
        if(index_temp == -1)
        {
            index_temp = co_blizej(punkty, event->pos().x(), event->pos().y());
        }
        if(index_temp > -1)
        {
            im.fill(0);
            im = im3;
            punkty[index_temp].setX(x);
            punkty[index_temp].setY(y);
            if (!bezir)
            {
                drawBezier(punkty, suwaczek);
            }
            else
            {
                drawBezierSpline(punkty, suwaczek);
            }
        }
        // Odswiezamy komponent
        update();
    }
}
void Ekran::mouseReleaseEvent(QMouseEvent *event)
{
    index_temp = -1;
    if (event->buttons() && co_rysowac == 0)
    {
        int x = event->pos().x();
        int y = event->pos().y();
        //koniec
        finishX = x;
        finishY = y;
        drawLine(startX, startY, finishX, finishY);
        im3 = im;
    }
    else if(event->buttons() && co_rysowac == 1)
    {
        int x = event->pos().x();
        int y = event->pos().y();
        //koniec
        finishX = x;
        finishY = y;
        drawCircle(startX, startY, finishX, finishY);
        im3 = im;
    }
    else if (event->buttons() && co_rysowac == 2)
    {
        int x = event->pos().x();
        int y = event->pos().y();
        //koniec
        finishX = x;
        finishY = y;
        drawElipse(startX, startY, finishX, finishY, suwaczek);
        im3 = im;
    }
    // odświeżanie okna
    update();
}
