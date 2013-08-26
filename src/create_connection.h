#ifndef MYCONNECTION_H
#define MYCONNECTION_H

#include <QMessageBox>
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include <QDebug>
#include <QFileDialog>

bool createConnection()
{
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("BiSz_db");
    if (db.open()) {
        QSqlQuery query;
        query.exec("select * from part");
        qDebug()<<"\n**** db opened**** :";
        qDebug() << "Connection established: " << db.connectionName();
    }
    else {
        QMessageBox::critical(0, qApp->tr("Cannot open database"),
            qApp->tr("Unable to establish a database connection.\n"
                     "This example needs SQLite support. Please read "
                     "the Qt SQL driver documentation for information how "
                     "to build it.\n\n"
                     "Click Cancel to exit."), QMessageBox::Cancel);
        return false;
    }

    return true;
}

#endif // MYCONNECTION_H
