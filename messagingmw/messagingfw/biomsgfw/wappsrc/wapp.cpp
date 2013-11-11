// Copyright (c) 2000-2009 Nokia Corporation and/or its subsidiary(-ies).
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

#include <e32std.h>
#include <msventry.h>
#include <bsp.h>
#include <wapp.h>
#include <wappdef.h>
#include "wapperr.h"
#include <wappstr.rsg>

#include "CWappBookmark.h"
#include "IspTableData.h"
#include "IpBearerData.h"
#include "SmsBearerData.h"
#include "GprsBearerData.h"

#include <mmssettingsproxybase.h>

#include <msvstd.h>
#include <msventry.h>
#include <msvids.h>
#include <msvuids.h>
#include <mtclreg.h>

#include <ipaddr.h>
#include <barsc.h>
#include <barsread.h>
#include <bautils.h>

#include <cbioasyncwaiter.h>
#include <cmsvattachment.h>
#include <mmsvattachmentmanager.h>
#include <mmsvattachmentmanagersync.h>

#ifdef SYMBIAN_BOOKMARK_DATABASE
#include <bookmarkdatabase.h>
#include <bookmark.h>
#endif // SYMBIAN_BOOKMARK_DATABASE

#ifdef _DEBUG
const TUid KUidSmartMessageMtm = {0x10001262};
const TInt32 KUidBIOWAPAccessPointMsg = 0x10005532;
#endif

const TInt KMaxNameBufferLength = 35;

_LIT8(KCRLinefeed,			"\r\n");
_LIT8(KEBookmarkItemBegin,	"BEGIN:eBOOKMARK\r\n");		//  Precedes a Bookmark in the Bookmark file
_LIT8(KEBookmarkItemName,	"NAME:");	// Precedes a Bookmark Name
_LIT8(KEBookmarkItemURL,	"URL:");		// Precedes a Bookmark URL
_LIT8(KEBookmarkType,		"TYPE:Wap\r\n");
_LIT8(KEBookmarkItemEnd,	"END:eBOOKMARK\r\n");

_LIT(KEBookmarkExtension,	".eBM");

_LIT(KWappResourceFile, "\\resource\\messaging\\wappstr.rsc");

const TUint32 KCharsetUCS2 = 1000;
const TUint32 KCharsetUTF8 = 106;
const TUint32 KCharsetASCII = 3;


// The number of characters that are constant for an eBookmark
// file. It is the sum of the above literals.
// 
const TInt KEBookmarkConstantChars = 55;

CMsvBIOWapAccessParser::CMsvBIOWapAccessParser(CRegisteredParserDll& aRegisteredParserDll, CMsvEntry& aEntry, RFs& aFs)
: CBaseScriptParser2(aRegisteredParserDll, aEntry, aFs)
   {
   }

EXPORT_C CMsvBIOWapAccessParser* CMsvBIOWapAccessParser::NewL(CRegisteredParserDll& aRegisteredParserDll, CMsvEntry& aEntry, RFs& aFs)
    {
    CMsvBIOWapAccessParser* self = new (ELeave) CMsvBIOWapAccessParser(aRegisteredParserDll, aEntry, aFs);
    CleanupStack::PushL(self);
    self->ConstructL();
    CleanupStack::Pop();
    return self;
    }

CMsvBIOWapAccessParser::~CMsvBIOWapAccessParser()
    {
	Cancel();
	// Char Conversion
	delete iCharsetConverter;
    delete iSettings;
	delete iSmsBuf;
	delete iStringTable;
	if (iBookmarkList!=NULL)
		{
		iBookmarkList->ResetAndDestroy();
		delete iBookmarkList;
		}
	delete iModemInitString;
    if (iParsedFieldArray != NULL)
        {
        iParsedFieldArray->ResetAndDestroy();
        delete iParsedFieldArray;
        }
	delete iWapIpISPTable;
	delete iWapSmsTable;
	delete iWapGprsTable;
	delete iGsmCsdWapIpTable;
	delete iGprsWapIpTable;

	delete iDBSession;
	REComSession::FinalClose();
    }

//
//	ConstructL			- ConstructL fn - creates the Parsed Field array, 
//						  and connects the file system if unconnected
//	
void CMsvBIOWapAccessParser::ConstructL()
    {
	iParsedFieldArray = new(ELeave) CArrayPtrSeg<CParsedField>(16);	
	iDBSession = CMDBSession::NewL(CMDBSession::LatestVersion());
	iBookmarkList = new(ELeave) CArrayPtrSeg<CWappBookmark> (4);// 4 should be enough
	iGsmCsdData = EFalse;
	iGsmSmsData = EFalse;
	iGprsData	= EFalse;
	iURLData	= EFalse;
	iNameData	= EFalse;
	iIdData		= EFalse;
	iMMSURLData = EFalse;
    CActiveScheduler::Add(this);
    }

//
//	ParseL		- public method forming part of the API.  Calls ChangeState to set 
//				  state for parsing. Helper functions do the parsing
//
void CMsvBIOWapAccessParser::ParseL(TRequestStatus& aStatus, const TDesC& aSms)
    {
    TMsvEntry entry = iEntry.Entry();   //  Get the generic stuff
    iEntryId = entry.Id();              //  store the TMsvId

	// Message must be unparsed, or parsed or committed - anything else is a big error
    __ASSERT_DEBUG((   entry.MtmData3() == BIO_MSG_ENTRY_UNPARSED
					|| entry.MtmData3() == BIO_MSG_ENTRY_PARSED
					|| entry.MtmData3() == BIO_MSG_ENTRY_PROCESSED),
                            Panic(EWappErrMessageProcessed));
	
	// Message must not be empty 
	__ASSERT_DEBUG (aSms.Length() > 0 ,Panic( EWappEmptyBuffer));

   //  Already parsed....just return
    if(entry.MtmData3() == BIO_MSG_ENTRY_PARSED || entry.MtmData3() == BIO_MSG_ENTRY_PROCESSED)
        {
        iReport = &aStatus;
        User::RequestComplete(iReport, KErrNone);
        }
    //  not parsed so start the Parsing operation
    else if(entry.MtmData3() == BIO_MSG_ENTRY_UNPARSED)
        {
        delete iSmsBuf;
        iSettings = NULL;
        iSmsBuf = aSms.AllocL();
        ChangeStateL(EStart);		// Do do initial processing, and go Active
        aStatus = KRequestPending;
        iReport = &aStatus;
        }
    else
        {
        User::Leave(KErrNotSupported);
        }
    }

//
//  ProcessL() --   If  parsed data is not already in memory, ProcessL loads it from the store
//					created during parsing. Creates a new entry in the WAP settings CommDb tables, from the data
//
void CMsvBIOWapAccessParser::ProcessL(TRequestStatus& aStatus)
    {
	// Calls ChangeState to get State Engine going. ( DoProcess does the hard work)

	iEntryId= iEntry.Entry().Id(); //store id of Bio Msg entry 

	// Must be a smart/Bio message, of type WAP access point and not have the failure flag set
	__ASSERT_DEBUG(iEntry.Entry().iMtm==KUidSmartMessageMtm, Panic(EWappInvalidEntry));
	__ASSERT_DEBUG(iEntry.Entry().iBioType ==KUidBIOWAPAccessPointMsg, Panic(EWappInvalidEntry));
	__ASSERT_DEBUG(iEntry.Entry().Failed()== 0 , Panic(EWappInvalidEntry));

	ResetProcessVariablesL();       // Reset variables to initial state
	ChangeStateL(EProcessInitialise);   //Set the initial state

	aStatus = KRequestPending;
	iReport = &aStatus; 	
   	}

//	 DoCancel	-  Cancels the Active Object operation and returns control to the caller
//
void CMsvBIOWapAccessParser::DoCancel()
    {
	User::RequestComplete(iReport,KErrCancel);
    }

//
//	RunL()		-		Called by the Active Scheduler when it finishes its Wait Loop. 
//						Doesn't do much as ParseL and Process keep going until they are complete.
//						Returns the current status. 
//
void CMsvBIOWapAccessParser::RunL()
    {
    iCompleted = iStatus.Int();

	if (iCompleted != KErrNone)
		{
		User::RequestComplete(iReport,iCompleted);
		return;
		}
	TInt currentState = iState;
	TRAPD(error, CallChangeStateL(currentState));
	switch (currentState)
		{
		case EStart:
		case EProcessInitialise:
		case EProcessMessage:
			if (error!=KErrNone)
				User::RequestComplete(iReport, error);  // Return error code
			break;
		case EParseMessage:
		case ECompleteMessage:
			User::RequestComplete(iReport, iCompleted); // Nothing more - allow calling RunL to complete and return status
			break;
		default:
			break;
		}
    }

void CMsvBIOWapAccessParser::CallChangeStateL(TInt aState)
	{
	switch (aState)
		{
		case EStart:
			ChangeStateL(EParseMessage);
			break;
		case EProcessInitialise:
			ChangeStateL(EProcessMessage);
			break;
		case EProcessMessage:
			ChangeStateL(ECompleteMessage);  // last stage change iMtmData3 
			break;
		default:
			break;
		}
	}

//
//	ChangeStateL		-	Used to advance the state to the next level.
//							Makes most of the important calls.
//
void CMsvBIOWapAccessParser::ChangeStateL(TParseSession aState)
    {
	iState = aState;
    switch (iState)
        {                    
		case EStart:	// Check the data we've got - if OK get the Active Stuff going
			if (!iSmsBuf)			
				User::Leave(KWappErrNullValue);
			else if (iSmsBuf->Length() == 0)
				User::Leave(KWappErrContent);
			break;
        case EParseMessage:
			iSms = iSmsBuf->Des();      // initialise TLex object
			ParseMessageL();
            break;
		case EProcessInitialise:
			if (!iSmsParsed)
				RestoreParsedDataL();
			PreProcessL();  //Scan iParsedFieldArray extracting data - leaves if data is invalid
			ValidateCharacteristicsL(); // Ensures that the mix of characteristic is valid
			break;
		case EProcessMessage:
			DoProcessL(); // Do the proper processing of and committing to CommDb
			break;
		case ECompleteMessage:
			CompleteMessageL();
			break;
        default:
            break;
        }

	// Finished this step -add ourselves to Scheduler,& return error code to calling thread.
	RequestComplete(iStatus, KErrNone); 
	SetActive();
    }

//
//	ParseMessageL()		- Performs various checks, if data has been saved it restores it  
//						  else it calls DoParseL which does the hard stuff).
//
void CMsvBIOWapAccessParser::ParseMessageL()
	{
	// MUST have Bio message, of type WAP access point and no failure flag set
	__ASSERT_DEBUG(iEntry.Entry().iMtm==KUidSmartMessageMtm, Panic(EWappInvalidEntry));
	__ASSERT_DEBUG(iEntry.Entry().iBioType ==KUidBIOWAPAccessPointMsg, Panic(EWappInvalidEntry));
	__ASSERT_DEBUG(iEntry.Entry().Failed()== 0 , Panic(EWappInvalidEntry));

	// Message already parsed(includes processed).  Just restore data and return
	if (iEntry.Entry().MtmData3() == BIO_MSG_ENTRY_PARSED || 
		iEntry.Entry().MtmData3() == BIO_MSG_ENTRY_PROCESSED)
		iSmsParsed = ETrue;   // Have a store produced during parsing. 
	else 
		iSmsParsed = EFalse;

	if(iSmsParsed)  // parsed before - restore data
		{
		RestoreParsedDataL();
		return;
		}

	DoParseL();			// Does the actual parsing of the message contents
	SettingsNamePresentL(); // Check for Name Characteristic- if not found appends a default one
	StoreParsedDataL(); // Saves the parsed fields to store & sets TMSvEntry values
	iSmsParsed=ETrue;
	}

//
//  CompleteMessageL()	-	Completes the ProcessL operation, by setting the iMtmData3 
//							flag to BIO_MSG_ENTRY_PROCESSEDMessage body, and TMsvEntry's
//							iDescription and iDetails are left unchanged
//
void CMsvBIOWapAccessParser::CompleteMessageL()
    {
	TMsvEntry entry= iEntry.Entry();
	entry.SetMtmData3(BIO_MSG_ENTRY_PROCESSED);

	iEntry.ChangeL(entry);
    }

//
//	RequestComplete()	-	Completes an AO operation, and passes control back to the caller
//
void CMsvBIOWapAccessParser::RequestComplete(TRequestStatus& aStatus, TInt aError)
    {
    TRequestStatus* p = &aStatus;
    User::RequestComplete(p, aError);
    }

//
//  DoParseL()		- Parses through encoded message body extracting the fields from the characteristics
//					Uses helper functions to handle reading and storing of characteristics and parms. 
//					Parsing performed in one continuous operation.
//
void CMsvBIOWapAccessParser::DoParseL()
    {
	const TInt KDecimalSizeOfMaxInt = 10; // length of 0xFFFFFFFF in a string in decimal
	TBuf<KDecimalSizeOfMaxInt> localBuf;
	TChar thisCharacter;
 
	thisCharacter = GetNextMessageByteL();	// Read the first char to get XML version number
	/*
	The version byte contains the the major version minus one in the upper four bits 
	of the 8 bit integer, and the minor version in the lower four bits.
	*/
	TUint wbxmlversionbyte = thisCharacter; 
	TUint majorversion = ((wbxmlversionbyte & 0xF0) + 0x10) - 0x0F; // gives the major version, eg if wbxml spec version 1.2, it gives 1.
	TUint minorversion = wbxmlversionbyte & 0x0F; //gives the minor version, eg if wbxml spec version 1.2, it gives 2.
	
	iWBXMLversionX10 = (majorversion *10) + minorversion; //i.e. If actual version is 1.1 this is *10 to remove the decimal place.

	//Append this to the array
	localBuf.NumUC(iWBXMLversionX10,EDecimal);  //Format to a decimal string
	AppendDataPairL(KWappXMLversion, localBuf); // Append to array of parsed fields

	// Now need to get the 32bit int encoded over a number of bytes
	iPublicID = Read_mb_u_int32L();		// Read in a multi byte 32bit int value
	localBuf.NumUC(iPublicID, EHex);	// Format to a string as hex 
	AppendDataPairL(KWappPublicID, localBuf); 
	
	// WAP Binary XML version 1.1 also has additional info for the character set.
	if (iWBXMLversionX10 !=10)				// If WAP Binary XML version > 1.0
		iCharacterSet = Read_mb_u_int32L(); // Get the character set
	else
		//This is the character set MIBenum for UTF-8. 
		//According to WBXML spec version 1.1 and 1.2, this is the default which should 
		//be assumed if the charset value is not available. Value 0 must not be used. 
		iCharacterSet = 106; 

	delete iCharsetConverter;
	iCharsetConverter = NULL;
	iCharsetConverter = CWapCharsetConverter::NewL(iFs, iCharacterSet);

	localBuf.NumUC(iCharacterSet, EHex);	// Convert to string as hex 
	AppendDataPairL(KWappCharacterSet,localBuf);

	// Coded string table length - don't need this once we have string table
	// as we store copies of the strings within parsed data fields 
	TUint32 stringTableLength = Read_mb_u_int32L();

	//Read in string table if it exists, i.e. length >0 otherwise skip this step
	if (stringTableLength>0) 
		{
		iStringTable = HBufC::NewL(stringTableLength);
		iSms.Mark();					// Mark the current position

		TUint32 remaining = iSms.RemainderFromMark().Length(); //Eliminates warning about comparison in next line
		
		if ( remaining > stringTableLength) // Sting Table doesn't exceed end of message
			iSms.Inc(stringTableLength);	// so increment to the character following the string table
		else
			User::Leave(KWappErrEOS); // Table exceeds message length - unrecoverable error
		iStringTable->Des().Copy(iSms.MarkedToken());
		}

	thisCharacter = GetNextMessageByteL();
	//This should be a Characteristic List with content, but no attributes
	if (thisCharacter == (KWAPP_CHAR_LIST+KWAPP_TAG_CONTENT))
		{
		// Main loop - keep moving through the message, until we reach the end or error
		TBool finished = EFalse;
		while (!finished)
			{		
			// Read in characteristics, any order is accepted
			if (!iSms.Eos() && (iSms.Peek() & 0x3F) == KWAPP_CHARACTERISTIC)
				{
				// Read in characteristic, inc. its terminator tag - no terminator =error!
				ReadCharacteristicL();
				}
			else
				{
				if (iSms.Eos()) // Error end of message
					User::Leave(KWappErrEOS);
				else if ((TUint)iSms.Peek()==KWAPP_END_TAG) // END Tag - OK we've finished!
					finished = ETrue;
				else if ( (iSms.Peek()& 0x3F != KWAPP_CHARACTERISTIC) || (iSms.Peek() != KWAPP_END_TAG) )
					User::Leave(KWappErrUnexpectedValue); //Not a Characteristic or END token
				}
			}
		} // End of if (thisCharacter == (KWAPP_CHAR_LIST..... )
	else
		User::Leave(KWappErrMandatoryTagMissing);
    }
// end CMsvBIOWapAccessParser::DoParseL()

//	
//	ReadCharacteristicL()		-  Reads in a characteristic from the message.
//
void  CMsvBIOWapAccessParser::ReadCharacteristicL()
	{
	TBool hasContent;
	TInt  characteristicIndex=0; // start position in array for this characteristic
	CParsedField* parsedField = new(ELeave)CParsedField();
	CleanupStack::PushL(parsedField);		

	TChar currentCharacter = GetNextMessageByteL();
	iCurrentCharacteristic =EWappUnknown; // Reset flag for current characteristic's type

	// Use bit comparison of token with mask 0x3F to check explicitly that correct 
	// bits are set i.e. that bits 0-5 = 0x06   (NB Actual value = 0x86 or =0xC6)
	if ( ( currentCharacter & 0x3F) != KWAPP_CHARACTERISTIC ) // i.e. not or 0x06
		User::Leave(KWappErrUnexpectedValue);		
	else
		{
		if ( !(currentCharacter & KWAPP_TAG_ATTRIBUTES)) // Bit 7 must be set!
			User::Leave(KWappErrNoAttributes); 
		if (currentCharacter & KWAPP_TAG_CONTENT)  //Bit 6 is set - characteristic has content
			hasContent =ETrue;
		else
			hasContent =EFalse;


	// ***************			IMPORTANT NOTE		   ************************
	// An XML element name without a defined token is sent as an inline string or
	// string table ref. prefixed by the TYPE token 0x05 (ie "TYPE="). (True for
	// WirelessFuture web site!!) THE ONLY UNDEFINED Type tag is BOOKMARK in version
	// 1.0. Therefore if it's 0x05  - check the version. For 1.0 read next char.
	// Token must be an inline string or string table ref.Otherwise we have an error.
	// ****************************************************************************

		// Read the next byte - info on the type of our characteristic  
		currentCharacter = (TUint) GetNextMessageByteL();
		
		// Check if the tokens 0x05 "TYPE=" - should be for WBXML version = 1.0 
		// Read next character
		if (currentCharacter == KWAPP_TYPE)
			{
			// "TYPE=" should be followed by an inline string, or string table reference
			// So read another char & increment a place - should now 0x03 or 0x83
			currentCharacter = (TUint) GetNextMessageByteL();
			if  (	!(currentCharacter & KWAPP_STR_I) &&	// Is current character != 0x03 ?
					!(currentCharacter & KWAPP_STR_T))		// or != 0x83 
				 User::Leave(KWappErrUnexpectedValue);		// Doc is badly formed doc - leave
			}

	// NB for address characteristics, need to store position of the start, so that
	// we can search for certain fields. If they're not present we must add defaults.  
	// Could just scan from the end of the array, but if we miss our target then may
	// just increment past this characteristic, through previous ones.
 
		switch (currentCharacter)
			{
			case KWAPP_ADDR_TYPE:  // Token = 0x06
				{
				AppendNameFieldL(KWappCharAddress);		
				characteristicIndex = iParsedFieldArray->Count()-1; // Store index for this element
				iCurrentCharacteristic = EWappAddressCharacteristic; //General code for an address
				break;
				}
			case KWAPP_URL_TYPE:   //Token = 0x07
				{
				AppendNameFieldL(KWappCharURL);
				iCurrentCharacteristic  = EWappURLCharacteristic; // it's a URL characteristic
				break;
				}
			case KWAPP_NAME_TYPE:  // Token = 0x08
				{
				AppendNameFieldL(KWappCharName); 
				iCurrentCharacteristic = EWappNameCharacteristic;//it's a NAME characteristic
				break;
				}
			case KWAPP_STR_I:		// Token = 0x03  , Global WAP XML value
				{
				// Null terminated inline string follows the token, call fn to read it
				parsedField->SetFieldNameL(*ReadInlineStringLC());
				CleanupStack::PopAndDestroy(); //inline string
				break;
				}
			case KWAPP_STR_T:		// Token = 0x83,  Global WAP XML value
				{
				// multi byte 32 bit integer follows, an reference to an offset in the string table
				TUint32 offset =0;
				offset = Read_mb_u_int32L(); // read in the integer
				parsedField->SetFieldNameL(*ReadStringTableReferenceLC(offset));//Get string table value,& set the descriptor
				CleanupStack::PopAndDestroy(); // string table reference
				break;
				}
			case KWAPP_ID:			// Token = 0x7D.  Used in version 5.0
				{
				AppendNameFieldL(KWappCharID);  
				iCurrentCharacteristic = EWappIDCharacteristic; 
				break;
				}
			case KWAPP_BOOKMARK:	// Token = 07F, currently bookmarks stored in file
				{
				AppendNameFieldL(KWappCharBookmark);  
				iCurrentCharacteristic = EWappBookmarkCharacteristic; 
				break;
				}
			case KWAPP_MMS_URL:
				{
				AppendNameFieldL(KWappCharMMSURL);
				iCurrentCharacteristic = EWappMMSURLCharacteristic;
				break;
				}
			default:
				User::Leave(KWappErrUnrecognised);   // Unrecognised token or somethings badly wrong
			}
		

		// If iCurrentCharacteristic isn't set we've just read in a string table ref
		// or inline string. Now check the token and set the flag.
		if (iCurrentCharacteristic == EWappUnknown)
			{
			// Get index of last element
			if (parsedField->FieldName().CompareF(KWappAddr)==0)
				{
				iCurrentCharacteristic = EWappAddressCharacteristic; 
				AppendNameFieldL(KWappCharAddress);	
				characteristicIndex = iParsedFieldArray->Count()-1;//May be an address so store index
				}
			else if (parsedField->FieldName().CompareF(KWappURL)==0)
				{
				iCurrentCharacteristic = EWappURLCharacteristic;
				AppendNameFieldL(KWappCharURL);
				}
			else if (parsedField->FieldName().CompareF(KWappMMSURL)==0)
				{
				iCurrentCharacteristic = EWappMMSURLCharacteristic;
				AppendNameFieldL(KWappCharMMSURL);
				}
			else if (parsedField->FieldName().CompareF(KWappName)==0)
				{
				iCurrentCharacteristic = EWappNameCharacteristic;
				AppendNameFieldL(KWappCharName); 
				}
			else if (parsedField->FieldName().CompareF(KWappBookmark)==0)
				{
				iCurrentCharacteristic = EWappBookmarkCharacteristic;
				AppendNameFieldL(KWappCharBookmark);
				}
			else if (parsedField->FieldName().CompareF(KWappID)==0)
				{
				iCurrentCharacteristic = EWappIDCharacteristic;
				AppendNameFieldL(KWappCharID); 
				}
			else 
				{
				User::Leave(KWappErrUnrecognised);// Unrecognised characteristic type 
				}
			}		
		CleanupStack::PopAndDestroy(parsedField);
	
		//Check for a VALUE attribute before checking for Parms
		currentCharacter =(TUint) GetNextMessageByteL();
		if (currentCharacter ==KWAPP_VALUE_ATTRIB)
			{
			// Have a value following the TYPE - read in value and store it.
			TInt  lastElement = iParsedFieldArray->Count()-1;
			currentCharacter = (TUint) GetNextMessageByteL();

			if (currentCharacter == KWAPP_STR_I)		//Inline string
				{
				iParsedFieldArray->At(lastElement)->SetFieldValueL(*ReadInlineStringLC()); //Set value of last element in array
				CleanupStack::PopAndDestroy();
				}

			else if (currentCharacter ==KWAPP_STR_T)	//String table reference
				{
				TUint32 tableOffset = Read_mb_u_int32L();
				iParsedFieldArray->At(lastElement)->SetFieldValueL(*ReadStringTableReferenceLC(tableOffset));  //Set Value of last element in array
				CleanupStack::PopAndDestroy();
				}

			else 
				{
				User::Leave(KWappErrUnexpectedValue);
				}

			// Read the next character for the END TAG check in the following line 
			currentCharacter = (TUint)GetNextMessageByteL();
			}

		//Check the character- should be 0x01 the end of the characteristic header
		if (!currentCharacter ==KWAPP_END_TAG) // Not END - error condition
			{
			User::Leave(KWappErrNoTermination); 
			}

		// We shouldn't be at the end of the message. The next byte should be one of the following 
		//	    - PARM if this characteristic has content, 
		//		- start of another characteristic
		//		- END token (0x01).
		// So take a peek at the next byte
		TUint peekAhead;

		if (iSms.Eos())
			User::Leave(KWappErrEOS);	// Somethings wrong - message end
		
		peekAhead = (TUint) iSms.Peek();

		// Type is Set - address,URL,Name, Bookmark or ID. Check hasContent flag set correctly. 
		if ( ((iCurrentCharacteristic == EWappAddressCharacteristic) && !hasContent) || //Address must have Content
		     ((iCurrentCharacteristic == EWappNameCharacteristic) && !hasContent)	 ||  //Name must have Content
			 ((iCurrentCharacteristic == EWappBookmarkCharacteristic) && !hasContent)|| //Bookmark must have Content
			 ((iCurrentCharacteristic == EWappIDCharacteristic) && !hasContent)		 || //ID must have Content
			 ((iCurrentCharacteristic == EWappURLCharacteristic) && hasContent)		 ||  // Url must NOT have Content
			 ((iCurrentCharacteristic == EWappMMSURLCharacteristic) && hasContent)     )  // MmsUrl does not have content
			 User::Leave(KWappErrContent); // Test for validity of content flag failed.
			

		if (hasContent)  // passed previous test, - exclude URL as hasContent=EFalse
			{
			if ( (peekAhead & 0x3F) ==KWAPP_PARM) // bit comparison with 0x07
				{
				// Check sucessful, now check for attributes and content (0x80, & 0x40)
				if ( (peekAhead & KWAPP_TAG_ATTRIBUTES) && ( !(peekAhead & KWAPP_TAG_CONTENT) ) )
					{
					//Have attributes, and no content - ALL OK!
					currentCharacter = (TUint32) GetNextMessageByteL();
					while (currentCharacter != KWAPP_END_TAG)
						{
						ReadParmL(); // Sets iCurrentCharacteristic for addresses
						
						currentCharacter = (TUint32) GetNextMessageByteL();  // Get the next character from message
						
						if ( (currentCharacter != (KWAPP_PARM+KWAPP_TAG_ATTRIBUTES))
							&& (currentCharacter !=KWAPP_END_TAG) ) // Not 0x87, nor 0x01 = error
							User::Leave(KWappErrUnexpectedValue); 
						}
					}
				else  // Token MUST be 0x87, parms MUST have content but NO attributes!
					User::Leave(KWappErrNoAttributes);  
				}
			else  //Bits 0-5 are Not 0x07  Should be a PARM token
				User::Leave(KWappErrUnexpectedValue); 
			} //END of (hasContent)
		}
		// END of  if ( !( (TUint32)currentCharacter & KWAPP_CHARACTERISTIC) )



// ************************** MANDATORY PARMS & DEFAULT VALUES **************************
// Finished reading in the parms for the characteristic.  Each Address Characteristic 
// has it's own set of defaults. If this is an address - ensure defaults are present.
		

// CHARACTERISTIC TYPE = ADDRESS, BEARER TYPE  = GSM/CSD or IS-136/CSD
//	 Bearer, Proxy, and Csd_Dialstring are mandatory..
//   Auth. Names and Passwords don't have defaults so are not set here!
		
		if ( iCurrentCharacteristic == EWappGsmCsdCharacteristic||
			iCurrentCharacteristic == EWappIS136CsdCharacteristic )
			{
			//Required PARMS				OTA v4.0	   v5.0
			//CSD-		PROXY					Y			Y
			//			PORT				add default of 9200	
			//			CSD_DIALSTRING			Y			Y
			//			CSD_AUTHNAME			Y			N
			//			CSD_AUTHSECRET			Y			N
			//			CSD_CALLTYPE			Y		Add default of PAP
			//			CSD_CALLSPEED		    Y       Add default of AUTO

			// Check mandatory PARMS present else bad message - unrecoverable error!
			TInt index;
		
			// OTA v4.0 & 5.0:  required Parms present?
			index = LocateParsedFieldName(KWappProxy,characteristicIndex);
			if (index == KErrNotFound) // TODO can this be null? || (iParsedFieldArray->At(index)->FieldValue() == TPtrC()))
				User::Leave(KWappErrMandatoryTagMissing);
			
			index = LocateParsedFieldName(KWappCsdDial,characteristicIndex);
			if (index == KErrNotFound) // Can this be Null?? || (iParsedFieldArray->At(index)->FieldValue() == TPtrC()) )
				User::Leave(KWappErrMandatoryTagMissing); 

			// OTA v4.0 only, mandatory Parms  present?
			if (iWBXMLversionX10 == 10)
				{
				index = LocateParsedFieldName(KWappPPPAuthName,characteristicIndex);
				if (index == KErrNotFound)// Can this be NULL || (iParsedFieldArray->At(index)->FieldValue() == TPtrC()))
					User::Leave(KWappErrMandatoryTagMissing);
			
				index = LocateParsedFieldName(KWappPPPAuthSecret,characteristicIndex);
				if (index == KErrNotFound) // Can this be a Null value? || (iParsedFieldArray->At(index)->FieldValue() == TPtrC()) )
					User::Leave(KWappErrMandatoryTagMissing); 
				}

			// Search for PORT from beginning of this characteristic to array end or next 
			// Characteristic - whichever's first. NB this is currently the last characteristic in array!

			index = LocateParsedFieldName(KWappPort, characteristicIndex);
			if (index ==KErrNotFound)	// Not Found - add the default value
				{
				index = LocateParsedFieldName(KWappProxy,characteristicIndex); // Mandatory - check already made
				InsertDataPairL(KWappPort, KWappPort9200, index+1);
				}
			
			// Check for the PPP_AUTHTYPE element
			index = LocateParsedFieldName(KWappPPPAuthType, characteristicIndex);
			if (index ==KErrNotFound)	// Not Found - add the default value
				{
				index = LocateParsedFieldName(KWappCsdDial, characteristicIndex); // always present
				// Create new element,NAME="PPP_CALLTYPE", VALUE="PAP"
				InsertDataPairL(KWappPPPAuthType, KWappPAP, index+1);
				}

			// Check for the CSD_CALLTYPE element
			index = LocateParsedFieldName(KWappCsdCallType, characteristicIndex);
			if (index ==KErrNotFound)	// Not Found - add the default value
				{
				index = LocateParsedFieldName(KWappPPPAuthType, characteristicIndex); 
				if (index ==KErrNotFound)
					User::Leave(KWappErrMandatoryTagMissing); // Error - should be here from step above!!!
				else
					// Create new element,NAME="CSD_CALLTYPE", VALUE="ANALOGUE"
					InsertDataPairL(KWappCsdCallType, KWappAnalogue, index+1);
				}

			// Check for the CSD_CALLSPEED element
			index = LocateParsedFieldName(KWappCsdCallSpeed, characteristicIndex);
			if (index ==KErrNotFound)	// Not Found - add the default value
				{
				index = LocateParsedFieldName(KWappCsdCallType,characteristicIndex);
				if (index ==KErrNotFound)
					User::Leave(KWappErrMandatoryTagMissing); // Error - should be here from step above!!!
				else				
					//Create new PARM,
					// If CSD_CALLTYPE="ANALOGUE"   ->  NAME="CSD_CALLSPEED", VALUE="AUTO" 
					// else if CSD_CALLTYPE="ISDN"  ->  NAME="CSD_CALLSPEED", VALUE="9600" 
					if (iParsedFieldArray->At(index)->FieldValue().CompareF(KWappISDN)==0)
						InsertDataPairL(KWappCsdCallSpeed, KWappSpeed9600, index+1);
					else
						InsertDataPairL(KWappCsdCallSpeed, KWappSpeedAuto, index+1);
				}
			}// END of if(iCurrentCharacteristic == EWappCSDCharacteristic)|| .......


// CHARACTERISTIC TYPE = ADDRESS, BEARER TYPE  = GSM/SMS 
// Only the PORT PARM value is optional - all the rest are mandatory
		
		if (iCurrentCharacteristic == EWappGsmSmsCharacteristic)
			{
			//Required PARMS				OTA v4.0	   v5.0
			//SMS-		PROXY					Y			Y
			//			SMS_SMSC_ADDRESS		Y
	
			// Check that required fields are present - badly formed message otherwise
			TInt fieldIndex = LocateParsedFieldName(KWappProxy, characteristicIndex);
			if (fieldIndex == KErrNotFound ) // TODO Check if this can be null || (iParsedFieldArray->At(fieldIndex)->FieldValue().Length()==0) )
				User::Leave(KWappErrMandatoryTagMissing); 

			fieldIndex = LocateParsedFieldName(KWappSMSCAddress, characteristicIndex);
			if ((fieldIndex ==KErrNotFound)) // TODO Check if this can be null   || (iParsedFieldArray->At(fieldIndex)->FieldValue().Length()==0) )
				User::Leave(KWappErrMandatoryTagMissing);


			if (LocateParsedFieldName(KWappPort, characteristicIndex) ==KErrNotFound) //Tag not found
				{
				fieldIndex = LocateParsedFieldName(KWappProxy, characteristicIndex); //Mandatory field
				// Create a new  element, NAME="PORT", VALUE="9200"
				InsertDataPairL(KWappPort,KWappPort9200, fieldIndex+1);
				}
			}//END of  if(iCurrentCharacteristic == EWappSMSCharacteristic)


// CHARACTERISTIC TYPE = ADDRESS, BEARER TYPE  = GSM/USSD 
		if (iCurrentCharacteristic == EWappGsmUssdCharacteristic)
			{
			
			//Only USSD Service Code is mandatory - MUST not be NULL
			TInt index = LocateParsedFieldName(KWappUSSDCode, characteristicIndex);
			if (index == KErrNotFound || iParsedFieldArray->At(index)->FieldValue().Length()==0)
				User::Leave(KWappErrMandatoryTagMissing);
			
			// PROXY is optional for USSD but has no default value
			// Do have defaults for PROXY_TYPE and PORT
			if (LocateParsedFieldName(KWappProxyType, characteristicIndex)  ==KErrNotFound) //Tag not found
				{
				index = LocateParsedFieldName(KWappUSSDCode, characteristicIndex); //Mandatory field
			
				// Create a new  element, NAME="PROXY_TYPE", VALUE="MSISDNNO"
				InsertDataPairL(KWappProxyType,KWappMsisdnNo, index+1);
				}

			if ( LocateParsedFieldName(KWappPort, characteristicIndex) == KErrNotFound) // Not Found - add the default value
				{
				index = LocateParsedFieldName(KWappProxyType,characteristicIndex);
				if (index ==KErrNotFound)
					User::Leave(KWappErrMandatoryTagMissing); // Error - should be here from step above!!!
				else	// Create new element,NAME="PORT", VALUE="9200"
					InsertDataPairL(KWappPort, KWappPort9200, index+1);
				}
			} // END of -  if (iCurrentCharacteristic == EWappGsmUssdCharacteristic)... 


// CHARACTERISTIC TYPE = ADDRESS, BEARER TYPE  = GPRS 
		if (iCurrentCharacteristic == EWappGprsCharacteristic)
			{
			// AHF - Removed check for tag GPRSACCESSPOINTNAME - no longer mandatory in OTA v7.0

			// Check that mandatory PROXY field is present - else it's a badly formed msg
			TInt index = LocateParsedFieldName(KWappProxy, characteristicIndex);
			if (index == KErrNotFound ) 
				User::Leave(KWappErrMandatoryTagMissing); 

			//Search for optional Port field - if not found, search for mandatory Proxy field and insert "9200"
			index = LocateParsedFieldName(KWappPort, characteristicIndex);
			if (index ==KErrNotFound)
				{
				index = LocateParsedFieldName(KWappProxy,characteristicIndex); // Mandatory - check already made
				InsertDataPairL(KWappPort, KWappPort9200, index+1); // NAME="PORT", VALUE = "9200"
				}

			// Check for the PPP_AUTHTYPE element - add default value if not found
			index = LocateParsedFieldName(KWappPPPAuthType, characteristicIndex);
			if (index ==KErrNotFound)	
				{
				index = LocateParsedFieldName(KWappCsdDial, characteristicIndex); // always present
				InsertDataPairL(KWappPPPAuthType, KWappPAP, index+1); // NAME="PPP_CALLTYPE", VALUE="PAP"
				}
			}



	}
// END of CMsvBIOWapAccessParser::ReadCharacteristicL()

//
//	ReadParmL()		-	Reads in contents of a PARM & inserts into iParsedFieldArray
//						as a CParsedField object. ASSUME that the PARM tag has
//						been read & verified  before this is called. Consequently
//					    now positioned at the start of the attributes 

void CMsvBIOWapAccessParser::ReadParmL()
	{

	CParsedField* parsedField = new(ELeave) CParsedField;
	CleanupStack::PushL(parsedField);
	TChar currentCharacter = GetNextMessageByteL();

	switch ((TUint) currentCharacter)
		{
		case KWAPP_NAME_ATTRIB:  //0x10 "NAME=..." ,string(inline or table ref.) should follow
			{
			//Start of NAME attribute, check for inline string/string table reference
			currentCharacter=GetNextMessageByteL();
			if ((TUint32) currentCharacter == KWAPP_STR_I)
				{
				parsedField->SetFieldNameL(*ReadInlineStringLC());
				CleanupStack::PopAndDestroy(); 
				}
			else if ((TUint32) currentCharacter== KWAPP_STR_T)
				{
				TUint32 offset = Read_mb_u_int32L();
				parsedField->SetFieldNameL(*ReadStringTableReferenceLC(offset));
				CleanupStack::PopAndDestroy(); 
				}
				else // it isn't an inline string or string table ref - ERROR!
					User::Leave(KWappErrNoTermination); 
			break;
			}
		case KWAPP_BEARER: //0x12 NAME="BEARER"
			{
			parsedField->SetFieldNameL(KWappBearer); 
			break;
			}
		case KWAPP_PROXY: //0x13   NAME="PROXY"  (Mandatory for GSM/CSD, GSM/SMS, IS136/CSD)
			{
			parsedField->SetFieldNameL(KWappProxy);	
			break;
			}
		case KWAPP_PORT	://0x14 NAME="PORT"
			{
			parsedField->SetFieldNameL(KWappPort);	
			break;
			}
		case KWAPP_NAME ://0x15 NAME = "NAME"
			{
			parsedField->SetFieldNameL(KWappName);	
			break;
			}
		case KWAPP_PROXYTYPE  ://0x16 NAME="PROXY_TYPE"
			{
			parsedField->SetFieldNameL(KWappProxyType);
			break;
			}
		case KWAPP_URL	:	// 0x17 NAME="URL"
			{
			parsedField->SetFieldNameL(KWappURL);	
			break;
			}
		case KWAPP_PROXY_AUTHNAME :			// 0x18  NAME="PROXY_AUTHNAME"
			{
			parsedField->SetFieldNameL(KWappProxyAuthName);
			break;
			}
		case KWAPP_PROXY_AUTHSECRET :		//0x19 NAME="PROXY_AUTHSECRET"
			{
			parsedField->SetFieldNameL(KWappProxyAuthSecret);
			break;
			}
		case KWAPP_PROXY_LOGIN_TYPE :
			{
			parsedField->SetFieldNameL(KWappProxyLoginType);
			break;
			}
		case KWAPP_SMS_SMSC_ADDR  :    //0x1A   NAME="SMS_SMSC_ADDR"  (Mandatory field for GSM/SMS)
			{
			parsedField->SetFieldNameL(KWappSMSCAddress); 
			break;
			}
		case KWAPP_USSD_SERV_CODE :	//0x1B NAME="USSD_SER_CODE" (Mandatory for GSM/USSD)
			{
			parsedField->SetFieldNameL(KWappUSSDCode);	
			break;
			}
		case KWAPP_ACCESS_POINT_NAME :	//0x1C	 NAME="ACCESSPOINTNAME" (Mandatory for GPRS)
			{
			parsedField->SetFieldNameL(KWappAccessPointName);
			break;
			}
		case KWAPP_CSD_DIALSTRING :		//0x21   NAME="CSD_DIALSTRING" (Mandatory for GSM/CSD,IS136/CSD)
			{
			parsedField->SetFieldNameL(KWappCsdDial);
			break;
			}
		case KWAPP_PPP_AUTHTYPE	  :		//0x22   NAME="CSD_AUTHTYPE"
			{
			parsedField->SetFieldNameL(KWappPPPAuthType);
			break;
			}
		case KWAPP_PPP_AUTHNAME	  :	  //0x23   NAME="CSD_AUTHNAME"
			{
			parsedField->SetFieldNameL(KWappPPPAuthName);
			break;
			}
		case KWAPP_PPP_AUTHSECRET :	  //0x24    NAME="CSD_AUTHSECRET"
			{
			parsedField->SetFieldNameL(KWappPPPAuthSecret);
			break;
			}
		case KWAPP_PPP_LOGIN_TYPE :
			{
			parsedField->SetFieldNameL(KWappPPPLoginType);
			break;
			}
		case KWAPP_CSD_CALLTYPE	  :	// 0x25    NAME="CSD_CALLTYPE"
			{
			parsedField->SetFieldNameL(KWappCsdCallType);
			break;
			}
		case KWAPP_CSD_CALLSPEED  :	//0x28    NAME="CSD_CALLSPEED"
			{
			parsedField->SetFieldNameL(KWappCsdCallSpeed);
			break;
			}
		case KWAPP_NAME_ISP	  :		//0x7E    NAME="ISP_NAME"
			{
			parsedField->SetFieldNameL(KWappISP);
			break;
			}

		default:
			User::Leave(KWappErrUnexpectedValue); // don't recognise the tag
		}

	// The early version of the WAP server, supporting version 4.0 of the Nokia OTA specs
	// codes up PROXY_AUTHNAME & PROXY_AUTHSECRET as the PROXY token followed by
	// a string/str ref. So we MUST check if inline string/str table ref follows
	if (currentCharacter==KWAPP_PROXY) 
		{
		if (iSms.Eos())
			User::Leave(KWappErrEOS); // ERROR - Unexpected end of message!! Nothing to peek at!
		else if ( ((TUint)iSms.Peek() == KWAPP_STR_I) || ((TUint)iSms.Peek() == KWAPP_STR_T)) 
			{
			// String follows immediately after Name token not a Value Tag
			TUint32 anOffset;
			currentCharacter = GetNextMessageByteL();
			HBufC* name = NULL;
			if ((TUint) currentCharacter == KWAPP_STR_I)
				{
				name = ReadInlineStringLC();
				}
			else 
				{
				if ((TUint) currentCharacter != KWAPP_STR_T)
					User::Leave(KWappErrUnexpectedValue); // Should not get here unless something is very wrong! 
				anOffset = Read_mb_u_int32L();
				name = ReadStringTableReferenceLC(anOffset);
				}

			//Defect fix for Def021848-Incorrect use of ReAllocL in wapp	
			HBufC* newName = name->ReAllocL(name->Length() + parsedField->FieldName().Length());
			CleanupStack::Pop(name);//Remove name from the Cleanup Stack as it would have been deleted by ReAllocL()
			CleanupStack::PushL(newName);
			newName->Des().Append(parsedField->FieldName()); //insert PROXY at front of string 
			parsedField->SetFieldNameL(*newName);			  //replace current string  with new one
			CleanupStack::PopAndDestroy(newName); 
			}
		}

	TBool addParsedField = ETrue;

	currentCharacter=GetNextMessageByteL(); //Get next byte - should be the Value tag
	switch( (TUint32) currentCharacter)
		{
		case KWAPP_VALUE_ATTRIB : // 0x11  "VALUE=", string(inline or table ref) follows
			{
			if (iSms.Eos())
				{
				User::Leave(KWappErrEOS); // ERROR - Unexpected end of message!! Nothing to peek at!
				}		
			//Check for 0x01 (KWAPP_END_TAG) - assign fieldvalue with empty value and ignore the other 
			if ( ((TUint)iSms.Peek() == KWAPP_END_TAG)) //Something wrong - Wrong PARAM Value 
				{
				addParsedField = EFalse;
  				break;
				}
			// Start of VALUE attribute, string should follow
			currentCharacter= GetNextMessageByteL();
			if ((TUint32) currentCharacter == KWAPP_STR_I)		//Inline string
				{
				HBufC* value = ReadInlineStringLC();
				parsedField->SetFieldValueL(*value);
				CleanupStack::PopAndDestroy();
				}
			else if ((TUint32) currentCharacter== KWAPP_STR_T)	//String Table ref
				{
				TUint32 offset = Read_mb_u_int32L();			// Read 32bit int
				HBufC* value = ReadStringTableReferenceLC(offset);	// Read in characters
				parsedField->SetFieldValueL(*value);
				CleanupStack::PopAndDestroy();
				}
				else //Not an inline string or string table ref - somethings wrong!
					User::Leave(KWappErrUnexpectedValue); 
			break;
			}
		case KWAPP_GSM_SMS_OLD	:	// 0x41 VALUE="GSM/SMS"
			{
			// Old value of GSM/SMS tag - ONLY valid for Version 4.0 of the specs, ie WBXML = 1.0!			
			parsedField->SetFieldValueL(KWappGsmSms);		
			break;
			}
		case KWAPP_GSM_CSD		:	// 0x45 VALUE="GSM/CSD"
			{
			parsedField->SetFieldValueL(KWappGsmCsd);			
			break;
			}		
		case KWAPP_GSM_SMS_NEW	:	//  0x46 VALUE="GSM/SMS" ONLY valid for Version 5.0 of the specs, i.e.WBXML = 1.1!
			{
			parsedField->SetFieldValueL(KWappGsmSms);		
			break;
			}
		case KWAPP_GSM_USSD		:	//0x47	VALUE="GSM/USSD" only defined for version 5.0 only 
			{
			parsedField->SetFieldValueL(KWappGsmUssd);
			break;
			}
		case KWAPP_IS136		:	//  0x48   VALUE="IS136/CSD" for version 5.0 only
			{
			parsedField->SetFieldValueL(KWappIS136Csd);	
			break;
			}
		case KWAPP_GPRS			:	// 0x49    VALUE="GSM/GPRS"
			{
			parsedField->SetFieldValueL(KWappGprs);
			break;
			}
		case KWAPP_PORT_9200	:	// 0x60 VALUE="9200"
			{
			parsedField->SetFieldValueL(KWappPort9200);	
			break;
			}
		case KWAPP_PORT_9201	:	// 0x61 VALUE="9201"
			{
			parsedField->SetFieldValueL(KWappPort9201);		
			break;
			}
		case KWAPP_PORT_9202	:	// 0x62  VALUE="9202"
			{
			parsedField->SetFieldValueL(KWappPort9202);		
			break;
			}
		case KWAPP_PORT_9203	:	// 0x63  VALUE="9203"
			{
			parsedField->SetFieldValueL(KWappPort9203);		
			break;
			}
		case KWAPP_AUTOMATIC	:	//	0x64 
			{
			parsedField->SetFieldValueL(KWappAuthAutomatic);
			break;
			}
		case KWAPP_MANUAL		:	// 0x65
			{
			parsedField->SetFieldValueL(KWappAuthManual);
			break;
			}
		case KWAPP_SPEED_AUTO	:	// 0x6A VALUE="AUTO"
			{
			parsedField->SetFieldValueL(KWappSpeedAuto);		
			break;
			}
		case KWAPP_SPEED_9600	:	// 0x6B VALUE="9600"
			{
			parsedField->SetFieldValueL(KWappSpeed9600);		
			break;
			}
		case KWAPP_SPEED_14400	:	// 0x6C VALUE="14400"
			{
			parsedField->SetFieldValueL(KWappSpeed14400);		
			break;
			}
		case KWAPP_SPEED_19200	:   //0x6D  VALUE="19200"
			{
			parsedField->SetFieldValueL(KWappSpeed19200);	
			break;
			}
		case KWAPP_SPEED_28800  :   //0x6E  VALUE="28800"
			{
			parsedField->SetFieldValueL(KWappSpeed28800);	
			break;
			}
		case KWAPP_SPEED_38400  :   //0X6F	VALUE="38400"
			{
			parsedField->SetFieldValueL(KWappSpeed38400);	
			break;
			}
		case KWAPP_PAP			:	// 0x70	VALUE="PAP"
			{
			parsedField->SetFieldValueL(KWappPAP);		
			break;
			}
		case KWAPP_CHAP			:	//0X71	VALUE="CHAP"
			{
			parsedField->SetFieldValueL(KWappCHAP);			
			break;
			}
		case KWAPP_ANALOGUE		:	//0X72	VALUE="ANALOGUE"
			{
			parsedField->SetFieldValueL(KWappAnalogue);		
			break;
			}
		case KWAPP_ISDN			:	//0X73	VALUE="ISDN"
			{
			parsedField->SetFieldValueL(KWappISDN);	
			break;
			}
		case KWAPP_SPEED_43200  :   //0X74	VALUE="43200"
			{
			parsedField->SetFieldValueL(KWappSpeed43200);	
			break;
			}
		case KWAPP_SPEED_56700  :	//0X75	VALUE="56700"
			{
			parsedField->SetFieldValueL(KWappSpeed56700);	
			break;
			}
		case KWAPP_MSISDN_NO    :  //0X76	VALUE="MSISDN_NO"
			{
			parsedField->SetFieldValueL(KWappMsisdnNo);	
			break;
			}
		case KWAPP_IPV4         :	//OX77	VALUE="IPV4"
			{
			parsedField->SetFieldValueL(KWappIpv4);	
			
			break;
			}
		case KWAPP_MSCHAP		:	//0x78  VALUE="MSCHAP"
			{
			parsedField->SetFieldValueL(KWappMSCHAP);
			break;
			}
			
		default:
			User::Leave(KWappErrUnexpectedValue); // Unknown tag - can't process it
		}

	// If iCurrentCharacteristic is a general address type and NAME="BEARER". Then
	//  set the current type flag to the correct type, so we know exactly  what we're dealing with
	if (iCurrentCharacteristic==EWappAddressCharacteristic 
					&& parsedField->FieldName().CompareF(KWappBearer)==0)
		{
		if (parsedField->FieldValue().CompareF(KWappGsmCsd)==0)
			iCurrentCharacteristic =EWappGsmCsdCharacteristic;
		else if (parsedField->FieldValue().CompareF(KWappGsmSms)==0)
			iCurrentCharacteristic = EWappGsmSmsCharacteristic;
		else if (parsedField->FieldValue().CompareF(KWappGsmUssd)==0)
			iCurrentCharacteristic = EWappGsmUssdCharacteristic;
		else if (parsedField->FieldValue().CompareF(KWappIS136Csd)==0)
			iCurrentCharacteristic = EWappIS136CsdCharacteristic;
		else if (parsedField->FieldValue().CompareF(KWappGprs)==0)
			iCurrentCharacteristic = EWappGprsCharacteristic;
		else 
			User::Leave(KWappErrUnrecognised);
		}
	
	// Got our Name and Value fields- check next byte - should be 0x01 =END
	currentCharacter = GetNextMessageByteL();

	if (currentCharacter != KWAPP_END_TAG) // Our PARM is not correctly terminated!
		User::Leave(KWappErrNoTermination);
	
	// Check flag to see if parsed field should be added or not
	if (addParsedField)
		{
		// Extracted the NAME and VALUE so create a container and append to our array
		iParsedFieldArray->AppendL(parsedField);		
		CleanupStack::Pop(parsedField);
		}
	else
		{
		CleanupStack::PopAndDestroy(parsedField);	
		}
	}
//End of CMsvBIOWapAccessParser::ReadParmL()


//	SettingsNamePresentL()  - Check if the NAME characteristic is defined.  if it isn't add 
//							 a default one that is unique.  Use a base stem of "WAP Settings" and
//							 append a suffix to it. The suffix is calculated by incrementing, until
//							 a unique digit is found. 
//
void CMsvBIOWapAccessParser::SettingsNamePresentL()
	{
	TBool nameDefined = EFalse;		// Is a Name characteristic present?
	TBool bearerPresent = EFalse;	// Is address characteristic present?

	// Search  for the start of a NAME characteristic - i.e. Field Name="NAME"
	TInt nFields = iParsedFieldArray->Count();

	TInt loopCount=0;
	while (loopCount < nFields && !nameDefined)
		{
		if (iParsedFieldArray->At(loopCount)->FieldName().CompareF(KWappCharName)==0)
			{
			// At start of a Name characteristic
			// Make sure that we won't try checking past the end of the array
			if (loopCount+1 < nFields) // NB loopCount started at zero
				{
				// Verify that this is followed by NAME parm - NAME token & a non NULL value
				if (iParsedFieldArray->At(loopCount+1)->FieldName().CompareF(KWappName)==0)
					{
					if (iParsedFieldArray->At(loopCount+1)->FieldValue().Length()!=0)
						nameDefined = ETrue;
					}
				}
			}
		loopCount++;
		}

	loopCount = 0;
	while (loopCount < nFields)
		{
		// Beginning of an Address Characteristic? 
		if (iParsedFieldArray->At(loopCount)->FieldName().CompareF(KWappCharAddress) ==0)
			bearerPresent = ETrue; // Assume  all Parms are present- will leave later if not!
		loopCount++;
		}

	if (nameDefined ==EFalse && bearerPresent)   // NAME characteristic not defined - add a default
		{
		TBuf<KCommsDbSvrMaxColumnNameLength> recordName;
		HBufC*	nameBuffer = HBufC::NewLC(30);
		TInt	currentRecord = 1;
		TBool	nameExists = EFalse; 

		TFileName fileName;
		Dll::FileName(fileName);
		TParse parse;
		parse.Set(KWappResourceFile, &fileName, NULL);
		fileName = parse.FullName();
		BaflUtils::NearestLanguageFile(iFs, fileName);

		RResourceFile resourceFile;
	    resourceFile.OpenL(iFs, fileName);
	    CleanupClosePushL(resourceFile);
	    HBufC8* buffer = resourceFile.AllocReadLC(DEFAULT_WAP_SETTINGS_NAME);
	    TResourceReader resourceReader;
	    resourceReader.SetBuffer(buffer);
	    TPtrC defaultSettingsName = resourceReader.ReadTPtrC();
		CCDWAPAccessPointRecord *wapRecord = static_cast<CCDWAPAccessPointRecord*>(CCDRecordBase::RecordFactoryL(KCDTIdWAPAccessPointRecord));
		CleanupStack::PushL(wapRecord);
		nameBuffer->Des().Format(defaultSettingsName, currentRecord);  // create our name string
		while(!nameExists)
			{
			wapRecord->iRecordName.SetMaxLengthL(nameBuffer->Length());
			wapRecord->iRecordName = *nameBuffer;				
				
			if(wapRecord->FindL(*iDBSession)) 		
				{
				++currentRecord;					
				nameBuffer->Des().Format(defaultSettingsName, currentRecord); 
				}
			else 
				nameExists = ETrue;				
			}
		CleanupStack::PopAndDestroy(wapRecord);

		// Add characteristic header, then name and value to the parsed fields
		AppendNameFieldL(KWappCharName);
		AppendDataPairL(KWappName, nameBuffer->Des());
		CleanupStack::PopAndDestroy(3, nameBuffer); // nameBuffer, resourceFile, buffer
		}

	}//END of SettingsNamePresentL()

//	RestoreParsedDataL()			-  Restores CParsedField data into the array
//
//
void CMsvBIOWapAccessParser::RestoreParsedDataL()
	{
	//	Assume that our server message context is correctly set to the Bio Msg entry	
	iEntry.SetEntryL(iEntryId);
	
	// Has the entry been parsed,(iMtmData3 =1) or parsed & processed (iMtmData3 =2)
	// If not leave with error code
	if (   iEntry.Entry().MtmData3() != BIO_MSG_ENTRY_PARSED
		&& iEntry.Entry().MtmData3() != BIO_MSG_ENTRY_PROCESSED)
		{
		iSmsParsed=EFalse;			// Entry has not been parsed yet no data to restore
		User::Leave(KWappErrMsgUnparsed);
		}

	if(!iEntry.HasStoreL())
		{
		iSmsParsed=EFalse;			// Entry has no store -  not parsed yet?
		User::Leave(KWappErrStoreNotFound);  
		}
	//Message has been parsed and data stored = restore it!
	CMsvStore* store=iEntry.EditStoreL();
	CleanupStack::PushL(store);
	RestoreL(*store);
	CleanupStack::PopAndDestroy();//store

	//Now set a few key variables using the data in the iParsedFieldArray
	TLex myLex;
	if (iParsedFieldArray->At(0)->FieldName().CompareF(KWappXMLversion)==0)
		{
		myLex =iParsedFieldArray->At(0)->FieldName(); 
		myLex.Mark();
		myLex.Val(iWBXMLversionX10, EDecimal);
		}
	else
		User::Leave(KWappErrMandatoryTagMissing);  // error- XML version is missing
	
	// Field at index=1 is PublicID - don't currently need this 
	// Field at index=2 is Character Set - will present if XML version =1.1, but not for 1.0

	if (iWBXMLversionX10==11)
		{ // Have the Character Set
		if (iParsedFieldArray->At(2)->FieldName().CompareF(KWappCharacterSet)==0 )
			{
			myLex =iParsedFieldArray->At(1)->FieldName();
			myLex.Mark(); //Mark the start.  
			myLex.Val(iCharacterSet, EHex);
			}
		}
	else 
		iCharacterSet=0;
	}
// End of function  CMsvBIOWapAccessParser::RestoreParsedDataL()

// StoreParsedDataL()  - stores array of parsed data in a store - calls base class StoreL()
// 
void CMsvBIOWapAccessParser::StoreParsedDataL()
	{
	// Set to the current Entry id, create a store & save data
	iEntry.SetEntryL(iEntryId);
	CMsvStore* store=iEntry.EditStoreL();
	CleanupStack::PushL(store);
	StoreL(*store);
	CleanupStack::PopAndDestroy();//store

	// Set the parsed flag on the TMsvEntry
	iEntry.SetEntryL(iEntryId);
	TMsvEntry entry= iEntry.Entry();
	entry.SetMtmData3(BIO_MSG_ENTRY_PARSED);

	iEntry.ChangeL(entry);
	}

//
//		ResetProcessVariablesL()-Resets array of bookmarks and recreates the objects mapping to CommDb Table s
//
//
void CMsvBIOWapAccessParser::ResetProcessVariablesL()
	{
	// Re-set boolean flags
	iGsmCsdData		= EFalse;
	iGsmSmsData		= EFalse;
	iGprsData		= EFalse;
	iURLData		= EFalse;
	iMMSURLData		= EFalse;
	iNameData		= EFalse;
	iIdData			= EFalse;
	// Zero IDs
	iRecordId = 0; 
	iWapAccessPtID = 0;	
	iWapLocation = 0; 
	iWapChargeCard = 0;

	iBookmarkList->ResetAndDestroy();

	iWAPSettingsName.Set(TPtrC());  // Null the value in this descriptor

	delete iGsmCsdWapIpTable;
	iGsmCsdWapIpTable = NULL;
	delete iGprsWapIpTable;
	iGprsWapIpTable =NULL;
	delete iWapIpISPTable;
	iWapIpISPTable = NULL;
	delete iWapSmsTable;
	iWapSmsTable = NULL;
	delete iWapGprsTable;
	iWapGprsTable = NULL;
	delete iModemInitString;
	iModemInitString = NULL;
	}

// 
// Read_mb_u_int32() -- Helper function that uses the TLex iSms to extract a 32-bit 
//						integer encoded in a number of bytes. If Bit7 of the first
//						byte is set, the number continues over into the next byte,
//						and so on. If Bit7 is not set then it's the last byte.
//
TUint32  CMsvBIOWapAccessParser::Read_mb_u_int32L()
	{
	TUint32 decodedValue = 0;
	TBool	completed =EFalse;

	while(!completed)
		{
		TUint32 thisByte = (TUint) GetNextMessageByteL();  // cast the character returned by Get to a TUint32
		if (decodedValue & 0x2000000)   // is Bit25 set? If so, bitshifting 7 places 
			User::Leave(KWappErrOutsideBoundaries);	// left will cause overflow 		
		
		(decodedValue = decodedValue<<7); // assignment to keep compiler happy 
								//shift current value 7 places to the left, i.e. *256

		if ( (thisByte & KWAPP_MB_INT32_CONTINUATION) ) 
			{ 
			// bit 7 set, integer continues into next byte so keep processing
			thisByte = thisByte & 0x7F; //	mask Bit7 by AND'ing with all other bits 
									    // set to 1, i.e. 01111111 =0x7F
			decodedValue += thisByte;
			// leave completed set to EFalse
			}
		else    // bit 7 isn't set, processing's completed so set flag  
			{
			decodedValue += thisByte;
			completed = ETrue;
			}
		}
	return decodedValue;
	}  

//
//		GetStringTableItemL()	- Returns the string contained within the string table starting
//								  at the offset passed in as a parameter.
//				
HBufC* CMsvBIOWapAccessParser::ReadStringTableReferenceLC(TUint32 anOffset)
	{
	if (iStringTable ==NULL)
		User::Leave(KWappErrStringTable);		// Making a call to an nonexistant object! 
	TUint32 stringTableLength = iStringTable->Length();
	if (stringTableLength<=0)					//Nothing to process, or worse, an error!
		User::Leave(KWappErrOutsideBoundaries);	//Past end of table
	if (anOffset >= stringTableLength)			// anOffset must be < last element of buffer								
		User::Leave(KWappErrOutsideBoundaries);	// Last element must be NULL terminator!

	TUint32 stringTableItem = anOffset; // Current element in iStringTable
	TUint32 stringLength=0; // Length of string
	while ( iStringTable->Des()[stringTableItem] !=KWAPP_STR_TERM)
		{	
		// increment through the String Table to first NULL terminator 
		stringTableItem++;
		if (stringTableItem > (TUint)iStringTable->Length())  // Cast to remove warning, Length >=0
			User::Leave(KWappErrOutsideBoundaries);   // Problem -gone past the end of the descriptor 
		stringLength++;
		}	
	// return a descriptor starting at the offset and up to the char before the NULL terminator

	// Char Conversion
	TPtrC data = iStringTable->Des().Mid(stringTableItem, stringTableLength-1);
	HBufC* text = HBufC::NewLC(data.Length());
	TPtr des(text->Des());
	iCharsetConverter->ConvertL(data, des);
	return text;
	}


/*
Finds the end of the UCS2 string. Each character is represented by exactly 2bytes of data 
According to WBXML version 1.0 
"if a character encoding includes a NULL then that NULL character must be used as the termination character"
Hence we are assuming that the end of the string is marked by two NULL bytes.
*/
void CMsvBIOWapAccessParser::ReadUCS2StringL()
{
	TBool endofinlinestring = EFalse;
	TBool endoflexstring = EFalse; 
	TBool foundNull;

	while (!endoflexstring && !endofinlinestring)
	{
		foundNull = EFalse;
		endoflexstring = iSms.Eos();

		if (!endoflexstring)
			{
			if (iSms.Peek() == KWAPP_STR_TERM)
				{
				foundNull = ETrue;
				}
			
			iSms.Inc();
			if (iSms.Eos()) //check we haven't reached the end of the sms
				User::Leave(KWappErrEOS);

			else if (foundNull && iSms.Peek()== KWAPP_STR_TERM)
				endofinlinestring = ETrue;
			}
		else
			User::Leave(KWappErrEOS);
	}
	
}



/*
According to WBXML version 1.0 
"if a character encoding includes a NULL then that NULL character must
 be used as the termination character"

Finding the end of the ASCII string:
 This is simply done as each character is represented by a maximum 
 of 7 bits of data, hence each character is one byte only. 
 Thus, it is assumed that the end of the string is marked by a NULL which in ASCII is 0x00

  so the string "hi" would be represented as 
  0x68 0x69 0x00

Finding the end of the UTF-8 string. 
 From RFC 2279:
 "In UTF-8, characters are encoded using sequences of 1 to 6 octets.
   The only octet of a "sequence" of one has the higher-order bit set to
   0, the remaining 7 bits being used to encode the character value. In
   a sequence of n octets, n>1, the initial octet has the n higher-order
   bits set to 1, followed by a bit set to 0.  The remaining bit(s) of
   that octet contain bits from the value of the character to be
   encoded.  The following octet(s) all have the higher-order bit set to
   1 and the following bit set to 0, leaving 6 bits in each to contain
   bits from the character to be encoded.

   The table below summarizes the format of these different octet types.
   The letter x indicates bits available for encoding bits of the UCS-4
   character value.

   UCS-4 range (hex.)           UTF-8 octet sequence (binary)
   0000 0000-0000 007F   0xxxxxxx
   0000 0080-0000 07FF   110xxxxx 10xxxxxx
   0000 0800-0000 FFFF   1110xxxx 10xxxxxx 10xxxxxx

   0001 0000-001F FFFF   11110xxx 10xxxxxx 10xxxxxx 10xxxxxx
   0020 0000-03FF FFFF   111110xx 10xxxxxx 10xxxxxx 10xxxxxx 10xxxxxx
   0400 0000-7FFF FFFF   1111110x 10xxxxxx ... 10xxxxxx
 "
 ASCII values 0-127 are represented in UTF8 by one byte with the msb set to 0.

 This means that no byte in a stream of bytes representing a (non NULL) character will ever be 0x00.
 So there will only be one 0x00 in the string and this will represent the end of the string.

 For example
  the string of roman numerals "VIIIIII" where VIII is the first character and III is the second character
  would be represented in UTF-8 as:

  0xE2 0x85 0xA2 0xE2 0x85 0xA7 0x00

 The single byte 0x00 is the NULL terminator representing the end of the string.
*/
void CMsvBIOWapAccessParser::ReadASCIIorUTF8StringL()
{
	while (!iSms.Eos() && iSms.Peek() !=KWAPP_STR_TERM /*0x00*/)
		{
		iSms.Inc();
		}

	if (iSms.Eos()) // End of the string! Other chars should follow
			User::Leave(KWappErrEOS); 

}

/*
because iSms is a TLex it is padded out and so 
when parsing unicode strings you end up with unnecessary characters
*/

//HBufC* CMsvBIOWapAccessParser::RemovePadding()
//{
//}


//
//		ReadInlineStringL()	- Returns the string begining at the current position in the 
//							  message. Increments TLex position until string termination found.
//
HBufC* CMsvBIOWapAccessParser::ReadInlineStringLC()
	{
	if (!iSms.Eos())//Mark the starting point of the string
		iSms.Mark(); 
	else			// End of the string! Can't proceed!
		User::Leave(KWappErrEOS);

		//find the end of the string by taking into accout how it is encoded
	switch (iCharacterSet)
		{
		case KCharsetASCII:
		case KCharsetUTF8:
			ReadASCIIorUTF8StringL();
			break;
		case KCharsetUCS2:
			ReadUCS2StringL();
			break;
		default:
			User::Leave(KErrNotSupported);
			break;
		};
	
	//the end of the string has been found so mark it. 
	TPtrC data =iSms.MarkedToken();
	iSms.Inc(); //Increment up to the KWAPP_STR_TERM so next time we read the char after the end of the string
	/*
	Marked token is now converted to unicode 
	for the purpose of applying to commdb for example. 
	*/
	
	// Char Conversion
	//for unicode parsing to remove unnecessary padding	
	if (iCharacterSet == KCharsetUCS2)
		{
		HBufC* text = HBufC::NewMaxLC(data.Length() / 2);
		TPtr destext(text->Des()); 

		TInt	j = 0;
		TInt	i = 1;

		while (j < data.Length() / 2)
			{
			destext[j] = data[i];
			i += 2;
			j++;
			}

		return text;
		}
	else
		{
		HBufC* text = HBufC::NewMaxLC(data.Length());
		TPtr destext(text->Des()); 

		iCharsetConverter->ConvertL(data, destext);

		return text;
		}
	}


//
//		AppendDataPairL()	- Appends a CParsedField containing data from the message, to the 
//							  array of parsed fields (iParsedFieldsArray).
//
void CMsvBIOWapAccessParser::AppendDataPairL(const TDesC& aFieldName, const TDesC& aFieldValue)
	{
	// Appends a CParsedField object to iParsedFieldArray.
	// Both fields have valid data
	CParsedField* localData = new (ELeave) CParsedField;
	CleanupStack::PushL(localData);
	localData->SetFieldNameL(aFieldName);
	localData->SetFieldValueL(aFieldValue);
	CleanupStack::Pop();
	iParsedFieldArray->AppendL(localData);
	}

//
//		AppendNameFieldL()	- Appends a CParsedField containing just a Name from the message, 
//							  to the array of parsed fields (iParsedFieldsArray).
//
void CMsvBIOWapAccessParser::AppendNameFieldL(const TDesC& aFieldName)
	{
	// Appends a CParsedField object to iParsedFieldArray.
	// Only the Name field has valid data, the Value field is left NULL
	CParsedField* localData = new (ELeave) CParsedField;
	CleanupStack::PushL(localData);
	localData->SetFieldNameL(aFieldName);
	localData->SetFieldValueL(KNullDesC);  // Set to an empty descriptor
	CleanupStack::Pop();
	iParsedFieldArray->AppendL(localData);
	}

void CMsvBIOWapAccessParser::InsertDataPairL(const TDesC& aFieldName, const TDesC& aFieldValue, TInt anIndex)
	{	
	// check that the position is valid!  Leave if out of bounds
	// Limits 0 - insert at front of array, iParsedData->Count() append to end
	if (!(anIndex<= iParsedFieldArray->Count() && anIndex >= 0) )
		User::Leave(KErrArgument);
	CParsedField* localField = new (ELeave)CParsedField;
	CleanupStack::PushL(localField);
	localField->SetFieldNameL(aFieldName);
	localField->SetFieldValueL(aFieldValue);
	CleanupStack::Pop();
	iParsedFieldArray->InsertL(anIndex, localField);
	}

TChar CMsvBIOWapAccessParser::GetNextMessageByteL()
	{	
	if (iSms.Eos())  // Error - unexpectedly reached the end of the TLex buffer!!!!!
		User::Leave(KWappErrEOS);
	return iSms.Get();
	}

TInt CMsvBIOWapAccessParser::LocateParsedFieldName(const TDesC& aFieldName, const TInt aStartIndex)
	{
	//Searches from a start pos'n, default=0, until either finds a match, or end of array
	TInt  count=aStartIndex;
	TBool nextCharacteristic =EFalse;
	
	//loop until end of the array, found the first match or found another Characteristic!
	TInt nFields = iParsedFieldArray->Count();
	while	(count < nFields &&			//	End of array?
			(iParsedFieldArray->At(count)->FieldName().CompareF(aFieldName)  !=0) && //Match found?
			!nextCharacteristic  )
		{

		if ( (count !=aStartIndex)  && (IsCharacteristicName(count)) )
			// Not 1st first element of characteristic and found field for another
			// characteristic- Must have reached next characteristic - stop looping
			{
			nextCharacteristic =ETrue;
			}

		++count;
		}

	if (count < nFields && !nextCharacteristic)    // found a match in the array, even if it is last element
		return count;
	else 
		return KErrNotFound;  // Didn't find a match or reached next characteristic - flag the errror
	}

//
//	DoProcessL()  - Workhorse for ProcessL function.  Extracts information from parsed field 
//					array into separate objects. Writes settings to the COMMS DB, if 
//					there is data for at least one bearer type. Saves any Bookmarks to file.
//
void CMsvBIOWapAccessParser::DoProcessL()
	{
	if (iGsmCsdData || iGsmSmsData || iGprsData) 
		{
		WriteDataToDBL();
		}
	WriteBookmarksToFileL();

	if( iMMSURLData )
		{
		WriteMMSURLL();
		}		

	if (!iGprsData && !iGsmCsdData && !iGsmSmsData && !iMMSURLData && (iBookmarkList->Count()==0)) 
		{
		// No bearers, no bookmarks - must be a settings message - wouldn't be here otherwise 
		if (iNameData && iURLData)   // iIDData optional
			{
			UpdateStartPageUrlL();   // update the URL of the start page in WAP table		
			}
		}
		
	}

//
//	PreProcessL -	goes through the fields in the array of parsed data, extracting information
//					only extracts information for first of each of the 4 bearer types - i.e. max  
//					of 4 entries in the ISP/WAP tables. Additional address characteristics are 
//					ignored. 
//
void CMsvBIOWapAccessParser::PreProcessL()
	{
	TInt arrayPtr=0;   // current position in the array of ParsedFields

	while (arrayPtr < iParsedFieldArray->Count())
		{
		// Check current field if characteristic - peek at next field -> bearer type
					// GSM/CSD		- extract data from GSM/CSD Address characteristic
					// GSM/SMS		- extract data from GSM/SMS Address characteristic
					// GPRS			- extract data from GPRS Address characteristic
					// GSM/USSD		- TO DO,  GSM/USSD Address characteristic unsupported
					// IS136/CSD	- TO DO,  IS136/CSD Address characteristic unsupported 
					// ID			- extract data from ID characteristic
					// NAME			- extract data from NAME characteristic
					// URL			- extract data from URL characteristic
					// BOOKMARK		- extract Bookmark characteristic & append to Bookmark array
		// Otherwise report an error

		// Increment past WBXML Version, public ID or character set to the data we want
		if ((iParsedFieldArray->At(arrayPtr)->FieldName().CompareF(KWappXMLversion) ==0) ||
			(iParsedFieldArray->At(arrayPtr)->FieldName().CompareF(KWappPublicID) ==0)||
			(iParsedFieldArray->At(arrayPtr)->FieldName().CompareF(KWappCharacterSet) ==0))
		
			arrayPtr++;	
		else if	(iParsedFieldArray->At(arrayPtr)->FieldName().CompareF(KWappCharAddress) ==0)
			{
			// Stepped past first 3 fields & its an Address Characteristic
			// So read the next line to determine type of bearer 

			TPtrC addressValue;
			arrayPtr++;			//increment to the next element 
			
			if (iParsedFieldArray->At(arrayPtr)->FieldName().CompareF(KWappBearer)==0) 
				addressValue.Set(iParsedFieldArray->At(arrayPtr)->FieldValue());   // Note the bearer type
			else
				User::Leave(KWappErrMandatoryTagMissing); // Not a BEARER - error!

			// Next - either extract the bearer data into the table objects, if this is the 
			// first of this type, or skip past the data to the next characteristic. 
			if ((addressValue.CompareF(KWappGsmCsd)==0)) 
				{
				// GSM/CSD Characteristic so add data to ISP and WAP table objects, or go to next characteristic
				ExtractBearerDataL(arrayPtr); 
				}
			
			else if ((addressValue.CompareF(KWappGsmSms)==0)) 
				{
				// GSM/CSD Characteristic so either add data to ISP and WAP table objects, or skip past to next characteristic
				ExtractBearerDataL(arrayPtr);
				}

			else if (addressValue.CompareF(KWappGsmUssd)==0)
				{
				User::Leave(KWappErrNotSupported);  // Don't support these at the moment!

		//		ExtractBearerDataL(arrayPtr); //Add data to ISP and WAP table objects
				}

			else if (addressValue.CompareF(KWappIS136Csd)==0)
				{
				User::Leave(KWappErrNotSupported);  // Don't do these at the moment!
		//		ExtractBearerDataL(arrayPtr); //Add data to ISP and WAP table objects 
				}
			else if (addressValue.CompareF(KWappGprs)==0)
				{
				ExtractBearerDataL(arrayPtr);
				}
			else 
				User::Leave(KWappErrUnrecognised); // Value doesn't match any of the known ones
			}

		// Check if it's a  URL characteristic
		else if (iParsedFieldArray->At(arrayPtr)->FieldName().CompareF(KWappCharURL) ==0)
			{ 
			// The URL Characteristic has NO PARM elements only a value

			if (iURLData == EFalse)  // Not been set previously
				{
				// Use the first instance of a URL - set it to the characteristic value even if it's null 
				iUrl.Set(iParsedFieldArray->At(arrayPtr)->FieldValue()); 
				iURLData = ETrue;
				}
			arrayPtr++;
			}

		// Check if it's a Name Characteristic
		else if (iParsedFieldArray->At(arrayPtr)->FieldName().CompareF(KWappCharName) ==0)
			{ 
			// The NAME Characteristic should have EXACTLY 1 PARM element
			arrayPtr++; // increment to the Parm in this characteristic
			if (arrayPtr == iParsedFieldArray->Count() || 
				iParsedFieldArray->At(arrayPtr)->FieldName().CompareF(KWappName)!=0) 
				User::Leave(KWappErrBadMessage);
			
			// Initialise the Settings Name variable if it hasn't been set
			if (iWAPSettingsName.Length()==0)    
				{
				// Use the first instance of NAME - set it to the value even if it's null 
				iWAPSettingsName.Set(iParsedFieldArray->At(arrayPtr)->FieldValue());
				iNameData = ETrue;
				}
			
			while ( arrayPtr< iParsedFieldArray->Count() && !(IsCharacteristicName(arrayPtr)) )
					arrayPtr++;   // increment to beginning of next characteristic
			
			}

		//Is it a BOOKMARK ?  Bookmarks must contain EXACTLY 2 PARM elements - NAME then URL 
		else if (iParsedFieldArray->At(arrayPtr)->FieldName().CompareF(KWappCharBookmark)==0)
			{  
			arrayPtr++;							
			ExtractBookmarkDataL(arrayPtr);
			}

		// Is it an ID characteristic ?
		else  if (iParsedFieldArray->At(arrayPtr)->FieldName().CompareF(KWappCharID) ==0)
			{  
			arrayPtr++;		//Increment to the NAME Parm

			// End of array or PARM Type != NAME
			if (arrayPtr == iParsedFieldArray->Count() || 
				iParsedFieldArray->At(arrayPtr)->FieldName().CompareF(KWappName)!=0)
				User::Leave(KWappErrBadMessage);

			if (iIdData == EFalse)
				{
				// Use the 1st instance of an ID - set to value even if it's null!
				iID.Set(iParsedFieldArray->At(arrayPtr)->FieldValue());
				iIdData = ETrue;
				}

			while ( arrayPtr< iParsedFieldArray->Count() && !(IsCharacteristicName(arrayPtr)) )
					arrayPtr++;   // increment to beginning of next characteristic
 			}
		else if(iParsedFieldArray->At(arrayPtr)->FieldName().CompareF(KWappCharMMSURL) ==0)
			{
			// The MMSURL Characteristic has NO PARM elements only a value

			if (iMMSURLData == EFalse)  // Not been set previously
				{
				// Use the first instance of a URL - set it to the characteristic value even if it's null 
				iMMSUrl.Set(iParsedFieldArray->At(arrayPtr)->FieldValue()); 
				iMMSURLData = ETrue;
				}

			arrayPtr++; // Increment to the next element in the array
			}

		else // Unrecognised characteristic token
			User::Leave(KWappErrUnrecognised);
		}
	}
// END of function  CMsvBIOWapAccessParser::PreProcessL()

//
//	ExtractBearerDataL()	-  
//
//			Called by PreProcessL when it finds an address bearer type.  Uses the reference to 
//			the current position in iParsedFieldArray to extract info to insert into CWapTableData 
//			and CIspTableData objects. Increment the index position as the function scans
//			through the iParsedFieldArray - ensures the value in calling function is correct. 
//
void CMsvBIOWapAccessParser::ExtractBearerDataL(TInt& aCurrentPos)
	{
	
	TWappBearerType currentBearer = EWappUnknownBearer;

	// local flags - 
	// For all address characteristics, only process 1st instance of each parm type - ignore the rest
	TBool bearer			= EFalse;
	TBool proxy				= EFalse;
	TBool port				= EFalse;
	TBool csdDialstring		= EFalse;
	TBool pppAuthType		= EFalse;
	TBool pppAuthName		= EFalse;
	TBool pppAuthSecret		= EFalse;
	TBool pppLoginType		= EFalse;
	TBool csdCallType		= EFalse;
	TBool csdCallSpeed		= EFalse;
	TBool pxAuthName		= EFalse;
	TBool pxAuthSecret		= EFalse;
	TBool pxLoginType		= EFalse;
	TBool ispName			= EFalse;
	TBool smsSMSCAddr		= EFalse;
//	TBool ussdServiceCode	= EFalse;  //Currently not used - will be needed if GSM/USSD supported

	TInt arrayEnd  = iParsedFieldArray->Count();
	TCsdCallType  callType =EAnalogue;
	TCsdCallSpeed callSpeed =EAutobauding;

	// First check - current parm  MUST  BE A BEARER
	// Following Parms can be in any order, but the first must be a BEARER.

	if (iParsedFieldArray->At(aCurrentPos)->FieldName().CompareF(KWappBearer) !=0)
		User::Leave(KWappErrMandatoryTagMissing);   // First Parm is not a BEARER - big problem!!


	while ( aCurrentPos< arrayEnd &&  // Stop checking if past the array end, or next value
			!(IsCharacteristicName(aCurrentPos))  )     // is another Characteristic, ie.  address, URL, Bookmark, ID or Name
		{ 
		TPtrC fieldName = iParsedFieldArray->At(aCurrentPos)->FieldName();
		if (IsValidTokenNameL(aCurrentPos) !=KErrNone) //Validate this Parm Name
			User::Leave(KWappErrUnrecognised);		   
		
		// Check the next parsed field data -  BEARER type

		if (fieldName.CompareF(KWappBearer) ==0)
			{
			//Set the current local bearer type, & then bearer variable
			TPtrC currentValue = iParsedFieldArray->At(aCurrentPos)->FieldValue();
			if (currentValue.CompareF(KWappGsmCsd) ==0)
				{
				if (!bearer)   // first time here!
					{
					if (!iGsmCsdData) // No previous GSM bearers
						{
						iGsmCsdWapIpTable		= CWapIpBearerData::NewL();   // Create the required table mapping objects
						iWapIpISPTable	= CIspTableData::NewL();
						currentBearer = EWappGsmCsdBearer;// set local bearer type variable
						bearer = ETrue;  // ignore further BEARER's only for this characteristic
						}
					else // Already got data increment to next characteristic and return
						{
						while ( aCurrentPos< arrayEnd && !(IsCharacteristicName(aCurrentPos)) )
							aCurrentPos++;   // increment to beginning of next characteristic
						return;				 // return to PrePocessL
						}
					}
				}

			else if (currentValue.CompareF(KWappGsmSms) ==0)
				{
				if (!bearer)  // haven't set the bearer type yet,  ignore all after first
					{
					if (!iGsmSmsData)  // No previous SMS bearers
						{
						iWapSmsTable =  CWapSmsBearerData::NewL();
						currentBearer =EWappGsmSmsBearer;
						bearer = ETrue;			// ignore any further BEARERs in this Characteristic 
						}
					else   // Already got bearer data  goto next characteristic & return
						{
						while ( aCurrentPos< arrayEnd && !(IsCharacteristicName(aCurrentPos)) )
							aCurrentPos++;  // increment to beginning of next characteristic
						return;				// return to PrePocessL
						}
					}
				}
			
			else if (currentValue.CompareF(KWappGsmUssd) ==0)
				{
				currentBearer =EWappGsmUssdBearer;
				//NB   Add support for USSD bearer here when required
				// NOTE - must consider proxy & proxy type with USSD!
				User::Leave(KWappErrNotSupported);
				}
			
			else if (currentValue.CompareF(KWappIS136Csd) ==0)
				{
				currentBearer =EWappIS136CsdBearer;
				//NB  Add support for IS 136 when required
				// NOTE - will it be the same as for GSM/CSD ?
				User::Leave(KWappErrNotSupported);
				}
			else if (currentValue.CompareF(KWappGprs) ==0)
				{
				if (!bearer)   // first time here!
					{
					if (!iGprsData) // No previous bearers
						{
						iGprsWapIpTable		= CWapIpBearerData::NewL();   // Create the required table mapping objects
						iWapGprsTable =  CWapGprsBearerData::NewL();
						iWapIpISPTable = CIspTableData::NewL();
						currentBearer = EWappGprsBearer;// set local bearer type variable
						bearer = ETrue;  // ignore further BEARER's only for this characteristic
						}
					else // Already got bearer data, increment to next characteristic and return
						{
						while ( aCurrentPos< arrayEnd && !(IsCharacteristicName(aCurrentPos)) )
							aCurrentPos++;   // increment to beginning of next characteristic
						return;				 // return to PrePocessL
						}
					}
				}
			else 
				User::Leave(KWappErrUnrecognised);   // Unknown bearer type - Help!!!!
			}

		//Check for the PROXY characteristics - String used to set Table Data
	
		// CSD proxy value into		iIspTable::iGateway	    -  goes in   ISP_IP_GATEWAY 
		//							iWapTable::iGatewayAddr	-  goes into WAP_GATEWAY_ADDRESS
		// SMS proxy value into		iWapTable::iGatewayAddr	-  goes into WAP_GATEWAY_ADDRESS
			
		// NB If support for USSD, or IS136 required, add additional code here.
		else if (fieldName.CompareF(KWappProxy) ==0)
			{
			if(!proxy) // First PROXY for this characteristic
				{
				switch(currentBearer)
					{
					case EWappGsmSmsBearer:
						iWapSmsTable->iGatewayAddr.Set(iParsedFieldArray->At(aCurrentPos)->FieldValue());
						break;
					case EWappGsmCsdBearer:
						iGsmCsdWapIpTable->iGatewayAddr->SetAddrL(iParsedFieldArray->At(aCurrentPos)->FieldValue());
						iWapIpISPTable->iGateway->SetAddrL(iParsedFieldArray->At(aCurrentPos)->FieldValue());
						break;
					case EWappGprsBearer:
						iGprsWapIpTable->iGatewayAddr->SetAddrL(iParsedFieldArray->At(aCurrentPos)->FieldValue());
						iWapGprsTable->iGatewayAddr->SetAddrL(iParsedFieldArray->At(aCurrentPos)->FieldValue());
						break;
					default: 	
						User::Leave(KWappErrUnrecognised); // Unrecognised currentBearer
					}
				proxy = ETrue;
				}
			}


		//Check for the PORT characteristics - first encounter set it, ignore it afterwards
		else if ((fieldName.CompareF(KWappPort)==0) &&  !port)   
			{   
			// Record that we've checked a port value - should be one of 4 values!
			
			if (  (iParsedFieldArray->At(aCurrentPos)->FieldValue().CompareF(KWappPort9200) ==0)
				||(iParsedFieldArray->At(aCurrentPos)->FieldValue().CompareF(KWappPort9201) ==0) 
				||(iParsedFieldArray->At(aCurrentPos)->FieldValue().CompareF(KWappPort9202) ==0)
				||(iParsedFieldArray->At(aCurrentPos)->FieldValue().CompareF(KWappPort9203) ==0))
			
				{
				if (currentBearer == EWappGsmSmsBearer)
					iWapSmsTable->iPort.Set(iParsedFieldArray->At(aCurrentPos)->FieldValue());
				else if (currentBearer == EWappGsmCsdBearer)
					iGsmCsdWapIpTable->iPort.Set(iParsedFieldArray->At(aCurrentPos)->FieldValue());
				else if (currentBearer == EWappGprsBearer)
					iGprsWapIpTable->iPort.Set(iParsedFieldArray->At(aCurrentPos)->FieldValue());
				}
			else
				{
				if (currentBearer == EWappGsmSmsBearer)
					iWapSmsTable->iPort.Set(KWappPort9200);
				else if (currentBearer == EWappGsmCsdBearer)
					iGsmCsdWapIpTable->iPort.Set(KWappPort9200);
				else if (currentBearer == EWappGprsBearer)
					iGprsWapIpTable->iPort.Set(KWappPort9200);
				}
			port = ETrue;
			}


// ******* Following group are for GSM/CSD & IS136/CSD - GSM/SMS and GSM/USSD follow after ********

		//	Is it a CSD_DIALSTRING? 
		else if (fieldName.CompareF(KWappCsdDial)==0)
			{
			// Must be either GSM/CSD or IS136/CSD
			if ( (currentBearer != EWappGsmCsdBearer) &&
				  (currentBearer != EWappIS136CsdBearer) )
				  User::Leave(KWappErrUnexpectedValue);
			else if (!csdDialstring)
				{
				iWapIpISPTable->iDefaultPhoneNum.Set(iParsedFieldArray->At(aCurrentPos)->FieldValue());
				csdDialstring=ETrue;
				}
			}

		//	Check for PPP_AUTHTYPE 
		//  Now don't set the iIFName - always "ppp" & this protocol takes care of rest
		else if (fieldName.CompareF(KWappPPPAuthType)==0 && !pppAuthType)
			{
			if(currentBearer==EWappGprsBearer)
				{
				if(iParsedFieldArray->At(aCurrentPos)->FieldValue().CompareF(KWappPAP)==0)
					iWapGprsTable->iPlainTextAuthDisabled = EFalse;
				else 
					iWapGprsTable->iPlainTextAuthDisabled = ETrue;
				}
			// set the flag so we don't check again
			pppAuthType =ETrue;
			}

		//	Check if its a PPP_AUTHNAME
		else if (fieldName.CompareF(KWappPPPAuthName)==0)
			{
			if (!pppAuthName)
				{
				switch(currentBearer)
					{
					case EWappGsmCsdBearer:
					case EWappIS136CsdBearer:
						iWapIpISPTable->iLoginName.Set(iParsedFieldArray->At(aCurrentPos)->FieldValue());
						break;
					case EWappGprsBearer:
						iWapGprsTable->iLoginName.Set(iParsedFieldArray->At(aCurrentPos)->FieldValue());
						break;
					default:
						// SMS, USSD and GPRS don't have CSD authentication names
						User::Leave(KWappErrUnexpectedValue); 
					}
				pppAuthName = ETrue;
				}
			}

		//	Is it PPP_AUTHSECRET?
		else if (fieldName.CompareF(KWappPPPAuthSecret)==0 )
			{

			if (!pppAuthSecret)
				{
				switch(currentBearer)
					{
					case EWappGsmCsdBearer:
					case EWappIS136CsdBearer:
						iWapIpISPTable->iLoginPass.Set(iParsedFieldArray->At(aCurrentPos)->FieldValue());
						break;
					case EWappGprsBearer:
						iWapGprsTable->iLoginPass.Set(iParsedFieldArray->At(aCurrentPos)->FieldValue());
						break;
					default:
						// SMS, GPRS & USSD don't have authentication type
						User::Leave(KWappErrUnexpectedValue);
					}
				pppAuthSecret = ETrue;
				}
			}
		//	Is it a PPP_LOGIN_TYPE? 
		else if (fieldName.CompareF(KWappPPPLoginType)==0)
			{
			if(!pppLoginType)
				{
				const TDesC& val = iParsedFieldArray->At(aCurrentPos)->FieldValue();
				switch(currentBearer)
					{
					case EWappGsmCsdBearer:
					case EWappIS136CsdBearer:
						{
						if(val.CompareF(KWappAuthAutomatic)==0)
							{
							iWapIpISPTable->iIFPromptForAuthentication = EFalse;
							iWapIpISPTable->iPromptForLoginDetails = EFalse;
							}
						else
							{
							iWapIpISPTable->iIFPromptForAuthentication = ETrue;
							iWapIpISPTable->iPromptForLoginDetails = ETrue;
							}
						}
						break;
					case EWappGprsBearer:
						{
						if(val.CompareF(KWappAuthAutomatic)==0)
							iWapGprsTable->iIFPromptForAuth = EFalse;
						else
							iWapGprsTable->iIFPromptForAuth = ETrue;
						}
						break;
					default:
						User::Leave(KWappErrUnexpectedValue);
					}
				}
			pppLoginType = ETrue;
			}

		//	Is it a CSD_CALLTYPE? 
		else if (fieldName.CompareF(KWappCsdCallType)==0)
			{
			// ONLY  GSM/CSD bearer has call types
			if ( (currentBearer !=  EWappGsmCsdBearer) )
				  User::Leave(KWappErrUnexpectedValue);

			else if (!csdCallType)
				{
				// Can have values ANALOGUE (don't set  nothing) & ISDN (set modem init string)
				if (iParsedFieldArray->At(aCurrentPos)->FieldValue().CompareF(KWappISDN)==0)
					{
					// TODO implement this function when Symbian know device specs
					SetModemInitStringL(); 
					callType = EIsdn;
					}
				else if ((iParsedFieldArray->At(aCurrentPos)->FieldValue().CompareF(KWappAnalogue)==0))
					{
					callType = EAnalogue;
					}
				else
					callType = EAnalogue;  // Not recognised so set to default of ANALOGUE
				csdCallType =ETrue;
				}
			}

		//Check if its a CSD_CALLSPEED
		else if (fieldName.CompareF(KWappCsdCallSpeed)==0)
			{
			// ONLY CSD bearer has call speed - anything else is an error
			if  (currentBearer !=  EWappGsmCsdBearer)
				  User::Leave(KWappErrUnexpectedValue);
			else if (!csdCallSpeed)
				{
				TPtrC value = iParsedFieldArray->At(aCurrentPos)->FieldValue();
				// Set the local variable for callspeed - will set the iIspTable value at end of fn
				if (value.CompareF(KWappSpeedAuto)==0)
					callSpeed = EAutobauding ; // Auto speed select - device chooses
				else if (value.CompareF(KWappSpeed9600)==0)
					callSpeed = E9600baud;
				else if (value.CompareF(KWappSpeed14400)==0)
					callSpeed = E14400baud;
				else if (value.CompareF(KWappSpeed19200)==0)
					callSpeed = E19200baud;
				else if (value.CompareF(KWappSpeed28800)==0)
					callSpeed = E28800baud;
				else if (value.CompareF(KWappSpeed38400)==0)
					callSpeed = E38400baud;
				else if (value.CompareF(KWappSpeed43200)==0)
					callSpeed = E43200baud;
				else if (value.CompareF(KWappSpeed56700)==0)
					callSpeed = E57600baud;
				else 
					callSpeed = EAutobauding;// Set to the default of AUTO 

				csdCallSpeed =ETrue;
				}
			}

		//	Check if its a PROXY_AUTHNAME - must be CSD or GPRS
		else if (fieldName.CompareF(KWappProxyAuthName)==0 )
			{
			switch(currentBearer)
				{
				case EWappGsmCsdBearer:
				case EWappIS136CsdBearer:
					if (!pxAuthName)
						{
						iGsmCsdWapIpTable->iProxyAuthName.Set(iParsedFieldArray->At(aCurrentPos)->FieldValue());
						pxAuthName =ETrue;
						}
					break;
				case EWappGprsBearer:
					if (!pxAuthName)
						{
						iGprsWapIpTable->iProxyAuthName.Set(iParsedFieldArray->At(aCurrentPos)->FieldValue());
						pxAuthName =ETrue;
						}
					break;
				default:
					User::Leave(KWappErrUnexpectedValue);
				}
			}

		//	Is it a PROXY_AUTHSECRET?  
		else if (fieldName.CompareF(KWappProxyAuthSecret)==0)
			{
			switch(currentBearer)
				{
				case EWappGsmCsdBearer:
				case EWappIS136CsdBearer:
					if (!pxAuthSecret)
						{
						iGsmCsdWapIpTable->iProxyAuthSecret.Set(iParsedFieldArray->At(aCurrentPos)->FieldValue());
						pxAuthSecret =ETrue;
						}
					break;
				case EWappGprsBearer:
					if (!pxAuthSecret)
						{
						iGprsWapIpTable->iProxyAuthSecret.Set(iParsedFieldArray->At(aCurrentPos)->FieldValue());
						pxAuthSecret =ETrue;
						}
					break;
				default:
					User::Leave(KWappErrUnexpectedValue);
				}
			}

		// Is it a PROXY_LOGIN_TYPE?
		
		else if (fieldName.CompareF(KWappProxyLoginType)==0)
			{
			//We just set the variable in the mapping object for the WAP IP Bearer Table. Don't do anything with it yet.
			switch(currentBearer)
				{
				case EWappGsmCsdBearer:
				case EWappIS136CsdBearer:
					if (!pxAuthSecret)
						{
						if (iParsedFieldArray->At(aCurrentPos)->FieldValue().CompareF(KWappAuthAutomatic))
							iGsmCsdWapIpTable->iProxyUseAutoLogin = ETrue;
						else
							iGsmCsdWapIpTable->iProxyUseAutoLogin = EFalse;
						}
					break;
				case EWappGprsBearer:
					if (!pxAuthSecret)
						{
						if (iParsedFieldArray->At(aCurrentPos)->FieldValue().CompareF(KWappAuthAutomatic))
							iGprsWapIpTable->iProxyUseAutoLogin = ETrue;
						else
							iGprsWapIpTable->iProxyUseAutoLogin = EFalse;
						}
					break;
				default:
					User::Leave(KWappErrUnexpectedValue);
				}


			pxLoginType = ETrue;
			}


		//  Is it a ISP _NAME element?
		else if (fieldName.CompareF(KWappISP)==0)
			{
			// Must be a CSD or Grps bearer
			if ( (currentBearer !=  EWappGsmCsdBearer) &&
				  (currentBearer  != EWappIS136CsdBearer) &&
				  (currentBearer  != EWappGprsBearer)   )
				  User::Leave(KWappErrUnexpectedValue);
			else if (!ispName)
				{
				iWapIpISPTable->iISPDescription.Set(iParsedFieldArray->At(aCurrentPos)->FieldValue());
				ispName =ETrue;
				}			
			} 


// ******************  Following are for GSM/SMS settings  *********************
		else if (fieldName.CompareF(KWappSMSCAddress)==0)
			{
			if (currentBearer  !=  EWappGsmSmsBearer) 
				User::Leave(KWappErrUnexpectedValue);// Only SMS has a SMSC address - error condition
			else if (!smsSMSCAddr)
				{
				iWapSmsTable->iWapServiceAddr.Set(iParsedFieldArray->At(aCurrentPos)->FieldValue());
				smsSMSCAddr = ETrue;
				}
			}

		//Check for PROXY_TYPE characteristic

		// TO DO When/If USSD support is required! Add it here
		// Need to know how to set Addressing Scheme for USSD & CSD/SMS/etc!!!!!
		// Currently only alter value of dummy variable to keep the compiler happy
		else if (fieldName.CompareF(KWappProxyType) ==0)
			{
			// Must be a USSD or GPRS bearer type - otherwise error!
			if (currentBearer == EWappGsmUssdBearer) 
				{
				// NB If USSD support req'd,set the Addressing type here. NB Not yet defined in CommDb!
				}
			else if(currentBearer == EWappGprsBearer)
				{
				if(iParsedFieldArray->At(aCurrentPos)->FieldValue().CompareF(KWappIpv4)!=0)
					User::Leave(KWappErrUnrecognised);
				}
			else
				User::Leave(KWappErrUnrecognised);
			}

		//  Check for USSD service code
		else if (fieldName.CompareF(KWappUSSDCode)==0)
			{
			if (currentBearer  == EWappGsmUssdBearer)
				{  
				// NB - If USSD support req'd, set the Service Centre Address here!
				}
			else 
				User::Leave(KWappErrUnrecognised); // Not a USSD address, so shouldn't be here - error
			}

		// Access point is GPRS only
		else if (fieldName.CompareF(KWappAccessPointName)==0)
			{
			if (currentBearer  == EWappGprsBearer)
				{
				iWapGprsTable->iAccessPointName.Set(iParsedFieldArray->At(aCurrentPos)->FieldValue());
				}
			else
				User::Leave(KWappErrUnrecognised); // Shouldn't have a GPRS address if we aren't GPRS!
			}

		aCurrentPos++;
		}// END of the While loop

	if (currentBearer == EWappGsmCsdBearer)
		{
		if (!iGsmCsdData)
			{
			if (!pppLoginType) 
				{
				// Login type not set so determine value for whether login and authorisation set
				if (iWapIpISPTable->iLoginName.Length()!=0 && iWapIpISPTable->iLoginPass.Length()!=0)
					{
					iWapIpISPTable->iPromptForLoginDetails =EFalse;
					iWapIpISPTable->iIFPromptForAuthentication =EFalse;
					}
				}
			if (! pxLoginType)
				if (iGsmCsdWapIpTable->iProxyAuthName.Length()!=0 && iGsmCsdWapIpTable->iProxyAuthSecret.Length()!=0)
					{
					iGsmCsdWapIpTable->iProxyUseAutoLogin = ETrue;
					}
	
			// Set the iIspTable->iCsdCallSpeed member -  need the calltype and the callspeed values
			if (callType == EAnalogue)
				{
				switch (callSpeed)
					{
					case EAutobauding:
						iWapIpISPTable->iCallSpeed = RMobileCall::ESpeedAutobauding;
						iWapIpISPTable->iCallDataProtocol = RMobileCall::EProtocolV34;
						break;
					case E9600baud:
						iWapIpISPTable->iCallSpeed = RMobileCall::ESpeed9600;
						iWapIpISPTable->iCallDataProtocol = RMobileCall::EProtocolV32;
						break;
					case E14400baud:
						iWapIpISPTable->iCallSpeed = RMobileCall::ESpeed14400;
						iWapIpISPTable->iCallDataProtocol = RMobileCall::EProtocolV34;
						break;
					case E19200baud:
						iWapIpISPTable->iCallSpeed =RMobileCall::ESpeed19200;
						iWapIpISPTable->iCallDataProtocol = RMobileCall::EProtocolV34;
						break;
					case E28800baud:
						iWapIpISPTable->iCallSpeed =RMobileCall::ESpeed28800;
						iWapIpISPTable->iCallDataProtocol = RMobileCall::EProtocolV34;
						break;
					case E38400baud:
					case E43200baud:
					case E57600baud:
						iWapIpISPTable->iCallSpeed = RMobileCall::ESpeedAutobauding;
						iWapIpISPTable->iCallDataProtocol = RMobileCall::EProtocolV34;
						break;
					default:  //SHOULD be one of the above!
						iWapIpISPTable->iCallSpeed = RMobileCall::ESpeedAutobauding;
						iWapIpISPTable->iCallDataProtocol = RMobileCall::EProtocolV34;
						break;
					}
				}
			else if (!iGsmCsdData && callType == EIsdn)
				{
				iWapIpISPTable->iCallDataProtocol = RMobileCall::EProtocolV110;
				switch (callSpeed)
					{
					case EAutobauding:				
						iWapIpISPTable->iCallSpeed = RMobileCall::ESpeedAutobauding;
					break;
					case E9600baud:
						iWapIpISPTable->iCallSpeed = RMobileCall::ESpeed9600;
						break;
					case E14400baud:
						iWapIpISPTable->iCallSpeed =RMobileCall::ESpeed14400;
						break;
					case E19200baud:
						iWapIpISPTable->iCallSpeed =RMobileCall::ESpeed19200;
						break;
					case E28800baud:
						iWapIpISPTable->iCallSpeed =RMobileCall::ESpeed28800;
						break;
					case E38400baud:
						iWapIpISPTable->iCallSpeed =RMobileCall::ESpeed38400;
						break;
					case E43200baud:
						iWapIpISPTable->iCallSpeed =RMobileCall::ESpeed48000;
						break;
					case E57600baud:
						iWapIpISPTable->iCallSpeed = RMobileCall::ESpeed56000;
						break;
					default:  //SHOULD be one of the above!
						iWapIpISPTable->iCallSpeed = RMobileCall::ESpeedAutobauding;
						break;
					}
				}
			else
				User::Leave(KWappErrUnrecognised); //Unrecognised value for calltype - error
			}
		iGsmCsdData = ETrue;
		}

	else if (currentBearer == EWappGsmSmsBearer)
		iGsmSmsData = ETrue;
	else if(currentBearer == EWappGprsBearer)
		{
		if (!pppLoginType)
			{
			if (iWapGprsTable->iLoginName.Length() > 0 && iWapGprsTable->iLoginPass.Length() > 0)
				iWapGprsTable->iIFPromptForAuth =EFalse;
			}
		iGprsData = ETrue;
		}

	}
//  End of function  CMsvBIOWapAccessParser::ExtractBearerDataL()

//
//	ExtractBookmarkDataL() - 
//			Extracts bookmark from parsed data.  Adds default if name field is not
//			set.  Discards the Bookmark if URL field is blank (ie nothing added to Bookmark 
//			List) NB exactly 2 PARMs in a bookmark, NAME & URL, any order.
//	
void CMsvBIOWapAccessParser::ExtractBookmarkDataL(TInt& aIndex)
	{
	TPtrC tempPtr;
	TBool foundName = EFalse;
	TBool foundURL = EFalse;
	TInt  parmsChecked = 0;

	HBufC* bookmarkName = NULL;
	HBufC* bookmarkURL = NULL;

	// Should be set to the next parm in the Bookmark  NOTE: Can't properly check that 
	// an Empty Name or URL Parsed field is not the start of another characteristic
	while (aIndex < iParsedFieldArray->Count() && !IsCharacteristicName(aIndex))  
		{
		// check token is correct - copy NAME 
		if (parmsChecked <2 && iParsedFieldArray->At(aIndex)->FieldName().CompareF(KWappName) ==0) 
			{
			if (foundName ==EFalse)
				{
				tempPtr.Set(iParsedFieldArray->At(aIndex)->FieldValue());
				if (tempPtr.Length()==0)
					tempPtr.Set(KWappDefaultBMName);
				bookmarkName = tempPtr.AllocL();
				CleanupStack::PushL(bookmarkName);
				foundName =ETrue;
				}
			parmsChecked++;
			}
		else if (parmsChecked <2 && iParsedFieldArray->At(aIndex)->FieldName().CompareF(KWappURL)==0)
			{
			if (foundURL == EFalse)
				{
				tempPtr.Set(iParsedFieldArray->At(aIndex)->FieldValue());
				if (tempPtr.Length() > 0)
					{
					bookmarkURL = tempPtr.AllocL();
					CleanupStack::PushL(bookmarkURL);
					foundURL =ETrue;
					}
				}
			parmsChecked++;
			}
		// Advance to next field if not at the end of the array
		if (aIndex <iParsedFieldArray->Count())
			aIndex++;		 
		}
		// If we've only looked at 2 Parms and have both a Name AND URL 
		// it's a well-formed bookmark so add to it to our List
		if (foundName && foundURL && parmsChecked ==2) 
			{
			CWappBookmark* newBookmark =  CWappBookmark::NewLC();
			newBookmark->SetNameL(*bookmarkName);
			newBookmark->SetUrlL(*bookmarkURL);
			iBookmarkList->AppendL(newBookmark); 				
			CleanupStack::Pop(); // release newBookmark	
			}
		if(bookmarkName) CleanupStack::PopAndDestroy();
		if(bookmarkURL) CleanupStack::PopAndDestroy();
	}

//
//	ValidateCharacteristicsL() - Validates the data extracted by PreProcessL. 
//
//	Following are legitimate messages
//		 -  entirely bookmarks
//		 -  1+ bearers with any no of other characteristics
//		 -  Non-Null URL and Non Null name of existing settings records
//
//	Any other combinations are invalid i.e.
//				
//				Bookmarks with ID/URL/NAME & NO Bearer
//									
void CMsvBIOWapAccessParser::ValidateCharacteristicsL()
	{
	
	// Bookmarks message - bookmarks & no bearers, should be no URL, ID or Settings Name
	if ((iBookmarkList->Count() >0) && 
		!(iGsmCsdData || iGsmSmsData || iGprsData) && 
		(iURLData || iNameData || iIdData))
			User::Leave(KWappErrBadMessage);  
	
	// start page update message - URL+NAME but no Bearers, no bookmarks
	if (iURLData && iNameData && 
		!(iGsmCsdData || iGsmSmsData || iGprsData) && 
		(iBookmarkList->Count()==0) && 
		((iUrl.Length()==0) || (iWAPSettingsName.Length()==0)))
			User::Leave(KWappErrBadMessage);

	if ((iGsmCsdData || iGsmSmsData || iGprsData) && 
		(iWAPSettingsName.Length()==0))     // Need a name for CommDb - default
			User::Leave(KWappErrBadMessage); //  should have been given if nothing in message 
	}

//
//	IsCharacteristicName	-	Returns ETrue if the Name at the index is a genuine 
//								Characteristic Type and the Value is NULL
//
TBool CMsvBIOWapAccessParser::IsCharacteristicName(TInt anIndex)
	{
	TPtrC name = iParsedFieldArray->At(anIndex)->FieldName();
	if (  
		(name.CompareF(KWappCharAddress) ==0) ||  // Address
		(name.CompareF(KWappCharURL) ==0) ||	  // URL 
		(name.CompareF(KWappCharMMSURL) ==0) ||	  // MMS URL 
		(name.CompareF(KWappCharName) ==0) ||  // NAME
		(name.CompareF(KWappCharBookmark) ==0) || // Bookmark
		(name.CompareF(KWappCharID) ==0)  ) // ID
			return ETrue;	// Is a characteristic name
	else
		return EFalse;		// Not a characteristic name
	}

//		
//	SetModemInitStringL() -  Sets the modem initialisation string
//
void CMsvBIOWapAccessParser::SetModemInitStringL() // Sets the modem initialisation string
	{
	delete iModemInitString;
	iModemInitString =NULL;
	iModemInitString=KDummyModemInitStr().AllocL();
	}


void CMsvBIOWapAccessParser::DeleteCommDatTableEntryL(TMDBElementId aElementName, const TDesC& aCsdNameStr)
	{	
	switch	(aElementName)
		{
		case KCDTIdWAPIPBearerRecord:
			{
			CCDWAPIPBearerRecord *wapIPRecord = static_cast<CCDWAPIPBearerRecord*>(CCDRecordBase::RecordFactoryL(KCDTIdWAPIPBearerRecord));
			CleanupStack::PushL(wapIPRecord);
			wapIPRecord->iWAPAccessPointId = iWapAccessPtID;
			
			if(wapIPRecord->FindL(*iDBSession))
				{
				TUint32 accessId = wapIPRecord->iWAPIAP;
				if ( !((iGsmCsdData && iGsmCsdWapIpTable->iIapId == accessId) ||
			           			(iGprsData && iGprsWapIpTable->iIapId == accessId)))					
					{
					wapIPRecord->DeleteL(*iDBSession);			
					}
				}
			CleanupStack::PopAndDestroy(wapIPRecord);					
			}			
			break;
		case KCDTIdWAPSMSBearerRecord:
			{
			CCDWAPSMSBearerRecord *smsRecord = static_cast<CCDWAPSMSBearerRecord*>(CCDRecordBase::RecordFactoryL(KCDTIdWAPSMSBearerRecord));
			CleanupStack::PushL(smsRecord);
			smsRecord->iWAPAccessPointId = iWapAccessPtID;
			if(smsRecord->FindL(*iDBSession))
				{
				smsRecord->DeleteL(*iDBSession);			
				}
			CleanupStack::PopAndDestroy(smsRecord);	
			}			
			break;
		case KCDTIdIAPRecord:
			{
			CCDIAPRecord *iapRecord = static_cast<CCDIAPRecord*>(CCDRecordBase::RecordFactoryL(KCDTIdIAPRecord));
			CleanupStack::PushL(iapRecord);
			iapRecord->iRecordName.SetMaxLengthL(aCsdNameStr.Length());
			iapRecord->iRecordName = aCsdNameStr;				
			if(iapRecord->FindL(*iDBSession))
				{
				iapRecord->DeleteL(*iDBSession);				
				}
			CleanupStack::PopAndDestroy(iapRecord);
			}			
			break;
		case KCDTIdDialOutISPRecord:
			{
			CCDDialOutISPRecord *dialOutISPRecord = static_cast<CCDDialOutISPRecord*>(CCDRecordBase::RecordFactoryL(KCDTIdDialOutISPRecord));
			CleanupStack::PushL(dialOutISPRecord);
			dialOutISPRecord->iRecordName.SetMaxLengthL(aCsdNameStr.Length());
			dialOutISPRecord->iRecordName = aCsdNameStr;
			if(dialOutISPRecord->FindL(*iDBSession))
				{
				dialOutISPRecord->DeleteL(*iDBSession);				
				}
			CleanupStack::PopAndDestroy(dialOutISPRecord);						
			}
			break;		
		case KCDTIdOutgoingGprsRecord:
			{
			CCDOutgoingGprsRecord *gprsRecord = static_cast<CCDOutgoingGprsRecord*>(CCDRecordBase::RecordFactoryL(KCDTIdOutgoingGprsRecord));
			CleanupStack::PushL(gprsRecord);
			gprsRecord->iRecordName.SetMaxLengthL(aCsdNameStr.Length());
			gprsRecord->iRecordName = aCsdNameStr;
			if(gprsRecord->FindL(*iDBSession))
				{
				gprsRecord->DeleteL(*iDBSession);				
				}
			CleanupStack::PopAndDestroy(gprsRecord);		
			}
			break;			
		}
	}	

void CMsvBIOWapAccessParser::WriteDataToDBL()
	{
	// Should have a table for at least one bearer!!
	__ASSERT_DEBUG (iWapSmsTable != NULL || iWapGprsTable != NULL || (iGsmCsdWapIpTable != NULL && iWapIpISPTable !=NULL), 
									  Panic(EWappNullPointer));

	if (iWAPSettingsName.Length()==0)
		{
		iWAPSettingsName.Set(KWapDefaultSettingsName);
		}	

	TBuf<KMaxNameBufferLength> csdNameStr;
	csdNameStr.Copy(iWAPSettingsName);
	csdNameStr.Append(KGsmCsdSuffix);
	if (iGsmCsdData)
		{
		iCurrentBearerType = EWappGsmCsdBearer;  // Set the bearer type indicator for use in next functions

		WriteToCommDatDialOutISPTableL(csdNameStr);
		SetWapIspEntryIdL(csdNameStr);	// Sets iWapIspID		
		
		WriteToCommsDatIAPTableL(csdNameStr);			
		SetWapIapEntryIdL(csdNameStr, *iGsmCsdWapIpTable);			// Sets iWapIapID
		}
	else
		{
		DeleteCommDatTableEntryL(KCDTIdDialOutISPRecord,csdNameStr);
		DeleteCommDatTableEntryL(KCDTIdIAPRecord,csdNameStr);
		}

	TBuf<KMaxNameBufferLength> gprsNameStr;
	gprsNameStr.Copy(iWAPSettingsName);
	gprsNameStr.Append(KGprsSuffix);
	if(iGprsData)
		{
		iCurrentBearerType = EWappGprsBearer;		
		WriteToCommDatGprsTableL(gprsNameStr);		
		SetWapIspEntryIdL(gprsNameStr);	// Sets iWapIspID
		WriteToCommsDatIAPTableL(gprsNameStr);				
				
		SetWapIapEntryIdL(gprsNameStr, *iGprsWapIpTable);			// Sets iWapIapID
		}
	else
		{
		DeleteCommDatTableEntryL(KCDTIdOutgoingGprsRecord,gprsNameStr);
		DeleteCommDatTableEntryL(KCDTIdIAPRecord,gprsNameStr);
		}
		
	WriteToCommDatWapAccessPointTableL(iWAPSettingsName);		

	SetWapAPEntryIdL();


	if (iGsmCsdData)
		{
		// Set the flag so that correct mapping object is used to set the data in the WAP IP Bearer table
		iCurrentBearerType = EWappGsmCsdBearer;
		WriteToCommDatWapIpBearerTableL();		
		}
	else 		
		{
		DeleteCommDatTableEntryL(KCDTIdWAPIPBearerRecord,csdNameStr);
		}
		

	if ((iGprsData))
		{
		// Set the flag so that correct mapping object is used to set the data in the WAP IP Bearer table
		iCurrentBearerType = EWappGprsBearer;
		WriteToCommDatWapIpBearerTableL();
		}
	else
		{
		DeleteCommDatTableEntryL(KCDTIdWAPIPBearerRecord,gprsNameStr);
		}
		
		
	if (iGsmSmsData)
		{
		WriteToCommDatWapSmsBearerTableL();	
		}		
	else		
		{
		DeleteCommDatTableEntryL(KCDTIdWAPSMSBearerRecord,iWAPSettingsName);
		}
		
		


//	if ((iGsmCsdData) && iModemInitString)    // Only if CSD and has string set (ie ISDN)
//		CreateTableL(TPtrC(MODEM), iWAPSettingsName);  //TODO Not yet implemented 
	}

// End of function  CMsvBIOWapAccessParser::WriteDataToDBL()


void CMsvBIOWapAccessParser::WriteToCommsDatIAPTableL(const TDesC& aRecName)
	{	
	CCDIAPRecord *iapRecord = static_cast<CCDIAPRecord*>(CCDRecordBase::RecordFactoryL(KCDTIdIAPRecord));
	CleanupStack::PushL(iapRecord);
	iapRecord->iRecordName.SetMaxLengthL(aRecName.Length());
	iapRecord->iRecordName = aRecName;
	
	TBool newRecord = EFalse;
	if(!iapRecord->FindL(*iDBSession))
		{
		iapRecord->SetRecordId(KCDNewRecordRequest);
		newRecord = ETrue;
		}		
			
	switch (iCurrentBearerType)
		{
		case EWappGsmCsdBearer:
			{
			iapRecord->iService = iWapIpISPTable->iISPCommDbRecordId;
			iapRecord->iServiceType.SetMaxLengthL(((TPtrC)KCDTypeNameDialOutISP).Length());
			iapRecord->iServiceType = (TPtrC)KCDTypeNameDialOutISP;			

			//there are 7 fields in IAP table which cannot have null values, and so when creating an IAP record we were only filling in one of the fields which meant we were submitting a record with lots of null values which can't be handled.
			iapRecord->iBearer = (TUint32)2;
			
			iapRecord->iBearerType.SetMaxLengthL(((TPtrC)KCDTypeNameModemBearer).Length());
			iapRecord->iBearerType = (TPtrC)KCDTypeNameModemBearer;
			iapRecord->iNetwork = (TUint32)1;
			iapRecord->iNetworkWeighting = (TUint32)0;
			iapRecord->iLocation = (TUint32)4;							
			}
			break;
		case EWappGprsBearer:
			{
			iapRecord->iService = iWapGprsTable->iGprsCommDbRecordId;
			iapRecord->iServiceType.SetMaxLengthL(((TPtrC)KCDTypeNameDialOutISP).Length());
			iapRecord->iServiceType = (TPtrC)KCDTypeNameOutgoingWCDMA;

			//there are 7 fields in IAP table which cannot have null values, and so when creating an IAP record we were only filling in one of the fields which meant we were submitting a record with lots of null values which can't be handled.
			iapRecord->iBearer = (TUint32)2;
			iapRecord->iBearerType.SetMaxLengthL(((TPtrC)KCDTypeNameModemBearer).Length());
			iapRecord->iBearerType = (TPtrC)KCDTypeNameModemBearer;
			iapRecord->iNetwork = (TUint32)1;
			iapRecord->iNetworkWeighting = (TUint32)0;	
			iapRecord->iLocation = (TUint32)4;	
			}									
			break;
		default:
			break;
		}	
	
	if(newRecord)
		{					
		iapRecord->StoreL(*iDBSession);
		}		
	else
		{
		iapRecord->ModifyL(*iDBSession);	
		}	
	
	CleanupStack::PopAndDestroy(iapRecord);
	}
	
void CMsvBIOWapAccessParser::WriteToCommDatWapSmsBearerTableL()
	{	
	CCDWAPSMSBearerRecord *wapSMSBearerRecord = static_cast<CCDWAPSMSBearerRecord*>(CCDRecordBase::RecordFactoryL(KCDTIdWAPSMSBearerRecord));
	CleanupStack::PushL(wapSMSBearerRecord);
	//Search based on WAPAccessPointId
	wapSMSBearerRecord->iWAPAccessPointId = iWapAccessPtID;	
	
	TBool newRecord = EFalse;	
	if(!wapSMSBearerRecord->FindL(*iDBSession))	
		{
		wapSMSBearerRecord->SetRecordId(KCDNewRecordRequest);			
		newRecord = ETrue;
		}			
	
	wapSMSBearerRecord->iWAPGatewayAddress.SetMaxLengthL(iWapSmsTable->iGatewayAddr.Length());
	wapSMSBearerRecord->iWAPGatewayAddress = iWapSmsTable->iGatewayAddr;
	
	wapSMSBearerRecord->iWAPServiceCentreAddress.SetMaxLengthL(iWapSmsTable->iWapServiceAddr.Length());
	wapSMSBearerRecord->iWAPServiceCentreAddress = iWapSmsTable->iWapServiceAddr;
	
	wapSMSBearerRecord->iWAPWSPOption = GetWSPType(iWapSmsTable->iPort);			
	wapSMSBearerRecord->iWAPSecurity = UseSecurity(iWapSmsTable->iPort);	
		
	if(newRecord)
		{					
		wapSMSBearerRecord->StoreL(*iDBSession);
		}		
	else
		{
		wapSMSBearerRecord->ModifyL(*iDBSession);	
		}		
	
	CleanupStack::PopAndDestroy(wapSMSBearerRecord);
	}

void CMsvBIOWapAccessParser::WriteToCommDatWapIpBearerTableL()
	{	
	CCDWAPIPBearerRecord *wapIPBearerRecord = static_cast<CCDWAPIPBearerRecord*>(CCDRecordBase::RecordFactoryL(KCDTIdWAPIPBearerRecord));
	CleanupStack::PushL(wapIPBearerRecord);
	//Search based on WAPAccessPointId
	wapIPBearerRecord->iWAPAccessPointId = iWapAccessPtID;	
	
	TBool newRecord = EFalse;	
	if(!wapIPBearerRecord->FindL(*iDBSession))
		{
		wapIPBearerRecord->SetRecordId(KCDNewRecordRequest);					
		newRecord = ETrue;
		}		
	
	CWapIpBearerData* wapIpTableData = NULL;
	switch (iCurrentBearerType)
		{
		case EWappGsmCsdBearer:
			{
			wapIpTableData = iGsmCsdWapIpTable;	
			}			
			break;
		case EWappGprsBearer:
			{
			wapIpTableData = iGprsWapIpTable;	
			}			
			break;
		default:
			User::Leave(KWappErrUnexpectedValue);
		}
	__ASSERT_DEBUG(wapIpTableData, Panic(KWappErrNullValue));

	wapIPBearerRecord->iWAPAccessPointId = iWapAccessPtID;
 	wapIPBearerRecord->iWAPIAP = wapIpTableData->iIapId;			// Get from SetWapEntryIdsL

	wapIPBearerRecord->iWAPGatewayAddress.SetMaxLengthL((wapIpTableData->iGatewayAddr->Addr()).Length());
	wapIPBearerRecord->iWAPGatewayAddress = wapIpTableData->iGatewayAddr->Addr();
	
	wapIPBearerRecord->iWAPWSPOption =  GetWSPType(wapIpTableData->iPort);
	wapIPBearerRecord->iWAPSecurity =  UseSecurity(wapIpTableData->iPort);
	
	TLex lex = wapIpTableData->iPort;
	TInt port = 0;	
	User::LeaveIfError(lex.Val(port));
	wapIPBearerRecord->iWAPProxyPort =  port;
	
	wapIPBearerRecord->iWAPProxyLoginName.SetMaxLengthL(wapIpTableData->iProxyAuthName.Length());
	wapIPBearerRecord->iWAPProxyLoginName = wapIpTableData->iProxyAuthName;
	
	wapIPBearerRecord->iWAPProxyLoginPass.SetMaxLengthL(wapIpTableData->iProxyAuthSecret.Length());
	wapIPBearerRecord->iWAPProxyLoginPass = wapIpTableData->iProxyAuthSecret;
	
	if(newRecord)
		{					
		wapIPBearerRecord->StoreL(*iDBSession);
		}		
	else
		{
		wapIPBearerRecord->ModifyL(*iDBSession);	
		}	
	
	CleanupStack::PopAndDestroy(wapIPBearerRecord);	
	}

void CMsvBIOWapAccessParser::WriteToCommDatWapAccessPointTableL(const TDesC& aRecName)
	{	
	CCDWAPAccessPointRecord *wapAPRecord = static_cast<CCDWAPAccessPointRecord*>(CCDRecordBase::RecordFactoryL(KCDTIdWAPAccessPointRecord));
	CleanupStack::PushL(wapAPRecord);
	wapAPRecord->iRecordName.SetMaxLengthL(aRecName.Length());	
	wapAPRecord->iRecordName = aRecName;
	TBool newRecord = EFalse;
	if(!wapAPRecord->FindL(*iDBSession))
		{
		wapAPRecord->SetRecordId(KCDNewRecordRequest);
		newRecord = ETrue;	
		}
				
	if (iUrl.Length() != 0)
		{
		wapAPRecord->iWAPStartPage.SetMaxLengthL(iUrl.Length());
		wapAPRecord->iWAPStartPage = iUrl;	
		}		

	if ((iGsmCsdData) || (iGprsData))
		wapAPRecord->iWAPCurrentBearer = TPtrC(WAP_IP_BEARER);
	else if (iGsmSmsData)
		wapAPRecord->iWAPCurrentBearer = TPtrC(WAP_SMS_BEARER);
	else
		User::Leave(KWappErrUnexpectedValue);	
	
	if(newRecord)
		{					
		wapAPRecord->StoreL(*iDBSession);
		}		
	else
		{
		wapAPRecord->ModifyL(*iDBSession);	
		}
	
	CleanupStack::PopAndDestroy(wapAPRecord);
	}

void CMsvBIOWapAccessParser::WriteToCommDatGprsTableL(const TDesC& aRecName)
	{	
	CCDOutgoingGprsRecord *gprsRecord = static_cast<CCDOutgoingGprsRecord*>(CCDRecordBase::RecordFactoryL(KCDTIdOutgoingGprsRecord));
	CleanupStack::PushL(gprsRecord);
	gprsRecord->iRecordName.SetMaxLengthL(aRecName.Length());
	gprsRecord->iRecordName = aRecName;
	
	TBool newRecord = EFalse;
	if(!gprsRecord->FindL(*iDBSession))	
		{
		gprsRecord->SetRecordId(KCDNewRecordRequest);
		newRecord = ETrue;	
		}
				
	gprsRecord->iGPRSAPN.SetMaxLengthL(iWapGprsTable->iAccessPointName.Length());
	gprsRecord->iGPRSAPN = iWapGprsTable->iAccessPointName;
	
	gprsRecord->iGPRSPDPType = iWapGprsTable->iPDPType;
	
	gprsRecord->iGPRSPDPAddress.SetMaxLengthL(iWapGprsTable->iPDPAddress->Des().Length());
	gprsRecord->iGPRSPDPAddress = iWapGprsTable->iPDPAddress->Des();

	gprsRecord->iGPRSIfPromptForAuth = iWapGprsTable->iIFPromptForAuth;
	
	gprsRecord->iGPRSIfAuthName.SetMaxLengthL(iWapGprsTable->iLoginName.Length());
	gprsRecord->iGPRSIfAuthName = iWapGprsTable->iLoginName;
	
	gprsRecord->iGPRSIfAuthPass.SetMaxLengthL(iWapGprsTable->iLoginPass.Length());
	gprsRecord->iGPRSIfAuthPass = iWapGprsTable->iLoginPass;
	
	gprsRecord->iGPRSIPGateway.SetMaxLengthL(iWapGprsTable->iGatewayAddr->Addr().Length());
	gprsRecord->iGPRSIPGateway = iWapGprsTable->iGatewayAddr->Addr();
	
	gprsRecord->iGPRSIPAddrFromServer = iWapGprsTable->iIpAddrFromServer;
	
	gprsRecord->iGPRSIPDNSAddrFromServer = iWapGprsTable->iIPDnsAddrFromServer;
	gprsRecord->iGPRSDisablePlainTextAuth = iWapGprsTable->iPlainTextAuthDisabled;	
	
	if(newRecord)
		{					
		gprsRecord->StoreL(*iDBSession);
		}		
	else
		{
		gprsRecord->ModifyL(*iDBSession);	
		}
	
	CleanupStack::PopAndDestroy(gprsRecord);	
	}

void CMsvBIOWapAccessParser::WriteToCommDatDialOutISPTableL(const TDesC& aRecName)
	{	
	CCDDialOutISPRecord *dialoutRecord = static_cast<CCDDialOutISPRecord*>(CCDRecordBase::RecordFactoryL(KCDTIdDialOutISPRecord));
	CleanupStack::PushL(dialoutRecord);
	dialoutRecord->iRecordName.SetMaxLengthL(aRecName.Length());	
	dialoutRecord->iRecordName = aRecName;
	
	TBool newRecord = EFalse;
	if(!dialoutRecord->FindL(*iDBSession))
		{										
		dialoutRecord->SetRecordId(KCDNewRecordRequest);
		newRecord = ETrue;
		
		iWapIpISPTable->iISPCommDbRecordId = dialoutRecord->iRecordTag.RecordId();
		}
		
	//It's time to write values	
	(dialoutRecord->iDescription).SetMaxLengthL(iWapIpISPTable->iISPDescription.Length());
	dialoutRecord->iDescription = iWapIpISPTable->iISPDescription;
	
	dialoutRecord->iType = iWapIpISPTable->iISPType; 
	
	dialoutRecord->iDefaultTelNum.SetMaxLengthL(iWapIpISPTable->iDefaultPhoneNum.Length());
	dialoutRecord->iDefaultTelNum = iWapIpISPTable->iDefaultPhoneNum;
	
	dialoutRecord->iDialResolution = iWapIpISPTable->iDialResolution; //Mandatory, ETrue  default

	dialoutRecord->iUseLoginScript = iWapIpISPTable->iUseScript;	//Mandatory, EFalse by default
	
	if (iWapIpISPTable->iLoginScript.Length() != 0)
		{
		(dialoutRecord->iLoginScript).SetMaxLengthL(iWapIpISPTable->iLoginScript.Length()); 	
		dialoutRecord->iLoginScript = iWapIpISPTable->iLoginScript; 
		}	
	dialoutRecord->iPromptForLogin = iWapIpISPTable->iPromptForLoginDetails; // Mandatory, ETrue by default
	dialoutRecord->iLoginName.SetMaxLengthL((iWapIpISPTable->iLoginName).Length());
	dialoutRecord->iLoginName = iWapIpISPTable->iLoginName;
	
	(dialoutRecord->iLoginPass).SetMaxLengthL((iWapIpISPTable->iLoginPass).Length());
	dialoutRecord->iLoginPass = iWapIpISPTable->iLoginPass;
	
	dialoutRecord->iDisplayPct = iWapIpISPTable->iDisplayPCT;

	(dialoutRecord->iIfParams).SetMaxLengthL((iWapIpISPTable->iIFParams).Length());
	dialoutRecord->iIfParams = iWapIpISPTable->iIFParams;
	
	(dialoutRecord->iIfNetworks).SetMaxLengthL((iWapIpISPTable->iIFNetworks->Des()).Length());
	dialoutRecord->iIfNetworks = iWapIpISPTable->iIFNetworks->Des(); // Set to "IP"
	
	dialoutRecord->iIfPromptForAuth = iWapIpISPTable->iPromptForLoginDetails; //Mandatory, default is ETrue
	
	(dialoutRecord->iIfAuthName).SetMaxLengthL((iWapIpISPTable->iLoginName).Length());
	dialoutRecord->iIfAuthName = iWapIpISPTable->iLoginName;
	
	(dialoutRecord->iIfAuthPass).SetMaxLengthL((iWapIpISPTable->iLoginPass).Length());
	dialoutRecord->iIfAuthPass = iWapIpISPTable->iLoginPass;
	
	dialoutRecord->iIfAuthRetries = iWapIpISPTable->iNumAuthAttempts;

	dialoutRecord->iIfCallbackEnabled = iWapIpISPTable->iCallBackEnabled;
	if (iWapIpISPTable->iCallBackEnabled)
		{
		dialoutRecord->iIfCallbackType = iWapIpISPTable->iCallBackType;		
		TPtr8 tempPtr(0,0); 		
		tempPtr.Copy(iWapIpISPTable->iCallBackInfo);
		
		dialoutRecord->iIfCallbackInfo.SetMaxLengthL(tempPtr.Length());
		dialoutRecord->iIfCallbackInfo = tempPtr;
		dialoutRecord->iCallbackTimeout = 10000; 
		}

	dialoutRecord->iIpAddrFromServer = iWapIpISPTable->iAddrFromServer; //Mandatory set to True
	
	(dialoutRecord->iIpAddr).SetMaxLengthL((iWapIpISPTable->iInOutAddress->Addr()).Length());
	dialoutRecord->iIpNetMask = iWapIpISPTable->iNetmask->Addr();
	
	(dialoutRecord->iIpGateway).SetMaxLengthL((iWapIpISPTable->iGateway->Addr()).Length());
	dialoutRecord->iIpGateway = iWapIpISPTable->iGateway->Addr();
	
	dialoutRecord->iIpDnsAddrFromServer = iWapIpISPTable->iDNSFromServer;
	// Following will write NULL strings to database, clearing any previous settings.
	
	(dialoutRecord->iIpNameServer1).SetMaxLengthL((iWapIpISPTable->iDNS1->Addr()).Length());
	dialoutRecord->iIpNameServer1 = iWapIpISPTable->iDNS1->Addr();
	
	(dialoutRecord->iIpNameServer2).SetMaxLengthL((iWapIpISPTable->iDNS2->Addr()).Length());
	dialoutRecord->iIpNameServer2 = iWapIpISPTable->iDNS2->Addr();
	
	dialoutRecord->iEnableIpHeaderComp = iWapIpISPTable->iHeaderCompressionEnabled;
	// Don't bother with LCP extensions -  leave as null field
	dialoutRecord->iDisablePlainTextAuth = iWapIpISPTable->iPlainTextAuthDisabled;
	dialoutRecord->iEnableSwComp = iWapIpISPTable->iSwCompressionEnabled;	
	dialoutRecord->iBearerSpeed = iWapIpISPTable->iCallSpeed;
	dialoutRecord->iBearerProtocol = iWapIpISPTable->iCallDataProtocol;

	if(newRecord)
		{					
		dialoutRecord->StoreL(*iDBSession);
		}		
	else
		{
		dialoutRecord->ModifyL(*iDBSession);	
		}
	
	CleanupStack::PopAndDestroy(dialoutRecord);
	}


//
//		SetWapIspEntryIdL	- The iCurrentBearerType variable MUST be correctly set before this 
//							  function is called. Searches the appropriate Service table - DIAL_OUT_ISP
//							 or  OUTGOING_WCDMA  - for records named as aEntryName (which should be unique!)
//						   When it finds the record, it uses the record ID to set the class member iWapIspID.
//
void CMsvBIOWapAccessParser::SetWapIspEntryIdL(const TDesC& aEntryName)
	{
	TUint32 recId = 0;	
	switch (iCurrentBearerType)
		{
		case EWappGsmCsdBearer:
			{
			CCDDialOutISPRecord *dialOutRecord = static_cast<CCDDialOutISPRecord*>(CCDRecordBase::RecordFactoryL(KCDTIdDialOutISPRecord));
			CleanupStack::PushL(dialOutRecord);
			dialOutRecord->iRecordName.SetMaxLengthL(aEntryName.Length());
			dialOutRecord->iRecordName = aEntryName;			
						
			if (dialOutRecord->FindL(*iDBSession))			
				{				
				recId = dialOutRecord->iRecordTag.RecordId();									
				}
			else
				{				
				User::Leave(KErrNotFound);
				}
			CleanupStack::PopAndDestroy(dialOutRecord);			
			}			
			break;
		case EWappGprsBearer:
			{
			CCDOutgoingGprsRecord *gprsRecord = static_cast<CCDOutgoingGprsRecord*>(CCDRecordBase::RecordFactoryL(KCDTIdOutgoingGprsRecord));
			CleanupStack::PushL(gprsRecord);
			gprsRecord->iRecordName.SetMaxLengthL(aEntryName.Length());
			gprsRecord->iRecordName = aEntryName;
						
			if (gprsRecord->FindL(*iDBSession))
				{
				recId = gprsRecord->iRecordTag.RecordId();					
				}
			else
				{				
				User::Leave(KErrNotFound);
				}
			CleanupStack::PopAndDestroy(gprsRecord);	
			}						
			break;
		default:
			User::Leave(KWappErrUnexpectedValue);
		}
		
	switch (iCurrentBearerType)
		{
		case EWappGsmCsdBearer:
			iWapIpISPTable->iISPCommDbRecordId = recId;
			break;
		case EWappGprsBearer:
			iWapGprsTable->iGprsCommDbRecordId = recId;
			break;
		default:
			User::Leave(KWappErrUnexpectedValue);
			break;
		}	
	}


//
//		SetWapIapEntryIdL	- Searches the IAP database for records of the same name 
//						  as aEntryName (which should be unique!). When it finds the 
//						  record, it uses the record ID to set the class member iWapIapID.
//
void CMsvBIOWapAccessParser::SetWapIapEntryIdL(const TDesC& aEntryName, CWapIpBearerData& aWapIpTable)
	{
	CCDIAPRecord *iAPRecord = static_cast<CCDIAPRecord*>(CCDRecordBase::RecordFactoryL(KCDTIdIAPRecord));
	CleanupStack::PushL(iAPRecord);
	iAPRecord->iRecordName.SetMaxLengthL(aEntryName.Length());
	iAPRecord->iRecordName = aEntryName;
				
	if (iAPRecord->FindL(*iDBSession))
		{
		aWapIpTable.iIapId = iAPRecord->iRecordTag.RecordId();					
		}
	else
		{
		User::Leave(KErrNotFound);		
		}
	CleanupStack::PopAndDestroy(iAPRecord);	
	}

//
//		SetWapIspEntryIdL	- Searches the WAP_ACCESS_POINT table for records of the same name 
//						  as iWapSettingsName (which should be unique!). When it finds the 
//						  record, it uses the record ID to set the class member iWapIspID.
//
void CMsvBIOWapAccessParser::SetWapAPEntryIdL()
	{
	CCDWAPAccessPointRecord *wapAccessRecord = static_cast<CCDWAPAccessPointRecord*>(CCDRecordBase::RecordFactoryL(KCDTIdWAPAccessPointRecord));
	CleanupStack::PushL(wapAccessRecord);
	wapAccessRecord->iRecordName.SetMaxLengthL(iWAPSettingsName.Length());
	wapAccessRecord->iRecordName = iWAPSettingsName;	
	
	if (wapAccessRecord->FindL(*iDBSession))
		{    
		iWapAccessPtID = wapAccessRecord->iRecordTag.RecordId();	//  set the WAP AP table ID					
		}
	else
		{
		User::Leave(KErrNotFound);		
		}
	CleanupStack::PopAndDestroy(wapAccessRecord);	
	}
	

void CMsvBIOWapAccessParser::WriteMMSURLL()
	{

	// check that the version of the MMS settings proxy dll is suitable
	RImplInfoPtrArray list;
   	REComSession::ListImplementationsL(KMMSSettingsProxyInterfaceUid, list);
   	TInt count = list.Count();
   	TBool found = EFalse;
   	// may be more than one implementation, look for the 'right' one
   	while (!found && count-- > 0)
    	{
      	if (list[count]->ImplementationUid() == KMMSSettingsProxyBaseUid &&
        	list[count]->Version() == KMMSSettingsProxyVersionSupported)
         	{
         	// Found supported version - stop looking.
         	found = ETrue;
         	}
      	}
   	list.ResetAndDestroy();
   	// if there are no implementations or the version is unsupported, then leave
	if (!found)
		User::Leave(KErrNotFound);

	CMMSSettingsProxyBase* settings = CMMSSettingsProxyBase::NewL(KMMSSettingsProxyBaseUid);
	CleanupStack::PushL(settings);

	TSglQue<TMMSSettingsPair> settingList(_FOFF(TMMSSettingsPair, iLink));

	// Different MMS implementations require different things.  Pass all the things they might
	// want and allow the implementation to use those it chooses.

	// The MMS Url
	TMMSSettingsPair urlPair;
	urlPair.iName.Set(KMMSUrl);
	urlPair.iValue = &iMMSUrl;
	settingList.AddFirst(urlPair);


	TBuf<KMaxNameBufferLength> nameStr;
	nameStr.Copy(iWAPSettingsName);
	if (iGsmCsdData)
		{
		nameStr.Append(KGsmCsdSuffix);
		}

	if(iGprsData)
		{
		nameStr.Append(KGprsSuffix);
		}	


	TUint32 id=0;

	switch (iCurrentBearerType)
		{
		case EWappGsmCsdBearer:
			{
			CCDDialOutISPRecord *dialOutISPRecord = static_cast<CCDDialOutISPRecord*>(CCDRecordBase::RecordFactoryL(KCDTIdDialOutISPRecord));
			CleanupStack::PushL(dialOutISPRecord);
			dialOutISPRecord->iRecordName.SetMaxLengthL(nameStr.Length());
			dialOutISPRecord->iRecordName = nameStr;		
			if(!dialOutISPRecord->FindL(*iDBSession))
				{
				User::Leave(KErrNotFound);	
				}				
			id = dialOutISPRecord->iRecordTag.RecordId();
			CleanupStack::PopAndDestroy(dialOutISPRecord);
			}
			break;
		case EWappGprsBearer:
			{
			CCDOutgoingGprsRecord *gprsRecord = static_cast<CCDOutgoingGprsRecord*>(CCDRecordBase::RecordFactoryL(KCDTIdOutgoingGprsRecord));
			CleanupStack::PushL(gprsRecord);
			gprsRecord->iRecordName.SetMaxLengthL(nameStr.Length());
			gprsRecord->iRecordName = nameStr;	
				
			if(!gprsRecord->FindL(*iDBSession))
				{
				User::Leave(KErrNotFound);	
				}			
			id = gprsRecord->iRecordTag.RecordId();
			CleanupStack::PopAndDestroy(gprsRecord);
			}
			break;
		default:
			User::Leave(KWappErrUnexpectedValue);
		}	

	TMMSSettingsPair commDbRecordId;
	commDbRecordId.iName.Set(KCommDbRecordId);
	commDbRecordId.iValue = &id;
	settingList.AddFirst(commDbRecordId);

	TMMSSettingsPair msvServerEntryPair;
	// 24/08/04
	// In messaging API v2, iEntry is no longer a CMsvServerEntry, but rather a CMsvEntry.
	// For this reason, the following code has been changed.
	// This breaks data compatability.
	// CMsvEntry
	msvServerEntryPair.iName.Set(KMsvEntry);
	msvServerEntryPair.iValue = &iEntry;
	settingList.AddFirst(msvServerEntryPair);

	settings->SetMMSValuesL(settingList);
	CleanupStack::PopAndDestroy(settings);
	}


//	GetWSPTypeL - Helper function used by IP and SMS functions above. Returns appropriate enum
//
TCommsDbWapWspOption CMsvBIOWapAccessParser::GetWSPType(TDesC& aPortValue)
	{
	TCommsDbWapWspOption wspType;

	// Is it Connection Orientated link ?
	if ( (aPortValue.CompareF(KWappPort9201)==0)||
			(aPortValue.CompareF(KWappPort9203)==0) )
		
			wspType=EWapWspOptionConnectionOriented;
	
	else
		if ( (aPortValue.CompareF(KWappPort9200)==0)||
		  (aPortValue.CompareF(KWappPort9202)==0) )

			wspType=EWapWspOptionConnectionless;
		else  //unknown type -  so apply the default 
			wspType=EWapWspOptionConnectionless;

	return  wspType;
	}

TBool CMsvBIOWapAccessParser::UseSecurity(TDesC& aPortValue)
	{
	TBool useWapSecurity;
	// Is it unsecured link?
	if ( (aPortValue.CompareF(KWappPort9200)==0)||
		  (aPortValue.CompareF(KWappPort9201)==0) )
		useWapSecurity=EFalse;
	else
		if ( (aPortValue.CompareF(KWappPort9202)==0)||
			 (aPortValue.CompareF(KWappPort9203)==0) )
			useWapSecurity=ETrue;
		else  // unknown Port value - shouldn't happen but use default anyway
			useWapSecurity = EFalse;
	
	return useWapSecurity;
	}

//	UpdateStartPageUrlL() - Searchs the WAP table for a match to the name and updates
//							the WAP_START_PAGE field with the value in the URL char of the message
//

void CMsvBIOWapAccessParser::UpdateStartPageUrlL()
	{
	__ASSERT_DEBUG(iWAPSettingsName.Length() != 0, Panic(EWappEmptyBuffer));
	__ASSERT_DEBUG(iUrl.Length() != 0, Panic(EWappEmptyBuffer));

	CCDWAPAccessPointRecord *wapAPRecord = static_cast<CCDWAPAccessPointRecord*>(CCDRecordBase::RecordFactoryL(KCDTIdWAPAccessPointRecord));
	CleanupStack::PushL(wapAPRecord);
	wapAPRecord->iRecordName.SetMaxLengthL(iWAPSettingsName.Length());
	wapAPRecord->iRecordName = iWAPSettingsName;	
	
	if(!wapAPRecord->FindL(*iDBSession))
		{
		User::Leave(KErrNotFound);
		}	
	wapAPRecord->iWAPStartPage.SetMaxLengthL(iUrl.Length());
	wapAPRecord->iWAPStartPage = iUrl;
	wapAPRecord->ModifyL(*iDBSession);	
	CleanupStack::PopAndDestroy(wapAPRecord);
	}


//
// IsValidTokenNameL()	- Checks that the name field at the position specified is 
//						  a valid Characteristic or Parm name
//

TInt CMsvBIOWapAccessParser::IsValidTokenNameL(const TInt aArrayIndex)
	{
	TPtrC name;

	name.Set(iParsedFieldArray->At(aArrayIndex)->FieldName());

	if ( (name.CompareF(KWappXMLversion) ==0)||
		 (name.CompareF(KWappPublicID) ==0)||
		 (name.CompareF(KWappCharacterSet) ==0)||   		
		 (name.CompareF(KWappCharAddress) ==0)||		// ADDRESS Characteristic
		 (name.CompareF(KWappCharURL) ==0)||			// URL Characteristic 
		 (name.CompareF(KWappCharMMSURL) ==0)||			// URL Characteristic 
		 (name.CompareF(KWappCharName) ==0)||				// NAME Characteristic OR ATTRIBUTE 
		 (name.CompareF(KWappCharBookmark) ==0)||		// BOOKMARK Characteristic 
		 (name.CompareF(KWappCharID) ==0)||				// ID Characteristic 
		 (name.CompareF(KWappURL) ==0)||				// URL Parm 
		 (name.CompareF(KWappName) ==0)||				// NAME Parm OR ATTRIBUTE 
		 (name.CompareF(KWappBearer) ==0)||				// BEARER Attribute 
		 (name.CompareF(KWappProxy) ==0)||				// PROXY
		 (name.CompareF(KWappPort) ==0)||				// PORT
		 (name.CompareF(KWappProxyType) ==0)||			// PROXY_TYPE  - for USSD only
		 (name.CompareF(KWappProxyAuthName) ==0)||		// PROXY_AUTHNAME
		 (name.CompareF(KWappProxyAuthSecret) ==0)||	// PROXY_AUTHSECRET
		 (name.CompareF(KWappProxyLoginType) ==0)||		// PROXY_LOGIN_TYPE
		 (name.CompareF(KWappSMSCAddress) ==0)||		// SMS_SMSC_ADDRESS
		 (name.CompareF(KWappUSSDCode) ==0)||			// USSD SERVICE CODE
		 (name.CompareF(KWappAccessPointName) ==0)||	// ACCESSPOINTNAME
		 (name.CompareF(KWappCsdDial) ==0)||			// CSD_DIALSTRING
		 (name.CompareF(KWappPPPAuthType) ==0)||		// PPP_AUTHTYPE
		 (name.CompareF(KWappPPPAuthName) ==0)||		// PPP_AUTHNAME
		 (name.CompareF(KWappPPPAuthSecret) ==0)||		// PPP_AUTHSECRET
		 (name.CompareF(KWappPPPLoginType) ==0)||		// PPP_LOGIN_TYPE
		 (name.CompareF(KWappCsdCallType) ==0)||		// CSD_CALLTYPE
		 (name.CompareF(KWappCsdCallSpeed) ==0)||		// CSD_CALLSPEED
		 (name.CompareF(KWappISP) ==0) )				// ISP_NAME
 

		return KErrNone;
	else 
		return KWappErrUnrecognised;
	}

//
//  WriteBookmarksToFileL() - Store Bookmarks as an attachment file 
//	in the .eBM format. If the system runs out of memory while
//  the bookmarks are being written to the file, the file will be
//	deleted. For example, if 2 bookmarks have already been written
//	to the file, and the writing of the third bookmark fails, the
//	file will be deleted. Otherwise, a failure of file writing would
//	need to be handled differently from memory allocation failure.
//
void CMsvBIOWapAccessParser::WriteBookmarksToFileL()
{
	
	TInt numberOfItems = iBookmarkList->Count();

	if (numberOfItems > 0)	// Only create a file if there is something 
							//to save
	{	

		TFileName filePath;


		// build the filename
		TMsvId entryId = iEntry.Entry().Id();
		filePath.Num(entryId,EHex);
		filePath.Append(KEBookmarkExtension);
		
		// Get the attachment manager and create an empty attachment file
		CMsvStore* store = iEntry.EditStoreL();
		CleanupStack::PushL(store);
		
		MMsvAttachmentManager& manager = store->AttachmentManagerL();
		MMsvAttachmentManagerSync& managerSync = store->AttachmentManagerExtensionsL();		
		RemoveAttachmentIfExistL(filePath, manager, managerSync);
		CMsvAttachment* attachment = CMsvAttachment::NewL(CMsvAttachment::EMsvFile);
		CleanupStack::PushL(attachment);
		attachment->SetAttachmentNameL(filePath);
		
		RFile file;
		managerSync.CreateAttachmentL(filePath, file, attachment);
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
		
		TInt count = 0;
			
		while(count < numberOfItems)
			{
			CWappBookmark& item = *iBookmarkList->At(count);
			
			// Allocate enough space to hold the full bookmark entry.

			TInt length =	item.Name().Length() + 
							item.Url().Length()  + 
							KEBookmarkConstantChars;
			
			HBufC8* writeBuf = HBufC8::NewLC(length);
			
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

			count++;
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
		CleanupStack::PopAndDestroy(2, store); // file, store

	}
}	
//  End of function WriteBookmarksToFileL()

void CMsvBIOWapAccessParser::RemoveAttachmentIfExistL(const TDesC& aFileName, MMsvAttachmentManager& aManager, MMsvAttachmentManagerSync& aManagerSync)
	{
	// If an attachment already exists with the same filename then delete
	// the existing one so multiple files of the same name dont exist
	for( TInt ii=0; ii<aManager.AttachmentCount(); ++ii )
		{
		CMsvAttachment* attachmentInfo = aManager.GetAttachmentInfoL(ii);
		CleanupStack::PushL(attachmentInfo);
		if( attachmentInfo->AttachmentName().CompareF(aFileName) == 0 )
			{
			
			// We have a match, delete the attachment as we will have
			// to add it as new one
			aManagerSync.RemoveAttachmentL(ii);
			CleanupStack::PopAndDestroy(attachmentInfo); // attachmentInfo
			break;
			}
		CleanupStack::PopAndDestroy(attachmentInfo);
		}
	}

void CMsvBIOWapAccessParser::Panic(TInt aPanic)
	{
	User::Panic(KWappDll, aPanic);
	}


CWapCharsetConverter* CWapCharsetConverter::NewL(RFs& aFs, TUint aMibEnum)
	{
	CWapCharsetConverter* self = new (ELeave) CWapCharsetConverter(aFs);
	CleanupStack::PushL(self);
	self->ConstructL(aMibEnum);
	CleanupStack::Pop(self);
	return self;
	}

void CWapCharsetConverter::ConstructL(TInt aMibEnum)
	{
	iConverter = CCnvCharacterSetConverter::NewL();
	iAvailableCharsets = iConverter->CreateArrayOfCharacterSetsAvailableL(iFs);

	if(aMibEnum == 0)
		{
		iParseable = EFalse;
		return;
		}
	
	TInt charset = iConverter->ConvertMibEnumOfCharacterSetToIdentifierL(aMibEnum, iFs);

	if(charset==0)
		{
		iParseable = EFalse;
		return;
		}

	CCnvCharacterSetConverter::TAvailability t = iConverter->PrepareToConvertToOrFromL(charset, iFs);
	iParseable = (t == CCnvCharacterSetConverter::EAvailable);
	}

void CWapCharsetConverter::ConvertL(const TDesC& aSource, TDes& aSink)
	{
	if(iParseable)
		{
		//defect fix for Inc021752
		HBufC8* des = HBufC8::NewLC(aSource.Length());
		des->Des().Copy(aSource);

		TInt state = CCnvCharacterSetConverter::KStateDefault;

		//defect fix for Inc021752
		User::LeaveIfError(iConverter->ConvertToUnicode(aSink, *des, state));
		CleanupStack::PopAndDestroy(des);
		}
	else
		{
		aSink.Copy(aSource);
		}
	}

CWapCharsetConverter::~CWapCharsetConverter()
	{
	delete iConverter;
	delete iAvailableCharsets;
	}
