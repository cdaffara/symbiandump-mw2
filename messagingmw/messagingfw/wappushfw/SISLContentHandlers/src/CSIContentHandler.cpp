// Copyright (c) 2001-2009 Nokia Corporation and/or its subsidiary(-ies).
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

// Local includes
//
#include "CSIContentHandler.h"

// System includes
//
#include  <push/sislpushmsgutils.h>
#include <push/csipushmsgentry.h>
#include <escapeutils.h>
#include <msvids.h>
#include <inetprottextutils.h>

// SI string tables.
#include "sitagstable.h"
#include "siattributetable.h"
#include "siattributevaluetable.h"


const TInt KValidUTCLength = 20;


#if defined(_DEBUG)
_LIT(KErrPushMsgNull,	"NULL CPushMessage");
#endif

// Constants
_LIT(KReserved, "Reserved");

void CSIContentHandler::CPushHandlerBase_Reserved1()
	{
	User::Panic(KReserved, KErrNotSupported);
	}

void CSIContentHandler::CPushHandlerBase_Reserved2()
	{
	User::Panic(KReserved, KErrNotSupported);
	}

/**
 * The SL Content handler private constructor.
 * Index number : ESIContentHandlerIndex 
 */ 
CSIContentHandler::CSIContentHandler()
: CContentHandlerBase(), iPushMsgAction(KErrNotFound), iExpiresTime(Time::NullTTime()), iCreatedTime(Time::NullTTime()), iValidDate(ETrue)
	{
	}

/**
 *  This will complete initialization of the object
 */
void CSIContentHandler::ConstructL()
	{
	iWapPushUtils = CSISLPushMsgUtils::NewL();
	CActiveScheduler::Add(this);
	}

/**
 * Static Factory Construction
 *
 * Version of NewL which leaves nothing
 * on the cleanup stack
 */
CSIContentHandler* CSIContentHandler::NewL()
	{
	CSIContentHandler* self = new(ELeave) CSIContentHandler;
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop(self);
	return self;
	}

/**
 * Default d'tor
 */
CSIContentHandler::~CSIContentHandler()
	{
	__LOG_PTR_DEBUG("CSIContentHandler:: Destructor Called"); 
	delete iHrefBuf;
	delete iSiIdBuf;
	delete iData;
	delete iWapPushUtils;
	}

/**
 * HandleMessage Async. Version
 *	Takes ownership of Push Message and sets self active to continue
 *	processing message.
 *	@param aPushMsg
 *		CPushMessage to process
 *	@param aStatus
 *		request status variable for use in asynchronous operations
 */
void CSIContentHandler::HandleMessageL(CPushMessage* aPushMsg, TRequestStatus& aStatus)
	{
	__LOG_PTR_DEBUG("CSIContentHandler:: HandleMessage Async Func. Called"); 
	__ASSERT_DEBUG( aPushMsg != NULL, User::Panic(KErrPushMsgNull, KErrNone));

	iMessage = aPushMsg;
	iAcknowledge = ETrue;
	SetConfirmationStatus(aStatus);

	iState= EParsing;
	IdleComplete();
	}

/**
 * HandleMessage Sync. Version
 *	Takes ownership of Push Message and sets self active to continue
 *	processing message.
 *
 *	Initial State: Set data members then go to the next state 
 *	@param aPushMsg
 *		CPushMessage to process
 */
void CSIContentHandler::HandleMessageL(CPushMessage* aPushMsg)
	{
	__LOG_PTR_DEBUG("CSIContentHandler:: HandleMessage Sync Func. Called");
	__ASSERT_DEBUG( aPushMsg != NULL, User::Panic(KErrPushMsgNull, KErrNone));
	
	iAcknowledge = EFalse;
	iMessage = aPushMsg;

	iState =EParsing;
	IdleComplete();
	}

/** 
 *	Parse the SI message from XML to its component parts which are held
 *	in the document tree. This tree is then examined and the attributes
 *	read and recorded for later reference. Once parsed the parser and tree
 *	are deleted as no longer required.
 *	@leave KErrNotFound
 *		indicates general processing error and to abandon processing
 *	@leave KErrCorrupt
 *		there is no body in the message
 *	@leave TDesC::AllocL
 
 */
void CSIContentHandler::ParsePushMsgL()
	{
	__LOG_PTR_DEBUG("CSIContentHandler:: ParsePushMsgL. Called"); 

	CMessageParser* myParser = CMessageParser::NewL ( *iMessage, 
													*this, 
													&sitagstable::Table,
													&siattributetable::Table,
													&siattributevaluetable::Table );
	CleanupStack::PushL ( myParser );
	
	myParser->ParseMessageL ();

	User::LeaveIfError ( myParser->LastError() );

	CleanupStack::PopAndDestroy ( myParser );

	if (iData)
	{
	( iData->Des() ).TrimLeft ();
	( iData->Des() ).TrimRight ();		
	}
	iState = EProcessing;
	IdleComplete();
	}

	// Return value NULL indicates failed conversion!
	// Ownership of the resulting string is passed up to caller.
	HBufC16* CSIContentHandler::FormatDateStringL(const TDesC& aData)
	{
	const TInt KDateTimePart = 7;
	enum TDateTimeParts {EYearTop, EYearBot, EMon, EDay, EHour, EMin, ESec};
	TInt dateData[KDateTimePart];	
	
	for (TInt i = 0; i < KDateTimePart; i++)
		dateData[i] = 0;
	
	TInt offSet = 0;
	TInt dataLen = aData.Length();
	

	while (offSet < KDateTimePart && dataLen > 0)
		{
		dateData[offSet] = aData[offSet];
		offSet++;
		dataLen--;
		}

		_LIT(KDateFormat,"%2x%02x-%02x-%02xT%02x:%02x:%02xZ");
		const TInt KDateFormatLen = 20;
		
		TBuf16<KDateFormatLen> dateBuf;
		dateBuf.Format(KDateFormat, dateData[EYearTop], 
								dateData[EYearBot], 
								dateData[EMon], 
								dateData[EDay], 
								dateData[EHour], 
								dateData[EMin], 
								dateData[ESec]);
	return( dateBuf.AllocL() );	
}
		

/** 
 *	Convert UTC date time into native TTime format. Method can not leave
 *	and traps any leaves from ValidateUTCTimeL.
 *	@param aDateTime
 *			the UTC string representing the date and time
 *	@param aConvertedDate
 *			member date variable passed in to accept converted UTC date as TTime varaible
 *	@return TBool
 *			indicates if conversion successful (ETrue) or conversion failed (EFalse)
 */
TBool CSIContentHandler::ConvertDateTimeL(const TDesC& aDateTime, TTime &aConvertedDate)
	{
	TTime convertedTime = Time::NullTTime();
	TBool convertedOK = EFalse;

	HBufC* dateTime = NULL;
	if (aDateTime.Length() == KValidUTCLength) //check supplied descriptor is the correct length
		{		
		dateTime = aDateTime.AllocLC ();
		}
	else
		{
		// Convert to UTC string
		dateTime = FormatDateStringL ( aDateTime );
		CleanupStack::PushL ( dateTime );		
		}
		
	TBuf<20> str = dateTime->Des();
	if (IsValidUTCTime(str))
		{
		// adjust UTC time to zero offset TTime
		const TInt KFirstMonthChar = 4;
		const TInt KSecondMonthChar = KFirstMonthChar + 1;
		const TInt KFirstDayChar = 6;
		const TInt KSecondDayChar = KFirstDayChar + 1;
		// check for special case of month = 10 which becomes 09
		if (str[KFirstMonthChar]=='1' && str[KSecondMonthChar] == '0')
			{
			str[KFirstMonthChar]='0';
			str[KSecondMonthChar]='9';
			}
		else
			// month value is either 11, 12 or less than 10, ie 1 - 9.
			// reduce day by one, eg 11 becomes 10, 12 becomes 11, 09 becomes 08
			str[KSecondMonthChar]-- ; 
		
		// check for special cases 10, 20, 30
		if (str[KSecondDayChar] == '0')
			{
			// reduce day by 1, ie 10 becomes 09, 20 becomes 19 ...
			str[KSecondDayChar] = '9';
			str[KFirstDayChar]--;
			}
		else
			// day value is between 1 and 9 so reduce day by one
			// eg 29 becomes 28, 11 bcomes 10, 31 becomes 30
			str[KSecondDayChar]--;
		
		// string is now syntaxically correct and year will be not be negative
		// so no errors will occur here and will now have a valid TTime
		convertedTime.Set(str);
		convertedOK = ETrue;
		}	
	
	aConvertedDate = convertedTime;
	CleanupStack::PopAndDestroy ( dateTime );
	if(!convertedOK)
	   {
	   iValidDate = EFalse;
	   }
	return convertedOK;
	}

/**
 *	Check given UTC time string conforms to expected format:
 *	YYYY-MM-DDTHH:MM:SSZ and strip out formatting characters
 *	then validate the remaining characters are all digits.
 *	If validated then add TTime formating character so final
 *	string returned will be of the format YYYYMMDD:HHMMSS.
 *	@param aDateTime
 *		UTC string which is validated and returned
 *	@return boolean 
 *		indicates if given date is valid (ETrue)
 *		or not (EFalse)
 */
TBool CSIContentHandler::IsValidUTCTime(TDes& aDateTime)
	{
	// strip out formatting characters
	TInt formatCharPos = 4;
	aDateTime.Delete(formatCharPos, 1); 
	// now move through two characters at a time and remove other chars 
	// to just leave digits
	TInt KRemainingFormatChars = 5;
	for (TInt i = 0 ; i < KRemainingFormatChars; i++)
		{
		formatCharPos += 2;
		aDateTime.Delete(formatCharPos, 1);
		}
	
	// check string now the right TTime length
	const TInt KValidTTimeLength = 14;
	if (aDateTime.Length() != KValidTTimeLength)
		return EFalse;

	// now have UTC string stripped of format characters - check remaining characters are
	// all digits - YYYYMMDDHHMMSS
	TChar ch;
	for (TInt x = 0; x < KValidTTimeLength; x++)
		{
		ch = aDateTime[x];
		if (ch.IsDigit() == EFalse)
			return EFalse;
		}
	
	TInt err = ValidateDateTime( aDateTime );
	if(!err)   
	return EFalse;

	// insert colon seperating date from time
	_LIT(colon, ":");
	const TInt KColonPosition = 8;
	aDateTime.Insert(KColonPosition, colon);
	return ETrue;
	}

/**
 * Validate date and time to check whether date and time values lie 
 * within the valid range
 *	@param aDateTime
 *		UTC string which is validated 
 *	@return boolean 
 *		indicates if given date and time is valid (ETrue)
 *		or not (EFalse)
 */
TBool CSIContentHandler::ValidateDateTime(TDes& aDateTime)
    {
	TPtrC ptr(aDateTime);
	TPtrC yr;
	yr.Set(ptr.Left(4));	   
	TInt yearInt;
	InetProtTextUtils::ConvertDescriptorToInt(yr,yearInt);
	   
	TPtrC month;
	month.Set(ptr.Mid(4));
	TPtrC mon;
	mon.Set(month.Left(2));
	TInt monthInt;
	InetProtTextUtils::ConvertDescriptorToInt(mon,monthInt);
	   
	TPtrC day;
	day.Set(month.Mid(2));
    TPtrC dy;
	dy.Set(day.Left(2));
	TInt dayInt;
	InetProtTextUtils::ConvertDescriptorToInt(dy,dayInt);
	   
	TPtrC hour;
    hour.Set(day.Mid(2));
    TPtrC hr;
    hr.Set(hour.Left(2));
	TInt hourInt;
    InetProtTextUtils::ConvertDescriptorToInt(hr,hourInt);
	   
	TPtrC minute;
	minute.Set(hour.Mid(2));
	TPtrC min;
	min.Set(minute.Left(2));
	TInt minInt;
    InetProtTextUtils::ConvertDescriptorToInt(min,minInt);
	   
    TPtrC second;
	second.Set(minute.Mid(2));
	TPtrC sec;
	sec.Set(second.Left(2));
	TInt secInt;
	InetProtTextUtils::ConvertDescriptorToInt(sec,secInt);
	    
		
	if ( monthInt == 1 ||  monthInt == 3 || monthInt == 5 || monthInt == 7
		 || monthInt == 8 || monthInt == 10 || monthInt == 12)
         {
         if (!(dayInt <= 31))
	         return EFalse;	
         }	 
    else if (monthInt == 04 ||  monthInt == 06 || monthInt == 9 || monthInt == 11)
             {
             if (!(dayInt <= 30))
	             return EFalse;
	         }
    else if (monthInt == 02)
             {
             if (Time::IsLeapYear(yearInt))
                 {
                 if (!(dayInt <= 29))
	                 return EFalse;		                  
	             }
	         else if (!(dayInt <= 28))	                  
	                  return EFalse;
	         }	                 
    else 
       {
       return EFalse;
       }
  
    if ( !(hourInt <= 23 && minInt <= 59 && secInt <= 59))
        return EFalse;
    
    return ETrue;
    }

/** 
 *	Convert the action string to a representative numeric value to facilitate 
 *	storing the message in the wap push utils.
 *	@param aActionString
 *		the attribute value indicating the action level eg 'signal-high'
 *	@return TUint
 *		a value representing the action type (as defined in PushEntry.h)
 */
TUint CSIContentHandler::ConvertActionString(const RString& aActionString)
	{
	// set to default signal value (to rid ourselves of build warning)
	TUint actionValue = CSIPushMsgEntry::ESIPushMsgSignalMedium;	
	switch ( aActionString.Index ( siattributevaluetable::Table ) )
		{
		case siattributevaluetable::EAction1:
		actionValue = CSIPushMsgEntry::ESIPushMsgSignalNone;
		break;
		case siattributevaluetable::EAction2:
		actionValue = CSIPushMsgEntry::ESIPushMsgSignalLow;
		break;
		case siattributevaluetable::EAction3:
 		actionValue = CSIPushMsgEntry::ESIPushMsgSignalMedium;
		break;
		case siattributevaluetable::EAction4:
 		actionValue = CSIPushMsgEntry::ESIPushMsgSignalHigh;
		break;		
		case siattributevaluetable::EAction5:
		actionValue = CSIPushMsgEntry::ESIPushMsgDelete;
		break;						
		default:
		User::Invariant ();
		}
	return actionValue;
	}

/** 
 *	Handles message reception and processes the received message following 
 *	the flow of actions detailed in 7.2 Reception 
 *	[WAP Service Indication 08-Nov-1999]
 *		1. check if received message is expired 
 *		2. if received message is an older version of stored SI,
 *			or is the same age - discard received message 
 *		3. if message is more recent version than stored SI - delete stored SI
 *		4. if message actioned to delete, and has Si Id set, then delete it 
 *			and any stored messages with same id
 *	finally if received message not set to deleted or discarded - store it
 */
void CSIContentHandler::ProcessingPushMsgEntryL()
	{
	__LOG_PTR_DEBUG("CSIContentHandler:: ProcessingPushMsgEntryL. called");
	TBool deletePushMsg = EFalse;
	
	// 1. Expiration
	TTime today;
	today.UniversalTime();
	// check if message has expiry date before today's date
	if (ExpiresFlag() && iExpiresTime < today)
		{
		deletePushMsg = ETrue;
		__LOG_PTR_DEBUG("CSIContentHandler: ProcessingPushMsgEntryL: Expired msg (1)");
		}
	
	// 2. handling out of order delivery & 
	// 3. replacement
	else if (SiIdFlag() || HrefFlag())
		deletePushMsg = HandleMsgOrderReceptionL();

	// 4. deletion
	if (!deletePushMsg && ActionFlag())
		{
		// check if action = delete and SiId value is set before can delete
		if ((iPushMsgAction == CSIPushMsgEntry::ESIPushMsgDelete) && (SiIdFlag())) 
			{
			__LOG_PTR_DEBUG("CSIContentHandler: ProcessingPushMsgEntryL: delete msg (4)");
			
			// delete msg and any matching messages (with same si-id)
			__LOG_PTR_DEBUG("CWapPushMsgUtils: FindSiIdLC  called");
			CMsvEntrySelection* matchingIdList = iWapPushUtils->FindSiIdLC(*iSiIdBuf);
			TInt matchingListCount = matchingIdList->Count();
			for (TInt count = 0; count < matchingListCount; count++)
				{
				TMsvId matchingSiMsgEntryId = matchingIdList->At(count);
				if (matchingSiMsgEntryId)
					{
					__LOG_PTR_DEBUG("CWapPushMsgUtils: DeleteEntryNowL called");
					iWapPushUtils->DeleteEntryNowL(matchingSiMsgEntryId);
					}
				}
			CleanupStack::PopAndDestroy(); // matchingIdList
			deletePushMsg = ETrue;
			}
		}

	// store message if not marked for deletion and date/time is valid
	if (!deletePushMsg && iValidDate)
		StoreSIMessageL();
	// Reset the flag to ETrue for next message
	iValidDate = ETrue;

	iState = EDone;
	IdleComplete();
	}

/**
 *	Check received SI message against existing stored messages
 *	with the same si-id and update the message(s) according to
 *	the creation date (if specified).
 *	@return TBool
 *		ETrue - Received SI message older than stored match(es)
 *				and should be marked for deletion
 *		EFalse - Receieved SI message to be retained as newer
 *				therefore do not delete it
 */
TBool CSIContentHandler::HandleMsgOrderReceptionL()
	{
	CMsvEntrySelection* matchingIdList;
	TBool discardPushMsg = EFalse;

	__LOG_PTR_DEBUG("CSISLPushMsgUtils: FindSiIdLC called");
	// get list of matching stored SI messages 
	if (SiIdFlag())
		matchingIdList = iWapPushUtils->FindSiIdLC(*iSiIdBuf);
	else
		matchingIdList = iWapPushUtils->FindSiIdLC(*iHrefBuf);

	// check we have a creation date specified before checking against list of matches
	if (matchingIdList->Count() && CreatedFlag())
		{
		CSIPushMsgEntry* siEntry=GetSiEntryL();	
		CleanupStack::PushL(siEntry);
			
		// delete older stored messages and/or mark current message for deletion
		// if same date or older than stored messages
		TInt matchingListCount = matchingIdList->Count();
		for (TInt count = 0; count < matchingListCount; count++)
			{
			TMsvId matchingSiMsgEntryId = matchingIdList->At(count);
						
			__LOG_PTR_DEBUG("CSIPushMsgEntry: RetrieveL called");
			siEntry->RetrieveL(iWapPushUtils->Session(), matchingSiMsgEntryId);
		
			// skip date comparisons if creation date not valid
			TTime existingSiCreatedTime = siEntry->Created();
			if (existingSiCreatedTime == Time::NullTTime())
				continue;
			//check if received SI is newer than existing stored Si (out of order)
			if (iCreatedTime > existingSiCreatedTime) 
				{
				__LOG_PTR_DEBUG("CWapPushMsgUtils: DeleteEntryNowL called");
				iWapPushUtils->DeleteEntryNowL(matchingSiMsgEntryId);
				__LOG_PTR_DEBUG("CSIContentHandler: ProcessingPushMsgEntryL: delete stored msg (3)");
				}
			else if (iCreatedTime <= existingSiCreatedTime)
				// received SI older than existing stored Si (replacement)
				discardPushMsg = ETrue;
			}
		CleanupStack::PopAndDestroy(siEntry);
		}
		
	CleanupStack::PopAndDestroy(matchingIdList);
		
	if (discardPushMsg)
		{
		__LOG_PTR_DEBUG("CSIContentHandler: ProcessingPushMsgEntryL: discard msg (2)");
		}
	return discardPushMsg;
	}


/** 
 *	Save push message in message store.
 * 	Creates link to message service provided by wap push utils
 *	and uses this and the associated push entry class to save
 *	the push message.
 */
void CSIContentHandler::StoreSIMessageL() 
	{
	__LOG_PTR_DEBUG("CSIContentHandler:: StoreSiMessageL. called");

	CSIPushMsgEntry* siEntry=GetSiEntryL();
	CleanupStack::PushL(siEntry);
	SetSIPushMsgEntryFieldsL(*siEntry);
	
	TMsvId folderId;
	iWapPushUtils->GetPushMsgFolderIdL(folderId);

	__LOG_PTR_DEBUG("CSIPushMsgEntry::SaveL called");
	siEntry->SaveL(iWapPushUtils->Session(), folderId);
	
	CleanupStack::PopAndDestroy(siEntry);
	}


CSIPushMsgEntry* CSIContentHandler::GetSiEntryL()
	{
	TBool isInt;
	TPtrC8 appURI;
	TInt appID=0;
	iMessage->GetAppIdL(appURI, appID, isInt);
	CSIPushMsgEntry* siEntry=NULL;
	if (isInt)
		{
		siEntry = CSIPushMsgEntry::NewL(appID);
		}
	else
		{
		siEntry = CSIPushMsgEntry::NewL(appURI);
		}
	return siEntry;
	}

/** 
 *	Create SI entry fields prior to storing message. The saving of
 * 	certain push message attributes will be set to defaults if 
 *	information missing to facilitate message searching, storing 
 *	and retrieval.
 *	@param siEntry 
 *		entry represents message format to use when storing it
 */
void CSIContentHandler::SetSIPushMsgEntryFieldsL(CSIPushMsgEntry& aSIPushMsgEntry)
	{
	// if message has no si-id but does have a href - use href as si-id and
	// if message has a si-id but no href use si-id for href
	
	if (SiIdFlag() || HrefFlag())
		{
		if (SiIdFlag() && (HrefFlag() == EFalse))
			{
			aSIPushMsgEntry.SetIdL(*iSiIdBuf);
			aSIPushMsgEntry.SetUrlL(*iSiIdBuf);
			}
		else if (HrefFlag() && (SiIdFlag() == EFalse))
			{
			aSIPushMsgEntry.SetIdL(*iHrefBuf);
			aSIPushMsgEntry.SetUrlL(*iHrefBuf);
			}
		else
			{
			aSIPushMsgEntry.SetIdL(*iSiIdBuf);
			aSIPushMsgEntry.SetUrlL(*iHrefBuf);
			}
		}
	
	if (ActionFlag())
		aSIPushMsgEntry.SetAction(iPushMsgAction);
	else // default if no action explicitly stated
		aSIPushMsgEntry.SetAction(CSIPushMsgEntry::ESIPushMsgSignalMedium);
	
	// uses default null time value if no explicit date set in message
	aSIPushMsgEntry.SetCreated(iCreatedTime);
	aSIPushMsgEntry.SetExpires(iExpiresTime);
	
	// PCDATA (text) from message
	if (DataFlag())
		aSIPushMsgEntry.SetTextL(*iData);
	
	TPtrC8 msgHeaderPtr;
	iMessage->GetHeader(msgHeaderPtr);	
	aSIPushMsgEntry.SetHeaderL(msgHeaderPtr);
	
	TPtrC8 from;
	if (!iMessage->GetBinaryHeaderField(EHttpFrom, from) &&
		!iMessage->GetBinaryHeaderField(EHttpXWapInitiatorURI, from) &&
		!iMessage->GetBinaryHeaderField(EHttpContentLocation, from) )
		{
		from.Set(KNullDesC8);
		}
		aSIPushMsgEntry.SetFromL(from);
	
	if(iMessage->MessageAllowed())
		{
		aSIPushMsgEntry.SetTrusted(ETrue);
		}
	else
		{
		aSIPushMsgEntry.SetTrusted(EFalse);	
		}
		
	TPtrC8 serverAddress8;
	iMessage->GetServerAddress(serverAddress8);
	aSIPushMsgEntry.SetMsgOriginUriL(serverAddress8);
	}

/** 
 *	Same functionality as DoCancel()
 */
void CSIContentHandler::CancelHandleMessage()
	{
	__LOG_PTR_DEBUG("CSIContentHandler:: CancelHandleMessage called");
	Complete(KErrCancel);
	}

/** 
 *	Cancels the handling of the message and revokes the active status
 *	of the handler 
 */
void CSIContentHandler::DoCancel()
	{
	__LOG_PTR_DEBUG("CSIContentHandler:: DoCancel Called");
	Complete(KErrCancel);	
	}

/** 
 *	Step through the various representative states for handling a message 
 *	1. validate the message by parsing it against the relevant DTD
 *	2. process the message in accordance with the Reception rules
 *	3. finish up 
 */
void CSIContentHandler::RunL()
	{
	__LOG_PTR_DEBUG("CSIContentHandler:: RunL Called");
	// use active state machine routine to manage activites:
	switch (iState)
		{
	case EParsing : 
		ParsePushMsgL();
		break;
	case EProcessing:
		ProcessingPushMsgEntryL();
		break;
	case EDone:
		Complete(KErrNone);
		break;
	default:
		break;
		}
	}

/** 
 * CPluginActiveBase methods
 */
TInt CSIContentHandler::RunError(TInt aError)
	{
	__LOG_PTR_DEBUG("CSIContentHandler:: RunError Called"); 
	iState=EDone;
	Complete(aError);
	return KErrNone;
	}

void CSIContentHandler::HandleElementL ( const RString& aTag, const RString& aAttributeName, const RString& aAttributeValue )
	{
	__LOG_PTR_DEBUG( "CSIContentHandler:: HandleElementL Called" );
	
	const TDesC8& tag = aTag.DesC();
	iIndicationTagFound = EFalse;
	
	if ( aTag.Index ( sitagstable::Table ) == sitagstable::EIndication )
		{
		iIndicationTagFound = ETrue;
		}
	
	TBool gotDate = EFalse;	
	HBufC* des16 = EscapeUtils::ConvertToUnicodeFromUtf8L ( aAttributeValue.DesC() );	
	CleanupStack::PushL ( des16 );

	switch ( aAttributeName.Index ( siattributetable::Table ) )
		{
		case siattributetable::EAction1:
		case siattributetable::EAction2:
		case siattributetable::EAction3:		
		case siattributetable::EAction4:
		case siattributetable::EAction5:
		iPushMsgAction = ConvertActionString( aAttributeValue );
		SetActionFlag(ETrue);		
		break;
			
		case siattributetable::ECreated:
		gotDate = ConvertDateTimeL ( *des16, iCreatedTime );
		SetCreatedFlag(gotDate);
		break;
		
		case siattributetable::EHref:
		delete iHrefBuf;
		iHrefBuf = NULL;
		
		iHrefBuf = ( des16->Des() ).AllocL ();
		SetHrefFlag(ETrue);		
		break;

		case siattributetable::ESiExpires:
		gotDate  = ConvertDateTimeL ( *des16, iExpiresTime );
		SetExpiresFlag(gotDate);
		break;
		
		case siattributetable::ESiId:
		delete iSiIdBuf;
		iSiIdBuf = NULL;
		
		iSiIdBuf = ( des16->Des() ).AllocL (); 
		SetSiIdFlag(ETrue);
		break;
		
		case siattributetable::EClass:
		break;

		default:
		// Shouldn't come here.
		User::Invariant ();
		}

	CleanupStack::PopAndDestroy ( des16 );
	}

void CSIContentHandler::HandleContentL ( const TDesC8& aBytes )
	{	
	// We do handle content only if it is in indication tag.
	// else we reject the content.
	if ( !iIndicationTagFound )
		{
		return;
		}
	if ( iData )
		{
		HBufC* des16 = EscapeUtils::ConvertToUnicodeFromUtf8L ( aBytes );
		CleanupStack::PushL ( des16 );		
		
		TInt newDataLength = des16->Length ();
		TInt existingDataLength = iData->Length ();
				
		if ( ( newDataLength + existingDataLength ) > existingDataLength )
			{
			iData = iData->ReAllocL ( newDataLength + existingDataLength );
			}
			
		(iData->Des()).Append( *des16 );			
		
		CleanupStack::PopAndDestroy ( des16 );		
		}
	else
		{
		iData = EscapeUtils::ConvertToUnicodeFromUtf8L ( aBytes );
		}	
	SetDataFlag ( ETrue );
	}

