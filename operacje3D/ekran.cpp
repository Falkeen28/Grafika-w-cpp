#include "ekran.h"
#include <QPainter>
#include <QMouseEvent>

Ekran::Ekran(QWidget *parent) : QWidget{parent}
{
    im = QImage(640,640,QImage::Format_RGB32);
    im.fill(0);
    //0
    punkty.append({210,210,0});
    //1
    punkty.append({210,210,100});
    //2
    punkty.append({420,210,0});
    //3
    punkty.append({420,210,100});
    //4
    punkty.append({210,420,0});
    //5
    punkty.append({210,420,100});
    //6
    punkty.append({420,420,0});
    //7
    punkty.append({420,420,100});
    /*
        Ściany:
        Przednia - 0,2,4,6
        Lewa - 1,0,5,4
        Prawa - 2,3,6,7
        Tylnia - 3,1,7,5
        Gorna - 1,3,0,2
        Dolna - 4,6,5,7
    */
}
void Ekran::resizeEvent(QResizeEvent *)
{
    QImage imCopy;
    imCopy = im.copy();
    im = QImage(szerokosc,wysokosc,QImage::Format_RGB32);
    obraz = QImage(szerokosc, wysokosc, QImage::Format_RGB32);
    obraz.load("/Users/truemonster/Desktop/PGK/operacje3D/image.jpeg");
    QPainter p(&im);
    p.drawImage(0,0,imCopy);
    liczMacierze();
}
void Ekran::reset()
{
    czysc();
    alphaX = 0, alphaY = 0, alphaZ = 0;
    sX = 1, sY = 1, sZ = 1;
    tX = 0, tY=  0, tZ = 0;
    liczMacierze();
}
void Ekran::setXTranslation(int value)
{
    tX = value;
    qDebug("%f",tX);
    liczMacierze();
}
void Ekran::setYTranslation(int value)
{
    tY = value;
    qDebug("%f",tY);
    liczMacierze();
}
void Ekran::setZTranslation(int value)
{
    tZ = value;
    qDebug("%f",tZ);
    liczMacierze();
}
void Ekran::setXScaling(int value)
{
    sX = value/100.0;
    qDebug("%f",sX);
    liczMacierze();
}
void Ekran::setYScaling(int value)
{
    sY = value/100.0;
    qDebug("%f",sY);
    liczMacierze();
}
void Ekran::setZScaling(int value)
{
    sZ = value/100.0;
    qDebug("%f",sZ);
    liczMacierze();
}
void Ekran::rotX(int value)
{
    alphaX = value;
    qDebug("%d",alphaX);
    liczMacierze();
}
void Ekran::rotY(int value)
{
    alphaY = value;
    qDebug("%d",alphaY);
    liczMacierze();
}
void Ekran::rotZ(int value)
{
    alphaZ = value;
    qDebug("%d",alphaZ);
    liczMacierze();
}
void Ekran::zmianaWidocznosci()
{
    if(!czyWidoczna)
    {
        qDebug("włączam widocznosc");
        czyWidoczna = true;
        liczMacierze();
    }
    else
    {
        qDebug("wyłączam widocznosc");
        czyWidoczna = false;
        liczMacierze();
    }
}
void Ekran::teksturaTryb()
{
    if(!czyTekstura)
    {
        qDebug("włączam teksture");
        czyTekstura = true;
        if(czyWidoczna)
        {
            liczMacierze();
        }
    }
    else
    {
        qDebug("wyłączam teksture");
        czyTekstura = false;
        liczMacierze();
    }
}
void Ekran::paintEvent(QPaintEvent *)
{
    QPainter p(this);
    p.drawImage(0, 0, im);
}
