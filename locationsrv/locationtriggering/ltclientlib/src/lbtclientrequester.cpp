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
#include <s32strm.h>
#include <s32mem.h>
#include <lbt.h>

#include "lbtclientrequester.h"
#include "lbttriggerinfo.h"





// ================= LOCAL FUNCTIONS ========================
 
// ================= MEMBER FUNCTIONS =======================

// ---------------------------------------------------------
// CLbtClientRequester::CLbtClientRequester
//
// Default constructor
// ---------------------------------------------------------
//
CLbtClientRequester::CLbtClientRequester( RLbt& aLbt ):
                    CActive(EPriorityStandard),iLbt(aLbt),
                    iClientStatus(NULL)
   
    {
    CActiveScheduler::Add(this);
    }

// ---------------------------------------------------------------------------
// CLbtClientRequester::ConstructL()
// Symbian 2nd phase constructor can leave.
// ---------------------------------------------------------------------------
//
void CLbtClientRequester::ConstructL()
    {

    }

// ---------------------------------------------------------------------------
// CLbtClientRequester::NewL()
//
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//
CLbtClientRequester* CLbtClientRequester::NewL( RLbt& aLbt )
    {
    CLbtClientRequester* self = new (ELeave) CLbtClientRequester( aLbt );
    CleanupStack::PushL(self);
    self->ConstructL();
    CleanupStack::Pop(self);
    return self;
    }
   

// ---------------------------------------------------------------------------
// CLbtClientRequester::~CLbtClientRequester()
// Destructor
// ---------------------------------------------------------------------------
//
CLbtClientRequester::~CLbtClientRequester()
    {
    if (IsActive())      
      	Cancel();
    iClientBuf.Close();
    }


// ---------------------------------------------------------
// CLbtTriggerChangeEventNotifier::Start
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CLbtClientRequester::Start()
    {
 	iType = EOpOther;
 	SetActive();
    }

// ---------------------------------------------------------
// CLbtTriggerChangeEventNotifier::Start
//
// (other items were commented in a header).
// ---------------------------------------------------------
//    
void CLbtClientRequester::Start(TAsyncReqType aType)
    {
    iType = aType;
    SetActive();
    }
 

// ---------------------------------------------------------
// CLbtTriggerChangeEventNotifier::Status
//
// (other items were commented in a header).
// ---------------------------------------------------------
// 
 TRequestStatus& CLbtClientRequester::Status()
    {
    return *iClientStatus;	
    }

// ---------------------------------------------------------
// CLbtTriggerChangeEventNotifier::SetStatus
//
// (other items were commented in a header).
// ---------------------------------------------------------
// 
void CLbtClientRequester::SetStatus(TRequestStatus& aStatus)
    {
	iClientStatus=&aStatus;
    }

// ---------------------------------------------------------
// CLbtTriggerChangeEventNotifier::SetBuffer
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CLbtClientRequester::SetBuffer(CBufFlat* aBuf )
    {
	iClientBuf.Reset();
	iClientBuf.Append(aBuf);
    }

// ---------------------------------------------------------
// CLbtTriggerChangeEventNotifier::SetListTriggerIdData
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CLbtClientRequester::SetListTriggerIdData(RArray < TLbtTriggerId >* aTriggerIdArray)
    {
    iTriggerIdArray = aTriggerIdArray;
    }

// ---------------------------------------------------------
// CLbtTriggerChangeEventNotifier::SetListTriggerIdData
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
RArray < TLbtTriggerId >* CLbtClientRequester::ListTriggerIdData()
    {
    return iTriggerIdArray;
    }
    
// ---------------------------------------------------------
// CLbtTriggerChangeEventNotifier::SetListTriggerData
//
// (other items were commented in a header).
// ---------------------------------------------------------
//    
void CLbtClientRequester::SetListTriggerData(RPointerArray < CLbtTriggerInfo >* aTriggerInfoList)
    {
    iTriggerInfoList = aTriggerInfoList;
    }

// ---------------------------------------------------------
// CLbtTriggerChangeEventNotifier::ListTriggerData
//
// (other items were commented in a header).
// ---------------------------------------------------------
//    
RPointerArray < CLbtTriggerInfo >* CLbtClientRequester::ListTriggerData()
    {
    return iTriggerInfoList;
    }
    
// ---------------------------------------------------------
// CLbtClientRequester::RunL
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CLbtClientRequester::RunL()
    {
    TInt bufCount=iClientBuf.Count();
    TInt i=0;
    while(i<bufCount)
        {
        delete iClientBuf[i];
        iClientBuf[i]=NULL;
        i++;
        }

    switch(iType)
        {
        case EOpOther: 
        	{
        	break;
        	}
        case EListTriggerIdsCount:
            {
            TRAPD(error,iLbt.HandleListTriggerIdsL());
            if( error != KErrNone )
                {
                User::RequestComplete(iClientStatus,error);
                return;
                }
            break;
            }
        case EListTriggersCount:
            {
            TRAPD(error,iLbt.HandleGetTriggersL());
            if( error != KErrNone )
                {
                User::RequestComplete(iClientStatus,error);
                return;
                }
            break;
            }
        }
   
   TInt error=iStatus.Int();
   User::RequestComplete(iClientStatus,error);
   }


// ---------------------------------------------------------
// CLbtClientRequester::RunError
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
TInt CLbtClientRequester::RunError(TInt /*aError*/)
    {
    return KErrNone;
    }

// ---------------------------------------------------------
// CLbtClientRequester::DoCancel
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CLbtClientRequester::DoCancel()
    {    
    User::RequestComplete(iClientStatus, iStatus.Int());
    }

// ---------------------------------------------------------
// CLbtClientRequester::ListTriggersSize
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
TInt& CLbtClientRequester::ListTriggersSize()
    {
    return iListTriggersSize;   
    }
    
//  End of File


