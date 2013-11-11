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
// BSP.CPP (Base Script Parser)
// 
//

#include "BSP.H"
#include "regpsdll.h"	// CRegisteredParserDll
#include "msventry.h"	// CMsvServerEntry
#include <msvuids.h>	// KUidMsvMessageEntry, KUidMsvServiceEntry
#include <msvids.h>
#include <biouids.h>
#include <biodb.h>

const TInt KMaxStringLength = 1024;

EXPORT_C CBaseScriptParser::CBaseScriptParser(CRegisteredParserDll& aRegisteredParserDll,CMsvServerEntry& aEntry, RFs& aFs)
:CActive(EPriorityStandard), iRegisteredParserDll(aRegisteredParserDll), iEntry(aEntry), iFs(aFs)
/** Constructor.

This is called by CBIOServerMtm to create a parser object.

@param aRegisteredParserDll Object that loaded the parser. It contains a reference 
counter of the use of the parser.
@param aEntry The message entry the parser should parse
@param aFs Connected file server handle */
	{
	iEntryId = iEntry.Entry().Id(); //id of SMS entry
	}

EXPORT_C CBaseScriptParser::~CBaseScriptParser()
/** Destructor.

This deletes iSettings and iReadBuffer and calls iRegisteredParserDll.ReleaseLibrary(). */
	{
	delete iReadBuffer;
	delete iSettings;
	iRegisteredParserDll.ReleaseLibrary();
	}


EXPORT_C void CBaseScriptParser::UnfoldMessageL()
/** Utility function for unfolding Smart Messages.

Nokia protocol allows for folding of long fields (see Nokia Smart Messaging 
spec 2.0.0pre, 3-34 and RFC822, 3.1.1). This method unfolds the message by 
deleting any linefeed characters which are followed immediately by linear 
white space. It expects the buffer to be in iSmsBuf. */
    {
    // Nokia protocol allows for folding of long fields (see Nokia Smart
    // Messaging spec 2.0.0pre, 3-34 and RFC822, 3.1.1). This method
    // unfolds the message by deleting any linefeed characters which are
    // followed immediately by linear white space.
    //   Note that the value returned by pBuf.Length() will change if
    // linefeeds are deleted. Hence this is called for each iteration to
    // avoid violating buffer bounds.

    TPtr pBuf(iSmsBuf->Des());				// Create modifiable pointer to HBufC

    for (TInt pos = 0; pos < (pBuf.Length() - 1); pos++)
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
// end CBaseScriptParser::UnfoldSmsL()


void CBaseScriptParser::InternalizeL(RMsvReadStream& aReadStream)
	{
	ResetL();
	iParsedFieldArray = new(ELeave) CArrayPtrSeg<CParsedField>(16);

	CParsedField* parsedField = NULL;
	TInt count = aReadStream.ReadUint8L();
	for (TInt i=0; i < count; i++)
		{
		parsedField = new (ELeave) CParsedField();
		CleanupStack::PushL(parsedField);
		parsedField->InternalizeL(aReadStream);
		iParsedFieldArray->AppendL(parsedField);
		CleanupStack::Pop(parsedField); 
		}
	}

void CBaseScriptParser::ExternalizeL(RMsvWriteStream& aStream) const
	{
	TInt count = iParsedFieldArray->Count();
	aStream.WriteInt8L(count);
	for(TInt number = 0; number<count; number++)	// must keep order, go forwards
		{
		aStream << *(*iParsedFieldArray)[number];
		}
	}

EXPORT_C void CBaseScriptParser::StoreL(CMsvStore& aMsvStore) const
/** Stores the parsed fields array.

It stores the iParsedFieldArray array in the specified CMsvStore.

@param aMsvStore Store to write to */
	{
	RMsvWriteStream out;
	out.AssignLC( aMsvStore, KUidMsvBIODataStream); // pushes 'out' to the stack
	TRAPD(error, ExternalizeL(out));
	if (error==KErrNone)
		out.CommitL();
	out.Close(); // make sure we close the file
	User::LeaveIfError(error);
	aMsvStore.CommitL();
	CleanupStack::PopAndDestroy();
	}

EXPORT_C void CBaseScriptParser::RestoreL( CMsvStore& aMessageStore )
/** Restores the parsed fields array.

It restores the iParsedFieldArray array from the specified CMsvStore.

@param aMessageStore Store to read from */
	{
	RMsvReadStream in;
	in.OpenLC( aMessageStore, KUidMsvBIODataStream );
	InternalizeL(in);
	CleanupStack::PopAndDestroy();
	}

//
//	write contents of Smart message to a file as attachment
//
EXPORT_C void CBaseScriptParser::StoreL(const TFileName& aFileName) const
/** Stores the message data in a specified file.

It stores iSettings in the specified file.

@param aFileName File to write to */
	{
	//
	TFileName filePath;
	// creates directory for current entry to put file into
	iEntry.GetFilePath(filePath);

	// create the full file name with path
	TParse parse;
	parse.Set(aFileName, &filePath, NULL);
	// create the file
	RFile file;
	User::LeaveIfError(file.Replace(iFs, parse.FullName(), EFileWrite|EFileShareExclusive|EFileStream));
	CleanupClosePushL(file);
	

	// if it worked stream data to it
	// first copy data into 8-bit buffer (it's 8bit content anyway?)
	TPtr dataContents = iSettings->Des();
	HBufC8* buf = HBufC8::NewLC(dataContents.Length());
	buf->Des().Copy(dataContents);

	TInt fileErr = file.Write( buf->Des() );
	file.Close();
	if(fileErr!=KErrNone)
		{
		iFs.Delete(parse.FullName());
		User::Leave(fileErr);
		}
	
	CleanupStack::PopAndDestroy(2); // buf, file
	}


EXPORT_C void CBaseScriptParser::RestoreL( const TFileName& aFileName )
/** Restores the message data from a specified file.

It restores iSettings from the specified file.

@param aFileName File to read from */
	{
	//
	TFileName filePath;
	// creates directory for current entry to put file into
	iEntry.GetFilePath(filePath);

	// create the full file name with path
	TParse parse;
	parse.Set(aFileName, &filePath, NULL);
	// create the file
	RFile file;
	User::LeaveIfError(file.Open(iFs, parse.FullName(), EFileShareExclusive|EFileStream));
	CleanupClosePushL(file);

	// read into descriptor resizing as we go
	iReadBuffer = HBufC8::NewL(256);
	TBuf8<256> fileChunk;
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
	file.Close();
	if(fileErr!=KErrNone)
		User::Leave(fileErr);
	CleanupStack::PopAndDestroy(1); // file
	delete iSettings;
	iSettings = NULL;
	iSettings = HBufC::NewL(iReadBuffer->Length());
	iSettings->Des().Copy(iReadBuffer->Des());
	} 

EXPORT_C TUid CBaseScriptParser::ParserUid()
/** Gets the UID of the BIO message type handled by the parser.

@return BIO message type UID */
	{
	return (iRegisteredParserDll.UidType()[2]);
	}

EXPORT_C void CBaseScriptParser::ResetL()
/** Deletes the iParsedFieldArray parsed fields array and sets it to NULL. */
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
EXPORT_C CParsedField::CParsedField()
/** Constructor. */
	{
	iFieldName =NULL;
	iFieldValue =NULL;
	iMandatoryField =ETrue;
	}

EXPORT_C CParsedField::~CParsedField()
/** Destructor. */
	{
	delete iFieldName;
	delete iFieldValue;
	}


EXPORT_C TPtrC CParsedField::FieldName() const
/** Gets the field name.

@return Field name */
	{
	return iFieldName->Des();
	}


EXPORT_C void CParsedField::SetFieldNameL(const TDesC& aFieldName)
/** Sets the field name.

@param aFieldName Field name */
	{
	HBufC* temp =aFieldName.AllocL();
	delete iFieldName;
	iFieldName = temp;	
	}


EXPORT_C TPtrC CParsedField::FieldValue() const 
/** Gets the field value.

@return Field value */
	{
	return iFieldValue->Des();
	}


EXPORT_C void CParsedField::SetFieldValueL(const TDesC& aFieldValue)
/** Sets the field value.

@param aFieldValue Field value */
	{
	HBufC* temp =aFieldValue.AllocL();
	delete iFieldValue;
	iFieldValue = temp;
	}


EXPORT_C TBool CParsedField::MandatoryField() const 
/** Tests if this is a mandatory field.

@return True if this is a mandatory field */
	{
	return iMandatoryField;
	}


EXPORT_C void CParsedField::SetMandatoryField(TBool aMandatoryField)
/** Sets/unsets this as a mandatory field.

@param aMandatoryField True if this is a mandatory field, false if not */
	{
	iMandatoryField = aMandatoryField;
	}

EXPORT_C void CParsedField::InternalizeL(RReadStream& aStream)
/** Internalises the object.

@param aStream Stream to read from */
	{
	Reset();
	delete iFieldName;
	iFieldName = NULL;
	iFieldName = HBufC::NewL( aStream, KMaxStringLength );
	TInt fieldValueLength = aStream.ReadInt16L();
	delete iFieldValue;
	iFieldValue = NULL;
	iFieldValue = HBufC::NewL( aStream, fieldValueLength );
	iMandatoryField = aStream.ReadInt8L();
	}

EXPORT_C void CParsedField::ExternalizeL(RWriteStream& aStream) const
/** Externalises the object.

@param aStream Stream to write to */
	{
	aStream << *iFieldName;
	aStream.WriteUint16L( iFieldValue->Length() );
	aStream << *iFieldValue;
	aStream.WriteUint8L( iMandatoryField ? 1 : 0);
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

