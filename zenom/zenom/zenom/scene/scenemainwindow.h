#ifndef SCENEMAINWINDOW_H
#define SCENEMAINWINDOW_H

#include <QMainWindow>
#include <QQuickView>
#include <QDockWidget>
#include <QMenuBar>
#include <QAction>
#include <QProgressDialog>

#include "hierarchywidget.h"
#include "inspectorwidget.h"
#include "binddialog.h"

class SceneMainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit SceneMainWindow(QWidget *pParent = nullptr);

    void clear();
    void readQmlFile(const QString& pFileName);
    void tick();
    void setSceneName(const QString& pName = QString());

signals:
    void warningMessage( const QString& pText );
    void closed(SceneMainWindow* sceneMainWindow);

protected:
    virtual void closeEvent(QCloseEvent *pEvent);

private slots:
    void onOpenQmlFileActionTriggered();
    void onViewStatusChanged(const QQuickView::Status pStatus);
    void changeInspectorObject();
    void bind(PropertyTracker* pPropertyTracker, LogVariable* pLogVariable);
    void unbind(PropertyTracker* pPropertyTracker);

private:
    QQuickView* mView;
    QProgressDialog* mBusyIndicator;
    HierarchyWidget* mHierarchyWidget;
    InspectorWidget* mInspectorWidget;
    BindDialog* mBindDialog;
    QSet<PropertyTracker*> mBindedProperties;
};

#endif // SCENEMAINWINDOW_H
