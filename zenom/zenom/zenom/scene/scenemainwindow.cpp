#include "scenemainwindow.h"

#include <QQuickItem>
#include <QFileDialog>

SceneMainWindow::SceneMainWindow(QWidget *pParent)
    : QMainWindow(pParent),
      mView(new QQuickView()),
      mBusyIndicator(new QProgressDialog(tr("Loading..."), QString(), 0, 0, this, Qt::FramelessWindowHint)),
      mHierarchyWidget(new HierarchyWidget(this)),
      mInspectorWidget(new InspectorWidget(this)),
      mBindDialog(new BindDialog(DataRepository::instance()->logVariables(), this))
{
    mBusyIndicator->close();
    resize(screen()->size() * 0.75);
    setSceneName();
    mView->setResizeMode(QQuickView::SizeRootObjectToView);

    auto viewContainer = createWindowContainer(mView, this);
    setCentralWidget(viewContainer);

    auto hierarchyDock = new QDockWidget(this);
    hierarchyDock->setWindowTitle(mHierarchyWidget->windowTitle());
    hierarchyDock->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
    addDockWidget(Qt::LeftDockWidgetArea, hierarchyDock);
    hierarchyDock->setWidget(mHierarchyWidget);

    auto inspectorDock = new QDockWidget(this);
    inspectorDock->setWindowTitle(mInspectorWidget->windowTitle());
    hierarchyDock->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
    addDockWidget(Qt::RightDockWidgetArea, inspectorDock);
    inspectorDock->setWidget(mInspectorWidget);

    auto fileMenu = menuBar()->addMenu(tr("File"));
    auto openQmlFileAction = fileMenu->addAction(tr("Open QML File"));
    auto closeAction = fileMenu->addAction(tr("Close"));

    auto viewMenu = menuBar()->addMenu(tr("View"));
    auto hierarchyAction = hierarchyDock->toggleViewAction();
    hierarchyAction->setText(tr("Scene Tree"));
    viewMenu->addAction(hierarchyAction);
    auto inspectorAction = inspectorDock->toggleViewAction();
    inspectorAction->setText(tr("Inspector"));
    viewMenu->addAction(inspectorAction);

    connect(mHierarchyWidget, &QWidget::windowTitleChanged, hierarchyDock, &QWidget::setWindowTitle);
    connect(mInspectorWidget, &QWidget::windowTitleChanged, inspectorDock, &QWidget::setWindowTitle);
    connect(openQmlFileAction, &QAction::triggered, this, &SceneMainWindow::onOpenQmlFileActionTriggered);
    connect(closeAction, &QAction::triggered, this, &QWidget::close);
    connect(mView, &QQuickView::statusChanged, this, &SceneMainWindow::onViewStatusChanged);
    connect(mHierarchyWidget, &QTreeWidget::itemSelectionChanged, this, &SceneMainWindow::changeInspectorObject);
    connect(mInspectorWidget, &InspectorWidget::bindDialogRequested, mBindDialog, &BindDialog::openDialog);
    connect(mBindDialog, &BindDialog::bindTriggered, this, &SceneMainWindow::bind);
    connect(mBindDialog, &BindDialog::unbindTriggered, this, &SceneMainWindow::unbind);
}

void SceneMainWindow::tick()
{
    for (auto propertyTracker : mBindedProperties)
        if (propertyTracker->canUpdate())
            propertyTracker->update();

    mInspectorWidget->updateValues();
}

void SceneMainWindow::clear()
{
    mHierarchyWidget->clear();
    mInspectorWidget->clear();
    mBindedProperties.clear();
}

void SceneMainWindow::onOpenQmlFileActionTriggered()
{
    const auto& fileName = QFileDialog::getOpenFileName(this, tr("Load QML File"), "", "QML Files (*.qml)");
    readQmlFile(fileName);
}

void SceneMainWindow::readQmlFile(const QString &pFileName)
{
    if (!pFileName.isEmpty())
    {
        if (!QFile::exists(pFileName))
        {
            emit warningMessage( QString("Warning: Failed opening file: '%1' does not exist.").arg(pFileName) );
            return;
        }

        mBusyIndicator->open();
        clear();
        mView->setSource(pFileName);
    }
}

void SceneMainWindow::closeEvent(QCloseEvent *pEvent)
{
    emit closed( this );
    QWidget::closeEvent( pEvent );
}

void SceneMainWindow::onViewStatusChanged(const QQuickView::Status pStatus)
{
    switch (pStatus)
    {
    case QQuickView::Loading:
        emit warningMessage(QString(tr("Loading scene: ")) + mView->source().toString());
        break;

    case QQuickView::Ready:
        mHierarchyWidget->setSceneRoot(mView->rootObject());
        setSceneName(mView->source().fileName());
        mBusyIndicator->close();
        break;

    case QQuickView::Null:
    case QQuickView::Error:
        setSceneName();
        const auto& errors = mView->errors();
        for (const auto& error : errors)
            emit warningMessage(error.toString());
        mBusyIndicator->close();
    break;
    }
}

void SceneMainWindow::changeInspectorObject()
{
    const auto& selectedItems = mHierarchyWidget->selectedItems();

    if (!selectedItems.isEmpty())
        mInspectorWidget->setCurrentObject(static_cast<ObjectHierarchyTreeWidgetItem*>(selectedItems.first())->object());
}

void SceneMainWindow::setSceneName(const QString &pName)
{
    static const QString TITLE_PREFIX(tr("Scene: "));
    setWindowTitle(TITLE_PREFIX + pName);
    mHierarchyWidget->setSceneName(pName);
    mInspectorWidget->setSceneName(pName);
}

void SceneMainWindow::bind(PropertyTracker* pPropertyTracker, LogVariable* pLogVariable)
{
    pPropertyTracker->bind(pLogVariable);
    mBindedProperties.insert(pPropertyTracker);
}

void SceneMainWindow::unbind(PropertyTracker* pPropertyTracker)
{
    pPropertyTracker->unbind();
    mBindedProperties.remove(pPropertyTracker);
}
