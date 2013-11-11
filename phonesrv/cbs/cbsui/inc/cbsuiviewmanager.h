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
* Description:
*
*/

#ifndef CBSUIVIEWMANAGER_H
#define CBSUIVIEWMANAGER_H

#include <QObject>
#include <CbsCommonTypes.h>
#include <hbdocumentloader.h>

class HbMainWindow;
class MCbs;
class CCbsUiTopicListMonitor;
class CbsUiTopicListView;
class CbsUiTopicView;
class HbAction;


// Here we create custom document loader to be able to use own classes in XML.
class CbsUiDocumentLoader : public HbDocumentLoader
{
public:
    virtual QObject *createObject(const QString &type, const QString &name);
};

class CbsUiViewManager : public QObject
{
    Q_OBJECT
public:

    /**
     * Constructor
     * 
     * @param window The mainwindow of qt mode.
     * @param server  The handler of connecting to Cbs server.
     */
    CbsUiViewManager(HbMainWindow *window, MCbs &server);
 
    /**
     * Destructor
     */
    ~CbsUiViewManager();

     /**
     * Get handler of CbsUiDocumentLoader
     * 
     * @return The handler of CbsUiDocumentLoader.
     */
    CbsUiDocumentLoader* xmlLoader() { return mLoader; };

public slots:

    /**
     * Activate the currenct view as Topic view
     * 
     * @param topicNumber The currenct topic number.
     */
    void activateTopicView(TCbsTopicNumber topicNumber);

     /**
     * Activate the currenct view as Topic list view
     */
    void activateMainView();

private:

    /**
     * Not Own, from CbsUiMainWindow
     */
    HbMainWindow *mMainWindow;
	
    /**
     * Own
     */
    CCbsUiTopicListMonitor *mTopicMonitor;

	/**
     * Not own, reference of MCbs
     */
    MCbs &mServer;	

    /**
     * Own but don't delete. Deleted via mObjects
     */
    CbsUiTopicListView *mTopicListView;
	
    /**
     * Own but don't delete. Deleted via mObjects
     */
    CbsUiTopicView *mTopicView;

    /**
     * Own
     */
    CbsUiDocumentLoader *mLoader;

    /**
     * objects load from XML. Needs to be deleted once application exits
     */
    QList<QObject *> mObjects;

    /**
     * Own but don't delete. Deleted via mObjects
     */
    HbAction *mSoftKeyBackAction;

    /**
     * Own but don't delete. Deleted via mObjects
     */
    HbAction *mSoftKeyExitAction;

};

#endif          // CBSUIVIEWMANAGER_H

// End of file

