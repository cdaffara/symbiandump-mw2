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

#include "t_msv_messagedbadapter_step.h"


#define DEFAULT_RETURN KErrNone
#define MY_ASSERT_TRUE(a)  \
		INFO_PRINTF1(KFunction);   \
		ASSERT_TRUE(a);
#define MY_ASSERT_FALSE(a)  \
		INFO_PRINTF1(KFunction);   \
		ASSERT_FALSE(a);
#define MY_ASSERT_EQUALS(a,b)  \
		INFO_PRINTF1(KFunction);   \
		ASSERT_EQUALS(a,b);
		

_LIT(KTestDbFileName, "\\messaging.db");




CTestMessageDbAdapter::CTestMessageDbAdapter()
	{
	}



CTestMessageDbAdapter::~CTestMessageDbAdapter()
	{
	iFieldStruct.Close();		
	}



void CTestMessageDbAdapter::SetupL()
	{
	_LIT(KFunction, "SetupL");
	INFO_PRINTF1(KFunction);

    __UHEAP_MARK;
    
    // Delete C://messaging.db
    TParse parse;
	TPtrC drive = TDriveUnit(RFs::GetSystemDrive()).Name();
	parse.Set(KTestDbFileName, &drive, NULL);
	TFileName dbName = parse.FullName();
	RSqlDatabase::Delete(dbName);

	// Create fresh C://messaging.db
	TInt sysDrive = (TInt)RFs::GetSystemDrive();
	TRAPD(error, CMsvDBAdapter::CreateDBL(TDriveNumber(sysDrive)));
	MY_ASSERT_EQUALS(error, KErrNone);

	// Creating DB interface
	TRAP(error, iDatabase = CMsvDBAdapter::NewL());
	MY_ASSERT_EQUALS(error, KErrNone);
	
	TMsvId maxId = 0;
	TRAP(error, iDatabase->AttachDBL(TDriveUnit(sysDrive), 1, maxId));
	MY_ASSERT_EQUALS(error, KErrNone);
	
	// Require to identify message server current drive.
	CMsvPreferredDriveList* driveList;
	TRAP(error, driveList = CMsvPreferredDriveList::CreateL());
	MY_ASSERT_EQUALS(error, KErrNone);
	
	TMsvPreferredDrive driveNode;
	driveNode.driveNum = TDriveNumber(2);
	driveNode.driveId = 1;
	driveList->Insert(driveNode, 0);
	driveList->SetCurrentDriveIndex(0);

	TRAP(error, iMsgDBAdapter = CMsvMessageDBAdapter::NewL(iDatabase));
	MY_ASSERT_EQUALS(error, KErrNone);
	}




void CTestMessageDbAdapter::TearDownL()
	{
	_LIT(KFunction, "TearDownL");
	INFO_PRINTF1(KFunction);
	
	TFileName dbName(KTestDbFileName);
	TInt err = RSqlDatabase::Delete(dbName);

 	delete iMsgDBAdapter;
	delete iDatabase;
	iFieldStruct.ResetAndDestroy();
	
	CMsvPreferredDriveList* driveList =	CMsvPreferredDriveList::GetDriveList();
	delete driveList;
	
	__UHEAP_MARKEND;
	}




void CTestMessageDbAdapter::TestHeaderTableCreation()
	{
	_LIT(KFunction, "TestHeaderTableCreation");
	INFO_PRINTF1(KFunction);	
	



	// 1. Test with no additional columns in iFieldStruct.
	TPtrC errorMessage;
	TUid mtmId = TUid::Uid(10);
	TRAPD(err, iMsgDBAdapter->CreateHeaderTableL(mtmId, iFieldStruct, errorMessage));
	MY_ASSERT_EQUALS(err, KErrNone);
	
	CMsvMessageDBAdapter::CHeaderMetaData* metadata = iMsgDBAdapter->iHeaderDataList[0];
	MY_ASSERT_EQUALS(iMsgDBAdapter->iHeaderDataList[0]->iMtmId, mtmId);
	MY_ASSERT_EQUALS(iMsgDBAdapter->iHeaderDataList[0]->iFieldList.Count(), 1);
	MY_ASSERT_EQUALS(iMsgDBAdapter->iHeaderDataList[0]->iFieldList[0]->iType, ESqlText);
	MY_ASSERT_FALSE(iMsgDBAdapter->iHeaderDataList[0]->iFieldList[0]->iName->Des().Compare(_L("Details")));
	




	// 2. Test with columns of all type.
	_LIT(KField1, "Field1");
	_LIT(KField2, "Field2");
	_LIT(KField3, "Field3");
	CFieldPair* field = new CFieldPair();
	TRAP(err, field->iFieldName = KField1().AllocL());
	MY_ASSERT_EQUALS(err, KErrNone);
	field->iFieldType = EIntegerField;
	TRAP(err, iFieldStruct.AppendL(field));
	MY_ASSERT_EQUALS(err, KErrNone);
	
	field = new CFieldPair();
	TRAP(err, field->iFieldName = KField2().AllocL());
	MY_ASSERT_EQUALS(err, KErrNone);
	field->iFieldType = ETextField;
	TRAP(err, iFieldStruct.AppendL(field));
	MY_ASSERT_EQUALS(err, KErrNone);
	
	field = new CFieldPair();
	TRAP(err, field->iFieldName = KField3().AllocL());
	MY_ASSERT_EQUALS(err, KErrNone);
	field->iFieldType = EDateField;
	TRAP(err, iFieldStruct.AppendL(field));
	MY_ASSERT_EQUALS(err, KErrNone);
	
	mtmId = TUid::Uid(20);
	TRAP(err, iMsgDBAdapter->CreateHeaderTableL(mtmId, iFieldStruct, errorMessage));
	MY_ASSERT_EQUALS(err, KErrNone);
	
	// Check Metadata structure
	metadata = iMsgDBAdapter->iHeaderDataList[0];
	MY_ASSERT_EQUALS(iMsgDBAdapter->iHeaderDataList[0]->iMtmId, mtmId);
	MY_ASSERT_EQUALS(iMsgDBAdapter->iHeaderDataList[0]->iFieldList.Count(), 4);
	MY_ASSERT_EQUALS(iMsgDBAdapter->iHeaderDataList[0]->iFieldList[0]->iType, ESqlInt);
	MY_ASSERT_FALSE(iMsgDBAdapter->iHeaderDataList[0]->iFieldList[0]->iName->Des().Compare(_L("Field1")));
	MY_ASSERT_EQUALS(iMsgDBAdapter->iHeaderDataList[0]->iFieldList[1]->iType, ESqlText);
	MY_ASSERT_FALSE(iMsgDBAdapter->iHeaderDataList[0]->iFieldList[1]->iName->Des().Compare(_L("Field2")));
	MY_ASSERT_EQUALS(iMsgDBAdapter->iHeaderDataList[0]->iFieldList[2]->iType, ESqlInt64);
	MY_ASSERT_FALSE(iMsgDBAdapter->iHeaderDataList[0]->iFieldList[2]->iName->Des().Compare(_L("Field3")));
	MY_ASSERT_EQUALS(iMsgDBAdapter->iHeaderDataList[0]->iFieldList[3]->iType, ESqlText);
	MY_ASSERT_FALSE(iMsgDBAdapter->iHeaderDataList[0]->iFieldList[3]->iName->Des().Compare(_L("Details")));
	
	
	
	




	// 4. Test with duplicate column in iFieldStruct.
	iFieldStruct.ResetAndDestroy();
	field = new CFieldPair();
	TRAP(err, field->iFieldName = KField1().AllocL());
	MY_ASSERT_EQUALS(err, KErrNone);
	field->iFieldType = ETextField;
	TRAP(err, iFieldStruct.AppendL(field));
	MY_ASSERT_EQUALS(err, KErrNone);
	
	field = new CFieldPair();
	TRAP(err, field->iFieldName = KField1().AllocL());
	MY_ASSERT_EQUALS(err, KErrNone);
	field->iFieldType = EDateField;
	TRAP(err, iFieldStruct.AppendL(field));
	MY_ASSERT_EQUALS(err, KErrNone);
	
	mtmId = TUid::Uid(30);
	TRAP(err, iMsgDBAdapter->CreateHeaderTableL(mtmId, iFieldStruct, errorMessage));
	MY_ASSERT_EQUALS(err, KErrGeneral);
	
	
	
	
	
	
	
	
	// 5. Test with duplicate MTM Id.
	// SHOULD PASS.
	mtmId = TUid::Uid(10);
	TRAP(err, iMsgDBAdapter->CreateHeaderTableL(mtmId, iFieldStruct, errorMessage));
	MY_ASSERT_EQUALS(err, KErrNone);

	
	
	
	
	
	
	
	// 6. Test with wrong colum-names.

	// 6.1 Column name with comma.	
	iFieldStruct.ResetAndDestroy();
	field = new CFieldPair();
	_LIT(KField4, "Field,1");
	TRAP(err, field->iFieldName = KField4().AllocL());
	MY_ASSERT_EQUALS(err, KErrNone);
	field->iFieldType = ETextField;
	TRAP(err, iFieldStruct.AppendL(field));
	MY_ASSERT_EQUALS(err, KErrNone);
	
	mtmId = TUid::Uid(30);
	TRAP(err, iMsgDBAdapter->CreateHeaderTableL(mtmId, iFieldStruct, errorMessage));
	MY_ASSERT_EQUALS(err, KErrNone);
	
	// 6.2 Column name with ",./&$@'
	iFieldStruct.ResetAndDestroy();
	field = new CFieldPair();
	_LIT(KField5, "Fie&$£ld1");
	TRAP(err, field->iFieldName = KField5().AllocL());
	MY_ASSERT_EQUALS(err, KErrNone);
	field->iFieldType = ETextField;
	TRAP(err, iFieldStruct.AppendL(field));
	MY_ASSERT_EQUALS(err, KErrNone);
	
	mtmId = TUid::Uid(40);
	TRAP(err, iMsgDBAdapter->CreateHeaderTableL(mtmId, iFieldStruct, errorMessage));
	MY_ASSERT_EQUALS(err, KErrNone);

	// 6.3 A very large column name.
	_LIT(KField6, "TEST_TO_CHECK_A_VERY_LONG_FIELD_NAME_THIS_SHOULD_PASS");
	iFieldStruct.ResetAndDestroy();
	field = new CFieldPair();
	TRAP(err, field->iFieldName = KField6().AllocL());
	MY_ASSERT_EQUALS(err, KErrNone);
	field->iFieldType = ETextField;
	TRAP(err, iFieldStruct.AppendL(field));
	MY_ASSERT_EQUALS(err, KErrNone);
	
	mtmId = TUid::Uid(50);
	TRAP(err, iMsgDBAdapter->CreateHeaderTableL(mtmId, iFieldStruct, errorMessage));
	MY_ASSERT_EQUALS(err, KErrNone);  
	
	
	

	
	// 7. FieldStruct have NULL column name.
	iFieldStruct.ResetAndDestroy();
	field = new CFieldPair();
	field->iFieldType = ETextField;
	TRAP(err, iFieldStruct.AppendL(field));
	MY_ASSERT_EQUALS(err, KErrNone);
	
	mtmId = TUid::Uid(60);
	TRAP(err, iMsgDBAdapter->CreateHeaderTableL(mtmId, iFieldStruct, errorMessage));
	MY_ASSERT_EQUALS(err, KErrArgument);
	}













void CTestMessageDbAdapter::TestHeaderEntry()
	{
	_LIT(KFunction, "TestHeaderEntry");
	INFO_PRINTF1(KFunction);	
	




	// 1. Create and load an header entry.
	_LIT(KField1, "Field1");
	_LIT(KField2, "Field2");
	_LIT(KField3, "Field3");
	TPtrC errorMessage;
	TUid mtmId = TUid::Uid(10);
	CFieldPair* field = new CFieldPair();
	TRAPD(err, field->iFieldName = KField1().AllocL());
	MY_ASSERT_EQUALS(err, KErrNone);
	field->iFieldType = EIntegerField;
	TRAP(err, iFieldStruct.AppendL(field));
	MY_ASSERT_EQUALS(err, KErrNone);
	
	field = new CFieldPair();
	TRAP(err, field->iFieldName = KField2().AllocL());
	MY_ASSERT_EQUALS(err, KErrNone);
	field->iFieldType = ETextField;
	TRAP(err, iFieldStruct.AppendL(field));
	MY_ASSERT_EQUALS(err, KErrNone);
	
	field = new CFieldPair();
	TRAP(err, field->iFieldName = KField3().AllocL());
	MY_ASSERT_EQUALS(err, KErrNone);
	field->iFieldType = EDateField;
	TRAP(err, iFieldStruct.AppendL(field));
	MY_ASSERT_EQUALS(err, KErrNone);

	// 1.1 Create the header table.
	TRAP(err, iMsgDBAdapter->CreateHeaderTableL(mtmId, iFieldStruct, errorMessage));
	MY_ASSERT_EQUALS(err, KErrNone);

	RPointerArray<CHeaderFields> fieldPairList;
	CHeaderFields* metadata = new CHeaderFields();
	metadata->iUid = TUid::Uid(1000);
	field = new CFieldPair();
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
	TRAP(err, iMsgDBAdapter->CreateHeaderEntryL(mtmId, (TMsvId)20000, fieldPairList));
	MY_ASSERT_EQUALS(err, KErrNone);
	
	// 1.3. Load the header entry.
	RPointerArray<CHeaderFields> rcvFieldPairList;
	TRAP(err, iMsgDBAdapter->LoadHeaderEntryL(mtmId, (TMsvId)20000, rcvFieldPairList));
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
	TRAP(err, iMsgDBAdapter->CreateHeaderEntryL(mtmId, (TMsvId)30000, fieldPairList));
	MY_ASSERT_EQUALS(err, KErrArgument);
	
	// 2.1.2 Create the header entry.
	field->iFieldTextValue = fieldBkp;
	field = fieldPairList[1]->iFieldPairList[0];
	fieldBkp = field->iFieldTextValue;
	field->iFieldTextValue = NULL;
	TRAP(err, iMsgDBAdapter->CreateHeaderEntryL(mtmId, (TMsvId)30000, fieldPairList));
	MY_ASSERT_EQUALS(err, KErrArgument);
	field->iFieldTextValue = fieldBkp;
	





	// 3. Create a header entry with insufficient values for header table.
	// 3.1 Remove a field from the primary header entry.
	field = fieldPairList[0]->iFieldPairList[1];
	delete field;
	fieldPairList[0]->iFieldPairList.Remove(1);
	
	// 3.2 Create the header entry.
	TRAP(err, iMsgDBAdapter->CreateHeaderEntryL(mtmId, (TMsvId)30000, fieldPairList));
	MY_ASSERT_EQUALS(err, KErrArgument);
	






	// 4. Similar to (1) but with default header table
	mtmId = TUid::Uid(40);
	iFieldStruct.ResetAndDestroy();
	
	// 4.1 Create the header table.
	TRAP(err, iMsgDBAdapter->CreateHeaderTableL(mtmId, iFieldStruct, errorMessage));
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
	TRAP(err, iMsgDBAdapter->CreateHeaderEntryL(mtmId, (TMsvId)40000, fieldPairList));
	MY_ASSERT_EQUALS(err, KErrNone);
	
	// 4.3. Load the header entry.
	rcvFieldPairList.ResetAndDestroy();
	TRAP(err, iMsgDBAdapter->LoadHeaderEntryL(mtmId, (TMsvId)40000, rcvFieldPairList));
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
	TRAP(err, iMsgDBAdapter->CreateHeaderEntryL(mtmId, (TMsvId)40000, fieldPairList));
	MY_ASSERT_EQUALS(err, KErrAlreadyExists);
	





	// 6. Create a header entry under non-existent header table.
	mtmId = TUid::Uid(60);
	TRAP(err, iMsgDBAdapter->CreateHeaderEntryL(mtmId, (TMsvId)40000, fieldPairList));
	MY_ASSERT_EQUALS(err, KErrNotFound);
	




	// 7. Create a header entry, fieldPairList is set to empty.
	mtmId = TUid::Uid(40);
	fieldPairList.ResetAndDestroy();
	TRAP(err, iMsgDBAdapter->CreateHeaderEntryL(mtmId, (TMsvId)40000, fieldPairList));
	MY_ASSERT_EQUALS(err, KErrNone);
	
	
	
	
	
	
	// 8. Load an non-existent entry.
	rcvFieldPairList.ResetAndDestroy();
	TRAP(err, iMsgDBAdapter->LoadHeaderEntryL(mtmId, (TMsvId)30000, rcvFieldPairList));
	MY_ASSERT_EQUALS(err, KErrNotFound);
	





	// 9. Delete an existent entry.
	TRAP(err, iMsgDBAdapter->DeleteHeaderEntryL(mtmId, (TMsvId)40000));
	MY_ASSERT_EQUALS(err, KErrNone);
	
	TRAP(err, iMsgDBAdapter->LoadHeaderEntryL(mtmId, (TMsvId)40000, rcvFieldPairList));
	MY_ASSERT_EQUALS(err, KErrNotFound);
	





	// 10. Delete an non-existent entry.
	TRAP(err, iMsgDBAdapter->DeleteHeaderEntryL(mtmId, (TMsvId)40000));
	MY_ASSERT_EQUALS(err, KErrNotFound);
	
	rcvFieldPairList.ResetAndDestroy();
	fieldPairList.ResetAndDestroy();
	}







void CTestMessageDbAdapter::TestUpdateHeaderEntry()
	{
	_LIT(KFunction, "TestUpdateHeaderEntry");
	INFO_PRINTF1(KFunction);	
	



	// 1. Modify a header entry.
	// 1.1. Create a header table.
	_LIT(KField1, "Field1");
	_LIT(KField2, "Field2");
	_LIT(KField3, "Field3");
	TPtrC errorMessage;
	TUid mtmId = TUid::Uid(10);
	CFieldPair* field = new CFieldPair();
	TRAPD(err, field->iFieldName = KField1().AllocL());
	MY_ASSERT_EQUALS(err, KErrNone);
	field->iFieldType = EIntegerField;
	TRAP(err, iFieldStruct.AppendL(field));
	MY_ASSERT_EQUALS(err, KErrNone);
	
	field = new CFieldPair();
	TRAP(err, field->iFieldName = KField2().AllocL());
	MY_ASSERT_EQUALS(err, KErrNone);
	field->iFieldType = ETextField;
	TRAP(err, iFieldStruct.AppendL(field));
	MY_ASSERT_EQUALS(err, KErrNone);
	
	field = new CFieldPair();
	TRAP(err, field->iFieldName = KField3().AllocL());
	MY_ASSERT_EQUALS(err, KErrNone);
	field->iFieldType = EDateField;
	TRAP(err, iFieldStruct.AppendL(field));
	MY_ASSERT_EQUALS(err, KErrNone);

	TRAP(err, iMsgDBAdapter->CreateHeaderTableL(mtmId, iFieldStruct, errorMessage));
	MY_ASSERT_EQUALS(err, KErrNone);

	// 1.2 Add entry to the header table.
	RPointerArray<CHeaderFields> fieldPairList;
	CHeaderFields* metadata = new CHeaderFields();
	metadata->iUid = TUid::Uid(1000);
	field = new CFieldPair();
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
	
	TRAP(err, iMsgDBAdapter->CreateHeaderEntryL(mtmId, (TMsvId)20000, fieldPairList));
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
	TRAP(err, iMsgDBAdapter->UpdateHeaderEntryL(mtmId, (TMsvId)20000, fieldPairList));
	MY_ASSERT_EQUALS(err, KErrNone);

	// 1.5. Load the header entry.
	RPointerArray<CHeaderFields> rcvFieldPairList;
	TRAP(err, iMsgDBAdapter->LoadHeaderEntryL(mtmId, (TMsvId)20000, rcvFieldPairList));
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
	TRAP(err, iMsgDBAdapter->UpdateHeaderEntryL(mtmId, (TMsvId)20000, fieldPairList));
	MY_ASSERT_EQUALS(err, KErrNone);

	// 2.6. Load the header entry.
	TRAP(err, iMsgDBAdapter->LoadHeaderEntryL(mtmId, (TMsvId)20000, rcvFieldPairList));
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
	TRAP(err, iMsgDBAdapter->UpdateHeaderEntryL(mtmId, (TMsvId)30000, fieldPairList));
	MY_ASSERT_EQUALS(err, KErrNotFound);
	
	
	
	
	
	

	// 4. Update header entry with some invalid values
	field = fieldPairList[0]->iFieldPairList[1];
	HBufC* fieldBkp = field->iFieldTextValue;
	field->iFieldTextValue = NULL;
	
	// 4.1 Update the header entry.
	TRAP(err, iMsgDBAdapter->UpdateHeaderEntryL(mtmId, (TMsvId)20000, fieldPairList));
	MY_ASSERT_EQUALS(err, KErrArgument);
	
	// 4.2 Update the header entry.
	field->iFieldTextValue = fieldBkp;
	field = fieldPairList[1]->iFieldPairList[0];
	fieldBkp = field->iFieldTextValue;
	field->iFieldTextValue = NULL;
	TRAP(err, iMsgDBAdapter->UpdateHeaderEntryL(mtmId, (TMsvId)20000, fieldPairList));
	MY_ASSERT_EQUALS(err, KErrArgument);
	field->iFieldTextValue = fieldBkp;

	// 4.3 Primary header has less data
	field = fieldPairList[0]->iFieldPairList[1];
	delete field;
	fieldPairList[0]->iFieldPairList.Remove(1);
	
	// 4.4 Create the header entry.
	TRAP(err, iMsgDBAdapter->CreateHeaderEntryL(mtmId, (TMsvId)20000, fieldPairList));
	MY_ASSERT_EQUALS(err, KErrArgument);



	


	// 5. Empty fieldPairList
	rcvFieldPairList.ResetAndDestroy();
	TRAP(err, iMsgDBAdapter->UpdateHeaderEntryL(mtmId, (TMsvId)20000, rcvFieldPairList));
	MY_ASSERT_EQUALS(err, KErrNone);
	
	// 5.1 Load the entry.
	TRAP(err, iMsgDBAdapter->LoadHeaderEntryL(mtmId, (TMsvId)20000, rcvFieldPairList));
	MY_ASSERT_EQUALS(err, KErrNotFound);

	fieldPairList.ResetAndDestroy();
	rcvFieldPairList.ResetAndDestroy();	
	}
	
	
	
	
	


void CTestMessageDbAdapter::TestIsHeaderTableExists()
	{
	_LIT(KFunction, "TestIsHeaderTableExists");
	INFO_PRINTF1(KFunction);	
	
	// 1. Test existence of a non-existent header table.
	TUid mtmId = TUid::Uid(10);
	
	TBool isTableExists = ETrue;
	TRAPD(err, isTableExists = iMsgDBAdapter->IsHeaderTableExistsL(mtmId));
	MY_ASSERT_EQUALS(err, KErrNone);
	MY_ASSERT_FALSE(isTableExists);
	
	// 2. Test existence of a existent header table.
	TPtrC errorMessage;
	TRAP(err, iMsgDBAdapter->CreateHeaderTableL(mtmId, iFieldStruct, errorMessage));
	MY_ASSERT_EQUALS(err, KErrNone);
	TRAP(err, isTableExists = iMsgDBAdapter->IsHeaderTableExistsL(mtmId));
	MY_ASSERT_EQUALS(err, KErrNone);
	MY_ASSERT_TRUE(isTableExists);
	}
	
	





void CTestMessageDbAdapter::TestCopyHeaderEntry()
	{
	_LIT(KFunction, "TestCopyHeaderEntry");
	INFO_PRINTF1(KFunction);	
	
	TMsvId srcId = 0;
	TMsvId destId = 0;
	TUid mtmId = TUid::Null();
	
	// 0 Create header table and test entries.
	// 0.1 Header table.
	_LIT(KField1, "Field1");
	_LIT(KField2, "Field2");
	_LIT(KField3, "Field3");
	TPtrC errorMessage;
	mtmId = TUid::Uid(10);
	CFieldPair* field = new CFieldPair();
	TRAPD(err, field->iFieldName = KField1().AllocL());
	MY_ASSERT_EQUALS(err, KErrNone);
	field->iFieldType = EIntegerField;
	TRAP(err, iFieldStruct.AppendL(field));
	ASSERT_EQUALS(err, KErrNone);
	
	field = new CFieldPair();
	TRAP(err, field->iFieldName = KField2().AllocL());
	ASSERT_EQUALS(err, KErrNone);
	field->iFieldType = ETextField;
	TRAP(err, iFieldStruct.AppendL(field));
	ASSERT_EQUALS(err, KErrNone);
	
	field = new CFieldPair();
	TRAP(err, field->iFieldName = KField3().AllocL());
	ASSERT_EQUALS(err, KErrNone);
	field->iFieldType = EDateField;
	TRAP(err, iFieldStruct.AppendL(field));
	ASSERT_EQUALS(err, KErrNone);

	TRAP(err, iMsgDBAdapter->CreateHeaderTableL(mtmId, iFieldStruct, errorMessage));
	MY_ASSERT_EQUALS(err, KErrNone);
	
	// 0.2 Test header entry.
	RPointerArray<CHeaderFields> fieldPairList;
	CHeaderFields* metadata = new CHeaderFields();
	metadata->iUid = TUid::Uid(1000);
	field = new CFieldPair();
	field->iFieldNumValue = 5555;
	TRAP(err, metadata->iFieldPairList.AppendL(field));
	ASSERT_EQUALS(err, KErrNone);
	field = new CFieldPair();
	TRAP(err, field->iFieldTextValue = KFunction().AllocL());
	ASSERT_EQUALS(err, KErrNone);
	TRAP(err, metadata->iFieldPairList.AppendL(field));
	ASSERT_EQUALS(err, KErrNone);
	field = new CFieldPair();
	TTime date;
	date.UniversalTime();
	field->iFieldNumValue = date.Int64();
	TRAP(err, metadata->iFieldPairList.AppendL(field));
	ASSERT_EQUALS(err, KErrNone);
	field = new CFieldPair();
	TRAP(err, field->iFieldTextValue = KFunction().AllocL());
	ASSERT_EQUALS(err, KErrNone);
	TRAP(err, metadata->iFieldPairList.AppendL(field));
	ASSERT_EQUALS(err, KErrNone);
	TRAP(err, fieldPairList.AppendL(metadata));
	ASSERT_EQUALS(err, KErrNone);
	
	_LIT(KData1, "Data_\"1001\"");
	metadata = new CHeaderFields();
	metadata->iUid = TUid::Uid(1001);
	field = new CFieldPair();
	TRAP(err, field->iFieldTextValue = KData1().AllocL());
	ASSERT_EQUALS(err, KErrNone);
	TRAP(err, metadata->iFieldPairList.AppendL(field));
	ASSERT_EQUALS(err, KErrNone);
	TRAP(err, fieldPairList.AppendL(metadata));
	ASSERT_EQUALS(err, KErrNone);
	
	_LIT(KData2, "Data_???&^1002");
	metadata = new CHeaderFields();
	metadata->iUid = TUid::Uid(1002);
	field = new CFieldPair();
	TRAP(err, field->iFieldTextValue = KData2().AllocL());
	ASSERT_EQUALS(err, KErrNone);
	TRAP(err, metadata->iFieldPairList.AppendL(field));
	ASSERT_EQUALS(err, KErrNone);
	TRAP(err, fieldPairList.AppendL(metadata));
	ASSERT_EQUALS(err, KErrNone);
	
	_LIT(KData3, "");
	metadata = new CHeaderFields();
	metadata->iUid = TUid::Uid(1003);
	field = new CFieldPair();
	TRAP(err, field->iFieldTextValue = KData3().AllocL());
	ASSERT_EQUALS(err, KErrNone);
	TRAP(err, metadata->iFieldPairList.AppendL(field));
	ASSERT_EQUALS(err, KErrNone);
	TRAP(err, fieldPairList.AppendL(metadata));
	MY_ASSERT_EQUALS(err, KErrNone);
	
	srcId = (TMsvId)20000;
	TRAP(err, iMsgDBAdapter->CreateHeaderEntryL(mtmId, srcId, fieldPairList));
	MY_ASSERT_EQUALS(err, KErrNone);

	
	
	
	
	// 1. Copy a non-existent header entry.
	TRAP(err, iMsgDBAdapter->CopyHeaderEntryL(mtmId, TMsvId(100), TMsvId(200)));
	MY_ASSERT_EQUALS(err, KErrNotFound);
	
	
	
	
	
	// 2. Copy an existing entry.
	// 2.1 Copy
	srcId = (TMsvId)20000;
	destId = srcId + 1;
	TRAP(err, iMsgDBAdapter->CopyHeaderEntryL(mtmId, srcId, destId));
	MY_ASSERT_EQUALS(err, KErrNone);
	
	// 2.2 Load and check the destination entry.
	// Load the header entry.
	RPointerArray<CHeaderFields> rcvFieldPairList;
	TRAP(err, iMsgDBAdapter->LoadHeaderEntryL(mtmId, destId, rcvFieldPairList));
	MY_ASSERT_EQUALS(err, KErrNone);
	
	// Check the entry loaded.
	ASSERT_EQUALS(fieldPairList.Count(), rcvFieldPairList.Count());
	for(TInt index=0; index<fieldPairList.Count(); index++)
		{
		CHeaderFields* SentHeaderRow = fieldPairList[index];
		CHeaderFields* RcvHeaderRow = rcvFieldPairList[index];
		ASSERT_EQUALS(SentHeaderRow->iUid, RcvHeaderRow->iUid);
		ASSERT_EQUALS(SentHeaderRow->iFieldPairList.Count(), RcvHeaderRow->iFieldPairList.Count());
		for(TInt i=0; i<SentHeaderRow->iFieldPairList.Count(); i++)
			{
			CFieldPair* sentFieldObj = SentHeaderRow->iFieldPairList[i];
			CFieldPair* RcvFieldObj = RcvHeaderRow->iFieldPairList[i];
			ASSERT_EQUALS(RcvHeaderRow->iFieldPairList[i]->iFieldNumValue, SentHeaderRow->iFieldPairList[i]->iFieldNumValue);
			if(sentFieldObj->iFieldTextValue)
				{
				MY_ASSERT_FALSE(RcvHeaderRow->iFieldPairList[i]->iFieldTextValue->Des().Compare(SentHeaderRow->iFieldPairList[i]->iFieldTextValue->Des()));
				}
			}
		}
	
	
	
	
	// 3. Attempt to copy to an already existing destination id.
	srcId = (TMsvId)20000;
	destId = srcId + 1;
	TRAP(err, iMsgDBAdapter->CopyHeaderEntryL(mtmId, srcId, destId));
	MY_ASSERT_EQUALS(err, KErrAlreadyExists);
	
	
	rcvFieldPairList.ResetAndDestroy();
	fieldPairList.ResetAndDestroy();
	}



CTestSuite* CTestMessageDbAdapter::CreateSuiteL(const TDesC& aName)
	{
	SUB_SUITE;
	ADD_ASYNC_TEST_STEP(TestHeaderTableCreation);
	ADD_ASYNC_TEST_STEP(TestHeaderEntry);
	ADD_ASYNC_TEST_STEP(TestUpdateHeaderEntry);
	ADD_ASYNC_TEST_STEP(TestIsHeaderTableExists);
	ADD_ASYNC_TEST_STEP(TestCopyHeaderEntry);
	END_SUITE;
	}

