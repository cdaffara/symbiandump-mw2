// Copyright (c) 1998-2009 Nokia Corporation and/or its subsidiary(-ies).
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

#include "MSVUTILS.H"
#include "MSVUIDS.H"
#include "MSVIDS.H"
#include "MSVPANIC.H"

#if (defined SYMBIAN_MSGS_ENHANCED_REMOVABLE_MEDIA_SUPPORT)
	// Code change for PREQ 557.
#include "msvpreferreddrivelist.h"
#endif		// #if (defined SYMBIAN_MSGS_ENHANCED_REMOVABLE_MEDIA_SUPPORT)
	

// static 
EXPORT_C TBool MsvUtils::ValidEntry(const TMsvEntry& aEntry)
//
// Returns true if the contents of the entry confrom to policy
// 
	{
	return ValidEntry(aEntry, EFalse);
	}

TBool MsvUtils::ValidEntry(const TMsvEntry& aEntry, TBool aCreate)
//
// Returns true if the contents of the entry confrom to policy
// 
	{
	if ((aEntry.iType!=KUidMsvNullEntry) && (aEntry.iMtm!=KUidMsvNullEntry) && (aEntry.Parent()!=KMsvNullIndexEntryId))
		{
		// it has a type, MTM and parent defined
		if (aEntry.iType!=KUidMsvServiceEntry)
			{
			// a non-service entry cannot be owned by root and must have a service id
			if (aEntry.Parent()!=KMsvRootIndexEntryId && aEntry.iServiceId!=KMsvNullIndexEntryId)
				return ETrue;
			}
		else 
			{
			// a service entry must be owned by root and has to have the service id pointed to itself
			// unless the entry is being created in which case we don't know the Id yet
			if (aEntry.Parent()==KMsvRootIndexEntryId && (aCreate || aEntry.iServiceId==aEntry.Id()))
				{
				return ETrue;
				}
			}
		}
	return EFalse;
	}

// static
EXPORT_C void MsvUtils::ConstructEntryName(TMsvId aService, TMsvId aEntry, TDes& aName, TNameMode aMode)
//
// Construct the name of the store, or the directory for binary files
// The aName should already be seeded with the main message folder.
//
	{
#if (defined SYMBIAN_MSGS_ENHANCED_REMOVABLE_MEDIA_SUPPORT)
	// Code change for PREQ 557.
	// Unmask the TMsvIds supplied.
	TInt driveId = GetDriveId(aEntry);
#ifdef SYMBIAN_MESSAGESTORE_UNIT_TESTCODE
	TInt drive = 0;
	if(0 == driveId)
		drive = TDriveUnit(aName.Left(1));
#endif
	aService = UnmaskTMsvId(aService);
	aEntry = UnmaskTMsvId(aEntry);
#endif
	
	__ASSERT_DEBUG(aEntry!=KMsvRootIndexEntryId||aMode==EStore, PanicServer(EMsvAskingForRootFolderOrPath));
	__ASSERT_DEBUG(aEntry!=aService || aMode!=EFolder || aService!=KMsvRootIndexEntryId, PanicServer(EMsvAskingForRootFolder));
	__ASSERT_DEBUG(aMode!=EPath || (aEntry==aService && aService!=KMsvRootIndexEntryId), PanicServer(EMsvPathAsForNonService));

	TBuf<KFileNameFixedWidth> dir;
	
	dir.NumFixedWidth(aService, EHex, KFileNameFixedWidth);

	// Change required for Preq557
#if (defined SYMBIAN_MSGS_ENHANCED_REMOVABLE_MEDIA_SUPPORT)
	// Delete the drive letter from the path as the drive can now be any 
	// other drive unlike C: earlier.
	// We can get the correct drive information from aEntry.
	TInt colonPos = aName.Locate(':');
	aName.Delete(0, colonPos + 1);
	
	// Add the appropriate drive letter.
	TDriveNumber driveNum;
#ifdef SYMBIAN_MESSAGESTORE_UNIT_TESTCODE
	if(0 == driveId)
		{
		driveNum = TDriveNumber(drive);
		}
	else
		{
		CMsvPreferredDriveList::GetDriveList()->GetDriveNumber(driveId, driveNum);
		}
#else
	CMsvPreferredDriveList::GetDriveList()->GetDriveNumber(driveId, driveNum);
#endif		

	aName.Insert(0, TDriveUnit(driveNum).Name());
#endif		// #if (defined SYMBIAN_MSGS_ENHANCED_REMOVABLE_MEDIA_SUPPORT)
	
	aName.Append(dir);

	// root or service entry
	if (aService==aEntry) // Both are unmasked, so this comparison is safe.
		{
		if (aMode==EPath)
			// path - ...\[aService]_S\.
			aName.Append(KMsvDirectoryExt);
		else if (aMode==EFolder)
			// folder - ...\[aService]_F\.
			aName.Append(KMsvBinaryFolderExt);
		// else
			// store - ...\[aService]
		return;
		}

	aName.Append(KMsvDirectoryExt);
	// service path - ...\[aService]_S\.
	if (aMode==EPath)
		return;

	_LIT(KDirSep,"\\");
	dir.NumFixedWidth(aEntry&0xf, EHex,1);
	aName.Append(dir);
	aName.Append(KDirSep);
	dir.NumFixedWidth(aEntry, EHex, KFileNameFixedWidth);
	aName.Append(dir);

	
	// entry store - ..\[aService]_S\[aEntry]
	if (aMode==EStore)
		return;

	aName.Append(KMsvBinaryFolderExt);
	// entry folder - ..\[aService]_S\[aEntry]_F
	

	}

// static
EXPORT_C TInt MsvUtils::HasDirectory(const RFs& aFs, const TDesC& aMessageFolder, TMsvId aService, TMsvId aEntry)
	{
	TFileName filename(aMessageFolder);
	ConstructEntryName(aService, aEntry, filename, EFolder);

	TUint attributes;
	TInt err = aFs.Att(filename, attributes);

	if (err == KErrNotFound || err == KErrPathNotFound)
		return 0;
	else if (err == KErrNone)
		return 1;
	
	return err;
	}

// static
EXPORT_C TInt MsvUtils::HasStore(const RFs& aFs, const TDesC& aMessageFolder, TMsvId aService, TMsvId aEntry)
	{
	TFileName filename(aMessageFolder);
	ConstructEntryName(aService, aEntry, filename, EStore);

	TUint attributes;
	TInt err = aFs.Att(filename, attributes);

	if (err == KErrNotFound || err == KErrPathNotFound)
		return 0;
	else if (err == KErrNone)
		return 1;
	
	return err;
	}

// static
EXPORT_C void MsvUtils::AddPathAndExtensionToResFileL(TFileName& aFileName)
//	Uses the following rules to make sure aFileName has a path and extension:
//	1. If aFileName has no extension add ".rsc"
//	2. If aFileName does have an extension then replace it with ".rsc"
//	3. If aFileName contains a path leave it
//	4. If aFileName contains no path prepend "z:\system\data\"

//	@param aFileName file name to be altered by this function
//	@leave system-wide error codes 
	{
	_LIT(KFileExtension, ".rsc");
	_LIT(KFileLocation, "z:\\resource\\messaging\\");

	TParse parser;
	User::LeaveIfError(parser.Set(KFileExtension, &aFileName, &KFileLocation));


	aFileName = parser.FullName();
	}


#if (defined SYMBIAN_MSGS_ENHANCED_REMOVABLE_MEDIA_SUPPORT)
//Code added for Preq557
//static
TMsvId MsvUtils::UnmaskTMsvId(TMsvId aMaskedId)
	{
	return (aMaskedId & KDriveMask);
	}

	
//static
TUint MsvUtils::GetDriveId(TMsvId aMaskedId)
	{
	TUint driveId = ( (aMaskedId & ~KDriveMask) >> 28);
	if(0 == driveId)
		{
	        return KCurrentDriveId;
		}
	return driveId;
	}


//static	
TBool MsvUtils::IsStandardId(TMsvId aId)
	{
	if(UnmaskTMsvId(aId) < KMsvUnkownServiceIndexEntryIdValue &&
	   UnmaskTMsvId(aId) > KMsvLocalServiceIndexEntryIdValue)
		{
		return ETrue;
		}
	return EFalse;
	}


//static
TMsvId MsvUtils::MaskTMsvId(TUint aDriveId, TMsvId aUnmaskedId)
	{
	return (UnmaskTMsvId(aUnmaskedId) | (aDriveId << 28));
	}
	
	
#endif
