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
// Contributors: Ashok Kumar
//
// Description:
//
 
#include "t_msv_version1version2converter.h"
#include "cmsvversion1version2converter.h"

//#include <msvapi.h>
#include <f32file.h>

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


#ifdef SYMBIAN_MESSAGESTORE_UNIT_TESTCODE
_LIT(KDbFileName, "d:\\messaging.db");
#else
_LIT(KDbFileName, "[1000484B]messaging.db");
#endif

_LIT(KDriveD,"D:\\");
_LIT(KConverterMail2Folder, "C:\\msgtest\\framework\\msgsqltest\\testdata\\converter\\Mail2");
_LIT(KConverterIndexFile, "C:\\msgtest\\framework\\msgsqltest\\testdata\\converter\\Mail2\\index");
_LIT(KVersion1MessageStore, "C:\\msgtest\\framework\\msgsqltest\\testdata\\converter\\messaging.db");
_LIT(KMail2FolderPathD, "d:\\private\\1000484b\\Mail2");
_LIT(KResumeIMAPDBPath, "C:\\msgtest\\framework\\msgsqltest\\testdata\\converter\\imapresume\\messaging.db");
_LIT(KResumePOPDBPath, "C:\\msgtest\\framework\\msgsqltest\\testdata\\converter\\popresume\\messaging.db");
_LIT(KResumeSMTPDBPath, "C:\\msgtest\\framework\\msgsqltest\\testdata\\converter\\smtpresume\\messaging.db");
_LIT(KIndexFileFolderPathD, "d:\\private\\1000484b\\Mail2\\");

	
	

CTestVersion1Version2Converter::CTestVersion1Version2Converter()
	{
	}

CTestVersion1Version2Converter::~CTestVersion1Version2Converter()
	{
	}



void CTestVersion1Version2Converter::SetupL()
	{
	_LIT(KFunction, "SetupL");
	INFO_PRINTF1(KFunction);
	
	__UHEAP_MARK;
	
	iConversionResume = EFalse;

	}



void CTestVersion1Version2Converter::TearDownL()
	{
	_LIT(KFunction, "TearDownL");
	INFO_PRINTF1(KFunction);
		
	delete iDbAdapter;
	delete iMessageDbAdapter;
	delete iVersion1Version2Converter;
		
	__UHEAP_MARKEND;
	}

void CTestVersion1Version2Converter::SetUpTest()
	{
	_LIT(KFunction, "SetUpTest");
	INFO_PRINTF1(KFunction);
	RFs fs;
	User::LeaveIfError(fs.Connect());
	
	CFileMan* fileMngr = CFileMan::NewL(fs);
	CleanupStack::PushL(fileMngr);
	
	// cleanup mesasging.db in drive D
	TRAPD(err,fileMngr->Delete(KDbFileName));	
	MY_ASSERT_EQUALS(err, KErrNone);
	
	//copy index file in drive D
	TRAP(err, fileMngr->Copy(KConverterIndexFile,KIndexFileFolderPathD));
	MY_ASSERT_EQUALS(err, KErrNone);
	
	// copy version 1 db in drive D
	TRAP(err, fileMngr->Copy(KVersion1MessageStore,KDriveD));
	
	CleanupStack::PopAndDestroy(fileMngr);
	fs.Close();	
	}
	
	

void CTestVersion1Version2Converter::TestStartEMailMessageConversionL()
	{
	_LIT(KFunction, "TestStartEMailMessageConversionL");
	INFO_PRINTF1(KFunction);
	
	SetUpTest();// copies the necessary files
					
	iConversionResume = EFalse;
	TBuf<50> fileName; 
	fileName.Append(KDbFileName);	
	iDbAdapter = CMsvDBAdapter::OpenL(fileName);	// open the db	
	
	iDrive = EDriveD;
	iMessageDbAdapter = CMsvMessageDBAdapter::NewL(iDbAdapter,ETrue);	
	
	iVersion1Version2Converter = CMsvVersion1Version2Converter::NewL(*iDbAdapter,*iMessageDbAdapter,iDrive,iConversionResume);
	
	TRAPD(err,iVersion1Version2Converter->ConvertMessageStoreL());

	MY_ASSERT_EQUALS(KErrNone, err);
	}

void CTestVersion1Version2Converter::TestAddCommonheaderFieldFieldL()
	{
	_LIT(KFunction, "TestAddCommonheaderFieldFieldL");
	INFO_PRINTF1(KFunction);
	
	SetUpTest();// copies the necessary files
	
	TBuf<50> fileName; 
	fileName.Append(KDbFileName);	
	iDbAdapter = CMsvDBAdapter::OpenL(fileName);	// open the db	
	
	iDrive = EDriveD;
	iMessageDbAdapter = CMsvMessageDBAdapter::NewL(iDbAdapter,ETrue);	
	
	iVersion1Version2Converter = CMsvVersion1Version2Converter::NewL(*iDbAdapter,*iMessageDbAdapter,iDrive,iConversionResume);
	
	RPointerArray<CFieldPair> iTableFieldPairList;
	
	TRAPD(err,iVersion1Version2Converter->AddFieldL(EFrom));//3
	MY_ASSERT_EQUALS(err, KErrNone);
	TRAP(err,iVersion1Version2Converter->AddFieldL(ESubject));//5
	MY_ASSERT_EQUALS(err, KErrNone);
	TRAP(err,iVersion1Version2Converter->AddFieldL(ETo));//7
	MY_ASSERT_EQUALS(err, KErrNone);
	TRAP(err,iVersion1Version2Converter->AddFieldL(ECC));//7
	MY_ASSERT_EQUALS(err, KErrNone);
	TRAP(err,iVersion1Version2Converter->AddFieldL(EBCC));//7
	MY_ASSERT_EQUALS(err, KErrNone);
	TRAP(err,iVersion1Version2Converter->AddFieldL(EInvalid));//7
	MY_ASSERT_EQUALS(err, KErrArgument);


	iTableFieldPairList.Close();
	}

void CTestVersion1Version2Converter::TestAddFieldL()
	{
	_LIT(KFunction, "TestAddFieldL");
	INFO_PRINTF1(KFunction);
	
	SetUpTest();// copies the necessary files
	
	TBuf<50> fileName; 
	fileName.Append(KDbFileName);	
	iDbAdapter = CMsvDBAdapter::OpenL(fileName);	// open the db	
	
	iDrive = EDriveD;
	iMessageDbAdapter = CMsvMessageDBAdapter::NewL(iDbAdapter,ETrue);	
	
	iVersion1Version2Converter = CMsvVersion1Version2Converter::NewL(*iDbAdapter,*iMessageDbAdapter,iDrive,iConversionResume);
		
	_LIT (KEmailHeaderVersion , "Version");
	HBufC* bufEmailHeadearversion = KEmailHeaderVersion().AllocL();
	TRAPD(err,iVersion1Version2Converter->AddFieldL(bufEmailHeadearversion,EIntegerField)); //0
	MY_ASSERT_EQUALS(err, KErrNone);
	
	_LIT (KReplyTo , "ReplyTo");
	HBufC* bufReplyTo = KReplyTo().AllocL();
	TRAP(err,iVersion1Version2Converter->AddFieldL(bufReplyTo,ETextField));//4
	MY_ASSERT_EQUALS(err, KErrNone);
	
	_LIT (KReceiptAddress , "ReceiptAddress");
	HBufC* bufReceiptAddress = KReceiptAddress().AllocL();
	TRAP(err,iVersion1Version2Converter->AddFieldL(bufReceiptAddress,ETextField));//1
	MY_ASSERT_EQUALS(err, KErrNone);
	
	_LIT (KImMsgId , "ImMsgId");
	HBufC* bufKImMsgId = KImMsgId().AllocL();
	TRAP(err,iVersion1Version2Converter->AddFieldL(bufKImMsgId,ETextField));//2
	MY_ASSERT_EQUALS(err, KErrNone);
	
	_LIT (KiRemoteSize , "iRemoteSize");
	HBufC* bufiRemoteSize = KiRemoteSize().AllocL();
	TRAP(err,iVersion1Version2Converter->AddFieldL(bufiRemoteSize,EIntegerField));//6
	MY_ASSERT_EQUALS(err, KErrNone);
	
	_LIT (KEncodingInfo , "EncodingInfo");
	HBufC* bufEncodingInfo = KEncodingInfo().AllocL();
	TRAP(err,iVersion1Version2Converter->AddFieldL(bufEncodingInfo,ETextField));//10
	MY_ASSERT_EQUALS(err, KErrNone);
	
	_LIT (KResentMsgId , "ResentMsgId");
	HBufC* bufResentMsgId = KResentMsgId().AllocL();
	TRAP(err,iVersion1Version2Converter->AddFieldL(bufResentMsgId,ETextField));//11
	MY_ASSERT_EQUALS(err, KErrNone);
	
	_LIT (KResentFrom , "ResentFrom");
	HBufC* bufResentFrom = KResentFrom().AllocL();
	TRAP(err,iVersion1Version2Converter->AddFieldL(bufResentFrom,ETextField));//12
	MY_ASSERT_EQUALS(err, KErrNone);
	
	_LIT (KResentToRecipients , "ResentToRecipients");
	HBufC* bufResentToRecipients = KResentToRecipients().AllocL();
	TRAP(err,iVersion1Version2Converter->AddFieldL(bufResentToRecipients,ETextField));//13
	MY_ASSERT_EQUALS(err, KErrNone);
	
	_LIT (KResentCcRecipients , "ResentCcRecipients");
	HBufC* bufResentCcRecipients = KResentCcRecipients().AllocL();
	TRAP(err,iVersion1Version2Converter->AddFieldL(bufResentCcRecipients,ETextField));//14
	MY_ASSERT_EQUALS(err, KErrNone);
	
	_LIT (KResentBccRecipients , "ResentBccRecipients");
	HBufC* bufResentBccRecipients = KResentBccRecipients().AllocL();
	TRAP(err,iVersion1Version2Converter->AddFieldL(bufResentBccRecipients,ETextField));//15
	MY_ASSERT_EQUALS(err, KErrNone);
	
	_LIT (KInReplyTo , "InReplyTo");
	HBufC* bufInReplyTo = KInReplyTo().AllocL();
	TRAP(err,iVersion1Version2Converter->AddFieldL(bufInReplyTo,ETextField));//16
	MY_ASSERT_EQUALS(err, KErrNone);
	
	_LIT (KBodyEncoding , "BodyEncoding");
	HBufC* bufBodyEncoding = KBodyEncoding().AllocL();
	TRAP(err,iVersion1Version2Converter->AddFieldL(bufBodyEncoding,EIntegerField));//17
	MY_ASSERT_EQUALS(err, KErrNone);
	
	_LIT (KCharset , "Charset");
	HBufC* bufCharset = KCharset().AllocL();
	TRAP(err,iVersion1Version2Converter->AddFieldL(bufCharset,EIntegerField));//18
	MY_ASSERT_EQUALS(err, KErrNone);
	
	// invalid value
	HBufC* nullValue = NULL;
	TRAP(err,iVersion1Version2Converter->AddFieldL(nullValue,EIntegerField));//18
	MY_ASSERT_EQUALS(err, KErrArgument);
	}


void CTestVersion1Version2Converter::TestReadEMailMessageHeaderL()
	{
	_LIT(KFunction, "TestReadEMailMessageHeaderL");
	INFO_PRINTF1(KFunction);
	
	SetUpTest();// copies the necessary files
	
	TBuf<50> fileName; 
	fileName.Append(KDbFileName);	
	iDbAdapter = CMsvDBAdapter::OpenL(fileName);	// open the db	
	iDbAdapter->InitializeL();
	iDrive = EDriveD;
	iMessageDbAdapter = CMsvMessageDBAdapter::NewL(iDbAdapter,ETrue);	
	
	iVersion1Version2Converter = CMsvVersion1Version2Converter::NewL(*iDbAdapter,*iMessageDbAdapter,iDrive,iConversionResume);
		
	//get service id's for imap
	TInt32 imapMtmId			= {0x1000102A};	// 268439594
	RArray<TMsvId> serviceIdArray;
	RArray<TMsvId> idArray;
	
	TRAPD(error ,iDbAdapter->GetAllServiceIDsForMtmL(imapMtmId, serviceIdArray));
	
	// get message ids now
	TRAP(error ,iDbAdapter->GetAllMessageIdsL(serviceIdArray[0] ,idArray,imapMtmId));
	MY_ASSERT_EQUALS(error , KErrNone);
	
	//construct the path of the first message
	//Get its owning id first
	TMsvId owningId;
	TRAP(error, iDbAdapter->GetOwningServiceL(idArray[0],owningId));
	MY_ASSERT_EQUALS(error , KErrNone);
	
	TBuf<50> path;
	TRAP(error,iVersion1Version2Converter->CreatePathL(path,idArray[0],owningId));
	MY_ASSERT_EQUALS(error , KErrNone);
	
		
	
	// read this message	
	TRAPD(err,iVersion1Version2Converter->ReadEMailMessageHeaderL(path));
	MY_ASSERT_EQUALS(err, KErrNone);

	// some invalid path
	_LIT16(KInvalidPathValue, "z:\\sdadsd");
	path.Zero();
	path.Append(KInvalidPathValue);
	err = iVersion1Version2Converter->ReadEMailMessageHeaderL(path);
	MY_ASSERT_TRUE(err == EFalse);


	serviceIdArray.Close();
	idArray.Close();
	}
	


void CTestVersion1Version2Converter::TestReadEMailMessageEncodedHeaderL()
	{
	_LIT(KFunction, "TestReadEMailMessageEncodedHeaderL");
	INFO_PRINTF1(KFunction);
	
	SetUpTest();// copies the necessary files
	
	TBuf<50> fileName; 
	fileName.Append(KDbFileName);	
	iDbAdapter = CMsvDBAdapter::OpenL(fileName);	// open the db	
	
	iDrive = EDriveD;
	iMessageDbAdapter = CMsvMessageDBAdapter::NewL(iDbAdapter,ETrue);	
	
	iVersion1Version2Converter = CMsvVersion1Version2Converter::NewL(*iDbAdapter,*iMessageDbAdapter,iDrive,iConversionResume);
	
	// hard coded address of the messsage
	
	//get service id's for imap
	TInt32 imapMtmId			= {0x1000102A};	// 268439594
	RArray<TMsvId> serviceIdArray;
	RArray<TMsvId> idArray;
	
	TRAPD(error ,iDbAdapter->GetAllServiceIDsForMtmL(imapMtmId, serviceIdArray));
	
	// get message ids now
	TRAP(error ,iDbAdapter->GetAllMessageIdsL(serviceIdArray[0] ,idArray,imapMtmId));
	MY_ASSERT_EQUALS(error , KErrNone);
	
	//construct the path of the first message
	//Get its owning id first
	TMsvId owningId;
	TRAP(error, iDbAdapter->GetOwningServiceL(idArray[0],owningId));
	MY_ASSERT_EQUALS(error , KErrNone);
	
	TBuf<50> path;
	TRAP(error,iVersion1Version2Converter->CreatePathL(path,idArray[0],owningId));
	MY_ASSERT_EQUALS(error , KErrNone);
	
	// match this path with the actual message path
	
	// read this message	
	TRAPD(err,iVersion1Version2Converter->ReadEMailMessageEncodedHeaderL(path));
	MY_ASSERT_EQUALS(err, KErrNone);
	
	// some invalid path
	_LIT16(KInvalidPathValue, "z:\\sdadsd");
	path.Zero();
	path.Append(KInvalidPathValue);
	err = iVersion1Version2Converter->ReadEMailMessageHeaderL(path);
	MY_ASSERT_TRUE(err == EFalse);
	
	serviceIdArray.Close();
	idArray.Close();
	}
	
void CTestVersion1Version2Converter::TestReadEmailMimeHeaderL()
	{
	_LIT(KFunction, "TestReadEmailMimeHeaderL");
	INFO_PRINTF1(KFunction);
	// hard coded address of the messsage
	
	TBuf<50> fileName; 
	fileName.Append(KDbFileName);	
	iDbAdapter = CMsvDBAdapter::OpenL(fileName);	// open the db	
	
	iDrive = EDriveD;
	iMessageDbAdapter = CMsvMessageDBAdapter::NewL(iDbAdapter,ETrue);	
	
	iVersion1Version2Converter = CMsvVersion1Version2Converter::NewL(*iDbAdapter,*iMessageDbAdapter,iDrive,iConversionResume);
	
	//get service id's for imap
	TInt32 smtpMtmId			= {0x10001028};	// 268439592
	RArray<TMsvId> serviceIdArray;
	RArray<TMsvId> idArray;
	
	TRAPD(error ,iDbAdapter->GetAllServiceIDsForMtmL(smtpMtmId, serviceIdArray));
	
	// get message ids now
	TRAP(error ,iDbAdapter->GetAllMessageIdsL(serviceIdArray[0] ,idArray,smtpMtmId));
	MY_ASSERT_EQUALS(error , KErrNone);
		
	// get mime ids now
	RArray<TMsvId> mimeidArray;
	
	for(TInt i = 0;i < idArray.Count();i++)
		{
		TMsvId msgId = idArray[i];		
		
		TRAP(error ,iDbAdapter->GetAllMimeIdsL( msgId ,mimeidArray));
		MY_ASSERT_EQUALS(error , KErrNone);
		//construct the path of the message
		//Get its owning id first and then convert
		for (TInt j=0; j < mimeidArray.Count();j++)
			{
			TBuf<50> path;
			TMsvId owningId;
			TRAP(error, iDbAdapter->GetOwningServiceL(mimeidArray[j],owningId));
			MY_ASSERT_EQUALS(error , KErrNone);
	
			
			TRAP(error,iVersion1Version2Converter->CreatePathL(path,mimeidArray[j],owningId));
			MY_ASSERT_EQUALS(error , KErrNone);
	
			// match this path with the actual message path
			TRAPD(err,iVersion1Version2Converter->ReadEmailMimeHeaderL(path));
			MY_ASSERT_EQUALS(err, KErrNone);
		
			mimeidArray.Close();			
			}
		}
		
	// some invalid path
	_LIT16(KInvalidPathValue, "z:\\sdadsd");
	TBuf<50> invalidPath;
	invalidPath.Append(KInvalidPathValue);
	TInt err = iVersion1Version2Converter->ReadEMailMessageHeaderL(invalidPath);
	MY_ASSERT_TRUE(err == EFalse);

	serviceIdArray.Close();
	idArray.Close();
	}

void CTestVersion1Version2Converter::TestResumeIMAPMessageConversionL()
	{
	_LIT(KFunction, "TestResumeIMAPMessageConversionL");
	INFO_PRINTF1(KFunction);
	
	// copy the db with resume table containing entries to resume for IMAP	
	RFs fs;
	User::LeaveIfError(fs.Connect());
	
	CFileMan* fileMngr = CFileMan::NewL(fs);
	CleanupStack::PushL(fileMngr);
	
	// cleanup mesasging.db in drive D
	TRAPD(err,fileMngr->Delete(KDbFileName));	
	MY_ASSERT_EQUALS(err, KErrNone);
	
		//copy mail2 in drive D
	TRAP(err, fileMngr->Copy(KConverterMail2Folder,KMail2FolderPathD,2));
	MY_ASSERT_EQUALS(err, KErrNone);
	
	// copy version 1 db in drive D
	TRAP(err, fileMngr->Copy(KResumeIMAPDBPath,KDriveD));
	
	CleanupStack::PopAndDestroy(fileMngr);
	fs.Close();	
	
	iConversionResume = ETrue;
	TBuf<50> fileName; 
	fileName.Append(KDbFileName);	
	iDbAdapter = CMsvDBAdapter::OpenL(fileName);	// open the db	
	
	iDrive = EDriveD;
	iMessageDbAdapter = CMsvMessageDBAdapter::NewL(iDbAdapter,ETrue);	
	
	iVersion1Version2Converter = CMsvVersion1Version2Converter::NewL(*iDbAdapter,*iMessageDbAdapter,iDrive,iConversionResume);
	
	TRAP(err,iVersion1Version2Converter->ConvertMessageStoreL());

	MY_ASSERT_EQUALS(KErrNone, err);
	}
	

void CTestVersion1Version2Converter::TestResumePOPMessageConversionL()
	{
	_LIT(KFunction, "TestResumePOPMessageConversionL");
	INFO_PRINTF1(KFunction);
	// copy the db with resume table containing entries to resume for POP
	
	RFs fs;
	User::LeaveIfError(fs.Connect());
	
	CFileMan* fileMngr = CFileMan::NewL(fs);
	CleanupStack::PushL(fileMngr);
	
	// cleanup mesasging.db in drive D
	TRAPD(err,fileMngr->Delete(KDbFileName));	
	MY_ASSERT_EQUALS(err, KErrNone);
	
	//copy mail2 in drive D
	TRAP(err, fileMngr->Copy(KConverterMail2Folder,KMail2FolderPathD,2));
	MY_ASSERT_EQUALS(err, KErrNone);
	
	// copy version 1 db in drive D
	TRAP(err, fileMngr->Copy(KResumePOPDBPath,KDriveD));
	
	CleanupStack::PopAndDestroy(fileMngr);
	fs.Close();	
	
	iConversionResume = ETrue;
	TBuf<50> fileName; 
	fileName.Append(KDbFileName);	
	iDbAdapter = CMsvDBAdapter::OpenL(fileName);	// open the db	
	
	iDrive = EDriveD;
	iMessageDbAdapter = CMsvMessageDBAdapter::NewL(iDbAdapter,ETrue);	
	
	iVersion1Version2Converter = CMsvVersion1Version2Converter::NewL(*iDbAdapter,*iMessageDbAdapter,iDrive,iConversionResume);
		
	TRAP(err,iVersion1Version2Converter->ConvertMessageStoreL());

	MY_ASSERT_EQUALS(KErrNone, err);
	}
	
void CTestVersion1Version2Converter::TestResumeSMTPMessageConversionL()
	{
	_LIT(KFunction, "TestResumeSMTPMessageConversionL");
	INFO_PRINTF1(KFunction);
	// copy the db with resume table containing entries to resume for SMTP
	
	RFs fs;
	User::LeaveIfError(fs.Connect());
	
	CFileMan* fileMngr = CFileMan::NewL(fs);
	CleanupStack::PushL(fileMngr);
	
	// cleanup mesasging.db in drive D
	TRAPD(err,fileMngr->Delete(KDbFileName));	
	MY_ASSERT_EQUALS(err, KErrNone);
	
	//copy mail2 in drive D
	TRAP(err, fileMngr->Copy(KConverterMail2Folder,KMail2FolderPathD,2));
	MY_ASSERT_EQUALS(err, KErrNone);
	
	// copy version 1 db in drive D
	TRAP(err, fileMngr->Copy(KResumeSMTPDBPath,KDriveD));
	
	CleanupStack::PopAndDestroy(fileMngr);
	fs.Close();	
	
	iConversionResume = ETrue;
	TBuf<50> fileName; 
	fileName.Append(KDbFileName);	
	iDbAdapter = CMsvDBAdapter::OpenL(fileName);	// open the db	
	
	iDrive = EDriveD;
	iMessageDbAdapter = CMsvMessageDBAdapter::NewL(iDbAdapter,ETrue);	
	
	iVersion1Version2Converter = CMsvVersion1Version2Converter::NewL(*iDbAdapter,*iMessageDbAdapter,iDrive,iConversionResume);
	
	
	TRAP(err,iVersion1Version2Converter->ConvertMessageStoreL());

	MY_ASSERT_EQUALS(KErrNone, err);
		
	}
	
void CTestVersion1Version2Converter::TestTImHeaderEncodingInfoL()
	{
	_LIT(KFunction, "TestTImHeaderEncodingInfoL");
	INFO_PRINTF1(KFunction);
	
	
	TImHeaderEncodingInfo info;
	
	info.SetField(TImHeaderEncodingInfo::EReplyTo);
	TImHeaderEncodingInfo::TFieldList val = info.Field();
	
	info.SetOffset(2);
	TInt offset = info.Offset();
	info.SetLength(34);
	TInt length = info.Length();
	
	info.SetCharsetUid(23);
	TUint charSetid =	info.CharsetUid();
	
	_LIT8(KSomeDesc, "asassa");
	info.SetEncodingType(KSomeDesc);
	
	info.EncodingType();
	info.SetEncodingType(TImHeaderEncodingInfo::ENoEncoding);
	info.SetEncodingType(TImHeaderEncodingInfo::ENoEncoding);
	
	_LIT8(KCharBSmall,"b");
	_LIT8(KCharBBig,"B");
	_LIT8(KCharQSmall,"q");
	_LIT8(KCharQBig,"Q");
	
	
	HBufC8* characterb = KCharBSmall().Alloc();
	HBufC8* characterB = KCharBBig().Alloc();
	HBufC8* characterq = KCharQSmall().Alloc();
	HBufC8* characterQ = KCharQBig().Alloc();
	
	
	info.SetEncodingType(characterb->Des());
	info.SetEncodingType(characterB->Des());
	info.SetEncodingType(characterq->Des());
	info.SetEncodingType(characterQ->Des());
	
	delete characterb;
	delete characterB;
	delete characterq;
	delete characterQ;
	
	TImHeaderEncodingInfo::TEncodingType encoding = info.EncodingType();
	
	info.SetArrayValue(6);
	TInt array = info.ArrayValue();
	
	TBool spaceFlag = ETrue;
	info.SetAddSpace(spaceFlag);
	TBool sFalg = info.AddSpace();
	
	info.SetEncodedLength(2323);
	info.SetEncodedLength(0);
	TInt eLength = info.EncodedLength();
	
	TImHeaderEncodingInfo info1(info);
	

	}
	
void CTestVersion1Version2Converter::TestCoverageL()
	{
	_LIT(KFunction, "TestCoverageL");
	INFO_PRINTF1(KFunction);
	
	// FOR  coverage
	// copy coverage mail2 folder
	RFs fs;
	User::LeaveIfError(fs.Connect());
	
	CFileMan* fileMngr = CFileMan::NewL(fs);
	CleanupStack::PushL(fileMngr);
	
	// cleanup mesasging.db in drive D
	TRAPD(err,fileMngr->Delete(KDbFileName));	
	MY_ASSERT_EQUALS(err, KErrNone);
	
	//delete mail2 in drive D
	TRAP(err, fileMngr->RmDir(KMail2FolderPathD));
	MY_ASSERT_EQUALS(err, KErrNone);
	
	TRAP(err, fs.MkDir(KMail2FolderPathD));
	MY_ASSERT_EQUALS(err, KErrNone);
	
	// copy messaging db
	// copy version 1 db in drive D
	_LIT(KCoverageDB, "c:\\msgtest\\framework\\msgsqltest\\testdata\\converter\\coverage\\messaging.db");
	TRAP(err, fileMngr->Copy(KCoverageDB,KDriveD));
	
	//copy mail2 in drive D
	_LIT(KConverterMail2CoverageFolder, "c:\\msgtest\\framework\\msgsqltest\\testdata\\converter\\coverage\\Mail2");
	TRAP(err, fileMngr->Copy(KConverterMail2CoverageFolder,KMail2FolderPathD,2));
	MY_ASSERT_EQUALS(err, KErrNone);
	
	CleanupStack::PopAndDestroy(fileMngr);
	fs.Close();	
	
	iConversionResume = EFalse;
	TBuf<50> fileName; 
	fileName.Append(KDbFileName);
	iDbAdapter = CMsvDBAdapter::OpenL(fileName);
	iDbAdapter->CreateConversionStatusTableL();
			
	iDrive = EDriveD;
	iMessageDbAdapter = CMsvMessageDBAdapter::NewL(iDbAdapter,ETrue);	
	
	iVersion1Version2Converter = CMsvVersion1Version2Converter::NewL(*iDbAdapter,*iMessageDbAdapter,iDrive,iConversionResume);
	
	
	TInt32 localMtm			= {0x10001028};	// 268439594
	RArray<TMsvId> serviceIdArray;		
	TConversionResumeStatus iResumeStatus;
	iResumeStatus.lastId = 0;
	TRAPD(error ,iDbAdapter->GetAllServiceIDsForMtmL(localMtm,serviceIdArray));
	
	for(TInt pos = 0; pos<serviceIdArray.Count();pos++)
		{
		RArray<TMsvId> iDArray;
		CleanupClosePushL(iDArray);
		TMsvId iD;
		
		iDbAdapter->GetAllMessageIdsL(serviceIdArray[pos],iDArray,localMtm, iResumeStatus.lastId);
		iResumeStatus.remainingCount = iDArray.Count();
			
		TInt i=0;
		for (;i<iDArray.Count();i++)
			{
			iDbAdapter->GetOwningServiceL(iDArray[i],iD);
						
			RBuf path;
			CleanupClosePushL(path);
			path.CreateL(KMaxPath);
						
			iVersion1Version2Converter->CreatePathL(path,iDArray[i],iD);
					
			//Read the message
			TInt headerError = iVersion1Version2Converter->ReadEMailMessageHeaderL(path);
			TInt encodeHeaderError = iVersion1Version2Converter->ReadEMailMessageEncodedHeaderL(path);
			CleanupStack::PopAndDestroy();
			}
		CleanupStack::PopAndDestroy(&iDArray);
		}
	
	serviceIdArray.Close();
	
	}


CTestSuite* CTestVersion1Version2Converter::CreateSuiteL(const TDesC& aName)
	{
	SUB_SUITE;
	ADD_ASYNC_TEST_STEP(TestStartEMailMessageConversionL);
	ADD_ASYNC_TEST_STEP(TestAddCommonheaderFieldFieldL);	
	ADD_ASYNC_TEST_STEP(TestAddFieldL);	
	ADD_ASYNC_TEST_STEP(TestReadEMailMessageHeaderL);	
	ADD_ASYNC_TEST_STEP(TestReadEMailMessageEncodedHeaderL);	
	ADD_ASYNC_TEST_STEP(TestReadEmailMimeHeaderL);
	ADD_ASYNC_TEST_STEP(TestResumeIMAPMessageConversionL);
	ADD_ASYNC_TEST_STEP(TestResumePOPMessageConversionL);
	ADD_ASYNC_TEST_STEP(TestResumeSMTPMessageConversionL);
	ADD_ASYNC_TEST_STEP(TestTImHeaderEncodingInfoL);
	ADD_ASYNC_TEST_STEP(TestCoverageL);
	
	END_SUITE;
	}

