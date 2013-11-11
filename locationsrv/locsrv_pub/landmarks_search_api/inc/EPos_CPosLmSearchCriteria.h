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
* Description:  CPosLmSearchCriteria class
*
*/


#ifndef CPOSLMSEARCHCRITERIA_H
#define CPOSLMSEARCHCRITERIA_H

#include <e32base.h>

/**
*  Abstract base class for landmark search criterion classes.
*
*  Criterion classes are used in @ref CPosLandmarkSearch to specify what to
*  search for. It specifies what criteria the landmark must fulfill to be
*  considered a search match.
*
*  @lib eposlmsearchlib.lib
*  @since S60 3.0
*/
class CPosLmSearchCriteria : public CBase
    {
    public:

        /**
        *  Specifies the subclass of the criterion object.
        */
        enum TCriteriaType
            {
            ECriteriaArea = 0     /**< subclass CPosLmAreaCriteria */,
            ECriteriaText         /**< subclass CPosLmTextCriteria */,
            ECriteriaComposite    /**< subclass CPosLmCompositeCriteria */,
            ECriteriaCategory     /**< subclass CPosLmCategoryCriteria */,
            ECriteriaFindNearest  /**< subclass CPosLmNearestCriteria */,
            ECriteriaIdList       /**< subclass CPosLmIdListCriteria */,

            ECriteriaCategoryByName = 100 /**< subclass CPosLmCatNameCriteria */
            };

    public:

        /**
        * Destructor.
        */
        virtual ~CPosLmSearchCriteria();

    public:

        /**
        * Returns the criterion type.
        *
        * @returns The criterion type.
        */
        IMPORT_C TCriteriaType CriteriaType() const;

    protected:

        // C++ constructor.
        CPosLmSearchCriteria( TCriteriaType aCriteriaType );

    private:

        // Prohibit copy constructor
        CPosLmSearchCriteria( const CPosLmSearchCriteria& );
        // Prohibit assigment operator
        CPosLmSearchCriteria& operator= ( const CPosLmSearchCriteria& );

    private:

        TCriteriaType iCriteriaType;

    };

#endif      // CPOSLMSEARCHCRITERIA_H


