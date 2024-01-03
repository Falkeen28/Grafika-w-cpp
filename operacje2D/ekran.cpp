#include "ekran.h"
#include <QPainter>
#include <QMouseEvent>

Ekran::Ekran(QWidget *parent) : QWidget{parent}
{
    temp =  QImage(QString::fromStdString("/Users/truemonster/Desktop/PGK/operacje2D/ziemia.jpg"));
    im2 =  QImage(QString::fromStdString("/Users/truemonster/Desktop/PGK/operacje2D/ziemia.jpg")); //tylko obrazek
    im =  QImage(640, 640, QImage::Format_RGB32); //obraz po przeksztalceniu

    im.fill(0);
    operacje2D(Tx, Ty, alfa, Sx, Sy, Shx, Shy);
    update();
}
void Ekran::reset()
{
    czysc();
    Tx = 0.0;
    Ty = 0.0;
    alfa = 0.0;
    Sx = 1.0;
    Sy = 1.0;
    Shx = 0.0;
    Shy = 0.0;
    operacje2D(Tx, Ty, alfa, Sx, Sy, Shx, Shy);
    update();
}
void Ekran::interpolacja_on()
{
    if(interpolacja_status)
    {
        interpolacja_status = false;
        qDebug("interpolacja WYL");
        operacje2D(Tx, Ty, alfa, Sx, Sy, Shx, Shy);
        update();
    }
    else
    {
        interpolacja_status = true;
        qDebug("interpolacja WL");
        operacje2D(Tx, Ty, alfa, Sx, Sy, Shx, Shy);
        update();
    }
}
void Ekran::setRotation(int value)
{
    czysc();
    alfa = value;
    operacje2D(Tx, Ty, alfa, Sx, Sy, Shx, Shy);
    update();
}
void Ekran::setXTranslation(int value)
{
    czysc();
    Tx = value;
    operacje2D(Tx, Ty, alfa, Sx, Sy, Shx, Shy);
    update();
}
void Ekran::setYTranslation(int value)
{
    czysc();
    Ty = value;
    operacje2D(Tx, Ty, alfa, Sx, Sy, Shx, Shy);
    update();
}
void Ekran::setXScaling(int value)
{
    czysc();
    Sx = (float)value/100;
    operacje2D(Tx, Ty, alfa, Sx, Sy, Shx, Shy);
    update();
}
void Ekran::setYScaling(int value)
{
    czysc();
    Sy = (float)value/100;
    operacje2D(Tx, Ty, alfa, Sx, Sy, Shx, Shy);
    update();
}
void Ekran::setXShearing(int value)
{
    czysc();
    Shx = (float)value/100;
    operacje2D(Tx, Ty, alfa, Sx, Sy, Shx, Shy);
    update();
}
void Ekran::setYShearing(int value)
{
    czysc();
    Shy = (float)value/100;
    operacje2D(Tx, Ty, alfa, Sx, Sy, Shx, Shy);
    update();
}
void Ekran::paintEvent(QPaintEvent *)
{
    QPainter p(this);
    p.drawImage(0, 0, im);
}
