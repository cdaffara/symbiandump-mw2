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


#ifndef CPOSLMINITIALIZER_H
#define CPOSLMINITIALIZER_H

#include "EPos_CPosLmServerOperation.h"

class CPosLmGlobalCategoryReader;
class RDbNamedDatabase;
class RDbIncremental;

/**
* Server operation that initializes a landmark database. Global categories are
* copied from a resource file into the database during the initialization.
*/
class CPosLmInitializer : public CPosLmServerOperation
    {
    public: 

        /**
        * Two-phased constructor.
        *
        * @param[in] aObserver Server operation observer.
        * @param[in] aUri Landmark database URI.
        */
        static CPosLmInitializer* NewL(
            CPosLmServer& aServer,
            MPosLmServerOperationObserver* aObserver,
            const TDesC& aUri );

        virtual ~CPosLmInitializer();

    public: // From CPosLmServerOperation.

        void RunL();
        TInt RunError( TInt aError );
        void NextStepL();

    private:

        enum TPhase
            {
            EPhaseUnknown,
            EPhaseRecovering,
            EPhaseCreateCatsStart,
            EPhaseCreatingCats,
            EPhaseUpdateCatsStart,
            EPhaseUpdatingCats,
            EPhaseCreatingLmFieldIndex,
            EPhaseCreatingLmCatIndex,
            EPhaseCompacting,
            EPhaseBuildingLmNameIndex,
            EPhaseDone
            };

        /**
        * @param[in,out] aObserver Server operation observer.
        */
        CPosLmInitializer( 
            CPosLmServer& aServer,
            MPosLmServerOperationObserver* aObserver );

        /**
        * @param[in] aUri Landmark database URI.
        */
        void ConstructL( const TDesC& aUri );

        /**
        * Removes all global categories. Leaves with @p KErrDiskFull
        * if not enough disk space is available to do this operation.
        */
        void RemoveAllGlobalCategoriesL();

        /** Performs single step of global categories creating phase 
         * @return progress of the phase, from 0.0 to 1.0 */
        TReal32 CreateCategoriesNextStepL();

        /** Performs single step of global categories updating phase 
         * @return progress of the phase, from 0.0 to 1.0 */
        TReal32 UpdateCategoriesNextStepL();

        /** Performs single step of index creation phase.
         * Creates index in LmID in Landmark-Field table
         * @return progress of the phase, from 0.0 to 1.0 */
        TReal32 CreateLmFieldIndexNextStepL();

        /** Performs single step of index creation phase 
         * Creates index in LmID in Landmark-Category table
         * @return progress of the phase, from 0.0 to 1.0 */
        TReal32 CreateLmCatIndexNextStepL();

        /** Performs single step of database recovery phase 
         * @return progress of the phase, from 0.0 to 1.0 */
        TReal32 RecoveryNextStepL();

        /** Performs single step of DBMS incremental operation. 
         * @return progress of the phase, from 0.0 to 1.0 */
        TReal32 IncrementalStepL();
        
        /** Performs single step of Lm name index generating. 
         * @return progress of the phase, from 0.0 to 1.0 */
        TReal32 BuildLmNameIndexNextStepL();
        
        /** Performs single step of compacting. 
         * @return progress of the phase, from 0.0 to 1.0 */
        TReal32 CompactNextStepL();

        /** Calculates total progress of the operation */ 
        void UpdateTotalProgress( TReal32 aCurrentPhaseProgress );

    private:

        TInt iPhase; // TPhase
    
        CPosLmGlobalCategoryReader* iGlCatReader;
        TInt iGlobalCategoryCount;
        TInt iCategoryCounter;

        // Used for incremental operation
        RDbIncremental iDbIncremental;
        TBool iIncrementalStarted;

        // The current step of the incremental operation
        TInt iIncrementalStep;

        // The start value of the incremental operation. Used for
        // calculating the progress.
        TInt iIncrementalStartValue;
        
        // This flag is used to force compaction during init
        // if new indexes were created
        TBool iCompactRequired;
    };

#endif      // CPOSLMINITIALIZER_H

// End of File

