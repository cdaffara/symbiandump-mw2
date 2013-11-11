/*
* Copyright (c) 2007 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  SDK/S60, Landmarks API
*
*/


#ifndef EPOS_CPOSLMDATABASEFASTCOUNTERS_H_
#define EPOS_CPOSLMDATABASEFASTCOUNTERS_H_

/**
 *  Landmark database extension interface to access fast counters.
 *
 *  This class defines methods, which are able to return following data fast:
 *   - total amount of landmarks, @ref LandmarksCount()
 *   - total amount of landmark categories, @ref CategoriesCount()
 *   - amount of uncategorized landmarks, @ref UncategorizedLandmarksCount()
 *   - amount of unreferenced landmark categories, @ref UnrferencedCategoriesCount()
 *
 *  @see CPosLandmarkDatabaseExtended
 *  @see CPosLandmarkDatabaseExtended::GetExtensionL
 *  @since S60 3.2
 *  @lib eposlandmarks.lib.
 */
class MPosLmDatabaseFastCounters
    {
    public:

        /** Returns total amount of landmarks in the database.
         *  @return Number of landmarks in the database or negative error code:
         *      - KErrNotSupported if not supported in current database.
         *      - KErrPosLmNotInitialized if database is not yet initialized. */
        virtual TInt TotalLandmarksCount() = 0;

        /** Returns total amount of landmark categories in the database.
         *  @return Number of landmark categories in the database or negative error code:
         *      - KErrNotSupported if not supported in current database.
         *      - KErrPosLmNotInitialized if database is not yet initialized. */
        virtual TInt TotalCategoriesCount() = 0;
        
        /** Returns amount of categorized landmarks in the database.
         *  @return Number of categorized landmarks in the database or negative error code:
         *      - KErrNotSupported if not supported in current database.
         *      - KErrPosLmNotInitialized if database is not yet initialized. */
        virtual TInt CategorizedLandmarksCount() = 0;

        /** Returns total amount of referenced landmark categories in the database.
         *  @return Number of referenced landmark categories in the database 
         *          or negative error code:
         *      - KErrNotSupported if not supported in current database.
         *      - KErrPosLmNotInitialized if database is not yet initialized. */
        virtual TInt ReferencedCategoriesCount() = 0;

        /** Returns amount of uncategorized (not assigned to any category) 
         *  landmarks in the database.
         *  @return Number of uncategorized landmarks in the database or negative error code:
         *      - KErrNotSupported if not supported in current database.
         *      - KErrPosLmNotInitialized if database is not yet initialized. */
        virtual TInt UncategorizedLandmarksCount() = 0;

        /** Returns total amount of unreferenced (not used by any landmark) 
         *  landmark categories in the database.
         *  @return Number of unreferenced landmark categories in the database 
         *          or negative error code:
         *      - KErrNotSupported if not supported in current database.
         *      - KErrPosLmNotInitialized if database is not yet initialized. */
        virtual TInt UnreferencedCategoriesCount() = 0;
    };


#endif /*EPOS_CPOSLMDATABASEFASTCOUNTERS_H_*/
