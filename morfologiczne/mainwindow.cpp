#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QApplication>

MainWindow::MainWindow(QWidget *parent): QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    connect(ui->wyjdz, SIGNAL(clicked()), qApp, SLOT(quit()));
    connect(ui->alpha_poziom, SIGNAL(valueChanged(int)), ui->widget, SLOT(promien(int)));
    connect(ui->dylatacja_b, SIGNAL(clicked(bool)), ui->widget, SLOT(dylatacja_przycisk()));
    connect(ui->erozja_b, SIGNAL(clicked(bool)), ui->widget, SLOT(erozja_przycisk()));
    connect(ui->domkniecie, SIGNAL(clicked(bool)), ui->widget, SLOT(domkniecie_przycisk()));
    connect(ui->otwarcie, SIGNAL(clicked(bool)), ui->widget, SLOT(otwarcie_przycisk()));
    connect(ui->reset, SIGNAL(clicked(bool)), ui->widget, SLOT(reset()));
    connect(ui->black_white, SIGNAL(clicked(bool)), ui->widget, SLOT(blackTOwhite()));
}
MainWindow::~MainWindow()
{
    delete ui;
}



