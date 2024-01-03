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
    connect(ui->DODAWANIE, SIGNAL(toggled(bool)), ui->widget, SLOT(co_rysuje0()));
    connect(ui->PRZESUWANIE, SIGNAL(toggled(bool)), ui->widget, SLOT(co_rysuje1()));
    connect(ui->Rysuj, SIGNAL(clicked()), ui->widget, SLOT(co_rysuje2()));
    connect(ui->INTERPOLACJA, SIGNAL(clicked()), ui->widget, SLOT(interpolacja_on()));
}
MainWindow::~MainWindow()
{
    delete ui;
}


