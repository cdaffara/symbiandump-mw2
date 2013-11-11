// Copyright (c) 2007-2010 Nokia Corporation and/or its subsidiary(-ies).
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
HEADER FILES
 */

#include "cmsvconverterwaiter.h"
#include "cmessageconvertermanager.h"

//const
const TUid KUidMsvIndexFile = {0x10003C6B};

static _LIT_SECURITY_POLICY_PASS(KAllowAllPolicy);
// literals
#if (defined SYMBIAN_MESSAGESTORE_UNIT_TESTCODE)
	_LIT(KMessagingDBName, "\\messaging.db");
#else
	_LIT(KMessagingDBName,"[1000484B]messaging.db");
#endif



// static class initialiser
CMsvConverterWaiter* CMsvConverterWaiter::iConverterWaiter = NULL;


/**
Instance()
The function returns already created instance of
this class to the caller. To create a new instance
the caller should call InstanceL(). 
@return CMsvConverterWaiter* : An instance of converter waiter.
*/ 
CMsvConverterWaiter* CMsvConverterWaiter::Instance()
	{
	return iConverterWaiter;
	}

/**
InstanceL()
Instatiates converter waiter object.It is responsible for initiating and completing 
message store conversion

@param aServer: Message Server Object
@return CMsvConverterWaiter* : An instance of converter waiter.
*/
CMsvConverterWaiter* CMsvConverterWaiter::InstanceL(CMsvServer* aServer)
	{
	if(iConverterWaiter == NULL)
		{
		CMsvConverterWaiter* self = new(ELeave)CMsvConverterWaiter(aServer);
		iConverterWaiter = self;
		}

	return iConverterWaiter;	
	}

/**
~CMsvConverterWaiter()
 
@param None
@return: None
@internalComponent
 */
CMsvConverterWaiter::~CMsvConverterWaiter()
	{
	iConverterWaiter = NULL;
	Cancel();
	iConverterQueue.Close();
	iConverterThread.Close();
	// Free the handle to the property
	iProperty.Close();
	}

/*
CMsvConverterWaiter()
Default constructor. Adds this active request into active scheduler queue.
   
@param aServer: messaging server
@param iActive: active object
@internalComponent
*/
CMsvConverterWaiter::CMsvConverterWaiter(CMsvServer* aServer)
: CActive(EPriorityLow),iServer(aServer)
	{ 
	CActiveScheduler::Add(this); 
	}

/**
DoCancel()
Stub Code

@param : None
@return: None
*/
void CMsvConverterWaiter::DoCancel()
	{
	}

/**
DefineProperty()
Define the property and attached it.

@param none.
@return void.
*/
TInt CMsvConverterWaiter::DefineProperty()
	{
	// Define the property and create a handle to it
	TInt err = RProperty::Define(KMyPropertyCat, KMyPropertyName,RProperty::EInt,KAllowAllPolicy,KAllowAllPolicy);
	if(!err)
		{
		iProperty.Attach(KMyPropertyCat,KMyPropertyName,EOwnerThread);
		}
	return err;
	}

/**
PublishProperty()
Publishing property.

@param aValue : value will published.
@return void.
*/
TInt CMsvConverterWaiter::PublishProperty(TInt aValue)
	{
	TInt err = iProperty.Set(aValue);	
	return err;
	}


/**
RunL()
Asynchronous request handler of an active object. 
Invoked when a notification is received from the converter thread.
Upon succesful conversion, this removes the index file , drive from the queue and closes 
the converter thread.
When the conversion is cancelled , it removes the partially converted database table fro headers
and completes the conversion request with KErrCancel.

@param None
@return: None
@internalComponent
*/
void CMsvConverterWaiter::RunL()
	{
	if(iStatus >= KErrNone)
		{
		//Update drive status for this drive. EMsvMessageStoreAvailableStatus
		iServer->UpdateDriveStatusL(TDriveUnit(iConverterQueue[0].iDriveNumber),EMsvMessageStoreAvailableStatus);
		
		//Close the existing handle to the thread
		
		
		//Complete this request
		Completed(KErrNone);
		iConverterThread.Close(); 
		}
	else if(iStatus == KErrCancel)	
		{
		// we would have published this property so we can safely delte it in case of cancel
		RProperty::Delete(KMyPropertyCat,KMyPropertyName);
				
		iConverterThread.Close(); 
		
		// The previous conversion request failed
		Completed(iStatus.Int());	
		}
	else
		{
		// The previous conversion request failed
		iConverterThread.Close(); 
		Completed(iStatus.Int());
		}

	// start conversion for the next drive in the queue
	if(iConverterQueue.Count())
		{
		StartMessageStoreConversionL(iConverterQueue[0].iMessage,ETrue);
		}
	}

/**
StartMessageStoreConversionL()
Starts the message store conversion.
Steps:
1. validates the drive.
2. Creates a RThread object for conversion.
3. Logons to the created thread to receive notofications.
4. Resumes the converter thread and starts message store conversion.
5. Marks this request as active. 

@param aMessage: RMessage2 representing client request and containing request data.
@param aQueuedRequest: Boolean flag specifying if the drive is queued.
@return: None
@internalComponent
*/
void CMsvConverterWaiter::StartMessageStoreConversionL(const RMessage2& aMessage,TBool aQueuedRequest)
	{
	_LIT(KConverterThread,"StartMessageStoreConversion");
	
	TDriveNumber driveNumber = (TDriveNumber) aMessage.Int0();
	TInt error = KErrNone;	
	
	//This is a new request, we need to validate this. 
	//Note : the drive is already validated if it is in the queue.
	if(!aQueuedRequest)
		{
		error = ValidateRequestedDriveL(driveNumber);		
		}
		
	if(error == KErrNone) // if proper
		{
		if(!aQueuedRequest) // if it is not already queued
			{
			TConversionQueue request;
			request.iDriveNumber = driveNumber;
			request.iMessage = aMessage;
			iConverterQueue.AppendL(request);
			}
	
		iDriveNumber = driveNumber;
		TAny* conversionStatus = &iDriveNumber;
	
		TInt err = iConverterThread.Create(KConverterThread, &CMessageConverterManager::StartConversion,KDefaultStackSize*8,KMinHeapSize,0x8000000,conversionStatus,EOwnerThread);
		
		iStatus = KRequestPending;
		if(!err)
			{
			iConverterThread.Logon(iStatus);
			iConverterThread.Resume();
			}
		// activate this request
		SetActive();
		}
	else
		{
		// complete this request with the right error
		aMessage.Complete(error);			
		}
		
	}

/**
QueueConversionRequestL()
Queues conversion request for a drive. A drive is queued for conversion when conversion is 
currently in progress for a drive.

@param aMessage: RMessage2 representing client request and containing request data.
@return: None
@internalComponent
*/
void CMsvConverterWaiter::QueueConversionRequestL(const RMessage2& aMessage)
	{
	TDriveNumber driveNumber = (TDriveNumber) aMessage.Int0();
	
	TInt pos = 0;
	TBool found = EFalse;	
	while(pos < iConverterQueue.Count())
		{
		if(iConverterQueue[pos++].iDriveNumber == driveNumber )// drive not in the queue
			{
			found = ETrue;
			}
		}
		
	if(found)
		{
		aMessage.Complete(KErrAlreadyExists);
		}
	else
		{
		TInt error = ValidateRequestedDriveL(driveNumber);
		if(error == KErrNone)
			{
			//validation fine. Add this request
			TConversionQueue request;
			request.iDriveNumber = driveNumber;
			request.iMessage = aMessage;
			iConverterQueue.AppendL(request);
			}
		else
			{
			aMessage.Complete(error);
			}
		}
	}

/**
ValidateRequestedDriveL()
Validates the message store in the requested drive.
It checks for the following conditions.
1) Drive is present in preferred drive list.
2) The drive is already present in conversion Queue.
3) Checks if the version 1 message store is present 
4) Identifies if version 0 message store is corrupt..

@param aDrive: Drive number.
@return: None
@internalComponent
*/
TInt CMsvConverterWaiter::ValidateRequestedDriveL(TDriveNumber aDrive)
	{
	// check if the drive is present in preferred drive list		
	TInt error = ValidateDriveInPreferredDriveListL(aDrive);
	if(error != KErrNone)
		{
		return error;
		}

	// Check if Version 1 message store exists. 
	TParse parse;
	TPtrC drive = TDriveUnit(aDrive).Name();
	parse.Set(KMessagingDBName, &drive, NULL);
	TFileName dbFileName = parse.FullName();
	
	// check version 1 message store
	RSqlDatabase temp;
	CleanupClosePushL(temp);
	
	error = temp.Open(dbFileName);
	
	temp.Close();
	CleanupStack::PopAndDestroy();  // temp
	
	//Version 1 message store may be corrupt
	if(error == KSqlErrCorrupt )
		{
		return error;
		}
	
	//version 1 does not exist, check version 0 is corrupt
	if(error == KErrNotFound) 
		{
		TBuf<KMaxPath> filePath;
		TPtrC drive(TDriveUnit(aDrive).Name());
		filePath.Append(drive);
		filePath.Append(KIndexFilePath);
		
		RFs ifsSession;
		User::LeaveIfError(ifsSession.Connect()); 
		
		RFile file;
		TInt error = file.Open(ifsSession, filePath, EFileShareAny|EFileWrite);
		
		if(error==KErrNone)
			{
			CPermanentFileStore* iIndexStore = CPermanentFileStore::FromL(file);
			CleanupStack::PushL(iIndexStore);
			if (iIndexStore->Type() != TUidType(KPermanentFileStoreLayoutUid, KUidMsvIndexFile))
				{
				User::Leave(KErrCorrupt);	
				}
			CleanupStack::PopAndDestroy(); // iIndexStore
			}
		ifsSession.Close();
		file.Close();
		}
	return KErrNone;
	}

/**
isRunningMessageStoreConverter()
Checks if converter waiter is active and waiting for completion of a conversion request. 
	
@param None
@return: Boolean flag specifying if the conversion is in progress,i,e if this active object is busy.
@internalComponent
*/
TBool CMsvConverterWaiter::isRunningMessageStoreConverter()
	{
	if( iStatus == KRequestPending )
		{
		return ETrue;
		}
	else
		{
		return EFalse;
		}
	}

/**
CancelConversionL()
Cancels conversion for a drive.
Steps to cancel.
1. Stop converter Thread.
2. Remove the partially converted header table.
3. Complete the asynchronous request for conversion of the drive.
4. Remove The drive from the drive array.
5. If more drives are found in the queue, than initiate conversion for the next drive.

@param aMessage: RMessage2 representing client request and containing request data.
@return None
@internalComponent
*/	
void CMsvConverterWaiter::CancelConversionL(const RMessage2& aMessage)
	{
	//Get the drive
	TDriveNumber driveNumber = (TDriveNumber) aMessage.Int0();
	
	TInt pos = 0;
	TBool found = EFalse;
	while(pos < iConverterQueue.Count())
		{
		if(iConverterQueue[pos].iDriveNumber == driveNumber)
			{
			found = ETrue;
			break;
			}
		else
			{
			pos++;
			}
		}
		
	if(pos == 0 && found)// this is the active request
		{
		// Define cancel property 
		TInt err = DefineProperty();	
		if(err == KErrNone)
			{
			// published the property so that, conversion thread will releases the memory and return.
			PublishProperty(KErrCancel);
			aMessage.Complete(KErrNone);
			}
		else
			{
			aMessage.Complete(err);
			}
		}
	else if(found)// this drive is queued
		{
		aMessage.Complete(KErrNone);
		iConverterQueue[pos].iMessage.Complete(KErrCancel);
		iConverterQueue.Remove(pos);
		}
	else// invalid rive for cancellation
		{
		aMessage.Complete(KErrNotFound);
		}
	}

/**
GetConversionStatus()
Returns conversion status for a drive.

@param aMessage: RMessage handle.
@return None
@internalComponent
*/	
void CMsvConverterWaiter::GetConversionStatus(const RMessage2& aMessage)
	{
	TDriveNumber driveNumber = (TDriveNumber) aMessage.Int0();
	TInt pos = 0;
	TBool found = EFalse;
	while(pos < iConverterQueue.Count())
		{
		if(iConverterQueue[pos++].iDriveNumber == driveNumber)
			{
			found = ETrue;
			}
		}
	if(found)
		{
		if (pos == 0)
			{
			aMessage.Complete(0); // This request is active
			}
		else
			{
			aMessage.Complete(1); // Drive is queued for conversion
			}
		}
	else
		{
		aMessage.Complete(-1);	// drive not found
		}
	}
	

/**
Completed()
Completes the asynchronous request for message store conversion for the completed drive.
When conversion completes successfully, it gets notified of the completion. 
The active conversion is alwasys the first element on the conversion queue.
Upon compeltion, the first element in the queue gets compeleted.and the drive is removed
from the drive array.

@param aStatus: MTM ID of the table.
@return None
@internalComponent
*/	
void CMsvConverterWaiter::Completed(const TInt aStatus)
	{
	iConverterQueue[0].iMessage.Complete(aStatus);
	iConverterQueue.Remove(0);
	
	}

/**
ValidateDriveInPreferredDriveListL()
Validates a drive by checking if it is present in preferred drive list and its ststus is
EMsvMessageStoreNotSupportedStatus
Also checks if the drive is already queued for conversion and as such is a duplicate request.

@param aDrive: Drive number.
@return None
@internalComponent
*/
TInt CMsvConverterWaiter::ValidateDriveInPreferredDriveListL(TDriveNumber aDrive)
	{
	CMsvPreferredDriveList *driveList = CMsvPreferredDriveList::GetDriveList();
	RArray<TDriveNumber> driveNumList;
	CleanupClosePushL(driveNumList);
	TInt index;
	for(index=0; index<driveList->Count(); index++)
		{
		if(EMsvMessageStoreNotSupportedStatus == (*driveList)[index].status)
			{
			driveNumList.AppendL((*driveList)[index].driveNum);		
			}
		}
	
	if( driveNumList.Find(aDrive) != KErrNotFound)
		{
		CleanupStack::PopAndDestroy();  // driveNumList
		return KErrNone;
		}
	CleanupStack::PopAndDestroy();  // driveNumList
	return KErrNotFound;
	}
