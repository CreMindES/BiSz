#ifndef OPTIONSDIALOG_H
#define OPTIONSDIALOG_H

#include <QDialog>

#include <QSettings>

#include <QCheckBox>
#include <QGroupBox>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QDialogButtonBox>
#include <QPushButton>

class OptionsDialog : public QDialog
{
    Q_OBJECT

public:
    OptionsDialog(QSettings *settings, QWidget *parent = 0);

    void saveSettings(QSettings *appSettings);
    void loadSettings(QSettings *appSettings);

    // TODO: create getter and put back to private
    QCheckBox *reducedControlCheckBox;

private:
    // Layout
    QGroupBox *defaultValueGroupBox;
    QGroupBox *hasDefaultValueGroupBox;
    QGroupBox *reducedControlGroupBox;
    QGroupBox *accountingGroupBox;
    QVBoxLayout *hasDefaultValueVBoxLayout;
    QVBoxLayout *defaultValueVBoxLayout;
    QHBoxLayout *optionsLayout;
    QVBoxLayout *mainLayout;
    QVBoxLayout *reducedControlLayout;
    QVBoxLayout *accountingLayout;

    // UI elements
    QCheckBox *customerViewHasDefaultValueCheckBox;
    QCheckBox *customerViewCheckBox;

    QCheckBox *onlineSearchHasDefaultValueCheckBox;
    QCheckBox *onlineSearchCheckBox;

    QCheckBox *exactMatchHasDefaultValueCheckBox;
    QCheckBox *exactMatchCheckBox;

    QPushButton *exportToAccountingButton;

    QDialogButtonBox *buttonBox;

    void createOptionsUI();

signals:
    void exportToAccountingButton_clicked();

private slots:
    void on_defaultEnabledValueChange();
    void on_exportToAccountingButton_clicked();
};

#endif // OPTIONSDIALOG_H
