#include "ekran.h"
#include <QPainter>
#include <QMouseEvent>

Ekran::Ekran(QWidget *parent) : QWidget{parent}
{
    im = QImage(640, 640, QImage::Format_RGB32);
    im_kolor = QImage(640,640, QImage::Format_RGB32);
    im_blackwhite_oryginal = QImage(640,640, QImage::Format_RGB32);
    im_kolor.load("/Users/truemonster/Desktop/PGK/morfologiczne/klaun.jpg");
    im = im_kolor;
    update();
}
void Ekran::reset()
{
    im = im_kolor;
    update();
}
void Ekran::blackTOwhite()
{
    color_to_blackwhite();
    update();
}
void Ekran::slot_czysc()
{
    czysc();
    update();
}
void Ekran::czysc()
{
    im.fill(0);
}
void Ekran::promien(int i)
{
    r = i;
    qDebug("%d", i);
}
void Ekran::dylatacja_przycisk()
{
    morfologia(r, false);
    copy_warstwa();
    update();
}
void Ekran::erozja_przycisk()
{
    morfologia(r, true);
    copy_warstwa();
    update();
}
void Ekran::domkniecie_przycisk()
{
    morfologia(r, false);
    copy_warstwa();
    morfologia(r, true);
    copy_warstwa();
    update();
}
void Ekran::otwarcie_przycisk()
{
    morfologia(r, true);
    copy_warstwa();
    morfologia(r, false);
    copy_warstwa();
    update();
}
void Ekran::paintEvent(QPaintEvent *)
{
    QPainter p(this);
    p.drawImage(0, 0, im);
}
void Ekran::mousePressEvent(QMouseEvent *)
{

}
void Ekran::mouseMoveEvent(QMouseEvent *)
{

}
void Ekran::mouseReleaseEvent(QMouseEvent *)
{

}
