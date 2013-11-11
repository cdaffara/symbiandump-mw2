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
* Description: Class used for encoding landmark content into an URL.
*
*
*/


#include    <lbsposition.h>
#include    <uri8.h>
#include    <EPos_LandmarksErrors.h>
#include    "epos_cposlmurlencoder.h"
#include    "EPos_CPosLmFileOutput.h"
#include    "EPos_CPosLmBufferOutput.h"
#include    "epos_cposlmurlencoderoperation.h"
#include    "EPos_PosLmConverterUtils.h"
#include    "EPos_CPosLmCategoryManager.h"
#include    "EPos_CPosLandmarkDatabase.h"


const TInt KPosBufferExpandSize = 600;
const TInt KRealPoint = 46;
const TInt KRealDecimalPlaces = 2;
const TInt KCoordinateDecimalPlaces = 6;
const TInt KSpecialCharacterAsciiLowerLimit1 = 32;
const TInt KSpecialCharacterAsciiUpperLimit1 = 47;
const TInt KSpecialCharacterAsciiLowerLimit2 = 58;
const TInt KSpecialCharacterAsciiUpperLimit2 = 64;
const TInt KSpecialCharacterAsciiLowerLimit3 = 91;
const TInt KSpecialCharacterAsciiUpperLimit3 = 96;
const TInt KSpecialCharacterAsciiLowerLimit4 = 123;
const TInt KSpecialCharacterAsciiUpperLimit4 = 126;


// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CPosLmUrlEncoder::CPosLmUrlEncoder
// Default Constructor
//-----------------------------------------------------------------------------
//
CPosLmUrlEncoder::CPosLmUrlEncoder()
    {
    }

// -----------------------------------------------------------------------------
// CPosLmUrlEncoder::NewL
// -----------------------------------------------------------------------------
//
CPosLmUrlEncoder* CPosLmUrlEncoder::NewL()
    {
    return new (ELeave) CPosLmUrlEncoder;
    }
// -----------------------------------------------------------------------------
// CPosLmUrlEncoder::~CPosLmUrlEncoder 
// Destructor
//-----------------------------------------------------------------------------
//
CPosLmUrlEncoder::~CPosLmUrlEncoder()
    {
    delete iOutput;
    iOutput = NULL;
    
    delete iLastLandmark;
    iLastLandmark = NULL;
    
    iCategories.ResetAndDestroy();
    }

//-----------------------------------------------------------------------------
// CPosLmUrlEncoder::SetUseOutputBufferL
//-----------------------------------------------------------------------------
//
CBufBase* CPosLmUrlEncoder::SetUseOutputBufferL()
    {
    // The encoder is reset before starting a new encoding process
    ResetEncoderL();
    // create an instance of the CPosLmBufferOutput 
    CPosLmBufferOutput* output = CPosLmBufferOutput::NewL(KPosBufferExpandSize);
    CleanupStack::PushL( output );
    // Assign the buffer output instance to the member variable
    iOutput = output;
       
    // Create the buffer into which the encoded output will be written
    CBufBase* buffer = output->BufferLC();
      
    // Initialize the encoding by writing the Url scheme & host
    InitializeEncodingL();

    CleanupStack::Pop( 2,output );
   
    return buffer;
    }

// -----------------------------------------------------------------------------
// CPosLmUrlEncoder::SetOutputFileL
// -----------------------------------------------------------------------------
//
void CPosLmUrlEncoder::SetOutputFileL(
    const TDesC& aOutputFile)
    {
    // The encoder is reset before starting a new encoding process
    ResetEncoderL();
    // Create an instance of the CPosLmFileOutput
    iOutput = CPosLmFileOutput::NewL(aOutputFile);
    // Initialize the encoding by writing the Url scheme & host
    InitializeEncodingL();
    }

// -----------------------------------------------------------------------------
// CPosLmUrlEncoder::AddCollectionDataL
// -----------------------------------------------------------------------------
//
void CPosLmUrlEncoder::AddCollectionDataL(
    TPosLmCollectionDataId /*aDataId*/,
    const TDesC& /*aCollectionData*/)
    {
    // Do Nothing,as URl format does not support collection data
    }

// -----------------------------------------------------------------------------
// CPosLmUrlEncoder::AddLandmarkL
// -----------------------------------------------------------------------------
//
void CPosLmUrlEncoder::AddLandmarkL( const CPosLandmark& aLandmark )
    {
    // Check that the encoder status is EPosInitialized when this method is called,
    // if not panic with EPosLmProtocolBreak
    __ASSERT_ALWAYS( iEncoderStatus == EPosInitialized,
            Panic( KPosLandmarksClientPanic, EPosLmProtocolBreak ) );
    
    // Check if landmark already exists,if yes then return KErrNotSupported
    // as multiple landmarks encoding is not supported.
    if ( iLastLandmark )
        {
        User::Leave( KErrNotSupported );
        }
    else
        {
        iLastLandmark = CPosLandmark::NewL( aLandmark );
        }
    
    // Encode the landmark
    EncodeLandmarkL();
    
    // Set encoder status after landamrk is added
    iEncoderStatus = EPosLmInfoAdded;
    }

// -----------------------------------------------------------------------------
// CPosLmUrlEncoder::AddCategoryForLatestLandmarkL
// -----------------------------------------------------------------------------
//
void CPosLmUrlEncoder::AddCategoryForLatestLandmarkL( const CPosLandmarkCategory& aCategory )
    {
    // Check that landmark is already added before adding a category,if not panic
    // with EPosLmProtocolBreak
    __ASSERT_ALWAYS(
            iEncoderStatus == EPosLmInfoAdded,
            Panic(KPosLandmarksClientPanic, EPosLmProtocolBreak));
    // Append the category information to the category array associated 
    // with the landmark to be encoded
    
    User::LeaveIfError( iCategories.Append( CPosLandmarkCategory::NewL( aCategory ) ) );
    
    
    // Encode the category and write it to the encoded output
    EncodeCategoryL();
      
    
    }

// -----------------------------------------------------------------------------
// CPosLmUrlEncoder::InitializeEncodingL
// -----------------------------------------------------------------------------
//
void CPosLmUrlEncoder::InitializeEncodingL()
    {
    // Write the URL scheme
    WriteUrlSchemeL();
    // Write the URL hostname
    WriteUrlHostL();
    
    // Set encoder status to  EPosInitialized
    iEncoderStatus = EPosInitialized;
    }

// -----------------------------------------------------------------------------
// CPosLmUrlEncoder::FinalizeEncodingL
// -----------------------------------------------------------------------------
//
CPosLmOperation* CPosLmUrlEncoder::FinalizeEncodingL()
    {
    // Check that the enocder status is EPosLmInfoAdded when this method is called,
    // if not panic with EPosLmProtocolBreak
    __ASSERT_ALWAYS(
            iEncoderStatus == EPosLmInfoAdded,
            Panic(KPosLandmarksClientPanic, EPosLmProtocolBreak));
    // Create an instance of the CPosLmUrlEncoderOperation,which will be returned
    // to the client,using which the client can call ExecuteL or NextStep to execute 
    // and complete the encoding operation.
    CPosLmUrlEncoderOperation* op = CPosLmUrlEncoderOperation::NewL( iOutput );
    // Ownership of iOutput has been transferred to op.
    iOutput = NULL;
    // Change staus back to EPosUninitialized for next encoding process
    iEncoderStatus = EPosUninitialized;
    return op;
    }


// -----------------------------------------------------------------------------
// CPosLmUrlEncoder::ResetEncoderL
// -----------------------------------------------------------------------------
//
void CPosLmUrlEncoder::ResetEncoderL()
    {
    // Delete all previous data
    delete iOutput;
    iOutput = NULL;
       
    delete iLastLandmark;
    iLastLandmark = NULL;
   
    iCategories.ResetAndDestroy(); 
    }

// -----------------------------------------------------------------------------
// CPosLmUrlEncoder::WriteUrlSchemeL
// -----------------------------------------------------------------------------
//
void CPosLmUrlEncoder::WriteUrlSchemeL()
    {
    CUri8* uri = CUri8::NewL();
    CleanupStack::PushL(uri);
    // Setting URI scheme component
    uri->SetComponentL(KUrlSchemeHttp, EUriScheme);
    const TDesC8& uriBuffer = uri->Uri().UriDes();
    
    // Convert retrieved uri descriptor to HBufC16
    HBufC16* uriScheme = HBufC16::NewL( uriBuffer.Length() );
    CleanupStack::PushL( uriScheme );
    uriScheme->Des().Copy( uriBuffer ); 
    
    
    // Write the URL scheme to encoded output
    iOutput->WriteL( *uriScheme );
    CleanupStack::PopAndDestroy( 2,uri );
    }

// -----------------------------------------------------------------------------
// CPosLmUrlEncoder::WriteUrlHostL
// -----------------------------------------------------------------------------
//
void CPosLmUrlEncoder::WriteUrlHostL()
    {
    CUri8* uri = CUri8::NewL();
    CleanupStack::PushL(uri);
    // Setting URI host component
    uri->SetComponentL(KUrlHostFull, EUriHost);
    const TDesC8& uriBuffer = uri->Uri().UriDes();
    
    // Convert retrieved uri descriptor to HBufC16
    HBufC16* uriHost = HBufC16::NewL( uriBuffer.Length() );
    CleanupStack::PushL( uriHost );
    uriHost->Des().Copy( uriBuffer );    
    
    // Write the URL host to encoded output
    iOutput->WriteL( *uriHost );
    CleanupStack::PopAndDestroy( 2,uri );
    }

// -----------------------------------------------------------------------------
// CPosLmUrlEncoder::EncodeLandmarkL
// -----------------------------------------------------------------------------
//
void CPosLmUrlEncoder::EncodeLandmarkL()
    {
    // Holds value of extracted landmark text fields
    TPtrC textField;
    // Write the Url query start string 
    iOutput->WriteL( KUrlQueryStart );
    
    // Get each of the landmark attributes and encode to Url
    
    // Holds coordinate information
    TLocality position;
    // Get landmark coordinate and
    // Write landmark coordinates to the buffer only if the latitude
    // and longitude values are not NaN,else leave with KErrArgument
    if ( iLastLandmark->GetPosition( position ))
        {
        User::Leave( KErrArgument );
        }
    WriteCoordinatesL( position );
    
    
    // Get Place id
    
    if( !( iLastLandmark->GetPlaceId( textField ) ) )
        {
        HBufC* encodedPid = EncodeTextL( textField,KUrlMaxTextEncodingLength);
        CleanupStack::PushL( encodedPid );
        WriteQueryParamValueL( KUrlPidParam, *encodedPid );
        CleanupStack::PopAndDestroy( encodedPid );
        }
    
    // Get Landmark name
    if( !( iLastLandmark->GetLandmarkName( textField ) ) )
        {
        HBufC* encodedName = EncodeTextL( textField,KUrlMaxTextEncodingLength );
        CleanupStack::PushL( encodedName );
        WriteQueryParamValueL( KUrlNameParam,*encodedName );
        CleanupStack::PopAndDestroy( encodedName );
        }
    
    // Get Landmark Description
    if( !( iLastLandmark->GetLandmarkDescription( textField ) ) )
        {
        HBufC* encodedDescription = EncodeTextL( textField,KUrlMaxDescriptionEncodingLength );
        CleanupStack::PushL( encodedDescription );
        WriteQueryParamValueL( KUrlDescriptionParam,*encodedDescription );
        CleanupStack::PopAndDestroy( encodedDescription );                        
        }
    
    // Get House Number
    if( !( iLastLandmark->GetPositionField( EPositionFieldStreetExtension,textField ) ) )
        
        {
        HBufC* encodedHouseNumber = EncodeTextL( textField,KUrlMaxTextEncodingLength );
        CleanupStack::PushL( encodedHouseNumber );
        WriteQueryParamValueL( KUrlHouseNumberParam,*encodedHouseNumber );        
        CleanupStack::PopAndDestroy( encodedHouseNumber );
        }
        
    // Get Street
    if( !( iLastLandmark->GetPositionField( EPositionFieldStreet,textField ) ) )
        {
        HBufC* encodedStreet = EncodeTextL( textField,KUrlMaxTextEncodingLength );
        CleanupStack::PushL( encodedStreet );
        WriteQueryParamValueL( KUrlStreetParam,*encodedStreet );        
        CleanupStack::PopAndDestroy( encodedStreet );
        }    
        
    // Get City
    if( !( iLastLandmark->GetPositionField( EPositionFieldCity,textField ) ) )
        {
        HBufC* encodedCity = EncodeTextL( textField,KUrlMaxTextEncodingLength );
        CleanupStack::PushL( encodedCity );
        WriteQueryParamValueL( KUrlCityParam,*encodedCity );        
        CleanupStack::PopAndDestroy( encodedCity );
        }    
        
    // Get State
    if( !( iLastLandmark->GetPositionField( EPositionFieldState,textField ) ) )
       {
       HBufC* encodedState = EncodeTextL( textField,KUrlMaxTextEncodingLength );
       CleanupStack::PushL( encodedState );
       WriteQueryParamValueL( KUrlStateParam,*encodedState );        
       CleanupStack::PopAndDestroy( encodedState );
       }
       
   // Get Country
   if( !( iLastLandmark->GetPositionField( EPositionFieldCountry,textField ) ) )
       {
       HBufC* encodedCountry = EncodeTextL( textField,KUrlMaxTextEncodingLength );
       CleanupStack::PushL( encodedCountry );
       WriteQueryParamValueL( KUrlCountryParam,*encodedCountry );        
       CleanupStack::PopAndDestroy( encodedCountry );
       }
   
   // Get Postal Code
   if( !( iLastLandmark->GetPositionField( EPositionFieldPostalCode,textField ) ) )
       {
       HBufC* encodedPostalcode = EncodeTextL( textField,KUrlMaxTextEncodingLength );
       CleanupStack::PushL( encodedPostalcode );
       WriteQueryParamValueL( KUrlPostalCodeParam,*encodedPostalcode );        
       CleanupStack::PopAndDestroy( encodedPostalcode );
       }
   
   // Get Telephone
   if( !( iLastLandmark->GetPositionField( EPositionFieldBuildingTelephone,textField ) ) )
       {
       HBufC* encodedTelephone = EncodeTextL( textField,KUrlMaxTextEncodingLength );
       CleanupStack::PushL( encodedTelephone );
       WriteQueryParamValueL( KUrlTelephoneParam,*encodedTelephone );        
       CleanupStack::PopAndDestroy( encodedTelephone );
       }
   
   // Use TRealFormat to format the real number obtained for speed & heading to display
   // upto 2 decimal digits.
   TRealFormat realFormat;
   realFormat.iPoint = KRealPoint;
   realFormat.iType = KRealFormatFixed | KDoNotUseTriads;
   realFormat.iPlaces = KRealDecimalPlaces;
   realFormat.iWidth = KRealWidth;
   // Get Speed
   if( !( iLastLandmark->GetPositionField( EPositionFieldHorizontalSpeed,textField ) ) )
       {
       TReal32 speed;
       // Descriptor that holds the formatted speed value
       TBuf<KRealWidth> speedValue;
       // Convert the descriptor obtained into real and check if the value is 
       // indeed real.If yes then write to the encoded output. 
       TRAPD( error, PosLmConverterUtils::DesToRealL( textField, speed ) );
       
       if( !error  )
           {
           TInt speedLength = speedValue.Num( speed, realFormat );
           if( speedLength != KErrGeneral )
               {
               WriteQueryParamValueL( KUrlSpeedParam,speedValue );
               }
           }
       }
   
   // Get Heading
   if( !( iLastLandmark->GetPositionField( EPositionFieldHeading,textField ) ) )
       {
       TReal32 heading;
       
       // Descriptor that holds the formatted heading value
       TBuf<KRealWidth> headingValue;
       // Convert the descriptor obtained into real and check if the value is 
       // indeed real.If yes then write to the encoded output.
       TRAPD( error, PosLmConverterUtils::DesToRealL( textField, heading ) );
       
       if( !error )
           {
           TInt headingLength = headingValue.Num( heading, realFormat );
           if( headingLength!= KErrGeneral )
               {
               WriteQueryParamValueL( KUrlHeadingParam, headingValue );
               }
           }
       }
   
   // Get Categories that are associated with this landmark.
   // In case of local category the category name information can only be obtained  
   // if the category is added to the landmark database.In cases of global category,
   // the category name can still be obtained using the category id.
   
   // Holds the category id array 
   RArray<TPosLmItemId> categoryIdArray;
   TRAPD( error, iLastLandmark->GetCategoriesL( categoryIdArray ) );
   // Check if there are categories added
   if( !error && categoryIdArray.Count() != 0 )
       {
       // Open the default database from which the category is read by using the
       // category id
       CPosLandmarkDatabase* defaultDb = CPosLandmarkDatabase::OpenL();
       CleanupStack::PushL( defaultDb );
       ExecuteAndDeleteLD( defaultDb->InitializeL() );
       
       // Create an instance of CPosLmCategoryManager which provides method to read
       // the category from the database
       CPosLmCategoryManager* categoryManager = CPosLmCategoryManager::NewL( *defaultDb );
       CleanupStack::PushL( categoryManager );
       
       // For each of the category id,get the category name and write it to 
       // the encoded output
       for( TInt i=0;i<categoryIdArray.Count();i++ )
           {
           CPosLandmarkCategory* category = NULL;
           // Read the category info from database
           TRAPD( error, category = 
                         categoryManager->ReadCategoryLC( categoryIdArray[i] ); 
                 // If the ReadCategoryLC does not leave then pop category so as 
                 // to avoid the panic e32user::cbase71
                 if (category!=NULL) CleanupStack::Pop( category ) );
           
           // Check that there is no error while reading the category info
           if( !error )
               {
               // Push category onto the cleanupstack
               CleanupStack::PushL( category );
               // Check that the category has a name before encoding it to the output
               if( category->GetCategoryName( textField ) == KErrNone )
                   {
                   // Encode the category name before writing it to the encoded output
                   HBufC* encodedCategoryName = EncodeTextL( textField, 
                                                KUrlMaxCategoryEncodingLength );
                   CleanupStack::PushL( encodedCategoryName );
                   WriteQueryParamValueL( KUrlCategoryParam, *encodedCategoryName );
                   CleanupStack::PopAndDestroy( encodedCategoryName );
                   }
               // Pop & destroy category which was left on   cleanupstack
               CleanupStack::PopAndDestroy( category );
               }

           }
       // Pop & destroy items pushed onto the cleanup stack
       CleanupStack::PopAndDestroy( 2,defaultDb );
       }
   
   // Get Web Url field
   // Check the position fields value within the range EPositionFieldMediaLinksStart
   // and EPositionFieldMedaiLinksEnd.Consider the first field within this range that
   // has a value specified for encoding.
   TPositionFieldId id = EPositionFieldMediaLinksStart;
   while ( id <= EPositionFieldMedaiLinksEnd )
       {
       // Get position field id value
       if( iLastLandmark->GetPositionField( id, textField ) != KErrNone )
           {
           // Increment position field id 
           id++ ;
           }
       else
           {
           // Encode the Web url
           HBufC* encodedWebUrl = EncodeTextL( textField, KUrlMaxTextEncodingLength );
           CleanupStack::PushL( encodedWebUrl );
           WriteQueryParamValueL( KUrlWebParam, *encodedWebUrl );
           CleanupStack::PopAndDestroy( encodedWebUrl );
           // break the while loop after the first url is found & encoded
           break;
           }
       }
   
   // Get the timestamp
   TTime timeStamp;
   if( !( iLastLandmark->GetTimeStamp( timeStamp ) ) )
       {
       // Encode time according to the format YYYYMMDD:HHMMSS.MMMMMM
       HBufC* encodedTime = EncodeTimeL( timeStamp );
       CleanupStack::PushL( encodedTime );
       WriteQueryParamValueL( KUrlTimeStampParam, *encodedTime );
       CleanupStack::PopAndDestroy( encodedTime );
       }
    }

// -----------------------------------------------------------------------------
// CPosLmUrlEncoder::WriteCoordinatesL
// -----------------------------------------------------------------------------
//
void CPosLmUrlEncoder::WriteCoordinatesL( const TLocality& aPosition )
    {
    TBuf<KRealWidth> latitude;
    TBuf<KRealWidth> longitude;
    TBuf<KRealWidth> altitude;
    TBuf<KRealWidth> positionAccuracy;
    TBuf<KRealWidth> altitudeAccuracy;
    
    // Use TRealFormat for formatting the real number while converting the latitude 
    // & longitude to descriptor to display upto 6 decimal digits.
    TRealFormat realCoordinateFormat;
    realCoordinateFormat.iPoint = KRealPoint;
    realCoordinateFormat.iType = KRealFormatFixed | KDoNotUseTriads;
    realCoordinateFormat.iPlaces = KCoordinateDecimalPlaces;
    realCoordinateFormat.iWidth = KRealWidth;
    
    // Use TRealFormat for formatting the real number while converting the altitude,
    // altitudeaccuracy,position accuracy to descriptor to display upto 2 decimal
    // digits.
    TRealFormat realFormat;
    realFormat.iPoint = KRealPoint;
    realFormat.iType = KRealFormatFixed | KDoNotUseTriads;
    realFormat.iPlaces = KRealDecimalPlaces;
    realFormat.iWidth = KRealWidth;
    
    // Check that latitude & longitude values are not NAN,if yes then leave 
    // with KErrArgument as these parameters are mandatory.
    if( !Math::IsNaN( aPosition.Latitude() ) && !Math::IsNaN( aPosition.Longitude() ) )
        {
        // formatting the real number to display upto 6 decimal digits
        TInt latitudeLength = latitude.Num( aPosition.Latitude(), realCoordinateFormat );
        TInt longitudeLength = longitude.Num( aPosition.Longitude(), realCoordinateFormat );
        if( latitudeLength != KErrGeneral && longitudeLength != KErrGeneral )
            {
            // Write latitude param
            iOutput->WriteL( KUrlLatitudeParam );
            iOutput->WriteL( latitude );
                    
            // Write longitude param
            WriteQueryParamValueL( KUrlLongitudeParam,longitude );
            }
        else
            {
            User::Leave( KErrGeneral );
            }
                  
        }
    else
        {
        User::Leave( KErrArgument );
        }
    // Check that altitude value is not NaN before writing to encoded output
    if( !Math::IsNaN( aPosition.Altitude() ) )
        {
        // formatting the real number to display upto 5 decimal digits
        TInt altitudeLength = altitude.Num( aPosition.Altitude(), realFormat );
        if( altitudeLength != KErrGeneral )
            {
            // Write altitude param
            WriteQueryParamValueL( KUrlAltitudeParam,altitude );
            }
        }
    
   
    // Get Horizontal accuracy which is considered as the position accuracy here
    // Check that position accuracy is not NaN before writing to encoded output
    if( !Math::IsNaN( aPosition.HorizontalAccuracy()  ) )
        {
        // Convert horizontal accuracy to descriptor
        TInt positionAccuracyLength = positionAccuracy.Num( aPosition.HorizontalAccuracy(),
                                                            realFormat );
        if( positionAccuracyLength != KErrGeneral )
            {
            // Write the position accuracy param
            WriteQueryParamValueL( KUrlPositionAccuracyParam,positionAccuracy );            
            }
        }
    
    // Get Vertical accuracy which is considered as altitude accuracy here
    // Check that altitude accuracy is not NaN before writing to encoded output
    if( !Math::IsNaN( aPosition.VerticalAccuracy() ) )
        {
        // Convert Vertical accuracy to descriptor
        TInt altitudeAccuracyLength = altitudeAccuracy.Num( aPosition.VerticalAccuracy(),
                                                            realFormat );
        if( altitudeAccuracyLength != KErrGeneral )
            {
            // Write the altitude accuracy param
            WriteQueryParamValueL( KUrlAltitudeAccuracyParam,altitudeAccuracy );
            }
        }
    }

// -----------------------------------------------------------------------------
// CPosLmUrlEncoder::WriteQueryParamValueL
// -----------------------------------------------------------------------------
//
void CPosLmUrlEncoder::WriteQueryParamValueL( const TDesC16& aParam,
                                              const TDesC16& aValue )
    {
    // Write the URL query delimeter before writing the param,value pair
    iOutput->WriteL( KUrlQueryDelimeter );
    iOutput->WriteL( aParam );
    iOutput->WriteL( aValue );
    }

// -----------------------------------------------------------------------------
// CPosLmUrlEncoder::EncodeTextL
// -----------------------------------------------------------------------------
//
HBufC* CPosLmUrlEncoder::EncodeTextL( const TDesC16& aText, const TInt aMaxEncodingLength )
    {
    TLex string( aText );
    HBufC* buffer = HBufC::NewL( aMaxEncodingLength );
    // Parse through the string and check if each of the character 
    // is either a digit/alphabet.If not get the ascii value of
    // character in hex and escape encode it.
    while( !string.Eos() )
        {
        TChar character = string.Get();
        // check if character is alpha digit
        if( !character.IsAlphaDigit() )
            {
            // Get Ascii value of character
            TInt ascii = character;
            // Check that the ascii value of the non-alphadigit 
            // character is in the range that can be encoded according 
            // to the grammar definition of URL mentioned in api spec.
            if( ( ascii >= KSpecialCharacterAsciiLowerLimit1 
                  && ascii <= KSpecialCharacterAsciiUpperLimit1 ) ||
                ( ascii >= KSpecialCharacterAsciiLowerLimit2 
                  && ascii <= KSpecialCharacterAsciiUpperLimit2 ) ||
                ( ascii >= KSpecialCharacterAsciiLowerLimit3 
                  && ascii <= KSpecialCharacterAsciiUpperLimit3 ) ||
                ( ascii >= KSpecialCharacterAsciiLowerLimit4 
                  && ascii <= KSpecialCharacterAsciiUpperLimit4 ) )
                {
                // Convert ascii in decimal to ascii in hex
                TBuf<2> buf;
                buf.Format(KUrlHex, character );
                // Write the escaped character to buffer
                buffer->Des().Append( KUrlEscapeCharacter ) ;
                buffer->Des().Append( buf ) ;
                }
            }
        else
        // Write the character as it is to the buffer if alpha/digit
            {
            buffer->Des().Append( character );
            }
        }
    // return the buffer containing the encoded text
    return buffer;
    }

// -----------------------------------------------------------------------------
// CPosLmUrlEncoder::EncodeCategoryL
// -----------------------------------------------------------------------------
//
void CPosLmUrlEncoder::EncodeCategoryL()
    {
    TPtrC categoryName;
    // Get Categories associated with the landmark to be encoded
    for( TInt i=0;i<iCategories.Count();i++ )
       {
       // Extract the category name for each of the categories
       if( iCategories[i]->GetCategoryName( categoryName ) == KErrNone )
           {
           // Encode the category name before writing it to the encoded output
           HBufC* encodedCategoryName = EncodeTextL( categoryName, 
                                                     KUrlMaxCategoryEncodingLength );
           CleanupStack::PushL( encodedCategoryName );
           WriteQueryParamValueL( KUrlCategoryParam, *encodedCategoryName );
           CleanupStack::PopAndDestroy( encodedCategoryName );
           }
       }
    }


// -----------------------------------------------------------------------------
// CPosLmUrlEncoder::EncodeTimeL
// -----------------------------------------------------------------------------
//
HBufC* CPosLmUrlEncoder::EncodeTimeL( const TTime& aTimeStamp )
    {
    // Buffer containing the encoded time field
    HBufC* timeField = HBufC::NewL( KUrlMaxTextEncodingLength );
    // Get the date time info from TTime 
    TDateTime dateTime = aTimeStamp.DateTime();
    
    // Encode each of the date time fields into the timeField buffer
    // Format the year field according to the number of digits
    timeField->Des().AppendFormat( KYearFormat, dateTime.Year() );
        
    // Format month field according to number of digits
    timeField->Des().AppendFormat( KFormat, dateTime.Month() );
        
    
    // Format month field according to number of digits
    timeField->Des().AppendFormat( KFormat, dateTime.Day() );
         
    // Append the date time separator
    timeField->Des().Append( KDateTimeSeparator );
        
    // Format month field according to number of digits
    timeField->Des().AppendFormat( KFormat, dateTime.Hour() );
       

    // Format month field according to number of digits
    timeField->Des().AppendFormat( KFormat, dateTime.Minute() );
          
    // Format month field according to number of digits
    timeField->Des().AppendFormat( KFormat, dateTime.Second() );
           
    // Append the time microseconds separator
    timeField->Des().Append( KTimeMicroSecondSeparator );
        
    // Format the microseconds field according to number of digits
    timeField->Des().AppendFormat( KFormatMicroSeconds, dateTime.MicroSecond() );
        
    return timeField;
    
    }
