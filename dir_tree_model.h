#ifndef DIRTREEMODEL_H
#define DIRTREEMODEL_H

#include <QObject>
#include <QFileSystemModel>
#include <QHash>

class DirTreeModel : public QFileSystemModel
{
public:
    DirTreeModel(QObject *parent = nullptr);

    QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const override;
    void calculateDirSize(const QString& path);

private:
    QHash<QString, qint64> dirSizes_;
};

#endif // DIRTREEMODEL_H
