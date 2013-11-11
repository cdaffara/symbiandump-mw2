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

#include "FT_CPosTp80.h"
#include "FT_LandmarkConstants.h"
#include <EPos_CPosLandmarkDatabase.h>
#include <EPos_CPosLmDatabaseManager.h>

// ================= CONSTANTS=======================
_LIT(KLongFileName, "01234567890123456789012345678901234567890123456789012345678901234567890123456789_COMPOSITEEPOSLM.LDB");
           
// ================= MEMBER FUNCTIONS =======================

// ---------------------------------------------------------
// CPosTp80::GetName
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CPosTp80::GetName(TDes& aName) const
    {
    _LIT(KTestName, "Tp80 - Landmarks DB Long File Name");
    aName = KTestName;
    }
    
// ---------------------------------------------------------
// CPosTp80::CloseTest
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CPosTp80::CloseTest()
    {
    iLog->Put(_L("CloseTest"));
    
    TRAPD(err, DeleteTestDbL());
    if (err != KErrNone) iLog->Put(_L("Error when DeleteTestDbL"));
    
    TRAP(err, RemoveAllLmDatabasesL());
    if (err != KErrNone) iLog->Put(_L("Error when RemoveAllLmDatabasesL"));
    }

// ---------------------------------------------------------
// CPosTp80::StartL
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CPosTp80::StartL()
    {
    // Init test part
    iLog->Put(_L("DeleteTestDbL"));
    DeleteTestDbL();
    iLog->Put(_L("RemoveAllLmDatabasesL"));
    RemoveAllLmDatabasesL();
    
    iLog->Put(_L("CopyTestDbFileL(KLongFileName)"));
    CopyTestDbFileL(KLongFileName);
    iLog->Put(_L("After CopyTestDbFileL(KLongFileName)"));
    
    CPosLmDatabaseManager* manager = CPosLmDatabaseManager::NewL();
    CleanupStack::PushL(manager);
    // Hangs here
    iLog->Put(_L("Before DatabaseExistsL"));
    TBool exist = manager->DatabaseExistsL(KLongFileName);
    iLog->Put(_L("After DatabaseExistsL"));
    
    // if it exists, try to open it
    if (exist)
        {
        CPosLandmarkDatabase* lmd = CPosLandmarkDatabase::OpenL(KLongFileName);
        CleanupStack::PushL(lmd);
        CleanupStack::PopAndDestroy(lmd);       
        }
    else iLog->Put(_L("DatabaseExistsL correctly returns EFalse"));
    
    CleanupStack::PopAndDestroy(manager);   
	}

// ---------------------------------------------------------
// CPosTp80::DeleteTestDbL
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CPosTp80::DeleteTestDbL()
    {
    iLog->Put(_L("DeleteTestDbL"));
    RFs fs;
    User::LeaveIfError(fs.Connect());
    CleanupClosePushL(fs);
    
    CFileMan* fileMan = CFileMan::NewL(fs);
    CleanupStack::PushL(fileMan);
    
    TBuf<150> path;
        
    path.Append(KLmDbPath);
    path.Append(KLongFileName);
       
    TInt err = fileMan->Delete(path);
    if (err != KErrNone) 
        {
        TBuf<100> buf;
        buf.Format(_L("Error %d when deleting file"), err);
        iLog->Put(buf);
        }
     
    CleanupStack::PopAndDestroy(2, &fs);
    }

//  End of File
