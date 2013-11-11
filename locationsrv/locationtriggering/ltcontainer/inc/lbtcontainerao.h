/*
* Copyright (c) 2006,2007 Nokia Corporation and/or its subsidiary(-ies).
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



#ifndef LBTCONTAINERAO_H
#define LBTCONTAINERAO_H

#include <lbt.h>
#include "lbtcontainer.h"
#include "lbtcontainertriggerentry.h"
#include "lbttriggerstoreinterface.h"
#include "lbtcontainerlistoptions.h"
#include "lbtcontainerAOOperation.h"
#include "lbttriggerstoreobserver.h"

class MLbtContainerOpObserver;
NONSHARABLE_CLASS( CLbtContainerAO ) : public CActive,
									   public MLbtTriggerStoreObserver
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
      
		static CLbtContainerAO* NewL(RPointerArray<MLbtTriggerStore>& aTriggerStores,
		                             RArray<CLbtContainer::TLbtTriggerStoreChangeObserver>& aObservers );
        /**
        * Destructor. 
        *
        * If the notification is started, the 
        * destructor will cancel the notification.
        */
        ~CLbtContainerAO();
        
        void GetTriggersL( const RArray<TLbtTriggerId>& aTriggerIds,
						   RPointerArray < CLbtContainerTriggerEntry >& aTriggers,
						   TInt& aOpId,
						   TRequestStatus& aStatus,
						   TLbtSecurityPolicy aSecurityPolicy = KLbtNullSecurity );

               
        void ListTriggersL( CLbtContainerListOptions* aFilter,
					        RPointerArray<CLbtContainerTriggerEntry>& aTriggers,
					        TInt aOpCode,
					        TRequestStatus& aStatus,
					        TLbtSecurityPolicy aSecurityPolicy);
        
        void UpdateTriggerL( CLbtContainerTriggerEntry& aEntry,
        					 TLbtTriggerDataMask aDataMask,
        					 TLbtTriggerAttributeFieldsMask aAttrMask, 
        					 TInt aOpCode,
        					 TRequestStatus& aStatus,
        					 TLbtSecurityPolicy aSecurityPolicy);
        
        void UpdateTriggersStateL( CLbtTriggerEntry::TLbtTriggerState aState,
                   				   CLbtContainerUpdateFilter* aFilter,
                    			   TInt aOpCode,
                    			   TLbtFireOnUpdate aFireOnUpdate,
                    			   TRequestStatus& aStatus,
                    			   TLbtSecurityPolicy aSecurityPolicy );

        void UpdateTriggersValidityL( TLbtTriggerDynamicInfo::TLbtTriggerValidity aValidity,
                    				  RArray<TLbtTriggerId>& aTriggerIds,
                    				  TInt aOpCode,
                    				  TRequestStatus& aStatus,
                    				  TLbtSecurityPolicy aSecurityPolicy );

		void UpdateTriggerFiredStateL( RArray<TLbtTriggerId>& aTriggerIds,
									   TBool aFireBool,
									   TInt aOpCode,
			                    	   TRequestStatus& aStatus );
         
         void DeleteTriggersL( CLbtContainerUpdateFilter* aFilter,
         					   TInt aOpCode, 
         					   TRequestStatus& aStatus,
         					   TLbtSecurityPolicy aSecurityPolicy );
    
         void AddAsyncOpToQueue( CLbtContainerAOOperation* aAsyncOp);
         
         void StartNextAsyncOperation();
         
         void CreateTriggerL(CLbtContainerTriggerEntry* aEntry,TInt aOpCode,TRequestStatus& aStatus); 
         
         void CompleteAsyncRequest(TInt aErr);
         
         void CancelAsyncRequest(TInt aOpId);
         
         void NotifyTriggerStoreReadyToServe();
    private:
        //Derived from CActive
        void RunL();
        TInt RunError( TInt aError );
        void DoCancel();

    private:
        //C++ default constructor
      
        CLbtContainerAO(RPointerArray<MLbtTriggerStore>& aTriggerStores,
                        RArray<CLbtContainer::TLbtTriggerStoreChangeObserver>& aObservers );
    

        //Second phase constructor
        void ConstructL();
        void NotifyListeners();
        void ProcessFilter(TBool& aIsRequested,TLbtTriggerEventMask& aRequestedMask,TLbtTriggerEventMask& aEventMask);
        void SortTriggers(RPointerArray<CLbtContainerTriggerEntry>& aTriggers);
        void SortByName(RPointerArray<CLbtContainerTriggerEntry>& aTriggers);
        void SortByDistance(RPointerArray<CLbtContainerTriggerEntry>& aTriggers);
        void ReverseArray(RPointerArray<CLbtContainerTriggerEntry>& aTriggers);

    private:
    TRequestStatus* iClientStatus;
    
    TContainerAOOperation iAsyncRequest;
    
    RPointerArray<MLbtTriggerStore>& iTriggerStores;
    TInt iTriggerStoreCtr;
    RPointerArray <CLbtContainerAOOperation> iAsyncOpQueue;
    RArray<CLbtContainer::TLbtTriggerStoreChangeObserver>& iObservers;
    TLbtTriggerDataMask iDataMask;
    TLbtTriggerAttributeFieldsMask iAttrMask;
    CLbtListTriggerOptions::TLbtListTriggerSorting iSortingOption;
    };

#endif // LBTCONTAINERAO_H

