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
* Description:   Class to handle backup and restore events and resore previous settings manually
*
*/



// INCLUDE FILES
#include <e32base.h>
#include <sbdefs.h>
#include <e32property.h>
#include <startupdomainpskeys.h>

#ifndef RD_STARTUP_CHANGE
	#include <PSVariables.h>
	#include <sysstartup.h>
#endif

#include <epos_csuplsettings.h>

#include "epos_comasupltrace.h"
#include "epos_comasuplbackuplistener.h"

const TInt KSettingsParamLen = 512;

_LIT( KBackupListener, "epos_comasuplbackuplistener.cpp" );



// CONSTANTS

// ================= MEMBER FUNCTIONS =======================

// C++ default constructor can NOT contain any code, that
// might leave.
//
COMASuplBackupListener::COMASuplBackupListener()
    : CActive(EPriorityStandard)
    {
    CActiveScheduler::Add(this);
    }

// Destructor
COMASuplBackupListener::~COMASuplBackupListener()
    {
	    Cancel();
	    iBackupProperty.Close();
	    iStartupObserver.Close();
	    iPreviousHSLPAddress.Close();
	    iIAPName.Close();
        if (iSettings)
	        iSettings->RemoveObserver();
	    delete iSettings;
    }

// Two-phased constructor
COMASuplBackupListener* COMASuplBackupListener::NewL()
    {
    COMASuplBackupListener* self = new (ELeave) COMASuplBackupListener;
    CleanupStack::PushL(self);
    self->ConstructL();
    CleanupStack::Pop(self);
    return self;
    }

// EPOC default constructor
void COMASuplBackupListener::ConstructL()
    {
    	iIsLastChangedByConfigurator = EFalse;
    	iRestoreInProgrees = EFalse;
	    iSettings = CSuplSettings::NewL();
	    iSettings->SetObserverL(*this);
	    User::LeaveIfError( iPreviousHSLPAddress.Create( KSettingsParamLen ) );
	    User::LeaveIfError( iIAPName.Create( KSettingsParamLen ));
	    
	    User::LeaveIfError(iBackupProperty.Attach(KUidSystemCategory, KUidBackupRestoreKey));
	    CheckForBootCompletionL();
		Trace( _L( "COMASuplBackupListener::ConstructL" ),KBackupListener,__LINE__ );

    }

// ---------------------------------------------------------
// COMASuplBackupListener::RunL
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void COMASuplBackupListener::RunL()
    {
    
    if(iStatus.Int() == KErrNone)
    	{
    		switch(iState)
    			{
    				case EBootCompletion:
    					{
    						Trace( _L( "Boot Completed..." ),KBackupListener,__LINE__ );
    						InitializeSettings();
    						break;
    					}
    					
    				case ERestore:
    					{
    						Trace( _L( "Backup/Restore started..." ),KBackupListener,__LINE__ );
    						ListenRestoreEventAgainL();
    						break;
    					}
    					
    				case ESettingInitilization:
    					{
    						TRAPD(err,ReadCurrentSettingsL(EFalse))
    						if(err == KErrNone)
    						{
									iState = ERestore;
									iBackupProperty.Subscribe(iStatus);
									SetActive();
								}
    						break;
    					}
    					
    				default:break;	
    			}
    	}
    }

// ---------------------------------------------------------
// COMASuplBackupListener::RunError()
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
TInt COMASuplBackupListener::RunError(TInt /*aError*/)
    {
    return KErrNone;
    }

// ---------------------------------------------------------
// COMASuplBackupListener::DoCancel
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void COMASuplBackupListener::DoCancel()
    {
			iBackupProperty.Cancel();
			iStartupObserver.Cancel();
			if(ESettingInitilization == iState)
			{
				iSettings->CancelInitialize();
			}	
    }

// ---------------------------------------------------------
// COMASuplBackupListener::ListenRestoreEventAgain
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void COMASuplBackupListener::ListenRestoreEventAgainL()
	{
		GetCurrentValue();
		if(iTBURState == conn::EBURRestoreFull || iTBURState == conn::EBURRestorePartial)
		{
			Trace( _L("Restore operation Started..."),KBackupListener, __LINE__ );
			//User::InfoPrint(_L("Start"));
			iRestoreInProgrees = ETrue;
			iState = ERestore;
			iBackupProperty.Subscribe(iStatus);
			SetActive();
		}
		else if( iRestoreInProgrees && iTBURState == conn::EBURNormal)
		{
			//User::InfoPrint(_L("Finish"));
			Trace( _L("Restore operation Finished..."),KBackupListener, __LINE__ );
			iRestoreInProgrees = EFalse;
			if(iIsLastChangedByConfigurator)
				{
					TRAP_IGNORE(ReadCurrentSettingsL(ETrue)) //Save the Settings...
				}
			
		}
		else   //This is for if we recevies Backup events ...which we are going to ignore
		{
			iState = ERestore;
			iBackupProperty.Subscribe(iStatus);
			SetActive();
		}

	}

// ---------------------------------------------------------
// COMASuplBackupListener::InitializeSettings
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void COMASuplBackupListener::InitializeSettings()
    {
	    iState = ESettingInitilization;
	    iSettings->Initialize( iStatus );
	    SetActive();
    }

// ---------------------------------------------------------
// COMASuplBackupListener::GetCurrentValue
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
TInt COMASuplBackupListener::GetCurrentValue()
    {
    //TBURPartType& aState
    TInt res, value;
    res = iBackupProperty.Get(value);
    if (res == KErrNone)
        {
        	iTBURState = (TBURPartType) (value & KBURPartTypeMask);
        }

    return res;
    }

// ---------------------------------------------------------------------------
// CheckForBootCompletion
// Check boot is completed... if its then do initilization of settings
// ---------------------------------------------------------------------------
//
void COMASuplBackupListener::CheckForBootCompletionL()
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
            	Trace( _L( "Boot Completed..." ),KBackupListener,__LINE__ );
            	iState = ERestore;
				iBackupProperty.Subscribe(iStatus);
				SetActive();
            }

	}
// ---------------------------------------------------------------------------
// COMASuplBackupListener::ReadCurrentSettingsL()
// Read & Saves current Settings
// ---------------------------------------------------------------------------
//
void COMASuplBackupListener::ReadCurrentSettingsL(TBool aUsage)
	{
        RBuf hslpaddr;
        User::LeaveIfError( hslpaddr.Create( KSettingsParamLen ) );
        CleanupClosePushL( hslpaddr );
        
        RBuf iapn;
        User::LeaveIfError( iapn.Create( KSettingsParamLen ) );
        CleanupClosePushL( iapn );
        
        RBuf imsi;
        User::LeaveIfError( imsi.Create( KSettingsParamLen ) );
        CleanupClosePushL( imsi );
        
        CSuplSettings::TSuplSettingsChangedBy changedby;
        
        TInt err = iSettings->Get( hslpaddr, iapn, imsi, changedby );
        
        if ( KErrNone != err && KErrGeneral != err )
        	{
        		User::Leave( err );
        	}
       		
       	
       	if(changedby == CSuplSettings::EConfigurator && !aUsage)
       		{
       			//User::InfoPrint(_L("EConfigurator"));
       			iPreviousHSLPAddress.Copy(hslpaddr);
       			iIAPName.Copy(iapn);
       			iIsLastChangedByConfigurator = ETrue;
       		}
       		
        if(aUsage)
        	{
				User::LeaveIfError( iSettings->LockSettingsL() );    
				err = iSettings->Set( iPreviousHSLPAddress, iIAPName, CSuplSettings::EConfigurator );
				if ( KErrNone != err )
				{
					Trace( _L( "Error updating SUPL Settings" ), 
					                            KBackupListener, __LINE__ );
					User::LeaveIfError( iSettings->UnlockSettings( EFalse ) );
					User::Leave( err );
				}
				
				User::LeaveIfError( iSettings->UnlockSettings() );
        	}
        
        CleanupStack::PopAndDestroy(3);

	}
	
// ---------------------------------------------------------------------------
// COMASuplBackupListener::HandleSuplSettingsChangeL()
// Listen for changes in settings...
// ---------------------------------------------------------------------------
//
void COMASuplBackupListener::HandleSuplSettingsChangeL (TSuplSettingsEventType /*aEvent*/)
	{
        RBuf hslpaddr;
        User::LeaveIfError( hslpaddr.Create( KSettingsParamLen ) );
        CleanupClosePushL( hslpaddr );
        
        RBuf iapn;
        User::LeaveIfError( iapn.Create( KSettingsParamLen ) );
        CleanupClosePushL( iapn );
        
        RBuf imsi;
        User::LeaveIfError( imsi.Create( KSettingsParamLen ) );
        CleanupClosePushL( imsi );
        
        CSuplSettings::TSuplSettingsChangedBy changedby;
        
        TInt err = iSettings->Get( hslpaddr, iapn, imsi, changedby );
        
        if(err == KErrNone && changedby == CSuplSettings::EConfigurator)
        {
						iPreviousHSLPAddress.Copy(hslpaddr);
						iIAPName.Copy(iapn);
        }
        else
        {
        		iIsLastChangedByConfigurator = EFalse;
        		Cancel();//cancel listening if setting has changed by UI
        }
        CleanupStack::PopAndDestroy(3);
	}	
	
// ---------------------------------------------------------------------------
// Wrapper function used for tracing
// ---------------------------------------------------------------------------
//   
void COMASuplBackupListener::Trace( const TDesC& aMessage, const TDesC& aFileName, TInt line )
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
