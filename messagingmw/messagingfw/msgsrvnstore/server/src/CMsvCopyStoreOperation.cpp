// Copyright (c) 2004-2009 Nokia Corporation and/or its subsidiary(-ies).
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

#include "MSVSERV.H"
#include "MSVAPI.H"
#include <cmsvcopystoreoperation.h>

// the amount of space used, even if empty (an approximation)
const TInt KMinimumSpace=400;

_LIT(KIndex, "Index");

CMsvCopyStoreOperation::CMsvCopyStoreOperation(const RMessage2& aMessage, CMsvServer& aServer)
: CMsvServerOperation(aMessage, aMessage.Int0(), KUidMsvLocalServiceMtm, KMsvLocalServiceIndexEntryId, -1), iDrive(TDriveUnit (aMessage.Int1() )), iServer(aServer)
	{
	CActiveScheduler::Add(this);
	}

CMsvCopyStoreOperation* CMsvCopyStoreOperation::NewL(const RMessage2& aMessage, CMsvServer& aServer)
	{
	CMsvCopyStoreOperation* self=new(ELeave)CMsvCopyStoreOperation(aMessage, aServer);
	return self;
	}

CMsvCopyStoreOperation::~CMsvCopyStoreOperation()
	{
	Cancel();
	delete iDir;
	delete iFileMan;
	}


void CMsvCopyStoreOperation::StartL()
	{
	__ASSERT_DEBUG(!IsActive() , PanicServer(EMsvCopyStoreReset));

	iProgress().iState=TMsvCopyProgress::ENotYetStarted;
	iPos=0;

	iFileMan = CFileMan::NewL(iServer.FileSession(), this);

	CompleteSelf();
	}

/* Function to retrieve progress of the Copy operation
**/
const TDesC8& CMsvCopyStoreOperation::Progress()
	{
	 return iProgress;
	}

void CMsvCopyStoreOperation::DoCancel()
	{
	iCopyCancel = ETrue;
	Completed(KErrCancel);
	}


/* Run different operations like LockStore, CopyStore
   UnlockStore and Completed */

void CMsvCopyStoreOperation::RunL()
	{
	User::LeaveIfError(iStatus.Int());
	switch(iProgress().iState)
		{
		case TMsvCopyProgress::ENotYetStarted:
			LockMailStoreL();
			iProgress().iState=TMsvCopyProgress::ELock;
			break;
		case TMsvCopyProgress::ELock:
			InitCopyStoreL();
			iProgress().iState=TMsvCopyProgress::EInitCopy;
			break;

		case TMsvCopyProgress::EInitCopy:
			iProgress().iState=TMsvCopyProgress::ECopyStore;
		//drop through the next case statement
		case TMsvCopyProgress::ECopyStore:
			if (iPos<iProgress().iTotal)
				{
				CopySourceL();
				}
			else
			   	{
				FinishCopyStoreL();
				iProgress().iState=TMsvCopyProgress::EUnlock;
			   	}
			break;
		case TMsvCopyProgress::EUnlock:
			Completed(KErrNone);
			iProgress().iState=TMsvCopyProgress::ECompleted;
			break;
		default:
			__ASSERT_DEBUG(EFalse,PanicServer(EMsvCopyStoreReset));
			Completed(KErrUnknown);
			iProgress().iState=TMsvCopyProgress::ECompleted;
			break;
		}
	}


/* Lock the Mail store so other operations dont use it while
   the copy event is in progress
*/
void CMsvCopyStoreOperation::LockMailStoreL()
	{
	User::LeaveIfError(iServer.Context().IndexAdapter()->ErrorState());
	iServer.NotifyChanged(EMsvMediaUnavailable, KMsvNullIndexEntryId, iServer.Drive());
	iServer.SetStartupState(EMsvMediaUnavailable);
	iServer.Context().IndexAdapter()->SetErrorState(KErrNone);
	//Complete yourself
	CompleteSelf();
	}


/* Initialise the copy process by setting the destination path,etc
*/
void CMsvCopyStoreOperation::InitCopyStoreL()
	{
	//Check if the drive already contains a store,
	//if so then unlock the index file and complete
	if(MessageServer::DriveContainsStore(iServer.FileSession(),iDrive))
		{
		User::Leave(KErrAlreadyExists);
		}
	else
		{
		//Create the destination path
		iDest = iDrive.Name();
		iDest.Append(iServer.Context().MessageFolder());

		//Delete the drive letters from Messagefolder path
		iDest.Delete(2,2);

		//check if the target disk has enough disk space
		CheckDiskSpaceL();

		//Get the directory structure of the source path
		delete iDir;
		iDir=NULL;
    	User::LeaveIfError(iServer.FileSession().GetDir(iServer.Context().MessageFolder(), KEntryAttDir|KEntryAttNormal, ESortNone, iDir));
    	iProgress().iTotal = iDir->Count();

 		//Check if there is any broken store already existing on iDest
 		TUint unused;
		TInt error=iServer.FileSession().Att(iDest,unused);
		if(error == KErrNone)
			{
			//delete any files that may exist
			User::LeaveIfError(iFileMan->RmDir(iDest,iStatus));
			SetActive();
			}
		else
			{
		    //Complete
   			CompleteSelf();
            }
		}
	}


/** Copy Message Store directories from source to destination
	except for the Index file, which is copied in the end after
	all goes well.
*/
void CMsvCopyStoreOperation::CopySourceL()
	{
	iCopyCancel = EFalse;
	TEntry entry;
	TFileName dest=iDest;
	entry = (*iDir)[iPos];

	if(entry.iName==KIndex)
    	{
    	//complete yourself
	   	CompleteSelf();
    	}
    else
    	{
	    iSrc.Zero();
    	iSrc.Append(iServer.Context().MessageFolder());
       	iSrc.Append(entry.iName);
       	dest.Append(entry.iName);

       	// Check if the entry is a directory
    	if(entry.IsDir())
    		{
    		dest.Append('\\');
    		iSrc.Append('\\');

      		// Get the contents of this directory
      		CDir *dirList=NULL;
      		TInt err = iServer.FileSession().GetDir(iSrc, KEntryAttDir, ESortByName, dirList);

      		// Get the count to check if the directory is empty
      		TInt count = dirList->Count();
      		delete dirList;

      		if(err == KErrNone)
      			{
      			// Create an empty directory
      			User::LeaveIfError(iServer.FileSession().MkDirAll(dest));

      			// Copy the contents over
      			if( count != 0 )
       	     		{
       	     		User::LeaveIfError(iFileMan->Copy(iSrc,dest,CFileMan::ERecurse,iStatus));
    				SetActive();
       	     		}
       	     	// Directory empty nothing needed
       	    	else
       				{
       				CompleteSelf();
       				}
      			}
      		else
      			{
       			User::Leave(err);
      			}
    		}
    	else
    		{
    		if(iPos==0)
    			{
    			User::LeaveIfError(iServer.FileSession().MkDirAll(dest));
    			}
    		User::LeaveIfError(iFileMan->Copy(iSrc,dest,CFileMan::EOverWrite,iStatus));
    		SetActive();
    		}
    	}
    dest.Zero();
    iProgress().iCurrent = iPos++;
    }


/** Finish the Copy Process by copying over the index
*/
void CMsvCopyStoreOperation::FinishCopyStoreL()
	{
	TDriveUnit drive = MessageServer::CurrentDriveL(iServer.FileSession());
	
	TParse parse;
	
	TPtrC currDrive(drive.Name());
	parse.Set(KMsvDbFile, &currDrive, NULL);
	TFileName src = parse.FullName();
	
	TPtrC newDrive(iDrive.Name());
	parse.Set(KMsvDbFile, &newDrive, NULL);
	TFileName dest = parse.FullName();
	
#if (defined SYMBIAN_MSGS_ENHANCED_REMOVABLE_MEDIA_SUPPORT)
	TRAP_IGNORE(iServer.Context().IndexAdapter()->GetDbAdapter()->DetachDBL(KCurrentDriveId));
#else
	delete iServer.Context().IndexAdapter()->GetDbAdapter();
#endif
	
	RSqlDatabase::Copy(src, dest);

#if (defined SYMBIAN_MSGS_ENHANCED_REMOVABLE_MEDIA_SUPPORT)
	// Update the drive status of the target 
	// drive in the preferred drive list.
	iServer.UpdateDriveStatusL(iDrive, EMsvMessageStoreAvailableStatus);
#endif

	CompleteSelf();
	}


/** Unlock the Index file so other functions can
    use it once again
*/
void CMsvCopyStoreOperation::UnlockMailStore()
	{
#if (defined SYMBIAN_MSGS_ENHANCED_REMOVABLE_MEDIA_SUPPORT)
	TMsvId maxId;
	TDriveNumber driveNum = CMsvPreferredDriveList::GetDriveList()->CurrentDriveNumber();
#if (defined SYMBIAN_MESSAGESTORE_HEADER_BODY_USING_SQLDB)
	TRAP_IGNORE(iServer.Context().IndexAdapter()->GetDbAdapter()->AttachDBL(driveNum, KCurrentDriveId, maxId, &iServer.MessageDBAdapter()));
#else
	TRAP_IGNORE(iServer.Context().IndexAdapter()->GetDbAdapter()->AttachDBL(driveNum, KCurrentDriveId, maxId));
#endif
#else
	TDriveUnit driveUnit;
	TRAP_IGNORE(driveUnit = MessageServer::CurrentDriveL(iServer.FileSession()));
	TPtrC drive(driveUnit.Name());
	TParse parse;
	parse.Set(KMsvDbFile, &drive, NULL);
	TFileName dest = parse.FullName();
	TRAP_IGNORE(iServer.Context().IndexAdapter()->OpenclosedL(dest));
#endif		// #if (defined SYMBIAN_MSGS_ENHANCED_REMOVABLE_MEDIA_SUPPORT)

	iServer.NotifyChanged(EMsvMediaAvailable, KMsvNullIndexEntryId, iServer.Drive());
	iServer.Context().IndexAdapter()->SetErrorState(KErrNone);
	iServer.SetStartupState(EMsvNullNotification);
	}


/** Complete the unlock process and Set yourself active
*/
void CMsvCopyStoreOperation::CompleteSelf()
	{
	TRequestStatus *status=&iStatus;
	iStatus=KRequestPending;
	SetActive();
	User::RequestComplete(status,KErrNone);
	}


TInt CMsvCopyStoreOperation::RunError(TInt aError)
	{

	//Delete any broken store on the target disk
	TUint unused;
	if (iDest.Length() > 0)
		{
		if(iServer.FileSession().Att(iDest,unused) == KErrNone)
			{
			iFileMan->RmDir(iDest);	// ignore error - don't care if fails
			}
		}

	Completed(aError);
	return(KErrNone);
	}

/* Completed */
void CMsvCopyStoreOperation::Completed(TInt aError)
	{
	iProgress().iError=aError;
	if(iProgress().iState >= TMsvCopyProgress::ELock)
		{
		 UnlockMailStore();
		}
	SetState(EMsvOperationCompleted);
	iMessage.Complete(KErrNone);
	}

/* Check the target disk space */
void CMsvCopyStoreOperation::CheckDiskSpaceL()
	{
	RFs& fs = iServer.FileSession();
	TInt64 spaceNeed=KMinimumSpace;
	CDirScan* dirScan = CDirScan::NewLC(fs);
	CDir* dirList = NULL;
    
	TVolumeIOParamInfo volumeIOInfo;
	User::LeaveIfError(fs.VolumeIOParam(TInt(iDrive), volumeIOInfo));
	TInt clusterSize = volumeIOInfo.iClusterSize;
	dirScan->SetScanDataL(iServer.Context().MessageFolder(), KEntryAttNormal, ESortByName, CDirScan::EScanDownTree);

	do{
		dirScan->NextL(dirList);
		CleanupStack::PushL(dirList);
		if(dirList)
			{
			for(TInt item = 0; item < dirList->Count(); ++item)
				{
				const TEntry& entry = (*dirList)[item];
				spaceNeed+=entry.iSize+clusterSize;
				}
			}
		else
			{
			spaceNeed+=KMinimumSpace;
			}
         
		CleanupStack::PopAndDestroy(dirList);
		} while(dirList); 

	 CleanupStack::PopAndDestroy(dirScan);
	 TVolumeInfo volumeInfo;
	 User::LeaveIfError(iServer.FileSession().Volume(volumeInfo, iDrive));
	 if(volumeInfo.iFree<spaceNeed)
	 	{
	 	User::Leave(KErrDiskFull);
	 	}
	 }
/**
NotifyFileManOperation is called by the Fileman observer framework after/before each
file operation.
@param None.
@return MFileManObserver::TControl
        ECancel If the operation is cancelled for one entry.
        EContinue If the operation needs to be continued.
*/
MFileManObserver::TControl CMsvCopyStoreOperation::NotifyFileManOperation()
	{
	if(iCopyCancel)
		{
		return MFileManObserver::ECancel;
		}
	return MFileManObserver::EContinue;
	}

/**
NotifyFileManEnded is called by the Fileman observer framework to inform the observer
that an operation is complete.
@param None.
@return MFileManObserver::TControl
        EAbort If the bulk operation is cancelled.
        EContinue If the operation needs to be continued.

*/
MFileManObserver::TControl CMsvCopyStoreOperation::NotifyFileManEnded()
	{
	if(iCopyCancel)
		{
		return MFileManObserver::EAbort;
		}
	return MFileManObserver::EContinue;
	}
