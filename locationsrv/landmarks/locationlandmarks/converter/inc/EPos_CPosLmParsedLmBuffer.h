/*
* Copyright (c) 2005 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description: Class for holding landmarks and landmark categories while parsing
* a Landmark XML file to a landmark(s).
*
*/


#ifndef CPOSLMPARSEDLMBUFFER_H
#define CPOSLMPARSEDLMBUFFER_H

//  INCLUDES
#include <e32base.h>
#include <EPos_CPosLandmark.h>
#include <EPos_CPosLandmarkCategory.h>

// CLASS DECLARATION

/**
*  Class for holding landmarks and landmark categories while parsing
* a Landmark XML file to a landmark(s).
*
*/
class CPosLmParsedLmBuffer : public CBase
    {
    public:  // Constructors and destructor

        /**
        * Constructor.
        */
        static CPosLmParsedLmBuffer* NewLC(
        /* IN */    CPosLandmark& aLm,
        /* IN */    RPointerArray<CPosLandmarkCategory>& aLmCategories
        );

        /**
        * Destructor.
        */
        ~CPosLmParsedLmBuffer();

    public: // New functions

        /**
        * Retrieve the landmark of the buffer.
        *
        * @return A landmark.
        */
        CPosLandmark* Landmark() const;

        /**
        * Retrieve the landmark categories of the buffer.
        *
        * @return Landmark categories.
        */
        RPointerArray<CPosLandmarkCategory> Categories() const;

    private:

        /**
        * C++ default constructor.
        */
        CPosLmParsedLmBuffer();

        /**
        * Symbian 2nd phase constructor.
        */
        void ConstructL(
            CPosLandmark& aLm,
            RPointerArray<CPosLandmarkCategory>& aLmCategories
        );

        // By default, prohibit copy constructor
        CPosLmParsedLmBuffer( const CPosLmParsedLmBuffer& );
        // Prohibit assigment operator
        CPosLmParsedLmBuffer& operator= ( const CPosLmParsedLmBuffer& );

    private:

        CPosLandmark*                       iLandmark;
        RPointerArray<CPosLandmarkCategory> iLandmarkCategories;

    };

#endif      // CPOSLMPARSEDLMBUFFER_H

// End of File
