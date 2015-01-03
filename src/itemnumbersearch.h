#ifndef ITEMNUMBERSEARCH_H
#define ITEMNUMBERSEARCH_H

#include <QWidget>

#include <QDebug>

#include <QSettings>

#include <QPushButton>
#include <QLabel>
#include <QCheckBox>
#include <QLineEdit>
#include <QComboBox>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QFrame>

#include <QSqlRelationalTableModel>
#include <QSqlRelationalDelegate>
#include <QSqlQuery>

#include <QTableView>
#include <QList>
#include <qlineeditwithclearbutton.h>

#include <QtQml/QQmlEngine>
#include <QtQuick/QQuickView>
#include <QQuickItem>

#include "databasemanager.h"
#include "myqsfpmodel.h"
#include "productdetailswidget.h"
#include "optionsdialog.h"
#include "productpreview.h"
#include "clickableimage.h"

class ItemNumberSearch : public QWidget
{
    Q_OBJECT

public:
    ItemNumberSearch(QWidget *parent = 0);
    ~ItemNumberSearch();

    QQuickView *qmlMenuView;
    QWidget *qmlMenuSidebar;

    DatabaseManager *myDatabase;

    QTableView* createView(QSortFilterProxyModel *model);

    QLabel *searchIconLabel;
    QLineEditWithClearButton *searchField;

    QPushButton *searchButton;

    // QML

    QObject *qmlMenuObject;
    QObject *qmlLogo;
    QObject *qmlNewButton;
    QObject *qmlModifyButton;
    QObject *qmlDeleteButton;
    QObject *qmlPrintButton;
    QObject *qmlOptionsButton;
    QObject *qmlExportCSVButton;
    QObject *qmlImportCSVButton;
    QObject *qmlProductThumbnail;

    //ClickableImage *productPreviewImage;

    QCheckBox *onlineSearchCheckBox;
    QCheckBox *exactMatchCheckBox;
    QCheckBox *productPreviewCheckBox;
    QCheckBox *customerViewCheckBox;

    bool onlineSearchCheckBoxPrevValue;

    QTableView *itemTableView;

    QHBoxLayout *mainLayout;
    QHBoxLayout *searchLayout;
    QVBoxLayout *browserLayout;
    QVBoxLayout *menuLayout;

    QSqlRelationalTableModel *myModel;
    MyQSFPModel *proxyModel;

    QTableView *view;

    ProductDetailsWidget *productDialog;
    OptionsDialog *optionsDialog;

    QStringList *categoryList;

    void setupModel(QSqlRelationalTableModel *model);
    void importFPI();

private:
    // Members
    QLabel *productCountLabel;
    QLabel *searchMatchCountLabel;
    QMap<int, QString> currentRecordData;
    bool connected;
    QSettings *appSettings;
    QString productPreviewFileName;
    ProductPreview bigProductPreview;

    // Methods
    void createQmlMenuSidebar();

    QString generateNewProductId();
    QSqlRecord createEmptyProductRecord();
    int generateNewId();
    void updateCurrentRecordData();
    void updateSearchMatchCountLabel();
    double roundWithPrecision(double f, double prec);
    void customFunction();

private slots:
    void searchInDatabase();
    void importFromTxt();
    void updateProductCount();

    bool importCSV();
    bool exportCSV();

    void saveAppSettings();
    void loadAppSettings();

    void onOnlineSearchCheckBoxStateChanged();
    void exactMatchChecked();
    void enableReducedFunctions(bool doEnable);

    void showAboutDialog();
    void on_addButton_clicked();
    void on_modifyButton_clicked();
    void on_deleteButton_clicked();
    bool on_exportToPrintButton_clicked();
    void on_optionsButton_clicked();
    void on_productThumbnail_clicked();
    void on_itemTableView_selectionModel_currentChanged();
    void on_productPreviewCheckBox_toggled(bool isChecked);
    void on_customerViewCheckBox_toggled(bool isChecked);
    void on_foreignProductIdEdited();

    void reconnectProfitMarginSignal();
    bool exportToAccountingApp();
};

#endif // ITEMNUMBERSEARCH_H
