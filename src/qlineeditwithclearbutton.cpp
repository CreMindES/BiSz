#include "qlineeditwithclearbutton.h"

#include <QToolButton>
#include <QStyle>
#include <QDebug>

QLineEditWithClearButton::QLineEditWithClearButton(QWidget *parent) :
    QLineEdit(parent)
{
    clearButton = new QToolButton(this);
    QPixmap pixmap(":images/clearSearchLineEdit.jpg");

    clearButton->setIcon(QIcon(pixmap.scaledToHeight(16, Qt::SmoothTransformation)));
    //clearButton->setIconSize(pixmap.size());
    clearButton->setGeometry(0,0,16,16);
    clearButton->setCursor(Qt::ArrowCursor);
    clearButton->setStyleSheet("QToolButton { border: none; padding: 0px; }");
    clearButton->hide();

    connect(clearButton, SIGNAL(clicked()), this, SLOT(clear()));
    connect(this, SIGNAL(textChanged(QString)), this, SLOT(updateClearButton(QString)));

    int frameWidth = style()->pixelMetric(QStyle::PM_DefaultFrameWidth);
    //setStyleSheet(QString("QLineEdit { padding-right: %1px; } ").arg(clearButton->sizeHint().width() + frameWidth + 1));
    setStyleSheet(QString("QLineEdit { padding-right: %1px; } ").arg(17 + frameWidth + 1));
    //QSize msz = minimumSizeHint();
    /*
    setMinimumSize(qMax(msz.width(), clearButton->sizeHint().height() + frameWidth * 2 + 2),
                   qMax(msz.height(), clearButton->sizeHint().height() + frameWidth * 2 + 2));
    */
    /*
    setMinimumSize(qMax(msz.width(), clearButton->height() + frameWidth * 2 + 2),
                   qMax(msz.height(), clearButton->height() + frameWidth * 2 + 2));
                   */
}

void QLineEditWithClearButton::resizeEvent(QResizeEvent *)
{
    QSize sz = clearButton->sizeHint();
    int frameWidth = style()->pixelMetric(QStyle::PM_DefaultFrameWidth);
    /*clearButton->move(rect().right() - frameWidth - sz.width(),
                (rect().bottom() + 1 - sz.height())/2);
                */
    clearButton->move(rect().right() - frameWidth - sz.width(),
                     (rect().bottom() + 1 - sz.height())/2 + 2);
}

void QLineEditWithClearButton::updateClearButton(const QString& text)
{
    if(text.isEmpty()) {
        emit textEmpty();
    }

    clearButton->setVisible(!text.isEmpty());
}

/*
QSize QLineEditWithClearButton::sizeHint()
{
    return QSize(16, 16);
}
*/
