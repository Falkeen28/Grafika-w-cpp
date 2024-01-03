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
    connect(ui->zmiana, SIGNAL(clicked()), ui->widget, SLOT(zmianaobrazka()));
    connect(ui->suwaczek, SIGNAL(valueChanged(int)), ui->widget, SLOT(zmiana_suwak(int)));
}
MainWindow::~MainWindow()
{
    delete ui;
}


