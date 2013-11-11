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

#ifndef CBSUITOPICVIEW_H
#define CBSUITOPICVIEW_H

#include <hbview.h>
#include <hbaction.h>
#include <CbsCommonTypes.h>
#include "mcbsuitopicmonitorobserver.h"

class HbListWidget;
class MCbs;
class CbsUiViewManager;

class CbsUiTopicView : public HbView,
                       public MCbsUiTopicMonitorObserver
{
    Q_OBJECT

public:

    /**
     * Constructor
     */
    CbsUiTopicView();
 
     /**
     * Destructor
     */
    ~CbsUiTopicView();

     /**
     * Constructor
     * @param server The handler of connecting to Cbs server.
     * @param viewManager  CbsUiViewManager.
     */
    void init(MCbs &server, CbsUiViewManager &viewManager);
    
public: // From MCbsUiTopicMonitorObserver 
    /**
    * Topic list or topic messages are changed.
    *
    * @param aEvent reason for event
    * @param aTopicNumber Topic number, which caused the event
    */    
    virtual void TopicListChangedL(TCbsTopicListEvent aEvent, 
                                    TCbsTopicNumber aTopicNumber);

    /**
    * Reload Messages
    *
    * @param topicNumber topic number
    */                                        
    void reloadMessages(TCbsTopicNumber topicNumber);
    
private:
    /**
    * It defines the type for message handle lists.
    */
    //typedef CArrayFixFlat< TCbsMessageHandle > TCbsUiMessageHandleList; 
    
    void createComponents();    
    void addMessage(const TCbsMessage &message, const QString &msgName);
    
private: // Data
    // Not own
    MCbs *mServer;
    
    // Not own
    CbsUiViewManager *mViewManager;
    
    // Own
    HbListWidget *mTopic;
    
    //TCbsUiMessageHandleList* mMsgHandles;
    TCbsTopicNumber mCurrentTopicNumber;
   
    // Previous Message(not own)
    HbAction *mPrevious;

    //Previous Message(not own)
    HbAction *mBack;  
};

#endif // CBSUITOPICVIEW_H
