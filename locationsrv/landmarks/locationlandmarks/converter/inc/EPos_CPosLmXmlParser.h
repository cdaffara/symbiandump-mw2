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
* Description: Parser that uses Symbian XML Framework to parse XML files with
*  Landmark Content Format.
*
*
*/


#ifndef CPOSLMXMLPARSER_H
#define CPOSLMXMLPARSER_H

//  INCLUDES
#include <f32file.h>
#include <xml/parser.h>
#include <xml/contenthandler.h>
#include <xml/documentparameters.h>
#include <EPos_CPosLandmarkParser.h>
#include "EPos_LmConverter.h"

// DATA TYPES
enum TLmXmlParserState
    {
    EStateParsingStateUnknown       = 0,
    EStateInitialized               = 0x01,
    EStateOperationConstructed      = 0x02,
    EStateParsingStarted            = 0x04,
    EStateParsing                   = 0x08,
    EStateParsedLandmark            = 0x10,
    EStateParsingForNextLm          = 0x20,
    EStateParsingPartReady          = 0x40,
    EStateParsingReady              = 0x80
    };

enum TXmlParserElementState
    {
    EXmlParserOnStartElement,
    EXmlParserOnEndElement
    };


// FORWARD DECLARATIONS
class CPosLmXmlParserOperation;
class CPosLmCollectionData;
class CPosLmParsedLmBuffer;

// CLASS DECLARATION

/**
*  Parser that uses Symbian XML Framework to parse XML files with
*  Landmark Content Format.
*
*/
class CPosLmXmlParser : public CPosLandmarkParser, public Xml::MContentHandler
    {
    public:  // Constructors and destructor

        /**
        * Destructor.
        */
        virtual ~CPosLmXmlParser();

    public: // New functions

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
        * Reset parser due to error during parsing.
        */
        void ResetParserOnError();

        /** Called by operation to inform that it's being destroyed */
        void OperationDestroyed();

    public: // From CPosLandmarkParser

        /**
        * @param aBuffer The buffer containing the landmark content.
        */
        void SetInputBuffer( const TDesC8&  aBuffer );

        /**
        * @param aFile The file containing the landmark content.
        */
        void SetInputFileL( const TDesC&  aFile );

        /**
        * @param aFileHandle The handle to the file which should be parsed.
        */
        void SetInputFileHandleL( RFile&  aFileHandle );

        /**
        * @param aBuildIndex Specifies if the parser should build an
        *   index.
        * @return A handle to the operation.
        */
        CPosLmOperation* ParseContentL( TBool  aBuildIndex = EFalse );

        /**
        * @return The total number of parsed landmarks.
        */
        TUint32 NumOfParsedLandmarks() const;

        /**
        * @return The first landmark collection data.
        */
        TPosLmCollectionDataId FirstCollectionDataId() const;

        /**
        * @param aCollectionDataId Collection data id.
        * @return The next landmark collection data.
        */
        TPosLmCollectionDataId NextCollectionDataId( TPosLmCollectionDataId  aCollectionDataId ) const;

        /**
        * @param aDataId The identifier for the collection data to retrieve.
        * @return The requested collection data.
        */
        virtual TPtrC CollectionData( TPosLmCollectionDataId  aDataId ) const;

        /**
        * @param aLandmarkIndex The index of the landmark to retrieve.
        * @return The requested landmark.
        */
        CPosLandmark* LandmarkLC( TUint  aLandmarkIndex = KPosLastParsedLandmark ) const;

        /**
        * @param aCategoryId The ID of the landmark category.
        * @return The requested landmark category.
        */
        CPosLandmarkCategory* LandmarkCategoryLC(
        /* IN  */   TPosLmItemId  aCategoryId
        ) const;

    public: // From Xml::MContentHandler

        void OnStartDocumentL(
            const Xml::RDocumentParameters& aDocParam,
            TInt aErrorCode );

        void OnEndDocumentL(TInt aErrorCode);

        void OnStartElementL(
            const Xml::RTagInfo& aElement,
            const Xml::RAttributeArray& aAttributes,
            TInt aErrorCode );

        void OnEndElementL( const Xml::RTagInfo& aElement, TInt aErrorCode );

        void OnContentL( const TDesC8& aBytes, TInt aErrorCode );

        void OnStartPrefixMappingL(
            const RString& aPrefix,
            const RString& aUri,
            TInt aErrorCode );

        void OnEndPrefixMappingL( const RString& aPrefix, TInt aErrorCode );

        void OnIgnorableWhiteSpaceL( const TDesC8& aBytes, TInt aErrorCode );

        void OnSkippedEntityL( const RString& aName, TInt aErrorCode );

        void OnProcessingInstructionL(
            const TDesC8& aTarget,
            const TDesC8& aData,
            TInt aErrorCode );

        void OnOutOfData();

        void OnError( TInt aErrorCode );

        TAny* GetExtendedInterface( const TInt32 aUid );

    protected:
    
        virtual void OnAttributesL( TPosXmlTagType aTag, const Xml::RAttributeArray& aAttributes ) = 0;
    
        virtual void GetNextAllowedTagsL( RArray<TPosXmlTagType>& aChildTags ) = 0;
    
        virtual void CheckMandatoryTagsL( TPosXmlTagType aTreeType ) = 0;
        
        virtual TBool IsLandmarkStarted( TPosXmlTagType aTag ) = 0;
    
        virtual TBool IsCategoryStarted( TPosXmlTagType aTag ) = 0;
    
        virtual void SetContentL( TPosXmlTagType aTag, const TDesC& aContent ) = 0;

        virtual TPosXmlTagType TagTypeL( const Xml::RTagInfo& aElement ) = 0;
        
        virtual TBool IsTreeTag( TPosXmlTagType aTag ) = 0;
        
    protected:

        /** Verifies if given tag has been parsed in current tree*/
        TBool ChildTagFoundL( TPosXmlTagType aTagType );

        /**
         * Add parsed collection data.
         *
         * @param aCollId Collection data identifier.
         * @param aContent Collection data content.
         */
        void AddCollectionDataL(
            TPosLmCollectionDataId aCollId,
            const TDesC& aContent );
        
        virtual TBool EqualsTagL( const TDesC8& aTag, const TDesC& aDefinedTag );
          
        /** Updates state of the parser when another landmark is parsed */
        void HandleLandmarkParsedL();
        
        /** Updates state of the parser when root tag is parsed */
        void HandleRootTag();
          
        /** Returns last parsed known child tag */
        TPosXmlTagType LastKnownChildTag();

        /** Returns sequence of tags given in aChildTags[aChildTagsCount],
         *  which includes only tags starting from the last parsed known child tag */
        void GenValidSequenceL( 
            const TPosXmlTagType* aChildTags,
            TInt aChildTagsCount,
            RArray<TPosXmlTagType>& aSequence );

        /** Returns set of tags given in aChildTags[aChildTagsCount],
         *  excluding all last parsed known child tags */
        void GenValidSetL( 
            const TPosXmlTagType* aChildTags,
            TInt aChildTagsCount,
            RArray<TPosXmlTagType>& aSet );

        /** Marks current tag as tree */
        void StartNewTreeL();

        /** Resets to previous tree tag */
        void CloseCurrentTree();
        
        /** Returns type of current tree */
        TPosXmlTagType CurrentTree();
        
        /** Returns type of current tag */
        TPosXmlTagType CurrentTag();
        
        /** Index of current tree tag in iReadTags */
        TInt CurrentTreeTagIndex();
        
        /** Checks if given tag has been processed in current tree */
        TBool ChildTagFound( TPosXmlTagType aTag );
        
        /** Compares namespace of given tag with root namespace */
        TBool MatchesRootNamespace( const Xml::RTagInfo& aElement );

    private:

        /**
        * Initiates the parser to parse from a file.
        *
        * The file is either passed as a file handle or as a file name. If the
        * file handle is @p NULL, then this method will open the file passed in
        * the @p aFile parameter.
        *
        * @param aFileHandle The handle to the file to parse, or @p NULL if a
        *   new file handle should be opened.
        * @param aFile The file to open if @p aFileHandle is set to @p NULL.
        */
        void SetInputFileInternalL(
            RFile* aFileHandle,
            const TDesC& aFile = KNullDesC );

        /**
        * Checks is parsing is complete.
        *
        * @return Whether test is complete or not.
        */
        TBool IsParsingComplete();

        /**
        * Completes parsing against the Symbian XML parser.
        *
        * @return System wide error code.
        */
        TInt CompleteParsing();

        /**
        * Resets the parser.
        */
        void ResetParser();

        /**
        * Resets the parser's state, meaning that it won't reset the underlying
        * Symbian XML parser. It will release it's allocated resources.
        */
        void ResetParserState();

        /**
        * Resets the parser partly, used between when each part is parsed.
        */
        void ResetParserPartState();

        /**
        * Current read buffer length.
        *
        * @return Length of read buffer.
        */
        TInt BufferLength() const;

        /**
        * Checks if parsed has parsed part/on parsed landmark.
        *
        * @return Whether one part is parsed.
        */
        TBool ParsedPart();

        /**
        * Reads a predefined length from buffer/file.
        *
        */
        void ReadNextPartL();

        /** Informs operation to disconnect from this parser */
        void DisconnectOperation();

        /** Removes all child tags of the current tree from the tag list */
        void RemoveChildTags();

        /**
         * Sets parser state.
         * @param aTag Tag to set parsed state for.
         */
        void SetParserStateL( TPosXmlTagType aTag );
        
        /** Converts current content to Unicode and calls SetContentL */
        void SaveContentL( TPosXmlTagType aTag );
        
        /** Verifies that given tag is allowed on current phase */
        void CheckSequenceOrderL( TPosXmlTagType aTag );
        
        

    protected:

        // C++ default constructor
        CPosLmXmlParser();
        
        /** Reset parser state, including symbina XML Parser, input Buffer*/
        void ResetContentL();
        
		/**
		*Close Filehandle if it is open
		*/
		void CloseCurrentFileHandle();
        
        
    private:    
        // By default, prohibit copy constructor
        CPosLmXmlParser( const CPosLmXmlParser& );
        // Prohibit assigment operator
        CPosLmXmlParser& operator= ( const CPosLmXmlParser& );
       
    protected:   

        // Saved parsed values
        TReal64                         iParsedLatitude;
        TReal64                         iParsedLongitude;
        TReal32                         iParsedAltitude;
        TReal32                         iParsedHorAcc;
        TReal32                         iParsedVertAcc;
        HBufC*                          iReadMimeType;
        HBufC*                          iReadUrl;
        TUint                           iNoOfMediaLinks;

        // Object that are changed during parsing
        TUint32                             iNumOfParsedLandmarks;
       
        TPosLmItemId                        iWorkingItemId;
        CPosLandmark*                       iWorkingLandmark;
        CPosLandmarkCategory*               iWorkingCategory;

        RPointerArray<CPosLandmarkCategory> iCategories;
        
        //data
        // Stuff for handling file
        RFs                             iFs;
        RFile                           iFile;
        RFile*                          iFileHandle;
        
        // Parser stuff
        Xml::CParser*                   iXmlParser;
        HBufC8*                         iBuffer;
        TInt                            iNumberOfBytesRead;
        TInt                            iParserError;
        TUint                           iParserStatus;
        TReal32                         iIncrBytesPerLm;

    private:    // Data



        // Input stuff
        TPtrC8                          iInputBuffer;
        TInt                            iContentSize;

        

        // Parsed data
        HBufC8*                         iRootNs;
        HBufC8*                         iReadContent;

        RArray<TPosXmlTagType>          iReadTags;
        RArray<TInt>                    iTrees;
        TXmlParserElementState          iLastElementState;

        // Operation to notify
        CPosLmXmlParserOperation*          iParserOp;

        RPointerArray<CPosLmParsedLmBuffer> iParsedLmBuffer;

        RPointerArray<CPosLmCollectionData> iCollectionData;
        
    };

#endif      // CPOSLMXMLPARSER_H

// End of File
