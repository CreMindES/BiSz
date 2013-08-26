#include "clickableimage.h"

ClickableImage::ClickableImage(const QString& text, QWidget *parent ) :
    QLabel(parent)
{
    this->setText(text);
    this->setStyleSheet("QLabel {"
                                          "border: 1px solid grey;"
                                          "border-radius: 10px;"

                                          "text-align: center"
                        "}"
                        "QLabel:hover {   "
                                          "border: 1px solid black;"
                                          "border-radius: 10px;"
                        "}"
                        "QLabel:disabled {"
                                          "qproperty-alignment: AlignCenter;"
                                          "font-weight: bold;"
                                          "border: none;"


                        "}");
}

ClickableImage::~ClickableImage()
{

}

void ClickableImage::mousePressEvent (QMouseEvent *event)
{
    emit clicked();
    emit mouseReleaseEvent(event);
}

/*
void ClickableImage::repaint()
{
//    Q_UNUSED(e)

    QBrush brush(this->pixmap()->toImage());

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.setBrush(brush);
    painter.drawRoundedRect(0, 0, width(), height(), 20, 20);
}
*/
