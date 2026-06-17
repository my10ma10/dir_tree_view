#include "dir_filter_model.h"
#include <QString>
#include <QDir>
#include <QFileSystemModel>

DirFilterModel::DirFilterModel(QObject *parent)
    : QSortFilterProxyModel{parent}
{

}

bool DirFilterModel::filterAcceptsRow(int sourceRow, const QModelIndex &sourceParent) const
{
    QModelIndex index = sourceModel()->index(sourceRow, 0, sourceParent);

    if (!index.isValid()) return false;
    if (!sourceParent.isValid()) return true;

    QString name = sourceModel()->data(index).toString();

    auto fsModel = qobject_cast<QFileSystemModel*>(sourceModel());
    QString path = fsModel->filePath(index);
    if (QDir::cleanPath(path) == rootPath_) return true;

    return matches(index, name);
}

bool DirFilterModel::matches(const QModelIndex &index, const QString& elemName) const
{
    return
        elemName.contains(filterRegularExpression()) ||
        hasMatchingAncestor(index) ||
        hasMatchingDescendant(index);
}

bool DirFilterModel::hasMatchingAncestor(const QModelIndex &index) const
{
    QModelIndex parent = index.parent();
    while (parent.isValid()) {
        QString name = sourceModel()->data(parent).toString();
        if (name.contains(filterRegularExpression())) return true;

        parent = parent.parent();
    }
    return false;
}

bool DirFilterModel::hasMatchingDescendant(const QModelIndex &index) const
{
//    qDebug() << sourceModel()->data(index).toString()
//             << sourceModel()->rowCount(index);

    const int rowsCount = sourceModel()->rowCount(index);
    for (int i = 0; i < rowsCount; ++i) {
        if (filterAcceptsRow(i, index)) {
            return true;
        }
    }
    return false;
}

void DirFilterModel::setRootPath(const QString& path)
{
    rootPath_ = QDir::cleanPath(path);
}
