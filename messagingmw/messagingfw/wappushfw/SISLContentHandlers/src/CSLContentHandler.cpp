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
#include "CSLContentHandler.h"

// System includes
//
#include  <push/sislpushmsgutils.h>
#include <push/cslpushmsgentry.h>
#include <escapeutils.h>
#include <msvids.h>

#include "sltagstable.h"
#include "slattributetable.h"
#include "slattributevaluetable.h"

//text SL MIME type
//_LIT(KSLTextContentType, "text/vnd.wap.sl");

#if defined(_DEBUG)
_LIT(KErrPushMsgNull,	"NULL CPushMessage");
#endif

// Constants
_LIT(KReserved, "Reserved");

void CSLContentHandler::CPushHandlerBase_Reserved1()
	{
	User::Panic(KReserved, KErrNotSupported);
	}

void CSLContentHandler::CPushHandlerBase_Reserved2()
	{
	User::Panic(KReserved, KErrNotSupported);
	}

/**
 * The SL Content handler private constructor.
 * Index number : ESLContentHandlerIndex 
 */ 
CSLContentHandler::CSLContentHandler()
: CContentHandlerBase(), iPushMsgAction(KErrNotFound), iSlMsgEntryId(KMsvNullIndexEntryId)
	{
	}

/**
 *  This will complete initialization of the object
 */
void CSLContentHandler::ConstructL()
	{
	iWapPushUtils= CSISLPushMsgUtils::NewL();
	CActiveScheduler::Add(this);
	}

/**
 * Static Factory Construction
 *
 * Version of NewL which leaves nothing
 * on the cleanup stack
 */
CSLContentHandler* CSLContentHandler::NewL()
	{
	CSLContentHandler* self = new(ELeave) CSLContentHandler;  
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop(self);
	return self;
	}

/**
 * Default d'tor
 */
CSLContentHandler::~CSLContentHandler()
	{
	 __LOG_PTR_DEBUG("CSLContentHandler:: Destructor Called"); 
	 delete iHrefBuf;
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
void CSLContentHandler::HandleMessageL(CPushMessage* aPushMsg, TRequestStatus& aStatus)
	{
	__LOG_PTR_DEBUG("CSLContentHandler:: HandleMessage Async Func. Called"); 
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
void CSLContentHandler::HandleMessageL(CPushMessage* aPushMsg)
	{
	__LOG_PTR_DEBUG("CSLContentHandler:: HandleMessage Sync Func. Called"); 
	__ASSERT_DEBUG( aPushMsg != NULL, User::Panic(KErrPushMsgNull, KErrNone));
	
	iAcknowledge = EFalse;
	iMessage = aPushMsg;
	
	iState= EParsing;
	IdleComplete();
	}

/**
 * Parse the Push SL message using XML parser.
 * If Push Message is an SLC then convert it first to text using 
 * CWbxmlConverterUtil class.
 */
void CSLContentHandler::ParsePushMsgL()
	{
	__LOG_PTR_DEBUG("CSLContentHandler::ParsePushMsgL called")

	CMessageParser* myParser = CMessageParser::NewL ( *iMessage, 
													*this, 
													&sltagstable::Table,
													&slattributetable::Table,
													&slattributevaluetable::Table );
	CleanupStack::PushL ( myParser );
	myParser->ParseMessageL ();

		

		

		

	
	
	
	User::LeaveIfError ( myParser->LastError() );
	
	CleanupStack::PopAndDestroy ( myParser );
	
  	// if 'action' attribute not specified, the value 'execute-low' is used.
	if( iPushMsgAction == KErrNotFound )
		{
		iPushMsgAction= CSLPushMsgEntry::ESLPushMsgExecuteLow;
		}
	


	// 'href' attribute is mandatory, if not specified then leave with an error.
	if( !iHrefBuf )
		{
		User::Leave(KErrCorrupt);
		}

	iState= ESearching;
	IdleComplete();
	}

/**
 * Searching for an existing SL Msg entry in the message store
 */
void CSLContentHandler::SearchingForDuplicatesMsgEntryL()
	{
	__LOG_PTR_DEBUG("CWapPushMsgUtils: FindUrlL called")
	iSlMsgEntryId= iWapPushUtils->FindUrlL(*iHrefBuf, KUidWapPushMsgSL);
	
	iState= EProcessing;
	IdleComplete();
	}

/**
 *  Creating/Saving or Updating SL Msg Entry in the message store
 *   - If there is NO duplicate Msg then Create/Save a PushMsgEntry from
 *      the received PushMsg.
 *   - If there is a  duplicate Msg then Update the existing PushMsgEntry.
 *
 *    The recieved PushMsg is deleted in any case in the destructor.
 */
void CSLContentHandler::ProcessingPushMsgEntryL()
	{	
	TBool isInt;
	TPtrC8 appURI;
	TInt appID=0;
	iMessage->GetAppIdL(appURI, appID, isInt);

	CSLPushMsgEntry* slPushMsgEntry=NULL;
	if (isInt)
		{
		slPushMsgEntry = CSLPushMsgEntry::NewL(appID);
		}
	else
		{
		slPushMsgEntry = CSLPushMsgEntry::NewL(appURI);
		}
	CleanupStack::PushL(slPushMsgEntry);

	if(iSlMsgEntryId == KMsvNullIndexEntryId)
		{
		//SL Msg with same href not existing in Msg Store ==> Create/Save the SL Msg
		SetSlPushMsgEntryFieldsL(*slPushMsgEntry);
		TMsvId localFolderId;
		iWapPushUtils->GetPushMsgFolderIdL(localFolderId);
		__LOG_PTR_DEBUG("CSLPushMsgEntry: SaveL called");
		iSlMsgEntryId = slPushMsgEntry->SaveL(iWapPushUtils->Session(), localFolderId);
		}
	else
		{
		//Find Msg of the same href THEN need to Update IF it has a low priorty action attribute
		__LOG_PTR_DEBUG("CWapPushMsgUtils: GetAction called");
		if(iPushMsgAction > iWapPushUtils->GetActionL(iSlMsgEntryId)) 
			{
			// The received SL Push Msg has higher action than existing SL Entry THEN
			// Update existing SL Msg Entry		
			// Change header Fields and action Attribute on the existing entry and UPDATE 
			__LOG_PTR_DEBUG("CSLPushMsgEntry: RetrieveL called");
			slPushMsgEntry->RetrieveL(iWapPushUtils->Session(),iSlMsgEntryId);
			SetSlPushMsgEntryFieldsL(*slPushMsgEntry);
			__LOG_PTR_DEBUG("CSLPushMsgEntry: UpdateL called");
			slPushMsgEntry->UpdateL(iWapPushUtils->Session());
			}
		}
	CleanupStack::PopAndDestroy(); //slPushMsgEntry

	iState= EDone;
	IdleComplete();
	}

/**
 *	Set SL entry fields prior to storing message.
 *	@param aSlPushMsgEntry
 *		entry represents message format to use when storing it
 */
void CSLContentHandler::SetSlPushMsgEntryFieldsL(CSLPushMsgEntry& aSlPushMsgEntry)
	{
	
	//set URL and Action fields
	aSlPushMsgEntry.SetUrlL(*iHrefBuf);
	aSlPushMsgEntry.SetAction(iPushMsgAction);
	
	// Set all the relevant header fields
	TPtrC8 msgHeaderPtr;
	iMessage->GetHeader(msgHeaderPtr);
	aSlPushMsgEntry.SetHeaderL(msgHeaderPtr); 

	TPtrC8 from;
	if (!iMessage->GetBinaryHeaderField(EHttpFrom,from) &&
		!iMessage->GetBinaryHeaderField(EHttpXWapInitiatorURI,from) &&
		!iMessage->GetBinaryHeaderField(EHttpContentLocation,from) )
		{
		from.Set(KNullDesC8);
		}
		aSlPushMsgEntry.SetFromL(from);
		
	if(iMessage->MessageAllowed())
		{
		aSlPushMsgEntry.SetTrusted(ETrue);
		}
	else
		{
		aSlPushMsgEntry.SetTrusted(EFalse);	
		}
		
	TPtrC8 serverAddress8;
	iMessage->GetServerAddress(serverAddress8);
	aSlPushMsgEntry.SetMsgOriginUriL(serverAddress8);
		
	TTime puchMsgDate;
	if(iMessage->GetHeaderField(EHttpDate, puchMsgDate))
		aSlPushMsgEntry.SetTimeSent(puchMsgDate );
	}

/**
 * Same functionality as DoCancel()
 */
void CSLContentHandler::CancelHandleMessage()
	{
	 __LOG_PTR_DEBUG("CSLContentHandler:: CancelHandleMessage Called"); 
	Complete(KErrCancel);
	}

/**
 *  Terminates any activity
 */
void CSLContentHandler::DoCancel()
	{
	 __LOG_PTR_DEBUG("CSLContentHandler:: DoCancel Called"); 
	Complete(KErrCancel);
	}

/**
 *	Step through the various representative states for handling a message 
 *  Four States:
 *	 EParsing - Parses SL push message (retrieving the href and action attributes)
 *	 ESearching - Searching in the msg Store for any duplicate Sl Msg Entry
 *	 EProcessing - Save or update SL msg in the Msg store
 *	 EDone - Clean up
 */
void CSLContentHandler::RunL()
	{
	 __LOG_PTR_DEBUG("CSLContentHandler:: RunL Called");
	 switch(iState)
		{
		case EParsing:
			ParsePushMsgL();
			break;
		case ESearching:
			SearchingForDuplicatesMsgEntryL(); 
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
 *	This is invoked when RunL Leaves with an error so clean up and return
 */
TInt CSLContentHandler::RunError(TInt aError)
	{
	__LOG_PTR_DEBUG("CSLContentHandler::RunError Called"); 
	iState=EDone;
	Complete(aError);
	return KErrNone;
	}

void CSLContentHandler::HandleElementL ( const RString& /* aTag */, const RString& aAttributeName, const RString& aAttributeValue )
	{
	__LOG_PTR_DEBUG("CSIContentHandler:: HandleElementL Called");
	
	switch ( aAttributeName.Index ( slattributetable::Table ) )
		{
		case slattributetable::EHref:
		delete iHrefBuf;
		iHrefBuf = NULL;
		iHrefBuf = EscapeUtils::ConvertToUnicodeFromUtf8L ( aAttributeValue.DesC() );
		break;
		
		case slattributetable::EAction1:
		case slattributetable::EAction2:
		case slattributetable::EAction3:
			{
			switch ( aAttributeValue.Index ( slattributevaluetable::Table ) ) 
				{
				// execute-low
				case slattributevaluetable::EAction1:
				iPushMsgAction = CSLPushMsgEntry::ESLPushMsgExecuteLow;
				break;
				
				case slattributevaluetable::EAction2:
				iPushMsgAction = CSLPushMsgEntry::ESLPushMsgExecuteHigh;
				break;
				
				case slattributevaluetable::EAction3:
				iPushMsgAction = CSLPushMsgEntry::ESLPushMsgExecuteCache;
				break;
				
				default:
				// shouldn't come here.
				User::Invariant ();	
				}
			}
		break;
		
		default:
		// shouldn't come here.
		User::Invariant ();
		}	
	}

void CSLContentHandler::HandleContentL ( const TDesC8& /* aBytes */ )
	{
	// SL message doesn't contain text. so can be ignored.	
	User::LeaveIfError ( KErrNone );
	}


