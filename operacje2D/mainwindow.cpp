#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QApplication>

MainWindow::MainWindow(QWidget *parent): QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    connect(ui->wyjdz, SIGNAL(clicked()), qApp, SLOT(quit()));
    connect(ui->reset, SIGNAL(clicked()), ui->widget, SLOT(reset()));
    connect(ui->x_slider, SIGNAL(valueChanged(int)), ui->widget, SLOT(setXTranslation(int)));
    connect(ui->y_slider, SIGNAL(valueChanged(int)), ui->widget, SLOT(setYTranslation(int)));
    connect(ui->alfa_slider, SIGNAL(valueChanged(int)), ui->widget, SLOT(setRotation(int)));
    connect(ui->sx_slider, SIGNAL(valueChanged(int)), ui->widget, SLOT(setXScaling(int)));
    connect(ui->sy_slider, SIGNAL(valueChanged(int)), ui->widget, SLOT(setYScaling(int)));
    connect(ui->shx_slider, SIGNAL(valueChanged(int)), ui->widget, SLOT(setXShearing(int)));
    connect(ui->shy_slider, SIGNAL(valueChanged(int)), ui->widget, SLOT(setYShearing(int)));
    connect(ui->INTERPOLACJA, SIGNAL(clicked()), ui->widget, SLOT(interpolacja_on()));
}
MainWindow::~MainWindow()
{
    delete ui;
}




