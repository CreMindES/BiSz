#include "itemnumbersearch.h"

#include <QSqlRecord>
#include <QHeaderView>
#include <QFile>
#include <QSqlField>
#include <QSqlError>

ItemNumberSearch::ItemNumberSearch(QWidget *parent) :
    QWidget(parent)
{
    // Setting up database and creating our model
    myDatabase = new DatabaseManager;
    myDatabase->openDB();

    myModel = new QSqlRelationalTableModel;
    proxyModel = new MyQSFPModel;
    itemTableView = new QTableView;

    setupModel(myModel);
    proxyModel->setSourceModel(myModel);
    createView(proxyModel);

    // Application settings
    appSettings = new QSettings("DorogButor", "BiSz", this);


    // UI
    searchButton = new QPushButton(trUtf8("Keresés"));

    onlineSearchCheckBox = new QCheckBox(trUtf8("Azonnali keresés"));
    exactMatchCheckBox = new QCheckBox(trUtf8("Teljes egyezés"));
    customerViewCheckBox = new QCheckBox(trUtf8("Vevői nézet"));
    productPreviewCheckBox = new QCheckBox(trUtf8("Előnézet"));

    QPixmap searchIconPixmap(":/images/searchIcon.png");
    //searchIconLabel->setPixmap(searchIconPixmap.scaledToHeight(30, Qt::SmoothTransformation));
    searchIconLabel = new QLabel;
    searchIconLabel->setPixmap(searchIconPixmap.scaledToHeight(30, Qt::SmoothTransformation));
    //searchIconLabel->setMaximumSize(30,30);
    searchField = new QLineEditWithClearButton;

    QFont font;
    font.setFamily("Helvetica");
    font.setPixelSize(13);
    searchField->setFont(font);
    searchField->setMinimumHeight(font.pixelSize() + 11);

    productCountLabel = new QLabel();
    searchMatchCountLabel = new QLabel();

    // Dialogs
    productDialog = new ProductDetailsWidget(this, myDatabase);
    optionsDialog = new OptionsDialog(appSettings, this);

    // Layouts
    mainLayout = new QHBoxLayout;
    searchLayout = new QHBoxLayout;
    browserLayout = new QVBoxLayout;
    menuLayout = new QVBoxLayout;

    searchLayout->addWidget(searchIconLabel);
    searchLayout->addWidget(searchField);
    searchLayout->addWidget(searchButton);
    searchLayout->addSpacing(40);
    searchLayout->addWidget(onlineSearchCheckBox);
    searchLayout->addWidget(exactMatchCheckBox);
    searchLayout->addWidget(customerViewCheckBox, 0, Qt::AlignRight);
    //searchLayout->addWidget(productPreviewCheckBox, 0, Qt::AlignLeft);
    searchLayout->addStretch();
    searchLayout->addWidget(searchMatchCountLabel);
    searchLayout->addSpacing(5);

    browserLayout->addLayout(searchLayout);
    //browserLayout->addWidget(createView(proxyModel));
    browserLayout->addWidget(itemTableView);
    browserLayout->addWidget(productCountLabel);

    createQmlMenuSidebar();
    menuLayout->addWidget(qmlMenuSidebar);
    menuLayout->addStretch();

    mainLayout->addLayout(browserLayout);
    mainLayout->addSpacing(10);
    mainLayout->addLayout(menuLayout);

    this->setLayout(mainLayout);

    //(searchField, SIGNAL(textChanged(QString)), this, SLOT(searchInDatabase()));
    //connect(importButton, SIGNAL(clicked()), this, SLOT(importFromTxt()));

    connect(onlineSearchCheckBox, SIGNAL(stateChanged(int)), this, SLOT(onOnlineSearchCheckBoxStateChanged()));
    connect(searchField, SIGNAL(returnPressed()), this, SLOT(searchInDatabase()));
    //connect(searchField, SIGNAL(textEmpty()), this, SLOT(searchInDatabase()));

    connect(exactMatchCheckBox, SIGNAL(toggled(bool)), this, SLOT(searchInDatabase()));
    connect(exactMatchCheckBox, SIGNAL(stateChanged(int)), this, SLOT(exactMatchChecked()));

    connect(productPreviewCheckBox, SIGNAL(toggled(bool)),
            this,                   SLOT(on_productPreviewCheckBox_toggled(bool)));

    connect(customerViewCheckBox, SIGNAL(toggled(bool)),
            this,                 SLOT(on_customerViewCheckBox_toggled(bool)));

    connect(itemTableView->selectionModel(), SIGNAL(selectionChanged(QItemSelection,QItemSelection)),
            this, SLOT(on_itemTableView_selectionModel_currentChanged()));
    connect(productDialog->foreignProductIdLineEdit, SIGNAL(editingFinished()),
            this                                   , SLOT(on_foreignProductIdEdited()));

    connect(optionsDialog->reducedControlCheckBox, SIGNAL(toggled(bool)),
            this,                                  SLOT(enableReducedFunctions(bool)));
    connect(optionsDialog, SIGNAL(exportToAccountingButton_clicked()),
            this,          SLOT(exportToAccountingApp()));

    onlineSearchCheckBox->setChecked(false);
    exactMatchCheckBox->setChecked(false);
    //productPreviewCheckBox->setChecked(true);

    itemTableView->setAlternatingRowColors(true);
    itemTableView->setEditTriggers(QTableView::NoEditTriggers);

    itemTableView->resizeColumnsToContents();

    itemTableView->selectRow(0);

    // demonstration
/*
    QSqlQueryModel *typeComboModel = new QSqlQueryModel;
    typeComboModel->setQuery("SELECT * FROM link");
    for(int i=0; i< typeComboModel->rowCount(); ++i) {
        QSqlRecord record = typeComboModel->record(i);
        qDebug() << "id:" << record.value("id").toInt()
                 << "| name:" << record.value("link").toString();
        //QString name = typeComboModel->data(i, Qt::DisplayRole).toString();
        //qDebug() << id; //<< name;
    }
*/
    updateProductCount();

    loadAppSettings();

    onOnlineSearchCheckBoxStateChanged();

//    customFunction();
}

ItemNumberSearch::~ItemNumberSearch()
{
    saveAppSettings();
}
// UI
void ItemNumberSearch::createQmlMenuSidebar()
{
    qmlMenuView = new QQuickView;
//    qmlMenuView->setSource(QUrl("qrc:/menu.qml"));
    qmlMenuView->setSource(QUrl::fromLocalFile("../src/menu.qml"));
//    QSurfaceFormat format;
//    format.setSamples(16);
//    qmlMenuView->setFormat(format);
    qmlMenuSidebar = new QWidget;
    qmlMenuSidebar = QWidget::createWindowContainer(qmlMenuView);
    qmlMenuSidebar->setMinimumSize(qmlMenuView->size());
//    qmlMenuSidebar->setMinimumSize(200, 500);
//    qmlMenuSidebar->setMaximumSize(qmlMenuSidebar->minimumSize());

    qmlMenuObject = qmlMenuView->rootObject();
        qmlLogo = qmlMenuObject->findChild<QObject*>("biglogo");
        qmlNewButton = qmlMenuObject->findChild<QObject*>("newButton");
        qmlModifyButton = qmlMenuObject->findChild<QObject*>("modifyButton");
        qmlDeleteButton = qmlMenuObject->findChild<QObject*>("deleteButton");
        qmlPrintButton = qmlMenuObject->findChild<QObject*>("printButton");
        qmlOptionsButton = qmlMenuObject->findChild<QObject*>("optionsButton");
        qmlExportCSVButton = qmlMenuObject->findChild<QObject*>("exportCSVButton");
        qmlImportCSVButton = qmlMenuObject->findChild<QObject*>("importCSVButton");
        qmlProductThumbnail = qmlMenuObject->findChild<QObject*>("productThumbnailImage");


    qmlModifyButton->setProperty("enabled", false);
    qmlDeleteButton->setProperty("enabled", false);

    connect(qmlLogo,             SIGNAL(clicked()), this, SLOT(showAboutDialog()));
    connect(qmlNewButton,        SIGNAL(clicked()), this, SLOT(on_addButton_clicked()));
    connect(qmlModifyButton,     SIGNAL(clicked()), this, SLOT(on_modifyButton_clicked()));
    connect(qmlDeleteButton,     SIGNAL(clicked()), this, SLOT(on_deleteButton_clicked()));
    connect(qmlPrintButton,      SIGNAL(clicked()), this, SLOT(on_exportToPrintButton_clicked()));
    connect(qmlOptionsButton,    SIGNAL(clicked()), this, SLOT(on_optionsButton_clicked()));
    connect(qmlExportCSVButton,  SIGNAL(clicked()), this, SLOT(exportCSV()));
    connect(qmlImportCSVButton,  SIGNAL(clicked()), this, SLOT(importCSV()));
    connect(qmlProductThumbnail, SIGNAL(clicked()), this, SLOT(on_productThumbnail_clicked()));
}

void ItemNumberSearch::enableReducedFunctions(bool doEnable)
{
    bool state;

    if(doEnable) {
        state = false;
    }
    else {
        state = true;
    }

    qmlNewButton->setProperty("enabled", state);
    qmlModifyButton->setProperty("enabled", state);
    qmlDeleteButton->setProperty("enabled", state);
    qmlExportCSVButton->setProperty("enabled", state);
    qmlImportCSVButton->setProperty("enabled", state);
}

// App logic
void ItemNumberSearch::saveAppSettings()
{
    appSettings->beginGroup("checkbox");
    appSettings->setValue("onlineSearchLastValue", onlineSearchCheckBox->isChecked());
    appSettings->setValue("exactMatchLastValue", exactMatchCheckBox->isChecked());
    appSettings->setValue("customerViewLastValue", customerViewCheckBox->isChecked());
    appSettings->setValue("productPreviewLastValue", productPreviewCheckBox->isChecked());
    appSettings->endGroup();
}

void ItemNumberSearch::loadAppSettings()
{
    appSettings->beginGroup("checkbox");

    appSettings->value("onlineSearchDefEnabled", false).toBool() ?
        onlineSearchCheckBox->setChecked(appSettings->value("onlineSearchLastValue", false).toBool()) :
        onlineSearchCheckBox->setChecked(appSettings->value("onlineSearchDefValue", false).toBool());
    appSettings->value("exactMatchDefEnabled", false).toBool() ?
        exactMatchCheckBox->setChecked(appSettings->value("exactMatchDefValue", false).toBool()) :
        exactMatchCheckBox->setChecked(appSettings->value("exactMatchLastValue", false).toBool());
    appSettings->value("customerViewDefEnabled", false).toBool() ?
        customerViewCheckBox->setChecked(appSettings->value("customerViewDefValue", false).toBool()) :
        customerViewCheckBox->setChecked(appSettings->value("customerViewLastValue", false).toBool());
    productPreviewCheckBox->setChecked(appSettings->value("productPreviewLastValue", true ).toBool());

    appSettings->endGroup();

    if(optionsDialog->reducedControlCheckBox->isChecked()) enableReducedFunctions(true);
}

void ItemNumberSearch::setupModel(QSqlRelationalTableModel *model)
{
    qDebug() << "Setting up QSqlRelationTableModel";
    model->setTable("product");

    model->setRelation(5, QSqlRelation("category", "id", "category"));
    //model->setRelation(4, QSqlRelation("part_package", "package_id", "package_name"));

    model->setHeaderData(0, Qt::Horizontal, QObject::trUtf8("id"));
    model->setHeaderData(1, Qt::Horizontal, QObject::trUtf8("Saját cikkszám"));
    model->setHeaderData(2, Qt::Horizontal, QObject::trUtf8("Idegen cikkszám"));
    model->setHeaderData(3, Qt::Horizontal, QObject::trUtf8("Név"));
    model->setHeaderData(4, Qt::Horizontal, QObject::trUtf8("Leírás"));
    model->setHeaderData(5, Qt::Horizontal, QObject::trUtf8("Kategóra"));
    model->setHeaderData(6, Qt::Horizontal, QObject::trUtf8("Csomagolási egység"));
    model->setHeaderData(7, Qt::Horizontal, QObject::trUtf8("Minimális rendelési egység"));
    model->setHeaderData(8, Qt::Horizontal, QObject::trUtf8("Webáruház ár"));
    model->setHeaderData(9, Qt::Horizontal, QObject::trUtf8("Beszerzési ár"));
    model->setHeaderData(10, Qt::Horizontal, QObject::trUtf8("Forgalomban"));
    model->setHeaderData(11, Qt::Horizontal, QObject::trUtf8("Egyedi ÁFA"));
    model->setHeaderData(12, Qt::Horizontal, QObject::trUtf8("Egyedi ÁFA értéke"));
    model->setHeaderData(13, Qt::Horizontal, QObject::trUtf8("Súly"));
    model->setHeaderData(14, Qt::Horizontal, QObject::trUtf8("Opciók"));
    model->setHeaderData(15, Qt::Horizontal, QObject::trUtf8("Akció"));
    model->setHeaderData(16, Qt::Horizontal, QObject::trUtf8("Akció mértéke"));
    model->setHeaderData(17, Qt::Horizontal, QObject::trUtf8("WebX5 id"));
//    model->setHeaderData(18, Qt::Horizontal, QObject::trUtf8("Nettó ár"));

    //model->setEditStrategy(QSqlTableModel::OnManualSubmit);
    model->setEditStrategy(QSqlTableModel::OnFieldChange);
    model->select();
    while(model->canFetchMore()) {
        //qDebug() << "fecthing more";
        model->fetchMore();
    }

    //importFPI();
}

QTableView* ItemNumberSearch::createView(QSortFilterProxyModel *model)
{
    //QTableView *view = new QTableView;
    //view = new QTableView;

    itemTableView->setModel(model);
    itemTableView->setItemDelegate(new QSqlRelationalDelegate(itemTableView));
    itemTableView->setColumnHidden(0, true);
    //itemTableView->setColumnHidden(6, true);
    itemTableView->setColumnHidden(7, true);
    itemTableView->setColumnHidden(10, true);
    itemTableView->setColumnHidden(11, true);
    itemTableView->setColumnHidden(12, true);
    itemTableView->setColumnHidden(14, true);
    itemTableView->setColumnHidden(17, true);

    //itemTableView->sortByColumn(1, Qt::AscendingOrder);

    itemTableView->setSelectionBehavior(QAbstractItemView::SelectRows);

    itemTableView->verticalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    itemTableView->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);


   // static int offset = 0;

    //view->setWindowTitle(title);
   // view->move(100 + offset, 100 + offset);
   // offset += 20;

    //view->setMinimumHeight(view->sizeHint().height());
    //view->setMinimumWidth(view->sizeHint().width());
    qDebug() << itemTableView->sizeHint().width();
    itemTableView->setMinimumWidth(750);
    itemTableView->setMinimumHeight(500);
    //itemTableView->adjustSize();
    //view->setEditTriggers(QAbstractItemView::NoEditTriggers);       // disable editing

    //view->show();

    return itemTableView;
}

void ItemNumberSearch::searchInDatabase()
{
    if(searchField->text().isEmpty()) {
        proxyModel->setFilterRegExp(QRegExp(""));
    }
    else if(!exactMatchCheckBox->isChecked()) {
        proxyModel->setFilterRegExp(QRegExp(searchField->text()));
    }
    else {
        proxyModel->setFilterRegExp(QRegExp("^" + searchField->text() + "$"));
    }
    proxyModel->setFilterCaseSensitivity(Qt::CaseInsensitive);
    //proxyModel->setFilterKeyColumn(1);

    updateSearchMatchCountLabel();
}

void ItemNumberSearch::updateSearchMatchCountLabel()
{
    if(searchField->text().isEmpty()) {
        searchMatchCountLabel->clear();
    }
    else {
        searchMatchCountLabel->setText(QString::number(proxyModel->rowCount()).append(" db találat"));
    }
}

void ItemNumberSearch::onOnlineSearchCheckBoxStateChanged()
{
    if(onlineSearchCheckBox->isChecked()) {
        qDebug() << "online search set";
        searchButton->setEnabled(false);
        connect(searchField, SIGNAL(textChanged(QString)), this, SLOT(searchInDatabase()));
        disconnect(searchButton, SIGNAL(clicked()), this, SLOT(searchInDatabase()));
        disconnect(searchField, SIGNAL(textEmpty()), this, SLOT(searchInDatabase()));
    }
    else {
        searchButton->setEnabled(true);
        disconnect(searchField, SIGNAL(textChanged(QString)), this, SLOT(searchInDatabase()));
        connect(searchButton, SIGNAL(clicked()), this, SLOT(searchInDatabase()));
        connect(searchField, SIGNAL(textEmpty()), this, SLOT(searchInDatabase()));
    }
}

void ItemNumberSearch::exactMatchChecked()
{
    if(exactMatchCheckBox->isChecked()) {
        onlineSearchCheckBoxPrevValue = onlineSearchCheckBox->isChecked();
        //onlineSearchCheckBox->setCheckable(false);
        onlineSearchCheckBox->setDisabled(true);
        onlineSearchCheckBox->setChecked(false);
        disconnect(searchField, SIGNAL(textChanged(QString)), this, SLOT(searchInDatabase()));
        connect(searchButton, SIGNAL(clicked()), this, SLOT(searchInDatabase()));
    }
    else {
        onlineSearchCheckBox->setEnabled(true);
        onlineSearchCheckBox->setChecked(onlineSearchCheckBoxPrevValue);
        connect(searchField, SIGNAL(textChanged(QString)), this, SLOT(searchInDatabase()));
        disconnect(searchButton, SIGNAL(clicked()), this, SLOT(searchInDatabase()));
    }
}

void ItemNumberSearch::importFromTxt()
{
    qDebug() << "Importing begin";

    QFile file("in.txt");
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        return;

    QTextStream in(&file);

    // just for coding and testing
    QString ownItemNumber =     "1000";
    QString foreignItemNumber = "2000";
    QString name =              "test";
    QString description =       "test desc.";
    qint8 packageUnit =         1;
    qint8 minimumOrderQuantiy = 1;
    qint8 link =                1;
    QString price =             "0 Ft";

    //int i = 0, j = 0;

    QHash<int, QString> categoryHash;

    QSqlQueryModel *categoryModel = new QSqlQueryModel;
    categoryModel->setQuery("SELECT * FROM link");
    for(int i=0; i< categoryModel->rowCount(); ++i) {
        QSqlRecord record = categoryModel->record(i);
        //qDebug() << record.value("id").toInt();
        //qDebug() << record.value("link").toString();
        categoryHash[record.value("id").toInt()] = record.value("link").toString();
    }

    while (!in.atEnd()) {
        QString line = in.readLine();

        if(line.trimmed().isEmpty()) continue;

        QStringList dataList = line.split('\t');

        QString temp = dataList[3];
        QStringList lineElements = temp.split('-');
        QString desc = "- ";
        for(int i=1; i<lineElements.count(); ++i) {
            desc.append(QString(lineElements[i]).trimmed());
            if(i!=lineElements.count()-1) {
                desc.append('\n');
                desc.append("- ");
            }
        }


        ownItemNumber       = dataList[1];
        foreignItemNumber   = dataList[2];
        name                = QString(lineElements[0]).trimmed();
        description         = desc;
        link                = categoryHash.key(dataList[0]);
        packageUnit         = 1;
        minimumOrderQuantiy = 1;
        price               = QString(dataList[4]).trimmed();

/*
        i++;
        if(i > 46 && i < 52) {
            qDebug() << "i:";
            qDebug() << link;
            qDebug() << ownItemNumber;
            qDebug() << foreignItemNumber;
            qDebug() << name;
            qDebug() << description;
            qDebug() << price;

        }
*/
        //qDebug() << "new line:";

        //test
/*
        QString ownItemNumber =     "1000";
        QString foreignItemNumber = "2000";
        QString name =              "test";
        QString description =       "test desc.";
        qint8 packageUnit =         1;
        qint8 minimumOrderQuantiy = 1;
        qint8 link =                1;
*/
        QSqlRecord record;

        QSqlField f1("own_item_number", QVariant::String);
        QSqlField f2("foreign_item_number", QVariant::String);
        QSqlField f3("name", QVariant::String);
        QSqlField f4("description", QVariant::String);
        QSqlField f5("package_unit", QVariant::Int);
        QSqlField f6("minimal_order_quantity", QVariant::Int);
        QSqlField f7("link", QVariant::Int);
        QSqlField f8("price", QVariant::String);

        f1.setValue(QVariant(ownItemNumber));
        f2.setValue(QVariant(foreignItemNumber));
        f3.setValue(QVariant(name));
        f4.setValue(QVariant(description));
        f5.setValue(QVariant(packageUnit));
        f6.setValue(QVariant(minimumOrderQuantiy));
        f7.setValue(QVariant(link));
        f8.setValue(QVariant(price));

        record.append(f1);
        record.append(f2);
        record.append(f3);
        record.append(f4);
        record.append(f5);
        record.append(f6);
        record.append(f7);
        record.append(f8);

        if( myModel->insertRecord(-1, record)) qDebug() << "ROW INSERTED";
        else qDebug()<<"FAIL!";  // always returns FAIL!
    }

    myModel->select();

    return;
}

bool ItemNumberSearch::importCSV()
{
    // TODO: add a progress bar window
    qDebug() << "\nBegin importing from CSV.";

    int numberOfRecords = 0;
    int numberOfRecordsInserted = 0;
    int numberOfRecordsUpdated = 0;
    int numberOfErrors = 0;
    int id = 1;

    if(myModel->rowCount() != 0) { // TODO: verify! - generateNewProductId?
        id = myModel->record(myModel->rowCount() - 1).value("id").toInt() + 1;
    } // else id remains 1

//    Open File Dialog - OK
    QString csvFileName = QFileDialog::getOpenFileName(this, trUtf8("CSV fájl megnyitása"),
                                                       "", trUtf8("Fájlok (*.csv)"));

    //QString csvFileName("F:/Projects/BiSz/Source/bipsz.csv");
    if(csvFileName.isEmpty()) return false;
    qDebug() << "Opening" << csvFileName << "...";
    QFile csvFile(csvFileName);

    if (!csvFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qDebug() << "Cannot open CSV file";
        return false;
    }

    qDebug() << "CSV file opened";

    QTextStream in(&csvFile);

    if(myDatabase->db.tables().isEmpty()) {
        myDatabase->createTable("product", myDatabase->tableProductFieldList,
                                           myDatabase->tableProductFieldHash);
        myDatabase->createTable("category", myDatabase->tableCategoryFieldList,
                                            myDatabase->tableCategoryFieldHash);
    }

//  Headers for Debug
//    TODO: check header!
    QString line0 = in.readLine();
    QStringList csvHeaderList = line0.split(';');


    while (!in.atEnd()) {
//    for(int i=0; i<1050; ++i)
//    int l = 0;
//    while(l < 3) {
        QHash<QString, QString> productHash;
        int categoryNumber;

//        for(int i=1; i<1053; ++i) {
//            in.readLine();
//        }
//        l++;

        QString line = in.readLine().toUtf8();
        if(line.trimmed().isEmpty()) continue;

        if(!line.contains(';')) return false;

        QStringList dataList = line.split(';');

        bool isCategoryExist = false;
        for(int j=0; j<myDatabase->categoryList.length(); ++j) {
            if(dataList.value(1) == myDatabase->categoryList.at(j)) {
                isCategoryExist = true;
                break;
            }
        }
        if(isCategoryExist) {
            categoryNumber = myDatabase->categoryList.indexOf(dataList.value(1)) + 1;
        }
        else {
            myDatabase->addNewCategory(dataList.value(1));
            categoryNumber = myDatabase->categoryList.count();
        }

        productHash.insert("web_id", dataList.value(0));
        productHash.insert("id", QString::number(id));
        productHash.insert("category_number", QString::number(categoryNumber));
        productHash.insert("own_product_id", dataList[2].split(" - ").value(0).trimmed());
        //productHash.insert("foreign_item_id", "none");
        //productHash.insert("name", dataList[2].split(" - ").value(1).trimmed());
        productHash.insert("name", dataList[2].remove(0, dataList[2].indexOf("-", 0) + 2).trimmed());

        // Modifying description
        QString descriptionAssembly;
        QStringList descriptionLines = dataList[3].split("\\n");
        for(int m=0; m<descriptionLines.count(); ++m) {
            if(!descriptionLines[m].isEmpty()) {
                descriptionAssembly.append(QString(descriptionLines[m]).trimmed());
            }
            if( m+1 != descriptionLines.count()) descriptionAssembly.append('\n');
        }

        productHash.insert("description", descriptionAssembly);
        productHash.insert("price", dataList[4].replace(',','.'));
        productHash.insert("enable_vat", dataList[5]);
        productHash.insert("vat_value", dataList[6]);
        productHash.insert("weight", dataList[7].replace(',','.'));
        productHash.insert("options", dataList[8]);
        productHash.insert("enable_discounts", dataList[9]);
        productHash.insert("discounts", dataList[10]);


/*      Sample modified readin from CSV
        qDebug() << "Product:";
        QHashIterator<QString, QString> productHashIterator(productHash);

        while(productHashIterator.hasNext()) {
            productHashIterator.next();
            qDebug() << productHashIterator.key() << "\t\t" << productHashIterator.value();
        }
*/

        // Search for match
        myModel->setFilter(QString("own_product_id = ").append("'").append(
                               productHash.value("own_product_id")).append("'"));

        // No match -> new product
        if(myModel->rowCount() == 0) {
//            qDebug() << "Adding new product with id =" << id;
            QSqlRecord productRecord;// = createEmptyProductRecord();

            for(int k=0; k<myDatabase->tableProductFieldList.length(); ++k) {
                productRecord.append(QSqlField(myDatabase->tableProductFieldList.at(k),
                                               QVariant::String));
                productRecord.setValue(myDatabase->tableProductFieldList.at(k),
                                       QVariant(productHash.value(
                                                    myDatabase->tableProductFieldList.at(k))));
            }


//            Setting default values for non-existing CSV fields
//            TODO: optimize this
            productRecord.setValue("purchase_price", "0");
            productRecord.setValue("is_active_product", "1");
            productRecord.setValue("minimal_order_quantity", "1");
            productRecord.setValue("foreign_product_id", "nincs cksz. megadva");
            productRecord.setValue("package_unit", "1");
            myModel->setTable("product");

            bool result = myModel->insertRecord(-1, productRecord);

            // TODO: find out why simle insertRecord result in missing values!
            myModel->setFilter(QString("own_product_id = ").append("'").append(
                                   productHash.value("own_product_id")).append("'"));
            myModel->setRecord(0, productRecord);
            // endTODO

            if(result) {
                numberOfRecordsInserted++;
                //qDebug() << "Product inserted:";// << myModel->record(0);
            }
            else {
                qDebug() << "Error at product id =" << productHash.value("own_product_id");
                qDebug() << myModel->lastError();
                numberOfErrors++;
                id--;
            }
        }

//        Product already exists
        else if(myModel->rowCount() == 1) {
//            qDebug() << "match in database, Product id =" << productHash.value("own_product_id");

            QSqlRecord existingProductRecord = myModel->record(0);

            /*qDebug() << "existingProductRecord fields:";
            for(int i=0; i<existingProductRecord.count(); ++i) {
                qDebug() << existingProductRecord.field(i);
            }
            */

            bool hasRecordChanged = false;

            for(int field = 0; field < myDatabase->csvImportFieldList.length(); ++field) {
                QString fieldName = myDatabase->csvImportFieldList.at(field);

                // TODO: simlify this!!
                if(fieldName == "category_number") {
                    QString oldCategory = myDatabase->categoryList.at(
                                          existingProductRecord.value("category_number").toInt()).trimmed();
                    QString newCategory = myDatabase->categoryList.at(
                                          productHash.value("category_number").toInt()-1).trimmed();

                    if(oldCategory == newCategory) continue;
                    else {
                        hasRecordChanged = true;
                        existingProductRecord.setValue("category", productHash.value("category_number"));

                        continue;
                    }
                }
                //QSqlField currentField = QSqlField(fieldName, QVariant::String);
                QSqlField currentField = existingProductRecord.field(fieldName);
                currentField.setValue(productHash.value(myDatabase->csvImportFieldList.at(field)));

                if(currentField.value() != existingProductRecord.value(fieldName)) {
                    //qDebug() << fieldName;
                    //qDebug() << currentField.value();
                    //qDebug() << existingProductRecord.value(fieldName);
                    existingProductRecord.replace(existingProductRecord.indexOf(
                                                  myDatabase->csvImportFieldList.at(field)), currentField);
                    hasRecordChanged = true;
                }
            }

            if(hasRecordChanged) {
                bool result = myModel->setRecord(0, existingProductRecord);
                if(result) {
                    id--;
                    if(hasRecordChanged) numberOfRecordsUpdated++;
                }
                else {
                    // BUG: sql error even on successful insert
                    numberOfErrors++;
                    qDebug() << myModel->lastError();
                    qDebug() << existingProductRecord;
                }
            }

            numberOfRecords++;
        }
        else {
            numberOfErrors++;
            qDebug() << "ERROR !!!!!!";
        }

        /*Test value assignment
        QSqlRecord record = myModel->record();
        QSqlField f1 = record.field(productHash.value("own_product_id"));
        f1.setValue(QVariant(productHash.value("own_product_id")));
        qDebug() << f1.value();
*/

        /*Test insert
        QSqlRecord record;
        QSqlField f1("own_product_id", QVariant::String);
        f1.setValue(QVariant("0000"));

        record.append(f1);
        if(!myModel->insertRecord(-1, record)) qDebug() << myModel->lastError();
*/

        id++;

        myModel->submitAll();
        myModel->select();
        updateProductCount();

        //myModel->setFilter("");
        //myModel->select();

        /*Filling categoryListDump
        bool match = false;
        for(int j=0; j<myDatabase->categoryListDump.length(); ++j) {
            if( myDatabase->categoryListDump.value(j) == dataList[1] ) {
                match = true;
                break;
            }
        }
        if(match == false) {
            myDatabase->categoryListDump.append(dataList[1]);
            qDebug() << "new category:" << myDatabase->categoryListDump.value(i);
        }
        */

        /*qDebug() << any input value
        for(int i=0; i<dataList.count(); ++i) {
            qDebug() << csvHeaderList.value(i);
            qDebug() << dataList.value(i);
        }
*/


    }

    setupModel(myModel);
    myDatabase->createDatabaseGlobals();
    updateProductCount();


    /*
        QHash<int, QString> categoryHash;

        QSqlQueryModel *categoryModel = new QSqlQueryModel;
        categoryModel->setQuery("SELECT * FROM link");
        for(int i=0; i< categoryModel->rowCount(); ++i) {
            QSqlRecord record = categoryModel->record(i);
            //qDebug() << record.value("id").toInt();
            //qDebug() << record.value("link").toString();
            categoryHash[record.value("id").toInt()] = record.value("link").toString();
        }
    */


//    myModel->select();

    qDebug() << numberOfRecordsInserted << "record inserted.";
    qDebug() << numberOfRecordsUpdated << "record updated.";
    qDebug() << numberOfErrors << "errors.";

    QMessageBox msgBox;
    msgBox.setIcon(QMessageBox::Information);
    msgBox.setWindowTitle("BiSz - Importálás napló");
    msgBox.setText("A külső adatok importálása befejeződött");
    QString informationText;
    informationText.append("Az importált CSV ");
    informationText.append(QString::number(numberOfRecords));
    informationText.append("db terméket tartalmazott.\n\n");
    informationText.append(QString::number(numberOfRecordsInserted));
    informationText.append(" db új rekord.\n");
    informationText.append(QString::number(numberOfRecordsUpdated));
    informationText.append(" db frissített rekord.\n");
    //informationText.append(QString::number(numberOfErrors));
    //informationText.append(" db hiba.\n");
    msgBox.setInformativeText(informationText);
    msgBox.setStandardButtons(QMessageBox::Ok);
    msgBox.setDefaultButton(QMessageBox::Ok);
    msgBox.exec();

    return true;
}

int ItemNumberSearch::generateNewId()
{
    int newId = 1;

    QSqlTableModel model;
    model.setTable("product");
    model.select();

    while(model.canFetchMore()) model.fetchMore();
    newId = model.record(model.rowCount()-1).value("id").toInt() + 1;

    return newId;
}

QString ItemNumberSearch::generateNewProductId()
{
    int newProductIdInt = 0;
    QString newProductId;
    QSqlTableModel model;

    model.setTable("product");
    model.select();

    // TODO: why the below isn't working?
    /*
    while(model.canFetchMore()) model.fetchMore();
    model.sort(1, Qt::AscendingOrder);
    int newProductIdInt = model.record(model.rowCount()-1).value("own_product_id").toInt() + 1;
    newProductId.setNum(newProductIdInt);
    model.sort(0, Qt::AscendingOrder);
    */

    while(model.canFetchMore()) model.fetchMore();

    qDebug() << model.rowCount();

    int currentProductId;
    for(int i=0; i<model.rowCount(); ++i) {
        currentProductId = model.record(i).value("own_product_id").toInt();
        if(currentProductId > newProductIdInt) newProductIdInt = currentProductId;
    }
    newProductIdInt++;
    newProductId.setNum(newProductIdInt);

    qDebug() << newProductIdInt;

    return newProductId;
}

QSqlRecord ItemNumberSearch::createEmptyProductRecord()
{
    QSqlRecord tmpRecord;

    tmpRecord.append(QSqlField("id", QVariant::Int));
    tmpRecord.append(QSqlField("own_product_id", QVariant::String));
    tmpRecord.append(QSqlField("foreign_product_id", QVariant::String));
    tmpRecord.append(QSqlField("name", QVariant::String));
    tmpRecord.append(QSqlField("description", QVariant::String));
    tmpRecord.append(QSqlField("category_number", QVariant::Int));
    tmpRecord.append(QSqlField("package_unit", QVariant::Int));
    tmpRecord.append(QSqlField("minimal_order_quantity", QVariant::Int));
    tmpRecord.append(QSqlField("price", QVariant::Double));
    tmpRecord.append(QSqlField("purchase_price", QVariant::Double));
    tmpRecord.append(QSqlField("is_active_product", QVariant::Int));
    tmpRecord.append(QSqlField("enable_vat", QVariant::Int));
    tmpRecord.append(QSqlField("vat_value", QVariant::Int));
    tmpRecord.append(QSqlField("weight", QVariant::Double));
    tmpRecord.append(QSqlField("options", QVariant::String));
    tmpRecord.append(QSqlField("enable_discounts", QVariant::Int));
    tmpRecord.append(QSqlField("discounts", QVariant::Int));
    tmpRecord.append(QSqlField("web_id", QVariant::String));

    return tmpRecord;
}

bool ItemNumberSearch::exportCSV()
{
    bool result = false;

    QString csvExportFileName = QFileDialog::getSaveFileName(this, trUtf8("CSV mentése"),
                                                           "", trUtf8("Fájlok (*.csv)"));
    QString exportData;

    exportData.append(QChar( QChar::ByteOrderMark ));

    for(int i=0; i<myDatabase->csvExportFieldList.count(); ++i) {
        QString csvFieldName = myDatabase->csvExportFieldList.at(i);
        csvFieldName.replace("_", " ");
        exportData.append(csvFieldName);
        if(i + 1 < myDatabase->csvExportFieldList.count()) {
            exportData.append(";");
        }
    }

    exportData.append("\n");

    for(int i=0; i<myModel->rowCount(); ++i) {
//    for(int i=0; i<10; ++i) {
        QSqlRecord currentRecord = myModel->record(i);

        exportData.append(currentRecord.value("web_id").toString());
        exportData.append(";");
//        exportData.append(myDatabase->categoryList.at(
//                              currentRecord.value("category_number").toInt()));
        exportData.append(currentRecord.value("category").toString());
        exportData.append(";");
        exportData.append(currentRecord.value("own_product_id").toString());
        exportData.append(" - ");
        exportData.append(currentRecord.value("name").toString());
        exportData.append(";");
        exportData.append(currentRecord.value("description").toString().replace('\n', "\\n"));
        exportData.append(";");
        exportData.append(currentRecord.value("price").toString().replace(" Ft", "").replace(".",","));
        exportData.append(";");
        exportData.append(currentRecord.value("enable_vat").toString());
        exportData.append(";");
        exportData.append(currentRecord.value("vat_value").toString());
        exportData.append(";");
        //weight field: replaced to price as requested
//        exportData.append(currentRecord.value("weight").toString());
        exportData.append(currentRecord.value("price").toString().replace(".",","));
        exportData.append(";");
        exportData.append(currentRecord.value("options").toString());
        exportData.append(";");
        exportData.append(currentRecord.value("enable_discounts").toString());
        exportData.append(";");
        // TODO: discounts has a special storing form, implement it, if neccessary
        //exportData.append(currentRecord.value("discounts").toString());
        exportData.append('\n');
    }

/*        not a good solution
        for(int j=0; j<myDatabase->csvExportFieldList.count(); ++j) {

            if(currentRecord.fieldName(j) != "category_number") {
                exportData.append(currentRecord.value(myDatabase->csvFieldList.at(j)).toString());
            }
            else {
                exportData.append(myDatabase->categoryList.at(
                                      currentRecord.value(myDatabase->csvFieldList.at(j)).toInt()
                                  ));
            }
            exportData.append(";");
        }

        exportData.append("\n");
    }
*/

    QFile file;
    file.setFileName(csvExportFileName);
    if(file.exists()) file.remove();
    if(!file.open(QIODevice::WriteOnly | QIODevice::Text)) return false;

    QByteArray exportByteArray;
    exportByteArray.append(exportData.toUtf8());
    result = file.write(exportByteArray);

    file.flush();
    file.close();

    return result;
}

bool ItemNumberSearch::exportToAccountingApp()
{
    bool result = false;

    QString csvExportFileName = QFileDialog::getSaveFileName(this, trUtf8("CSV mentése"),
                                                           "", trUtf8("Fájlok (*.csv)"));
    QString exportData;

    exportData.append(QChar( QChar::ByteOrderMark ));

    exportData.append("Cikkszám;Termék-Leírás;Ár\n");

    for(int i=0; i<myModel->rowCount(); ++i) {
        QSqlRecord currentRecord = myModel->record(i);

        exportData.append(currentRecord.value("own_product_id").toString());
        exportData.append(";");
        exportData.append(currentRecord.value("name").toString());
        exportData.append(" - \"");
        exportData.append(currentRecord.value("description").toString().replace("\n- ", ", ").replace(
                              "- ", "").replace('\n', " "));
        exportData.append("\"");
        exportData.append(";");
        exportData.append(currentRecord.value("price").toString().replace(" Ft", "").replace(".",","));
        exportData.append('\n');
    }

    QFile file;
    file.setFileName(csvExportFileName);
    if(file.exists()) file.remove();
    if(!file.open(QIODevice::WriteOnly | QIODevice::Text)) return false;

    QByteArray exportByteArray;
    exportByteArray.append(exportData.toUtf8());
    result = file.write(exportByteArray);

    file.flush();
    file.close();

    return result;
}


void ItemNumberSearch::on_optionsButton_clicked()
{
    if(optionsDialog->exec()) {
        optionsDialog->saveSettings(appSettings);
        qDebug() << "beállítások mentve";
    }
    else {
        qDebug() << "beállítások elvetve";
    }
}

bool ItemNumberSearch::on_exportToPrintButton_clicked()
{
    bool result = false;

    QString csvExportFileName = QFileDialog::getSaveFileName(this, trUtf8("CSV mentése"),
                                                           "", trUtf8("Fájlok (*.csv)"));
    QString exportData;

    exportData.append(QChar( QChar::ByteOrderMark ));

    exportData.append("Cikkszám;");
    exportData.append("Idegen cikkszám");
    exportData.append("Név;");
    exportData.append("Kategória;");
    exportData.append("Leírás;");
    exportData.append("Beszerzési ár (HUF);");
    exportData.append("Ár (HUF);");
    exportData.append("Haszon;");

    exportData.append("\n");

    for(int i=0; i<myModel->rowCount(); ++i) {
//    for(int i=0; i<10; ++i) {
        QSqlRecord currentRecord = myModel->record(i);

        double price = currentRecord.value("price").toString().toDouble();
        double purchase_price = currentRecord.value("purchase_price").toString().toDouble();
        double profit = price/purchase_price * 100 - 100;

        exportData.append(currentRecord.value("own_product_id").toString());
        exportData.append(";");
        exportData.append(currentRecord.value("foreign_product_id").toString());
        exportData.append(";");
        exportData.append(currentRecord.value("name").toString());
        exportData.append(";");
        exportData.append(currentRecord.value("category").toString().split(" > ").at(0));
        exportData.append(";");
        exportData.append(currentRecord.value("description").toString().replace('\n', ", "));
        exportData.append(";");
        exportData.append(currentRecord.value("purchase_price").toString().replace(".",","));
        exportData.append(";");
        exportData.append(currentRecord.value("price").toString().replace(" Ft", "").replace(".", ","));
        exportData.append(";");
        exportData.append(QString::number(profit, 'f' , 2).append(" %"));
        exportData.append(";");
        //exportData.append(currentRecord.value("weight").toString());
        //exportData.append(";");
        exportData.append('\n');
    }

    QFile file;
    file.setFileName(csvExportFileName);
    if(file.exists()) file.remove();
    if(!file.open(QIODevice::WriteOnly | QIODevice::Text)) return false;

    QByteArray exportByteArray;
    exportByteArray.append(exportData.toUtf8());
    result = file.write(exportByteArray);

    file.flush();
    file.close();

    return result;
}

void ItemNumberSearch::reconnectProfitMarginSignal()
{
    if(productDialog->purchasePriceSpinBox->value()) {
        productDialog->disconnect(productDialog->profitMarginSpinBox, SIGNAL(editingFinished()),
                                  this,              SLOT(reconnectProfitMarginSignal()));
        productDialog->connect(productDialog->profitMarginSpinBox, SIGNAL(valueChanged(double)),
                               productDialog,        SLOT(on_profitMarginSpinBox_valueChanged()));
        connected = true;
    }
}

void ItemNumberSearch::showAboutDialog()
{
    //qDebug() << "about slot called";
    qApp->aboutQt();
}

void ItemNumberSearch::on_addButton_clicked()
{   
    productDialog->setWindowTitle(trUtf8("BiSz - Új termék hozzáadása"));
    productDialog->okButton->setText(trUtf8("Hozzáadás"));
    productDialog->cancelButton->setText(trUtf8("Mégse"));

 /*
    // disconnecting certain signals
    productDialog->disconnect(productDialog->profitMarginSpinBox, SIGNAL(valueChanged(double)),
                              productDialog, SLOT(on_profitMarginSpinBox_valueChanged()));
    productDialog->connect(productDialog->profitMarginSpinBox, SIGNAL(editingFinished()),
                           this,                SLOT(reconnectProfitMarginSignal()));
    connected = false;
*/

    // Setting up default product field values
    productDialog->productNameLineEdit->clear();
    productDialog->ownProductIdLineEdit->setText(generateNewProductId());
    productDialog->ownProductIdLineEdit->setDisabled(true);
    productDialog->foreignProductIdLineEdit->clear();
    //productDialog->purchasePriceSpinBox->clear();
    productDialog->descriptionTextEdit->clear();

    productDialog->categoryListModel.setStringList(myDatabase->categoryListLevel_0);
    productDialog->categoryComboBox->setCurrentIndex(
                    productDialog->categoryListModel.stringList().count()-1);
    productDialog->categorySub1ComboBox->setHidden(true);
    productDialog->categorySub2ComboBox->setHidden(true);

    productDialog->packageUnitLineEdit->clear();
    //productDialog->weightSpinBox->clear();
    productDialog->weightSpinBox->setValue(0);
    //productDialog->priceSpinBox->clear();

    productDialog->purchasePriceSpinBox->setValue(0);
    productDialog->profitMarginSpinBox->setValue(70);
    productDialog->priceSpinBox->setValue(0);
    //productDialog->discountValueSpinBox->clear();
    productDialog->discountValueSpinBox->setValue(0);
    productDialog->discountCheckBox->setChecked(false);
    emit productDialog->discountCheckBox->toggled(false);

    productDialog->clearErrorDescriptionLabel();

    // Setting "Új termék" as default product category
    //productDialog->categoryComboBox->setCurrentIndex(188);
                //myDatabase->categoryListLevel_0.);

    //productDialog->categorySub1ListModel.setStringList(QStringList());
    //productDialog->categorySub2ListModel.setStringList(QStringList());

    if(productDialog->exec()) {
        // qDebug() << "Add exec";

        // TODO: test if you can add new product to an empty database
        QSqlRecord newProductRecord = myModel->record(0);

        //qDebug() << newProductRecord;

        newProductRecord.setValue("id", generateNewId());
        newProductRecord.setValue("name", productDialog->productNameLineEdit->text());
        newProductRecord.setValue("own_product_id", productDialog->ownProductIdLineEdit->text());
        newProductRecord.setValue("foreign_product_id",
                                  productDialog->foreignProductIdLineEdit->text());
        newProductRecord.setValue("purchase_price", productDialog->purchasePriceSpinBox->value());
        newProductRecord.setValue("description",
                        productDialog->descriptionTextEdit->document()->toPlainText());
        newProductRecord.setValue("package_unit", productDialog->packageUnitLineEdit->text());
        newProductRecord.setValue("weight", productDialog->weightSpinBox->value());
        newProductRecord.setValue("price", productDialog->priceSpinBox->value());
        newProductRecord.setValue("enable_discounts",
                                  int(productDialog->discountCheckBox->isChecked()));
        newProductRecord.setValue("discounts", productDialog->discountValueSpinBox->value());
        newProductRecord.setValue("web_id", "");

        QString category = productDialog->categoryListModel.stringList().at(
                            productDialog->categoryComboBox->currentIndex());

        int category1Lenght = productDialog->categorySub1ListModel.stringList().length();

        if(category1Lenght) {
            QString category1 = productDialog->categorySub1ListModel.stringList().at(
                                    productDialog->categorySub1ComboBox->currentIndex());

            if(category1 != "") {
                category.append(" > ");
                category.append(category1);

                int category2Lenght = productDialog->categorySub2ListModel.stringList().length();

                if(category2Lenght) {
                    QString category2 = productDialog->categorySub2ListModel.stringList().at(
                                        productDialog->categorySub2ComboBox->currentIndex());

                    if(category2 != "")
                    {
                        category.append(" > ");
                        category.append(category2);
                    }
                }
            }
        }

        /*newProductRecord.insert(newProductRecord.indexOf("category"), QSqlField("category_number",
                                                                                QVariant::Int));
                                                                                */
        newProductRecord.setValue("category", myDatabase->categoryList.indexOf(category) + 1);
        //newProductRecord.remove(newProductRecord.indexOf("category"));
        //qDebug() << "category: " << category;
        //qDebug() << "category number: " << myDatabase->categoryList.indexOf(category) + 1;

        //myModel->submitAll();

        //qDebug() << "New \n \n";
        //qDebug() << newProductRecord;

//        BUG: QSqlError(-1, "", "")
        if( myModel->insertRecord(-1, newProductRecord) ) qDebug() << "New record Added.";
        else {
            qDebug() << "Failed to add new record!";
            qDebug() << myModel->lastError();
        }

        if(!myModel->submitAll()) {
            qDebug() << "Model error";
            qDebug() << myModel->lastError();
        }

        myModel->select();
        while(myModel->canFetchMore())
            myModel->fetchMore();
        itemTableView->scrollToBottom();

        /*
        if(!connected) {
            productDialog->disconnect(productDialog->profitMarginSpinBox, SIGNAL(editingFinished()),
                                      this,              SLOT(reconnectProfitMarginSignal()));
            productDialog->connect(productDialog->profitMarginSpinBox, SIGNAL(valueChanged(double)),
                                   productDialog,        SLOT(on_profitMarginSpinBox_valueChanged()));
        }
        */

        updateProductCount();
        //myModel->select();
    }
}

void ItemNumberSearch::on_modifyButton_clicked()
{
    // Storing current row and column
    QModelIndex selectedIndex = itemTableView->selectionModel()->currentIndex();
    int row = selectedIndex.row();
    int column = selectedIndex.column();

    // TODO: idegen cikkszám egyezés vizsgálata
    productDialog->setWindowTitle(trUtf8("BiSz - Termék adatainak módosítása"));
    productDialog->okButton->setText(trUtf8("Módosítás"));
    productDialog->cancelButton->setText(trUtf8("Mégse"));
    productDialog->ownProductIdLineEdit->setDisabled(true);
    productDialog->errorDescriptionLabel->clear();

    updateCurrentRecordData();

/*    Print out current record
    for(int i=0; i<currentRecordData.count(); ++i) {
        qDebug() << currentRecordData.value(i);
    }
*/

    productDialog->productNameLineEdit->setText(currentRecordData.value(3));
    productDialog->ownProductIdLineEdit->setText(currentRecordData.value(1));
    productDialog->foreignProductIdLineEdit->setText(currentRecordData.value(2));
    productDialog->purchasePriceSpinBox->setValue(currentRecordData.value(9).toDouble());
    productDialog->descriptionTextEdit->setText(currentRecordData.value(4));
    productDialog->packageUnitLineEdit->setText(currentRecordData.value(6));

//     Category
    QStringList categoryParts;
    QString currentCategory;
    QString currentCategorySub1;
    QString currentCategorySub2;

    categoryParts = currentRecordData.value(5).split(" > ");
    currentCategory = categoryParts.at(0);

    productDialog->categoryListModel.setStringList(myDatabase->categoryListLevel_0);
    productDialog->categoryComboBox->setCurrentIndex(
                                        myDatabase->categoryListLevel_0.indexOf(currentCategory));
    if(categoryParts.length() > 1) {
        currentCategorySub1 = categoryParts.at(1);
        productDialog->categorySub1ComboBox->setCurrentIndex(
                        productDialog->categorySub1ListModel.stringList().indexOf(
                            currentCategorySub1));
    }
    if(categoryParts.length() > 2) {
        currentCategorySub2 = categoryParts.at(2);
        productDialog->categorySub2ComboBox->setCurrentIndex(
                        productDialog->categorySub2ListModel.stringList().indexOf(
                            currentCategorySub2));
    }

//    Category

    productDialog->weightSpinBox->setValue(currentRecordData.value(13).toDouble());

    productDialog->priceSpinBox->setValue(currentRecordData.value(8).toDouble());

    productDialog->discountValueSpinBox->setValue(currentRecordData.value(16).toInt());
    productDialog->discountCheckBox->setChecked(bool(currentRecordData.value(15).toInt()));

//    emitting signals

    emit productDialog->discountCheckBox->toggled(productDialog->discountCheckBox->isChecked());
    emit productDialog->purchasePriceSpinBox->editingFinished();

//    Dialog exec

    if(productDialog->exec()) {
        QSqlTableModel model;
        model.setTable("product");
        QString temp = currentRecordData.value(0).toUtf8();
        QString filter("id = ");
        filter.append(temp);
        model.setFilter(filter);
        model.select();
        if(model.rowCount() == 1) {
            QSqlRecord record = model.record(0);

            record.setValue("name", productDialog->productNameLineEdit->text());
            record.setValue("foreign_product_id", productDialog->foreignProductIdLineEdit->text());
            record.setValue("purchase_price", productDialog->purchasePriceSpinBox->value());
            record.setValue("description",
                            productDialog->descriptionTextEdit->document()->toPlainText());

            QString category = productDialog->categoryListModel.stringList().at(
                                productDialog->categoryComboBox->currentIndex());

            if(productDialog->categorySub1ComboBox->currentIndex() >= 0) {
                category.append(" > ");
                category.append(productDialog->categorySub1ListModel.stringList().at(
                                    productDialog->categorySub1ComboBox->currentIndex()));
                if(productDialog->categorySub2ComboBox->currentIndex() >= 0) {
                    category.append(" > ");
                    category.append(productDialog->categorySub2ListModel.stringList().at(
                                        productDialog->categorySub2ComboBox->currentIndex()));
                }
            }

            record.setValue("category_number", myDatabase->categoryList.indexOf(category) + 1);
            record.setValue("package_unit", productDialog->packageUnitLineEdit->text());
            record.setValue("weight", productDialog->weightSpinBox->value());
            record.setValue("price", productDialog->priceSpinBox->value());
            record.setValue("enable_discounts", int(productDialog->discountCheckBox->isChecked()));
            record.setValue("discounts", productDialog->discountValueSpinBox->value());

            /*
            for(int i=0; i<record.count(); ++i) {
                qDebug() << record.value(i);
            }
            */

            model.setRecord(0, record);
            if(model.submitAll()) qDebug() << "Record Modified.";
            else qDebug() << "Failed to update record";

            myModel->select();
            while(myModel->canFetchMore()) {
                myModel->fetchMore();
            }

            selectedIndex = myModel->index(row, column);

            itemTableView->scrollTo(selectedIndex, QAbstractItemView::PositionAtCenter);
            itemTableView->selectRow(row);

        }
        else {
            qDebug() << "aaaa -.-";
        }
    }
}

void ItemNumberSearch::on_foreignProductIdEdited()
{
    // BUG: multiplie call
    bool match = false;
    QString currentForeignProductId = productDialog->foreignProductIdLineEdit->text().trimmed();
    QString foreignProductId = currentRecordData.value(2);

    QSqlQuery query("Select foreign_product_id FROM product");
    while(query.next()) {
        if(query.value(0) == currentForeignProductId
           && query.value(0) != foreignProductId) {
            qDebug() << "match";
            match = true;
        }
    }
    if(match) {
        productDialog->errorDescriptionLabel->setText(
                        trUtf8("Ilyen idegen cikkszám már szerepel az adatbázisban!"));
        productDialog->numberOfErrors++;
    }
    if(foreignProductId == currentForeignProductId) {
        qDebug() << "current id set";
        productDialog->errorDescriptionLabel->setText("");
        productDialog->repaint();
    }
}

void ItemNumberSearch::updateCurrentRecordData()
{
    QModelIndex currentIndex;
    QModelIndexList currentIndexList;

    currentIndexList = itemTableView->selectionModel()->selectedIndexes();

    currentRecordData.clear();

    for(int i=0; i<currentIndexList.count(); ++i) {
        currentIndex = currentIndexList.at(i);
        currentRecordData.insert(i, proxyModel->data(currentIndex).toString());
        //qDebug() << myQMap.value(i);
    }
}

void ItemNumberSearch::on_deleteButton_clicked()
{
    QMessageBox msgBox;
    msgBox.setIcon(QMessageBox::Question);
    msgBox.setWindowTitle("BiSz - Termék törlése");
    msgBox.setText("Biztosan törölni szeretné a terméket?");
    //msgBox.setInformativeText("Biztosan törölni szeretné a terméket?");
    msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
    msgBox.setDefaultButton(QMessageBox::No);
    int reply = msgBox.exec();

    if(reply == QMessageBox::Yes) {
        qDebug() << "törlés";

        updateCurrentRecordData();

        QSqlTableModel model;
        model.setTable("product");
        QString temp = currentRecordData.value(0).toUtf8();
        QString filter("id = ");
        filter.append(temp);
        model.setFilter(filter);
        model.select();
        if(model.rowCount() == 1) {
            model.removeRows(0, 1);
            if( model.submitAll() ) qDebug() << "törölve";
            else qDebug() << model.lastError();
        }
        else {
            qDebug() << "error: more items";
            qDebug() << currentRecordData.value(0);
        }

        QString previewImage(QString(QDir::currentPath()).append("/images/products/").append(
                                 currentRecordData.value(2)).append(".jpg"));

        if(QFile::exists(previewImage)) QFile::remove(previewImage);

        myModel->select();
        while (myModel->canFetchMore())
                myModel->fetchMore();
        updateProductCount();
    }
}

void ItemNumberSearch::on_itemTableView_selectionModel_currentChanged()
{
    //qDebug() << "SLOT called: on_itemTableView_selectionModel_currentChanged";
    //modifyButton->setEnabled(!selection.isEmpty());

    int selectionCount = itemTableView->selectionModel()->selection().indexes().count();
    int columnCount = proxyModel->columnCount();
    if(!optionsDialog->reducedControlCheckBox->isChecked()) {
        if(selectionCount == 0) {
            qmlModifyButton->setProperty("enabled", false);
        }
        else if(selectionCount/columnCount == 1) {
            qmlModifyButton->setProperty("enabled", true);
        }
        else {
            qmlModifyButton->setProperty("enabled", false);
        }

        if(selectionCount) {
            qmlDeleteButton->setProperty("enabled", true);
        }
        else {
            qmlDeleteButton->setProperty("enabled", false);
        }
    }

    //if(productPreviewCheckBox->isChecked()) {
    if(true) {
        //qDebug() << "changing image";
        updateCurrentRecordData();

        QString qmlProductThumbnailPath;

        productPreviewFileName = QString(QDir::currentPath()).append("/images/products/").append(
                                   currentRecordData.value(2)).append(".jpg");
        if(!QFile(productPreviewFileName).exists())
        {
            // gyűjtőcsoport
//            productPreviewFileName = QString(QDir::currentPath()).append("/images/products/").append(
//                        currentRecordData.value(3)).toLower().append(".jpg");
            productPreviewFileName = QString(QDir::currentPath()).append("/images/products/").append(
                        "facsavarok").append(".jpg");
        }
//        qDebug() << "Product thumbnail path: " << productPreviewFileName;

        if(QFile(productPreviewFileName).exists()) {
            qmlProductThumbnailPath = QString("file:///").append(productPreviewFileName);
            qmlProductThumbnail->setProperty("source", qmlProductThumbnailPath);
            qmlProductThumbnail->setProperty("visible", true);
        }
        else {
            qmlProductThumbnail->setProperty("visible", false);
        }

        if(!bigProductPreview.isActiveWindow()) bigProductPreview.close();
    }
}


//void ItemNumberSearch::on_productPreviewImage_clicked()
void ItemNumberSearch::on_productThumbnail_clicked()
{
    bigProductPreview.showImage(productPreviewFileName);
    bigProductPreview.open();
}

void ItemNumberSearch::updateProductCount()
{
    productCountLabel->setText(QString("Jelenleg %1 db termék található "
                                   "a webruházban.").arg(myModel->rowCount()));
}

void ItemNumberSearch::on_productPreviewCheckBox_toggled(bool isChecked)
{
    qDebug() << "toogled:" << productPreviewCheckBox->isChecked();
    if(isChecked) {
        on_itemTableView_selectionModel_currentChanged();
        qmlProductThumbnail->setProperty("visible", false);
    }
    else {
        qmlProductThumbnail->setProperty("visible", true);
    }
}

void ItemNumberSearch::on_customerViewCheckBox_toggled(bool isChecked)
{
    itemTableView->setColumnHidden(2, isChecked);
    itemTableView->setColumnHidden(9, isChecked);
}

void ItemNumberSearch::importFPI()
{

    QSqlRecord record;

    myModel->setEditStrategy(QSqlTableModel::OnManualSubmit);

    myDatabase->closeDB(&myDatabase->db);
    myDatabase->openDBold();

    QSqlTableModel oldModel;
    oldModel.setTable("items");
    oldModel.select();
    while(oldModel.canFetchMore()) oldModel.fetchMore();

    for(int i=0; i<myModel->rowCount(); ++i) {
        for(int j=0; j<oldModel.rowCount(); ++j) {
            if(   myModel->record(i).value("own_product_id")
               == oldModel.record(j).value("own_item_number")
              )
            {
                record = myModel->record(i);
                record.setValue("foreign_product_id",
                                oldModel.record(j).value("foreign_item_number"));
                /*
                qDebug() << "match";
                qDebug() << myModel->record(i).value("own_product_id") << " : "
                         << oldModel.record(j).value("foreign_item_number");
                */

                myModel->setRecord(i, record);
            }
        }
    }

    //myDatabase->closeDB(&myDatabase->db2);
    myDatabase->openDB();

    myModel->submitAll();
}

void ItemNumberSearch::customFunction()
{
    QSqlRecord currentRecord;

    myModel->setEditStrategy(QSqlTableModel::OnManualSubmit);

    for(int i=0; i<myModel->rowCount(); ++i) {
        currentRecord = myModel->record(i);
        /*
        if(currentRecord.value("category").toString().contains("Fogantyúk")) {
            if(currentRecord.value("name").toString().contains("FAGOMB")) {
                currentRecord.setValue("name", currentRecord.value("name").toString().replace("FAGOMB", "fagomb"));
            }
            else if(currentRecord.value("name").toString().contains("GOMB")) {
                currentRecord.setValue("name", currentRecord.value("name").toString().replace("GOMB", "gomb"));
            }
            else if(currentRecord.value("name").toString().contains("ÜVEGBETÉTES")) {
                currentRecord.setValue("name", currentRecord.value("name").toString().replace("ÜVEGBETÉTES", "üvegbetétes"));
            }
            else if(currentRecord.value("name").toString().contains("FABETÉTES")) {
                currentRecord.setValue("name", currentRecord.value("name").toString().replace("FABETÉTES", "fabetétes"));
            }
            else {
                currentRecord.setValue("name", currentRecord.value("name").toString().append(" fogas"));
            }
        }
        if(currentRecord.value("category").toString().contains("Fogasok")) {
            currentRecord.setValue("name", currentRecord.value("name").toString().append(" fogas"));
        }
        */
        //currentRecord.setValue("price", currentRecord.value("price").toString().replace(',', '.').toDouble());
        //currentRecord.setValue("price", currentRecord.value("price").toString().replace(',', '.'));
        //currentRecord.setValue("purchase_price", currentRecord.value("purchase_price").toString().replace(',', '.'));


        if(currentRecord.value("name").toString().contains("fogantyú csavar",Qt::CaseInsensitive) ||
           currentRecord.value("name").toString().contains("facsavar",Qt::CaseInsensitive)) {
            continue;
        }
        else {
            double currentPrice = currentRecord.value("price").toDouble();

            if(currentPrice > 15) {
                int newPrice = round(currentPrice);
                currentRecord.setValue("price", newPrice);
            }
        }

        myModel->setRecord(i, currentRecord);
    }

    myModel->submitAll();
}
