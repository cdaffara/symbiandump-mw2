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
* Description:    SUPL Notifiers Base class definition
*
*/



// System Includes
#include <eikenv.h>
#include <e32std.h>
#include <aknmessagequerydialog.h>
#include <bautils.h>
#include <StringLoader.h>
#include <data_caging_path_literals.hrh>
#include <epos_omasuplnotifier.rsg>
#include <AknNotiferAppServerApplication.h>
#include <epos_omasuplquerycoverui.h>

// User Includes
#include "epos_comasuplnotifier.h"
#include "locsettingsuiclient.h"

// Constant Definitions
_LIT(KResourceFileName, "epos_omasuplnotifier.rsc");
_LIT(KNewLine, "\n");

//
// ------------------------- Member Function definition ----------------------

// ----------------------------------------------------------
// CSuplNotifier::CSuplNotifier()
// ----------------------------------------------------------
//
CSuplNotifier::CSuplNotifier()
    :CActive( EPriorityStandard )
    {
    }

// ----------------------------------------------------------
// CSuplNotifier::~CSuplNotifier()
// ----------------------------------------------------------
//
CSuplNotifier::~CSuplNotifier()
    {
    // If there is an existing message outstanding then
    // terminate the message
    CompleteMessage( KErrServerTerminated );
        
    Cancel();
    
    // Delete the Settings UI client
    delete iSettingsUi;
    
    // Delete the link
    delete iQueryText;
    
    // Free the resources
    iEikEnv->DeleteResourceFile(iResourceFileFlag);
        
    if ( iAppSwitchSupress && iAppUi )
        {
        iAppUi->SuppressAppSwitching(EFalse); 
        iAppSwitchSupress = EFalse;                        
        }            
    }

// ----------------------------------------------------------
// void CSuplNotifier::ConstructL()
// ----------------------------------------------------------
//    
void CSuplNotifier::ConstructL()
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
    
    // Load the Link text
    HBufC* linkText = StringLoader::LoadLC( R_LOC_SUPL_SETTINGS_LINK );
    
    // Obtain the query text
    HBufC* msgText = QueryTextL();
    CleanupStack::PushL( msgText );
    
    // Prepare the Query text
    iQueryText = HBufC::NewL( msgText->Des().Length() +
    					      KNewLine().Length() +
    						  KOpeningLinkTag().Length() +
    						  linkText->Des().Length() +
    						  KClosingLinkTag().Length());
	TPtr queryText( iQueryText->Des());
	queryText.Copy( msgText->Des());
	queryText.Append( KNewLine());
	queryText.Append( KOpeningLinkTag());
	queryText.Append( linkText->Des());
	queryText.Append( KClosingLinkTag());    						  
    						      
    CleanupStack::PopAndDestroy( 2, linkText );
    
    // Create the Settings UI Client handle
    iSettingsUi = CLocSettingsUiClient::NewL();
    
    // Add the object to the Active scheduler
    CActiveScheduler::Add( this );
    }

// ----------------------------------------------------------
// void CSuplNotifier::Release()
// ----------------------------------------------------------
//    
void CSuplNotifier::Release()  
    {
    // This method is called to free all the resources
    // associated with the Notifier. Here it would mean deleting
    // the notifier handle
    delete this;        
    }
    
// ----------------------------------------------------------
// MEikSrvNotifierBase2::TNotifierInfo CSuplNotifier::Info() const
// ----------------------------------------------------------
//    
MEikSrvNotifierBase2::TNotifierInfo CSuplNotifier::Info() const
    {
    // Return the notifier information
    return iInfo;
    }
    
// ----------------------------------------------------------
// TPtrC8 CSuplNotifier::StartL
// ----------------------------------------------------------
//    
TPtrC8 CSuplNotifier::StartL( const TDesC8&        /* aBuffer */ )
    {    
    TPtrC8 ret(KNullDesC8);
    return (ret);    
    }

// ----------------------------------------------------------
// void CSuplNotifier::StartL
// ----------------------------------------------------------
//    
void CSuplNotifier::StartL( const TDesC8&          /* aBuffer */,
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
// void CSuplNotifier::Cancel
// ----------------------------------------------------------
//    
void CSuplNotifier::Cancel()
    {
    // Delete the running SUPL query
    delete iSuplQuery;
    iSuplQuery = NULL;
    	         
    CActive::Cancel();
    }

// ----------------------------------------------------------
// TPtrC8 CSuplNotifier::UpdateL
// ----------------------------------------------------------
//    
TPtrC8 CSuplNotifier::UpdateL( const TDesC8&   /* aBuffer */ )
    {
    TPtrC8 ret(KNullDesC8);
    return (ret);    
    }

// ----------------------------------------------------------
// TPtrC8 CSuplNotifier::UpdateL
// ----------------------------------------------------------
//
void CSuplNotifier::UpdateL( const TDesC8&         /* aBuffer */, 
                                   TInt            /* aReplySlot */, 
                             const RMessagePtr2&   aMessage)
    {
    aMessage.Complete( KErrNotSupported );
    }

// ----------------------------------------------------------
// TInt CSuplNotifier::LinkClickedL()
// ----------------------------------------------------------
//
TInt CSuplNotifier::LinkClickedL( TAny* 	aMyClass )
	{
	CSuplNotifier* myClass = reinterpret_cast< CSuplNotifier* >( aMyClass );
	
	myClass->iSuplEvent = EDismissNotifier;
    TRequestStatus* status = &myClass->iStatus;
    myClass->SetActive();    
    User::RequestComplete( status, KErrNone ); 
   
	return KErrNone;
	}

// ----------------------------------------------------------
// void CSuplNotifier::CompleteMessage()
// ----------------------------------------------------------
// 
void CSuplNotifier::CompleteMessage( TInt aError )
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
// void CSuplNotifier::RunL()
// ----------------------------------------------------------
//                             
void CSuplNotifier::RunL()
    {
    // Do this only on KErrNone
    if ( !iStatus.Int())
    	{
    	switch( iSuplEvent )
    		{
    		case EDisplayNotifier:
    			{
				CAknMessageQueryDialog*	queryDialog = 
				    new ( ELeave ) CAknMessageQueryDialog( CAknQueryDialog::EConfirmationTone );
				queryDialog->PrepareLC( R_SUPL_CONFIRM_QUERY );
			    queryDialog->SetFocus( ETrue );
			    queryDialog->SetMessageTextL( *iQueryText );
			    
			    TCallBack callback( CSuplNotifier::LinkClickedL, this );
			    queryDialog->SetLink( callback );	
			    
			    // Supress the App switching
			    if ( !iAppSwitchSupress )
			        {
                    iAppUi->SuppressAppSwitching( ETrue );    	         
    	            iAppSwitchSupress = ETrue;			        
			        }
	            
	            // Display the dialog in cover UI
	            queryDialog->PublishDialogL( SuplQueryId(), KCatUidSuplQuery );
	                	         			    
			    // Store the value for the Query dialog. this would be useful
			    // for cancelling it.
			    iSuplQuery = queryDialog;
		    	TInt keypress = queryDialog->RunLD();
		    	iSuplQuery = NULL;		    	 
                        		    	
		    	// If the dialog had been dismissed because of the the
		    	// link press then there would be no completion
		    	if ( ESettingsLaunched != iSuplEvent )
		    	    {
    	    		if ( keypress )
    	    			{
    	    			CompleteMessage( KErrNone );
    	    			}
    	    		else
    	    			{
    	    			CompleteMessage( KErrAbort );
    	    			}		    	    
		    	    }
		    	break;
    			}
    		case EDismissNotifier:
    		    {
            	// Dismiss the Query dialog
            	delete iSuplQuery;
                iSuplQuery = NULL;            	
            	    
                // Launch the SUPL Settings UI as an embedded application
                iSettingsUi->
                            LaunchSettingsUiL( TUid::Uid( KLocSUPLSettingsUID ),
                                               ELocSuplSettingsPage,
                                               iStatus );
                
            	// Launch the Embedded Link
            	iSuplEvent = ESettingsLaunched;
                SetActive();
        		    
    		    break;
    		    }
    		case ESettingsLaunched:
    			{
    			CompleteMessage( KErrCompletion );
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
// void CSuplNotifier::DoCancel()
// ----------------------------------------------------------
// 
void CSuplNotifier::DoCancel()
    {
    // If there is an IPC message, complete it with KErrCancel
    CompleteMessage( KErrCancel );
    
    // Ignoring the value since there is nothing to do incase
    // there is an error.
    iSettingsUi->CancelLaunchedSettingsUi();
    
    // Reset the state machine
    iSuplEvent = ENone;    
    }

// ----------------------------------------------------------
// TInt CSuplNotifier::RunError()
// ----------------------------------------------------------
//    
TInt CSuplNotifier::RunError( TInt aError )
	{
	// If the dialog value is set then the dialog must be
	// set to NULL because the dialog would have been destroyed
	// already since its present on the cleanup stack
	iSuplQuery = NULL;
	
	// Complete the outstanding request with the error code
	CompleteMessage( aError );
	
    // Cancel the settings UI if its outstanding;
    // Ignoring the value since there is nothing to do incase
    // there is an error.    
    iSettingsUi->CancelLaunchedSettingsUi();
    
    // Reset the state machine.
    iSuplEvent = ENone;  
    	
	return CActive::RunError( aError );	
	}
