#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    
private:
<<<<<<< HEAD
<<<<<<< HEAD
    //Ui::MainWindow *ui;

=======
    Ui::MainWindow *ui;
>>>>>>> 2307d5a6beea8efef46df353b36c8c58330782a1
=======
    Ui::MainWindow *ui;
>>>>>>> 2307d5a6beea8efef46df353b36c8c58330782a1
};

#endif // MAINWINDOW_H
