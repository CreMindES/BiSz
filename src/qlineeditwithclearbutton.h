#ifndef QLINEEDITWITHCLEARBUTTON_H
#define QLINEEDITWITHCLEARBUTTON_H

#include <QLineEdit>

class QToolButton;

class QLineEditWithClearButton : public QLineEdit
{
    Q_OBJECT

public:
    explicit QLineEditWithClearButton(QWidget *parent = 0);

protected:
    void resizeEvent(QResizeEvent *);
//    QSize sizeHint();
    
private slots:
    void updateClearButton(const QString& text);

private:
    QToolButton *clearButton;

signals:
    void textEmpty();

public slots:
    
};

#endif // QLINEEDITWITHCLEARBUTTON_H
