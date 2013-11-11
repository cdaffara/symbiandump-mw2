/*
* Copyright (c) 2003 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  Virtual class defining an interface for an observer which gets
*                notified when the topic list is changed. 
*
*/



#ifndef     MCBSDBTOPICLISTOBSERVER_H
#define     MCBSDBTOPICLISTOBSERVER_H

//  INCLUDES

#include <e32base.h>
#include "CbsCommon.h"

//  CLASS DECLARATION 

/**
*   This class defines the interface for topic list observers.
*/
class MCbsDbTopicListObserver
    {
    public:        // New functions
        /**
        *   Indicates that the topic list has been initialized.
        */
        virtual void TopicListInitializedIndL() = 0;

        /**
        *   Indicates that a new topic has been added.
        *
        *   @param  aNumber         Number of the topic that has been added
        */
        virtual void TopicAddedIndL( const TCbsDbTopicNumber& aNumber ) = 0;

        /**
        *   Indicates that a topic has been deleted.
        *
        *   @param  aNumber         Number of the topic that was deleted.
        */
        virtual void TopicDeletedIndL( const TCbsDbTopicNumber& aNumber ) = 0;

        /**
        *   Indicates that a topic has been modified.
        *
        *   @param  aNumber         Number of the topic that has been modified.
        */
        virtual void TopicModifiedIndL( const TCbsDbTopicNumber& aNumber ) = 0;

        /**
        *   Indicates that a topic has received a new message.
        *
        *   @param  aNumber         Number of the topic that has received 
        *                           a new message.
        */
        virtual void TopicNewMessageReceivedIndL( 
            const TCbsDbMessageHandle& aNumber ) = 0;
    };

#endif      //  MCBSDBTOPICLISTOBSERVER_H   
            
// End of File
