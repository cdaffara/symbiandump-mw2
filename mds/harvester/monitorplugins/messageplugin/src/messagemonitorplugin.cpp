/*
* Copyright (c) 2006-2009 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  
*
*/


// INCLUDE FILES
#include "messagemonitorplugin.h"
#include "harvesterlog.h"
#include "mdesession.h"
#include "clientkeywatcherkeys.h" 

#include <smut.h>
#include <mmsvattachmentmanager.h>	// MMsvAttachmentManager
#include <msventry.h>	// CMsvEntry
#include <msvfind.h>
#include <msvuids.h>
#include <mmsconst.h>
#include <mtclreg.h>
#include <smsclnt.h>
#include <driveinfo.h>
#include <pathinfo.h>

// CONSTANTS
#ifdef _DEBUG
_LIT( KMsgMonitorHarvesterMsvPanic, "Message monitor harvester: Message server terminated" );
#endif
_LIT( KMsgURI, "%d.MSG" );

const TInt KUriLength = 127;

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CMessageMonitorPlugin::NewL
// -----------------------------------------------------------------------------
//
CMessageMonitorPlugin* CMessageMonitorPlugin::NewL()
	{
	CMessageMonitorPlugin* self = new ( ELeave ) CMessageMonitorPlugin();
	CleanupStack::PushL( self );
	self->ConstructL();
	CleanupStack::Pop( self );
	return self;
	}

// -----------------------------------------------------------------------------
// CMessageMonitorPlugin::~CMessageMonitorPlugin
// -----------------------------------------------------------------------------
//
CMessageMonitorPlugin::~CMessageMonitorPlugin() 
	{
	WRITELOG("ENTER ~CMessageMonitorPlugin");
    delete iMsvSession;
    delete iMessageScannerAO;
	WRITELOG("END ~CMessageMonitorPlugin");	
	}


// -----------------------------------------------------------------------------
// CMessageMonitorPlugin::StartMonitoring
// -----------------------------------------------------------------------------
//
TBool CMessageMonitorPlugin::StartMonitoring( MMonitorPluginObserver& aObserver, 
											  CMdESession* aMdEClient, 
											  CContextEngine* /*aCtxEngine*/,
											  CHarvesterPluginFactory* /*aHarvesterPluginFactory*/ )
	{
	WRITELOG("ENTER CMessageMonitorPlugin::StartMonitoring");
	
	iMonitorPluginObserver = &aObserver;
	iMdeSession = aMdEClient;
	
	if ( iMsvSession )
		{
		delete iMsvSession;
		iMsvSession = NULL;
		}
	TInt error( KErrNone );
	// Open a new session to the message server
	TRAP( error, iMsvSession = CMsvSession::OpenSyncL( *this ) );
	WRITELOG1("Open msgSession error: %d" , error );
	
	TRAP_IGNORE( CheckRestoreL() );
		
	TBool result = 	error ? EFalse : ETrue ;
	WRITELOG("END CMessageMonitorPlugin::StartMonitoring");	
		
	return result;
	}

// -----------------------------------------------------------------------------
// CMessageMonitorPlugin::CreateFileNameL
// Filename flag can be changed to use cen rep!
// -----------------------------------------------------------------------------
//
void CMessageMonitorPlugin::CreateFileNameL() 
	{
	TInt drive ( 0 );
	User::LeaveIfError( 
			DriveInfo::GetDefaultDrive( DriveInfo::EDefaultSystem, drive ) );

    RFs fsSession;
    User::LeaveIfError( fsSession.Connect() );
  
    TChar driveLetter;
    fsSession.DriveToChar( drive, driveLetter );
	
    iFileName.Append( driveLetter );
    iFileName.Append( KRestoreFile );  // result-> C:\\private\\200009F5\\restoredone
 
    fsSession.Close();
	}

// -----------------------------------------------------------------------------
// CMessageMonitorPlugin::StopMonitoring
// -----------------------------------------------------------------------------
//
TBool CMessageMonitorPlugin::StopMonitoring()
	{
	WRITELOG("ENTER CMessageMonitorPlugin::StopMonitoring");
	if( iMsvSession )
		{
	    delete iMsvSession;  
		iMsvSession = NULL;
		}
	
	WRITELOG("END CMessageMonitorPlugin::StopMonitoring");	
	
	return ETrue;
	}

// -----------------------------------------------------------------------------
// CMessageMonitorPlugin::ResumeMonitoring
// -----------------------------------------------------------------------------
//
TBool CMessageMonitorPlugin::ResumeMonitoring( MMonitorPluginObserver& /*aObserver*/, 
											   CMdESession* /*aMdEClient*/, 
											   CContextEngine* /*aCtxEngine*/,
											   CHarvesterPluginFactory* /*aHarvesterPluginFactory*/ )
	{
	WRITELOG("ENTER CMessageMonitorPlugin::ResumeMonitoring");
	WRITELOG("END CMessageMonitorPlugin::ResumeMonitoring");
	
	return ETrue;
	}

// -----------------------------------------------------------------------------
// CMessageMonitorPlugin::PauseMonitoring
// -----------------------------------------------------------------------------
//
TBool CMessageMonitorPlugin::PauseMonitoring()
	{
	WRITELOG("ENTER CMessageMonitorPlugin::PauseMonitoring");	
	WRITELOG("END CMessageMonitorPlugin::PauseMonitoring");	
	
	return ETrue;
	}

void CMessageMonitorPlugin::CheckRestoreL()
	{
	RFs fs;
	User::LeaveIfError( fs.Connect() );
	CleanupClosePushL( fs );

	RFile64 file;
	CleanupClosePushL( file );
	// only needed information is does file exist
	TInt fileError = file.Open( fs, iFileName, EFileShareReadersOrWriters );
	WRITELOG1("CMessageMonitorPlugin::CheckRestoreL - fileError: %d", fileError);
	User::LeaveIfError( fileError );
	
	CleanupStack::PopAndDestroy( &file );
	CleanupStack::PopAndDestroy( &fs );

   	iMessageScannerAO = CMessageScannerAO::NewL( iMsvSession, iMdeSession, 
   			iMonitorPluginObserver );
   	iMessageScannerAO->StartL();
	}

// -----------------------------------------------------------------------------
// CMessageMonitorPlugin::CMessageMonitorPlugin
// -----------------------------------------------------------------------------
//
CMessageMonitorPlugin::CMessageMonitorPlugin() : CMonitorPlugin(), 
												 iMonitorPluginObserver( NULL ),
												 iMsvSession( NULL ),
												 iMdeSession( NULL ),
												 iMessageScannerAO( NULL )
	{
	WRITELOG("ENTER CMessageMonitorPlugin::CMessageMonitorPlugin");
	WRITELOG("END CMessageMonitorPlugin::CMessageMonitorPlugin");		
	}

// -----------------------------------------------------------------------------
// CMessageMonitorPlugin::ConstructL
// -----------------------------------------------------------------------------
//
void CMessageMonitorPlugin::ConstructL() 
	{
	WRITELOG("ENTER CMessageMonitorPlugin::ConstructL");
	
	CreateFileNameL();
	
	WRITELOG("END CMessageMonitorPlugin::ConstructL");
	}
	
	
// -----------------------------------------------------------------------------
// CMessageMonitorPlugin::HandleSessionEventL
// -----------------------------------------------------------------------------
//
void CMessageMonitorPlugin::HandleSessionEventL( TMsvSessionEvent aEvent, 
										  		 TAny* aArg1, 
										  		 TAny* aArg2, 
										  		 TAny* aArg3 )
	{		
	WRITELOG("ENTER CMessageMonitorPlugin::HandleSessionEventL");
	WRITELOG1("TMsvSessionEvent aEvent: %d", aEvent );			
		
	switch( aEvent )
		{
		case EMsvEntriesCreated:
		case EMsvEntriesChanged:
			{
            CMsvEntrySelection* selection =  
                    		reinterpret_cast<CMsvEntrySelection*> ( aArg1 );
            TMsvId folderId = *( reinterpret_cast<TMsvId*> ( aArg2 ) );                    
			HandleMsgCreatedChangedL( *selection, folderId, aEvent );		
			break;
			}		
		case EMsvEntriesDeleted:
			{
		    CMsvEntrySelection* selection = 
                reinterpret_cast<CMsvEntrySelection*> ( aArg1 );
            HandleMsgDeletedL( *selection );						
			break;
			}
		case EMsvEntriesMoved:
			{
			// track move operations
			// or sent folder
			// ... extract the useful parameters
            if( aArg1 && aArg2 && aArg3 )
                {
                CMsvEntrySelection* selection = 
                    reinterpret_cast<CMsvEntrySelection*> ( aArg1 );
                TMsvId folderId1 = *( reinterpret_cast<TMsvId*> ( aArg2 ) );
                TMsvId folderId2 = *( reinterpret_cast<TMsvId*> ( aArg3 ) );
		        HandleMsgMovedL( folderId1, folderId2, *selection );
                }
			break;
			}			
		case EMsvServerFailedToStart:			
		case EMsvCloseSession:
		case EMsvServerTerminated:
			{
			__ASSERT_DEBUG( EFalse, User::Panic( KMsgMonitorHarvesterMsvPanic, KErrServerTerminated ));
			break;				
			}			
		case EMsvMtmGroupInstalled:
		case EMsvMtmGroupDeInstalled:
		case EMsvGeneralError: // not used after v5
		case EMsvServerReady:
		case EMsvCorruptedIndexRebuilt:
		case EMsvMediaChanged:
		case EMsvMediaUnavailable:
		case EMsvMediaAvailable:
		case EMsvMediaIncorrect:
		case EMsvCorruptedIndexRebuilding:
		default:
			{
			break;
			}							
		}	
	WRITELOG("END CMessageMonitorPlugin::HandleSessionEventL");	
		
	}

// -----------------------------------------------------------------------------
// CMessageMonitorPlugin::HandleMsgMovedL
// -----------------------------------------------------------------------------
//
void CMessageMonitorPlugin::HandleMsgMovedL( const TMsvId aFolderId1, 
											 const TMsvId aFolderId2, 
								   			 const CMsvEntrySelection& aSelection )
	{
	
	
	
	WRITELOG("ENTER CMessageMonitorPlugin::HandleMsgMovedL");
	
	TInt count( aSelection.Count() );
	// cycle through every message in the CMsvEntrySelection
	for ( TInt i=0; i < count; ++i )
		{
		TMsvId msgId = aSelection[i];
		WRITELOG1("msgId: %d", msgId );					
		
		TMsvId service = 0;
        TMsvEntry entry;
		TInt err = iMsvSession->GetEntry( msgId, service, entry );

        // if we have a message and it has been moved from the outbox to the sent 
        // items, and it is a valid message
        if ( ( !err ) &&
            ( KUidMsvMessageEntry == entry.iType ) &&
            ( KMsvSentEntryId == aFolderId1 ) &&
            ( KMsvGlobalOutBoxIndexEntryId == aFolderId2 ) && 
            ( IsValidMessageTypeL( entry, *iMsvSession ) ) )
            {            
            HBufC* uri = CreateUriL( entry.iMtm, msgId );
            CleanupStack::PushL( uri );
            CHarvesterData* hd = CHarvesterData::NewL( uri );
            CleanupStack::Pop( uri );
            hd->SetEventType( EHarvesterAdd );
            hd->SetTakeSnapshot( EFalse );
            hd->SetBinary( EFalse );
            iMonitorPluginObserver->MonitorEvent( hd );
            }
            
               
#ifdef __WINSCW__
		else if (!err && KMsvDraftEntryId == aFolderId2 && KMsvGlobalOutBoxIndexEntryId == aFolderId1 )
			{
		    if( entry.iMtm.iUid != KUidMsgTypeMultimedia.iUid )
		        {
                CClientMtmRegistry* clientMtmReg;
                clientMtmReg = CClientMtmRegistry::NewL(*iMsvSession);
                CleanupStack::PushL(clientMtmReg);
            
                CSmsClientMtm* smsMtm = static_cast<CSmsClientMtm*>(clientMtmReg->NewMtmL(KUidMsgTypeSMS)); 
                CleanupStack::PushL(smsMtm);
                smsMtm->SwitchCurrentEntryL( msgId );
            
                TMsvSelectionOrdering selection;
                selection.SetShowInvisibleEntries(ETrue);
            
                CMsvEntry* parentEntry = CMsvEntry::NewL( smsMtm->Session(),
                    smsMtm->Entry().Entry().Parent(), selection );
             
                CleanupStack::PushL(parentEntry);
                // Move the message
                TRAP_IGNORE( parentEntry->MoveL( msgId, KMsvSentEntryId ) );
                CleanupStack::PopAndDestroy(3,clientMtmReg); // parentEntry
		        }
			}
#endif             
		
        }
	WRITELOG("END CMessageMonitorPlugin::HandleMsgMovedL");	        
	}


// -----------------------------------------------------------------------------
// CMessageMonitorPlugin::HandleMsgCreatedChangedL
// -----------------------------------------------------------------------------
//
void CMessageMonitorPlugin::HandleMsgCreatedChangedL( const CMsvEntrySelection& aSelection,
                                          	   		  const TMsvId aFolderId,
                                          	          const TMsvSessionEvent aEvent )
	{
	WRITELOG("ENTER CMessageMonitorPlugin::HandleMsgCreatedChangedL");
	
	TInt count( aSelection.Count() );
	
    // cycle through every message in the CMsvEntrySelection
	for ( TInt i=0; i < count; ++i )
		{
		// extract the message server entry from the parameters
		TMsvId msgId = aSelection[i];
		WRITELOG1("msgId: %d", msgId );					
		
        TMsvId service = 0;
        TMsvEntry entry;
		TInt err = iMsvSession->GetEntry( msgId, service, entry );

		WRITELOG1("HandleMsgCreatedChangedL error: %d ", err);		
		
		THarvesterEventType event = 
						( aEvent == EMsvEntriesCreated ) || entry.Unread() ? 
						  	  								 EHarvesterAdd : 
															 EHarvesterEdit;
		
        
        // ... we only want to process message entries, that are in the inbox
        // ... and are valid for adding to Mds.
        if ( ( !err ) && 
            ( KUidMsvMessageEntry == entry.iType ) && 
            ( KMsvGlobalInBoxIndexEntryId == aFolderId ) &&
            ( entry.Visible() && entry.Complete() && !entry.InPreparation() ) &&
            ( IsValidMessageTypeL( entry, *iMsvSession ) ) )
            {
            HBufC* uri = CreateUriL( entry.iMtm, msgId );    
            CleanupStack::PushL( uri );
            CHarvesterData* hd = CHarvesterData::NewL( uri );
            CleanupStack::Pop( uri );
            hd->SetEventType( event );
            hd->SetBinary( EFalse );
            hd->SetTakeSnapshot( EFalse );
			iMonitorPluginObserver->MonitorEvent( hd );
            }
        }
	WRITELOG("END CMessageMonitorPlugin::HandleMsgCreatedChangedL");	        
	}
	
// -----------------------------------------------------------------------------
// CMessageMonitorPlugin::HandleMsgDeletedL
// -----------------------------------------------------------------------------
//
void CMessageMonitorPlugin::HandleMsgDeletedL( const CMsvEntrySelection& aSelection )
	{
	WRITELOG("ENTER CMessageMonitorPlugin::HandleMsgDeletedL");
	
	TInt count( aSelection.Count() );	
	// cycle through every message in the CMsvEntrySelection
	for( TInt i = 0; i < count; ++i )
		{
		TMsvId msgId = aSelection[i];	
		WRITELOG1("msgId: %d", msgId );
		HBufC* uri = CreateUriL( KUidMsgTypeSMS, msgId );
		CleanupStack::PushL( uri );
		RemoveObjectL( *uri );
		CleanupStack::PopAndDestroy( uri );
		}
	WRITELOG("END CMessageMonitorPlugin::HandleMsgDeletedL");	
	}	

// ---------------------------------------------------------------------------
// CHarvesterMessagePluginAO::RemoveObjectL
// ---------------------------------------------------------------------------
//
void CMessageMonitorPlugin::RemoveObjectL( const TDesC& aUri )
	{
    WRITELOG( "CMessageMonitorPlugin::RemoveObjectL enter" );
	CMdEObject* mdeObject = iMdeSession->GetObjectL( aUri );

    // check if in mde
    if ( mdeObject )
    	{
    	TItemId objectId = mdeObject->Id();
	    delete mdeObject;
	    mdeObject = NULL;
	    
		TTime time( 0 );
		CMdENamespaceDef& nsDef = iMdeSession->GetDefaultNamespaceDefL();
		CMdEEventDef& eventDef = nsDef.GetEventDefL( MdeConstants::Events::KDeleted );
	
		iMdeSession->RemoveObjectL( aUri, &nsDef );
		WRITELOG1( "aUri: %S", &aUri );							
		time.UniversalTime();
		CMdEEvent* event =  iMdeSession->NewEventL( eventDef, objectId, time,NULL,NULL );
		
		iMdeSession->AddEventL( *event );
		delete event;
		event = NULL;
    	}
    
    WRITELOG( "CMessageMonitorPlugin::RemoveObjectL return" );
	}

// -----------------------------------------------------------------------------
// CMessageMonitorPlugin::IsValidMessageTypeL
// -----------------------------------------------------------------------------
//	
TBool CMessageMonitorPlugin::IsValidMessageTypeL( const TMsvEntry& aEntry,
                                                  CMsvSession& /*aSession*/)
	{
	WRITELOG("ENTER CMessageMonitorPlugin::IsValidMessageTypeL");
	
    TBool isValidMsg( EFalse );

    // Mms message
	if ( aEntry.iMtm.iUid == KUidMsgTypeMultimedia.iUid )
        {	
        isValidMsg = ETrue;
        }
    // check if this is a BIO message, if so exclude it from Hoover
    else if ( aEntry.iBioType != 0 )
        {
        isValidMsg = EFalse;
        }
    // else if we have a normal text based SMS it is valid    
	else if ( aEntry.iMtm.iUid == KUidMsgTypeSMS.iUid )
		{
		isValidMsg = ETrue;
		}
	WRITELOG1( "isValidMsg: %d" , isValidMsg );					
		
	WRITELOG("END CMessageMonitorPlugin::IsValidMessageTypeL");		
        
	return isValidMsg;
	}	
	
// -----------------------------------------------------------------------------
// CMessageMonitorPlugin::CreateUriL
// -----------------------------------------------------------------------------
//	
HBufC* CMessageMonitorPlugin::CreateUriL( const TUid& /*aMsgTypeUid*/, const TMsvId aMsgId )
	{
	WRITELOG("ENTER CMessageMonitorPlugin::CreateUriL");
	
    HBufC* uri = HBufC::NewL( KUriLength );
	TPtr ptrUri = uri->Des();
	ptrUri.Format( KMsgURI, aMsgId );	
	
	WRITELOG("END CMessageMonitorPlugin::CreateUriL");	
		
	return uri;
	}

// End Of File
