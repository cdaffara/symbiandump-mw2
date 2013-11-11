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
#include "FT_CPosTp40.h"
#include <EPos_CPosLandmarkDatabase.h>
#include <EPos_CPosLmTextCriteria.h>
           
// ================= MEMBER FUNCTIONS =======================

// ---------------------------------------------------------
// CPosTp40::GetName
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CPosTp40::GetName(TDes& aName) const
    {
    _LIT(KTestName, "Tp40 - Verify KErrLocked not returned, etc");
    aName = KTestName;
    }

// ---------------------------------------------------------
// CPosTp40::CloseTest
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CPosTp40::CloseTest()
    {
    delete iDatabase;
    iDatabase = NULL;
    }
// ---------------------------------------------------------
// CPosTp40::StartL
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CPosTp40::StartL()
    {
    RemoveDefaultDbL();

    iDatabase = CPosLandmarkDatabase::OpenL();
    if (iDatabase->IsInitializingNeeded())
       {
       ExecuteAndDeleteLD(iDatabase->InitializeL()); // Synchronous since no argument
       }
       
    ExecuteAndDeleteLD(iDatabase->CompactL());

    // Test error report ESLI-5ZYMPM
    TestPart1L();
    ExecuteAndDeleteLD(iDatabase->CompactL());
    // Test error report ESLI-62FGEV, (debug panic, only raised on WINS (not on target))
    TestPart2L();

    delete iDatabase;
    iDatabase = NULL;
    }
// ---------------------------------------------------------
// CPosTp40::TestPart1L
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CPosTp40::TestPart1L()
    {
    //
    // Testing error report ESLI-5ZYMPM
    // Landmarks - KErrLocked returned on write operations even if no operations are outstanding.
    //
    iLog->Put(_L("TestPart1L"));
    
    CPosLandmarkSearch* landmarkSearch = CPosLandmarkSearch::NewL(*iDatabase);
    CleanupStack::PushL(landmarkSearch);

    CPosLmTextCriteria* textCriteria = CPosLmTextCriteria::NewLC();   
    textCriteria->SetTextL(_L("Library*"));

    CPosLmOperation* operation = landmarkSearch->StartLandmarkSearchL(*textCriteria);
    RArray<TPosLmItemId> arr1;
    CleanupClosePushL(arr1);
    TPosLmItemId id1(1);
    TPosLmItemId id2(2);
    arr1.Append(id1);
    arr1.Append(id2);
    operation -> ExecuteL();

    CPosLmOperation* operationPartial = iDatabase -> PreparePartialLandmarksL(arr1);
    CleanupStack::PushL(operationPartial);
    delete operation; //if this is moved to before PreparePartial... everything works
    operation = NULL;
    operationPartial->ExecuteL();
    
    iDatabase -> RemoveLandmarkL(1); //Before leaved with -22 here -> ERROR
    CleanupStack::PopAndDestroy(operationPartial);
    CleanupStack::PopAndDestroy(&arr1);
    CleanupStack::PopAndDestroy(textCriteria);
    CleanupStack::PopAndDestroy(landmarkSearch);
    iLog->Put(_L("TestPart1L completed successfully"));
	}

// ---------------------------------------------------------
// CPosTp40::TestPart2L
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CPosTp40::TestPart2L()
    {
    //
    // Testing error report ESLI-62FGEV
    // Landmarks - TPosLmItemId array with 0 value and PreparePartialLandmarksL
    //
    iLog->Put(_L("TestPart2L"));

    CPosLandmarkSearch* landmarkSearch = CPosLandmarkSearch::NewL(*iDatabase);
    CleanupStack::PushL(landmarkSearch);
    CPosLmTextCriteria* textCriteria = CPosLmTextCriteria::NewLC();   
    textCriteria->SetTextL(_L("Library*"));

    CPosLmOperation* operation = landmarkSearch->StartLandmarkSearchL(*textCriteria);
    RArray<TPosLmItemId> arr1;
    CleanupClosePushL(arr1);
    TPosLmItemId id1(0); // Is actually not allowed to have a 0 value
    TPosLmItemId id2(1);
    arr1.Append(id1);
    arr1.Append(id2);
    operation -> ExecuteL();

    // When passing an array with 0 value method should leave with error,
    // instead the operationPartial->ExecuteL() method panics with -2
    CPosLmOperation* operationPartial = iDatabase -> PreparePartialLandmarksL(arr1);
    CleanupStack::PushL(operationPartial);
    delete operation;
    operation = NULL;
    // This method Panics (debug panic only on WINS) with -2
    operationPartial->ExecuteL();
    
    CleanupStack::PopAndDestroy(operationPartial);
    CleanupStack::PopAndDestroy(&arr1);
    CleanupStack::PopAndDestroy(textCriteria);
    CleanupStack::PopAndDestroy(landmarkSearch);
    iLog->Put(_L("TestPart2L completed successfully"));
	}

//  End of File
