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


// INCLUDES
#include <mpxlog.h>
#include "mpxuser.h"
#include "mpxmessagequeue.h"
#include "mpxclientlistobserver.h"
#include "mpxclientlist.h"
#include <mpxsubscription.h>

// ============================ LOCAL FUNCTIONS ==============================

// ----------------------------------------------------------------------------
// Searches process id for target selector plugin.
// When the player has been opened in a mode, which is bound to the same engine
// than certain application's engine, then need to fetch its process id.
// (As a reference for parameter aMode see MMPXPlaybackUtility modes.)
// Otherwise target selector plugin is notified wrongly about registered client
// and key events are not propagated to correct application.
// ----------------------------------------------------------------------------
//
static void FindProcessIdForTsp( TInt aMode, TProcessId& aProcessId )
    {
    TBool processFound( EFalse );
    TFindProcess processFinder;
    TFullName processName;

    while ( processFinder.Next( processName ) == KErrNone && !processFound )
        {
        RProcess process;
        TInt err = process.Open( processFinder );
        if( err == KErrNone )
            {
            if( process.SecureId().iId == aMode && process.ExitType() == EExitPending )
                {
                MPX_DEBUG4("FindProcessIdForTsp(): pid changed from %d to %d (mode 0x%x)",
                           TUint(aProcessId), TUint(process.Id()), aMode);
                aProcessId = process.Id();
                processFound = ETrue;
                }
            process.Close();
            }
        }
    }

// ============================ MEMBER FUNCTIONS ==============================

// ----------------------------------------------------------------------------
// Two-phased constructor.
// ----------------------------------------------------------------------------
//
EXPORT_C CMPXClientList* CMPXClientList::NewL(
    MMPXClientlistObserver* aObserver /*=NULL*/)
    {
    CMPXClientList* list=new(ELeave)CMPXClientList(aObserver);
    CleanupStack::PushL(list);
    list->ConstructL();
    CleanupStack::Pop(list);
    return list;
    }

// ----------------------------------------------------------------------------
// Constructor.
// ----------------------------------------------------------------------------
//
CMPXClientList::CMPXClientList(MMPXClientlistObserver* aObserver)
    : iIdentity(CMPXClientList::ClientsMatch),
    iObserver(aObserver), 
    iPrimaryClient(NULL)
    {}

// ----------------------------------------------------------------------------
// 2nd phase constructor.
// ----------------------------------------------------------------------------
//
void CMPXClientList::ConstructL()
    {
    }

// ----------------------------------------------------------------------------
// Destructor
// ----------------------------------------------------------------------------
//
EXPORT_C CMPXClientList::~CMPXClientList()
    {
    iClients.ResetAndDestroy();
    iClients.Close();
    iClientProcesses.Close();
    }

// ----------------------------------------------------------------------------
// Add a client into the client list
// ----------------------------------------------------------------------------
//
EXPORT_C void CMPXClientList::AddClientL(
    TThreadId aId,
    CMPXMessageQueue* aMsgQueue)
    {
    AddClientL(aId, KErrUnknown, aMsgQueue);
    }

// ----------------------------------------------------------------------------
// Add a client into the client list
// ----------------------------------------------------------------------------
//
EXPORT_C void CMPXClientList::AddClientL(
    TThreadId aId,
    TInt aMode,
    CMPXMessageQueue* aMsgQueue)
    {
    TProcessId pid=MPXUser::ProcessIdL(aId);
    CClientId* newClient = new(ELeave) CClientId(aId,pid,aMode,aMsgQueue);
    CleanupStack::PushL(newClient);

    // add an empty subscription by default
    CMPXSubscription* subscription = CMPXSubscription::NewL();
    CleanupStack::PushL(subscription);

    // transfer the ownership
    newClient->iSubscriptions.AppendL(subscription);
    CleanupStack::Pop(subscription);

    iClients.AppendL(newClient);
    CleanupStack::Pop(newClient);

    if (iClientProcesses.Find(pid)==KErrNotFound)
        {
        iClientProcesses.AppendL(pid);
        if (iObserver)
            {
            FindProcessIdForTsp(aMode, pid);
            iObserver->HandleClientChange(pid, MMPXClientlistObserver::EAdd);
            }
        }
    }
// ----------------------------------------------------------------------------
// Add a client into the client list
// ----------------------------------------------------------------------------
//

EXPORT_C void CMPXClientList::AddClientL(
    TThreadId aId,
    TInt aMode,
    CMPXMessageQueue* aMsgQueue,
    const TInt aCategory)
    {
    TProcessId pid=MPXUser::ProcessIdL(aId);
    CClientId* newClient = new(ELeave) CClientId(aId,pid,aMode,aMsgQueue, aCategory);
    CleanupStack::PushL(newClient);
    // add an empty subscription by default
    CMPXSubscription* subscription = CMPXSubscription::NewL();
    CleanupStack::PushL(subscription);
    // transfer the ownership
    newClient->iSubscriptions.AppendL(subscription);
    CleanupStack::Pop(subscription);
    iClients.AppendL(newClient);
    CleanupStack::Pop(newClient);
    if (iClientProcesses.Find(pid)==KErrNotFound)
        {
        iClientProcesses.AppendL(pid);
        if (iObserver)
            {
            FindProcessIdForTsp(aMode, pid);
            iObserver->HandleClientChange(pid, MMPXClientlistObserver::EAdd);
            }
        }
    }
// SK
// ----------------------------------------------------------------------------
// Remove a client from the list
// ----------------------------------------------------------------------------
//
EXPORT_C void CMPXClientList::RemoveClient(TInt aIndex)
    {
    // USER panic 130, if aIndex is negative or is greater than the number of
    // objects currently in the array.
    CClientId* id( iClients[aIndex] );
    iClients.Remove(aIndex);

    //set primary client to NULL if primary client is removed
  	if (iPrimaryClient != NULL)
    {
		if (aIndex == Find(*iPrimaryClient))
        	{
        	iPrimaryClient = NULL;
        	}
	}    
	CClientId removeId( id->iPid );
    if ( iClients.Find( &removeId, iIdentity ) == KErrNotFound )
        //
        // There's no other client from the same process, so
        // remove it from the process list
        //
        {
        TInt i=iClientProcesses.Find(id->iPid);
        if (KErrNotFound != i)
            {
            if (iObserver)
                {
                TProcessId pid(id->iPid);
                FindProcessIdForTsp(id->iMode, pid);
                iObserver->HandleClientChange(pid, MMPXClientlistObserver::ERemove);
                }
            iClientProcesses.Remove(i);
            }
        }
    delete id;
    }

// -----------------------------------------------------------------------------
// Return the number of clients
// -----------------------------------------------------------------------------
//
EXPORT_C TInt CMPXClientList::ClientCount() const
    {
    return iClients.Count();
    }

// -----------------------------------------------------------------------------
// Return array of client process ids
// -----------------------------------------------------------------------------
//
EXPORT_C TArray<TProcessId> CMPXClientList::ClientProcessList() const
    {
    return iClientProcesses.Array();
    }

// -----------------------------------------------------------------------------
// Check if the client in the client list or not
// -----------------------------------------------------------------------------
//
EXPORT_C TBool CMPXClientList::IsClient(TThreadId aId) const
    {
    return (KErrNotFound != Find(aId));
    }

// -----------------------------------------------------------------------------
// Find a client denoted by message queue in this list.
// -----------------------------------------------------------------------------
//
EXPORT_C TInt CMPXClientList::Find(const CMPXMessageQueue& aMsgQueue) const
    {
    CClientId id( const_cast<CMPXMessageQueue*>( &aMsgQueue ));
    return iClients.Find( &id, iIdentity );
    }

// -----------------------------------------------------------------------------
// Found a client by Id
// -----------------------------------------------------------------------------
//
EXPORT_C TInt CMPXClientList::Find(TThreadId aId) const
    {
    CClientId id( aId );
    return iClients.Find( &id ,iIdentity );
    }

// -----------------------------------------------------------------------------
// Return client mode
// -----------------------------------------------------------------------------
//
EXPORT_C TInt CMPXClientList::ClientMode(TInt aIndex) const
    {
    MPX_ASSERT(aIndex>=0 && aIndex<ClientCount());
    return iClients[aIndex]->iMode;
    }
	

// -----------------------------------------------------------------------------
// Return client category
// -----------------------------------------------------------------------------
//
EXPORT_C TInt CMPXClientList::ClientCategory(TInt aIndex) const
    {
    MPX_ASSERT(aIndex>=0 && aIndex<ClientCount());
    TInt category = iClients[aIndex]->iCategory;
    return category;
    }

	
// -----------------------------------------------------------------------------
// Send message to all clients in the list
// -----------------------------------------------------------------------------
//
EXPORT_C void CMPXClientList::SendMsg(const CMPXMessage* aMsg, TInt aError)
    {
    MPX_FUNC_EX("CMPXClientList::SendMsg");
    for (TInt i=iClients.Count();--i>=0;)
        {
        SendMsg( i, aMsg, aError );
        }
    }

// -----------------------------------------------------------------------------
// Send message to a client
// -----------------------------------------------------------------------------
//
EXPORT_C void CMPXClientList::SendMsg(
    TInt aIndex,
    const CMPXMessage* aMsg,
    TInt aError)
    {
    MPX_FUNC_EX("CMPXClientList::SendMsgL(aIndex)");
    MPX_ASSERT(aIndex>=0 && aIndex<iClients.Count());
    MPX_ASSERT(iClients[aIndex]->iMsgQueue);

    // check the subscriptions
    TBool send(EFalse);
    TInt err(KErrNone);
    if (aMsg)
        {
        TRAP(err, send = IsMsgSubscribedL(aIndex, aMsg));
        }
    else
        {
        send = ETrue; // broadcast error message aError
        }

    // only send if the client has subscribed for it
    if (send && KErrNone == err)
        {
        iClients[aIndex]->iMsgQueue->Add(aMsg, aError);
        }
    }

// -----------------------------------------------------------------------------
// CMPXClientList::AddSubscriptionL
// -----------------------------------------------------------------------------
//
EXPORT_C void CMPXClientList::AddSubscriptionL(
    TInt aIndex,
    CMPXSubscription* aSubscription)
    {
    MPX_FUNC("CMPXClientList::AddSubscriptionL");

    CClientId* id( iClients[aIndex] );
    TInt subCount( id->iSubscriptions.Count() );
    const CMPXMediaArray* items = id->iSubscriptions[0]->ItemsL();
    // check if it has an empty subscription
    if ( 1 == subCount  &&  items->Count() )
        {
        // remove it if so
        id->RemoveAllSubscriptionsL();
        }

    // add the new subscription
    id->AddSubscriptionL(aSubscription);
    }

// -----------------------------------------------------------------------------
// CMPXClientList::RemoveSubscriptionL
// -----------------------------------------------------------------------------
//
EXPORT_C void CMPXClientList::RemoveSubscriptionL(
    TInt aIndex,
    const CMPXSubscription& aSubscription)
    {
    MPX_FUNC("CMPXClientList::RemoveSubscriptionL");

    iClients[aIndex]->RemoveSubscriptionL(aSubscription);
    }

// -----------------------------------------------------------------------------
// CMPXClientList::RemoveAllSubscriptionsL
// -----------------------------------------------------------------------------
//
EXPORT_C void CMPXClientList::RemoveAllSubscriptionsL(
    TInt aIndex)
    {
    MPX_FUNC("CMPXClientList::RemoveAllSubscriptionsL");

    iClients[aIndex]->RemoveAllSubscriptionsL();
    }

// -----------------------------------------------------------------------------
// CMPXClientList::IsMsgSubscribedL
// -----------------------------------------------------------------------------
//
TBool CMPXClientList::IsMsgSubscribedL(TInt aIndex, const CMPXMessage* aMsg)
    {
    // check the subscriptions
    TBool IsSubScribed(EFalse);
    TInt subCount(iClients[aIndex]->iSubscriptions.Count());

    if (subCount)
        {
        const CMPXMediaArray* subscriptionItems =
                                 iClients[aIndex]->iSubscriptions[0]->ItemsL();
        if (1==subCount && (0== subscriptionItems->Count()))
            {// one empty subscription - send everything
            IsSubScribed = ETrue;
            }
        else
            {// have to check the message against the subscriptions.
            MPX_ASSERT(aMsg);
            const TArray<TMPXAttribute> msgAttrs = aMsg->Attributes();
            TInt msgAttrCount(msgAttrs.Count());
            // iterate subscriptions
            for (TInt subIndex = 0; subIndex<subCount && !IsSubScribed; ++subIndex)
                {
                // iterate items for the current subscription
                subscriptionItems =
                            iClients[aIndex]->iSubscriptions[subIndex]->ItemsL();
                TInt itemCount(subscriptionItems->Count());
                for (TInt itemIndex = 0; itemIndex < itemCount; ++itemIndex)
                    {
                    // check the message attributes for the current subscription item
                    TBool subMatch(ETrue);
                    TInt attrMatchCount(0);
                    CMPXSubscriptionItem* subItem(subscriptionItems->AtL(itemIndex));

                    for (TInt msgAttrIndex = 0; msgAttrIndex < msgAttrCount; ++msgAttrIndex)
                        {
                        TBool attrExists(EFalse);
                        TBool attrMatch(EFalse);
                        const TMPXAttribute& msgAttr( msgAttrs[msgAttrIndex] );

                        if ( subItem->IsSupported(msgAttr))
                            {
                            attrExists = ETrue;

                            if ( subItem->Match( *aMsg, msgAttr ))
                                {
                                attrMatch = ETrue;
                                attrMatchCount++;
                                }
                            }

                        if (attrExists && !attrMatch)
                            {
                            subMatch = EFalse;
                            break;
                            }
                        }

                    // send the message if all attributes that exist in both the message and the subscription
                    // have the same values and all subscription attributes match
                    if ( subMatch && ( attrMatchCount == subItem->Count()) )
                        {
                        IsSubScribed = ETrue;
                        break;
                        }
                    }
                }
            }
        } // else subCount = 0, IsSubScribed = EFalse (default)
    return IsSubScribed;
    }

// ----------------------------------------------------------------------------
// Comparison function. If the names are set in the client objects, then
// that's used to test for equality. Otherwise, if the thread ids are set,
// that's used. Else, it's assumed that the equality test is based on process
//  ids.
// ----------------------------------------------------------------------------
//
TBool CMPXClientList::ClientsMatch(const CClientId& aClient1,
                                   const CClientId& aClient2)
    {
    TBool match=EFalse;
    if (aClient1.iMsgQueue && aClient2.iMsgQueue)
        {
        match=aClient1.iMsgQueue==aClient2.iMsgQueue;
        }
    else if (aClient1.iTid.Id()!=KNullThreadId &&
             aClient2.iTid.Id()!=KNullThreadId)
        {
        match=(aClient1.iTid==aClient2.iTid);
        }
    else
        {
        match=(aClient1.iPid==aClient2.iPid);
        }
    return match;
    }

// -----------------------------------------------------------------------------
// C++ constructor
// -----------------------------------------------------------------------------
//
CMPXClientList::CClientId::CClientId(
    TThreadId aTid,
    TProcessId aPid,
    CMPXMessageQueue* aMsgQueue)
:   iTid(aTid),
    iPid(aPid),
    iMode(KErrNotFound),
    iMsgQueue(aMsgQueue)
    {}

// -----------------------------------------------------------------------------
// C++ constructor
// -----------------------------------------------------------------------------
//
CMPXClientList::CClientId::CClientId(
    TThreadId aTid,
    TProcessId aPid,
    TInt aMode,
    CMPXMessageQueue* aMsgQueue)
:   iTid(aTid),
    iPid(aPid),
    iMode(aMode),
    iMsgQueue(aMsgQueue)
    {}


// -----------------------------------------------------------------------------
// C++ constructor
// -----------------------------------------------------------------------------
//
    CMPXClientList::CClientId::CClientId(
        TThreadId aTid,
        TProcessId aPid,
        TInt aMode,
        CMPXMessageQueue* aMsgQueue,
        TInt aCategory)
    :   iTid(aTid),
        iPid(aPid),
        iMode(aMode),
        iCategory(aCategory),
        iMsgQueue(aMsgQueue)
        {}

// -----------------------------------------------------------------------------
// C++ constructor
// -----------------------------------------------------------------------------
//
CMPXClientList::CClientId::CClientId(CMPXMessageQueue* aMsgQueue)
    :iTid(static_cast<TUint64>(KNullThreadId)),
     iPid(static_cast<TUint64>(KNullProcessId)),
     iMode(KErrUnknown),
     iMsgQueue(aMsgQueue)
    {}

// -----------------------------------------------------------------------------
// C++ constructor
// -----------------------------------------------------------------------------
//
CMPXClientList::CClientId::CClientId(TThreadId aTid)
:   iTid(aTid),
    iPid(static_cast<TUint64>(KNullProcessId)),
    iMode(KErrUnknown),
    iMsgQueue(NULL)
    {}

// -----------------------------------------------------------------------------
// C++ constructor
// -----------------------------------------------------------------------------
//
CMPXClientList::CClientId::CClientId(TProcessId aPid)
:   iTid(static_cast<TUint64>(KNullThreadId)),
    iPid(aPid),
    iMode(KErrUnknown),
    iMsgQueue(NULL)
    {}

// -----------------------------------------------------------------------------
// Destructor
// -----------------------------------------------------------------------------
//
CMPXClientList::CClientId::~CClientId()
    {
    iSubscriptions.ResetAndDestroy();
    }

// -----------------------------------------------------------------------------
// CClientId::operator=
// -----------------------------------------------------------------------------
//
CMPXClientList::CClientId& CMPXClientList::CClientId::operator=(
    const CClientId& aClient)
    {
    iTid = aClient.iTid;
    iPid = aClient.iPid;
    iMode = aClient.iMode;
    iMsgQueue = aClient.iMsgQueue;

    RemoveAllSubscriptionsL();

    // copy the subscriptions
    TInt count(aClient.iSubscriptions.Count());
    for (TInt index = 0; index < count; ++index)
        {
        CMPXSubscription* subscription = static_cast<CMPXSubscription*>(
            CMPXMedia::NewL(*(aClient.iSubscriptions[index])));
        CleanupStack::PushL(subscription);

        // transfer the ownership
        iSubscriptions.AppendL(subscription);

        CleanupStack::Pop(subscription);
        }
    return *this;
    }

// -----------------------------------------------------------------------------
// CClientId::AddSubscriptionL
// -----------------------------------------------------------------------------
//
void CMPXClientList::CClientId::AddSubscriptionL(
    CMPXSubscription* aSubscription)
    {
    // add the subscription
    iSubscriptions.AppendL(aSubscription);
    }

// -----------------------------------------------------------------------------
// CClientId::RemoveSubscriptionL
// -----------------------------------------------------------------------------
//
void CMPXClientList::CClientId::RemoveSubscriptionL(
    const CMPXSubscription& aSubscription)
    {
    TInt count(iSubscriptions.Count());
    for (TInt index = 0; index < count; ++index)
        {
        CMPXSubscription* subscription = iSubscriptions[index];
        if (aSubscription == *subscription)
            {
            // found the subscription, remove it
            iSubscriptions.Remove(index);
            delete subscription;
            break;
            }
        }
    }

// -----------------------------------------------------------------------------
// CClientId::RemoveAllSubscriptionsL
// -----------------------------------------------------------------------------
//
void CMPXClientList::CClientId::RemoveAllSubscriptionsL()
    {
    iSubscriptions.ResetAndDestroy();
    }

// -----------------------------------------------------------------------------
// CMPClientList::SetPrimaryClient
// Set the primary client
// -----------------------------------------------------------------------------
//
EXPORT_C TInt CMPXClientList::SetPrimaryClient(CMPXMessageQueue& aMsgQueue)
    {
    MPX_FUNC("CMPXClientList::SetPrimaryClient");
    TInt index = Find(aMsgQueue);
    MPX_DEBUG2("CMPXClientList::SetPrimaryClient, index = %d", index);
    if (index >= 0)
        {
        iPrimaryClient = &aMsgQueue;
        return KErrNone;
        }
    return index;
    }
// -----------------------------------------------------------------------------
// CMPClientList::SendSyncMsg
// Send a sync message to the primary client
// -----------------------------------------------------------------------------
//
EXPORT_C TInt CMPXClientList::SendSyncMsg(const CMPXMessage* aMsg)
    {
    MPX_FUNC("CMPXClientList::SendSyncMsg");
    TInt err = KErrNone;
    if (iPrimaryClient == NULL)
        {
        return KErrNotFound;
        }
    TInt index = Find(*iPrimaryClient);
    MPX_DEBUG2("CMPXClientList::SendSyncMsg, index = %d", index);
    if (index >= 0)
        {
        err = iClients[index]->iMsgQueue->AddFirst(aMsg, KErrNone);
        }
    else
        {
        err = KErrNotFound;
        }
    return err;
    }
// End of File
