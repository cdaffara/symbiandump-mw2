// MSVPREFERREDDRIVELIST.CPP

// Copyright (c) 2008-2010 Nokia Corporation and/or its subsidiary(-ies).
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
// USER INCLUDES
// 
//

#include "msvpreferreddrivelist.h"
#include "MSVPANIC.H"
#ifdef _DEBUG
#include <flogger.h>
#endif

CMsvPreferredDriveList* CMsvPreferredDriveList::iMsvPreferredDriveList = NULL;


/**
 * FUNCTION DEFINITIONS
 */


/**
 * CreateL()
 * @return CMsvPreferredDriveList*: The newly created CMsvPreferredDriveList object.
 * 
 * Function added as part of PREQ 557.
 * It returns an instance of CMsvPreferredDriveList class. 
 */
CMsvPreferredDriveList* CMsvPreferredDriveList::CreateL()
	{
	CMsvPreferredDriveList* self = new(ELeave)CMsvPreferredDriveList;
	CleanupStack::PushL(self);
	
	self->ConstructL();
	iMsvPreferredDriveList = self;
	
	CleanupStack::Pop(self);
	return self;
	}



/**
 * GetDriveList()
 */
CMsvPreferredDriveList* CMsvPreferredDriveList::GetDriveList()
	{
	__ASSERT_DEBUG(iMsvPreferredDriveList!=NULL, PanicServer(EMsvPreferredDriveListNotCreated));
	return iMsvPreferredDriveList;
	}
	

	
/**
 * CMsvPreferredDriveList()
 *
 * Default constructor.
 */
CMsvPreferredDriveList::CMsvPreferredDriveList()
	{
	
	}



/**
 * ~CMsvPreferredDriveList()
 *
 * Destructor.
 */
CMsvPreferredDriveList::~CMsvPreferredDriveList()
	{
	iDrives->Close();
	delete iDrives;
	iDrives = NULL;
	
	iMsvPreferredDriveList = NULL;
	}
	
	
	
/**
 * ConstructL()
 * @param None.
 * @return None.
 *
 * Second phase constructor.
 */
void CMsvPreferredDriveList::ConstructL()
	{
	iDrives = new(ELeave)RArray<TMsvPreferredDrive>;
	}
	


/**
 * GetDriveNumber()
 * @param TUint: Drive Id of the drive.
 * @return TInt: Drive number of the drive in the preferred drive list.
 *				 KErrNotFound if drive does not exist.
 *
 * Gets the drive number of the drive in the preferred drive list.
 */
TInt CMsvPreferredDriveList::GetDriveNumber(TUint aDriveId, TDriveNumber& aDriveNum)
	{
	if(0 == aDriveId)
		{
		aDriveNum = (*iDrives)[iCurDriveIndex].driveNum;
		return KErrNone;
		}
	
	TInt driveCount = iDrives->Count();
	for(TInt index = 0; index < driveCount; ++index)
		{
		if(aDriveId == (*iDrives)[index].driveId)
			{
			aDriveNum = (*iDrives)[index].driveNum;
			return KErrNone;
			}
		}
	return KErrNotFound;
	}



/**
 * Insert()
 * @param TMsvPreferredDrive: The drive to be inserted into the preferred
 *							  drive list.
 * @param TInt: Position of the drive in the preferred drive list.
 * @return None.
 *
 * Inserts a drive at the specified position in the preferred drive list.
 */
void CMsvPreferredDriveList::Insert(TMsvPreferredDrive aDrive, TInt aIndex)
	{
	if(aIndex <= iCurDriveIndex)
		{
		++iCurDriveIndex;
		}
	iDrives->Insert(aDrive, aIndex);
	}
	
	
	
/**
 * Remove()
 * @param TInt: Position of the drive in the preferred drive list.
 * @return None.
 *
 * Removes a drive from a specified position in the preferred drive list.
 */
void CMsvPreferredDriveList::Remove(TInt aIndex)
	{
	if(aIndex <= iCurDriveIndex && 0 < iCurDriveIndex)
		{
		--iCurDriveIndex;
		}
	iDrives->Remove(aIndex);
	}



/**
 * DriveInfoL()
 * @param TUint: Index of the drive in the preferred drive list.
 * @param TMsvPreferredDrive&: Output parameter for drive info object.
 * @return None.
 */
void CMsvPreferredDriveList::DriveInfoL(TUint aIndex, TMsvPreferredDrive& aDriveInfo) const
	{
	if(aIndex >= iDrives->Count())
		{
		User::Leave(KErrNotFound);
		}
	aDriveInfo = (*iDrives)[aIndex];	
	}
	


/**
 * UpdateDriveId()
 * @param TUint: Index of the drive in the preferred drive list.
 * @param TUint: New drive Id.
 * @return TInt: KErrNotFound, if drive entry does not exist.
 */	
void CMsvPreferredDriveList::UpdateDriveIdL(TUint aIndex, TUint aDriveId)
	{
	if(aIndex >= iDrives->Count())
		{
		User::Leave(KErrNotFound);
		}
	(*iDrives)[aIndex].driveId = aDriveId;
	}
	
	

/**
 * UpdateDriveStatus()
 * @param TUint: Index of the drive in the preferred drive list.
 * @param TUint: New drive status.
 * @return TInt: KErrNotFound, if drive entry does not exist.
 */	
void CMsvPreferredDriveList::UpdateDriveStatusL(TUint aIndex, TDriveState aStatus)
	{
	if(aIndex >= iDrives->Count())
		{
		User::Leave(KErrNotFound);
		}
	(*iDrives)[aIndex].status = aStatus;
	}



// Returns index of the drive in the drive list. For test purpose only.
TInt CMsvPreferredDriveList::GetDriveIndex(const TDriveNumber aDriveNum, TUint& aDriveIndex)
	{
	TInt count = Count();
	for(TInt index = 0; index < count; ++index)
		{
		if(aDriveNum == (*iDrives)[index].driveNum)
			{
			aDriveIndex = index;
			return KErrNone;
			}
		}
	return KErrNotFound;
	}
	

#if (defined SYMBIAN_MESSAGESTORE_UNIT_TESTCODE)
#ifdef _DEBUG
// Destroys the preferred drive list.
void CMsvPreferredDriveList::Destroy(CMsvPreferredDriveList*& aDriveList)
	{
	if(NULL != aDriveList)
		{
		delete aDriveList;
		aDriveList = NULL;
		}
	}
void CMsvPreferredDriveList::Print()
	{
	_LIT8(KLtBracket, "[");
	_LIT8(KRtBracket, "]: DRIVE-NUM: ");
	_LIT8(KDriveId, " DRIVE-ID: ");
	_LIT8(KStatus, " STATUS: ");
	_LIT8(KAvailable, "EMsvMessageStoreAvailableStatus");
	_LIT8(KUnavailable, "EMsvMessageStoreUnavailableStatus");
	_LIT8(KNotSupported, "EMsvMessageStoreNotSupportedStatus");
	_LIT8(KDiskNotAvailable, "EMsvDriveDiskNotAvailableStatus");
	_LIT8(KCorruptStore, "EMsvMessageStoreCorruptStatus");
	_LIT8(KInvalid, "EMsvInvalidDriveStatus");
		
	RFileLogger logger;
	if (logger.Connect() == KErrNone)
		{
		logger.CreateLog(_L("msgs"), _L("DriveList.txt"), EFileLoggingModeAppend);
		logger.SetDateAndTime(EFalse, EFalse);
		logger.Write(_L(" Preferred Drive List:"));
		logger.Write(_L("--------------------------------"));
		logger.Write(_L(""));
		}
	
	TInt count = Count();
	for(TInt index = 0; index < count; ++index)
		{		
		RBuf8 text;
		text.CreateL(100);
		text.Append(KLtBracket);
		text.AppendNum(index);
		text.Append(KRtBracket);
		text.AppendNum((*iDrives)[index].driveNum);
		text.Append(KDriveId);
		text.AppendNum((*iDrives)[index].driveId);
		text.Append(KStatus);
		switch((*iDrives)[index].status)
			{	
			case EMsvMessageStoreAvailableStatus:
				text.Append(KAvailable);
				break;
			case EMsvMessageStoreUnavailableStatus:
				text.Append(KUnavailable);
				break;
			case EMsvMessageStoreNotSupportedStatus:
				text.Append(KNotSupported);
				break;
			case EMsvDriveDiskNotAvailableStatus:
				text.Append(KDiskNotAvailable);
				break;
			case EMsvMessageStoreCorruptStatus:
				text.Append(KCorruptStore);
				break;
			case EMsvInvalidDriveStatus:
				text.Append(KInvalid);
				break;
			}
				
		logger.Write(text);
		text.Close();		
		logger.Write(_L(""));
		}
	logger.CloseLog();
	logger.Close();
	}
	
	
#endif
#endif
