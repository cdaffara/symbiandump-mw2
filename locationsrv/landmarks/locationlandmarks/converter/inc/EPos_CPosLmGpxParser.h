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


#ifndef CPOSLMGPXPARSER_H
#define CPOSLMGPXPARSER_H

//  INCLUDES
#include <f32file.h>
#include "EPos_CPosLmXmlParser.h"

/**
 *  Parser implementation for GPX format.
 */
class CPosLmGpxParser : public CPosLmXmlParser
    {
    public:  // Constructors and destructor

        /**
        * Two-phased constructor.
        * @returns A new instance of this class.
        */
        static CPosLmGpxParser* NewL();

        /**
        * Destructor.
        */
        virtual ~CPosLmGpxParser();
        
    protected:
    
        /** Determines type of attribute */
        TPosXmlTagType AttributeTypeL( 
            TPosXmlTagType aParent, 
            const Xml::RTagInfo& aElement );

    protected: // From Xml::MContentHandler
        /**
        * @param aDocParam Document parameters.
        * @param aErrorCode A error code.
        */
        void OnStartDocumentL(
            const Xml::RDocumentParameters& aDocParam,
            TInt aErrorCode
        );
    
        
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

        /**
        * C++ default constructor.
        */
        CPosLmGpxParser();

        
        // By default, prohibit copy constructor
        CPosLmGpxParser( const CPosLmGpxParser& );
        // Prohibit assigment operator
        CPosLmGpxParser& operator= ( const CPosLmGpxParser& );

    private:    // Data

        enum TGpxVersion
            {
            EGpxVersionUnknown,
            EGpxVersion10,
            EGpxVersion11,
            };

        TGpxVersion iGpxVersion;
    };

#endif      // CPOSLMGPXPARSER_H

// End of File
