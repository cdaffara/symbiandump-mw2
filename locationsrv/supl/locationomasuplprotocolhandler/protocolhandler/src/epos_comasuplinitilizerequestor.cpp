/*
* Copyright (c) 2002-2009 Nokia Corporation and/or its subsidiary(-ies). 
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



#include <centralrepository.h>

#include "lbs/epos_comasuplposhandlerbase.h"
#include "epos_omasuplconfigurationkeys.h"

#include "epos_comasupltrace.h"
#include "epos_comasuplinitilizerequestor.h"
#include "epos_comasuplsettings.h"

static const TUid KSUPLVarRepositoryUid = { 0x102750A0 };
const TInt KMaxIPAddressLength = 256;

_LIT(KTraceFileName,"SUPL_OMA_PH::EPos_COMASUPLInitializeRequestor.cpp");

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// COMASuplInitilizeRequestor::COMASuplInitilizeRequestor
// C++ constructor can NOT contain any code, that might leave.
// -----------------------------------------------------------------------------
//
COMASuplInitilizeRequestor::COMASuplInitilizeRequestor(COMASuplPosHandlerBase *aPosHandler,
													   MCompleteInitilization& aObserver,COMASuplSettings* aOMASuplSettings):
    				 								   CActive( EPriorityStandard),
    				 								   iPosHandler(aPosHandler),
    				 								   iObserver( aObserver ),
    				 								   iOMASuplSettings (aOMASuplSettings)
    {
    }

// -----------------------------------------------------------------------------
// COMASuplInitilizeRequestor::ConstructL
// Symbian 2nd phase constructor can leave.
// -----------------------------------------------------------------------------
//
void COMASuplInitilizeRequestor::ConstructL()
    {
        iTrace = COMASuplTrace::NewL();	
        iSuplSettings = CSuplSettings::NewL();
    	CActiveScheduler::Add( this );
    	iState = ENotInitialized;
    	iVariantEnabled = ETrue;
    	SetVariant();
    }
    
// -----------------------------------------------------------------------------
// COMASuplInitilizeRequestor::SetVariant()
// Checks if IMSI variant is enabled
// -----------------------------------------------------------------------------
//
void COMASuplInitilizeRequestor::SetVariant()
    {
	iTrace->Trace(_L("COMASuplInitilizeRequestor::SetVariant Start"), KTraceFileName, __LINE__);
    CRepository *repository = NULL;
    TRAPD( err, repository = CRepository::NewL( KSUPLVarRepositoryUid ) );
    if ( KErrNone != err )
        {
        return;
        }
    TInt val;
    TInt rc = repository->Get( KOMASuplProtocolHandlerVariant, val );
    if ( KErrNone == rc && val == 0 )
        {
        iVariantEnabled = EFalse;
        }
    delete repository;
	iTrace->Trace(_L("COMASuplInitilizeRequestor::SetVariant End"), KTraceFileName, __LINE__);
    }    
    
// -----------------------------------------------------------------------------
// COMASuplInitilizeRequestor::NewL
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
COMASuplInitilizeRequestor* COMASuplInitilizeRequestor::NewL(COMASuplPosHandlerBase *aPosHandler,
															 MCompleteInitilization& aObserver,COMASuplSettings* aOMASuplSettings)
    {
	    COMASuplInitilizeRequestor* self =  new ( ELeave ) 
	    									   COMASuplInitilizeRequestor(aPosHandler,aObserver,aOMASuplSettings);
	    CleanupStack::PushL( self );
	    self->ConstructL();
	    CleanupStack::Pop(self);
	    return self;
    }

// -----------------------------------------------------------------------------
// COMASuplInitilizeRequestor::~COMASuplInitilizeRequestor
// -----------------------------------------------------------------------------
//
COMASuplInitilizeRequestor::~COMASuplInitilizeRequestor()
    {
    	Cancel();
	    delete iTrace;	
    	delete iSuplSettings;
    }

// -----------------------------------------------------------------------------
// COMASuplInitilizeRequestor::CompleteSelf
// -----------------------------------------------------------------------------
//
void COMASuplInitilizeRequestor::CompleteSelf()
    {
	iTrace->Trace(_L("COMASuplInitilizeRequestor::CompleteSelf Start"), KTraceFileName, __LINE__);
		TRequestStatus* pStat = &iStatus;
		User::RequestComplete(pStat, KErrNone);
		SetActive();
	iTrace->Trace(_L("COMASuplInitilizeRequestor::CompleteSelf End"), KTraceFileName, __LINE__);
    }
    
// -----------------------------------------------------------------------------
// COMASuplInitilizeRequestor::DoInitilizationL
// Starts initilization of POS if present
// -----------------------------------------------------------------------------
//
void COMASuplInitilizeRequestor::DoInitilizationL()
    {	
	iTrace->Trace(_L("COMASuplInitilizeRequestor::DoInitilizationL Start"), KTraceFileName, __LINE__);
		if(iPosHandler)
		{
		  	iPosHandler->InitializeL(iStatus);
		  	iState = EInitializePosHandler;
		  	SetActive();
		}    		
		else
		{
			CompleteSelf();
		}
	iTrace->Trace(_L("COMASuplInitilizeRequestor::DoInitilizationL End"), KTraceFileName, __LINE__);
    }
    
// -----------------------------------------------------------------------------
// COMASuplInitilizeRequestor::GetIMSI
// Retrieves IMSI value from SIM using SUPL Settings API
// -----------------------------------------------------------------------------
//
TInt COMASuplInitilizeRequestor::GetIMSI(TDes& aIMSI)
    {
	iTrace->Trace(_L("COMASuplInitilizeRequestor::GetIMSI"), KTraceFileName, __LINE__);
        TInt err = KErrNone;
        if( iVariantEnabled )
            {
            if( iState == EInitializationComplete )
                {
                err = iSuplSettings->GetImsi( aIMSI );
                if( err != KErrNone )
                    return err;
                }
            else
                return KErrNotReady;
            }
        else
        	{
        	return KErrNotSupported;
        	} 
        return err;
    }
  
// -----------------------------------------------------------------------------
// COMASuplInitilizeRequestor::RunL
// -----------------------------------------------------------------------------
//
void COMASuplInitilizeRequestor::RunL()
    {
	iTrace->Trace(_L("COMASuplInitilizeRequestor::RunL"), KTraceFileName, __LINE__);
    		switch( iState )
    		    {
    		        case EInitializePosHandler:
    		            if( iVariantEnabled && iStatus.Int() == KErrNone)
    		                {
    		                iState = EInitializeSuplSettings;
    		                iSuplSettings->Initialize( iStatus );        		            
        		            SetActive();
        		            break;
    		                }

    		        case EInitializeSuplSettings:
    		        	{
    		        	RBuf HslpFromImsi;
    		        	HslpFromImsi.CreateL(KMaxIPAddressLength);
									CleanupClosePushL(HslpFromImsi);
						
									//Coverity fix - checking for return code of GenerateHslpAddressFromImsi
						
									TInt genError = iSuplSettings->GenerateHslpAddressFromImsi( HslpFromImsi );
									if(genError != KErrNone)
						    		{
                    iTrace->Trace(_L("GenerateHslpAddressFromImsi failed - completing observer"), KTraceFileName, __LINE__);
										CleanupStack::PopAndDestroy();
                    iObserver.InitilizationCompletedL( genError );                            
                    return;
						    		}
               		 iOMASuplSettings->SetHSLPAddressFromIMSI(HslpFromImsi);
                	 CleanupStack::PopAndDestroy();    		            
                	 iState = EInitializationComplete;             	    

    		        	}
						//comment to ignore coverity missing break error
    		        	//coverity[MISSING_BREAK  :FALSE]
    		        default:
    		        	if( iState != EInitializationComplete && iVariantEnabled )
    		        		{
    		                iSuplSettings->Initialize( iStatus );
        		            iState = EInitializeSuplSettings;
        		            SetActive();
        		            break;
    		        		}
                        iObserver.InitilizationCompletedL( iStatus.Int() );
    		    }
    }

// -----------------------------------------------------------------------------
// COMASuplInitilizeRequestor::DoCancel
// -----------------------------------------------------------------------------
//
void COMASuplInitilizeRequestor::DoCancel()
    {    
	iTrace->Trace(_L("COMASuplInitilizeRequestor::DoCancel"), KTraceFileName, __LINE__);
		if( iState == EInitializePosHandler )
		{
			iPosHandler->CancelInitialize();
		}
		if( iState == EInitializeSuplSettings )
		{
		    iSuplSettings->CancelInitialize();
		}
    }

//  End of File
