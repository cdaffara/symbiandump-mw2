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

#include "pushtests.h"
#include "dummywapstack.h"
#include "wappushmsggen.h"
#include <push/cwappushmsgutils.h>
#include <commdb.h>



/** set test name to 'Service Initiation Application'
 *	@param void
 *	@return string
 *		string represents test name
 */
const TDesC& CWapPushSIAMessageTest::TestName()
	{
	_LIT(KTextSIAMessageTest, "Service Intiation Application");
	return KTextSIAMessageTest;
	}

/** 
 *	set the number of test messages to use 
 */
void CWapPushSIAMessageTest::NumberTestCasesToRun()
	{
	iNumberOfTestCases = 3;
	}

/** build different test messages to test specific SIA
 *	handling of messages.
 *	@param TInt
 *		the number of test messages to create and use
 *	@return void
 */
void CWapPushSIAMessageTest::PrepareTestMessageL(TInt aTestCase)
	{
	static TBool setup = ETrue;
	if (setup)
		{
		SetupAddressL();
		setup = EFalse;
		}

	// this set of test messages adds a test message
	const TUint8 KSIAMessageHeader[] = 
		{
		0xB4,	//Content type "application/vnd.wap.sia" 
		0x92,  // Date
		0x04,  // length of field
		0x39,  // Monday 15 August 2000 10:41:37 GMT
		0x90,	
		0x6a,
		0xd0,
		0x94,  // Expires 
		0x04,  // length of field
		0x39,  // Monday 15 August 2000 10:41:37 GMT
		0x9a,	
		0x6a,
		0xd0,
		0xAF,  //X-Wap-Application-ID
		0x82  // Short int - 0x02 represent x-wap-application:push.ua
		};	
	iHeadersBuf.Append(KSIAMessageHeader, sizeof(KSIAMessageHeader));
	
	iBodyBuf.Delete(0,KBufferSize);
	//iBodyBuf.Zero();

	// NOTE: tests for valid address require address in COMMDB
	
	switch (aTestCase)
		{
	case 0 :	// normal SIA message with single valid address 
		{
		const TUint8 SIA[]=
			{
			0x00,		//	version
			0x02,		//	AppIdListLen
			0x44,		//	Application Id List 
			0x44,			
			0x1B,		// ContactPointsLen
			// Contact Points
			0xD7,		//	bearer flag and port flag and address len 
			0x0A,		//	bearer Type (CSD)
			0x0B,		//	port number
			0x84,		//		= 2948
			0x68,		// address - "http://www.symbian.com/" (as text)
			0x74,		
			0x74,
			0x70,
			0x3A,
			0x2F,
			0x2F,
			0x77,		
			0x77,		
			0x77,
			0x2E,
			0x73,
			0x79,
			0x6D,
			0x62,
			0x69,
			0x61,
			0x6E,
			0x2E,
			0x63,
			0x6F,
			0x6D,
			0x2F
			};
		iBodyBuf.Append(SIA, sizeof(SIA));
		}
		break;
	case 1 :	// normal SIA message but no port or bearer ( valid )
		{
		const TUint8 SIA[]=
			{
			0x00,		//	version
			0x02,		//	AppIdListLen
			0x44,		//	Application Id List 
			0x44,			
			0x18,		// ContactPointsLen
			// Contact Points
			0x17,		//	 no bearer flag or port flag only address len 
						// no bearer or port specified
			0x68,		// address - "http://www.symbian.com/" (as text)
			0x74,		
			0x74,
			0x70,
			0x3A,
			0x2F,
			0x2F,
			0x77,		
			0x77,		
			0x77,
			0x2E,
			0x73,
			0x79,
			0x6D,
			0x62,
			0x69,
			0x61,
			0x6E,
			0x2E,
			0x63,
			0x6F,
			0x6D,
			0x2F
			};
		iBodyBuf.Append(SIA, sizeof(SIA));
		}
		break;	
	case 2 :	// SIA message with no contact points specified (error)
		{
		const TUint8 SIA[]=
			{
			0x00,		//	version
			0x02,		//	AppIdListLen
			0x44,		//	Application Id List 
			0x44,			
			0x00		// ContactPointsLen == 0
					// No Contact Points
			};
		iBodyBuf.Append(SIA, sizeof(SIA));
		}
		break;
	case 3 :	// normal SIA message with three addresses - 3rd valid	
		break;
	case 4 :	// SIA message with incorrect contact point (error)
		break;
	default: 
		// do nothing
		break;
		}
	
	}

/** confirm the message has been stored.
 *	Writes the number of stored messages found to the log file - if present.
 *	@param void
 *	@return TBool
 *		returns ETrue if messages found
 *		returns EFalse if no messages found 
 */
TBool CWapPushSIAMessageTest::ConfirmMessagesSavedL()
	{
	// no messages to save....
	return EFalse;
	}

/**
 *	Prime the commdb with address and security port settings.
 *	
 */
void CWapPushSIAMessageTest::SetupAddressL()
	{
	CCommsDatabase *db = CCommsDatabase::NewL(EDatabaseTypeUnspecified);
	CleanupStack::PushL(db);
	
	CCommsDbTableView* waptable;
	
	TPtrC tableName(WAP_IP_BEARER);	 
	TPtrC columnName(WAP_GATEWAY_ADDRESS);
//	TPtrC SecurityCol(WAP_SECURITY);
	_LIT(address, "http://www.symbian.com/");
	
	TBuf<30> KTestAddress(address);
	//TBool secure = ETrue;

	waptable = db->OpenTableLC(tableName);
	
	TUint32 recordId;
	TInt ret;
	ret = waptable->InsertRecord(recordId);
	waptable->WriteUintL(TPtrC(WAP_ACCESS_POINT_ID),recordId);
	if (ret == KErrNone)
		{
		// void WriteTextL(const TDesC& aColumn, const TDesC& aValue);
		waptable->WriteTextL(columnName, KTestAddress);
		// void WriteBoolL(const TDesC& aColumn, const TBool& aValue);

		// leave Security field unset - shd be NULL ? (= EFalse)

		// TInt PutRecordChanges(TBool aHidden = EFalse, TBool aReadOnly = EFalse);
		waptable->PutRecordChanges();
		}

	CleanupStack::PopAndDestroy(2); // db, view
	}
