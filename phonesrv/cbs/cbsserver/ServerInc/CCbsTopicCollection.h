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
* Description:  This file contains the header file of the CCbsTopicCollection class.
*    
*                The server-side topic collection subsession class.
*
*/



#ifndef     CCBSTOPICCOLLECTION_H
#define     CCBSTOPICCOLLECTION_H

//  INCLUDES

#include "CCbsObject.h"
#include "MCbsDbTopicCollectionObserver.H"

//  FORWARD DECLARATIONS

class CCbsSession;
class CCbsDbImpTopicCollection;
class CCbsDbImpTopicList;

//  CLASS DECLARATION 

/**
*   CCbsTopicCollection is the server-side subsession object for handling
*   CbsClient requests on topic identities of index messages.
*/
class CCbsTopicCollection : public CCbsObject, 
    public MCbsDbTopicCollectionObserver
    {
    public:     // New functions

        /**
        *   Creates a new instance of the class.
        *
        *   @param  aSession        Session.
        *   @param  aCollection     Topic collection manager.
        *   @param  aList           Current topic list.
        *   @return                 The new instance of the class.
        */
        static CCbsTopicCollection* NewL( CCbsSession& aSession, 
            CCbsDbImpTopicCollection& aCollection, 
            CCbsDbImpTopicList& aList );

        /**
        *   Destructor.
        */
        ~CCbsTopicCollection();

    public:     // From CCbsObject
        
        /**
        *   Handles the requests for the subsession.
        *
        *   @param  aMessage        Message of the client
        *   @return                 ETrue, if the request was handled here
        */
        TBool HandleRequestsL( const RMessage2& aMessage );

    private:        // From MCbsDbTopicCollectionObserver

        /**
        *   Called whenever topic collection contents are changed.
        */
        void TopicCollectionContentsChangedInd();

    private:

        /**
        *   Constructor.
        *
        *   @param  aSession        Session
        *   @param  aCollection     Topic collection
        *   @param aList            Current topic list
        */
        CCbsTopicCollection( CCbsSession& aSession, 
            CCbsDbImpTopicCollection& aCollection, 
            CCbsDbImpTopicList& aList );

        /**
        *   Finalizes the construction.
        */
        void ConstructL();

        /**
        *   Closes the subsession.
        */
        void CloseCollection();

        /**
        *   Returns the total amount of topic info records.
        */
        void GetTopicCountL();

        /**
        *   Returns a topic info structure to the client.
        */
        void GetTopicInfoL();

        /**
        *   Returns a truth value determining if the given topic already 
        *   exists in the current topic list.
        */
        void TopicInListL();

        /**
        *   Sets up a notification so that the client will be notified when 
        *   the collecton is changed.
        */
        void NotifyOnChange();

        /**
        *   Cancels the pending request.
        */
        void NotifyOnChangeCancel();

        /**
        *   Notifies the client
        */
        void Notify();

    private:    // Prohibited operators and functions

        // Copy constructor
        CCbsTopicCollection( const CCbsTopicCollection& );

        // Assignment operator
        CCbsTopicCollection& operator=( const CCbsTopicCollection& );

    private:    // Data

        // Topic collection manager.
        CCbsDbImpTopicCollection& iCollection;

        /**
        *   The current topic list which is needed to tell client
        *   only of topics not included in the topic list.
        */
        CCbsDbImpTopicList& iTopicList;
        
        // Indicates whether there is a pending notify request.
        TBool iIsMessage;

        // In case there is a pending notify request, this is the request.
        RMessage2 iMessage;

    };

#endif      //  CCBSTOPICCOLLECTION_H
            
// End of File


