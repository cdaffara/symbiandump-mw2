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

#include <hbaction.h>
#include <hbtoolbar.h>
#include <hbicon.h>
#include <hblabel.h>
#include <hblistwidgetitem.h>
#include <hbabstractviewitem.h>
#include <hbinputdialog.h>
#include <hbmessagebox.h>
#include <hblistwidget.h>
#include <hbvalidator.h>
#include <hbmenu.h>
#include <hbmessagebox.h>

#include "cbsuitopiclistview.h"
#include "cbsuiviewmanager.h"
#include "rcbsui.h"
#include "cbslog.h"
#include "cbsuiconstants.h"
#include "ccbsuitopiclistmonitor.h"

// DocML usage
const char *T_TOPIC_LIST = "t:ListWidget";
const char *T_TOPICINDEX = "t:topicIndex"; 
// other items (for decoration)
const char *T_LABEL = "t:label";

const char *T_TOGGLE_RECEPTION = "t:toggle_reception";
const char *T_ADD_TOPIC = "t:add_topic";

// Options menu
const char *T_EXIT = "t:exit";

const char *T_OPEN = "t:open";
const char *T_SUBSCRIBE = "t:subscribe";
const char *T_UNSUBSCRIBE = "t:unsubscribe";
const char *T_HOTMARK = "t:hotmark";
const char *T_UNHOTMARK = "t:unhotmark";
const char *T_DELETE = "t:delete";
const char *T_EDIT = "t:edit";

// ======== MEMBER FUNCTIONS ==================================================

// ----------------------------------------------------------------------------
// CbsUiTopicListView::CbsUiTopicListView
// ----------------------------------------------------------------------------
CbsUiTopicListView::CbsUiTopicListView()
    : HbView(),
      mServer(NULL),
      mViewManager(NULL),
      mTopicRow(-1),
      mTopicIndexWidget(NULL),
      mTopicsWidget(NULL),
      mReceptionButton(NULL), 
      mOpenAction(NULL),
      mSubscibeAction(NULL), 
      mUnSubscibeAction(NULL), 
      mHotmarkAction(NULL), 
      mUnHotmarkAction(NULL), 
      mEditAction(NULL), 
      mDeleteAction(NULL),
      mMonitor(NULL),
      mTopicQuery(NULL),
      mTopicNumberList(NULL)
{
    LOG(NORMAL,"CBSUI: CbsUiTopicListView::CbsUiTopicListView IN");
    mTopicNumberList = new TCbsUiTopicNumberList(KNumberOfStandardTopics);
    QT_TRAP_THROWING(mTopicNumberList->AppendL(0));
    LOG(NORMAL,"CBSUI: CbsUiTopicListView::CbsUiTopicListView OUT");
}

// ----------------------------------------------------------------------------
// CbsUiTopicListView::~CbsUiTopicListView
// ----------------------------------------------------------------------------
CbsUiTopicListView::~CbsUiTopicListView()
{
    LOG(NORMAL,"CBSUI: CbsUiTopicListView::~CbsUiTopicListView IN");
    delete mTopicNumberList;
    LOG(NORMAL,"CBSUI: CbsUiTopicListView::~CbsUiTopicListView OUT");
}

// ----------------------------------------------------------------------------
// CbsUiTopicListView::init
// ----------------------------------------------------------------------------
void CbsUiTopicListView::init(MCbs &server, 
               CbsUiViewManager &viewManager)
{
    LOG(NORMAL,"CBSUI: CbsUiTopicListView::init IN");
    mViewManager = &viewManager;
    LOG(NORMAL,"CBSUI: CbsUiTopicListView::init 1");
    mServer = &server;
    LOG(NORMAL,"CBSUI: CbsUiTopicListView::init 2");
    // First get pointers to UI components
    createComponents();

    // Update reception status to UI
    TBool status; //Symbian variable..
    mServer->GetReceptionStatus(status);
    if (status) {
       LOG(NORMAL,"CBSUI: CbsUiTopicListView::init reception ON");
       mReceptionButton->setText(hbTrId("txt_cbs_deactivate"));
    } else {
       LOG(NORMAL,"CBSUI: CbsUiTopicListView::init reception OFF");
       mReceptionButton->setText(hbTrId("txt_cbs_activate"));
    }

    // Update Topic index and Topics   
    reloadTopicIndex();
    reloadTopics();

    // Symbian code, leave etc to be handled.
    // QT_TRAP_THROWING( mMonitor = CCbsUiTopicListMonitor::NewL( *mServer ) );
    // mMonitor->AddObserverL( this );
    // Longpress in topic list, this is needed always
    bool ret(false);

    // conntect topic index list widget operation
    ret = connect(mTopicIndexWidget, 
                SIGNAL(longPressed(HbListWidgetItem *, QPointF)),
            this, SLOT(showTopicIndexMenu(HbListWidgetItem *, QPointF)));
    LOG2(NORMAL,"CBSUI: CbsUiTopicListView::init connect \
            TopicIndex long press %d", ret); 

    ret = connect(mTopicIndexWidget, SIGNAL(activated(HbListWidgetItem *)), 
            this, SLOT(showTopicSelectionView(HbListWidgetItem *)));
    LOG2(NORMAL,"CBSUI: CbsUiTopicListView::init connect \
            TopicIndex short press %d", ret);

    // conntect topics list widget operation
    ret = connect(mTopicsWidget,
            SIGNAL(longPressed(HbListWidgetItem*, QPointF)), 
            this, SLOT(showTopicsMenu(HbListWidgetItem *, QPointF)));
    LOG2(NORMAL,"CBSUI: CbsUiTopicListView::init connect\
             Topics long press %d", ret); 

    ret = connect(mTopicsWidget, SIGNAL(activated(HbListWidgetItem *)), 
            this, SLOT(showTopicView(HbListWidgetItem *)));
    LOG2(NORMAL,"CBSUI: CbsUiTopicListView::init connect \
            topics short press %d", ret);

    // connect reception action
    ret = connect(mReceptionButton, SIGNAL(triggered(bool)), 
            this, SLOT(toggleReception()));  
    LOG2(NORMAL,"CBSUI: CbsUiTopicListView::init connect reception %d", ret);

    // Open
    mOpenAction = qobject_cast<HbAction *>(
    mViewManager->xmlLoader()->findObject(T_OPEN));
    ret = connect(mOpenAction, SIGNAL(triggered()),
            this, SLOT(showTopicView()));
    LOG2(NORMAL,"CBSUI: CbsUiTopicListView::init open %d", ret)

    // Subscribe
    mSubscibeAction = qobject_cast<HbAction *>(
    mViewManager->xmlLoader()->findObject(T_SUBSCRIBE));
    ret = connect(mSubscibeAction, SIGNAL(triggered()),
            this, SLOT(toggleSubscription()));
    LOG2(NORMAL,"CBSUI: CbsUiTopicListView::init subscribe %d", ret)

    // UnSubscribe
    mUnSubscibeAction = qobject_cast<HbAction *>(
    mViewManager->xmlLoader()->findObject(T_UNSUBSCRIBE));
    ret = connect(mUnSubscibeAction, SIGNAL(triggered()),
            this, SLOT(toggleSubscription()));
    LOG2(NORMAL,"CBSUI: CbsUiTopicListView::init subscribe %d", ret)    

    // Hotmark
    mHotmarkAction = qobject_cast<HbAction *>(
    mViewManager->xmlLoader()->findObject(T_HOTMARK));
    ret = connect(mHotmarkAction, SIGNAL(triggered()),
                 this, SLOT(toggleHotmark()));
    LOG2(NORMAL,"CBSUI: CbsUiTopicListView::init hotmark %d", ret)

    // UnHotmark
    mUnHotmarkAction = qobject_cast<HbAction *>(
    mViewManager->xmlLoader()->findObject(T_UNHOTMARK));
    ret = connect(mUnHotmarkAction, SIGNAL(triggered()),
              this, SLOT(toggleHotmark()));
    LOG2(NORMAL,"CBSUI: CbsUiTopicListView::init hotmark %d", ret)

    // Edit
    mEditAction = qobject_cast<HbAction *>(
            mViewManager->xmlLoader()->findObject(T_EDIT));
    ret = connect(mEditAction, SIGNAL(triggered()) , 
            this, SLOT(editTopic()));
    LOG2(NORMAL,"CBSUI: CbsUiTopicListView::init edit %d", ret)

    // Delete
    mDeleteAction =  qobject_cast<HbAction *>(
            mViewManager->xmlLoader()->findObject(T_DELETE));
    ret = connect(mDeleteAction, SIGNAL(triggered()),
                  this, SLOT(deleteTopic()));
    LOG2(NORMAL,"CBSUI: CbsUiTopicListView::init delete %d", ret)

    LOG(NORMAL,"CBSUI: CbsUiTopicListView::init OUT");   
}

// ----------------------------------------------------------------------------
// CbsUiTopicListView::TopicListChangedL
// ---------------------------------------------------------------------------- 
void CbsUiTopicListView::TopicListChangedL(TCbsTopicListEvent /*aEvent*/, 
                                           TCbsTopicNumber /*aTopicNumber*/ )
{
    LOG(NORMAL,"CBSUI: CbsUiTopicListView::TopicListChangedL IN");
    reloadTopicIndex();
    reloadTopics();
    LOG(NORMAL,"CBSUI: CbsUiTopicListView::TopicListChangedL OUT");
}

// ----------------------------------------------------------------------------
// CbsUiTopicListView::createComponents
// ----------------------------------------------------------------------------
void CbsUiTopicListView::createComponents()
{
    LOG(NORMAL,"CBSUI: CbsUiTopicListView::createComponents IN");
    // The topic index widget
    mTopicIndexWidget = qobject_cast<HbListWidget *>( 
        mViewManager->xmlLoader()->findWidget(T_TOPICINDEX));
    LOG2(NORMAL,"CBSUI: CbsUiTopicListView::createComponents\
         mTopicIndexWidget=%d", mTopicIndexWidget)

    // The topic list widget
    mTopicsWidget = qobject_cast<HbListWidget *>( 
        mViewManager->xmlLoader()->findWidget(T_TOPIC_LIST));
    LOG2(NORMAL,"CBSUI: CbsUiTopicListView::createComponents\
         mTopicsWidget=%d", mTopicsWidget)

    // Button for changing reception status
    mReceptionButton = qobject_cast<HbAction *>( 
        mViewManager->xmlLoader()->findObject(T_TOGGLE_RECEPTION));
    LOG2(NORMAL,"CBSUI: CbsUiTopicListView::createComponents\
        mReceptionButton=%d", mReceptionButton)

    // Connect signals of DocML objects
    HbAction *addTopic = qobject_cast<HbAction *>( 
        mViewManager->xmlLoader()->findObject(T_ADD_TOPIC));
    LOG2(NORMAL,"CBSUI: CbsUiTopicListView::createComponents\
         addTopic=%d", addTopic)
    connect(addTopic, SIGNAL(triggered(bool)), this, SLOT(addTopic()));

    HbAction *exit =  qobject_cast<HbAction *>( 
        mViewManager->xmlLoader()->findObject(T_EXIT));
    LOG2(NORMAL,"CBSUI: CbsUiTopicListView::createComponents\
         exit=%d", exit)
    connect(exit, SIGNAL(triggered(bool)), mViewManager, SLOT(close()));

    LOG(NORMAL,"CBSUI: CbsUiTopicListView::createComponents OUT")  
}

// ----------------------------------------------------------------------------
// CbsUiTopicListView::reloadTopicIndex
// ----------------------------------------------------------------------------  
void CbsUiTopicListView::reloadTopicIndex()
{
    LOG(NORMAL,"CBSUI: CbsUiTopicListView::reloadTopicIndex IN")
    if (mTopicIndexWidget && mTopicNumberList) {
        LOG(NORMAL,"CBSUI: CbsUiTopicListView::reloadTopicIndex reload")
        HbListWidgetItem *item = new HbListWidgetItem();

        TCbsTopic topic;
        mServer->GetTopic(0, topic);
        mTopicNumberList->At(0) = 0;

        updateItem(*item, topic);
        mTopicIndexWidget->clear();
        mTopicIndexWidget->addItem(item);  

        // Connect signal to indicate topic selection
        connect(mTopicIndexWidget, SIGNAL(activated(HbListWidgetItem *)),
                this, SLOT(showTopicSelectionView(HbListWidgetItem *)));     
    }
    LOG(NORMAL,"CBSUI: CbsUiTopicListView::reloadTopicIndex OUT")
}

// ----------------------------------------------------------------------------
// CbsUiTopicListView::reloadTopics
// ----------------------------------------------------------------------------  
void CbsUiTopicListView::reloadTopics()
{
    LOG(NORMAL,"CBSUI: CbsUiTopicListView::reloadTopics IN");
    if (mTopicNumberList && mTopicsWidget) {
        int count(0);
        mServer->GetTopicCount(count);   
        LOG2(NORMAL,"CBSUI: CbsUiTopicListView::reloadTopics total count %d",
                count);
        // Reset first
        mTopicNumberList->Reset();
        QT_TRAP_THROWING(mTopicNumberList->AppendL(0));
        mTopicsWidget->clear();

        // Add updated topics one by one
        for (int i = 1; i < count; i++) {
            TCbsTopic topic;
            mServer->GetTopic(i, topic);
            QT_TRAP_THROWING(mTopicNumberList->AppendL(topic.iNumber));
            HbListWidgetItem *item = new HbListWidgetItem();
            updateItem(*item, topic);
            mTopicsWidget->addItem(item);    

            // Connect signal to indicate topic selection
            connect(mTopicsWidget, SIGNAL(activated(HbListWidgetItem *)), 
                    this, SLOT(showTopicView(HbListWidgetItem *)));
        }
    }
    LOG(NORMAL,"CBSUI: CbsUiTopicListView::reloadTopics OUT");
}

// ----------------------------------------------------------------------------
// CbsUiTopicListView::showTopicSelectionView
// ----------------------------------------------------------------------------
void CbsUiTopicListView::showTopicSelectionView(HbListWidgetItem */*item*/)
{
    LOG(NORMAL,"CBSUI: CbsUiTopicListView::showTopicSelectionView IN"); 
    mTopicRow = 0;
    if (mTopicNumberList) {
        TInt messages(0);
        mServer->GetMessageCount(0, messages);
        if (messages) {
            // TODO: show topic selection view
        }
    }
    LOG(NORMAL,"CBSUI: CbsUiTopicListView::showTopicSelectionView OUT");
}

// ----------------------------------------------------------------------------
// CbsUiTopicListView::showTopicView
// ----------------------------------------------------------------------------
void CbsUiTopicListView::showTopicView(HbListWidgetItem *item)
{
    LOG(NORMAL,"CBSUI: CbsUiTopicListView::topicSelection IN");
    mTopicRow = mTopicsWidget->row(item)+1;
    LOG2(NORMAL,"CBSUI: CbsUiTopicListView::topicSelection index=%d", 
            mTopicRow);
    if (mTopicNumberList) {
        TCbsTopicNumber selectTopic(mTopicNumberList->At(mTopicRow));
        TInt messages(0);
        mServer->GetMessageCount(selectTopic, messages);
        if (messages) {
           // it will be reopened
           // mViewManager->activateTopicView(selectTopic);
        }
    }
    LOG(NORMAL,"CBSUI: CbsUiTopicListView::topicSelection OUT");
}

// ----------------------------------------------------------------------------
// CbsUiTopicListView::addTopic
// ----------------------------------------------------------------------------
void CbsUiTopicListView::addTopic()
{
    LOG(NORMAL,"CBSUI: CbsUiTopicListView::addTopic IN");
    // Handle not OK cases, KErrAlreadyExists, KErrArgument, KErrDiskFull
    // Todo: Add from index if it is available
    TCbsTopic topic;
    if (showTopicQuery(topic, true) &&
        KErrNone == mServer->AddTopic(topic)) {
        reloadTopics(); 
    }
    LOG(NORMAL,"CBSUI: CbsUiTopicListView::addTopic OUT");
}


// ----------------------------------------------------------------------------
// CbsUiTopicListView::editTopic
// ----------------------------------------------------------------------------
void CbsUiTopicListView::editTopic()
{
    LOG(NORMAL,"CBSUI: CbsUiTopicListView::editTopic IN");
    LOG2(NORMAL,"CBSUI: CbsUiTopicListView::editTopic mTopicRow=%d", mTopicRow)
    TInt count(0);
    mServer->GetTopicCount(count);
    if (mTopicRow > 0 && mTopicRow < count) {
        // Get topic we need to edit
        TCbsTopic topic;
        mServer->GetTopic(mTopicRow, topic);
        TCbsTopicNumber oldTopicNumber = topic.iNumber;
        TCbsTopicNumber newTopicNumber(EMaxTopicNumber);
        TCbsTopicName newTopicName;
        if (showTopicQuery(topic, false)) {
            newTopicNumber = topic.iNumber;
            newTopicName = topic.iName;
        }
        TInt err = mServer->ChangeTopicNameAndNumber(oldTopicNumber, 
                newTopicNumber, newTopicName);
        if (KErrNone == err) {
            reloadTopics();
        }
    }
    LOG(NORMAL,"CBSUI: CbsUiTopicListView::editTopic OUT");
}

// ----------------------------------------------------------------------------
// CbsUiTopicListView::deleteTopic
// ----------------------------------------------------------------------------
void CbsUiTopicListView::deleteTopic()
    {
    LOG2(NORMAL,"CBSUI: CbsUiTopicListView::deleteTopic IN row %d", mTopicRow);
    TInt count(0);
    mServer->GetTopicCount(count);
    if (mTopicRow > 0 && mTopicRow < count) {
        // user confirmation 
        HbMessageBox *box = new HbMessageBox(hbTrId("txt_cbs_delete_topic"),
                HbMessageBox::MessageTypeQuestion);
        HbAction *action = box->exec();
        if (action == box->primaryAction()) {
            TCbsTopicNumber selectTopic(mTopicNumberList->At(mTopicRow));
            LOG2(NORMAL,"CBSUI: CbsUiTopicListView::deleteTopic topic number %d",
                    selectTopic);
            if (KErrNone == mServer->DeleteTopic(selectTopic)) {
                reloadTopics();
            }
        }
        delete box;
        box = NULL;
    }
    LOG(NORMAL,"CBSUI: CbsUiTopicListView::deleteTopic OUT");
}

// ----------------------------------------------------------------------------
// CbsUiTopicListView::toggleReception
// ----------------------------------------------------------------------------
void CbsUiTopicListView::toggleReception()
{
    LOG(NORMAL,"CBSUI: CbsUiTopicListView::ToggleReception IN");
    TBool status; //Symbian variable..
    mServer->GetReceptionStatus(status);

    if (status) {
        // Deactivate reception and switch the text on UI
        LOG(NORMAL,"CBSUI: CbsUiTopicListView::ToggleReception 1->0");
        mServer->SetReceptionStatus(EFalse);
       
        mReceptionButton->setText( hbTrId("txt_cbs_activate") );
    } else {
        LOG(NORMAL,"CBSUI: CbsUiTopicListView::ToggleReception 0->1");
        // Activate reception and switch the text on UI
        mServer->SetReceptionStatus(ETrue);
        mReceptionButton->setText(hbTrId("txt_cbs_deactivate"));
    }
    LOG(NORMAL,"CBSUI: CbsUiTopicListView::ToggleReception OUT");
}

// ----------------------------------------------------------------------------
// CbsUiTopicListView::toggleHotmark
// ----------------------------------------------------------------------------
void CbsUiTopicListView::toggleHotmark()
{
    LOG(NORMAL,"CBSUI: CbsUiTopicListView::toggleHotmark IN")
    LOG2(NORMAL,"CBSUI: CbsUiTopicListView::toggleHotmark\
         mTopicRow %d", mTopicRow);
    TInt count(0);
    mServer->GetTopicCount(count);
    if (mTopicRow >= 0 && mTopicRow < count) {
        // Update data
        TCbsTopic topic;
        mServer->GetTopic(mTopicRow, topic);
        LOG3(NORMAL,"CBSUI: CbsUiTopicListView::toggleHotmark\
             before toggle hormark (S,H)=(%d,%d)", 
             topic.iSubscribed, topic.iHotmarked);

        if (topic.iHotmarked) {
            mServer->ChangeTopicHotmarkStatus(topic.iNumber, EFalse);
            mServer->ChangeTopicSubscriptionStatus(topic.iNumber, ETrue);
            topic.iHotmarked = EFalse;
            topic.iSubscribed = ETrue;        
        } else {
            if (topic.iSubscribed){
                mServer->ChangeTopicHotmarkStatus(topic.iNumber, ETrue);
                topic.iHotmarked = ETrue;
            } else {
                mServer->ChangeTopicHotmarkStatus(topic.iNumber, ETrue);
                mServer->ChangeTopicSubscriptionStatus(topic.iNumber, ETrue);
                topic.iHotmarked = ETrue;
                topic.iSubscribed = ETrue;
            }
        }
        LOG3(NORMAL,"CBSUI: CbsUiTopicListView::toggleHotmark\
             after toggle hotmark (S,H)=(%d,%d)", 
             topic.iSubscribed, topic.iHotmarked);  
        // Update icon
        updateCurrentItemIcon(topic);
    }
    LOG(NORMAL,"CBSUI: CbsUiTopicListView::toggleHotmark OUT")
}

// ----------------------------------------------------------------------------
// CbsUiTopicListView::toggleSubscription
// ----------------------------------------------------------------------------    
void CbsUiTopicListView::toggleSubscription()
{
    LOG(NORMAL,"CBSUI: CbsUiTopicListView::toggleSubsciption IN")
    LOG2(NORMAL,"CBSUI: CbsUiTopicListView::toggleSubsciption mTopicRow=%d", 
            mTopicRow)
    TInt count(0);
    mServer->GetTopicCount(count);
    if (mTopicRow >= 0 && mTopicRow < count) {
        TCbsTopic topic;
        mServer->GetTopic(mTopicRow, topic);
        LOG2(NORMAL,"CBSUI: CbsUiTopicListView::toggleSubsciption\
             topic number %d", topic.iNumber);
        LOG3(NORMAL,"CBSUI: CbsUiTopicListView::toggleSubscription\
             before toggle subscibe (S,H)=(%d,%d)", 
             topic.iSubscribed, topic.iHotmarked);

        if (topic.iSubscribed) {
            if (topic.iHotmarked) {
                mServer->ChangeTopicHotmarkStatus(topic.iNumber, EFalse);
                topic.iHotmarked = EFalse;
            }
            mServer->ChangeTopicSubscriptionStatus(topic.iNumber, EFalse);
            topic.iSubscribed = EFalse;
        } else {
            mServer->ChangeTopicSubscriptionStatus(topic.iNumber, ETrue);
            topic.iSubscribed = ETrue;
        }
        LOG3(NORMAL,"CBSUI: CbsUiTopicListView::toggleSubscription\
             after toggle subscibe (S,H)=(%d,%d)", 
             topic.iSubscribed, topic.iHotmarked);
        // Update icon
        updateCurrentItemIcon(topic);
    }
    LOG(NORMAL,"CBSUI: CbsUiTopicListView::toggleSubsciption OUT");
}

// ----------------------------------------------------------------------------
// CbsUiTopicListView::showTopicIndexMenu
// ----------------------------------------------------------------------------  
void CbsUiTopicListView::showTopicIndexMenu(HbListWidgetItem *item, QPointF coords)
{
    LOG(NORMAL,"CBSUI: CbsUiTopicListView::showTopicIndexMenu IN");
    Q_UNUSED(item);
    mTopicRow = 0;
    TCbsTopic topic;
    mServer->GetTopic(0, topic);
    LOG3(NORMAL,"CBSUI: CbsUiTopicListView::showTopicIndexMenu (row %d: %d)",
            mTopicRow, topic.iNumber);    
    showPopupMenu(topic, coords);
    LOG(NORMAL,"CBSUI: CbsUiTopicListView::showTopicIndexMenu OUT");
}

// ----------------------------------------------------------------------------
// CbsUiTopicListView::showTopicsMenu
// ----------------------------------------------------------------------------  
void CbsUiTopicListView::showTopicsMenu(HbListWidgetItem *item, QPointF coords)
{
    LOG(NORMAL,"CBSUI: CbsUiTopicListView::showTopicsMenu IN");
    mTopicRow = mTopicsWidget->row(item)+1;
    TCbsTopic topic;
    mServer->GetTopic(mTopicRow, topic);
    LOG3(NORMAL,"CBSUI: CbsUiTopicListView::showTopicsMenu (row %d: %d)",
            mTopicRow, topic.iNumber);
    showPopupMenu(topic, coords);
    LOG(NORMAL,"CBSUI: CbsUiTopicListView::showTopicsMenu OUT");
}

// ----------------------------------------------------------------------------
// CbsUiTopicListView::showPopupMenu
// ----------------------------------------------------------------------------
void CbsUiTopicListView::showPopupMenu(const TCbsTopic &topic, QPointF coords)
{
    LOG(NORMAL,"CBSUI: CbsUiTopicListView::showPopupMenu IN");
    HbMenu *contextMenu = new HbMenu();
    contextMenu->setTimeout(HbPopup::NoTimeout);
    contextMenu->setDismissPolicy(HbPopup::TapOutside);

    TInt messageCount(0);
    mServer->GetMessageCount(topic.iNumber, messageCount);
    LOG2(NORMAL,"CBSUI: CbsUiTopicListView::showPopupMenu\
         messageCount=%d", messageCount)

    // Open
    if (topic.iNumber && messageCount) {
        contextMenu->addAction(mOpenAction);
    }
    // Subscribe / UnSubscribe
    if (topic.iSubscribed) {
        contextMenu->addAction(mUnSubscibeAction);
    } else {
        contextMenu->addAction(mSubscibeAction);
    }
    // Hotmark / UnHotmark
    if (topic.iHotmarked) {
        contextMenu->addAction(mUnHotmarkAction);
    } else {
        contextMenu->addAction(mHotmarkAction);
    }
    // Edit & Delete
    if (topic.iNumber) {
        contextMenu->addAction(mEditAction);
        contextMenu->addAction(mDeleteAction);
    }

    // Show context sensitive menu. 
    contextMenu->exec(coords);
    contextMenu->clearActions();
    delete contextMenu;
    contextMenu = NULL;
    LOG(NORMAL,"CBSUI: CbsUiTopicListView::showPopupMenu OUT")
}

// ----------------------------------------------------------------------------
// CbsUiTopicListView::updateQueryAction
// ----------------------------------------------------------------------------
//
void CbsUiTopicListView::updateQueryAction( QString text )
{
    LOG(NORMAL, "SATAPP: CbsUiTopicListView::updateQueryAction IN" )
    LOG2(NORMAL, "SATAPP: CbsUiTopicListView::updateQueryAction text=%s", 
        text.utf16())
    if (mTopicQuery && text.length() > 0) {
        mTopicQuery->primaryAction()->setEnabled(true);
        LOG(NORMAL, "SATAPP: CbsUiTopicListView::updateQueryAction enable OK")
    }
    LOG(NORMAL, "SATAPP: CbsUiTopicListView::updateQueryAction OUT" )
}

// ----------------------------------------------------------------------------
// CbsUiTopicListView::updateItem
// ----------------------------------------------------------------------------  
void CbsUiTopicListView::updateItem(HbListWidgetItem &item, 
        const TCbsTopic &topic)
{
    LOG(NORMAL, "SATAPP: CbsUiTopicListView::updateItem IN" )
    updateItemText(item, topic);
    updateItemIcon(item, topic);
    LOG(NORMAL, "SATAPP: CbsUiTopicListView::updateItem OUT" )
}

// ----------------------------------------------------------------------------
// CbsUiTopicListView::updateItemIcon
// ----------------------------------------------------------------------------  
void CbsUiTopicListView::updateItemIcon(HbListWidgetItem &item, 
        const TCbsTopic &topic)
{
    LOG(NORMAL,"CBSUI: CbsUiTopicListView::updateItemIcon IN");
    // TODO: what is the correct way to handle graphics (SVG?)

    if (topic.iSubscribed) {
        if (topic.iHotmarked) {
            item.setDecorationIcon(HbIcon(":/icons/icons/hotmark2.png"));
            LOG(NORMAL,"CBSUI: CbsUiTopicListView::updateItemIcon Hotmarked");
        } else {
            item.setDecorationIcon(HbIcon(":/icons/icons/sub2.png"));
            LOG(NORMAL,"CBSUI: CbsUiTopicListView::updateItemIcon Subscribed");
        }
    } else {
          // No icon if not subscribted:
          // List item without an icon is alligned on left (looks bad)
          // temporary solution is to use empty icon
          item.setDecorationIcon(HbIcon(":/icons/icons/empty.png"));
          LOG(NORMAL,"CBSUI: CbsUiTopicListView::updateItemIcon Empty");
    } 
    LOG(NORMAL,"CBSUI: CbsUiTopicListView::updateItemIcon OUT");
}

// ----------------------------------------------------------------------------
// CbsUiTopicListView::updateItemText
// ----------------------------------------------------------------------------  
void CbsUiTopicListView::updateItemText(HbListWidgetItem &item, 
        const TCbsTopic &topic)
{
    LOG(NORMAL,"CBSUI: CbsUiTopicListView::updateItemText IN");
    QString txtL1;
    QString txtL2;
    if (0 == topic.iNumber) {
        LOG(NORMAL,"CBSUI: CbsUiTopicListView::updateItemText topic index");
        txtL1 = hbTrId("txt_cbs_000_topic_index");
        txtL2 = hbTrId("txt_cbs_not_available");
    } else {
        LOG(NORMAL,"CBSUI: CbsUiTopicListView::updateItemText topics");
        txtL1 = QVariant(topic.iNumber).toString();
        txtL2 = hbTrId("txt_cbs_no_messages");
        // instert zeros to topic number in case 2 or 1 digit topic number.
        switch (txtL1.length()) {
            case 1:
                {
                txtL1.insert(0, "0");
                txtL1.insert(0, "0");
                break;
                }
            case 2:
                {
                txtL1.insert(0, "0");
                break;
                }
            default:
                {
                break;
                }
            // do nothing
            }

        txtL1.append(' '); // a space
        QString name = QString::fromUtf16(topic.iName.Ptr(), topic.iName.Length());
        txtL1.append(name);
        if (topic.iUnreadMessages) {
            txtL1.append(" ("); // space + (
            txtL1.append(QVariant(topic.iUnreadMessages).toString());
            txtL1.append(')');  // closing )
        } 
        // Update the status of topic
        TInt numberOfMessages(0);
        mServer->GetMessageCount(topic.iNumber, numberOfMessages);
        if (numberOfMessages) {
            TCbsMessage message;
            TBuf<KMaxMessageTitleLength> buffer;  // symbian code
            // take the last message, not sure is this correct
            TInt err = mServer->GetMessage(topic.iNumber,
                    numberOfMessages - 1, message);
            if (KErrNone == err) {
                err = mServer->GetMessageContents(message.iHandle, buffer);
                txtL2 = QString::fromUtf16(buffer.Ptr(), buffer.Length()); 
                // TODO: timestamp and second text line   
            }
        }
    }
    item.setText(txtL1);
    item.setSecondaryText(txtL2);
    LOG(NORMAL,"CBSUI: CbsUiTopicListView::updateItemText OUT");
}

// ----------------------------------------------------------------------------
// CbsUiTopicListView::updateCurrentItem
// ----------------------------------------------------------------------------  
void CbsUiTopicListView::updateCurrentItem(const TCbsTopic &topic)
{
    LOG(NORMAL, "SATAPP: CbsUiTopicListView::updateCurrentItem IN" )
    LOG2(NORMAL, "SATAPP: CbsUiTopicListView::updateCurrentItem row=%d",
        mTopicRow)
    updateCurrentItemIcon(topic);
    updateCurrentItemText(topic);
    LOG(NORMAL, "SATAPP: CbsUiTopicListView::updateCurrentItem OUT" )
}

// ----------------------------------------------------------------------------
// CbsUiTopicListView::updateCurrentItemIcon
// ----------------------------------------------------------------------------  
void CbsUiTopicListView::updateCurrentItemIcon(const TCbsTopic &topic)
{
    LOG(NORMAL, "SATAPP: CbsUiTopicListView::updateCurrentItemIcon IN" )
    LOG2(NORMAL, "SATAPP: CbsUiTopicListView::updateCurrentItemIcon row=%d",
        mTopicRow)    
    HbListWidgetItem *list = NULL;
    if (0 == mTopicRow) {
        list = mTopicIndexWidget->item(0); 
    } else {
        list = mTopicsWidget->item(mTopicRow - 1);
    }
    updateItemIcon(*list, topic);
    LOG(NORMAL, "SATAPP: CbsUiTopicListView::updateCurrentItemIcon OUT" )
}

// ----------------------------------------------------------------------------
// CbsUiTopicListView::updateCurrentItemText
// ----------------------------------------------------------------------------  
void CbsUiTopicListView::updateCurrentItemText(const TCbsTopic &topic)
{
    LOG(NORMAL, "SATAPP: CbsUiTopicListView::updateCurrentItemText IN" )
    LOG2(NORMAL, "SATAPP: CbsUiTopicListView::updateCurrentItemText row=%d",
        mTopicRow)
    HbListWidgetItem *list = NULL;
    if (0 == mTopicRow) {
        list = mTopicIndexWidget->item(0); 
    } else {
        list = mTopicsWidget->item(mTopicRow - 1);
    }
    updateItemText(*list, topic);
    LOG(NORMAL, "SATAPP: CbsUiTopicListView::updateCurrentItemText OUT" )
}

// ----------------------------------------------------------------------------
// CbsUiTopicListView::showTopicQuery
// ----------------------------------------------------------------------------  
bool CbsUiTopicListView::showTopicQuery(TCbsTopic &topic, const bool isAddTopic)
{
    LOG(NORMAL,"CBSUI: CbsUiTopicListView::showTopicQuery IN");
    bool ret = false; // user didn't click OK
    // Compose dialog
    mTopicQuery = new HbInputDialog();
    mTopicQuery->setSecondRowVisible(true);
    // Name
    mTopicQuery->setInputMode(HbInputDialog::TextInput, 0);
    mTopicQuery->lineEdit(0)->setMaxLength(KMaxTopicNameLength);
    QString name = QString::fromUtf16(topic.iName.Ptr(), topic.iName.Length()); 
    if (name.length()) {
        mTopicQuery->setTextValue(QVariant(name), 0);
        LOG2(NORMAL,"CBSUI: CbsUiTopicListView::showTopicQuery old name %s",
                name.utf16());
    }

    // Number
    mTopicQuery->setInputMode(HbInputDialog::IntInput, 1);
    mTopicQuery->lineEdit(1)->setMaxLength(3);
    int number = topic.iNumber;
    if (number){
        mTopicQuery->setTextValue(QVariant(number), 1);
        LOG2(NORMAL,"CBSUI: CbsUiTopicListView::showTopicQuery old number%d", number);
    }

    mTopicQuery->setPromptText(hbTrId("txt_cbs_topic_name"), 0);
    mTopicQuery->setPromptText(hbTrId("txt_cbs_topic_number"), 1);

    // Disable OK button if invalid topic number input
    HbAction *perimaryAction = mTopicQuery->primaryAction();
    perimaryAction->setEnabled(false);
    connect(mTopicQuery->lineEdit(1), SIGNAL(textChanged(QString)), 
        this, SLOT( updateQueryAction(QString)));

    // Add validation for topic number (1-999)
    HbValidator *val =new HbValidator;
    QRegExp r("\\d{1,3}"); // from 1 to 3 digits
    val->setMasterValidator(new QRegExpValidator(r,0));
    val->addField(new QIntValidator(1,999,0),"");
    mTopicQuery->setValidator(val, 1);

    // Show the dialog
    HbAction *action = mTopicQuery->exec();

    // Fatch name and number from dialog
    name = mTopicQuery->value(0).toString();
    LOG2(NORMAL,"CBSUI: CbsUiTopicListView::showTopicQuery name=%s ", name.utf16());
    number = mTopicQuery->value(1).toString().toInt();
    LOG2(NORMAL,"CBSUI: CbsUiTopicListView::showTopicQuery number=%d ", number);
    bool isNumberExist(false);
    if (isAddTopic) {
        int count(0);
        mServer->GetTopicCount(count);   
        LOG2(NORMAL,"CBSUI: CbsUiTopicListView::showTopicQuery topic total count %d", 
                count);     
        // Check if there is a toipc number same as number which user add
        for (int i = 1; i < count; i++) {
            TCbsTopic currentTopic;
            mServer->GetTopic(i, currentTopic);
            if (currentTopic.iNumber == number) {
            isNumberExist = true;
            break;
            }     
        }      
    }

    // check that a topic number was entered and ok pressed
    // name is optional
    if (action == mTopicQuery->primaryAction()) {
        if ( isNumberExist || !number){
            // 000 topic and exist number topic can't be edited
            showNumberInUsePopup();
            showTopicQuery( topic, isAddTopic);
        } else {
            ret = true;
            LOG(NORMAL,"CBSUI: CbsUiTopicListView::showTopicQuery OK")
            topic.iNumber = number;
            topic.iSubscribed = ETrue;
            topic.iHotmarked = EFalse;
            if (name.length()) {
                topic.iName = name.utf16();
            }       
        }
    }
    delete mTopicQuery;
    mTopicQuery = NULL;

    LOG(NORMAL,"CBSUI: CbsUiTopicListView::showTopicQuery OUT");
    return ret;
}

// ----------------------------------------------------------------------------
// CbsUiTopicListView::showNumberInUsePopup
// ----------------------------------------------------------------------------  
void CbsUiTopicListView::showNumberInUsePopup()
{
    LOG(NORMAL,"CBSUI: CbsUiTopicListView::showNumberInUsePopup IN");
    HbMessageBox *inUsedPopup = new HbMessageBox(HbMessageBox::MessageTypeWarning);
    inUsedPopup->setText(tr("topic number already in use "));
    inUsedPopup->exec();
    delete inUsedPopup;
    inUsedPopup = NULL;
    LOG(NORMAL,"CBSUI: CbsUiTopicListView::showNumberInUsePopup OUT");
}

// End of file
