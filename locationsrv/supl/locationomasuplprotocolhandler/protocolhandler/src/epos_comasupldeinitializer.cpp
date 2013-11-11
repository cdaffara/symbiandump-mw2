/*
* Copyright (c) 2009 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:   Class for doing asynchronous initilization of Protocol Handler.
*
*/


#include "epos_comasupldeinitializer.h"
#include "epos_comasupltrace.h"

_LIT(KTraceFileName,"SUPL_OMA_PH::EPos_COMASUPLDeInitializer.cpp");
// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// COMASuplDeInitilizer::COMASuplDeInitilizer
// C++ constructor can NOT contain any code, that might leave.
// -----------------------------------------------------------------------------
//
COMASuplDeInitilizer::COMASuplDeInitilizer( MCompleteDeInitialization& aObserver ):
    				 								   CActive( EPriorityStandard),
    				 								   iObserver( aObserver ),
    				 								   iOMASuplProtocolHandler(NULL)
    				 								   
    {
    CActiveScheduler::Add( this );
    }

// -----------------------------------------------------------------------------
// COMASuplDeInitilizer::ConstructL
// Symbian 2nd phase constructor can leave.
// -----------------------------------------------------------------------------
//
void COMASuplDeInitilizer::ConstructL()
    {
    iTrace = COMASuplTrace::NewL();	    
    }
    
    
// -----------------------------------------------------------------------------
// COMASuplDeInitilizer::NewL
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
COMASuplDeInitilizer* COMASuplDeInitilizer::NewL( MCompleteDeInitialization& aObserver )
    {
	    COMASuplDeInitilizer* self =  new ( ELeave ) 
	    									   COMASuplDeInitilizer( aObserver );
	    CleanupStack::PushL( self );
	    self->ConstructL();
	    CleanupStack::Pop(self);
	    return self;
    }

// -----------------------------------------------------------------------------
// COMASuplDeInitilizer::~COMASuplDeInitilizer
// -----------------------------------------------------------------------------
//
COMASuplDeInitilizer::~COMASuplDeInitilizer()
    {
    Cancel();
	delete iTrace;	
	iTrace = NULL;
	}

  
// -----------------------------------------------------------------------------
// COMASuplDeInitilizer::RunL
// -----------------------------------------------------------------------------
//
void COMASuplDeInitilizer::RunL()
    {
	iTrace->Trace(_L("COMASuplDeInitilizer::RunL"), KTraceFileName, __LINE__);
	iObserver.DeInitilizationCompletedL( iStatus.Int() );
    }

// -----------------------------------------------------------------------------
// COMASuplDeInitilizer::DoCancel
// -----------------------------------------------------------------------------
//
void COMASuplDeInitilizer::DoCancel()
    {    
	iTrace->Trace(_L("COMASuplDeInitilizer::DoCancel"), KTraceFileName, __LINE__);
	iOMASuplProtocolHandler->CancelDeInitialize();
    }

// -----------------------------------------------------------------------------
// COMASuplDeInitilizer::Deinitialize
// -----------------------------------------------------------------------------
//
void COMASuplDeInitilizer::Deinitialize( CSUPLProtocolManagerBase* aOMASuplProtocolHandler )
	{
	iTrace->Trace(_L("COMASuplDeInitilizer::Deinitialize"), KTraceFileName, __LINE__);
	if( !IsActive() )
		{
		iTrace->Trace(_L("COMASuplDeInitilizer::Deinitialize issued"), KTraceFileName, __LINE__);
		iOMASuplProtocolHandler=aOMASuplProtocolHandler;
		iOMASuplProtocolHandler->DeInitialize(iStatus);
		SetActive();
		}
	}

//  End of File
