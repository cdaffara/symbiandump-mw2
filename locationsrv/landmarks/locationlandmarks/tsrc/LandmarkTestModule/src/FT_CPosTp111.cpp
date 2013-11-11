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

#include "FT_CPosTp111.h"
#include <f32file.h>
#include <CentralRepository.h>
#include <EPos_CPosLmDatabaseManager.h>

// CONSTANTS
const TUid KCRUidLandmarks = {0x101FE99B}; 
const TUint32 KLandmarkDefaultUri = 0x00000001;
_LIT(KFactoryDbUri, "file://c:eposlm.ldb");
//_LIT(KRepositoryFile, "c:\\private\\10202BE9\\[101FE99B].txt"); // 10202BE9 = SID for Central repository
_LIT(KRepositoryFile, "c:\\private\\10202BE9\\101FE99B.txt"); // 10202BE9 = SID for Central repository

// Resource files
//_LIT(KDefaultDbNameRsc, "eposlmdefaultdbname.rsc");
//_LIT(KDefaultDbRsc, "eposlmdefaultdb.rsc");
_LIT(KCompactDbLevelRsc, "eposlmcompactdblevel.rsc");
//_LIT(KInternalServicesRsc, "eposlmintservices.rsc");
_LIT(KGlobalCategoriesRsc, "eposlmglobalcategories.r01");
_LIT(KAsynchOperationsRsc, "eposlmasyncops.rsc");

// Resource files owned by process
_LIT(KLmServerRsc, "eposlmserver.rsc");

// ECom plugins
//_LIT(KConverterRsc, "101FDF85.rsc");
//_LIT(KLocalAccessProvRsc, "101FDF89.rsc");
//_LIT(KLocalDbManagerProvRsc, "101FE977.rsc");
//_LIT(KLocalSearchProvRsc, "101FDF86.rsc");

// Libraries
_LIT(KLandmarksDll, "eposlandmarks.dll");
_LIT(KLocalSearchDll, "eposlmsearchlib.dll");
_LIT(KXmlConverterDll, "eposlmconverter.dll");
_LIT(KDbManagerDll, "eposlmdbmanlib.dll");
_LIT(KIntServicesDll, "eposlmintservices.dll");
_LIT(KLocalAccessDll, "eposlmlocalaccess.dll");
_LIT(KLocalAccessProvDll, "eposlmlocalaccessprovider.dll");
_LIT(KLocalDbManProvDll, "eposlmlocaldbmanprovider.dll");
_LIT(KLocalSearchProvDll, "eposlmlocalsearchprovider.dll");
_LIT(KMultiDbSearchDll, "eposlmmultidbsearch.dll");
_LIT(KRecognizer, "eposlmrecognizer.dll");
_LIT(KLmServer, "eposlmserver.exe");
// KDummyDll should not be found! Only used to generate language sensitive 
// resource file for global categories. Removed during build process.
// _LIT(KDummyDll, "eposlandmarks_dummy.dll");

// ================= MEMBER FUNCTIONS =======================

// ---------------------------------------------------------
// CPosTp111::GetName
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CPosTp111::GetName(TDes& aName) const
    {
    _LIT(KTestName, "TP111 - Data caging and central repository");
    aName = KTestName;
    }

// ---------------------------------------------------------
// CPosTp111::StartL
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CPosTp111::StartL()
    {
#ifdef __WINS__
    LogErrorAndLeave(_L("This is a target test! Emulator execution is not supported."), KErrNotSupported);
#endif
    
    // TC step 1
    iLog->Put(_L("TC step 1"));
    TBool testSuccessful = FindResourceFilesL();
    
    // TC step 2
    iLog->Put(_L("TC step 2"));
    TBool subTestSuccessful = FindServerResourceFilesL();
    if (testSuccessful)
        {
        testSuccessful = subTestSuccessful;
        }
    
    // TC step 3
    /*
    iLog->Put(_L("TC step 3"));
    subTestSuccessful = FindEcomPluginsL();
    if (testSuccessful)
        {
        testSuccessful = subTestSuccessful;
        }
    */
    
    // TC step 4
    iLog->Put(_L("TC step 4"));
    subTestSuccessful = FindLibrariesL();
    if (testSuccessful)
        {
        testSuccessful = subTestSuccessful;
        }
        
    // TC steps 5-13
    iLog->Put(_L("TC step 5-13"));
    TestCentralRepositoryL();    
    
    // Summarize
    if (!testSuccessful)
        {
        LogErrorAndLeave(_L("Test failed with errors"));
        }
	}

// ---------------------------------------------------------
// CPosTp111::FindResourceFilesL
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
TBool CPosTp111::FindResourceFilesL()
    {
    iLog->Put(_L("FindResourceFilesL"));
    CDesCArray* rscFiles = new (ELeave) CDesCArrayFlat(8);
    CleanupStack::PushL(rscFiles);
    //rscFiles->AppendL(KDefaultDbNameRsc);
    //rscFiles->AppendL(KDefaultDbRsc);
    rscFiles->AppendL(KCompactDbLevelRsc);
    //rscFiles->AppendL(KInternalServicesRsc);
    rscFiles->AppendL(KGlobalCategoriesRsc);
    rscFiles->AppendL(KAsynchOperationsRsc);
   
    _LIT(KExpectedPath, "\\resource\\");
    
    TBool result = FindFilesAndVerifyPathL(*rscFiles, KExpectedPath);

    CleanupStack::PopAndDestroy(rscFiles);
    return result;
    }
    
// ---------------------------------------------------------
// CPosTp111::FindServerResourceFilesL
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
TBool CPosTp111::FindServerResourceFilesL()
    {
    iLog->Put(_L("FindServerResourceFilesL"));
    CDesCArray* rscFiles = new (ELeave) CDesCArrayFlat(8);
    CleanupStack::PushL(rscFiles);
    rscFiles->AppendL(KLmServerRsc);
   
    // To be updated when SID is known
    _LIT(KExpectedPath, "\\private\\101FDF81\\");
    
    TBool result = FindFilesAndVerifyPathL(*rscFiles, KExpectedPath);

    CleanupStack::PopAndDestroy(rscFiles);
    return result;
    }

// ---------------------------------------------------------
// CPosTp111::FindEcomPluginsL
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
TBool CPosTp111::FindEcomPluginsL()
    {
    iLog->Put(_L("FindEcomPluginsL"));
    CDesCArray* ecomPlugins = new (ELeave) CDesCArrayFlat(8);
    CleanupStack::PushL(ecomPlugins);
    //ecomPlugins->AppendL(KConverterRsc);
    //ecomPlugins->AppendL(KLocalAccessProvRsc);
    //ecomPlugins->AppendL(KLocalDbManagerProvRsc);
    //ecomPlugins->AppendL(KLocalSearchProvRsc);
    
    // To be updated with correct SID
    _LIT(KExpectedPath, "\\private\\101FDF81\\import\\"); // 101FDF81 = SID for LM srv
    
    TBool result = FindFilesAndVerifyPathL(*ecomPlugins, KExpectedPath);

    CleanupStack::PopAndDestroy(ecomPlugins);
    return result;
    }    


// ---------------------------------------------------------
// CPosTp111::FindLibrariesL
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
TBool CPosTp111::FindLibrariesL()
    {
    iLog->Put(_L("FindLibrariesL"));
    CDesCArray* libraries = new (ELeave) CDesCArrayFlat(8);
    CleanupStack::PushL(libraries);
    libraries->AppendL(KLandmarksDll);
    libraries->AppendL(KLocalSearchDll);
    libraries->AppendL(KXmlConverterDll);
    libraries->AppendL(KDbManagerDll);
    libraries->AppendL(KIntServicesDll);
    libraries->AppendL(KLocalAccessDll);
    libraries->AppendL(KLocalAccessProvDll);
    libraries->AppendL(KLocalDbManProvDll);
    libraries->AppendL(KLocalSearchProvDll);
    libraries->AppendL(KMultiDbSearchDll);
    libraries->AppendL(KRecognizer);
    libraries->AppendL(KLmServer);
    
    _LIT(KExpectedPath, "\\sys\\bin\\");
    
    TBool result = FindFilesAndVerifyPathL(*libraries, KExpectedPath);

    CleanupStack::PopAndDestroy(libraries);
    return result;
    }

// ---------------------------------------------------------
// CPosTp111::TestCentralRepositoryL
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CPosTp111::TestCentralRepositoryL()
    {
    iLog->Put(_L("TestCentralRepositoryL"));
    _LIT(KDbUri, "file://c:landmarks.ldb");
    CPosLmDatabaseManager* dbMan = CPosLmDatabaseManager::NewL();
    CleanupStack::PushL(dbMan);
    
    // 5. Set default database
    HPosLmDatabaseInfo* dbInfo = HPosLmDatabaseInfo::NewLC(KDbUri);
    TRAPD(err, dbMan->CreateDatabaseL(*dbInfo));
    CleanupStack::PopAndDestroy(dbInfo);
    if (err != KErrNone && err != KErrAlreadyExists)
        {
        LogErrorAndLeave(_L("Creating db failed"), err);
        }
    dbMan->SetDefaultDatabaseUriL(KDbUri);
    
    // 6. Retrieve default db using LMFW
    VerifyDefaultDbUriL(KDbUri, dbMan);
    
    // 7. Retrieve default db using central repository
    CRepository* repository = CRepository::NewLC(KCRUidLandmarks);    
    VerifyDefaultDbUriL(KDbUri, repository);

    // 8. Reset central repository
    User::LeaveIfError(repository->Reset(KLandmarkDefaultUri));
    
    // 9. Retrieve default db using LMFW
    VerifyDefaultDbUriL(KFactoryDbUri, dbMan);
    
    // 10. Retrieve default db using central repository
    VerifyDefaultDbUriL(KFactoryDbUri, repository);
    
    // 11. Remove central repository file on c:
    RFs fileServer;
    User::LeaveIfError(fileServer.Connect());
    CleanupClosePushL(fileServer);
    CFileMan* fileMan = CFileMan::NewL(fileServer);
    CleanupStack::PushL(fileMan);    
    err = fileMan->Delete(KRepositoryFile);
    if (err != KErrNone && err != KErrNotFound)
        {
        LogErrorAndLeave(_L("Unable to delete repository file"), err);
        }
    CleanupStack::PopAndDestroy(2, &fileServer);
    
    // 12. Retrieve default db using LMFW
    VerifyDefaultDbUriL(KFactoryDbUri, dbMan);
    
    // 13. Retrieve default db using central repository
    VerifyDefaultDbUriL(KFactoryDbUri, repository);
    
    CleanupStack::PopAndDestroy(2, dbMan);
    }
    
// ---------------------------------------------------------
// CPosTp111::FindFileL
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CPosTp111::FindFileL(const TDesC& aFile, TInt& aNrOfMatches, TDes& aPath)
    {
    iLog->Put(_L("FindFileL"));
    TBuf<200> msg;
    RFs fileServer;
    User::LeaveIfError(fileServer.Connect());
    CleanupClosePushL(fileServer);
    TDriveList driveList;
    fileServer.DriveList(driveList);
    CDirScan* scanner = CDirScan::NewLC(fileServer);

    // Search all drives for the file.    
    aNrOfMatches = 0;
    aPath = KNullDesC;
    for (TInt i = EDriveA; i <= EDriveZ; i++)
        {
        // Investigate if drive exists on this terminal.
        if (driveList[i])
            {
            TChar driveLetter;
            RFs::DriveToChar(i, driveLetter);
            TBuf<4> root;
            root.Append(driveLetter);
            root.Append(_L(":\\"));
            scanner->SetScanDataL(root, KEntryAttNormal, ESortByName|EAscending);
            
            // List all files in each folder on the drive
            CFileList* files = NULL;
            scanner->NextL(files);
            while (files)
                {
                CleanupStack::PushL(files);
                TPtrC currentFolder = scanner->FullPath();
                _LIT(KSearching, "Searching for file %S in folder %S");
                msg.Format(KSearching, &aFile, &currentFolder);
                iLog->Put(msg);                
                
                // Search the currentFolder for the specified file
                TFindFile fileSearcher(fileServer);
                TInt res = fileSearcher.FindByDir(aFile, currentFolder);
                if (res == KErrNone)
                    {
                    _LIT(KFileFound, "Found file %S in %S");
                    msg.Format(KFileFound, &aFile, &currentFolder);
                    iLog->Put(msg);
                    
                    aNrOfMatches++;
                    aPath = currentFolder;
                    }
                
                CleanupStack::PopAndDestroy(files);
                files = NULL;
                scanner->NextL(files);
                }          
            }
        }
        
    CleanupStack::PopAndDestroy(2, &fileServer);
    }
    
// ---------------------------------------------------------
// CPosTp111::FindFilesAndVerifyPathL
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
TBool CPosTp111::FindFilesAndVerifyPathL(
    const CDesCArray& aFileNames, 
    const TDesC& aExpectedPath)
    {
    iLog->Put(_L("FindFileL"));
    TBool successful = ETrue;
    TBuf<100> msg;
    TBool nrOfMatches = 0;
    TBuf<200> path;
    
    for (TInt i = 0; i < aFileNames.Count(); i++)
        {
        TPtrC fileName = aFileNames[i];
        FindFileL(fileName, nrOfMatches, path);
        if (path.Length() == 0)
            {
            _LIT(KFormat, "File %S not found.");
            msg.Format(KFormat, &fileName);
            iLog->PutError(msg);
            successful = EFalse;
            }
        else
            {
            if (nrOfMatches > 1)
                {
                _LIT(KFormat, "Found %d copies of file %S.");
                msg.Format(KFormat, nrOfMatches, &fileName);
                iLog->PutError(msg);
                successful = EFalse;
                }
            else
                {
                if (aExpectedPath != path)
                    {
                    _LIT(KFormat, "Found file %S at the wrong place, %S");
                    msg.Format(KFormat, &fileName, &path);
                    iLog->PutError(msg);
                    successful = EFalse;
                    }
                }
            }
        }
        
    return successful;    
    }
    
// ---------------------------------------------------------
// CPosTp111::VerifyDefaultDbUriL
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CPosTp111::VerifyDefaultDbUriL(
    const TDesC& aExpectedUri, 
    CPosLmDatabaseManager* aDbMan)
    {
    HBufC* dbUriFromLMFW = aDbMan->DefaultDatabaseUriLC();
    if (*dbUriFromLMFW != aExpectedUri)
        {
        TBuf<100> msg;
        _LIT(KFormat, "Unexpected db uri from LMFW, %S != %S");
        msg.Format(KFormat, dbUriFromLMFW, &aExpectedUri);
        LogErrorAndLeave(msg);
        }
    CleanupStack::PopAndDestroy(dbUriFromLMFW);
    }

// ---------------------------------------------------------
// CPosTp111::VerifyDefaultDbUriL
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CPosTp111::VerifyDefaultDbUriL(
    const TDesC& aExpectedUri, 
    CRepository* aRepository)
    {
    TBuf16<NCentralRepositoryConstants::KMaxUnicodeStringLength> dbUriFromRep;
    User::LeaveIfError(aRepository->Get(KLandmarkDefaultUri, dbUriFromRep));
    if (dbUriFromRep != aExpectedUri)
        {
        TBuf<100> msg;
        _LIT(KFormat, "Unexpected db uri from repository, %S != %S");
        msg.Format(KFormat, dbUriFromRep, &aExpectedUri);
        LogErrorAndLeave(msg);
        }
    }

//  End of File
