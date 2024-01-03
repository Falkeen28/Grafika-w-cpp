#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QApplication>

MainWindow::MainWindow(QWidget *parent): QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    connect(ui->wyjdz, SIGNAL(clicked()), qApp, SLOT(quit()));
    connect(ui->clear, SIGNAL(clicked()), ui->widget, SLOT(slot_czysc()));

    connect(ui->RED, SIGNAL(valueChanged(int)), ui->widget, SLOT(red_wartosc(int)));
    connect(ui->GREEN, SIGNAL(valueChanged(int)), ui->widget, SLOT(green_wartosc(int)));
    connect(ui->BLUE, SIGNAL(valueChanged(int)), ui->widget, SLOT(blue_wartosc(int)));
    connect(ui->HUE, SIGNAL(valueChanged(int)), ui->widget, SLOT(hue_wartosc(int)));
    connect(ui->SATURATION, SIGNAL(valueChanged(int)), ui->widget, SLOT(saturation_wartosc(int)));
    connect(ui->VALUE, SIGNAL(valueChanged(int)), ui->widget, SLOT(value_wartosc(int)));
}
MainWindow::~MainWindow()
{
    delete ui;
}


