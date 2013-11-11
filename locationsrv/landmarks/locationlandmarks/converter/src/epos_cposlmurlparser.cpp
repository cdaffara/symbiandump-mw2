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


#include    <lbsposition.h>
#include    <gulutil.h>
#include    <uri8.h>
#include    <escapeutils.h>
#include    <EPos_LandmarksErrors.h>
#include    "epos_cposlmurlparser.h"
#include    "EPos_CPosLmCategoryManager.h"
#include    "EPos_CPosLandmarkDatabase.h"
#include    "epos_cposlmurlparseroperation.h"
#include    "EPos_PosLmConverterUtils.h"
#include    "EPos_PosLmImplExtension.h"
#include    "EPos_CPosLandmarkCategory.h"


// Constants for formatting the Real numbers
const TInt KRealPoint = 46;
const TInt KRealDecimalPlaces = 2;
const TInt KCoordinateDecimalPlaces = 6;
// constants for checking the range of the coordinates
const TReal KMinLatitude = -90.0;
const TReal KMaxLatitude = 90.0;
const TReal KMinLongitude = -180.0;
const TReal KMaxLongitude = 180.0;
// constants that define the decode buffer length
const TInt KDecodeBufferLength = 3;
// constants defining the length for the date time buffers
const TInt KMaxDateTimeBufferLength = 22;
const TInt KMaxMicroSecondsLength = 6;
const TInt KMaxDateFieldLength = 8;
const TInt KMaxTimeFieldLength = 6;
// constants defining the length of the delimeters
const TInt KHostDelimeterLength = 3;
const TInt KQueryStartDelimeterLength = 2;
const TInt KLocationDelimeterLength = 2;
const TInt KParameterDelimeterLength = 1;
// constants defining length of chunk size
const TInt KMaxChunkSize = 5012;


// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CPosLmUrlParser::CPosLmUrlParser
// Default Constructor
//-----------------------------------------------------------------------------
//
CPosLmUrlParser::CPosLmUrlParser()
    {
    // initialize the TRealFormat variable for latitude & longitude formatting
    iRealFormatCoordinates.iPoint = KRealPoint;
    iRealFormatCoordinates.iType = KRealFormatFixed | KDoNotUseTriads;
    iRealFormatCoordinates.iPlaces = KCoordinateDecimalPlaces;
    iRealFormatCoordinates.iWidth = KRealWidth;
 
    // initialize the TRealFormat variable for formatting other real32 numbers
    iRealFormat.iPoint = KRealPoint;
    iRealFormat.iType = KRealFormatFixed | KDoNotUseTriads;
    iRealFormat.iPlaces = KRealDecimalPlaces;
    iRealFormat.iWidth = KRealWidth;
    
    ResetUrlParameters();
    }

// -----------------------------------------------------------------------------
// CPosLmUrlParser::NewL
// -----------------------------------------------------------------------------
//
CPosLmUrlParser* CPosLmUrlParser::NewL()
    {
    return  new (ELeave) CPosLmUrlParser;
    }
// -----------------------------------------------------------------------------
// CPosLmUrlParser::~CPosLmUrlParser 
// Destructor
//-----------------------------------------------------------------------------
//
CPosLmUrlParser::~CPosLmUrlParser()
    {
    ResetUrlParser();
    }

// -----------------------------------------------------------------------------
// CPosLmUrlParser::SetInputBuffer 
// 
//-----------------------------------------------------------------------------
//
void CPosLmUrlParser::SetInputBuffer( const TDesC8&  aBuffer )
    {
    // Reset the parser first,so as to start parsing freshly
    ResetUrlParser();
    // Set the input buffer
    iInputBuffer.Set( aBuffer );

    // Set size of input buffer
    iContentSize = iInputBuffer.Length();
    
    // Change parser status to EStateInitialized
    iParserStatus = EStateInitialized;
    
    }

// -----------------------------------------------------------------------------
// CPosLmUrlParser::SetInputFileL 
// 
//-----------------------------------------------------------------------------
//
void CPosLmUrlParser::SetInputFileL( const TDesC&  aFile )
    {
    // Reset the parser first,so as to start parsing freshly
    ResetUrlParser();
    // Connect to the file server
    User::LeaveIfError( iFs.Connect() );
    // Open the file in the read only mode
    User::LeaveIfError( iFile.Open( iFs, aFile,
            EFileShareReadersOnly | EFileRead ) );
    // Set the file handle
    iFileHandle = &iFile;
    // Set the file content size
    User::LeaveIfError( iFileHandle->Size( iContentSize ) );
    // Change parser status to EStateInitialized
    iParserStatus = EStateInitialized;
    
    
    }

// -----------------------------------------------------------------------------
// CPosLmUrlParser::SetInputFileHandleL 
// 
//-----------------------------------------------------------------------------
//
void CPosLmUrlParser::SetInputFileHandleL( RFile&  aFileHandle )
    {
    // Reset the parser first,so as to start parsing freshly
    ResetUrlParser();
    // set the file handle
    iFileHandle = &aFileHandle;
    // Set the file content size
    User::LeaveIfError( iFileHandle->Size( iContentSize ) );
    // Change parser status to EStateInitialized
    iParserStatus = EStateInitialized;
    
    }

// -----------------------------------------------------------------------------
// CPosLmUrlParser::ParseContentL 
// 
//-----------------------------------------------------------------------------
//
CPosLmOperation* CPosLmUrlParser::ParseContentL( TBool  aBuildIndex )
    {
    // Check that the parser status is EStateInitialized when this method is called,
    // if not panic with EPosLmProtocolBreak
    __ASSERT_ALWAYS( iParserStatus == EStateInitialized,
            Panic( KPosLandmarksClientPanic, EPosLmProtocolBreak ) );
    
    // Build index not supported ,hence leave with KErrNotSupported if specified
    if( aBuildIndex )
        {
        User::Leave( KErrNotSupported );
        }
    // Reset the url parameters
    ResetUrlParameters();
    
    // Delete any previous operation
    DisconnectUrlOperation();
    
    // Create a new instance of the operation class to be returned
    iUrlParserOp = CPosLmUrlParserOperation::NewL( *this );
    
    // change the parser status to EStateOperationCreated
    iParserStatus = EStateOperationCreated;
    
    return iUrlParserOp;
    }

// -----------------------------------------------------------------------------
// CPosLmUrlParser::NumOfParsedLandmarks 
// 
//-----------------------------------------------------------------------------
//
TUint32 CPosLmUrlParser::NumOfParsedLandmarks() const
    {
    return iNumOfParsedLandmarks;
    }

// -----------------------------------------------------------------------------
// CPosLmUrlParser::FirstCollectionDataId 
// 
//-----------------------------------------------------------------------------
//
TPosLmCollectionDataId CPosLmUrlParser::FirstCollectionDataId() const
    {
    // Do nothing,as collection data is not supported in url parsing
    return EPosLmCollDataNone;
    }

// -----------------------------------------------------------------------------
// CPosLmUrlParser::NextCollectionDataId 
// 
//-----------------------------------------------------------------------------
//
TPosLmCollectionDataId CPosLmUrlParser::NextCollectionDataId( 
                       TPosLmCollectionDataId  /*aCollectionDataId*/ ) const
    {
    // Do nothing,as collection data is not supported in url parsing
    return EPosLmCollDataNone;                   
    }

// -----------------------------------------------------------------------------
// CPosLmUrlParser::CollectionData 
// 
//-----------------------------------------------------------------------------
//
TPtrC CPosLmUrlParser::CollectionData( TPosLmCollectionDataId  /*aDataId*/ ) const
    {
    // Do nothing,as collection data is not supported in url parsing
    TPtrC nullDescriptor( KNullDesC );
    return nullDescriptor;
    }

// -----------------------------------------------------------------------------
// CPosLmUrlParser::LandmarkLC 
// 
//-----------------------------------------------------------------------------
//
CPosLandmark* CPosLmUrlParser::LandmarkLC( TUint aLandmarkIndex ) const
    {
    // Check that index passed always equal to KPosLastParsedLandmark
    // or less than the number of parsed landmarks but not negative
    __ASSERT_ALWAYS(
            aLandmarkIndex < iNumOfParsedLandmarks ||aLandmarkIndex == KPosLastParsedLandmark,
            Panic( KPosLandmarksClientPanic, EPosInvalidIndex ) );

    // Check if there are any parsed landmarks before returning the landmark,
    // else return KErrNotFound
    if( !iNumOfParsedLandmarks )
        {
        User::Leave( KErrNotFound );
        }
    
    return CPosLandmark::NewLC( *iParsedLandmark );
    }

// -----------------------------------------------------------------------------
// CPosLmUrlParser::LandmarkCategoryLC 
// 
//-----------------------------------------------------------------------------
//
CPosLandmarkCategory* CPosLmUrlParser::LandmarkCategoryLC( TPosLmItemId  
                                                           aCategoryId  ) const
    {
    // indicates whether the category with the id specified is found or not
    TBool found = EFalse;
    TInt i = 0;
    // check that there is a parsed landmark before getting any categories associated
    // with it,else leave with KErrNotFound
    if( !iNumOfParsedLandmarks )
        {
        User::Leave( KErrNotFound );
        }
    // check that categories exist for the parsed landmark,else leave with KErrNotFound
    if( !iParsedCategories.Count() )
        {
        User::Leave( KErrNotFound );
        }
    else
        {
        // Find the category with the id provided ,from the parsed categories,if not found 
        // leave with KErrNotFound
        for(  i=0; i<iParsedCategories.Count();i++ )  
            {
            if( iParsedCategories[i]->CategoryId() ==  aCategoryId )
                {
                found = ETrue;
                break;
                }
            }
        if( !found )
            {
            User::Leave( KErrNotFound );
            }
        }
    return CPosLandmarkCategory::NewLC( *iParsedCategories[i] );
    }

// -----------------------------------------------------------------------------
// CPosLmUrlParser::ResetUrlParser 
// 
//-----------------------------------------------------------------------------
//
void CPosLmUrlParser::ResetUrlParser()
    {
    // set input buffer to NULL
    iInputBuffer.Set( NULL, 0 );
    // if any filehandle exists,close it 
    if( iFileHandle )
        {
        iFileHandle = NULL;
        iFile.Close();
        iFs.Close();
        // delete the file buffer
        delete iFileReadBuffer;
        iFileReadBuffer = NULL;
        }
    // delete the previously parsed landmark
    if( iParsedLandmark )
        {
        delete iParsedLandmark;
        iParsedLandmark = NULL;
        }
    // reset the number of parsed landmarks to 0
    iNumOfParsedLandmarks = 0;
    // reset the categories array
    iParsedCategories.ResetAndDestroy();
    // Reset the parser status to EStateUnitialized
    iParserStatus = EStateUnitialized;
    
    }

// -----------------------------------------------------------------------------
// CPosLmUrlParser::DisconnectUrlOperation 
// -----------------------------------------------------------------------------
//
void CPosLmUrlParser::DisconnectUrlOperation()
    {
    if ( iUrlParserOp )
        {
        iUrlParserOp->DisconnectUrlParser();
        iUrlParserOp = NULL;
        }
    }

// -----------------------------------------------------------------------------
// CPosLmUrlParser::UrlOperationDestroyed
// -----------------------------------------------------------------------------
//
void CPosLmUrlParser::UrlOperationDestroyed()
    {
    iUrlParserOp = NULL;
    }

// -----------------------------------------------------------------------------
// CPosLmUrlParser::ParseUrlL
// -----------------------------------------------------------------------------
//
TInt CPosLmUrlParser::ParseUrlL()
    {
    TInt status = KPosLmOperationNotComplete;
    // Set the chunk size to be read from the file
    iChunkSize = iContentSize < KMaxChunkSize ? iContentSize : KMaxChunkSize;

    // Create instance of CPosLandmark to hold the parsed info
    iParsedLandmark = CPosLandmark::NewL();
    
    // Check on the parser status,if the status is EStateParsingPending
    // then continue with parsing the next location info
    if( iParserStatus == EStateParsingPending )
        {
        // Parse the next location info
        ParseUrlLocationL();
        // Now check the parsing status to confirm if its complete,if 
        // yes change the status to KErrNone
        if( iParserStatus == EStateParsingComplete )
            {
            status = KErrNone;
            }
        }
    else
        {
        // start parsing afresh by setting the input buffer/file
        // Check if the input is through a buffer or file
        if( iInputBuffer.Ptr() )
            {
            ParseBufferL( iInputBuffer );
            }
        else
            {
            // for file parsing read the file in parts & parse the chunks
            while(iSeek < iContentSize)
                {
                iFileReadBuffer = HBufC8::NewL( iChunkSize );
                TPtr8 fileReadBufferPtr = iFileReadBuffer->Des();
                User::LeaveIfError( iFileHandle->Read( iSeek, fileReadBufferPtr, iChunkSize ) );
                ParseBufferL( iFileReadBuffer->Des() );
                }
            }
        // Check on the parser status to set the status 
        if( iParserStatus == EStateParsingComplete )
            {
            status = KErrNone;
            }
        }
    return status;

    }

// -----------------------------------------------------------------------------
// CPosLmUrlParser::ParseBufferL
// -----------------------------------------------------------------------------
//
void CPosLmUrlParser::ParseBufferL( const TDesC8& aUrlBuffer )
    {
    // Check if the file parsing has already begun,if yes then parse
    // only the location params since the URL host is already parsed
    // in the beginning
    if( iSeek > 0 && iFileHandle )
        {
        iUrlLocation.Set( aUrlBuffer );
        // Parse & validate the URl location info
        ParseUrlLocationL();
        }
    else
        {
        // parse the buffer to find the query start delimeter
        TInt queryStartDelimeterPosition = aUrlBuffer.Find( KUrlQueryStartDelimeter );
        if( queryStartDelimeterPosition != KErrNotFound )
            {
            // Extract the url host 
            iUrlHost.Set( aUrlBuffer.Mid( 0, queryStartDelimeterPosition ) );
            // Increment the file seek position until the query start delimeter
            iSeek = iSeek + iUrlHost.Length()+ KQueryStartDelimeterLength;
            // Parse & validate the url host
            ParseUrlHostL();
            // Extract the Url location parameters
            iUrlLocation.Set( aUrlBuffer.Mid( queryStartDelimeterPosition+KQueryStartDelimeterLength, 
                              ( aUrlBuffer.Length() - 
                              ( queryStartDelimeterPosition+KQueryStartDelimeterLength ) ) ) );
            // Parse & validate the URl location info
            ParseUrlLocationL();
            }
        else
            {
            User::Leave( KErrPosLmUnknownFormat );
            }
        }
    }

// -----------------------------------------------------------------------------
// CPosLmUrlParser::ParseUrlHostL
// -----------------------------------------------------------------------------
//
void CPosLmUrlParser::ParseUrlHostL()
    {
    // Holds the extracted scheme portion of the URl
    TPtrC8 parsedUriScheme ;
    // Holds the extracted host name portion of the URL
    TPtrC8 parsedUriHost;
    // Find the Url host delimeter position
    TInt urlHostDelimeterPosition = iUrlHost.Find( KUrlHostDelimeter );
    // Check if the delimeter is found,if yes then validate the scheme
    // & host name with the allowed values,else only validate the hostname 
    if( urlHostDelimeterPosition != KErrNotFound )
        {
        // Extract the scheme part of the Url
        parsedUriScheme.Set( iUrlHost.Mid( 0, urlHostDelimeterPosition ) );
        // Extract the host name
        parsedUriHost.Set( iUrlHost.Mid( urlHostDelimeterPosition+KHostDelimeterLength,
                           iUrlHost.Length() - 
                           ( urlHostDelimeterPosition+KHostDelimeterLength ) ) );
        // validate the uri scheme by Comparing with the allowed values for Url Scheme, 
        // if it does not match then leave with KErrPosLmUnknownFormat
        if( ( parsedUriScheme.CompareC( KUrlSchemeHttp ) ) &&
            ( parsedUriScheme.CompareC( KUrlSchemeHttps ) ) )
            {
            User::Leave( KErrPosLmUnknownFormat );
            }
        // validate the uri host by Comparing with the allowed values for Url Host, 
        // if it does not match then leave with KErrPosLmUnknownFormat
        if( ( parsedUriHost.CompareC( KUrlHostFull ) ) &&
            ( parsedUriHost.CompareC( KUrlHost ) ) )
            {
            User::Leave( KErrPosLmUnknownFormat );
            }
        }
    else
        {
        // validate the uri host by Comparing with the allowed values for Url Host, 
        // if it does not match then leave with KErrPosLmUnknownFormat
        if( ( iUrlHost.CompareC( KUrlHostFull ) ) &&
            ( iUrlHost.CompareC( KUrlHost ) ) )
            {
            User::Leave( KErrPosLmUnknownFormat );
            }
        }
    }


// -----------------------------------------------------------------------------
// CPosLmUrlParser::ParseUrlLocationL
// -----------------------------------------------------------------------------
//
void CPosLmUrlParser::ParseUrlLocationL()
    {
    // Holds the extracted location info
    TPtrC8 parsedUrlLocation;
    // Get the location delimeter position
    TInt locationDelimeterPosition = iUrlLocation.Find( KUrlLocationDelimeter );
    // Continue parsing till the next delimeter is not found
    if( locationDelimeterPosition != KErrNotFound )
        {
        // Reset the URL parameters
        ResetUrlParameters();
        // Extract the location information for a single landmark
        parsedUrlLocation.Set( iUrlLocation.Mid( 0, locationDelimeterPosition ) );
        // Remove the extracted location from iUrlLocation
        iUrlLocation.Set( iUrlLocation.Mid( locationDelimeterPosition+KLocationDelimeterLength,  
                       ( iUrlLocation.Length() -
                       ( locationDelimeterPosition+KLocationDelimeterLength ) ) ) );
        // parse the url parameters for the extracted location
        ParseUrlParametersL( parsedUrlLocation );
        // Set parser status to EStateParsingPending since there could 
        // be more location info following the location delimeter
        // iParserStatus = EStateParsingPending;
        
        // Currently since multiple URL parsing is not supported,the rest
        // of the location info is ignored. Therefore,
        // set parser status to EStateParsingComplete so as to complete
        // the parsing for the first location info found
        SetParserStatusL( EStateParsingComplete );
        }
    else
        {
        // Check that there is another set of location info before parsing
        if( iUrlLocation != KNullDesC8 )
            {
            // Parse the url parameters for the location information
            ParseUrlParametersL( iUrlLocation );
            }
        else
            {
            iFileparsed = ETrue;
            }
        SetParserStatusL( EStateParsingComplete );
        }
    }

// -----------------------------------------------------------------------------
// CPosLmUrlParser::SetParserStatusL
// -----------------------------------------------------------------------------
//
void CPosLmUrlParser::SetParserStatusL( TInt aStatus )
    {
    // Check that there are no more chunks left before changing parser 
    // status to EStateParsingComplete incase of file parsing
    if( iFileHandle )
        {
        // 
        if( iSeek+iChunkSize >= iContentSize && iFileparsed )
            {
            // Set parser status to EStateParsingComplete since no more
            // location info present in the URL
            iParserStatus = aStatus;
            SetLocalityL();
            }
        }
    else
        {
        // Set parser status to EStateParsingComplete since no more
        // location info present in the URL
        iParserStatus = aStatus;
        SetLocalityL();
        }
    
    }

// -----------------------------------------------------------------------------
// CPosLmUrlParser::SetLocalityL
// -----------------------------------------------------------------------------
//
void CPosLmUrlParser::SetLocalityL()
    {
    // Set the coordinates for the parsed landmark.If the latitude & longitude 
    // cannot be set then leave with KErrPosLmUnknownFormat
    TLocality position( TCoordinate(iParsedLatitudeValue, iParsedLongitudeValue,iParsedAltitudeValue ),
              iParsedPositionAccuracyValue,iParsedAltitudeAccuracyValue );
    TRAPD( error, iParsedLandmark->SetPositionL( position ) );
    if( error == KErrArgument )
        {
        User::Leave( KErrPosLmUnknownFormat );
        }
    else
        {
        // increment the number of landmarks parsed
        ++iNumOfParsedLandmarks;
        }    
    }


// -----------------------------------------------------------------------------
// CPosLmUrlParser::ParseUrlParametersL
// -----------------------------------------------------------------------------
//
void CPosLmUrlParser::ParseUrlParametersL( const TDesC8& aUrlParameters )
    {
    // Holds the extracted parameter value pair
    TPtrC parsedUrlParameters;
    // Holds the next Url parameter value pairs
    TPtrC urlParameters ;
    TInt result = KErrNone;
    TInt column = 0;
    // Get the parameter delimeter position
    TInt parameterDelimeterPosition = aUrlParameters.Find( KUrlParameterDelimeter );
    // Holds the the url chunk in TDesC type
    HBufC *sourceUrl = HBufC::NewLC( aUrlParameters.Length() );
    sourceUrl->Des().Copy( aUrlParameters );
    //Check if the Param Delimiter exists in the URL
    if( parameterDelimeterPosition != KErrNotFound )
        {
        iParamDelimiterParsed= ETrue;
        while( result != KErrNotFound )
        	{
        	//Extract the location parameter from the url
        	result = TextUtils::ColumnText( parsedUrlParameters, column, sourceUrl, KParamSeperator );
            //Check if the input is from the file
            if( iFileHandle )
                {
                //Skip the last location parameter in the url, it should come with the next url chunk
                if( TextUtils::ColumnText( urlParameters, (column+1), sourceUrl, KParamSeperator )!= KErrNotFound )
                    {
                    //increase the seek with the length of the parsed param and delimeter length
                    iSeek = iSeek + parsedUrlParameters.Length()+ KParameterDelimeterLength;
                    //check if the parsed parameter is a valid tag value pair
                    if( parsedUrlParameters.Find( KUrlParameterValueDelimeter ) != KErrNotFound )
                        {
                        ParseParamValueL( parsedUrlParameters );
                        }
                    }
                //Check if the last param is ending with &
                else if( parameterDelimeterPosition == parsedUrlParameters.Length() )
                    {
                    ParseParamValueL( parsedUrlParameters );
                    iSeek = iSeek + parsedUrlParameters.Length()+ KParameterDelimeterLength;
                    // Check if it is the last chunk in the file
                    if( iSeek+iChunkSize >= iContentSize && !iFileparsed )
                        {
                        iFileparsed = ETrue;
                        }
                    }
                }
            //if input is from buffer
            else
                if ( parsedUrlParameters != KNullDesC )
                    {
                    ParseParamValueL( parsedUrlParameters );
                    }
        	column++;
        	}
        }
    else
        {
        // Check if the parsing is from file,if yes then check if there
        // are more chunks left
        if( iFileHandle )
            {
            if( iSeek+iChunkSize >= iContentSize && !iFileparsed )
                {
                // Parse the param values if any
                ParseParamValueL( *sourceUrl );
                iSeek = iSeek + sourceUrl->Length();
                iFileparsed = ETrue;
                }
            else
            	{
            	if( sourceUrl->Find( KUrlParameterValueDelimeter ) != KErrNotFound && 
            			( sourceUrl->Length() == iChunkSize ) )
					{
					ParseParamValueL( *sourceUrl );
					iSeek = iSeek + sourceUrl->Length()+ KParameterDelimeterLength; 
					}
            	else if( sourceUrl->Find( KUrlParameterValueDelimeter ) == KErrNotFound && ( iParamDelimiterParsed == EFalse ) )
            	    {
            	    iSeek = iSeek + sourceUrl->Length()+ KParameterDelimeterLength;
            	    }
            	}
            }
         }
    CleanupStack::PopAndDestroy( sourceUrl );
    }

// -----------------------------------------------------------------------------
// CPosLmUrlParser::ParseParamValueL
// -----------------------------------------------------------------------------
//
void CPosLmUrlParser::ParseParamValueL( const TDesC& aParamValue )
    {
    // holds the extracted  parameter name
    TPtrC paramName;
    // holds the extracted parameter value
    TPtrC paramValue;
    // Get the param value delimeter position
    TInt paramValueDelimeterPosition = aParamValue.Find( KUrlParameterValueDelimeter );
    // Check that the delimeter is found,else leave with KErrPosLmUnknownFormat
    if( paramValueDelimeterPosition != KErrNotFound )
        {
        // Extract the param name
        paramName.Set( aParamValue.Mid( 0, paramValueDelimeterPosition ) );
        // Extract the param value
        paramValue.Set( aParamValue.Mid( paramValueDelimeterPosition+KParameterDelimeterLength,
                       ( aParamValue.Length() -
                       ( paramValueDelimeterPosition+KParameterDelimeterLength ) ) ) );
        //Coverting the paramname to TDesC8
        HBufC8* paramName8 = HBufC8::NewLC( paramName.Length() );
        paramName8->Des().Copy( paramName );
        //Convering the paramValue to TDesC8
        HBufC8* paramValue8 = HBufC8::NewLC( paramValue.Length() );
        paramValue8->Des().Copy( paramValue );
        // Compare the param values
        CompareParamValueL( *paramName8,*paramValue8 );
        CleanupStack::PopAndDestroy( 2, paramName8 );
        }
    else
        {
        User::Leave( KErrPosLmUnknownFormat );
        }
    
    }

// -----------------------------------------------------------------------------
// CPosLmUrlParser::CompareParamValueL
// -----------------------------------------------------------------------------
//
void CPosLmUrlParser::CompareParamValueL( const TDesC8& aParam, const TDesC8& aValue )
    {
    iParamDelimiterParsed = EFalse;

    // Compare the param name with the possible parameter values
    if( !aParam.CompareC( KUrlLatitude ) )
        {
        // Check if the latitude param has already been parsed once,to avoid 
        // setting the values again incase of repetition of params
        if( !iLatitudeParsed )
            {
            // Indicates that the latitude has been parsed once
            iLatitudeParsed = ETrue;
            SetLatLongParamL( aValue, iParsedLatitudeValue, ELatitude );
            }
        }
    else if( !aParam.CompareC( KUrlLongitude ) )
            {
            // Check if the longitude param has already been parsed once,to avoid 
            // setting the values again incase of repetition of params
            if( !iLongitudeParsed )
                {
                // Indicates that the longitude has been parsed once
                iLongitudeParsed = ETrue;
                SetLatLongParamL( aValue, iParsedLongitudeValue, ELongitude );
                }
            }
    else if( !aParam.CompareC( KUrlAltitude ) )
            {
            // Check if the altitude param has already been parsed once,to avoid 
            // setting the values again incase of repetition of params
            if( !iAltitudeParsed )
                {
                // Indicates that the altitude has been parsed once
                iAltitudeParsed = ETrue;
                SetNumberParamL( aValue, iParsedAltitudeValue );
                }
            }
    else if( !aParam.CompareC( KUrlPositionAccuracy ) )
            {
            // Check if the position accuracy param has already been parsed once,to avoid 
            // setting the values again incase of repetition of params
            if( !iPositionAccuracyParsed )
                {
                // Indicates that the position accuracy has been parsed once
                iPositionAccuracyParsed = ETrue;
                SetNumberParamL( aValue, iParsedPositionAccuracyValue );
                }
            }
    else if( !aParam.CompareC( KUrlAltitudeAccuracy ) )
            {
            // Check if the altiude accuracy param has already been parsed once,to avoid 
            // setting the values again incase of repetition of params
            if( !iAltitudeAccuracyParsed )
                {
                // Indicates that the altitude accuracy has been parsed once
                iAltitudeAccuracyParsed = ETrue;
                SetNumberParamL( aValue, iParsedAltitudeAccuracyValue );
                }
            }
    else if( !aParam.CompareC( KUrlSpeed ) )
            {
            // Check if the speed param has already been parsed once,to avoid 
            // setting the values again incase of repetition of params
            if( !iSpeedParsed )
                {
                // Indicates that the speed has been parsed once
                iSpeedParsed = ETrue;
                SetPositionFieldNumberParamL( aValue, ESpeed );
                }
            }
    else if( !aParam.CompareC( KUrlHeading ) )
            {
            // Check if the heading param has already been parsed once,to avoid 
            // setting the values again incase of repetition of params
            if( !iHeadingParsed )
                {
                // Indicates that the heading has been parsed once
                iHeadingParsed = ETrue;
                SetPositionFieldNumberParamL( aValue, EHeading );
                }
            }
    else if ( !aParam.CompareC( KUrlName ) )
            {
            // Check if the name param has already been parsed once,to avoid 
            // setting the values again incase of repetition of params
            if( !iNameParsed )
                {
                // Indicates that the name has been parsed once
                iNameParsed = ETrue;
                SetTextParamL( aValue, KPosLmMaxTextFieldLength, EName );
                }
            }
    else if ( !aParam.CompareC( KUrlStreet ) )
            {
            // Check if the street param has already been parsed once,to avoid 
            // setting the values again incase of repetition of params
            if( !iStreetParsed )
                {
                // Indicates that the street has been parsed once
                iStreetParsed = ETrue;
                SetTextParamL( aValue, KPosLmMaxTextFieldLength, EStreet );
                }
            }
    else if ( !aParam.CompareC( KUrlHouseNumber ) )
            {
            // Check if the housenumber param has already been parsed once,to avoid 
            // setting the values again incase of repetition of params
            if( !iHouseNumberParsed )
                {
                // Indicates that the housenumber has been parsed once
                iHouseNumberParsed = ETrue;
                SetTextParamL( aValue, KPosLmMaxTextFieldLength, EHouseNumber );
                }
            }
    else if ( !aParam.CompareC( KUrlPostalCode ) )
            {
            // Check if the PostalCode param has already been parsed once,to avoid 
            // setting the values again incase of repetition of params
            if( !iPostalCodeParsed )
                {
                // Indicates that the PostalCode has been parsed once
                iPostalCodeParsed = ETrue;
                SetTextParamL( aValue, KPosLmMaxTextFieldLength, EPostalCode );
                }
            }
    else if ( !aParam.CompareC( KUrlCity ) )
            {
            // Check if the city param has already been parsed once,to avoid 
            // setting the values again incase of repetition of params
            if( !iCityParsed )
                {
                // Indicates that the city has been parsed once
                iCityParsed = ETrue;
                SetTextParamL( aValue, KPosLmMaxTextFieldLength, ECity );
                }
            }
    else if ( !aParam.CompareC( KUrlState ) )
            {
            // Check if the State param has already been parsed once,to avoid 
            // setting the values again incase of repetition of params
            if( !iStateParsed )
                {
                // Indicates that the State has been parsed once
                iStateParsed = ETrue;
                SetTextParamL( aValue, KPosLmMaxTextFieldLength, EState );
                }
            }
    else if ( !aParam.CompareC( KUrlCountry ) )
            {
            // Check if the Country param has already been parsed once,to avoid 
            // setting the values again incase of repetition of params
            if( !iCountryParsed )
                {
                // Indicates that the Country has been parsed once
                iCountryParsed = ETrue;
                SetTextParamL( aValue, KPosLmMaxTextFieldLength, ECountry );
                }
            }
    else if ( !aParam.CompareC( KUrlTelephone ) )
            {
            // Check if the Telephone param has already been parsed once,to avoid 
            // setting the values again incase of repetition of params
            if( !iTelephoneParsed )
                {
                // Indicates that the Telephone has been parsed once
                iTelephoneParsed = ETrue;
                SetTextParamL( aValue, KPosLmMaxTextFieldLength, ETelephone );
                }
            }
    else if ( !aParam.CompareC( KUrlWebUrl ) )
            {
            // Check if the WebUrl param has already been parsed once,to avoid 
            // setting the values again incase of repetition of params
            if( !iWebUrlParsed )
                {
                // Indicates that the WebUrl has been parsed once
                iWebUrlParsed = ETrue;
                SetTextParamL( aValue, KPosLmMaxTextFieldLength, EWebUrl );
                }
            }
    else if ( !aParam.CompareC( KUrlDescription ) )
            {
            // Check if the Description param has already been parsed once,to avoid 
            // setting the values again incase of repetition of params
            if( !iDescriptionParsed )
                {
                // Indicates that the Description has been parsed once
                iDescriptionParsed = ETrue;
                SetTextParamL( aValue, KPosLmMaxDescriptionLength, EDescription );
                }
            }
    else if ( !aParam.CompareC( KUrlPlaceId ) )
            {
            // Check if the PlaceId param has already been parsed once,to avoid 
            // setting the values again incase of repetition of params
            if( !iPlaceIdParsed )
                {
                // Indicates that the PlaceId has been parsed once
                iPlaceIdParsed = ETrue;
                SetTextParamL( aValue, KPosLmMaxTextFieldLength, EPlaceId );
                }
            }
    else if ( !aParam.CompareC( KUrlTimestamp ) )
            {
            SetTimestampParamL( aValue );
            }
    else if ( !aParam.CompareC( KUrlCategory ) )
            {
            SetCategoryParamL( aValue );
            }
    }


// -----------------------------------------------------------------------------
// CPosLmUrlParser::SetLatLongParamL
// -----------------------------------------------------------------------------
//
void CPosLmUrlParser::SetLatLongParamL( const TDesC8& aParsedValue, TReal64& aValue, 
                                        TInt aField  )
    {
    // holds the parsed number in text form
    TBuf<KRealWidth> parsedNumber;
    // Check that value exists,else leave with KErrPosLmUnknownFormat since the
    // latitude & longitude field is mandatory
    if( aParsedValue == KNullDesC8 )
        {
        User::Leave( KErrPosLmUnknownFormat );
        }
    // Convert the descriptor obtained into real and check if the value is 
    // indeed real.
    TRAPD( error, PosLmConverterUtils::DesToRealL( aParsedValue, aValue ) );
    if( error != KErrNone )
        {
        User::Leave( KErrPosLmUnknownFormat );
        }
    TInt length = parsedNumber.Num( aValue, iRealFormatCoordinates );
    if( !length )
        {
        User::Leave( length );
        }
    TRAPD( err, PosLmConverterUtils::DesToRealL( parsedNumber, aValue ) );
    // check the value with the allowed range for latitude & longitude
    switch( aField )
        {
        case ELatitude :
             {
             if( err || !( aValue >= KMinLatitude 
                 && aValue <= KMaxLatitude ) ) 
                 {
                 User::Leave( KErrPosLmUnknownFormat );
                 }
             break;
             }
        case ELongitude :
             {
             if( err || !( aValue >= KMinLongitude 
                 && aValue <= KMaxLongitude ) ) 
                 {
                 User::Leave( KErrPosLmUnknownFormat );
                 }
             break;
             }
        default :
            {
            break;
            }
        }
    }


// -----------------------------------------------------------------------------
// CPosLmUrlParser::SetNumberParamL
// -----------------------------------------------------------------------------
//
void CPosLmUrlParser::SetNumberParamL( const TDesC8& aParsedValue, TReal32& aValue )
    {
    // holds the parsed number in text form
    TBuf<KRealWidth> parsedValue;
    // Check that value exists
    if( aParsedValue != KNullDesC8 )
        {
        // Convert the descriptor obtained into real and check if the value is 
        // indeed real.
        TRAPD( error, PosLmConverterUtils::DesToRealL( aParsedValue, aValue ) );
        if( !error )
            {
            TInt length = parsedValue.Num( aValue, iRealFormat );
            if( length != KErrGeneral )
                {
                TRAPD( error, PosLmConverterUtils::DesToRealL( parsedValue, aValue ) );
                if( error ) 
                    {
                    PosLmConverterUtils::SetNaN( aValue );
                    }
                 }
            else
                {
                PosLmConverterUtils::SetNaN( aValue );
                }  
            }
        else
            {
            PosLmConverterUtils::SetNaN( aValue );
            }
        }        
    }

// -----------------------------------------------------------------------------
// CPosLmUrlParser::SetPositionFieldNumberParamL
// -----------------------------------------------------------------------------
//
void CPosLmUrlParser::SetPositionFieldNumberParamL( const TDesC8& aParsedValue, TInt aField  )
    {
    // holds the parsed number in text form
    TBuf<KRealWidth> parsedValue;
    // holds the number value in real32
    TReal32 number;
    // Check that value exists
    if( aParsedValue != KNullDesC8 )
        {
        // Convert the descriptor obtained into real and check if the value is 
        // indeed real.
        TRAPD( error, PosLmConverterUtils::DesToRealL( aParsedValue, number ) );
        if( !error )
            {
            TInt length = parsedValue.Num( number, iRealFormat );
            if( length != KErrGeneral )
                {
                // set the position field for apped or heading depending 
                // on the field value that is passed
                switch( aField )
                    {
                    case ESpeed :
                        {
                        TInt error = KErrNone;
                        TRAP( error, iParsedLandmark->SetPositionFieldL( 
                               EPositionFieldHorizontalSpeed, parsedValue ) );
                        break;
                        }
                    case EHeading :
                        {
                        TInt err = KErrNone;
                        TRAP( err, iParsedLandmark->SetPositionFieldL( 
                                EPositionFieldHeading, parsedValue ) );
                        break;
                        }
                    default :
                        {
                        break;
                        }
                    }
                }
            }
        }
    }


// -----------------------------------------------------------------------------
// CPosLmUrlParser::SetTextParamL
// -----------------------------------------------------------------------------
// 
void CPosLmUrlParser::SetTextParamL( const TDesC8& aParsedValue, const TInt aMaxParseValueLength,
                                     TInt aField )
    {
    // parse the text value
    HBufC* parsedText = ParseTextL( aParsedValue, aMaxParseValueLength );
    CleanupStack::PushL( parsedText );
    // switch on the field & set the corresponding field values
    switch( aField )
        {
        case EName :
            {
            iParsedLandmark->SetLandmarkNameL( *parsedText );
            break;
            }
        case EStreet :
            {
            iParsedLandmark->SetPositionFieldL( EPositionFieldStreet, *parsedText );
            break;
            }
        case EHouseNumber :
            {
            iParsedLandmark->SetPositionFieldL( EPositionFieldStreetExtension, *parsedText );            
            break;
            }
        case EPostalCode :
            {
            iParsedLandmark->SetPositionFieldL( EPositionFieldPostalCode, *parsedText );
            break;
            }
        case ECity :
            {
            iParsedLandmark->SetPositionFieldL( EPositionFieldCity, *parsedText );
            break;
            }
        case EState :
            {
            iParsedLandmark->SetPositionFieldL( EPositionFieldState, *parsedText );
            break;
            }
        case ECountry :
            {
            iParsedLandmark->SetPositionFieldL( EPositionFieldCountry, *parsedText );
            break;
            }
        case ETelephone :
            {
            iParsedLandmark->SetPositionFieldL( EPositionFieldBuildingTelephone, *parsedText );
            break;
            }
        case EWebUrl :
            {
            iParsedLandmark->SetPositionFieldL( EPositionFieldMediaLinksStart, *parsedText );
            break;
            }
        case EDescription :
            {
            iParsedLandmark->SetLandmarkDescriptionL( *parsedText );
            break;
            }
        case EPlaceId :
            {
            iParsedLandmark->SetPlaceIdL( *parsedText );
            break;
            }
        default :
            {
            break;
            }
        }
    CleanupStack::PopAndDestroy( parsedText );
    }

// -----------------------------------------------------------------------------
// CPosLmUrlParser::ParseTextL
// -----------------------------------------------------------------------------
//
HBufC* CPosLmUrlParser::ParseTextL( const TDesC8& aParseValue, const TInt aMaxParseValueLength )
    {
    // use a TLex instance to parse the text
    TLex8 string( aParseValue );
    // Holds the parsed text value that is returned
    HBufC* parsedText = HBufC::NewL( aMaxParseValueLength );
    CleanupStack::PushL( parsedText );

    // Extract each character & validate to check if it is alphadigit,if not then
    // check for the encoded triplet & decode it 
    while( !string.Eos() )
        {
        // Check that the length of the parsed string is not beyond 
        // the max allowed length,if yes then stop parsing.
        if( parsedText->Length() == aMaxParseValueLength )
            {
            break;
            }
        TChar character = string.Get();

        // Check if character is alpha digit
        if( !character.IsAlphaDigit() )
            {
            // check for the url escape character
            if(  character == '%' )
                {
                // Holds the string to be decoded
                HBufC* decodeBuffer = HBufC::NewL( KDecodeBufferLength );
                CleanupStack::PushL( decodeBuffer );
                // holds the decoded data
                HBufC* decodedBuffer =NULL;
                // extract the next 2 characters that follows the escape character
                decodeBuffer->Des().Append( character );
                decodeBuffer->Des().Append( string.Get() );
                decodeBuffer->Des().Append( string.Get() );
                // decode the escaped sequence and append to parsed buufer if no errors
                 TRAPD( error, decodedBuffer = 
                       EscapeUtils::EscapeDecodeL( decodeBuffer->Des() );
                        );
                if( !error )
                    {
                    parsedText->Des().Append( decodedBuffer->Des() );
                    CleanupStack::PopAndDestroy(  decodeBuffer );
                    delete decodedBuffer;
                    }
                else
                    {
                    User::Leave( KErrPosLmUnknownFormat );
                    }
                }
            else
                {
                // stop the parsing if any other special character 
                // is encountered.
                break;
                }
            
            }
        else
            {
            // append the aplha digit character to parsed buffer
            parsedText->Des().Append( character );
            }
        }
    
    CleanupStack::Pop( parsedText );
    
    return parsedText;
    }


// -----------------------------------------------------------------------------
// CPosLmUrlParser::SetTimestampParamL
// -----------------------------------------------------------------------------
//
void CPosLmUrlParser::SetTimestampParamL( const TDesC8& aParsedValue )
    {
    // Check if the timeStamp param has already been parsed once,to avoid 
    // setting the values again incase of repetition of params
    if( !iTimeStampParsed )
        {
        // Indicates that the TimeStamp has been parsed once
        iTimeStampParsed = ETrue;
        // holds the extracted date
        TPtrC8 parsedDate;
        // holds the extracted full time including microseconds
        TPtrC8 parsedFullTime;
        // hold only the time part
        TPtrC8 parsedTime;
        // holds the extracted microseconds field
        TPtrC8 parsedMicroseconds;
        // Indicates if any error during parsing of date field
        TInt dateError = KErrNone;
        // Indicates if any error during parsing of time field
        TInt timeError = KErrNone;
        // Indicates if any error during parsing of microseconds field
        TInt microsecondsError = KErrNone;
        // holds the parsed date field
        HBufC* parsedDateBuffer = HBufC::NewL( KMaxDateFieldLength );
        CleanupStack::PushL( parsedDateBuffer );
        // holds the parsed time field
        HBufC* parsedTimeBuffer = HBufC::NewL( KMaxTimeFieldLength );
        CleanupStack::PushL( parsedTimeBuffer );
        // holds the parsed microseconds field
        HBufC* parsedMicrosecondsBuffer = HBufC::NewL( KMaxMicroSecondsLength );
        CleanupStack::PushL( parsedMicrosecondsBuffer );
        // holds the parsed date time 
        HBufC* parsedDateTime = HBufC::NewL( KMaxDateTimeBufferLength );
        CleanupStack::PushL( parsedDateTime );
        
        // Find the date time delimeter
        TInt dateTimeDelimeterPosition = aParsedValue.Find( KUrlDateTimeDelimeter );
        if( dateTimeDelimeterPosition != KErrNotFound )
            {
            parsedDate.Set( aParsedValue.Mid( 0, dateTimeDelimeterPosition ) );
            parsedFullTime.Set( aParsedValue.Mid( dateTimeDelimeterPosition+1,
                          ( aParsedValue.Length() -( dateTimeDelimeterPosition+1 ) ) ) );
            // Parse the date Field
            dateError = ParseDate( parsedDate, *parsedDateBuffer );
            
            // Check that there is no error during date parsing
            if( !dateError )
                {
                // Find the next time delimeter position
                TInt timeDelimeterPosition = parsedFullTime.Find( KUrlTimeDelimeter );
                if( timeDelimeterPosition != KErrNotFound )
                    {
                    // Extract the time & microseconds fields separately & parse them
                    parsedTime.Set( parsedFullTime.Mid( 0, timeDelimeterPosition ) );
                    parsedMicroseconds.Set( parsedFullTime.Mid( timeDelimeterPosition+1, 
                            parsedFullTime.Length() - ( timeDelimeterPosition+1 ) ) );
                    // parse the time field
                    timeError = ParseTime( parsedTime, *parsedTimeBuffer );
                    // Check that there is no error during time parsing
                    if( !timeError )
                        {
                        // parse  the value of microseconds upto 6 digits
                        TLex8 microsecondsString( parsedMicroseconds );
                        while( !microsecondsString.Eos() && ( parsedMicrosecondsBuffer->Length() 
                                                         <= KMaxMicroSecondsLength ) )
                            {
                            // get each character & check if it is a digit 
                            TChar microsecondCharacter = microsecondsString.Get();
                            if( microsecondCharacter.IsDigit() )
                                 {
                                 parsedMicrosecondsBuffer->Des().Append( microsecondCharacter );
                                 }
                             else
                                 {
                                 microsecondsError = KErrGeneral;
                                 break;
                                 }               
                            }
                        }
                    }
                
                else
                    {
                    timeError = ParseTime( parsedFullTime, *parsedTimeBuffer );
                    }
                }
            }
        else
            {
            dateError = ParseDate( aParsedValue, *parsedDateBuffer );
            }
        
        // Check if there are any errors during the date parsing before appending
        // to buffer and setting the timestamp
        if( !dateError )
            {
            parsedDateTime->Des().Append( parsedDateBuffer->Des() );
            // Check if there are any errors in time parsing before appending
            if( !timeError )
                {
                // Check that the time field exists before appending to buffer
                if( parsedTimeBuffer->Des() != KNullDesC16 )
                    {
                    parsedDateTime->Des().Append( KDateTimeSeparator );
                    parsedDateTime->Des().Append( parsedTimeBuffer->Des() );
                    // Check if there are no errors inmicroseconds parsing before
                    // appending
                    if( !microsecondsError )
                        {
                        // Check that the microseconds field exists before
                        // appending to the buffer
                        if( parsedMicrosecondsBuffer->Des() != KNullDesC16 )
                            {
                            parsedDateTime->Des().Append( KTimeMicroSecondSeparator );
                            parsedDateTime->Des().Append( parsedMicrosecondsBuffer->Des() );
                            }
                        }
                    }
                }
            // convert the parsed date time buffer into a TTime object & set the timestamp 
            // for the parsed landmark
            TTime timestamp;
            TInt error = timestamp.Set( parsedDateTime->Des() );
            // Check that there is no error returned while setting the time
            if ( !error )
                {
                TDateTime dateTime = timestamp.DateTime();
                // Check that the year & day fields are not zero to avoid 
                // SetTimeStampL from leaving with KErrArgument
                if( dateTime.Year() && dateTime.Day() )
                    {
                    iParsedLandmark->SetTimeStampL( timestamp );
                    }
                }
            }
        // delete all items on cleanupstack
        CleanupStack::PopAndDestroy( 4, parsedDateBuffer );
        }
    }

// -----------------------------------------------------------------------------
// CPosLmUrlParser::SetCategoryParamL
// -----------------------------------------------------------------------------
//
void CPosLmUrlParser::SetCategoryParamL( const TDesC8& aParsedValue )
    {
    TBool categoryPresent = EFalse;
    // parse the category name
    HBufC* parsedCategoryValue = ParseTextL( aParsedValue, KPosLmMaxCategoryNameLength );
    CleanupStack::PushL( parsedCategoryValue );

    // Check that the category name exists before creating a category instance
    if( *parsedCategoryValue != KNullDesC )
        {
        // Check that there are categories present in the parsed category array
        if ( !iParsedCategories.Count() )
            {
            categoryPresent = EFalse;
            }
        // Check that the parsed category name is not already present,create
        // category only with unique names
        for( TInt i=0;i<iParsedCategories.Count();i++ )
            {
            TPtrC existingCategoryName;
            // Get category name of each of the parsed categories
            if( !( iParsedCategories[i]->GetCategoryName( existingCategoryName ) ) )
                {
                // Compare the existing category name with the newly parsed category
                // name
                if( !parsedCategoryValue->Des().CompareC( existingCategoryName ) ) 
                    {
                    categoryPresent = ETrue;
                    }
                else
                    {
                    categoryPresent = EFalse;
                    }
                }
            }
        // if the category is not present then create the category instance
        if( !categoryPresent )
            {
            // Create a category instance
            CPosLandmarkCategory* parsedCategory = CPosLandmarkCategory::NewL();
            CleanupStack::PushL( parsedCategory );
            // Check that category id is not equal to any of the reserved global
            // category ids.If so then increment the id before setting it.
            if( iParsedCategoryId%3000 == 0 )
                {
                iParsedCategoryId++;
                }
            // set the category id for the parsed categories
            PosLmImplExtension::SetCategoryIdL( *parsedCategory, iParsedCategoryId++ );
            
            // set the category name that has been parsed
            TPtrC categoryName;
            categoryName.Set( *parsedCategoryValue );
            parsedCategory->SetCategoryNameL( categoryName );
            // Add the category to the parsed landmark
            iParsedLandmark->AddCategoryL( parsedCategory->CategoryId() );
            User::LeaveIfError( iParsedCategories.Append( CPosLandmarkCategory::NewL(
                                                          *parsedCategory ) ) );
            CleanupStack::PopAndDestroy( parsedCategory );
            }
        }
   CleanupStack::PopAndDestroy( parsedCategoryValue );
   }

// -----------------------------------------------------------------------------
// CPosLmUrlParser::ParseDate
// -----------------------------------------------------------------------------
//
TInt CPosLmUrlParser::ParseDate( const TDesC8& aParsedDate , 
                                 HBufC& aParsedDateBuffer )
    {
    TInt error = KErrNone;
    // parse the date field
    if( aParsedDate.Length() != KMaxDateFieldLength )
        {
        error = KErrGeneral;
        }
    else
        {
        TLex8 dateString( aParsedDate );
        while ( !dateString.Eos() )
            {
            TChar character = dateString.Get();
            if( character.IsDigit() )
                {
                aParsedDateBuffer.Des().Append( character );
                }
            else
                {
                error = KErrGeneral;
                break;
                }
            }
        }
    return error;    
    }
    
// -----------------------------------------------------------------------------
// CPosLmUrlParser::ParseTime
// -----------------------------------------------------------------------------
//
TInt CPosLmUrlParser::ParseTime( const TDesC8& aParsedTime , 
                                 HBufC& aParsedTimeBuffer )
    {
    TInt error = KErrNone;
    // parse the date field
    if( aParsedTime.Length() != KMaxTimeFieldLength )
        {
        error = KErrGeneral;
        }
    else
        {
        TLex8 timeString( aParsedTime );
        while ( !timeString.Eos() )
            {
            TChar character = timeString.Get();
            if( character.IsDigit() )
                {
                aParsedTimeBuffer.Des().Append( character );
                }
            else
                {
                error = KErrGeneral;
                break;
                }
            }
        }
    return error;    
    }

// -----------------------------------------------------------------------------
// CPosLmUrlParser::ResetUrlParameters
// -----------------------------------------------------------------------------
//
void CPosLmUrlParser::ResetUrlParameters()
    {
    // Initialize the coordinate values to NaN
    PosLmConverterUtils::SetNaN( iParsedLatitudeValue );
    PosLmConverterUtils::SetNaN( iParsedLongitudeValue );
    PosLmConverterUtils::SetNaN( iParsedAltitudeValue );
    PosLmConverterUtils::SetNaN( iParsedPositionAccuracyValue );
    PosLmConverterUtils::SetNaN( iParsedAltitudeAccuracyValue );
    // Initialize all the parameters parsed value to be EFalse
    // during the start of parsing,indicating that the params have
    // not been parsed yet.
    iLatitudeParsed = EFalse;
    iLongitudeParsed = EFalse;
    iAltitudeParsed = EFalse;
    iPositionAccuracyParsed = EFalse;
    iAltitudeAccuracyParsed = EFalse;
    iSpeedParsed = EFalse;
    iHeadingParsed = EFalse;
    iNameParsed = EFalse;
    iStreetParsed = EFalse;
    iHouseNumberParsed = EFalse;
    iPostalCodeParsed = EFalse;
    iCityParsed = EFalse;
    iStateParsed = EFalse;
    iCountryParsed = EFalse;
    iTelephoneParsed = EFalse;
    iWebUrlParsed = EFalse;
    iDescriptionParsed = EFalse;
    iPlaceIdParsed = EFalse;
    iTimeStampParsed = EFalse;
    // initializing the category id 
    iParsedCategoryId = 1;
    // Reset the parsed categories array
    iParsedCategories.Reset();
    iSeek = 0;
    iFileparsed = EFalse;
    iParamDelimiterParsed = ETrue;
    }
    
