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
* Description:  CPosLmIdListCriteria class
*
*/


#ifndef CPOSLMIDLISTCRITERIA_H
#define CPOSLMIDLISTCRITERIA_H

#include <e32base.h>
#include <EPos_Landmarks.h>
#include "EPos_CPosLmSearchCriteria.h"

/**
*  Landmark ID list search criterion.
*
*  This criterion is used if the client only wants to search a subset of the
*  landmarks in the database.
*
*  This criterion must be combined with other search criteria using a
*  @ref CPosLmCompositeCriteria. It is of no use on its own. If it is not
*  combined with another criterion,
*  @ref CPosLandmarkSearch::StartLandmarkSearchL will fail with error code
*  @p KErrArgument.
*
*  @p Example: If this criterion is combined with a @ref CPosLmTextCriteria,
*  the search operation searches the landmarks specified in the ID list
*  criterion and returns those which match the given text string.
*
*  Only one ID list criterion is allowed in each composite criterion, otherwise
*  @ref CPosLandmarkSearch::StartLandmarkSearchL will fail with error code
*  @p KErrArgument.
*
*  If the criterion does not contain any landmark IDs,
*  @ref CPosLandmarkSearch::StartLandmarkSearchL will fail with error code
*  @p KErrArgument.
*
*  @lib eposlmsearchlib.lib
*  @since S60 3.0
*/
class CPosLmIdListCriteria : public CPosLmSearchCriteria
    {
    public:

        /**
        * Two-phased constructor.
        *
        * @returns A new instance of this class.
        */
        IMPORT_C static CPosLmIdListCriteria* NewLC();

        /**
        * Destructor
        */
        IMPORT_C virtual ~CPosLmIdListCriteria();

    public:

        /**
        * Sets the IDs of the landmarks which should be included in the search.
        *
        * @param[in] aIdArray The IDs of the landmarks which should be included
        *   in the search.
        */
        IMPORT_C void SetLandmarkIdsL( const RArray<TPosLmItemId>&  aIdArray );

        /**
        * Retrieves the list of IDs of the landmarks which should be included
        * in the search.
        *
        * @param[out] aIdArray On return, contains the IDs of the landmarks which
        *   should be included in the search.
        */
        IMPORT_C void GetLandmarkIdsL( RArray<TPosLmItemId>&  aIdArray ) const;

    private:

        // C++ constructor
        CPosLmIdListCriteria();

        // Prohibit copy constructor
        CPosLmIdListCriteria( const CPosLmIdListCriteria& );
        // Prohibit assigment operator
        CPosLmIdListCriteria& operator= ( const CPosLmIdListCriteria& );

    private:

        RArray<TPosLmItemId> iIdArray;
    };

#endif      // CPOSLMIDLISTCRITERIA_H


