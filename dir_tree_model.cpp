#include "dir_tree_model.h"

DirTreeModel::DirTreeModel(QObject *parent)
    : QObject(parent)
{
    model_.setRootPath("");
    model_.setFilter(QDir::AllEntries | QDir::Hidden | QDir::System);
    // TODO: show hidden by '-a' flag

}

QFileSystemModel *DirTreeModel::getModelPtr()
{
    return &model_;
}

void DirTreeModel::setOption(QFileSystemModel::Option option, bool on)
{
    model_.setOption(option, on);
}

QModelIndex DirTreeModel::index(const QString &path) const
{
    return model_.index(path);
}
