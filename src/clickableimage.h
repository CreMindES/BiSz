#ifndef CLICKABLEIMAGE_H
#define CLICKABLEIMAGE_H

#include <QLabel>
#include <QMouseEvent>
#include <QBrush>
#include <QPainter>
#include <QImage>

class ClickableImage : public QLabel
{
    Q_OBJECT
public:
    explicit ClickableImage(const QString &text = "", QWidget *parent = 0);
    ~ClickableImage();
    //void repaint();
    
signals:
    void clicked();
    //void mouseLeft(ClickableImage* sender);
    //void mouseReleaseEvent(QMouseEvent *ev);

protected:
    void mousePressEvent ( QMouseEvent * event );

    
};

#endif // CLICKABLEIMAGE_H
