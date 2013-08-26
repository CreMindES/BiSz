#include <QApplication>

#include "mainwindow.h"
#include "create_connection.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    a.setApplicationName("BiSz - Bútoripari Szerevények webáruház adatbázis");
    a.setApplicationVersion("0.2.0");
    QPixmap appIconPixmap(":/images/BiSzIcon.ico");
    //appIconPixmap = appIconPixmap.scaled(QSize(128, 128), Qt::KeepAspectRatio, Qt::SmoothTransformation);

    a.setWindowIcon(QIcon(appIconPixmap));

    //if (!createConnection()) return 1;

    MainWindow w;
    w.showMaximized();
    //w.show();
    
    return a.exec();
}
