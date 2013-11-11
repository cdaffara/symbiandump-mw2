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
#include <StifParser.h>
#include "FT_CPosTp13.h"
#include <epos_cposlandmarkdatabase.h>
#include <epos_cposlmcategorymanager.h>
#include <epos_cposlmtextcriteria.h>
#include <epos_cposlmcategorycriteria.h>
#include <epos_cposlandmarksearch.h>
#include <bautils.h>
#include "EPos_Landmarks.h"
           
//  CONSTANTS
_LIT(KTP13Db, "eposlm_tp13.ldb");

// ================= MEMBER FUNCTIONS =======================

// ---------------------------------------------------------
// CPosTp13::CloseTest
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CPosTp13::CloseTest()
    {
    iLog->Log(_L("CloseTest"));
    }

// ---------------------------------------------------------
// CPosTp13::StartL
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CPosTp13::StartL(CStifItemParser& /*aItem*/)
    {
    _LIT( KTransportName, "Teleport" );
    _LIT( KPeopleName, "People" );
    _LIT( KLmAName, "A" );
    _LIT( KLmBName, "B" );
    
    const TInt KTransportGlobalId = 45000;
    const TInt KPeopleGlobalId = 27000;
   
#undef TP13_PREP

#ifdef TP13_PREP    
    // Preparation code, should be executed in Finnish (non-English) locale
    // Default database should be removed so that new database is created

    CPosLandmarkDatabase* db = CPosLandmarkDatabase::OpenL();
    CleanupStack::PushL(db);
    
    CPosLmItemIterator* iter = NULL;
    if ( db->IsInitializingNeeded() )
        {
        ExecuteAndDeleteLD( db->InitializeL() );
        }

    CPosLmCategoryManager* catMan = CPosLmCategoryManager::NewL( *db );
    CleanupStack::PushL( catMan );

    // Rename global category
    TPosLmItemId id = catMan->GetGlobalCategoryL( KTransportGlobalId ); 
    CPosLandmarkCategory* cat = catMan->ReadCategoryLC( id );
    cat->SetCategoryNameL( KTransportName );
    catMan->UpdateCategoryL( *cat );
    CleanupStack::PopAndDestroy( cat );

    // Create user category
    cat = CPosLandmarkCategory::NewLC();
    cat->SetCategoryNameL( KPeopleName );
    TPosLmItemId idUserPeople = catMan->AddCategoryL( *cat );
    CleanupStack::PopAndDestroy( cat );
    
    // Find matching global category
    TPosLmItemId idPeople = catMan->GetGlobalCategoryL( KPeopleGlobalId );
    
    // Create landmark for user's People category
    CPosLandmark* lmA = CPosLandmark::NewLC();
    lmA->SetLandmarkNameL( KLmAName );
    lmA->AddCategoryL( idUserPeople );
    db->AddLandmarkL( *lmA );
    CleanupStack::PopAndDestroy( lmA );

    // Create landmark for global People category
    CPosLandmark* lmB = CPosLandmark::NewLC();
    lmA->SetLandmarkNameL( KLmBName );
    lmA->AddCategoryL( idPeople );
    db->AddLandmarkL( *lmB );
    CleanupStack::PopAndDestroy( lmB );

    CleanupStack::PopAndDestroy( catMan );
    CleanupStack::PopAndDestroy( db );
    
#else
    
    // Test code, should be executed in English locale
    
    TInt lang = User::Language();
    iLog->Log(_L("Current language is %d"), lang);

    if ( lang != ELangEnglish )
        { 
        iLog->Log(_L("Wrong language, should be English (1)"));
        User::Leave( KErrGeneral );
        }
    
    CopyTestDbFileL( KTP13Db );
    
    // 1. Open database
    iLog->Log(_L("1. Open database"));

    CPosLandmarkDatabase* db = CPosLandmarkDatabase::OpenL( KTP13Db );
    CleanupStack::PushL(db);
    
    // 2. Try to use it
    iLog->Log(_L("2. Try use it"));

    CPosLmItemIterator* iter = NULL;
    TRAPD( err, iter = db->LandmarkIteratorL() );
    if ( err != KErrPosLmNotInitialized )
        {
        iLog->Log(_L("Unexpected error on uninitialized database"));
        iErrorsFound++;
        }

    // 3. Initialize database
    iLog->Log(_L("3. Init database"));

    if ( db->IsInitializingNeeded() )
        {
        ExecuteAndDeleteLD( db->InitializeL() );
        }
    else
        {
        iLog->Log(_L("Intialization not requried"));
        iErrorsFound++;
        }

    // 4. Verify names of global categories
    iLog->Log(_L("4. Verify globals"));

    CPosLmCategoryManager* catMan = CPosLmCategoryManager::NewL( *db );
    CleanupStack::PushL( catMan );

    iter = catMan->CategoryIteratorL();
    CleanupStack::PushL( iter );

    TPosLmItemId id = iter->NextL();
    while ( id != KPosLmNullItemId )
        {
        CPosLandmarkCategory* cat = catMan->ReadCategoryLC( id );
        TPosLmGlobalCategory globalId = cat->GlobalCategory();
        
        TPtrC name;
        cat->GetCategoryName( name );

        if ( globalId != KTransportGlobalId )
            {
            HBufC* globalName = catMan->GlobalCategoryNameL( globalId );
            if ( name.Compare( *globalName ) != 0 )
                {
                iLog->Log( _L("Global category %d name mismatch: actual '%S', predefined '%S'"),
                    globalId, &name, globalName );
                iErrorsFound++;
                }
            delete globalName;
            }
        else // should retain user-defiend name
            {
            if ( name.Compare( KTransportName() ) != 0 )
                {
                iLog->Log( _L("Global category %d name mismatch: actual '%S', user-defined '%S'"),
                    globalId, &name, &KTransportName() );
                iErrorsFound++;
                }
            }
        
        CleanupStack::PopAndDestroy( cat );
        id = iter->NextL();
        }
    CleanupStack::PopAndDestroy( iter );

    // 5. Verify user-created category People, which now matches global category
    iLog->Log(_L("5. Verify user's"));
    TPosLmItemId idPeople = catMan->GetCategoryL( KPeopleName );
    CPosLandmarkCategory* cat = catMan->ReadCategoryLC( idPeople );
    TPosLmGlobalCategory idPeopleGlobal = cat->GlobalCategory();
    if ( idPeopleGlobal != KPeopleGlobalId )
        {
        iLog->Log( _L("Global category People ID mismatch: actual '%d', predefined '%d'"),
            idPeopleGlobal, KPeopleGlobalId );
        iErrorsFound++;
        }

    RArray<TPosLmItemId> lmIds;
    CleanupClosePushL( lmIds );
    FindLandmarksL( *db, idPeople, lmIds );
    if ( lmIds.Count() != 2 )
        {
        iLog->Log( _L("Global category People landmark list count mismatch: actual '%d', expected '%d'"),
            lmIds.Count(), 2 );
        iErrorsFound++;
        }
    CleanupStack::PopAndDestroy( &lmIds );

    // verify that those landmarks belong only to People
    RArray<TPosLmItemId> catIds;

    CPosLandmark* lmA = FindLandmarkLC( *db, KLmAName );
    lmA->GetCategoriesL( catIds );
    if ( catIds.Count() != 1 )
        {
        iLog->Log( _L("LmA: wrong category list count, actual '%d', expected '%d'"),
            catIds.Count(), 1 );
        iErrorsFound++;
        }
    if ( catIds.Count() && catIds[0] != idPeople )
        {
        iLog->Log( _L("LmB: wrong category in list, actual '%d', expected '%d'"),
            catIds[0], idPeople );
        iErrorsFound++;
        }
    catIds.Reset();
    CleanupStack::PopAndDestroy( lmA );

    CPosLandmark* lmB = FindLandmarkLC( *db, KLmBName );
    lmA->GetCategoriesL( catIds );
    if ( catIds.Count() != 1 )
        {
        iLog->Log( _L("LmB: wrong category list count, actual '%d', expected '%d'"),
            catIds.Count(), 1 );
        iErrorsFound++;
        }
    if ( catIds.Count() && catIds[0] != idPeople )
        {
        iLog->Log( _L("LmB: wrong category in list, actual '%d', expected '%d'"),
            catIds[0], idPeople );
        iErrorsFound++;
        }
    catIds.Reset();
    CleanupStack::PopAndDestroy( lmB );
    CleanupStack::PopAndDestroy( cat );
    
    // 6. Reset global categories and verify again
    iLog->Log(_L("6. Reset and verify globals"));
    ExecuteAndDeleteLD(catMan->ResetGlobalCategoriesL());
    
    iter = catMan->CategoryIteratorL();
    CleanupStack::PushL( iter );

    id = iter->NextL();
    while ( id != KPosLmNullItemId )
        {
        CPosLandmarkCategory* cat = catMan->ReadCategoryLC( id );
        TPosLmGlobalCategory globalId = cat->GlobalCategory();
        
        TPtrC name;
        cat->GetCategoryName( name );

        HBufC* globalName = catMan->GlobalCategoryNameL( globalId );
        if ( name.Compare( *globalName ) != 0 )
            {
            iLog->Log( _L("Global category %d name mismatch: actual '%S', predefined '%S'"),
                globalId, &name, globalName );
            iErrorsFound++;
            }
        delete globalName;
        CleanupStack::PopAndDestroy( cat );
        id = iter->NextL();
        }
    CleanupStack::PopAndDestroy( iter );

    CleanupStack::PopAndDestroy( catMan );
    CleanupStack::PopAndDestroy( db );
    
    if ( iErrorsFound > 0 )
        {
        iLog->Log(_L("Errors found in TP13 = %d"), iErrorsFound );
        User::Leave( KErrGeneral );
        }
#endif
	}

// ---------------------------------------------------------
// ---------------------------------------------------------
//
CPosLandmark* CPosTp13::FindLandmarkLC( CPosLandmarkDatabase& aDb, const TDesC& aName )
    {
    CPosLmTextCriteria* crit = CPosLmTextCriteria::NewLC();
    crit->SetTextL( aName );
    crit->SetAttributesToSearch( CPosLandmark::ELandmarkName );
    
    CPosLandmarkSearch* search = CPosLandmarkSearch::NewL( aDb );
    CleanupStack::PushL( search );
    
    CPosLmOperation* op = search->StartLandmarkSearchL( *crit );
    CleanupStack::PushL( op );
    op->ExecuteL();
    
    CPosLmItemIterator* iter = search->MatchIteratorL();
    CleanupStack::PushL( iter );
    RArray<TPosLmItemId> lmIds;
    iter->GetItemIdsL( lmIds, 0, iter->NumOfItemsL() );
    CleanupClosePushL( lmIds );
    
    if ( lmIds.Count() < 1 )
        {
        iLog->Log(_L("Couldn't find landmarks, which match name '%S'"), &aName );
        User::Leave( KErrNotFound );
        }
    else
        {
        iLog->Log(_L("Found %d landmarks, which match name '%S'"), lmIds.Count(), &aName );
        }
    
    CPosLandmark* lm = aDb.ReadLandmarkLC( lmIds[0] );
    CleanupStack::Pop();
    
    CleanupStack::PopAndDestroy( &lmIds );
    CleanupStack::PopAndDestroy( iter );
    CleanupStack::PopAndDestroy( op );
    CleanupStack::PopAndDestroy( search );
    CleanupStack::PopAndDestroy( crit );
    
    CleanupStack::PushL( lm );
    return lm;
    }

// ---------------------------------------------------------
// ---------------------------------------------------------
//
void CPosTp13::FindLandmarksL( 
    CPosLandmarkDatabase& aDb, 
    TPosLmItemId aCategoryId,  
    RArray<TPosLmItemId>& aLmIds )
    {
    CPosLmCategoryCriteria* crit = CPosLmCategoryCriteria::NewLC();
    crit->SetCategoryItemId( aCategoryId );
    
    CPosLandmarkSearch* search = CPosLandmarkSearch::NewL( aDb );
    CleanupStack::PushL( search );
    
    CPosLmOperation* op = search->StartLandmarkSearchL( *crit );
    CleanupStack::PushL( op );
    op->ExecuteL();
    
    CPosLmItemIterator* iter = search->MatchIteratorL();
    CleanupStack::PushL( iter );

    iter->GetItemIdsL( aLmIds, 0, iter->NumOfItemsL() );

    CleanupStack::PopAndDestroy( iter );
    CleanupStack::PopAndDestroy( op );
    CleanupStack::PopAndDestroy( search );
    CleanupStack::PopAndDestroy( crit );
    }

// ---------------------------------------------------------
// ---------------------------------------------------------
//
void CPosTp13::ListCategoriesL( CPosLmCategoryManager& aCatMan )
    {
    iLog->Log(_L("--- Global Categories ---"));

    CPosLmItemIterator* iter = aCatMan.CategoryIteratorL();
    CleanupStack::PushL(iter);
	TBuf<50> buffe;

    TPosLmItemId idde = iter->NextL();
    while (idde != KPosLmNullItemId)
        {       
        CPosLandmarkCategory* cat = aCatMan.ReadCategoryLC(idde);

        TPtrC name;
        cat->GetCategoryName(name);

        TPosLmGlobalCategory globalId = cat->GlobalCategory();

        TPtrC iconName;
        TInt iconNr;
        TInt iconMaskIndex;
        cat->GetIcon(iconName, iconNr, iconMaskIndex);
        
        iLog->Log(_L("Category %d, globalId %d, name '%S', icon {'%S',%d,%d}"),
            cat->CategoryId(), cat->GlobalCategory(), &name, &iconName, iconNr, iconMaskIndex );

        CleanupStack::PopAndDestroy(cat); 

        idde = iter->NextL();
        }
    CleanupStack::PopAndDestroy(iter);

	iLog->Log(_L("------"));
    }


//  End of File
