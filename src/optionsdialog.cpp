#include <QtGui>
#include "optionsdialog.h"

OptionsDialog::OptionsDialog(QSettings *settings, QWidget *parent) : QDialog(parent)
{
    createOptionsUI();
    loadSettings(settings);

    emit on_defaultEnabledValueChange();
}

void OptionsDialog::createOptionsUI()
{
    this->setWindowTitle(tr("BiSz - Beállítások"));

    // Layouts
    hasDefaultValueGroupBox   = new QGroupBox;
    hasDefaultValueVBoxLayout = new QVBoxLayout;

    defaultValueGroupBox   = new QGroupBox;
    defaultValueVBoxLayout = new QVBoxLayout;

    reducedControlGroupBox = new QGroupBox;
    reducedControlLayout = new QVBoxLayout;

    accountingGroupBox = new QGroupBox;
    accountingLayout = new QVBoxLayout;

    optionsLayout = new QHBoxLayout;
    mainLayout    = new QVBoxLayout;

    // UI elements
    customerViewHasDefaultValueCheckBox = new QCheckBox(tr("Vevői nézet"), this);
    customerViewCheckBox = new QCheckBox(tr("Vevői nézet"), this);

    onlineSearchHasDefaultValueCheckBox = new QCheckBox(tr("Azonnali keresés"), this);
    onlineSearchCheckBox = new QCheckBox(tr("Azonnali keresés"), this);

    exactMatchHasDefaultValueCheckBox = new QCheckBox(tr("Teljes egyezés"), this);
    exactMatchCheckBox = new QCheckBox(tr("Teljes egyezés"), this);

    reducedControlCheckBox = new QCheckBox(tr("Csökkentett funkciók"), this);

    exportToAccountingButton = new QPushButton(tr("Exportálás a számlázó programhoz"));
//    exportToAccountingButton->setMaximumSize(exportToAccountingButton->sizeHint());

    buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel);
    buttonBox->button(QDialogButtonBox::Ok)->setText(tr("Mentés"));
    buttonBox->button(QDialogButtonBox::Cancel)->setText(tr("Mégse"));

    // Setting up UI layouts
    hasDefaultValueVBoxLayout->addWidget(customerViewHasDefaultValueCheckBox);
    hasDefaultValueVBoxLayout->addWidget(onlineSearchHasDefaultValueCheckBox);
    hasDefaultValueVBoxLayout->addWidget(exactMatchHasDefaultValueCheckBox);

    defaultValueVBoxLayout->addWidget(customerViewCheckBox);
    defaultValueVBoxLayout->addWidget(onlineSearchCheckBox);
    defaultValueVBoxLayout->addWidget(exactMatchCheckBox);

    reducedControlLayout->addWidget(reducedControlCheckBox);

    accountingLayout->addWidget(exportToAccountingButton);
//    accountingLayout->setAlignment(exportToAccountingButton, Qt::AlignHCenter);

    // Setting up UI GroupBoxes
    hasDefaultValueGroupBox->setTitle(tr("Alapértelmezett beállítás engedélyezése"));
    hasDefaultValueGroupBox->setLayout(hasDefaultValueVBoxLayout);

    defaultValueGroupBox->setTitle(tr("Alapértelmezett beállítás"));
    defaultValueGroupBox->setLayout(defaultValueVBoxLayout);

    reducedControlGroupBox->setTitle(tr("Csökkentett funkciók"));
    reducedControlGroupBox->setLayout(reducedControlLayout);

    accountingGroupBox->setTitle(tr("Könyvelőprogram"));
    accountingGroupBox->setLayout(accountingLayout);

    optionsLayout->addWidget(hasDefaultValueGroupBox);
    optionsLayout->addWidget(defaultValueGroupBox);

    // Assembling final layout from sublayouts
    mainLayout->addLayout(optionsLayout);
    mainLayout->addWidget(reducedControlGroupBox);
    mainLayout->addWidget(accountingGroupBox);
    mainLayout->addWidget(buttonBox);

    this->setLayout(mainLayout);

    this->setFixedSize(this->sizeHint());

    // Connecting signals and slots
    connect(buttonBox, SIGNAL(accepted()), this, SLOT(accept()));
    connect(buttonBox, SIGNAL(rejected()), this, SLOT(reject()));

    connect(customerViewHasDefaultValueCheckBox, SIGNAL(toggled(bool)),
            this, SLOT(on_defaultEnabledValueChange()));
    connect(onlineSearchHasDefaultValueCheckBox, SIGNAL(toggled(bool)),
            this, SLOT(on_defaultEnabledValueChange()));
    connect(exactMatchHasDefaultValueCheckBox,   SIGNAL(toggled(bool)),
            this, SLOT(on_defaultEnabledValueChange()));
    connect(exportToAccountingButton,            SIGNAL(clicked()),
            this, SLOT(on_exportToAccountingButton_clicked()));
}

void OptionsDialog::saveSettings(QSettings *appSettings)
{
    appSettings->beginGroup("checkbox");

    appSettings->setValue("onlineSearchDefEnabled", onlineSearchHasDefaultValueCheckBox->isChecked());
    appSettings->setValue("exactMatchDefEnabled", exactMatchHasDefaultValueCheckBox->isChecked());
    appSettings->setValue("customerViewDefEnabled", customerViewHasDefaultValueCheckBox->isChecked());

    appSettings->setValue("onlineSearchDefValue", onlineSearchCheckBox->isChecked());
    appSettings->setValue("exactMatchDefValue", exactMatchCheckBox->isChecked());
    appSettings->setValue("customerViewDefValue", customerViewCheckBox->isChecked());

    appSettings->setValue("reducedFunctions", reducedControlCheckBox->isChecked());

    appSettings->endGroup();
}

void OptionsDialog::loadSettings(QSettings *appSettings)
{
    appSettings->beginGroup("checkbox");

    onlineSearchHasDefaultValueCheckBox->setChecked(
                appSettings->value("onlineSearchDefEnabled", false).toBool());
    exactMatchHasDefaultValueCheckBox->setChecked(
                appSettings->value("exactMatchDefEnabled", false).toBool());
    customerViewHasDefaultValueCheckBox->setChecked(
                appSettings->value("customerViewDefEnabled", false).toBool());

    onlineSearchCheckBox->setChecked(appSettings->value("onlineSearchDefValue", false).toBool());
    exactMatchCheckBox->setChecked(appSettings->value("exactMatchDefValue", false).toBool());
    customerViewCheckBox->setChecked(appSettings->value("customerViewDefValue", false).toBool());

    reducedControlCheckBox->setChecked(appSettings->value("reducedFunctions", false).toBool());

    appSettings->endGroup();
}

void OptionsDialog::on_defaultEnabledValueChange()
{
    //qDebug() << "changed";
    customerViewCheckBox->setEnabled(customerViewHasDefaultValueCheckBox->isChecked());
    onlineSearchCheckBox->setEnabled(onlineSearchHasDefaultValueCheckBox->isChecked());
    exactMatchCheckBox->setEnabled(exactMatchHasDefaultValueCheckBox->isChecked());
}

void OptionsDialog::on_exportToAccountingButton_clicked()
{
    emit exportToAccountingButton_clicked();
}
