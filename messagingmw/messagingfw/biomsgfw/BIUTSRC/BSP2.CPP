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
// BSP2.CPP
//
#include "BSP.H"
#include "REGPSDLL.H"	// CRegisteredParserDll
#include "msvapi.h"		// CMsvEntry
#include <cbioasyncwaiter.h>

#include <msvuids.h>	// KUidMsvMessageEntry, KUidMsvServiceEntry
#include <msvids.h>
#include <biouids.h>
#include <biodb.h>
#include <mmsvattachmentmanager.h>
#include <mmsvattachmentmanagersync.h>
#include <cmsvattachment.h>
#ifdef SYMBIAN_ENABLE_SPLIT_HEADERS 
#include "tmsvbioinfo.h"
#include <biomessageuids.h>
#include <bifchangeobserver.h>
#endif

// constants
const TInt KMaxStringLength = 1024;
const TInt KParsedFieldArrayGranularity = 16;
const TInt KReadBufSize = 256;

/** Constructor.

This is called by CBIOServerMtm to create a parser object.

@param aRegisteredParserDll Object that loaded the parser. It contains a reference 
counter of the use of the parser.
@param aEntry The message entry the parser should parse
@param aFs Connected file server handle 
*/
EXPORT_C CBaseScriptParser2::CBaseScriptParser2(CRegisteredParserDll& aRegisteredParserDll,CMsvEntry& aEntry, RFs& aFs) :
	CActive(EPriorityStandard), 
	iRegisteredParserDll(aRegisteredParserDll), 
	iEntry(aEntry), 
	iFs(aFs), 
	iEntryId(iEntry.Entry().Id())
	{
	}

/** Destructor.

This deletes iSettings and iReadBuffer and calls iRegisteredParserDll.ReleaseLibrary(). 
*/
EXPORT_C CBaseScriptParser2::~CBaseScriptParser2()
	{
	delete iReadBuffer;
	delete iSettings;
	iRegisteredParserDll.ReleaseLibrary();
	}

/** Utility function for unfolding Smart Messages.

Nokia protocol allows for folding of long fields (see Nokia Smart Messaging 
spec 2.0.0pre, 3-34 and RFC822, 3.1.1). This method unfolds the message by 
deleting any linefeed characters which are followed immediately by linear 
white space. It expects the buffer to be in iSmsBuf. 
*/
EXPORT_C void CBaseScriptParser2::UnfoldMessageL()
    {
    // Nokia protocol allows for folding of long fields (see Nokia Smart
    // Messaging spec 2.0.0pre, 3-34 and RFC822, 3.1.1). This method
    // unfolds the message by deleting any linefeed characters which are
    // followed immediately by linear white space.
    //   Note that the value returned by pBuf.Length() will change if
    // linefeeds are deleted. Hence this is called for each iteration to
    // avoid violating buffer bounds.

    TPtr pBuf(iSmsBuf->Des());				// Create modifiable pointer to HBufC

    for (TInt pos = 0; pos < (pBuf.Length() - 1); ++pos)
        {
        // Find linefeed followed by whitespace
        if (pBuf[pos] == KCharLineFeed  &&
            (pBuf[pos+1] == KCharSpace  ||  pBuf[pos+1] == KCharTab))
            {
            pBuf.Delete(pos, 1);
            }
        }
    iSmsBuf = iSmsBuf->ReAllocL(pBuf.Length());		// Reallocate iSmsBuf with new size.
    }

void CBaseScriptParser2::InternalizeL(RMsvReadStream& aReadStream)
	{
	ResetL();
	iParsedFieldArray = new(ELeave) CArrayPtrSeg<CParsedField>(KParsedFieldArrayGranularity);

	CParsedField* parsedField(NULL);
	TInt count = aReadStream.ReadUint8L();
	for (TInt i=0; i < count; ++i)
		{
		parsedField = new (ELeave) CParsedField();
		CleanupStack::PushL(parsedField);
		parsedField->InternalizeL(aReadStream);
		iParsedFieldArray->AppendL(parsedField);
		CleanupStack::Pop(parsedField); 
		}
	}

void CBaseScriptParser2::ExternalizeL(RMsvWriteStream& aStream) const
	{
	TInt count = iParsedFieldArray->Count();
	aStream.WriteInt8L(count);
	for (TInt number = 0; number<count; ++number) // must keep order, go forwards
		{
		aStream << *(*iParsedFieldArray)[number];
		}
	}

/** Stores the parsed fields array.

It stores the iParsedFieldArray array in the specified CMsvStore.

@param aMsvStore Store to write to 
*/
EXPORT_C void CBaseScriptParser2::StoreL(CMsvStore& aMsvStore) const
	{
	RMsvWriteStream out;
	out.AssignLC(aMsvStore, KUidMsvBIODataStream); // pushes 'out' to the stack
	ExternalizeL(out);
	out.CommitL();
	out.Close(); // make sure we close the file
	aMsvStore.CommitL();
	CleanupStack::PopAndDestroy(); // out
	}

/** Restores the parsed fields array.

It restores the iParsedFieldArray array from the specified CMsvStore.

@param aMessageStore Store to read from 
*/
EXPORT_C void CBaseScriptParser2::RestoreL(CMsvStore& aMessageStore)
	{
	RMsvReadStream in;
	in.OpenLC(aMessageStore, KUidMsvBIODataStream);
	InternalizeL(in);
	CleanupStack::PopAndDestroy(); // in
	}

/** Stores the message data in a specified file.

It stores iSettings in the specified file.
Write contents of Smart message to a file as attachment

@param aFileName File to write to 
*/
EXPORT_C void CBaseScriptParser2::StoreL(const TFileName& aFileName) const
	{
	// Get the data contents, stored as 8-bit
	TPtr dataContents = iSettings->Des();
	HBufC8* buf = HBufC8::NewLC(dataContents.Length());
	buf->Des().Copy(dataContents);
	
	// Create the attachment object and copy the data into the file
	CMsvStore* store = iEntry.EditStoreL();
	CleanupStack::PushL(store);
	
	
	MMsvAttachmentManager& manager = store->AttachmentManagerL();
	MMsvAttachmentManagerSync& managerSync = store->AttachmentManagerExtensionsL();
	
	// If an attachment already exists with the same filename then delete
	// the existing one so multiple files of the same name dont exist
	for( TInt ii=0; ii<manager.AttachmentCount(); ++ii )
		{
		CMsvAttachment* attachmentInfo = manager.GetAttachmentInfoL(ii);
		CleanupStack::PushL(attachmentInfo);
		if( attachmentInfo->AttachmentName().CompareF(aFileName) == 0 )
			{
			// We have a match, delete the attachment as we will have
			// to add it as new one
			managerSync.RemoveAttachmentL(ii);
			CleanupStack::PopAndDestroy(attachmentInfo);
			break;
			}
		CleanupStack::PopAndDestroy(attachmentInfo);
		}
		
	// Create and add as a new attachment	
	CMsvAttachment* attachment = CMsvAttachment::NewL(CMsvAttachment::EMsvFile);
	CleanupStack::PushL(attachment);
	attachment->SetSize(buf->Length());
	attachment->SetAttachmentNameL(aFileName);
		
	RFile file;
	managerSync.CreateAttachmentL(aFileName, file, attachment);
	CleanupStack::Pop(attachment); // ownership passed to manager
	CleanupClosePushL(file);
	User::LeaveIfError(file.Write(*buf));
	CleanupStack::PopAndDestroy(&file);
	
	// Commit the changes and cleanup
	store->CommitL();
	CleanupStack::PopAndDestroy(2, buf); // store, buf
	}


/** Restores the message data from a specified file.

It restores iSettings from the specified file.

@param aFileName File to read from 
*/
EXPORT_C void CBaseScriptParser2::RestoreL(const TFileName& aFileName)
	{
	// Find the attachment with the provided file name
	CMsvStore* store = iEntry.ReadStoreL();
	CleanupStack::PushL(store);
	MMsvAttachmentManager& manager = store->AttachmentManagerL();
	
	TInt foundIndex = KErrNotFound;	
	for( TInt ii=0; ii<manager.AttachmentCount(); ++ii )
		{
		CMsvAttachment* attachmentInfo = manager.GetAttachmentInfoL(ii);
		if( attachmentInfo->AttachmentName().CompareF(aFileName) == 0 )
			{
			// found the attachment
			foundIndex = ii;
			delete attachmentInfo;
			break;
			}
		delete attachmentInfo;
		}
	
	// Leave if the attachment file has not been found
	User::LeaveIfError(foundIndex);
	
	// Get the file attachment...
	RFile file = manager.GetAttachmentFileL(foundIndex);
	
	CleanupStack::PopAndDestroy(store);
	
	CleanupClosePushL(file);

	// read into descriptor resizing as we go
	iReadBuffer = HBufC8::NewL(KReadBufSize);
	TBuf8<KReadBufSize> fileChunk;
	TInt fileErr = KErrNone;
	while(fileErr==KErrNone)
		{
		fileErr = file.Read(fileChunk, fileChunk.MaxLength());
		if(fileChunk.Length()==0)
			break;
		// check if we need to resize
		TInt newLength = iReadBuffer->Des().Length() + fileChunk.Length();
		if( newLength > iReadBuffer->Des().MaxLength())
			{
			iReadBuffer = iReadBuffer->ReAllocL(newLength);
			}
		iReadBuffer->Des().Append(fileChunk);
		}
	// if it worked stream data to it
	// first copy data into 8-bit buffer (it's 8bit content anyway?)
	CleanupStack::PopAndDestroy(&file); // file
	User::LeaveIfError(fileErr);

	delete iSettings;
	iSettings = NULL;
	iSettings = HBufC::NewL(iReadBuffer->Length());
	iSettings->Des().Copy(iReadBuffer->Des());
	} 

/** Gets the UID of the BIO message type handled by the parser.

@return BIO message type UID 
*/
EXPORT_C TUid CBaseScriptParser2::ParserUid()
	{
	return (iRegisteredParserDll.UidType()[2]);
	}

/** Deletes the iParsedFieldArray parsed fields array and sets it to NULL. 
*/
EXPORT_C void CBaseScriptParser2::ResetL()
	{
	// delete previous array and create a new one
	if(iParsedFieldArray)
		{
		iParsedFieldArray->ResetAndDestroy();
		delete iParsedFieldArray;
		iParsedFieldArray=NULL;
		}
	}

//
// Class CParsed field container class for 
// data item extracted from smart message
//
/** Constructor. */
EXPORT_C CParsedField::CParsedField()
	{
	iFieldName =NULL;
	iFieldValue =NULL;
	iMandatoryField =ETrue;
	}

/** Destructor. */
EXPORT_C CParsedField::~CParsedField()
	{
	delete iFieldName;
	delete iFieldValue;
	}


/** Gets the field name.

@return Field name 
*/
EXPORT_C TPtrC CParsedField::FieldName() const
	{
	return iFieldName->Des();
	}


/** Sets the field name.

@param aFieldName Field name 
*/
EXPORT_C void CParsedField::SetFieldNameL(const TDesC& aFieldName)
	{
	HBufC* temp =aFieldName.AllocL();
	delete iFieldName;
	iFieldName = temp;	
	}


/** Gets the field value.

@return Field value 
*/
EXPORT_C TPtrC CParsedField::FieldValue() const 
	{
	return iFieldValue->Des();
	}


/** Sets the field value.

@param aFieldValue Field value 
*/
EXPORT_C void CParsedField::SetFieldValueL(const TDesC& aFieldValue)
	{
	HBufC* temp =aFieldValue.AllocL();
	delete iFieldValue;
	iFieldValue = temp;
	}


/** Tests if this is a mandatory field.

@return True if this is a mandatory field 
*/
EXPORT_C TBool CParsedField::MandatoryField() const 
	{
	return iMandatoryField;
	}


/** Sets/unsets this as a mandatory field.

@param aMandatoryField True if this is a mandatory field, false if not 
*/
EXPORT_C void CParsedField::SetMandatoryField(TBool aMandatoryField)
	{
	iMandatoryField = aMandatoryField;
	}

/** Internalises the object.

@param aStream Stream to read from 
*/
EXPORT_C void CParsedField::InternalizeL(RReadStream& aStream)
	{
	Reset();
	delete iFieldName;
	iFieldName = NULL;
	iFieldName = HBufC::NewL(aStream, KMaxStringLength);
	TInt fieldValueLength = aStream.ReadInt16L();
	delete iFieldValue;
	iFieldValue = NULL;
	iFieldValue = HBufC::NewL(aStream, fieldValueLength);
	iMandatoryField = aStream.ReadInt8L();
	}

/** Externalises the object.

@param aStream Stream to write to 
*/
EXPORT_C void CParsedField::ExternalizeL(RWriteStream& aStream) const
	{
	aStream << *iFieldName;
	aStream.WriteUint16L(iFieldValue->Length());
	aStream << *iFieldValue;
	aStream.WriteUint8L(iMandatoryField ? 1 : 0);
	}

//
// Reset contetnts of field
//
void CParsedField::Reset()
	{
	delete iFieldName;
	iFieldName = NULL;

	delete iFieldValue;
	iFieldValue = NULL;

	iMandatoryField= EFalse;
	}
