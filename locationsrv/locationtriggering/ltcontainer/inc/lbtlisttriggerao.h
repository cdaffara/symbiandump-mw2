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
* Description:  location triggering server client interface
*
*/



#ifndef LBTLISTTRIGGERAO_H
#define LBTLISTTRIGGERAO_H

#include <lbt.h>
#include "lbtcontainertriggerentry.h"
#include "lbttriggerstoreinterface.h"
#include "lbtcontainerlistoptions.h"

class MLbtListTriggerObserver;
NONSHARABLE_CLASS( CLbtListTriggerAO ) : public CActive
    {
    public: //constructor
        /**
        * Construct a trigger change event notifier object. 
        *
        * When the object is constructed. The trigger change 
        * notification request is not issued to the
        * Location Triggering Server. Client shall call 
        * CLbtTriggerChangeEventNotifier::Start() to start
        * notification.
        *
        * @param[in] aLbt A reference to RLbt object. The subsession 
        * must be opened. Otherwise a panic is raised.
        * @param[in] aObserver A reference to the 
        * observer object to receive trigger change event.
        * @param[in] aPriority An integer specifying the 
        * priority of this active object. CActive::TPriority 
        * defines a standard set of priorities. 
        *
        * @panic LocTriggering ELbtServerBadHandle If the 
        * subsession of aLbt is not opened.
        */
      
		static CLbtListTriggerAO* NewL(MLbtListTriggerObserver& aObserver);
        /**
        * Destructor. 
        *
        * If the notification is started, the 
        * destructor will cancel the notification.
        */
        ~CLbtListTriggerAO();

        /**
        * Start trigger change event notification. 
        *
        * After this function is called, when any 
        * trigger is changed in Location Trigger Server, the
        * client will be notified from the observer interface.
        * 
        * Client shall call Cancel() function to stop the
        * trigger change event notification.
        */
        void Start();
        
        
        void ListTriggers(CLbtContainerListOptions* aFilter,
        RPointerArray < CLbtContainerTriggerEntry >& aTriggers,
        RPointerArray<MLbtTriggerStore>& aTriggerStores);
    
    private:
        //Derived from CActive
        void RunL();
        TInt RunError( TInt aError );
        void DoCancel();

    private:
        //C++ default constructor
      
        CLbtListTriggerAO(MLbtListTriggerObserver& aObserver);
    

        //Second phase constructor
        void ConstructL();

    private:
     TRequestStatus* iClientStatus;
    // CBufFlat* iClientBuf;  
    RPointerArray < CBufFlat > iClientBuf;
    MLbtListTriggerObserver& iObserver;
        
    };

#endif // LBTTRIGGERCHANGEEVENTNOTIFIER_H

