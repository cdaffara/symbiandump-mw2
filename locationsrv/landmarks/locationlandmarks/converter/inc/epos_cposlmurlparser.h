/*
* Copyright (c) 2009 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description: Class used for parsing a Landmarks URL & obtaining 
*							 landmarks information from it.
*
*
*/


#ifndef CPOSLMURLPARSER_H
#define CPOSLMURLPARSER_H

//  INCLUDES
#include <f32file.h>
#include <EPos_CPosLandmarkParser.h>
#include "EPos_LmConverter.h"



// FORWARD DECLARATIONS
class CPosLmUrlParserOperation;


// CLASS DECLARATION

/**
*  Parser that parses an URL containing location information to 
*  obtain a landmark object.
*
*/
class CPosLmUrlParser : public CPosLandmarkParser
    {
    public:  

        /**
        * Two-phased constructor.
        *
        * @returns A new instance of this class.
        */
        static CPosLmUrlParser* NewL();
        /**
        * Destructor.
        */
        ~CPosLmUrlParser();
       
    private:
        /**
        *  Default Constructor
        */
        CPosLmUrlParser();
        /**
        * Prohibit copy constructor
        */ 
        CPosLmUrlParser( const CPosLmUrlParser& );
        /**
        * Prohibit assigment operator
        */ 
        CPosLmUrlParser& operator= ( const CPosLmUrlParser& );
 
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
        
    public:
        /**
         * Delete the parser operation,called by the operation class to inform the 
         * parser about it being destroyed.
         */
        void UrlOperationDestroyed();
        /**
         * Parses the URL.Called by the Operation class NextStep or ExecuteL method
         * in order to execute the parsing operation.
         * @return KErrNone - if parsing successful
         *         KErrPosLmUnknownFormat - if there is any problem during parsing
         *         (i.e URL does not conform to the grammar as specified in the api spec)
         */
        TInt ParseUrlL();
        /**
         * Resets the URL parsing by deleting the previous data.
         */
        void ResetUrlParser();
        
    private:

        /**
         * Deletes the operation instance if it has been created.
         */
        void DisconnectUrlOperation();
        /**
         * Parses the URL in the extracted buffer 
         */
        void ParseBufferL( const TDesC8& aUrlBuffer );
        /**
         * Parse & validate the URL host
         */
        void ParseUrlHostL();
        /**
         * Parse & validate the URl location info
         */
        void ParseUrlLocationL();
        /**
         * Sets the parser status to complete
         */
        void SetParserStatusL( TInt aStatus );
        /**
         * Sets the Position for the landmark
         */
        void SetLocalityL();
        /**
         * Parse & validate the URl parameters
         */
        void ParseUrlParametersL( const TDesC8& aUrlParameters );
        /**
         * Parses the extracted param value pairs
         */
        void ParseParamValueL( const TDesC& aParamValue );
        /**
         * Compares the parameters & checks the values
         */
        void CompareParamValueL( const TDesC8& aParam, const TDesC8& aValue );
        /**
         * Sets the latitude/longitude param depending on the enum value
         * specifying which param is to be set
         * @param in aParsedValue The number in text form obtained from URL
         * @param in aValue The number in TReal64 format after parsing
         * @param in aField Indicates which of the field( latitude/longitude)
         *                  is being set.
         */
        void SetLatLongParamL( const TDesC8& aParsedValue, TReal64& aValue, 
                               TInt aField );
        /**
         * Sets the number fields such as altitude,altitudeaccuracy,
         * positionaccuracy
         * @param in aParsedValue The number in text form obtained from URL
         * @param in aValue The number in TReal32 format after parsing
         * @leave Any of the Symbian error codes
         */
        void SetNumberParamL( const TDesC8& aParsedValue, TReal32& aValue );
        /**
         * Sets the number fields such as speed & heading which are set
         * as position fields
         * @param in aParsedValue The number in text form obtained from URL
         * @param in aField Indicates which of the field( speed/heading )
         *                  is being set.
         * @leave Any of the Symbian error codes
         */
        void SetPositionFieldNumberParamL( const TDesC8& aParsedValue, TInt aField  );
        /**
         * Sets the text params depending on the field value to be set
         * @param in aParsedValue The text value obtained from URL
         * @param in aMaxParseValueLength The max length allowed for the field
         * @param in aField Indicates the text field that is to be set
         * @leave Any of the Symbian error codes
         */
        void SetTextParamL( const TDesC8& aParsedValue, const TInt aMaxParseValueLength,
                            TInt aField );
        /**
         * Parses the text field & decodes the data
         */
        HBufC* ParseTextL( const TDesC8& aParseValue, const TInt aMaxParseValueLength );
        /**
         * Sets the timestamp  field of landmark
         */
        void SetTimestampParamL( const TDesC8& aParsedValue );
        /**
         * Sets the category field of landmark
         */
        void SetCategoryParamL( const TDesC8& aParsedValue );
        /*
         * Parses the date field of timestamp
         * @param in aParsedDate - the date field to be parsed
         * @param out aParsedDateBuffer - the buffer containing the parsed
         *                                date value
         * @return KErrNone if succefully parsed else KErrGeneral
         */
        TInt ParseDate( const TDesC8& aParsedDate ,HBufC& aParsedDateBuffer );
        /*
         * Parses the Time field of timestamp
         * @param in aParsedTime - the Time field to be parsed
         * @param out aParsedTimeBuffer - the buffer containing the parsed
         *                                Time value
         * @return KErrNone if succefully parsed else KErrGeneral
         */
        TInt ParseTime( const TDesC8& aParsedTime ,HBufC& aParsedTimeBuffer );
        /**
         * Resets the URL parameters to default values
         */
        void ResetUrlParameters();
              
    private:
        /**
         * A handle to the file server session
         */
        RFs iFs;
        /**
         * An instance of RFile used to open the file
         */
        RFile iFile;
        /**
         * A Handle used to open the file
         */
        RFile* iFileHandle;
        /**
         * A buffer to hold the input URL which is set by the SetInputBuffer method
         */
        TPtrC8 iInputBuffer;
        /**
         * Contains the size of the input buffer
         */
        TInt iContentSize;
        /**
         * A buffer to hold the input URL that is read from the file
         */
        HBufC8* iFileReadBuffer;
        /**
         * Enumeration used to specify the different states of parsing
         */
        enum TUrlParserState
            {
            EStateUnitialized = 0,
            EStateInitialized,
            EStateOperationCreated,
            EStateParsingPending,
            EStateParsingComplete,
            EStateParsed
            };
        /**
         * Enumeration to indicate which param is to be set
         */
        enum TLandmarkField
            {
            ELatitude = 0,
            ELongitude,
            ESpeed,
            EHeading,
            EName,
            EStreet,
            EHouseNumber,
            EPostalCode,
            ECity,
            EState,
            ECountry,
            ETelephone,
            EWebUrl,
            EDescription,
            EPlaceId
            };
        /**
         * TRealFormat variable used to format the real numbers such as
         *  latitude & longitude when converted to descriptor
         */
        TRealFormat iRealFormatCoordinates;
        /**
         * TRealFormat variable used to format the other real numbers 
         * when converted to descriptor
         */
        TRealFormat iRealFormat;

        /**
         * Indicates the state of the parser
         */
        TInt iParserStatus;
        /**
         * An instance of the CPosLmUrlParserOperation that is to be returned
         */
        CPosLmUrlParserOperation*  iUrlParserOp;
        /**
         * Holds the number of parsed landmarks
         */
        TUint32 iNumOfParsedLandmarks;
        /**
         * CPosLandmark instance that holds the parsed landmark content
         */
        CPosLandmark*  iParsedLandmark;
        /**
         * Holds the Url host extracted from the buffer
         */
        TPtrC8 iUrlHost;
        /**
         * Holds the URL location parameters extracted from the buffer
         */
        TPtrC8 iUrlLocation;
         /**
         * Holds the parsed latitude value in treal64
         */ 
        TReal64 iParsedLatitudeValue;
        /**
         * Holds the parsed longitude value in treal64
         */ 
        TReal64 iParsedLongitudeValue;
        /**
         * Holds the parsed altitude value in treal32
         */ 
        TReal32 iParsedAltitudeValue;
        /**
         * Holds the parsed horizontal accuracy value in treal32
         */ 
        TReal32 iParsedPositionAccuracyValue;
        /**
         * Holds the parsed vertical accuracy value in treal32
         */ 
        TReal32 iParsedAltitudeAccuracyValue;
        /**
         * Holds the category id for the parsed categories
         */
        TUint32 iParsedCategoryId;
        /**
         * An array containing the parsed categories
         */
        RPointerArray<CPosLandmarkCategory> iParsedCategories;
        /**
         * Indicates if the latitude param has been parsed or not
         */
        TBool iLatitudeParsed;
        /**
         * Indicates if the longitude param has been parsed or not
         */
        TBool iLongitudeParsed;        
        /**
         * Indicates if the altitude param has been parsed or not
         */
        TBool iAltitudeParsed;        
        /**
         * Indicates if the position accuracy param has been parsed or not
         */
        TBool iPositionAccuracyParsed;        
        /**
         * Indicates if the altitude accuracy param has been parsed or not
         */
        TBool iAltitudeAccuracyParsed;        
        /**
         * Indicates if the speed param has been parsed or not
         */
        TBool iSpeedParsed;        
        /**
         * Indicates if the heading param has been parsed or not
         */
        TBool iHeadingParsed;        
        /**
         * Indicates if the name param has been parsed or not
         */
        TBool iNameParsed;        
        /**
         * Indicates if the street param has been parsed or not
         */
        TBool iStreetParsed;        
        /**
         * Indicates if the house number param has been parsed or not
         */
        TBool iHouseNumberParsed;        
        /**
         * Indicates if the postal code param has been parsed or not
         */
        TBool iPostalCodeParsed;        
        /**
         * Indicates if the city param has been parsed or not
         */
        TBool iCityParsed;        
        /**
         * Indicates if the state param has been parsed or not
         */
        TBool iStateParsed;        
        /**
         * Indicates if the country param has been parsed or not
         */
        TBool iCountryParsed;        
        /**
         * Indicates if the telephone param has been parsed or not
         */
        TBool iTelephoneParsed;        
        /**
         * Indicates if the web url param has been parsed or not
         */
        TBool iWebUrlParsed;        
        /**
         * Indicates if the description param has been parsed or not
         */
        TBool iDescriptionParsed;        
        /**
         * Indicates if the place id param has been parsed or not
         */
        TBool iPlaceIdParsed;        
        /**
         * Indicates if the timestamp param has been parsed or not
         */
        TBool iTimeStampParsed;  
        /**
         * Seeks the position in the file while parsing in chunks
         */
        TInt iSeek;
        /**
         * Holds the chunk size to be parsed from the file
         */
        TInt iChunkSize;
        /**
         * Indicates whether the file has been parsed
         */
        TBool iFileparsed;
        /**
         * Indicates whether parameter delimeter is parsed
         */
        TBool iParamDelimiterParsed;
    };

#endif      // CPOSLMURLPARSER_H

// End of File
