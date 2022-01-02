#ifndef SCENEMANAGER_H
#define SCENEMANAGER_H

#include <QObject>
#include <QSettings>

#include "scenemainwindow.h"

class SceneManager : public QObject
{
    Q_OBJECT

public:
    SceneManager(QObject *pParent);

    ~SceneManager();
	
    void saveSettings( QSettings& pSettings );

    void loadSettings( QSettings& pSettings );

    void tick();

    void clear();

public slots:
    SceneMainWindow* createScene();

signals:
    void warningMessage( const QString& pText );

private slots:
    void sceneWindowDestroyed( SceneMainWindow* pSceneMainWindow );

private:
    QVector<SceneMainWindow*> mSceneList;
};

#endif // SCENEMANAGER_H
