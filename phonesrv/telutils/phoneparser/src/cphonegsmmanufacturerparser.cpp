/*
* Copyright (c) 2002 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  Implementation of manufacturer parser.
*
*/


// INCLUDE FILES
#include    "cphonegsmmanufacturerparser.h" 
#include    "cphonegsmoptioncontainer.h" 
#include    "cphonegsmparserresult.h" 
#include    "cphoneparserfeatures.h" 

// CONSTANTS
const TInt KPhoneManufacturerCodeArrayGanularity = 5;

_LIT( KPhoneHashStr, "#" );
#ifdef PHONEPARSER_PARSE_DEBUG_CODE
_LIT( KPhoneDebugPrefixStr, "*#74603" );
#endif

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CPhoneGsmManufacturerParser::CPhoneGsmManufacturerParser
// -----------------------------------------------------------------------------
//
CPhoneGsmManufacturerParser::CPhoneGsmManufacturerParser()
    {
    }

// -----------------------------------------------------------------------------
// CPhoneGsmManufacturerParser::ConstructL
// -----------------------------------------------------------------------------
//
void CPhoneGsmManufacturerParser::ConstructL()
    {
    iArray = new ( ELeave ) CCodeArray( 
        KPhoneManufacturerCodeArrayGanularity );
    }

// -----------------------------------------------------------------------------
// CPhoneGsmManufacturerParser::NewLC
// -----------------------------------------------------------------------------
//
CPhoneGsmManufacturerParser* CPhoneGsmManufacturerParser::NewLC()
    {
    CPhoneGsmManufacturerParser* self = 
        new ( ELeave ) CPhoneGsmManufacturerParser;
    
    CleanupStack::PushL( self );
    self->ConstructL();

    return self;
    }

// -----------------------------------------------------------------------------
// CPhoneGsmManufacturerParser::~CPhoneGsmManufacturerParser
// -----------------------------------------------------------------------------
//
CPhoneGsmManufacturerParser::~CPhoneGsmManufacturerParser()
    { 
    delete iArray;
    }

// -----------------------------------------------------------------------------
// CPhoneGsmManufacturerParser::AddStringL
// -----------------------------------------------------------------------------
//
void CPhoneGsmManufacturerParser::AddStringL( 
        const TDesC& aString, 
        TInt aFlags, 
        TUint aCommand,
        TInt aFeatureId )
    {
    TCodeInfo info;
    info.iString.Set( aString );
    info.iFlags = aFlags;
    info.iCommand = aCommand;
    info.iFeatureId = aFeatureId;

    iArray->AppendL( info );
    }

// -----------------------------------------------------------------------------
// CPhoneGsmManufacturerParser::ParseL
// -----------------------------------------------------------------------------
//
TBool CPhoneGsmManufacturerParser::ParseL( 
        const TDesC& aString,
        CPhoneGsmParserResult& aResult,
        CPhoneGsmOptionContainerBase& aOptions )
    {
    TBool result = EFalse;
    
    aResult.ClearL();

    TInt index;
    TInt count = iArray->Count();

    // Go through all elements.
    for ( index = 0; index < count; index++ )
        {
        TCodeInfo& info = iArray->At( index ); 

        // Check if string matches to code information.
        if ( CheckCode( aString, info, aOptions ) ) 
            {
            // Yes, fill result
            aResult.SetUid( KPhoneUidManufacturerCode );
            aResult.SetAuxInformation( info.iCommand );
            result = ETrue;
            break;
            }
        }

#ifdef PHONEPARSER_PARSE_DEBUG_CODE
    if ( !result )
        {
        // Check special debug code.

        if ( IsPrefixOf( aString, KPhoneDebugPrefixStr ) && 
             IsPostfixOf( aString, KPhoneHashStr ) )
            {
            aResult.SetUid( KPhoneUidManufacturerDebugCode );
            aResult.SetAuxInformation( 0 );
            aResult.AddParameterL( 
                aString.Mid( 
                    KPhoneDebugPrefixStr().Length(), 
                    aString.Length() - KPhoneDebugPrefixStr().Length() - 1 ) );
            result = ETrue;
            }
        }
#endif

    return result;
    }

// -----------------------------------------------------------------------------
// CPhoneGsmManufacturerParser::CheckCode
// -----------------------------------------------------------------------------
//
TBool CPhoneGsmManufacturerParser::CheckCode( 
        const TDesC& aString, 
        const TCodeInfo& aInfo,
        CPhoneGsmOptionContainerBase& aOptions )
    {
    TInt flags = aInfo.iFlags;

    if ( !aString.Length() )
        {
        return EFalse;
        }

    if ( !CheckSituation( aString, flags, aOptions ) )
        {
        return EFalse;
        }
    
    if ( aInfo.iFeatureId )
        {
        if ( !CPhoneParserFeatures::FeatureSupported( aInfo.iFeatureId ) )
            {
            return EFalse;
            }
        }

    // Check if string is ok.
    if ( ( flags & EFlagPrefix ) )
        {
        return ( IsPrefixOf( aString, aInfo.iString ) );
        }
    else
        {
        return ( aString == aInfo.iString );
        }
    }

// -----------------------------------------------------------------------------
// CPhoneGsmManufacturerParser::CheckSituation
// -----------------------------------------------------------------------------
//
TBool CPhoneGsmManufacturerParser::CheckSituation( 
        const TDesC& aString,
        TInt aFlags,
        CPhoneGsmOptionContainerBase& aOptions )
    {
    TBool result = ETrue;

    // Check if it is send operation or hash check.
    TBool send = aOptions.FindOptionStatus( KPhoneOptionSend );

    TBool isCode = aFlags & EFlagCode;
    TBool isSend = aFlags & EFlagSend;

    if ( isCode && send )
        {
        result = EFalse;
        }
    else if ( isCode && !send )
        {
        // For code, there must be hash.

        if ( aString.Right( 1 ) != KPhoneHashStr )  // Compare rightmost characters
            {
            result = EFalse;
            }
        }

    if ( isSend && !send )
        {
        result = EFalse;
        }
    
    // Check if phone is in correct state.
    TBool incall = aOptions.FindOptionStatus( KPhoneOptionInCall );

    if ( ( aFlags & EFlagNotInIdle ) && !incall )
        {
        result = EFalse;
        }

    if ( ( aFlags & EFlagNotInInCall ) && incall )
        {
        result = EFalse;
        }

    return result;
    }

//  End of File

