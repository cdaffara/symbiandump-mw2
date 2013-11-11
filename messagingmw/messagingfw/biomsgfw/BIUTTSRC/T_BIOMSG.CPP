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
// Test harness 
// 
//



#pragma warning( disable : 4100 )

//  Includes...
#include <e32test.h>
#include <bsp.h>
#include <e32uid.h>
#include <msvruids.h>
#include <biouids.h>

//  Global functions..
//  functions()
GLDEF_C void doMainL();
GLDEF_C TInt E32Main();

#define DataComponentFileName _L("c:\\system\\mtm\\cbiomtmu.dat")

//forward reference
class CTestScheduler;
LOCAL_C void DisplayErrorReason(TInt& aReason);

//  Resources..
GLDEF_C RTest test(_L("BIO Test Logger Tester"));
LOCAL_D CTrapCleanup*       myCleanup;
LOCAL_D RFs                 rFs;


    //
    // CExampleScheduler //
    //

class CTestScheduler : public CActiveScheduler
    {
    public:
		void ErrorL(TInt aError) const;
    };

void CTestScheduler::ErrorL(TInt aError) const
    {
    CActiveScheduler::Stop();
    test.Printf(_L("\nLeave signalled, reason=%d\n"),aError);
    test.Getch();
	}
// end CExampleScheduler::Error(TInt) const

LOCAL_D CTestScheduler*		theScheduler;

    //
    // CTestSaveBioMsg //
    //

class CTestSaveBioMsg : public CBase, public MMsvSessionObserver
	{
public:
	static CTestSaveBioMsg* NewL();
	~CTestSaveBioMsg();

	void CreateMessageL();
private:
	// some private functions
	CTestSaveBioMsg();
	void ConstructL();

	// from MMsvSessionObserver
	void HandleSessionEventL(TMsvSessionEvent aEvent, TAny* aArg1, TAny* aArg2, TAny* aArg3);

private:
	// some private members
	};

//
// CSaveBIOMessage create the message entry via client
//
CTestSaveBioMsg* CTestSaveBioMsg::NewL()
	{
	CTestSaveBioMsg* self = new (ELeave) CTestSaveBioMsg();
    CleanupStack::PushL(self);
    self->ConstructL();
    CleanupStack::Pop();
    return self;
	}

CTestSaveBioMsg::~CTestSaveBioMsg()
	{
	}


CTestSaveBioMsg::CTestSaveBioMsg()
	{
	}

void CTestSaveBioMsg::ConstructL()
	{
	TUid mtmDllTypeUids[4];
	TUid smartMtmDllTypeUids[4];
	TInt smartMtmDllEntryPoints[4];

	mtmDllTypeUids[0] = KUidMtmServerComponent;
	mtmDllTypeUids[1] = KUidMtmClientComponent;
	mtmDllTypeUids[2] = KUidMtmUiComponent;
	mtmDllTypeUids[3] = KUidMtmUiDataComponent;

	smartMtmDllTypeUids[0] = KUidBIOMessageServerMtmDll; 
	smartMtmDllTypeUids[1] = KUidBIOMessageClientMtmDll; 
	smartMtmDllTypeUids[2] = KUidBIOMessageUiLayerMtmDll;  //TUid for Bium - UI
	smartMtmDllTypeUids[3] = KUidBIOMessageUiLayerMtmDll;  //				- UI data


	smartMtmDllEntryPoints[0] = 1;
	smartMtmDllEntryPoints[1] = 1;
	smartMtmDllEntryPoints[2] = 1;
	smartMtmDllEntryPoints[3] = 2;

 
	// Make a data component for the text mtm, so the server puts the MTMs into the registry
	CMtmDllInfoArray* mtmdllinfoarray=new(ELeave) CMtmDllInfoArray;
	CleanupStack::PushL(mtmdllinfoarray);

	TBuf<20> name(_S("BIO UI"));
	for (TInt ii=0; ii < KMsvNumMtmDllTypes; ++ii) // There are 4 mtm types
		{
		TUidType mtmUid(KDynamicLibraryUid, mtmDllTypeUids[ii], smartMtmDllTypeUids[ii]);
		TVersion mtmVersion(1, 0, 100);
		CMtmDllInfo* dllInfo = CMtmDllInfo::NewL(name, mtmUid, smartMtmDllEntryPoints[ii], mtmVersion);
		CleanupStack::PushL(dllInfo);
		mtmdllinfoarray->AppendL(dllInfo);
		CleanupStack::Pop();
		}

	CMtmGroupData* mtmgroupdata=CMtmGroupData::NewL(KUidBIOMessageTypeMtm, KNullUid, *mtmdllinfoarray);
	CleanupStack::PushL(mtmgroupdata);

	CFileStore* filestore = CPermanentFileStore::ReplaceLC(rFs,DataComponentFileName,EFileShareExclusive|EFileStream|EFileWrite);
	TUidType uidtype(KPermanentFileStoreLayoutUid, KUidMsvDataComponent, KUidBIOMessageTypeMtm);
	filestore->SetTypeL(uidtype);
	RStoreWriteStream out;
	TStreamId streamid=out.CreateLC(*filestore);							// Push to stack
	mtmgroupdata->ExternalizeL(out);
	out.CommitL();
	CleanupStack::PopAndDestroy(); // out
	filestore->SetRootL(streamid);
	filestore->CommitL();
	CleanupStack::PopAndDestroy(3); // filestore, mtmgroupdata, mtmdllinfoarray

	CMsvSession* msvSession = CMsvSession::OpenSyncL(*this);
	CClientMtmRegistry* mtmReg=CClientMtmRegistry::NewL(*msvSession);
	if (!mtmReg->IsPresent(KUidBIOMessageTypeMtm))
		{
		User::LeaveIfError(msvSession->InstallMtmGroup(DataComponentFileName));
		}

	delete mtmReg;
	delete msvSession;
	}

void CTestSaveBioMsg::HandleSessionEventL(TMsvSessionEvent /*aEvent*/, TAny* /*aArg1*/, TAny* /*aArg2*/, TAny* /*aArg3*/)
	{
	}

void CTestSaveBioMsg::CreateMessageL()
	{
	TUid wibbleParser = {0x1000552F};
	CSaveBIOMessage* saveBioMsg = CSaveBIOMessage::NewL(wibbleParser);
	CleanupStack::PushL(saveBioMsg);

	//	Create and fill a CRichText object for the jobbie..
	CParaFormatLayer* paraFormatLayer = CParaFormatLayer::NewL();
	CleanupStack::PushL(paraFormatLayer);
	CCharFormatLayer* charFormatLayer = CCharFormatLayer::NewL();
	CleanupStack::PushL(charFormatLayer);
	CRichText* richText = CRichText::NewL(paraFormatLayer,charFormatLayer);
	CleanupStack::PushL(richText);

	TInt pos = richText->DocumentLength();		// Ought to be 0
	richText->InsertL(pos,_L("Here's some random cobblers, cobblers wibble hatstand"));
	
	TTime theTime;
	saveBioMsg->CreateMessageL(_L("Bloody bugger test msg"),
		theTime,
		*richText);
	CleanupStack::PopAndDestroy(3); // 

	CleanupStack::PopAndDestroy();
	}



//*****************************************************************************
//
//  Implementation; global stuff
//
//*****************************************************************************
GLDEF_C TInt E32Main()
    {
	__UHEAP_MARK;
	myCleanup = CTrapCleanup::New();
	TRAPD(err,doMainL());		
	test(err==KErrNone);
	delete myCleanup;	
	test.End();
	test.Close();
	__UHEAP_MARKEND;
	return(KErrNone);
    }

GLDEF_C void doMainL()
    {
	// Create an active scheduler for the program session
    theScheduler = new (ELeave) CTestScheduler();
    CleanupStack::PushL(theScheduler);
    CActiveScheduler::Install(theScheduler);
    test.Title();
    test.Printf(_L("\n"));
    test.Start(_L("Starting save BIO msg test harness"));

	// install our mtm
	rFs.Connect();
	
	CTestSaveBioMsg* tester = CTestSaveBioMsg::NewL();
	CleanupStack::PushL(tester);
	tester->CreateMessageL();

	CleanupStack::PopAndDestroy(2);	// theScheduler, tester
    }
