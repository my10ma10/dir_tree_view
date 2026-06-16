#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "dir_tree_model.h"
#include "dir_tree_view.h"

#include <QMainWindow>

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
    Ui::MainWindow *ui;
    DirTreeModel* model_;
    DirTreeView* view_;

};
#endif // MAINWINDOW_H
