#include "productdetailswidget.h"
#include "itemnumbersearch.h"

ProductDetailsWidget::ProductDetailsWidget(QWidget *parent, DatabaseManager *database)
    : QDialog(parent)
{
    numberOfErrors = 0;
    error = 0;

    db = database;

    productNameLabel       =    new QLabel(trUtf8("Terméknév"));
    ownPartNumberLabel     =    new QLabel(trUtf8("Saját cikkszám"));
    foreignPartNumberLabel =    new QLabel(trUtf8("Idegen cikkszám"));
    descriptionLabel       =    new QLabel(trUtf8("Termékleírás"));
    categoryLabel          =    new QLabel(trUtf8("Kategória"));
    categorySub1Label      =    new QLabel(trUtf8("Alkategória 1"));
    categorySub2Label      =    new QLabel(trUtf8("Alkategória 2"));
    packageUnitLabel       =    new QLabel(trUtf8("Csomagolási egység"));
    weightLabel            =    new QLabel(trUtf8("Súly (kg)"));
    purchasePriceLabel     =    new QLabel(trUtf8("Beszerzési ár (HUF)"));
    priceLabel             =    new QLabel(trUtf8("Ár (HUF)"));
    profitMarginLabel      =    new QLabel(trUtf8("Haszonkulcs"));
    discountLabel          =    new QLabel(trUtf8("Akció (%)"));
    discountValueLabel     =    new QLabel(trUtf8("Akció (%)"));
    discountedPriceLabel   =    new QLabel(trUtf8("Akciós ár (HUF)"));


    productNameLineEdit = new QLineEdit;
    ownProductIdLineEdit = new QLineEdit;

    foreignProductIdLineEdit = new QLineEdit;

    categoryComboBox = new QComboBox;
    categoryComboBox->setEditable(false);
    categorySub1ComboBox = new QComboBox;
    categorySub2ComboBox = new QComboBox;

    categoryComboBox->setModel(&categoryListModel);
    categorySub1ComboBox->setModel(&categorySub1ListModel);
    categorySub2ComboBox->setModel(&categorySub2ListModel);

    descriptionTextEdit = new QTextEdit;
    descriptionTextEdit->setFixedHeight(100);

    packageUnitLineEdit = new QLineEdit;

    weightSpinBox = new QDoubleSpinBox;
    //weightSpinBox->setMaximum(200);
    weightSpinBox->setMinimum(0);
//    weightSpinBox->setSuffix(" kg");

    purchasePriceSpinBox = new QDoubleSpinBox;
    purchasePriceSpinBox->setMaximum(999999.99);
    purchasePriceSpinBox->setMinimum(0);
//    purchasePriceSpinBox->setSuffix(" Ft");

    roundingUpButton = new QPushButton("Kerekítés");

    //QRegExp numberRegExp("[1-9]\\.?[0-9]{0,6}");
    //ownPartNumberLineEdit->setValidator((new QRegExpValidator(numberRegExp, this)));
    //priceLineEdit->setValidator(new QRegExpValidator(numberRegExp, this));

    priceSpinBox = new QDoubleSpinBox;
//    priceSpinBox->setSuffix(" Ft");
    priceSpinBox->setMaximum(999999.99);
    priceSpinBox->setMinimum(0.00);

    profitMarginSpinBox = new QDoubleSpinBox;
    profitMarginSpinBox->setSuffix("%");
    profitMarginSpinBox->setMaximum(999999.99);
    profitMarginSpinBox->setMinimum(-100.00);
    //profitMarginSpinBox->setDecimals(2);

    discountCheckBox = new QCheckBox;
    discountValueSpinBox = new QSpinBox;
    discountValueSpinBox->setMaximum(99);
    discountValueSpinBox->setMinimum(0);
//    discountValueSpinBox->setSuffix("%");
    discountValueSpinBox->setMinimumWidth(50);
    discountedPriceSpinBox = new QDoubleSpinBox;
    discountedPriceSpinBox->setMaximum(999999.99);
    discountedPriceSpinBox->setMinimum(0.00);

    okButton = new QPushButton("Ok");
    cancelButton = new QPushButton("Cancel");

    errorDescriptionLabel = new QLabel;
    errorDescriptionLabel->setText("");
    errorDescriptionLabel->setStyleSheet("QLabel { color : red; }");


    errorIcon = new QLabel;
    errorIcon->setPixmap(QPixmap(":/images/error_icon.png").scaledToHeight(30, Qt::SmoothTransformation));

    //QMetaObject::connectSlotsByName(this);

    buttonHLayout = new QHBoxLayout;

    buttonHLayout->addWidget(okButton);
    buttonHLayout->addWidget(cancelButton);

    productGridLayout = new QGridLayout;

    int rowNum = 0;

    productGridLayout->addWidget(productNameLabel, rowNum, 0);
    productGridLayout->addWidget(productNameLineEdit, rowNum++, 1);

    productGridLayout->addWidget(ownPartNumberLabel, rowNum, 0);
    productGridLayout->addWidget(ownProductIdLineEdit, rowNum++, 1);

    productGridLayout->addWidget(foreignPartNumberLabel, rowNum, 0);
    productGridLayout->addWidget(foreignProductIdLineEdit, rowNum++, 1);

    productGridLayout->addWidget(categoryLabel, rowNum, 0);
    productGridLayout->addWidget(categoryComboBox, rowNum++, 1);
    productGridLayout->addWidget(categorySub1Label, rowNum, 0);
    productGridLayout->addWidget(categorySub1ComboBox, rowNum++, 1);
    productGridLayout->addWidget(categorySub2Label, rowNum, 0);
    productGridLayout->addWidget(categorySub2ComboBox, rowNum++, 1);

    categorySub1Label->setHidden(true);
    categorySub1ComboBox->setHidden(true);
    categorySub2Label->setHidden(true);
    categorySub2ComboBox->setHidden(true);

    productGridLayout->addWidget(descriptionLabel, rowNum, 0);
    productGridLayout->addWidget(descriptionTextEdit, rowNum++, 1);

    productGridLayout->addWidget(packageUnitLabel, rowNum, 0);
    productGridLayout->addWidget(packageUnitLineEdit, rowNum++, 1);

    productGridLayout->addWidget(weightLabel, rowNum, 0);
    productGridLayout->addWidget(weightSpinBox, rowNum++, 1);

    priceGridLayout = new QGridLayout;
    rowNum = 0;

    priceGridLayout->addWidget(purchasePriceLabel, rowNum, 0);
    priceGridLayout->addWidget(purchasePriceSpinBox, rowNum++, 1);

    //priceGridLayout->addWidget(roundingUpButton, rowNum++, 1);

    priceGridLayout->addWidget(profitMarginLabel, rowNum, 0);
    priceGridLayout->addWidget(profitMarginSpinBox, rowNum++, 1);

    priceGridLayout->addWidget(priceLabel, rowNum, 0);
    priceGridLayout->addWidget(priceSpinBox, rowNum++, 1);

/*
    discountHLayout = new QHBoxLayout;

    discountHLayout->addWidget(discountValueSpinBox);
    discountHLayout->addSpacing(10);
    //discountLayout->addWidget(discountValueLabel);
    discountHLayout->addWidget(discountCheckBox, 0, Qt::AlignHCenter);
    discountHLayout->addStretch();

    //priceGridLayout->addWidget(discountLabel, rowNum, 0);
    //priceGridLayout->addLayout(discountLayout, rowNum++, 1);

    priceGridLayout->addWidget(discountValueLabel, rowNum, 1);
    priceGridLayout->addWidget(discountValueSpinBox, rowNum++, );

    //priceGridLayout->addWidget(discountedPriceLabel, rowNum, 0);
    //priceGridLayout->addWidget(discountedPriceSpinBox, rowNum++, 1);
*/

    //priceGridLayout->addWidget(errorDescriptionLabel, rowNum++, 0, 1, 2, Qt::AlignLeft);

    //priceGridLayout->addLayout(buttonHLayout, rowNum, 1, 2, 1, Qt::AlignBottom);

    QGroupBox *productGroupBox = new QGroupBox(trUtf8("Termék"));
    QGroupBox *priceGroupBox = new QGroupBox(trUtf8("Ár"));

    productGroupBox->setLayout(productGridLayout);
    priceGroupBox->setLayout(priceGridLayout);

    mainVLayout = new QVBoxLayout;

    mainVLayout->addWidget(productGroupBox);
    mainVLayout->addWidget(priceGroupBox);
    mainVLayout->addWidget(errorDescriptionLabel);
    mainVLayout->addLayout(buttonHLayout);

    this->setLayout(mainVLayout);
    //this->setFixedSize(this->sizeHint());

    connect(okButton, SIGNAL(clicked()), this, SLOT(on_okButton_clicked()));
    connect(cancelButton, SIGNAL(clicked()), this, SLOT(on_cancelButton_clicked()));
    connect(categoryComboBox, SIGNAL(currentIndexChanged(int)),
            this,             SLOT(on_currentCategoryChange(int)));
    connect(categorySub1ComboBox, SIGNAL(currentIndexChanged(int)),
            this,                 SLOT(on_currentCategorySub1Change(int)));
    connect(purchasePriceSpinBox, SIGNAL(valueChanged(double)),
            this,                 SLOT(on_purchasePriceSpinBox_valueChanged(double)));
    connect(profitMarginSpinBox, SIGNAL(valueChanged(double)),
            this,                SLOT(on_profitMarginSpinBox_valueChanged(double)));
    connect(priceSpinBox,   SIGNAL(valueChanged(double)),
            this,           SLOT(on_priceSpinBox_valueChanged(double)));
    connect(roundingUpButton, SIGNAL(clicked()), this, SLOT(on_roundingUpButton_clicked()));
    connect(discountCheckBox, SIGNAL(toggled(bool)),
            this,             SLOT(on_discountCheckBox_toggled(bool)));

}

bool ProductDetailsWidget::validateInputData()
{
    qDebug() << "Validating user data";

    if(productNameLineEdit->text().isEmpty()) {
        errorDescriptionLabel->setText(trUtf8("Nem adtál meg terméknevet!"));
        numberOfErrors++;
    }
    if(foreignProductIdLineEdit->text().length() < 4) {
        numberOfErrors++;
        errorDescriptionLabel->setText(trUtf8("Túl rövid idegen cikkszám!"));
    }
/*
    if(descriptionTextEdit->document()->isEmpty()) {
        numberOfErrors++;
        errorDescriptionLabel->setText(trUtf8("Túl rövid leírás!"));
    }
*/
    if(categoryComboBox->currentIndex() == -1) {
        numberOfErrors++;
        errorDescriptionLabel->setText(trUtf8("Nem adtál meg kategóriát!"));
    }
    if(categorySub1ComboBox->isVisible()
            && categorySub1ComboBox->currentIndex() == -1) {
        numberOfErrors++;
        errorDescriptionLabel->setText(trUtf8("Nem adtál meg alkategória 1-et!"));
    }
    if(categorySub2ComboBox->isVisible()
            && categorySub2ComboBox->currentIndex() == -1) {
        numberOfErrors++;
        errorDescriptionLabel->setText(trUtf8("Nem adtál meg alkategória 2-őt!"));
    }
    /*
    if(priceSpinBox->value() == 0) {
        numberOfErrors++;
        errorDescriptionLabel->setText(trUtf8("Hé, ingyen nem adunk semmit!"));
    }
    */

    QString errorString;
    errorString.append(trUtf8("Hiányos adatok ")).append(
                QString::number(numberOfErrors)).append(
                           trUtf8(" db mezőnél!"));

    if(numberOfErrors > 1) {
        errorDescriptionLabel->setText(errorString);
        error = multiplie_error;
    }

    if(numberOfErrors) {
        numberOfErrors = 0;
        return false;
    }

    numberOfErrors = 0;
    return true;
}

void ProductDetailsWidget::submitUserData()
{
    qDebug() << "submitting begin";
    this->accept();
}

void ProductDetailsWidget::clearErrorDescriptionLabel()
{
    errorDescriptionLabel->setText("");
}

void ProductDetailsWidget::on_cancelButton_clicked()
{
//    qDebug() << "Cancel button pressed";
    this->close();
}

void ProductDetailsWidget::on_okButton_clicked()
{
//    qDebug() << "Ok button pressed";
    if(validateInputData()) {
        submitUserData();
    }
}

void ProductDetailsWidget::on_currentCategoryChange(int currentIndex)
{
    if(currentIndex<0) {
        return;
    }

    QStringList categoryListLevel_1;

    //TODO: make this more elegant
    categorySub1ListModel.setStringList(categoryListLevel_1);
    categorySub2ListModel.setStringList(categoryListLevel_1);
    categorySub1ComboBox->setHidden(true);
    categorySub2ComboBox->setHidden(true);

    for(int i=0; i<db->categoryList.length(); ++i) {
        QStringList currentCategoryString = db->categoryList.at(i).split(" > ");
        QString currentCategoryLevel0 = currentCategoryString.value(0);
        QString choosenCategoryLevel0 = categoryListModel.stringList().at(currentIndex);

        if(currentCategoryLevel0 == choosenCategoryLevel0) {
            if(!categoryListLevel_1.contains(currentCategoryString.value(1)))
            categoryListLevel_1.append(currentCategoryString.value(1));
        }
    }

    if(categoryListLevel_1.length() == 1 && categoryListLevel_1.at(0) == "") {
        categoryListLevel_1.clear();
    }

    categoryListLevel_1.sort();
    if(categoryListLevel_1.length()) {
        categorySub1ListModel.setStringList(categoryListLevel_1);
        categorySub1ComboBox->setVisible(true);
    }
    else {
        categorySub1ComboBox->setHidden(true);
    }

    qDebug() << "subcat1 lenght: " << categorySub1ListModel.stringList().length();
}

void ProductDetailsWidget::on_currentCategorySub1Change(int currentIndex)
{
     qDebug() << "subcat1 index: " << categorySub1ComboBox->currentIndex();

     if(currentIndex<0) {
        return;
    }

    QStringList categoryListLevel_2;

    categorySub2ListModel.setStringList(categoryListLevel_2);
    categorySub2ComboBox->setHidden(true);

    for(int i=0; i<db->categoryList.length(); ++i) {
        QStringList currentCategoryString = db->categoryList.at(i).split(" > ");

        QString currentCategoryLevel0 = currentCategoryString.value(0);
        QString currentCategoryLevel1 = currentCategoryString.value(1);

        QString choosenCategoryLevel0 = db->categoryListLevel_0.at(
                                            categoryComboBox->currentIndex());
        QString choosenCategoryLevel1 = categorySub1ListModel.stringList().at(currentIndex);

        if(   currentCategoryLevel1 == choosenCategoryLevel1
           && currentCategoryLevel0 == choosenCategoryLevel0 ) {
                if(!categoryListLevel_2.contains(currentCategoryString.value(2))) {
                    categoryListLevel_2.append(currentCategoryString.value(2));
                }
        }
    }

    if(categoryListLevel_2.length() == 1 && categoryListLevel_2.at(0) == "") {
        categoryListLevel_2.clear();
    }

    categoryListLevel_2.sort();
    if(categoryListLevel_2.length()) {
        categorySub2ListModel.setStringList(categoryListLevel_2);
        categorySub2ComboBox->setVisible(true);
    }
    else {
        categorySub2ComboBox->setHidden(true);
    }

         qDebug() << "subcat2 lenght: " << categorySub2ListModel.stringList().length();
}

void ProductDetailsWidget::updateProfitMargin(double purchasePrice, double webshopPrice)
{
    if(purchasePriceSpinBox->value() == 0 || priceSpinBox->value() == 0) {
        profitMarginSpinBox->setValue(0);
        return;
    }

    profitMarginSpinBox->setValue((webshopPrice / purchasePrice - 1) * 100);
}

void ProductDetailsWidget::on_purchasePriceSpinBox_valueChanged(double newPurchasePrice)
{
    updateProfitMargin(newPurchasePrice, priceSpinBox->value());
}

void ProductDetailsWidget::on_profitMarginSpinBox_valueChanged(double newProfitMargin)
{
    if(purchasePriceSpinBox->value() == 0) return;

    priceSpinBox->setValue(purchasePriceSpinBox->value() * (newProfitMargin/100 + 1));
}

void ProductDetailsWidget::on_priceSpinBox_valueChanged(double newPrice)
{
    updateProfitMargin(purchasePriceSpinBox->value(), newPrice);
}


void ProductDetailsWidget::on_discountCheckBox_toggled(bool isChecked)
{
    // BUG: description editor blackout
    discountedPriceLabel->setVisible(isChecked);
    discountedPriceSpinBox->setVisible(isChecked);
    descriptionTextEdit->updateGeometry();
    descriptionTextEdit->repaint();
    productGridLayout->update();


    if(isChecked) {
        connect(discountValueSpinBox, SIGNAL(valueChanged(int)),
                this,                 SLOT(on_discountValueSpinBox_valueChanged(int)));
        connect(discountedPriceSpinBox, SIGNAL(valueChanged(double)),
                this,                   SLOT(on_discountedPriceSpinBox_valueChanged(double)));
        emit discountValueSpinBox->valueChanged(discountValueSpinBox->value());
    }
    else {
        disconnect(discountValueSpinBox, SIGNAL(valueChanged(int)),
                this,                 SLOT(on_discountValueSpinBox_valueChanged(int)));
        disconnect(discountedPriceSpinBox, SIGNAL(valueChanged(double)),
                this,                   SLOT(on_discountedPriceSpinBox_valueChanged(double)));
    }
}

void ProductDetailsWidget::on_discountValueSpinBox_valueChanged(int newDiscountValue)
{
    discountedPriceSpinBox->setValue(priceSpinBox->value() * (1.00 - newDiscountValue/100.00));
}

void ProductDetailsWidget::on_discountedPriceSpinBox_valueChanged(double newDiscountedPrice)
{
    // BUG: rounding
    double price = priceSpinBox->value();
    qDebug() << newDiscountedPrice/price;
    qDebug() << newDiscountedPrice/price * 100;
    qDebug() << 1.00 - newDiscountedPrice/price * 100;
    //int discountValue = int(100 - newDiscountedPrice/price * 100);
    //discountValueSpinBox->setValue(discountValue);
}

void ProductDetailsWidget::on_roundingUpButton_clicked()
{
    priceSpinBox->setValue(ceil(priceSpinBox->value()));
}
