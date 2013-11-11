/*
* Copyright (c) 2005-2006 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  Methods for CAiwDialData class.
*
*/


// INCLUDE FILES
#include    "aiwdialdata.h"
#include    "aiwinternaldialdata.h"

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CAiwDialData::CAiwDialData
// C++ default constructor can NOT contain any code, that
// might leave.
// -----------------------------------------------------------------------------
//
CAiwDialData::CAiwDialData()
    {
    }

// -----------------------------------------------------------------------------
// CAiwDialData::NewL
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
EXPORT_C CAiwDialData* CAiwDialData::NewL()
    {
    CAiwDialData* self = CAiwDialData::NewLC();
    CleanupStack::Pop( self );
    return self;
    }

// -----------------------------------------------------------------------------
// CAiwDialData::NewLC
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
EXPORT_C CAiwDialData* CAiwDialData::NewLC()
    {
    CAiwDialData* self = new( ELeave ) CAiwDialData;
    CleanupStack::PushL( self );
    self->ConstructL();
    return self;
    }

// ---------------------------------------------------------------------------
// 
// Symbian OS constructor
// 
// ---------------------------------------------------------------------------
//
void CAiwDialData::ConstructL()
    {
    iInternalDialData = CAiwInternalDialData::NewL();
    }
    
// Destructor
CAiwDialData::~CAiwDialData()
    {
    delete iInternalDialData;
    iInternalDialData = NULL;
    }

// ---------------------------------------------------------------------------
// Fills the parameter list.
// ---------------------------------------------------------------------------
//
EXPORT_C void CAiwDialData::FillInParamListL( 
    CAiwGenericParamList& aInParamList )
    {
    iInternalDialData->FillInParamListL( aInParamList );
    }

// ---------------------------------------------------------------------------
// Sets phone number.
// ---------------------------------------------------------------------------
//
EXPORT_C void CAiwDialData::SetPhoneNumberL( const TDesC& aPhoneNumber ) 
    {
    iInternalDialData->SetPhoneNumberL( aPhoneNumber );
    }

// ---------------------------------------------------------------------------
// Sets call type.
// ---------------------------------------------------------------------------
//   
EXPORT_C void CAiwDialData::SetCallType( TCallType aCallType ) 
    {
    iInternalDialData->SetCallType( aCallType );
    }

// ---------------------------------------------------------------------------
// Sets window group.
// ---------------------------------------------------------------------------
//    
EXPORT_C void CAiwDialData::SetWindowGroup( TInt aWindowGroup ) 
    {
    iInternalDialData->SetWindowGroup( aWindowGroup );
    }

// ---------------------------------------------------------------------------
// Returns phone number.
// ---------------------------------------------------------------------------
//    
EXPORT_C const TDesC& CAiwDialData::PhoneNumber() const 
    {
    return iInternalDialData->PhoneNumber();   
    }
    
// ---------------------------------------------------------------------------
// Returns call type.
// ---------------------------------------------------------------------------
//    
EXPORT_C CAiwDialData::TCallType CAiwDialData::CallType() const 
    {
    return iInternalDialData->CallType();
    }

// ---------------------------------------------------------------------------
// Return window group.
// ---------------------------------------------------------------------------
//    
EXPORT_C TInt CAiwDialData::WindowGroup() const
    {
    return iInternalDialData->WindowGroup();
    }
    
//  End of File  
