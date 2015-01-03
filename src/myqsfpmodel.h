#ifndef MYQSFPMODEL_H
#define MYQSFPMODEL_H

#include <QSortFilterProxyModel>
#include <QSqlTableModel>
#include <QModelIndex>
#include <QDebug>

class MyQSFPModel : public QSortFilterProxyModel
{
    Q_OBJECT

public:
    MyQSFPModel(QObject *parent = 0);

    //int columnCount(const QModelIndex &parent = QModelIndex()) const;
    /*
    QModelIndex index(int row, int column, const QModelIndex &parent) const;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const;
    */

protected:
    bool filterAcceptsRow(int sourceRow, const QModelIndex &sourceParent) const;

private:
//    int differenceColumn;
};

#endif // MYQSFPMODEL_H
