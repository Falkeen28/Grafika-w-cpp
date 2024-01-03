#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent) , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    connect(ui->wyjdz, SIGNAL(clicked()), qApp, SLOT(quit()));
    connect(ui->reset, SIGNAL(clicked()), ui->widget, SLOT(reset()));
    connect(ui->widoczne, SIGNAL(clicked()), ui->widget, SLOT(zmianaWidocznosci()));
    connect(ui->tekstura, SIGNAL(clicked()), ui->widget, SLOT(teksturaTryb()));

    connect(ui->x_slider, SIGNAL(valueChanged(int)), ui->widget, SLOT(setXTranslation(int)));
    connect(ui->y_slider, SIGNAL(valueChanged(int)), ui->widget, SLOT(setYTranslation(int)));
    connect(ui->z_slider, SIGNAL(valueChanged(int)), ui->widget, SLOT(setZTranslation(int)));

    connect(ui->skalowanie_x, SIGNAL(valueChanged(int)), ui->widget, SLOT(setXScaling(int)));
    connect(ui->skalowanie_y, SIGNAL(valueChanged(int)), ui->widget, SLOT(setYScaling(int)));
    connect(ui->skalowanie_z, SIGNAL(valueChanged(int)), ui->widget, SLOT(setZScaling(int)));

    connect(ui->rotacja_x, SIGNAL(valueChanged(int)), ui->widget, SLOT(rotX(int)));
    connect(ui->rotacja_y, SIGNAL(valueChanged(int)), ui->widget, SLOT(rotY(int)));
    connect(ui->rotacja_z, SIGNAL(valueChanged(int)), ui->widget, SLOT(rotZ(int)));
}

MainWindow::~MainWindow()
{
    delete ui;
}

