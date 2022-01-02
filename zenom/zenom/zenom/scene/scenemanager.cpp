#include "scenemanager.h"
#include "scenemainwindow.h"

SceneManager::SceneManager(QObject *pParent)
    : QObject(pParent)
{
    qRegisterMetaType<LogVariableConverter>();
    QMetaType::registerConverter<LogVariableConverter, double>();
    QMetaType::registerConverter<LogVariableConverter, int>();
    QMetaType::registerConverter<LogVariableConverter, float>();
    QMetaType::registerConverter<LogVariableConverter, QColor>();
    QMetaType::registerConverter<LogVariableConverter, QMatrix4x4>();
    QMetaType::registerConverter<LogVariableConverter, QPointF>();
    QMetaType::registerConverter<LogVariableConverter, QPoint>();
    QMetaType::registerConverter<LogVariableConverter, QQuaternion>();
    QMetaType::registerConverter<LogVariableConverter, QRectF>();
    QMetaType::registerConverter<LogVariableConverter, QRect>();
    QMetaType::registerConverter<LogVariableConverter, QSizeF>();
    QMetaType::registerConverter<LogVariableConverter, QSize>();
    QMetaType::registerConverter<LogVariableConverter, QVector2D>();
    QMetaType::registerConverter<LogVariableConverter, QVector3D>();
    QMetaType::registerConverter<LogVariableConverter, QVector4D>();
}

SceneManager::~SceneManager()
{

}

void SceneManager::saveSettings( QSettings& pSettings )
{
    pSettings.beginGroup("Scenes");
    pSettings.setValue("size", mSceneList.size());
    for ( int i = 0; i < mSceneList.size(); ++i )
    {
        pSettings.beginGroup( QString("Scene") + QString::number(i) );
        //mSceneList[i]->saveSettings(pSettings);
        pSettings.endGroup();
    }
    pSettings.endGroup();
}

void SceneManager::loadSettings( QSettings& pSettings )
{
    pSettings.beginGroup("Scenes");
    int size = pSettings.value("size", 0).toInt();
    for ( int i = 0; i < size; ++i )
    {
        pSettings.beginGroup( QString("Scene") + QString::number(i) );
        SceneMainWindow* scene = createScene();
        //scene->loadSettings(pSettings);
        pSettings.endGroup();
    }
    pSettings.endGroup();
}

void SceneManager::tick()
{
    for ( int i = 0; i < mSceneList.size(); ++i )
    {
        mSceneList[i]->tick();
    }
}

void SceneManager::clear()
{
    for ( int i = 0; i < mSceneList.size(); ++i )
    {
        mSceneList[i]->disconnect();
        delete mSceneList[i];
    }
    mSceneList.clear();
}

SceneMainWindow* SceneManager::createScene()
{
    SceneMainWindow* sceneWindow = new SceneMainWindow( static_cast<QWidget*>(parent()) );
    sceneWindow->show();
    connect( sceneWindow, SIGNAL(closed(SceneMainWindow*)), SLOT(sceneWindowDestroyed(SceneMainWindow*)) );
    connect( sceneWindow, SIGNAL(warningMessage(const QString&)), SIGNAL(warningMessage(const QString&)) );
    mSceneList.push_back( sceneWindow );
	
    return sceneWindow;
}

void SceneManager::sceneWindowDestroyed( SceneMainWindow* pSceneWindow )
{
    int index = mSceneList.indexOf( pSceneWindow );
    if ( index >= 0 )
        mSceneList.remove( index );
    pSceneWindow->deleteLater();
}
