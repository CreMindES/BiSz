#ifndef MYQSFPMODEL_H
#define MYQSFPMODEL_H

#include <QSortFilterProxyModel>
#include <QDebug>

class MyQSFPModel : public QSortFilterProxyModel
{
    Q_OBJECT

public:
    MyQSFPModel(QObject *parent = 0);

protected:
    bool filterAcceptsRow(int sourceRow, const QModelIndex &sourceParent) const;
};

#endif // MYQSFPMODEL_H
