/*
* Copyright (c) 2008-2009 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  This implements CRestoreWatcher class.
*
*/

// SYSTEM INCLUDE
#include <centralrepository.h>
#include <sbdefs.h>
#include <driveinfo.h>
#include <pathinfo.h>

// USER INCLUDE
#include "restorewatcher.h"
#include "harvesterlog.h"
#include "propertywatcher.h"
#include "clientkeywatcherkeys.h"

// STATIC MEMBERS
TInt CRestoreWatcher::iRegisteredClients = 0;
TBool CRestoreWatcher::iRestoreDone = EFalse;


// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CRestoreWatcher::NewL
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
CRestoreWatcher* CRestoreWatcher::NewL()
	{
	WRITELOG("CRestoreWatcher* CRestoreWatcher::NewL()");
	CRestoreWatcher* self = new (ELeave) CRestoreWatcher();
	CleanupStack::PushL( self );
	self->ConstructL();
	CleanupStack::Pop( self );
	return self;
	}

// -----------------------------------------------------------------------------
// CRestoreWatcher::CRestoreWatcher
// Standard C++ constructor.
// -----------------------------------------------------------------------------
//
CRestoreWatcher::CRestoreWatcher() : iPropertyWatcher( NULL ), iClients( 0 )
	{
	// No implementation required.
	}

// -----------------------------------------------------------------------------
// CRestoreWatcher::~CRestoreWatcher
// Standard C++ destructor.
// -----------------------------------------------------------------------------
//
CRestoreWatcher::~CRestoreWatcher()
	{
	if( iPropertyWatcher )
		{
		iPropertyWatcher->StopListeningKeyChanges( 
				KUidSystemCategory,
				conn::KUidBackupRestoreKey, this );
		
	
		iPropertyWatcher->StopListeningKeyChanges( 
				KPSRestoreWatcherCategory,
				KPSRestoreWatcherClientsKey, this );
		
		iPropertyWatcher->Delete(); // Release connection to TLS object.
		}
	
	iFs.Close();
	}

// -----------------------------------------------------------------------------
// CRestoreWatcher::ConstructL
// Symbian 2nd phase constructor.
// -----------------------------------------------------------------------------
//
void CRestoreWatcher::ConstructL()
	{
	WRITELOG("CRestoreWatcher::ConstructL()");

    User::LeaveIfError( iFs.Connect() );
	
	CreateFileNameL();
	CheckRestoreL();
    WRITELOG1("CRestoreWatcher::ConstructL() - iRestoreDone: %d", iRestoreDone );
	
	WRITELOG("CRestoreWatcher::ConstructL() - creating restore watcher PS key");
	RProperty clientsProperty;
    const TInt error = clientsProperty.Define(
    		KPSRestoreWatcherCategory, 
			KPSRestoreWatcherClientsKey,
			RProperty::EInt );
	
    clientsProperty.Close();
	
	if( error != KErrAlreadyExists ) // KErrAlreadyExists not error.
		{
		User::LeaveIfError( error );
		}
	
	iPropertyWatcher = CPropertyWatcher::GetInstanceL();
	
	// Want to listen when backup/restore starts.
	// Calls NotifyKeyL when key's state has changed. 
	iPropertyWatcher->ListenKeyChangesL( 
			KUidSystemCategory,
			conn::KUidBackupRestoreKey, this );
	
	// Listen message clients.
	iPropertyWatcher->ListenKeyChangesL( 
			KPSRestoreWatcherCategory,
			KPSRestoreWatcherClientsKey, this );
	
	
	WRITELOG("CRestoreWatcher::ConstructL() ends");
	}
	
// -----------------------------------------------------------------------------
// CRestoreWatcher::CreateFileNameL
// Filename flag can be changed to use cen rep!
// -----------------------------------------------------------------------------
//
void CRestoreWatcher::CreateFileNameL() 
	{
	TInt drive ( 0 );
	User::LeaveIfError( 
			DriveInfo::GetDefaultDrive( DriveInfo::EDefaultSystem, drive ) );
  
    TChar driveLetter;
    iFs.DriveToChar( drive, driveLetter );
	
    iFileName.Append( driveLetter );
    iFileName.Append( KRestoreFile );  // result-> C:\\private\\200009F5\\restoredone
	}


// -----------------------------------------------------------------------------
// CRestoreWatcher::NotifyKeyL
// CPropertyWatcher's callback.
// -----------------------------------------------------------------------------
//
void CRestoreWatcher::NotifyKeyL( 
		const TInt aKeyValue,
		const TUid aPropertyCategory,
		const TUint aKey )
	{
	WRITELOG("CRestoreWatcher::NotifyKeyL() - begin");
	
	if( aPropertyCategory == KUidSystemCategory &&
			aKey == conn::KUidBackupRestoreKey )
		{
		HandleBackupRestoreKeyActionL( aKeyValue );
		}
	
	if( aPropertyCategory == KPSRestoreWatcherCategory &&
			aKey == KPSRestoreWatcherClientsKey )
		{
		HandleClientsKeyActionL( aKeyValue );
		}
	
	WRITELOG("CRestoreWatcher::NotifyKeyL() - end");
	}

// -----------------------------------------------------------------------------
// CRestoreWatcher::HandleBackupKeyActionL
// When user starts restore sets flag on.
// -----------------------------------------------------------------------------
//
void CRestoreWatcher::HandleBackupRestoreKeyActionL( const TUint aKeyValue )
	{	
	WRITELOG("CRestoreWatcher::HandleBackupKeyActionL() - begin");
	
	TInt backupStateValue = aKeyValue;
	backupStateValue &= conn::KBURPartTypeMask;

	if ( backupStateValue == conn::EBURRestoreFull ||
	     backupStateValue == conn::EBURRestorePartial )
		{
		WRITELOG("CRestoreWatcher::RunL() - setting restore to TRUE");
		SetRestoreFlagL( ETrue );
		}
	
	WRITELOG("CRestoreWatcher::HandleBackupKeyActionL() - end");
	}

// -----------------------------------------------------------------------------
// CRestoreWatcher::HandleClientsKeyActionL
// Handle message clients registering to watcher.
// -----------------------------------------------------------------------------
//
void CRestoreWatcher::HandleClientsKeyActionL( const TUint aKeyValue )
	{	
	WRITELOG("CRestoreWatcher::HandleBackupKeyActionL() - begin");

	// aKeyValue contains clients count.
	if ( aKeyValue < iClients )
		{
		WRITELOG("CRestoreWatcher::HandleClientsKeyActionL - unregistering");
		UnregisterL();
		}
	else if ( aKeyValue > iClients )
		{
		WRITELOG("CRestoreWatcher::HandleClientsKeyActionL - registering");
		Register();
		}
	
	iClients = aKeyValue;

	WRITELOG("CRestoreWatcher::HandleBackupKeyActionL() - end");	
	}

// -----------------------------------------------------------------------------
// CRestoreWatcher::Register
// Registers client to watcher.
// -----------------------------------------------------------------------------
//
TBool CRestoreWatcher::Register()
	{	
	WRITELOG("CRestoreWatcher::Register()");
	if ( iRestoreDone )
		{
		WRITELOG("CRestoreWatcher::Register() - partial restore was done.");
		iRegisteredClients++;
		WRITELOG1("CRestoreWatcher::Register() - registered clients: %d", iRegisteredClients);
		}
	
	return iRestoreDone;
	}

// -----------------------------------------------------------------------------
// CRestoreWatcher::UnregisterL
// Unregister client to watcher.
// -----------------------------------------------------------------------------
//
void CRestoreWatcher::UnregisterL()
	{
	WRITELOG("CRestoreWatcher::UnregisterL()");
	if ( iRegisteredClients <= 0 )
		{
		WRITELOG("CRestoreWatcher::UnregisterL() - 0 clients");
		return;
		}
	
	iRegisteredClients--;
	WRITELOG1("CRestoreWatcher::UnregisterL() - registered clients: %d", iRegisteredClients);
	
	if ( iRegisteredClients == 0 )
		{
		WRITELOG("CRestoreWatcher::UnregisterL() - setting CentRep value to 0");
		SetRestoreFlagL( EFalse );
		}
	}

// -----------------------------------------------------------------------------
// CRestoreWatcher::SetRestoreFlagL
// Sets restore flag on.
// -----------------------------------------------------------------------------
//
void CRestoreWatcher::SetRestoreFlagL( TBool aRestoreDone )
	{	
	iRestoreDone = aRestoreDone;
	if( aRestoreDone )
		{	
		RFile64 file;
		file.Replace( iFs, iFileName, EFileWrite );
		file.Close();
		}
	else
		{
		iFs.Delete( iFileName );
		}
	}

// -----------------------------------------------------------------------------
// CRestoreWatcher::CheckRestoreL
// Check if restore has done when booting up harvester server.
// -----------------------------------------------------------------------------
//
void CRestoreWatcher::CheckRestoreL()
	{
	RFile64 file;
	TInt fileError( KErrNotFound );
	fileError = file.Open( iFs, iFileName, EFileRead );
	file.Close();
    
	WRITELOG1("CRestoreWatcher::StartMonitoring - fileError: %d", fileError);
	
    if ( fileError == KErrNone )
    	{
    	iRestoreDone = ETrue;
    	}
    else
    	{
    	iRestoreDone = EFalse;
    	}
	}

// End of file.
