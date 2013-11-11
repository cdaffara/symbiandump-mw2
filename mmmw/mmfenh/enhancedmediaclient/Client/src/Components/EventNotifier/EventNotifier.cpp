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
* Description:  Implementation of EventNotifier class.
*
*/



#include "EventNotifier.h"
#include "EventBase.h"
#include <ControlObserver.h>

#include "tracemacros.h"

using namespace multimedia;

const TInt KInvalidIndex = -1;

///////////////////////////////////////////////////////////////////////////////
//                  CObserverManager
///////////////////////////////////////////////////////////////////////////////

CObserverManager::CObserverManager()
:iObserverCount(0),
iCurrentObserverIndex(KInvalidIndex)

    {
    // No impl
    }

CObserverManager::~CObserverManager()
    {
    iObserversList.Reset();
    iActiveList.Reset();
    iObserversList.Close();
    iActiveList.Close();
    }

CObserverManager* CObserverManager::NewL()
    {
    CObserverManager* self = new (ELeave)CObserverManager;
    CleanupStack::PushL(self);
    self->ConstructL();
    CleanupStack::Pop(self);
    return self;
    }
void CObserverManager::ConstructL()
    {

    }

// Adds observer to list
TInt CObserverManager::AddObserver( const MControlObserver& aObserver )
    {
    // Add to list if observer is not already added
    TInt status = iObserversList.Find(&aObserver);
    if ( status == KErrNotFound )
        {
        status = iObserversList.Append(&aObserver);
        status = iActiveList.Append(ETrue);
        // Update count
        iObserverCount++;
        }
    else
        {
        status = KErrAlreadyExists;
        }
    return status;
    }

// Marks observer as inactive in the list
TInt CObserverManager::RemoveObserver( const MControlObserver& aObserver )
    {
    TInt status = iObserversList.Find(&aObserver);
    // If found status has index to observer in the array
    // else it would contain KErrNotFound
    if ( status >= 0 )
        {
        // Mark the observer as inactive, but remove it later in Purge();
        iActiveList[status] = EFalse;
        // Update count
        iObserverCount--;
        status = KErrNone;
        }
    return status;
    }

// Moves the pointer to element before the first in list
void CObserverManager::Init()
    {
    iCurrentObserverIndex = KInvalidIndex;
    }

// Moves the pointer to next active observer. Client should
// call this after Init() to get to the first element.
TBool CObserverManager::Next()
    {
    TBool status(EFalse);
    // Starting from the next index count find an active observer
    TInt i(iCurrentObserverIndex+1);
    // Assume that we are not going to find one.
    iCurrentObserverIndex = KInvalidIndex;
    for (; i< iActiveList.Count(); i++)
        {
        if ( iActiveList[i] == TInt(ETrue) )
            { // We have found an active observer in the list
            iCurrentObserverIndex = i;
            status = ETrue;
            break;
            }
        }
    return status;
    }

// Returns a reference to current observer
void CObserverManager::Observer( MControlObserver*& aObserver )
    {
    if ( iCurrentObserverIndex == KInvalidIndex )
        {
        aObserver = NULL;
        }
    else
        {
        aObserver = iObserversList[iCurrentObserverIndex];
        }
    }

// Returns the count of active observers
TInt CObserverManager::Count()
    {
    return iObserverCount;
    }

// Purge inactive observers in the list
void CObserverManager::Purge()
    {
    TInt index(0);
    iObserverCount = 0; // Recalculate observer count
    while (index < iActiveList.Count())
        {
        if ( iActiveList[index] == TInt(EFalse) )
            {
            iObserversList.Remove(index);
            iActiveList.Remove(index);
            }
        else
            {
            iObserverCount++;
            index++;
            }
        }
    }

// Resets the list
void CObserverManager::Reset()
    {
    iCurrentObserverIndex = KInvalidIndex;
    iObserverCount = 0;
    iObserversList.Reset();
    iActiveList.Reset();
    }


///////////////////////////////////////////////////////////////////////////////
//                  CEventQueueItem
///////////////////////////////////////////////////////////////////////////////

CEventQueueItem::CEventQueueItem(MControl* aControl, TUint aEvent, CEventBase* aEventObject )
: iEvent( aEvent), iEventObject(aEventObject),iControl(aControl)
    {
    }

CEventQueueItem::~CEventQueueItem()
    {
    if ( iEventObject )
        {
        delete iEventObject;
        }
    }

CEventQueueItem* CEventQueueItem::NewL(MControl* aControl, TUint aEvent, CEventBase* aEventObject )
    {
    CEventQueueItem* self = new (ELeave)CEventQueueItem( aControl,aEvent, aEventObject );
    CleanupStack::PushL(self);
    self->ConstructL();
    CleanupStack::Pop(self);
    return self;
    }

void CEventQueueItem::ConstructL()
    {
    // No impl
    }

TUint CEventQueueItem::EventType()
    {
    return iEvent;
    }

MControl* CEventQueueItem::ControlObject()
    {
    return iControl;
    }

CEventBase* CEventQueueItem::EventObject()
    {
    return iEventObject;
    }

TAny* CEventQueueItem::GetInterface()
    {
    if ( iEventObject )
        return iEventObject->GetInterface();
    else
        return NULL;
    }

///////////////////////////////////////////////////////////////////////////////
//                  CObserverManager
///////////////////////////////////////////////////////////////////////////////

CEventNotifier::CEventNotifier()
: CActive( CActive::EPriorityStandard )
    {
    CActiveScheduler::Add(this);
    }

CEventNotifier::~CEventNotifier()
    {
    Cancel();
    delete iObserverManager;
    EmptyQueue();
    delete iEventQueue;
    }

CEventNotifier* CEventNotifier::NewL()
    {
    CEventNotifier* self = new (ELeave)CEventNotifier;
    CleanupStack::PushL(self);
    self->ConstructL();
    CleanupStack::Pop(self);
    return self;
    }

void CEventNotifier::ConstructL()
    {
    iObserverManager = CObserverManager::NewL();
    iEventQueue = new(ELeave) TSglQue<CEventQueueItem>(_FOFF(CEventQueueItem, iLink));
    }

// Adds observer
TInt CEventNotifier::AddObserver( const MControlObserver& aObserver )
    {
    return iObserverManager->AddObserver(aObserver);
    }

// Remove observer
TInt CEventNotifier::RemoveObserver( const MControlObserver& aObserver )
    {
    return iObserverManager->RemoveObserver(aObserver);
    }

// Send notification to observers. If another event is
// being processed, this will append event to queue and observers will
// be notified after current event is notified
TInt CEventNotifier::Event(MControl* aControl, TUint aEvent, CEventBase* aEventObject )
    {
    CEventQueueItem* item(NULL);
    TInt status(KErrNone);
    // If there are no observers in the list, just delete the event and return
    if ( iObserverManager->Count() == 0 )
        {
        EMC_TRACE2(_L("CEventNotifier::Event:No observers for this event[%d]"), aEvent);
        delete aEventObject;
        }
    else // Observer list is not empty, proceed notifying observers
        {
        TRAP( status, item = CEventQueueItem::NewL(aControl, aEvent, aEventObject ) );
        if ( status == KErrNone )
            {
            iEventQueue->AddLast(*item);
            KickSignal();
            }
        }
    return status;
    }

void CEventNotifier::KickSignal()
    {
    if (!IsActive())
        {
        TRequestStatus* s = &iStatus;
        SetActive();
        User::RequestComplete( s, KErrNone );
        }
    }
void CEventNotifier::EventNotified()
    {
    CEventQueueItem* eventItem = iEventQueue->First();
    iEventQueue->Remove(*eventItem);
    delete eventItem;
    iObserverManager->Init();
    }

void CEventNotifier::RunL()
    {
    if ( !iEventQueue-> IsEmpty() && (iStatus == KErrNone) )
        {
        // Get the current item from the observer list
        if ( iObserverManager->Next() )
            {
            MControlObserver* observer(NULL);
            iObserverManager->Observer(observer);
            if ( observer ) // Just to make sure. This should never happen
                {
                CEventQueueItem* eventItem = iEventQueue->First();
                observer->Event(eventItem->ControlObject(), eventItem->EventType(), eventItem->GetInterface() );
                }
#ifdef _DEBUG
            else
                {
                EMC_TRACE1(_L("CEventNotifier::RunL:ERROR[Observer reference is NULL]"));
                }
#endif // _DEBUG
            }
        else // if ( iObserverManager->Next() )
            { // There are no more observers in the list, all observers are
            // notified of this event.
            EventNotified();
            }
        KickSignal();
        }
    }

void CEventNotifier::DoCancel()
    {

    }

TInt CEventNotifier::RunError(TInt /*aError*/)
    {
    return KErrNone;
    }

void CEventNotifier::EmptyQueue()
    {
    CEventQueueItem* item;
    while ( !iEventQueue->IsEmpty() )
        {
        item = iEventQueue->First();
        iEventQueue->Remove(*item);
        delete item;
        }
    }
// End of file
