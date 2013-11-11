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
* Description:  Implementation of option container.
*
*/


// INCLUDE FILES
#include    "cphonegsmoptioncontainer.h" 
#include    "phonegsmparser.h" 
#include    "cphoneparserfeatures.h" 

// CONSTANTS
const TInt KPhoneGsmOptionGranularity = 5;


// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CPhoneGsmOptionContainer::CPhoneGsmOptionContainer
// -----------------------------------------------------------------------------
//
CPhoneGsmOptionContainer::CPhoneGsmOptionContainer()
    {
    }

// -----------------------------------------------------------------------------
// CPhoneGsmOptionContainer::ConstructL
// -----------------------------------------------------------------------------
//
void CPhoneGsmOptionContainer::ConstructL()
    {
    iOptions = 
        new ( ELeave ) COptionArray( KPhoneGsmOptionGranularity );
    }

// -----------------------------------------------------------------------------
// CPhoneGsmOptionContainer::NewL
// -----------------------------------------------------------------------------
//
CPhoneGsmOptionContainer* CPhoneGsmOptionContainer::NewL()
    {
    CPhoneGsmOptionContainer* self = 
        new (ELeave) CPhoneGsmOptionContainer;

    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop( self );

    return self;
    }

// -----------------------------------------------------------------------------
// CPhoneGsmOptionContainer::~CPhoneGsmOptionContainer
// -----------------------------------------------------------------------------
//
CPhoneGsmOptionContainer::~CPhoneGsmOptionContainer()
    {
    delete iOptions;
    }

// -----------------------------------------------------------------------------
// CPhoneGsmOptionContainer::IsOptionDefined
// -----------------------------------------------------------------------------
//
TBool CPhoneGsmOptionContainer::IsOptionDefined( 
        TInt aOptionUid ) const
    {
    // For two digit calling, we have one option defined
    // in extension.
    if ( aOptionUid == KPhoneOptionTwoDigitCalling )
        {
        return ETrue;
        }

    TKeyArrayFix key = MakeKey();
    TOptionItem option;
    option.iUid = aOptionUid;
    TInt index;

    return !iOptions->FindIsq( option, key, index );
    }

// -----------------------------------------------------------------------------
// CPhoneGsmOptionContainer::FindOptionStatus
// -----------------------------------------------------------------------------
//
TBool CPhoneGsmOptionContainer::FindOptionStatus( 
        TInt aOptionUid ) const
    {
    // For two digit calling, we have one option defined
    // in extension.
    if ( aOptionUid == KPhoneOptionTwoDigitCalling )
        {
        return CPhoneParserFeatures::TwoDigitCallingEnabled();
        }

    TKeyArrayFix key = MakeKey();
    TOptionItem option;
    option.iUid = aOptionUid;
    TInt index;

    if ( iOptions->FindIsq( option, key, index ) )
        {
        PhoneGsmParser::Panic( PhoneGsmParser::EOptionNotDefined );
        }

    TBool result = iOptions->At( index ).iStatus;
    return result;
    }

// -----------------------------------------------------------------------------
// CPhoneGsmOptionContainer::SetOptionStatus
// -----------------------------------------------------------------------------
//
void CPhoneGsmOptionContainer::SetOptionStatus( 
        TInt aOptionUid, 
        TBool aStatus )
    {
    // For two digit calling, we have one option defined
    // in extension - not allowed to be modified.
    if ( aOptionUid == KPhoneOptionTwoDigitCalling )
        {
        PhoneGsmParser::Panic( PhoneGsmParser::EIncorrectUse );
        }

    TKeyArrayFix key = MakeKey();
    TOptionItem option;
    option.iUid = aOptionUid;
    TInt index;

    if ( iOptions->FindIsq( option, key, index ) )
        {
        PhoneGsmParser::Panic( PhoneGsmParser::EOptionNotDefined );
        }

    iOptions->At( index ).iStatus = aStatus;
    }

// -----------------------------------------------------------------------------
// CPhoneGsmOptionContainer::DefineOptionL
// -----------------------------------------------------------------------------
//
void CPhoneGsmOptionContainer::DefineOptionL( 
        TInt aOptionUid, 
        TBool aStatus )
    {
    if ( IsOptionDefined( aOptionUid ) )
        {
        PhoneGsmParser::Panic( PhoneGsmParser::EOptionAlreadyDefined );
        }

    TKeyArrayFix key = MakeKey();
    TOptionItem option;
    option.iUid = aOptionUid;
    option.iStatus = aStatus;

    iOptions->InsertIsqL( option, key );
    }   

// -----------------------------------------------------------------------------
// CPhoneGsmOptionContainer::MakeKey
// -----------------------------------------------------------------------------
//
TKeyArrayFix CPhoneGsmOptionContainer::MakeKey()
    {
    return TKeyArrayFix( _FOFF( TOptionItem, iUid ), ECmpTInt );
    }

//  End of File  
