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
* Description:  Methods for CAiwDialDataExt class.
*
*/


// INCLUDE FILES
#include    "aiwdialdataext.h"
#include    "aiwinternaldialdata.h"

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CAiwDialDataExt::CAiwDialDataExt
// C++ default constructor can NOT contain any code, that
// might leave.
// -----------------------------------------------------------------------------
//
CAiwDialDataExt::CAiwDialDataExt() 
    {
    }

// -----------------------------------------------------------------------------
// CAiwDialDataExt::NewL
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
EXPORT_C CAiwDialDataExt* CAiwDialDataExt::NewL()
    {
    CAiwDialDataExt* self = CAiwDialDataExt::NewLC();
    CleanupStack::Pop( self );
    return self;
    }

// -----------------------------------------------------------------------------
// CAiwDialDataExt::NewLC
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
EXPORT_C CAiwDialDataExt* CAiwDialDataExt::NewLC()
    {
    CAiwDialDataExt* self = new( ELeave ) CAiwDialDataExt;
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
void CAiwDialDataExt::ConstructL()
    {
    iInternalDialData = CAiwInternalDialData::NewL();
    }
    
// Destructor
CAiwDialDataExt::~CAiwDialDataExt()
    {
    delete iInternalDialData;
    iInternalDialData = NULL;
    }

// ---------------------------------------------------------------------------
// Fills the parameter list.
// ---------------------------------------------------------------------------
//
EXPORT_C void CAiwDialDataExt::FillInParamListL( 
    CAiwGenericParamList& aInParamList )
    {
    iInternalDialData->FillInParamListL( aInParamList );
    }
        
// ---------------------------------------------------------------------------
// Sets phone number.
// ---------------------------------------------------------------------------
//
EXPORT_C void CAiwDialDataExt::SetPhoneNumberL( const TDesC& aPhoneNumber ) 
    {
    iInternalDialData->SetPhoneNumberL( aPhoneNumber );
    }

// ---------------------------------------------------------------------------
// Sets call type.
// ---------------------------------------------------------------------------
//   
EXPORT_C void CAiwDialDataExt::SetCallType( CAiwDialData::TCallType aCallType ) 
    {
    iInternalDialData->SetCallType( aCallType );
    }

// ---------------------------------------------------------------------------
// Sets window group.
// ---------------------------------------------------------------------------
//    
EXPORT_C void CAiwDialDataExt::SetWindowGroup( TInt aWindowGroup ) 
    {
    iInternalDialData->SetWindowGroup( aWindowGroup );
    }

// ---------------------------------------------------------------------------
// Set initiate call.
// ---------------------------------------------------------------------------
//    
EXPORT_C void CAiwDialDataExt::SetInitiateCall( TBool aInitCall )
    {
    iInternalDialData->SetInitiateCall( aInitCall );
    }
   
// ---------------------------------------------------------------------------
// Set name.
// ---------------------------------------------------------------------------
//    
EXPORT_C void CAiwDialDataExt::SetNameL( const TDesC& aName ) 
    {
    iInternalDialData->SetNameL( aName );
    }
    
// ---------------------------------------------------------------------------
// Sets redial value.
// ---------------------------------------------------------------------------
//    
EXPORT_C void CAiwDialDataExt::SetRedial( TInt aRedial ) 
    {
    iInternalDialData->SetRedial( aRedial );
    }
 
// ---------------------------------------------------------------------------
// Set show number.
// ---------------------------------------------------------------------------
//    
EXPORT_C void CAiwDialDataExt::SetShowNumber( TBool aShowNumber ) 
    {
    iInternalDialData->SetShowNumber( aShowNumber );
    }
        
// ---------------------------------------------------------------------------
// Sets used contact link.
// ---------------------------------------------------------------------------
//
EXPORT_C void CAiwDialDataExt::SetContactLinkL( const TDesC8& aContactLink ) 
    {
    iInternalDialData->SetContactLinkL( aContactLink );
    }
    
// -----------------------------------------------------------------------------
// Sets used service id.
// -----------------------------------------------------------------------------
//
EXPORT_C void CAiwDialDataExt::SetServiceId( TUint32 aServiceId )
    {
    return iInternalDialData->SetServiceId( aServiceId );
    }    


// ---------------------------------------------------------------------------
// Returns phone number.
// ---------------------------------------------------------------------------
//    
EXPORT_C const TDesC& CAiwDialDataExt::PhoneNumber() const 
    {
    return iInternalDialData->PhoneNumber();  
    }
    
// ---------------------------------------------------------------------------
// Returns call type.
// ---------------------------------------------------------------------------
//    
EXPORT_C CAiwDialData::TCallType CAiwDialDataExt::CallType() const 
    {
    return iInternalDialData->CallType();
    }

// ---------------------------------------------------------------------------
// Return window group.
// ---------------------------------------------------------------------------
//    
EXPORT_C TInt CAiwDialDataExt::WindowGroup() const
    {
    return iInternalDialData->WindowGroup();
    }

// ---------------------------------------------------------------------------
// Returns initiate call.
// ---------------------------------------------------------------------------
//
EXPORT_C TBool CAiwDialDataExt::InitiateCall() const 
    {
    return iInternalDialData->InitiateCall();
    }
     
// ---------------------------------------------------------------------------
// Returns name.
// ---------------------------------------------------------------------------
//
EXPORT_C const TDesC& CAiwDialDataExt::Name() const 
    {
    return iInternalDialData->Name();
    }
     
// ---------------------------------------------------------------------------
// Return redial value.
// ---------------------------------------------------------------------------
//    
EXPORT_C TInt CAiwDialDataExt::Redial() const 
    {
    return iInternalDialData->Redial();
    }
// ---------------------------------------------------------------------------
// Returns show number.
// ---------------------------------------------------------------------------
//   
EXPORT_C TBool CAiwDialDataExt::ShowNumber() const 
    {
    return iInternalDialData->ShowNumber();
    }
    
// ---------------------------------------------------------------------------
// Return contact link.
// ---------------------------------------------------------------------------
//    
EXPORT_C const TDesC8& CAiwDialDataExt::ContactLink() const 
    {
    return iInternalDialData->ContactLink();
    }  

// -----------------------------------------------------------------------------
// Reads used service id.
// -----------------------------------------------------------------------------
//
EXPORT_C TUint32 CAiwDialDataExt::ServiceId() const
    {
    return iInternalDialData->ServiceId();
    }

//  End of File  
