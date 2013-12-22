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
    QVBoxLayout *hasDefaultValueVBoxLayout;
    QVBoxLayout *defaultValueVBoxLayout;
    QHBoxLayout *optionsLayout;
    QVBoxLayout *mainLayout;
    QVBoxLayout *reducedControlLayout;

    // UI elements
    QCheckBox *customerViewHasDefaultValueCheckBox;
    QCheckBox *customerViewCheckBox;

    QCheckBox *onlineSearchHasDefaultValueCheckBox;
    QCheckBox *onlineSearchCheckBox;

    QCheckBox *exactMatchHasDefaultValueCheckBox;
    QCheckBox *exactMatchCheckBox;

    QDialogButtonBox *buttonBox;

    void createOptionsUI();

private slots:
    void on_defaultEnabledValueChange();
};

#endif // OPTIONSDIALOG_H
