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


#include "t_msv_smsmessage_v1v2_converter_step.h"
#include <MSVRCPT.h>

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


#if (defined SYMBIAN_MESSAGESTORE_UNIT_TESTCODE)
_LIT(KMessagingDBName, "\\messaging.db");
#endif

#ifdef SYMBIAN_MESSAGESTORE_UNIT_TESTCODE
_LIT(KDbFileName, "D:\\messaging.db");
#else
_LIT(KDbFileName, "[1000484B]messaging.db");
#endif










/* start v1tov2 APIs */

CTestSmsMessageV1toV2Converter::CTestSmsMessageV1toV2Converter()
{
	iDrive = EDriveD;
}

CTestSmsMessageV1toV2Converter::~CTestSmsMessageV1toV2Converter()
{
//	delete iServiceSettings;
//	delete iRichText;
//	delete iCharFormat;
//	delete iParaFormat;
	
}

void CTestSmsMessageV1toV2Converter::SetupL()
	{
	_LIT(KFunction, "SetupL");
	INFO_PRINTF1(KFunction);
	}

void CTestSmsMessageV1toV2Converter::TearDownL()
	{
	
	_LIT(KFunction, "Sms Header Convertion Verified");
	INFO_PRINTF1(KFunction);
	
	_LIT(KFunction1, "TearDownL");
	INFO_PRINTF1(KFunction1);
	
	delete iMessageDbAdapter;
	delete iDbAdapter;
	delete iMessageConverter;
	}


	




void CTestSmsMessageV1toV2Converter::TestFillEmptyEosBufferL()
	{
	_LIT(KFunction, "TestFillEmptyEosBufferL");
	INFO_PRINTF1(KFunction);
		
	TParse parse;
	TPtrC drive = TDriveUnit(iDrive).Name();
	parse.Set(KMessagingDBName, &drive, NULL);
	dbFileName = parse.FullName();
	
	iDbAdapter = CMsvDBAdapter::OpenL(dbFileName);	// open the db	
	
	
	iMessageDbAdapter = CMsvMessageDBAdapter::NewL(iDbAdapter,ETrue);
	iMessageConverter = CMsvVersion1Version2Converter::NewL(*iDbAdapter,*iMessageDbAdapter,iDrive,EFalse/*,smsMtmId*/);

	iMessageConverter->InitializeL();
	iMessageConverter->FillEmptyEosBufferL();
	
	_LIT16(KInvalidPathValue, "z:\\sdadsd");
	RBuf path;
	path.Create(100);
	path.Append(KInvalidPathValue);
	
	iMessageConverter->ReadSmsMessageHeaderL(path);
	
	}

void CTestSmsMessageV1toV2Converter::TestConvertMessageStoreL()
    {
    _LIT(KFunction, "TestConvertMessageStoreL");
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
    _LIT(KConverterSMSMail2Folder, "C:\\msgtest\\framework\\msgsqltest\\testdata\\converter\\sms\\Mail2");
    _LIT(KMail2FolderPathD, "d:\\private\\1000484b\\Mail2");
    
    TRAP(err, fileMngr->RmDir(KMail2FolderPathD));
    MY_ASSERT_EQUALS(err, KErrNone);
    
   	fs.MkDirAll(_L("D:\\private\\1000484b\\mail2\\"));
	    
    TRAP(err, fileMngr->Copy(KConverterSMSMail2Folder,KMail2FolderPathD,2));
    MY_ASSERT_EQUALS(err, KErrNone);
    
    // copy version 1 db in drive D
    _LIT(KSMSDB, "C:\\msgtest\\framework\\msgsqltest\\testdata\\converter\\sms\\messaging.db");
    _LIT(KDriveD,"D:\\");
    TRAP(err, fileMngr->Copy(KSMSDB,KDriveD));
    
    CleanupStack::PopAndDestroy(fileMngr);
    fs.Close(); 
    
    TBool iConversionResume = ETrue;
    TBuf<50> fileName; 
    fileName.Append(KDbFileName);   
    iDbAdapter = CMsvDBAdapter::OpenL(fileName);    // open the db  
    
    iDrive = EDriveD;
    iMessageDbAdapter = CMsvMessageDBAdapter::NewL(iDbAdapter,ETrue);   
    
    iMessageConverter = CMsvVersion1Version2Converter::NewL(*iDbAdapter,*iMessageDbAdapter,iDrive,iConversionResume);
    
    
    TRAP(err,iMessageConverter->ConvertMessageStoreL());

    MY_ASSERT_EQUALS(KErrNone, err);
   }


void CTestSmsMessageV1toV2Converter::TestCleanAllTestDataL()
    {
    _LIT(KFunction, "TestCleanAllTestDataL");
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
    _LIT(KMail2FolderPathD, "d:\\private\\1000484b\\Mail2");
    _LIT(KImapResume, "d:\\imapresume");
    _LIT(KSmtpResume, "d:\\smtpresume");
    _LIT(KPopResume, "d:\\popresume");
    _LIT(KCoverage, "d:\\coverage");
    _LIT(KV0Resume, "d:\\version0version1resume");
    _LIT(KV1Resume, "d:\\version1version2resume");
    _LIT(KSms, "d:\\sms");
    TRAP(err, fileMngr->RmDir(KMail2FolderPathD));
    MY_ASSERT_EQUALS(err, KErrNone);
    
    _LIT(KMail2FolderPathE, "e:\\private\\1000484b\\Mail2");
    TRAP(err, fileMngr->RmDir(KMail2FolderPathE));
    MY_ASSERT_EQUALS(err, KErrNone);
    
    TRAP(err, fileMngr->RmDir(KImapResume));
    MY_ASSERT_EQUALS(err, KErrNone);
    
    TRAP(err, fileMngr->RmDir(KSmtpResume));
    MY_ASSERT_EQUALS(err, KErrNone);
    
    TRAP(err, fileMngr->RmDir(KPopResume));
    MY_ASSERT_EQUALS(err, KErrNone);
    
    TRAP(err, fileMngr->RmDir(KCoverage));
    MY_ASSERT_EQUALS(err, KErrNone);
    
    TRAP(err, fileMngr->RmDir(KV0Resume));
    MY_ASSERT_EQUALS(err, KErrNone);
    
    TRAP(err, fileMngr->RmDir(KV1Resume));
    MY_ASSERT_EQUALS(err, KErrNone);
    
    TRAP(err, fileMngr->RmDir(KSms));
    MY_ASSERT_EQUALS(err, KErrNone);
    TRAP(err, fs.MkDir(KMail2FolderPathD));
    MY_ASSERT_EQUALS(err, KErrNone);
    
    CleanupStack::PopAndDestroy(fileMngr);
    fs.Close(); 
    
    }


CTestSuite* CTestSmsMessageV1toV2Converter::CreateSuiteL(const TDesC& aName)
	{
	SUB_SUITE;
	ADD_ASYNC_TEST_STEP(TestConvertMessageStoreL);
	ADD_ASYNC_TEST_STEP(TestFillEmptyEosBufferL);
	ADD_ASYNC_TEST_STEP(TestCleanAllTestDataL);
	END_SUITE;
	}
	
