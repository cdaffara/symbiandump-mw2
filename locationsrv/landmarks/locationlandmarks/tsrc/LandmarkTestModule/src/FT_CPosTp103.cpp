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

#include "FT_CPosTp103.h"
#include <EPos_CPosLandmarkDatabase.h>
#include <EPos_CPosLmDatabaseManager.h>
#include <EPos_CPosLmTextCriteria.h>
           
// ================= MEMBER FUNCTIONS =======================

// ---------------------------------------------------------
// CPosTp103::CloseTest
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CPosTp103::CloseTest()
    {
	iLog->Log(_L("CloseTest"));
	
    TRAPD(err, RemoveAllLmDatabasesL());
    if (err != KErrNone) iLog->Log(_L("RemoveAllLmDatabasesL() leaved"));
    }
    
// ---------------------------------------------------------
// CPosTp103::StartL
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CPosTp103::StartL()
    {
    // TInt expNrOfDbs = 0;

    RemoveAllLmDatabasesL();

    TestPartCopyDatabasesL();
	}

// ---------------------------------------------------------
// CPosTp103::TestPartCopyDatabasesL
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CPosTp103::TestPartCopyDatabasesL()
    {
    iLog->Log(_L("TestPartCopyDatabasesL"));

    _LIT(KDbName1, "TP103_Database1.ldb");
    _LIT(KDbNameNonExisting, "NonExisting_TP103_Database1.ldb");
    _LIT(KDbNameCopy1, "file://C:TP103_DATABASE1COPY1.LDB");
    _LIT(KDbNameCopy5, "file://C:TP103_DATABASE2COPY5.LDB");
    _LIT(KDbNameCopy6, "file://C:TP103_DATABASE2COPY6.LDB");
    
    _LIT(KDbName7MissingDrive, "file://TP103_DATABASE7.LDB");
    _LIT(KDbName7, "file://C:TP103_DATABASE7.LDB");
    
    // DB on drive that does no exist
    _LIT(KDbName8, "file://R:TP103_DATABASE8.LDB");
    _LIT(KDbName8_1, "file://C:TP103_DATABASE8_1.LDB");
    
    _LIT(KDefaultCopyDb, "CopyOfDefaultDb.ldb");
    _LIT(KDefaultCopyDbUri, "file://C:CopyOfDefaultDb.ldb");
    
    _LIT(KDbSourceNameMissingExtension, "file://C:TP103_DATABASE2COPY6");
    _LIT(KDbTargetNameMissingExtension, "file://C:TP103_DATABASE2COPY_666");
    
    _LIT(KRemoteDatabaseUri, "http://RemoteLmDb.ldb");
    
#ifdef __WINS__
	// D on wins
	_LIT(KDbNameCopy2, "file://D:TP103_DATABASE1COPY2.LDB");
	_LIT(KDbNameCopy2MissingDrive, "file://TP103_DATABASE1COPY2.LDB");
    _LIT(KDbNameCopy3, "file://D:TP103_DATABASE1COPY3.LDB");
#else	
    _LIT(KDbNameCopy2, "file://F:TP103_DATABASE1COPY2.LDB");
    _LIT(KDbNameCopy2MissingDrive, "file://TP103_DATABASE1COPY2.LDB");
    _LIT(KDbNameCopy3, "file://F:TP103_DATABASE1COPY3.LDB");
#endif
    _LIT(KDbNameCopy4, "file://C:TP103_DATABASE1COPY4.LDB");

    _LIT(KDbNameEmptyUri1, "");
    _LIT(KDbNameEmptyUri2, "file://c");

    _LIT(KDummyDb, "file://SomeDummyDbName.ldb");
    TInt err = KErrNone;

    iLog->Log(_L("Warning: this test case requires E: drive"));
    
    CPosLmDatabaseManager* dbManager = CPosLmDatabaseManager::NewL();
    CleanupStack::PushL(dbManager);

    // check the count of databases that could not be delted since they were in use
    CDesCArray* dbList = dbManager->ListDatabasesLC();
    TInt count = dbList->Count();
    TInt verifyDbcount;
    
    // 1) Create a new database
    HPosLmDatabaseInfo* dbInfo1 = HPosLmDatabaseInfo::NewLC(KDbName1);
    dbManager->CreateDatabaseL(*dbInfo1);
    CPosLandmarkDatabase* db1 = CPosLandmarkDatabase::OpenL(KDbName1);
    CleanupStack::PopAndDestroy(dbInfo1);
    CleanupStack::PushL(db1);
    if (db1->IsInitializingNeeded())
        {
        iLog->Log(_L("db1->InitializeL"));
        ExecuteAndDeleteLD(db1->InitializeL());
        }
    CleanupStack::PopAndDestroy(db1);
    
    // 2) Copy db to file system, include file:// in URI
    dbManager->CopyDatabaseL(KDbName1, KDbNameCopy1);

    // 3) Copy db to removable media, include file:// in URI
    dbManager->CopyDatabaseL(KDbName1, KDbNameCopy2);

    // 4) Copy the db on removable media to removable media
    dbManager->CopyDatabaseL(KDbNameCopy2, KDbNameCopy3);

    // 5) Copy the db from removable media to c drive
    dbManager->CopyDatabaseL(KDbNameCopy2, KDbNameCopy4);

    // 6) Verify that the four databases (copied in step 2-5) exists
    if (!dbManager->DatabaseExistsL(KDbNameCopy1)) 
    {
    	iLog->Log(_L("DatabaseCopy1 should exist"));
    	User::Leave(-1);
    }
    if (!dbManager->DatabaseExistsL(KDbNameCopy2))
    {
    	iLog->Log(_L("DatabaseCopy2 should exist"));
    	User::Leave(-1);
    }
    if (!dbManager->DatabaseExistsL(KDbNameCopy3))
    {
    	iLog->Log(_L("DatabaseCopy3 should exist"));
    	User::Leave(-1);
    }
    if (!dbManager->DatabaseExistsL(KDbNameCopy4))
    {
    	iLog->Log(_L("DatabaseCopy4 should exist"));
    	User::Leave(-1);
    }

    // 7) Repat step 5, but this time should the target of the copy operation exist
    TRAP(err, dbManager->CopyDatabaseL(KDbNameCopy2, KDbNameCopy4));
    if (err != KErrAlreadyExists) 
    {
    	iLog->Log(_L("CopyDatabaseL should leave with KErrAlreadyExists"));
    	User::Leave(err);
    }

    // 8) Take a read lock (using a search operation) on db and try to copy it
    CPosLandmarkDatabase* lmd = CPosLandmarkDatabase::OpenL(KDbNameCopy2);
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
    
    // Try to copy db with read lock
    TRAP(err, dbManager->CopyDatabaseL(KDbNameCopy2, KDbNameCopy5));
    if (err != KErrInUse)
    {
    	iLog->Log(_L("8) Copy should leave with KErrInUse"));
    	User::Leave(err);
    }
    ExecuteAndDeleteLD(iOperation);
        
    // 9) Take a write lock on db and try to copy it
    iOperation = lmd->RemoveAllLandmarksL();
    TRAP(err, dbManager->CopyDatabaseL(KDbNameCopy2, KDbNameCopy6));
    if (err != KErrLocked) iLog->Log(_L("9) Copy should leave with KErrLocked"));
    ExecuteAndDeleteLD(iOperation);
    
    CleanupStack::PopAndDestroy(textCriteria);
    CleanupStack::PopAndDestroy(landmarkSearch);
    CleanupStack::PopAndDestroy(lmd);

    // 10) Try to copy a database that does not exist
    TRAP(err, dbManager->CopyDatabaseL(KDbNameNonExisting, _L("DummyTarget.ldb")));
    if (err != KErrNotFound) 
    {
    	iLog->Log(_L("10) CopyDatabaseL should leave with KErrNotFound"));
    	User::Leave(err);
    }
    
    // 11) Try to copy an existing db to a nonexisting path/target
    TRAP(err, dbManager->CopyDatabaseL(KDbName1, _L("c://pelle//DummyTarget.ldb")));
    if (err != KErrArgument)
    {
    	iLog->Log(_L("11) CopyDatabaseL should leave with KErrNotFound"));
    	User::Leave(err);
    }

    // 12) Try to copy an existing db to remote path/target
    TRAP(err, dbManager->CopyDatabaseL(KDbName1, KRemoteDatabaseUri));
    if (err != KErrArgument) //LogErrorAndLeave(_L("12) CopyDatabaseL should leave with KErrBadName"));
    {
    	iLog->Log(_L("12) CopyDatabaseL should leave with KErrBadName"));
    	User::Leave(err);
    }
    
    // 13) Try to copy a db on c drive without specifying a drive in the source uri
    dbManager->CopyDatabaseL(KDbName1, KDbNameCopy5);
    verifyDbcount = count +6;
    VerifyNrOfDatabasesL(verifyDbcount, KDbNameCopy5);

    // 14) Try to copy a db on removable media drive without specifying the drive in the source uri
    TRAP(err, dbManager->CopyDatabaseL(KDbNameCopy2MissingDrive, KDummyDb));
    if (err != KErrNotFound)
    {
    	iLog->Log(_L(" CopyDatabaseL should leave with KErrNotFound"));
    	User::Leave(err);
    }

    // 15) Try to copy a db on removable media drive without specifying the drive in the target uri
    if (!dbManager->DatabaseExistsL(KDbNameCopy2)) 
    {
    	iLog->Log(_L("DatabaseCopy2 should exist"));
    	User::Leave(KErrNone);	
    }
    TRAP(err, dbManager->CopyDatabaseL(KDbNameCopy2, KDbName7MissingDrive));
    if (err != KErrNone) 
    {
    	iLog->Log(_L("KDbNameCopy2 should be copied"));
    	User::Leave(err);
    }
    verifyDbcount = count + 7;
    VerifyNrOfDatabasesL(verifyDbcount, KDbName7);

    // 16) Try to copy without .ldb in source URI
    TRAP(err, dbManager->CopyDatabaseL(KDbSourceNameMissingExtension,  _L("DummyTarget.ldb")));
    if (err != KErrArgument) 
    {
    	iLog->Log(_L("CopyDatabaseL should leave with KErrArgument"));
    	User::Leave(err);	
    }

    // 17) Try to copy without .ldb in target URI
    TRAP(err, dbManager->CopyDatabaseL(KDbName1, KDbTargetNameMissingExtension));
    if (err != KErrArgument) 
    {
    	iLog->Log(_L("CopyDatabaseL should leave with XXX"));
    	User::Leave(err);
    }

    // 18) Try to copy db to itself
    TRAP(err, dbManager->CopyDatabaseL(KDbName1, KDbName1));
    if (err != KErrAlreadyExists) 
    {
    	iLog->Log(_L("CopyDatabaseL should leave with KErrAlreadyExists"));
    	User::Leave(err);
    }

    // 19) Try to copy db to itself XXX Should be removed, same as step18
    TRAP(err, dbManager->CopyDatabaseL(KDbName1, KDbName1));
    if (err != KErrAlreadyExists) 
    {
    	iLog->Log(_L("CopyDatabaseL should leave with KErrAlreadyExists"));
    	User::Leave(err);
    }

    // 20) Try to copy db with empty target URI
    TRAP(err, dbManager->CopyDatabaseL(KDbName1, KDbNameEmptyUri1));
    if (err != KErrArgument)
    {
    	iLog->Log(_L("20a) CopyDatabaseL should leave with KErrArgument"));
    	User::Leave(err);
    }
    TRAP(err, dbManager->CopyDatabaseL(KDbName1, KDbNameEmptyUri2));
    if (err != KErrArgument)
    {
    	iLog->Log(_L("20b) CopyDatabaseL should leave with KErrArgument"));
    	User::Leave(err);
    }

    // 21) Try to copy db with empty source URI
    TRAP(err, dbManager->CopyDatabaseL(KDbNameEmptyUri1, KDummyDb));
    if (err != KErrArgument)
    {
    	iLog->Log(_L("21a) CopyDatabaseL should leave with KErrArgument"));
    	User::Leave(err);
    }
    TRAP(err, dbManager->CopyDatabaseL(KDbNameEmptyUri2, KDummyDb));
    if (err != KErrArgument)
    {
    	iLog->Log(_L("21b) CopyDatabaseL should leave with KErrArgument"));
    	User::Leave(err);
    }

    // 22) Try to copy db with target URI that is longer than allowed
    TRAP(err, dbManager->CopyDatabaseL(_L("c:\\SomeVeryLongFilename1234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890.ldb"), KDummyDb));
    if (err != KErrArgument)
    {
    	iLog->Log(_L("22) CopyDatabaseL should leave with KErrArgument"));
    	User::Leave(err);
    }

    // 23) Try to copy db with source URI that is longer than allowed
	TRAP(err, dbManager->CopyDatabaseL(KDbName1, _L("c:\\SomeVeryLongFilename1234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890.ldb")));
    if (err != KErrArgument) 
    {
    	iLog->Log(_L("23) CopyDatabaseL should leave with KErrArgument"));
    	User::Leave(err);
    }

    // 24) Try to copy the default db
    dbManager->SetDefaultDatabaseUriL(KDbName7);
    HBufC* defaultDbUri;
    defaultDbUri = dbManager->DefaultDatabaseUriLC();    
    dbManager->CopyDatabaseL(*defaultDbUri, KDefaultCopyDb);

    verifyDbcount = count + 8;
    VerifyNrOfDatabasesL(verifyDbcount, KDefaultCopyDbUri);
    
    // 25) Delete default db and then copy a new db to its location
    dbManager->DeleteDatabaseL(*defaultDbUri);
    dbManager->CopyDatabaseL(KDefaultCopyDbUri, *defaultDbUri);
    verifyDbcount = count + 8;
    VerifyNrOfDatabasesL(verifyDbcount , KDefaultCopyDbUri);
    VerifyNrOfDatabasesL(verifyDbcount , *defaultDbUri);
    
    // 26) Try to copy a db on a drive that does not exist
    TRAP(err, dbManager->CopyDatabaseL(KDbName8, KDbName8_1));
    if (err != KErrNotReady)
    {
    	iLog->Log(_L("26) CopyDatabaseL should leave with KErrNotReady"));
    	User::Leave(err);
    }
    VerifyNrOfDatabasesL(verifyDbcount, KDefaultCopyDbUri);
        
    // 27) Try to copy defaultdb to db with long filenames
    _LIT(KLongFileName, "file://C:ABCDEFGHIJKLMNOPQRSTABCDEFGHIJKLMNOPQRSTABCDEFGHIJKLMNOPQRSTABCDEFGHIJKLMNOPQRSTABCDEFGHIJKLMNOPQRST.ldb");
    
    TRAP(err, dbManager->CopyDatabaseL(KDefaultCopyDbUri, KLongFileName));
    if (err != KErrArgument) 
    {
    	iLog->Log(_L("27) CopyDatabaseL should leave"));
    	User::Leave(err);
    }
    
    VerifyNrOfDatabasesL(verifyDbcount);
    
    // 28) Try to copy db with long file name
    TRAP(err, dbManager->CopyDatabaseL(KLongFileName, _L("SomeDummyFile.ldb")));
    if (err != KErrArgument)
    {
    	iLog->Log(_L("28) CopyDatabaseL should leave "));
    	User::Leave(err);
    }
    VerifyNrOfDatabasesL(verifyDbcount);
    
	CleanupStack::PopAndDestroy(defaultDbUri);
	CleanupStack::PopAndDestroy(dbList);
	CleanupStack::PopAndDestroy(dbManager);
    }

// ---------------------------------------------------------
// CPosTp103::VerifyNrOfDatabasesL
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CPosTp103::VerifyNrOfDatabasesL(TInt aExpNrOfDatabases, const TDesC&  aDatabaseUri)
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
