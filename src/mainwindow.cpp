#include "mainwindow.h"
<<<<<<< HEAD
<<<<<<< HEAD
=======
#include "ui_mainwindow.h"

>>>>>>> 2307d5a6beea8efef46df353b36c8c58330782a1
=======
#include "ui_mainwindow.h"

>>>>>>> 2307d5a6beea8efef46df353b36c8c58330782a1
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
<<<<<<< HEAD
<<<<<<< HEAD
    //delete ui;
=======
    delete ui;
>>>>>>> 2307d5a6beea8efef46df353b36c8c58330782a1
=======
    delete ui;
>>>>>>> 2307d5a6beea8efef46df353b36c8c58330782a1
}
