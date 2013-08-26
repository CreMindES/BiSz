#include "mainwindow.h"
//#include "ui_mainwindow.h"
#include "itemnumbersearch.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent)//,
    //ui(new Ui::MainWindow)
{
    ItemNumberSearch *itemNumberWidget = new ItemNumberSearch(this);
    this->setWindowIcon(QIcon(QPixmap(":images/logo.png")));
    this->setCentralWidget(itemNumberWidget);
    this->show();
    //ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    //delete ui;
}
