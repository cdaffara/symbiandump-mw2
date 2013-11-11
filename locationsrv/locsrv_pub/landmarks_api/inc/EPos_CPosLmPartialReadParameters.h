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
* Description:  CPosLmPartialReadParameters class
*
*/


#ifndef CPOSLMPARTIALREADPARAMETERS_H
#define CPOSLMPARTIALREADPARAMETERS_H

#include <e32base.h>
#include <lbsfields.h>
#include "EPos_CPosLandmark.h"

/**
*  Container class for partial landmark settings.
*  Partial settings are used to define which landmark data should be returned
*  when @ref CPosLandmarkDatabase::ReadPartialLandmarkLC is called.
*
*  @lib eposlandmarks.lib
*  @since S60 3.0
*/
class CPosLmPartialReadParameters : public CBase
    {
    public:

        /**
        * Two-phased constructor.
        * @returns A new instance of this class.
        */
        IMPORT_C static CPosLmPartialReadParameters* NewLC();

        /**
        * Destructor.
        */
        virtual ~CPosLmPartialReadParameters();

    public:

        /**
        * Returns a bitmap of the landmark attributes which should be included
        * in a partial landmark.
        *
        * The bitmap is a combination of the values in
        * CPosLandmark::TAttribute.
        *
        * @return The partial landmark attributes bitmap.
        */
        IMPORT_C CPosLandmark::TAttributes RequestedAttributes() const;

        /**
        * Sets a bitmap of the landmark attributes which should be included in
        * a partial landmark.
        *
        * The bitmap is constructed from the values in
        * CPosLandmark::TAttribute.
        *
        * @param[in] aAttributes A partial landmark attributes bitmap.
        */
        IMPORT_C void SetRequestedAttributes(
            CPosLandmark::TAttributes aAttributes
        );

        /**
        * Returns the position fields which should be included in a partial
        * landmark.
        *
        * @param[out] aRequestedFields Will contain the requested position fields,
        *   i.e. @p TPositionFieldId.
        * @returns @p KErrNone if successful, otherwise one of the system wide
        *   error codes.
        */
        IMPORT_C TInt GetRequestedPositionFields(
            RArray<TUint>& aRequestedFields
        ) const;

        /**
        * Sets the position fields which should be included in a partial
        * landmark.
        *
        * @param[in] aRequestedFields The requested position fields, i.e.
        *   @p TPositionFieldId.
        * @returns @p KErrNone if successful, otherwise one of the system wide
        *   error codes.
        */
        IMPORT_C TInt SetRequestedPositionFields(
            const RArray<TUint>& aRequestedFields
        );

    private:

        // C++ constructor.
        CPosLmPartialReadParameters();

        // Symbian constructor
        void ConstructL();

        // Prohibit copy constructor
        CPosLmPartialReadParameters(
            const CPosLmPartialReadParameters& );
        // Prohibit assigment operator
        CPosLmPartialReadParameters& operator= (
            const CPosLmPartialReadParameters& );

    private:

        // Requested position fields
        RArray<TUint> iRequestedFields;

        // Requested attributes
        TUint32 iAttributes;
    };

#endif      // CPOSLMPARTIALREADPARAMETERS_H


