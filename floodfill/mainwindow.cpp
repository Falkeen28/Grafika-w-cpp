#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QApplication>
#include <QtCore>
#include <QRadioButton>

MainWindow::MainWindow(QWidget *parent): QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    connect(ui->czysc, SIGNAL(clicked()), ui->widget, SLOT(slot_czysc()));
    connect(ui->wyjdz, SIGNAL(clicked()), qApp, SLOT(quit()));
    connect(ui->suwak, SIGNAL(valueChanged(int)), ui->widget, SLOT(Suwak(int)));
    connect(ui->linia, SIGNAL(toggled(bool)), ui->widget, SLOT(co_rysuje0()));
    connect(ui->kolo, SIGNAL(toggled(bool)), ui->widget, SLOT(co_rysuje1()));
    connect(ui->elipsa, SIGNAL(toggled(bool)), ui->widget, SLOT(co_rysuje2()));
    connect(ui->DODAWANIE, SIGNAL(toggled(bool)), ui->widget, SLOT(co_rysuje3()));
    connect(ui->PRZESUWANIE, SIGNAL(toggled(bool)), ui->widget, SLOT(co_rysuje4()));
    connect(ui->suwak, SIGNAL(valueChanged(int)), this, SLOT(set_wartosc(int)));
    connect(ui->bezsklejania, SIGNAL(clicked(bool)), ui->widget, SLOT(bezeir1()));
    connect(ui->sklejanie, SIGNAL(clicked(bool)), ui->widget, SLOT(bezeir2()));
    connect(ui->wiadro, SIGNAL(toggled(bool)), ui->widget, SLOT(co_rysuje5()));
}
MainWindow::~MainWindow()
{
    delete ui;
}
void MainWindow::set_wartosc(int n)
{
    ui->wartosc->setText(QString("%0").arg(n));
}


