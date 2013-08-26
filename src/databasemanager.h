#ifndef DATABASEMANAGER_H
#define DATABASEMANAGER_H

#include <QObject>

#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include <QSqlTableModel>
#include <QSqlRecord>
#include <QSqlField>

#include <QMessageBox>

#include <QDebug>

class DatabaseManager : public QObject
{
    Q_OBJECT
public:
    explicit DatabaseManager(QObject *parent = 0);

    QSqlDatabase db;
    QSqlDatabase db2;

    bool openDB();
    void openDBold();
    void closeDB(QSqlDatabase *database);
    bool createTable(QString tableName, QStringList tableFieldList, QHash<QString, QString> tableFieldHash);
    bool backUpDB();
    void createDatabaseGlobals();
    void lookUpCategories();
    bool addNewCategory(QString category);

    QStringList tableProductFieldList;
    QStringList tableCategoryFieldList;
    QStringList csvImportFieldList;
    QStringList csvExportFieldList;

    QHash<QString, QString> tableProductFieldHash;
    QHash<QString, QString> tableCategoryFieldHash;

    QStringList categoryList;

    QStringList categoryListLevel_0;
    QHash<QString, QString> categoryHashLevel_1;
    QHash<QString, QString> categoryHashLevel_2;

private:
    
signals:
    
public slots:
    
};

#endif // DATABASEMANAGER_H
