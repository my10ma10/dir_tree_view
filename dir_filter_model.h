#ifndef DIRFILTERMODEL_H
#define DIRFILTERMODEL_H

#include <QSortFilterProxyModel>
#include <QWidget>

class DirFilterModel : public QSortFilterProxyModel
{
    Q_OBJECT

public:
    explicit DirFilterModel(QObject *parent = nullptr);

    void setRootPath(const QString& path);

protected:
    bool filterAcceptsRow(int sourceRow,
                const QModelIndex &sourceParent) const override;

    bool matches(const QModelIndex& index, const QString& elemName) const;

    bool hasMatchingAncestor(const QModelIndex& index) const;
    bool hasMatchingDescendant(const QModelIndex& index) const;


    QString rootPath_;
};

#endif // DIRFILTERMODEL_H
