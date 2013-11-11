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
* Description:   Class to handle Product Configuration
*
*/



// INCLUDE FILES
#include <e32base.h>
#include <sbdefs.h>
#include <e32property.h>
#include <startupdomainpskeys.h>
#include <flogger.h>
#ifndef RD_STARTUP_CHANGE
	#include <PSVariables.h>
	#include <sysstartup.h>
#endif

#include <epos_csuplsettings.h>
#include "epos_csuplsettingsinternal.h"
#include "epos_comasupltrace.h"
#include "epos_comasuplproductconfig.h"
#include "epos_csuplsettingparams.h"
#include "epos_comasuplproductconfigcrkeys.h"
#include "epos_mproductconfigobserver.h"


const TInt KMaxServerNameLength = 255;

_LIT( KProductConfig, "epos_comasuplproductconfig.cpp" );



// CONSTANTS

// ================= MEMBER FUNCTIONS =======================

// C++ default constructor can NOT contain any code, that
// might leave.
//
COMASuplProductConfig::COMASuplProductConfig(MProductConfigObserver& aObserver)
    : CActive(EPriorityStandard), iProductObserver(aObserver)
    {
    CActiveScheduler::Add(this);
    }

// Destructor
COMASuplProductConfig::~COMASuplProductConfig()
    {
    Trace(_L("COMASuplProductConfig::~COMASuplProductConfig" ),KProductConfig,__LINE__ );
        if (iSettings)
	        
	    delete iSettings;
        iSettings = NULL;
        iStartupObserver.Close();
    }

// Two-phased constructor
COMASuplProductConfig* COMASuplProductConfig::NewL(MProductConfigObserver& aObserver)
    {
    COMASuplProductConfig* self = new (ELeave) COMASuplProductConfig(aObserver);
    CleanupStack::PushL(self);
    self->ConstructL();
    CleanupStack::Pop(self);
    return self;
    }

// EPOC default constructor
void COMASuplProductConfig::ConstructL()
    {
    Trace(_L("COMASuplProductConfig::ConstructL"),KProductConfig, __LINE__);

        iProductRepository = CRepository::NewL(KCRUidProductConfig);
        iSettings = CSuplSettingsInternal::NewL();
        iSettings->DeleteAllSessionRecords();//Remove all active sessions if any...
		Trace(_L("End of COMASuplProductConfig::ConstructL" ),KProductConfig,__LINE__ );

    }
// ---------------------------------------------------------
// COMASuplProductConfig::StartProductConfiguration
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void COMASuplProductConfig::StartProductConfigurationL()
    {
        Trace(_L("Start of Product Configuration"),KProductConfig, __LINE__);
        
        TUint32 key = KProductConfigActualKeysStart;
        
        User::LeaveIfError(iServerAddress.Create(KMaxServerNameLength));
        User::LeaveIfError(iIapName.Create(KMaxServerNameLength));
        
        TBool serverEnabled;
        TBool simChangeRemove;
        TBool usageInHomeNw;
        TBool editable;
        
        TInt64 slpId;
        TInt noOfServers = 0;
        
        CServerParams* params = CServerParams::NewL();
        TInt value;
        iProductRepository->Get(KProductConfigFlag,value);
        iProductRepository->Get(KProductConfigNumberOfServers,noOfServers);
        
        TBuf<32> errCode;
        errCode.AppendNum(noOfServers);
        Trace(_L("Number of servers:"),KProductConfig, __LINE__);
        Trace(errCode,KProductConfig, __LINE__);
        errCode.Zero();
        
        
        if(value) 
            {
            for( TInt i = 0; i < noOfServers; i++ )
                {
                serverEnabled = ETrue;
                simChangeRemove = EFalse;
                usageInHomeNw = ETrue;
                editable = ETrue;

                TInt err = iProductRepository->Get(key,iServerAddress);
                key++;
                Trace(iServerAddress,KProductConfig, __LINE__);
                if(err == KErrNone)
                    {
                    iProductRepository->Get(key,iIapName);
                    key++;
                    Trace(iIapName,KProductConfig, __LINE__);
                    iProductRepository->Get(key,serverEnabled);
                    key++;
                    iProductRepository->Get(key,simChangeRemove);
                    key++;
                    iProductRepository->Get(key,usageInHomeNw);
                    key++;
                    iProductRepository->Get(key,editable);
                    key++;
                    }
                else
                    {
                    key = key + 5;
                    }
                if(iServerAddress.Length()!=0)
                    {
                    //last parameter is ETrue since this is a product configured server
                    err = params->Set(iServerAddress,iIapName,serverEnabled,simChangeRemove,usageInHomeNw,editable,0);//by default product config
                    errCode.AppendNum(err);
                    Trace(_L("params->Set returned:"),KProductConfig, __LINE__);
                    Trace(errCode,KProductConfig, __LINE__);
                    errCode.Zero();
                    err = iSettings->AddNewServer(params,slpId);
                    errCode.AppendNum(err);
                    Trace(_L("Add new Server returned:"),KProductConfig, __LINE__);
                    Trace(errCode,KProductConfig, __LINE__);
                    errCode.Zero();
                    }

                }
            iProductRepository->Set(KProductConfigFlag,0);
            }
   
            delete iSettings;
            iSettings = NULL;
            delete params;
            params = NULL;
            delete iProductRepository;
            iProductRepository = NULL;
            
            iServerAddress.Close();
            iIapName.Close();
            Trace(_L("End of Product Configuration"),KProductConfig, __LINE__);
            
            iProductObserver.HandleProductConfigCompleteL();
    }
// ---------------------------------------------------------
// COMASuplProductConfig::RunL
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void COMASuplProductConfig::RunL()
    {
    
    if(iStatus.Int() == KErrNone)
    	{
    	Trace( _L( "Boot Completed..." ),KProductConfig,__LINE__ );
    	StartProductConfigurationL();
    	}
    }

// ---------------------------------------------------------
// COMASuplProductConfig::RunError()
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
TInt COMASuplProductConfig::RunError(TInt /*aError*/)
    {
    return KErrNone;
    }

// ---------------------------------------------------------
// COMASuplProductConfig::DoCancel
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void COMASuplProductConfig::DoCancel()
    {
		
    }

// ---------------------------------------------------------------------------
// CheckForBootCompletion
// Check boot is completed... if its then do product configuration
// ---------------------------------------------------------------------------
//
void COMASuplProductConfig::CheckForBootCompletionL()
	{
		
        User::LeaveIfError( iStartupObserver.Attach( KPSUidStartup, 
                                        KPSGlobalSystemState, EOwnerThread ) );

        TInt bootCompleteStatus = 0;
        User::LeaveIfError( iStartupObserver.Get( bootCompleteStatus ) );
        
         if(! ((bootCompleteStatus == ESwStateNormalRfOn ) ||
            (bootCompleteStatus == ESwStateNormalRfOff ) ||
            (bootCompleteStatus == ESwStateNormalBTSap ) )
            )
            {
            	iState = EBootCompletion;
	            iStartupObserver.Subscribe( iStatus );
	            SetActive();
            }
         else
             {
             StartProductConfigurationL();
             }

	}
	
// ---------------------------------------------------------------------------
// Wrapper function used for tracing
// ---------------------------------------------------------------------------
//   
void COMASuplProductConfig::Trace( const TDesC& aMessage, const TDesC& aFileName, TInt line )
    {
    COMASuplTrace* trace = NULL;
    TRAPD( err, trace = COMASuplTrace::NewL() );
    if ( KErrNone == err )
        {
        trace->Trace( aMessage, aFileName, line );
        delete trace;
        }
    }
	
// End of File
