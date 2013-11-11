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
* Description:   SUPL settings respository class
*
*/




#include "epos_comasupltrace.h"
#include "epos_comasuplsettingsrepository.h"
#include "epos_omasuplprovhandler.pan"
#include "epos_omasuplprovhandler.hrh"

#include "epos_csuplsettingparams.h"

_LIT( KTraceSuplSettingsRepository, "epos_comasuplsettingsrepository.cpp" );

const TInt KMaxLengthBuffer = 255;


// -----------------------------------------------------------------------------
// COMASuplSettingsRepository::NewL
// Factory method that creates an instance of COMASuplSettings
// -----------------------------------------------------------------------------
//
COMASuplSettingsRepository* COMASuplSettingsRepository::NewL()
    {
    COMASuplSettingsRepository* self = new ( ELeave ) COMASuplSettingsRepository();
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop();
    return self;
    }


// -----------------------------------------------------------------------------    
// Destructor
//
// -----------------------------------------------------------------------------
//
COMASuplSettingsRepository::~COMASuplSettingsRepository()
    {
    Deque();
    delete iSettings;
    delete iTrace;
    }


// -----------------------------------------------------------------------------
// COMASuplSettingsRepository::SaveL
//
// -----------------------------------------------------------------------------
//

void COMASuplSettingsRepository::SaveL( const TDesC& aIAP, const TDesC& aHSLP )
    {
      
    iTrace->Trace( _L( "COMASuplSettingsRepository::SaveL() Start" ), 
                                            KTraceSuplSettingsRepository, __LINE__ );
    
    if( iState == ENotInitialized || iState == EInitializing )
        {
        iTrace->Trace( _L( "Settings Not Initialized" ), 
                                            KTraceSuplSettingsRepository, __LINE__ );
        User::Leave( KErrNotReady );
        }
    
    
    CServerParams* paramValues = CServerParams::NewL();
    CleanupStack::PushL(paramValues);

    HBufC* serverAddress = HBufC::NewL(KMaxLengthBuffer);
    CleanupStack::PushL(serverAddress);

    HBufC* iapName = HBufC::NewL(KMaxLengthBuffer);
    
    CleanupStack::PushL(iapName);
    
    if( aIAP.Length() )
        {
        *iapName = aIAP; 
        }
    else
        {
        iTrace->Trace( _L( "IAP Name is NULL" ), 
                                         KTraceSuplSettingsRepository, __LINE__ );
        }
    
    if ( aHSLP.Length() )
        {
        *serverAddress = aHSLP;
        }
    else
        {
        iTrace->Trace( _L( "Server name is NULL" ), 
                                         KTraceSuplSettingsRepository, __LINE__ );
        }
        
    paramValues->Set(*serverAddress,*iapName);
    
    TInt64 slpID;
    
    paramValues->SetServerConfigurationType(EFalse);
    TInt err = iSettings->AddNewServer(paramValues,slpID);
    if (err == KErrNone || err == KErrAlreadyExists)
    iSettings->SetTopPriority(slpID);
    
    
    
    
    if ( KErrNone != err )
        {
        	TBuf<32> errcode;
    			errcode.AppendNum(err);
    			
        iTrace->Trace( _L( "Error updating SUPL Settings" ), 
                                 KTraceSuplSettingsRepository, __LINE__ );
        iTrace->Trace( errcode,KTraceSuplSettingsRepository, __LINE__ );
        User::Leave( err );
        }
        
    CleanupStack::PopAndDestroy(3);
		
    
    }

// -----------------------------------------------------------------------------
// COMASuplSettingsRepository::COMASuplSettingsRepository
// C++ default constructor can NOT contain any code, that
// might leave.
// -----------------------------------------------------------------------------
//
COMASuplSettingsRepository::COMASuplSettingsRepository():CActive( EPriorityNormal )
    {
    CActiveScheduler::Add( this );
    }
    

// -----------------------------------------------------------------------------    
// Initialize
//
// -----------------------------------------------------------------------------
//
void COMASuplSettingsRepository::ConstructL()
    {
    iSettings = CSuplSettingsInternal::NewL();
    iTrace = COMASuplTrace::NewL();
    iSettings->Initialize( iStatus );
    iState = EInitializing;
    SetActive();
    }


// -----------------------------------------------------------------------------
// COMASuplSettingsRepository::RunL
// 
// -----------------------------------------------------------------------------
//
void COMASuplSettingsRepository::RunL()
    {
    if( KErrNone == iStatus.Int() )
        {
        iState = EInitialized;
        }
    else
        {
        iTrace->Trace( _L( "Error Initializing SUPL Settings" ), 
                                        KTraceSuplSettingsRepository, __LINE__ );
        iState = ENotInitialized;
        }
    }


// -----------------------------------------------------------------------------
// COMASuplSettingsRepository::DoCancel
// Cancels an outstanding request to initialize SUPL Settings
// -----------------------------------------------------------------------------
//
void COMASuplSettingsRepository::DoCancel()
    {
    if ( iState == EInitializing )
        {
        iSettings->CancelInitialize();
        }
    }
