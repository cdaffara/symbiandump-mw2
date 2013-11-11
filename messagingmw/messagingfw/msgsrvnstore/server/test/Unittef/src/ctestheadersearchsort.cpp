// Copyright (c) 2008-2009 Nokia Corporation and/or its subsidiary(-ies).
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

#include "ctestheadersearchsort.h"
#include <msvapi.h>
#include <msvsearchsortquery.h>
#include "msvsearchsortoponheaderbody.h"
#include <msvids.h>
#include <msvuids.h>
#include <msvapi.h>
#include <mtclreg.h>
#include <mtclbase.h>
#include <mtmdef.h>
#include <txtrich.h>
#include "emailtestutils.h"
#include "msvtestutils.h"
#include <miuthdr.h>
#include <e32base.h>



#define MY_ASSERT_TRUE(a)  \
		INFO_PRINTF1(KFunction);   \
		ASSERT_TRUE(a);
#define MY_ASSERT_FALSE(a)  \
		INFO_PRINTF1(KFunction);   \
		ASSERT_FALSE(a);
#define MY_ASSERT_EQUALS(a,b)  \
		INFO_PRINTF1(KFunction);   \
		ASSERT_EQUALS(a,b);



//const TUid KUidFindTestMtm       = {0x10005126};
const TUid KUidFindTestPOP3Mtm = {0x10001029};
//const TUid KUidFindTestSMTPMtm = {0x10001028};
//const TUid KUidFindTestIMAP4Mtm = {0x1000102A};




_LIT(KMiutSubject1, "short mail about something interesting");
_LIT(KMiutSubject2, "Fwd : Why men lie and woman cry");
_LIT(KMiutSubject3, "Fwd : Intere");


_LIT8(KMiutMsgId, "0123456789AB.CdeFGHIj@symbian.com");

_LIT16(KMiutFrom1, "ashok@symbian.commmm");
_LIT16(KMiutFrom2, "yatish@symbian.com");
_LIT(KMiutFrom3, "someone@symbian.com");


_LIT(KMiutReplyTo, "yatish@symbian.com");


_LIT(KMiutToRecipients1, "ashok@symbian.com");
_LIT(KMiutToRecipients2, "yatish@address.com");
_LIT(KMiutToRecipients3, "Ashok@address.com");
_LIT(KMiutToRecipients4, "AshokKumar@address.com"); 
_LIT(KMiutToRecipients5, "ashokkumar@address.com");
_LIT(KMiutToRecipients6, "symbianrocks@address.com");


_LIT(KMiutCcRecipients1, "channa@address.com");
_LIT(KMiutCcRecipients2, "sukanta@address.com");
_LIT(KMiutCcRecipients3, "kavita_khatawate@address.com");
_LIT(KMiutCcRecipients4, "sukantabanik@address.com");
_LIT(KMiutCcRecipients5, "Sukantabanik@address.com");

_LIT(KMiutBccRecipients1, "bcc1@address.com");
_LIT(KMiutBccRecipients2, "bcc2@address.com");
_LIT(KMiutBccRecipients3, "bcc3@address.com");
_LIT(KMiutBccRecipients4, "bcc4@address.com");
_LIT(KMiutBccRecipients5, "bcc5@address.com");
_LIT(KMiutBccRecipients6, "bcc6@address.com");



_LIT8(KMiutResentMsgId, "0123456789AB.CdeFGHIj@symbian.com");
_LIT(KMiutResentFrom, "test@psion6.demon.co.uk");
_LIT(KMiutResentToRecipients1, "recipient1@address.com");
_LIT(KMiutResentToRecipients2, "recipient2@address.com");
_LIT(KMiutResentCcRecipients1, "cc1@address.com");
_LIT(KMiutResentCcRecipients2, "cc2@address.com");
_LIT(KMiutResentBccRecipients, "bcc@address.com");


#define KMiutRemoteSize 					123456
#define KMaxImHeaderStringLengthLong		KMaxImHeaderStringLength+1


LOCAL_D TBuf<KMaxImHeaderStringLengthLong> longValue;
LOCAL_D TBuf8<KMaxImHeaderStringLengthLong> longValue2;

RTest test(_L("Find Test"));
LOCAL_D CEmailTestUtils* testUtils;

//Database
_LIT(KTestDbFileName, "c:\\messaging.db");

/*
LOCAL_C void InitL()
	{
	testUtils = CEmailTestUtils::NewLC(test);
	testUtils->CreateAllTestDirectories();
	testUtils->FileSession().SetSessionPath(_L("C:\\"));
	testUtils->CleanMessageFolderL();
		
	}
	
LOCAL_C void Closedown()
	{
	test.Close();
	CleanupStack::PopAndDestroy(1);  //testUtils
	}*/


CTestHeaderSearchSortOperation::CTestHeaderSearchSortOperation()
	{
	}

CTestHeaderSearchSortOperation ::~CTestHeaderSearchSortOperation()
	{
	
	}


void CTestHeaderSearchSortOperation::SetupL()
	{
	_LIT(KSetupL, "SetupL");
	INFO_PRINTF1(KSetupL);
	
	iActiveWaiter = new (ELeave) CActiveWaiter();
	
	}




void CTestHeaderSearchSortOperation::SaveMessageL(CMsvSession& aSession, TMsvEntry aEntry, TBool aRichText)
	{

	CClientMtmRegistry* clientRegistry = CClientMtmRegistry::NewL(aSession);
	CleanupStack::PushL(clientRegistry);

	CBaseMtm* baseMtm = clientRegistry->NewMtmL(aEntry.iMtm);
	CleanupStack::PushL(baseMtm);

	// SetCurrentEntryL takes ownership of this
	CMsvEntry* entry = CMsvEntry::NewL(aSession, aEntry.Id(), TMsvSelectionOrdering());
	baseMtm->SetCurrentEntryL(entry);
	if (aRichText)
		baseMtm->Body().InsertL(0, _L("Some Rich Text"));
	baseMtm->SaveMessageL();

	CleanupStack::PopAndDestroy(2); // baseMtm, clientRegistry
	}
	
	

	
LOCAL_C void CreateMessageHeaderL(CImHeader& imHeader)
	{
	// a bunch of data for the header object....
	imHeader.Reset();
	imHeader.SetSubjectL(KMiutSubject1);

	imHeader.SetImMsgIdL(KMiutMsgId);

	imHeader.SetFromL(KMiutFrom1);
	imHeader.SetFromL(KMiutFrom2);
	imHeader.SetReplyToL(KMiutReplyTo);
	imHeader.SetRemoteSize(KMiutRemoteSize);

	imHeader.ToRecipients().AppendL(KMiutToRecipients1);
	imHeader.ToRecipients().AppendL(KMiutToRecipients2);
	imHeader.ToRecipients().AppendL(KMiutToRecipients3);
	imHeader.ToRecipients().AppendL(KMiutToRecipients4);
	imHeader.CcRecipients().AppendL(KMiutCcRecipients1);
	imHeader.CcRecipients().AppendL(KMiutCcRecipients2);
	imHeader.CcRecipients().AppendL(KMiutCcRecipients3);
	imHeader.CcRecipients().AppendL(KMiutCcRecipients4);
	imHeader.BccRecipients().AppendL(KMiutBccRecipients1);
	imHeader.BccRecipients().AppendL(KMiutBccRecipients2);
	}

LOCAL_C void CreateMessageHeader2L(CImHeader& imHeader)
	{
	// a bunch of data for the header object....
	imHeader.Reset();
	imHeader.SetSubjectL(KMiutSubject2);

	imHeader.SetImMsgIdL(KMiutMsgId);

	imHeader.SetFromL(KMiutFrom1);
	imHeader.SetReplyToL(KMiutReplyTo);
	imHeader.SetRemoteSize(KMiutRemoteSize);

	imHeader.ToRecipients().AppendL(KMiutToRecipients3);
	imHeader.ToRecipients().AppendL(KMiutToRecipients4);
	imHeader.ToRecipients().AppendL(KMiutToRecipients5);
	imHeader.CcRecipients().AppendL(KMiutCcRecipients3);
	imHeader.CcRecipients().AppendL(KMiutCcRecipients4);
	imHeader.BccRecipients().AppendL(KMiutBccRecipients3);
	imHeader.BccRecipients().AppendL(KMiutBccRecipients4);
	
	imHeader.SetResentMsgIdL(KMiutResentMsgId);
	imHeader.SetResentFromL(KMiutResentFrom);
	imHeader.ResentToRecipients().AppendL(KMiutResentToRecipients1);
	imHeader.ResentToRecipients().AppendL(KMiutResentToRecipients2);
	imHeader.ResentCcRecipients().AppendL(KMiutResentCcRecipients1);
	imHeader.ResentCcRecipients().AppendL(KMiutResentCcRecipients2);
	imHeader.ResentBccRecipients().AppendL(KMiutResentBccRecipients);
	}
	
	
LOCAL_C void CreateMessageHeader3L(CImHeader& imHeader)
	{
	// a bunch of data for the header object....
	imHeader.Reset();
	imHeader.SetSubjectL(KMiutSubject3);

	imHeader.SetImMsgIdL(KMiutMsgId);

	imHeader.SetFromL(KMiutFrom3);
	imHeader.SetReplyToL(KMiutReplyTo);
	imHeader.SetRemoteSize(KMiutRemoteSize);

	imHeader.ToRecipients().AppendL(KMiutToRecipients4);
	imHeader.ToRecipients().AppendL(KMiutToRecipients5);
	imHeader.ToRecipients().AppendL(KMiutToRecipients6);
	imHeader.CcRecipients().AppendL(KMiutCcRecipients4);
	imHeader.CcRecipients().AppendL(KMiutCcRecipients5);
	imHeader.BccRecipients().AppendL(KMiutBccRecipients5);
	imHeader.BccRecipients().AppendL(KMiutBccRecipients6);
	
	imHeader.SetResentMsgIdL(KMiutResentMsgId);
	imHeader.SetResentFromL(KMiutResentFrom);
	imHeader.ResentToRecipients().AppendL(KMiutResentToRecipients1);
	imHeader.ResentToRecipients().AppendL(KMiutResentToRecipients2);
	imHeader.ResentCcRecipients().AppendL(KMiutResentCcRecipients1);
	imHeader.ResentCcRecipients().AppendL(KMiutResentCcRecipients2);
	imHeader.ResentBccRecipients().AppendL(KMiutResentBccRecipients);
	}

	
void CTestHeaderSearchSortOperation::CreateMessagesL(TUid aMtm)
	{
	_LIT(KName, "TEST");
	HBufC* data = HBufC::NewL(15); 
	*data = KName;
	
	
	
	CParaFormatLayer* paraLayer = CParaFormatLayer::NewL();
	CleanupStack::PushL(paraLayer);
	CCharFormatLayer* charLayer = CCharFormatLayer::NewL();
	CleanupStack::PushL(charLayer);

	// create entry to work under
	TMsvEntry folder;
	folder.iType = KUidMsvFolderEntry;
	folder.iMtm = KUidMsvLocalServiceMtm;
	folder.iServiceId = KMsvLocalServiceIndexEntryId;
	folder.iDescription.Set(data->Des());
	folder.iDetails.Set(data->Des());
	testUtils->SetEntryL(KMsvGlobalInBoxIndexEntryId);
	testUtils->iMsvEntry->CreateL(folder);
	testUtils->iMsvEntry->SetEntryL(folder.Id());
	//folderId=folder.Id();

	TMsvEntry entry1;
	entry1.iType = KUidMsvMessageEntry;
	entry1.iServiceId = KMsvLocalServiceIndexEntryId;
	entry1.iMtm = aMtm;
	entry1.iDescription.Set(data->Des());
	entry1.iDetails.Set(data->Des());
	testUtils->iMsvEntry->SetEntryL(KMsvGlobalInBoxIndexEntryId);
	testUtils->iMsvEntry->CreateL(entry1);
	TRAPD(error,testUtils->iMsvEntry->SetEntryL(entry1.Id()));
	//entry1Id=entry1.Id();
	iSelectionArray.AppendL(entry1.Id());


	CMsvStore* fileStore = testUtils->iMsvEntry->EditStoreL();
	CleanupStack::PushL(fileStore);

	CImHeader* header = CImHeader::NewLC(); // Cleaned up when exiting this menu selection
	CreateMessageHeaderL(*header);
	header->StoreL(*fileStore);
	fileStore->CommitL();
	CleanupStack::PopAndDestroy(2); // filestore, header

	TMsvEmailEntry ementry1;
	ementry1.iMtm = aMtm;
	ementry1.iServiceId = testUtils->iMsvEntry->Entry().iServiceId;
	ementry1.iType = KUidMsvEmailTextEntry;
	ementry1.iDescription.Set(data->Des());
	ementry1.iDetails.Set(data->Des());
	testUtils->iMsvEntry->CreateL(ementry1);
	TRAP(error,testUtils->iMsvEntry->SetEntryL(ementry1.Id()));

	fileStore=NULL;
	fileStore = testUtils->iMsvEntry->EditStoreL();
	CleanupStack::PushL(fileStore);
	
	CRichText* text1=CRichText::NewL(paraLayer, charLayer);
	CleanupStack::PushL(text1);
	text1->InsertL(0,_L("This is the body of first mail which has something"));
	fileStore->StoreBodyTextL(*text1);
	fileStore->CommitL();
	CleanupStack::PopAndDestroy(2); // filestore


	fileStore=NULL;
	TMsvEntry entry2;
	entry2.iType = KUidMsvMessageEntry;
	entry2.iMtm = aMtm;
	entry2.iServiceId = KMsvLocalServiceIndexEntryId;
	entry2.iDescription.Set(data->Des());
	entry2.iDetails.Set(data->Des());
	testUtils->iMsvEntry->SetEntryL(KMsvGlobalInBoxIndexEntryId);
	testUtils->iMsvEntry->CreateL(entry2);
	TRAP(error,testUtils->iMsvEntry->SetEntryL(entry2.Id()));
	//entry2Id=entry2.Id();
	iSelectionArray.AppendL(entry2.Id());
	
	fileStore = testUtils->iMsvEntry->EditStoreL();
	CleanupStack::PushL(fileStore);
	CImHeader* header1 = CImHeader::NewLC(); 
	CreateMessageHeader2L(*header1);
	header1->StoreL(*fileStore);
	fileStore->CommitL();
	CleanupStack::PopAndDestroy(2); // filestore,header


	TMsvEmailEntry ementry2;
	ementry2.iMtm = aMtm;
	ementry2.iServiceId = testUtils->iMsvEntry->Entry().iServiceId;
	ementry2.iType = KUidMsvEmailTextEntry;
	ementry2.iDescription.Set(data->Des());
	ementry2.iDetails.Set(data->Des());
	testUtils->iMsvEntry->CreateL(ementry2);
	TRAP(error,testUtils->iMsvEntry->SetEntryL(ementry2.Id()));

	fileStore=NULL;
	fileStore = testUtils->iMsvEntry->EditStoreL();
	CleanupStack::PushL(fileStore);
	CRichText* text2=CRichText::NewL(paraLayer, charLayer);
	CleanupStack::PushL(text2);
	text2->InsertL(0,_L("Be slow to promise and quick to perform.Be swift to hear, slow to speak. thats love..."));
	fileStore->StoreBodyTextL(*text2);
	fileStore->CommitL();
	CleanupStack::PopAndDestroy(2); // filestore,text


	fileStore=NULL;
	TMsvEntry entry3;
	entry3.iType = KUidMsvMessageEntry;
	entry3.iMtm = aMtm;
	entry3.iServiceId = KMsvLocalServiceIndexEntryId;
	entry3.iDescription.Set(data->Des());
	entry3.iDetails.Set(data->Des());
	testUtils->iMsvEntry->SetEntryL(KMsvGlobalInBoxIndexEntryId);
	testUtils->iMsvEntry->CreateL(entry3);
	TRAP(error,testUtils->iMsvEntry->SetEntryL(entry3.Id()));
	//entry3Id=entry3.Id();
	iSelectionArray.AppendL(entry3.Id());
	
	fileStore = testUtils->iMsvEntry->EditStoreL();
	CleanupStack::PushL(fileStore);
	CImHeader* header2 = CImHeader::NewLC(); 
	CreateMessageHeader3L(*header2);
	header2->StoreL(*fileStore);
	fileStore->CommitL();
	CleanupStack::PopAndDestroy(2); // filestore,header
	
	
	TMsvEmailEntry ementry3;
	ementry3.iMtm = aMtm;
	ementry3.iServiceId = testUtils->iMsvEntry->Entry().iServiceId;
	ementry3.iType = KUidMsvEmailTextEntry;
	ementry3.iDescription.Set(data->Des());
	ementry3.iDetails.Set(data->Des());
	testUtils->iMsvEntry->CreateL(ementry3);
	TRAP(error,testUtils->iMsvEntry->SetEntryL(ementry3.Id()));

	fileStore=NULL;
	fileStore = testUtils->iMsvEntry->EditStoreL();
	CleanupStack::PushL(fileStore);
	CRichText* text3=CRichText::NewL(paraLayer, charLayer);
	CleanupStack::PushL(text3);
	text3->InsertL(0,_L("You can understand the facts of life. But facts of love are much different. To love you love someone or someone loves you.. But when you both love each other you're in love with one another. First become friends and let it go. Then get serious and get together. Just make yourself known as a person not as someone you don't want to be known as..."));
	fileStore->StoreBodyTextL(*text3);
	fileStore->CommitL();
	CleanupStack::PopAndDestroy(4); // filestore,header,text,layers
	delete data;
	data = NULL;


	}

void CTestHeaderSearchSortOperation::TestHeaderSearchWithSortL()
{
	
	testUtils = CEmailTestUtils::NewLC(test);
	testUtils->CreateAllTestDirectories();
	testUtils->FileSession().SetSessionPath(_L("C:\\"));
	testUtils->CleanMessageFolderL();
	testUtils->GoClientSideL();	
		
	_LIT(KFunction, "Test Header Search and Sort");
	INFO_PRINTF1(KFunction);
	
	CreateMessagesL(KUidFindTestPOP3Mtm);
	CreateMessagesL(KUidFindTestPOP3Mtm);
	
	_LIT(KFormatString, "String copied for - [%S]");
	_LIT(KFormatStr, " Result found  %D TMsvIds ");
	_LIT(KFormatId, "TMsvId %D");
	_LIT(KFormatName1," String searching for as*");
	_LIT(KFormatName2," String searching for ka*");
	
	_LIT(KSearchString, "as*");//Search String
	TBufC<10> searchString(KSearchString);
	
	_LIT(KSearchString1, "ka*");//Search String
	TBufC<10> searchString1(KSearchString1);
	
	CMsvSearchSortQuery *searchQuery= CMsvSearchSortQuery::NewL();
	CleanupStack::PushL(searchQuery);
	searchQuery->SetResultType(EMsvResultAsTMsvId);
	searchQuery->SetWholeWord(EFalse);
	searchQuery->SetCaseSensitiveOption(EFalse);
	searchQuery->SetWildCardSearch(ETrue);
	searchQuery->iIsSearchQuery=ETrue;
	searchQuery->iExplicitSort=ETrue;
	searchQuery->iSortOnMessagePart=EMsvBcc;
	searchQuery->AddSearchOptionL (EMsvTo, searchString, EMsvEqual);
	searchQuery->AddSearchOptionL (EMsvCc, searchString1, EMsvEqual);
	
	//CMsvSearchsortOpOnHeaderBody* search = CMsvSearchsortOpOnHeaderBody::NewL(*testUtils->iMsvSession,TPriority::EPriorityStandard);		
	CMsvSearchsortOpOnHeaderBody* search = CMsvSearchsortOpOnHeaderBody::NewL(*testUtils->iMsvSession,0);
	search->SearchSortOnHeaderBodyL(searchQuery,iSelectionArray, EFalse, iActiveWaiter->iStatus);
	CleanupStack::PushL(search);
	
	iActiveWaiter->WaitActive();
	
	/* searchQuery1  has 1 level
	EMsgTo=ashok 
	wholeword=false
	casesensitive=false
	*/ 
	
	RArray<TMsvIdWithSortField> array;
	search->GetResultAsTMsvIdWithSortFieldL(array);
	
	INFO_PRINTF1(KFormatName1);
	INFO_PRINTF1(KFormatName2);
	INFO_PRINTF2(KFormatStr,array.Count());
	for(TInt i=0;i<array.Count();i++)
		{
		HBufC16* as = array[i].iContentMessagePart.Alloc();
		INFO_PRINTF2(KFormatString, as);
		INFO_PRINTF2(KFormatId,array[i].iMessageId);
	//	INFO_PRINTF2(KFormatString,iResultIdSortFieldArray[i].iContentMessagePart);
		}
		
	ASSERT_EQUALS(array.Count(), 4);
	
	iSelectionArray.Close();
	array.Close();
	CleanupStack::PopAndDestroy(3);
//	Closedown();
	
	
}

void CTestHeaderSearchSortOperation::TestHeaderSortToL()
{
	testUtils = CEmailTestUtils::NewLC(test);
	testUtils->CreateAllTestDirectories();
	testUtils->FileSession().SetSessionPath(_L("C:\\"));
	testUtils->CleanMessageFolderL();
	testUtils->GoClientSideL();
	
	CreateMessagesL(KUidFindTestPOP3Mtm);
	CreateMessagesL(KUidFindTestPOP3Mtm);
	
	_LIT(KFunction, "TestHeaderSort To Field");
	INFO_PRINTF1(KFunction);
	
	RBuf16 compareString1;
	RBuf16 compareString2;
	RBuf16 compareString3;
	RBuf16 compareString4;
	RBuf16 compareString5;
	RBuf16 compareString6;
	
	compareString1.CreateL(50);
	compareString2.CreateL(50);
	compareString3.CreateL(50);
	compareString4.CreateL(50);
	compareString5.CreateL(50);
	compareString6.CreateL(50);
	
	_LIT(KFirstField, "ashok@symbian.com");
	_LIT(KSecondField, "Ashok@address.com");
	_LIT(KThirdField, "AshokKumar@address.com");
	_LIT(KFourthField, "ashok@symbian.com");
	_LIT(KFifthField, "Ashok@address.com");
	_LIT(KSixthField, "AshokKumar@address.com");
	
	_LIT(KFormatAfter, "string copied - [%S]");
	
	// Sort based on To field
	
	CMsvSearchSortQuery *searchQuery= CMsvSearchSortQuery::NewL();
	CleanupStack::PushL(searchQuery);	
	searchQuery->SetResultType(EMsvResultAsTMsvId);
	searchQuery->iIsSearchQuery=EFalse;
	searchQuery->iExplicitSort=EFalse;
	//searchQuery->iSortOnMessagePart=EMsvTo;
	searchQuery->AddSortOptionL(EMsvTo,EMsvSortDescending);

	CMsvSearchsortOpOnHeaderBody* search = CMsvSearchsortOpOnHeaderBody::NewL(*testUtils->iMsvSession,0);
	search->SearchSortOnHeaderBodyL(searchQuery,iSelectionArray, ETrue, iActiveWaiter->iStatus);
	CleanupStack::PushL(search);
	
	iActiveWaiter->WaitActive();

	RArray<TMsvIdWithSortField> array;
	search->GetResultAsTMsvIdWithSortFieldL(array);	

	
//	ASSERT_EQUALS(aResultIdSortFieldArray.Count(), 3);
	
	//delete search;
	compareString1.Copy(array[0].iContentMessagePart);
	INFO_PRINTF2(KFormatAfter,&compareString1);
	MY_ASSERT_EQUALS(0, compareString1.Compare(KFirstField));
	
	compareString2.Copy(array[1].iContentMessagePart);
	INFO_PRINTF2(KFormatAfter,&compareString2);
	MY_ASSERT_EQUALS(0,compareString2.Compare(KSecondField));
	
	compareString3.Copy(array[2].iContentMessagePart);
	INFO_PRINTF2(KFormatAfter,&compareString3);
	MY_ASSERT_EQUALS(0,compareString3.Compare(KThirdField));
	
	compareString4.Copy(array[3].iContentMessagePart);
	INFO_PRINTF2(KFormatAfter,&compareString4);
	MY_ASSERT_EQUALS(0,compareString4.Compare(KFourthField));
	
	compareString5.Copy(array[4].iContentMessagePart);
	INFO_PRINTF2(KFormatAfter,&compareString5);
	MY_ASSERT_EQUALS(0,compareString5.Compare(KFifthField));
	
	compareString6.Copy(array[5].iContentMessagePart);
	INFO_PRINTF2(KFormatAfter,&compareString6);
	MY_ASSERT_EQUALS(0,compareString6.Compare(KSixthField));
	
	
	compareString1.Delete(0,50);
	compareString2.Delete(0,50);
	compareString3.Delete(0,50);
	compareString4.Delete(0,50);
	compareString5.Delete(0,50);
	compareString6.Delete(0,50);
	
	compareString1.Close();
	compareString2.Close();
	compareString3.Close();
	compareString4.Close();
	compareString5.Close();
	compareString6.Close();

	iSelectionArray.Close();
//	delete searchQuery;
	array.Reset();
	array.Close();
	CleanupStack::PopAndDestroy(3);

//	Closedown();
}


void CTestHeaderSearchSortOperation::TestHeaderSortFromL()
{
	testUtils = CEmailTestUtils::NewLC(test);
	testUtils->CreateAllTestDirectories();
	testUtils->FileSession().SetSessionPath(_L("C:\\"));
	testUtils->CleanMessageFolderL();
	testUtils->GoClientSideL();

	CreateMessagesL(KUidFindTestPOP3Mtm);
	CreateMessagesL(KUidFindTestPOP3Mtm);

	_LIT(KFunction, "TestHeaderSort From Field");
	INFO_PRINTF1(KFunction);
	
	RBuf16 compareString1;
	RBuf16 compareString2;
	RBuf16 compareString3;
	RBuf16 compareString4;
	RBuf16 compareString5;
	RBuf16 compareString6;

	
	compareString1.CreateL(50);
	compareString2.CreateL(50);
	compareString3.CreateL(50);
	compareString4.CreateL(50);
	compareString5.CreateL(50);
	compareString6.CreateL(50);
	
/*	_LIT(KFirstFrom, "channa@address.com");
	_LIT(KSecondFrom, "kavita_khatawate@address.com");
	_LIT(KThirdFrom, "sukantabanik@address.com");
	_LIT(KFourthFrom, "channa@address.com");
	_LIT(KFifthFrom, "kavita_khatawate@address.com");
	_LIT(KSixthFrom, "sukantabanik@address.com");*/
	_LIT(KFormatAfter, "string copied - [%S]");

	CMsvSearchSortQuery *searchQuery= CMsvSearchSortQuery::NewL();
	CleanupStack::PushL(searchQuery);
	
	searchQuery->SetResultType(EMsvResultAsTMsvId);
	searchQuery->iIsSearchQuery=EFalse;
	searchQuery->iExplicitSort=ETrue;
	//searchQuery->iSortOnMessagePart=EMsvFrom;
	searchQuery->AddSortOptionL(EMsvFrom ,EMsvSortAscending);
	
	CMsvSearchsortOpOnHeaderBody* search = CMsvSearchsortOpOnHeaderBody::NewL(*testUtils->iMsvSession,0);
	search->SearchSortOnHeaderBodyL(searchQuery,iSelectionArray, ETrue, iActiveWaiter->iStatus);
	CleanupStack::PushL(search);
	
	iActiveWaiter->WaitActive();
	
	
	
	RArray<TMsvIdWithSortField> array;
	search->GetResultAsTMsvIdWithSortFieldL(array);
//	search->SearchInHeaderBodyL(iSelectionArray, aResultIdSortFieldArray);
	
	
		
	compareString1.Copy(array[0].iContentMessagePart);
	INFO_PRINTF2(KFormatAfter,&compareString1);
//	MY_ASSERT_EQUALS(0, compareString1.Compare(KMiutFrom2));
	
	compareString2.Copy(array[1].iContentMessagePart);
	INFO_PRINTF2(KFormatAfter,&compareString2);
//	MY_ASSERT_EQUALS(0,compareString2.Compare(KSecondFrom));
	
	compareString3.Copy(array[2].iContentMessagePart);
	INFO_PRINTF2(KFormatAfter,&compareString3);
//	MY_ASSERT_EQUALS(0,compareString3.Compare(KThirdFrom));
	
	compareString4.Copy(array[3].iContentMessagePart);
	INFO_PRINTF2(KFormatAfter,&compareString4);
//	MY_ASSERT_EQUALS(0,compareString4.Compare(KFourthFrom));
	
	compareString5.Copy(array[4].iContentMessagePart);
	INFO_PRINTF2(KFormatAfter,&compareString5);
//	MY_ASSERT_EQUALS(0,compareString5.Compare(KFifthFrom));
	
	compareString6.Copy(array[5].iContentMessagePart);
	INFO_PRINTF2(KFormatAfter,&compareString6);
//	MY_ASSERT_EQUALS(0,compareString6.Compare(KSixthFrom));
	

	CleanupStack::PopAndDestroy(3);
	
	compareString1.Delete(0,50);
	compareString2.Delete(0,50);
	compareString3.Delete(0,50);
	compareString4.Delete(0,50);
	compareString5.Delete(0,50);
	compareString6.Delete(0,50);
	
	compareString1.Close();
	compareString2.Close();
	compareString3.Close();
	compareString4.Close();
	compareString5.Close();
	compareString6.Close();

	iSelectionArray.Close();
//	delete searchQuery;
	array.Reset();
	array.Close();

//	Closedown();
}


void CTestHeaderSearchSortOperation::TestHeaderSortCcL()
{
	testUtils = CEmailTestUtils::NewLC(test);
	testUtils->CreateAllTestDirectories();
	testUtils->FileSession().SetSessionPath(_L("C:\\"));
	testUtils->CleanMessageFolderL();
	testUtils->GoClientSideL();

	CreateMessagesL(KUidFindTestPOP3Mtm);
	CreateMessagesL(KUidFindTestPOP3Mtm);
	
	_LIT(KFunction, "TestHeaderSort Cc Field");
	INFO_PRINTF1(KFunction);
	
	RBuf16 compareString1;
	RBuf16 compareString2;
	RBuf16 compareString3;
	RBuf16 compareString4;
	RBuf16 compareString5;
	RBuf16 compareString6;
	
	compareString1.CreateL(50);
	compareString2.CreateL(50);
	compareString3.CreateL(50);
	compareString4.CreateL(50);
	compareString5.CreateL(50);
	compareString6.CreateL(50);
	
	_LIT(KFirstCc, "channa@address.com");
	_LIT(KSecondCc, "kavita_khatawate@address.com");
	_LIT(KThirdCc, "sukantabanik@address.com");
	_LIT(KFourthCc, "channa@address.com");
	_LIT(KFifthCc, "kavita_khatawate@address.com");
	_LIT(KSixthCc, "sukantabanik@address.com");
	_LIT(KFormatAfter, "string copied - [%S]");
	//Sort on Cc
	CMsvSearchSortQuery *searchQuery= CMsvSearchSortQuery::NewL();
	CleanupStack::PushL(searchQuery);
	searchQuery->SetResultType(EMsvResultAsTMsvId);
	searchQuery->iIsSearchQuery=EFalse;
	searchQuery->iExplicitSort=ETrue;
	//searchQuery->iSortOnMessagePart=EMsvCc;
	searchQuery->AddSortOptionL(EMsvCc ,EMsvSortAscending);
	
	CMsvSearchsortOpOnHeaderBody* search = CMsvSearchsortOpOnHeaderBody::NewL(*testUtils->iMsvSession,0);
	search->SearchSortOnHeaderBodyL(searchQuery,iSelectionArray, ETrue, iActiveWaiter->iStatus);
	CleanupStack::PushL(search);
	
	iActiveWaiter->WaitActive();

	RArray<TMsvIdWithSortField> array;
	search->GetResultAsTMsvIdWithSortFieldL(array);
		
	
		

	compareString1.Copy(array[0].iContentMessagePart);
	INFO_PRINTF2(KFormatAfter,&compareString1);
	MY_ASSERT_EQUALS(0, compareString1.Compare(KFirstCc));
	
	compareString2.Copy(array[1].iContentMessagePart);
	INFO_PRINTF2(KFormatAfter,&compareString2);
	MY_ASSERT_EQUALS(0,compareString2.Compare(KSecondCc));
	
	compareString3.Copy(array[2].iContentMessagePart);
	INFO_PRINTF2(KFormatAfter,&compareString3);
	MY_ASSERT_EQUALS(0,compareString3.Compare(KThirdCc));
	
	compareString4.Copy(array[3].iContentMessagePart);
	INFO_PRINTF2(KFormatAfter,&compareString4);
	MY_ASSERT_EQUALS(0,compareString4.Compare(KFourthCc));
	
	compareString5.Copy(array[4].iContentMessagePart);
	INFO_PRINTF2(KFormatAfter,&compareString5);
	MY_ASSERT_EQUALS(0,compareString5.Compare(KFifthCc));
	
	compareString6.Copy(array[5].iContentMessagePart);
	INFO_PRINTF2(KFormatAfter,&compareString6);
	MY_ASSERT_EQUALS(0,compareString6.Compare(KSixthCc));
		
	compareString1.Delete(0,50);
	compareString2.Delete(0,50);
	compareString3.Delete(0,50);
	compareString4.Delete(0,50);
	compareString5.Delete(0,50);
	compareString6.Delete(0,50);
	
	compareString1.Close();
	compareString2.Close();
	compareString3.Close();
	compareString4.Close();
	compareString5.Close();
	compareString6.Close();

	iSelectionArray.Close();
//	delete searchQuery;
	array.Close();
	CleanupStack::PopAndDestroy(3);
//	Closedown();
}


void CTestHeaderSearchSortOperation::TestHeaderSortBccL()
{
	testUtils = CEmailTestUtils::NewLC(test);
	testUtils->CreateAllTestDirectories();
	testUtils->FileSession().SetSessionPath(_L("C:\\"));
	testUtils->CleanMessageFolderL();
	testUtils->GoClientSideL();
	

	CreateMessagesL(KUidFindTestPOP3Mtm);
	CreateMessagesL(KUidFindTestPOP3Mtm);
	
	_LIT(KFunction, "TestHeaderSort Bcc Field");
	INFO_PRINTF1(KFunction);
	
	RBuf16 compareString1;
	RBuf16 compareString2;
	RBuf16 compareString3;
	RBuf16 compareString4;
	RBuf16 compareString5;
	RBuf16 compareString6;
	
	compareString1.CreateL(50);
	compareString2.CreateL(50);
	compareString3.CreateL(50);
	compareString4.CreateL(50);
	compareString5.CreateL(50);
	compareString6.CreateL(50);
	
	/*_LIT(KFirstBCc, "bcc1@address.com");
	_LIT(KSecondBCc, "bcc2@address.com");
	_LIT(KThirdBCc, "bcc3@address.com");
	_LIT(KFourthBCc, "bcc4@address.com");
	_LIT(KFifthBCc, "bcc5@address.com");
	_LIT(KSixthBCc, "bcc6@address.com");*/
	_LIT(KFormatAfter, "string copied - [%S]");
	
	
	CMsvSearchSortQuery *searchQuery= CMsvSearchSortQuery::NewL();
	CleanupStack::PushL(searchQuery);
	searchQuery->SetResultType(EMsvResultAsTMsvId);
	searchQuery->iIsSearchQuery=EFalse;
	searchQuery->iExplicitSort=ETrue;
//	searchQuery->iSortOnMessagePart=EMsvBcc;
	searchQuery->AddSortOptionL(EMsvBcc ,EMsvSortAscending);
	
	
	CMsvSearchsortOpOnHeaderBody* search = CMsvSearchsortOpOnHeaderBody::NewL(*testUtils->iMsvSession,0);
//	search->SearchInHeaderBodyL(iSelectionArray, aResultIdSortFieldArray);
	CleanupStack::PushL(search);
	
	search->SearchSortOnHeaderBodyL(searchQuery,iSelectionArray, ETrue, iActiveWaiter->iStatus);
	
	iActiveWaiter->WaitActive();
	
	RArray<TMsvIdWithSortField> array;
	search->GetResultAsTMsvIdWithSortFieldL(array);	

	

	compareString1.Copy(array[0].iContentMessagePart);
	INFO_PRINTF2(KFormatAfter,&compareString1);
//	MY_ASSERT_EQUALS(0, compareString1.Compare(KFirstBCc));
	
	compareString2.Copy(array[1].iContentMessagePart);
	INFO_PRINTF2(KFormatAfter,&compareString2);
//	MY_ASSERT_EQUALS(0,compareString2.Compare(KSecondBCc));
	
	compareString3.Copy(array[2].iContentMessagePart);
	INFO_PRINTF2(KFormatAfter,&compareString3);
//	MY_ASSERT_EQUALS(0,compareString3.Compare(KThirdBCc));
	
	compareString4.Copy(array[3].iContentMessagePart);
	INFO_PRINTF2(KFormatAfter,&compareString4);
//	MY_ASSERT_EQUALS(0,compareString4.Compare(KFourthBCc));
	
	compareString5.Copy(array[4].iContentMessagePart);
	INFO_PRINTF2(KFormatAfter,&compareString5);
//	MY_ASSERT_EQUALS(0,compareString5.Compare(KFifthBCc));
	
	compareString6.Copy(array[5].iContentMessagePart);
	INFO_PRINTF2(KFormatAfter,&compareString6);
//	MY_ASSERT_EQUALS(0,compareString6.Compare(KSixthBCc));
	
	iSelectionArray.Close();
////delete searchQuery;
	array.Reset();
	array.Close();
	
	compareString1.Delete(0,50);
	compareString2.Delete(0,50);
	compareString3.Delete(0,50);
	compareString4.Delete(0,50);
	compareString5.Delete(0,50);
	compareString6.Delete(0,50);
	
	compareString1.Close();
	compareString2.Close();
	compareString3.Close();
	compareString4.Close();
	compareString5.Close();
	compareString6.Close();

	
	CleanupStack::PopAndDestroy(3);

}


void CTestHeaderSearchSortOperation::TestHeaderSortSubjectL()
{
	testUtils = CEmailTestUtils::NewLC(test);
	testUtils->CreateAllTestDirectories();
	testUtils->FileSession().SetSessionPath(_L("C:\\"));
	testUtils->CleanMessageFolderL();
	testUtils->GoClientSideL();
	

	CreateMessagesL(KUidFindTestPOP3Mtm);
	CreateMessagesL(KUidFindTestPOP3Mtm);
	
	_LIT(KFunction, "TestHeaderSort Subject Field");
	INFO_PRINTF1(KFunction);
	_LIT(KFormatAfter, "string copied - [%S]");
	RBuf16 compareString1;
	RBuf16 compareString2;
	RBuf16 compareString3;
	RBuf16 compareString4;
	RBuf16 compareString5;
	RBuf16 compareString6;
	
	compareString1.CreateL(50);
	compareString2.CreateL(50);
	compareString3.CreateL(50);
	compareString4.CreateL(50);
	compareString5.CreateL(50);
	compareString6.CreateL(50);
	
	//_LIT(KSubject, "short mail about something int");
	
	// Sort on subject field
	CMsvSearchSortQuery *searchQuery= CMsvSearchSortQuery::NewL();
	CleanupStack::PushL(searchQuery);
	searchQuery->SetResultType(EMsvResultAsTMsvId);
	searchQuery->iIsSearchQuery=EFalse;
	searchQuery->iExplicitSort=EFalse;
	//searchQuery->iSortOnMessagePart=EMsvSubject;
	searchQuery->AddSortOptionL(EMsvSubject ,EMsvSortDescending);
	
	CMsvSearchsortOpOnHeaderBody* search = CMsvSearchsortOpOnHeaderBody::NewL(*testUtils->iMsvSession,0);
	search->SearchSortOnHeaderBodyL(searchQuery,iSelectionArray, ETrue, iActiveWaiter->iStatus);
	CleanupStack::PushL(search);

	iActiveWaiter->WaitActive();
	
	RArray<TMsvIdWithSortField> array;
	search->GetResultAsTMsvIdWithSortFieldL(array);
	
	
	
	compareString1.Copy(array[0].iContentMessagePart);
	INFO_PRINTF2(KFormatAfter,&compareString1);
//	MY_ASSERT_EQUALS(0, compareString1.Compare(KSubject));
	
	compareString2.Copy(array[1].iContentMessagePart);
	INFO_PRINTF2(KFormatAfter,&compareString1);
//	MY_ASSERT_EQUALS(0,compareString1.Compare(KSubject));
	
	compareString3.Copy(array[2].iContentMessagePart);
	INFO_PRINTF2(KFormatAfter,&compareString1);
//	MY_ASSERT_EQUALS(0,compareString1.Compare(KSubject));
	
	compareString4.Copy(array[3].iContentMessagePart);
	INFO_PRINTF2(KFormatAfter,&compareString1);
//	MY_ASSERT_EQUALS(0,compareString1.Compare(KSubject));
	
	compareString5.Copy(array[4].iContentMessagePart);
	INFO_PRINTF2(KFormatAfter,&compareString1);
//	MY_ASSERT_EQUALS(0,compareString1.Compare(KSubject));
	
	compareString6.Copy(array[5].iContentMessagePart);
	INFO_PRINTF2(KFormatAfter,&compareString1);
//	MY_ASSERT_EQUALS(0,compareString1.Compare(KSubject));
	
	
	compareString1.Delete(0,50);
	compareString2.Delete(0,50);
	compareString3.Delete(0,50);
	compareString4.Delete(0,50);
	compareString5.Delete(0,50);
	compareString6.Delete(0,50);
	
	compareString1.Close();
	compareString2.Close();
	compareString3.Close();
	compareString4.Close();
	compareString5.Close();
	compareString6.Close();

	iSelectionArray.Close();
	array.Reset();
	array.Close();
	CleanupStack::PopAndDestroy(3);	
//	Closedown();
}

void CTestHeaderSearchSortOperation::TestHeaderSearchToL()
	{
	testUtils = CEmailTestUtils::NewLC(test);
	testUtils->CreateAllTestDirectories();
	testUtils->FileSession().SetSessionPath(_L("C:\\"));
	testUtils->CleanMessageFolderL();
	testUtils->GoClientSideL();
	
	CreateMessagesL(KUidFindTestPOP3Mtm);
	
	//_LIT(KFormatString, "String searched for - [%S]");
	_LIT(KFormatStr, " Result found  %D TMsvIds ");
	_LIT(KFormatId, "TMsvId %D");
	
	_LIT(KSearchString, "as*");//Search String
	TBufC<10> searchString(KSearchString);
	
	_LIT(KSearchString1, "ya*");//Search String
	TBufC<10> searchString1(KSearchString1);
	
	CMsvSearchSortQuery *searchQuery= CMsvSearchSortQuery::NewL();
	CleanupStack::PushL(searchQuery);
	searchQuery->SetResultType(EMsvResultAsTMsvId);
	searchQuery->SetWholeWord(EFalse);
	searchQuery->SetCaseSensitiveOption(EFalse);
	searchQuery->SetWildCardSearch(ETrue);
	searchQuery->iIsSearchQuery=ETrue;
	//searchQuery->AddSearchQueryOptions(EMsvTo, searchString, EMsvEqual, EAnd);
    searchQuery->AddSearchOptionL (EMsvTo, searchString, EMsvEqual);

	
	
	
	CMsvSearchsortOpOnHeaderBody* search = CMsvSearchsortOpOnHeaderBody::NewL(*testUtils->iMsvSession,0);
	search->SearchSortOnHeaderBodyL(searchQuery,iSelectionArray, EFalse, iActiveWaiter->iStatus);
	CleanupStack::PushL(search);
	
	iActiveWaiter->WaitActive();
	
	/* searchQuery  has 1 level
	EMsvTo=ashok 
	wholeword=false
	casesensitive=false
	*/ 
	
	RArray<TMsvId> array;
	search->GetResultAsTMsvIdL(array);	
	INFO_PRINTF1(_L("Searching for - as*"));
	INFO_PRINTF2(KFormatStr,array.Count());
	for(TInt i=0;i<array.Count();i++)
		{
		INFO_PRINTF2(KFormatId,array[i]);
		}
		
	ASSERT_EQUALS(array.Count(), 3);
	
	// Fetching TMsvEntries
	search->DoGetEntryForAllIdL(array,iActiveWaiter->iStatus);
	
	iActiveWaiter->WaitActive();
	
	search->GetResultAsTmsvEntryL(iEntryArray);	
	
	INFO_PRINTF1(_L("Searching for - as*"));
	INFO_PRINTF2(KFormatStr,array.Count());
	for(TInt i=0;i<array.Count();i++)
		{
		INFO_PRINTF2(KFormatId,iEntryArray[i].Id());
		}
	
	
	// fetching invalid Id's
	array.Reset();
	search->GetResultAsInvalidTMsvIdL(array);
	INFO_PRINTF1(_L("Printing invalid ID's - "));
	INFO_PRINTF2(KFormatStr,array.Count());
	for(TInt i=0;i<array.Count();i++)
		{
		INFO_PRINTF2(KFormatId,array[i]);
		}
		
	iSelectionArray.Close();
//	delete searchQuery;
	array.Reset();
	array.Close();
	CleanupStack::PopAndDestroy(3);
//	Closedown();
	
	}

void CTestHeaderSearchSortOperation::TestHeaderSearchCcL()
	{
	testUtils = CEmailTestUtils::NewLC(test);
	testUtils->CreateAllTestDirectories();
	testUtils->FileSession().SetSessionPath(_L("C:\\"));
	testUtils->CleanMessageFolderL();
	testUtils->GoClientSideL();
	
	CreateMessagesL(KUidFindTestPOP3Mtm);
	CreateMessagesL(KUidFindTestPOP3Mtm);
	
	//_LIT(KFormatString, "String searched for - [%S]");
	_LIT(KFormatStr, " Result found  %D TMsvIds ");
	_LIT(KFormatId, "TMsvId %D");
	
	_LIT(KSearchCc, "sukanta");//Search String
	TBufC<10> searchStringCc(KSearchCc);
	
		
	CMsvSearchSortQuery *searchQuery= CMsvSearchSortQuery::NewL();
	CleanupStack::PushL(searchQuery);
	searchQuery->SetResultType(EMsvResultAsTMsvEntry);
	searchQuery->SetWholeWord(EFalse);
	searchQuery->SetCaseSensitiveOption(EFalse);
	searchQuery->SetWildCardSearch(EFalse);
	searchQuery->AddSearchOptionL (EMsvCc, searchStringCc, EMsvEqual);
	
	
	CMsvSearchsortOpOnHeaderBody* search = CMsvSearchsortOpOnHeaderBody::NewL(*testUtils->iMsvSession,0);
	search->SearchSortOnHeaderBodyL(searchQuery,iSelectionArray, EFalse, iActiveWaiter->iStatus);
	CleanupStack::PushL(search);
	iActiveWaiter->WaitActive();
	
	RArray<TMsvId> array;
	search->GetResultAsTMsvIdL(array);
	INFO_PRINTF1(_L("Searching for - sukanta"));
	INFO_PRINTF2(KFormatStr,array.Count());
	for(TInt i=0;i<array.Count();i++)
		{
		INFO_PRINTF2(KFormatId,array[i]);
		}
	
	ASSERT_EQUALS(array.Count(), 6);
	
	iSelectionArray.Close();
	array.Reset();
	array.Close();
	CleanupStack::PopAndDestroy(3);
//	Closedown();
	}

void CTestHeaderSearchSortOperation::TestHeaderSearchBccL()
	{
	testUtils = CEmailTestUtils::NewLC(test);
	testUtils->CreateAllTestDirectories();
	testUtils->FileSession().SetSessionPath(_L("C:\\"));
	testUtils->CleanMessageFolderL();
	testUtils->GoClientSideL();
	
	
	CreateMessagesL(KUidFindTestPOP3Mtm);
	
	//_LIT(KFormatString, "String searched for - [%S]");
	_LIT(KFormatStr, " Result found  %D TMsvIds ");
	_LIT(KFormatId, "TMsvId %D");
	
	_LIT(KSearchBcc, "bcc1");//Search String
	TBufC<10> searchStringBcc(KSearchBcc);
	
	_LIT(KSearchBcc1, "bcc2");//Search String
	TBufC<10> searchStringBcc1(KSearchBcc1);
	
	//TMsvSearchSortQuery searchQuery;
	CMsvSearchSortQuery *searchQuery= CMsvSearchSortQuery::NewL();
	CleanupStack::PushL(searchQuery);
	searchQuery->SetResultType(EMsvResultAsTMsvEntry);
	searchQuery->SetWholeWord(EFalse);
	searchQuery->SetCaseSensitiveOption(EFalse);
	searchQuery->SetWildCardSearch(EFalse);
//	searchQuery->AddSearchQueryOptions(EMsvBcc, searchStringBcc, EMsvEqual, EAnd);
	searchQuery->AddSearchOptionL (EMsvBcc, searchStringBcc, EMsvEqual);
	
	
	CMsvSearchsortOpOnHeaderBody* search = CMsvSearchsortOpOnHeaderBody::NewL(*testUtils->iMsvSession,0);
	search->SearchSortOnHeaderBodyL(searchQuery,iSelectionArray, EFalse, iActiveWaiter->iStatus);
	CleanupStack::PushL(search);
	iActiveWaiter->WaitActive();
	
	RArray<TMsvId> array;
	search->GetResultAsTMsvIdL(array);
	INFO_PRINTF1(_L("Searching for - bcc1"));
	INFO_PRINTF2(KFormatStr,array.Count());
	for(TInt i=0;i<array.Count();i++)
		{
		INFO_PRINTF2(KFormatId,array[i]);
		}
	ASSERT_EQUALS(array.Count(), 1);
	
		
	// fetching invalid Id's
	array.Reset();
	search->GetResultAsInvalidTMsvIdL(array);
	INFO_PRINTF1(_L("Printing invalid ID's - "));
	INFO_PRINTF2(KFormatStr,array.Count());
	for(TInt i=0;i<array.Count();i++)
		{
		INFO_PRINTF2(KFormatId,array[i]);
		}
	
	iSelectionArray.Close();
//	delete searchQuery;
	array.Reset();
	array.Close();
	CleanupStack::PopAndDestroy(3);
//	Closedown();
	}
	
void CTestHeaderSearchSortOperation::TestHeaderSearchFromL()
	{
	
	testUtils = CEmailTestUtils::NewLC(test);
	testUtils->CreateAllTestDirectories();
	testUtils->FileSession().SetSessionPath(_L("C:\\"));
	testUtils->CleanMessageFolderL();
	testUtils->GoClientSideL();
	
	
	CreateMessagesL(KUidFindTestPOP3Mtm);
	CreateMessagesL(KUidFindTestPOP3Mtm);
	
	//_LIT(KFormatString, "String searched for - [%S]");
	_LIT(KFormatStr, " Result found  %D TMsvIds ");
	_LIT(KFormatId, "TMsvId %D");
	
	_LIT(KSearchFrom, "yatish");//Search String
	TBufC<10> searchStringFrom(KSearchFrom);
	
	_LIT(KSearchFrom1, "ashok");//Search String
	TBufC<10> searchStringFrom1(KSearchFrom1);
	
	CMsvSearchSortQuery *searchQuery= CMsvSearchSortQuery::NewL();
	CleanupStack::PushL(searchQuery);
	searchQuery->SetResultType(EMsvResultAsTMsvEntry);
	searchQuery->SetWholeWord(EFalse);
	searchQuery->SetCaseSensitiveOption(EFalse);
	searchQuery->SetWildCardSearch(EFalse);
	searchQuery->AddSearchOptionL (EMsvFrom, searchStringFrom, EMsvEqual);

	CMsvSearchsortOpOnHeaderBody* search = CMsvSearchsortOpOnHeaderBody::NewL(*testUtils->iMsvSession,0);
	search->SearchSortOnHeaderBodyL(searchQuery,iSelectionArray, EFalse, iActiveWaiter->iStatus);
	CleanupStack::PushL(search);
	iActiveWaiter->WaitActive();
	
	RArray<TMsvId> array;
	search->GetResultAsTMsvIdL(array);
	INFO_PRINTF1(_L("Searching for - yatish"));
	INFO_PRINTF2(KFormatStr,array.Count());
	for(TInt i=0;i<array.Count();i++)
		{
		INFO_PRINTF2(KFormatId,array[i]);
		}
	ASSERT_EQUALS(array.Count(), 2);
	
	iSelectionArray.Close();
	array.Reset();
	array.Close();
	CleanupStack::PopAndDestroy(3);
//	Closedown();
	}
	
void CTestHeaderSearchSortOperation::TestHeaderSearchSubjectL()
	{
	testUtils = CEmailTestUtils::NewLC(test);
	testUtils->CreateAllTestDirectories();
	testUtils->FileSession().SetSessionPath(_L("C:\\"));
	testUtils->CleanMessageFolderL();
	testUtils->GoClientSideL();	
	
	
	CreateMessagesL(KUidFindTestPOP3Mtm);
	CreateMessagesL(KUidFindTestPOP3Mtm);
	
	//_LIT(KFormatString, "String searched for - [%S]");
	_LIT(KFormatStr, " Result found  %D TMsvIds ");
	_LIT(KFormatId, "TMsvId %D");
	
	_LIT(KSearchSubject, "interesting");//Search String
	TBufC<20> searchStringSubject(KSearchSubject);
	
		
	CMsvSearchSortQuery *searchQuery= CMsvSearchSortQuery::NewL();
	CleanupStack::PushL(searchQuery);
	searchQuery->SetResultType(EMsvResultAsTMsvEntry);
	searchQuery->SetWholeWord(ETrue);
	searchQuery->SetCaseSensitiveOption(EFalse);
	searchQuery->SetWildCardSearch(EFalse);
	searchQuery->AddSearchOptionL (EMsvSubject, searchStringSubject, EMsvEqual);
		
	CMsvSearchsortOpOnHeaderBody* search = CMsvSearchsortOpOnHeaderBody::NewL(*testUtils->iMsvSession,0);
	search->SearchSortOnHeaderBodyL(searchQuery,iSelectionArray, EFalse, iActiveWaiter->iStatus);
	CleanupStack::PushL(search);
	
	iActiveWaiter->WaitActive();
	
	RArray<TMsvId> array;
	search->GetResultAsTMsvIdL(array);
	INFO_PRINTF1(_L("Searching for - interesting"));
	INFO_PRINTF2(KFormatStr,array.Count());
	for(TInt i=0;i<array.Count();i++)
		{
		INFO_PRINTF2(KFormatId,array[i]);
		}
	ASSERT_EQUALS(array.Count(), 2);
	
		// Fetching TMsvEntries
		
	
		
	iSelectionArray.Close();
	array.Reset();
	CleanupStack::PopAndDestroy(3);
//	Closedown();
	}
	
void CTestHeaderSearchSortOperation::TestHeaderSearchBodyL()
	{
	testUtils = CEmailTestUtils::NewLC(test);
	testUtils->CreateAllTestDirectories();
	testUtils->FileSession().SetSessionPath(_L("C:\\"));
	testUtils->CleanMessageFolderL();
	testUtils->GoClientSideL();
	
	
	CreateMessagesL(KUidFindTestPOP3Mtm);
	CreateMessagesL(KUidFindTestPOP3Mtm);
	
	//_LIT(KFormatString, "String searched for - [%S]");
	_LIT(KFormatStr, " Result found  %D TMsvIds ");
	_LIT(KFormatId, "TMsvId %D");
	
	_LIT(KSearchBody, "love");//Body String
	TBufC<20> searchStringBody(KSearchBody);
	
		
	CMsvSearchSortQuery *searchQuery= CMsvSearchSortQuery::NewL();
	CleanupStack::PushL(searchQuery);
	searchQuery->SetResultType(EMsvResultAsTMsvEntry);
	searchQuery->SetWholeWord(ETrue);
	searchQuery->SetCaseSensitiveOption(EFalse);
	searchQuery->SetWildCardSearch(EFalse);
	searchQuery->AddSearchOptionL (EMsvBody, searchStringBody, EMsvEqual);
		
	CMsvSearchsortOpOnHeaderBody* search = CMsvSearchsortOpOnHeaderBody::NewL(*testUtils->iMsvSession,0);
	search->SearchSortOnHeaderBodyL(searchQuery,iSelectionArray, EFalse, iActiveWaiter->iStatus);
	CleanupStack::PushL(search);
	
	iActiveWaiter->WaitActive();
	
	RArray<TMsvId> array;
	search->GetResultAsTMsvIdL(array);
	INFO_PRINTF1(_L("Searching for - love"));
	INFO_PRINTF2(KFormatStr,array.Count());
	for(TInt i=0;i<array.Count();i++)
		{
		INFO_PRINTF2(KFormatId,array[i]);
		}
	ASSERT_EQUALS(array.Count(), 4);
	
		// Fetching TMsvEntries
	
	
	// fetching invalid Id's
	array.Reset();
	search->GetResultAsInvalidTMsvIdL(array);
	INFO_PRINTF1(_L("Printing invalid ID's - "));
	INFO_PRINTF2(KFormatStr,array.Count());
	for(TInt i=0;i<array.Count();i++)
		{
		INFO_PRINTF2(KFormatId,array[i]);
		}
	
	iSelectionArray.Close();
	array.Reset();
	array.Close();
	CleanupStack::PopAndDestroy(3);
//	Closedown();
	
	
	}
	
void CTestHeaderSearchSortOperation::TestHeaderSearchTwoLevelL()
	{
	testUtils = CEmailTestUtils::NewLC(test);
	testUtils->CreateAllTestDirectories();
	testUtils->FileSession().SetSessionPath(_L("C:\\"));
	testUtils->CleanMessageFolderL();
	testUtils->GoClientSideL();
	
	
	CreateMessagesL(KUidFindTestPOP3Mtm);
	CreateMessagesL(KUidFindTestPOP3Mtm);
	
	//_LIT(KFormatString, "String searched for - [%S]");
	_LIT(KFormatStr, " Result found  %D TMsvIds ");
	_LIT(KFormatId, "TMsvId %D");
	
	_LIT(KSearchString, "ashok");//Search String
	TBufC<10> searchString(KSearchString);
	
	_LIT(KSearchCc, "sukanta");//Search String
	TBufC<10> searchStringCc(KSearchCc);
	
	CMsvSearchSortQuery *searchQuery= CMsvSearchSortQuery::NewL();
	CleanupStack::PushL(searchQuery);
	searchQuery->SetResultType(EMsvResultAsTMsvEntry);
	searchQuery->SetWholeWord(EFalse);
	searchQuery->SetCaseSensitiveOption(EFalse);
	searchQuery->SetWildCardSearch(EFalse);
	searchQuery->iIsSearchQuery=ETrue;
	searchQuery->AddSearchOptionL (EMsvTo, searchString, EMsvEqual);
	searchQuery->AddSearchOptionL (EMsvCc, searchStringCc, EMsvEqual);
	
	
	CMsvSearchsortOpOnHeaderBody* search = CMsvSearchsortOpOnHeaderBody::NewL(*testUtils->iMsvSession,0);
	search->SearchSortOnHeaderBodyL(searchQuery,iSelectionArray, EFalse, iActiveWaiter->iStatus);
	CleanupStack::PushL(search);
	
	iActiveWaiter->WaitActive();
	
	/* searchQuery1  has 1 level
	EMsgTo=ashok 
	wholeword=false
	casesensitive=false
	*/ 
	RArray<TMsvId> array;
	search->GetResultAsTMsvIdL(array);

	INFO_PRINTF2(KFormatStr,array.Count());
	for(TInt i=0;i<array.Count();i++)
		{
		INFO_PRINTF2(KFormatId,array[i]);
		}
		
	ASSERT_EQUALS(array.Count(), 6);
	
		// Fetching TMsvEntries
	
		
	// fetching invalid Id's
	array.Reset();
	search->GetResultAsInvalidTMsvIdL(array);
	INFO_PRINTF1(_L("Printing invalid ID's - "));
	INFO_PRINTF2(KFormatStr,array.Count());
	for(TInt i=0;i<array.Count();i++)
		{
		INFO_PRINTF2(KFormatId,array[i]);
		}
	
	iSelectionArray.Close();
	array.Reset();
	array.Close();
	CleanupStack::PopAndDestroy(3);
//	Closedown();	
	}
	
void CTestHeaderSearchSortOperation::TestHeaderSearchTwoLevelWithSortL()
	{
	testUtils = CEmailTestUtils::NewLC(test);
	testUtils->CreateAllTestDirectories();
	testUtils->FileSession().SetSessionPath(_L("C:\\"));
	testUtils->CleanMessageFolderL();
	testUtils->GoClientSideL();
	
	_LIT(KFormatId, "TMsvId %D");
	CreateMessagesL(KUidFindTestPOP3Mtm);
	
	//_LIT(KFormatString, "String searched for - [%S]");
	_LIT(KFormatStr, " Result found  %D TMsvIds ");
	//_LIT(KFormatId, "TMsvId %D");
	_LIT(KFormatAfter, "string copied - [%S]");
	_LIT(KSearchString, "ashok");//Search String
	TBufC<10> searchString(KSearchString);
	
	_LIT(KSearchCc, "sukanta");//Search String
	TBufC<10> searchStringCc(KSearchCc);
	
	//TMsvSearchSortQuery searchQuery;
	CMsvSearchSortQuery *searchQuery= CMsvSearchSortQuery::NewL();
	CleanupStack::PushL(searchQuery);
	searchQuery->SetResultType(EMsvResultAsTMsvId);
	searchQuery->SetWholeWord(EFalse);
	searchQuery->SetCaseSensitiveOption(EFalse);
	searchQuery->SetWildCardSearch(EFalse);
	searchQuery->iIsSearchQuery=ETrue;
	searchQuery->iExplicitSort=ETrue;
	searchQuery->iSortOnMessagePart=EMsvBcc;
//	searchQuery->AddSearchQueryOptions(EMsvTo, searchString, EMsvEqual, EAnd);
//	searchQuery->AddSearchQueryOptions(EMsvCc, searchStringCc, EMsvEqual, EAnd);	
	searchQuery->AddSearchOptionL (EMsvTo, searchString, EMsvEqual);
	searchQuery->AddSearchOptionL (EMsvCc, searchStringCc, EMsvEqual);
	
	
	
	CMsvSearchsortOpOnHeaderBody* search = CMsvSearchsortOpOnHeaderBody::NewL(*testUtils->iMsvSession,0);
	search->SearchSortOnHeaderBodyL(searchQuery,iSelectionArray, EFalse, iActiveWaiter->iStatus);
	CleanupStack::PushL(search);
	
	iActiveWaiter->WaitActive();
	RArray<TMsvIdWithSortField> array;
	search->GetResultAsTMsvIdWithSortFieldL(array);
	RArray<TMsvId> idarray;
	search->GetResultAsTMsvIdL(idarray);
	
	INFO_PRINTF2(KFormatStr,array.Count());
	for(TInt i=0;i<array.Count();i++)
		{
		RBuf16 compareString1;
		compareString1.CreateL(50);
		compareString1.Copy(array[i].iContentMessagePart);
		INFO_PRINTF2(KFormatAfter,&compareString1);
		compareString1.Delete(0,50);
		}
		
	ASSERT_EQUALS(array.Count(), 3);
	
		
	// fetching invalid Id's
	array.Reset();
	search->GetResultAsInvalidTMsvIdL(idarray);
	INFO_PRINTF1(_L("Printing invalid ID's - "));
	INFO_PRINTF2(KFormatStr,idarray.Count());
	for(TInt i=0;i<array.Count();i++)
		{
		INFO_PRINTF2(KFormatId,idarray[i]);
		}

	iSelectionArray.Close();
//	delete searchQuery;
	array.Reset();
	CleanupStack::PopAndDestroy(3);
//	Closedown();		
	}

void CTestHeaderSearchSortOperation::TearDownL()
	{
	_LIT(KTearDownL, "TearDownL");
	INFO_PRINTF1(KTearDownL);
	delete iActiveWaiter;
	test.Close();
	//CleanupStack::PopAndDestroy(1);  //testUtils
	}

CTestSuite* CTestHeaderSearchSortOperation::CreateSuiteL(const TDesC& aName)
	{
	SUB_SUITE;
	ADD_ASYNC_TEST_STEP(TestHeaderSortToL);
	ADD_ASYNC_TEST_STEP(TestHeaderSortCcL);
	ADD_ASYNC_TEST_STEP(TestHeaderSortBccL);
	ADD_ASYNC_TEST_STEP(TestHeaderSortFromL);
	ADD_ASYNC_TEST_STEP(TestHeaderSortSubjectL);
	ADD_ASYNC_TEST_STEP(TestHeaderSearchWithSortL);
	ADD_ASYNC_TEST_STEP(TestHeaderSearchTwoLevelL);
	ADD_ASYNC_TEST_STEP(TestHeaderSearchTwoLevelWithSortL);
	ADD_ASYNC_TEST_STEP(TestHeaderSearchToL);
	ADD_ASYNC_TEST_STEP(TestHeaderSearchCcL);
	ADD_ASYNC_TEST_STEP(TestHeaderSearchBccL);
	ADD_ASYNC_TEST_STEP(TestHeaderSearchFromL);
	ADD_ASYNC_TEST_STEP(TestHeaderSearchSubjectL);
	ADD_ASYNC_TEST_STEP(TestHeaderSearchBodyL);
	END_SUITE;
	}
