#ifndef PRODUCTPREVIEW_H
#define PRODUCTPREVIEW_H

#include <QDialog>

#include <QLabel>
#include <QIcon>
#include <QImage>
#include <QPixmap>
#include <QString>
#include <QHBoxLayout>

#include <QDebug>

class ProductPreview : public QDialog
{
    Q_OBJECT
public:
    explicit ProductPreview(QWidget *parent = 0);

    void showImage(QString filename);

    QLayout *myLayout;
    QLabel productPreview;
    QPixmap myPixmap;
//    QImage myImage;
    
signals:
    
public slots:
    
};

#endif // PRODUCTPREVIEW_H
