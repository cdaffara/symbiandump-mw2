/*
* Copyright (c) 2005 Nokia Corporation and/or its subsidiary(-ies). 
* All rights reserved.
* This component and the accompanying materials are made available
* under the terms of "Eclipse Public License v1.0"
* which accompanies this distribution, and is available
* at the URL "http://www.eclipse.org/legal/epl-v10.html".
*
* Initial Contributors:
* Nokia Corporation - initial contribution.
*
* Contributors:
*
* Description: 
*   ?description_line
*
*/


//  INCLUDES
#include "FT_CPosTp105.h"
#include <EPos_CPosLandmarkDatabase.h>
#include "FT_LandmarkConstants.h"
#include <centralrepository.h>
#include <bautils.h>
           
// CONSTANTS

_LIT(KResourceFileROM, "z:\\resource\\eposlmdefaultdbname.R");
_LIT(KResourceFile, "c:\\resource\\eposlmdefaultdbname.R");
_LIT(KResourceFileCOPY, "c:\\resource\\COPY_eposlmdefaultdbname.R");

#ifdef __WINS__
	_LIT(KResourceFileTEST, "z:\\system\\test\\testdata\\tp105_eposlmdefaultdbname.R");
#else
	_LIT(KResourceFileTEST, "c:\\system\\test\\testdata\\tp105_eposlmdefaultdbname.R");
#endif

_LIT(KDefaultName_En, "TP105 Default Landmarks");
_LIT(KDefaultName_Sw, "Mina små landmarks");
_LIT(KEnglish, "English");
_LIT(KSwedish, "Swedish");

// ================= MEMBER FUNCTIONS =======================

// ---------------------------------------------------------
// CPosTp105::StartL
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CPosTp105::StartL()
    {
#ifdef __WINS__
    _LIT(KNotSupported, "Test case is not supported on WINS, execute on target!");
    LogErrorAndLeave(KNotSupported);
#endif
    _LIT(KCancelled, "Test cancelled");
    _LIT(KLangChanged, "Have you changed the language to swedish in the phone ? (Should be English currently.)");
    _LIT(KChangeLang, "Change language to Swedish in phone (you will need to reboot the phone)");
    _LIT(KWrongLanguageEnglish, "Wrong language on phone, should be English");
    _LIT(KWrongLanguageSwedish, "Wrong language on phone, should be Swedish");
    
    /*
LMREQ200: Display Name for Default Database
If the display name has not been set for the default database, 
a localized default display name shall be used. The localized display name 
is read from a resource file and it is language dependent. 
If the user changes language for the phone, the localized display name shall change accordingly.
*/
    
    TUtfwUserAnswer answer = iUserInfo->ShowDialog(KLangChanged, EUtfwDialogTypeYesNo, EFalse);
    //TUtfwUserAnswer answer = EUtfwUserAnswerNo;
    if(answer == EUtfwUserAnswerNo)
        {
        RemoveAllLmDatabasesL();
        // Copy the default display name resource file with english name to
        // c:\\resource
        // This file should be used when language is english
		CopyResourceFileL(_L("01"));
		
        // Create a database and set it to be default
        // Since the display name has not been set for this db it should be read
        // from localiased resource file
        _LIT(KDB, "TP105_Db.ldb");
        iDbMan = CPosLmDatabaseManager::NewL();
        HPosLmDatabaseInfo* dbInfo01 = HPosLmDatabaseInfo::NewLC(KDB);
        iDbMan->CreateDatabaseL(*dbInfo01);
        CleanupStack::PopAndDestroy(dbInfo01);
        iDbMan->SetDefaultDatabaseUriL(KDB);
        
        delete iDbMan;
        iDbMan = NULL;
    
        TBuf<50> buf;
		TInt lang = User::Language();
		buf.Format(_L("***** Language %d ******"), lang);
		iLog->Put(buf);
		
		if (lang != 1) 
		    {
		    LogErrorAndLeave(KWrongLanguageEnglish, KErrGeneral);
		    }
		
		CheckDisplayNameL(KEnglish, KDefaultName_En);
		
	    answer = iUserInfo->ShowDialog(KChangeLang, EUtfwDialogTypeOkCancel, EFalse);
	    //answer = EUtfwUserAnswerOk;
        AssertTrueSecL(answer == EUtfwUserAnswerOk, KCancelled, KErrCancel);
        }
    else
        {
        TInt lang = User::Language();
		TBuf<50> buf;
		buf.Format(_L("***** Language %d ******"), lang);
		iLog->Put(buf);
		
		if (lang != 6)
		    {
		    LogErrorAndLeave(KWrongLanguageSwedish, KErrGeneral);
		    }
		    
        // Copy the default display name resource file with swedish name to
        // c:\\resource
        // This file should be used when language is changed into swedish
		CopyResourceFileL(_L("06"));
		
        CheckDisplayNameL(KSwedish, KDefaultName_Sw);
        
        _LIT(KTestSucceeded, "Localization test for displayname succeeded!");
        //iUserInfo->ShowDialog(KTestSucceeded, EUtfwDialogTypeOk, EFalse);
        
        // Remove both swedish and english test resource files
		// used in this test, also restores the original resource files
		RemoveResourceFileL(_L("01"));
		RemoveResourceFileL(_L("06"));
		iLog->Put(KTestSucceeded);
        }
    }
    
// ---------------------------------------------------------
// CPosTp105::CheckDisplayNameL
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CPosTp105::CheckDisplayNameL(const TDesC& aLanguage, const TDesC& aDisplayName)
    {
    iLog->Put(_L("CheckDisplayNameL"));
    _LIT(KQuestionFormat, "Does the language look like %S?: %S");
    _LIT(KErrLangError, "The language of the displayname is not correct.");
    _LIT(KMatchCriteria, "://");
    _LIT(KErrMatch, "Error. Displayname matches filename.");
	_LIT(KErrMatchUri, "Error. Displayname matches URI.");
	_LIT(KErrMatchDisplay, "Error. Displayname does not match localised name");
    
    // Get the display name:
	iDbMan = CPosLmDatabaseManager::NewL();
	HBufC* defaultUri = iDbMan->DefaultDatabaseUriLC();
	
	HPosLmDatabaseInfo* dbInfo = HPosLmDatabaseInfo::NewLC(defaultUri->Des());
	
	iDbMan->GetDatabaseInfoL(*dbInfo);
	
    TPosLmDatabaseSettings settings = dbInfo->Settings();
    TPtrC defaultDisplayName = settings.DatabaseName();
    CleanupStack::PopAndDestroy(dbInfo);
	
	TInt stringPos = defaultUri->Find(KMatchCriteria);
	TBuf<50> uriFileName = defaultUri->Right(defaultUri->Length() - (stringPos + 5));
	
	AssertTrueSecL(uriFileName != defaultDisplayName, KErrMatch);
	AssertTrueSecL(defaultUri->Des() != defaultDisplayName, KErrMatchUri);
	AssertTrueSecL(defaultDisplayName.CompareC(aDisplayName) == 0, KErrMatchDisplay);
	
	iLog->Put(defaultDisplayName);
	iLog->Put(uriFileName);
	iLog->Put(defaultUri->Des());
	
	TBuf<100> question;
	question.Format(KQuestionFormat, &aLanguage, &defaultDisplayName);
	TUtfwUserAnswer answer = iUserInfo->ShowDialog(question, EUtfwDialogTypeYesNo, EFalse);
	CleanupStack::PopAndDestroy(defaultUri);
	//TUtfwUserAnswer answer = EUtfwUserAnswerYes;
	AssertTrueSecL(answer == EUtfwUserAnswerYes, KErrLangError);

	delete iDbMan;
    iDbMan = NULL;

    }
    
// ---------------------------------------------------------
// CPosTp105::CloseTest
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CPosTp105::CloseTest()
	{
	iLog->Put(_L("CloseTest"));
	
	delete iDbMan;
    iDbMan = NULL;

	}

// ---------------------------------------------------------
// CPosTp105::CopyResourceFileL
// 
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CPosTp105::CopyResourceFileL(const TDesC& aResourceNr)
    {
	iLog->Put(_L("CopyResourceFileL"));
    RFs fs;
    User::LeaveIfError(fs.Connect());
    CleanupClosePushL(fs);
    
    CFileMan* fileMan = CFileMan::NewL(fs);
    CleanupStack::PushL(fileMan);
	
	TBuf<55> pathROM;
	TBuf<55> pathWINS;
	TBuf<55> pathCOPY;
	TBuf<55> pathTEST;

	pathROM.Append(KResourceFileROM);
	pathWINS.Append(KResourceFile);
	pathCOPY.Append(KResourceFileCOPY);
	pathTEST.Append(KResourceFileTEST);

	pathROM.Append(aResourceNr);
	pathWINS.Append(aResourceNr);
	pathCOPY.Append(aResourceNr);
	pathTEST.Append(aResourceNr);

	iLog->Put(pathROM);
    //check if landmark is flashed
	if (!BaflUtils::FileExists(fs, pathROM))
		{
        iLog->Put(_L("Landmark is NOT flashed, rename resource file"));
        //Landmark is not flashed rename the file before copy a own defiend file.
        //since landmark is not flashed the file should exist hence leaving if it is not found!	
		iLog->Put(pathWINS);
		iLog->Put(pathCOPY);

		TInt err = fileMan->Rename(pathWINS, pathCOPY, CFileMan::EOverWrite);
		if (err != KErrNone && err != KErrNotFound)
			iLog->Put(_L("Problem renaming original global categories file"));
        }
     else
        {
        iLog->Put(_L("Landmark is flashed, copy resource file"));
        }
	iLog->Put(pathTEST);
	iLog->Put(pathWINS);

    User::LeaveIfError(fileMan->Copy(pathTEST, pathWINS, CFileMan::EOverWrite));
 
    CleanupStack::PopAndDestroy(2, &fs);
	iLog->Put(_L("CopyResourceFileL Done"));
    }

// ---------------------------------------------------------
// CPosTp105::RemoveResourceFileL
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CPosTp105::RemoveResourceFileL(const TDesC& aResourceNr)
    {
	iLog->Put(_L("RemoveResourceFileL"));

    RFs fs;
    User::LeaveIfError(fs.Connect());
    CleanupClosePushL(fs);
    CFileMan* fileMan = CFileMan::NewL(fs);
    CleanupStack::PushL(fileMan);

	TBuf<55> pathROM;
	TBuf<55> pathWINS;
	TBuf<55> pathCOPY;
	TBuf<55> pathTEST;

	pathROM.Append(KResourceFileROM);
	pathWINS.Append(KResourceFile);
	pathCOPY.Append(KResourceFileCOPY);
	pathTEST.Append(KResourceFileTEST);

	pathROM.Append(aResourceNr);
	pathWINS.Append(aResourceNr);
	pathCOPY.Append(aResourceNr);
	pathTEST.Append(aResourceNr);
    
    //check if the default display name resource file exist in rom
    //if landmarks is flashed it is just okej to remove
    // file in on the c drive.
	iLog->Put(pathROM);
    if (BaflUtils::FileExists(fs, pathROM))
       {
        iLog->Put(_L("Landmark is flashed, delete resource file"));
        //in this case it is same just to remove it form c if it exists:
		iLog->Put(pathWINS);
        if (BaflUtils::FileExists(fs, pathWINS))
            {
            User::LeaveIfError(fileMan->Delete(pathWINS));
            }
        }
    else
        {
        iLog->Put(_L("Landmark is NOT flashed, delete resource file and rename the old file"));
        //If a copy exist this should be used, but if it does not
        //exist the file is not removed since it is hard to know
        //if anything has gone wrong.
		iLog->Put(pathCOPY);
		iLog->Put(pathWINS);
        if(BaflUtils::FileExists(fs, pathCOPY))
            {
            //first delete the used file, if it exist
            if (BaflUtils::FileExists(fs, pathWINS))
                {
                User::LeaveIfError(fileMan->Delete(pathWINS));
                }
            //Rename the copy file
            User::LeaveIfError(fileMan->Rename(pathCOPY, pathWINS, CFileMan::EOverWrite));
            }
        }  
    CleanupStack::PopAndDestroy(2);
	iLog->Put(_L("RemoveResourceFileL Done"));
    }
    
//  End of File

