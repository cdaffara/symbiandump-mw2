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
// TESTWAPP.H
// Test Enviroment for WAP Access Point Settings parser.
// Started by Sean Jesson of Teleca Ltd. November 1999.
// Updated by Andrew Fawcett, Teleca Ltd Feb 2000
// 
//


#if !defined (__TESTWAPP_H__)
#define __TESTWAPP_H__

#ifndef _TESTWAPP_H_
#define _TESTWAPP_H_

#include <e32std.h>
#include <e32test.h>
#include <c32comm.h>
#include <in_sock.h>

#include <commsdattypesv1_1.h>
#include <commsdat.h>
using namespace CommsDat;

extern RTest Test;
//extern RSocketServ SocketServer;
//extern RHeap* TestHeap;

#define BIO_MSG_ENTRY_PARSED     1 // Set entry.iMtmData3 to 1, indicates entry has store i.e parsed & externalised

const TUint	KWappModemInitValue  = (99999);

//Some string defines
_LIT(KBIOWAPPrefix, "WAP*");	// prefix for WAP message
const  TPtrC KWappTestTxtFileDir = _L("c:\\test\\bio\\wapp\\");//Source files dir
#define KSmsFileName        _L("c:\\Mailtest\\Wapptest\\Wapp.txt")
const  TPtrC KWappLogFile = _L("C:\\Logs\\Bio\\Wap.log");	 // Test log file 
const  TPtrC KFileName   = _L("Wap.dat");
const  TPtrC KAttachmentName	= _L("Wappbmp.cng"); 

#define KTestWappPriority   (EPriorityStandard)    //Change this....

// Panic errors:
const TInt KWappNullPtr		      = (111);
const TInt KBioMsgNotParsed		  = (222);
const TInt KBioMsgEntryHasNoStore = (333);
const TInt KBioMsgNotProcessed    = (444);

//const TUid KUidMsvMessageDriveStream = {0x1000163E};
const TUid  KUIDMsvBIODataStream     = {0x10005268};

const TPtrC KWappToken	= _L("WAP Access Point");		// A script header.
const TPtrC KWappTab	= _L("\t");	
const TPtrC KWappLinefeed16 = _L("\n");	
const TPtrC KWappParserName = _L("WAP Access Point Parser");	// A human readable name.
const TVersion KVer1(0,0,1);							// A version number.

//
//				Forward references:			   //
//
class TWappBearerType;


//
//				Global declarations:			//
//
LOCAL_D RTest test(_L("WAPP Test Harness"));

void CommInitL(TBool aEnhanced);


//
// Class definitions: //
//

//
// CExampleScheduler: INTERFACE //
//
class CTestScheduler : public CActiveScheduler
	{
public:
	void		Error(TInt anError) const;
	};


//
//			CTestWapp: INTERFACE			//	
//

class CTestWapp : public CActive
    {
	public:
        enum TSessionState
            {
			EWappClearCommDB,
			EWappCreateNewMessages, // Creating messages in Global Inbox
			EWappCreateParser,		// Create a parser set to next message entry 
			EWappProcessUnparsed,	// Call ProcessL for an unparsed message - should fail!
            EWappParse,				// parse the message
			EWappParseAgain,		// parse it a second time
            EWappProcess,			// process the parsed fields of the message
			EWappProcessAgain,		// process a second time
			EWappDisplay,			// Display entry data.
			EWappComplete			// wrap-up everything
            };

		enum TTestMode
			{
			ETWappConcise,   // Minimal info
			ETWappDetailed,  // Logs Parsed Fields
			ETWappVerbose	 // Logs everything parsed fields, and commdb entries
			};

		enum TTestWappBearerType
			{
			ETestWappUnknownBearer,
			ETestWappGsmCsdBearer,
			ETestWappGsmSmsBearer,
			ETestWappGsmUssdBearer,
			ETestWappIS136CsdBearer,
			ETestWappGprsBearer
			};

    public:
        ~CTestWapp();
        CTestWapp(RFs& aFs);
        static CTestWapp* NewL(RFs& aFs);
        void ConstructL();
        void StartL();
        void RunL();
        TInt RunError(TInt aError);
        void DoCancel();
		void RequestComplete(TRequestStatus& aStatus,TInt aCompletion);

	private:
		void RunAutoTest();
		void ShowMenuL();
		void CreateParserL();
		void QueueOperationAsync(TInt aErr);    // Queues the parser on the AO list
		void InitialiseTesterL();				// Initialises test harness. 
		void DoRunL();
		void CreateFilenameArrayL();			// Creates an array with names of test files
		void GenerateNextMessageL();			// Creates a dummy BIO message
		void UnloadParser();
		void ExtractBodyPartsL();	
		void ParseMessageL();
		void ProcessMessageL();
		void ProcessUnparsedMessageL();
		void RecordEntryDataL();
		void LogParsedDataL();
		void SetEntryNameL();
		
		void DisplayTestSummaryL();
		void DisplayBookmarksL();
		CMDBRecordSetBase* GetGenericPointerL(TMDBElementId aRecordElementId); //Returns a RecordSetBase
																		// Based on the supplied element id
		void DisplayCommDatTablesL();												
		TInt DisplayCommDatWapApTableL(CMDBRecordSetBase& aGenericRecBase);		
		TInt DisplayCommDatWapIpTableL(CMDBRecordSetBase& aGenericRecBase, TDesC& aEntryName, TUint32 aIapId);		
		TInt DisplayCommDatWapSmsTableL(CMDBRecordSetBase& aGenericRecBase);
		TInt DisplayCommDatDialOutISPTableL(CMDBRecordSetBase& aGenericRecBase, TDesC& aEntryName);
		TInt DisplayCommDatIAPTableL(CMDBRecordSetBase& aGenericRecBase, TDesC& aEntryName, TTestWappBearerType aBearerType);
		//void DisplayCommDatModemTableL(CCDModemBearerRecord& aModemBearerRecord, TInt aRecCount);
		TInt DisplayCommDatGprsTableL(CMDBRecordSetBase& aGenericRecBase, TDesC& aEntryName);		
		void DisplayErrorReasonL(TInt& aReason);
		void DisplayMMSURLL();
		
		TBool IsLineTermination(TChar aChar);
		void OutputTextListL( const TDesC& aFieldName, const TDesC& aValueName);
		void OutputTextListL( const TDesC& aFieldName, TUint aValue);
		void OutputFormattedTextL(TRefByValue<const TDesC> aFmt,...);
		void PrintDescriptor(const TDesC& aBuffer);

		void DeleteCommDbTableL(); //trashes all entries in the WAP, Dial In/Out ISP, IAP tables


    private:
		RFs&						iFs;			// Our file server session
        TInt						iState;			// Holds the current state of the test harness.
		CMsvEntrySelection*			iMsvSelection;  // TMsvIds of our messages.
		TMsvId						iEntryId;		// ID of sms entry.
      
		TInt						iParsedFieldsNum;
		TInt						iTestSuccesses;
		TInt						iTestFailures;
		TInt						iTestCounter;
		TInt						iMessagesCreated;
		TInt						iFilesProcessed;
		TTestMode					iTestMode;  // Holds logging mode

        CBaseScriptParser2*			iWappParser;

        HBufC*						iMessageBody;
		CBioTestUtils*				iBioTestUtils;  // Makes messages, etc.
		TInt						iNumWapEntries; //No of entries in  WAP table
		HBufC*						iCurrentSettingsName;  // Name of the current settings 
		TUint32						iCurrentIspID;
		TUint32						iCurrentIapID;
		TUint32						iWAPModemID;
		TUint32						iWapAccessPointID;  //ID of WAP_ACCESS_POINT entry 

		CCommsDatabase*				iTestCommDb;
		TUint32						iGsmCsdIapId;
		TUint32						iGprsIapId;
	   };


#endif

#endif // __TESTWAPP_H__
