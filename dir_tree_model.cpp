#include "dir_tree_model.h"

DirTreeModel::DirTreeModel(QObject *parent)
    : QFileSystemModel(parent)
{
    this->setRootPath("");
    this->setFilter(QDir::AllEntries | QDir::Hidden | QDir::System);
    // TODO: show hidden by '-a' flag

}

