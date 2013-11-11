/*
* Copyright (c) 2005 Nokia Corporation and/or its subsidiary(-ies).
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


// INCLUDE FILES
#include <e32base.h>
#include "lbtlisttriggerao.h"
#include "lbtlisttriggerobserver.h"
#include "lbtlogger.h"





// ================= LOCAL FUNCTIONS ========================
 
// ================= MEMBER FUNCTIONS =======================

// C++ default constructor can NOT contain any code, that
// might leave.
//

CLbtListTriggerAO::CLbtListTriggerAO(MLbtListTriggerObserver& aObserver ):
CActive(EPriorityStandard),
iClientStatus(NULL),
iObserver(aObserver)   
    
      {
   
    CActiveScheduler::Add(this);
    
    }

// EPOC default constructor can leave.
void CLbtListTriggerAO::ConstructL()
    {

    }

// Two-phased constructor.
CLbtListTriggerAO* CLbtListTriggerAO::NewL(MLbtListTriggerObserver& aObserver)
    {
   
    CLbtListTriggerAO* self = new (ELeave) CLbtListTriggerAO(aObserver);
    CleanupStack::PushL(self);
    self->ConstructL();
    CleanupStack::Pop(self);
    return self;
    }
   

// Destructor
CLbtListTriggerAO::~CLbtListTriggerAO()
    {
   
	if (IsActive())      
      	Cancel();
    }


// ---------------------------------------------------------
// CLbtTriggerChangeEventNotifier::Start
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CLbtListTriggerAO::Start()
    {
 	SetActive();
    }
    

 

void CLbtListTriggerAO::ListTriggers(
    CLbtContainerListOptions* aFilter,
    RPointerArray < CLbtContainerTriggerEntry >& aTriggers,
    RPointerArray<MLbtTriggerStore>& aTriggerStores)
    {
    TInt error = KErrNone;    
    TRequestStatus* status=&iStatus;
    for (TInt i = 0; i < aTriggerStores.Count(); i++)
        {
            TRAP(error, aTriggerStores[i]->ListTriggersL(aFilter,aTriggers););
        if ( error != KErrNotFound )
            {
            break;
            }
        }
    SetActive();    
    User::RequestComplete(status,error);            
    }
// ---------------------------------------------------------
// CLbtListTriggerAO::RunL
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CLbtListTriggerAO::RunL()
    {
    FUNC_ENTER("CLbtListTriggerAO::RunL");
   	TInt error=iStatus.Int();
   	iObserver.ListTriggerOpComplete(error);       
    }

// ---------------------------------------------------------
// CLbtListTriggerAO::RunError
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
TInt CLbtListTriggerAO::RunError(TInt /*aError*/)
    {
   
    return KErrNone;
    }

// ---------------------------------------------------------
// CLbtListTriggerAO::DoCancel
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CLbtListTriggerAO::DoCancel()
    {

    
    }


 
    
//  End of File
