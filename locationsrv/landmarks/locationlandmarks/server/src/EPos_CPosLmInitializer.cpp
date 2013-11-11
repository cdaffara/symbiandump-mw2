/*
* Copyright (c) 2002-2007 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description: Provides initializing of landmark database
*
*
*/


#include <d32dbms.h>

#include <epos_poslmcategoryhandler.h>
#include <epos_cposlmglobalcategoryreader.h>
#include <epos_landmarkdatabasestructure.h>
#include <epos_poslmdatabaseutility.h>

#include "epos_lmdebug.h"
#include "EPos_PosLmServerCommon.h"
#include "EPos_CPosLmServer.h"
#include "EPos_CPosLmNameIndex.h"
#include "epos_cposlmindexmanager.h"
#include "epos_poslmserverutility.h"
#include "EPos_CPosLmInitializer.h"

const TInt KEstimatedNoOfGlobalCategories = 15;

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
CPosLmInitializer::CPosLmInitializer( 
    CPosLmServer& aServer,
    MPosLmServerOperationObserver* aObserver)
:   CPosLmServerOperation( aServer, aObserver )
    {
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
void CPosLmInitializer::ConstructL( const TDesC& aUri )
    {
    BaseConstructL(aUri);

    iGlCatReader = CPosLmGlobalCategoryReader::NewL();
    iGlobalCategoryCount = iGlCatReader->Count();
    
    iPhase = EPhaseRecovering;
    CompleteSelf();
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
CPosLmInitializer* CPosLmInitializer::NewL(
    CPosLmServer& aServer,
    MPosLmServerOperationObserver* aObserver,
    const TDesC& aUri )
    {
    CPosLmInitializer* self = new( ELeave ) CPosLmInitializer( aServer, aObserver );
    CleanupStack::PushL( self );
    self->ConstructL( aUri );
    CleanupStack::Pop( self );
    return self;
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
CPosLmInitializer::~CPosLmInitializer()
    {
    delete iGlCatReader;
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
void CPosLmInitializer::RunL()
    {
    if ( iStatusFlag == KPosLmOperationNotComplete )
        {
        NextStepL(); // perform one more step
        }

    if ( iStatusFlag == KPosLmOperationNotComplete )
        {
        CompleteSelf(); // continue executing
        }
    // else - stop executing, operation complete
    
    NotifyClients();
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
TInt CPosLmInitializer::RunError( TInt aError )
    {
    if ( aError == KErrDiskFull &&
         ( iPhase == EPhaseCreatingLmFieldIndex || 
           iPhase == EPhaseCreatingLmCatIndex ) )
        {
        iPhase++;       // ignore these phases
        CompleteSelf(); // and continue
        return KErrNone;
        }
    else
        {
        return CPosLmServerOperation::RunError( aError );
        }
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
void CPosLmInitializer::NextStepL()
    {
    TReal32 phaseProgress = 0.0;
    switch ( iPhase )
        {
        case EPhaseRecovering:
            phaseProgress = RecoveryNextStepL();
            break;
        case EPhaseCreateCatsStart:
        case EPhaseCreatingCats:
            phaseProgress = CreateCategoriesNextStepL();
            break;
        case EPhaseUpdateCatsStart:
        case EPhaseUpdatingCats:
            phaseProgress = UpdateCategoriesNextStepL();
            break;
        case EPhaseCreatingLmFieldIndex:
            phaseProgress = CreateLmFieldIndexNextStepL();
            break;
        case EPhaseCreatingLmCatIndex:
            phaseProgress = CreateLmCatIndexNextStepL();
            break;
        case EPhaseCompacting:
            phaseProgress = CompactNextStepL();
            break;
        case EPhaseBuildingLmNameIndex:
            phaseProgress = BuildLmNameIndexNextStepL();
            break;
        default:
            break;
        }
    
    UpdateTotalProgress( phaseProgress );

    if ( phaseProgress >= KOperationDone )
        {
        LOG3("Initializer: phase %d, progress %5.2f ( db '%S')", 
            iPhase, iProgress, iDbUri );
        iPhase++;
        }
    
    if ( iPhase == EPhaseDone )
        {
        iProgress = KOperationDone;
        iStatusFlag = KErrNone;
        }
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
TReal32 CPosLmInitializer::CreateCategoriesNextStepL()
    {
    TReal32 progress = 0.0;
    

    switch ( iPhase )
        {
        case EPhaseCreateCatsStart:
            {
            TBool initNeeded( EFalse );
            User::LeaveIfError( iDb->IsInitializeNeededL( initNeeded ) );

            if ( initNeeded )
                {
                iCategoryCounter = 0;
                RemoveAllGlobalCategoriesL();
                }
            else
                {
                progress = KOperationDone;
                }

            iPhase = EPhaseCreatingCats;
            break;
            }
        case EPhaseCreatingCats:
            {
            if ( iCategoryCounter < iGlobalCategoryCount )
                {
                // Add global category to database
                CPosLandmarkCategory* category =
                    iGlCatReader->GlobalCategoryLC( iCategoryCounter );

                iDiskUtilities->DiskSpaceBelowCriticalLevelL(
                    iDiskUtilities->EstimatedDiskSizeOfLmOperation(
                    CPosLmDiskUtilities::EAddLmCategoryOp, *category ), iDbDrive );

                PosLmCategoryHandler::AddCategoryL( *iDb, *category );
                CleanupStack::PopAndDestroy( category );
                
                // Increase counter and calculate progress
                ++iCategoryCounter;
                progress = TReal32( iCategoryCounter ) / TReal32( iGlobalCategoryCount );
                }
            break;
            }
        default:
            break;
        }

    // Check whether the operation is done or not
    if ( iCategoryCounter >= iGlobalCategoryCount || iGlobalCategoryCount == 0 )
    {
    	progress = KOperationDone;
    	iDb->SetLanguageL( User::Language() );
    }

	if(progress == KOperationDone)
	{
		iDb->SetInitializeDoneL();
	}
    
    
    return progress;
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
TReal32 CPosLmInitializer::UpdateCategoriesNextStepL()
    {
    TReal32 progress = 0.0;
    switch ( iPhase )
        {
        case EPhaseUpdateCatsStart:
            {
            TBool langChangeNeeded( EFalse );
            iDb->IsLanguageChangeNeededL( langChangeNeeded );

            if ( langChangeNeeded )
                {
                iCategoryCounter = 0;
                if ( iGlobalCategoryCount == 0 )
                    {
                    // There are no global categories defined
                    // so remove them all from database
                    RemoveAllGlobalCategoriesL();
                    progress = KOperationDone;
                    }
                }
            else
                {
                progress = KOperationDone;
                }
            iPhase = EPhaseUpdatingCats;
            break;
            }
        case EPhaseUpdatingCats:
            {
            if ( iCategoryCounter < iGlobalCategoryCount )
                {
                // Update global category in database
                CPosLandmarkCategory* category = iGlCatReader->GlobalCategoryLC( iCategoryCounter );
            
                iDiskUtilities->DiskSpaceBelowCriticalLevelL(
                    iDiskUtilities->EstimatedDiskSizeOfLmOperation(
                    CPosLmDiskUtilities::EUpdateLmCategoryOp, *category ), iDbDrive );
            
                PosLmCategoryHandler::UpdateGlobalCategoryNameL( *iDb, *category );
                CleanupStack::PopAndDestroy( category );
            
                // Increase counter and calculate progress
                ++iCategoryCounter;
                progress = TReal32( iCategoryCounter ) / TReal32( iGlobalCategoryCount );
                }
            break;
            }
        default:    
            break;
        }
    
    // Check whether the operation is done or not
    if ( iCategoryCounter >= iGlobalCategoryCount || iGlobalCategoryCount == 0 )
        {
        progress = KOperationDone;
        }
    
    if ( progress == KOperationDone )
        {
        iDb->SetLanguageL( User::Language() );
        }

    return progress;
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
void CPosLmInitializer::RemoveAllGlobalCategoriesL()
    {
    iDiskUtilities->DiskSpaceBelowCriticalLevelL(
        iDiskUtilities->EstimatedDiskSizeOfLmOperation(
        CPosLmDiskUtilities::ERemoveLmCategoryOp,
        TPosLmItemId()) * KEstimatedNoOfGlobalCategories, iDbDrive);
    
    PosLmCategoryHandler::RemoveAllGlobalCategoriesL( *iDb );
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
TReal32 CPosLmInitializer::CreateLmFieldIndexNextStepL()
    {
    if ( !iIncrementalStarted )
        {
        iDiskUtilities->DiskSpaceBelowCriticalLevelL(
            PosLmServerUtility::EstimatedDiskSizeOfIndex(
                PosLmServerUtility::ECreateFieldsLmIdIndex,
                PosLmServerUtility::GetTableRowCountL( *iDb, KPosLmLandmarkFieldTable ) ), 
            iDbDrive );

        RDbNamedDatabase database;
        iDb->GetDatabase( database );

        if ( !PosLmServerUtility::IndexExistsL( database, KPosLmLandmarkFieldTable, KPosLmFieldsLmIdIndex ) )
            {
            CDbKey* key = CDbKey::NewLC();
            key->AddL( TDbKeyCol( KPosLmLandmarkIdCol ) );
    
            iDbIncremental.CreateIndex( 
                database,
                KPosLmFieldsLmIdIndex,
                KPosLmLandmarkFieldTable,
                *key,
                iIncrementalStep );
            CleanupStack::PopAndDestroy( key );

            iIncrementalStartValue = iIncrementalStep;
            iIncrementalStarted = ETrue;
            iCompactRequired = ETrue;
            }
        else
            {
            return KOperationDone;
            }
        }

    return IncrementalStepL();
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
TReal32 CPosLmInitializer::CreateLmCatIndexNextStepL()
    {
    if ( !iIncrementalStarted )
        {
        iDiskUtilities->DiskSpaceBelowCriticalLevelL(
            PosLmServerUtility::EstimatedDiskSizeOfIndex(
                PosLmServerUtility::ECreateCategoriesLmIdIndex,
                PosLmServerUtility::GetTableRowCountL( *iDb, KPosLmLandmarkCategoryTable ) ), 
            iDbDrive );

        RDbNamedDatabase database;
        iDb->GetDatabase( database );

        if ( !PosLmServerUtility::IndexExistsL( database, KPosLmLandmarkCategoryTable, KPosLmCategoryLmIdIndex ) )
            {
            CDbKey* key = CDbKey::NewLC();
            key->AddL( TDbKeyCol( KPosLmLandmarkIdCol ) );
    
            iDbIncremental.CreateIndex( 
                database,
                KPosLmCategoryLmIdIndex,
                KPosLmLandmarkCategoryTable,
                *key,
                iIncrementalStep );
            CleanupStack::PopAndDestroy( key );

            iIncrementalStartValue = iIncrementalStep;
            iIncrementalStarted = ETrue;
            iCompactRequired = ETrue;
            }
        else
            {
            return KOperationDone;
            }
        }

    return IncrementalStepL();
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
TReal32 CPosLmInitializer::RecoveryNextStepL()
    {
    if ( !iIncrementalStarted )
        {
        RDbNamedDatabase database;
        iDb->GetDatabase( database );

        if ( database.IsDamaged() )
            {
            User::LeaveIfError( iDbIncremental.Recover( database, iIncrementalStep ) );
            iIncrementalStartValue = iIncrementalStep;
            iIncrementalStarted = ETrue;
            }
        else
            {
            return KOperationDone;
            }
        }

    return IncrementalStepL();
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
TReal32 CPosLmInitializer::IncrementalStepL()
    {
    if ( iIncrementalStep > 0 )
        {
        User::LeaveIfError( iDbIncremental.Next( iIncrementalStep ) );
        }
    
    if ( iIncrementalStep == 0 )
        {
        iDbIncremental.Close();
        iIncrementalStarted = EFalse;
        iIncrementalStep = 0;
        return KOperationDone;
        }
    else
        {
        return  TReal32( iIncrementalStartValue - iIncrementalStep ) / 
                TReal32( iIncrementalStartValue );
        }
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
TReal32 CPosLmInitializer::BuildLmNameIndexNextStepL()
    {
    TReal32 progress = 0;
    if ( iServer.IndexManager().Find( *iDbUri ) >= 0 )
        {
        CPosLmNameIndex& index = iServer.IndexManager().Index( *iDbUri );
        TInt status = index.Status(); 
        switch ( status )
            {
            case KErrNotReady:
                progress = index.Evaluate();
                break;
            case KErrNone:
                progress = KOperationDone;
                break;
            default:
                User::Leave( status );
            }
        return progress;
        }
    else
        {
        return KOperationDone;
        }
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
TReal32 CPosLmInitializer::CompactNextStepL()
    {
    if ( !iIncrementalStarted )
        {
        if ( iCompactRequired || iDb->IsCompactNeededL() )
            {
            RDbNamedDatabase database;
            iDb->GetDatabase( database );
    
            User::LeaveIfError( iDbIncremental.Compact( database, iIncrementalStep ) );
            iIncrementalStartValue = iIncrementalStep;
            iIncrementalStarted = ETrue;
            }
        else
            {
            return KOperationDone;
            }
        }

    return IncrementalStepL();
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
void CPosLmInitializer::UpdateTotalProgress( TReal32 aCurrentPhaseProgress )
    {
    iProgress = 
        ( TReal32( iPhase - 1 ) + aCurrentPhaseProgress ) / 
        TReal32 ( EPhaseDone - 1 ); // all phases "-" final phase
    }
