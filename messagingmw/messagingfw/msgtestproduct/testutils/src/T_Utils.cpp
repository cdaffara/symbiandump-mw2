// Copyright (c) 2006-2009 Nokia Corporation and/or its subsidiary(-ies).
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
// This is the Cpp file which contains the utility functions common across the servers
// 
//

/**
 @file
*/


// User includes
#include <t_utils.h>
#include <t_utilscentralrepository.h>

// Epoc includes
#include <msvids.h>
#include <bautils.h>
#include "msvutils.h"
#include <barsc.h>


// Literals Used
const TInt KDefBufferSize = 1024;


/**
CT_MsgUtils()
Constructor
*/
EXPORT_C CT_MsgUtils::CT_MsgUtils()
	{
	}


/**
~CT_MsgUtils()
Constructor
*/
CT_MsgUtils::~CT_MsgUtils()
	{
	}

/**
OpenResourceFileL()
Opens a resource File

@param rFile
Resource File Handle

@param anFs
File Session associated

@param aFileName
The Name of resource File to be opened
*/
EXPORT_C void CT_MsgUtils::OpenResourceFileL(RResourceFile& rFile, RFs& anFs, const TDesC& aFileName )
 	{
 	// open resource file
 	TFileName fileName(aFileName);
 	MsvUtils::AddPathAndExtensionToResFileL(fileName);
 	BaflUtils::NearestLanguageFile( anFs, fileName );
 	if (BaflUtils::FileExists(anFs,fileName))
  		{
 		rFile.OpenL( anFs, fileName );
  		return;
  		}
  	User::Leave(KErrNotFound);
 	}

/**
CloseResourceFile()
Closes a recource File

@param aFile
The Name of resource File to be closed
*/
 EXPORT_C void CT_MsgUtils::CloseResourceFile( TAny* aFile )
 	{
 	((RResourceFile*)aFile)->Close();
 	}


/**
FindEntryByNameL()
Search the entries under a given folder ID by name of the entry
i.e, iDescription or iDetails value of an entry

@param aSession
Session object

@param aParentId
Parent Id under which the entry needs to be found

@param aEntryName
The entry name that needs to be found

@leave KErrNotFound
Leaves if no entry's iDescription or iDetails matches with aEntryName

@return
Id of the entry that match 
*/
EXPORT_C TMsvId CT_MsgUtils::FindEntryByNameL(CMsvSession* aSession, TMsvId aParentId, TDesC& aEntryName, TBool aCaseSensitive)
	{
	
	TMsvId msgEntryId = KMsvNullIndexEntryId;

	// Set the context to the Parent
	CMsvEntry* entry = CMsvEntry::NewL(*aSession, aParentId,TMsvSelectionOrdering
									(KMsvNoGrouping,EMsvSortByNone, ETrue));
	CleanupStack::PushL(entry);
	
	entry->SetEntryL(aParentId);
 	
 	msgEntryId = FindEntryByNameL(entry, aEntryName, aCaseSensitive);
 	
 	CleanupStack::PopAndDestroy(entry);
 	
 	return msgEntryId;
	}


/**
FindEntryByNameL()
Search the entries under a given folder ID by name of the entry
i.e, iDescription or iDetails value of an entry

@param aSession
Session object

@param aParentId
Parent Id under which the entry needs to be found

@param aEntryName
The entry name that needs to be found

@param aParentEntryName
The Parent entry name that needs to be matched with the entry found

@leave KErrNotFound
Leaves if no entry's iDescription or iDetails matches with aEntryName

@return
Id of the entry that match 
*/
EXPORT_C TMsvId CT_MsgUtils::FindEntryByNameL(CMsvSession* aSession, TMsvId aParentId, TDesC& aEntryName, TDesC& aParentEntryName, TBool aCaseSensitive)
	{
	
	TMsvId msgEntryId = KMsvNullIndexEntryId;
	
	// Set the context to the Parent
	CMsvEntry* entry = CMsvEntry::NewL(*aSession, aParentId,TMsvSelectionOrdering
									(KMsvNoGrouping,EMsvSortByNone, ETrue));
	CleanupStack::PushL(entry);
	
	entry->SetEntryL(aParentId);

 	msgEntryId = FindEntryByNameL(entry, aEntryName, aParentEntryName, aCaseSensitive);
 	
 	CleanupStack::PopAndDestroy(entry);
 	
 	return msgEntryId;
	}


/**
FindEntryByNameL()
Search the entries under a given entry(an object of CMsvEntry) by name of the entry
i.e, iDescription or iDetails value of an entry

@param aEntry	
Parent entry under which the given entry needs to be searched

@param aEntryName
The entry name that needs to be found

@leave KErrNotFound
Leaves if no entry's iDescription or iDetails matches with aEntryName

@return
Id of the entry that match 
*/
EXPORT_C TMsvId  CT_MsgUtils::FindEntryByNameL(CMsvEntry* aEntry,TDesC& aEntryName, TBool aCaseSensitive)
	{
	TMsvId msgEntryId = KMsvNullIndexEntryId;
	
	msgEntryId = RecFindEntryByNameL(*aEntry, aEntryName, aCaseSensitive);
	
	if(msgEntryId == KMsvNullIndexEntryId)
		{
		User::Leave(KErrNotFound);
		}

	return msgEntryId;
	}


/**
FindEntryByNameL()
Search the entries under a given entry(an object of CMsvEntry) by name of the entry
i.e, iDescription or iDetails value of an entry

@param aEntry	
Parent entry under which the given entry needs to be searched

@param aEntryName
The entry name that needs to be found

@param aParentEntryName
The Parent entry name that needs to be matched with the entry found

@leave KErrNotFound
Leaves if no entry's iDescription or iDetails matches with aEntryName

@return
Id of the entry that match 
*/
EXPORT_C TMsvId  CT_MsgUtils::FindEntryByNameL(CMsvEntry* aEntry,TDesC& aEntryName, TDesC& aParentEntryName, TBool aCaseSensitive)
	{
	TMsvId msgEntryId = KMsvNullIndexEntryId;
	
	msgEntryId = RecFindEntryByNameL(*aEntry, aEntryName, aParentEntryName, aCaseSensitive);
	
	if(msgEntryId == KMsvNullIndexEntryId)
		{
		User::Leave(KErrNotFound);
		}

	return msgEntryId;
	}


/**
RecFindEntryByNameL()
Recursively search the entries (including all sub entries) under a given entry (an object of CMsvEntry) 
by name of the entry i.e, iDescription or iDetails value of an entry

@param aEntry	
Parent entry under which the given entry needs to be searched

@param aEntryName
The entry name that needs to be found

@leave KErrNotFound
Leaves if no entry's iDescription or iDetails matches with aEntryName

@return
Id of the entry that match 
*/
TMsvId  CT_MsgUtils::RecFindEntryByNameL(CMsvEntry& aEntry, TDesC& aEntryName, TBool aCaseSensitive)
 	{
 
 	TMsvId msgEntryId = KMsvNullIndexEntryId;
 	
 	CMsvEntrySelection* selection = aEntry.ChildrenL();
 	CleanupStack::PushL(selection);
 	// Search for the entry in the children selection, based on the entry name given
 	TBool	found = EFalse;
 	TInt count = selection->Count();
 	for (TInt index = 0; (index<count) && (!found); index++)
 		{
  		// Set the context to the child entry
  		aEntry.SetEntryL((*selection)[index]);

		if (aCaseSensitive)
			{
			if (( aEntryName.Compare( aEntry.Entry().iDescription ) == 0 ) ||
			    ( aEntryName.Compare( aEntry.Entry().iDetails ) == 0 ))
				{
				found = ETrue;
				}
			}
		else if (( aEntryName.CompareF( aEntry.Entry().iDescription ) == 0 ) ||
		         ( aEntryName.CompareF( aEntry.Entry().iDetails ) == 0 ))
			{
			found = ETrue;
			}

		if (found)
			{
			msgEntryId = (*selection)[index];
			}
		else
			{
			CMsvEntry* nextEntry = CMsvEntry::NewL(aEntry.Session(), (*selection)[index],TMsvSelectionOrdering(KMsvNoGrouping, EMsvSortByNone, ETrue));
			CleanupStack::PushL(nextEntry);

			msgEntryId = RecFindEntryByNameL(*nextEntry, aEntryName, aCaseSensitive);
			if(msgEntryId != KMsvNullIndexEntryId)
				{
				found = ETrue;
				}
   		CleanupStack::PopAndDestroy(nextEntry);  
   		}
 		}
		
 	CleanupStack::PopAndDestroy(selection);
 	return msgEntryId;
 	}


/**
RecFindEntryByNameL()
Recursively search the entries (including all sub entries) under a given entry (an object of CMsvEntry) 
by name of the entry i.e, iDescription or iDetails value of an entry

@param aEntry	
Parent entry under which the given entry needs to be searched

@param aEntryName
The entry name that needs to be found

@param aParentEntryName
The Parent entry name that needs to be matched with the entry found

@leave KErrNotFound
Leaves if no entry's iDescription or iDetails matches with aEntryName

@return
Id of the entry that match 
*/
TMsvId  CT_MsgUtils::RecFindEntryByNameL(CMsvEntry& aEntry, TDesC& aEntryName, TDesC& aParentEntryName, TBool aCaseSensitive)
 	{
 	TMsvId msgEntryId = KMsvNullIndexEntryId;
 	
 	CMsvEntrySelection* selection = aEntry.ChildrenL();
 	CleanupStack::PushL(selection);
 	// Search for the entry in the children selection, based on the entry name given
 	TBool	found = EFalse;
 	TInt count = selection->Count();
	TBool matched = EFalse;
 	
 	for (TInt index = 0; (index<count) && (!found); index++)
 		{
  		// Set the context to the child entry
  		aEntry.SetEntryL((*selection)[index]);

		matched = EFalse;

		if (aCaseSensitive)
			{
			if (( aEntryName.Compare( aEntry.Entry().iDescription ) == 0 ) ||
			    ( aEntryName.Compare( aEntry.Entry().iDetails ) == 0 ))
				{
				matched = ETrue;
				}
			}
		else if (( aEntryName.CompareF( aEntry.Entry().iDescription ) == 0 ) ||
		         ( aEntryName.CompareF( aEntry.Entry().iDetails ) == 0 ))
			{
			matched = ETrue;
			}

		if (matched)
			{
			//Check Parent
			CMsvEntry* parentEntry = CMsvEntry::NewL(aEntry.Session(), aEntry.Entry().Parent(),TMsvSelectionOrdering(KMsvNoGrouping, EMsvSortByNone, ETrue));
			CleanupStack::PushL(parentEntry);

			if(CheckParent(*parentEntry,aParentEntryName))
				{
				found = ETrue;
				msgEntryId = (*selection)[index];
				}
			CleanupStack::PopAndDestroy(parentEntry);
			}

		if (!found)
			{
			CMsvEntry* nextEntry = CMsvEntry::NewL(aEntry.Session(), (*selection)[index],TMsvSelectionOrdering(KMsvNoGrouping, EMsvSortByNone, ETrue));
			CleanupStack::PushL(nextEntry);

			msgEntryId = RecFindEntryByNameL(*nextEntry, aEntryName, aParentEntryName, aCaseSensitive);
			if(msgEntryId != KMsvNullIndexEntryId)
				{
				found = ETrue;
				}
			CleanupStack::PopAndDestroy(nextEntry);
			}
  		}

 	CleanupStack::PopAndDestroy(selection);
 	return msgEntryId;
 	}
 
 
/**
CheckParent()
Checks if the given parent name matches the name of the given parent entry

@param aParentEntry	
Parent entry whose name is to be checked

@param aParentEntryName
The Parent entry name that we are checking against

@return
ETrue if they match and EFalse if they do not
*/
TBool CT_MsgUtils::CheckParent(CMsvEntry& aParentEntry, TDesC& aParentEntryName)
	{

 	TBool ret = EFalse;
 	
 	if (( aParentEntryName.CompareF( aParentEntry.Entry().iDescription ) == 0 ) ||
     	( aParentEntryName.CompareF( aParentEntry.Entry().iDetails ) == 0 ))
 		{
 		ret = ETrue;
 		}
 	
 	return ret;
	}


/**
CreateChildrenSelectionL()
This function creates a selection of the children entries under a given parent Id

@param aSession	
A CMsvSession object
		
@param aEntryId	
Id of the entry

@return
Returns a selection of all child entries under the given entry
*/
EXPORT_C CMsvEntrySelection* CT_MsgUtils::CreateChildrenSelectionL(CMsvSession* aSession, 
																   TMsvId       aEntryId)
	{
	CMsvEntry* entry = CMsvEntry::NewL(*aSession, aEntryId,TMsvSelectionOrdering
									( KMsvNoGrouping,EMsvSortByNone, ETrue));
	CleanupStack::PushL(entry);
	entry->SetEntryL(aEntryId);

	TMsvSelectionOrdering order;
	order.SetShowInvisibleEntries(ETrue);
	entry->SetSortTypeL(order);

	CMsvEntrySelection* selection = entry->ChildrenL();
	CleanupStack::PopAndDestroy(entry);
	
	return selection;
	}

/**
CreateChildrenSelectionWithTypeL()
This function creates a selection of the children entries under a given parent Id
which are of a specified type.

@param aSession	
A CMsvSession object
		
@param aEntryId	
Id of the entry

@param aType
Type of entry to look for

@return
Returns a selection of all child entries under the given entry of the specified type
*/
EXPORT_C CMsvEntrySelection* CT_MsgUtils::CreateChildrenSelectionWithTypeL(CMsvSession* aSession, TMsvId aEntryId, TUid aType)
	{
	CMsvEntry* entry = CMsvEntry::NewL(*aSession, aEntryId, TMsvSelectionOrdering(KMsvNoGrouping, EMsvSortByNone, ETrue));
	CleanupStack::PushL(entry);
	entry->SetEntryL(aEntryId);

	TMsvSelectionOrdering order;
	order.SetShowInvisibleEntries(ETrue);
	entry->SetSortTypeL(order);

	CMsvEntrySelection* selection = entry->ChildrenWithTypeL(aType);
	CleanupStack::PopAndDestroy(entry);

	return selection;
	}


/**
CreateChildrenSelectionRemoteFolderL()
This function creates a selection of the child entries under the 
remote folder Id if the service name is specified

@param aSession		
A CMsvSession object

@param aAccountName	
The account name or service name

@param aFolderName
The name of the remote folder whose entries need to be fetched

@return
The selection of all entries under the given entry
*/
EXPORT_C CMsvEntrySelection*  CT_MsgUtils::CreateChildrenSelectionRemoteFolderL(CMsvSession*
										aSession,TPtrC aAccountName,TPtrC aFolderName)
	{
	TMsvId remoteFolderId = GetRemoteFolderIdByNameL(aSession, aAccountName, aFolderName);
	return 	CreateChildrenSelectionL(aSession,remoteFolderId);
	}

/**
CreateChildrenSelectionRemoteFolderWithTypeL()
This function creates a selection of the child entries under the 
remote folder Id with a specified type if the service name is specified

@param aSession		
A CMsvSession object

@param aAccountName	
The account name or service name

@param aFolderName
The name of the remote folder whose entries need to be fetched

@param aType
Type of entry to look for

@return
The selection of all entries under the given entry with the specified type
*/
EXPORT_C CMsvEntrySelection*  CT_MsgUtils::CreateChildrenSelectionRemoteFolderWithTypeL(CMsvSession*
										aSession,TPtrC aAccountName,TPtrC aFolderName, TUid aType)
	{
	TMsvId remoteFolderId = GetRemoteFolderIdByNameL(aSession, aAccountName, aFolderName);
	return CreateChildrenSelectionWithTypeL(aSession, remoteFolderId, aType);
	}


/**
GetRemoteFolderIdByNameL()
Searches for a given entry under the remote folder Id specified and gets it Id

@param aSession		
A CMsvSession object

@param aAccountName
The account name

@param aFolderName
The name of the remote folder whose Id is to be found

@return
The Id of the specified folder
*/
EXPORT_C TMsvId CT_MsgUtils::GetRemoteFolderIdByNameL(CMsvSession* aSession,TPtrC aAccountName,TPtrC aFolderName, TBool aCaseSensitive)
	{
	TMsvId imapServiceId = CT_MsgUtilsCentralRepository::GetImapServiceIdL((TDes&)aAccountName);
	return CT_MsgUtils::FindEntryByNameL(aSession, imapServiceId, (TDesC&)aFolderName, aCaseSensitive);
	}


/**
GetRemoteFolderIdByNameL()
Searches for a given entry under the remote folder Id specified and gets it Id

@param aSession		
A CMsvSession object

@param aAccountName
The account name

@param aFolderName
The name of the remote folder whose Id is to be found

@param aFolderName
The name of the remote parent folder of the entry to be found

@return
The Id of the specified folder
*/
EXPORT_C TMsvId CT_MsgUtils::GetRemoteFolderIdByNameL(CMsvSession* aSession,TPtrC aAccountName,TPtrC aFolderName, TPtrC aParentFolderName, TBool aCaseSensitive)
	{
	TMsvId imapServiceId = CT_MsgUtilsCentralRepository::GetImapServiceIdL((TDes&)aAccountName);
	return CT_MsgUtils::FindEntryByNameL(aSession, imapServiceId, (TDesC&)aFolderName, (TDesC&)aParentFolderName, aCaseSensitive);
	}


/**
SearchMessageBySubjectL()
This function searches for a message based on the 
subject under the specified local folder

@param aSession
A CMsvSession object

@param afolderId
The folder Id under which the search needs to be done

@param aSubject
The subject name of the message

@leave KErrNotFound
Leaves if the message is not found

@return
If the message is found it returns the message Id
*/
EXPORT_C TMsvId CT_MsgUtils::SearchMessageBySubjectL(CMsvSession* aSession,TMsvId afolderId,TPtrC aSubject)
	{
	TMsvId	messageId = KMsvNullIndexEntryId;

	CMsvEntry*	entry = aSession->GetEntryL(afolderId);
	CleanupStack::PushL(entry);

	// Set the entry to the source folder id
	entry->SetEntryL(afolderId);

	//Get CMsvEntrySelection object
	CMsvEntrySelection*	selection = entry->ChildrenL();
	CleanupStack::PushL(selection);
	
	TInt count = selection->Count();

	if ( count > 0)
		{
		CMsvEntry*	childEntry = aSession->GetEntryL(selection->At(0));
		CleanupStack::PushL(childEntry);

		for(TInt entryIndex = 0 ; entryIndex< count; entryIndex++)
			{
			childEntry->SetEntryL(selection->At(entryIndex));
					
			if (childEntry->Entry().iDescription.Compare(aSubject) == 0)
				{	
				messageId = selection->At(entryIndex);
				break;
				}
			}
		CleanupStack::PopAndDestroy(childEntry);
		}
	if(messageId == KMsvNullIndexEntryId)
		{
		User::Leave(KErrNotFound);
		}
	CleanupStack::PopAndDestroy(2,entry);	//selection,entry
	return messageId; 
	}
	
/**
SearchMessageBySubjectL()
This function searches for a message based on the subject 
under the specified local folder. This fucntion provides the
flexibility of making the invisible children entries visible.

@param aSession
A CMsvSession object

@param afolderId
The folder Id under which the search needs to be done

@param aSubject
The subject name of the message

@leave KErrNotFound
Leaves if the message is not found

@return
If the message is found it returns the message Id
*/
EXPORT_C TMsvId CT_MsgUtils::SearchMessageBySubjectL(CMsvSession* aSession,TMsvId afolderId,TPtrC aSubject, TBool aVisibleFlag)
	{
	TMsvId	messageId = KMsvNullIndexEntryId;

	CMsvEntry*	entry = aSession->GetEntryL(afolderId);
	CleanupStack::PushL(entry);

	// Set the entry to the source folder id
	entry->SetEntryL(afolderId);
	if(aVisibleFlag)
		{
		// Make the message entries visible
		TMsvSelectionOrdering order;
		order.SetShowInvisibleEntries(ETrue);
		entry->SetSortTypeL(order);	
		}

	//Get CMsvEntrySelection object
	CMsvEntrySelection*	selection = entry->ChildrenL();
	CleanupStack::PushL(selection);
	
	TInt count = selection->Count();

	if ( count > 0)
		{
		CMsvEntry*	childEntry = aSession->GetEntryL(selection->At(0));
		CleanupStack::PushL(childEntry);

		for(TInt entryIndex = 0 ; entryIndex< count; entryIndex++)
			{
			childEntry->SetEntryL(selection->At(entryIndex));
					
			if (childEntry->Entry().iDescription.Compare(aSubject) == 0)
				{	
				messageId = selection->At(entryIndex);
				break;
				}
			}
		CleanupStack::PopAndDestroy(childEntry);
		}
	if(messageId == KMsvNullIndexEntryId)
		{
		User::Leave(KErrNotFound);
		}
	CleanupStack::PopAndDestroy(2,entry);	//selection,entry
	return messageId; 
	}
	
/**
CompareFilesL()
Compares two input files (ASCII & Unicode).

@param aFileOne
The first file to be compared

@param aFileTwo
The second file to be compared

@param aIsUnicode
To check if its unicode

@param aIgnoreCharList
An array of type CArrayFixFlat

@param aDiffFlag
Flag to indicate the difference

@return
Returns ETrue if the files are different.
*/
EXPORT_C TInt CT_MsgUtils::CompareFilesL(TPtrC aFileOne, TPtrC aFileTwo, TBool aIsUnicode,
									  CArrayFixFlat<TUint16>* aIgnoreCharList, TBool& aDiffFlag)
	{
	// Return Code
	TInt retCode = KErrNone;

	// Streams for the file1 and file2 to compare
	RFileReadStream fileReadStream1;
	RFileReadStream fileReadStream2;

	// Files difference flag
	aDiffFlag = EFalse;

	RFs iFs;
	User::LeaveIfError(iFs.Connect());

	// Open file one to compare in the read only mode
	retCode = fileReadStream1.Open( iFs, aFileOne, EFileShareReadersOnly);

	// If the file open of the first file is successful then open the second file, also in read only mode
	if(retCode == KErrNone)
		{
		retCode = fileReadStream2.Open( iFs, aFileTwo, EFileShareReadersOnly);

		// The file open of the second file is successful
		if(retCode == KErrNone)
			{
			// To hold the line content of file one 
			HBufC* lineOneBuffer = HBufC::NewLC(KDefBufferSize);
			TPtr ptrLineOne = lineOneBuffer->Des();
			
			// To hold the line content of file two
			HBufC* lineTwoBuffer = HBufC::NewLC(KDefBufferSize);
			TPtr ptrLineTwo = lineTwoBuffer->Des();

			// End of File indicators
			TBool	eofOne = EFalse;
			TBool	eofTwo = EFalse; 
			
			// Read the file one and file two data
			do   
				{
				// Read file one data
				eofOne = ReadDataL(fileReadStream1, ptrLineOne, aIsUnicode, aIgnoreCharList);

				// Read file two data
				eofTwo = ReadDataL(fileReadStream2, ptrLineTwo, aIsUnicode, aIgnoreCharList);

				// Check EOF state of the files.
				// Either both the files will be in EOF state or 
				// none of the files will be in EOF state			  
				if((!eofOne && !eofTwo)||(eofOne && eofTwo))
	            	{			
					// Compare the read lines from file one and the file two
					if(ptrLineOne.Compare(ptrLineTwo) != 0)
						{
						// Different data content so set the differnce flag
						aDiffFlag = ETrue;
						}
					}
				else
					{
					// Different EOF 
					aDiffFlag = ETrue;
					}
				} while(!eofOne && !eofTwo && !aDiffFlag);				
			
			// Delete line buffers
			CleanupStack::PopAndDestroy(2, lineOneBuffer);				
			}
		}
	
	// Close the open streams
	fileReadStream1.Close();
	fileReadStream2.Close();	

	iFs.Close();
	// Return the difference flag
	return retCode;
	}


/**
ReadDataL()
Reads data chunk from the file stream. Returns ETrue if the EOF found

@param aFileReadStream
File Stream

@param aPtrLineBuffer		
Buffer to hold a line

@param aIsUnicode
To check if its unicode

@param aIgnoreCharList
List of the characters to be ignored

@leave KErrNotFound
Leaves if aIgnoreCharList is empty

@return 
Return end of file flag	
*/
EXPORT_C TBool CT_MsgUtils::ReadDataL(RFileReadStream& aFileReadStream,
	  TPtr& aPtrLineBuffer, TBool aIsUnicode, CArrayFixFlat<TUint16>* aIgnoreCharList)
 	{
	TUint16			element = 0;
	TKeyArrayFix	key(0, ECmpTUint16);
	TInt			charPosition = 0;
	TBool			eof = EFalse;
	TInt			errorCode = KErrNone;
	TUint8			aCharASCII = 0;
	
	aPtrLineBuffer.FillZ();
	aPtrLineBuffer.SetLength(0);	
	
	// Validate the input ignore char list
	if (!aIgnoreCharList)
		{
		// Invalid parameter passed to ReadDataL: No ignore char list passed
		User::Leave(KErrNotFound);
		}

	// Read data from file and store it in lineBuffer 
	do   
		{
		if(aIsUnicode)
			{
			TRAP(errorCode, (element = aFileReadStream.ReadUint16L()));
			}
		else
			{
			TRAP(errorCode, (aCharASCII = aFileReadStream.ReadUint8L()));
			element = (TUint16) aCharASCII;
			}
			
		if (errorCode != KErrEof)
			{		
			// Check the availability of ignore char in the array							
			if ( aIgnoreCharList->Find(element, key, charPosition) != KErrNone )
				{
				// Append the char to the buffer if the read char is not ignore char
				aPtrLineBuffer.Append(element);
				}
			}								
			else
				{
				eof = ETrue;
				break;
				}
				
			// Stop at the end of line or no more data 
			} while(aPtrLineBuffer.Length() < KDefBufferSize);
	
	// Return end of file flag		
	return eof;	
	}
