/*
* Copyright (c) 2009 Nokia Corporation and/or its subsidiary(-ies).
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
* Description: 
*
*/

#include <QtGui>

#include <QObject>
#include <QString>

#include <hbapplication.h>
#include <hbmainwindow.h>
#include <hbview.h>
#include <hbaction.h>

#include <welcomeview.h>
#include <qtracker.h>

#include <xqpluginloader.h>
#include <xqplugininfo.h>

#include <mpxviewframeworkqt.h>
#include <mpxviewpluginqt.h>

#include <QList>

int main(int argc, char *argv[])
{
    HbApplication app(argc, argv);
    QTracker::log("--- Start ---");
    MpxViewFramework mainWindow;
    
    HbView *view = new WelcomeView();
        
    mainWindow.viewCount();
    
    mainWindow.addView(view);
    mainWindow.addView();
    
    /* LOADING PLUGINS DIRECTLY */
    XQPluginLoader pLoader(0xE1253164);
    
    QList<XQPluginInfo> info;
    pLoader.listImplementations("org.nokia.mmdt.TestPlugin/1.0", info);
//    QObject *instance = pLoader.instance();
//    PluginViewInterface *viewInterface = qobject_cast<PluginViewInterface*>(pLoader.instance());

//    mainWindow.addView(viewInterface->getView());
//    viewInterface->setParent(viewInterface->getView());
    
//    HbAction *backAction = new HbAction("Back", &mainWindow);
//    mainWindow.addSoftKeyAction( Hb::SecondarySoftKey, backAction );
// ---- 8< ----
//    mainWindow.addView(new ExampleResolver("welcomeviewplugin.dll"));

    QTracker::log(QString("List available plugins:"));

    for (int i=0; i<info.size(); i++) {
        int uid = info[i].uid();
        QTracker::log(QString("#%1 UID: %2").arg(i).arg(uid));
    }
    
    QObject* obj = pLoader.instance();
    QTracker::log(QString("instancing 0xE1253164: %1").arg(reinterpret_cast<int>(obj)));
    
    MpxPluginViewInterface *interf = qobject_cast<MpxPluginViewInterface*>(obj);
    QTracker::log(QString("casting to PluginViewInterface 0xE1253164: %1").arg(reinterpret_cast<int>(interf)));
    
    MpxViewPlugin *plugView = interf->viewPlugin(); 
    QTracker::log(QString("returning QViewPlugin: %1").arg(reinterpret_cast<int>(plugView)));
    
    QList<int> uids;
    uids.append(0x10000000);
    uids.append(0xE419BEEE);

    MpxViewPlugin *pluginView2 = mainWindow.resolvePlugin(uids);
	QTracker::log(QString("Using MPX resolver: %1").arg(reinterpret_cast<int>(pluginView2)));
    
// ---- 8< ----
    
    
    mainWindow.show();
    return app.exec();
}

