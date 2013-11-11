// Copyright (c) 1999-2009 Nokia Corporation and/or its subsidiary(-ies).
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
// Compact Business Card Parser for the BIO Messaging project.
// Still in a rudimentary shape, pending specifications and design from Symbian.
// When ParseL is called, the parser parses the data into an array. This array is saved to a store.
// The message is converted to vCard format and then saved as an attachment file. 
// Finally, the message body is converted to an informative message for the user.
// Possibles: May have to implement a ProcessL fn to either save the file in a different directory
// or launch an application after the user presses the accept button in the viewer.
// N.B. The Compact Business Card specification defines the delimiter to be <line-feed>.
// However, it will often be the case that the <line-feed> is preceded by a <carriage-return>.
// The <carriage-return> characters must be stripped out.
// 
//

#include <msvids.h>
#include <msvuids.h>
#include <msventry.h>

#include <bsp.h>
#include "CBCP.H"
#include "CBCPDEF.H"
#ifdef SYMBIAN_ENABLE_SPLIT_HEADERS 
#include "tmsvbioinfo.h"
#endif

// Factory methods
EXPORT_C CBusinessCardParser* CBusinessCardParser::NewL(CRegisteredParserDll& aRegisteredParserDll, CMsvEntry& aEntry, RFs& aFs)
    {
    CBusinessCardParser* self = new (ELeave) CBusinessCardParser(aRegisteredParserDll, aEntry, aFs);
    CleanupStack::PushL(self);
    self->ConstructL();
    CleanupStack::Pop();
    return self;
    }
// end CBusinessCardParser::NewL()


void CBusinessCardParser::ConstructL()
    {
    iParsedFieldArray = new (ELeave) CArrayPtrSeg<CParsedField>(16);
    CActiveScheduler::Add(this);
    }
// end CBusinessCardParser::ConstructL()


void CBusinessCardParser::ParseL(TRequestStatus& aStatus, const TDesC& aSms)
// Doesn't actually do any parsing (see ParseMessageL) but kicks off the parsing process.
    {
    TMsvEntry entry = iEntry.Entry();   //  Get the generic stuff.
    iEntryId = entry.Id();              //  Store the TMsvId.

    __ASSERT_DEBUG((entry.MtmData3() == 0 || entry.MtmData3() == 1),
                            User::Panic(_L("CBCP-DLL"),KErrGeneral));
    //  Already parsed....
    if(entry.MtmData3() == 1)
        {
        iReport = &aStatus;
        User::RequestComplete(iReport, KErrNone);
        }
    //  Not parsed....
    else if(entry.MtmData3() == 0)
        {
        if (iSmsBuf != NULL)
            {
            delete iSmsBuf;
            iSmsBuf = NULL;
            }
        iSmsBuf = aSms.AllocL();            // Allocate new HBufC object.
        ChangeStateL(EUnfoldMessage);       // Unfold message and set thread active.
		aStatus = KRequestPending;
        iReport = &aStatus;
        }
    else
        {
        User::Leave(KErrNotSupported);
        }
    }
// end CBusinessCardParser::ParseL(TRequestStatus&, const TDesC&)


//  ProcessL() --   Business Card parser doesn't use this function at present.
void CBusinessCardParser::ProcessL(TRequestStatus& aStatus)
    {
    iReport = &aStatus;
    User::RequestComplete(iReport, KErrNotSupported);
    }
// end CBusinessCardParser::CommitL(TRequestStatus&)


void CBusinessCardParser::DoCancel()	// Must be provided.
    {
	User::RequestComplete(iReport,KErrCancel);
    }
// end CBusinessCardParser::DoCancel()


void CBusinessCardParser::RunL()	// Must be provided since this is derived from a CActive object.
    {
    iCompleted = iStatus.Int();
    if (iCompleted != KErrNone)
        {
        User::RequestComplete(iReport,iCompleted);
        return;
        }

    switch (iState)
        {
        case EUnfoldMessage:        // Message unfolded so parse it.
            TRAPD(err, ChangeStateL(EParseMessage));
            if (err != KErrNone)
                User::RequestComplete(iReport, err);
            break;

        case EParseMessage:			// Message parsed so update message.
            TRAP(err, ChangeStateL(ECompleteMessage));
            if(err != KErrNone)
                {
                User::RequestComplete(iReport, err);
                }
            break;
                        
        case ECompleteMessage:		// CompleteMessage done. We're finished here.
            User::RequestComplete(iReport, KErrNone);
            break;

        default:
            break;
        }
    }
// end CBusinessCardParser::RunL()


void CBusinessCardParser::ChangeStateL(TParseSession aState)
    {
    iState = aState;				// Increments iState.
    switch (iState)
        {                    
        case EUnfoldMessage:
            UnfoldMessageL();		// Perform MIME unfolding of SMS message.
            break;

        case EParseMessage:
            ParseMessageL();		// Parse message in Tlex object into array.
            break;

        case ECompleteMessage:
            CompleteMessageL();		// Copy message data to child and write new message body.
            break;

        default:
            break;
        }
	RequestComplete(iStatus, KErrNone);		// Go back to ready state.
    SetActive();
    }
// end CBusinessCardParser::ChangeStateL(TParseSession)


void CBusinessCardParser::ParseMessageL()
// Parses Tlex object iSms into array. 
    {
	StripCarriageReturnsL(); // Strip carriage returns from message.
    TPtrC fieldName;
    TPtrC fieldValue;
	TInt i;

	// First line of a CBC is "Business Card". This keyword may not exist if the card is 
	// received over NBS (port 5501 decimal, 157D hex).
	// Need to hunt for keyword. If it does not exist then we assume that it is a valid
	// CBC received over NBS.

	// reset parsedfield array

	if (iParsedFieldArray->Count() > 0)
	        iParsedFieldArray->ResetAndDestroy();

	TLexMark startMark;
	iSms.Mark(startMark);	
	iSms.SkipSpaceAndMark();
	TBool isCompactBusinessCard = EFalse;
	do
		{
		while ( (iSms.Peek() != 'B') && (!iSms.Eos()) ) iSms.Inc();
		if (iSms.Peek() == 'B')						// If the next character is a B then...
			{
			iSms.Mark();
			iSms.SkipCharacters();					// Skip over non-white space (hopefully "Business").
			if (!iSms.Eos()) iSms.Inc();			// Skip over a single white space.
			if (!iSms.Eos()) iSms.SkipCharacters();	// Skip over non-white space (hopefully "Card").
													// N.B. '/n' is a white space character.

			// Extract text from between mark and current position and check that it is as we hope.
			if (iSms.MarkedToken() != KBusinessCardHeader)	// Not keyword so keep looking.
				{
				iSms.UnGetToMark();
				iSms.Inc();
				iSms.Mark();
				}
			else
				isCompactBusinessCard = ETrue;
			}
		else 										// No keyword so received over NBS.
			{
			isCompactBusinessCard = ETrue;
			iSms.UnGetToMark(startMark);
			}
		}
		while (!isCompactBusinessCard);
	// Move on to next line:
	while (iSms.Peek() != KCharLineFeed  &&  !iSms.Eos()) iSms.Inc();
	if (iSms.Eos()) 
		User::Leave(KBspInvalidMessage);					// Leave.
	else
		iSms.SkipAndMark(1);

    // Parse the first three compulsory fields:
    AddRequiredFieldL(KHeaderName);
	AddRequiredFieldL(KHeaderCompany);
    AddRequiredFieldL(KHeaderTitle);

	for (i=0; i<3; i++)
        {
        while (iSms.Peek() != KCharLineFeed  &&  !iSms.Eos())	// Skip to next delimiter.
			iSms.Inc();
        fieldName.Set(iParsedFieldArray->At(i)->FieldName());	// Store (pointer to) field name.
        fieldValue.Set(iSms.MarkedToken());						// Store (pointer to) field value.
		AddParsedFieldL(fieldName, fieldValue, ETrue);			// All fields are mandatory.
        if (!iSms.Eos()) 
			iSms.SkipAndMark(1);								// Move on to next line.
		else
			User::Leave(KBspInvalidMessage);	// Leave - not long enough for valid CBC.
        }
	iNumberOfCbcFields = i;

    // Now parse the fax and telephone fields:
	// The first telephone field must be the fourth line of the compact-card-body. It must
	// either be empty or start with "tel". Subsequent telephone lines must also start with
	// "tel", otherwise we move on to the fax line(s).
	iNumberOfTel = 0;
	AddRequiredFieldL(KHeaderPhone);
	if (iSms.Peek() != KCharLineFeed)	// We have a potential phone number.
		{
		if ( iSms.Peek() != 't' || iSms.Remainder().Length() < 4 ) 
			User::Leave(KBspInvalidMessage);		// No valid phone number line.
		do	{
			iSms.Inc(3);
			if (iSms.MarkedToken().Compare(_L("tel")) != 0 && iNumberOfTel == 0) 
				User::Leave(KBspInvalidMessage);	// No valid phone number line.
			else if (iSms.MarkedToken().Compare(_L("tel")) != 0)
				{									// Not a valid phone number line.
				iSms.UnGetToMark();					// Go back to start of line.
				break;								// Exit; go on to fax number lines
				}
			else if (iSms.MarkedToken().Compare(_L("tel")) == 0 && iNumberOfTel > 0) 
				{									// Additional phone number.
				TBuf<8> aPhoneHeader;
				aPhoneHeader.Append(KHeaderPhone);
				TBuf<3> aNumber;
				aNumber.NumFixedWidth(++iNumberOfTel,EDecimal,3);
				aPhoneHeader.Append(aNumber);
				AddRequiredFieldL(aPhoneHeader);	
				}
			else
				iNumberOfTel = 1;					// First phone number
			for (;;)
				{
				if (IsValidTelephoneCharacter())
					{
					iSms.Mark();
					while (iSms.Peek() != KCharLineFeed  &&  !iSms.Eos()) iSms.Inc();
					i = iNumberOfCbcFields + iNumberOfTel - 1;
					fieldName.Set(iParsedFieldArray->At(i)->FieldName());// Store (pointer to) field name.
					fieldValue.Set(iSms.MarkedToken());					// Store (pointer to) field value.
					AddParsedFieldL(fieldName, fieldValue, ETrue);		// All fields are mandatory.
					iSms.SkipAndMark(1);								// Move on to next line.
					break;
					}
				else if (iSms.Peek() == KCharLineFeed)
					{
					iSms.SkipAndMark(1);	// Move on to next line.
					break;
					}
				else if (iSms.Eos()) 
					User::Leave(KBspInvalidMessage);// Leave - not long enough for valid CBC.
				else
					iSms.Inc();							// Skip to next digit.
				}
			}
		while (iSms.Peek() == 't' && iSms.Remainder().Length() >= 3);
		}
	else
		{
		iSms.SkipAndMark(1);					// No phone number - move on to next line.
		iNumberOfTel = 1;						// One (empty) phone number field.
		}

	// The first fax field must be the next line of the compact-card-body. It must either
	// be empty or start with "fax". Subsequent fax number lines must also start with "fax". 
    AddRequiredFieldL(KHeaderFax);
	iNumberOfFax = 0;
	if (iSms.Peek() != KCharLineFeed)	// We have a potential fax number.
		{
		if ( iSms.Peek() != 'f' || iSms.Remainder().Length() < 4 ) 
			User::Leave(KBspInvalidMessage);		// No valid fax number line.
		do	{
			iSms.Inc(3);
			if (iSms.MarkedToken().Compare(_L("fax")) != 0 && iNumberOfFax == 0) 
				User::Leave(KBspInvalidMessage);	// No valid fax number line.
			else if (iSms.MarkedToken().Compare(_L("fax")) != 0)
				{									// Not a valid fax number line.
				iSms.UnGetToMark();					// Go back to start of line.
				break;								// Exit; go on to email line
				}
			else if (iSms.MarkedToken().Compare(_L("fax")) == 0 && iNumberOfFax > 0) 
				{									// Additional fax number.
				TBuf<6> aFaxHeader;
				aFaxHeader.Append(KHeaderFax);
				TBuf<3> aNumber;
				aNumber.NumFixedWidth(++iNumberOfFax,EDecimal,3);
				aFaxHeader.Append(aNumber);
				AddRequiredFieldL(aFaxHeader);	
				}
			else
				iNumberOfFax = 1;					// First fax number
			for (;;)								// Could use a switch.
				{
				if (IsValidTelephoneCharacter())
					{
					iSms.Mark();
					while (iSms.Peek() != KCharLineFeed  &&  !iSms.Eos()) iSms.Inc();
					i = iNumberOfCbcFields + iNumberOfTel + iNumberOfFax - 1;
					fieldName.Set(iParsedFieldArray->At(i)->FieldName());// Store (pointer to) field name.
					fieldValue.Set(iSms.MarkedToken());					// Store (pointer to) field value.
					AddParsedFieldL(fieldName, fieldValue, ETrue);		// All fields are mandatory.
					iSms.SkipAndMark(1);					// Move on to next line.
					break;
					}
				else if (iSms.Peek() == KCharLineFeed)
					{
					iSms.SkipAndMark(1);	// Move on to next line.
				break;
					}
				else if (iSms.Eos()) 
					User::Leave(KBspInvalidMessage);// Leave - not long enough for valid CBC.
				else
					iSms.Inc();							// Skip to next digit.
				}
			}
		while (iSms.Peek() == 'f' && iSms.Remainder().Length() >= 3);
		}
	else
		{
		iSms.SkipAndMark(1);				// No fax number - move on to next line.
		iNumberOfFax = 1;					// One (empty) fax number field.
		}

    // Now parse the last six compulsory fields:
	iNumberOfCbcFields += (iNumberOfFax + iNumberOfTel);
    AddRequiredFieldL(KHeaderEmail);
    AddRequiredFieldL(KHeaderStreet1);
    AddRequiredFieldL(KHeaderStreet2);
    AddRequiredFieldL(KHeaderStreet3);
    AddRequiredFieldL(KHeaderStreet4);
    AddRequiredFieldL(KHeaderStreet5);
	for (i=iNumberOfCbcFields; i<iNumberOfCbcFields+6; i++)
        {
		while (!iSms.Eos() && iSms.Peek() != KCharLineFeed) iSms.Inc();	// Skip to next delimiter.
        fieldName.Set(iParsedFieldArray->At(i)->FieldName());	// Store (pointer to) field name.
        fieldValue.Set(iSms.MarkedToken());						// Store (pointer to) field value.
		AddParsedFieldL(fieldName, fieldValue, ETrue);			// All fields are mandatory.
        if (!iSms.Eos()) 
			{
			iSms.SkipAndMark(1);					// Move on to next line.
			}
		else
			i=iNumberOfCbcFields+6;								// Exit FOR loop.
        }
	iNumberOfCbcFields = i;

	// Throw any extra lines into the note field:
	if (!iSms.Eos())
		{
		AddRequiredFieldL(KHeaderExtra);
		iNumberOfCbcFields++;
		while (!iSms.Eos()) iSms.Inc();					// Include remainder of text.
		TInt notesLength = iSms.TokenLength();
		iSms.UnGetToMark();
		delete iNotesBuffer;
		iNotesBuffer = NULL;
		iNotesBuffer=HBufC::NewLC(notesLength);			// Buffer large enough for remainder of text.
		TPtr notesPtr = iNotesBuffer->Des();

		iSms.SkipSpaceAndMark();						
		// Need to strip out line feeds:
		while (!iSms.Eos())		
			{
			while (!iSms.Eos() && iSms.Peek() != KCharLineFeed) iSms.Inc();
			notesPtr.Append(iSms.MarkedToken());
			notesPtr.Append(KCharComma);				// Delimit data with commas.
			if (!iSms.Eos()) iSms.SkipSpaceAndMark();
			}

		notesLength = notesPtr.Length();				// Reduce size of buffer to minimum.
		iNotesBuffer = iNotesBuffer->ReAllocL(notesLength);

		fieldValue.Set(iNotesBuffer->Des());			// Store (pointer to) field value
		fieldName.Set(KHeaderExtra);					// Store (pointer to) field name
		AddParsedFieldL(fieldName, fieldValue, EFalse);	// This field is optional.
		CleanupStack::Pop();							// iNotesBuffer (deleted in destructor).
		}
   }
// end CBusinessCardParser::ParseMessageL


void CBusinessCardParser::StripCarriageReturnsL()
	{
	// Strips out the carriage returns; they shouldn't be there anyway!
	TPtr smsPtr = iSmsBuf->Des();
    for (TInt pos = 0; pos < (smsPtr.Length() - 1); pos++)
        if (smsPtr[pos] == KCharCarriageReturn)
            smsPtr.Delete(pos, 1);
    iSmsBuf = iSmsBuf->ReAllocL(smsPtr.Length());			// Reallocate iSmsBuf with new size.
	iSms = iSmsBuf->Des();						// Initialise Tlex object.
	}
// end CBusinessCardParser::StripCarriageReturnsL


void CBusinessCardParser::CompleteMessageL()
// Streams the contents of iParsedFieldsArray as a child entry of the original message.
// Converts data to vCard format and saves to a file.
// If this returns OK sets the iMtmData3 flag to 1 (parsed) and writes a new message body into the SmartMessage.
    {
	// Create new stream (containing parsed data) within store associated with message entry:
	iEntry.SetEntryL(iEntryId);						// Changes the entry used as the Message Server context.
	CMsvStore* store=iEntry.EditStoreL();			// Returns the message store for the current context with write access.
													// 'store' is the 'active object' responsible for the request.
	CleanupStack::PushL(store);
	StoreL(*store);									// Store the parsed fields into a message stream.
	CleanupStack::PopAndDestroy();	

	// Change Entry Body text to vCard and save to a file:
    iEntry.SetEntryL(iEntryId);						// Changes the entry used as the Message Server context.

	CreateDataL();
    
	TBuf<64> tempBuf;
	tempBuf.Format(_L("%x"), iEntry.Entry().Id());
	tempBuf.Append(KVCardExt);
	StoreL(TFileName(tempBuf));	// Pass the filename to base class method - creates a new folder 
								// linked to message entry and stores body as an 8 bit attachment file.
    
	TMsvEntry entry= iEntry.Entry();				// 'entry' is the 'active object' responsible for the request.

    entry.SetMtmData3(1);							// Indicates that we've parsed it.
	// 
	iEntry.ChangeL(entry);										// Changes the current context.
    }
// end CBusinessCardParser::CompleteMessageL()

void CBusinessCardParser::AddRequiredFieldL(const TDesC& aFieldName)
    {
    // Create new CParsedField object on heap:
    CParsedField* parsedField = new (ELeave) CParsedField;
	CleanupStack::PushL(parsedField);
	
	// Initialise field name:
    parsedField->SetFieldNameL(aFieldName);
    parsedField->SetFieldValueL(KInitialFieldValue);
    parsedField->SetMandatoryField(EFalse);

    // Add pointer to array:
    iParsedFieldArray->AppendL(parsedField);
    CleanupStack::Pop(parsedField);	// No need to delete the CParsedField object, since it's now owned by the array.
    }
// end CBusinessCardParser::AddRequiredFieldL(const TDesC&)


void CBusinessCardParser::CreateDataL()
	{
	delete iSettings;
	iSettings = HBufC::NewL(512);

	// Write vCard to iSettings:
	AppendItemL( KVCardHeader);		// Header.
	AppendItemL( KVCardVersion);		// Version of vCard spec.
	AppendItemL( KVCardFormattedName);	// Formatted name.
	AppendItemL( iParsedFieldArray->At(0)->FieldValue());

	// Parse out name into vCard semicolon delimited fields:
	// The Nokia Smart Messaging Specification, Revision 2.0.0, Section 3.3.1 suggests that the name
	// should be in the format "last-name first-name". The following code assumes that this convention
	// is used. A single name is assumed to be a last name. If there are more than two words then the 
	// remaining text is assumed to be additional names.
	AppendItemL( KVCardName);			// Name, semicolon delimited.
	TLex iField = iParsedFieldArray->At(0)->FieldValue();				// Put name into a TLex object
    iField.SkipSpace();													// Last name.
	if (!iField.Eos())
		{
		iField.Mark();
		iField.SkipCharacters();
		AppendItemL( iField.MarkedToken());
		}
	AppendItemL( KCharSemiColon );	
    iField.SkipSpace();													// First name.
	if (!iField.Eos())
		{
		iField.Mark();
		iField.SkipCharacters();
		AppendItemL( iField.MarkedToken());
		}
	AppendItemL( KCharSemiColon);			
    iField.SkipSpace();												
	AppendItemL( iField.Remainder());	// Additional names.
	AppendItemL( KCharSemiColon);		// Honourific prefices.
	AppendItemL( KCharSemiColon);		// Honourific suffices.	
	
	AppendItemL( KVCardAddress);		// Address, semicolon delimited.
	TInt addressOffset = iNumberOfFax + iNumberOfTel + KTelOffset;		// Offset of address lines from start of cbc.
	TInt i;
	for (i=0; i<KNumberOfAddressLines; i++)
		{
		AppendItemL( KCharSemiColon);
		AppendItemL( iParsedFieldArray->At(addressOffset+i)->FieldValue());
		}

	// note info is optional - so append if it exists

	if (addressOffset+KNumberOfAddressLines < iParsedFieldArray->Count())
		{
		// Extra gubbins tacked to end of notes field, after address:
		AppendItemL( KVCardNote);			
		AppendItemL( 
			iParsedFieldArray->At(addressOffset+KNumberOfAddressLines)->FieldValue());
		}

	// Telephone numbers:
	for (i=KTelOffset-1; i<iNumberOfTel+KTelOffset-1; i++)					
		{
		AppendTelItemL(iParsedFieldArray->At(i)->FieldValue(), KVCardTel, KVCardTelCell);
		}		

	// Fax numbers:
	for (; i<iNumberOfFax+iNumberOfTel+KTelOffset-1; i++)									
		{
		AppendTelItemL(iParsedFieldArray->At(i)->FieldValue(), KVCardFax, KVCardFaxCell);
		}		

	// Other stuff:
	AppendItemL( KVCardEmail);			// Email address.
	AppendItemL( 
		iParsedFieldArray->At(iNumberOfFax+iNumberOfTel+KTelOffset-1)->FieldValue());
	AppendItemL( KVCardTitle);			// Title (job description).
	AppendItemL( iParsedFieldArray->At(2)->FieldValue());
	AppendItemL( KVCardOrg);			// Organisation: company.
	AppendItemL( iParsedFieldArray->At(1)->FieldValue());
	AppendItemL( KVCardFooter);		// End of vCard.
	}

void CBusinessCardParser::AppendTelItemL(const TDesC& aTel, const TDesC& aDefaultLabel, const TDesC& aCellLabel)
	{
	// We must check for the prefix (GSM) to see if the number should 
	// be stored in the cellphone field instead

	// Store the field for the first number and check for the (GSM) prefix

	TLex telLex(aTel); 
	const TPtrC prefixCell(KVPrefixCell);
	const TInt prefixCellLen = prefixCell.Length();

	TPtrC label(aDefaultLabel);
	TPtrC value(aTel);

	if (aTel.Length() > prefixCellLen && telLex.Peek() == TChar(*prefixCell.Ptr()))
		{
		telLex.Inc(prefixCellLen);
		if(telLex.MarkedToken().CompareF(prefixCell) == 0) 		// If there is a GSM prefix create as cellphone number
			{
			telLex.SkipSpaceAndMark();
			label.Set(aCellLabel);
			value.Set(telLex.RemainderFromMark());
			}
		}

	AppendItemL(label);
	AppendItemL(value);
	}

void CBusinessCardParser::AppendItemL(const TDesC& aItem)
	{
	TInt newLength = iSettings->Des().Length() + aItem.Length();
	if(iSettings->Des().MaxLength() < newLength)
		{
		iSettings = iSettings->ReAllocL(newLength);
		}
	TPtr des = iSettings->Des();
	des.Append(aItem);
	}

void CBusinessCardParser::AddParsedFieldL(const TDesC& aFieldName,
                                              const TDesC& aFieldValue,
                                              TBool aMandatory)
    {
    // This method is responsible for adding field values found by the parser to the array of parsed fields.
    // It enforces the "each field once only" rule (see Nokia Smart Messaging spec, 2.0.0pre, 3-34).
    for (TInt i = 0; i < iParsedFieldArray->Count(); i++)
        {
        // Use CompareF() to perform case-insensitive match:
        if (aFieldName.CompareF(iParsedFieldArray->At(i)->FieldName()) == 0)
            {
            // Field name exists, so insert a value:
            iParsedFieldArray->At(i)->SetFieldValueL(aFieldValue);
            iParsedFieldArray->At(i)->SetMandatoryField(aMandatory);
            return;
            }
        }
    }
// end CBusinessCardParser::AddParsedField(const TDesC&, const TDesC&, TBool)


void CBusinessCardParser::RequestComplete(TRequestStatus& aStatus, TInt aError)
    {
    TRequestStatus* p = &aStatus;
    User::RequestComplete(p, aError);
    }
// end CBusinessCardParser::RequestComplete(TRequestStatus&, TInt)

TBool CBusinessCardParser::IsValidTelephoneCharacter() const
	{
	const TChar ch = iSms.Peek();

	if (ch.IsDigit() ||
		ch == '(' ||
		ch == '-' ||
		ch == '*' ||
		ch == '#' ||
		ch == 'w' ||
		ch == 'W' ||
		ch == 'p' ||
		ch == 'P' ||
		ch =='+')
		return ETrue;
	else
		return EFalse;
	}

// Constructor
CBusinessCardParser::CBusinessCardParser(CRegisteredParserDll& aRegisteredParserDll, CMsvEntry& aEntry, RFs& aFs)
: CBaseScriptParser2(aRegisteredParserDll, aEntry, aFs) 
    {
    }
// end CBusinessCardParser::CBusinessCardParser()


CBusinessCardParser::~CBusinessCardParser()
    {
    if (iSmsBuf != NULL)
        {
        delete iSmsBuf;
        }
    if (iParsedFieldArray != NULL)
        {
        iParsedFieldArray->ResetAndDestroy();
        delete iParsedFieldArray;
		delete iNotesBuffer;
        }
    }
// end CBusinessCardParser::~CBusinessCardParser()
