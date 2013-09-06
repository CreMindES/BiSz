#ifndef PRODUCTDETAILSWIDGET_H
#define PRODUCTDETAILSWIDGET_H

#include <QDialog>
#include <QtWidgets>
#include "databasemanager.h"

class ProductDetailsWidget : public QDialog
{
    Q_OBJECT

public:
    ProductDetailsWidget(QWidget *parent = 0, DatabaseManager *database = NULL);

    QPushButton *okButton;
    QPushButton *cancelButton;

    QLineEdit *productNameLineEdit;
    QLineEdit *ownProductIdLineEdit;
    QLineEdit *foreignProductIdLineEdit;
    //QLineEdit *priceLineEdit;
    QComboBox *categoryComboBox;
    QComboBox *categorySub1ComboBox;
    QComboBox *categorySub2ComboBox;
    QTextEdit *descriptionTextEdit;
    QLineEdit *packageUnitLineEdit;
    QDoubleSpinBox *weightSpinBox;
    QDoubleSpinBox *purchasePriceSpinBox;
    QPushButton *roundingUpButton;
    QDoubleSpinBox *priceSpinBox;
    //QSlider *profitMarginSlider;
    QDoubleSpinBox *profitMarginSpinBox;
    QCheckBox *discountCheckBox;
    QSpinBox  *discountValueSpinBox;
    QDoubleSpinBox *discountedPriceSpinBox;

    QStringListModel categoryListModel;
    QStringListModel categorySub1ListModel;
    QStringListModel categorySub2ListModel;

    void clearErrorDescriptionLabel();

    QLabel *errorDescriptionLabel;

    int numberOfErrors;
    int error;

private:
    DatabaseManager *db;
    QLabel *productNameLabel;
    QLabel *ownPartNumberLabel;
    QLabel *foreignPartNumberLabel;
    QLabel *descriptionLabel;
    QLabel *categoryLabel;
    QLabel *categorySub1Label;
    QLabel *categorySub2Label;
    QLabel *packageUnitLabel;
    QLabel *weightLabel;
    QLabel *purchasePriceLabel;
    QLabel *priceLabel;
    QLabel *profitMarginLabel;
    QLabel *discountLabel;
    QLabel *discountValueLabel;
    QLabel *discountedPriceLabel;

    QLabel *errorIcon;

    // NOTE: consider using QFormLayout
    QHBoxLayout *buttonHLayout;
    QHBoxLayout *discountHLayout;
    QGridLayout *productGridLayout;
    QGridLayout *priceGridLayout;
    QVBoxLayout *mainVLayout;

    QGroupBox *productGroupBox;
    QGroupBox *priceGroupBox;

    enum Errors {
        no_error,
        error_ProductNameLenght,
        error_ForeignProductCodeLenght,
        error_ForeignProductCodeMatch,
        error_ProductDescriptionLengt,
        error_PriceValue,
        multiplie_error
    };

private slots:
    void on_okButton_clicked();
    void on_cancelButton_clicked();
    void on_currentCategoryChange(int currentIndex);
    void on_currentCategorySub1Change(int currentIndex);
    void on_discountCheckBox_toggled(bool isChecked);
    void on_profitMarginSpinBox_valueChanged();
    void on_priceSpinBox_valueChanged();
    void on_purchasePriceSpinBox_valueChanged();
    void on_discountValueSpinBox_valueChanged(int newDiscountValue);
    void on_discountedPriceSpinBox_valueChanged(double newDiscountedPrice);
    void on_roundingUpButton_clicked();

private:
    bool validateInputData();
    void submitUserData();
    void updateProfitMargin(double purchasePrice, double webshopPrice);

};

#endif // PRODUCTDETAILSWIDGET_H
