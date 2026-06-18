#include "mainwindow.h"
#include "./ui_mainwindow.h"

#include <QDir>
#include <QFileIconProvider>
#include <QScreen>
#include <QCommandLineOption>
#include <QVBoxLayout>
#include <QDebug>
#include <QTimer>


MainWindow::MainWindow(const QApplication& app, QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    dirModel_ = new DirTreeModel(this);
    dirView_ = new DirTreeView(this);
    filterModel_ = new DirFilterModel(this);

    lineEdit_ = new QLineEdit(this);
    lineEdit_->setPlaceholderText("Поиск");
    enableCaseSensetivityButton_ = new QCheckBox("Учитывать регистр", this);
    enableDeepSearchButton_ = new QCheckBox("Глубокий поиск", this);

    QCommandLineParser parser;
    parseCommandLine(parser, app);

    rootPath_ = parser.positionalArguments().isEmpty()
        ? QString(QDir::homePath()) : parser.positionalArguments().first();

    filterModel_->setRootPath(rootPath_);
    filterModel_->setSourceModel(dirModel_);
    filterModel_->setFilterCaseSensitivity(Qt::CaseInsensitive);
    filterModel_->setRecursiveFilteringEnabled(true);

    dirView_->setModel(filterModel_);


    setupRootPath();

    dirView_->setup();

    setupLayout();

    const QSize availableSize = \
            dirView_->screen()->availableGeometry().size();
    dirView_->resize(availableSize / 2);
    dirView_->setColumnWidth(0, dirView_->width() / 3);

    debounceTimer_ = new QTimer(this);
    debounceTimer_->setSingleShot(true);


    connect(this->lineEdit_, &QLineEdit::textChanged,
            this, &MainWindow::pathChanged);
    connect(this->enableCaseSensetivityButton_, &QCheckBox::clicked,
            this, &MainWindow::enableCaseSensetivity);
    connect(this->enableDeepSearchButton_, &QCheckBox::toggled,
            this, &MainWindow::enableDeepSearch);
    connect(this->dirModel_, &QFileSystemModel::directoryLoaded,
            this, &MainWindow::debouncePath);


    connect(this->debounceTimer_, &QTimer::timeout, this, [this]() {
        filterModel_->invalidate();
    });

    connect(this->lineEdit_, &QLineEdit::textChanged, this, [this](const QString& text) {
        pendingText_ = text;
        debounceTimer_->start(300);
    });

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::parseCommandLine(QCommandLineParser& parser, const QApplication& app)
{
    parser.setApplicationDescription("Утилита отображения дерева файловой системы");
    parser.addHelpOption();
    parser.addVersionOption();

    QCommandLineOption dontUseCustomDirectoryIconsOption("c", "Отключить отображение пользовательских иконок.");
    parser.addOption(dontUseCustomDirectoryIconsOption);

    QCommandLineOption dontWatchOption("w", "Отключить отслеживание изменений.");
    parser.addOption(dontWatchOption);

    parser.addPositionalArgument("директория", "Начальная директория.");

    parser.process(app);

    if (parser.isSet(dontUseCustomDirectoryIconsOption)) {
        dirModel_->setOption(QFileSystemModel::DontUseCustomDirectoryIcons);
    }
    if (parser.isSet(dontWatchOption)) {
        dirModel_->setOption(QFileSystemModel::DontWatchForChanges);
    }

}

void MainWindow::setupLayout()
{
    QHBoxLayout* topLayout = new QHBoxLayout;

    topLayout->addWidget(lineEdit_);
    topLayout->addWidget(enableCaseSensetivityButton_);
    topLayout->addWidget(enableDeepSearchButton_);


    QWidget* central_widget = new QWidget(this);

    QVBoxLayout* layout = new QVBoxLayout(central_widget);
    layout->addLayout(topLayout);
    layout->addWidget(dirView_);

    setCentralWidget(central_widget);
}

void MainWindow::setupRootPath()
{
    if (!rootPath_.isEmpty()) {
        const QModelIndex sourceIndex = dirModel_->index(QDir::cleanPath(rootPath_));
        if (sourceIndex.isValid()) {
            const QModelIndex proxyIndex = filterModel_->mapFromSource(sourceIndex);
            dirView_->setRootIndex(proxyIndex);
        }
    }
}

void MainWindow::configureExpand(const QString& text)
{
    if (text.isEmpty() ||
            filterModel_->getSearchDepth() == SearchDepth::Shallow
    ) {
        dirView_->collapseAll();
    }
    else {
        expandMatching(dirView_->rootIndex());
    }
}

void MainWindow::expandMatching(const QModelIndex &parent)
{
    const int rowsCount = filterModel_->rowCount(parent);
    for (int i = 0; i < rowsCount; ++i) {
        QModelIndex index = filterModel_->index(i, 0, parent);
        if (filterModel_->rowCount(index) > 0) {
            dirView_->expand(index);
            expandMatching(index);
        }
    }
}

void MainWindow::pathChanged(const QString& text)
{
    filterModel_->setFilterRegularExpression(text);
    QTimer::singleShot(0, this, [this, text]() {
        setupRootPath();
        configureExpand(text);
    });
}

void MainWindow::enableCaseSensetivity()
{
    if (filterModel_->filterCaseSensitivity() != Qt::CaseSensitive) {
        filterModel_->setFilterCaseSensitivity(Qt::CaseSensitive);
    }
    else {
        filterModel_->setFilterCaseSensitivity(Qt::CaseInsensitive);
    }

    const QString& text = lineEdit_->text();
    configureExpand(text);
}

void MainWindow::enableDeepSearch(bool checked)
{
    filterModel_->setSearchDepth(static_cast<SearchDepth>(checked));
}

void MainWindow::debouncePath(const QString &path)
{
    if (!path.startsWith(rootPath_)) return;
    debounceTimer_->start(150);
}


