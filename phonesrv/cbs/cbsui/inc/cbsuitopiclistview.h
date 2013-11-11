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

#ifndef CBSUITOPICLISTVIEW_H
#define CBSUITOPICLISTVIEW_H

#include <hbview.h>
#include <CbsCommon.h>
#include <CbsCommonTypes.h>

#include "mcbsuitopicmonitorobserver.h"

// CONSTANTS  
const TInt KNumberOfStandardTopics = 17; //TODO what's this

class HbListWidget;
class HbAction;
class CCbsUiTopicListMonitor;
class CbsUiViewManager;
class MCbs;
class HbInputDialog;
class HbListWidgetItem;
class QPointF;

typedef CArrayFixFlat< TCbsTopicNumber > TCbsUiTopicNumberList;

class CbsUiTopicListView: public HbView,
                          public MCbsUiTopicMonitorObserver
{
    Q_OBJECT
public:
    /**
     * Constructor
     */
    CbsUiTopicListView();

    /**
     * Destructor
     */
    ~CbsUiTopicListView();

    /**
    * Init.
    *
    * @param server MCbs reference from CbsUiMainWindow
    * @param viewManager CbsUiViewManager reference from CbsUiMainWindow
    */    
    void init(MCbs &server, CbsUiViewManager &viewManager);

public: // from  MCbsUiTopicMonitorObserver
    /**
    * Topic list or topic messages are changed.
    *
    * @param aEvent reason for event
    * @param aTopicNumber Topic number, which caused the event
    */
    virtual void TopicListChangedL(TCbsTopicListEvent aEvent, 
                                   TCbsTopicNumber aTopicNumber);

private slots:

    /**
     * Connect with left toolbar
     */
    void toggleReception();

    /**
     * Connect with popup menu hotmark action
     */
    void toggleHotmark();

    /**
     * Connect with popup menu Subscription action
     */
    void toggleSubscription();

    /**
     * Connect with add topic toolbar
     */
    void addTopic();

    /**
     * Connect with popup menu delete action
     */
    void deleteTopic();

    /**
     * Connect with popup menu edit action
     */
    void editTopic();

    /**
     * Connect with short press
     * 
     * @param item selection item
     */
    void showTopicSelectionView(HbListWidgetItem *item);

    /**
     * showTopicView
     * 
     * @param item selection item
     */
    void showTopicView(HbListWidgetItem *item);

    /**
     * showTopicIndexMenu(000)
     * 
     * @param item selection item
     * @param coords availalbe menu show position
     */
    void showTopicIndexMenu(HbListWidgetItem *item, QPointF coords);

    /**
     * ShowTopicsMenu(1-999)
     * 
     * @param item selection item
     * @param coords availalbe menu show position
     */
    void showTopicsMenu(HbListWidgetItem *item, QPointF coords);

    /**
     * Connect with add / edit popup dialog to en/dis able OK button
     */
    void updateQueryAction(QString text);

private:

    /**
     * createComponents
     */
    void createComponents();

    /**
     * Reload topic index data and update topic index widget. (000)
     */
    void reloadTopicIndex();

    /**
     * Reload topics data and update topics widget (1-999)
     */
    void reloadTopics();

    /**
     * Update given item
     * 
     * @param item selection item
     * @param topic selection topic
     */
    void updateItem(HbListWidgetItem &item, const TCbsTopic &topic);

    /**
     * UpdateItemIcon
     * 
     * @param item selection item
     * @param topic selection topic
     */
    void updateItemIcon(HbListWidgetItem &item, const TCbsTopic &topic);

    /**
     * updateItemText
     * 
     * @param item selection item
     * @param topic selection topic
     */
    void updateItemText(HbListWidgetItem &item, const TCbsTopic &topic);

    /**
     * Update current item by mTopicRow
     * 
     * @param topic selection topic
     */
    void updateCurrentItem(const TCbsTopic &topic);

    /**
     * UpdateCurrentItemIcon, (un)subcribe and (un)hotmark
     * 
     * @param topic selection topic
     */
    void updateCurrentItemIcon(const TCbsTopic &topic);

    /**
     * UpdateCurrentItemText
     * 
     * @param topic selection topic
     */
    void updateCurrentItemText(const TCbsTopic &topic);

    /**
     * ShowPopupMenu
     * 
     * @param topic selection topic
     * @param coords availalbe menu show position
     */
    void showPopupMenu(const TCbsTopic &topic, QPointF coords);

    /**
     * Show current topic query dialog
     * (edit/add current topic number or name)
     *    
     * @param topic selection topic
     * @param isAddTopic check if add or edit mode
     */
    bool showTopicQuery(TCbsTopic &topic, const bool isAddTopic);

    /**
     * Show topic number in use popup when edit topic
     */
    void showNumberInUsePopup();

private: // Data
    /**
     * Not own, from CbsUiMainWindow
     */
    MCbs *mServer;
    /**
     * Not own, from CbsUiMainWindow
     */
    CbsUiViewManager *mViewManager;
    
    int mTopicRow; 

    /**
     * Not own, load from docml
     */
    HbListWidget *mTopicIndexWidget;

    /**
     * Not own, load from docml
     */
    HbListWidget *mTopicsWidget;

    /**
     * Not own, load from docml
     */
    HbAction *mReceptionButton;

    /**
     * Not own, load from docml
     */
    HbAction *mOpenAction;

    /**
     * Not own, load from docml
     */
    HbAction *mSubscibeAction;

    /**
     * Not own, load from docml
     */
    HbAction *mUnSubscibeAction;

    /**
     * Not own, load from docml
     */
    HbAction *mHotmarkAction;

    /**
     * Not own, load from docml
     */
    HbAction *mUnHotmarkAction;

    /**
     * Not own, load from docml
     */
    HbAction *mEditAction;

    /**
     * Not own, load from docml
     */
    HbAction *mDeleteAction;    
    
    //Monitors changes in topic list (Symbian).
    CCbsUiTopicListMonitor *mMonitor;

    /**
     * Own, for add / edit topic
     */
    HbInputDialog *mTopicQuery;

    /**
     * Own
     */
    TCbsUiTopicNumberList *mTopicNumberList;
};

#endif // CBSUITOPICLISTVIEW_H
