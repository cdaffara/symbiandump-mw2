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
* Description:  CPosLmCompositeCriteria class
*
*/


#ifndef CPOSLMCOMPOSITECRITERIA_H
#define CPOSLMCOMPOSITECRITERIA_H

#include <e32base.h>
#include "EPos_CPosLmSearchCriteria.h"

/**
*  Class used to combine multiple search criterion.
*
*  For instance, to search for all restaurants in the area, this class can be
*  used to combine a @ref CPosLmAreaCriteria and a @ref CPosLmCategoryCriteria.
*
*  If a @ref CPosLmNearestCriteria is used and no sort preference is specified,
*  the result will be sorted by distance. If more than one
*  @ref CPosLmNearestCriteria are combined using a @ref CPosLmCompositeCriteria,
*  the sort order will be undefined unless a sort preference is specified.
*
*  Nested @ref CPosLmCompositeCriteria are not allowed.
*
*  This criterion only supports searching for landmarks, e.g. if it is passed
*  to @p CPosLandmarkSearch::StartCategorySearchL, the function will fail with
*  error code @p KErrNotSupported.
*
*  @lib eposlmsearchlib.lib
*  @since S60 3.0
*/
class CPosLmCompositeCriteria : public CPosLmSearchCriteria
    {
    public:

        /**
        *  Specifies the type of the composite criterion.
        *  ECompositionOR - is supported only when all the contained critarias are CPosLmTextCriteria. it is not supported for other criteria.
        */
        enum TCompositionType
            {
            ECompositionAND = 0,     /**< Search results must match all
                                    * contained criteria. */
            ECompositionOR = 1		/**< Search result must match one of the contained criteria
            	
            						* ECompositionOR - is supported only when all the contained critarias are CPosLmTextCriteria. it is not supported for other criteria*/
            };

    public:

        /**
        * Two-phased constructor.
        *
        * @param[in] aType The composition type to use.
        * @returns A new instance of this class.
        */
        IMPORT_C static CPosLmCompositeCriteria* NewLC(
            TCompositionType  aType
        );

        /**
        * Destructor.
        */
        virtual ~CPosLmCompositeCriteria();

    public:

        /**
        * Returns the type of this composite criterion.
        *
        * @return The composition type. 
        */
        IMPORT_C TCompositionType CompositionType() const;

        /**
        * Set the type of this composite criterion.
        *
        * @param[in] The composition type to use. 
        */
        IMPORT_C void SetCompositionType( TCompositionType  aType );

        /**
        * Returns the number of criteria this object contains.
        *
        * @return The number of criteria this object contains.
        */
        IMPORT_C TUint NumOfArguments() const;

        /**
        * Returns a criterion argument contained in this object.
        *
        * This overload returns a const reference to the criterion argument.
        * There is another overload which returns a non-const reference.
        *
        * @param aIndex The argument to read. Must be in the interval
        *   [0, @ref NumOfArguments - 1], or this function will raise a
        *   USER-130 panic.
        * @return The requested argument.
        */
        IMPORT_C const CPosLmSearchCriteria& Argument( TUint aIndex ) const;

        /**
        * Returns a criterion argument contained in this object.
        *
        * This overload returns a non-const reference to the criterion
        * argument. There is another overload which returns a const reference.
        *
        * @param aIndex The argument to read. Must be in the interval
        *   [0, @ref NumOfArguments - 1], or this function will raise a
        *   USER-130 panic.
        * @return The requested argument.
        */
        IMPORT_C CPosLmSearchCriteria& Argument( TUint aIndex );

        /**
        * Adds a criterion to this composition.
        *
        * The composite criterion must contain at least one argument, otherwise
        * @ref CPosLandmarkSearch::StartLandmarkSearchL will fail with error
        * code @p KErrArgument.
        *
        * If this function returns without an error code, ownership of the
        * added criterion object is transferred to the composite object.
        *
        * @ref CPosLmCompositeCriteria are not supported when composition type is @ref ECompositionAND.
        * @ref Only CPosLmTextCriteria is supported in the case of @ref ECompositionOR
        *
        * Searching for landmark categories using @ref CPosLmCompositeCriteria 
        * is not supported in the case of ECompositionAND.
        *
        * @param[in] aCriteria The criterion to add to the composition.
        * @returns - @p KErrNone if successful
        *          - @p KErrNotSupported 
        *				- @p if @ref CPosLmCompositeCriteria or @ref CPosLmCatNameCriteria is specified when coposition type is ECompositionAND.
        *			 	- @p If criteria other than CPosLmTextCriteria is specified with ECompositionOR
        *          - @p otherwise a system wide error code.
        */
        IMPORT_C TInt AddArgument( CPosLmSearchCriteria* aCriteria );

        /**
        * Removes a criterion from this composition.
        *
        * Note: This function does not delete the criterion object. Instead,
        * the ownership of the object is passed to the caller.
        *
        * @param aIndex The argument to remove. Must be in the interval
        *   [0, @ref NumOfArguments - 1], or this function will raise a
        *   USER-130 panic.
        *
        * @returns The criterion object which was removed from the composition.
        */
        IMPORT_C CPosLmSearchCriteria* RemoveArgument( TUint  aIndex );

        /**
        * Removes and deletes all contained criterion objects.
        *
        * The composite criterion must contain at least one argument, otherwise
        * @ref CPosLandmarkSearch::StartLandmarkSearchL will fail with error
        * code @p KErrArgument.
        */
        IMPORT_C void ClearArguments();

    private:

        CPosLmCompositeCriteria( TCompositionType aType );

        void ConstructL();

        // Prohibit copy constructor
        CPosLmCompositeCriteria( const CPosLmCompositeCriteria& );
        // Prohibit assigment operator
        CPosLmCompositeCriteria& operator= ( const CPosLmCompositeCriteria& );

    private:

        TCompositionType                    iCompositionType;
        RPointerArray<CPosLmSearchCriteria> iArguments;

    };

#endif      // CPOSLMCOMPOSITECRITERIA_H


