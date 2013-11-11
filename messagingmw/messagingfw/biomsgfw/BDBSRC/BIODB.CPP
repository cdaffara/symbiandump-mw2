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
// BIODB.CPP
//

#include "BIODB.H"
#include "BDBPAN.H"
#include "cbifentry.h"

#include <s32file.h>		// CFileStore
#include <bautils.h>

#include <basched.h>
#ifdef SYMBIAN_ENABLE_SPLIT_HEADERS 
#include <bifchangeobserver.h>
#endif

//
// Globals & Constants
//
// Old search paths are /system/bif
// New (platsec) search paths are /resource/messaging/bif

_LIT(KSecureBifPath, "\\resource\\messaging\\Bif\\");     
_LIT(KSecureBifSearchString, "?:\\resource\\messaging\\Bif\\");

_LIT(KLanguageFileExt, ".r");
_LIT(KDotRsc, ".rsc");

// 30 seconds between attempts to scan drives if we get completed with an error,
const TInt KBifRetryTimeOut=30000000;
// Maximam attempts to scan drives if we get completed with an error,
const TInt KMaxRetries = 1;

// default size of array of entries in a bif directory
const TInt KBifEntryArrayDefaultSize=5;
const TInt KMinFileNameLength = 5;

/*
 *	CBifChangeObserver
 */
 
/** Allocates and constructs a new BIF change observer.

@param	aObserver 
Callback interface to notify when a change occurs

@param	afileSystem 
Connected file system handle

@return 
New BIF change observer 
*/
EXPORT_C CBifChangeObserver* CBifChangeObserver::NewL(MBifChangeObserver& aObserver, RFs& aFs)
	{
	CBifChangeObserver* self=new(ELeave) CBifChangeObserver(aObserver, aFs);
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop(self);
	return self;
	}

void CBifChangeObserver::ConstructL()
	{
	iBioDB = CBIODatabase::NewL(iFs);
	User::LeaveIfError(iTimer.CreateLocal());
	}

CBifChangeObserver::CBifChangeObserver(MBifChangeObserver& aObserver, RFs& aFs)
: CActive(EPriorityStandard), iChangeObserver(aObserver), iFs(aFs)
	{
	CActiveScheduler::Add(this);
	}

/** Destructor. */
CBifChangeObserver::~CBifChangeObserver()
	{
	Cancel();
	iTimer.Close();
	delete iBioDB;
	iEntries.ResetAndDestroy();
	}

/** Starts the object watching for changes.

To stop the object, simply delete it.
*/
EXPORT_C void CBifChangeObserver::Start()
	{
	// just complete myself so that RunL builds the list of entries and asks the file
	// sever to notify us of changes.
	TRequestStatus *status=&iStatus;
	User::RequestComplete(status,KErrNone);
	SetActive();
	}

void CBifChangeObserver::RunL()
	{
	TInt err=KErrNone;
	if(iStatus.Int()==KErrNone)
		{
		// ignore the error
		TRAP(err, DoRunL());
		}
#ifdef _DEBUG
 	else
		{
		// I don't expect to get here, it means that RFs::NotifyChangeL has completed
		// this class with an error, which means we would loop rather quickly.
		User::Invariant();
		}
#endif
	
	if((iStatus.Int() != KErrNone || err != KErrNone) && iRetryCount < KMaxRetries)
		{
		iTimer.After(iStatus,KBifRetryTimeOut);
		iRetryCount ++;
		SetActive();
		}
	else
		{
		WaitForFileNotification();	
		iRetryCount = 0;
		}
	}

void CBifChangeObserver::DoRunL()
    {
    TBool notify=EFalse;
	// build a list of all the TEntrys in KBifPath on all drives/
	RPointerArray<CBifEntry> entryList(KBifEntryArrayDefaultSize);
	TCleanupItem cleanup(CleanupBifArray, &entryList);
	CleanupStack::PushL(cleanup);
	
	TFindFile fileFinder(iFs);     
	CDir *dir=NULL;     
	// Scan this folder from V1.5 onwards
	TInt err = fileFinder.FindWildByDir(_L("*"),KSecureBifPath,dir);
	while(err==KErrNone)
		{
		CleanupStack::PushL(dir);
		TInt count=dir->Count();
		while(count--)
			{
			CBifEntry* bifEntry = CBifEntry::NewLC((*dir)[count]);
			entryList.AppendL(bifEntry);
			CleanupStack::Pop(bifEntry);
			}
		CleanupStack::PopAndDestroy(dir);
		err=fileFinder.Find();
		} 
	
	if (err != KErrNotFound)
		User::LeaveIfError(err);

	// Check to see if iEntries matches entryList
	if(iEntries.Count()!=0) // iEntries is empty the first time in so don't notify anyone.
		{
		// check to see if the two lists of entries are the same
		if(iEntries.Count()!=entryList.Count())
			notify=ETrue;  // not the same if one is larger than the other
		else
			{
			TInt count=entryList.Count();
			while(count-- && notify==EFalse)
				{
				TInt index;
				TInt found=FindEntry(*(entryList)[count],iEntries,index);
				if(found==KErrNone)
					{
					delete (iEntries)[index];
					iEntries.Remove(index);
					}
				else
					notify=ETrue; // not the same if entryList contains something not in iEntries
				}
			if(iEntries.Count()!=0)
				notify=ETrue; // not the same if iEntries contains something not in entryList;
			}
		}
		
	// replace old iEntries with new entryList
	CleanupStack::Pop(&entryList);
	iEntries.ResetAndDestroy();
	iEntries=entryList;
	
	// notify observers if iEntries was != entryList;
	if(notify!=EFalse) 
		NotifyObserverL();
	}

TInt CBifChangeObserver::FindEntry(const CBifEntry& aBifEntry, const RPointerArray<CBifEntry>& aEntries, TInt& aIndex) const
	{
	aIndex = aEntries.Count();

	while (aIndex--)
		{
		if ( aBifEntry == *aEntries[aIndex] )
			{
			return KErrNone;
			}
		}

	return KErrNotFound;
	}

void CBifChangeObserver::WaitForFileNotification()
	{
	// Observe this folder from V1.5 onwards
	iFs.NotifyChange(ENotifyAll, iStatus, KSecureBifSearchString);
	SetActive();
	}

void CBifChangeObserver::DoCancel()
	{
    iTimer.Cancel();
	
	iFs.NotifyChangeCancel(iStatus);	// Cancel only the specific
										// request made by this observer
	}

TBool CBifChangeObserver::CompareReaders(const CBioInfoFileReader& aReader1, const CBioInfoFileReader& aReader2) const
	{
	TBool retVal = aReader1.BifEntry() == aReader2.BifEntry();

	if (retVal)
		{
		//Compare the readers
		retVal = 
			   (aReader1.MessageParserName().Compare(aReader2.MessageParserName()) == 0)
			&& (aReader1.MessageAppCtrlName().Compare(aReader2.MessageAppCtrlName()) == 0)
			&& (aReader1.MessageAppUid() == aReader2.MessageAppUid())
			&& (aReader1.GeneralData1() == aReader2.GeneralData1())
			&& (aReader1.GeneralData2() == aReader2.GeneralData2())
			&& (aReader1.GeneralData3() == aReader2.GeneralData3())
			&& (aReader1.IconsFilename().Compare(aReader2.IconsFilename()) == 0)
			&& (aReader1.Description().Compare(aReader2.Description()) == 0);

		//To Do: Should aReader1.IdsLC() and aReader2.IdsLC() be compared?
		}

	return retVal;
	}

void CBifChangeObserver::NotifyObserverL()
	{
	CArrayFixFlat<TUid>* addedBifs = new (ELeave) CArrayFixFlat<TUid>(1);
	CleanupStack::PushL(addedBifs);

	CArrayFixFlat<TUid>* changedBifs = new (ELeave) CArrayFixFlat<TUid>(1);
	CleanupStack::PushL(changedBifs);

	CArrayFixFlat<TUid>* deletedBifs = new (ELeave) CArrayFixFlat<TUid>(1);
	CleanupStack::PushL(deletedBifs);

	// Figure out what kind of change really happened here
	CBIODatabase* newBioDB = CBIODatabase::NewL(iFs);
	CleanupStack::PushL(newBioDB);


	TInt err = KErrNone;

	TUid uid;
	TInt index = 0;
	TKeyArrayFix key(0, ECmpTInt);
	TInt count = newBioDB->BIOCount();
	
	while (count--)
		{
		newBioDB->GetBioMsgID(count, uid);
		TRAP(err, iBioDB->GetBioIndexWithMsgIDL(uid, index));
		
		if (err)
			{
			//BIF in New BIO DB not found in Old BIO DB
			addedBifs->InsertIsqL(uid, key);
			}
		else
			{
			
			//BIF in both BIO DBs, therefore check to see if the files have changed
			if  (!CompareReaders(iBioDB->BifReader(index), newBioDB->BifReader(count)))
				{
				changedBifs->InsertIsqL(uid, key);
				}

			//Remove this BIF from the Old BIO DB for efficiency in the next while loop.
			iBioDB->RemoveBifL(index);
			}
		}

	count = iBioDB->BIOCount();

	while (count--)
		{
		//All remaining members of iBioDB are deleted BIFs
		iBioDB->GetBioMsgID(count, uid);
		deletedBifs->InsertIsqL(uid, key);
		}

	count = addedBifs->Count();

	while (count--)
		{
		iChangeObserver.HandleBifChangeL(MBifChangeObserver::EBifAdded, addedBifs->At(count));
		}

	count = deletedBifs->Count();

	while (count--)
		{
		iChangeObserver.HandleBifChangeL(MBifChangeObserver::EBifDeleted, deletedBifs->At(count));
		}

	count = changedBifs->Count();

	while (count--)
		{
		iChangeObserver.HandleBifChangeL(MBifChangeObserver::EBifChanged, changedBifs->At(count));
		}

	// update the list we're holding onto
	delete iBioDB;
	iBioDB = newBioDB;
	
	CleanupStack::Pop(); //newBioDB
  	CleanupStack::PopAndDestroy(3); //arrays
	}
	
void CBifChangeObserver::CleanupBifArray( TAny* aBifArray )
	{
	RPointerArray<CBifEntry>* bifArray = static_cast<RPointerArray<CBifEntry>*>(aBifArray);
	bifArray->ResetAndDestroy();
	}

//
// CBIODatabase
//

//
// Construction
//	protected
//
CBIODatabase::CBIODatabase()
	{
	// We're a CBase derived class so we can assume that our data members are intialized to 0
	}

//
EXPORT_C CBIODatabase* CBIODatabase::NewLC(RFs& afileSystem)
/** Allocates and constructs a new BIO database object, leaving the object on the 
cleanup stack.

It initialises the object from all the BIF files in the system\\bif directory.

@param afileSystem Connected file system handle
@return New BIO database object */
	{
	CBIODatabase* self=new(ELeave) CBIODatabase();
	CleanupStack::PushL(self);
	self->ConstructL(afileSystem);
	return self;
	}

//
EXPORT_C CBIODatabase* CBIODatabase::NewL(RFs& afileSystem)
/** Allocates and constructs a new BIO database object.

It initialises the object from all the BIF files in the system\\bif directory.

@param afileSystem Connected file system handle
@return New BIO database object */
	{
	CBIODatabase* self=CBIODatabase::NewLC(afileSystem);
	CleanupStack::Pop();		// self
	return self;
	}

//
// Construction/destruction
void CBIODatabase::ConstructL(RFs& afileSystem)
	{
	// put the real construction here
	iBifReaders= new (ELeave) CArrayPtrFlat<CBioInfoFileReader>(10);
	AddAllBifsL(afileSystem);
	}

//
// Construction/destruction
EXPORT_C CBIODatabase::~CBIODatabase()
/** Destructor. */
	{
	if (iBifReaders)
		iBifReaders->ResetAndDestroy();
	delete iBifReaders;
	}


//
EXPORT_C TInt CBIODatabase::BIOCount()
/** Gets the number of BIF files.

@return Number of BIF files */
	{
	return iBifReaders->Count();
	}

//
EXPORT_C void CBIODatabase::AddAllBifsL(RFs& afileSystem)
/** Initialises the object from all the BIF files in the system\\bif or resource\\messaging\\bif directory.

@param afileSystem Connected file system handle */
	{
	// Walk through all the BIF Files, adding each one
	// to the iBifReaders array
	TDriveList drvList;

	// User::LeaveIfError(afileSystem.Connect());
	User::LeaveIfError(afileSystem.DriveList(drvList));

	for (TInt n=0 ; n<KMaxDrives ; ++n)
		{
		if (drvList[n]!=0)
			{
			TDriveInfo driveinfo;
			TInt ret=afileSystem.Drive(driveinfo,n);
			if ((ret==KErrNone) && (driveinfo.iType!=EMediaNotPresent) && (driveinfo.iType!=EMediaRemote) 
												&& (!(driveinfo.iMediaAtt & KMediaAttLocked)) )
				{
				TDriveUnit driveUnit(n);
				TPath fullPath; // Length must hold x:\resource\messaging\bif\ 
				fullPath.FillZ();
				fullPath = driveUnit.Name();
				fullPath += KSecureBifPath;

				if (afileSystem.SetSessionPath(fullPath) == KErrNone)
					{
					CDir* entryList = NULL;

					// Walk through all the BIF Files, adding each one

 					ret = afileSystem.GetDir( fullPath,
											  KEntryAttNormal|KEntryAttAllowUid,
											  ESortByDate,
											  entryList );
					if (ret==KErrNone)
						{
						CleanupStack::PushL(entryList);
						TInt cnt = entryList->Count();
						for (TInt i = 0; i < cnt; ++i)
							{
							// get the file
							TEntry entry = (*entryList)[i];
							
								TFileName fileName(fullPath);
								fileName.Append(entry.iName);

								// Remove localization index from the filename and replace it with "rsc"
								// example, abc.r05 -> abc.rsc
								TInt extLength(0);
								if ( IsLanguageFileL(entry.iName, extLength) && (fileName.Length() >= KMinFileNameLength) )
									{
									fileName.Replace(fileName.Length() - extLength, extLength, KDotRsc);
									}
								// Use the file most appropriate to the current language
								BaflUtils::NearestLanguageFile(afileSystem,fileName);

								// Load the bif file
								CBioInfoFileReader* bifReader = NULL;
								TRAPD(error, bifReader=CBioInfoFileReader::NewL(afileSystem,fileName));
								if (!error)
									{
									AddBifL(bifReader);									
									}
								else
									{
									// Ignore corrupt files
									delete bifReader;

									if (error != KErrCorrupt && error != KErrExtended && error != KErrNotFound)
										{
										User::LeaveIfError(error);
										}
									}
							}
						CleanupStack::PopAndDestroy(entryList); 
						}
					}
				}
				if (!(ret==KErrNone || 
					  ret==KErrPathNotFound || 
					  ret==KErrNotFound || 
					  ret==KErrNotReady  || 
					  ret==KErrCorrupt ||
					  ret==KErrInUse ||
					  ret==KErrLocked || 
					  ret==KErrNotSupported))
					{
					User::LeaveIfError(ret);
					}
			}
		}

	}

// Is the file a language file
TBool CBIODatabase::IsLanguageFileL(const TDesC& aFileName, TInt& aExtLength) const
	{
	// Check the extension to see if it's a language file. we
 	// have to assume ".rXXX" is a resource file
 	TParse parse;
 	User::LeaveIfError(parse.Set(aFileName, NULL, NULL));
	aExtLength=parse.Ext().Length();
 	return parse.Ext().Left(2).CompareF(KLanguageFileExt) == 0;
	}

//
// This will add the Parser
// Error returned
EXPORT_C void CBIODatabase::AddBifL(CBioInfoFileReader* aBifReader)
/** Adds a specified BIF.

@param abifReader BIF file to add */
	{
	// This will manage the array - if a conflicting message
	// id comes in, it will take the one with higher confidence
	// or if a tie, the first one in
	CleanupStack::PushL(aBifReader);

	// Check if we have a reader with this message id
	TInt index = 0;
    
	TRAPD(leaveValue,GetBioIndexWithMsgIDL(aBifReader->MessageTypeUid(), index));
    
	// We haven't read a BIF with the msgID in yet
	if (leaveValue == KErrNotFound)
		{
		// just add it
		iBifReaders->AppendL(aBifReader);
		CleanupStack::Pop(); // aBifReader
		}
	else
		{
		CApaDataRecognizerType::TRecognitionConfidence newBifConf, oldBifConf;

		// If we do have a match, take the one with higher confidence
		const CArrayFix<TBioMsgId>* msgIDArray = BIOEntryLC(index);
		oldBifConf = (*msgIDArray)[0].iConfidence;
		CleanupStack::PopAndDestroy(); //msgIDArray
		
		newBifConf = (*(aBifReader->IdsLC()))[0].iConfidence; 
		CleanupStack::PopAndDestroy(); //IdsLC
		
		if ( newBifConf > oldBifConf )
			{
			CleanupStack::PushL(aBifReader);
			RemoveBifL(aBifReader->MessageTypeUid());
			iBifReaders->AppendL(aBifReader);
			CleanupStack::Pop();	// aBifReader
			}
		else
			CleanupStack::PopAndDestroy();// aBifReader

		}

	}

//
// Error returned
EXPORT_C void CBIODatabase::RemoveBifL(TUid aMsgID)
/** Removes a BIF specified by UID.

@param aMsgID BIO type UID */
	{
	TInt index = 0;
    
	TRAPD(leaveValue,GetBioIndexWithMsgIDL(aMsgID, index));
    
	// We haven't read a BIF with the msgID in yet
	if (leaveValue == KErrNone)
		{
		// just remove it
		RemoveBifL(index);
		}
	else
		{
		//__ASSERT_DEBUG(Panic(EBioDBNotFound));
		}
	}

EXPORT_C void CBIODatabase::RemoveBifL(TInt aIndex)
/** Removes a BIF specified by index.

@param aIndex Index of file in this object */
	{
	CBioInfoFileReader *bifReader = iBifReaders->At(aIndex);
	iBifReaders->Delete(aIndex);
	delete bifReader;
	}


//
// Get the BioEntry at this index
// Return Index if found, error if not
EXPORT_C const CBioInfoFileReader& CBIODatabase::BifReader(TInt aIndex) const
/** Gets a BIF reader for a BIF at a specified index.

@param aIndex Index
@return BIF file reader */
	{
		__ASSERT_DEBUG(aIndex >= 0 && aIndex <= iBifReaders->Count(),
								Panic(EBioDBNotFound));
		return *iBifReaders->At(aIndex);
	}

//
// Get the BioEntry at this index
// Return Index if found, error if not
EXPORT_C const CArrayFix<TBioMsgId>* CBIODatabase::BIOEntryLC(TInt index)
/** Gets the ID array of a BIF at a specified index.

@param index Index
@return ID array, or NULL if not found */
	{
	if (index >= 0 && index < iBifReaders->Count())
		{
		return iBifReaders->At(index)->IdsLC();
		}
	else
		return NULL;
	}

//
// Get the Index for the BioMessageID
EXPORT_C void CBIODatabase::GetBioIndexWithMsgIDL(TUid aMsgID, TInt& rIndex)
/** Gets the index of a BIF of a specified BIO message type.

@param aMsgID BIO message type
@param rIndex On return, index of found BIF
@leave KErrNotFound Not found */
	{
	rIndex = iBifReaders->Count();

	// See if we have one of these Messages
	while (rIndex--)
		{
		if ((*iBifReaders)[rIndex]->MessageTypeUid() == aMsgID)
			{
			return;
			}
		}
	User::Leave(KErrNotFound);
	}


//
// Get the BioMessageID for the Index
EXPORT_C void CBIODatabase::GetBioMsgID(TInt aIndex, TUid& rMsgID)
/** Gets the BIO message type for a specified index.

@param aIndex Index
@param rMsgID On return, BIO message type */
	{
	__ASSERT_DEBUG(aIndex >= 0 && aIndex <= iBifReaders->Count(),
								Panic(EBioDBNotFound));
	
	rMsgID =  (*iBifReaders)[aIndex]->MessageTypeUid();
	}


//
// Get the BioParserName for the Index
EXPORT_C const TPtrC CBIODatabase::GetBioParserName(TInt aIndex)
/** Gets the BIO parser name for a specified index.

@param aIndex Index
@return BIO parser name */
	{
	__ASSERT_DEBUG( aIndex >= 0 && aIndex <= iBifReaders->Count(),
								Panic(EBioDBNotFound));
	return (*iBifReaders)[aIndex]->MessageParserName();	
	}
	
/** Depreacated method.

Need to use CBIODatabase::GetBioControlName instead.

@see CBIODatabase::GetBioControlName

@param aIndex Not used
@param rControlID Not used 

@panic USER	EInvariantFalse

@deprecated */
EXPORT_C void CBIODatabase::GetBioControlID(TInt /*aIndex*/, TUid& /*rControlID*/)
	{
	User::Invariant();
	}
	
/** Gets the BIO control name for a specified index.

@param aIndex Index
@return BIO control name */
EXPORT_C const TPtrC CBIODatabase::GetBioControlName(TInt aIndex)
	{
	__ASSERT_DEBUG( aIndex >= 0 && aIndex <= iBifReaders->Count(), 
								Panic(EBioDBNotFound));
	return (*iBifReaders)[aIndex]->MessageAppCtrlName();
	}
 

//
// Get the first Port Number for this messaga type 
EXPORT_C void CBIODatabase::GetPortNumberL(TUid aMsgID, TBioMsgIdType aPortType, TInt& aPortNumber)
/** Gets the port number for the BIF ID entry for a specified BIO message type 
and bearer type.

@param aMsgID BIO message type
@param aPortType Bearer type
@param aPortNumber On return, the port number
@leave KErrNotFound Not found */
	{
	TInt index = 0;
	GetBioIndexWithMsgIDL(aMsgID, index);
	
	// We haven't read a BIF with the msgID in yet
	TBioMsgId tranportID;
	GetTransportIDL(index, aPortType, tranportID);
	aPortNumber = tranportID.iPort;
	}


//
// Get the first ID String for this messaga type 
EXPORT_C void CBIODatabase::GetIdentifierTextL(TUid aMsgID, TBioMsgIdType aPortType, TBioMsgIdText& aText)
/** Gets the identifier text for the BIF ID entry for a specified BIO message type 
and bearer type.

@param aMsgID BIO message type
@param aPortType Bearer type
@param aText On return, the identifier text */
	{
	TInt index = 0;
	GetBioIndexWithMsgIDL(aMsgID, index);
	
	TBioMsgId tranportID;
	GetTransportIDL(index, aPortType, tranportID);
	aText.Copy(tranportID.iText);
	}

//
// Get the GetTransportIDL 
void CBIODatabase::GetTransportIDL(TInt aIndex, TBioMsgIdType aPortType, TBioMsgId& aBioMsgID) 
	{
	TBioMsgId tranportID;
	TBool found = EFalse;

	const CArrayFix<TBioMsgId>* msgIDArray = BIOEntryLC(aIndex);

	for (TInt i = 0; i < msgIDArray->Count() && !found ; i++)
		{
		if ( (*msgIDArray)[i].iType == aPortType)
			{
			tranportID = (*msgIDArray)[i];

			// Really should make a copy contructor & = operator
			aBioMsgID.iType = (*msgIDArray)[i].iType;
			aBioMsgID.iConfidence = (*msgIDArray)[i].iConfidence;			
			aBioMsgID.iText.Copy((*msgIDArray)[i].iText);
			aBioMsgID.iPort = (*msgIDArray)[i].iPort;
			aBioMsgID.iCharacterSet = (*msgIDArray)[i].iCharacterSet;
			aBioMsgID.iGeneralIdData = (*msgIDArray)[i].iGeneralIdData;
			found = ETrue;
			}
		}
	CleanupStack::PopAndDestroy(); //msgIDArray
	
	if (!found)
		User::Leave(KErrNotFound);
	}


//
// Get the BioParserName for the Message ID
EXPORT_C const TPtrC CBIODatabase::GetBioParserNameL(TUid aMsgID)
/** Gets the BIO parser name for a specified BIO message type.

@param aMsgID BIO message type
@return BIO parser name
@leave KErrNotFound Not found */
	{
	TInt index = 0;
	TRAPD(leaveValue, GetBioIndexWithMsgIDL(aMsgID, index));

	// We haven't read a BIF with the msgID in yet
	if (leaveValue != KErrNone)
		{
		__ASSERT_DEBUG(0, Panic(EBioDBNotFound));
		User::Leave(leaveValue);
		}
		
	return GetBioParserName(index);
	}


/** Depreacated method.

Need to use CBIODatabase::GetBioControlNameL instead.

@see CBIODatabase::GetBioControlNameL

@param aIndex Not used
@param rControlID Not used

@panic USER	EInvariantFalse

@deprecated */
EXPORT_C void CBIODatabase::GetBioControlIDL(TUid /*aMsgID*/, TUid& /*rControlID*/)
	{
	User::Invariant();
	}
	
/** Gets the BIO control name for a specified BIO message type.

@param aMsgID BIO message type
@return BIO Control name

@leave KErrNotFound BIO message type does not exist */
EXPORT_C const TPtrC CBIODatabase::GetBioControlNameL(TUid aMsgID)
 	{
 	TInt index = 0;
	TRAPD(leaveValue, GetBioIndexWithMsgIDL(aMsgID, index));
     
 	// We haven't read a BIF with the msgID in yet
	if( leaveValue != KErrNone )
 		{
 		__ASSERT_DEBUG(0, Panic(EBioDBNotFound));
		User::Leave(leaveValue);
 		}
		
	return GetBioControlName(index);
 	}
 

//
// Get the String Extension for the BioMessageID
EXPORT_C const TPtrC CBIODatabase::GetFileExtL(TUid aMsgID)
/** Gets the BIO file extension for a specified BIO message type.

@param aMsgID BIO message type
@leave KErrNotFound Not found
@return BIO file extension */
{
	TInt index = 0;
	GetBioIndexWithMsgIDL(aMsgID, index);
	return (*iBifReaders)[index]->FileExtension();
}


//
EXPORT_C void CBIODatabase::GetDefaultSendBearerL(TUid aBioUID, TBioMsgId& rBioMsgIdentifier)
/** Gets the default BIF ID entry for a specified BIO message type.

@param aBioUID BIO message type
@param rBioMsgIdentifier On return, the default BIF ID entry
@leave KErrNotFound Not found */
	{
	TInt index = 0;
	TBioMsgIdType portType = EBioMsgIdUnknown;
	GetBioIndexWithMsgIDL(aBioUID, index);
	GetDefaultSendBearerTypeL(aBioUID, portType);
	GetTransportIDL(index, portType, rBioMsgIdentifier);
	}

//
EXPORT_C void CBIODatabase::GetDefaultSendBearerTypeL(TUid aBioUID, TBioMsgIdType& rPortType)
/** Gets the bearer type for the default BIF ID entry for a specified BIO message 
type.

@param aBioUID BIO message type
@param rPortType On return, the bearer type
@leave KErrNotFound Not found */
	{
	TInt index = 0;
	GetBioIndexWithMsgIDL(aBioUID, index);
	const CArrayFix<TBioMsgId>* msgIDArray = BIOEntryLC(index);
	rPortType = (*msgIDArray)[0].iType;
	CleanupStack::PopAndDestroy(); 
	}

//
EXPORT_C void CBIODatabase::GetDefaultSendBearerByTypeL(TUid aBioUID, TBioMsgIdType aPortType, TBioMsgId& rBioMsgIdentifier)
/** Gets the default BIF ID entry for a specified BIO message type and bearer type.

@param aBioUID BIO message type
@param aPortType Bearer type
@param rBioMsgIdentifier On return, the default BIF ID entry
@leave KErrNotFound Not found */
	{
	TInt index = 0;
	GetBioIndexWithMsgIDL(aBioUID, index);
	GetTransportIDL(index, aPortType, rBioMsgIdentifier);
	}

//
// Get the BIO Entry based on what type it is, pos indicates where to start looking after
// if it can't be found, NULL returned
EXPORT_C const CArrayFix<TBioMsgId>* CBIODatabase::BioEntryByTypeLC(
							TSearchList aSearchType,
							TBioMsgIdType portType, 
							TInt& rIndex)
/** Gets the ID array of a BIF of a specified bearer source type.

@param aSearchType Search type
@param portType Bearer source type
@param rIndex On return, index of found BIF
@return ID array, or NULL if not found */
	{
	
	switch(aSearchType)
	{
		case EStart:
			rIndex = 0;
			break;
		case ENext:
			rIndex++;
			break;
		default:
			__ASSERT_DEBUG(0 ,Panic(EBioDBNotFound));
			break;
	}


		
	const CArrayFix<TBioMsgId>* msgIDArray = BIOEntryLC(rIndex);
	
	while (msgIDArray)
		{
		for (TInt i = 0; i < msgIDArray->Count() ; i++)
			{
			if ( (*msgIDArray)[i].iType == portType)
				return msgIDArray;
			}
		CleanupStack::PopAndDestroy(); //msgIDArray
		msgIDArray = BIOEntryLC(++rIndex);
		}
	return NULL;
	
	}	



//
// This will compare strings for both NBS & IANA types handy when you have a start string
// and want to know if there's a Bio Message to support this

EXPORT_C TInt CBIODatabase::IsBioMessageL(TBioMsgIdType aPortType, const TDesC& aPattern, TUint16 aPort, TUid& rBioMsgUID)
/** Tests if there is a BIO parser for the specificed BIF ID settings.

@param aPortType Bearer type
@param aPattern Text identifier to match
@param aPort Port to match
@param rBioMsgUID On return, the matching BIO message type
@return KErrNone if found, else KErrNotFound */
{
	TBioMsgId bioMessageData;
	bioMessageData.iType = aPortType;
	bioMessageData.iText.Copy(aPattern.Left(KMaxBioIdText));
	bioMessageData.iPort = aPort;

	return ( IsBioMessageL(bioMessageData, rBioMsgUID) );
}

//
EXPORT_C TInt CBIODatabase::IsBioMessageL(TBioMsgId bioMessageData, TUid& rBioMsgUID)
/** Tests if there is a BIO parser for a specified BIF ID.

@param bioMessageData BIF ID to match
@param rBioMsgUID On return, the matching BIO message type
@return KErrNone if found, else KErrNotFound */
	{
	rBioMsgUID = KNullUid;
	TInt pos;
	TUid msgID;
	TBool bioFound = EFalse;

	const CArrayFix<TBioMsgId>* bioMsgIDs = BioEntryByTypeLC(EStart, bioMessageData.iType, pos);
	
	while (!bioFound && bioMsgIDs)
		{
		GetBioMsgID(pos, msgID);
		for (TInt i = 0; !bioFound && i < bioMsgIDs->Count(); i++)
			{
			if (bioMsgIDs->At(i).iType == bioMessageData.iType)
				{
				switch(bioMessageData.iType)
					{
					case EBioMsgIdIana:
					case EBioMsgIdNbs:
						if (bioMsgIDs->At(i).iText.CompareF(bioMessageData.iText)==0)
						{
							rBioMsgUID = msgID;
							bioFound = ETrue;
						}
						break;
					case EBioMsgIdWap:
					case EBioMsgIdWapSecure:
					case EBioMsgIdWsp:
					case EBioMsgIdWspSecure:
						if (bioMsgIDs->At(i).iPort == bioMessageData.iPort)
						{
							rBioMsgUID = msgID;
							bioFound = ETrue;
						}
						break;
					case EBioMsgIdUnknown:
					default:
						break;
					}
				}
			}
		CleanupStack::PopAndDestroy();	// bioMsgIDs

		if (!bioFound)
			bioMsgIDs = BioEntryByTypeLC(ENext, bioMessageData.iType, pos);	
		} 
	

	if (!bioFound)
		return KErrNotFound;

	return KErrNone;
	}

