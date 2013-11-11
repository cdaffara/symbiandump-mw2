// Copyright (c) 2008-2009 Nokia Corporation and/or its subsidiary(-ies).
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


/**
 * HEADER FILES
 */

#include <s32fileiter.h>
#include "cmsvversion0version1converter.h"


// Literals
const TInt KMsvLongestStringAllowed=KMaxTInt32; 

/*
CMsvVersion0Version1Converter()
Default Constructor
 
@param aDBAdapter: CMsvDbAdapter object reference.
@param aDriveNum: Drive Number
@return None
*/	
CMsvVersion0Version1Converter::CMsvVersion0Version1Converter(CMsvDBAdapter& aDBAdapter,TDriveNumber aDriveNum,TBool aConversionResume)
: iDBAdapter(aDBAdapter),iDrive(aDriveNum),iResume(aConversionResume)
	{
	}

/*
NewL()
Constructs and returns CMsvVersion0Version1Converter object

@param aFileStore:  A reference to store object. This supports full manipulation of store contents.
@param aDBAapter: A handle to db ada[ter object
@param aDriveNum: The Drive number of the drive to be converted.
@return : CMsvVersion0Version1Converter object
*/
CMsvVersion0Version1Converter* CMsvVersion0Version1Converter::NewL(CMsvDBAdapter& aDBAapter,TDriveNumber aDriveNum,TBool aConversionResume)
	{
	CMsvVersion0Version1Converter* self = new(ELeave) CMsvVersion0Version1Converter(aDBAapter,aDriveNum,aConversionResume);
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop(self);
	return self;
	}

/*
~CMsvVersion0Version1Converter()
Destructor

@param : None 
@return: None
*/
CMsvVersion0Version1Converter::~CMsvVersion0Version1Converter()
	{
	delete iStream;
	delete iDescriptionBuffer;
	delete iDetailsBuffer;
	}	

/*
ConstructL()
Second phase constructor.

@param aDriveNum: Drive to be converted.
@param aConversionResume: Boolean flag indicating if conversion is to be resumed.
@return None
@internalComponent
*/	
void CMsvVersion0Version1Converter::ConstructL()
	{
	// create the stream index
	iStream=new (ELeave) CArrayFixFlat<TStreamId>(KMsvStreamIndexGranularity);
	}

/*
ConvertMessageStoreL()
Starts conversion of version 0 to version 1 message store.
If the conversion is to be resumed, it reads the resume status from conversion status table and starts reading entries from their.
For a new conversion request, it creates the conversion status entry in the status table.

@param :None
@return: None
@internalComponent
*/
void CMsvVersion0Version1Converter::ConvertMessageStoreL()
	{
	RFs fs;
	User::LeaveIfError(fs.Connect()); 
	CleanupClosePushL(fs);
	
	TBuf<KMaxPath> filePath;
	TPtrC drive = TDriveUnit(iDrive).Name();
	filePath.Append(drive);
	filePath.Append(KIndexFilePath);
		
	RFile file;
	TInt error = file.Open(fs, filePath, EFileShareAny|EFileWrite);
	
	iIndexStore = CPermanentFileStore::FromL(file);
	CleanupStack::PushL(iIndexStore);
			
	RestoreStreamIndexL();

	// Check for ConversionStatus table
	if(iResume)
		{
		iDBAdapter.GetConversionStatusL(iResumeStatus);
		iEntryStreamIndex = iStream->Count() - iResumeStatus.remainingCount;
		}
	else
		{
		iEntryStreamIndex += 2;

		iResumeStatus.lastId = 0;
		iResumeStatus.mtmId.iUid = 0;
		iResumeStatus.serviceId = 0;
		iResumeStatus.sourceVersion = 0;
		iResumeStatus.targetVersion = 1;
		iResumeStatus.remainingCount = iStream->Count() - 1;
		iDBAdapter.AddConversionStatusEntryL(iResumeStatus);
		iResumeStatus.remainingCount --;
		}

	// read TMsvEntries now
	while(ReadNextEntriesL())
		{
		}
		
	// update remaining visible parent id's
	UpdateVisibleParentL();
	// we are finished with v0-v1 conversion. Lets remove the status entry from resumestatus table
	iDBAdapter.RemoveConversionStatusEntriesL();

	CleanupStack::PopAndDestroy(2);  //iIndexStore,fs
	file.Close();
	fs.Close();
	}
	
/*
GetPropertyValue()
Gets the value of RProperty object. This value is used to check if cancel request is 
issued for the store conversion in progress

@param None
@return: Value of the property
@internalComponent
*/

TInt CMsvVersion0Version1Converter::GetPropertyValue()
	{
	TInt value;
	TInt err = RProperty::Get(KMyPropertyCat,KMyPropertyName, value);
	return value;
	}
/*
RestoreStreamIndexL()
Restores all the stream index id's (id of TMveEntry) by reading them from the index file store.
These ids are than stored in an array and for each of them, its TMsvEntry restored from the file store.

@param None
@return: None
@internalComponent
*/
void CMsvVersion0Version1Converter::RestoreStreamIndexL()
	{
	// Create a stream iterator
	RPermanentFileStoreIter iter;
	iter.ResetLC(*iIndexStore);

	// Build the new stream index
	TStreamId id;
	while( (id = iter.NextL()) != KNullStreamIdValue )
		{
		iStream->AppendL(id);
		}
	CleanupStack::PopAndDestroy(&iter);
	}


/*
ReadNextEntriesL()
Prepares the index that determines how many entries will be read in each iteration
It than reads the index file on the initial and final range specified.

@param None
@return: None
@internalComponent
*/
TBool CMsvVersion0Version1Converter::ReadNextEntriesL()
	{
	if (iEntryStreamIndex<iStream->Count())
		{
		TInt nextIndex = Min(iEntryStreamIndex+KIndexReadGranularity, iStream->Count());
		PopulateDatabaseL(iEntryStreamIndex, nextIndex);
		iEntryStreamIndex = nextIndex;
		return ETrue;
		}
	return EFalse;
	}

/*
PopulateDatabaseL()
Reads each entry information from the file stream and stores this in the database table.
Also updates the conversion status table entry for each iteration = 10.

@param aFirstEntry: Initial index
@param aNextEntry: Final index
@return:None
@internalComponent
*/
void CMsvVersion0Version1Converter::PopulateDatabaseL(TInt aFirstEntry, TInt aNextEntry)
	{
	iDBAdapter.BeginTransactionL();
	
	TInt i=aFirstEntry;
	while (i<aNextEntry)
		{
		RStoreReadStream readStream;
		readStream.OpenLC(*iIndexStore, iStream->At(i++));

		InternalizeL(readStream);		
		
		TMsvId visParent = -99;
		// can we find parents visFlag. 
		TBool parentVisFlag = iDBAdapter.GetParentVisibleFlagL(iEntry.Parent());	
					
		if(parentVisFlag)
			{
			if( (iEntry.Visible()) && 
		    (iEntry.iType == KUidMsvFolderEntry || iEntry.iType == KUidMsvServiceEntry)
		      )
				{
				iEntry.SetVisibleFolderFlag(ETrue);
				}
			else
				{
				iEntry.SetVisibleFolderFlag(EFalse);
				}
			iDBAdapter.GetVisibleParentIdL(iEntry.Parent(),visParent);				
			}
	
		
		//create the db entry
		iDBAdapter.CreateIndexEntryL(iEntry,visParent);
		CleanupStack::PopAndDestroy(); //  readStream
		}
	
	//check if we have a request for cancellation
	TInt cancel = GetPropertyValue();
	if(cancel == KErrCancel	)
		{
		User::Leave(KErrCancel);
		}
	// commit for this iteration
	iResumeStatus.lastId = iEntry.iId;
	iResumeStatus.remainingCount -= 10;
	iDBAdapter.UpdateConversionStatusEntryL(iResumeStatus);
	iDBAdapter.CommitTransactionL();
	
	}

/*
InternalizeL()
Reads TMsvEntry from the stream.

@param aStream: File stream 
@return: None
@internalComponent
*/
void CMsvVersion0Version1Converter::InternalizeL(RReadStream& aStream)
	{
	// NOTE: This has been added in case versioning is required in the future
	TUint8 ver;
	aStream >> ver;

	aStream >> iEntry;
	TInt dummy = aStream.ReadUint32L();
	dummy  =  aStream.ReadUint32L();

	// Get last change and creation dates
	aStream >> iLastChangeDays;
	aStream >> iLastChangeMinutes;
	aStream >> iCreatedDays;
	aStream >> iCreatedMinutes;

	HBufC* des = HBufC::NewL(aStream, KMsvLongestStringAllowed);
	delete iDescriptionBuffer;
	iDescriptionBuffer = des;

	HBufC* det = HBufC::NewL(aStream, KMsvLongestStringAllowed);
	delete iDetailsBuffer;
	iDetailsBuffer = det;

	iEntry.iDescription.Set(*iDescriptionBuffer);
	iEntry.iDetails.Set(*iDetailsBuffer);
	}
	
/*
UpdateVisibleParentL()
Update ids whose visible parent was not set previously

@param None: 
@return: None
@internalComponent
*/
void CMsvVersion0Version1Converter::UpdateVisibleParentL()
	{
				
	TMsvId visibleParent;;
	RArray<TMsvId> idArray;
	CleanupClosePushL(idArray);
	
	iDBAdapter.GetRemainingIdsL(idArray);
	iDBAdapter.BeginTransactionL();
	
	for(TInt i = 0; i<idArray.Count(); i++)
		{
		iDBAdapter.GetVisibleParentL(idArray[i],visibleParent);
		iDBAdapter.UpdateVisibleFolderIdL(idArray[i],visibleParent);
		
		if (i % 50 == 0 && i!=0)
			{
			iDBAdapter.CommitTransactionL();	
			iDBAdapter.BeginTransactionL();
			}
		}
	
	iDBAdapter.CommitTransactionL();	
	CleanupStack::PopAndDestroy();  //idArray
	}

