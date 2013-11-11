// Copyright (c) 1999-2009 Nokia Corporation and/or its subsidiary(-ies).
// All rights reserved.
// This component and the accompanying materials are made available
// under the terms of "Eclipse Public License v1.0"
// which accompanies this distribution, and is available
// at the URL "http://www.eclipse.org/legal/epl-v10.html".
//
// Initial Contributors:
// Nokia Corporation - initial contribution.
//
// Contributors:
//
// Description:
//

#ifdef _DEBUG
#undef _NO_SERVER_LOGGING_
#endif



#include <s32file.h>
#include <bautils.h>
#include <barsc.h>

#include <msgs.rsg>

#include "indexcontext.h"
#include "MSVSERV.H"
#include "MSVUTILS.H"
#include "MSVRBLD.H"

// will this header be expoerted to epoc\include
#include <sqldb.h>
#include "msvindexadapter.h"

#if (defined SYMBIAN_MESSAGESTORE_UNIT_TESTCODE)
	_LIT(KDBFileName,"\\messaging.db");
#else
	_LIT(KDBFileName,"[1000484B]messaging.db");
#endif

_LIT(KServerResourceFile,"\\resource\\messaging\\msgs.rsc");

#if (defined SYMBIAN_MESSAGESTORE_UNIT_TESTCODE)
	_LIT(KStoreDeletedFile,"\\private\\1000484b\\storedeleted.tmp");
#else
	_LIT(KStoreDeletedFile,"storedeleted.tmp");
#endif

const TInt KMsvInitDelayTime = 0x00000001; // as soon as possible
const TUid KUidMsvIndexFile = {0x10003C6B};
extern const TInt KMsvIndexStoreVersionNumber = 1;   // See MsvIndex.cpp
//**********************************
// CMsvIndexContext
//**********************************

CMsvIndexContext* CMsvIndexContext::NewL(CMsvServer& aServer, MMsvContextObserver& aObserver)
	{
	CMsvIndexContext* self = CMsvIndexContext::NewLC(aServer, aObserver);
	CleanupStack::Pop();
	return self;
	}

CMsvIndexContext* CMsvIndexContext::NewLC(CMsvServer& aServer, MMsvContextObserver& aObserver)
	{
	CMsvIndexContext* self = new(ELeave)CMsvIndexContext(aServer, aObserver);
	CleanupStack::PushL(self);
	self->ConstructL();
	return self;
	}

void CMsvIndexContext::ConstructL()
	{
	iUpdateEntryStreams = new(ELeave)CMsvEntrySelection;
	iRemoveEntries = new(ELeave)CMsvEntrySelection;

	TFileName fileName(KServerResourceFile);
	MsvUtils::AddPathAndExtensionToResFileL(fileName);
 	BaflUtils::NearestLanguageFile(iServer.FileSession(), fileName);

	// Load the initial index structure from resource file
	RResourceFile resource;
	resource.OpenL(iServer.FileSession(), fileName);
	CleanupClosePushL(resource);

	// Get ready to read resource

	iBuf = resource.AllocReadL(R_SERVER_INDEX_STARTUP);

	CleanupStack::PopAndDestroy(); //resource

	}

CMsvIndexContext::~CMsvIndexContext()
	{
	Cancel();
	delete iBuf;
	delete iIndexAdapter;
	delete iUpdateEntryStreams;
	delete iRemoveEntries;
	}

CMsvIndexContext::CMsvIndexContext(CMsvServer& aServer, MMsvContextObserver& aObserver)
: CActive(EPriorityStandard), iServer(aServer), iObserver(aObserver)
	{
	CActiveScheduler::Add(this);
	}



// CODE USED AFTER PREQ 557.
#if (defined SYMBIAN_MSGS_ENHANCED_REMOVABLE_MEDIA_SUPPORT)

/**
 * CreateIndex()
 * 
 * Code added for PREQ 557.
 */
void CMsvIndexContext::CreateIndexL()
	{
	__ASSERT_DEBUG(iProgress.iState == TMsvIndexLoadProgress::EIndexNotLoaded, PanicServer(EMsvBadIndexState));
	
	DoCreateIndexL();
	IndexLoadingCompleted();	
	}
	
	

void CMsvIndexContext::IndexLoadingCompleted()
	{
	iProgress.iError = KErrNone;
	
	IndexLoaded(EMsvNullNotification);

	// Tell the observer that we've finshed
	iObserver.ContextComplete(KErrNone, iRunMailInit);
	}




/**
 * DoCreateIndexL()
 *
 * Create the index adapter object.
 * Code added in PREQ 557.
 */
void CMsvIndexContext::DoCreateIndexL()
	{	
	// Remember stuff
	iObserverStatus = NULL;
	iRunMailInit = EFalse;

	// Update drive status in preferred drive list.
	TDriveState driveStatus = EMsvInvalidDriveStatus;
	for(TUint index=0; index<CMsvPreferredDriveList::GetDriveList()->Count(); index++)
		{
		UpdateDriveStatusL(index, driveStatus);
		}

	// Look for the current drive.
	// The first drive in the preferred drive list, whose 
	// status is either EMsvMessageStoreAvailable or 
	// EMsvMessageStoreUnavailable will be the current drive.
	TMsvPreferredDrive driveEntry;
	CMsvPreferredDriveList *driveList = CMsvPreferredDriveList::GetDriveList();
	for(TInt currentDriveIndex=0; currentDriveIndex<driveList->Count(); ++currentDriveIndex)
		{
		driveEntry = (*driveList)[currentDriveIndex];
		if( (EMsvMessageStoreAvailableStatus   == driveEntry.status) ||
			(EMsvMessageStoreUnavailableStatus == driveEntry.status)
		  )
			{
			// Set the current drive index to server.
			CMsvPreferredDriveList::GetDriveList()->SetCurrentDriveIndex(currentDriveIndex);
			
			TParse parse;
			TPtrC drive(TDriveUnit(driveEntry.driveNum).Name());
			parse.Set(KMsvDefaultIndexFile2(), &drive, NULL);
			iMessageFolder = parse.DriveAndPath();
				
			// If the drive already has a message store...
			if(EMsvMessageStoreUnavailableStatus == driveEntry.status)
				{
				// Create the database and all standard table.
				CMsvDBAdapter::CreateDBL(driveEntry.driveNum);
		
				// Update the drive status.
				CMsvPreferredDriveList::GetDriveList()->UpdateDriveStatusL(currentDriveIndex, EMsvMessageStoreAvailableStatus);
				
				ResetAndCreateNewMailStoreL();
				iRunMailInit = ETrue;
			
				// Creating index adapter object.
				iIndexAdapter = CMsvIndexAdapter::NewL(iServer);
				
				// Get updated drive id.
				CreateStandardEntriesFromResourceFileL(KCurrentDriveId);		
				iIndexAdapter->SetLocalServiceComplete();
				}
			else
				{
				// The drive already has a message store.
				
				// Creating index adapter object.
				iIndexAdapter = CMsvIndexAdapter::NewL(iServer);
				
				ResetAndCreateNewMailStoreL(EFalse);						
				}
			break;
			}
		}	
	}	




/**
 * UpdateDriveStatusL()
 * @param TUint: The index of the said drive in preferred
 *               drive list.
 */	
void CMsvIndexContext::UpdateDriveStatusL(TUint aDriveIndex, TDriveState& aStoreStatus)
	{
	aStoreStatus = EMsvInvalidDriveStatus;
	TBool deleteStore = EFalse;
	TMsvPreferredDrive driveEntry;
	CMsvPreferredDriveList::GetDriveList()->DriveInfoL(aDriveIndex, driveEntry);

	// Check if the media is available in the drive.
	TVolumeInfo volume;
	if (iServer.FileSession().Volume(volume, driveEntry.driveNum) != KErrNone)
		{
		CMsvPreferredDriveList::GetDriveList()->UpdateDriveStatusL(aDriveIndex, EMsvDriveDiskNotAvailableStatus);
		aStoreStatus = EMsvDriveDiskNotAvailableStatus;
		return;
		}
	
	// Validate the database. The function opens the database
	// and check its version and returns appropriate error.
	TRAPD(err, CMsvDBAdapter::ValidateDatabaseL(driveEntry.driveNum));
	
	// If no error, database is available.
	if(KErrNone == err)
		{
		CMsvPreferredDriveList::GetDriveList()->UpdateDriveStatusL(aDriveIndex, EMsvMessageStoreAvailableStatus);
		return;
		}
	
	// If the database is not found in the drive.
	if(KErrNotFound == err)
		{
		// Check if index file exists.
		RFile file;
		TParse parse;
		TPtrC drive(TDriveUnit(driveEntry.driveNum).Name());
		parse.Set(KMsvDefaultIndexFile2, &drive, NULL);
		TFileName indexFileName = parse.FullName();
		err = file.Open(iServer.FileSession(), indexFileName, EFileShareAny|EFileWrite);
		//file.Close();
		
		// If index file exists, set drive status to NOT SUPPORTED.
		if(KErrNone == err)
			{
			CPermanentFileStore* fileStoreIndex = NULL;
			TRAPD(permError, fileStoreIndex = CPermanentFileStore::FromL(file));
			if(KErrNone == permError)
				{
				// Check if the store is corrupt. If so then delete it.
				if (fileStoreIndex->Type() != TUidType(KPermanentFileStoreLayoutUid, KUidMsvIndexFile))
					{
					deleteStore = ETrue;
					}
				}
			// There was an error in getting a permanent filestore object.
			// Mark the message store for deletion.
			else
				{
				deleteStore = ETrue;
				}
			
			delete fileStoreIndex;
			
			//If message store is corrupted, wipe it.
			if(deleteStore)
				{
				TFileName mail2Folder = parse.DriveAndPath();
				CFileMan* fileMan = CFileMan::NewL(iServer.FileSession());
				// Remove the readonly attribute..
				(void)fileMan->Attribs(mail2Folder, 0, KEntryAttReadOnly, TTime(0), CFileMan::ERecurse);
				CleanupStack::PushL(fileMan);
				//Check if the mailfolder exists..
				TBool mail2FolderExists = BaflUtils::FileExists(iServer.FileSession(), mail2Folder);
				if(mail2FolderExists)
					{
					// Remove old message store if exists..
					User::LeaveIfError(fileMan->RmDir(mail2Folder));
					}
				CleanupStack::PopAndDestroy(fileMan);
				CMsvPreferredDriveList::GetDriveList()->UpdateDriveStatusL(aDriveIndex, EMsvMessageStoreUnavailableStatus);
				aStoreStatus = EMsvMessageStoreCorruptStatus;
				
				// Create store delete file in the respective drive.
				CreateStoreDeleteFile(driveEntry.driveNum);
				}
			//Else only set status as EMsvMessageStoreNotSupportedStatus.
			else
				{
				CMsvPreferredDriveList::GetDriveList()->UpdateDriveStatusL(aDriveIndex, EMsvMessageStoreNotSupportedStatus);
				aStoreStatus = EMsvMessageStoreNotSupportedStatus;
				}
			
			file.Close();
			return;
			}
		
		// If index file does not exists, set drive status to STORE UNAVAILABLE.
		if((KErrNotFound == err) || (KErrPathNotFound == err))
			{
			CMsvPreferredDriveList::GetDriveList()->UpdateDriveStatusL(aDriveIndex, EMsvMessageStoreUnavailableStatus);
			aStoreStatus = EMsvMessageStoreUnavailableStatus;
			return;
			}
			
		User::Leave(err);
		}
	
	//If a database exists, but is corrupt.
	if( (KSqlErrCorrupt == err) ||
		(KSqlErrNotDb   == err)
	  )
		{
		// If the database is corrupt, delete
		// the database and old message store.
		
		// Delete the database.
		TParse parse;
		TPtrC drive(TDriveUnit(driveEntry.driveNum).Name());
		parse.Set(KDBFileName, &drive, NULL);
		TFileName dBFile = parse.FullName();
		RSqlDatabase::Delete(dBFile);
		
		// Delete the message store. //ISSUE Can we use ResetAndCreateNewMailStore()?
		parse.Set(KMsvDefaultIndexFile2, &drive, NULL);
		TFileName mail2Folder = parse.DriveAndPath();
		CFileMan* fileMan = CFileMan::NewL(iServer.FileSession());
		// Remove the readonly attribute..
		(void)fileMan->Attribs(mail2Folder, 0, KEntryAttReadOnly, TTime(0), CFileMan::ERecurse);
		CleanupStack::PushL(fileMan);
		//Check if the mailfolder exists..
		TBool mail2FolderExists = BaflUtils::FileExists(iServer.FileSession(), mail2Folder);
		if( mail2FolderExists)
			{
			// Remove old message store if exists..
			User::LeaveIfError(fileMan->RmDir(mail2Folder));
			}
		CleanupStack::PopAndDestroy(fileMan);
		
		// Update the drive status.
		CMsvPreferredDriveList::GetDriveList()->UpdateDriveStatusL(aDriveIndex, EMsvMessageStoreUnavailableStatus);
		aStoreStatus = EMsvMessageStoreCorruptStatus;

		// Create store delete file in the respective drive.
		CreateStoreDeleteFile(driveEntry.driveNum);
		return;
		}
	   
	//If the database version is not supported.
	if(KErrNotSupported == err)
		{
		// If the drive as old version of database.
		CMsvPreferredDriveList::GetDriveList()->UpdateDriveStatusL(aDriveIndex, EMsvMessageStoreNotSupportedStatus);
		aStoreStatus = EMsvMessageStoreNotSupportedStatus;
		return;
		}

	// Do not return DB error.
	if(err <= KSqlErrGeneral)
		{
		// If DB Error, return KErrGeneral.
		User::LeaveIfError(KErrGeneral);
		}
	else
		{
		User::LeaveIfError(err);
		}
	}
	
	


TInt CMsvIndexContext::ChangeDrive(TUint aNewDriveIndex, TRequestStatus& aStatus)
	{
	TInt err = ChangeDrive(aNewDriveIndex, ETrue, ETrue);
	
	// Remember the status which we'll complete
	aStatus = KRequestPending;
	iObserverStatus = &aStatus;
	return err;
	}



void CMsvIndexContext::ChangeDriveCompleted(TInt aError)
	{
	// Remember error
	iProgress.iError = aError;	
	
	if (iObserverStatus)
		{
		User::RequestComplete(iObserverStatus, aError);
		}
	}
	
	
	
/**
 * ChangeDriveL()
 * @param TUint: Index of the new current drive in the preferred drive list.
 * @param TBool: True, if old drive is still present in the preferred drive list.
 *               False, otherwise.
 * 
 */
TInt CMsvIndexContext::ChangeDrive(TUint aNewDriveIndex, TBool aIsRemovePartial /*= ETrue*/, TBool aAsync /*=EFalse */)
	{
	TDriveNumber newDriveNumber;
	TRAPD(err, DoChangeDriveL(aNewDriveIndex, aIsRemovePartial, newDriveNumber));

	// Add a function ChangeDriveComplete() in server
	// which should be called here. The function should 
	// run mailInit and recreate backup object.
	iObserver.ChangeDriveComplete(err, iRunMailInit, newDriveNumber);
	
	if(aAsync)
		{
		iStatus = KRequestPending;
		SetActive();
		TRequestStatus* st = &iStatus;
		User::RequestComplete(st, err);
		}
	return err;
	}



void CMsvIndexContext::DoChangeDriveL(TUint aNewDriveIndex, TBool aIsRemovePartial, TDriveNumber& aNewDriveNumber)
	{
	iObserverStatus = NULL;
	TMsvPreferredDrive driveEntry;
	CMsvPreferredDriveList::GetDriveList()->DriveInfoL(aNewDriveIndex, driveEntry);

	aNewDriveNumber = driveEntry.driveNum;
	if(EMsvMessageStoreUnavailableStatus == driveEntry.status)
		{
		// Create the database and all standard table.
		CMsvDBAdapter::CreateDBL(driveEntry.driveNum);
		
		// Update the drive status.
		CMsvPreferredDriveList::GetDriveList()->UpdateDriveStatusL(aNewDriveIndex, EMsvMessageStoreAvailableStatus);
		
		TParse parse;
		TPtrC drive(TDriveUnit(driveEntry.driveNum).Name());
		parse.Set(KMsvDefaultIndexFile2(), &drive, NULL);
		iMessageFolder = parse.DriveAndPath();
		
		ResetAndCreateNewMailStoreL();
		iRunMailInit = ETrue;
			
		// Perform ChangeDrive operation.
		iIndexAdapter->ChangeDriveL(aNewDriveIndex, aIsRemovePartial);
		
		// Read initial entries from resources
		CreateStandardEntriesFromResourceFileL(KCurrentDriveId);
		iIndexAdapter->SetLocalServiceComplete();
		}
	else
		{
		// Update the message folder local variable.
		TParse parse;
		TPtrC drive(TDriveUnit(driveEntry.driveNum).Name());
		parse.Set(KMsvDefaultIndexFile2(), &drive, NULL);
		iMessageFolder = parse.DriveAndPath();
		
		ResetAndCreateNewMailStoreL(EFalse);
		// Perform ChangeDrive operation.
		iIndexAdapter->ChangeDriveL(aNewDriveIndex, aIsRemovePartial);			
		iRunMailInit = EFalse;
		}
	
	// Create service directories in new drive.
	TRAP_IGNORE(DoCreateServiceDirsL(KCurrentDriveId));
	
	TRAP_IGNORE(LocalizeStandardFoldersL());
	}




void CMsvIndexContext::GetInPreparationIds(TUint aDriveId)
	{
	delete iRemoveEntries;
	iRemoveEntries = new CMsvEntrySelection;
	
	iIndexAdapter->GetInPreparationIds(*iRemoveEntries, aDriveId);
	}
	



/**
 * CreateStandardEntriesFromResourceFileL()
 * @param None:
 *
 * Will read messaging resource file and create entries.
 */	
void CMsvIndexContext::CreateStandardEntriesFromResourceFileL(TUint aDriveId)
	{
	// Read initial entries from resources
	TResourceReader reader;
	reader.SetBuffer(iBuf);	
	const TInt numberOfEntries = reader.ReadInt16();
	
	for (TInt index=0; index < numberOfEntries; ++index)
		{
		TMsvEntry newEntry;
				
		// Values from resource file
		newEntry.iId = MaskTMsvId(aDriveId, reader.ReadInt32());
		newEntry.iParentId = reader.ReadInt32();
		newEntry.iServiceId = reader.ReadInt32();
		newEntry.iType.iUid = reader.ReadInt32();
		newEntry.iMtm.iUid = reader.ReadInt32();
		newEntry.iData = reader.ReadInt32();
		newEntry.iDescription.Set(reader.ReadTPtrC());
		newEntry.iDetails.Set(reader.ReadTPtrC());
		newEntry.iDate.UniversalTime();
		newEntry.iSize=0;

		// Create the new entry.
		// This is required to create associated service directory.
		User::LeaveIfError(iServer.AddEntry(this, newEntry, KMsvServerId, EFalse));
		}
	}

#else		// #if (defined SYMBIAN_MSGS_ENHANCED_REMOVABLE_MEDIA_SUPPORT)



void CMsvIndexContext::CreateIndexL(TInt aDrive, TRequestStatus& aStatus)
	{
	// Kick off index loading
	CreateIndexL(aDrive, EFalse);
	// Remember the status which we'll complete
	aStatus = KRequestPending;
	iObserverStatus = &aStatus;
	}



void CMsvIndexContext::CreateIndexL(TInt aDrive, TBool aSync)
	{
	__ASSERT_DEBUG(iProgress.iState == TMsvIndexLoadProgress::EIndexNotLoaded, PanicServer(EMsvBadIndexState));
	__ASSERT_DEBUG(RFs::IsValidDrive(aDrive), PanicServer(EMsvInvalidDrive));

	// Remember stuff
	iConfig.iDrive = TDriveUnit(aDrive);
	iObserverStatus = NULL;
	iRunMailInit = EFalse;

	// Set the drive for this index file
	TParse parse;
	TPtrC drive(iConfig.iDrive.Name());
	parse.Set(KMsvDefaultIndexFile2(), &drive, NULL);

	// Construct the message folder name etc
	iMessageFolder = parse.DriveAndPath();
	TFileName indexFileName = parse.FullName();
	// Adding the new code
	// Now we are creating the new database adapter.
	// Find out if the db exists
	// Construct the db file name
	parse.Set(KDBFileName, &drive, NULL);
	TFileName dBFile = parse.FullName();	
	RSqlDatabase db;
	TRAPD(err,db.OpenL(dBFile));
	TBool create=EFalse;
	
	// Database may be corrupted, so delete the old database and create a new one.
	if(err == KSqlErrCorrupt || err == KSqlErrNotDb )
		{
		RSqlDatabase::Delete(dBFile);
		create = ETrue;
		}
	// Database is not present. We need to check if the store is an old one.
	// If it is, we check if it is corrupt. 
	// If it is corrupt we can safely delete the store and create a new one
	// in its place. If not, then we leave with KErrNotSupported stating that
	// the old store is incompatible with this implementation of the message
	// store which uses an SQL database for the index.
	else if( err == KErrNotFound)
		{
		// Check if any of the old index files exist in the Mail2 folder.
		TParse parse1;
		parse1.Set(KMsvDefaultIndexFile, &drive, NULL); //old version of index file
		if( BaflUtils::FileExists(iServer.FileSession(), parse1.FullName()) ||
		    BaflUtils::FileExists(iServer.FileSession(), indexFileName))
			{
			// Open index store.
			RFile file;
			TInt error = file.Open(iServer.FileSession(), indexFileName, EFileShareAny|EFileWrite);
			
			// Check if we have an old 'legacy' version of the index file.
			CPermanentFileStore* index = NULL;
			TRAPD(permError, index = CPermanentFileStore::FromL(file));
			CleanupStack::PushL(index);
			if (KErrNone == permError)
				{
				// Check if the store is corrupt. If so then delete it.
				if (index->Type() != TUidType(KPermanentFileStoreLayoutUid, KUidMsvIndexFile))
					{
					create = ETrue;
					}
				}
			// There was an error in getting a permanent filestore object.
			// Delete the message store.
			else
				{
				create = ETrue;
				}
			
			CleanupStack::PopAndDestroy(); //index
			file.Close();
			
			// If the old store is valid, then throw an error.
			if(!create)
				{
				User::Leave(KErrNotSupported);
				}
			}
		//Index file does not exist in Mail2 folder. Go ahead with database creation.
		else
			{
			create = ETrue;
			}
		}	
	else
		{
		db.Close();
		User::LeaveIfError(err);
		}
	
	db.Close();
	
	if(!create)
		{
		// Check for database version.
		TRAP(err, CMsvDBAdapter::ValidateDatabaseL(dBFile));
		User::LeaveIfError(err);
		
		iIndexAdapter = CMsvIndexAdapter::OpenL(iServer, dBFile);
		ResetAndCreateNewMailStoreL(EFalse);
		}
	else
		{
		iIndexAdapter = CMsvIndexAdapter::NewL(iServer, dBFile);
		ResetAndCreateNewMailStoreL();
		CreateStoreDeleteFile();
		iRunMailInit = ETrue;
		
		TResourceReader reader;
		reader.SetBuffer(iBuf);	
		// Read initial entries from resources
		const TInt numberOfEntries = reader.ReadInt16();
		for (TInt index=0; index<numberOfEntries; ++index)
			{
			TMsvEntry newEntry;
			// Values from resource file			
			newEntry.iId = reader.ReadInt32();
			newEntry.iParentId = reader.ReadInt32();
			newEntry.iServiceId = reader.ReadInt32();
			newEntry.iType.iUid = reader.ReadInt32();
			newEntry.iMtm.iUid = reader.ReadInt32();
			newEntry.iData = reader.ReadInt32();
			newEntry.iDescription.Set(reader.ReadTPtrC());
			newEntry.iDetails.Set(reader.ReadTPtrC());
			newEntry.iDate.UniversalTime();
			newEntry.iSize=0;

			// Create the new entry
			// This is required to create associated service directory.
			TInt error = iServer.AddEntry(this, newEntry, KMsvServerId, EFalse);
			User::LeaveIfError(error);
			}
		iIndexAdapter->SetLocalServiceComplete();	
			
		}
	if(aSync)
		{
		IndexLoadingCompleted(KErrNone);
		}
	else
		{
		iStatus = KRequestPending;
		SetActive();
		TRequestStatus* st = &iStatus;
		User::RequestComplete(st, KErrNone);// do we need to propogate the error from adapter to here
		}
	}



TInt CMsvIndexContext::LoadStoreConfig(TBool aLoad)
//
// In-between function to save the cost of a trap
//
	{
	TRAPD(error, aLoad ? CMsvServer::CurrentConfigL(iServer.FileSession(), iConfig) : DoStoreConfigL());
	return error;
	}




void CMsvIndexContext::DoStoreConfigL()
	{
	// we only want to store the config file if it has changed,
	// we also don't care about UniqueIDs for the internal drive
	if(iConfig.iDebug!=iConfig.iDebugAsLoaded ||
		iConfig.iDrive!=iConfig.iDriveAsLoaded ||
		(iConfig.iDrive!=iServer.FileSession().GetSystemDrive() && iConfig.iUniqueID!=iConfig.iUniqueIDAsLoaded))
		{
		TChar driveChar= iServer.FileSession().GetSystemDriveChar();
		TBuf<2> systemDrive;
		systemDrive.Append(driveChar);
		systemDrive.Append(KDriveDelimiter);
	    TPath pathName(systemDrive);
		pathName.Append(KServerINIFile);
		CDictionaryFileStore *store=CDictionaryFileStore::OpenLC(iServer.FileSession(),pathName,KNullUid);
		RDictionaryWriteStream stream;
		stream.AssignLC(*store, KUidMsvMessageDriveStream);

		stream.WriteUint8L(KMsvMessageDriveStreamVersionNumber); // version number
		stream << iConfig.iDrive.Name();
		stream.WriteUint32L(iConfig.iUniqueID);
		stream.WriteInt8L(iConfig.iDebug);

		stream.CommitL();
		store->CommitL();
		CleanupStack::PopAndDestroy(2,store); // stream, store
		}
	}



void CMsvIndexContext::IndexLoadingCompleted(TInt aError)
	{
	// Remember error
	iProgress.iError = aError;

	// If there is no index at this point it's because the index loading is synchronous
	if (!iIndexAdapter && !aError)
		return;
	
	// If there was an error, wipe the message folder and create a blank one
	if (aError == KErrCorrupt || aError == KErrOverflow || aError == KErrNotFound || aError == KErrEof || aError == KErrNotSupported)
		{
#ifndef _NO_SERVER_LOGGING_
		iServer.Log(_L("Index corrupted, starting rebuild (%d)"), aError);
#endif
		// Note: Following doesn't send any notifications
		IndexFailedToLoad(KErrNone);
		
		//Create a file to let the UI know that the store has been deleted.
 		CreateStoreDeleteFile();
		}

	// Get the drive Id
	if (!aError)
		{
		TVolumeInfo volume;
		aError = iServer.FileSession().Volume(volume, TInt(iConfig.iDrive));
		if (!aError)
			{
			iConfig.iUniqueID = volume.iUniqueID;
			}
		}		

	// If we are not rebuilding, then inform clients of outcome
	if (aError)
		IndexFailedToLoad(aError);
	else
		IndexLoaded(EMsvNullNotification);

	// Tell the observer that we've finshed
	iObserver.ContextComplete(aError, iRunMailInit);

	if (iObserverStatus)
		User::RequestComplete(iObserverStatus, aError);
	}



void CMsvIndexContext::IndexFailedToLoad(TInt aError)
//
// The index has failed to load. Remove partially created in-memory index
//
	{
	// Update progress
	iProgress.iError = aError;
	delete iIndexAdapter;
	iIndexAdapter = NULL;
	iProgress.iState = TMsvIndexLoadProgress::EIndexNotLoaded;
	if (aError)
		iServer.NotifyChanged(EMsvIndexFailedToLoad, KMsvNullIndexEntryId, aError);
	}


#endif 		// #if (defined SYMBIAN_MSGS_ENHANCED_REMOVABLE_MEDIA_SUPPORT)

	
	

void CMsvIndexContext::IndexLoaded(TMsvServerChangeNotificationType aNotification)
//
// The index has been successfully loaded
//
	{
	iProgress.iState = TMsvIndexLoadProgress::EIndexComplete;
	iProgress.iError = KErrNone;

	// Remove any orphaned entries
	__ASSERT_DEBUG(iRemoveEntries->Count()==0, PanicServer(EMsvRemovingOrphanedEntries));
	iIndexAdapter->GetInPreparationIds(*iRemoveEntries);

	// kick off the delay timer, this will allow the message server to make
	// any changes it wants to the index on startup, currently it will
	// remove any entries in iRemoveEntries and try and localize the standard folders.
	if (!iServer.Delay().IsActive())
		iServer.Delay().After(KMsvInitDelayTime);
	
#if (defined SYMBIAN_MSGS_ENHANCED_REMOVABLE_MEDIA_SUPPORT)
	TRAP_IGNORE(DoCreateServiceDirsL(KCurrentDriveId));
#else
	TRAP_IGNORE(DoCreateServiceDirsL());
#endif

	// Queue notification - it will be sent before EMsvIndexLoaded
	// when we can guarantee that index is fully loaded
	if (aNotification != EMsvNullNotification)
		iServer.QueueNotifyChanged(aNotification);
	}

void CMsvIndexContext::LocalizeStandardFoldersL()
	{
	// Get ready to read resource
	TResourceReader reader;
	reader.SetBuffer(iBuf);

	// Read initial entries from resources
	const TInt numberOfEntries = reader.ReadInt16();
	for (TInt ii=0; ii<numberOfEntries; ii++)
		{
		TMsvEntry resourceEntry;

		// Values from resource file
		resourceEntry.iId = reader.ReadInt32();
		resourceEntry.iParentId = reader.ReadInt32();
		resourceEntry.iServiceId = reader.ReadInt32();
		resourceEntry.iType.iUid = reader.ReadInt32();
		resourceEntry.iMtm.iUid = reader.ReadInt32();
		resourceEntry.iData = reader.ReadInt32();
		resourceEntry.iDescription.Set(reader.ReadTPtrC());
		resourceEntry.iDetails.Set(reader.ReadTPtrC());

		// Add Universal time and initialise size
		resourceEntry.iDate.UniversalTime();
		resourceEntry.iSize=0;
		TMsvEntry *entry;
		
		// 557. Will by default pick entries from current drive.
		TInt error = iIndexAdapter->GetEntry(resourceEntry.Id(),entry);
		if(error ==KErrNone)
			{
			TMsvEntry newEntry= *entry;
			TBool changed=EFalse;
			if(newEntry.iDescription.Compare(resourceEntry.iDescription)!=0)
				{
				newEntry.iDescription.Set(resourceEntry.iDescription);
				changed=ETrue;
				}
			if(newEntry.iDetails.Compare(resourceEntry.iDetails)!=0)
				{
				newEntry.iDetails.Set(resourceEntry.iDetails);
				changed=ETrue;
				}

			// ignore the error we don't want to fail just because the
			// inbox isn't in the right language, we will try again next
            // time the server starts.
			if(changed!=EFalse)
				{
				iIndexAdapter->LockEntry(newEntry.Id());
				iIndexAdapter->ChangeEntryInternal(newEntry, KMsvServerId);
				iIndexAdapter->ReleaseEntry(newEntry.Id());
				}
		   	}
		else if( error == KErrNotFound )
        	{
#if (defined SYMBIAN_MSGS_ENHANCED_REMOVABLE_MEDIA_SUPPORT)
			resourceEntry.SetParent(MaskTMsvId(KCurrentDriveId, resourceEntry.iParentId));
			resourceEntry.SetId(MaskTMsvId(KCurrentDriveId, resourceEntry.iId));
#endif
			// Create the new entry
			iServer.AddEntry(this, resourceEntry, KMsvServerId, EFalse);
        	}
	    }
	}



#if (defined SYMBIAN_MSGS_ENHANCED_REMOVABLE_MEDIA_SUPPORT)
void CMsvIndexContext::DoCreateServiceDirsL(TUint aDriveId)
#else
void CMsvIndexContext::DoCreateServiceDirsL()
#endif
	{
	CMsvEntryFilter* filter = CMsvEntryFilter::NewLC();
	filter->SetOrder(TMsvSelectionOrdering(0, EMsvSortByNone, ETrue));

	CMsvEntrySelection* selection = new(ELeave)CMsvEntrySelection;
	CleanupStack::PushL(selection);
	TInt err = KErrNone;
#if (defined SYMBIAN_MSGS_ENHANCED_REMOVABLE_MEDIA_SUPPORT)
	err = iIndexAdapter->GetChildrenId(MaskTMsvId(aDriveId, KMsvRootIndexEntryId), *filter, *selection);
#else
	err = iIndexAdapter->GetChildrenId(KMsvRootIndexEntryId, *filter, *selection);
#endif

	// make sure the service directories are there
	if (err == KErrNone)
		{
		TInt count = selection->Count();
		while (count--)
			{
			TFileName filename;
			filename.Copy(iMessageFolder);

			TMsvId id = selection->At(count);

#if (defined SYMBIAN_MSGS_ENHANCED_REMOVABLE_MEDIA_SUPPORT)
			MsvUtils::ConstructEntryName(UnmaskTMsvId(id), MaskTMsvId(aDriveId, id), filename, MsvUtils::EPath);
#else
			MsvUtils::ConstructEntryName(id, id, filename, MsvUtils::EPath);
#endif

			iServer.FileSession().MkDir(filename); // ignore any error
			}
		}

	CleanupStack::PopAndDestroy(2); // selection, filter
	}




void CMsvIndexContext::DoCancel()
	{
	if (iIndexAdapter)
		iIndexAdapter->Cancel();
	if (iObserverStatus)
		User::RequestComplete(iObserverStatus, KErrCancel);
	}

void CMsvIndexContext::RunL()
	{
#if (defined SYMBIAN_MSGS_ENHANCED_REMOVABLE_MEDIA_SUPPORT)	
	// In 557, ChangeDriveL() can be asynchronous,
	// but CreateIndexL() will always be synchronous.
	ChangeDriveCompleted(iStatus.Int());
#else
	IndexLoadingCompleted(iStatus.Int());
#endif
	}

const TMsvIndexLoadProgress& CMsvIndexContext::Progress()
	{
	// Get the progress from the index first
	if (iIndexAdapter)
		iProgress.iIndex = iIndexAdapter->Progress();
	return iProgress;
	}



TBool CMsvIndexContext::GetAndClearIndexCorruptFlagL()
	{
	RFs& fs=iServer.FileSession();

#if (defined SYMBIAN_MSGS_ENHANCED_REMOVABLE_MEDIA_SUPPORT)
	TDriveNumber driveNum = CMsvPreferredDriveList::GetDriveList()->CurrentDriveNumber();
	User::LeaveIfError(fs.SetSessionToPrivate(driveNum));
#else
	User::LeaveIfError(fs.SetSessionToPrivate(Config().iDrive));
#endif		// #if (defined SYMBIAN_MSGS_ENHANCED_REMOVABLE_MEDIA_SUPPORT)

	TInt error=fs.Delete(KStoreDeletedFile);
	if(error !=KErrNone && error != KErrNotFound)
		{
		User::Leave(error);
		}
	// at this point error is either KErrNone if the file indicating the index
	// was corrupt is present and has been deleted or KErrNotFound if the file
	// was not present. The function returns ETrue if the file was present and
	// therefore the index was corrupt and the user should be informed.
	return error == KErrNone ? ETrue : EFalse;
	}



#if (defined SYMBIAN_MSGS_ENHANCED_REMOVABLE_MEDIA_SUPPORT)
void CMsvIndexContext::CreateStoreDeleteFile(TDriveNumber aDriveNum)
#else
void CMsvIndexContext::CreateStoreDeleteFile()
#endif
	{
	
#if (!defined SYMBIAN_MSGS_ENHANCED_REMOVABLE_MEDIA_SUPPORT)
	TDriveNumber aDriveNum = (TDriveNumber)(TInt) Config().iDrive;
#endif

	RFs& fs = iServer.FileSession();
	
	TInt err = fs.SetSessionToPrivate(aDriveNum);	
	if(err == KErrNone)
		{
		RFile storeDeletedFile;
		#if (defined SYMBIAN_MESSAGESTORE_UNIT_TESTCODE)
			TParse parse;
			TPtrC drive(TDriveUnit(aDriveNum).Name());
			parse.Set(KStoreDeletedFile, &drive, NULL);
			TFileName fileName = parse.FullName();
		#else
			TFileName fileName(KStoreDeletedFile);
		#endif		// #if (defined SYMBIAN_MESSAGESTORE_UNIT_TESTCODE)
		
		err = storeDeletedFile.Replace(fs, fileName, EFileShareExclusive);
		if(err == KErrNone)
			{
			_LIT8(KMsvIndexStoreDeleted,"StoreDeleted");
			storeDeletedFile.Write(KMsvIndexStoreDeleted);
			}
		storeDeletedFile.Close();
		}
	}




// Reset Mail Store(Mail2) so that old messages are deleted when new index file created..
void CMsvIndexContext::ResetAndCreateNewMailStoreL(TBool aDelete)
	{
	CFileMan* fileMan = CFileMan::NewL(iServer.FileSession());
	CleanupStack::PushL(fileMan);

	if(aDelete)
		{
		//Check if the mailfolder exists..
		if(BaflUtils::FileExists(iServer.FileSession(), iMessageFolder))
			{
			// Remove the readonly attribute..
			(void)fileMan->Attribs(iMessageFolder,0, KEntryAttReadOnly, TTime(0), CFileMan::ERecurse);
			// Remove old message store if exists..
			User::LeaveIfError(fileMan->RmDir(iMessageFolder));
			}
		}
		
	// Create the folder for the message store
	TInt err = iServer.FileSession().MkDirAll(iMessageFolder);
	if(err != KErrAlreadyExists)
		{
		User::LeaveIfError(err);
		}
	CleanupStack::PopAndDestroy(fileMan);
	}
