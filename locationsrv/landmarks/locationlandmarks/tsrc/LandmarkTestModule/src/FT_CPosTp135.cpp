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

#include "FT_CPosTp135.h"
#include <EPos_CPosLandmarkDatabase.h>
#include <EPos_CPosLmDatabaseManager.h>


// ================= CONSTANTS =======================

_LIT(KRemoteDb1, "test://RemoteDb1.ldb");
_LIT(KRemoteDb2, "test://RemoteDb2.ldb");
_LIT(KLocalDb1, "file://C:SOMEDB.LDB");
_LIT(KLocalDbName1, "SOMEDB");
_LIT(KRemoteDbName1, "RemoteDb1");
_LIT(KRemoteDbName2, "RemoteDb2");
_LIT(KRemoteDb3, "test://RemoteDb3.ldb");
_LIT(KRemoteDb4, "test://RemoteDb4.ldb");
_LIT(KRemoteDb5, "RemoteDb5.ldb");
_LIT(KLocalDb2, "file://C:SOMELOCALDB.LDB");
_LIT(KRemoteNonExistingDb, "test://NonExistingDb.ldb");

// ================= MEMBER FUNCTIONS =======================

// Cleanup for RPointerArray<HPosLmDatabaseInfo> objects
void CleanupTestPointerArray(TAny* aArray)

    {
    (static_cast<RPointerArray<HPosLmDatabaseInfo>*>(aArray))->ResetAndDestroy();
    }

// Cleanup for RPointerArray<HPosLmDatabaseInfo> objects
void CleanupTestPointerArray2(TAny* aArray)

    {
    
    (static_cast<RPointerArray<HPosLmDatabaseInfo>*>(aArray))->Reset();
    }

// ---------------------------------------------------------
// CPosTp135::CloseTest
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CPosTp135::CloseTest()
    {
	iLog->Log(_L("CloseTest"));
	
	iDbArray->Reset();
	delete iDbArray;
	iDbArray = NULL;
	
    TRAPD(err, RemoveAllLmDatabasesL());
    if (err != KErrNone) iLog->Log(_L("RemoveAllLmDatabasesL() leaved"));
        
    }
    
// ---------------------------------------------------------
// CPosTp135::StartL
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CPosTp135::StartL()
    {
    _LIT(KEmptyProtocol, "");
    _LIT(KRemoteProtocol, "test");

    TInt err=KErrNone;
    iErrorsFound = KErrNone;

    RemoveAllLmDatabasesL();
       
    iDbArray = NULL;
    iDbArray = new (ELeave) CDesCArrayFlat(5);

	// Create defaultdb
    CPosLmDatabaseManager* dbManager = CPosLmDatabaseManager::NewL();
    CleanupStack::PushL(dbManager);

    // TC Step 1 Create a remote database (uses the test provider implementation, using test as identifier 
    // (compare with file)	
	HPosLmDatabaseInfo* db1 = HPosLmDatabaseInfo::NewLC(KRemoteDb1);
	dbManager->CreateDatabaseL(*db1);
	    
    // TC Step 2 Register db
    dbManager->RegisterDatabaseL(*db1);
    TPosLmDatabaseSettings settings = db1->Settings();
    TPosLmDatabaseName bufName(KRemoteDbName1);
    settings.SetDatabaseName(bufName);
    dbManager->ModifyDatabaseSettingsL(KRemoteDb1, settings);
    CleanupStack::PopAndDestroy(db1);
    
    // TC Step 3 Create local db and try to register it
    HPosLmDatabaseInfo* localDb1 = HPosLmDatabaseInfo::NewLC(KLocalDb1);
    dbManager->CreateDatabaseL(*localDb1);
    
    settings = db1->Settings();
    TPosLmDatabaseName bufName2(KLocalDbName1);
    settings.SetDatabaseName(bufName2);
    dbManager->ModifyDatabaseSettingsL(KLocalDb1, settings);
    
    TRAP(err, dbManager->RegisterDatabaseL(*localDb1));
    if (err != KErrNotSupported) 
    {
    iLog->Log(_L("Register a local db should leave with KErrNotSupported"))	;
    User::Leave(err);
    }
    CleanupStack::PopAndDestroy(localDb1);
    
    // TC Step 4 Create another remote database and register it
    db1 = HPosLmDatabaseInfo::NewLC(KRemoteDb2);
	dbManager->CreateDatabaseL(*db1);    
    // Register db,
    dbManager->RegisterDatabaseL(*db1);
    settings = db1->Settings();
    TPosLmDatabaseName bufName3(KRemoteDbName2);
    settings.SetDatabaseName(bufName3);
    dbManager->ModifyDatabaseSettingsL(KRemoteDb2, settings);
    CleanupStack::PopAndDestroy(db1);    

    // TC Step 5 List all dbs without specifying protocol
    CDesCArray* list = dbManager->ListDatabasesLC();
    // Verify list should contain two remote and one local database
    iDbArray->AppendL(KLocalDb1);
    iDbArray->AppendL(KRemoteDb1);
    iDbArray->AppendL(KRemoteDb2);
    VerifyDbListL(list);
    CleanupStack::PopAndDestroy(list);
    
    // TC Step 6 List all dbs but specify protocol (protocol "local")
    list = dbManager->ListDatabasesLC(_L("file"));
    iDbArray->Delete(0, iDbArray->Count());
    iDbArray->AppendL(KLocalDb1);
    //Verify list should contain one local database
    VerifyDbListL(list);
    CleanupStack::PopAndDestroy(list);

    // TC Step 7 List all dbs but specify protocol (protocol "test")
    list = dbManager->ListDatabasesLC(KRemoteProtocol);
    //Verify list should contain two remote databases
    iDbArray->Reset();
    iDbArray->AppendL(KRemoteDb1);
    iDbArray->AppendL(KRemoteDb2);
    //Verify list should contain one local database
    VerifyDbListL(list);
    CleanupStack::PopAndDestroy(list);
    
    // TC Step 8 Also test with empty protocol, All existing dbs will be returned
    list = dbManager->ListDatabasesLC(_L(""));
    iDbArray->Reset();
    iDbArray->AppendL(KLocalDb1);
    iDbArray->AppendL(KRemoteDb1);
    iDbArray->AppendL(KRemoteDb2);
    //Verify list
    VerifyDbListL(list);
    CleanupStack::PopAndDestroy(list);    
    
    // TC Step 9 Retrive info about all dbs without specifying protocol
    HPosLmDatabaseInfo* dbInfo01 = HPosLmDatabaseInfo::NewLC(KLocalDb1);
    HPosLmDatabaseInfo* dbInfo02 = HPosLmDatabaseInfo::NewLC(KRemoteDb1);
    HPosLmDatabaseInfo* dbInfo03 = HPosLmDatabaseInfo::NewLC(KRemoteDb2);
    
    RPointerArray<HPosLmDatabaseInfo> myDbInfoList;
    CleanupStack::PushL(TCleanupItem(CleanupTestPointerArray2, &myDbInfoList));
    
    RPointerArray<HPosLmDatabaseInfo> infoList;
    CleanupStack::PushL(TCleanupItem(CleanupTestPointerArray, &infoList));
        
    dbManager->ListDatabasesL(infoList);
    // Verify list should contain all dbs

    myDbInfoList.Append(dbInfo01); 
    myDbInfoList.Append(dbInfo02); 
    myDbInfoList.Append(dbInfo03);
    
    VerifyDbInfoListL(infoList, myDbInfoList);
    CleanupStack::PopAndDestroy(&infoList);
    myDbInfoList.Reset();
    CleanupStack::PushL(TCleanupItem(CleanupTestPointerArray, &infoList));
    
    // TC Step 10 Retrive info about dbs specifying remote protocol
    dbManager->ListDatabasesL(infoList, KRemoteProtocol);
    // Verify list should contain two remote databases
    myDbInfoList.Append(dbInfo03); myDbInfoList.Append(dbInfo02);
    VerifyDbInfoListL(infoList, myDbInfoList);
    CleanupStack::PopAndDestroy(&infoList);
    myDbInfoList.Reset();
    CleanupStack::PushL(TCleanupItem(CleanupTestPointerArray, &infoList));
    
    // TC Step 11 Retrive info but specify empty protocol,
    dbManager->ListDatabasesL(infoList, KEmptyProtocol);
    // Verify list should contain all databases
    myDbInfoList.Append(dbInfo01);myDbInfoList.Append(dbInfo02);myDbInfoList.Append(dbInfo03);
    VerifyDbInfoListL(infoList, myDbInfoList);
    CleanupStack::PopAndDestroy(&infoList);
    CleanupStack::PushL(TCleanupItem(CleanupTestPointerArray, &infoList));
    myDbInfoList.Reset();

    // TC Step 12 Also test when specifying KNullDesC
    dbManager->ListDatabasesL(infoList, KEmptyProtocol);
    // Verify list should contain all databases
    myDbInfoList.Append(dbInfo01);myDbInfoList.Append(dbInfo02);myDbInfoList.Append(dbInfo03);
    VerifyDbInfoListL(infoList, myDbInfoList);
    CleanupStack::PopAndDestroy(&infoList);
    CleanupStack::PushL(TCleanupItem(CleanupTestPointerArray, &infoList));
    myDbInfoList.Reset();

    // TC Step 13 Unregister one remote db and list dbs
    dbManager->UnregisterDatabaseL(KRemoteDb1);
    
    list = dbManager->ListDatabasesLC(KRemoteProtocol);
    // Verify list should contain one remote databases
    iDbArray->Delete(0, iDbArray->Count());
    iDbArray->AppendL(KRemoteDb2);
    VerifyDbListL(list);
    CleanupStack::PopAndDestroy(list);

    // TC Step 14 Retrieves info about dbs, specify remote protocol
    dbManager->ListDatabasesL(infoList, KRemoteProtocol);
    // Verify list, should contain one remote
    myDbInfoList.Append(dbInfo03);
    VerifyDbInfoListL(infoList, myDbInfoList);
    CleanupStack::PopAndDestroy(&infoList);
    myDbInfoList.Reset();
    CleanupStack::PushL(TCleanupItem(CleanupTestPointerArray, &infoList));
    
    // TC Step 15 Unregisters all remote dbs
    dbManager->UnregisterAllDatabasesL(KRemoteProtocol);
    // Retrieves info about all dbs without specifying protocol
    CleanupStack::PopAndDestroy(&infoList);
    CleanupStack::PushL(TCleanupItem(CleanupTestPointerArray, &infoList));
    dbManager->ListDatabasesL(infoList);
    // Verify list should contain one local database
    myDbInfoList.Append(dbInfo01);
    VerifyDbInfoListL(infoList, myDbInfoList);
    myDbInfoList.Reset();

    CleanupStack::PopAndDestroy(&infoList);
    CleanupStack::PopAndDestroy(&myDbInfoList);
    CleanupStack::PopAndDestroy(dbInfo03);
	CleanupStack::PopAndDestroy(dbInfo02);
	CleanupStack::PopAndDestroy(dbInfo01);
    
    // TC Step 16 set the default database to a remote database
    TRAP(err, dbManager->SetDefaultDatabaseUriL(KRemoteDb1));
    if (err != KErrNotSupported) 
    {
    iLog->Log(_L("ERROR: SetDefaultDatabaseUriL to a remote db should leave with KErrNotSupported"))	;
    User::Leave(err);
    }
    
    // TC Step 17 Try to register a landmark database with an incorrect URI
    db1 = HPosLmDatabaseInfo::NewLC(_L("test:/awd/C:Pelle.ldb"));
    TRAP(err, dbManager->RegisterDatabaseL(*db1));
    if (err != KErrNotSupported) 
    {
    	iLog->Log(_L("ERROR: RegisterDatabaseL with incorrect URI should leave with KErrNotSupported (1)"))	;
    	User::Leave(err);

    }
    CleanupStack::PopAndDestroy(db1);
    
    // TC Step 18 Try to register a landmark database with an incorrect URI
    db1 = HPosLmDatabaseInfo::NewLC(_L("test:/\"/C:Pelle.ldb"));
    TRAP(err, dbManager->RegisterDatabaseL(*db1));
    if (err != KErrNotSupported)
    {
    	iLog->Log(_L("ERROR: RegisterDatabaseL with incorrect URI should leave with KErrNotSupported (2)"))	;
    	User::Leave(err);

    }
    CleanupStack::PopAndDestroy(db1);
    
    // TC Step 19 Try to register a landmark database with an incorrect URI
    db1 = HPosLmDatabaseInfo::NewLC(_L("file:/awd/C:C:Pelle.ldb"));
    TRAP(err, dbManager->RegisterDatabaseL(*db1));
    if (err != KErrNotSupported) 
    {
    	iLog->Log(_L("ERROR: RegisterDatabaseL with incorrect URI should leave with KErrNotSupported (3)"))	;
    	User::Leave(err);

    }
    CleanupStack::PopAndDestroy(db1);
    
    // TC Step 20 Try to register a landmark database with an incorrect URI
    db1 = HPosLmDatabaseInfo::NewLC(_L("file:/ /C:C:Pelle.ldb"));
    TRAP(err, dbManager->RegisterDatabaseL(*db1));
    if (err != KErrNotSupported) 
    {
    	iLog->Log(_L("ERROR: RegisterDatabaseL with incorrect URI should leave with KErrNotSupported (4)"))	;
    	User::Leave(err);

    }
    CleanupStack::PopAndDestroy(db1);
    
    // TC Step 21 Try to un-register a landmark database with an incorrect URI
    TRAP(err, dbManager->UnregisterDatabaseL(_L("test:/ /C:Pelle.ldb")));
    if (err != KErrNotSupported) 
    {
    	iLog->Log(_L("ERROR: UnregisterDatabaseL with incorrect URI should leave with KErrArgument (1)"))	;
    	User::Leave(err);

    }
    
    // TC Step 22 Try to un-register a landmark database with an incorrect URI
    TRAP(err, dbManager->UnregisterDatabaseL(_L("file: //C:C:Pelle.ldb")));
    if (err != KErrNotSupported) 
    {
    	iLog->Log(_L("ERROR: UnregisterDatabaseL with incorrect URI should leave with KErrArgument (2)"))	;
    	User::Leave(err);

    }

    // TC Step 23 Try to register a local landmark database
    db1 = HPosLmDatabaseInfo::NewLC(KLocalDb1);
    TRAP(err, dbManager->RegisterDatabaseL(*db1));
    if (err != KErrNotSupported)
    {
    	iLog->Log(_L("ERROR: RegisterDatabaseL a local db should leave with KErrNotSupported"))	;
    	User::Leave(err);

    }
    CleanupStack::PopAndDestroy(db1);
    
    // TC Step 24 Try to unregister a local landmark database
    TRAP(err, dbManager->UnregisterDatabaseL(KLocalDb1));
    if (err != KErrNotSupported) 
    {
    	iLog->Log(_L("ERROR:UnRegisterDatabaseL a local db should leave with KErrNotSupported"))	;
    	User::Leave(err);

    }
    
    // TC Step 25 Unregisters all dbs using empty protocol
    // make sure that one is registered
    db1 = HPosLmDatabaseInfo::NewLC(KRemoteDb1);
    dbManager->RegisterDatabaseL(*db1);
    TRAP(err, dbManager->UnregisterAllDatabasesL(_L("")));
    if (err != KErrArgument) 
    {
    	iLog->Log(_L("ERROR: UnregisterAllDatabasesL with empty protocol should leave with KErrArgument"))	;
    	User::Leave(err);

    }
        
    // TC Step 26 Unregisters all dbs using KNullDesC
    // make sure that one is registered
    
    TRAP(err, dbManager->UnregisterAllDatabasesL(KNullDesC));
    if (err != KErrArgument) 
    {
    	iLog->Log(_L("ERROR: UnregisterAllDatabasesL with KNullDesc protocol should leave with KErrArgument"))	;
    	User::Leave(err);

    }
    
    // TC Step 27 Unregisters all dbs using local protocol
    // make sure that one is registered
    
    TRAP(err, dbManager->UnregisterAllDatabasesL(_L("file")));
    if (err != KErrNotSupported) 
    {
    	iLog->Log(_L("ERROR: UnregisterAllDatabasesL with file protocol should leave with KErrArgument(1)"))	;
    	User::Leave(err);

    }
    
    // TC Step 28 Unregisters all dbs using local protocol
    // make sure that one is registered
    TRAP(err, dbManager->UnregisterAllDatabasesL(_L("file://")));
    if (err != KErrNotSupported) 
    {
    	iLog->Log(_L("ERROR: UnregisterAllDatabasesL with file protocol should leave with KErrNotSupported(2)"))	;
    	User::Leave(err);

    }
	    
    // TC 29 try to register again, should leave with KErrAlreadyExists
    
    TRAP(err, dbManager->RegisterDatabaseL(*db1));
    if (err != KErrAlreadyExists) 
    {
    	iLog->Log(_L("ERROR: RegisterDatabaseL already registered db should leave with KErrAlreadyExists"))	;
    	User::Leave(err);

    }
	
	CleanupStack::PopAndDestroy(db1);
	
	// TC Step 30 Copy database and verify that it is registered		
    dbManager->UnregisterAllDatabasesL(_L("test"));
    db1 = HPosLmDatabaseInfo::NewLC(KRemoteDb3);
	dbManager->CreateDatabaseL(*db1);    
    // Register db,
    dbManager->RegisterDatabaseL(*db1);
    CleanupStack::PopAndDestroy(db1);
    dbManager->CopyDatabaseL(KRemoteDb3, KRemoteDb4);
    // Only list "remote" dbs
    list = dbManager->ListDatabasesLC(_L("test"));
    iDbArray->Reset();
    iDbArray->AppendL(KRemoteDb3);
    iDbArray->AppendL(KRemoteDb4);
    
    VerifyDbListL(list);
    CleanupStack::PopAndDestroy(list);
    
    // TC Step 31) Check DatabaseExistsL method
    TBool result = dbManager->DatabaseExistsL(KRemoteDb3);
    if (!result)
    {
    	iLog->Log(_L("DatabaseExistsL should return ETrue"))	;
    	User::Leave(-1);

    }
    result = dbManager->DatabaseExistsL(KRemoteNonExistingDb);
    if (result) 
    {
    	iLog->Log(_L("DatabaseExistsL should return EFalse"))	;
    	User::Leave(-1);
    }
    
    // TC Step 32) Try to copy remote db to local
    TRAP(err, dbManager->CopyDatabaseL(KRemoteDb3, KLocalDb2));
    if (err != KErrArgument)
    {
    	iLog->Log(_L("Copy from remote to local should leave with KErrArgument"))	;
    	User::Leave(err);
    }
    
    // TC Step 33) Try to copy remote db to local
    TRAP(err, dbManager->CopyDatabaseL(KRemoteDb3, KRemoteDb5));
    if (err != KErrArgument) 
    {
    	iLog->Log(_L("Copy from remote to unspecified should leave with KErrArgument"))	;
    	User::Leave(err);

    }

    CleanupStack::PopAndDestroy(dbManager);
    if (iErrorsFound != KErrNone) 
    	{
    	iLog->Log(_L("Errors found in TP135"))	;
    	User::Leave(-1);

    	}
    else iLog->Log(_L("CPosTp135 Passed"));
	}

// ---------------------------------------------------------
// CPosTp135::VerifyDbListL
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CPosTp135::VerifyDbListL(CDesCArray* aDbArray)
    {
    iLog->Log(_L("VerifyDbListL"));
    if (aDbArray->Count() != iDbArray->Count())
        {
        TBuf<100> buf;
        buf.Format(_L("List was %d should be %d "), aDbArray->Count(), iDbArray->Count());
        
        iLog->Log(buf);
        User::Leave(KErrNone);
        }

    TInt found = 0;
    TInt count = aDbArray->Count();
    TInt i = 0;
        
    for (i = 0; i<count; i++)
        {
        // Below for debug purposes
        
        TInt pos;
        if (aDbArray->Find(iDbArray->MdcaPoint(i), pos) != KErrNone)
            {
            iLog->Log(_L("Did Not Found:"));
            iLog->Log(iDbArray->MdcaPoint(i));
            iLog->Log(_L("------------------"));
            }
        else
            {
            iLog->Log(iDbArray->MdcaPoint(i));
            found++;
            }
        }
        
    if (found != count)
        {
        
        iLog->Log(_L("List was not correct"));
        User::Leave(-1);
        
        }
        
    }
    
// ---------------------------------------------------------
// CPosTp135::VerifyDbInfoListL
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CPosTp135::VerifyDbInfoListL(RPointerArray<HPosLmDatabaseInfo> aDbInfoList,
                                  RPointerArray<HPosLmDatabaseInfo> aDbInfoList2)
    {
    iLog->Log(_L("VerifyDbInfoListL"));
    
    TBuf<100> debug;

    if (aDbInfoList.Count() != aDbInfoList2.Count())
        {
        TBuf<100> buf;
        buf.Format(_L("List was %d should be %d "), aDbInfoList.Count(), aDbInfoList2.Count());
        
        iLog->Log(buf);
        User::Leave(KErrNone);
        
        }
        
    TBool found = EFalse;
    TInt count = aDbInfoList.Count();
    TInt j=0;
    
    for (j = 0; j<count; j++)
        {
        TInt i = 0;
        found = EFalse;
        iLog->Log(_L("Looking for: "));
        iLog->Log(aDbInfoList[j]->DatabaseUri());
            
        for (i = 0; i<count && !found; i++)
            {
            if (((aDbInfoList[j])->DatabaseUri()).CompareC((aDbInfoList2[i])->DatabaseUri()) == KErrNone)
                {
                iLog->Log(_L("Found:"));
                iLog->Log(aDbInfoList2[i]->DatabaseUri());
                found = ETrue;
                TPosLmDatabaseSettings settings = aDbInfoList[j]->Settings();
                
                if (aDbInfoList[j]->IsDefault()) 
                {
                	iLog->Log(_L("Db should never be default"));
        			User::Leave(KErrNone);
                }
                
                if (aDbInfoList[j]->DatabaseUri() == KRemoteDb1)
                    {
                    if (settings.DatabaseName() != KRemoteDbName1)
                    	{
                    	iLog->Log(_L("Incorrect Databasename for remote db1"));
        				User::Leave(-1);
                    	}
                    if (aDbInfoList[j]->Protocol() != _L("test"))
                    	{
                    	iLog->Log(_L("Incorrect protocol for remote db1"));
        				User::Leave(-1);
                    	}
                    if (aDbInfoList[j]->DatabaseMedia() != EMediaRemote)
                    	{
                    	iLog->Log(_L("Incorrect media for remote db1"));
        				User::Leave(-1);
                    	}
                    if (aDbInfoList[j]->DatabaseDrive() != 'U')
                    	{
                    	iLog->Log(_L("Incorrect drive for remote db1"));
        				User::Leave(-1);
                    	}
                    }
                else if (aDbInfoList[j]->DatabaseUri() == KRemoteDb2)
                    {
                    if (settings.DatabaseName() != KRemoteDbName2) 
                    	{
                    	iLog->Log(_L("Incorrect Databasename for remote db2"));
        				User::Leave(-1);
                    	}
                    if (aDbInfoList[j]->Protocol() != _L("test")) 
                    	{
                    	iLog->Log(_L("Incorrect protocol for remote db2"));
        				User::Leave(-1);
                   		}
                    if (aDbInfoList[j]->DatabaseMedia() != EMediaRemote)
                    	{
                    	iLog->Log(_L("Incorrect media for remote db2"));
        				User::Leave(-1);
                    	}
                    if (aDbInfoList[j]->DatabaseDrive() != 'U') 
                    	{
                    	iLog->Log(_L("Incorrect drive for remote db2"));
        				User::Leave(-1);
                    	}
                    }
                else
                    {
                    // Local db
                    if (aDbInfoList[j]->Protocol() != _L("file")) 
                    	{
                    	iLog->Log(_L("Incorrect protocol for local db"));
        				User::Leave(-1);
                    	}
                    if (settings.DatabaseName() != KLocalDbName1) 
                    	{
                    	iLog->Log(_L("Incorrect Databasename for local db"));
        				User::Leave(-1);
                    	}

                             

                    if (aDbInfoList[j]->DatabaseDrive() != 'C')
                    	{
                    	iLog->Log(_L("Incorrect drive for remote db1"));
        				User::Leave(-1);
                    	}
                    
                    }

                if (aDbInfoList[j]->Size() <1) 
                {
                	iLog->Log(_L("Size should have a value"));
        			User::Leave(-1);
                }
                if (!settings.IsAttributeSet(TPosLmDatabaseSettings::EName)) 
                {
                	iLog->Log(_L("Name attribute should be set"));
        			User::Leave(-1);
                }

                }
            }
        if (!found)
            {
           
           	iLog->Log(_L("List was not correct"));
        	User::Leave(-1);
            }
        }
    }
    
//  End of File
