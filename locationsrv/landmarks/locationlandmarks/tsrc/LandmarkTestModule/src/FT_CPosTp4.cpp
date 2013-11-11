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

#include "FT_CPosTp4.h"
#include <EPos_CPosLandmarkDatabase.h> 
#include <EPos_CPosLMItemIterator.h> 
#include <EPos_CPosLmDatabaseManager.h>
#include "FT_LandmarkConstants.h" 
          
// ================= MEMBER FUNCTIONS =======================


// ---------------------------------------------------------
// CPosTp4::StartL
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CPosTp4::StartL()
    {
    TestCompactL(ESynchronous);

    TestCompactL(EAsynchronous);

    TestCompactL(EWaitForRequest);
	}

// ---------------------------------------------------------
// CPosTp4::TestCompactL
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CPosTp4::TestCompactL(TExecutionMode aExecutionMode)
    {
    _LIT(KFileSizeErr, "Returned file size is incorrect");
    _LIT(KUsageErr, "Returned usage is incorrect");
    _LIT(KFileSizeAfterCompactErr, "The file size after compact is not less than the file size before compact");
    _LIT(KUsageAfterCompactErr, "The usage is equal or less than before compact");
        
    RemoveDefaultDbL();
    RemoveAllLmDatabasesL();
    
	CPosLmDatabaseManager* manager = CPosLmDatabaseManager::NewL();
    CleanupStack::PushL(manager);

	// Create new test db
	HPosLmDatabaseInfo* dbInfoDefault = HPosLmDatabaseInfo::NewLC(KTp4TestDb);
    manager->CreateDatabaseL(*dbInfoDefault);
    CleanupStack::PopAndDestroy(dbInfoDefault);
    // This db must exist
    manager->SetDefaultDatabaseUriL(KTp4TestDb);
    CleanupStack::PopAndDestroy(manager);
       
    CPosLandmarkDatabase* tmp = CPosLandmarkDatabase::OpenL(); // Make sure default database is created
    CleanupStack::PushL(tmp);
    if (tmp->IsInitializingNeeded())
       {
       ExecuteAndDeleteLD(tmp->InitializeL()); 
       }
    CleanupStack::PopAndDestroy(tmp);

    RFile file;
	User::LeaveIfError(file.Open(iFileSession, KLmTp4DefaultDbPath, EFileRead));
    TInt fileSize=0;
    file.Size(fileSize);
    file.Close();

    CPosLandmarkDatabase* lmd = CPosLandmarkDatabase::OpenL();
    CleanupStack::PushL(lmd);

    CPosLandmarkDatabase::TSize size = lmd->SizeL();

    AssertTrueSecL(size.iFileSize == fileSize, KFileSizeErr);    
    AssertTrueSecL(size.iUsage >= 0.0 && size.iUsage <= 1.0, KUsageErr); // For now, when compact is supported should usage be 100%

    CPosLmOperation* op = lmd->CompactL();
    switch (aExecutionMode)
        {
        case ESynchronous:
            ExecuteAndDeleteLD(op);
            break;
        case EAsynchronous:
            RunAsyncOperationLD(op); //*** async
            break;
        case EWaitForRequest:
            CleanupStack::PushL(op);
            RunAsyncOperationByWaitForReqL(op);
            CleanupStack::PopAndDestroy(op);
            break;
        default:
        
            break;
        }
    
    CPosLandmarkDatabase::TSize sizeAfterCompact = lmd->SizeL();
    
    AssertTrueSecL(sizeAfterCompact.iFileSize < size.iFileSize, KFileSizeAfterCompactErr);    
    
    AssertTrueSecL(sizeAfterCompact.iUsage >= size.iUsage, KUsageAfterCompactErr); 

    CleanupStack::PopAndDestroy(lmd);
    }

//  End of File
