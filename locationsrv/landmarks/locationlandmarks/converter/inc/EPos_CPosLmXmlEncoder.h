/*
* Copyright (c) 2002-2007 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description: Class used for encoding xml landmark content.
*
*/


#ifndef CPOSLMXMLENCODER_H
#define CPOSLMXMLENCODER_H

#include <EPos_CPosLandmarkEncoder.h>
#include "EPos_LmConverter.h"

class CPosLmOutput;

/**
*  Class used for encoding xml landmark content.
*
*  Output is written either to a buffer or to a file.
*
*  The basic protocol for encoding is to 1) define where to write the
*  output to by calling @ref SetUseOutputBufferL or @ref SetOutputFileL, 2)
*  optionally add collection data using @ref AddCollectionDataL, 3) add
*  landmark data to encode by using functions in @ref CPosLandmarkEncoder
*  and/or @ref CPosLandmarkDatabase::ExportLandmarksL and 4) finalize the
*  encoding by calling @ref FinalizeEncodingL. If this protocol is not followed
*  the client is panicked with code @p EPosLmProtocolBreak. Encoding can be
*  performed multiple times using the same encoder object.
*
*/
class CPosLmXmlEncoder : public CPosLandmarkEncoder
    {
    public:  

        /**
        * Destructor.
        */
        ~CPosLmXmlEncoder();

    public: // From CPosLandmarkEncoder

        CBufBase* SetUseOutputBufferL();

        void SetOutputFileL( const TDesC&  aOutputFile );

        void AddCollectionDataL(
            TPosLmCollectionDataId  aDataId,
            const TDesC&  aCollectionData );

        void AddLandmarkL( const CPosLandmark&  aLandmark );

        void AddCategoryForLatestLandmarkL( const CPosLandmarkCategory&  aCategory );

        CPosLmOperation* FinalizeEncodingL();

    protected:
    
        /** Writes start tags. At least root tag should be written */
        virtual void WriteStartTagsL() = 0;
        
        /** Writes collection name and description, if supported */
        virtual void WriteCollectionDataL( 
            const HBufC* aCollectionName,
            const HBufC* aCollectionDescription ) = 0;
        
        /** Writes landmark with categories */
        virtual void WriteLandmarkL( 
            const CPosLandmark& aLandmark, 
            const RPointerArray<const CPosLandmarkCategory>& aCategories ) = 0;
        
        /** Writes end tags. At least root tag closure should be written, but also
         *  any other tag added in WriteStartTagsL should be closed */
        virtual void WriteFinalTagsL() = 0;

        /** Returns string of tag name for given tag type */
        virtual TPtrC TagNameL( TPosXmlTagType aTagType ) = 0;

    protected:
      
        /**
         * Checks if the specified tag type is a leaf or a branch.
         *
         * @param aXmlTagType The tag type.
         * @return ETrue if the tag type is a leaf,
         *   EFalse if the tag type is a branch.
         */
         TBool IsTagLeaf( TPosXmlTagType aXmlTagType );

         /** Writes XML prolog */
         void WritePrologL();

         /** Writes start of XML tag with attributes string */
         void WriteSTagL( TPosXmlTagType aTagType, const TDesC& aAttributes );

         /** Writes end of XML tag */
         void WriteETagL( TPosXmlTagType aTagType );

         /** Writes full leaf tag with string value 
          *  @param aDes tag value 
          *  @param aTagType tag type */
         void WriteStringWithTagsL( const TDesC16& aDes, TPosXmlTagType aTagType );

         /** Writes full leaf tag with real value 
          *  @param aValue tag value 
          *  @param aTagType tag type */
         void WriteTRealWithTagsL( TReal aValue, TPosXmlTagType aTagType );

         /** Writes string value escaping prohibited characters
          *  @param aDes tag value */
         void WriteEscapedStringL( const TDesC& aDes );

    private:

        CBufBase* CreateOutPutBufferLC();

        void CreateOutputFileL( const TDesC&  aOutputFile );

        /** Starts encoding. Writes prolog string. */
        void InitializeEncodingL();
        
        void ResetEncoderL();

        HBufC16* IndentationLC();
        void ResetIndentation();
        
        /**
         * Get the prolog string.
         * Caller takes ownership of the returned string.
         *
         * @return The prolog string.
         */
        static HBufC16* PrologStringLC();
        
         /**
         * Get the start tag for the specified tag type.
         * Caller takes ownership of the returned string.
         *
         * @param aTagType The tag type.
         * @param aAttributes String of attributes if any needed.
         * @return The start tag.
         */
        HBufC16* STagStringLC( TPosXmlTagType aTagType, const TDesC& aAttributes );
        
         /**
         * Get the end tag for the specified tag type.
         * Caller takes ownership of the returned string.
         *
         * @param aTagType The tag type.
         * @return The end tag.
         */
        HBufC16* ETagStringLC( TPosXmlTagType aTagType );
        
        /** Dumps last added landmark with categories */
        void WriteLastLandmarkL();
        
    protected:
        CPosLmXmlEncoder();
        
    private:
        // Prohibit copy constructor
        CPosLmXmlEncoder( const CPosLmXmlEncoder& );
        // Prohibit assigment operator
        CPosLmXmlEncoder& operator= ( const CPosLmXmlEncoder& );

    protected:
        CPosLandmark* iLastLandmark;
        RPointerArray<const CPosLandmarkCategory> iCategories;
    
    private:    // Data

        enum TPosEncoderStatus
            {
            EPosUninitialized = 0,
            EPosInitialized,
            EPosCollectionInfoAdded,
            EPosLmInfoAdded
            };

        TPosEncoderStatus           iEncoderStatus;

        CPosLmOutput*               iOutput;

        HBufC*                      iCollectionDataName;
        HBufC*                      iCollectionDataDescription;

        TInt                        iIndentationDepth;
    };

#endif      // CPOSLMXMLENCODER_H
