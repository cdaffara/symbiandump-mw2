/*
* Copyright (c) 2007 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  This file contains the header file of the CCbsMessageClientImpl class.
*
*
*/



#ifndef     CCBSMESSAGECLIENTIMPL_H
#define     CCBSMESSAGECLIENTIMPL_H

// INCLUDES
#include <e32base.h>
#include <ccbsmessageclient.h>
#include "RCbs.h"

//  CLASS DECLARATION 

/**
*   Implementation of CCbsMessageClient interface.
*
*   @lib CbsClient.lib
*   @since 3.2
*/
NONSHARABLE_CLASS( CCbsMessageClientImpl ) : public CCbsMessageClient
    {
    public:     // Construction
    
        /**
        *   Instantiates new CCbsMessageClientImpl object and establishes a session with
        *   CBS server. 
        *   
        *   Active scheduler has to be installed prior to calling this function.
        *
        *   @return                     A new instance of CCbsMessageClientImpl
        */
        static CCbsMessageClientImpl* NewL();

        /**
        *   Destructor.
        */
        ~CCbsMessageClientImpl();

    public:     // From base class
    
        /**
        *   Finds a message by given handle.
        *
        *   @see CCbsMessageClient::FindMessageByHandle
        */
        TInt FindMessageByHandle( 
            const TCbsMessageHandle& aHandle, 
            TCbsMessage& aMessage );
                
        /**
        *   Finds the topic by the given number.
        *
        *   @see CCbsMessageClient::FindTopicByNumber
        */
        TInt FindTopicByNumber( TCbsTopicNumber aNumber,
            TCbsTopic& aTopic );

        /**
        *   Returns the total amount of topics the topic list contains.
        *
        *   @see CCbsMessageClient::TopicCount
        */        
        TInt TopicCount();

        /**
        *   Returns information about a topic from the topic list.
        *
        *   @see CCbsMessageClient::GetTopic
        */
        TInt GetTopic( const TInt aIndex, TCbsTopic& aTopic );
        
        /**
        *   Returns the total amount of unread messages.
        *
        *   @see CCbsMessageClient::UnreadMessageCount
        */
        TInt UnreadMessageCount();
        
        /**
        *   Returns the total amount of messages the topic contains.    
        *
        *   @see CCbsMessageClient::GetMessageCount
        */
        TInt GetMessageCount( TCbsTopicNumber aNumber,
            TInt& aCount );
                            
        /**
        *   Returns message information.
        *
        *   @see CCbsMessageClient::GetMessage
        */
        TInt GetMessage( TCbsTopicNumber aNumber, TInt aIndex,
            TCbsMessage& aMessage );
            
        /**
        *   Returns the message contents.
        *
        *   @see CCbsMessageClient::GetMessageContents
        */
        TInt GetMessageContents( 
            const TCbsMessageHandle& aHandle, 
            TDes& aBuffer );       
            
        /**
        *   Sets the message as read.
        *
        *   @see CCbsMessageClient::SetMessageAsRead
        */
        TInt SetMessageAsRead( const TCbsMessageHandle& aHandle );
                        
    private:
    
        /**
        *   Default constructor.
        */
        CCbsMessageClientImpl();
        
        /**
        *   2nd-phase constructor
        */
        void ConstructL();  
        
    private:    // Data
    
        // CBS Client API
        RCbs iCbsClient;        
    };
    
    

#endif      //  CCBSMESSAGECLIENTIMPL_H   
            
// End of File


