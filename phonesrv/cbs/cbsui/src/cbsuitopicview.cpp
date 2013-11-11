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
#include <QGraphicsLinearLayout>

#include <hbapplication.h>
#include <hbmenu.h>
#include <hbicon.h>
#include <hblistwidget.h>
#include <hblistwidgetitem.h>

#include "cbsuitopicview.h"
#include "cbsuiviewmanager.h"
#include "rcbsui.h"
#include "cbslog.h"
#include "cbsuiconstants.h"

const char *T_TITLE = "listWidget2";
const char *T_PREVIOUS= "action";
const char *T_BACK= "action_2";

// ======== MEMBER FUNCTIONS ==================================================

// ----------------------------------------------------------------------------
// CbsUiTopicView::CbsUiTopicView
// ----------------------------------------------------------------------------
CbsUiTopicView::CbsUiTopicView()
    : HbView(),
      mServer(NULL),
      mViewManager(NULL),
      mTopic(NULL),
      mCurrentTopicNumber(NULL)
{
    LOG(NORMAL,"CBSUI: CbsUiTopicView::CbsUiTopicView IN"); 
    
    LOG(NORMAL,"CBSUI: CbsUiTopicView::CbsUiTopicView OUT"); 
}

// ----------------------------------------------------------------------------
// CbsUiTopicView::~CbsUiTopicView
// ----------------------------------------------------------------------------
CbsUiTopicView::~CbsUiTopicView()
{
    LOG(NORMAL,"CBSUI: CbsUiTopicView::~CbsUiTopicView IN");
    LOG(NORMAL,"CBSUI: CbsUiTopicView::~CbsUiTopicView OUT");
}

// ----------------------------------------------------------------------------
// CbsUiTopicView::init
// ----------------------------------------------------------------------------
void CbsUiTopicView::init(MCbs &server, CbsUiViewManager &viewManager)
{
    LOG(NORMAL,"CBSUI: CbsUiTopicView::init IN");
    mViewManager = &viewManager;
    mServer = &server;
    createComponents();
    LOG(NORMAL,"CBSUI: CbsUiTopicView::init OUT");
}

// ----------------------------------------------------------------------------
// CbsUiTopicView::TopicListChangedL
// ----------------------------------------------------------------------------
void CbsUiTopicView::TopicListChangedL(TCbsTopicListEvent /*aEvent*/,
        TCbsTopicNumber /*aTopicNumber*/)                                        
{
    LOG(NORMAL,"CBSUI: CbsUiTopicView::TopicListChangedL IN");    
    /*if (mTopic && 
         mCurrentTopicNumber == aTopicNumber &&
         ECbsTopicReceivedNewMessage == aEvent) {
        reloadMessages(aTopicNumber);
    }    */
    LOG(NORMAL,"CBSUI: CbsUiTopicView::TopicListChangedL OUT");
}

// ----------------------------------------------------------------------------
// CbsUiTopicView::createComponents
// ----------------------------------------------------------------------------
void CbsUiTopicView::createComponents()
{
    LOG(NORMAL,"CBSUI: CbsUiTopicView::createComponents IN"); 
    
    mTopic = qobject_cast<HbListWidget *>(
        mViewManager->xmlLoader()->findWidget(T_TITLE));
    LOG2(NORMAL,"CBSUI: CbsUiTopicView::createComponents mTopic=%d", mTopic); 
    
    // Previous Action
    mPrevious = qobject_cast<HbAction *>( 
        mViewManager->xmlLoader()->findObject(T_PREVIOUS));
    
    if (!mPrevious) {
        LOG(NORMAL,"CBSUI: CbsUiTopicListView::createComponents\
                 mPrevious faild");
    }

    // Back Action
    mBack = qobject_cast<HbAction *>( 
        mViewManager->xmlLoader()->findObject(T_BACK));
    
    if (!mBack) {
        LOG(NORMAL,"CBSUI: CbsUiTopicListView::createComponents\
                 mBack faild");
    } else {
    connect(mBack, SIGNAL(triggered(bool)), mViewManager, SLOT(activateMainView()));
    LOG(NORMAL,"CBSUI: CbsUiTopicListView::createComponents\
         mBack successfully");
    }

    LOG(NORMAL,"CBSUI: CbsUiTopicView::createComponents OUT"); 
}

// ----------------------------------------------------------------------------
// CbsUiTopicView::reloadMessages
// ----------------------------------------------------------------------------
void CbsUiTopicView::reloadMessages(TCbsTopicNumber topicNumber)
{
    LOG(NORMAL,"CBSUI: CbsUiTopicView::reloadMessages IN"); 
    mCurrentTopicNumber = topicNumber;
    // Update topic details
    HbListWidgetItem* listItem = new HbListWidgetItem();
    TCbsTopic topic;

    mServer->GetTopic(mCurrentTopicNumber, topic);
   
    // Clear first
    mTopic->clear();
    QString name = QString::fromUtf16(topic.iName.Ptr(), topic.iName.Length()); 
    QString numberAndName = QVariant(topic.iNumber).toString();
    // instert zeros to topic number in case 2 or 1 digit topic number.
    switch (numberAndName.length()) {
        case 1:
            {
            numberAndName.insert(0, "0");
            numberAndName.insert(0, "0");
            break;
            }
        case 2:
            {
            numberAndName.insert(0, "0");
            break;
            }
        default:{}
        // do nothing
        }

    listItem->setText(numberAndName);
    //updateStatusIcon(*listItem, topic);
    mTopic->addItem(listItem);
    listItem = NULL;
    LOG(NORMAL,"CBSUI: CbsUiTopicView::reloadMessages OUT"); 
}

// ----------------------------------------------------------------------------
// CbsUiTopicView::addMessage
// ----------------------------------------------------------------------------
void CbsUiTopicView::addMessage(const TCbsMessage& /*message*/, 
                                const QString& /*msgName*/)
{
    LOG(NORMAL,"CBSUI: CbsUiTopicView::addMessage IN");     
    LOG(NORMAL,"CBSUI: CbsUiTopicView::addMessage OUT");     
}

// End of file
