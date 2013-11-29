#include "productpreview.h"

ProductPreview::ProductPreview(QWidget *parent) :
    QDialog(parent)
{
    QHBoxLayout myLayout;
    myLayout.addWidget(&productPreview);
    this->setLayout(&myLayout);
    this->setMaximumSize(500, 500);
}

void ProductPreview::showImage(QString filename)
{
    if(!filename.isEmpty()) {
        myPixmap.load(filename);
        productPreview.setPixmap(myPixmap);
        productPreview.setGeometry(myPixmap.rect());
        if(myPixmap.height() >= 500) {
            productPreview.setPixmap(myPixmap.scaled(500, 500, Qt::KeepAspectRatio,
                                                     Qt::SmoothTransformation));
            productPreview.setGeometry(0,0,500,500);
            //qDebug() << "scaling";
        }
    }
}

