//
// Copyright (c) 2009 Nokia Corporation and/or its subsidiary(-ies).
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

#include "t_msv_clnt_test_header_step.h"
#include <msvruids.h>
#include <msvuids.h>
#include <msvids.h>
#include <msvapi.h>
#include <sqldb.h>

#define MY_ASSERT_TRUE(a)  \
		INFO_PRINTF1(KFunction);   \
		ASSERT_TRUE(a);
#define MY_ASSERT_FALSE(a)  \
		INFO_PRINTF1(KFunction);   \
		ASSERT_FALSE(a);
#define MY_ASSERT_EQUALS(a,b)  \
		INFO_PRINTF1(KFunction);   \
		ASSERT_EQUALS(a,b);



_LIT(KDbFileName, "\\messaging.db");
_LIT(KMsvServerPattern, "!MsvServer*");


// ---------- Standard methods. ---------------

CTestHeaderEntry::CTestHeaderEntry():iSession(NULL)
	{
	}

CTestHeaderEntry::~CTestHeaderEntry()
	{
	delete iSession;
	}


void CTestHeaderEntry::SetupL()
	{
	_LIT(KFunction, "SetupL");
	INFO_PRINTF1(KFunction);
	
	__UHEAP_MARK;
	
	_LIT(KMsgDriveList, "C:\\private\\1000484b\\msgprioritydrivelist.ini");
	_LIT(KMsgDriveList1, "C:\\private\\1000484b\\msgprioritydrivelist2.ini");

	RFs fs;
	User::LeaveIfError(fs.Connect());
	CFileMan* file = CFileMan::NewL(fs);
	CleanupStack::PushL(file);
	TInt err = file->Copy(KMsgDriveList1, KMsgDriveList);
	MY_ASSERT_EQUALS(err, KErrNone);
	CleanupStack::PopAndDestroy();		// file
	fs.Close();
		
	TParse parse;
	TPtrC drive = TDriveUnit(EDriveC).Name();
	parse.Set(KDbFileName, &drive, NULL);
	TFileName dbFile = parse.FullName();
	RSqlDatabase::Delete(dbFile);
	
	TRAP(err, iMsvSesionObserver = new(ELeave) CTestSessionObserver());	
	MY_ASSERT_EQUALS(err, KErrNone);	
	TRAP(err, iSession = CMsvSession::OpenSyncL(*iMsvSesionObserver));
	MY_ASSERT_EQUALS(err, KErrNone);
	}




void CTestHeaderEntry::TearDownL()
	{
	_LIT(KFunction, "TearDownL");
	INFO_PRINTF1(KFunction);

	iSession->CloseMessageServer();
	delete iSession;
	iSession = NULL;	
	delete iMsvSesionObserver;
	iMsvSesionObserver = NULL;

	TFindProcess find(KMsvServerPattern);
	TFullName name;
	if (find.Next(name) == KErrNone)
		{
		TRequestStatus status;
		RProcess process;
		User::LeaveIfError(process.Open(name));
		process.Logon(status);
		User::WaitForRequest(status);
		User::After(200000);	
		}
	
	__UHEAP_MARKEND;
	}








	
// -------------- Test methods ---------------

// Test cases when preferred drive list ini file is empty.
void CTestHeaderEntry::CTestHeaderTableCreation()
	{
	_LIT(KFunction, "CTestHeaderTableCreation()");
	INFO_PRINTF1(KFunction);
	
	// 1. Create a default table.
	// 1.1 Create store object.
	TUid mtmId = TUid::Uid(10);
	CMsvHeaderStore* headerTable = NULL;
	TRAPD(err, headerTable = CMsvHeaderStore::NewL(mtmId, *iSession));
	MY_ASSERT_EQUALS(err, KErrNone);
		
	// 1.2 Create store.
	TRAP(err, headerTable->CreateStoreL());
	MY_ASSERT_EQUALS(err, KErrNone);
	delete headerTable;
	
	
	
		
	
	// 2. Error condition.
	mtmId = TUid::Uid(30);
	TRAP(err, headerTable = CMsvHeaderStore::NewLC(mtmId, *iSession);
			  CleanupStack::Pop(headerTable);
	);
	MY_ASSERT_EQUALS(err, KErrNone);
	
	// 2.1 Add duplicate fields
	TRAP(err, headerTable->AddFieldL(CMsvHeaderStore::ETo));
	MY_ASSERT_EQUALS(err, KErrNone);
	
	TRAP(err, headerTable->AddFieldL(CMsvHeaderStore::ETo));
	MY_ASSERT_EQUALS(err, KErrNone);
	
	// 2.2 Create store.
	TRAP(err, headerTable->CreateStoreL());
	MY_ASSERT_EQUALS(err, KErrGeneral);
	
	// 2.3 Check for error text.
	TPtrC errorMsg;
	TRAP(err, errorMsg.Set(headerTable->LastErrorMessageL()));
	MY_ASSERT_EQUALS(err, KErrNone);
	delete headerTable;






	// 3. Create a full table.
	// 3.1 	Create store object.
	mtmId = TUid::Uid(20);
	TRAP(err, headerTable = CMsvHeaderStore::NewL(mtmId, *iSession));
	MY_ASSERT_EQUALS(err, KErrNone);
	
	// 3.2 Add Fields
	TRAP(err, headerTable->AddFieldL(CMsvHeaderStore::EFrom));
	MY_ASSERT_EQUALS(err, KErrNone);
	
	TRAP(err, headerTable->AddFieldL(CMsvHeaderStore::ETo));
	MY_ASSERT_EQUALS(err, KErrNone);
	
	TRAP(err, headerTable->AddFieldL(CMsvHeaderStore::ECC));
	MY_ASSERT_EQUALS(err, KErrNone);
	
	TRAP(err, headerTable->AddFieldL(CMsvHeaderStore::EBCC));
	MY_ASSERT_EQUALS(err, KErrNone);
	
	TRAP(err, headerTable->AddFieldL(CMsvHeaderStore::ESubject));
	MY_ASSERT_EQUALS(err, KErrNone);
	
	_LIT16(KField1, "Field-1");
	_LIT16(KField2, "Field-2");
	_LIT16(KField3, "Field-3");
	_LIT16(KField4, "Field-4");
	HBufC16* name = KField1().AllocL();
	TRAP(err, headerTable->AddFieldL(name, EIntegerField));
	MY_ASSERT_EQUALS(err, KErrNone);
		
	name = KField2().AllocL();
	TRAP(err, headerTable->AddFieldL(name, ETextField));
	MY_ASSERT_EQUALS(err, KErrNone);
	
	name = KField3().AllocL();
	TRAP(err, headerTable->AddFieldL(name, EDateField));
	MY_ASSERT_EQUALS(err, KErrNone);
	
	name = KField4().AllocL();
	TRAP(err, headerTable->AddFieldL(name, ETextField));
	MY_ASSERT_EQUALS(err, KErrNone);
	name = NULL;
	
	// 3.2.1 Testing error condition as well.
	TRAP(err, headerTable->AddFieldL(name, ETextField));
	MY_ASSERT_EQUALS(err, KErrArgument);
		
	// 3.3 Create store.
	TRAP(err, headerTable->CreateStoreL());
	MY_ASSERT_EQUALS(err, KErrNone);
	
	// 3.4 Check for error text.
	TRAP(err, errorMsg.Set(headerTable->LastErrorMessageL()));
	MY_ASSERT_EQUALS(err, KErrNone);
	
	// 3.4.1 For conditional coverage.
	TRAP(err, errorMsg.Set(headerTable->LastErrorMessageL()));
	MY_ASSERT_EQUALS(err, KErrNone);	
	delete headerTable;
	}
	
	





void CTestHeaderEntry::CTestHeaderEntryCreation()
	{
	_LIT(KFunction, "CTestHeaderEntryCreation()");
	INFO_PRINTF1(KFunction);
	
	// 1. Create a default table.
	// 1.1 Create header store.
	TUid mtmId = TUid::Uid(10);
	CMsvHeaderStore* headerTable = NULL;
	TRAPD(err, headerTable = CMsvHeaderStore::NewL(mtmId, *iSession));
	MY_ASSERT_EQUALS(err, KErrNone);
		
	_LIT(KField1, "Field1");
	_LIT(KField2, "Field2");
	_LIT(KField3, "Field3");
	HBufC* name = NULL;
	TRAP(err, name = KField1().AllocL());
	MY_ASSERT_EQUALS(err, KErrNone);
	
	TRAP(err, headerTable->AddFieldL(name, EIntegerField));
	MY_ASSERT_EQUALS(err, KErrNone);
	name = KField2().AllocL();
	TRAP(err, headerTable->AddFieldL(name, ETextField));
	MY_ASSERT_EQUALS(err, KErrNone);
	name = KField3().AllocL();
	TRAP(err, headerTable->AddFieldL(name, EDateField));
	MY_ASSERT_EQUALS(err, KErrNone);
	
	TRAP(err, headerTable->CreateStoreL());
	MY_ASSERT_EQUALS(err, KErrNone);

	RPointerArray<CHeaderFields> fieldPairList;
	CleanupClosePushL(fieldPairList);
	
	CHeaderFields* metadata = new CHeaderFields();
	metadata->iUid = TUid::Uid(1000);
	CFieldPair* field = new CFieldPair();
	field->iFieldNumValue = 5555;
	TRAP(err, metadata->iFieldPairList.AppendL(field));
	MY_ASSERT_EQUALS(err, KErrNone);
	field = new CFieldPair();
	TRAP(err, field->iFieldTextValue = KFunction().AllocL());
	MY_ASSERT_EQUALS(err, KErrNone);
	TRAP(err, metadata->iFieldPairList.AppendL(field));
	MY_ASSERT_EQUALS(err, KErrNone);
	field = new CFieldPair();
	TTime date;
	date.UniversalTime();
	field->iFieldNumValue = date.Int64();
	TRAP(err, metadata->iFieldPairList.AppendL(field));
	MY_ASSERT_EQUALS(err, KErrNone);
	field = new CFieldPair();
	TRAP(err, field->iFieldTextValue = KFunction().AllocL());
	MY_ASSERT_EQUALS(err, KErrNone);
	TRAP(err, metadata->iFieldPairList.AppendL(field));
	MY_ASSERT_EQUALS(err, KErrNone);
	TRAP(err, fieldPairList.AppendL(metadata));
	MY_ASSERT_EQUALS(err, KErrNone);
	
	_LIT(KData1, "Data_\"1001\"");
	metadata = new CHeaderFields();
	metadata->iUid = TUid::Uid(1001);
	field = new CFieldPair();
	TRAP(err, field->iFieldTextValue = KData1().AllocL());
	MY_ASSERT_EQUALS(err, KErrNone);
	TRAP(err, metadata->iFieldPairList.AppendL(field));
	MY_ASSERT_EQUALS(err, KErrNone);
	TRAP(err, fieldPairList.AppendL(metadata));
	MY_ASSERT_EQUALS(err, KErrNone);
	
	_LIT(KData2, "Data_???&^1002");
	metadata = new CHeaderFields();
	metadata->iUid = TUid::Uid(1002);
	field = new CFieldPair();
	TRAP(err, field->iFieldTextValue = KData2().AllocL());
	MY_ASSERT_EQUALS(err, KErrNone);
	TRAP(err, metadata->iFieldPairList.AppendL(field));
	MY_ASSERT_EQUALS(err, KErrNone);
	TRAP(err, fieldPairList.AppendL(metadata));
	MY_ASSERT_EQUALS(err, KErrNone);
	
	_LIT(KData3, "");
	metadata = new CHeaderFields();
	metadata->iUid = TUid::Uid(1003);
	field = new CFieldPair();
	TRAP(err, field->iFieldTextValue = KData3().AllocL());
	MY_ASSERT_EQUALS(err, KErrNone);
	TRAP(err, metadata->iFieldPairList.AppendL(field));
	MY_ASSERT_EQUALS(err, KErrNone);
	TRAP(err, fieldPairList.AppendL(metadata));
	MY_ASSERT_EQUALS(err, KErrNone);
	
	// 1.2. Create the header entry.
	TRAP(err, iSession->Session().CreateHeaderEntryL(mtmId, (TMsvId)20000, fieldPairList));
	MY_ASSERT_EQUALS(err, KErrNone);
	
	// 1.3. Load the header entry.
	RPointerArray<CHeaderFields> rcvFieldPairList;
	CleanupClosePushL(rcvFieldPairList);
	TRAP(err, iSession->Session().LoadHeaderEntryL(mtmId, (TMsvId)20000, rcvFieldPairList));
	MY_ASSERT_EQUALS(err, KErrNone);
	
	// 1.4 Check the entry loaded.
	MY_ASSERT_EQUALS(fieldPairList.Count(), rcvFieldPairList.Count());
	for(TInt index=0; index<fieldPairList.Count(); index++)
		{
		CHeaderFields* SentHeaderRow = fieldPairList[index];
		CHeaderFields* RcvHeaderRow = rcvFieldPairList[index];
		MY_ASSERT_EQUALS(SentHeaderRow->iUid, RcvHeaderRow->iUid);
		MY_ASSERT_EQUALS(SentHeaderRow->iFieldPairList.Count(), RcvHeaderRow->iFieldPairList.Count());
		for(TInt i=0; i<SentHeaderRow->iFieldPairList.Count(); i++)
			{
			CFieldPair* sentFieldObj = SentHeaderRow->iFieldPairList[i];
			CFieldPair* RcvFieldObj = RcvHeaderRow->iFieldPairList[i];
			MY_ASSERT_EQUALS(RcvHeaderRow->iFieldPairList[i]->iFieldNumValue, SentHeaderRow->iFieldPairList[i]->iFieldNumValue);
			if(sentFieldObj->iFieldTextValue)
				{
				MY_ASSERT_FALSE(RcvHeaderRow->iFieldPairList[i]->iFieldTextValue->Des().Compare(SentHeaderRow->iFieldPairList[i]->iFieldTextValue->Des()));
				}
			}
		}







	// 2. Create a header entry with invalid argument.
	// 2.1. Pass the NULL value for text field. This is 
	// different than passing "", which is handled in 
	// previous usecase.
	field = fieldPairList[0]->iFieldPairList[1];
	HBufC* fieldBkp = field->iFieldTextValue;
	field->iFieldTextValue = NULL;
	
	// 2.1.1 Create the header entry.
	TRAP(err, iSession->Session().CreateHeaderEntryL(mtmId, (TMsvId)30000, fieldPairList));
	MY_ASSERT_EQUALS(err, KErrArgument);
	
	// 2.1.2 Create the header entry.
	field->iFieldTextValue = fieldBkp;
	field = fieldPairList[1]->iFieldPairList[0];
	fieldBkp = field->iFieldTextValue;
	field->iFieldTextValue = NULL;
	TRAP(err, iSession->Session().CreateHeaderEntryL(mtmId, (TMsvId)30000, fieldPairList));
	MY_ASSERT_EQUALS(err, KErrArgument);
	field->iFieldTextValue = fieldBkp;
	





	// 3. Create a header entry with insufficient values for header table.
	// 3.1 Remove a field from the primary header entry.
	field = fieldPairList[0]->iFieldPairList[1];
	delete field;
	fieldPairList[0]->iFieldPairList.Remove(1);
	
	// 3.2 Create the header entry.
	TRAP(err, iSession->Session().CreateHeaderEntryL(mtmId, (TMsvId)30000, fieldPairList));
	MY_ASSERT_EQUALS(err, KErrArgument);
	






	// 4. Similar to (1) but with default header table
	mtmId = TUid::Uid(40);
	fieldPairList.ResetAndDestroy();
	
	delete headerTable;
	TRAP(err, headerTable = CMsvHeaderStore::NewL(mtmId, *iSession));
	MY_ASSERT_EQUALS(err, KErrNone);		

	// 4.1 Create the header table.
	TRAP(err, headerTable->CreateStoreL());
	MY_ASSERT_EQUALS(err, KErrNone);

	fieldPairList.ResetAndDestroy();
	metadata = new CHeaderFields();
	metadata->iUid = TUid::Uid(7000);
	field = new CFieldPair();
	TRAP(err, field->iFieldTextValue = KField1().AllocL());
	MY_ASSERT_EQUALS(err, KErrNone);
	TRAP(err, metadata->iFieldPairList.AppendL(field));
	MY_ASSERT_EQUALS(err, KErrNone);
	TRAP(err, fieldPairList.AppendL(metadata));
	MY_ASSERT_EQUALS(err, KErrNone);
	
	metadata = new CHeaderFields();
	metadata->iUid = TUid::Uid(8000);
	field = new CFieldPair();
	TRAP(err, field->iFieldTextValue = KField2().AllocL());
	MY_ASSERT_EQUALS(err, KErrNone);
	TRAP(err, metadata->iFieldPairList.AppendL(field));
	MY_ASSERT_EQUALS(err, KErrNone);
	TRAP(err, fieldPairList.AppendL(metadata));
	MY_ASSERT_EQUALS(err, KErrNone);
	
	metadata = new CHeaderFields();
	metadata->iUid = TUid::Uid(9000);
	field = new CFieldPair();
	TRAP(err, field->iFieldTextValue = KField3().AllocL());
	MY_ASSERT_EQUALS(err, KErrNone);
	TRAP(err, metadata->iFieldPairList.AppendL(field));
	MY_ASSERT_EQUALS(err, KErrNone);
	TRAP(err, fieldPairList.AppendL(metadata));
	MY_ASSERT_EQUALS(err, KErrNone);
	
	// 4.2. Create the header entry.
	TRAP(err, iSession->Session().CreateHeaderEntryL(mtmId, (TMsvId)40000, fieldPairList));
	MY_ASSERT_EQUALS(err, KErrNone);
	
	// 4.3. Load the header entry.
	rcvFieldPairList.ResetAndDestroy();
	TRAP(err, iSession->Session().LoadHeaderEntryL(mtmId, (TMsvId)40000, rcvFieldPairList));
	MY_ASSERT_EQUALS(err, KErrNone);
	
	// 4.4 Check the entry loaded.
	MY_ASSERT_EQUALS(fieldPairList.Count(), rcvFieldPairList.Count());
	for(TInt index=0; index<fieldPairList.Count(); index++)
		{
		CHeaderFields* SentHeaderRow = fieldPairList[index];
		CHeaderFields* RcvHeaderRow = rcvFieldPairList[index];
		MY_ASSERT_EQUALS(SentHeaderRow->iUid, RcvHeaderRow->iUid);
		MY_ASSERT_EQUALS(SentHeaderRow->iFieldPairList.Count(), RcvHeaderRow->iFieldPairList.Count());
		for(TInt i=0; i<SentHeaderRow->iFieldPairList.Count(); i++)
			{
			CFieldPair* sentFieldObj = SentHeaderRow->iFieldPairList[i];
			CFieldPair* RcvFieldObj = RcvHeaderRow->iFieldPairList[i];
			MY_ASSERT_EQUALS(RcvHeaderRow->iFieldPairList[i]->iFieldNumValue, SentHeaderRow->iFieldPairList[i]->iFieldNumValue);
			if(sentFieldObj->iFieldTextValue)
				{
				MY_ASSERT_FALSE(RcvHeaderRow->iFieldPairList[i]->iFieldTextValue->Des().Compare(SentHeaderRow->iFieldPairList[i]->iFieldTextValue->Des()));
				}
			}
		}






	// 5. Create a duplicate header entry.
	TRAP(err, iSession->Session().CreateHeaderEntryL(mtmId, (TMsvId)40000, fieldPairList));
	MY_ASSERT_EQUALS(err, KErrAlreadyExists);
	





	// 6. Create a header entry under non-existent header table.
	mtmId = TUid::Uid(60);
	TRAP(err, iSession->Session().CreateHeaderEntryL(mtmId, (TMsvId)40000, fieldPairList));
	MY_ASSERT_EQUALS(err, KErrNotFound);
	




	// 7. Create a header entry, fieldPairList is set to empty.
	mtmId = TUid::Uid(40);
	fieldPairList.ResetAndDestroy();
	TRAP(err, iSession->Session().CreateHeaderEntryL(mtmId, (TMsvId)40000, fieldPairList));
	MY_ASSERT_EQUALS(err, KErrNone);
	
	
	
	
	
	
	// 8. Load an non-existent entry.
	rcvFieldPairList.ResetAndDestroy();
	TRAP(err, iSession->Session().LoadHeaderEntryL(mtmId, (TMsvId)30000, rcvFieldPairList));
	MY_ASSERT_EQUALS(err, KErrNotFound);
	





	// 9. Delete an existent entry.
	TRAP(err, iSession->Session().DeleteHeaderEntryL(mtmId, (TMsvId)40000));
	MY_ASSERT_EQUALS(err, KErrNone);
	
	TRAP(err, iSession->Session().LoadHeaderEntryL(mtmId, (TMsvId)40000, rcvFieldPairList));
	MY_ASSERT_EQUALS(err, KErrNotFound);
	





	// 10. Delete an non-existent entry.
	TRAP(err, iSession->Session().DeleteHeaderEntryL(mtmId, (TMsvId)40000));
	MY_ASSERT_EQUALS(err, KErrNotFound);

	CleanupStack::PopAndDestroy(2);		// rcvFieldPairList, fieldPairList
	delete headerTable;	
	}
	
	
	
	
	



void CTestHeaderEntry::TestUpdateHeaderEntry()
	{
	_LIT(KFunction, "TestUpdateHeaderEntry");
	INFO_PRINTF1(KFunction);	
	
	// 1. Modify a header entry.
	// 1.1. Create a header table.
	TUid mtmId = TUid::Uid(10);
	CMsvHeaderStore* headerTable = NULL;
	TRAPD(err, headerTable = CMsvHeaderStore::NewL(mtmId, *iSession));
	MY_ASSERT_EQUALS(err, KErrNone);

	_LIT(KField1, "Field1");
	_LIT(KField2, "Field2");
	_LIT(KField3, "Field3");
	HBufC* name = NULL;
	TRAP(err, name = KField1().AllocL());
	MY_ASSERT_EQUALS(err, KErrNone);
	TRAP(err, headerTable->AddFieldL(name, EIntegerField));
	MY_ASSERT_EQUALS(err, KErrNone);
	name = KField2().AllocL();
	TRAP(err, headerTable->AddFieldL(name, ETextField));
	MY_ASSERT_EQUALS(err, KErrNone);	
	name = KField3().AllocL();
	TRAP(err, headerTable->AddFieldL(name, EDateField));
	MY_ASSERT_EQUALS(err, KErrNone);	
	
	TRAP(err, headerTable->CreateStoreL());
	MY_ASSERT_EQUALS(err, KErrNone);
	delete headerTable;
	headerTable = NULL;
	
	// 1.2 Add entry to the header table.
	RPointerArray<CHeaderFields> fieldPairList;
	CleanupClosePushL(fieldPairList);
	
	CHeaderFields* metadata = new CHeaderFields();
	metadata->iUid = TUid::Uid(1000);
	CFieldPair* field = new CFieldPair();
	field->iFieldNumValue = 5555;
	TRAP(err, metadata->iFieldPairList.AppendL(field));
	MY_ASSERT_EQUALS(err, KErrNone);
	field = new CFieldPair();
	TRAP(err, field->iFieldTextValue = KFunction().AllocL());
	MY_ASSERT_EQUALS(err, KErrNone);
	TRAP(err, metadata->iFieldPairList.AppendL(field));
	MY_ASSERT_EQUALS(err, KErrNone);
	field = new CFieldPair();
	field->iFieldNumValue = 6666;
	TRAP(err, metadata->iFieldPairList.AppendL(field));
	MY_ASSERT_EQUALS(err, KErrNone);
	field = new CFieldPair();
	TRAP(err, field->iFieldTextValue = KFunction().AllocL());
	MY_ASSERT_EQUALS(err, KErrNone);
	TRAP(err, metadata->iFieldPairList.AppendL(field));
	MY_ASSERT_EQUALS(err, KErrNone);
	TRAP(err, fieldPairList.AppendL(metadata));
	MY_ASSERT_EQUALS(err, KErrNone);
	
	_LIT(KData1, "Data_\"1001\"");
	metadata = new CHeaderFields();
	metadata->iUid = TUid::Uid(1001);
	field = new CFieldPair();
	TRAP(err, field->iFieldTextValue = KData1().AllocL());
	MY_ASSERT_EQUALS(err, KErrNone);
	TRAP(err, metadata->iFieldPairList.AppendL(field));
	MY_ASSERT_EQUALS(err, KErrNone);
	TRAP(err, fieldPairList.AppendL(metadata));
	MY_ASSERT_EQUALS(err, KErrNone);
	
	_LIT(KData2, "Data_???&^1002");
	metadata = new CHeaderFields();
	metadata->iUid = TUid::Uid(1002);
	field = new CFieldPair();
	TRAP(err, field->iFieldTextValue = KData2().AllocL());
	MY_ASSERT_EQUALS(err, KErrNone);
	TRAP(err, metadata->iFieldPairList.AppendL(field));
	MY_ASSERT_EQUALS(err, KErrNone);
	TRAP(err, fieldPairList.AppendL(metadata));
	MY_ASSERT_EQUALS(err, KErrNone);
	
	_LIT(KData3, "");
	metadata = new CHeaderFields();
	metadata->iUid = TUid::Uid(1003);
	field = new CFieldPair();
	TRAP(err, field->iFieldTextValue = KData3().AllocL());
	MY_ASSERT_EQUALS(err, KErrNone);
	TRAP(err, metadata->iFieldPairList.AppendL(field));
	MY_ASSERT_EQUALS(err, KErrNone);
	TRAP(err, fieldPairList.AppendL(metadata));
	MY_ASSERT_EQUALS(err, KErrNone);
	
	TRAP(err, iSession->Session().CreateHeaderEntryL(mtmId, (TMsvId)20000, fieldPairList));
	MY_ASSERT_EQUALS(err, KErrNone);
	
	// 1.3 Update the header entry structure.
	// 1.3.1 Update primary entry.
	metadata = fieldPairList[0];
	field = metadata->iFieldPairList[0];
	field->iFieldNumValue = 1111;
	
	_LIT16(KUpdate_1, "!¬\"£$%^ &*()_+-=");
	field = metadata->iFieldPairList[1];
	delete field->iFieldTextValue;
	field->iFieldTextValue = KUpdate_1().AllocL();
	
	_LIT16(KUpdate_2, "");
	field = metadata->iFieldPairList[3];
	delete field->iFieldTextValue;
	field->iFieldTextValue = KUpdate_2().AllocL();
	
	// 1.3.2 Shifting second and third row.
	metadata = fieldPairList[1];
	fieldPairList.Remove(1);
	
	TRAP(err, fieldPairList.InsertL(metadata, 2));
	MY_ASSERT_EQUALS(err, KErrNone);
	
	// 1.3.3 Removing fourth row by updating it's UID.
	metadata = fieldPairList[3];
	metadata->iUid = TUid::Uid(1004);
	
	// 1.4 Update the header entry.
	TRAP(err, iSession->Session().UpdateHeaderEntryL(mtmId, (TMsvId)20000, fieldPairList));
	MY_ASSERT_EQUALS(err, KErrNone);

	// 1.5. Load the header entry.
	RPointerArray<CHeaderFields> rcvFieldPairList;
	CleanupClosePushL(rcvFieldPairList);
	TRAP(err, iSession->Session().LoadHeaderEntryL(mtmId, (TMsvId)20000, rcvFieldPairList));
	MY_ASSERT_EQUALS(err, KErrNone);
	
	// 1.6 Verify.
	// Correct the order, so that it is easy to verify.
	metadata = fieldPairList[1];
	fieldPairList.Remove(1);
	TRAP(err, fieldPairList.InsertL(metadata, 2));
	MY_ASSERT_EQUALS(err, KErrNone);
	
	MY_ASSERT_EQUALS(fieldPairList.Count(), rcvFieldPairList.Count());
	for(TInt index=0; index<fieldPairList.Count(); index++)
		{
		CHeaderFields* SentHeaderRow = fieldPairList[index];
		CHeaderFields* RcvHeaderRow = rcvFieldPairList[index];
		MY_ASSERT_EQUALS(SentHeaderRow->iUid, RcvHeaderRow->iUid);
		MY_ASSERT_EQUALS(SentHeaderRow->iFieldPairList.Count(), RcvHeaderRow->iFieldPairList.Count());
		for(TInt i=0; i<SentHeaderRow->iFieldPairList.Count(); i++)
			{
			CFieldPair* sentFieldObj = SentHeaderRow->iFieldPairList[i];
			CFieldPair* RcvFieldObj = RcvHeaderRow->iFieldPairList[i];
			MY_ASSERT_EQUALS(RcvHeaderRow->iFieldPairList[i]->iFieldNumValue, SentHeaderRow->iFieldPairList[i]->iFieldNumValue);
			if(sentFieldObj->iFieldTextValue)
				{
				MY_ASSERT_FALSE(RcvHeaderRow->iFieldPairList[i]->iFieldTextValue->Des().Compare(SentHeaderRow->iFieldPairList[i]->iFieldTextValue->Des()));
				}
			}
		}

	rcvFieldPairList.ResetAndDestroy();

	
	
	
	
	
	// 2. Modify header and remove/add primary entry.
	// 2.1 Update primary header entry
	_LIT16(KUpdate_5, "\b\f\t\v\\\?\'\000");
	metadata = fieldPairList[0];
	metadata->iUid = TUid::Uid(1005);
	field = metadata->iFieldPairList[1];
	delete field->iFieldTextValue;
	field->iFieldTextValue = KUpdate_5().AllocL();
	
	// 2.2 Modify second row.
	_LIT16(KUpdate_3, ";:[]{}@~'#?>		\a\n\r<,./`\\|");
	metadata = fieldPairList[1];
	field = metadata->iFieldPairList[0];
	delete field->iFieldTextValue;
	field->iFieldTextValue = KUpdate_3().AllocL();
	
	// 2.3 Modify fourth row.
	_LIT16(KUpdate_4, 
	"This is the header body. This is the header body. This is the header body. \
	This is the header body.This is the header body.This is the header body.This is the header body.\
	This is is the header body.This is the header body.This is the header body.This is the header body.\
	This is is the header body.This is the header body.This is the header body.This is the header body.\
	This is is the header body.This is the header body.This is the header body.This is the header body.\
	This is is the header body.This is the header body.This is the header body.This is the header body.\
	This is is the header body.This is the header body.This is the header body.This is the header body.\
	This is is the header body.This is the header body.This is the header body.This is the header body.\
	This is is the header body.This is the header body.This is the header body.This is the header body.\
	This is is the header body.This is the header body.This is the header body.This is the header body.\
	This is is the header body.This is the header body.This is the header body.This is the header body.\
	This is is the header body.This is the header body.This is the header body.This is the header body.\
	This is is the header body.This is the header body.This is the header body.This is the header body.\
	This is is the header body.This is the header body.This is the header body.This is the header body.\
	This is is the header body.This is the header body.This is the header body.This is the header body.\
	This is is the header body.This is the header body.This is the header body.This is the header body.\
	This is is the header body.This is the header body.This is the header body.This is the header body.\
	This is is the header body.This is the header body.This is the header body.This is the header body.\
	This is is the header body.This is the header body.This is the header body.This is the header body.\
	This is is the header body.This is the header body.This is the header body.This is the header body.\
	This is is the header body.This is the header body.This is the header body.This is the header body.\
	This is is the header body.This is the header body.This is the header body.This is the header body.\
	This is is the header body.This is the header body.This is the header body.This is the header body.\
	This is is the header body.This is the header body.This is the header body.This is the header body.\
	This is is the header body.This is the header body.This is the header body.This is the header body.\
	This is is the header body.This is the header body.This is the header body.This is the header body.\
	This is is the header body.This is the header body.This is the header body.This is the header body.\
	This is is the header body.This is the header body.This is the header body.This is the header body.\
	This is is the header body.This is the header body.This is the header body.This is the header body.\
	This is is the header body.This is the header body.This is the header body.This is the header body.\
	This is is the header body.This is the header body.This is the header body.This is the header body.\
	This is is the header body.This is the header body.This is the header body.This is the header body.\
	This is is the header body.This is the header body.This is the header body.This is the header body.\
	This is is the header body.This is the header body.This is the header body.This is the header body.\
	This is is the header body.This is the header body.This is the header body.This is the header body.\
	This is is the header body.This is the header body.This is the header body.This is the header body.\
	This is is the header body.This is the header body.This is the header body.This is the header body.\
	This is is the header body.This is the header body.This is the header body.This is the header body.\
	This is is the header body.This is the header body.This is the header body.This is the header body.\
	This is is the header body.This is the header body.This is the header body.This is the header body.\
	This is is the header body.This is the header body.This is the header body.This is the header body.\
	This is is the header body.This is the header body.This is the header body.This is the header body.\
	This is is the header body.This is the header body.This is the header body.This is the header body.\
	This is is the header body.This is the header body.This is the header body.This is the header body.\
	This is is the header body.This is the header body.This is the header body.This is the header body.\
	This is is the header body.This is the header body.This is the header body.This is the header body.\
	This is is the header body.This is the header body.This is the header body.This is the header body.\
	This is is the header body.This is the header body.This is the header body.This is the header body.\
	This is is the header body.This is the header body.This is the header body.This is the header body.\
	This is is the header body.This is the header body.This is the header body.This is the header body.\
	This is is the header body.This is the header body.This is the header body.This is the header body.\
	This is is the header body.This is the header body.This is the header body.This is the header body.\
	This is is the header body.This is the header body.This is the header body.This is the header body.\
	This is is the header body.This is the header body.This is the header body.This is the header body.\
	This is is the header body.This is the header body.This is the header body.This is the header body.\
	This is is the header body.This is the header body.This is the header body.This is the header body.\
	This is is the header body.This is the header body.This is the header body.This is the header body.\
	This is is the header body.This is the header body.This is the header body.This is the header body.\
	This is is the header body.This is the header body.This is the header body.This is the header body.\
	This is is the header body.This is the header body.This is the header body.This is the header body.");
	metadata = fieldPairList[3];
	field = metadata->iFieldPairList[0];
	delete field->iFieldTextValue;
	field->iFieldTextValue = KUpdate_4().AllocL();
	
	// 2.4 Removing third row.
	metadata = fieldPairList[2];
	delete metadata;
	fieldPairList.Remove(2);
	
	// 2.5 Update the header entry.
	TRAP(err, iSession->Session().UpdateHeaderEntryL(mtmId, (TMsvId)20000, fieldPairList));
	MY_ASSERT_EQUALS(err, KErrNone);

	// 2.6. Load the header entry.
	TRAP(err, iSession->Session().LoadHeaderEntryL(mtmId, (TMsvId)20000, rcvFieldPairList));
	MY_ASSERT_EQUALS(err, KErrNone);
	
	// 2.7. Verify.
	// Update the order of primary row for verification.
	metadata = rcvFieldPairList[2];
	rcvFieldPairList.Remove(2);
	TRAP(err, rcvFieldPairList.InsertL(metadata, 0));
	MY_ASSERT_EQUALS(err, KErrNone);

	MY_ASSERT_EQUALS(fieldPairList.Count(), rcvFieldPairList.Count());
	for(TInt index=0; index<fieldPairList.Count(); index++)
		{
		CHeaderFields* SentHeaderRow = fieldPairList[index];
		CHeaderFields* RcvHeaderRow = rcvFieldPairList[index];
		MY_ASSERT_EQUALS(SentHeaderRow->iUid, RcvHeaderRow->iUid);
		MY_ASSERT_EQUALS(SentHeaderRow->iFieldPairList.Count(), RcvHeaderRow->iFieldPairList.Count());
		for(TInt i=0; i<SentHeaderRow->iFieldPairList.Count(); i++)
			{
			CFieldPair* sentFieldObj = SentHeaderRow->iFieldPairList[i];
			CFieldPair* RcvFieldObj = RcvHeaderRow->iFieldPairList[i];
			MY_ASSERT_EQUALS(RcvHeaderRow->iFieldPairList[i]->iFieldNumValue, SentHeaderRow->iFieldPairList[i]->iFieldNumValue);
			if(sentFieldObj->iFieldTextValue)
				{
				MY_ASSERT_FALSE(RcvHeaderRow->iFieldPairList[i]->iFieldTextValue->Des().Compare(SentHeaderRow->iFieldPairList[i]->iFieldTextValue->Des()));
				}
			}
		}

	
	
	
	
	// 3. Update non-existent header entry
	TRAP(err, iSession->Session().UpdateHeaderEntryL(mtmId, (TMsvId)30000, fieldPairList));
	MY_ASSERT_EQUALS(err, KErrNotFound);
	
	
	
	
	
	

	// 4. Update header entry with some invalid values
	field = fieldPairList[0]->iFieldPairList[1];
	HBufC* fieldBkp = field->iFieldTextValue;
	field->iFieldTextValue = NULL;
	
	// 4.1 Update the header entry.
	TRAP(err, iSession->Session().UpdateHeaderEntryL(mtmId, (TMsvId)20000, fieldPairList));
	MY_ASSERT_EQUALS(err, KErrArgument);
	
	// 4.2 Update the header entry.
	field->iFieldTextValue = fieldBkp;
	field = fieldPairList[1]->iFieldPairList[0];
	fieldBkp = field->iFieldTextValue;
	field->iFieldTextValue = NULL;
	TRAP(err, iSession->Session().UpdateHeaderEntryL(mtmId, (TMsvId)20000, fieldPairList));
	MY_ASSERT_EQUALS(err, KErrArgument);
	field->iFieldTextValue = fieldBkp;

	// 4.3 Primary header has less data
	field = fieldPairList[0]->iFieldPairList[1];
	delete field;
	fieldPairList[0]->iFieldPairList.Remove(1);
	
	// 4.4 Create the header entry.
	TRAP(err, iSession->Session().CreateHeaderEntryL(mtmId, (TMsvId)20000, fieldPairList));
	MY_ASSERT_EQUALS(err, KErrArgument);



	


	// 5. Empty fieldPairList
	rcvFieldPairList.ResetAndDestroy();
	TRAP(err, iSession->Session().UpdateHeaderEntryL(mtmId, (TMsvId)20000, rcvFieldPairList));
	MY_ASSERT_EQUALS(err, KErrNone);
	
	// 5.1 Load the entry.
	TRAP(err, iSession->Session().LoadHeaderEntryL(mtmId, (TMsvId)20000, rcvFieldPairList));
	MY_ASSERT_EQUALS(err, KErrNotFound);

	fieldPairList.ResetAndDestroy();
	rcvFieldPairList.ResetAndDestroy();
	CleanupStack::PopAndDestroy(2);		// rcvFieldPairList, fieldPairList
	}
	
	
	
	
void CTestHeaderEntry::TestIsHeaderTableExists()
	{
	_LIT(KFunction, "TestIsHeaderTableExists");
	INFO_PRINTF1(KFunction);	
	
	// 1. Test existence of a non-existent header table.
	TBool isTableExists = ETrue;
	TUid mtmId = TUid::Uid(10);
	TRAPD(err, isTableExists = CMsvHeaderStore::DoesStoreExistsL(mtmId, *iSession));
	MY_ASSERT_EQUALS(err, KErrNone);
	MY_ASSERT_FALSE(isTableExists);
		
	// 2. Test existence of a existent header table.
	// 1.1. Create a header table.	
	CMsvHeaderStore* headerTable = NULL;
	TRAP(err, headerTable = CMsvHeaderStore::NewL(mtmId, *iSession));
	MY_ASSERT_EQUALS(err, KErrNone);
	TRAP(err, headerTable->CreateStoreL());
	MY_ASSERT_EQUALS(err, KErrNone);
	delete headerTable;
	
	TRAP(err, isTableExists = CMsvHeaderStore::DoesStoreExistsL(mtmId, *iSession));
	MY_ASSERT_EQUALS(err, KErrNone);
	MY_ASSERT_TRUE(isTableExists);
	}
	
		



TMsvId CTestHeaderEntry::CreateEntryL(TMsvId aParentId, TUid aType)
	{
	_LIT(KFunction, "CreateEntryL");
	INFO_PRINTF1(KFunction);

	CMsvEntry* parentEntry = iSession->GetEntryL(aParentId);
		
	TMsvEntry entry;
	entry.iType = aType;
	entry.iMtm = KUidMsvLocalServiceMtm;
	entry.iServiceId = KMsvLocalServiceIndexEntryId;
	entry.SetParent(aParentId);
	
	TRAPD(err, parentEntry->CreateL(entry));
	MY_ASSERT_EQUALS(err, KErrNone);
	CActiveScheduler::Start();
	MY_ASSERT_EQUALS(iMsvSesionObserver->iType, MMsvSessionObserver::EMsvEntriesCreated);
	return iMsvSesionObserver->iArg1;
	}



	
CTestSuite* CTestHeaderEntry::CreateSuiteL(const TDesC& aName)
	{
	SUB_SUITE;

	ADD_ASYNC_TEST_STEP(CTestHeaderTableCreation);
	ADD_ASYNC_TEST_STEP(CTestHeaderEntryCreation);
	ADD_ASYNC_TEST_STEP(TestUpdateHeaderEntry);
	ADD_ASYNC_TEST_STEP(TestIsHeaderTableExists);
	END_SUITE;
	}
