#include "myqsfpmodel.h"

MyQSFPModel::MyQSFPModel(QObject *parent)
    : QSortFilterProxyModel(parent)
{
//    differenceColumn = 16;
}

bool MyQSFPModel::filterAcceptsRow(int sourceRow, const QModelIndex &sourceParent) const
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
/*
int MyQSFPModel::columnCount(const QModelIndex &parent) const
{
    return QSortFilterProxyModel::columnCount(parent) + 1;
}

QModelIndex MyQSFPModel::index(int row, int column, const QModelIndex &parent) const
{

    if(column == differenceColumn) {
        qDebug() << "yeah";
        return createIndex(row, column, 18);
    }

    return QModelIndex();

}

QVariant MyQSFPModel::data(const QModelIndex &index, int role) const
{
    if (index.column() == differenceColumn && role == Qt::DisplayRole)
    {
        const QSqlTableModel *model = qobject_cast<const QSqlTableModel *>(sourceModel());
        Q_ASSERT(model);
        const int firstColumnIndex = model->fieldIndex("price");
        //const int secondColumnIndex = model->fieldIndex("MySecondColumn");
        const QModelIndex dataIndex1 = this->index(index.row(), firstColumnIndex);
        //const QModelIndex dataIndex2 = this->index(index.row(), secondColumnIndex);

        const QVariant data1 = mapToSource(dataIndex1).data(Qt::DisplayRole);
        ///const QVariant data2 = mapToSource(dataIndex2).data(Qt::DisplayRole);

        const int priceIncVat = data1.toInt() * 1.27;

        return priceIncVat;
    }

    return QSortFilterProxyModel::data(index, role);
}

QVariant MyQSFPModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (orientation == Qt::Horizontal && section == differenceColumn)
    {
        return tr("Nettó ár");
    }

    return QSortFilterProxyModel::headerData(section, orientation, role);
}


*/
