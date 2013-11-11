/*
* Copyright (c) 2004 Nokia Corporation and/or its subsidiary(-ies). 
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

#include "FT_CPosTp57.h"
#include <EPos_CPosLandmarkDatabase.h>
#include <EPos_CPosLmTextCriteria.h>
#include <EPos_CPosLmAreaCriteria.h>

// CONSTANTS  

// ================= MEMBER FUNCTIONS =======================

// ---------------------------------------------------------
// CPosTp57::GetName
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CPosTp57::GetName(TDes& aName) const
    {
    _LIT(KTestName, "TP57 - Multiple db search");
    aName = KTestName;
    }

// ---------------------------------------------------------
// CPosTp57::CloseTest
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CPosTp57::CloseTest()
    { 
    iSearchResults.ResetAndDestroy();
    }
    
// ---------------------------------------------------------
// CPosTp57::StartL
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CPosTp57::StartL()
    {
    
    iDatabase = UseGeneratedDbFileL();
    
    CPosLandmark* lm = NULL;
    TRAPD(err, 
        {    
        lm = iDatabase->ReadLandmarkLC(1);
        CleanupStack::PopAndDestroy(lm);
        }
    );
    
/*    if (!err)
        {
        delete lm;
        }
  */      
    
    
    /*
    CreateDatabasesL();
  
    SearchWithTextCriteriaL();
    
    SearchWithAreaCriteriaL();
    */
	}



// ---------------------------------------------------------
// CPosTp57::SearchWithTextCriteria
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CPosTp57::SearchWithTextCriteriaL()
    {
    AppendSearchResultsL();
    
    for(TInt i=0; i<iSearchResults.Count(); i++)
        {   
        CPosLmTextCriteria* textCriteria = CPosLmTextCriteria::NewLC();
        
        textCriteria->SetTextL(iSearchResults[i]->TextCriteria());
        textCriteria->SetAttributesToSearch(iSearchResults[i]->Attributes());
        
        textCriteria->SetPositionFieldsToSearchL(iSearchResults[i]->PositionFields()); 
        
        
        
        }
        
    }

// ---------------------------------------------------------
// CPosTp57::SearchWithAreaCriteriaL
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CPosTp57::SearchWithAreaCriteriaL()
    {
    AppendAreaSearchResultsL();	
    
    
    TBuf<255> info;
    for (TInt i=0; i<iAreaSearchResults.Count(); i++)
        {
        CPosLmAreaCriteria* areaCriteria=NULL;
        TRAPD(err, areaCriteria = CPosTp57::NewLmAreaCriteriaL(iAreaSearchResults[i]->SouthLat(),
                                                               iAreaSearchResults[i]->NorthLat(),
                                                               iAreaSearchResults[i]->WestLong(),
                                                               iAreaSearchResults[i]->EastLong()
                                                               ));
        CleanupStack::PushL(areaCriteria);
    
        if (err != iAreaSearchResults[i]->ErrorCode())
            {
            iLog->Put(_L("Unexpected error code returned"));    
            iLog->Put(_L("Area Search result read from file:"));
            
            iAreaSearchResults[i]->Print(info);
            iLog->Put(info);

            _LIT(KReturned, "NewLmAreaCriteriaL error = %d");
            info.Format(KReturned, err);
            iLog->Put(info);
            User::Leave(KErrGeneral);
            } 
            
        if (!err)
            {
            
            
            
            }     
         }   
               
    }

// ---------------------------------------------------------
// CPosTp57::NewLmAreaCriteriaL
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
CPosLmAreaCriteria* CPosTp57::NewLmAreaCriteriaL(const TReal64 &aSouthLat, 
                                                 const TReal64 &aNorthLat,
                                                 const TReal64 &aWestLong, 
                                                 const TReal64 &aEastLong) 
    {
    CPosLmAreaCriteria* areaCriteria = CPosLmAreaCriteria::NewLC(aSouthLat, 
                                                                aNorthLat, 
                                                                aWestLong, 
                                                                aEastLong
                                                                );
    CleanupStack::Pop();
    
    return areaCriteria;
    }    
// ---------------------------------------------------------
// CPosTp57::CreateDatabasesL
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CPosTp57::CreateDatabasesL()
    {
    /* 
    Very inefficient and time consuming method, maybe better to execute once and store the 
    created databases as testdata
    
    Db name:            Landmarks id: 
    eposlm1.ldb         1-10                
    eposlm2.ldb         11-20
    eposlm3.ldb         21-30
    eposlm4.ldb         31-40
    eposlm5.ldb         41-50    
    eposlm6.ldb         51-60
    eposlm7.ldb         61-70
    eposlm8.ldb         71-80
    eposlm9.ldb         81-90
    eposlm10.ldb        91-100
    eposlm11.ldb        101-105
    */
    
    // Paths will need to be changed to the secure dbms path when security is implemented
    _LIT(KDefaultDbPath, "c:\\system\\data\\eposlm.ldb"); // Contains now no landmarks but the global categories
    _LIT(KDbNamePath, "c:\\system\\data\\eposlm%d.ldb");
    
    const TInt KLastId=105;
    const TInt KNoDatabases=11;
     
    RemoveDefaultDbL();
    
    CPosLandmarkDatabase* lmd = CPosLandmarkDatabase::OpenL();
    CleanupStack::PushL(lmd);

    if (lmd->IsInitializingNeeded())
        {
        ExecuteAndDeleteLD(lmd->InitializeL()); 
        }
    CleanupStack::PopAndDestroy(lmd); 
        
    RFs fs;
    User::LeaveIfError(fs.Connect());
    CleanupClosePushL(fs);
   
    CFileMan* fileMan = CFileMan::NewL(fs);
    CleanupStack::PushL(fileMan);

    // Create 11 databases
    TInt i=0;
    for (i=1; i <= KNoDatabases; i++)
        {
        TBuf<100> db;
        db.Format(KDbNamePath, i);
        
        fs.Delete(db);
        User::LeaveIfError(fileMan->Copy(KDefaultDbPath, db, CFileMan::EOverWrite));    
        }
    CleanupStack::PopAndDestroy(2, &fs);  
     
    iDatabase = UseGeneratedDbFileL();
    
    if (iDatabase->IsInitializingNeeded())
       {
       ExecuteAndDeleteLD(iDatabase->InitializeL());
       }
        
    _LIT(KDbName, "eposlm%d.ldb");   
   
    TInt databaseNumber=1;
    for (i=1; i <= KLastId; i++)
        {
        CPosLandmark* lm = iDatabase->ReadLandmarkLC(i);
        
        TBuf<100> name;
        name.Format(KDbName, databaseNumber);
            
        iLog->Put(name);
        CPosLandmarkDatabase* lmd = CPosLandmarkDatabase::OpenL(name);
        CleanupStack::PushL(lmd);
        
        lmd->AddLandmarkL(*lm);
        CleanupStack::PopAndDestroy(2, lm); 
        
        if (i % 10 == 0)
            {
            ++databaseNumber;
            }       
        }   
         
    // DEBUG 
    for (i=1; i <= KNoDatabases; i++)
        {
        TBuf<100> dbNo;
        dbNo.Format(_L(" -------- Printing Db %d --------------"), i);
        iLog->Put(dbNo);
        
        TBuf<100> db;
        db.Format(KDbName, i);
        iLog->Put(db);
        
        CPosLandmarkDatabase* lmd = CPosLandmarkDatabase::OpenL(db);
        CleanupStack::PushL(lmd);
        
        CPosLmItemIterator* iter = lmd->LandmarkIteratorL();
        CleanupStack::PushL(iter);

        TPosLmItemId id = iter->NextL();
        
        while (id != KPosLmNullItemId)
            {
            CPosLandmark* lm = lmd->ReadLandmarkLC(id);
        
            TBuf<255> info;
            PrintLandmark(*lm, info);
            iLog->Put(info);
            CleanupStack::PopAndDestroy(lm); 
            
            id = iter->NextL();
            }    
       
        CleanupStack::PopAndDestroy(2, lmd);
        }
    }

//  End of File