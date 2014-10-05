#include <QGuiApplication>
#include <QQuickView>

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

    // BiSz application
    app.setApplicationName("BiSz - Bútoripari Szerevények webáruház adatbázis");
    app.setApplicationVersion("0.5.0");

    // BiSz engine
//    BiSz ourBiSz;

    // BiSz interface
    QQuickView BiSzInterface;

    QPixmap appIconPixmap(":/images/BiSzIcon.ico");
    BiSzInterface.setIcon( QIcon(appIconPixmap) );
    BiSzInterface.setSource( QUrl::fromLocalFile("../src/main.qml") );
    BiSzInterface.show();
    
    return app.exec();
}
