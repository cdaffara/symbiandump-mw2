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
//

#include <msvids.h>
#include <msvuids.h>
#include <msvapi.h>
#include <cemailaccounts.h>

#include "ENP.H"
#include <pop3set.h>
#include <imapset.h>
#include <miutconv.h>
#include <e32math.h>

#ifdef SYMBIAN_ENABLE_SPLIT_HEADERS 
#include "tmsvbioinfo.h"
#endif

// Parse constants
_LIT(KEmailHeader,"//MLAP11");
#define KCharColon          ':'
#define KCharSpace          ' '
#define KCharTab            '\t'
#define KCharLineFeed       '\n'
#define KCharSlash			'/'

// Header fields defined in Nokia Smart Messaging Specification grammar (3.9.2.1).
// N.B. If new fields are added, (cf. <extension-field>) PrintFieldToRichTextL()
// will need to be changed accordingly.
_LIT(KHeaderNumberMessages, "msgcount");
_LIT(KHeaderFrom,"from");
_LIT(KHeaderSubject,"subject");
_LIT(KHeaderSize,"size");
_LIT(KHeaderUidImap,"iuid");
_LIT(KHeaderUidPop,"puid");
_LIT(KHeaderServerId,"sid");
_LIT(KHeaderAttachments,"att");
_LIT(KHeaderTo,"to");
_LIT(KHeaderCc,"cc");
_LIT(KHeaderDate,"date");
_LIT(KHeaderFolder,"fldr");
_LIT(KHeaderSender,"sender");
_LIT(KHeaderReplyTo,"reply-to");
_LIT(KHeaderUidValidity,"uidv");
_LIT(KInitialFieldValue,"");

#define KUidCharsetISO88591         0x10003b10
#define	KServerIdModVal				512

//
// Factory methods
//
EXPORT_C CEmailNotificationParser* CEmailNotificationParser::NewL(CRegisteredParserDll& aRegisteredParserDll, CMsvEntry& aEntry, RFs& aFs)
    {
    CEmailNotificationParser* self = new (ELeave) CEmailNotificationParser(aRegisteredParserDll, aEntry, aFs);
    CleanupStack::PushL(self);
    self->ConstructL();
    CleanupStack::Pop();
    return self;
    }
// end CEmailNotificationParser::NewL()


void CEmailNotificationParser::ConstructL()
    {
    iParsedFieldArray = new (ELeave) CArrayPtrSeg<CParsedField>(16);

    // Initialise array with required field names
    // N.B. This dictates the order in which the headers will appear in the message body.
    AddRequiredFieldL(KHeaderNumberMessages);
	AddRequiredFieldL(KHeaderFrom);
    AddRequiredFieldL(KHeaderSubject);
    AddRequiredFieldL(KHeaderTo);
    AddRequiredFieldL(KHeaderDate);
	AddRequiredFieldL(KHeaderAttachments);
	AddRequiredFieldL(KHeaderCc);
    AddRequiredFieldL(KHeaderSize);
    AddRequiredFieldL(KHeaderServerId);
    AddRequiredFieldL(KHeaderUidImap);
    AddRequiredFieldL(KHeaderUidPop);
	//The following fields added to fix defect INC023328 
	AddRequiredFieldL(KHeaderFolder);
    AddRequiredFieldL(KHeaderSender);
    AddRequiredFieldL(KHeaderReplyTo);
    AddRequiredFieldL(KHeaderUidValidity);

	iFoundServer = EFalse;

    CActiveScheduler::Add(this);
    }
// end CEmailNotificationParser::ConstructL()

void CEmailNotificationParser::ParseL(TRequestStatus& aStatus, const TDesC& aSms)
    {
    TMsvEntry entry = iEntry.Entry();   //  Get the generic stuff
    iEntryId = entry.Id();              //  store the TMsvId

    __ASSERT_DEBUG((entry.MtmData3() == 0 || entry.MtmData3() == 1),
                            User::Panic(_L("ENP-DLL"),KErrCorrupt));
    //  Already parsed....
    if(entry.MtmData3() == 1)
        {
        iReport = &aStatus;
        User::RequestComplete(iReport, KErrNone);
        }
    //  not parsed
    else if(entry.MtmData3() == 0)
        {
        if (iSmsBuf != NULL)
            {
            delete iSmsBuf;
            iSmsBuf = NULL;
            }
        iSmsBuf = aSms.AllocL();            // Allocate new HBufC object
        ChangeStateL(EUnfoldMessage);       //Set to initial request
        aStatus = KRequestPending;
        iReport = &aStatus;
        }
    else
        {
        User::Leave(KErrNotSupported);
        }
    }
// end CEmailNotificationParser::ParseL(TRequestStatus&, const TDesC&)



//
//  ProcessL() --    Email notification parser doesn't actually commit
//                  any information to persistant store other than the 
//                  iParsedFieldsArray data as part of the parse method
//                  .. Complete  KErrNotSupported..
//
void CEmailNotificationParser::ProcessL(TRequestStatus& aStatus)
    {
    iReport = &aStatus;
    User::RequestComplete(iReport, KErrNotSupported);
    }
// end CEmailNotificationParser::CommitL(TRequestStatus&)


void CEmailNotificationParser::DoCancel()
    {
	User::RequestComplete(iReport,KErrCancel);
    }
// end CEmailNotificationParser::DoCancel()


void CEmailNotificationParser::RunL()
    {
    iCompleted = iStatus.Int();
    if (iCompleted != KErrNone)
        {
        User::RequestComplete(iReport,iCompleted);
        return;
        }

    switch (iState)
        {
        case EUnfoldMessage:        // UnfoldSmsL completed
            TRAPD(err, ChangeStateL(EParseMessage));
            if (err != KErrNone)
                User::RequestComplete(iReport, err);
            break;

        case EParseMessage:
            TRAP(err, ChangeStateL(ECompleteMessage));
            if(err != KErrNone)
                {
                User::RequestComplete(iReport, err);
                }
            break;
                        
        case ECompleteMessage:
            // CompleteMessage completed
            User::RequestComplete(iReport, KErrNone);
            break;

        default:
            break;
        }
    }
// end CEmailNotificationParser::RunL()


void CEmailNotificationParser::ChangeStateL(TParseSession aState)
    {
    iState = aState;
    switch (iState)
        {                    
        case EUnfoldMessage:
            UnfoldMessageL();
            iSms = iSmsBuf->Des();      // initialise Tlex object
            break;

        case EParseMessage:
            ParseMessageL();
            break;

        case ECompleteMessage:
            CompleteMessageL();
            break;

        default:
            break;
        }
	RequestComplete(iStatus, KErrNone);

    SetActive();
    }
// end CEmailNotificationParser::ChangeStateL(TParseSession)


void CEmailNotificationParser::UnfoldMessageL()
    {
    // Nokia protocol allows for folding of long fields (see Nokia Smart
    // Messaging spec 2.0.0pre, 3-34 and RFC822, 3.1.1). This method
    // unfolds the message by deleting any linefeed characters which are
    // followed immediately by linear white space.
    //   Note that the value returned by pBuf.Length() will change if
    // linefeeds are deleted. Hence this is called for each iteration to
    // avoid violating buffer bounds.

    TPtr pBuf(iSmsBuf->Des());     // Create modifiable pointer to HBufC

    for (TInt pos = 0; pos < (pBuf.Length() - 1); pos++)
        {
        // Find linefeed followed by whitespace
        if (pBuf[pos] == KCharLineFeed  &&
            (pBuf[pos+1] == KCharSpace  ||  pBuf[pos+1] == KCharTab))
            {
            pBuf.Delete(pos, 1);
            }
        }

    // Reallocate iSmsBuf with new size
    iSmsBuf = iSmsBuf->ReAllocL(pBuf.Length());
    }
// end CEmailNotificationParser::UnfoldSmsL()

//
//  ParseMessageL()
//
//  Simple Notification:
//      Looks for number of messages and "From:" field
//      writes numMsg into the parsed field array, sets
//      entry.iDetails to the "From:" 
//  Complicated Notification:
//      does the above and writes everything else into
//      the array....
//
void CEmailNotificationParser::ParseMessageL()
    {
    TBool isSimpleNotification = EFalse; 
    TLex  tokenLex;
    TPtrC fieldName;
    TPtrC fieldValue;

	// reset parsedfield array
	for(TInt i = iParsedFieldArray->Count();--i>=0;)
		(*iParsedFieldArray)[i]->SetFieldValueL(_L(""));

    // Set extraction mark at first character
    iSms.SkipSpaceAndMark();
	if(iSms.Peek() == KCharSlash)
		{
		// Check first line is <header>
		iSms.SkipCharacters();

		if (iSms.MarkedToken() != KEmailHeader)
			{
			User::Leave(KBspInvalidMessage);
			}
		// Get <new-amount> from second line and check for terminating linefeed
		iSms.SkipSpaceAndMark();
		}

    // Val() seeks forward from next char position, looking for valid
    // digits and incrementing next char as it goes.
    if (!(iSms.Val(iMessageCount) == KErrNone  &&  iMessageCount >= 0))     // If marked token is not a valid positive integer
        {
        iMessageCount = 0;
        User::Leave(KBspInvalidMessage);
        }
    else
        {
        fieldValue.Set(iSms.MarkedToken());                            //  The message count..
        AddParsedFieldL(KHeaderNumberMessages, fieldValue, ETrue);
        }

    // Next character may now be at newline or space after integer.
    // If at space, advance to newline.
    while (iSms.Peek() != KCharLineFeed && !iSms.Eos())
        iSms.Inc();

    iSms.SkipSpaceAndMark();

    // Now parse the rest of the fields, if any.
    while (!iSms.Eos())
        {
        while (iSms.Peek() != KCharLineFeed  &&  !iSms.Eos())
            iSms.Inc();     //  Skip to next delimiter

        if (iSms.Eos())
            break;          //  we've finished break out of the function

        if (iSms.TokenLength() == 0)
            User::Leave(KBspSmartMessageInvalidToken);

        //  Parsing....
        tokenLex.Assign(iSms.MarkedToken());        // Assign token to a new TLex
        while (tokenLex.Peek() != KCharColon  &&  !tokenLex.Eos())
            tokenLex.Inc();     //  Advance to a ':'

        if (tokenLex.Eos()  ||  tokenLex.TokenLength() == 0)
            User::Leave(KBspSmartMessageInvalidToken);


        fieldName.Set(tokenLex.MarkedToken());      // Store (pointer to) field name
        tokenLex.Inc();

		//fix for DEF017686
		LeaveIfEmptyFieldsL(fieldName,tokenLex);
		
        
        tokenLex.SkipSpaceAndMark();                // Step past optional spaces

		//fix for DEF017686
        LeaveIfEmptyFieldsL(fieldName,tokenLex);
	
		// if it's the server id field try to extract the id value
		// and match to an existing email service
		if(fieldName.CompareF(KHeaderServerId)==KErrNone)
			{
			TInt valErr = tokenLex.Val(iServerId);
			if(valErr != KErrNone)
				iServerId = 0;
			else
				GetEmailServicesL();

			tokenLex.UnGetToMark();
			}	
		fieldValue.Set(tokenLex.Remainder());       // Store (pointer to) field value

        if(!isSimpleNotification)
            {
            AddParsedFieldL(fieldName, fieldValue, EFalse);
            }		

        // Successfully parsed a token. Move iSms's next character past the
        // (linefeed) delimiter, set the extraction mark and reiterate to
        // look for next token.
        iSms.SkipSpaceAndMark();
        }
    }
// end CEmailNotificationParser::ParseMessageL()


//fix for DEF017686: EMail notification parser leaves when field data is empty  
void CEmailNotificationParser::LeaveIfEmptyFieldsL(const TDesC& aFieldName,
												   const TLex& aTokenLex)
	{
/*
 		The following  checks for an empty field value.
 		The only fields allowed to be empty are the subject  and extension fields.
 		Hence, the check is done when aFieldName is not  a subject field and an extension field.
 		However, it is not possible to explicitly check for an extension field as we don't 
 		know which strings to compare. Therefore, a comparison with all the known field names
 		must be performed instead. The risk is fairly small as the change is localised and it can 
 		be easily tested.
*/

	if ((aFieldName.CompareF(KHeaderFrom)==KErrNone) ||
 		(aFieldName.CompareF(KHeaderSize)==KErrNone) ||
		(aFieldName.CompareF(KHeaderUidImap)==KErrNone) ||
 		(aFieldName.CompareF(KHeaderUidPop)==KErrNone) ||
 		(aFieldName.CompareF(KHeaderServerId)==KErrNone) ||
 		(aFieldName.CompareF(KHeaderAttachments)==KErrNone) ||
 		(aFieldName.CompareF(KHeaderTo)==KErrNone) ||
 		(aFieldName.CompareF(KHeaderCc)==KErrNone) ||
 		(aFieldName.CompareF(KHeaderDate)==KErrNone) ||
 		(aFieldName.CompareF(KHeaderFolder)==KErrNone) ||
 		(aFieldName.CompareF(KHeaderSender)==KErrNone) ||
 		(aFieldName.CompareF(KHeaderReplyTo)==KErrNone) ||
 		(aFieldName.CompareF(KHeaderUidValidity)==KErrNone) //not sure if this actually exists*/
	   )
 		
 		if (aTokenLex.Eos())
 			User::Leave(KBspSmartMessageInvalidToken);	
	}


//
//  CompleteMessageL()
//
//  -   Function streams the contents of iParsedFieldsArray
//      as a child entry of the original message. If this
//      returns OK sets the iMtmData3 flag to 1 (parsed) and
//      writes a new message body into the SmartMessage
//
//  -   If externalization fails the function sets the message
//      not parsed and does not alter the message body.
//
void CEmailNotificationParser::CompleteMessageL()
    {
	//Create new stream (containing parsed data) within CMsvStore associated with message entry
	iEntry.SetEntryL(iEntryId);
	CMsvStore* store=iEntry.EditStoreL();
	CleanupStack::PushL(store);
	StoreL(*store);
	CleanupStack::PopAndDestroy();

	TMsvEntry entry= iEntry.Entry();
    entry.SetMtmData3(1);                                //  Indicates that we've parsed it..

	// set iRelated id if we know which server the message corresponds to
	// iRelatedId is set to the id of the relevant service entry.
	if(iFoundServer)
		entry.iRelatedId = iServiceMsvId;
	iEntry.ChangeL(entry);
    }
// end CEmailNotificationParser::CompleteMessageL()

void CEmailNotificationParser::AddRequiredFieldL(const TDesC& aFieldName)
    {
    // Create new CParsedField object on heap
    CParsedField* parsedField = new (ELeave) CParsedField;
	CleanupStack::PushL(parsedField);
    // Initialise field name
    parsedField->SetFieldNameL(aFieldName);
    parsedField->SetFieldValueL(KInitialFieldValue);
    parsedField->SetMandatoryField(EFalse);

    // Add pointer to array
    iParsedFieldArray->AppendL(parsedField);
    CleanupStack::Pop();
    // No need to delete the CParsedField object, since it's now owned by the array
    }
// end CEmailNotificationParser::AddRequiredFieldL(const TDesC&)


void CEmailNotificationParser::AddParsedFieldL(const TDesC& aFieldName,
                                              const TDesC& aFieldValue,
                                              TBool aMandatory)
    {
    // This method is responsible for adding field values found
    // by the parser to the array of parsed fields. It also
    // enforces the "each field once only" rule (see Nokia Smart
    // Messaging spec, 2.0.0pre, 3-34).
    for (TInt i = 0; i < iParsedFieldArray->Count(); i++)
        {
        // Use CompareF() to perform case-insensitive match
        if (aFieldName.CompareF(iParsedFieldArray->At(i)->FieldName()) == 0)
            {
            // Field name exists, so insert a value
            iParsedFieldArray->At(i)->SetFieldValueL(aFieldValue);
            iParsedFieldArray->At(i)->SetMandatoryField(aMandatory);
            iExtendedNotification = ETrue;
			return;
            }
        }

	// This has been added to fix defect INC38950
	// If we have got here, then we have a new field, not specified by one of the fields in iParseFieldArray
    CParsedField* parsedField = new (ELeave) CParsedField;
	CleanupStack::PushL(parsedField);
    parsedField->SetFieldNameL(aFieldName);
    parsedField->SetFieldValueL(aFieldValue);
    parsedField->SetMandatoryField(EFalse);
    iParsedFieldArray->AppendL(parsedField);
    CleanupStack::Pop(parsedField);
	}
// end CEmailNotificationParser::AddParsedFieldL(const TDesC&, const TDesC&, TBool)

//
// Get all email services
//
void CEmailNotificationParser::GetEmailServicesL()
	{

	CCnvCharacterSetConverter* charconv = CCnvCharacterSetConverter::NewL();
	CleanupStack::PushL(charconv);
	CArrayFix<CCnvCharacterSetConverter::SCharacterSet> *charsetsAvailable = charconv->CreateArrayOfCharacterSetsAvailableL(iFs);
	CleanupStack::PushL(charsetsAvailable);

	charconv->PrepareToConvertToOrFromL(KUidCharsetISO88591,*charsetsAvailable,iFs);
	
	const TMsvId current = iEntry.Entry().Id();
	iEntry.SetEntryL(KMsvRootIndexEntryId);
	// Get all POP3 and Imap services
	CMsvEntrySelection* rootChildren = iEntry.ChildrenL();
	CleanupStack::PushL(rootChildren);

	TMsvEntry entry;

	CEmailAccounts* accounts = CEmailAccounts::NewLC();

	const TInt count = rootChildren->Count(); 
	TInt j =0;
	while(j < count && iFoundServer ==EFalse)
		{
		// set context to service entry
		iEntry.SetEntryL((*rootChildren)[j]);
		entry = iEntry.Entry(); 
		if (entry.iType == KUidMsvServiceEntry && (entry.iMtm == KUidMsgTypePOP3 || entry.iMtm == KUidMsgTypeIMAP4))
			{
			TBuf8<8> port;

			if(entry.iMtm == KUidMsgTypePOP3)
				{
				// calculate hash value
				CImPop3Settings* settings = new(ELeave)CImPop3Settings;
				CleanupStack::PushL(settings);
 
 				TPopAccount id;
				accounts->GetPopAccountL(entry.Id(), id);
				accounts->LoadPopSettingsL(id, *settings);

				port.Num((TUint)KPOP3DefaultPortNumber);
				iFoundServer = CalcHashValueL( *charconv, settings->LoginName(), settings->ServerAddress(), port);
				}
			else // entry.iMtm == KUidMsgTypeIMAP4
				{
				CImImap4Settings* settings = new(ELeave)CImImap4Settings;
				CleanupStack::PushL(settings);
				
 				TImapAccount id;
				accounts->GetImapAccountL(entry.Id(), id);
				accounts->LoadImapSettingsL(id, *settings);

				port.Num((TUint)KIMAPDefaultPortNumber);
				iFoundServer = CalcHashValueL( *charconv, settings->LoginName(), settings->ServerAddress(), port);
				}
			CleanupStack::PopAndDestroy(); // settings
			}
		j++;
		}

	if(	iFoundServer )
		iServiceMsvId = entry.Id();

	CleanupStack::PopAndDestroy(4, charconv); // charconv, charsetsAvailable, rootChildren, accounts
	
	iEntry.SetEntryL(current);
	}

//
// Calculate a hash value for each server entry
// 
TBool CEmailNotificationParser::CalcHashValueL(CCnvCharacterSetConverter& aCharConv, const TDesC8& aUserName, const TDesC& aServerName, const TDesC8& aPortNum)
	{
	TInt total = 0;
	TInt loop = 0;
	TInt unConvertChar;
	TInt posOfUnconv;

	delete iServerName;
	iServerName = NULL;
	iServerName = HBufC8::NewL(aServerName.Length());

	TPtr8 svr = iServerName->Des();
	aCharConv.ConvertFromUnicode( svr, aServerName, unConvertChar,posOfUnconv);

	// Fix for DEF044588: the Smart Messaging Specification (Revision 3.0.0) states (page 3-17) that
	// the value of the first character of S is multiplied by one, second character by two etc.
	for(loop = 0; loop < aUserName.Length(); loop++)
		{
		total += ( (loop+1) * (TUint)aUserName[loop]);
		}

	TInt pos = aUserName.Length();

	for(loop = 0; loop < svr.Length(); loop++)
		{
		total += ( (pos + (loop+1) )* (TUint)svr[loop]);
		}

	pos += aServerName.Length();

	for(loop = 0; loop < aPortNum.Length(); loop++)
		{
		total += ( (pos + (loop+1) )* (TUint)aPortNum[loop]);
		}

	TReal hashVal;
	User::LeaveIfError(Math::Mod(hashVal, total, KServerIdModVal));
	return ((TInt)hashVal == iServerId);
	}

void CEmailNotificationParser::RequestComplete(TRequestStatus& aStatus, TInt aError)
    {
    TRequestStatus* p = &aStatus;
    User::RequestComplete(p, aError);
    }
// end CEmailNotificationParser::RequestComplete(TRequestStatus&, TInt)

//
// Constructor
//
CEmailNotificationParser::CEmailNotificationParser(CRegisteredParserDll& aRegisteredParserDll, CMsvEntry& aEntry, RFs& aFs)
: CBaseScriptParser2(aRegisteredParserDll, aEntry, aFs), 
iState(0), iSmsBuf(0), iReport(0), iCompleted(0), iMessageCount(0), iExtendedNotification(EFalse)
    {
    }
// end CEmailNotificationParser::CEmailNotificationParser()

CEmailNotificationParser::~CEmailNotificationParser()
    {
	delete iSmsBuf;
	if(iParsedFieldArray)
		{
		iParsedFieldArray->ResetAndDestroy();
		delete iParsedFieldArray;
		}
	delete iServerName;
    }
// end CEmailNotificationParser::~CEmailNotificationParser()
