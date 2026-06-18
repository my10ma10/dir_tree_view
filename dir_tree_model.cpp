#include "dir_tree_model.h"
#include <QLocale>

DirTreeModel::DirTreeModel(QObject *parent)
    : QFileSystemModel(parent)
{
    this->setRootPath("");
    this->setFilter(QDir::AllEntries | QDir::Hidden | QDir::System);
}

QVariant DirTreeModel::data(const QModelIndex &index, int role) const
{
    if (index.column() == 1 && role == Qt::DisplayRole) {
        QFileInfo info = fileInfo(index);

        if (info.isDir()) {
            auto it = dirSizes_.find(info.absoluteFilePath());

            if (it != dirSizes_.end())
                return QLocale().formattedDataSize(it.value());

            return "Обновить";
        }
    }
    return QFileSystemModel::data(index, role);
}

void DirTreeModel::calculateDirSize(const QString &path)
{
    qint64 size = 0;

    QDirIterator it(
        path,
        QDir::Files | QDir::Hidden | QDir::NoDotAndDotDot,
        QDirIterator::Subdirectories
    );

    while (it.hasNext() && !it.fileInfo().isSymLink())
    {
        it.next();
        size += it.fileInfo().size();
    }

    dirSizes_[path] = size;

    QModelIndex idx = index(path);

    emit dataChanged(this->index(idx.row(), 1, idx.parent()),
                     this->index(idx.row(), 1, idx.parent()));
}

