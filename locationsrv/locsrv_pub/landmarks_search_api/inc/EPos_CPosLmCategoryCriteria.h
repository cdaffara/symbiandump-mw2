/*
* Copyright (c) 2005-2006 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  CPosLmCategoryCriteria class
*
*/


#ifndef CPOSLMCATEGORYCRITERIA_H
#define CPOSLMCATEGORYCRITERIA_H

#include <e32base.h>
#include <EPos_Landmarks.h>

#include "EPos_CPosLmSearchCriteria.h"

/**
*  Criterion used for searching for landmarks which belong to a certain
*  category.
*
*  A category has an ID in the database and it has also a unique name. There
*  are also some global categories which are known by all databases. It is
*  possible to specify either a local category in a specific landmark
*  database, a global category or the name of the category. If one is set,
*  the others are reset and not used.
*
*  It is also possible to search for uncategorized landmarks, i.e. landmarks
*  which do not contain any categories. If neither item ID, global ID nor name
*  are set, then @ref CPosLandmarkSearch::StartLandmarkSearchL will search for
*  uncategorized landmarks.
*
*  This criterion is only valid when searching for landmarks, i.e. if it
*  is passed to @p CPosLandmarkSearch::StartCategorySearchL, the function
*  will fail with error code @p KErrArgument.
*
*  @lib eposlmsearchlib.lib
*  @since S60 3.0
*/
class CPosLmCategoryCriteria : public CPosLmSearchCriteria
    {
    public:

        /**
        * Two-phased constructor.
        *
        * @returns A new instance of this class.
        */
        IMPORT_C static CPosLmCategoryCriteria* NewLC();

        /**
        * Destructor.
        */
        virtual ~CPosLmCategoryCriteria();

    public:

        /**
        * Returns the item ID of the category which should be used as
        * landmark search criterion.
        *
        * @return The item ID of the category or @p KPosLmNullItemId
        *   if not set.
        */
        IMPORT_C TPosLmItemId CategoryItemId() const;

        /**
        * Returns the global category which should be used as landmark search
        * criterion.
        *
        * @return The global category or @p KPosLmNullGlobalCategory if not
        *   set.
        */
        IMPORT_C TPosLmGlobalCategory GlobalCategory() const;

        /**
        * Returns the name of the category which should be used as landmark
        * search criterion.
        *
        * @return The category name or empty descriptor if not
        *   set.
        */
        IMPORT_C TPtrC CategoryName() const;

        /**
        * Sets the item ID of the category which should be used as landmark
        * search criterion.
        *
        * Passing @p KPosLmNullItemId means uncategorized landmarks search.
        *
        * @param aItemId The item ID of the category.
        */
        IMPORT_C void SetCategoryItemId( TPosLmItemId aItemId );

        /**
        * Sets the global category which should be used as landmark search
        * criterion.
        *
        * Passing @p KPosLmNullGlobalCategory means uncategorized landmarks
        * search.
        *
        * @param aGlobalCategory The global category.
        */
        IMPORT_C void SetGlobalCategory( TPosLmGlobalCategory aGlobalCategory );

        /**
        * Sets the name of the category which should be used as landmark
        * search criterion.
        *
        * The exact category name must be specified. Wild-card characters are
        * not used as such, they will be considered as ordinary characters.
        * It is possible to use @ref CPosLmCatNameCriteria to
        * list categories which match a category name pattern containing
        * wild-card characters.
        *
        * The matching is case sensitive.
        *
        * An empty descriptor means uncategorized landmarks search.
        *
        * @param aCategoryName The category name.
        *
        * @leave KErrArgument The category name is longer than @p KPosLmMaxCategoryNameLength.
        */
        IMPORT_C void SetCategoryNameL( const TDesC&  aCategoryName );

    private:

        // C++ constructor
        CPosLmCategoryCriteria();

        void ConstructL();

        // Prohibit copy constructor
        CPosLmCategoryCriteria( const CPosLmCategoryCriteria& );
        // Prohibit assigment operator
        CPosLmCategoryCriteria& operator= ( const CPosLmCategoryCriteria& );

    private:

        TPosLmItemId            iItemId;
        TPosLmGlobalCategory    iGlobalCategory;
        HBufC*                  iCategoryName;

    };

#endif      // CPOSLMCATEGORYCRITERIA_H


