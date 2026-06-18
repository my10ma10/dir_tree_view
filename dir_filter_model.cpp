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
    if (matchedPaths_.isEmpty() && currentSearchPattern_.isEmpty())
        return true;

    QModelIndex index = sourceModel()->index(sourceRow, 0, sourceParent);
    if (!index.isValid()) return false;

    QString name = sourceModel()->data(index).toString();
    if (name == "." || name == "..") return false;

    auto fsModel = qobject_cast<QFileSystemModel*>(sourceModel());
    QString path = QDir::cleanPath(fsModel->filePath(index));

    return matchedPaths_.contains(path);
}

void DirFilterModel::setRootPath(const QString& path)
{
    rootPath_ = QDir::cleanPath(path);
}

void DirFilterModel::setMatchedPaths(const QSet<QString> paths)
{
    matchedPaths_ = paths;
    invalidateFilter();
}

void DirFilterModel::setSearchDepth(SearchDepth depth)
{
    searchDepth_ = depth;
}

void DirFilterModel::setSearchText(const QString& text)
{
    currentSearchPattern_ = text;
}

SearchDepth DirFilterModel::getSearchDepth() const
{
    return searchDepth_;
}
