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
// WAP Push - ConnectionManager Test Harness
// 
//

#ifndef __T_PUSHMESSAGE_H__
#define __T_PUSHMESSAGE_H__


#include <msvapi.h>
#include  <pushentry.h>
#include  <push/cmultipartpushmsgentry.h>
#include  <push/cunknownpushmsgentry.h>

#include  <push/csipushmsgentry.h>
#include  <push/cslpushmsgentry.h>
#include  <push/sislpushmsgutils.h>

// Forward references:

class CSIPushMsgEntry;
class CSLPushMsgEntry;
class CUnknownPushMsgEntry;
class CMultipartPushMsgEntry;

const TUint8 KMessageHeader2[] = 
	{
	0xAF,  //Content type "text/vnd.wap.sl"
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
	0x41,   // 'A'
	0x4E,
	0x20,
	0x50,
	0x50,
	0x4C,
	0x49,
	0x43,
	0x41,
	0x54,
	0x49,
	0x4F,
	0x4E, 
	0x20,
	0x46,
	0x4F,
	0x52,
	0x20,
	0x59,
	0x4F,
	0x57
	};

const TUint8 KMessageHeader3[]=
	{
	0xB4,  //Content type "text/vnd.wap.wml"
	0x94,  // Date
	0x04,  // length of field
	0x35,  // Thur 23 Apr 1998 13:41:37 GMT
	0x3F,	
	0x45,
	0x11,
	0xAF,	// X-Wap-Application ID
	0x83	// Id is short integer 3.
	};

//Literals
const TTime KDateExpiry1(_L("19991130:235959.0000"));
const TTime KDateExpiry2(_L("20000101:000000.0000"));
const TTime KDateExpiry3(_L("20000625:235959.0000"));
const TTime KDateExpiry4(_L("20011130:235959.0000"));
const TTime KDateExpiry5(_L("20050610:120912.0001"));
const TTime KDateExpiry6(_L("20000101:001000.0000"));
const TTime KDateExpiry7(_L("20040212:010000.0001"));
const TTime KDateExpiry8(_L("20020901:000000.0000"));
const TTime KDateExpiry9(_L("20030415:200000.0001"));
const TTime KDateExpiry10(_L("20000921:000000.0000"));

_LIT8(KFrom, "Big Joe Bloggs in Peckham");
_LIT8(KDummyBody, "abcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyz1234567890");
_LIT8(KDummyBody2,"afsdaf9vbzc823rASF3safB..asdfm,ca\\k@#3fv-3tqas=`12'as,t,we;'t[!");

_LIT(KUrl1, "http://www.here.com");
_LIT(KUrl2,"mmm://www.bigcorp.com");
_LIT(KUrl3, "http:://www.news.com");
_LIT(KUrl4, "http://www.maybe.com");
_LIT(KUrl5, "http://www.dontknow.com");
_LIT(KUrl6, "http://www.somecompany.com");
_LIT(KUrl7, "http://www.buyit.com");
_LIT(KUrl8, "http://www.lemon.com");
_LIT(KUrl9, "http://www.bigapple.com");
_LIT(KUrl10, "http://www.manypcs.com");
_LIT8(KUrl11, "http://www.zipadee.com");

_LIT(KSiID1, "99228833@global.com");
_LIT(KSiID2, "afhHIN1731@wappo.co.uk");
_LIT(KSiID3, "HU124REQWR9UJAKM@nowhere.com");
_LIT(KSiID4, "AJGAET941054@somewhere.com");
_LIT(KSiID5, "bluragaeh@nowhere.com");
_LIT(KSiID6, "asgdk9815@somecompany.com");
_LIT(KSiID7, "abcdefgig@buyit.com");
_LIT(KSiID8, "q1w2ew3r4@lemon.com");
_LIT(KSiID9, "ft1jjf019@bigapple.com");
_LIT(KSiID10, "afd09@manypcs.com");

_LIT(KUknContent,"image/jpeg" );
_LIT(KMultiContent, "multipart/related");


//
//          CExampleScheduler: INTERFACE        //
//
class CTestScheduler : public CActiveScheduler
	{
public:
	void		Error(TInt anError) const;
	};

//
//          CMessageDetails: INTERFACE          //
//
class CMessageDetails:public CBase
	{
public:
	static CMessageDetails* NewL();
	~CMessageDetails();
	void ResetL();
	TBool	iNullEntries;
	TInt32  iMtm;
	TInt32  iPushMsgType;   //Uid of message type goes in iBioType
	TUint	iStatusFlag;
	TUint	iActionFlag;
	HBufC* 	iSiId;
	TTime	iCreated;
	TTime	iExpires;
	HBufC*  iURL;
	HBufC*  iText;
	HBufC8* iFrom;
	HBufC8* iHeader;
	HBufC*  iContentType;
	HBufC8* iMessageBody;

	HBufC*  iMsgDetails;
	HBufC*  iMsgDescription;
	HBufC*	iFilename;

private:
	void ConstructL();
	CMessageDetails();
	};

	


//
//          CPushEntryTester: INTERFACE          //
//
class CPushEntryTester: public CActive, public MMsvSessionObserver
	{
public:
	
	enum TEntryTestState 
		{
		ETestInitialising,
	    ETestSIGetSet,
		ETestSISaveRetrieve,
		ETestSISaveRetrieveWithStringAppID,
		ETestSISaveRetrieveWithIntAppID,
		ETestSIGetSetNull,
		ETestSISaveRetrieveNull,
		ETestSIChangeEntry,
		ETestSLGetSet,
		ETestSLSaveRetrieve,
		ETestSLGetSetNull,		
		ETestSLSaveRetrieveNull,
		ETestSLChangeEntry,
		ETestMultiGetSet,
		ETestMultiSaveRetrieve,
		ETestMultiGetSetNull,
		ETestMultiSaveRetrieveNull,
		ETestUnknownGetSet,
		ETestUnknownSaveRetrieve,
		ETestUnknownSaveRetrieveWithFilename,
		ETestUnknownGetSetNull,
		ETestUnknownSaveRetrieveNull,
		ETestUnknownSaveRetrieveNullWithFilename,
		ETestUnknownChangeEntry,
		ETestUnknownChangeEntryWithFilename,
		ETestSIFindSiId,
		ETestSIFindSiIdNull,
		ETestSIFindSiUrl,
		ETestSIFindSiUrlNull,
		ETestSLFindUrl,
		ETestSLFindUrlNull,
		ETestSIGetHiAction,
		ETestSLGetHiAction,
		ETestGetExpiry,
		ETestGetNextExpiry,
		ETestComplete
	};
	static CPushEntryTester* NewL(MWapPushLog& aLog);
	~CPushEntryTester();

	void HandleSessionEventL(TMsvSessionEvent /*aEvent*/, TAny* /*aArg1*/, TAny* /*aArg2*/, TAny* /*aArg3*/) {}
	void StartL();
	inline void SetPushServiceId(TMsvId aId) {iPushServiceId = aId;}
	inline void SetPushFolderId(TMsvId aId) {iPushMsgFolderId = aId;}
	


private:
	CPushEntryTester(MWapPushLog& aLog);
	void ConstructL();

	void RunL();
	void DoCancel();
	void QueueOperationAsync(TInt aErr);
	TInt RunError(TInt aError);


	void CreateMsvMembersL();
	void CreateHeaders();
	void CreateMsgDetailsL(TBool aNotNullFields);
	void CreateMoreSiMessagesL();
	void CreateMoreSLMessagesL();

	
	void OutputTMsvEntryDetailsL(const TMsvEntry& aEntry);
	void OutputPushMsgDetailsL(const CPushMsgEntryBase& aSIEntry);

	//Overloaded output functions
	void OutputMessageDetailsL(CSIPushMsgEntry& aSIEntry);
	void OutputMessageDetailsL(CSLPushMsgEntry& aSLEntry);
	void OutputMessageDetailsL(CUnknownPushMsgEntry& aUnknownEntry);
	void OutputMessageDetailsL(CMultiPartPushMsgEntry& aMultiEntry);

	// Following tests get/Set, and Save/Retrieve with/without heap testing. 
	void SiGetSetTestL();
	void SiSaveRetrieveTestL();
	// Following used for Get/Set and Save/Retrieve operations
	CSIPushMsgEntry* DoSiCreateTestL();
	void DoSiSetTestL(CSIPushMsgEntry& aSiEntry);
	CSIPushMsgEntry* DoSiSaveTestL();
	CSIPushMsgEntry* DoSiRetrieveTestL(TMsvId aMsgId);
	
	// Following tests get/Set, and Save/Retrieve with/without heap testing. 
	void SlGetSetTestL();
	void SlSaveRetrieveTestL();

	CSLPushMsgEntry*  DoSlCreateTestL();
	void DoSlSetTestL(CSLPushMsgEntry& aSlEntry);
	CSLPushMsgEntry* DoSlSaveTestL();
	CSLPushMsgEntry* DoSlRetrieveTestL(TMsvId aId);


	void MultiGetSetTestL();
	void MultiSaveRetrieveTestL();
	CMultiPartPushMsgEntry* DoMultiCreateTestL();
	void DoMultiSetTestL(CMultiPartPushMsgEntry& aMultiEntry);
	CMultiPartPushMsgEntry* DoMultiSaveTestL();
	CMultiPartPushMsgEntry*  DoMultiRetrieveTestL(TMsvId aId);

	
	void UnknownGetSetTestL();
	void UnknownSaveRetrieveTestL(TBool aFilename);
	CUnknownPushMsgEntry* DoUnknownCreateTestL();
	void DoUnknownSetTestL(CUnknownPushMsgEntry& aUnknownEntry);
	CUnknownPushMsgEntry* DoUnknownSaveTestL(TBool aFilename);
	CUnknownPushMsgEntry* DoUnknownRetrieveTestL(TMsvId aId);

	void ChangeSIEntryTestL();
	void DoChangeSiEntryL(CSIPushMsgEntry& aSiMsg);
	void ChangeSLEntryTestL();
	void DoChangeSlEntryL(CSLPushMsgEntry& aSlMsg);
	void ChangeUnknownEntryTestL(TBool aFilename);
	void DoChangeUnknownEntryL(CUnknownPushMsgEntry& aUnkMsg, TBool aFilename);


	void FindSiIdTestL(TBool aSearchForNullStr);
	void FindUrlTestL(TBool aSearchForNullStr,  TUid aUid);
	void GetActionTestL(TUid aPushType);
	void GetExpiryTestL();
	void DeleteEarliestExpiryL();

	void DumpPushMessageDetailsL(TUid aPushType);
	void CreateMultiChildrenL(TMsvId aEntry);

	//Logging functions
	void LogStr(const TDesC& aDes);
	void LogFormatInt(const TDesC& aFormat, TInt aInt);
	void LogFormatInt(const TDesC& aFormat, TInt aInt1, TInt aInt2);
	void LogFormatStr(const TDesC& aFormat, const TDesC& aDes);

	TBuf8<40> iDummyHeader1;
	TBuf8<40> iDummyHeader2;
	CSISLPushMsgUtils* iSISLPushUtilities;
	CWapPushMsgUtils* iPushUtilities;
	CMessageDetails* iDetails;
	CPushMsgEntryBase* iPushEntry;
	CMsvSession* iMsvSession;
	CMsvEntry*	 iMsvEntry;
	TInt		iState;
	TMsvId iPushServiceId;
	TMsvId iPushMsgFolderId;
	// Following hold Ids so that the success 
	// of find operations can be determined
	TMsvId iTargetSiIDMsg;
	TMsvId iTargetSIUrlMsg;
	TMsvId iTargetSLUrlMsg;
	TMsvId iHighestSIActionID;
	TMsvId iHighestSLActionID;
	TMsvId iSINullMsg;
	TMsvId iSLNullMsg;
	// These hold ids of the first of each type of message
	// Used to locate the records to test update
	TMsvId iSIToChange;
	TMsvId iSLToChange;
	TMsvId iUnknownToChange;
	// Used to hold the id of a message to delete 
	// - tests delete & find expiry again
	TMsvId iExpired;
	TInt   iFailures;
	MWapPushLog& iLog;
	TBuf<256> iFormatBuffer;
	};


#endif
