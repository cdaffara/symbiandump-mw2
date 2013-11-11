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
// Telephone Voice Mailbox number settings parser
// 
//

#include <msventry.h>

#include <cmsvattachment.h>
#include <mmsvattachmentmanager.h>
#include <mmsvattachmentmanagersync.h>
#include <cbioasyncwaiter.h>

#ifdef SYMBIAN_BOOKMARK_DATABASE
#include <bookmarkdatabase.h>
#include <bookmark.h>
#endif // SYMBIAN_BOOKMARK_DATABASE

#include "BSP.H"
#include "IACP.H"
#include "wwwp.h"
#include "IACPDEF.H"
#include "IACPERR.H"

_LIT8(KCRLinefeed, "\r\n");
_LIT8(KEBookmarkItemBegin, "BEGIN:eBOOKMARK\r\n");		//  Precedes a Bookmark in the Bookmark file
_LIT8(KEBookmarkItemName, "NAME:");	// Precedes a Bookmark Name
_LIT8(KEBookmarkItemURL, "URL:");		// Precedes a Bookmark URL
_LIT8(KEBookmarkType, "TYPE:Web\r\n");
_LIT8(KEBookmarkItemEnd, "END:eBOOKMARK\r\n");

_LIT(KEBookmarkExtension, ".eBM");

// The number of characters that are constant for an eBookmark
// file. It is the sum of the above literals.
// 
const TInt KEBookmarkConstantChars = 55;


CWWWHotlistItem::CWWWHotlistItem()
	{
	}

CWWWHotlistItem::~CWWWHotlistItem()
	{
	delete iBmkName;
	delete iBmkUrl;
	}

const TDesC& CWWWHotlistItem::Name() const
	{
	if (iBmkName == NULL)
		return KNullDesC;
	else
		return *iBmkName;
	}
	
const TDesC& CWWWHotlistItem::Url() const
	{
	if (iBmkUrl == NULL)
		return KNullDesC;
	else
		return iBmkUrl->Addr();
	}


void CWWWHotlistItem::SetNameL(const TDesC& aDes)
	{
	HBufC* tempName = aDes.AllocL();
	delete iBmkName;
	iBmkName = tempName;
	}
	
void CWWWHotlistItem::SetUrlL(const TDesC& aDes)
	{
	if(!iBmkUrl)
		iBmkUrl = CIpAddress::NewL(aDes);
	else
		iBmkUrl->SetAddrL(aDes);
	}


//
// Constructor
//
CWWWHotlistParser::CWWWHotlistParser(RFs& aFs)
:iFs(aFs)
	{
	}
//
// Destruction
//
CWWWHotlistParser::~CWWWHotlistParser()
	{
	if(iHotlistItemList)
		iHotlistItemList->ResetAndDestroy();

	delete iHotlistItemList;
	}

//
// factory fn
//
CWWWHotlistParser* CWWWHotlistParser::NewL(RFs& aFs)
	{
	CWWWHotlistParser* self = new(ELeave)CWWWHotlistParser(aFs);
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop();
	return self;
	}

void CWWWHotlistParser::ConstructL()
	{
	iHotlistItemList = new(ELeave)CArrayPtrSeg<CWWWHotlistItem>(5);
	}

//
// Parse/Set data members
//
void CWWWHotlistParser::ParseL(CParsedFieldCollection& aIacpFields)
	{
	CWWWHotlistItem* item = NULL;
	// step through
	TInt count = aIacpFields.Count();
	for(TInt i = 0; i< count;i++)
		{
		CParsedField& field = *(aIacpFields[i]);
		if(field.FieldName().Compare(SMS_HOTLIST_ITEM_NAME)==0)
			{
			item = new(ELeave)CWWWHotlistItem();
			CleanupStack::PushL(item);	

			item->SetNameL(field.FieldValue());
			if(++i<count)
				{
				CParsedField& urlfield = *(aIacpFields[i]);
				if(urlfield.FieldName().Compare(SMS_HOTLIST_ITEM_URL)==0)
					{
					item->SetUrlL(urlfield.FieldValue());
					iHotlistItemList->AppendL(item);
					CleanupStack::Pop();
					}
				else
					{
					CleanupStack::PopAndDestroy();
					User::Leave(KErrCorrupt);
					}
				}
			else
				{
				CleanupStack::PopAndDestroy();
				User::Leave(KErrCorrupt);
				}
			}
		}
	}
//
// Currently just write items to a file
//
void CWWWHotlistParser::ProcessL(CMsvEntry& aEntry)
	{
	StoreL(aEntry);
	}

//
//  StoreL() - Store bookmarks as an attachment file 
//	in the .eBM format. If the system runs out of memory while
//  the bookmarks are being written to the file, the file will be
//	deleted. For example, if 2 bookmarks have already been written
//	to the file, and the writing of the third bookmark fails, the
//	file will be deleted. Otherwise, a failure of file writing would
//	need to be handled differently from memory allocation failure.
//
void CWWWHotlistParser::StoreL(CMsvEntry& aEntry)
{
	TInt numberOfItems = iHotlistItemList->Count();
	
	if (numberOfItems>0)  // Only create a file if there is something to save!
	{

		// Generate fileName from msgId and bookmark file extension.
		// The file name consists of the msgId in hex format 
		// followed by .eBM. Sizeof operator returns the size of msgId
		// in bytes and each byte requires 2 hex digits to represent it,
		// hence sizeof is multipled by 2. Didn't want to make
		// fileNameLength constant because the size of TMsvId may change
		// in the future.

		TMsvId entryId = aEntry.Entry().Id();

		TInt fileNameLength = 2*sizeof(entryId) + 
				KEBookmarkExtension().Length();

		HBufC *fileName = HBufC::NewLC(fileNameLength);
			
		TPtr fileNameDes = fileName->Des();

		// The file name uses the hex representation of the entry Id.
		// If this changes to some other representation then
		// fileNameLength will need to be calculated differently.

		fileNameDes.Num(entryId,EHex);
			
		fileNameDes.Append(KEBookmarkExtension);

		// Get the attachment manager and create an empty attachment file
		CMsvStore* store = aEntry.EditStoreL();
		CleanupStack::PushL(store);
		
		MMsvAttachmentManagerSync& managerSync = store->AttachmentManagerExtensionsL();
		CMsvAttachment* attachment = CMsvAttachment::NewL(CMsvAttachment::EMsvFile);
		CleanupStack::PushL(attachment);
		attachment->SetAttachmentNameL(*fileName);
		RFile file;
		managerSync.CreateAttachmentL(*fileName, file, attachment);
		CleanupStack::Pop(attachment); // ownership passed
		CleanupClosePushL(file);

#ifdef SYMBIAN_BOOKMARK_DATABASE
		// Open the bookmark database ready to add the bookmarks
		RBkDatabase bookmarkDb;
		bookmarkDb.OpenL();
		CleanupClosePushL(bookmarkDb);
#endif // SYMBIAN_BOOKMARK_DATABASE

		// Stream each bookmark into the file.
		// The eBookmark file must contain only 8bit ascii.
		// Add a linefeed to the end of each line.
			
		for(TInt count=0; count < numberOfItems; count++)
			{
			CWWWHotlistItem &item = *iHotlistItemList->At(count);
			
			// Allocate enough space to hold the full bookmark entry.

			TInt length =	item.Name().Length() + 
							item.Url().Length()  + 
							KEBookmarkConstantChars;
			
			HBufC8 *writeBuf = HBufC8::NewLC(length);
			
			TPtr8 des = writeBuf->Des();

			des.Append(KEBookmarkItemBegin);
			des.Append(KEBookmarkItemURL);
			des.Append(item.Url());
			des.Append(KCRLinefeed);
			des.Append(KEBookmarkItemName);
			des.Append(item.Name());
			des.Append(KCRLinefeed);
			des.Append(KEBookmarkType);
			des.Append(KEBookmarkItemEnd);

			User::LeaveIfError(file.Write(des));
			
			CleanupStack::PopAndDestroy();
			
#ifdef SYMBIAN_BOOKMARK_DATABASE
			// Add the bookmark to the bookmark database
			RBkBookmark bookmark = bookmarkDb.CreateBookmarkL();
			CleanupClosePushL(bookmark);
			bookmark.SetTitleL(item.Name());
			// Convert Uri to 8-bit
			HBufC8* bookmarkUri = HBufC8::NewLC(item.Url().Length());
			bookmarkUri->Des().Copy(item.Url());
			bookmark.SetUriL(*bookmarkUri);
			CleanupStack::PopAndDestroy(2, &bookmark); // bookmarkUri, bookmark
#endif // SYMBIAN_BOOKMARK_DATABASE
			}
			
#ifdef SYMBIAN_BOOKMARK_DATABASE			
		// Commit all the added bookmarks and close bookmark db
		bookmarkDb.CommitL();
		CleanupStack::PopAndDestroy(&bookmarkDb);
#endif // SYMBIAN_BOOKMARK_DATABASE

		// File writing has completed, set the size in the attachment
		TInt fileSize = 0;
		User::LeaveIfError(file.Size(fileSize));
		attachment->SetSize(fileSize);
		
		// commit the changes
		store->CommitL();
		CleanupStack::PopAndDestroy(3, fileName); // file, store, fileName
	}
}

