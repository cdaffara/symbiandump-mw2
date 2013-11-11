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
#include "FT_CPosTp55.h"
#include "FT_LandmarkConstants.h"
//#include <eposlmintservices.rsg>
#include <centralrepository.h>
#include <UiklafInternalCRKeys.h>
#include <utfw.h>
#include <f32file.h>
#include <coemain.h>
#include <EPos_CPosLandmarkEncoder.h>
#include <EPos_CPosLandmarkParser.h>
#include <barsread.h>
#include <bautils.h>

// CONSTANTS
_LIT(KFile1, "c:\\documents\\Tp55_1.txt");
_LIT(KFile2, "c:\\documents\\Tp55_2.txt");
_LIT(KFile3, "c:\\documents\\Tp55_3.txt");
const TInt KOneK = 1024;
const TInt KNrOfLandmarksToExport = 10;
//_LIT(KPosResourceFile, "\\resource\\eposlmintservices.rsc");
#ifdef __WINS__
_LIT(KXmlInputFile, "z:\\system\\test\\testdata\\TP55-DiskFullDuringExportImport.xml"); 
#else
_LIT(KXmlInputFile, "c:\\system\\test\\testdata\\TP55-DiskFullDuringExportImport.xml"); 
#endif
_LIT(KXmlOutputFile, "c:\\documents\\TP55-ExportedLandmarks.xml"); 
_LIT(KXmlBufferDumpFile, "c:\\documents\\BufferDump.xml");
_LIT(KInternalServicesResFileRom, "z:\\resource\\eposlmintservices.rsc");
_LIT(KInternalServicesResFileBackup, "c:\\resource\\copy_of_eposlmintservices.rsc");
_LIT(KInternalServicesResFileCdrive, "c:\\resource\\eposlmintservices.rsc");
//_LIT(KInternalServicesLargeResFile, "c:\\system\\test\\testdata\\eposlmintservices_453KB.rsc");
//_LIT(KInternalServicesSmallResFile, "c:\\system\\test\\testdata\\eposlmintservices_198KB.rsc");

// Approx 2141 bytes
_LIT8(KNonsense, "nonsensenonsensenonsensenonsensenonsensenonsensenonsensenonsensenonsense\
nonsensenonsensenonsensenonsensenonsensenonsensenonsensenonsense\
nonsensenonsensenonsensenonsensenonsensenonsensenonsensenonsense\
nonsensenonsensenonsensenonsensenonsensenonsensenonsensenonsense\
nonsensenonsensenonsensenonsensenonsensenonsensenonsensenonsense\
nonsensenonsensenonsensenonsensenonsensenonsensenonsensenonsense\
nonsensenonsensenonsensenonsensenonsensenonsensenonsensenonsense\
nonsensenonsensenonsensenonsensenonsensenonsensenonsensenonsense\
nonsensenonsensenonsensenonsensenonsensenonsensenonsensenonsense\
nonsensenonsensenonsensenonsensenonsensenonsensenonsensenonsense\
nonsensenonsensenonsensenonsensenonsensenonsensenonsensenonsense\
nonsensenonsensenonsensenonsensenonsensenonsensenonsensenonsense\
nonsensenonsensenonsensenonsensenonsensenonsensenonsensenonsense\
nonsensenonsensenonsensenonsensenonsensenonsensenonsensenonsense\
nonsensenonsensenonsensenonsensenonsensenonsensenonsensenonsense\
nonsensenonsensenonsensenonsensenonsensenonsensenonsensenonsense\
nonsensenonsensenonsensenonsensenonsensenonsensenonsensenonsense\
nonsensenonsensenonsensenonsensenonsensenonsensenonsensenonsense\
nonsensenonsensenonsensenonsensenonsensenonsensenonsensenonsense\
nonsensenonsensenonsensenonsensenonsensenonsensenonsensenonsense\
nonsensenonsensenonsensenonsensenonsensenonsensenonsensenonsense\
nonsensenonsensenonsensenonsensenonsensenonsensenonsensenonsense\
nonsensenonsensenonsensenonsensenonsensenonsensenonsensenonsense\
nonsensenonsensenonsensenonsensenonsensenonsensenonsensenonsense\
nonsensenonsensenonsensenonsensenonsensenonsensenonsensenonsense\
nonsensenonsensenonsensenonsensenonsensenonsensenonsensenonsense\
nonsensenonsensenonsensenonsensenonsensenonsensenonsensenonsense\
nonsensenonsensenonsensenonsensenonsensenonsensenonsensenonsense\
nonsensenonsensenonsensenonsensenonsensenonsensenonsensenonsense\
nonsensenonsensenonsensenonsensenonsensenonsensenonsensenonsense\
nonsensenonsensenonsensenonsensenonsensenonsensenonsensenonsense\
sensenonsensenonsensenonsensenonsensenonsensenonsense");

_LIT8(K64B, "nonsensenonsensenonsensenonsensenonsensenonsensenonsensenonsense");

// ================= MEMBER FUNCTIONS =======================

// ---------------------------------------------------------
// CPosTp55::GetName
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CPosTp55::GetName(TDes& aName) const
    {
    _LIT(KTestName, "Tp55 - Disk Full Checking during Import/Export");
    aName = KTestName;
    }

// ---------------------------------------------------------
// CPosTp55::CloseTest
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CPosTp55::CloseTest()
    {
    delete iDatabase;
    iDatabase = NULL;

    delete iOperationWrapper;
    delete iLandmarkEncoder;
    delete iLandmarkParser;
    delete iOutputBuffer;
    delete iInputBuffer;

    iFile1.Close();
    iFile2.Close();
    iFile3.Close();
    iFileServer.Delete(KFile1);
    iFileServer.Delete(KFile2);
    iFileServer.Delete(KFile3);

    //TryRestoreResourceFile();

    iXmlOutputFile.Close();
    iFileServer.Delete(KXmlOutputFile);
    iFileServer.Delete(KXmlBufferDumpFile);

    iFileServer.Close();
    }

// ---------------------------------------------------------
// CPosTp55::InitTestL
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CPosTp55::InitTestL()
    {
    // File setup, create two files
    User::LeaveIfError(iFileServer.Connect());

    // Try to restore if previous execution of Tp55 panicked or was killed.
    //TryRestoreResourceFile();

    iOperationWrapper = new (ELeave) CActiveLmOperation(iLog);
    }

// ---------------------------------------------------------
// CPosTp55::StartL
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CPosTp55::StartL()
    {
    // Retreive threshold from centralrepository
    CRepository* repository = CRepository::NewLC(KCRUidUiklaf);
    User::LeaveIfError(repository->Get(KUikOODDiskCriticalThreshold, iCriticalThreshold));
    CleanupStack::PopAndDestroy(repository);

    // Test import/export LMs when critical level is set to MLFW defined value.
    InitiateCriticalLevelL();
    TestCriticalLevelL();

/*
    // Test import/export LMs when critical level is set to a large value.
    iLog->Put(_L("\r\nRedefining critical level to 453 KB and running all tests again...."));
    CopyResourceFileL(KInternalServicesLargeResFile);
    InitiateCriticalLevelL();
    TestCriticalLevelL();

    // Test import/export LMs when critical level is set to a small value.
    iLog->Put(_L("\r\nRedefining critical level to 198 KB and running all tests again...."));
    CopyResourceFileL(KInternalServicesSmallResFile);
    InitiateCriticalLevelL();
    TestCriticalLevelL();
    */
    }

// ---------------------------------------------------------
// CPosTp55::NotifyProgress
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CPosTp55::NotifyProgress(TReal aProgress)
    {
    iProgress = (TInt) (aProgress * 100);
//    iMsg.Format(_L("Progress = %d"), iProgress);
//    iLog->Put(iMsg);
//    LogVolumeInfoL();
    }

// ---------------------------------------------------------
// CPosTp55::TestCriticalLevelL
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CPosTp55::TestCriticalLevelL()
    {
    // Restore disk space
    RecreateAndOpenFilesL();

    // ************ 1. Open database ************
    delete iDatabase;
    iDatabase = NULL;
    iDatabase = UseGeneratedDbFileL();
    if (iDatabase->IsInitializingNeeded())
       {
       ExecuteAndDeleteLD(iDatabase->InitializeL());
       }

    // ************ 2. Fill up disk space ************
    iLog->Put(_L("Consuming disk space..."));
    ConsumeDiskSpaceL(iFile1, iCriticalLevel + 3 * KOneK);
    LogVolumeInfoL();

//// Test importing landmarks
    iLog->Put(_L("Testing Importing..."));
    iLog->Put(_L("-- Testing import below critical level"));
    TBool testBeyondCriticalLevel = ETrue;
    ImportLandmarksTestL(!testBeyondCriticalLevel);
    
    // ************ 11. Repeat steps 3-10 ************
    iLog->Put(_L("\r\n-- Testing import beyond critical level"));
    ExecuteAndDeleteLD(iDatabase->CompactL());
    iLog->Put(_L("Consuming disk space..."));
    User::LeaveIfError(iFile2.Create(iFileServer, KFile2, EFileWrite));
    ConsumeDiskSpaceL(iFile2, iSizeLimit2);
    LogVolumeInfoL();
    ImportLandmarksTestL(testBeyondCriticalLevel);

    // Restore disk space
    RecreateAndOpenFilesL();

    // Use suitable database for exporting LMs
    delete iDatabase;
    iDatabase = NULL;
    iDatabase = UseGeneratedDbFileL();
    if (iDatabase->IsInitializingNeeded())
       {
       ExecuteAndDeleteLD(iDatabase->InitializeL());
       }

    // ************ 12. Fill up disk space ************
    iLog->Put(_L("Consuming disk space..."));
    ConsumeDiskSpaceL(iFile1, iSizeLimit1);
    LogVolumeInfoL();

//// Test importing landmarks
    iLog->Put(_L("Testing Exporting..."));
    ExportLandmarksTestL();
    }

// ---------------------------------------------------------
// CPosTp55::RecreateAndOpenFilesL
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CPosTp55::RecreateAndOpenFilesL()
    {
    iLog->Put(_L("Removing large files..."));

    // Delete files
    iFile1.Close();
    iFileServer.Delete(KFile1);
    iFile2.Close();
    iFileServer.Delete(KFile2);
    iFile3.Close();
    iFileServer.Delete(KFile3);

    // Create and open files
    User::LeaveIfError(iFile1.Create(iFileServer, KFile1, EFileWrite));
    User::LeaveIfError(iFile2.Create(iFileServer, KFile2, EFileWrite));
    User::LeaveIfError(iFile3.Create(iFileServer, KFile3, EFileWrite));
    }

// ---------------------------------------------------------
// CPosTp55::ExportLandmarksTestL
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CPosTp55::ExportLandmarksTestL()
    {
    TInt result(0);
    TBool doAddAllLandmarks = ETrue;
    TBool doExecuteIncrementally = ETrue;

//// Testing Disk full during addition of LMs to encoder
    iLog->Put(_L("\r\n--- Testing Disk full during addition of LMs to encoder"));

    // 13. ************ Export to file using encoder stand-alone ************
    iLog->Put(_L("------ Export to file incrementally using encoder stand-alone"));
    ReInitializeEncoderEtcL();
    iLandmarkEncoder->SetOutputFileL(KXmlOutputFile);
    AddCollectionDataToEncoderL();
    AddLandmarksToEncoderL(doAddAllLandmarks);

    // ************ Export to buffer incrementally using encoder stand-alone ************
//    iLog->Put(_L("------ Export to buffer incrementally using encoder stand-alone"));
// Not tested here since it does not result in KErrDiskFull but in KErrNoMemory. Time-consuming test
/*    ReInitializeEncoderEtcL();
    delete iOutputBuffer;
    iOutputBuffer = NULL;
    iOutputBuffer = iLandmarkEncoder->SetUseOutputBufferL();
    AddCollectionDataToEncoderL();
    while (result == KErrNone)
        {
        TRAP(result, AddLandmarksToEncoderL(!doAddAllLandmarks));
        iMsg.Format(_L("Size of output buffer: %d"), iOutputBuffer->Size());
        iLog->Put(iMsg);
        }
    iMsg.Format(_L("Export to buffer left with error code %d"), result);
    iLog->Put(iMsg);
    */

//// Testing Disk full during ExportLandmarksL()
    iLog->Put(_L("\r\n--- Testing Disk full during ExportLandmarksL()"));

    // Create array of all IDs in database
    RArray<TPosLmItemId> itemIds;
    CleanupClosePushL(itemIds);
    PopulateArrayL(itemIds, doAddAllLandmarks);

    // ************ 14. Export to file incrementally using ExportLandmarksL() ************
    iLog->Put(_L("------ Export to file incrementally using ExportLandmarksL()"));
    ReInitializeEncoderEtcL();
    iLandmarkEncoder->SetOutputFileL(KXmlOutputFile);
    AddCollectionDataToEncoderL();
    ExportWithExportLandmarksMethodL(itemIds, doExecuteIncrementally);

    // ************ 15. Export to file synchronously using ExportLandmarksL() ************
    iLog->Put(_L("------ Export to file synchronously using ExportLandmarksL()"));
    ReInitializeEncoderEtcL();
    iLandmarkEncoder->SetOutputFileL(KXmlOutputFile);
    AddCollectionDataToEncoderL();
    ExportWithExportLandmarksMethodL(itemIds, !doExecuteIncrementally);

    // ************ Export to buffer incrementally using ExportLandmarksL() ************
//    iLog->Put(_L("------ Export to buffer incrementally using ExportLandmarksL()"));
// Not tested here since it does not result in KErrDiskFull but in KErrNoMemory. Time-consuming test

    // ************ Export to buffer synchronously ExportLandmarksL() ************
//    iLog->Put(_L("------ Export to buffer synchronously ExportLandmarksL() ------"));
// Not tested here since it does not result in KErrDiskFull but in KErrNoMemory. Time-consuming test

    CleanupStack::PopAndDestroy(&itemIds);

//// Testing Disk full during finalizing of encoder
    iLog->Put(_L("\r\n--- Testing Disk full during finalizing of encoder"));

    // ************ 16. Export to file using encoder stand-alone and finalizing incrementally ************
    iLog->Put(_L("------ Export to file using encoder stand-alone and finalizing incrementally"));
    ReInitializeEncoderEtcL();
    iLandmarkEncoder->SetOutputFileL(KXmlOutputFile);
    AddCollectionDataToEncoderL();
    AddLandmarksToEncoderL(!doAddAllLandmarks);
    FillFile3L(); // Fill up disk space so much that next call to FinalizeL results in KErrDiskFull.
    FinalizeEncoderL(doExecuteIncrementally);

    // ************ 17. Export to file using encoder stand-alone and finalizing synchronously ************
    iLog->Put(_L("------ Export to file using encoder stand-alone and finalizing synchronously"));
    ReInitializeEncoderEtcL();
    iLandmarkEncoder->SetOutputFileL(KXmlOutputFile);
    AddCollectionDataToEncoderL();
    AddLandmarksToEncoderL(!doAddAllLandmarks);
    FillFile3L(); // Fill up disk space so much that next call to FinalizeL results in KErrDiskFull.
    FinalizeEncoderL(!doExecuteIncrementally);

    // Create array of some IDs in database
    CleanupClosePushL(itemIds);
    PopulateArrayL(itemIds, !doAddAllLandmarks);

    // ************ 18. Export to file using ExportLandmarksL() and finalizing incrementally ************
    iLog->Put(_L("------ Export to file using ExportLandmarksL() and finalizing incrementally"));
    ReInitializeEncoderEtcL();
    iLandmarkEncoder->SetOutputFileL(KXmlOutputFile);
    AddCollectionDataToEncoderL();
    ExportWithExportLandmarksMethodL(itemIds, doExecuteIncrementally);
    FillFile3L(); // Fill up disk space so much that next call to FinalizeL results in KErrDiskFull.
    FinalizeEncoderL(doExecuteIncrementally);

    // ************ 19. Export to file using ExportLandmarksL() and finalizing synchronously ************
    iLog->Put(_L("------ Export to file using ExportLandmarksL() and finalizing synchronously"));
    ReInitializeEncoderEtcL();
    iLandmarkEncoder->SetOutputFileL(KXmlOutputFile);
    AddCollectionDataToEncoderL();
    ExportWithExportLandmarksMethodL(itemIds, !doExecuteIncrementally);
    FillFile3L(); // Fill up disk space so much that next call to FinalizeL results in KErrDiskFull.
    FinalizeEncoderL(!doExecuteIncrementally);

    CleanupStack::PopAndDestroy(&itemIds);

//// Testing when disk is already full
    iLog->Put(_L("\r\n--- Testing when disk is already full"));
    TVolumeInfo volumeInfoBeforeTest, volumeInfoAfterTest;

    // ************ 20. CPosLandmarkEncoder::SetOutPutFileL() ************
    iLog->Put(_L("------ CPosLandmarkEncoder::SetOutPutFileL()"));
    iFile2.Close();
    DeleteFileL(KFile2);
    ReInitializeEncoderEtcL();
    User::LeaveIfError(iFile2.Create(iFileServer, KFile2, EFileWrite));
    ConsumeDiskSpaceL(iFile2, iSizeLimit2);
    User::LeaveIfError(iFileServer.Volume(volumeInfoBeforeTest, EDriveC));
    TRAP(result, iLandmarkEncoder->SetOutputFileL(KXmlOutputFile));
    User::LeaveIfError(iFileServer.Volume(volumeInfoAfterTest, EDriveC));

    // Verify
    _LIT(KSetFile1, "SetOutPutFileL() didn't result in KErrDiskFull but in %d");
    AssertTrueSecL(result == KErrDiskFull, KSetFile1, result);
    iLog->Put(_L("SetOutputFileL() left with KErrDiskFull"));
    _LIT(KSetFile2, "SetOutPutFileL() consumed disk space even if critical level has been passed. %d");
    AssertTrueSecL(volumeInfoBeforeTest.iFree == volumeInfoAfterTest.iFree, KSetFile2);

    // ************ 21. CPosLandmarkEncoder::AddLandmarkL() ************
    iLog->Put(_L("------ CPosLandmarkEncoder::AddLandmarkL()"));
    iFile2.Close();
    DeleteFileL(KFile2);
    ReInitializeEncoderEtcL();
    iLandmarkEncoder->SetOutputFileL(KXmlOutputFile);
    AddCollectionDataToEncoderL();
    CPosLmItemIterator* iterator = iDatabase->LandmarkIteratorL();
    CleanupStack::PushL(iterator);
    CPosLandmark* landmark = iDatabase->ReadLandmarkLC(iterator->NextL());
    User::LeaveIfError(iFile2.Create(iFileServer, KFile2, EFileWrite));
    ConsumeDiskSpaceL(iFile2, iSizeLimit2);
    User::LeaveIfError(iFileServer.Volume(volumeInfoBeforeTest, EDriveC));
    TRAP(result, iLandmarkEncoder->AddLandmarkL(*landmark));
    User::LeaveIfError(iFileServer.Volume(volumeInfoAfterTest, EDriveC));
    CleanupStack::PopAndDestroy(2, iterator);

    // Verify
    _LIT(KAddLmErr1, "Adding LM to encoder didn't result in KErrDiskFull but in %d when beyond critical level");
    AssertTrueSecL(result == KErrDiskFull, KAddLmErr1, result);
    iLog->Put(_L("AddLandmarkL() left with KErrDiskFull"));
    _LIT(KAddLmErr2, "AddLandmarkL() consumed disk space even if critical level has been passed. %d");
    AssertTrueSecL(volumeInfoBeforeTest.iFree == volumeInfoAfterTest.iFree, KAddLmErr2);

    // ************ 22. CPosLandmarkEncoder::FinalizeL() ************
    iLog->Put(_L("------ CPosLandmarkEncoder::FinalizeL()"));
    iFile2.Close();
    DeleteFileL(KFile2);
    ReInitializeEncoderEtcL();
    iLandmarkEncoder->SetOutputFileL(KXmlOutputFile);
    AddCollectionDataToEncoderL();
    AddLandmarksToEncoderL(!doAddAllLandmarks);
    User::LeaveIfError(iFile2.Create(iFileServer, KFile2, EFileWrite));
    ConsumeDiskSpaceL(iFile2, iSizeLimit2);
    delete iOperation;
    iOperation = NULL;
    User::LeaveIfError(iFileServer.Volume(volumeInfoBeforeTest, EDriveC));
    TRAP(result, iOperation = iLandmarkEncoder->FinalizeEncodingL());
    User::LeaveIfError(iFileServer.Volume(volumeInfoAfterTest, EDriveC));

    // Verify
    _LIT(KFinalizeErr1, "FinalizeL() didn't result in KErrDiskFull but in %d");
    AssertTrueSecL(result == KErrDiskFull, KFinalizeErr1, result);
    iLog->Put(_L("FinalizeEncodingL() left with KErrDiskFull"));
    _LIT(KFinalizeErr2, "FinalizeL() consumed disk space even if critical level has been passed. %d");
    AssertTrueSecL(volumeInfoBeforeTest.iFree == volumeInfoAfterTest.iFree, KFinalizeErr2);

    // ************ 23. CPosLandmarkEncoder::SetOutPutFileL() - Bonus test 1 ************
    iLog->Put(_L("------ CPosLandmarkEncoder::SetOutPutFileL() - Bonus test 1"));
    iFile2.Close();
    DeleteFileL(KFile2);
    ReInitializeEncoderEtcL();
    User::LeaveIfError(iFile2.Create(iFileServer, KFile2, EFileWrite));
    ConsumeDiskSpaceL(iFile2, iSizeLimit2);
    TRAP(result, iLandmarkEncoder->SetOutputFileL(KXmlOutputFile));

    // Verify
    AssertTrueSecL(result == KErrDiskFull, KSetFile1, result);
    iLog->Put(_L("SetOutputFileL() left with KErrDiskFull"));

    // Try it again
    TRAP(result, iLandmarkEncoder->SetOutputFileL(KXmlOutputFile));

    // Verify
    _LIT(KSetFile3, "SetOutPutFileL() didn't result in KErrAlreadyExists but in %d");
    AssertTrueSecL(result == KErrAlreadyExists, KSetFile3, result);
    iLog->Put(_L("SetOutputFileL() left with KErrAlreadyExists"));

    // ************ 24. CPosLandmarkEncoder::FinalizeL() - Bonus test 2 ************
    // This test tests if it is possible to finalize when adding LMs to encoder has 
    // been interrupted due to KErrDiskFull.
    iLog->Put(_L("------ CPosLandmarkEncoder::FinalizeL() - Bonus test 2"));
    iFile2.Close();
    DeleteFileL(KFile2);
    ReInitializeEncoderEtcL();
    iLandmarkEncoder->SetOutputFileL(KXmlOutputFile);
    AddCollectionDataToEncoderL();

    // Add landmarks will result in KErrDiskFull
    AddLandmarksToEncoderL(doAddAllLandmarks);

    // Restore some disk space
    iFile1.Close();
    DeleteFileL(KFile1);

    // Try to finalize
    delete iOperation;
    iOperation = NULL;
    TRAP(result, ExecuteAndDeleteLD(iLandmarkEncoder->FinalizeEncodingL()));
    AssertTrueSecL(result == KErrNone, _L("FinalizeL() left with %d"), result);
    iLog->Put(_L("FinalizeL() successful"));

    // ************ CPosLandmarkEncoder::SetOutPutFileL() - Bonus test 3 ************
//  Not tested here since this is a panic test. (Not tested anywhere else)
/*    iLog->Put(_L("------ CPosLandmarkEncoder::SetOutPutFileL() - Bonus test 3"));
    iFile2.Close();
    DeleteFileL(KFile2);
    ReInitializeEncoderEtcL();
    User::LeaveIfError(iFile2.Create(iFileServer, KFile2, EFileWrite));
    ConsumeDiskSpaceL(iFile2, iSizeLimit2);
    TRAP(result, iLandmarkEncoder->SetOutputFileL(KXmlOutputFile));

    // Verify
    AssertTrueSecL(result == KErrDiskFull, KSetFile1, result);
    iLog->Put(_L("SetOutputFileL() left with KErrDiskFull"));

    // Free some disc space and try to finish the export operation. (AddLandmarksL will panic with EPosLmProtocolBreak)
    iFile2.Close();
    DeleteFileL(KFile2);
    AddLandmarksToEncoderL(!doAddAllLandmarks);
    FinalizeEncoderL(doExecuteIncrementally);*/
    }

// ---------------------------------------------------------
// CPosTp55::ImportLandmarksTestL
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CPosTp55::ImportLandmarksTestL(TBool aCriticalLevelPassed)
    {
    delete iInputBuffer;
    iInputBuffer = NULL;
    iInputBuffer = CreateInputBufferL();
    TBool doExecuteIncrementally = ETrue;

    // ************ 3. Import all LMs in XML file incrementally ************
    iLog->Put(_L("---- Import all LMs in XML file incrementally"));
    ImportLandmarksL(KXmlInputFile, NULL, doExecuteIncrementally, aCriticalLevelPassed);

    // ************ 4. Import all LMs in XML file synchronously ************
    iLog->Put(_L("---- Import all LMs in XML file synchronously"));
    ImportLandmarksL(KXmlInputFile, NULL, !doExecuteIncrementally, aCriticalLevelPassed);

    // ************ 5. Import all LMs in XML buffer incrementally ************
    iLog->Put(_L("---- Import all LMs in XML buffer incrementally"));
    ImportLandmarksL(KNullDesC, NULL, doExecuteIncrementally, aCriticalLevelPassed);

    // ************ 6. Import all LMs in XML buffer synchronously ************
    iLog->Put(_L("---- Import all LMs in XML buffer synchronously"));
    ImportLandmarksL(KNullDesC, NULL, !doExecuteIncrementally, aCriticalLevelPassed);

    // Create array of ids to Import (subset of all landmarks in the input file/buffer)
    RArray<TUint> itemIds;
    CleanupClosePushL(itemIds);
    for (TInt i = 10; i < 60; i++)
        {
        User::LeaveIfError(itemIds.Append(i));
        }

    // ************ 7. Import a subset of LMs in XML file incrementally ************
    iLog->Put(_L("---- Import a subset of LMs in XML file incrementally"));
    ImportLandmarksL(KXmlInputFile, &itemIds, doExecuteIncrementally, aCriticalLevelPassed);

    // ************ 8. Import a subset of LMs in XML file synchronously ************
    iLog->Put(_L("---- Import a subset of LMs in XML file synchronously"));
    ImportLandmarksL(KXmlInputFile, &itemIds, !doExecuteIncrementally, aCriticalLevelPassed);

    // ************ 9. Import a subset of LMs in XML buffer incrementally ************
    iLog->Put(_L("---- Import a subset of LMs in XML buffer incrementally"));
    ImportLandmarksL(KNullDesC, &itemIds, doExecuteIncrementally, aCriticalLevelPassed);

    // ************ 10. Import a subset of LMs in XML buffer synchronously ************
    iLog->Put(_L("---- Import a subset of LMs in XML buffer synchronously"));
    ImportLandmarksL(KNullDesC, &itemIds, !doExecuteIncrementally, aCriticalLevelPassed);

    CleanupStack::PopAndDestroy(&itemIds);

    iFile2.Close();
    DeleteFileL(KFile2);
    }

// ---------------------------------------------------------
// CPosTp55::ConsumeDiskSpaceL
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CPosTp55::ConsumeDiskSpaceL(
    RFile& aFile, TInt aSizeLimit)
    {
    TInt err(KErrNone);

    TVolumeInfo volumeInfo;
    err=iFileServer.Volume(volumeInfo, EDriveC);

    // Fill file fast
    HBufC8* bigBuf = (&KNonsense)->AllocLC();
    while (volumeInfo.iFree > aSizeLimit)
        {
        err = aFile.Write(*bigBuf);
        if (err != KErrNone) iLog->Put(_L("Problem writing"));
        err = iFileServer.Volume(volumeInfo, EDriveC);
        if (err != KErrNone) iLog->Put(_L("Problem voluming"));
        }

    CleanupStack::PopAndDestroy(bigBuf);
    }

// ---------------------------------------------------------
// CPosTp55::ImportLandmarksL
//
// (other items were commented in a header).
// ---------------------------------------------------------
//	
void CPosTp55::ImportLandmarksL(
    const TDesC& aInputFile,
    RArray<TUint>* aArrayOfIds,
    TBool aExecuteIncrementally,
    TBool aTestBeyondCriticalLevel)
    {
    // Recreate parser
    delete iLandmarkParser;
    iLandmarkParser = NULL;
    
    // to minimize risk of KErrLocked
    User::After(500000);
    iLog->Put(_L("Compacting"));
    ExecuteAndDeleteLD(iDatabase->CompactL());
    User::After(500000);

    iLandmarkParser = CPosLandmarkParser::NewL(KMimeType);

    iProgress = 0;
    TBool useInputFile = aInputFile != KNullDesC;
    TBool importSubset = aArrayOfIds != NULL;
    TInt result(KErrNone);

    // Prepare and set output for exported LMs
    if (useInputFile)
        {
        iLog->Put(_L("SetInputFileL"));
        iLandmarkParser->SetInputFileL(aInputFile);
        }
    else // use buffer as output
        {
        iLog->Put(_L("SetInputBuffer"));
        iLandmarkParser->SetInputBuffer(*iInputBuffer);
        }

    // Sample the db size
    CPosLandmarkDatabase::TSize dbSizeBeforeImport;
    TInt err = KErrLocked;
    while (err == KErrLocked)
        {
        TRAP(err, dbSizeBeforeImport = iDatabase->SizeL());
        if (err == KErrLocked) 
            {
            iLog->Put(_L("KErrLocked from iDatabase->SizeL()"));
            User::After(100000);
            }
        }

    // Start importing LMs.
    delete iOperation;
    iOperation = NULL;
    if (importSubset)
        {
        iLog->Put(_L("ImportLandmarksL"));
        iOperation = iDatabase->ImportLandmarksL(
            *iLandmarkParser, *aArrayOfIds, CPosLandmarkDatabase::EDefaultOptions);
        }
    else
        {
        iLog->Put(_L("ImportLandmarksL"));
        iOperation = iDatabase->ImportLandmarksL(
            *iLandmarkParser, CPosLandmarkDatabase::EDefaultOptions);
        }

    // Execute importing LMs
    iLog->Put(_L("ExecuteLmOperationL"));
    result = ExecuteLmOperationL(aExecuteIncrementally);

    // Check result
    _LIT(KErrMsg, "ImportLandmarksL() didn't result in KErrDiskFull but in %d");
    AssertTrueSecL(result == KErrDiskFull, KErrMsg, result);

    if (aTestBeyondCriticalLevel)
        {
        // Sample the db size
        TInt err2 = KErrLocked;
        CPosLandmarkDatabase::TSize dbSizeAfterImport;
        while (err2 == KErrLocked)
            {
            TRAP(err2, dbSizeAfterImport = iDatabase->SizeL());
            if (err2 == KErrLocked)
                {
                iLog->Put(_L("KErrLocked from iDatabase->SizeL(2)"));
                User::After(100000);
                }
            }

        TBool dbSizeIsConstant = dbSizeAfterImport.iFileSize == dbSizeBeforeImport.iFileSize;
        _LIT(KErrMsg, "Database size has changed when importing landmarks beyond critical level");
        if (!dbSizeIsConstant)
            {
            LogErrorAndLeave(KErrMsg);
            }
        iLog->Put(_L("Size of database untouched when importing landmarks beyond critical level"));
        }
    else
        {
        if (aExecuteIncrementally)
            {
            _LIT(KFormat, "Got KErrDiskFull after importing %d%% of the landmarks.");
            iMsg.Format(KFormat, iProgress);
            }
        else
            {
            _LIT(KFormat, "Got KErrDiskFull when importing landmarks synchronously");
            iMsg.Copy(KFormat());
            }
        iLog->Put(iMsg);
        }
    }

// ---------------------------------------------------------
// CPosTp55::InitiateCriticalLevelL
// Initiates iCriticalLevel. (value of iCriticalLevel is extracted from an MLFW 
// resource file that can reside on any drive)
// (other items were commented in a header).
// ---------------------------------------------------------
//	
void CPosTp55::InitiateCriticalLevelL()
    { 
    iLog->Put(_L("InitiateCriticalLevelL"));
    /*
    // Find drive that the resource file resides on.
    TFindFile* filefinder = new (ELeave) TFindFile(iFileServer);
    CleanupStack::PushL(filefinder);
    TParse* fileparser = new (ELeave) TParse;
    CleanupStack::PushL(fileparser);
    fileparser->Set(KPosResourceFile, NULL, NULL);
    User::LeaveIfError(
        filefinder->FindByDir(fileparser->NameAndExt(), fileparser->DriveAndPath()));

    // Open resource file
    RResourceFile resFile;
    resFile.OpenL(iFileServer, filefinder->File());
    CleanupClosePushL(resFile);

    // Allocate the critical level as a buffer
    //TInt R_POS_LANDMARK_INTERNAL_CRITICAL_DISK_LEVEL = 250;
    HBufC8* resourceBuffer = resFile.AllocReadLC(iCriticalThreshold);

    // Convert the buffer to an int
    TResourceReader resReader;
    resReader.SetBuffer(resourceBuffer);
    iCriticalLevel = KOneK * resReader.ReadInt16();
    CleanupStack::PopAndDestroy(4, filefinder);
    */
    iCriticalLevel = iCriticalThreshold;
    iSizeLimit1 = iCriticalLevel + 18 * KOneK; // Do not mess with this value!
    iSizeLimit2 = iCriticalLevel - 20 * KOneK;

    iMsg.Format(_L("CriticalLevel = %d, limit1 = %d, limit2 = %d "), iCriticalLevel, iSizeLimit1, iSizeLimit2);
    iLog->Put(iMsg);
    }

// ---------------------------------------------------------
// CPosTp55::LogVolumeInfoL
// Initiates iCriticalLevel. (value of iCriticalLevel is extracted from an MLFW 
// resource file that can reside on any drive)
// (other items were commented in a header).
// ---------------------------------------------------------
//	
void CPosTp55::LogVolumeInfoL()
    {
    _LIT(KUID,"Unique ID: %08x\r\n");
    _LIT(KSize,"Size: %d bytes\r\n");
    _LIT(KFree,"Free space: %d bytes\r\n");

    TVolumeInfo volumeInfo;
    TInt err = iFileServer.Volume(volumeInfo, EDriveC);
    if (err != KErrNone) 
        LogErrorAndLeave(_L("Error from iFileServer"));

    iMsg.Zero();
    iMsg.AppendFormat(KUID, volumeInfo.iUniqueID);
	iMsg.AppendFormat(KSize, volumeInfo.iSize);
	iMsg.AppendFormat(KFree, volumeInfo.iFree);
    iLog->Put(_L("\r\nMemory info:"));
    iLog->Put(iMsg);
    }

// ---------------------------------------------------------
// CPosTp55::DeleteFileL
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CPosTp55::DeleteFileL(const TDesC& aFile)
    {
    TInt err = iFileServer.Delete(aFile);

    if (err != KErrNone && err != KErrNotFound)
        {
        _LIT(KErrText, "DeleteFileL(): The file %S could not be deleted, errorcode %d");
        iMsg.Format(KErrText, &aFile, err);
        LogErrorAndLeave(iMsg, err);
        }
    }

// ---------------------------------------------------------
// CPosTp55::FillFile3L
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CPosTp55::FillFile3L()
    {
    iFile3.Close();
    iFileServer.Delete(KFile3);
    User::LeaveIfError(iFile3.Create(iFileServer, KFile3, EFileWrite));

    TVolumeInfo volumeInfo;
    User::LeaveIfError(iFileServer.Volume(volumeInfo, EDriveC));
    while (volumeInfo.iFree > iCriticalLevel + 512)
        {
        User::LeaveIfError(iFile3.Write(K64B));
        User::LeaveIfError(iFileServer.Volume(volumeInfo, EDriveC));
        }
    }

// ---------------------------------------------------------
// CPosTp55::CreateInputBufferL
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
HBufC8* CPosTp55::CreateInputBufferL()
    {
    RFile inputFile;
    User::LeaveIfError(inputFile.Open(iFileServer, KXmlInputFile, EFileRead));
    CleanupClosePushL(inputFile);

    TInt fileSize;
    User::LeaveIfError(inputFile.Size(fileSize));
    HBufC8* buffer = HBufC8::NewLC(fileSize);

    // Copy file contents to buffer
    TPtr8 writableBuffer = buffer->Des();
    User::LeaveIfError(inputFile.Read(writableBuffer, fileSize));

    CleanupStack::Pop(buffer);
    CleanupStack::PopAndDestroy(&inputFile);
    return buffer;
    }

// ---------------------------------------------------------
// CPosTp55::ReInitializeEncoderEtcL
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CPosTp55::ReInitializeEncoderEtcL()
    {
    delete iLandmarkEncoder;
    iLandmarkEncoder = NULL;
    iLandmarkEncoder = CPosLandmarkEncoder::NewL(KMimeType);

    DeleteFileL(KXmlOutputFile);
    DeleteFileL(KXmlBufferDumpFile);
    iFile3.Close();
    DeleteFileL(KFile3);

    iProgress = 0;
    }

// ---------------------------------------------------------
// CPosTp55::ReInitializeEncoderEtcL
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
TInt CPosTp55::ExecuteLmOperationL(
    TBool aExecuteIncrementally)
    {
    TInt result(0);

    if (aExecuteIncrementally)
        {
        // Execute the finalizing incrementally.
        iOperationWrapper->Start(iOperation, this);
        CActiveScheduler::Start();
        result = iOperationWrapper->iStatus.Int();
        }
    else
        {
        TRAP(result, iOperation->ExecuteL());
        }

    return result;
    }

// ---------------------------------------------------------
// CPosTp55::SetOutputBufferL
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CPosTp55::SetOutputBufferL()
    {
    delete iOutputBuffer;
    iOutputBuffer = NULL;
    iOutputBuffer = iLandmarkEncoder->SetUseOutputBufferL(); 
    }

// ---------------------------------------------------------
// CPosTp55::AddCollectionDataToEncoderL
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CPosTp55::AddCollectionDataToEncoderL()
    {
    // Add long collection name to encoder
    HBufC* collectionName = HBufC::NewLC(2 * KNonsense().Length());
    collectionName->Des().Copy(KNonsense);
    iLandmarkEncoder->AddCollectionDataL(EPosLmCollDataCollectionName, *collectionName);

    // Add long collection desc to encoder
    HBufC* collectionDesc = HBufC::NewLC(3 * collectionName->Length());
    collectionDesc->Des().Append(*collectionName);
    collectionDesc->Des().Append(*collectionName);
    collectionDesc->Des().Append(*collectionName);
    iLandmarkEncoder->AddCollectionDataL(EPosLmCollDataCollectionDescription, *collectionDesc);
    CleanupStack::PopAndDestroy(2, collectionName);
    }

// ---------------------------------------------------------
// CPosTp55::AddLandmarksToEncoderL
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CPosTp55::AddLandmarksToEncoderL(TBool aAddAll)
    {
    // Add landmarks to export to encoder manually.
    TInt result(0);
    CPosLmItemIterator* iterator = iDatabase->LandmarkIteratorL();
    CleanupStack::PushL(iterator);
    TInt maxNrOfLMs = (aAddAll) ? iterator->NumOfItemsL() : KNrOfLandmarksToExport;
    for (TInt i = 0; i < maxNrOfLMs && result != KErrDiskFull; i++)
        {
        CPosLandmark* landmark = iDatabase->ReadLandmarkLC(iterator->NextL());
        TRAP(result, iLandmarkEncoder->AddLandmarkL(*landmark));
        if (result == KErrLocked)
            {
            iLog->Put(_L("KErrLocked when AddLandmarkL in AddLandmarksToEncoderL"));
            User::After(100000);
            }
        CleanupStack::PopAndDestroy(landmark);
        iProgress = (TInt) (100 * i / maxNrOfLMs);
        }
    CleanupStack::PopAndDestroy(iterator);

    if (aAddAll)
        {
        if (result == KErrDiskFull)
            {
            iMsg.Format(_L("Got KErrDiskFull after adding %d%% of the landmarks to the encoder."), iProgress);
            iLog->Put(iMsg);
            }
        else
            {
            _LIT(KAddLmErr, "Didn't get EKrrDiskFull but %d when adding LMs to encoder");
            LogVolumeInfoL();
            LogErrorAndLeave(KAddLmErr);
            }
        }
    else
        {
        _LIT(KAddLmErr, "Didn't get EKrrNone but %d when adding LMs to encoder");
        AssertTrueSecL(result == KErrNone, KAddLmErr, result);
        }
    }

// ---------------------------------------------------------
// CPosTp55::ExportWithExportLandmarksMethodL()
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CPosTp55::ExportWithExportLandmarksMethodL(
    RArray<TPosLmItemId>& aArrayOfIds,
    TBool aExecuteIncrementally)
    {
    delete iOperation;
    iOperation = NULL;
    iOperation = iDatabase->ExportLandmarksL(
        *iLandmarkEncoder, aArrayOfIds, CPosLandmarkDatabase::EDefaultOptions);
    TInt result = ExecuteLmOperationL(aExecuteIncrementally);

    if (aArrayOfIds.Count() == KNrOfLandmarksToExport) // not all of the LMs have been added
        {
        // Check that KErrDiskFull is not returned
        _LIT(KErrMsg, "Expected KErrNone from ExportLandmarksL() but got %d.");
        AssertTrueSecL(result == KErrNone, KErrMsg, result);
        }
    else // We should have gotten KErrDiskFull when adding LMs to encoder.
        {
        _LIT(KErrMsg, "ExportLandmarksL() didn't result in KErrDiskFull but in %d");
        AssertTrueSecL(result == KErrDiskFull, KErrMsg, result);
        if (aExecuteIncrementally)
            {
            iMsg.Format(_L("Got KErrDiskFull after adding %d%% of the landmarks to the encoder."), iProgress);
            }
        else
            {
            iMsg = _L("Got KErrDiskFull when exporting landmarks synchronously");            
            }
        iLog->Put(iMsg);
        }
    }

// ---------------------------------------------------------
// CPosTp55::FinalizeEncoderL()
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CPosTp55::FinalizeEncoderL(
    TBool aExecuteIncrementally)
    {
    delete iOperation;
    iOperation = NULL;
    TRAPD(result, iOperation = iLandmarkEncoder->FinalizeEncodingL());
    if (result == KErrNone)
        {
        result = ExecuteLmOperationL(aExecuteIncrementally);

        // Check that KErrDiskFull is returned
        _LIT(KErrMsg, "FinalizeL() didn't result in KErrDiskFull but in %d");
        AssertTrueSecL(result == KErrDiskFull, KErrMsg, result);
        iLog->Put(_L("Execution of FinalizeEncodingL() left with KErrDiskFull"));
        }
    else
        {
        AssertTrueSecL(result == KErrDiskFull, _L("FinalizeEncodingL() left with %d"), result);
        iLog->Put(_L("FinalizeEncodingL() left with KErrDiskFull"));
        }
    }

// ---------------------------------------------------------
// CPosTp55::FinalizeEncoderL()
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CPosTp55::PopulateArrayL(
    RArray<TPosLmItemId>& aArray,
    TBool aDoAddAllLms)
    {
    CPosLmItemIterator* iterator = iDatabase->LandmarkIteratorL();
    CleanupStack::PushL(iterator);
    TInt nrOfLms = aDoAddAllLms ? iterator->NumOfItemsL() : KNrOfLandmarksToExport;
    for (TInt i = 0; i < nrOfLms; i++)
        {
        CPosLandmark* landmark = iDatabase->ReadLandmarkLC(iterator->NextL());
        User::LeaveIfError(aArray.Append(landmark->LandmarkId()));
        CleanupStack::PopAndDestroy(landmark);
        }
    CleanupStack::PopAndDestroy(iterator);
    }

// ---------------------------------------------------------
// CPosTp55::CopyResourceFileL
// 
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CPosTp55::CopyResourceFileL(const TDesC& aResourceFile)
    {
    // Release all landmark references to make DLLs be unloaded.
    delete iDatabase;
    iDatabase = NULL;
    delete iLandmarkEncoder;
    iLandmarkEncoder = NULL;
    delete iLandmarkParser;
    iLandmarkParser = NULL;
    delete iOperation;
    iOperation = NULL;

    CFileMan* fileMan = CFileMan::NewL(iFileServer);
    CleanupStack::PushL(fileMan);
    
    // Check if landmark is flashed
     if (!BaflUtils::FileExists(iFileServer, KInternalServicesResFileRom))
        {
         iLog->Put(_L("Landmark is NOT flashed, backing up eposlmintservices.rsc by renaming it bofore copying rsc-file."));
        // Landmark FW is not flashed. Rename the file before copying an own defiend file.
        // Since landmark FW is not flashed the file should exist hence leaving if it is not found!
         User::LeaveIfError(fileMan->Rename(KInternalServicesResFileCdrive, KInternalServicesResFileBackup, CFileMan::EOverWrite));
        }
     else
        {
        iLog->Put(_L("Landmark framework is flashed, copying global categories to c:"));
        }

    User::LeaveIfError(fileMan->Copy(aResourceFile, KInternalServicesResFileCdrive, CFileMan::EOverWrite));
 
    CleanupStack::PopAndDestroy(fileMan);
    }

// ---------------------------------------------------------
// CPosTp55::TryRestoreResourceFile
// 
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CPosTp55::TryRestoreResourceFile()
    {
    CFileMan* fileMan = NULL;
    TRAPD(err, fileMan = CFileMan::NewL(iFileServer));
    if (fileMan)
        {
        // Try to restore the backup up rsc-file (only possible when LM FW is not flashed.
        TInt result = fileMan->Copy(KInternalServicesResFileBackup, KInternalServicesResFileCdrive, CFileMan::EOverWrite);
        if (result == KErrNotFound)
            {
            if (BaflUtils::FileExists(iFileServer, KInternalServicesResFileRom))
                {
                // LM FW is flashed. We need to remove the rsc-file on c:
                fileMan->Delete(KInternalServicesResFileCdrive);
                }
            }
        delete fileMan;
        }
    }

// End of file
