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
// MACRO Declaration
// 
//

/**
 @file 
 @internalTechnology
*/

#include "t_msv_inidata.h"


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

//Ini file which contains the cache setting
_LIT(KSTDMSGINI, "C:\\private\\1000484b\\msgcache.ini");
_LIT(KMSGINI, "C:\\private\\1000484b\\msgcache.ini");
_LIT(KMSGINI_2, "C:\\private\\1000484b\\msgcache2.ini");
_LIT(KMSGINI_4, "C:\\private\\1000484b\\msgcache4.ini");
_LIT(KCONFIGFILE, "C:\\private\\1000484b\\config.ini");

void CTestIniData::SetupL()
	{
	_LIT(KFunction, "SetupL");
	INFO_PRINTF1(KFunction);
	
	__UHEAP_MARK;
		
	TInt err =	iFileHandle.Connect();
	MY_ASSERT_EQUALS(err, KErrNone);
	CFileMan* file = CFileMan::NewL(iFileHandle);
	err = file->Copy(KMSGINI_4, KSTDMSGINI);
	MY_ASSERT_EQUALS(err, KErrNone);
	delete file;
	}

void CTestIniData::TearDownL()
	{
	_LIT(KFunction, "TearDownL");
	INFO_PRINTF1(KFunction);	
	iFileHandle.Close();
	
	__UHEAP_MARKEND;
	}

CTestIniData::CTestIniData()
	{
	
	}

CTestIniData::~CTestIniData()
	{

	}

void CTestIniData::TestCreation()
	{
	_LIT(KFunction, "TestCreation");
	INFO_PRINTF1(KFunction);
	
	TRAP_IGNORE(iIniFile = CMsvIniData::NewL(KMSGINI));
	
	HBufC* configFile = NULL;
	TRAPD(err, configFile = KSTDMSGINI().AllocL());
	MY_ASSERT_EQUALS(err, KErrNone);
	MY_ASSERT_EQUALS(*(iIniFile->iName), *configFile);
	delete iIniFile;
	delete configFile;
	}
	

void CTestIniData::TestFindVarL()
	{
	_LIT(KFunction, "TestFindVar");
	INFO_PRINTF1(KFunction);
	
	_LIT(KIniMsvInitialCacheCreation, "MsvInitialCacheCreation");
	_LIT(KIniMsvMaximumCacheSize, "MsvMaximumCacheSize");
	
	TBool ret = EFalse;
	
	
	
	//Creation of the CMsvIniData object
	TRAP_IGNORE(iIniFile = CMsvIniData::NewL(KMSGINI));
	CleanupStack::PushL(iIniFile);
	HBufC* configFile = NULL;
	TRAP_IGNORE(configFile = KSTDMSGINI().AllocL());
	MY_ASSERT_EQUALS(*(iIniFile->iName), *configFile);
	delete configFile;
	
	
	//Testing reading of an existing variable.
	TInt msvMaximumCacheSize = 0;
	User::LeaveIfError(ret = iIniFile->FindVar(KIniMsvMaximumCacheSize, msvMaximumCacheSize));
	MY_ASSERT_TRUE(ret);
	MY_ASSERT_EQUALS(msvMaximumCacheSize, 1024);
	//The same test, but using the TPtrC variant.
	ret = EFalse;
	msvMaximumCacheSize = 0;
	TPtrC result;
	User::LeaveIfError(ret = iIniFile->FindVar(KIniMsvMaximumCacheSize, result));
	MY_ASSERT_TRUE(ret);
	TLex lex1(result);
	MY_ASSERT_EQUALS(lex1.Val(msvMaximumCacheSize), KErrNone);
	MY_ASSERT_EQUALS(msvMaximumCacheSize, 1024);
	//The same test, but using the section variant for TPtrC output.
	ret = EFalse;
	msvMaximumCacheSize = 0;
	User::LeaveIfError(ret = iIniFile->FindVar((TDesC&)KNullDesC ,KIniMsvMaximumCacheSize, result));
	MY_ASSERT_TRUE(ret);
	TLex lex3(result);
	MY_ASSERT_EQUALS(lex3.Val(msvMaximumCacheSize), KErrNone);
	MY_ASSERT_EQUALS(msvMaximumCacheSize, 1024);
	//The same test, but using the section variant for TInt output.
	ret = EFalse;
	msvMaximumCacheSize = 0;
	User::LeaveIfError(ret = iIniFile->FindVar((TDesC&)KNullDesC ,KIniMsvMaximumCacheSize, msvMaximumCacheSize));
	MY_ASSERT_TRUE(ret);
	MY_ASSERT_EQUALS(msvMaximumCacheSize, 1024);
	
	
	
	//Testing reading of a non-existing variable.
	//---Change the INI file
	CleanupStack::PopAndDestroy(); //iIniFile
	CFileMan* file = NULL;
	TRAP_IGNORE(file = CFileMan::NewL(iFileHandle));
	TInt err = file->Copy(KMSGINI_2, KSTDMSGINI);
	MY_ASSERT_EQUALS(err, KErrNone);
	
	TRAP_IGNORE(iIniFile = CMsvIniData::NewL(KMSGINI));
	CleanupStack::PushL(iIniFile);
	TInt msvInitialCacheSize = 0;
	ret = ETrue;
	User::LeaveIfError(ret = iIniFile->FindVar(KIniMsvInitialCacheCreation, msvInitialCacheSize));
	MY_ASSERT_FALSE(ret);
	MY_ASSERT_EQUALS(msvInitialCacheSize, 0); //the variable could not be found in the INI
	//The same test, but using the TPtrC variant.
	ret = ETrue;
	msvInitialCacheSize = 0;
	User::LeaveIfError(ret = iIniFile->FindVar(KIniMsvInitialCacheCreation, result));
	MY_ASSERT_FALSE(ret);
	TLex lex2(result);
	MY_ASSERT_FALSE(lex2.Val(msvInitialCacheSize) == KErrNone);
	MY_ASSERT_EQUALS(msvInitialCacheSize, 0); //the variable could not be found in the INI
	//The same test, but using the section variant for TPtrC output.
	ret = ETrue;
	msvInitialCacheSize = 0;
	User::LeaveIfError(ret = iIniFile->FindVar((TDesC&)KNullDesC ,KIniMsvInitialCacheCreation, result));
	MY_ASSERT_FALSE(ret);
	TLex lex4(result);
	MY_ASSERT_FALSE(lex4.Val(msvInitialCacheSize) == KErrNone);
	MY_ASSERT_EQUALS(msvInitialCacheSize, 0); //the variable could not be found in the INI
	//The same test, but using the section variant for TInt output.
	ret = ETrue;
	msvInitialCacheSize = 0;
	User::LeaveIfError(ret = iIniFile->FindVar((TDesC&)KNullDesC ,KIniMsvInitialCacheCreation, msvMaximumCacheSize));
	MY_ASSERT_FALSE(ret);
	MY_ASSERT_EQUALS(msvInitialCacheSize, 0); //the variable could not be found in the INI
	//---Restore the INI file
	err = file->Copy(KMSGINI_4, KSTDMSGINI);
	MY_ASSERT_EQUALS(err, KErrNone);
	delete file;
	
	CleanupStack::PopAndDestroy(); //iIniFile
	
	}

void CTestIniData::TestFindVarWithSectionsL()
	{
	_LIT(KFunction, "TestFindVarWithSectionsL");
	INFO_PRINTF1(KFunction);
	
	_LIT(KSection1, "MEDIA1");
	_LIT(KSection2, "MEDIA2");
	_LIT(KSection3, "MEDIA3");
	_LIT(KSection4, "Non-existing");
	_LIT(KSection5, "MEDIA5");
	_LIT(KSection6, "MEDIA6");
	_LIT(KTypeVar, "type");
	_LIT(KWidthVar, "width");
	_LIT(KMediaType1, "file");
	_LIT(KMediaType2, "serial");
	_LIT(KMaxSizeVar, "max size");
	
	TBool ret = EFalse;
	TPtrC result;
	
	
	//Creation of the CMsvIniData object
	TRAP_IGNORE(iIniFile = CMsvIniData::NewL(KCONFIGFILE));
	CleanupStack::PushL(iIniFile);
	HBufC* configFile = NULL;
	TRAP_IGNORE(configFile = KCONFIGFILE().AllocL());
	MY_ASSERT_EQUALS(*(iIniFile->iName), *configFile);
	delete configFile;
	
	
	//Testing reading of an existing variable.
	User::LeaveIfError(ret = iIniFile->FindVar((TDesC&)KSection1, KTypeVar, result));
	MY_ASSERT_TRUE(ret);
	TLex lex1(result);
	MY_ASSERT_TRUE(lex1.NextToken()	== KMediaType1());
	//Similar test for retrieving numeric value.
	TInt maxSize = 0;
	ret = EFalse;
	User::LeaveIfError(ret = iIniFile->FindVar((TDesC&)KSection1, KMaxSizeVar, result));
	MY_ASSERT_TRUE(ret);
	TLex lex3(result);
	MY_ASSERT_TRUE(lex3.Val(maxSize) == KErrNone);
	MY_ASSERT_EQUALS(maxSize, 100);
	//Similar test using the TInt variant.
	maxSize = 0;
	ret = EFalse;
	User::LeaveIfError(ret = iIniFile->FindVar((TDesC&)KSection1, KMaxSizeVar, maxSize));
	MY_ASSERT_TRUE(ret);
	MY_ASSERT_EQUALS(maxSize, 100);
	//Similar test for another section.
	ret = EFalse;
	User::LeaveIfError(ret = iIniFile->FindVar((TDesC&)KSection2, KTypeVar, result));
	MY_ASSERT_TRUE(ret);
	TLex lex2(result);
	MY_ASSERT_TRUE(lex2.NextToken()	== KMediaType2());
	
	
	
	//Testing reading of a non-existing variable.
	//..from the second section.
	maxSize = 0;
	ret = ETrue;
	User::LeaveIfError(ret = iIniFile->FindVar((TDesC&)KSection2, KMaxSizeVar, maxSize));
	MY_ASSERT_FALSE(ret);
	MY_ASSERT_EQUALS(maxSize, 0); //second section does not have max size.
	//Similar test using the TPtrC variant.
	maxSize = 0;
	ret = ETrue;
	User::LeaveIfError(ret = iIniFile->FindVar((TDesC&)KSection2, KMaxSizeVar, result));
	MY_ASSERT_FALSE(ret);
	TLex lex4(result);
	MY_ASSERT_FALSE(lex4.Val(maxSize) == KErrNone);
	MY_ASSERT_EQUALS(maxSize, 0); //second section does not have max size.
	
	
	
	//Test reading from non-existing section.
	maxSize = 0;
	ret = ETrue;
	User::LeaveIfError(ret = iIniFile->FindVar((TDesC&)KSection4, KMaxSizeVar, maxSize));
	MY_ASSERT_FALSE(ret);
	MY_ASSERT_EQUALS(maxSize, 0);
	
	
	//Test reading from commented and uncommented sections and variables.
	TInt width = 0;
	ret = ETrue;
	User::LeaveIfError(ret = iIniFile->FindVar((TDesC&)KSection3, KWidthVar, width));
	MY_ASSERT_FALSE(ret);
	MY_ASSERT_EQUALS(width, 0); //section was found but variable had no 'key=value' format
	//Similar test but now the key has no value part in the file.
	width = 0;
	ret = ETrue;
	User::LeaveIfError(ret = iIniFile->FindVar((TDesC&)KSection5, KWidthVar, width));
	MY_ASSERT_FALSE(ret);
	MY_ASSERT_EQUALS(width, 0); 
	//Similar test but now the key has no '=' or value part in the file.
	width = 0;
	ret = ETrue;
	User::LeaveIfError(ret = iIniFile->FindVar((TDesC&)KSection6, KWidthVar, width));
	MY_ASSERT_FALSE(ret);
	MY_ASSERT_EQUALS(width, 0); 
	
	
	
	CleanupStack::PopAndDestroy(); //iIniFile
	}


CTestSuite* CTestIniData::CreateSuiteL(const TDesC& aName)
	{
	SUB_SUITE;
	ADD_ASYNC_TEST_STEP(TestCreation);
	ADD_ASYNC_TEST_STEP(TestFindVarL);
	ADD_ASYNC_TEST_STEP(TestFindVarWithSectionsL);
	END_SUITE;
	}

