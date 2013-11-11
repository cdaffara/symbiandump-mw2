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
* Description:   SUPL Timeout Notifier class definition
*
*/



// System Includes
#include <eikenv.h>
#include <e32std.h>
#include <AknQueryDialog.h>
#include <bautils.h>
#include <StringLoader.h>
#include <data_caging_path_literals.hrh>
#include <epos_omasuplnotifier.rsg>
#include <AknNotiferAppServerApplication.h>
#include <epos_omasuplquerycoverui.h>

// User Includes
#include "epos_omasuplnotifierinterface.h"
#include "epos_comasupltimeoutnotifier.h"

// Constant Definitions
_LIT(KResourceFileName, "epos_omasuplnotifier.rsc");

//
// ------------------------- Member Function definition ----------------------

// ----------------------------------------------------------
// CSuplTimeOutNotifier::CSuplTimeOutNotifier()
// ----------------------------------------------------------
//
CSuplTimeOutNotifier::CSuplTimeOutNotifier()
    :CActive( EPriorityStandard )
    {
    }

// ----------------------------------------------------------
// CSuplTimeOutNotifier::~CSuplTimeOutNotifier()
// ----------------------------------------------------------
//
CSuplTimeOutNotifier::~CSuplTimeOutNotifier()
    {
    // If there is an existing message outstanding then
    // terminate the message
    CompleteMessage( KErrServerTerminated );
        
    Cancel();
            
    // Free the resources
    iEikEnv->DeleteResourceFile(iResourceFileFlag);

    // Delete the link
    delete iQueryText;

    if ( iAppSwitchSupress && iAppUi )
        {
        iAppUi->SuppressAppSwitching(EFalse); 
        iAppSwitchSupress = EFalse;                        
        }            
    }

// ----------------------------------------------------------
// void CSuplTimeOutNotifier::ConstructL()
// ----------------------------------------------------------
//    
void CSuplTimeOutNotifier::ConstructL()
    {
    // Store a Local pointer for Eikon Env
    iEikEnv = CEikonEnv::Static();
    iAppUi = static_cast< CAknNotifierAppServerAppUi* >( iEikEnv->EikAppUi());
    
    // Open the localized resource file
    TFileName dllDrive;;
    Dll::FileName( dllDrive );

    // File name without the drive letter.
    TFileName filename;
    filename += KDC_RESOURCE_FILES_DIR; 
    filename += KResourceFileName;        
        
    // Append the drive letter to the file name.    
    TParse parse;
	User::LeaveIfError( parse.Set( dllDrive, NULL, NULL ) );
    User::LeaveIfError( parse.Set( parse.Drive(), &filename, NULL ) );
	filename = parse.FullName();	                        
    
    BaflUtils::NearestLanguageFile(iEikEnv->FsSession(),filename);
    iResourceFileFlag=iEikEnv->AddResourceFileL(filename);     

    // Load the SUPL Tiemout String
    iQueryText = StringLoader::LoadL( R_LOC_SUPL_TIMEOUT_NO_CONNECTION );

    // Add the object to the Active scheduler
    CActiveScheduler::Add( this );
    }

// ----------------------------------------------------------
// CSuplTimeOutNotifier* CSuplTimeOutNotifier::NewL()
// ----------------------------------------------------------
//
CSuplTimeOutNotifier* CSuplTimeOutNotifier::NewL()
    {
    CSuplTimeOutNotifier* self = new ( ELeave ) CSuplTimeOutNotifier;
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop( self );
    return self;
    }

// ----------------------------------------------------------
// void CSuplTimeOutNotifier::Release()
// ----------------------------------------------------------
//    
void CSuplTimeOutNotifier::Release()  
    {
    // This method is called to free all the resources
    // associated with the Notifier. Here it would mean deleting
    // the notifier handle
    delete this;        
    }
    
// ----------------------------------------------------------
// MEikSrvNotifierBase2::TNotifierInfo CSuplTimeOutNotifier::Info() const
// ----------------------------------------------------------
//    
MEikSrvNotifierBase2::TNotifierInfo CSuplTimeOutNotifier::Info() const
    {
    // Return the notifier information
    return iInfo;
    }
    
// ----------------------------------------------------------
// TPtrC8 CSuplTimeOutNotifier::StartL
// ----------------------------------------------------------
//    
TPtrC8 CSuplTimeOutNotifier::StartL( const TDesC8&        /* aBuffer */ )
    {    
    TPtrC8 ret(KNullDesC8);
    return (ret);    
    }

// ----------------------------------------------------------
// void CSuplTimeOutNotifier::StartL
// ----------------------------------------------------------
//    
void CSuplTimeOutNotifier::StartL( const TDesC8&          /* aBuffer */,
                                  TInt             /* aReplySlot */,
                            const RMessagePtr2&    aMessage )          
    {
    if ( IsActive())
        {
        User::Leave ( KErrInUse );
        }
    
    iSuplEvent = EDisplayNotifier;    
    iMessage = aMessage;
    TRequestStatus* status = &iStatus;
    SetActive();    
    User::RequestComplete( status, KErrNone );    
    }

// ----------------------------------------------------------
// void CSuplTimeOutNotifier::Cancel
// ----------------------------------------------------------
//    
void CSuplTimeOutNotifier::Cancel()
    {
    // Delete the running SUPL query
    delete iSuplQuery;
    iSuplQuery = NULL;
    	         
    CActive::Cancel();
    }

// ----------------------------------------------------------
// TPtrC8 CSuplTimeOutNotifier::UpdateL
// ----------------------------------------------------------
//    
TPtrC8 CSuplTimeOutNotifier::UpdateL( const TDesC8&   /* aBuffer */ )
    {
    TPtrC8 ret(KNullDesC8);
    return (ret);    
    }

// ----------------------------------------------------------
// TPtrC8 CSuplTimeOutNotifier::UpdateL
// ----------------------------------------------------------
//
void CSuplTimeOutNotifier::UpdateL( const TDesC8&         /* aBuffer */, 
                                   TInt            /* aReplySlot */, 
                             const RMessagePtr2&   aMessage)
    {
    aMessage.Complete( KErrNotSupported );
    }

// ----------------------------------------------------------
// void CSuplTimeOutNotifier::CompleteMessage()
// ----------------------------------------------------------
// 
void CSuplTimeOutNotifier::CompleteMessage( TInt aError )
	{
	if ( !iMessage.IsNull() )
		{
		iMessage.Complete( aError );				
		}
    
    if ( iAppSwitchSupress && iAppUi )
        {
        iAppUi->SuppressAppSwitching(EFalse); 
        iAppSwitchSupress = EFalse;                        
        }
    iSuplEvent = ENone;		
	}
	
// ----------------------------------------------------------
// void CSuplTimeOutNotifier::RunL()
// ----------------------------------------------------------
//                             
void CSuplTimeOutNotifier::RunL()
    {
    // Do this only on KErrNone
    if ( !iStatus.Int())
    	{
    	switch( iSuplEvent )
    		{
    		case EDisplayNotifier:
    			{
                // Show Query Dialog "Positioning server SUPL request timed out. Network
                // connection not started."       
                CAknQueryDialog* queryDialog = CAknQueryDialog::NewL();
                
			    // Supress the App switching
			    if ( !iAppSwitchSupress )
			        {
                    iAppUi->SuppressAppSwitching( ETrue );    	         
    	            iAppSwitchSupress = ETrue;			        
			        }
	            
	            // Display the dialog in cover UI
	            queryDialog->PublishDialogL( ECmdSuplTimeOut, KCatUidSuplQuery );
	            	    
			    // Store the value for the Query dialog. this would be useful
			    // for cancelling it.
			    iSuplQuery = queryDialog;
		    	TInt keypress = queryDialog->ExecuteLD( R_SUPL_TIMEOUT_QUERY, *iQueryText );
		    	iSuplQuery = NULL;	
		    	
	    		if ( EEikBidOk == keypress )
	    			{
	    			CompleteMessage( KErrNone );
	    			}
	    		else
	    			{
	    			CompleteMessage( KErrAbort );
	    			}   

		    	break;
    			}
    		default:
    			{
    			User::Leave( KErrNotSupported );
    			break;
    			}    		
    		}
    	}
	else
		{
		CompleteMessage( iStatus.Int());
		}		
    }

// ----------------------------------------------------------
// void CSuplTimeOutNotifier::DoCancel()
// ----------------------------------------------------------
// 
void CSuplTimeOutNotifier::DoCancel()
    {
    // If there is an IPC message, complete it with KErrCancel
    CompleteMessage( KErrCancel );
        
    // Reset the state machine
    iSuplEvent = ENone;    
    }

// ----------------------------------------------------------
// TInt CSuplTimeOutNotifier::RunError()
// ----------------------------------------------------------
//    
TInt CSuplTimeOutNotifier::RunError( TInt aError )
	{
	// If the dialog value is set then the dialog must be
	// set to NULL because the dialog would have been destroyed
	// already since its present on the cleanup stack
	iSuplQuery = NULL;
	
	// Complete the outstanding request with the error code
	CompleteMessage( aError );
	
    // Reset the state machine.
    iSuplEvent = ENone;  
    	
	return CActive::RunError( aError );	
	}

// ----------------------------------------------------------
// CSuplTimeOutNotifier::TNotifierInfo CSuplTimeOutNotifier::RegisterL()
// ----------------------------------------------------------
//
CSuplTimeOutNotifier::TNotifierInfo CSuplTimeOutNotifier::RegisterL()
    {
    iInfo.iUid          = KSuplTimeoutNotifier;
    iInfo.iChannel      = KSuplTimeoutNotifierChannel;
    iInfo.iPriority     = ENotifierPriorityVHigh;
    return iInfo;    
    }
