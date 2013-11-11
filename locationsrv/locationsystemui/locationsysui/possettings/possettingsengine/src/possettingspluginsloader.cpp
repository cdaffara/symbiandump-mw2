/*
 * Copyright (c) 2010 Nokia Corporation and/or its subsidiary(-ies).
 * All rights reserved.
 * This component and the accompanying materials are made available
 * under the terms of "Eclipse Public License v1.0"
 * which accompanies this distribution, and is available
 * at the URL "http://www.eclipse.org/legal/epl-v10.html".
 *
 * Initial Contributors:
 * Nokia Corporation - initial contribution.
 *
 * Contributors:
 *
 * Description: Implementation of the Positioning Settings Plugins Loader class 
 *
 */


#include "possettingspluginsloader.h"
#include "possettingscrkey.h"
#include <QDir>
#include <QFileInfoList>
#include <QFileInfo>
#include <QPluginLoader>
#include <QDebug>
#include <xqsettingsmanager.h>
#include <xqsettingskey.h>


#ifdef Q_OS_SYMBIAN
#define PLUGIN_PATH QString("\\resource\\qt\\plugins\\positioningsettings")

#else
#define PLUGIN_PATH QString("C:\\qt\\plugins\\positioningsettings")
#endif 

// constants
const int KNoOfDigits = 4;

//---------------------------------------------------------------------
// PosSettingsPluginsLoader::PosSettingsPluginsLoader
// 
//---------------------------------------------------------------------
PosSettingsPluginsLoader::PosSettingsPluginsLoader()
    {
    qDebug() << "+ PosSettingsPluginsLoader::PosSettingsPluginsLoader()";
    qDebug() << "- PosSettingsPluginsLoader::PosSettingsPluginsLoader()";
    }

//---------------------------------------------------------------------
// PosSettingsPluginsLoader::~PosSettingsPluginsLoader
// 
//---------------------------------------------------------------------
PosSettingsPluginsLoader::~PosSettingsPluginsLoader()
    {
    qDebug() << "+ PosSettingsPluginsLoader::~PosSettingsPluginsLoader()";
    mDllNameList.clear();
    qDebug() << "- PosSettingsPluginsLoader::~PosSettingsPluginsLoader()";
    }
//---------------------------------------------------------------------
// PosSettingsPluginsLoader::loadPlugins
// 
//---------------------------------------------------------------------
QList<PosSettingsAdvInterface*> PosSettingsPluginsLoader::loadPlugins()
    {
    qDebug() << "+ PosSettingsPluginsLoader::loadPlugins()";
    // parse the default cen rep key value which is used to determine
    // the order of loading the default positioning settings plugins
    ParseCenRepKey();
    // List containing the plugins implementing the PositioningSettingsAdvancedInterface
    QList<PosSettingsAdvInterface*> pluginsList;
    // Check for the files under the positioningsettings directory
    QDir pluginsDir(PLUGIN_PATH + QDir::separator());
    QFileInfoList fileInfoList = pluginsDir.entryInfoList();
    // check each file in this directory,only if its a dll give it to the
    // plugin loader
    foreach ( const QFileInfo &fileInfo, fileInfoList )
            {
            QString fileName = fileInfo.absoluteFilePath();
						qDebug() << "Filename: " <<  fileName;
						
            if (!QLibrary::isLibrary(fileName))
                {
                continue;
                }
            // load the dlls using QPluginLoader
            QPluginLoader pluginLoader(pluginsDir.absoluteFilePath(fileName));
            qDebug() << "Plugin Filename: " <<  pluginsDir.absoluteFilePath(fileName);
            QObject *plugin = pluginLoader.instance();
            // Check if the plugin found is an implementation of the
            // PosSettingsAdvInterface,if yes add it to the plugins list

            if (plugin)
                {
                PosSettingsAdvInterface* advancedInterface = qobject_cast<
                        PosSettingsAdvInterface *> (plugin);

                if (advancedInterface)
                    {
                    
                    QString dllName = fileInfo.baseName();
                    // check the position into which the plugin needs to be
                    // inserted if it is one of the default plugins
                    for (int i = 0; i < mDllNameList.count(); i++)
                        {
                        if (dllName.compare(mDllNameList[i],Qt::CaseInsensitive) == KErrNone)
                            {
                            pluginsList.insert(i, advancedInterface);
                            break;
                            }
                        }
                    }
                }
            }
    qDebug() << "- PosSettingsPluginsLoader::loadPlugins()";
    return pluginsList;
    }

//---------------------------------------------------------------------
// PosSettingsPluginsLoader::ParseCenRepKey
// 
//---------------------------------------------------------------------
void PosSettingsPluginsLoader::ParseCenRepKey()
    {
    qDebug() << "+ PosSettingsPluginsLoader::ParseCenRepKey()";
    // create a XQSettingsManager instance
    XQSettingsManager* manager = new XQSettingsManager();
    // create a XQSettingsKey object to read the cenrep key value
    XQSettingsKey cenrepKey(XQSettingsKey::TargetCentralRepository,
            KCRUidPositioningSettings.iUid, KPositioningSettingsPlugins);
    // read the cenrep key value
    QVariant key(manager->readItemValue(cenrepKey,XQSettingsManager::TypeString));
    // convert the key value to string
    QString keyStringValue = key.toString();
    // if the length of default value is less than 4 digits then return
    if (keyStringValue.size() < KNoOfDigits)
        {
        qDebug() << "- PosSettingsPluginsLoader::ParseCenRepKey()";
        return;
        }
    // get the count of the number of plugin dlls
    int dllCount = keyStringValue.left(KNoOfDigits).toInt();
    // initialise current index to 4
    int currentIndex = KNoOfDigits;
    // extract the name of each of the dll from the cenrep key value
    for (int i = 0; i < dllCount; i++)
        {
        // extract length of dll name
        int length = keyStringValue.mid(currentIndex, KNoOfDigits).toInt();
        // increment the current index value
        currentIndex += KNoOfDigits;
        // get the dll name
        QString dllName = keyStringValue.mid(currentIndex, length);
        // append to list of dll names
        mDllNameList.append(dllName);
        // increment the value of currentIndex by length of name
        currentIndex += length;
        }
    qDebug() << "- PosSettingsPluginsLoader::ParseCenRepKey()";
    }

