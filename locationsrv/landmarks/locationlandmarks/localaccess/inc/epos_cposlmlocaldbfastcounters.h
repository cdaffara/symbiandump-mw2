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
* Description: Fast counters for local database
*
*
*/


#ifndef EPOS_CPOSLMLOCALDBFASTCOUNTERS_H_
#define EPOS_CPOSLMLOCALDBFASTCOUNTERS_H_

#include <epos_cposlandmarkdatabaseextended.h>
#include <EPos_CPosLandmarkEncoder.h>
#include <EPos_CPosLandmarkParser.h>
#include <epos_cposlmdiskutilities.h>
#include <epos_mposlmdatabasefastcounters.h>

class CPosLmLocalDbAccess;
class CPosLandmark;

/**
* Fast counters for local database
*/

class CPosLmLocalDbFastCounters : public CBase, public MPosLmDatabaseFastCounters
    {

    public: // Constructors and destructor

        static CPosLmLocalDbFastCounters* NewL( CPosLmLocalDbAccess& aDbAccess );

        /**
        * Destructor.
        */
        ~CPosLmLocalDbFastCounters();
        
    protected:

        CPosLmLocalDbFastCounters( CPosLmLocalDbAccess& aDbAccess );
        
    public: // from MPosLmDatabaseFastCounters
    
        /** Returns total amount of landmarks in the database.
         *  @return Number of landmarks in the database or negative error code:
         *      - KErrNotSupported if not supported in current database.
         *      - KErrPosLmNotInitialized if database is not yet initialized. */
        virtual TInt TotalLandmarksCount();
        
        /** Returns total amount of landmark categories in the database.
         *  @return Number of landmark categories in the database or negative error code:
         *      - KErrNotSupported if not supported in current database.
         *      - KErrPosLmNotInitialized if database is not yet initialized. */
        virtual TInt TotalCategoriesCount();
        
        /** Returns amount of categorized landmarks in the database.
         *  @return Number of categorized landmarks in the database or negative error code:
         *      - KErrNotSupported if not supported in current database.
         *      - KErrPosLmNotInitialized if database is not yet initialized. */
        virtual TInt CategorizedLandmarksCount();
    
        /** Returns total amount of referenced landmark categories in the database.
         *  @return Number of referenced landmark categories in the database 
         *          or negative error code:
         *      - KErrNotSupported if not supported in current database.
         *      - KErrPosLmNotInitialized if database is not yet initialized. */
        virtual TInt ReferencedCategoriesCount();
    
        /** Returns amount of uncategorized landmarks in the database.
         *  @return Number of uncategorized landmarks in the database or negative error code:
         *      - KErrNotSupported if not supported in current database.
         *      - KErrPosLmNotInitialized if database is not yet initialized. */
        virtual TInt UncategorizedLandmarksCount();
        
        /** Returns total amount of unreferenced landmark categories in the database.
         *  @return Number of unreferenced landmark categories in the database 
         *          or negative error code:
         *      - KErrNotSupported if not supported in current database.
         *      - KErrPosLmNotInitialized if database is not yet initialized. */
        virtual TInt UnreferencedCategoriesCount();
    
    private:

        CPosLmLocalDbAccess& iDbAccess;

    };

#endif /*EPOS_CPOSLMLOCALDBFASTCOUNTERS_H_*/
