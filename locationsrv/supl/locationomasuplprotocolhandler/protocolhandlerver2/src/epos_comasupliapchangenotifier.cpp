/*
* Copyright (c) 2007 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:   Notifier for any change in IAP
*
*/



// INCLUDE FILES
#include "epos_comasupltrace.h"
#include "epos_comasupliapchangenotifier.h"

_LIT(KTraceFileName,"SUPL_OMA_SESSION::EPos_COMASuplIapChangeNotifier.cpp");

// ============================ MEMBER FUNCTIONS OF COMASuplIapNotifier ===============================

// -----------------------------------------------------------------------------
// COMASuplIapNotifier::COMASuplIapNotifier,constuctor
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//

COMASuplIapNotifier::COMASuplIapNotifier(MOMASuplIapChangeNotifierObserver& aObserver) 
					    : CActive(EPriorityStandard),
					      iObserver(aObserver)
	{
	CActiveScheduler::Add(this);            
	}

// -----------------------------------------------------------------------------
// COMASuplIapNotifier::ConstructL,two phase constuctor
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void COMASuplIapNotifier::ConstructL()
    {
    iTrace = COMASuplTrace::NewL();	
    iTrace->Trace(_L("COMASuplIapNotifier::ConstructL"), KTraceFileName, __LINE__); 
    iCommDb = CCommsDatabase::NewL(EDatabaseTypeIAP);
    }

// -----------------------------------------------------------------------------
// COMASuplIapNotifier::NewL,two phase constuctor
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
COMASuplIapNotifier* COMASuplIapNotifier::NewL(MOMASuplIapChangeNotifierObserver& aObserver)
    {
    COMASuplIapNotifier* self = new( ELeave ) COMASuplIapNotifier(aObserver);
    
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop(self);

    return self;
    }


// -----------------------------------------------------------------------------
// COMASuplIapNotifier::DoCancel
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void COMASuplIapNotifier::DoCancel()
    {
    iCommDb->CancelRequestNotification(); 
    }            

// -----------------------------------------------------------------------------
// COMASuplIapNotifier::~COMASuplIapNotifier,Destructor
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
COMASuplIapNotifier::~COMASuplIapNotifier()
    {
    Cancel();
    delete iCommDb;
    delete iTrace;
    }
            
// -----------------------------------------------------------------------------
// COMASuplIapNotifier::CheckForRoaming,For retriving the cell id information
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void COMASuplIapNotifier::NotifyIapSettingsChange()
    {
    iTrace->Trace(_L("COMASuplIapNotifier::NotifyIapSettingsChange..."),KTraceFileName, __LINE__);
    iCommDb->RequestNotification( iStatus);
    SetActive();            
    }  
      
// -----------------------------------------------------------------------------
// COMASuplIapNotifier::RunL
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void COMASuplIapNotifier::RunL()
    {
    if (iStatus.Int() == 2)
        {            
        TBuf<128> msg(_L("COMASuplIapNotifier::RunL, status "));
        msg.AppendNum(iStatus.Int());
        iTrace->Trace(msg, KTraceFileName, __LINE__); 
        iObserver.HandleIapSettingsChangeL(iStatus.Int());
        }
    NotifyIapSettingsChange();
    }

// -----------------------------------------------------------------------------
// COMASuplIapNotifier::RunError, Will be called by framework,if RunL leaves.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TInt COMASuplIapNotifier::RunError(TInt aError)
	{
    TRAP_IGNORE(iObserver.HandleIapSettingsChangeL(aError));    
    return KErrNone;
	}

//  End of File  
