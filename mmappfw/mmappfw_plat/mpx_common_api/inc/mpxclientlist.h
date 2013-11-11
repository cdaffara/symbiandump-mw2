/*
* Copyright (c) 2006 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  Encapsulates all the clients
*
*/


#ifndef CMPXCLIENTLIST_H
#define CMPXCLIENTLIST_H

// INCLUDES
#include <e32base.h>
#include <mpxmessage.h>
#include <mpxmessage2.h>

// CLASS FORWARDS
class CMPXMessageQueue;
class MMPXClientlistObserver;
class CMPXSubscription;

/**
*   Manages a list of clients.
*
*   @lib mpxcommon.lib
*/
class CMPXClientList : public CBase
    {
public:
    /**
    * Two-phased constructor.
    *
    * @since S60 3.2.3
    * @param aObserver observer of the client list
    * @return object of constructed
    */
    IMPORT_C static CMPXClientList* NewL(MMPXClientlistObserver* aObserver = NULL);

    /**
    * Destructor.
    *
    * @since S60 3.2.3
    */
    IMPORT_C virtual ~CMPXClientList();

public:
    /**
    * Add  another client into the list.
    *
    * @since S60 3.2.3
    * @param aId thread id of the client
    * @param aMsgQueue message queue for the client
    * @leave The function leaves with one of the system wide error codes,
    *         if the operation fails.
    */
    IMPORT_C void AddClientL(TThreadId aId,
                             CMPXMessageQueue* aMsgQueue);

    /**
    * Add  another client into the list.
    *
    * @since S60 3.2.3
    * @param aId thread id of the client
    * @param aMode client mode
    * @param aMsgQueue message queue for the client
    * @leave The function leaves with one of the system wide error codes,
    *         if the operation fails.
    */
    IMPORT_C void AddClientL(TThreadId aId,
                             TInt aMode,
                             CMPXMessageQueue* aMsgQueue);
							 

	/**
    * Add  another client into the list.
    *
    * @since S60 3.2.3
    * @param aId thread id of the client
    * @param aMode client mode
    * @param aMsgQueue message queue for the client
	* @param aCategory category of the client, refer to TMPXCategory
    * @leave The function leaves with one of the system wide error codes,
    *         if the operation fails.
    */
    IMPORT_C void AddClientL(TThreadId aId,
                             TInt aMode,
                             CMPXMessageQueue* aMsgQueue,
                             const TInt aCategory);

    /**
    * Remove a client at a specified position.
    *
    * @since S60 3.2.3
    * @param aIndex The position within the list from where the client is to
    *                be removed
    * @panic USER 130, if aIndex is out of bound
    */
    IMPORT_C void RemoveClient(TInt aIndex);

    /**
    * The number of clients.
    *
    * @since S60 3.2.3
    * @return number of the clients
    */
    IMPORT_C TInt ClientCount() const;

    /**
    * A list of process ids of all the clients in this list.
    *
    * @since S60 3.2.3
    * @return array of process id of clients
    */
    IMPORT_C TArray<TProcessId> ClientProcessList() const;

    /**
    * Is this (denoted by thread id) a client in this list?.
    *
    * @since S60 3.2.3
    * @param aId thread id of the client
    * @return ETrue, the client is in the list
    */
    IMPORT_C TBool IsClient(TThreadId aId) const;

    /**
    * Find a client denoted by message queue in this list.
    *
    * @since S60 3.2.3
    * @param aMsgQueue Message queue for the client
    * @return The index of the client within the list
    *          KErrNotFound, if no matching object can be found
    */
    IMPORT_C TInt Find(const CMPXMessageQueue& aMsgQueue) const;

    /**
    * Find a client denoted by thread id in the list.
    *
    * @since S60 3.2.3
    * @param aId thread id of the client
    * @return TInt index to the client list
    */
    IMPORT_C TInt Find(TThreadId aId) const;

    /**
    * Return client mode.
    *
    * @since S60 3.2.3
    * @param aIndex index to the client
    * @return the mode of client
    * @panic USER 0 if aIndex out of bound
    */
    IMPORT_C TInt ClientMode(TInt aIndex) const;

	/**
    * Return client category.
    *
    * @since S60 3.2.3
    * @param aIndex index to the client
    * @return the category of client
    * @panic USER 0 if aIndex out of bound
    */
    IMPORT_C TInt ClientCategory(TInt aIndex) const;


    /**
    * Dispatches message to all clients.
    *
    * @since S60 3.2.3
    * @param aMsg message, pointer to message, ownership not transferred.
    * @param aError error code
    */
    IMPORT_C void SendMsg(const CMPXMessage* aMsg, TInt aError);

    /**
    * Dispatches message to a client.
    *
    * @since S60 3.2.3
    * @param aIndex index to the client
    * @param aMsg message object, ownership not transferred.
    * @param aError error code
    */
    IMPORT_C void SendMsg(TInt aIndex, const CMPXMessage* aMsg, TInt aError);

    /**
     * Adds a subscription to a client identified by a specified position.
     *
     * @since S60 3.2.3
     * @param aIndex The position within the list where the client is.
     * @param aSubscription subscription to add. Ownership is transferred.
     */
    IMPORT_C void AddSubscriptionL(
        TInt aIndex,
        CMPXSubscription* aSubscription);

    /**
     * Removes a subscription from a client identified by a specified position.
     *
     * @since S60 3.2.3
     * @param aIndex The position within the list where the client is.
     * @param aSubscription subscription to remove.
     */
    IMPORT_C void RemoveSubscriptionL(
        TInt aIndex,
        const CMPXSubscription& aSubscription);

    /**
     * Removes all subscriptions from a client identified by a specified position.
     *
     * @since S60 3.2.3
     * @param aIndex The position within the list where the client is.
     */
    IMPORT_C void RemoveAllSubscriptionsL(TInt aIndex);

    /**
     * Convert TMPXMessage into CMPXMessage.
     *
     * @since S60 3.2.3
     * @param aMsg TMPXMessage message
     * @return CMPXMessage object
     * @leave The function leaves with one of the system wide error codes,
     *         if the operation fails.
     * @deprecated should not be used
     */
     inline CMPXMessage* ConvertMsgLC(const TMPXMessage& aMsg);

    /** 
    * Dispatches message to all clients.
    *
    * @since S60 3.2.3
    * @param aMsg message
    * @leave The function leaves with one of the system wide error codes,
    *         if the operation fails.
    * @deprecated Use method CMPXClientList::SendMsg(const CMPXMessage* aMsg, 
    *          TInt aError) instead.
    */
    inline void SendMsgL(const TMPXMessage& aMsg);

    /** 
    * Dispatches message to a client.
    *
    * @since S60 3.2.3
    * @param aIndex index to the client
    * @param aMsg message
    * @leave The function leaves with one of the system wide error codes,
    *         if the operation fails.
    * @deprecated Use method CMPXClientList::SendMsg(TInt aIndex, 
    *         const CMPXMessage* aMsg, TInt aError) instead.
    */
    inline void SendMsgL(TInt aIndex, const TMPXMessage& aMsg);

    
    /**
    * Sets the primary client
    *
    * @since S60 9.2
    * @return system error
    */        
    IMPORT_C TInt SetPrimaryClient(CMPXMessageQueue& aMsgQueue);
        


    /**
    * Send a sync message to the primary client
    *
    * @since S60 9.2
    * @return system error
    */    
    IMPORT_C TInt SendSyncMsg(const CMPXMessage* aMsg);

    
private:
    /**
    *  Encapsulates a client: all the information associated with a client of
    *  the playback utility interface.
    *
    *  @lib mpxcommon.lib
    */
    NONSHARABLE_CLASS(CClientId): public CBase
        {
    public:
        /**
        * Constructor.
        *
        * @since S60 3.2.3
        * @param aTid thread id of the client
        * @param aPid process id of the client
        * @param aMsgQueue message queue for the client
        */
        CClientId(TThreadId aTid,
                  TProcessId aPid,
                  CMPXMessageQueue* aMsgQueue);

        /**
        * Constructor.
        *
        * @since S60 3.2.3
        * @param aTid thread id of the client
        * @param aPid process id of the client
        * @param aMode client mode
        * @param aMsgQueue message queue for the client
        */
        CClientId(TThreadId aTid,
                  TProcessId aPid,
                  TInt aMode,
                  CMPXMessageQueue* aMsgQueue);

	    /**
        * Constructor.
        *
        * @since S60 3.2.3
        * @param aTid thread id of the client
        * @param aPid process id of the client
        * @param aMode client mode
        * @param aMsgQueue message queue for the client
        * @param aCategory client category
        */
        CClientId(TThreadId aTid,
                  TProcessId aPid,
                  TInt aMode,
                  CMPXMessageQueue* aMsgQueue,
                  const TInt aCategory); 

        /**
        * Constructor.
        *
        * @since S60 3.2.3
        * @param aMsgQueue message queue for the client
        */
        CClientId(CMPXMessageQueue* aMsgQueue);

        /**
        * Constructor.
        *
        * @since S60 3.2.3
        * @param aTid thread id of the client
        */
        CClientId(TThreadId aTid);

        /**
        * Constructor.
        *
        * @since S60 3.2.3
        * @param aPid process id of the client
        */
        CClientId(TProcessId aPid);

        /**
         * Constructor.
         *
         * @since S60 3.2.3
         */
        CClientId();

        /**
         *  Destructor.
         *
         * @since S60 3.2.3
         */
        virtual ~CClientId();

        /**
        * Assignment operator.
        *
        * @since S60 3.2.3
        * @param aClientId client ID instance to assign from.
        * @return reference to the current updated instance.
        */
        CClientId& operator=(const CClientId& aClientId);

        /**
        * Adds a subscription to the client.
        *
        * @since S60 3.2.3
        * @param aSubscription subscription to add. Ownership is transferred.
        */
        void AddSubscriptionL(CMPXSubscription* aSubscription);

        /**
        * Removes a subscription from the client.
        *
        * @since S60 3.2.3
        * @param aSubscription subscription to remove.
        */
        void RemoveSubscriptionL(const CMPXSubscription& aSubscription);

        /**
        * Removes all subscriptions from the client.
        *
        * @since S60 3.2.3
        */
        void RemoveAllSubscriptionsL();


    public:
        TThreadId iTid;
        TProcessId iPid;
        TInt iMode;
        TInt iCategory;
        CMPXMessageQueue* iMsgQueue; // Not owned
        RPointerArray<CMPXSubscription> iSubscriptions; // array of subscriptions
        };
private:
    /**
    * C++ constructor.
    *
    * @since S60 3.2.3
    * @param aObserver observer of the client list
    */
    CMPXClientList(MMPXClientlistObserver* aObserver);

    /**
    * 2nd phase constructor.
    *
    * @since S60 3.2.3
    */
    void ConstructL();

    /**
    * Check if the msg subscribed by the client.
    *
    * @since S60 3.2.3
    * @param aIndex index to the client
    * @param aMsg message object, ownership not transferred.
    * @param leave leave with system error code
    */
    TBool IsMsgSubscribedL(TInt aIndex, const CMPXMessage* aMsg);

    /**
    * Comparison function. If the names are set in the client objects, then
    * that's used to test for equality. Otherwise, if the thread ids are set,
    * that's used. Else, it's assumed that the equality test is based on process
    * ids.
    *
    * @since S60 3.2.3
    * @param aClient1 client id of the client 1
    * @param aClient2 client id of the client 2
    * @return  ETrue if two ids are the same
    */
    static TBool ClientsMatch(const CClientId& aClient1,
                              const CClientId& aClient2);
private:
    TIdentityRelation<CClientId> iIdentity;
    RPointerArray<CClientId> iClients;
    RArray<TProcessId> iClientProcesses;
    MMPXClientlistObserver* iObserver;
    CMPXMessageQueue* iPrimaryClient;
    };

#include "mpxclientlist.inl"

#endif // CMPXCLIENTLIST_H
