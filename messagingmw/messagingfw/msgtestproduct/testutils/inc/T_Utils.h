/**
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
* This is the header file which contains the common utility functions
* 
*
*/



/**
 @file
*/


#ifndef __T_UTILS_H__
#define __T_UTILS_H__


// Epoc includes
#include <msvapi.h>

// Forward Declaration
class RResourceFile;

/**
Implements a class to contain the utility functions required across servers
*/
class CT_MsgUtils
	{	
public:
	IMPORT_C CT_MsgUtils();
	~CT_MsgUtils();

	// Returns the EntryID of the entry based on the entry name
	IMPORT_C static TMsvId FindEntryByNameL(CMsvSession* aSession, TMsvId aParentId, TDesC& aEntryName, TBool aCaseSensitive = EFalse);
	 
	// Returns the EntryID of the entry based on the entry name and entry parent name
	IMPORT_C static TMsvId FindEntryByNameL(CMsvSession* aSession, TMsvId aParentId, TDesC& aEntryName, TDesC& aParentEntryName, TBool aCaseSensitive = EFalse);
	
	// Returns the EntryId of the entry based on the entry name under the given parent entry
	IMPORT_C static TMsvId FindEntryByNameL(CMsvEntry* aEntry,TDesC& aEntryName, TBool aCaseSensitive = EFalse);
	
	// Returns the EntryID of the entry based on the entry name and entry parent name under the given parent entry
	IMPORT_C static TMsvId FindEntryByNameL(CMsvEntry* aEntry,TDesC& aEntryName, TDesC& aParentEntryName, TBool aCaseSensitive = EFalse);

	// Creates a selection of the children
	IMPORT_C static CMsvEntrySelection* CreateChildrenSelectionL(CMsvSession* aSession, TMsvId aEntryId);

	// Creates a selection of the children with a specified type
	IMPORT_C static CMsvEntrySelection* CreateChildrenSelectionWithTypeL(CMsvSession* aSession, TMsvId aEntryId, TUid aType);

	// Returns a selection of the child entries under the specified remote folder
	IMPORT_C static CMsvEntrySelection*  CreateChildrenSelectionRemoteFolderL(CMsvSession* aSession,TPtrC aAccountName,
															TPtrC aFolderName);

	// Returns a selection of the child entries under the specified remote folder with a specified type
	IMPORT_C static CMsvEntrySelection*  CreateChildrenSelectionRemoteFolderWithTypeL(CMsvSession* aSession,TPtrC aAccountName,
															TPtrC aFolderName, TUid aType);
									
	// Searches for a given entry under the specified remote folder Id for IMAP
	IMPORT_C static TMsvId GetRemoteFolderIdByNameL(CMsvSession* aSession,TPtrC aAccountName,TPtrC aFolderName, TBool aCaseSensitive = EFalse);
	
	// Searches for a given entry under the specified remote folder Id for IMAP - overloaded version with parent folder name
	IMPORT_C static TMsvId GetRemoteFolderIdByNameL(CMsvSession* aSession,TPtrC aAccountName,TPtrC aFolderName, TPtrC aParentFolderName, TBool aCaseSensitive = EFalse);

	// Searches for a message in a given folder based ont he message subject
	IMPORT_C static TMsvId SearchMessageBySubjectL(CMsvSession* aSession, TMsvId afolderId, TPtrC aSubject);
	
	// Searches for a message in a given folder based ont he message subject ..overloaded
	IMPORT_C static TMsvId SearchMessageBySubjectL(CMsvSession* aSession,TMsvId afolderId,TPtrC aSubject, TBool aVisibleFlag);
	
	// Compares two input files
	IMPORT_C static TInt CompareFilesL(TPtrC aFileOne, TPtrC aFileTwo, TBool aIsUnicode,CArrayFixFlat<TUint16>* aIgnoreCharList,
																			TBool& aDiffFlag);

	// Reads data chunk from the file stream.Returns ETrue if the EOF found.
	IMPORT_C static TBool ReadDataL(RFileReadStream& aFileReadStream,TPtr& aPtrLineBuffer, TBool aIsUnicode, 
																	CArrayFixFlat<TUint16>* aIgnoreCharList);

	IMPORT_C static void OpenResourceFileL(RResourceFile& rFile, RFs& anFs, const TDesC& aFileName );
 		
 	//Closes the Open Resource File
 	IMPORT_C static void CloseResourceFile( TAny* aFile );
 	
 private:
 	// Recursively searches the EntryID of the entry based on the entry name
 	TMsvId static RecFindEntryByNameL(CMsvEntry& aEntry, TDesC& aEntryName, TBool aCaseSensitive);
 	
 	// Recursively searches the EntryID of the entry based on the entry name and parent entry name
 	TMsvId static RecFindEntryByNameL(CMsvEntry& aEntry, TDesC& aEntryName, TDesC& aParentEntryName, TBool aCaseSensitive);
 	
 	// Checks if the given parent name matches the name of the given parent entry
 	TBool static CheckParent(CMsvEntry& aParentEntry, TDesC& aParentEntryName);
	};
#endif //__T_UTILS_H__
