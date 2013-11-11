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

#include "FT_CPosTp102.h"
#include <EPos_CPosLandmarkDatabase.h>
#include <EPos_CPosLmDatabaseManager.h>
#include "FT_DatabaseUtility.h"
#include <EPos_CPosLmTextCriteria.h>
           
// ================= MEMBER FUNCTIONS =======================

// ---------------------------------------------------------
// CPosTp102::CloseTest
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CPosTp102::CloseTest()
    {
	iLog->Log(_L("CloseTest"));
	
    TRAPD(err, RemoveAllLmDatabasesL());
    if (err != KErrNone) iLog->Log(_L("RemoveAllLmDatabasesL() leaved"));
    
    }

// ---------------------------------------------------------
// CPosTp102::StartL
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CPosTp102::StartL()
    {
    _LIT(KDatabase1, "Tp102_Database1.ldb");
    _LIT(KDatabaseUri1, "file://C:Tp102_Database1.ldb");
    _LIT(KDatabase1MissingProtocol, "C:Tp102_Database1.ldb");
    _LIT(KDatabase1MissingDrive, "Tp102_Database1.ldb");
    _LIT(KDatabase1MissingExtension, "C:Tp102_Database1");
    _LIT(KDatabase1FullPath, "c:\\private\\100012a5\\DBS_101FE978_Tp102_Database1.ldb");
    _LIT(KRemoteDatabaseUri, "http://c:eposlm.ldb");
    _LIT(KDatabase3, "C:Tp102_DBMS_Database3.ldb");
    _LIT(KDatabaseUri3, "file://C:Tp102_DBMS_Database3.ldb");
    // Change here if path to private area ever changes
    _LIT(KDatabase4FullPath, "c:\\private\\100012a5\\DBS_101FE978_Tp102_DefaultDatabase.ldb");
    _LIT(KDatabase4, "Tp102_DefaultDatabase.ldb");
    _LIT(KDatabaseUri4, "file://C:Tp102_DefaultDatabase.ldb");
    
    _LIT(KDatabaseUri5, "file://R:NonExistingDrive.ldb");

#ifdef __WINS__
	// D on wins
    _LIT(KDbOnMedia, "D:TP102EPOSLM.LDB");
    _LIT(KDbOnMediaUri, "file://D:TP102EPOSLM.LDB");
    
    _LIT(KDbOnMedia2, "D:TP102EPOSLM_2.LDB");
    _LIT(KDbOnMedia2Uri, "file://D:TP102EPOSLM_2.LDB");
    _LIT(KDbOnMedia2MissingDrive, "file://TP102EPOSLM_2.LDB");
#else

    _LIT(KDbOnMedia, "F:TP102EPOSLM.LDB");
    _LIT(KDbOnMediaUri, "file://F:TP102EPOSLM.LDB");
    
    _LIT(KDbOnMedia2, "F:TP102EPOSLM_2.LDB");
    _LIT(KDbOnMedia2Uri, "file://F:TP102EPOSLM_2.LDB");
    _LIT(KDbOnMedia2MissingDrive, "file://TP102EPOSLM_2.LDB");
#endif

    _LIT(KEmptyURI, "");

    iLog->Log(_L("Warning: this test case requires E: drive"));
    
    TInt err=KErrNone;

    RemoveAllLmDatabasesL();
	// Create defaultdb
    CPosLmDatabaseManager* dbManager = CPosLmDatabaseManager::NewL();
    CleanupStack::PushL(dbManager);
    HPosLmDatabaseInfo* dbInfo4 = HPosLmDatabaseInfo::NewLC(KDatabase4);
	dbManager->CreateDatabaseL(*dbInfo4);
	CleanupStack::PopAndDestroy(dbInfo4);
	dbManager->SetDefaultDatabaseUriL(KDatabase4);
	
    // 1) Remove all landmark db
    RemoveAllLmDatabasesL();
    
    // check the count of databases that could not be delted since they were in use
    CDesCArray* dbList = dbManager->ListDatabasesLC();
    TInt count = dbList->Count();


    // 2) Verify no *.ldb files on file system    
    VerifyNrOfDatabasesL(count);

    // 3) Check default db
    HBufC* defaultDbUri;
    defaultDbUri = dbManager->DefaultDatabaseUriLC();
    
    // 4) List all database, should return 0
    VerifyNrOfDatabasesL(count);

    // 5) Check that database does not exist
    if (dbManager->DatabaseExistsL(KDatabaseUri1))
        {
        
        iLog->Log(_L("This database should not exist"));
        User::Leave(-1);
        }

    // 6) Create a new database using db api
    HPosLmDatabaseInfo* dbInfo1 = HPosLmDatabaseInfo::NewLC(KDatabase1);
    dbManager->CreateDatabaseL(*dbInfo1);
    CleanupStack::PopAndDestroy(dbInfo1);

    // 7) List dbs, should contain db created in step6
    CDesCArray* dbArray = dbManager->ListDatabasesLC();
    if (dbArray->Count() != count+1) 
    {
    	iLog->Log(_L("Database does not contain the last created db"));
        User::Leave(-1);
    }

	iLog->Log((*dbArray)[0]);
    // 8) Check that dabase does not exist
    if (!dbManager->DatabaseExistsL(KDatabaseUri1))
        {
        
        	iLog->Log(_L("This database should exist"));
        	User::Leave(-1);
        }
	CleanupStack::PopAndDestroy(dbArray);
	
    // 9)
    CPosLandmarkDatabase* db1 = CPosLandmarkDatabase::OpenL(KDatabaseUri1);
    CleanupStack::PushL(db1);
    if (db1->IsInitializingNeeded())
        {
        iLog->Log(_L("db1->InitializeL"));
        ExecuteAndDeleteLD(db1->InitializeL());
        }
    CleanupStack::PopAndDestroy(db1);

        
    CleanupStack::PopAndDestroy(defaultDbUri);

    // 11) Open the default db
    CPosLandmarkDatabase* defaultDatabase = CPosLandmarkDatabase::OpenL();
    CleanupStack::PushL(defaultDatabase);

	iLog->Log(_L("12) List all databases"));
    // 12) List all databases
	TInt verifyDbCount = count +2;
    VerifyNrOfDatabasesL(verifyDbCount, KDatabaseUri4);

    // 13)
    iLog->Log(_L("13) Does defaultDB exist"));
    if (!dbManager->DatabaseExistsL(KDatabaseUri4))
        {
        
        	iLog->Log(_L("The default database should have been created"));
        	User::Leave(-1);
        }

    // 14) Try to delete the physical file while db is open

	// Fix this path
    err = iFileSession.Delete(KDatabase4FullPath);
    if (err != KErrInUse)
        {
        
       	iLog->Log(_L("It was possible to delete default db from file system"));
        User::Leave(err);
        }
	CleanupStack::PopAndDestroy(defaultDatabase);
	
    // 15) Create a landmark database using the DBMS API
    iLog->Log(_L("15) Create a landmark database using the DBMS API"));
    FTDatabaseUtility::CreateDatabaseL(KDatabase3);
    // 16)
    verifyDbCount = count +3;
    VerifyNrOfDatabasesL(verifyDbCount, KDatabaseUri3);

    // 17) Create a database on a removable media
    iLog->Log(_L("17) Create a database on a removable media"));
    HPosLmDatabaseInfo* dbInfoMedia = HPosLmDatabaseInfo::NewLC(KDbOnMedia);
    dbManager->CreateDatabaseL(*dbInfoMedia);
    CleanupStack::PopAndDestroy(dbInfoMedia);

    // 18) Verify database
    verifyDbCount = count + 4;
    VerifyNrOfDatabasesL(verifyDbCount, KDbOnMediaUri);

    // 19) Retrieve the display name for the default db, it should be different from actual the filename
    iLog->Log(_L("19) Retrieve the display name for the default db, it should be different from actual the filename"));
    HBufC* defUri = dbManager->DefaultDatabaseUriLC();
    HPosLmDatabaseInfo* dbInfoReadback = HPosLmDatabaseInfo::NewLC(*defUri);
    dbManager->GetDatabaseInfoL(*dbInfoReadback);
    TPosLmDatabaseSettings settings = dbInfoReadback->Settings();
    TPtrC name1 = settings.DatabaseName();
    CleanupStack::PopAndDestroy(dbInfoReadback);
    CleanupStack::PopAndDestroy(defUri);
    

    // 20) Set a new db as default
    dbManager->SetDefaultDatabaseUriL(KDbOnMediaUri);
    
    // 21) Retrieve the display name for the new default db, should be the same as in step 19
    iLog->Log(_L("21) Retrieve the display name for the new default db, should be the same as in step 19"));
    dbInfoReadback = HPosLmDatabaseInfo::NewLC(KDbOnMediaUri);
    dbManager->GetDatabaseInfoL(*dbInfoReadback);
    TPosLmDatabaseSettings settings2 = dbInfoReadback->Settings();
    TPtrC name2 = settings2.DatabaseName();
    CleanupStack::PopAndDestroy(dbInfoReadback);
    
    if (name1.Compare(name2) != 0)
    	{
    	iLog->Log(_L("Names does not match"));
        User::Leave(-1);
    	}
    
    _LIT(KFileColonSlashSlash, "file://");
    _LIT(KCColonSomeTestDbDotLdb, "c:SomeTestDb.ldb");
    _LIT(KFileColonSlashSlashSomeTestDb2DotLdb, "file://SomeTestDb2.ldb");
    _LIT(KCColonSomeTestDb3DotDoc, "c:SomeTestDb3.doc");
    _LIT(KEmptyString, "");
    _LIT(KFileColonSlahSlashCColonDotLdb, "file://C:.LDB");
    _LIT(KVeryLongString, "c:\\SomeVeryLongFilename1234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890.ldb");
    
    HPosLmDatabaseInfo* dbInfoFileColonSlashSlash = HPosLmDatabaseInfo::NewLC(KFileColonSlashSlash);
    HPosLmDatabaseInfo* dbInfoCColonSomeTestDbDotLdb = HPosLmDatabaseInfo::NewLC(KCColonSomeTestDbDotLdb);
    HPosLmDatabaseInfo* dbInfoFileColonSlashSlashSomeTestDb2DotLdb = HPosLmDatabaseInfo::NewLC(KFileColonSlashSlashSomeTestDb2DotLdb);
    HPosLmDatabaseInfo* dbInfoCColonSomeTestDb3DotDoc = HPosLmDatabaseInfo::NewLC(KCColonSomeTestDb3DotDoc);
    HPosLmDatabaseInfo* dbInfoEmptyString = HPosLmDatabaseInfo::NewLC(KEmptyString);
    HPosLmDatabaseInfo* dbInfoFileColonSlahSlashCColonDotLdb = HPosLmDatabaseInfo::NewLC(KFileColonSlahSlashCColonDotLdb);
    HPosLmDatabaseInfo* dbInfoVeryLongString = HPosLmDatabaseInfo::NewLC(KVeryLongString);
    
    
    // 22) Createa db with same name as an existing
    dbInfo1 = HPosLmDatabaseInfo::NewLC(KDatabase1);
	TRAP(err, dbManager->CreateDatabaseL(*dbInfo1));
	CleanupStack::PopAndDestroy(dbInfo1);
	if (err != KErrAlreadyExists) 
	{
		iLog->Log(_L("22 Should leave with KErrAlreadyExist"));
        User::Leave(err);
	}

	// 23) Create a new database using the db management API with a nonsense URI
	TRAP(err, dbManager->CreateDatabaseL(*dbInfoFileColonSlashSlash));
	if (err != KErrArgument) 
	{
		iLog->Log(_L("23 Should leave with KErrArgument"));
        User::Leave(err);	
	}
	
	// 24) Create a new database using the db management API with a URI specifying a remote database
	HPosLmDatabaseInfo* dbInfoRem = HPosLmDatabaseInfo::NewLC(KRemoteDatabaseUri);
	TRAP(err, dbManager->CreateDatabaseL(*dbInfoRem));
	CleanupStack::PopAndDestroy(dbInfoRem);
	
	if (err != KErrNotSupported)
	{
		iLog->Log(_L("24 Should leave with KErrNotSupported"));
        User::Leave(err);
	}
	
	// 25) Create a new database using the db management API with a URI with missing protocol
	TRAP(err, dbManager->CreateDatabaseL(*dbInfoCColonSomeTestDbDotLdb));
	if (err != KErrNone) 
	{
		iLog->Log(_L("25 Should NOT leave"));
        User::Leave(err);
	}
	
	// 26) Create a new database using the db management API with a URI with missing drive
	TRAP(err, dbManager->CreateDatabaseL(*dbInfoFileColonSlashSlashSomeTestDb2DotLdb));
	if (err != KErrNone) 
	{
		iLog->Log(_L("26 Should NOT leave"));
        User::Leave(err);
	}

	// 27) Create a new database using the db management API with a URI that does not end with *.ldb
	TRAP(err, dbManager->CreateDatabaseL(*dbInfoCColonSomeTestDb3DotDoc));
	if (err != KErrArgument) 
	{
		iLog->Log(_L("27 Should leave with KErrArgument"));
        User::Leave(err);
	}
	
	// 28) Create a new database using the db management API with a URI containing the path to the database file
	HPosLmDatabaseInfo* dbInfoUri1 = HPosLmDatabaseInfo::NewLC(KDatabaseUri1);
	TRAP(err, dbManager->CreateDatabaseL(*dbInfoUri1));
	CleanupStack::PopAndDestroy(dbInfoUri1);
	if (err != KErrAlreadyExists)
	{
		iLog->Log(_L("28 Should leave with KErrAlreadyExist"));
        User::Leave(err);
	}
	
	// 29) Create a new database using the db management API with an empty URI
	TRAP(err, dbManager->CreateDatabaseL(*dbInfoEmptyString));
	if (err != KErrArgument) 
	{
		iLog->Log(_L("29 Should leave with KErrArgument"));
        User::Leave(err);
	}
	
	// 29b) Create a new database using the db management API with an empty URI (containing file://c:.ldb)
	TRAP(err, dbManager->CreateDatabaseL(*dbInfoFileColonSlahSlashCColonDotLdb));
	if (err != KErrArgument) 
	{
		iLog->Log(_L("29b Should leave with KErrArgument"));
        User::Leave(err);
	}
	
    // 30) Create a new database using the db management API with a URI that is longer than allowed
    TRAP(err, dbManager->CreateDatabaseL(*dbInfoVeryLongString));
	if (err != KErrArgument) 
	{
		iLog->Log(_L(" Should leave with KErrArgument"));
        User::Leave(err);
	}
    
    CleanupStack::PopAndDestroy(dbInfoVeryLongString);
    CleanupStack::PopAndDestroy(dbInfoFileColonSlahSlashCColonDotLdb);
    CleanupStack::PopAndDestroy(dbInfoEmptyString);
    CleanupStack::PopAndDestroy(dbInfoCColonSomeTestDb3DotDoc);
    CleanupStack::PopAndDestroy(dbInfoFileColonSlashSlashSomeTestDb2DotLdb);
    CleanupStack::PopAndDestroy(dbInfoCColonSomeTestDbDotLdb);
    CleanupStack::PopAndDestroy(dbInfoFileColonSlashSlash);
      
    // 31 Try to delete an existing db
    dbManager->DeleteDatabaseL(KDatabaseUri1);
    // 32) Check that it is deleted from file system
    iLog->Log(_L("Trap error below:"));
    verifyDbCount = count + 5;
    TRAP(err, VerifyNrOfDatabasesL(verifyDbCount, KDatabaseUri1));
    if (err != KErrNotFound) 
    {
    	iLog->Log(_L("32 Should not be found"));
        User::Leave(err);
    }

    // 33)  Delete same file again, according to header file, nothing should happen
    // when the file does not exist
    TRAP(err, dbManager->DeleteDatabaseL(KDatabaseUri1));
    if (err != KErrNone) 
    {
    	iLog->Log(_L("33 DeleteDatbaseL, nothing should happen"));
        User::Leave(err);
    }
    iLog->Log(_L("After DeleteDatabaseL"));

    CFileMan* fileMan = NULL;
    fileMan = CFileMan::NewL(iFileSession);
	CleanupStack::PushL(fileMan);
	
	dbInfo1 = HPosLmDatabaseInfo::NewLC(KDatabase1);
	dbManager->CreateDatabaseL(*dbInfo1);
	CleanupStack::PopAndDestroy(dbInfo1);
	verifyDbCount = count +6;
	VerifyNrOfDatabasesL(verifyDbCount, KDatabaseUri1);
    // 34 Set an existing non-default db to be read-only (attribute found in F32FILE.H)
    err = fileMan->Attribs(KDatabase1FullPath, KEntryAttReadOnly, KEntryAttNormal, TTime(0));
    if (err != KErrNone) 
    {
    	iLog->Log(_L("Attributes could not be changed for KDatabase1FullPath"));
        User::Leave(err);	
    }
        
    // 35) Try to delete db with read only
	TRAP(err, dbManager->DeleteDatabaseL(KDatabaseUri1));
	if (err != KErrAccessDenied) 
	{
		iLog->Log(_L("35 Err delete of KDatabaseUri1"));
        User::Leave(err);	
	}
	
	// Change back to writeable (attribute found in F32FILE.H)
    err = fileMan->Attribs(KDatabase1FullPath, KEntryAttNormal, KEntryAttReadOnly, TTime(0));
    if (err != KErrNone)
    {
    	iLog->Log(_L("Attributes could not be changed KDatabase1FullPath"));
        User::Leave(err);	
    }
	
    // 36) Take a read lock on a db by using a search operation	
    CPosLandmarkDatabase* lmd = CPosLandmarkDatabase::OpenL(KDatabaseUri1);
    CleanupStack::PushL(lmd);
    
    if (lmd->IsInitializingNeeded())
        {
        ExecuteAndDeleteLD(lmd->InitializeL()); 
        }
        
    CPosLandmarkSearch* landmarkSearch = CPosLandmarkSearch::NewL(*lmd);
    CleanupStack::PushL(landmarkSearch);
    
    CPosLmTextCriteria* textCriteria = CPosLmTextCriteria::NewLC();
    textCriteria->SetTextL(_L("*"));
    iOperation = landmarkSearch->StartLandmarkSearchL(*textCriteria, EFalse);
    
    // 37) Try to delete db with read lock
    TRAP(err, dbManager->DeleteDatabaseL(KDatabaseUri1));
	if (err != KErrInUse) 
	{
	iLog->Log(_L("37) Err delete of Read Locked KDatabaseUri1"));
     User::Leave(err);		
	}
	
	ExecuteAndDeleteLD(iOperation);
	
    // 38) List databases
	verifyDbCount = count + 6;
    VerifyNrOfDatabasesL(verifyDbCount, KDatabaseUri1);

    //39) Take a write lock on a db by trying to delete all landmarks operations
	iOperation = lmd->RemoveAllLandmarksL();
    // 40) Try to delete db with write lock
    TRAP(err, dbManager->DeleteDatabaseL(KDatabaseUri1));
	if (err != KErrInUse)
	{
	iLog->Log(_L("40) Err delete of Write Locked KDatabaseUri1"));
     User::Leave(err);		
	}
	
	ExecuteAndDeleteLD(iOperation);

    // 41) List databases
	verifyDbCount = count + 6;
    VerifyNrOfDatabasesL(verifyDbCount, KDatabaseUri1);
    
    CleanupStack::PopAndDestroy(textCriteria);
    CleanupStack::PopAndDestroy(landmarkSearch);
    CleanupStack::PopAndDestroy(lmd);
    
    CleanupStack::PopAndDestroy(fileMan);

    // 42) Try to delete a non-existing URI
    TRAP(err, dbManager->DeleteDatabaseL(_L("c:\\NonsenseURI")));
    if (err != KErrArgument) 
        {
        
        iLog->Log(_L("42: Should leave with KErrArgument11"));
     	User::Leave(err);		
        }
        
    // 43) Try to delete a remote URI
    TRAP(err, dbManager->DeleteDatabaseL(KRemoteDatabaseUri));
    if (err!=KErrNotSupported) 
    {
    	iLog->Log(_L("43) Err KRemoteDatabaseUri"));
     	User::Leave(err);		
    }
    
    // 44) Try to delete a local db specifying URI with missing protocol
    dbManager->DeleteDatabaseL(KDatabase1MissingProtocol);
    
    // 45) List databases
    verifyDbCount = count + 5;
    VerifyNrOfDatabasesL(verifyDbCount);

    // 46) Try to delete a db on removable media excluding drive in URI
    // Since the DeleteDatabaseL just returns if the db is not found, verify that no db has been removed

    HPosLmDatabaseInfo* dbInfoMedia2 = HPosLmDatabaseInfo::NewLC(KDbOnMedia2);
	dbManager->CreateDatabaseL(*dbInfoMedia2);
	CleanupStack::PopAndDestroy(dbInfoMedia2);
	verifyDbCount = count +6;
	VerifyNrOfDatabasesL(verifyDbCount, KDbOnMedia2Uri);
	TRAP(err , dbManager->DeleteDatabaseL(KDbOnMedia2MissingDrive));
	
    if (err != KErrNone)
        {
        TBuf<100> buffe;
        buffe.Format(_L("46) Wrong error when deleting KDbOnMedia2MissingDrive, was %d should be %d"), err, KErrNone);
        
        iLog->Log(buffe);
     	User::Leave(err);		
        }
    verifyDbCount = count + 6;
	VerifyNrOfDatabasesL(verifyDbCount, KDbOnMedia2Uri);
	
    // 47) Try to delete a db on c drive excluding drive in URI
    iLog->Log(_L("47) Try to delete a db on c drive excluding drive in URI"));
    dbInfo1 = HPosLmDatabaseInfo::NewLC(KDatabase1);
    TRAP(err, dbManager->CreateDatabaseL(*dbInfo1));
    CleanupStack::PopAndDestroy(dbInfo1);
    if (err != KErrNone && err != KErrAlreadyExists)
    {
    	iLog->Log(_L("Problem creating file KDatabase1"));
     	User::Leave(err);		
    }
	TRAP(err, dbManager->DeleteDatabaseL(KDatabase1MissingDrive));
	if (err != KErrNone)
	{
		iLog->Log(_L("Wrong errro when deleting file"));
     	User::Leave(err);
	}
	verifyDbCount = count + 6;
	VerifyNrOfDatabasesL(verifyDbCount);
	
	dbInfo1 = HPosLmDatabaseInfo::NewLC(KDatabase1);
	dbManager->CreateDatabaseL(*dbInfo1);
	CleanupStack::PopAndDestroy(dbInfo1);
	verifyDbCount = count +7;
	VerifyNrOfDatabasesL(verifyDbCount, KDatabaseUri1);
	
    // 48) Try to delete a db with URI that does not end with .ldb
    iLog->Log(_L("48) Try to delete a db with URI that does not end with .ldb"));
    TRAP(err, dbManager->DeleteDatabaseL(KDatabase1MissingExtension));
    if (err != KErrArgument) 
    {
    	iLog->Log(_L("48) Wrong error when deleting KDatabase1MissingExtension"));
     	User::Leave(err);
    }

    // 49) Try to delete a database with a URI that includes the path to the database
    iLog->Log(_L("49) Try to delete a database with a URI that includes the path to the database"));
    TRAP(err, dbManager->DeleteDatabaseL(KDatabase1FullPath));
    if (err != KErrArgument) 
    {
    	iLog->Log(_L("Wrong error when deleting KDatabase1FullPath"));
     	User::Leave(err);
    }
    verifyDbCount = count +7;
    VerifyNrOfDatabasesL(verifyDbCount);

    // 50 Delete with empty URI
    iLog->Log(_L("50) "));
    TRAP(err, dbManager->DeleteDatabaseL(KEmptyURI));
    if (err != KErrArgument) 
    {
    	iLog->Log(_L("Wrong error when deleting KEmptyURI"));
     	User::Leave(err);
    }

    // 51) Try to delete a db with URI that is longer than allowed
    iLog->Log(_L("51) "));
	TRAP(err, dbManager->DeleteDatabaseL(_L("SomeVeryLongFilename1234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890.ldb")));
	if (err != KErrArgument) 
	{
		iLog->Log(_L("Wrong error when deleting long filename file"));
     	User::Leave(err);
	}
    
    // 52) Try to delete the existing default db
    iLog->Log(_L("52) "));
    dbManager->SetDefaultDatabaseUriL(KDatabase1);
    dbManager->DeleteDatabaseL(KDatabase1);

    // 53) List databases
    iLog->Log(_L("53) "));
    verifyDbCount = count +6;
    VerifyNrOfDatabasesL(verifyDbCount);
    
    // 54 Create db on drive that does not exist
    HPosLmDatabaseInfo* dbInfoUri5 = HPosLmDatabaseInfo::NewLC(KDatabaseUri5);
    TRAP(err, dbManager->CreateDatabaseL(*dbInfoUri5));
    CleanupStack::PopAndDestroy(dbInfoUri5);
    if (err != KErrNotReady) 
    {
    	iLog->Log(_L("54) Wrong error when creating db on media that does not exist"));
     	User::Leave(err);
    }
    verifyDbCount = count +6;
    VerifyNrOfDatabasesL(verifyDbCount);
    
    // 55 Delete db on drive that does not exist
    TRAP(err, dbManager->DeleteDatabaseL(KDatabaseUri5));
    if (err != KErrNotReady) 
    {
    	iLog->Log(_L("55) Wrong error when deleting db on media that does not exist"));
     	User::Leave(err);
    }
    
    _LIT(KLongFileName, "file://C:ABCDEFGHIJKLMNOPQRSTABCDEFGHIJKLMNOPQRSTABCDEFGHIJKLMNOPQRSTABCDEFGHIJKLMNOPQRSTABCDEFGHIJKLMNOPQRST.ldb");
    
	// 56 Create db with long filename
	HPosLmDatabaseInfo* dbInfoLong = HPosLmDatabaseInfo::NewLC(KLongFileName); 
    TRAP(err, dbManager->CreateDatabaseL(*dbInfoLong));
    CleanupStack::PopAndDestroy(dbInfoLong);
    if (err != KErrArgument)
    {
    	iLog->Log(_L("57) Wrong error when creating db with long filename"));
     	User::Leave(err);
    }
    verifyDbCount = count +6;
    VerifyNrOfDatabasesL(verifyDbCount);
    
    // 57) Check that database with long filename does not exist
    if (dbManager->DatabaseExistsL(KLongFileName))
        {
        //LogErrorAndLeave(_L("Database with long filename should not exist"));
        iLog->Log(_L("57) Database with long filename should not exist"));
     	User::Leave(-1);
        }

    // 58) Check error ESLI-6ARN66, delete a db with strange filename
    HPosLmDatabaseInfo* dbInfoWeird = 
        HPosLmDatabaseInfo::NewLC(_L("file://c:!#§%&()=+`¥^®~'ßΩ-_,;.{}[]$£@0123456789Â‰ˆ≈ƒ÷.ldb"));
	dbManager->CreateDatabaseL(*dbInfoWeird);
	CleanupStack::PopAndDestroy(dbInfoWeird);
	verifyDbCount = count +7;
	VerifyNrOfDatabasesL(verifyDbCount);
	
	CleanupStack::PopAndDestroy(dbList);
	CleanupStack::PopAndDestroy(dbManager);
	
	TRAP(err, RemoveAllLmDatabasesL());
    if (err != KErrNone) 
        {
        
        iLog->Log(_L("RemoveAllLmDatabasesL should not leave with error"));
     	User::Leave(err);
        }
        
   TestDifferentUrisL();
        
	}

// ---------------------------------------------------------
// CPosTp102::TestDifferentUrisL
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CPosTp102::TestDifferentUrisL()
    {
    iLog->Log(_L("TestDifferentUrisL"));
    _LIT(KDbUri1, "file://:://c:::en helt \" sjuk .ldb databas\".ldb.ldb.ldb");
    _LIT(KDbUri2, "file://c:en helt sjuk databas.ldb");
    _LIT(KDbUri3, "file:///c:en helt sjuk databas.ldb");
    _LIT(KDbUri4, "file://c::en helt sjuk databas.ldb");
    _LIT(KDbUri5, "file://c:en helt sjuk .ldb databas.ldb");
    _LIT(KDbUri6, "file://c:en helt sjuk databas.ldb.ldb.ldb");
    _LIT(KDbUri7, "file://c:en helt \" sjuk databas.ldb");
    _LIT(KDbUri8, "file://c:en helt < sjuk databas.ldb");
    _LIT(KDbUri9, "file://c:en helt > sjuk databas.ldb");
    _LIT(KDbUri10, "file://c:en helt | sjuk databas.ldb");
    _LIT(KDbUri11, "file://c:en helt / sjuk databas.ldb");
    _LIT(KDbUri12, "file://c:en helt ? sjuk databas.ldb");
    _LIT(KDbUri13, "file://c:en helt \\ sjuk databas.ldb");
    _LIT(KDbUri14, "file://c:en helt : sjuk databas.ldb");
    // Below tested separately in step 58)
    //_LIT(KDbUri15, "file://c:!#§%&()=+`¥^®~'ßΩ-_,;.{}[]$£@0123456789Â‰ˆ≈ƒ÷.ldb");
    _LIT(KDbUri16, "file://c:qwertyuiopasdfghjklzxcvbnmQWERTYUIOPLKJHGFDSAZXCVBNM.ldb");
    _LIT(KDbUri17, "file://en otroligt helt sjuk databas.ldb");
    _LIT(KDbUri18, "c:en helsjuk databas.ldb");
    _LIT(KDbUri19, "file://c:\\system\\data\\en helt sjuk databas.ldb");
    _LIT(KDbUri20, "file://c:.ldb");
    _LIT(KDbUri21, "file://c:filename");
    _LIT(KDbUri22, "file://c:filename.dip");
    _LIT(KDbUri23, "file://c:filename. ldb");
    _LIT(KDbUri24, "file://c:*.ldb");
    _LIT(KDbUri25, "file://allan:en helt sjuk databas.ldb");
    _LIT(KDbUri26, "file://g:en helt sjuk databas.ldb");
    _LIT(KDbUri27, "file://c:filename.ldb");
    _LIT(KDbUri28, "file://c:012345678901234567890123456789012345678901234567890123456789.ldb");
    
    const TInt KNrOfUris = 8;
    const TDesC* listOfUris[] = {&KDbUri2,  &KDbUri5, &KDbUri6
    , &KDbUri16 , &KDbUri17, &KDbUri18
    , &KDbUri27, &KDbUri28
    };
    /* &KDbUri1*/

    const TInt KNrOfNonWorkingUris = 19;
    const TDesC* listOfUrisNonWorking[] = {&KDbUri1, &KDbUri3 ,&KDbUri4, &KDbUri7, &KDbUri8, &KDbUri9
    , &KDbUri10, &KDbUri11, &KDbUri12, &KDbUri13, &KDbUri14, &KDbUri19, &KDbUri20, &KDbUri21
    , &KDbUri22, &KDbUri23, &KDbUri24, &KDbUri25, &KDbUri26};
    
    TInt errorList[] = {KErrArgument, KErrBadName, KErrBadName, KErrBadName, KErrBadName ,KErrBadName,KErrBadName,
                   KErrBadName, KErrBadName, KErrArgument, KErrBadName, KErrArgument, KErrArgument,
                   KErrArgument, KErrArgument,KErrArgument, KErrBadName, KErrBadName, KErrNotReady};
        
    CPosLmDatabaseManager* dbManager = CPosLmDatabaseManager::NewL();
    CleanupStack::PushL(dbManager);	    
	    
    RemoveAllLmDatabasesL();
    TBuf<100> buf;
    // get the count of dbs that have notbeen deleted isnce they are in use
    CDesCArray* dbArray1 = dbManager->ListDatabasesLC();
    TInt count = dbArray1->Count();
    TInt verifyDbcount;
    
    for (TInt i=0;i<KNrOfUris;i++)
        {
        HPosLmDatabaseInfo* dbInfo = HPosLmDatabaseInfo::NewLC(*listOfUris[i]);
        iLog->Log(_L("Creating db:"));
        iLog->Log(*listOfUris[i]);
	    TRAPD(err, dbManager->CreateDatabaseL(*dbInfo));
	    if (err != KErrNone)
	        {
	        buf.Format(_L("Failed with err %d"), err);
	        iLog->Log(buf);
     		User::Leave(err);
	        }
	    else
	        {
	        iLog->Log(_L("Db created..."));
	        }
	    CleanupStack::PopAndDestroy(dbInfo);
	    verifyDbcount = count + i+1;
	    VerifyNrOfDatabasesL(verifyDbcount);
	    CDesCArray* dbArray = dbManager->ListDatabasesLC();
        TInt nr = dbArray->Count();
        
        buf.Format(_L("DB count %d"), nr);
        iLog->Log(buf);
        CleanupStack::PopAndDestroy(dbArray);
        }
        
    for (TInt i=0;i<KNrOfNonWorkingUris;i++)
        {
        HPosLmDatabaseInfo* dbInfo = HPosLmDatabaseInfo::NewLC(*listOfUrisNonWorking[i]);
        iLog->Log(_L("Trying to create db:"));
        iLog->Log(*listOfUrisNonWorking[i]);
	    TRAPD(err, dbManager->CreateDatabaseL(*dbInfo));
	    if (err != errorList[i])
	        {
	        buf.Format(_L("Failed with err %d should be %d"), err, errorList[i]);
	        iLog->Log(buf);
	       
	       	iLog->Log(buf);
	       	User::Leave(err);
	        }
	    CleanupStack::PopAndDestroy(dbInfo);
	    verifyDbcount = count + KNrOfUris;
	    VerifyNrOfDatabasesL(verifyDbcount);
        }
	
    CleanupStack::PopAndDestroy(dbArray1);
    CleanupStack::PopAndDestroy(dbManager);
    iLog->Log(_L("TestDifferentUrisL"));
    }
    
// ---------------------------------------------------------
// CPosTp102::VerifyNrOfDatabasesL
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CPosTp102::VerifyNrOfDatabasesL(TInt aExpNrOfDatabases, const TDesC&  aDatabaseUri)
    {
    iLog->Log(_L("VerifyNrOfDatabasesL"));
    CPosLmDatabaseManager* dbManager = CPosLmDatabaseManager::NewL();
    CleanupStack::PushL(dbManager);

    CDesCArray* dbArray = dbManager->ListDatabasesLC();
    TInt nr = dbArray->Count();
    if (nr != aExpNrOfDatabases) 
        {
        CleanupStack::PopAndDestroy(dbArray);
    	CleanupStack::PopAndDestroy(dbManager);
    
        TBuf<100> buf;
        buf.Format(_L("Wrong number of databases, was %d, should be %d"), nr, aExpNrOfDatabases);
        
        iLog->Log(buf);
	    User::Leave(-1);
        }
        
    TBool found = EFalse;
    if (aDatabaseUri != KNullDesC)
    	{
    	for (TInt i=0;i<nr;i++)
    		{
    		iLog->Log((*dbArray)[i]);
    		if ((*dbArray)[i].Compare(aDatabaseUri) == KErrNone) 
    		{
    		found = ETrue;
    		}
    		}
    	}

	CleanupStack::PopAndDestroy(dbArray);
    CleanupStack::PopAndDestroy(dbManager);
    
    if (aDatabaseUri != KNullDesC && !found) 
    	{
    		iLog->Log(_L("Database was not found"));
	       	User::Leave(KErrNotFound);
    	}
    }

//  End of File
