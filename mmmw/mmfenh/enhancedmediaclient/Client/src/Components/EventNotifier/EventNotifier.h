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
* Description:  header of EventNotifier class.
*
*/



#ifndef EVENTNOTIFIER_H
#define EVENTNOTIFIER_H

#include <e32base.h>
#include <Events.h>

namespace multimedia
    {
    class MControlObserver;
    class CEventBase;
    class MControl;
    
    class CObserverManager : public CBase
        {
        public:
            static CObserverManager* NewL();
            ~CObserverManager();
            
            // Adds observer to list
            TInt AddObserver( const MControlObserver& aObserver );
            // Marks observer as inactive in the list
            TInt RemoveObserver( const MControlObserver& aObserver );
            
            // Moves the pointer to element before the first in list
            void Init();
            // Returns true and moves pointer to next active observer.
            TBool Next();
            // aObserver will contain pointer to current observer
            void Observer( MControlObserver*& aObserver );
            
            // Returns the count of active observers
            TInt Count();
            // Purge inactive observers in the list
            void Purge();
            // Resets the list
            void Reset();
            
        private:
            CObserverManager();
            void ConstructL();
            
        private:
            TInt    iObserverCount;
            TInt    iCurrentObserverIndex;
            // For now we maintain 2 RPointerArray lists
            // better option is to merge into one data structure
            RPointerArray<MControlObserver> iObserversList;
            RArray<TInt>                    iActiveList;
        };
    
    class CEventQueueItem : public CBase
        {
        public:
            static CEventQueueItem* NewL( MControl* aControl, TUint aEvent, CEventBase* aEventObject );
            ~CEventQueueItem();

        TUint EventType();
        CEventBase* EventObject();
        MControl* ControlObject();
            TAny* GetInterface();
            
        private:
            CEventQueueItem( MControl* aControl,TUint aEvent, CEventBase* aEventObject );
            void ConstructL();
            
        public:
            // next item
            TSglQueLink* iLink;
            
        private:
            TUint  iEvent;
            CEventBase*       iEventObject;
            MControl* iControl;
        };
    
    class CEventNotifier : public CActive
        {
        public:
            static CEventNotifier* NewL();
            ~CEventNotifier();
            
            // Adds observer
            TInt AddObserver( const MControlObserver& aObserver );
            // Remove observer
            TInt RemoveObserver( const MControlObserver& aObserver );
            // Send notification to observers. If another event is
            // being processed, this will add event to pending events
            // and observers will be notified later.
            // Takes ownership of aEventObject
            TInt Event( MControl* aControl, TUint aEvent, CEventBase* aEventObject );
            
            // From CActive
            void RunL();
            void DoCancel();
            TInt RunError(TInt aError);
            
        private:
            CEventNotifier();
            void ConstructL();
            
            // Signals to continue processing.
            void KickSignal();
            // Signals the event has been notified to all observers
            void EventNotified();
            
            void EmptyQueue();
            
        private:
            // Pointer to observer manager
            CObserverManager* iObserverManager;
            // List holding events to be notified
            TSglQue<CEventQueueItem>* iEventQueue;
        };
    } // namespace multimedia
    
#endif // EVENTNOTIFIER_H
    
    // End of file
