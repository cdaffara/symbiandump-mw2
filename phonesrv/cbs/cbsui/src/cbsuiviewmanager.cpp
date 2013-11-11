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

#include <hbmainwindow.h>
#include <hbdocumentloader.h>
#include <hbaction.h>
#include <hbmenu.h>

#include "cbsuiviewmanager.h"
#include "cbsuitopiclistview.h"
#include "cbsuitopicview.h"
#include "mcbs.h"
#include "cbslog.h"

const char *CBS_APP_XML = ":/xml/cbs_app.docml";
const char *CBS_TOPIC_VIEW_XML = ":/xml/cbs_topic_view.docml";
const char *CBS_MAIN_VIEW = "t:view";
const char *CBS_TOPIC_VIEW ="t:view2";

// ======== MEMBER FUNCTIONS ==================================================
// ----------------------------------------------------------------------------
// CbsUiViewManager::CbsUiViewManager
// ----------------------------------------------------------------------------
//
CbsUiViewManager::CbsUiViewManager(HbMainWindow *window, MCbs &server)
    :mMainWindow(window), 
    mServer(server), 
    mTopicListView(NULL),
    mTopicView(NULL), 
    mLoader(NULL), 
    mSoftKeyBackAction(NULL), 
    mSoftKeyExitAction(NULL)
{
    LOG(NORMAL,"CBSUI: CbsUiViewManager::CbsUiViewManager IN"); 
    
    // SYMBIAN CODE:
    //mTopicMonitor = CCbsUiTopicListMonitor::NewL(mServer); not in use at the moment
    
    bool ok = false;
    mLoader = new CbsUiDocumentLoader();
    // ownership of the objects are transferred to caller
    mObjects = mLoader->load(CBS_APP_XML, &ok);
    if (ok) {
        activateMainView();
    } else {
        LOG(NORMAL,"CBSUI: CbsUiViewManager::CbsUiViewManager DocML failure!");
    }

	LOG(NORMAL,"CBSUI: CbsUiViewManager::CbsUiViewManager OUT"); 
}

// ----------------------------------------------------------------------------
// CbsUiViewManager::~CbsUiViewManager
// ----------------------------------------------------------------------------
//
CbsUiViewManager::~CbsUiViewManager()
{
    LOG(NORMAL,"CBSUI: CbsUiViewManager::~CbsUiViewManager IN"); 
    //delete mTopicMonitor;  not in use at the moment
    delete mLoader;
    // delete all objects created from DocML.
    while (!mObjects.isEmpty()) {
        LOG(NORMAL,"CBSUI: CbsUiViewManager::~CbsUiViewManager deleting..."); 
        delete mObjects.takeFirst();
    }
    LOG(NORMAL,"CBSUI: CbsUiViewManager::~CbsUiViewManager OUT"); 
}

// ----------------------------------------------------------------------------
// CbsUiViewManager::activateTopicView
// ----------------------------------------------------------------------------
//
void CbsUiViewManager::activateTopicView(TCbsTopicNumber topicNumber)
{
    LOG(NORMAL,"CBSUI: CbsUiViewManager::activateTopicView IN");
    if (!mTopicView) {
        bool ok = false;
        // ownership of the objects are transferred to caller
        mObjects += mLoader->load(CBS_TOPIC_VIEW_XML, &ok);
        if (ok) {
            mTopicView = 
            qobject_cast<CbsUiTopicView *>(mLoader->findWidget(CBS_TOPIC_VIEW));
            mTopicView->init(mServer, *this); 
        } else {
            LOG(NORMAL,"CBSUI: CbsUiViewManager::activateTopicView DocML failure!");
        }
    }

    if (mTopicView != mMainWindow->currentView()) {
        if (!mSoftKeyBackAction) {
            mSoftKeyBackAction = new HbAction(Hb::BackAction,mTopicView);
            LOG2(NORMAL,"CBSUI: CbsUiViewManager::activateTopicView \
                    mSoftKeyBackAction==%d", mSoftKeyBackAction);
        }
        
        mTopicView->reloadMessages(topicNumber);
        mMainWindow->addSoftKeyAction(Hb::SecondarySoftKey,mSoftKeyBackAction);
        bool ret = connect(mSoftKeyBackAction, SIGNAL(triggered()),
                       this, SLOT(activateMainView()));
        HbView* view = mMainWindow->currentView();
        mMainWindow->removeView(view);
        // just temp implement
        mTopicListView = 0;
        mMainWindow->addView(mTopicView);
        LOG2(NORMAL,"CBSUI: CbsUiViewManager::activateTopicView\
                 mTopicView=%d", mTopicView);
    }
    LOG(NORMAL,"CBSUI: CbsUiViewManager::activateTopicView OUT");
}

// ----------------------------------------------------------------------------
// CbsUiViewManager::activateMainView
// ----------------------------------------------------------------------------
//
void CbsUiViewManager::activateMainView()
{
    LOG(NORMAL,"CBSUI: CbsUiViewManager::activateMainView IN");
    if (!mTopicListView) {
        // First time called
        mTopicListView = qobject_cast<CbsUiTopicListView *>
            (mLoader->findWidget(CBS_MAIN_VIEW));
        mTopicListView->init(mServer, *this); 
        // Connect signal to change view
        bool ret = connect(mTopicListView, 
                            SIGNAL(topicSelected(TCbsTopicNumber)), this, 
                            SLOT(activateTopicView(TCbsTopicNumber)));
        LOG2(NORMAL,"CBSUI: CbsUiViewManager::activateMainView ret=%d", ret);
    } else {
        HbView *view = mMainWindow->currentView();
        mMainWindow->removeView(view);
        // just temp implement
        mTopicView = 0;
    }

    if (!mSoftKeyExitAction) {
        mSoftKeyExitAction = new HbAction(Hb::QuitAction, mTopicListView);
        LOG(NORMAL,"CBSUI: CbsUiViewManager::activateMainView add quit action");
    } 
    if ( mSoftKeyBackAction ) {
        mMainWindow->removeSoftKeyAction(Hb::SecondarySoftKey, mSoftKeyBackAction);
        mSoftKeyBackAction = 0;
        LOG(NORMAL,"CBSUI: CbsUiViewManager::activateMainView remove back action");
    }

    mMainWindow->addSoftKeyAction(Hb::SecondarySoftKey,mSoftKeyExitAction);
    bool ret = connect(mSoftKeyExitAction, SIGNAL(triggered()), 
            mMainWindow, SLOT(close()));
    LOG2(NORMAL,"CBSUI: CbsUiViewManager::activateMainView connect exit=%d", ret);
    mMainWindow->addView(mTopicListView);
    mMainWindow->setCurrentView(mTopicListView);
    LOG(NORMAL,"CBSUI: CbsUiViewManager::activateMainView OUT");
}

// ----------------------------------------------------------------------------
// CbsUiDocumentLoader::createObject
// For DocML usage: 
// ----------------------------------------------------------------------------
//
QObject *CbsUiDocumentLoader::createObject(
        const QString& type, const QString &name)
{
    LOG(NORMAL,"CBSUI: CbsUiViewManager::createObject IN");
    if (type == CbsUiTopicListView::staticMetaObject.className()) {
        QObject *object = new CbsUiTopicListView;
        object->setObjectName(name);
        LOG(NORMAL,"CBSUI: CbsUiViewManager::createObject:CbsUiTopicListView OUT");
        return object;
        }  else if (type == CbsUiTopicView::staticMetaObject.className()) {
        QObject *object = new CbsUiTopicView;
        object->setObjectName(name);
        LOG(NORMAL,"CBSUI: CbsUiViewManager::createObject:CbsUiTopicView OUT");
        return object;
        }
    LOG2(NORMAL,"CBSUI: CbsUiViewManager::createObject: \
            object name = %s",name.utf16());
    return HbDocumentLoader::createObject(type, name);
}

// End of file
