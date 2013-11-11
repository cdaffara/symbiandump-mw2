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
* Description:  This file contains the header file of the CCbsDbImpTopicCollection class.
*    
*                This class represents the topic collection stored in the database.
*                Topic collection keeps a record of topic identities received from
*                the network. Topic identity includes a topic name and a topic number. 
*                These can be accessed but not modified.
*
*/


#ifndef     CCBSDBIMPTOPICCOLLECTION_H   
#define     CCBSDBIMPTOPICCOLLECTION_H   

//  INCLUDES

#include <e32base.h>
#include <e32std.h>
#include <s32stor.h>
#include <f32file.h>
#include <s32file.h>

#include "MCbsDbTopicCollectionObserver.H"

//  CLASS DECLARATION 

class CCbsDbImp;

/**
*   This class defines the interface for handling topic collections.
*
*   User's Guide for accessing stored topic identities:
*   GetTopicIdentityCount(TInt&)
*       Retrieves the number of topic identities currently stored in the 
*       topic collection.
*   GetTopicIdentityL(TInt&, TCbsDbTopicIdentity&)
*       Retrieves a single topic identity identified with an index.
*
*   User's Guide for observing the topic collection.
*   AddObserverL(MCbsDbTopicCollectionObserver*)
*       Registers an observer which is notified of changes in the topic
*       collection.
*   RemoveObserverL(MCbsDbTopicCollectionObserver*)
*       Unregisters an observer.
*
*   User's Guide for modifying the topic collection:
*   Clear()
*       Clears the current topic collection. This is called by the index
*       message parsing routine when it's about to parse a root index
*       message. Topic identities contained in subsequent child subindex 
*       messages, if any, are then simply added with AddTopicIdentityL()
*   AddTopicIdentityL(TCbsDbTopicIdentity&)
*       Adds a topic identity into the topic collection's buffer.
*   Apply()
*       Writes the topic identity buffer into persistent storage.
*       Clears the buffer.
*   
*/
class CCbsDbImpTopicCollection : public CBase
    {

    public:     // New functions.
        /**
        *   Creates a new instance of the class.
        *       
        *   @return                     A pointer to the created 
        *                               CCbsDbImpTopicCollection instance.
        */
        static CCbsDbImpTopicCollection* NewL();

        /**
        *   Destructor. 
        */
        ~CCbsDbImpTopicCollection();

    public: 
        /**
        *   Returns the total amount of topic identities 
        *   the collection contans.
        *
        *   @param aCount               The method stores the topic identity
        *                               count to this variable.
        */
        void GetTopicIdentityCount( TInt& aCount ) const;

        /**
        *   Stores a topic identity (index is aIndex in the list) to aIdentity.
        *   The method leaves if the parameter aIndex is invalid.
        *
        *   Exceptions:
        *   KErrNotFound                The requested topic identity was not 
        *                               found.
        *
        *   @param aIndex               Used to index identities.
        *   @param aIdentity            The method stores a topic identity to 
        *                               the parameter.
        */
        void GetTopicIdentityL( 
            TInt& aIndex, 
            TCbsDbTopicIdentity& aIdentity ) const;

        /**
        *   Adds a topic collection observer. 
        *
        *   Panics if aObserver is null.
        *
        *   Exceptions:
        *   KErrNoMemory                OOM
        *
        *   @param aObserver            Pointer to the new observer to be added
        */
        void AddObserverL( MCbsDbTopicCollectionObserver* aObserver );

        /**
        *   Removes a topic collection observer. 
        *
        *   Panics if aObserver is null or aObserver is not registered
        *   as an observer.
        *
        *   @param aObserver            Pointer to the observer to be removed
        */
        void RemoveObserver( const MCbsDbTopicCollectionObserver* aObserver );

        /**
        *   Clears the topic collection builder's buffered topic list.
        *   This does not have any effect on identities stored in 
        *   the database.
        *
        *   This method should be called prior to parsing a root index
        *   message. Any topic identities extracted from subindex messages
        *   should be appended into collection with AddTopicIdentityL()
        *   without any call to Clear().
        */
        void Clear();

        /**
        *   Stores topic identities which were added to the topic 
        *   collection with AddTopicIdentityL() after previous call to 
        *   Apply().
        *
        */
        void Apply();

        /**
        *   Adds a new topic identity to the buffered collection.
        *   A topic identity contains topic name and number. If the
        *   topic exists already, nothing is done.
        *   
        *   This method will return KErrAlreadyExists, if there is already 
        *   another topic identity that has the same number.
        *
        *   Exceptions:
        *   KErrNoMemory                OOM
        *   KErrArgument                Topic number of the given identity is
        *                               out of valid range.
        *
        *   @param  aIdentity           Topic identity to be added.
        */
        void AddTopicIdentityL( const TCbsDbTopicIdentity& aIdentity );

    private:

        /**
        *   Constructor. 
        *
        */
        CCbsDbImpTopicCollection();

        /**
        *   Finalizes the construction.
        *
        */
        void ConstructL();

        /**
        *   Notifies topic collection observers about changed topic 
        *   collection.
        */       
        void NotifyObservers();

    private:

        // Own: Contains the observers.
        CArrayFixFlat< MCbsDbTopicCollectionObserver* >* iObservers;

        // Own: Contains buffered topic identities.
        CArrayFix< TCbsDbTopicIdentity >* iIdentities;

    };

#endif      //  CCBSDBIMPTOPICCOLLECTION_H   
            
// End of File
