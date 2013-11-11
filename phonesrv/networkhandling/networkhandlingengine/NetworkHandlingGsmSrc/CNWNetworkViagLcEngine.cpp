/*
* Copyright (c) 2002-2009 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  This module contains the implementation of CPENetworkViagEngine
*                 class member functions.
*
*/



// INCLUDE FILES
#include <rmmcustomapi.h>
#include "CNWNetworkViagLcEngine.h"
#include "CNWNetworkViagBaseEngine.h"
#include "CNWNetworkViagTimer.h"
#include "NWPanic.pan"
#include <networkhandlingdomainpskeys.h>
#include "NWLogger.h"

//TEST
#include <f32file.h>


// CONSTANTS
const TInt KNWDigitMask = 0x30;
const TInt KNWRatMask = 0xC0;
const TInt KNWCodeMask = 0xF;
const TInt KTagLength( 12 );
const TInt KStopChar( 255 ); // 0xFF
const TUint KInvalid( 65535 ); // 0xFFFF 
const TInt KDualHandset( 1 );
const TInt KMaxVersion ( 255 );//  0xFF
const TInt KTagCount( 4 );
const TInt KTagOffset( 48 );
const TInt KCSTwo( 2 );
const TInt KCSNine( 9 );
const TInt KCSFour( 4 );
const TInt KCSThree( 3 );
const TInt KCSTen( 10 );
const TInt KCellAndLACLength( 5 );
const TInt KCmpLength( 4 );

// ============================ MEMBER FUNCTIONS ==============================

// ----------------------------------------------------------------------------
// CNWNetworkViagLcEngine::CNWNetworkViagLcEngine
// C++ default constructor can NOT contain any code, that
// might leave.
// ----------------------------------------------------------------------------
//
CNWNetworkViagLcEngine::CNWNetworkViagLcEngine(
    CNWNetworkViagBaseEngine& aBase,
    TNWInfo& aNetworkInfo,
    RMmCustomAPI& aCustomAPI,
    TNWInterInfo& aInterNetworkInfo)
      : iBase ( aBase ),
        iNWNetworkInfo( aNetworkInfo ),
        iCustomAPI( aCustomAPI ),
        iInterNetworkInfo( aInterNetworkInfo ),
        iLastSection ( EFalse ),
        iMatchFound ( EFalse ),
        iActivation( ETrue ),
        iCbActivation( EFalse ),
        iCurrentLcCache( NULL )
    {
    NWLOGSTRING( KNWOBJECT, 
        "NW: CNWNetworkViagLcEngine::CNWNetworkViagLcEngine() called." );
    }

// ----------------------------------------------------------------------------
// CNWNetworkViagLcEngine::ConstructL
// Symbian 2nd phase constructor can leave.
// --------------------KTagLength----------------------------------------------
//
void CNWNetworkViagLcEngine::ConstructL()
    {
    NWLOGSTRING( KNWOBJECT, "NW: CNWNetworkViagLcEngine::ConstructL() Begin" );
    iLcCacheOne = new ( ELeave ) TViagElementReocord;
    iLcCacheTwo = new ( ELeave ) TViagElementReocord;
    iLcCacheThree = new ( ELeave ) TViagElementReocord;
    iLcCacheFour = new ( ELeave ) TViagElementReocord;
    NWLOGSTRING( KNWOBJECT, "NW: CNWNetworkViagLcEngine::ConstructL() End" );
    }

// ----------------------------------------------------------------------------
// CNWNetworkViagLcEngine::NewL
// Two-phased constructor.
// ----------------------------------------------------------------------------
//
CNWNetworkViagLcEngine* CNWNetworkViagLcEngine::NewL(
                CNWNetworkViagBaseEngine& aBase,
                TNWInfo& aNetworkInfo, 
                RMmCustomAPI& aCustomAPI,
                TNWInterInfo& aInterNetworkInfo)
    {
    NWLOGSTRING( KNWOBJECT, 
        "NW: CNWNetworkViagLcEngine::NewL() Begin." );
    
    CNWNetworkViagLcEngine* self = new (ELeave) CNWNetworkViagLcEngine(
                aBase,
                aNetworkInfo,
                aCustomAPI,
                aInterNetworkInfo );
    
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop( self );
    
    NWLOGSTRING( KNWOBJECT, 
        "NW: CNWNetworkViagLcEngine::NewL() End." );
    return self;
    }

// ----------------------------------------------------------------------------
// CNWNetworkViagLcEngine::~CNWNetworkViagLcEngine
// Destructor
// ----------------------------------------------------------------------------
//
CNWNetworkViagLcEngine::~CNWNetworkViagLcEngine()
    {
    NWLOGSTRING( KNWOBJECT,
        "NW: CNWNetworkViagLcEngine::~CNWNetworkViagLcEngine() Begin" );
    if ( iLcCacheOne )
        {
        ResetLcCache( *iLcCacheOne ); // Reset old values.     
        delete iLcCacheOne;
        }

    if ( iLcCacheTwo )
        {
        ResetLcCache( *iLcCacheTwo );
        delete iLcCacheTwo;
        }
    
    if ( iLcCacheThree )
        {
        ResetLcCache( *iLcCacheThree );
        delete iLcCacheThree;
        }

    if ( iLcCacheFour )
        {
        ResetLcCache( *iLcCacheFour );
        delete iLcCacheFour;
        }

    NWLOGSTRING(KNWOBJECT, 
        "NW: CNWNetworkViagLcEngine::CNWNetworkViagLcEngine() End");
    }

// ----------------------------------------------------------------------------
// CNWNetworkViagLcEngine::AddInBufferAsTUint
// 
// ----------------------------------------------------------------------------
//
void CNWNetworkViagLcEngine::AddInBufferAsTUint( 
        TDes & aBuffer, const TUint aData )
    {
    NWLOGSTRING2( KNWOBJECT, 
        "NW: CNWNetworkViagLcEngine::AddInBufferAsTUint() Begin,\
        aData = %d.", aData );
    
    aBuffer.Append( TChar( aData ) );    
    
    NWLOGSTRING( KNWOBJECT, 
        "NW: CNWNetworkViagLcEngine::AddInBufferAsTUint() End." );
    }
    
// ----------------------------------------------------------------------------
// CNWNetworkViagLcEngine::ExtendTUintIntoBuffer
// 
// ----------------------------------------------------------------------------
//
void CNWNetworkViagLcEngine::ExtendTUintIntoBuffer( 
        TDes & aDestData, const TUint aData )
    {
    NWLOGSTRING( KNWOBJECT, 
        "NW: CNWNetworkViagLcEngine::ExtendTUintIntoBuffer() Begin." );
    
    aDestData.Zero();
    aDestData.AppendNum( aData );    
    
    NWLOGSTRING( KNWOBJECT, 
        "NW: CNWNetworkViagLcEngine::ExtendTUintIntoBuffer() End." );
    }

// ----------------------------------------------------------------------------
// CNWNetworkViagLcEngine::ParseResult
// Decides which parser is used. 
// ----------------------------------------------------------------------------
//
void CNWNetworkViagLcEngine::ParseResult( 
    TNWNetworkElementFile aElementFile, const TDesC8&  aResponse )
    {
    NWLOGSTRING2( KNWOBJECT, "NW: CNWNetworkViagLcEngine::ParseResult() Begin,\
        aElementFile = 0x%x", aElementFile );
    
    switch (aElementFile)
        {
        case EFileSettings:
            ParseSettings( aResponse );
            break;
        case EFileTags:        
            ParseTags( aResponse );
            break;
        case EFileSubscribedOne:
        case EFileSubscribedTwo:
        case EFileSubscribedThree:
        case EFileSubscribedFour:
            ParseSubscribed( aElementFile, aResponse  );
            break;
        case EFileHZ:
        case EFileUnknown:
            {
            NWLOGSTRING2( KNWOBJECT, 
                "NW: CNWNetworkViagSimReader::ReadSimFile NOT SUPPORTED VALUE \
                 %d", aElementFile );
            } 
        }
    
    NWLOGSTRING( KNWOBJECT, "NW: CNWNetworkViagLcEngine::ParseResult() End" );
    }

// ----------------------------------------------------------------------------
// CNWNetworkViagLcEngine::GetActivation
// Gets activation bits. 
// ----------------------------------------------------------------------------
//
void CNWNetworkViagLcEngine::GetActivation(
        TBool& aActivation, TBool& aCbActivation )
    {
    NWLOGSTRING3( KNWOBJECT, 
        "NW: CNWNetworkViagLcEngine::GetActivation() Begin,\
        aActivation = %d, aCbActivation = %d", 
        iActivation, iCbActivation );
    
    aActivation = iActivation;
    aCbActivation = iCbActivation;
    
    NWLOGSTRING( KNWOBJECT, 
        "NW: CNWNetworkViagLcEngine::ParseResult() End" );
    }

// ----------------------------------------------------------------------------
// CNWNetworkViagLcEngine::ParseSettings
// Parse setting value from response and update it if wanted. 
// ----------------------------------------------------------------------------
//
void CNWNetworkViagLcEngine::ParseSettings( const TDesC8& aResponse )
    {
    NWLOGSTRING( KNWOBJECT,
        "NW: CNWNetworkViagLcEngine::ParseSettings() Begin" );
    
    if ( aResponse.Length() >= 2 )
        {
        iActivation =  ( aResponse[0] & 0x01 );
        iCbActivation = STATIC_CAST( TBool,  ( aResponse[0] >> 1 ) & 0x01 );
        iVersion = static_cast<TUint8> (aResponse[1] );
        NWLOGSTRING2( KNWOBJECT, 
            "NW: CNWNetworkViagLcEngine::ParseSettings Version = %d", 
            iVersion );
     
        if ( iActivation ) // The general activation must be "activation"
            {
            if ( ( iVersion + KDualHandset ) > KMaxVersion ) // The update version must not be over 0xFF if so the 
                {
                iVersion = 0x00; 
                }
            else
                {
                iVersion =  static_cast<TUint8> ( iVersion + KDualHandset  );
                NWLOGSTRING2( KNWOBJECT, 
                    "NW: CNWNetworkViagLcEngine::UpdateVersion Version = %d", 
                    iVersion );
                }
            UpdateVersion();
            }
        }
    
    NWLOGSTRING( KNWOBJECT, 
        "NW: CNWNetworkViagLcEngine::ParseSettings() End" );
    }

// ----------------------------------------------------------------------------
// CNWNetworkViagLcEngine::UpdateVersion
// Updates version number to SIM
// ----------------------------------------------------------------------------
//
void CNWNetworkViagLcEngine::UpdateVersion( )
    {
    NWLOGSTRING( KNWOBJECT,
        "NW: CNWNetworkViagLcEngine::UpdateVersion() Begin" );
    
    TInt errorCode( KErrNone );
    RMmCustomAPI::TViagUHZIUESettings hzSettings;
    if ( iActivation )
        {
        hzSettings.iSettings = 1;
        if ( iCbActivation )
            {
            hzSettings.iSettings = 3;
            }
        hzSettings.iVersion = iVersion;
        
        NWLOGSTRING2( KNWOBJECT, 
            "NW: CNWNetworkViagLcEngine::UpdateVersion iSettings = %d", 
            hzSettings.iSettings );
        NWLOGSTRING2( KNWOBJECT, 
            "NW: CNWNetworkViagLcEngine::UpdateVersion Version = %d", 
            hzSettings.iVersion );
        
        errorCode = iCustomAPI.WriteViagHomeZoneUHZIUESettings( hzSettings ); 
        if ( errorCode != KErrNone )
            {
            NWLOGSTRING2( KNWOBJECT,
                "NW: CNWNetworkViagLcEngine::UpdateVersion Error %d", 
                errorCode );
            }
        }
    else
        {
        NWLOGSTRING( KNWOBJECT,
            "NW: CNWNetworkViagLcEngine::UpdateVersion \
            General activation is OFF" );
        }
   
    NWLOGSTRING( KNWOBJECT,
        "NW: CNWNetworkViagLcEngine::UpdateVersion() End" );
    }


// ----------------------------------------------------------------------------
// CNWNetworkViagLcEngine::ParseTags
// Parses zone tags from response and writes them to iZoneTags struct. 
// ----------------------------------------------------------------------------
//
void CNWNetworkViagLcEngine::ParseTags( const TDesC8&  aResponse )
    {
    NWLOGSTRING2( KNWOBJECT, 
        "NW: CNWNetworkViagLcEngine::ParseTags() Begin,\
        length of aResponse = %d",aResponse.Length());

    TLex8 parse( aResponse );
    TInt zoneCount( 1 );
    TLexMark8 start;
    parse.Inc( KTagOffset ); // first 48 bits are ignored. 
 
    while ( zoneCount <= KTagCount ) // 4 zones, 
        {
        // 48.....96    Tag file legth is 96 bytes
        if ( ( KTagLength * zoneCount   ) <= aResponse.Length() )
            {
            parse.Mark( start );
            parse.Inc( KTagLength ); // // First tag
            
            TPtrC8 tag = parse.MarkedToken( start );
            TBuf<KTagLength> zoneText;
            TInt length = tag.Locate( KStopChar );
            
            TPtrC8 tempTag;
            // Check that there does'n come overflow
            if( length > KTagLength )
                {
                length = KTagLength;
                }
            if ( length > 0 )
                {
                tempTag.Set( tag.Left( length ) );
                iBase.DefaultAlphabetDecode( tempTag, zoneText );
                }
            else if ( length == KErrNotFound )
                {
                tempTag.Set( tag.Left( KTagLength ) );
                iBase.DefaultAlphabetDecode( tempTag, zoneText );
                } 
            else
                {
                zoneText.Copy( KNullDesC );
                }
            iBase.SetZoneTag( zoneCount+3, zoneText ); // First 4 tags comes from HZ file.
            }
        zoneCount++;    
        }
    NWLOGSTRING2( KNWOBJECT, "NW: CNWNetworkViagLcEngine::ParseTags(),\
        zoneCount = %d ", zoneCount ); 
    
    NWLOGSTRING( KNWOBJECT, "NW: CNWNetworkViagLcEngine::ParseTags() End" );
    }

// ----------------------------------------------------------------------------
// CNWNetworkViagLcEngine::ParseSubscribed
// Checks if current LAC file contains any subscribed cells. If at least one subscribed cell is found 
// parsefield method is called. 
// ----------------------------------------------------------------------------
//
void CNWNetworkViagLcEngine::ParseSubscribed( 
        TNWNetworkElementFile aElementFile, 
        const TDesC8&  aResponse )
    {
    NWLOGSTRING3( KNWOBJECT, 
        "NW: CNWNetworkViagLcEngine::ParseSubscribed() Begin,\
        aElementFile = 0x%x, length(aResponse) = %d",
        aElementFile, aResponse.Length());
    
    switch ( aElementFile )
        {
        case EFileSubscribedOne:
            { 
            iCurrentLcCache  =  iLcCacheOne;
            iLastSection = EFalse;
            break;    
            }
            
        case EFileSubscribedTwo:
            {
            iCurrentLcCache  =  iLcCacheTwo;
            iLastSection = EFalse;
            break;
            }
        case EFileSubscribedThree:
            {
            iCurrentLcCache  =  iLcCacheThree;    
            iLastSection = EFalse;
            break;
            }
        case EFileSubscribedFour:
            {
            iCurrentLcCache  =  iLcCacheFour;    
            iLastSection = EFalse;
            break;
            }

        case EFileHZ:
        case EFileTags:
        case EFileSettings:
        case EFileUnknown:
        default:
            {
            NWLOGSTRING( KNWOBJECT, 
                "NW: CNWNetworkViagLcEngine::ParseSubscribed() NOT SUPPORTED \
                element file" );
            break;
            }
        }
    ResetLcCache( *iCurrentLcCache );
    if ( aResponse.Length() >= 2 )
        {
        TLex8 parse ( aResponse );
        //************************************************    
        //** Network Coding is 3 bytes length 
        //************************************************

        iCurrentLcCache->iRatType = static_cast<TNWRatType> ( 
                aResponse[0] & KNWRatMask ) ; 
        if ( (aResponse[0] == 0xFF) && (aResponse[1] ==0xFF) && 
                (aResponse[2] ==0xFF))
            {
             // Lac file not contain any subscribed cells.
            NWLOGSTRING( KNWOBJECT, 
                "NW: CNWNetworkViagLcEngine::ParseSubscribed(),\
                ac file not contain any subscribed cells" );
            
             iCurrentLcCache->iCellSubscribed = EFalse;
            return;
            }
        iCurrentLcCache->iCellSubscribed = ETrue;
        TUint temp = ( aResponse[0] & KNWCodeMask) ;        
        temp =   temp  << 16  | aResponse[1] << 8  | aResponse[2] ;
        TBuf<6> netdes;
        netdes.Num(temp);    
        iCurrentLcCache->iMCC = netdes.Left( 3 );  
        NWLOGSTRING2( KNWOBJECT, 
            "NW: CNWNetworkViagLcEngine::ParseSubscribed() iMCC = %S", 
            &iCurrentLcCache->iMCC );
        if ( (aResponse[0] & KNWDigitMask) == 0x00 )
            {
            NWLOGSTRING( KNWOBJECT, 
               "NW: CNWNetworkViagLcEngine::ParseSubscribed(),\
               (aResponse[0] & KNWDigitMask) == 0x00 " );
            iCurrentLcCache->iMNC = netdes.Mid( 3,2 );  
            }
        else
            {
            iCurrentLcCache->iMNC = netdes.Right( 3 );      
            }
        NWLOGSTRING2( KNWOBJECT, 
            "NW: CNWNetworkViagLcEngine::ParseSubscribed() iMNC = %S", 
            &iCurrentLcCache->iMNC );
        parse.Inc( 3 ); //  Skip the network coding out because above used hard coded values.
        
        //*******************************************************
        //** Start field parsing.            
        //*******************************************************
        iCurrentLcCache->iCellSubscribed = ParseField( parse );
        }
    
    NWLOGSTRING( KNWOBJECT, 
        "NW: CNWNetworkViagLcEngine::ParseSubscribed() End" );
    }



// ----------------------------------------------------------------------------
// CNWNetworkViagLcEngine::ParseField
// Parses fields from given string to iField 
// ----------------------------------------------------------------------------
//
TBool CNWNetworkViagLcEngine::ParseField( TLex8& aParse )
    {
    NWLOGSTRING( KNWOBJECT, "NW: CNWNetworkViagLcEngine::ParseField() Begin" );
    // Parse so long that last section 
    
    while ( !iLastSection ) // Continue so long that last section is ongoing.
        {
        TBool invalid =  ParseSection( aParse ); // Parse next section...
        if ( invalid )
            {
            iLastSection = ETrue;
            return EFalse;
            //continue;
            }

        NWLOGSTRING2( KNWOBJECT, "NW: CNWNetworkViagLcEngine::ParseField(),\
            iCodingScheme = %d", iCodingScheme );
        
        for ( TInt fieldIndex = 0; fieldIndex < iNumberOfFields; fieldIndex++ ) // Section contains one or more fields.
            {
            TNWViagFieldContent field;
            if ( iCodingScheme == KCSTwo || 
                    iCodingScheme == KCSFour || 
                    iCodingScheme == KCSTen )
                {
                field.iFieldLAC = static_cast<TUint16> (( 
                    aParse.Get() << 8 ) | aParse.Get() ); 
                NWLOGSTRING2( KNWOBJECT, 
                    "NW: CNWNetworkViagLcEngine::ParseField() CodingScheme is ok fieldLAC = %d ", field.iFieldLAC );
                }
            else 
                {
                field.iFieldLAC  = 0; // no lac 
                NWLOGSTRING( KNWOBJECT, 
                    "NW: CNWNetworkViagLcEngine::ParseField() No fieldLAC " );
                }
            if (field.iFieldLAC == KInvalid )
                {
                NWLOGSTRING( KNWOBJECT, 
                   "NW: CNWNetworkViagLcEngine::ParseField() FieldLAC data is \
                   INVALID! " );
                return EFalse;
                //break;
                }
            field.iWildCarded = !static_cast<TBool> ( aParse.Get() >> 7 );
            aParse.UnGet(); 
            iNumberOfUnits = static_cast<TInt> ( aParse.Get() & 0x7F );
            
            TLexMark8 start;
            TInt lastSection = iCurrentLcCache->iCache.Count();
            
            for ( TInt unitIndex = 0; unitIndex < iNumberOfUnits; unitIndex++  ) // Field contains one or more units.
                {    
                // Cell id is readed  
                if ( iCodingScheme != KCSThree )
                    {
                    field.iUnitType = EUnitCellId;
                    }
                else
                    {
                    field.iUnitType = EUnitLac; // If SC is 3 the only field contains lac units.
                    }
                aParse.Mark( start );
               
                for ( TInt offset = 1; offset <= 2; offset++ )
                    {
                    if ( aParse.Eos() )
                        {
                        return EFalse; 
                        }
                    aParse.Inc();
                    }
                TPtrC8  ci = aParse.MarkedToken( start );
                
                // The iUnit is TBuf that contains the lac or ci in separated space like " 23800 44444 12345"    

                TUint unit = ( ci[ 0 ] << 8 ) | ci [ 1 ] ;
                NWLOGSTRING2( KNWOBJECT, 
                    "NW: CNWNetworkViagLcEngine::ParseField() unit = %d ", 
                    unit );
              
                if (  unit  == KInvalid ) // Cell or LAC should not be 0xFF if is then there is no valid data.
                    {
                    NWLOGSTRING( KNWOBJECT, 
                        "NW: CNWNetworkViagLcEngine::ParseField() unit data is \
                        INVALID! " );
                    return EFalse;
                    //break;
                    }
                AddInBufferAsTUint( field.iUnit, unit );
                } // Inner for loop end.

            // Store the field information.    
            if ( iNumberOfUnits != 0 ||( iNumberOfUnits == 0 && 
                    iCodingScheme == KCSTwo ) )
                {
                iCurrentLcCache->iCache[lastSection-1].iField.Append( field );
                }
            }// Outage for loop end.
        }// While loop end.
    NWLOGSTRING2( KNWOBJECT, "NW: CNWNetworkViagLcEngine::ParseField(),\
        after while loop the running variable iLastSection = %d",
        iLastSection );

    NWLOGSTRING( KNWOBJECT, "NW: CNWNetworkViagLcEngine::ParseField() End " );
    return ETrue;
}



// ----------------------------------------------------------------------------
// CNWNetworkViagLcEngine::ParseSection
// Finds a section information from given string and saves it to iCurrentCache struct. 
// ----------------------------------------------------------------------------
//
TBool CNWNetworkViagLcEngine::ParseSection( TLex8& aParse  )             
    {
    NWLOGSTRING( KNWOBJECT, "NW: CNWNetworkViagLcEngine::ParseSection() Begin" );
//*****************************************
//** Section Description.
//*****************************************

    TInt sectFirst = static_cast<TUint> ( aParse.Get() );
    TInt secSecond = static_cast<TUint> ( aParse.Get() );

    if ( sectFirst == KStopChar )
        {
        NWLOGSTRING( KNWOBJECT, 
            "NW: CNWNetworkViagLcEngine::ParseSection() Invalid data!! " );
        return ETrue; // Invalid data...  RETURN BOOLEAN IS SECTION VALID OR NOT:....
        }

    TNWViagSectionContent section;
    section.iCodingScheme = static_cast<TInt> (( sectFirst & 0x78 ) >>3 );
    iCodingScheme = section.iCodingScheme;
    if ( iCodingScheme != KCSTwo && iCodingScheme != KCSThree && 
            iCodingScheme != KCSFour &&iCodingScheme != KCSNine && 
            iCodingScheme != KCSTen )
        {
        NWLOGSTRING2( KNWOBJECT, 
           "NW: CNWNetworkViagLcEngine::ParseSection() Invalid CodingScheme = \
           %d !! ", iCodingScheme );
        return ETrue; // Invalid coding scheme...
        }
    section.iLastSection = static_cast<TBool> ( sectFirst >> 7 );  
    section.iZoneId = static_cast<TInt>( sectFirst & 0x07 );
    if ( secSecond == KStopChar )
        {
        iNumberOfFields = 0;
        }
    else
        {
        iNumberOfFields = secSecond;
        }
    
    iLastSection = section.iLastSection;
    iCurrentLcCache->iCache.Append( section ); // Store the section info.        
    
    NWLOGSTRING2( KNWOBJECT, 
        "NW: CNWNetworkViagLcEngine::ParseSection() iLastSection = %d", 
        iLastSection );
    NWLOGSTRING2( KNWOBJECT, 
        "NW: CNWNetworkViagLcEngine::ParseSection() iZoneId = %d", 
        section.iZoneId );
    NWLOGSTRING2( KNWOBJECT, 
        "NW: CNWNetworkViagLcEngine::ParseSection() Coding Scheme = %d", 
        iCodingScheme );
    NWLOGSTRING( KNWOBJECT, 
        "NW: CNWNetworkViagLcEngine::ParseSection() End" );

    return EFalse;
    }

// ----------------------------------------------------------------------------
// CNWNetworkViagLcEngine::IsGsmCellSubscribed
// Returns a boolean to indicate if there is at least one subscribed GSM cell. 
// ----------------------------------------------------------------------------
//    
TBool CNWNetworkViagLcEngine::IsGsmCellSubscribed()
    {
    NWLOGSTRING( KNWOBJECT, 
        "NW: CNWNetworkViagLcEngine::IsGsmCellSubscribed() Begin" );
    TBool subscribed( EFalse );
    for (TInt lacIndex = EFileSubscribedOne; lacIndex <= EFileSubscribedFour; lacIndex++)
        {
        switch ( lacIndex )
            {
            case EFileSubscribedOne:
                { 
                iCurrentLcCache  =  iLcCacheOne;            
                break;    
                }        
            case EFileSubscribedTwo:
                {
                iCurrentLcCache  =  iLcCacheTwo;    
                break;
                }
            case EFileSubscribedThree:
                {
                iCurrentLcCache  =  iLcCacheThree;    
                break;
                }
            case EFileSubscribedFour:
                {
                iCurrentLcCache  =  iLcCacheFour;    
                break;
                }
            default:
                break;
            }
            //  Ask from 02 that should the ERatAny also be in here.
        if ( ( iCurrentLcCache->iRatType == ERatGsm || 
                iCurrentLcCache->iRatType == ERatAny )  &&
               iCurrentLcCache->iCellSubscribed )
            {
            subscribed =  ETrue; // if any of four files contains gsm cells then return true
            }
        }
    
    NWLOGSTRING2( KNWOBJECT, 
        "NW: CNWNetworkViagLcEngine::IsGsmCellSubscribed() End,\
        Subscribed = %d", subscribed );
    return subscribed;
    }



// ----------------------------------------------------------------------------
// CNWNetworkViagLcEngine::CellReselection
// Is called by CNWNetworkCurrentNetworkMonitor in case of cell re-selection.
// ----------------------------------------------------------------------------
//
void CNWNetworkViagLcEngine::CellReselection( )
    {
    NWLOGSTRING( KNWOBJECT, 
        "NW: CNWNetworkViagLcEngine::CellReselection() Begin" );
    NWLOGSTRING2( KNWOBJECT, 
        "NW: CNWNetworkViagLcEngine::CellReselection() NetInfo LAC = %d ", 
        iInterNetworkInfo.iLac );
    NWLOGSTRING2( KNWOBJECT, 
        "NW: CNWNetworkViagLcEngine::CellReselection() NetInfo CountryCode = \
        %S ", &iNWNetworkInfo.iCountryCode );
    NWLOGSTRING2( KNWOBJECT, 
        "NW: CNWNetworkViagLcEngine::CellReselection() NetInfo NetworkId = %S"
        , &iNWNetworkInfo.iNetworkId );    
    
    TBuf<KCellAndLACLength> lacBuf;
    lacBuf.Num( iInterNetworkInfo.iLac );
    NWLOGSTRING2( KNWOBJECT, 
        "NW: CNWNetworkViagLcEngine::CellReselection() NetInfo CellId = %d ",
        iInterNetworkInfo.iCellId );
    TBuf<10> cellBuf;
    cellBuf.Num( iInterNetworkInfo.iCellId ); 
    iMatchFound = EFalse;
 
    if ( iNWNetworkInfo.iNetworkMode == ENWModeWcdma )
        {
        TInt64  desCell;
        TLex cellId( cellBuf );
        TUint realId;
        cellId.Val( desCell );
        realId = static_cast<TUint16> (I64LOW( desCell ) );
        cellBuf.Num( realId );
        }

    //*************************************
    //** Lc-Caches - for loop
    //*************************************

    for (TInt lacIndex = EFileSubscribedOne; lacIndex <= EFileSubscribedFour; lacIndex++)
        {
        // Very first thing to do is to check if we heve the match already.
        if ( iMatchFound )
            {
            break;
            }

        // Get the next LC cache
        switch ( lacIndex  )
            {
            case EFileSubscribedOne:
                { 
                iCurrentLcCache  =  iLcCacheOne;            
                break;    
                }        
            case EFileSubscribedTwo:
                {
                iCurrentLcCache  =  iLcCacheTwo;    
                break;
                }
            case EFileSubscribedThree:
                {
                iCurrentLcCache  =  iLcCacheThree;    
                break;
                }
            case EFileSubscribedFour:
                {
                iCurrentLcCache  =  iLcCacheFour;    
                break;
                }
            }

        //**************************************************************************
        //** MNC,MCC,RAT => Search and select LC-cache that match with MNC,MCC, RAT
        //**************************************************************************
        NWLOGSTRING2( KNWOBJECT, 
            "NW: CNWNetworkViagLcEngine::CellReselection() LacIndex  = %d \
            ( 0 = iLcCacheOne, ... , 3 =  iLcCacheFour )", lacIndex );
        NWLOGSTRING2( KNWOBJECT, 
            "NW: CNWNetworkViagLcEngine::CellReselection() \
            CountryCode = %S ", &iCurrentLcCache->iMCC );
        NWLOGSTRING2( KNWOBJECT, 
            "NW: CNWNetworkViagLcEngine::CellReselection() NetworkId   \
            = %S ", &iCurrentLcCache->iMNC );
        NWLOGSTRING2( KNWOBJECT, 
            "NW: CNWNetworkViagLcEngine::CellReselection() NetInfo \
            NetworkMode = %d ( 0=ENWModeUnknown, 1=ENWModeUnregistered, \
            2=ENWModeGsm, 3=ENWModeAmps, 4=ENWModeCdma95, 5=ENWModeCdma2000, \
            6=ENWModeWcdma, 7=ENWModeTdcdma ) ", 
            iNWNetworkInfo.iNetworkMode );
        NWLOGSTRING2( KNWOBJECT, 
            "NW: CNWNetworkViagLcEngine::CellReselection() RatType\
            = %d ( 0=ERatUtran, 64=ERatGsm, 128=ERatAny, \
            192=ERatUnvalid ) ", iCurrentLcCache->iRatType );

        // Data check. If there are no content there is no need to continue further
        if ( !iCurrentLcCache->iCellSubscribed )
            {
            // If no subscribed cell then change lac file. 
            continue; 
            }
        if ( iCurrentLcCache->iCache.Count() == 0 )
            {
            NWLOGSTRING( KNWOBJECT, 
                "NW: CNWNetworkViagLcEngine::CellReselection() Lac file does \
                not contain any data, change lac file" );
            continue; // Lac file doesn't contain any data change lac file.
            }

        // Network check. If the subscribed network is same than the current one 
        // then there are no need to continue further
        if( !HandleNetworkCheck() )
            {
            continue;                
            }

        //***************************************
        //**    SECTION - while loop
        //***************************************
        TInt sectionIndex( KErrNotFound );
        do 
            {
            sectionIndex++;
            // Index = section 
            iCodingScheme = iCurrentLcCache->iCache[sectionIndex].iCodingScheme;
            TInt fieldCount = iCurrentLcCache->iCache[sectionIndex].iField.Count();
            
            if ( iCodingScheme == KCSNine )
                {
                WriteViagValues( sectionIndex );
                break; // go out field search
                }

            NWLOGSTRING2( KNWOBJECT, 
                "NW: CNWNetworkViagLcEngine::CellReselection() SECTION iCoding\
                Scheme = %d ", iCodingScheme );

            //************************************
            //** FIELD - for loop
            //************************************
            for (TInt index = 0; index < fieldCount; index++ )
                {
                // In following codingschemes all the cells that belongs field lac
                if ( iCodingScheme != KCSThree )
                    {
                    if( !HandleCellMatch( cellBuf, lacBuf, sectionIndex, index ) )
                       {
                       continue;
                       }
                    }
                else
                    {
                    // Fos coding scheme 3
                    HandleLACMatch( sectionIndex, index, lacBuf );                  
                    }
                
                // Check if we got match
                if ( iMatchFound )
                    {
                    break;
                    }
                }

            } while ( !iCurrentLcCache->iCache[sectionIndex].iLastSection && 
                    !iMatchFound );
        
        NWLOGSTRING3( KNWOBJECT, 
            "NW: CNWNetworkViagLcEngine::CellReselection(), after do-while\
            loop condition variable iCurrentLcCache->iCache[sectionIndex].\
            iLastSection = %d, iMatchFound = %d ",
            iCurrentLcCache->iCache[sectionIndex].iLastSection,
            iMatchFound );
        } // for 

    NWLOGSTRING( KNWOBJECT, "NW: CNWNetworkViagLcEngine::CellReselection() End" );
    
    } // ************* CELL RESELECTION ******************

// ----------------------------------------------------------------------------
// CNWNetworkViagLcEngine::ResetLcCache
// Is called by CNWNetworkCurrentNetworkMonitor in case of cell re-selection.
// ----------------------------------------------------------------------------
//
void CNWNetworkViagLcEngine::ResetLcCache( TViagElementReocord& aCurrentcache )
    {
    NWLOGSTRING( KNWOBJECT, 
        "NW: CNWNetworkViagLcEngine::ResetLcCache() Begin" );
    
    NWLOGSTRING2( KNWOBJECT, 
        "NW: CNWNetworkViagLcEngine::ResetLcCache(),\
        aCurrentcache.iCache.Count() = %d ",
        aCurrentcache.iCache.Count() );
    
    if ( aCurrentcache.iCache.Count() > 0 )
        {
        for ( TInt index = 0; index < aCurrentcache.iCache.Count(); index ++ )
            {
            aCurrentcache.iCache[index].iField.Close();
            }
        aCurrentcache.iCache.Close();
        }
    aCurrentcache.iRatType = ERatUnvalid;
    aCurrentcache.iMCC = KNullDesC;
    aCurrentcache.iMNC = KNullDesC;
    aCurrentcache.iCellSubscribed = EFalse;
    
    NWLOGSTRING( KNWOBJECT, 
        "NW: CNWNetworkViagLcEngine::ResetLcCache() End" );
    }

// ----------------------------------------------------------------------------
// CNWNetworkViagLcEngine::WriteViagValues
// Selects correct zone type and calls iBase class WriteViagValues method.
// ----------------------------------------------------------------------------
//
void CNWNetworkViagLcEngine::WriteViagValues( TInt aSectionIndex )
    {
    NWLOGSTRING2( KNWOBJECT, 
        "NW: CNWNetworkViagLcEngine::WriteViagValues() Begin,\
        aSectionIndex = %d ", aSectionIndex );
    
    TInt zoneId( KErrNotFound );
    if ( aSectionIndex >= 0 && aSectionIndex < iCurrentLcCache->iCache.Count() )
        {
        zoneId = iCurrentLcCache->iCache[aSectionIndex].iZoneId;
        

        NWLOGSTRING2( KNWOBJECT, 
            "NW: CNWNetworkViagLcEngine::WriteViagValues() ZoneId = %d ( 4 = \
            CityZone others HomeZone )", zoneId );
        TNWViagIndicatorType zoneType;

        if ( zoneId == 0 ) // Homezone
            {
            zoneType = ENWViagIndicatorTypeHomeZone; 
            }    
        else
            {
            zoneType = ENWViagIndicatorTypeCityZone; 
            }

        iBase.WriteViagValues( zoneType, zoneId );
        iMatchFound = ETrue;
        }
    
    NWLOGSTRING( KNWOBJECT, 
        "NW: CNWNetworkViagLcEngine::WriteViagValues() End " );
    }


// ----------------------------------------------------------------------------
// CNWNetworkViagLcEngine::HandleNetworkCheck
// Check network parameters.
// ----------------------------------------------------------------------------
//
TBool CNWNetworkViagLcEngine::HandleNetworkCheck()
    {
    NWLOGSTRING( KNWOBJECT, 
        "NW: CNWNetworkViagLcEngine::HandleNetworkCheck() Begin " );
    
    TBool networkOk = ETrue;
    
    if ( ( iNWNetworkInfo.iCountryCode  == iCurrentLcCache->iMCC ) &&
         ( iNWNetworkInfo.iNetworkId  == iCurrentLcCache->iMNC ) )
        {
        if ( iNWNetworkInfo.iNetworkMode  == ENWModeGsm ) 
            {
            if ( iCurrentLcCache->iRatType != ERatGsm &&
                 iCurrentLcCache->iRatType != ERatAny )
                {
                NWLOGSTRING( KNWOBJECT, 
                    "NW: CNWNetworkViagLcEngine::CellReselection() MNC and MCC \
                    mathes but iRatType != ERatGsm or iRatType != ERatAny ==> \
                    next cache file !" );
                networkOk = EFalse;
                }
            }
        else if ( iNWNetworkInfo.iNetworkMode == ENWModeWcdma )
            {
             if ( iCurrentLcCache->iRatType != ERatUtran &&
                  iCurrentLcCache->iRatType != ERatAny )
                {
                NWLOGSTRING( KNWOBJECT, 
                   "NW: CNWNetworkViagLcEngine::CellReselection() MNC and MCC \
                   mathes but iRatType != ERatUtran or iRatType != ERatAny \
                   ==> next cache file !" );
                networkOk = EFalse;
                }
            }
        }
    else
        {
        NWLOGSTRING( KNWOBJECT, 
            "NW: CNWNetworkViagLcEngine::CellReselection() MNC or MCC does not\
             match ==> next cache file !" );
        networkOk = EFalse;    // The MNC or MCC doesn't match.===> next cache file.
        }

    NWLOGSTRING2( KNWOBJECT, 
        "NW: CNWNetworkViagLcEngine::HandleNetworkCheck() End,\
        networkOk = %d ", networkOk );
    return networkOk;
    }
    
    
// -----------------------------------------------------------------------------
// CNWNetworkViagLcEngine::HandleCellMatch
// Search a matching cell id from given Cache index.
// -----------------------------------------------------------------------------
//
TBool CNWNetworkViagLcEngine::HandleCellMatch( 
        const TDesC & aCell, const TDesC & aLac,
        TInt aSectionIndex, TInt aIndex )
    {
    NWLOGSTRING3( KNWOBJECT, 
        "NW: CNWNetworkViagLcEngine::HandleCellMatch() Begin,\
        aSectionIndex = %d, aIndex = %d", 
        aSectionIndex, aIndex );
    
    TBool retVal = ETrue;
    TInt sectionIndex = aSectionIndex;
    TInt index = aIndex;
   
    TBuf<KCellAndLACLength> tempLac;
    tempLac.Num(  iCurrentLcCache->iCache[sectionIndex].iField[index].iFieldLAC ); 
    NWLOGSTRING2( KNWOBJECT, 
        "NW: CNWNetworkViagLcEngine::HandleCellMatch() Field FieldLAC = %d ", 
        iCurrentLcCache->iCache[sectionIndex].iField[index].iFieldLAC  );
    NWLOGSTRING2( KNWOBJECT, 
        "NW: CNWNetworkViagLcEngine::HandleCellMatch() NetInfo LAC = %d ", 
        iInterNetworkInfo.iLac );
               
    if ( iCodingScheme == KCSTen )  // CodingScheme 10 LAC is wildcarded "123**"
        {
        if ( aLac.Left( 3 ) != tempLac.Left( 3 ) ) 
            {
            NWLOGSTRING( KNWOBJECT, 
                "NW: CNWNetworkViagLcEngine::HandleCellMatch() FIELD loop LAC\
                 does not Match ");
            retVal = EFalse; // Lac doesn't match go next FIELD
            }
        }
    else if ( tempLac != aLac ) // Coding schemes 2,4...
        {
        NWLOGSTRING( KNWOBJECT, "NW: CNWNetworkViagLcEngine::HandleCellMatch()\
                 FIELD loop LAC does not Match CodingScheme != 10 ");
        // If Lac doesn't match then go next Field because the Lac is descriped in Field
        retVal = EFalse;
        }    

    // *************************************************
    // ** Match cell 
    // *************************************************

    if ( retVal )
        {
        TInt match( KErrNotFound );
        NWLOGSTRING( KNWOBJECT, 
            "NW: CNWNetworkViagLcEngine::HandleCellMatch() LAC MATCHS ");
        if ( iCurrentLcCache->iCache[sectionIndex].iField[index].iUnitType == EUnitCellId )
            {    
            NWLOGSTRING( KNWOBJECT, 
                "NW: CNWNetworkViagLcEngine::CellReselection()  \
                Unit Type *CELL* ");

            NWLOGSTRING( KNWOBJECT, 
                "NW: CNWNetworkViagLcEngine::HandleCellMatch() \
                MATH CELL Wild Card Used" );
            TBuf<5> workBuf;
            workBuf.Zero();
            TInt cursorPos = 0;
            
            TInt dataLen = iCurrentLcCache->iCache[sectionIndex].iField[index].iUnit.Length();
            
            TPtrC  cmpCell;
            //*********** WHILE *********************
            while ( match == KErrNotFound ) // Match is found or we are end of string.
                {
                if ( cursorPos == dataLen )
                    {
                    // The end of data reached
                    break;
                    }
                TUint unit( iCurrentLcCache->iCache[sectionIndex].iField[index].iUnit[cursorPos] );
                workBuf.Zero();
                // Expand TUint into string for the comparisation operations
                ExtendTUintIntoBuffer( workBuf, unit );
                cmpCell.Set( workBuf );
                
                NWLOGSTRING2( KNWOBJECT, 
                    "NW: CNWNetworkViagLcEngine::HandleCellMatch()  CELL ID %S\
                     FROM LAC FILE", &cmpCell );
                NWLOGSTRING2( KNWOBJECT, 
                    "NW: CNWNetworkViagLcEngine::HandleCellMatch()  CELL ID %S\
                    FROM NETWORK", &aCell ); 
                if ( cmpCell.Length() > 0 )
                    {
                    if ( iCurrentLcCache->iCache[sectionIndex].iField[index].iWildCarded )
                        {
                        // If cell is id start with "6" make exact match.
                        if ( cmpCell[0] == '6' && cmpCell.Length() == KCellAndLACLength ||
                             aCell[0] == '6' && aCell.Length() == KCellAndLACLength )   // If the wild card is in use 
                            {
                            if ( aCell == cmpCell ) 
                                {
                                match = KErrNone;
                                }
                            }
                        else 
                            {
                            if ( aCell.Length() < KCmpLength || cmpCell.Length() < KCmpLength )
                                {
                                match = CompareTIntCellIdValues( aCell, cmpCell );
                                }
                            else
                                {
                                if ( aCell.Right( KCmpLength ) == cmpCell.Right( KCmpLength )  )
                                    {
                                    match = KErrNone;
                                    }
                                }
                            }
                        } //  **** Wild card if 
                    else
                        {
                        if ( aCell == cmpCell ) 
                            {
                            NWLOGSTRING( KNWOBJECT, 
                                "NW: CNWNetworkViagLcEngine::HandleCellMatch()\
                                 EXACT MATCH FOUND *NO WC * " );
                            match = KErrNone;
                            }
                        }
                    }                                    
                    cursorPos++;                                    
                    
                } //***************** WHILE END *****************
    
            } // if ( iCurrentLcCache->iCache[sectionIndex].iField[index].iUnitType == EUnitCellId ) 
        
        //************************************************                        
        //** Match cell End.    
        //************************************************
                
        // If match is found call writeViagValues, BUT in CS 2 match MUST be KErrNotFound that
        // values is updated    
        if ( ( match != KErrNotFound && iCodingScheme != KCSTwo ) ||
             ( match == KErrNotFound && iCodingScheme == KCSTwo ) )
            {
            WriteViagValues( sectionIndex );
            }    
        }
    NWLOGSTRING2( KNWOBJECT, 
        "NW: CNWNetworkViagLcEngine::HandleCellMatch() End,\
        retVal = %d", retVal );
    
    return retVal;
    }

// ----------------------------------------------------------------------------
// CNWNetworkViagLcEngine::HandleLACMatch
// Finds a matching Lac Id.
// ----------------------------------------------------------------------------
//
void CNWNetworkViagLcEngine::HandleLACMatch( 
        TInt aSectionIndex, 
        TInt aIndex, 
        const TDesC& aLacBuf )
    {
    NWLOGSTRING3( KNWOBJECT, 
        "NW: CNWNetworkViagLcEngine::HandleLACMatch() \
        Begin, aSectionIndex = %d, aIndex = %d ",
        aSectionIndex, aIndex );
    
    // Make sure that unit contains lac units.
    TInt match( KErrNotFound );

    if ( iCurrentLcCache->iCache[aSectionIndex].iField[aIndex].iUnitType == EUnitLac )
        {
        TBuf<5> workBuf;    // This is enough for TUint16 (max value 65655)
        TInt cursorPos = 0;
        TInt dataLen = iCurrentLcCache->iCache[aSectionIndex].iField[aIndex].iUnit.Length();

        TPtrC  cmpLac;
        while ( match == KErrNotFound ) // Match is found or we are end of string.
            {
            NWLOGSTRING( KNWOBJECT, 
                "NW: CNWNetworkViagLcEngine::HandleLACMatch()  *2*" );
            if( cursorPos == dataLen )
                {
                NWLOGSTRING( KNWOBJECT, 
                    "NW: CNWNetworkViagLcEngine::HandleLACMatch()  *3*" );
                break;
                }
            NWLOGSTRING( KNWOBJECT, 
                "NW: CNWNetworkViagLcEngine::HandleLACMatch()  *4 *" );
            // Get the LAC id as TUint...
            TUint unit( iCurrentLcCache->iCache[aSectionIndex].iField[aIndex].iUnit[cursorPos] );
            // ... and expand TUint into string for the comparisation operations
            workBuf.Zero();
            ExtendTUintIntoBuffer( workBuf, unit );
            cmpLac.Set( workBuf );


            NWLOGSTRING2( KNWOBJECT, 
                "NW: CNWNetworkViagLcEngine::HandleLACMatch()  LAC ID %S FROM \
                LAC FILE", &cmpLac );
            NWLOGSTRING2( KNWOBJECT, 
                "NW: CNWNetworkViagLcEngine::HandleLACMatch()  LAC ID %S FROM \
                NETWORK", &aLacBuf ); 
            if ( cmpLac.Length() > 0 )
                {  
                if ( aLacBuf == cmpLac )
                    {
                    NWLOGSTRING( KNWOBJECT, 
                        "NW: CNWNetworkViagLcEngine::HandleLACMatch()  *5* LAC \
                        MATCH FOUND" );
                    match = KErrNone;
                    WriteViagValues( aSectionIndex );
                    }
                }
            cursorPos++;    
            } // ******************* while END ***********************
        }// If lac is found then all cell ids are ok...

    NWLOGSTRING( KNWOBJECT, 
        "NW: CNWNetworkViagLcEngine::HandleLACMatch() End " );
    }

// -----------------------------------------------------------------------------
// CNWNetworkViagLcEngine::CompareTIntCellIdValues
// 
// -----------------------------------------------------------------------------
// 
TInt CNWNetworkViagLcEngine::CompareTIntCellIdValues( 
        const TDesC& aCell, 
        const TDesC& aCmpCell )
    {
    NWLOGSTRING3( KNWOBJECT, 
        "NW: CNWNetworkViagLcEngine::CompareTIntCellIdValues() Begin,\
        aCell.length() = %d, aCmpCell.length() = %d ",
        aCell.Length(), aCmpCell.Length() );
    
    TLex tempCellId( aCell.Right( KCmpLength ) );
    TLex tempCmpCellId ( aCmpCell.Right( KCmpLength ) );
                             
    TInt cellid;
    TInt cmpcellid;
    TInt err = tempCellId.Val( cellid );
    err = tempCmpCellId.Val( cmpcellid );
    
    if ( err == KErrNone && cellid == cmpcellid )
        {
        NWLOGSTRING3( KNWOBJECT, 
            "NW: CNWNetworkViagLcEngine::CompareTIntCellIdValues(),\
            err = %d, cellid = %d ", err, cellid );
        return KErrNone;
        }
    else 
        {
        NWLOGSTRING( KNWOBJECT, 
            "NW: CNWNetworkViagLcEngine::CompareTIntCellIdValues(),\
            values not match " );
        return KErrNotFound;
        }
    }

//  End of File  
