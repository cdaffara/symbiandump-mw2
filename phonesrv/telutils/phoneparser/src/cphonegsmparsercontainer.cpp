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
* Description:  Container for parsers.
*
*/


// INCLUDE FILES
#include    <featmgr.h>

#include    "cphonegsmparsercontainer.h" 
#include    "cphonegsmparserresult.h" 
#include    "cphonegsmoptioncontainer.h" 
#include    "phonegsmparser.h" 

// CONSTANTS
const TInt KPhoneParserArrayGranularity = 5;

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CPhoneGsmParserContainer::CPhoneGsmParserContainer
// -----------------------------------------------------------------------------
//
CPhoneGsmParserContainer::CPhoneGsmParserContainer()
    {
    }

// -----------------------------------------------------------------------------
// CPhoneGsmParserContainer::ConstructL
// -----------------------------------------------------------------------------
//
void CPhoneGsmParserContainer::ConstructL()
    {
    iArray = 
        new ( ELeave ) CParserArray( KPhoneParserArrayGranularity );
    }

// -----------------------------------------------------------------------------
// CPhoneGsmParserContainer::NewLC
// -----------------------------------------------------------------------------
//
CPhoneGsmParserContainer* CPhoneGsmParserContainer::NewLC()
    {
    CPhoneGsmParserContainer* self = 
        new ( ELeave ) CPhoneGsmParserContainer;
    
    CleanupStack::PushL( self );
    self->ConstructL();
    
    return self;
    }

// -----------------------------------------------------------------------------
// CPhoneGsmParserContainer::~CPhoneGsmParserContainer
// -----------------------------------------------------------------------------
//
CPhoneGsmParserContainer::~CPhoneGsmParserContainer()
    {
    if ( iArray )
        {
        iArray->ResetAndDestroy();
        }
    delete iArray;
    }

// -----------------------------------------------------------------------------
// CPhoneGsmParserContainer::AddL
// -----------------------------------------------------------------------------
//
void CPhoneGsmParserContainer::AddL(
        CPhoneGsmParserBase& aParser )
    {
    TInt index;
    TInt count = iArray->Count();

    for ( index = 0; index < count; index++ )
        {
        CPhoneGsmParserBase* parser = iArray->At( index );

        if ( parser == &aParser )
            {
            PhoneGsmParser::Panic( PhoneGsmParser::EIncorrectUse );
            }
        }

    iArray->AppendL( &aParser );
    }

// -----------------------------------------------------------------------------
// CPhoneGsmParserContainer::ParseL
// -----------------------------------------------------------------------------
//
TBool CPhoneGsmParserContainer::ParseL( 
        const TDesC& aString,
        CPhoneGsmParserResult& aResult,
        CPhoneGsmOptionContainerBase& aOptions )
    {
    TInt index;
    TInt count = iArray->Count();
    TBool result = EFalse;

    for ( index = 0; index < count; index++ )
        {
        CPhoneGsmParserBase* parser = iArray->At( index );

        result = parser->ParseL( aString, aResult, aOptions );

        if ( result )
            {
            break;
            }
        }

    return result;
    }

//  End of File  
