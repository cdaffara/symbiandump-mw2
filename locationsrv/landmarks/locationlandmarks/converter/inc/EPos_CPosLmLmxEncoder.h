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
* Description: Implementation of CPosLmXmlEncoder for LMX format.
*
*/


#ifndef CPOSLMLMXENCODER_H
#define CPOSLMLMXENCODER_H

#include "EPos_CPosLmXmlEncoder.h"

/**
*  Implementation of CPosLmXmlEncoder for LMX format.
*/
class CPosLmLmxEncoder : public CPosLmXmlEncoder
    {
    public:  // Constructors and destructor

        /**
        * Two-phased constructor.
        *
        * @returns A new instance of this class.
        */
        static CPosLmLmxEncoder* NewL();

        /**
        * Destructor.
        */
        ~CPosLmLmxEncoder();

    protected: // From CPosLmXmlEncoder
      
        void WriteStartTagsL();
        
        void WriteCollectionDataL( 
            const HBufC* aCollectionName,
            const HBufC* aCollectionDescription );
        
        void WriteLandmarkL( 
            const CPosLandmark& aLandmark, 
            const RPointerArray<const CPosLandmarkCategory>& aCategories );
        
        void WriteFinalTagsL();
        
        TPtrC TagNameL( TPosXmlTagType aTagType );

    private:

        /** Writes coordinate tags */
        void WriteLmCoordinatesL( const TLocality&  aLocality );

        /** Writes address information tags */
        void WriteLmPositionFieldsL( const CPosLandmark&  aLandmark );

        /** Writes category tags */
        void WriteCategoryL( const CPosLandmarkCategory&  aCategory );

        /**
         * Get the tag type for the specified position field id.
         *
         * @param aPosFieldId The position field id.
         * @return The tag type.
         */
         static TPosXmlTagType XmlTagForPositionField( TPositionFieldId aPosFieldId );

    private:

        CPosLmLmxEncoder();
        // Prohibit copy constructor
        CPosLmLmxEncoder( const CPosLmLmxEncoder& );
        // Prohibit assigment operator
        CPosLmLmxEncoder& operator= ( const CPosLmLmxEncoder& );

    private: 

    };

#endif      // CPOSLMLMXENCODER_H
