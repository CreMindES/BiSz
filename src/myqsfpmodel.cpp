#include "myqsfpmodel.h"

MyQSFPModel::MyQSFPModel(QObject *parent)
    : QSortFilterProxyModel(parent)
{
}

bool MyQSFPModel::filterAcceptsRow(int sourceRow,
        const QModelIndex &sourceParent) const
{
    QModelIndex index0 = sourceModel()->index(sourceRow, 1, sourceParent);
    QModelIndex index1 = sourceModel()->index(sourceRow, 2, sourceParent);
    QModelIndex index2 = sourceModel()->index(sourceRow, 3, sourceParent);
    QModelIndex index3 = sourceModel()->index(sourceRow, 4, sourceParent);
    QModelIndex index6 = sourceModel()->index(sourceRow, 7, sourceParent);

    return (sourceModel()->data(index0).toString().contains(filterRegExp())
            || sourceModel()->data(index1).toString().contains(filterRegExp())
            || sourceModel()->data(index2).toString().contains(filterRegExp())
            || sourceModel()->data(index3).toString().contains(filterRegExp())
            || sourceModel()->data(index6).toString().contains(filterRegExp())
            );
}


