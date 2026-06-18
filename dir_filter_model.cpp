#include "dir_filter_model.h"
#include <QString>
#include <QDir>
#include <QFileSystemModel>
#include <QDebug>

DirFilterModel::DirFilterModel(QObject *parent)
    : QSortFilterProxyModel{parent}
{

}

bool DirFilterModel::filterAcceptsRow(int sourceRow, const QModelIndex &sourceParent) const
{
    QModelIndex index = sourceModel()->index(sourceRow, 0, sourceParent);
    if (!index.isValid()) return false;

    QString name = sourceModel()->data(index).toString();
    if (name == "." || name == "..") return false;

    auto fsModel = qobject_cast<QFileSystemModel*>(sourceModel());
    QString path = fsModel->filePath(index);
    if (QDir::cleanPath(path) == rootPath_) return true;

    return matches(index, name);
}

bool DirFilterModel::matches(const QModelIndex &index, const QString& elemName) const
{
    return
        elemName.contains(filterRegularExpression()) ||
        hasMatchingDescendant(index);
}

//bool DirFilterModel::hasMatchingAncestor(const QModelIndex &index) const
//{
//    QModelIndex parent = index.parent();
//    while (parent.isValid()) {
//        QString name = sourceModel()->data(parent).toString();
//        if (name.contains(filterRegularExpression())) return true;

//        parent = parent.parent();
//    }
//    return false;
//}

bool DirFilterModel::hasMatchingDescendant(const QModelIndex &index) const
{

    if (searchDepth_ == SearchDepth::Shallow) return false;

    auto fsModel = qobject_cast<QFileSystemModel*>(sourceModel());
    QString path = fsModel->filePath(index);

    if (!path.startsWith(rootPath_)) return false;

    if (fsModel->canFetchMore(index))
        fsModel->fetchMore(index);

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

void DirFilterModel::setSearchDepth(SearchDepth depth)
{
    searchDepth_ = depth;
    invalidate();
    qDebug() << (depth == SearchDepth::Deep ? "Deep"  : "Shallow");
}

SearchDepth DirFilterModel::getSearchDepth() const
{
    return searchDepth_;
}
