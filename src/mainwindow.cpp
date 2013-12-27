#include "mainwindow.h"
//#include "ui_mainwindow.h"


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent)//,
    //ui(new Ui::MainWindow)
{
    itemNumberWidget = new ItemNumberSearch();
    this->setWindowIcon(QIcon(QPixmap(":images/logo.png")));
    this->setCentralWidget(itemNumberWidget);
    this->show();
    //ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    //delete ui;
}
