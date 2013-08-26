#include "databasemanager.h"

DatabaseManager::DatabaseManager(QObject *parent) :
    QObject(parent)
{
    lookUpCategories();
}

bool DatabaseManager::openDB()
{
    db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("BiSz_db");
    if (db.open()) {
        QSqlQuery query;
        query.exec("select * from product");
        qDebug()<<"\n****" << db.connectionName() << "opened****\n";
    }
    else {
        QMessageBox::critical(0, this->tr("Cannot open database"),
            this->tr("Unable to establish a database connection.\n"
                     "This program needs SQLite support. Please read "
                     "the Qt SQL driver documentation for information how "
                     "to build it.\n\n"
                     "Click Cancel to exit."), QMessageBox::Cancel);
        return false;
    }

    createDatabaseGlobals();

    return true;
}

void DatabaseManager::closeDB(QSqlDatabase *database)
{
    database->close();
}

void DatabaseManager::openDBold()
{
    db2 = QSqlDatabase::addDatabase("QSQLITE");
    db2.setDatabaseName("BiSz_db_old");
    if (db2.open()) {
        QSqlQuery query;
        query.exec("select * from items");
        qDebug()<<"\n****" << db2.connectionName() << "opened****\n";
    }
}

void DatabaseManager::createDatabaseGlobals()
{
    tableProductFieldList.append("id");
    tableProductFieldList.append("own_product_id");
    tableProductFieldList.append("foreign_product_id");
    tableProductFieldList.append("name");
    tableProductFieldList.append("description");
    tableProductFieldList.append("category_number");
    tableProductFieldList.append("package_unit");
    tableProductFieldList.append("minimal_order_quantity");
    tableProductFieldList.append("price");
    tableProductFieldList.append("purchase_price");
    tableProductFieldList.append("is_active_product");
    tableProductFieldList.append("enable_vat");
    tableProductFieldList.append("vat_value");
    tableProductFieldList.append("weight");
    tableProductFieldList.append("options");
    tableProductFieldList.append("enable_discounts");
    tableProductFieldList.append("discounts");
    tableProductFieldList.append("web_id");

    tableProductFieldHash.insert("id", "integer primary key");
    tableProductFieldHash.insert("own_product_id", "TEXT");
    tableProductFieldHash.insert("foreign_product_id", "TEXT");
    tableProductFieldHash.insert("name", "TEXT");
    tableProductFieldHash.insert("description", "TEXT");
    tableProductFieldHash.insert("category_number", "NUMERIC");
    tableProductFieldHash.insert("package_unit", "TEXT");
    tableProductFieldHash.insert("minimal_order_quantity", "NUMERIC");
    tableProductFieldHash.insert("price", "NUMERIC");
    tableProductFieldHash.insert("purchase_price", "NUMERIC");
    tableProductFieldHash.insert("is_active_product", "NUMERIC");
    tableProductFieldHash.insert("enable_vat", "NUMERIC");
    tableProductFieldHash.insert("vat_value", "NUMERIC");
    tableProductFieldHash.insert("weight", "NUMERIC");
    tableProductFieldHash.insert("options", "TEXT");
    tableProductFieldHash.insert("enable_discounts", "NUMERIC");
    tableProductFieldHash.insert("discounts", "TEXT");
    tableProductFieldHash.insert("web_id", "TEXT");

    tableCategoryFieldList.append("id");
    tableCategoryFieldList.append("category");

    tableCategoryFieldHash.insert("id", "integer primary key");
    tableCategoryFieldHash.insert("category", "TEXT");

    lookUpCategories();

    // TODO: fusion?
    csvImportFieldList.append("own_product_id");
    csvImportFieldList.append("name");
    csvImportFieldList.append("description");
    csvImportFieldList.append("category_number");
    csvImportFieldList.append("price");
    csvImportFieldList.append("enable_vat");
    csvImportFieldList.append("vat_value");
    csvImportFieldList.append("weight");
    csvImportFieldList.append("options");
    csvImportFieldList.append("enable_discounts");
    csvImportFieldList.append("discounts");
    csvImportFieldList.append("web_id");

    csvExportFieldList.append("id");
    csvExportFieldList.append("category");
    csvExportFieldList.append("name/code");
    csvExportFieldList.append("description");
    csvExportFieldList.append("price");
    csvExportFieldList.append("enable_vat");
    csvExportFieldList.append("vat_value");
    csvExportFieldList.append("weight");
    csvExportFieldList.append("options");
    csvExportFieldList.append("enable_discounts");
    csvExportFieldList.append("discounts");

}

void DatabaseManager::lookUpCategories()
{
    QSqlTableModel model;
    if(db.tables().isEmpty()) return;
    model.setTable("category");
    model.select();

    categoryList.clear();

    for(int i=0; i<model.rowCount(); ++i) {
//        Read out category
        categoryList.append(model.record(i).value("category").toString());
//        Split up category string
        QStringList categoryListParts = categoryList.at(i).split(" > ");

//        Fill up categorylists and -hashes
        if(!categoryListLevel_0.contains(categoryListParts.value(0))) {
            categoryListLevel_0.append(categoryListParts.value(0));
        }
        //qDebug() << "Category inserted: " << categoryListLevel_0.at(i);

        if(categoryListParts.length() > 1) {
            QString categoryLevel1Key   = categoryListParts.value(0);
            QString categoryLevel1Value = categoryListParts.value(1);

            if(!categoryHashLevel_1.values(categoryLevel1Key).contains(categoryLevel1Value)) {
                categoryHashLevel_1.insertMulti(categoryLevel1Key, categoryLevel1Value);
                //qDebug() << "SubCategory inserted: " << categoryLevel1Key << ":" << categoryLevel1Value;
            }
        }

        if(categoryListParts.length() > 2) {
            QString categoryLevel2Key   = categoryListParts.value(1);
            QString categoryLevel2Value = categoryListParts.value(2);

            if(!categoryHashLevel_2.values(categoryLevel2Key).contains(categoryLevel2Value)) {
                categoryHashLevel_2.insertMulti(categoryLevel2Key, categoryLevel2Value);
                //qDebug() << "SubSubCategory inserted: " << categoryLevel2Value;
            }
        }
    }

    categoryListLevel_0.sort();

/*    Print out categoryTree
    qDebug() << "There are" << categoryListLevel_0.length() << "main categories: ";
    for(int i=0; i<categoryListLevel_0.length(); ++i) {
        qDebug() << categoryListLevel_0.at(i);
        QStringList categoryListLevel_1 = categoryHashLevel_1.values(categoryListLevel_0.at(i));
        for(int j=0; j<categoryListLevel_1.length(); ++j) {
            qDebug() << "  - " << categoryListLevel_1.at(j);
            QStringList categoryListLevel_2 = categoryHashLevel_2.values(categoryListLevel_1.at(j));
            for(int k=0; k<categoryListLevel_2.length(); ++k) {
                qDebug() << "    - " << categoryListLevel_2.at(k);
            }
        }
    }
*/

}

bool DatabaseManager::addNewCategory(QString category)
{
    bool result = false;

    qDebug() << "Adding new category:" << category;

    categoryList.append(category);

    QSqlTableModel model;
    model.setTable("category");
    model.select();

    QSqlRecord categoryRecord;
    categoryRecord.append(QSqlField("category", QVariant::String));
    categoryRecord.setValue("category", category);

    result = model.insertRecord(-1, categoryRecord);
    if(!result) qDebug() << model.lastError();

    return result;
}

bool DatabaseManager::createTable(QString tableName, QStringList tableFieldList,
                                  QHash<QString, QString> tableFieldHash)
{
    bool result = false;

    if(db.open()) {
        QSqlQuery query;
        QString queryString;

        queryString = "create table ";
        queryString.append(tableName);
        queryString.append(" (");

        for(int i=0; i<tableFieldList.length(); ++i) {
            queryString.append(tableFieldList.at(i));
            queryString.append(" ");
            queryString.append(tableFieldHash.value(tableFieldList.at(i)));
            if(i+1 != tableFieldList.length()) {
                queryString.append(", ");
            }
            else {
                queryString.append(")");
            }
        }

//        qDebug() << "Executing the following query:\n" << queryString << "\n";
        if(!query.prepare(queryString)) {
            qDebug() << "Query is invalid";
            qDebug() << query.lastError();
        }
        if(!query.exec()) {
            qDebug() << query.lastError();
        }
        else {
            qDebug() << tableName << "TABLE created.";
        }
    }
    return result;
}
