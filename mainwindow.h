#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "dir_tree_model.h"
#include "dir_tree_view.h"
#include "dir_filter_model.h"

#include <QMainWindow>
#include <QSortFilterProxyModel>
#include <QCommandLineParser>
#include <QLineEdit>
#include <QCheckBox>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(const QApplication& app, QWidget *parent = nullptr);
    ~MainWindow();

private:
    void parseCommandLine(QCommandLineParser& parser, const QApplication& app);
    void setupRootPath();
    void setupLayout();
    void configureExpand(const QString& text);
    void expandMatching(const QModelIndex& parent);
    QSet<QString> shallowSearchFindMatches(const QString& text);
    QSet<QString> deepSearchFindMatches(const QString& text);
    void addAncestors(QString path, QSet<QString>& result);


    Ui::MainWindow *ui;
    DirTreeModel* dirModel_;
    DirTreeView* dirView_;

    DirFilterModel* filterModel_;

    QLineEdit* lineEdit_;
    QCheckBox* enableCaseSensetivityButton_;
    QCheckBox* enableDeepSearchButton_;

    QString rootPath_;

    QTimer* debounceTimer_;
    QString pendingText_;

private slots:
    void pathChanged(const QString& text);
    void enableCaseSensetivity();
    void enableDeepSearch(bool checked);

    void debouncePath(const QString& path);
};
#endif // MAINWINDOW_H
