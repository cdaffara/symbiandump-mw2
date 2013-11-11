// MSVPREFERREDDRIVELIST.H

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

#ifndef __MSVPREFFEREDDRIVELIST_H__
#define __MSVPREFFEREDDRIVELIST_H__

#include <f32file.h>

// One of the possible state of a drive
// in the preferred drive list.
enum TDriveState
	{
	EMsvMessageStoreAvailableStatus = 0,		// Media has valid message store.
	EMsvMessageStoreUnavailableStatus,			// Media does not have message store.
	EMsvMessageStoreNotSupportedStatus,			// Media has older version of message store.
	EMsvDriveDiskNotAvailableStatus,			// Drive does not have media.
	EMsvMessageStoreCorruptStatus,				// The message store in the media is corrupt.
	EMsvInvalidDriveStatus						// Initial status of the drive.
	};

	
// Each element in the preferred drive list.
struct TMsvPreferredDrive
	{
	TDriveNumber driveNum;	// Range 0-25.
	TUint driveId;			// Range 1-7, 999.
	TDriveState status;
	};


/**
 * List of drives, also called preferred drive list.
 * Contains associated methode which work on the drive list.
 
@internalComponent
@prototype
*/
NONSHARABLE_CLASS (CMsvPreferredDriveList): public CBase
	{
public:
	static CMsvPreferredDriveList* GetDriveList();
	
	TInt GetDriveNumber(TUint aDriveId, TDriveNumber& aDriveNum);
	void DriveInfoL(TUint aIndex, TMsvPreferredDrive& aDriveInfo) const;
	inline TMsvPreferredDrive& operator[](TInt aIndex);
	inline TMsvPreferredDrive operator[](TInt aIndex) const;
	inline TInt Count();
	inline TUint CurrentDriveIndex();
	inline TDriveNumber CurrentDriveNumber();
	TInt GetDriveIndex(const TDriveNumber aDriveNum, TUint& aDriveIndex);

#if (defined SYMBIAN_MESSAGESTORE_UNIT_TESTCODE)
	static void Destroy(CMsvPreferredDriveList*& aDriveList);
#ifdef _DEBUG
	void Print();
#endif
#endif

private:
	static CMsvPreferredDriveList* CreateL();
	CMsvPreferredDriveList();
	void ConstructL();
	~CMsvPreferredDriveList();
	
	void Insert(TMsvPreferredDrive aDrive, TInt aIndex);
	void Remove(TInt aIndex);
	void UpdateDriveIdL(TUint aIndex, TUint aDriveId);
	void UpdateDriveStatusL(TUint aIndex, TDriveState aStatus);
	
	inline void SetCurrentDriveIndex(TInt aIndex);
	
private:
	static CMsvPreferredDriveList* iMsvPreferredDriveList;
	RArray<TMsvPreferredDrive>* iDrives;
	TUint iCurDriveIndex;

	// Friend classes - only these can create, modify
	// and destroy the preferred drive list.
	friend class CMsvServer;
	friend class CMsvIndexContext;
	friend class CMsvIndexAdapter;
	friend class CMsvBackupHandler;

#if (defined SYMBIAN_MESSAGESTORE_UNIT_TESTCODE)
	friend class CTestIndexAdapter;
	friend class CTestIndexContext;
	friend class CTestDbAdapter;
	friend class CTestMsvServer;
	friend class CTestMsvPreferredDriveList;
	friend class CTestMsvSession;
	friend class CTestVisibleFolder;
	friend class CTestEntryFreePool;
	friend class CTestMessageDbAdapter;
#endif
		
	};
	
	
	
inline TInt CMsvPreferredDriveList::Count()
	{
	return iDrives->Count();
	}
	
inline TMsvPreferredDrive& CMsvPreferredDriveList::operator[](TInt aIndex)
	{
	return (*iDrives)[aIndex];
	}

inline TMsvPreferredDrive CMsvPreferredDriveList::operator[](TInt aIndex) const
	{
	return (*iDrives)[aIndex];
	}
	
inline void CMsvPreferredDriveList::SetCurrentDriveIndex(TInt aIndex)
	{
	if(aIndex > Count())
		{
		return;
		}
	iCurDriveIndex = aIndex;
	}

inline TUint CMsvPreferredDriveList::CurrentDriveIndex()
	{
	return iCurDriveIndex;
	}
	
inline TDriveNumber CMsvPreferredDriveList::CurrentDriveNumber()
	{
	return (*iDrives)[iCurDriveIndex].driveNum;	
	}
	
#endif //__MSVPREFFEREDDRIVELIST_H__
