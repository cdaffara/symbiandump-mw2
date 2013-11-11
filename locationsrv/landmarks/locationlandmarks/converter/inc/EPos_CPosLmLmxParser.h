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
* Description: Parser implementation for GPX format.
*
*
*/


#ifndef CPOSLMLMXPARSER_H
#define CPOSLMLMXPARSER_H

//  INCLUDES
#include <f32file.h>
#include "EPos_CPosLmXmlParser.h"

/**
 *  Parser implementation for GPX format.
 */
class CPosLmLmxParser : public CPosLmXmlParser
    {
    public:  // Constructors and destructor

        /**
        * Two-phased constructor.
        * @returns A new instance of this class.
        */
        static CPosLmLmxParser* NewL();

        /**
        * Destructor.
        */
        virtual ~CPosLmLmxParser();

    protected: // From CPosLmXmlParser

         void OnAttributesL( TPosXmlTagType aTag, const Xml::RAttributeArray& aAttributes );

         void RemoveTagsL( TPosXmlTagType aTag );

         void GetNextAllowedTagsL( RArray<TPosXmlTagType>& aChildTags );
         
         void CheckMandatoryTagsL( TPosXmlTagType aTag );
         
         TBool IsLandmarkStarted( TPosXmlTagType aTag );

         TBool IsCategoryStarted( TPosXmlTagType aTag );
    
         void SetContentL( TPosXmlTagType aTag, const TDesC& aContent );
         
         TPosXmlTagType TagTypeL( const Xml::RTagInfo& aElement );

         TBool IsTreeTag( TPosXmlTagType aTag );

    private:

    TPositionFieldId PositionFieldId( TPosXmlTagType aXmlTagType );
         
    private:

        /**
        * C++ default constructor.
        */
        CPosLmLmxParser();
        
        // By default, prohibit copy constructor
        CPosLmLmxParser( const CPosLmLmxParser& );
        // Prohibit assigment operator
        CPosLmLmxParser& operator= ( const CPosLmLmxParser& );

    private:    // Data

    };

#endif      // CPOSLMLMXPARSER_H

// End of File
