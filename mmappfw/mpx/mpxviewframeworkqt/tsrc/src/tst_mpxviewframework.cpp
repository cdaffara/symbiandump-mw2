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
#include <QtTest/QtTest>
#include <QDebug>

#include <hbview.h>

#include <mpxviewpluginqt.h>
#include <mpxviewframeworkqt.h>
#include <xqpluginloader.h>

class TestMpxViewFramework : public QObject
{
    Q_OBJECT

public:
    TestMpxViewFramework(): QObject(), fw(0) {};

private slots:

    void initTestCase();
    void cleanupTestCase();
    
    void init();
    void cleanup();
    
    void testDryPluginViewInstance();
    
    void testMpxResolver();
    
private:
    
    void testMpxPlugViewInstance(MpxViewPlugin *plugin);

public:
    MpxViewFramework* fw;
};

void TestMpxViewFramework::initTestCase()
{
}
    
void TestMpxViewFramework::cleanupTestCase()
{
}

void TestMpxViewFramework::init()
{
    fw = new MpxViewFramework();
    QVERIFY(fw != 0);
}
    
void TestMpxViewFramework::cleanup()
{
    delete fw;
}

// = = Actual Tests = = 

void TestMpxViewFramework::testDryPluginViewInstance()
{
    // #1 instantiate it as real plugin

    QList<int> uidList;
    uidList << 0xE1253177 << 0xE1253178 << 0xE1253179;
    
    foreach (int uid, uidList) {
        XQPluginLoader pluginLoader(uid);
        QObject *objInstance = pluginLoader.instance();
        QVERIFY(objInstance != 0);
        
        MpxPluginViewInterface *interfaceInstance = qobject_cast<MpxPluginViewInterface*>(objInstance);
        QVERIFY(interfaceInstance != 0);
        
        MpxViewPlugin *plugView = interfaceInstance->viewPlugin();
        QVERIFY(plugView != 0);
        
        HbView *view = qobject_cast<HbView*>(plugView->getView());
        qDebug(view->title().toLatin1().data());

        pluginLoader.unload();
    }
    
}

void TestMpxViewFramework::testMpxResolver()
{
    QList<int> uids;
    
    uids.append( 0x10000123 );
    uids.append( 0x30000001 );
    
    MpxViewPlugin *plugin = fw->resolvePlugin(uids);
    QVERIFY(plugin != 0);

    HbView *view = qobject_cast<HbView*>(plugin->getView());
    qDebug(view->title().toLatin1().data());

}

void TestMpxViewFramework::testMpxPlugViewInstance(MpxViewPlugin *plugin)
{
    QVERIFY(plugin != 0);
    QGraphicsWidget *widget = plugin->getView();
    QVERIFY(widget != 0);
}



QTEST_MAIN(TestMpxViewFramework)

#include "tst_mpxviewframework.moc"
