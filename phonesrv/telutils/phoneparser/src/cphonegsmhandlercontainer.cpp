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
* Description:  Implementation of handler container.
*
*/


// INCLUDE FILES
#include    "cphonegsmhandlercontainer.h" 
#include    "phonegsmparser.h" 

// CONSTANTS
const TInt KPhoneHandlerArrayGranularity = 10;
const TInt KPhoneHandlerArrayReserve = 10;

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CPhoneGsmHandlerContainer::CPhoneGsmHandlerContainer
// -----------------------------------------------------------------------------
//
CPhoneGsmHandlerContainer::CPhoneGsmHandlerContainer()
    {
    }

// -----------------------------------------------------------------------------
// CPhoneGsmHandlerContainer::ConstructL
// -----------------------------------------------------------------------------
//
void CPhoneGsmHandlerContainer::ConstructL()
    {
    iHandlers = 
        new ( ELeave ) CHandlerArray( KPhoneHandlerArrayGranularity );
    iHandlers->SetReserveL( KPhoneHandlerArrayReserve );
    }

// -----------------------------------------------------------------------------
// CPhoneGsmHandlerContainer::NewL
// -----------------------------------------------------------------------------
//
EXPORT_C CPhoneGsmHandlerContainer* CPhoneGsmHandlerContainer::NewL()
    {
    CPhoneGsmHandlerContainer* self = 
        new (ELeave) CPhoneGsmHandlerContainer;
    
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop( self );

    return self;
    }

// -----------------------------------------------------------------------------
// CPhoneGsmHandlerContainer::~CPhoneGsmHandlerContainer
// -----------------------------------------------------------------------------
//
EXPORT_C CPhoneGsmHandlerContainer::~CPhoneGsmHandlerContainer()
    {
    if ( iHandlers )
        {
        iHandlers->ResetAndDestroy();
        }
    delete iHandlers;
    }

// -----------------------------------------------------------------------------
// CPhoneGsmHandlerContainer::AddHandlerL
// -----------------------------------------------------------------------------
//
EXPORT_C void CPhoneGsmHandlerContainer::AddHandlerL( 
        CPhoneGsmHandlerBase& aHandler )
    {
    TInt index;
    TInt count = iHandlers->Count();

    for ( index = 0; index < count; index++ )
        {
        CPhoneGsmHandlerBase* handler = iHandlers->At( index );

        if ( handler == &aHandler )
            {
            PhoneGsmParser::Panic( PhoneGsmParser::EIncorrectUse );
            }
        }

    iHandlers->AppendL( &aHandler );
    }

// -----------------------------------------------------------------------------
// CPhoneGsmHandlerContainer::ProcessL
// -----------------------------------------------------------------------------
//
void CPhoneGsmHandlerContainer::ProcessL( 
        CPhoneGsmParserResult& aResult )
    {
    TInt index;
    TInt count = iHandlers->Count();

    for ( index = 0; index < count; index++ )
        {
        iHandlers->At( index )->ProcessL( aResult );
        }
    }

//  End of File  
