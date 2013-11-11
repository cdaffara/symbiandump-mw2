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
* Description:  CPosLmTextCriteria class
*
*/


#ifndef CPOSLMTEXTCRITERIA_H
#define CPOSLMTEXTCRITERIA_H

#include <e32base.h>
#include <lbsfields.h>
#include <EPos_CPosLandmark.h>
#include "EPos_CPosLmSearchCriteria.h"

const TInt KPosLmMaxSearchStringLength = 255;

/**
*  Criterion for searching for landmarks which contain a certain text.
*
*  The search is defined by providing a text to search for and the position
*  fields and text attributes to search.
*
*  If no attributes are specified for the search, all text attributes in the
*  landmarks are searched. If no position fields are specified, then all
*  fields are searched.
*
*  If this criterion is passed to
*  @ref CPosLandmarkSearch::StartLandmarkSearchL, only landmarks which contain
*  the specified text are returned.
*
*  Wild-card characters are supported.
*
*  This criterion is only valid when searching for landmarks, i.e. if it
*  is passed to @p CPosLandmarkSearch::StartCategorySearchL, the function
*  will fail with error code @p KErrArgument.
*
*  @lib eposlmsearchlib.lib
*  @since S60 3.0
*/
class CPosLmTextCriteria : public CPosLmSearchCriteria
    {
    public:

        /**
        * Two-phased constructor.
        * @returns A new instance of this class.
        */
        IMPORT_C static CPosLmTextCriteria* NewLC();

        /**
        * Destructor.
        */
        virtual ~CPosLmTextCriteria();

    public:

        /**
        * Retrieves the text to search for.
        *
        * @return The text to search for.
        */
        IMPORT_C TPtrC Text() const;

        /**
        * Sets the search string.
        *
        * A non-empty text string must be set, otherwise
        * @ref CPosLandmarkSearch::StartLandmarkSearchL will leave with error
        * code @p KErrArgument.
        *
        * The search is case insensitive.
        *
        * Wild-card characters "?" and "*" are supported in the search string.
        * "?" matches a single occurrence of any character and "*" matches
        * zero or more consecutive occurrences of any characters.
        *
        * A landmark matches the criterion if specified text is found anywhere
        * in selected attributes or position fields.
        *
        * @param aText The text to search for.
        *
        * @leave KErrArgument The search string is longer than @p KPosLmMaxSearchStringLength.
        */
        IMPORT_C void SetTextL( const TDesC& aText );

        /**
        * Retrieves the landmark attributes which should be searched.
        *
        * This function returns a bitmap of landmark attributes. The landmark
        * attributes are defined by @p CPosLandmark::_TAttributes. It is only
        * possible to search the @p ELandmarkName and @p EDescription
        * attributes.
        *
        * @return A bitmap indicating which landmark attributes should be
        *   searched.
        */
        IMPORT_C CPosLandmark::TAttributes AttributesToSearch() const;

        /**
        * Sets which landmark attributes to search.
        *
        * The client passes a bitmap of the landmark attributes to search. It
        * is only possible to search the @p ELandmarkName and @p EDescription
        * attributes.
        *
        * @param aAttributes A bitmap indicating which landmark attributes
        *   should be searched.
        *
        * @panic "Landmarks Client"-EPosInvalidLandmarkAttribute
        *   Unsupported attribute is passed.
        */
        IMPORT_C void SetAttributesToSearch(
            CPosLandmark::TAttributes aAttributes
        );

        /**
        * Retrieves a list of the position fields to search.
        *
        * @param[out] aFieldArray On return, contains a list of the position fields
        *   which are used in a search. If no position fields have been set,
        *   the array is empty. Position fields are identified by values defined
        *   in @p TPositionFieldId.
        */
        IMPORT_C void GetPositionFieldsToSearchL(
            RArray<TUint>& aFieldArray
        ) const;

        /**
        * Sets the position fields to search.
        *
        * Any previously set position fields are cleared by this call.
        *
        * @param[in] aFieldArray A list of the position fields to search.
        *   Position fields are identified by values defined in
        *   @p TPositionFieldId.
        */
        IMPORT_C void SetPositionFieldsToSearchL(
            const RArray<TUint>&  aFieldArray
        );

        /**
        * Clears the position fields list used in a search.
        */
        IMPORT_C void ClearPositionFieldsToSearch();

    private:

        CPosLmTextCriteria();

        void ConstructL();

        // Prohibit copy constructor
        CPosLmTextCriteria( const CPosLmTextCriteria& );
        // Prohibit assigment operator
        CPosLmTextCriteria& operator= ( const CPosLmTextCriteria& );

    private:

        HBufC*                      iText;
        RArray<TUint>               iPositionFieldArray;
        CPosLandmark::TAttributes   iAttributes;

    };

#endif      // CPOSLMTEXTCRITERIA_H


