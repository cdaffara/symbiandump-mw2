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

#include <wapmessage.h>
// Local includes
//
#include "csiacontenthandler.h"
#include "clwatcher.h"

// System includes
//
#include <commdb.h>
#include "wapdecoder.h"
#include <push/cwappushmsgutils.h>

const TUid KSIADialogNotifierUid={0x100096f6}; //from WapPushSIADialogNotifier.h
_LIT8(KWapPushCmdConnect,"Connect"); // from WapPushSIADialogNotifier.h

#if defined(_DEBUG)
_LIT(KErrPushMsgNull,	"NULL CPushMessage");
#endif

//comment out this line if testing without notifier
#define _ASK_USER_TO_CONNECT

//supported bearers for connection-oriented push
const TUint8 KCSDBearer = 0x0A;
const TUint8 KGPRSBearer = 0x0B;

//bitmasks
const TUint8 KPortFlagBitMask = 0x40;
const TUint8 KBearerFlagBitMask = 0x80;
const TUint8 KAddressLengthBitMask = 0x3F;

// Constants
_LIT(KReserved, "Reserved");

void CSIAContentHandler::CPushHandlerBase_Reserved1()
	{
	User::Panic(KReserved, KErrNotSupported);
	}

void CSIAContentHandler::CPushHandlerBase_Reserved2()
	{
	User::Panic(KReserved, KErrNotSupported);
	}

/**
 * The SIA App handler private constructor
 * Index number : ESIAContentHandlerIndex 
 */ 
CSIAContentHandler::CSIAContentHandler()
: CContentHandlerBase()
	{
	}

/**
 *  This will complete initialization of the object
 */
void CSIAContentHandler::ConstructL()
	{
	iWapPushUtils = CWapPushMsgUtils::NewL();
	iCommDb = CCommsDatabase::NewL(EDatabaseTypeIAP);
	CActiveScheduler::Add(this);
	}

/**
 * Static Factory Construction
 *
 * Version of NewL which leaves nothing
 * on the cleanup stack
 */
CSIAContentHandler* CSIAContentHandler::NewL()
	{
	CSIAContentHandler* self = new(ELeave) CSIAContentHandler;
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop(self);
	return self;
	}

/**
 * Default d'tor
 */
CSIAContentHandler::~CSIAContentHandler()
	{
	__LOG_PTR_DEBUG("CSIAContentHandler:: Destructor Called");
	delete iWapPushUtils;
	delete iCommDb;
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
void CSIAContentHandler::HandleMessageL(CPushMessage* aPushMsg, TRequestStatus& aStatus)
	{
	__LOG_PTR_DEBUG("CSIAContentHandler:: HandleMessage Async Func. Called");
	__ASSERT_DEBUG( aPushMsg != NULL , User::Panic(KErrPushMsgNull, KErrNone));

	iMessage = aPushMsg;
	iAcknowledge = ETrue;
	SetConfirmationStatus(aStatus);

	iState=EParsing;
	IdleComplete();
	}

/**
 *	Abort handling  
 */
void CSIAContentHandler::CancelHandleMessage()
	{
	__LOG_PTR_DEBUG("CSIAContentHandler:: CancelHandleMessage Called");
	Complete(KErrCancel);
	}

/**
 * HandleMessage Sync. Version
 *	Takes ownership of Push Message and sets self active to continue
 *	processing message.
 *	@param aPushMsg
 *		CPushMessage to process
 */
void CSIAContentHandler::HandleMessageL(CPushMessage* aPushMsg)
	{
	__LOG_PTR_DEBUG("CSIAContentHandler:: HandleMessage Sync Func. Called");
	__ASSERT_DEBUG( aPushMsg != NULL , User::Panic(KErrPushMsgNull, KErrNone));
	
	iAcknowledge = EFalse;
	iMessage = aPushMsg;
	
	iState = EParsing;
	IdleComplete();
	}

/**
 * Stop all actions and quit
 */
void CSIAContentHandler::DoCancel()
	{
	 __LOG_PTR_DEBUG("CSIAContentHandler:: DoCancel Called");
	
#ifdef _ASK_USER_TO_CONNECT
	//cancel notifier if appropriate
	if (iState == EOpeningCOconn && iStatus.Int() == KErrNone)
		 iNotifier.CancelNotifier(KSIADialogNotifierUid);
#endif
	
	Complete(KErrCancel);
	}

/**	
 * Step through the various representative states for handling a message 
 * 1. Initial State - Parsing SIA Push Msg
 * 2. Make a CO connection
 * 3. Delete Content Handler Plugin Owner (i.e self)
 */
void CSIAContentHandler::RunL()
	{
	__LOG_PTR_DEBUG("CSIAContentHandler:: RunL Called");
	// use active state machine routine to manage activites:
	switch(iState)
 		{
	case EParsing:
		ParsePushMsgL();
		break;
	case ERequestingConn:
		RequestConnectionL();
		break;
	case EOpeningCOconn:
		OpenConnectionL();
		break;
	case EDone:
 		Complete(KErrNone);
		break;
	default:
		break;
		}
	}

/**
 *	This is invoked when RunL Leaves with an error.
 *	Cleans up and returns.
 *	@param aError Error passed into this function
 */
TInt CSIAContentHandler::RunError(TInt aError)
	{
	__LOG_PTR_DEBUG("CSIAContentHandler:: RunError Called"); 
	iState=EDone;
	Complete(aError);
	return KErrNone;
	}

/**
 *	Retrieve SIA body, which is in octets, from within CPushMessage,
 *	parse it for a valid connection point.
 *	@leave KErrNotFound no valid connection point located
 *	@leave KErrCorrupt Will leave if the message body contains corrupted data
 */
void CSIAContentHandler::ParsePushMsgL()
	{
	__LOG_PTR_DEBUG("CSIAContentHandler:: ParsePushMsgL Called");
	
	// get contact points list from message
	ObtainContactPointsListL();
	
	// got a ContactList so now need to parse this and obtain a TPushConnPoint
	TBool foundValidConnectionDetails = EFalse;
	TBool moreContactPointsAvailable = ETrue;
	// search iContactList for valid connection Point
	while (!foundValidConnectionDetails && moreContactPointsAvailable)
		{
		if (RetrieveContactPoint())
			foundValidConnectionDetails = ValidContactPointL();
		if (foundValidConnectionDetails == EFalse)
			moreContactPointsAvailable = iContactList.Length() > 1;
		}
	if (!foundValidConnectionDetails)
		{
		__LOG_PTR_DEBUG("CSIAContentHandler: No valid connection point located.");
		User::Leave(KErrNotFound);
		}
	
	// will leave if no connection possible
	iState = ERequestingConn;
	IdleComplete();
	}

/** 
 *	Retrieve contact point details from within SIA content body. This 
 *	method looks up the owned CPushMessage message body and gets a 
 *	pointer to it which it then parses to obtain the contact Points
 *	string only which it sets the iContactList pointer to.
 *	@leave KErrNotFound	Will leave if the contacts list can not be obtained
 *	@leave KErrCorrupt Will leave if the message body contains corrupted data
 */
void CSIAContentHandler::ObtainContactPointsListL()
	{
	__LOG_PTR_DEBUG("CSIAContentHandler::ObtainContactPointsListL Called");
	TPtrC8 rFieldValue;
	TBool gotList = EFalse;
	if (iMessage->GetMessageBody(rFieldValue))
		{
		TPtrC8 messageBodyPtr = rFieldValue;
		TWapBinCodex::TUintvar rMultiByte;
		TUint startpos = 1;
		TWapBinCodex::ExtractUIntvarL(messageBodyPtr, startpos, rMultiByte);
		TUint fieldSize = rMultiByte.iOctetSize;
		TUint appIdFieldLength = rMultiByte.iValue;
		TUint contactPointOffset = fieldSize + appIdFieldLength + startpos;
		// now get Uintvar of ContactPointLen
		TWapBinCodex::ExtractUIntvarL(messageBodyPtr, contactPointOffset, rMultiByte);
		//actual Contact Points field starts after ContactPointLen uintvar
		TUint contactPointLenSize = rMultiByte.iOctetSize;
		TUint contactPointLen = rMultiByte.iValue;
		TUint index = contactPointOffset + contactPointLenSize;
		iContactList.Set(messageBodyPtr.Mid(index, contactPointLen));
		if (iContactList.Length())
			gotList = ETrue;
		}
	
	// failed to parse the contact points list
	if (!gotList)
		{
		__LOG_PTR_DEBUG("CSIAContentHandler: Invalid Message Body - unable to parse.");
		User::Leave(KErrNotFound);
		}
	}

/**
 *	Parse iContactList string for the first Contact Point.
 *	If found sets iContactPoint to retrieved point and
 *	removes this point from iContactList so that any further
 *	calls will start with the next unused Contact Point in the
 *	iContactList.
 *	@return TBool
 *		returns True if found a contact point
 *		returns False if no contact point found
 */
TBool CSIAContentHandler::RetrieveContactPoint()
	{
	__LOG_PTR_DEBUG("CSIAContentHandler:: RetrieveContactPoint Called");
	// ensure have the first byte at least which indicates bearer, port and address
	// length of ContactPoint within list
	if (iContactList.Length() > 1)
		{
		const TInt KFirstByte = 0;
		TUint8 byte = iContactList[KFirstByte];
		
		// check for bit flags settings
		TUint8 bearerFlagPresent = STATIC_CAST(TUint8, byte & KBearerFlagBitMask);
		TUint8 portFlagPresent = STATIC_CAST(TUint8, byte & KPortFlagBitMask);

		// get address length to calculate length of ContactPoint
		TUint8 addressLength = STATIC_CAST(TUint8, byte & KAddressLengthBitMask);

		TInt length = 1;
		if (bearerFlagPresent)
			length++;
		if (portFlagPresent)
			length += 2;
		length += addressLength;

		// check not going to access beyond List
		if (length > iContactList.Length())
			{
			// avoid retrying same retrieval and exiting at this point
			// set list length to zero as no contact points left in it
			const TInt KZero = 0;
			iContactList.Set(iContactList.Left(KZero));
			return EFalse;
			}
		iContactPoint.Set(iContactList.Left(length));
		// if contact list has no further data will be set to length of zero
		TInt pos = length;
		TInt count = iContactList.Length() - pos;
		iContactList.Set(iContactList.Mid(pos, count));
		return ETrue;
		}
	
	return EFalse;
	}

/**
 *	Check the current retrieved Contact Point, contained in
 *	iContactPoint, to check that the connection details are
 *	valid for a connection-oriented Push session. If the
 *	iContactPoint passes the tests iConnPoint is set and 
 *	this value will be used for opening a connection.
 *	@return TBool
 *		Returns True if ContactPoint is valid
 *		retruns False if invalid contact point
 *	@leave CCommsDatabase::OpenViewMatchingTextLC
 *		CommDb methods can Leave - so just propogate
 *		as can't verfiy address and security.
 *	@leave CCommsDbTableView::ReadBoolL
 */
TBool CSIAContentHandler::ValidContactPointL()
	{
	__LOG_PTR_DEBUG("CSIAContentHandler:: ValidContactPointL Called");

	TBool valid = ETrue;
	TUint index = 0;

	// check first bit of first byte to see if bearer Type present 
	TUint8 byte = iContactPoint[index];
	index++;
	// will be zero if flag not set - non-zero overwise
	TUint8 bearerFlagPresent = STATIC_CAST(TUint8, byte & KBearerFlagBitMask);
	TUint8 portFlagPresent = STATIC_CAST(TUint8, byte & KPortFlagBitMask);
	// mask off 1st two bits to get the 6 bits representing address length
	TUint8 addressLength = STATIC_CAST(TUint8, byte & KAddressLengthBitMask);

	//check bearer type
	if (bearerFlagPresent)
		{
		byte = iContactPoint[index];
		index++;
		if ((byte != KCSDBearer) && (byte != KGPRSBearer))
			{
			// unsupported bearer
			__LOG_PTR_DEBUG("CSIAContentHandler: unsupported Bearer Type");
			valid = EFalse;
			return valid;
			}
		}

	//set postion of the first byte of the port
	TUint portStartByte = index; 
	
	// get address
	if (portFlagPresent)
		index += 2;
	TPtrC8 address;
	address.Set(iContactPoint.Mid(index, addressLength));

	//Look up supplied address in commsdb. Leave possible from here on in... 
	TPtrC KTableName(WAP_IP_BEARER);
	TPtrC KColumnName(WAP_GATEWAY_ADDRESS);
	TPtrC KSecurityCol(WAP_SECURITY);
	CCommsDbTableView* table = iCommDb->OpenViewMatchingTextLC(KTableName, KColumnName, address); 
	
	TInt ret = table->GotoFirstRecord();
	if (ret == KErrNone)	//address found
		{
		__LOG_PTR_DEBUG("CSIAContentHandler:: Commdb lookup");

		// set port number, using the following logic:
		// 1. if port number supplied, use it with an unsecure security setting
		// 2. if no port supplied, use the default port for the security setting 
		//    returned by the commsdb address lookup
		TUint16 port;
		if (portFlagPresent)
			{
			TUint8 portNumHighByte = iContactPoint[portStartByte];
			TUint8 portNumlowByte = iContactPoint[portStartByte + 1];
			port = STATIC_CAST(TUint16, ((portNumHighByte<<8)|portNumlowByte));
			}
		else
			{
			TBool secure;
			table->ReadBoolL(KSecurityCol, secure);
			 if (secure)
				port = KPushPortSecure;
			 else
				port = KPushPortUnsecure;
			}

		//set connection point
		iConnPoint.iHost = address;
		iConnPoint.iPort = port;
		}
	else	//address lookup failed
		{
		valid = EFalse;
		__LOG_PTR_DEBUG("CSIAContentHandler:: Commdb lookup failed");
		}
	CleanupStack::PopAndDestroy(); // table

	// will only return true if not SMS and address in CommDB
	return valid;
	}

/**
 *	Start Notifier to inform the user that an SIA request
 *  has been received and a connection-oriented session is
 *	required to be initiated.
 */
void CSIAContentHandler::RequestConnectionL()
	{
	__LOG_PTR_DEBUG("CSIAContentHandler:: RequestConnectionL Called"); 
	
	iNotifierResponse.SetMax();

#ifdef _ASK_USER_TO_CONNECT
	User::LeaveIfError(iNotifier.Connect());
	iNotifier.StartNotifierAndGetResponse(iStatus, KSIADialogNotifierUid, iConnPoint.iHost, iNotifierResponse);
	iState=EOpeningCOconn;
	SetActive();
#else	//always connect case (notifier not started)
	iNotifierResponse = KWapPushCmdConnect;
	iState=EOpeningCOconn;
	IdleComplete();
#endif


	}

/**
 *	If user accepts connection request, call Connection Manager 
 *  with valid Connection Point to initiate connection-oriented 
 *	session. Handler has now finished and closes down leaving 
 *	connection manager to carry on.
 */
void CSIAContentHandler::OpenConnectionL()
	{
	__LOG_PTR_DEBUG("CSIAContentHandler:: OpenConnectionL Called"); 
	
	//open connection if user accepted	
	if ((iStatus.Int() == KErrNone) && (iNotifierResponse == KWapPushCmdConnect))
		iManager->CMOpenConnectionL(iConnPoint);
	
	iState=EDone;
	IdleComplete();
	}
