#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "dir_tree_model.h"
#include "dir_tree_view.h"

#include <QMainWindow>
#include <QSortFilterProxyModel>
#include <QCommandLineParser>

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
    void parseCommandLine(QCommandLineParser& parser);

    Ui::MainWindow *ui;
    DirTreeModel* dir_model_;
    DirTreeView* dir_view_;
    QSortFilterProxyModel* sort_filter_model_;

};
#endif // MAINWINDOW_H
