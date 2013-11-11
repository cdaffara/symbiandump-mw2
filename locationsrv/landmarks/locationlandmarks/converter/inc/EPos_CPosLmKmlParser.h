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
* Description: Parser implementation for KML format.
*
*
*/


#ifndef CPOSLMKMLPARSER_H
#define CPOSLMKMLPARSER_H

//  INCLUDES
#include <f32file.h>
#include "EPos_CPosLmXmlParser.h"


/**
 *  Parser implementation for KML format.
 */
class CPosLmKmlParser : public CPosLmXmlParser
    {
    public:  // Constructors and destructor

        /**
        * Two-phased constructor.
        * @returns A new instance of this class.
        * @param aMimeType Mime type of the file.
        */
        static CPosLmKmlParser* NewL( TAny* aMimeType);

        /**
        * Destructor.
        */
        virtual ~CPosLmKmlParser();
        
     protected: // From Xml::MContentHandler   
     
        void OnStartElementL(
            const Xml::RTagInfo& aElement,
            const Xml::RAttributeArray& aAttributes,
            TInt aErrorCode );

        void OnEndElementL( const Xml::RTagInfo& aElement, TInt aErrorCode );
        /**
        * @param aDocParam Document parameters.
        * @param aErrorCode A error code.
        */
        void OnStartDocumentL(
            const Xml::RDocumentParameters& aDocParam,
            TInt aErrorCode
        );
        
         /** Determines type of attribute */
        TPosXmlTagType AttributeTypeL( 
            TPosXmlTagType aParent, 
            const Xml::RTagInfo& aElement );
       
    protected: //From CposXmlParser
     
        /**
        * Parses next part of either buffer or from file.
        *
        * @param[out] aProgress The progress of the parsing after the function
        *   is finished.
        * @return KErrNone if parsing is ready, otherwise
        * KPosLmOperationNotComplete.
        */
        virtual TInt ParseNextPartL( TReal32& aProgress );

        /**
        * @param aFile The file containing the landmark content.
        */
        void SetInputFileL( const TDesC&  aFile );

        /**
        * @param aFileHandle The handle to the file which should be parsed.
        */
        void SetInputFileHandleL( RFile&  aFileHandle );
        
        
        void OnAttributesL( TPosXmlTagType aTag, const Xml::RAttributeArray& aAttributes );
        
        void RemoveTagsL( TPosXmlTagType aTag );
        
        void GetNextAllowedTagsL( RArray<TPosXmlTagType>& aChildTags );
        
        void CheckMandatoryTagsL( TPosXmlTagType aTag );
         
        TBool IsLandmarkStarted( TPosXmlTagType aTag );
        
        TBool IsCategoryStarted( TPosXmlTagType aTag );
        
        void SetContentL( TPosXmlTagType aTag, const TDesC& aContent );
        
        TPosXmlTagType TagTypeL( const Xml::RTagInfo& aElement );
        
        TBool IsTreeTag( TPosXmlTagType aTag );
        
        virtual TBool EqualsTagL( const TDesC8& aTag, const TDesC& aDefinedTag );
        
        
    protected:
    		/** 
    		*Clear data, after all the parsing done, if Required 
    		*/
        virtual void ClearParserData();
       
    private:

        /**
        * C++ default constructor.
        */
        CPosLmKmlParser(TAny* aMimeType);
        
        /** 
        *check that given string contains only numbers
        *@param aStr input string
        */
		TBool IsNumber( const TDesC& aStr);
			
		/** 
		* Calculate and returns the number of KML file packed inside KMZ file 
		*
		*/
		TInt GetKmlFileCountL(const TDesC& aFileName);
	
		
        
        // By default, prohibit copy constructor
        CPosLmKmlParser( const CPosLmKmlParser& );
        // Prohibit assigment operator
        CPosLmKmlParser& operator= ( const CPosLmKmlParser& );
     
    private: //data   
    		//Holds category name
        RPointerArray<HBufC> iCategoryArray;
        RArray<TBool> iCategoryAdded;
        TInt iFolderCount;
        TBool iIsKmzFile;
        CDir* iKmlFileList;
        TInt iCurrentProssedFile;
        TBool iPreviousFileCompleted;
       

    
    };

#endif      // CPosLmKmlParser_H

// End of File
