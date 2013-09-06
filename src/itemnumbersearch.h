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

#include "databasemanager.h"
#include "myqsfpmodel.h"
#include "productdetailswidget.h"
#include "productpreview.h"
#include "clickableimage.h"

class ItemNumberSearch : public QWidget
{
    Q_OBJECT

public:
    ItemNumberSearch(QWidget *parent = 0);
    ~ItemNumberSearch();

    DatabaseManager *myDatabase;

    QTableView* createView(QSortFilterProxyModel *model);

    QLabel *searchIconLabel;
    QLineEditWithClearButton *searchField;

    QPushButton *searchButton;

    QPixmap biszLogoPixmap;
    QLabel *biszLogoLabel;

    QPushButton *addButton;
    QPushButton *modifyButton;
    QPushButton *deleteButton;
    QPushButton *importButton;
    QPushButton *exportButton;
    QPushButton *exportToPrintButton;

    ClickableImage *productPreviewImage;

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

    QStringList *categoryList;

    void setupModel(QSqlRelationalTableModel *model);
    void importFPI();

private:
    QLabel *productCountLabel;
    QLabel *searchMatchCountLabel;
    int generateNewId();
    QString generateNewProductId();
    void updateCurrentRecordData();
    QSqlRecord createEmptyProductRecord();
    QMap<int, QString> currentRecordData;
    bool connected;

    QString productPreviewFileName;
    ProductPreview bigProductPreview;

    void updateSearchMatchCountLabel();

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
    void on_addButton_clicked();
    void on_modifyButton_clicked();
    void on_deleteButton_clicked();
    bool on_exportToPrintButton_clicked();
    void on_productPreviewImage_clicked();
    void on_itemTableView_selectionModel_currentChanged();
    void on_productPreviewCheckBox_toggled(bool isChecked);
    void on_customerViewCheckBox_toggled(bool isChecked);
    void on_foreignProductIdEdited();

    void reconnectProfitMarginSignal();
};

#endif // ITEMNUMBERSEARCH_H
