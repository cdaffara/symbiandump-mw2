/*
* Copyright (c) 2008 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  Call params
*
*/

//  INCLUDE FILES
#include <e32base.h>
#include <cccecallparameters.h>



// ======== MEMBER FUNCTIONS ========
// ---------------------------------------------------------------------------
// Constructor
// ---------------------------------------------------------------------------
//
    
// ---------------------------------------------------------------------------
// Creates the params object.
// ---------------------------------------------------------------------------
//
EXPORT_C CCCECallParameters* CCCECallParameters::NewL()
    { 
    CCCECallParameters* self = new(ELeave)CCCECallParameters();
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop( self );
    return self;
    }

// -----------------------------------------------------------------------------
// CCCECallParameters::~CCCECallParameters
// -----------------------------------------------------------------------------
//
CCCECallParameters::~CCCECallParameters()
    {
    }

// -----------------------------------------------------------------------------
// CCCECallParameters::CloneL
// -----------------------------------------------------------------------------
//
CCCPCallParameters* CCCECallParameters::CloneL() const 
    {
    CCCECallParameters* cloned = CCCECallParameters::NewL();
    // Base class data
    cloned->SetCallType(CallType());
    cloned->SetServiceId(ServiceId());
    
    // Self data
    cloned->SetBearer(iBearer);
    cloned->SetSubAddress(iSubAddress);
    cloned->SetLineType(iLineType);
    cloned->SetUUSId(iUUSId);
    cloned->SetOrigin(iOrigin);
    cloned->SetAlphaId(iAlphaId);
    return cloned;
    }

// -----------------------------------------------------------------------------
// CCCECallParameters::CCCECallParameters
// -----------------------------------------------------------------------------
//
CCCECallParameters::CCCECallParameters()
    {
    }

// -----------------------------------------------------------------------------
// CCCECallParameters::ConstructL
// -----------------------------------------------------------------------------
//
void CCCECallParameters::ConstructL()
    {
    CCCPCallParameters::ConstructL();
    }
    
// -----------------------------------------------------------------------------
// Sets the line type of call.
// -----------------------------------------------------------------------------
//
void CCCECallParameters::SetLineType(CCCECallParameters::TCCELineType aLineType) 
    { 
    iLineType = aLineType; 
    }

// -----------------------------------------------------------------------------
// Gets the line type of the call.
// -----------------------------------------------------------------------------
//
CCCECallParameters::TCCELineType CCCECallParameters::LineType() const 
    {
    return iLineType;
    }

// -----------------------------------------------------------------------------
// Sets the service id identifying the service provider of the call.
// -----------------------------------------------------------------------------
//
void CCCECallParameters::SetUUSId(TBuf<KCCPUUSIdSize> aUUSId)
    {
    iUUSId = aUUSId;
    }

// -----------------------------------------------------------------------------
// Gets the UUS id of the call.
// -----------------------------------------------------------------------------
//
TBuf<KCCPUUSIdSize> CCCECallParameters::UUSId() const 
    {
    return iUUSId;
    }

// -----------------------------------------------------------------------------
// Sets the bearer of the call.
// -----------------------------------------------------------------------------
//
void CCCECallParameters::SetBearer(TBuf8<KCCEBearerMaxSize> aBearer) 
    {
    iBearer = aBearer;
    }

// -----------------------------------------------------------------------------
// Gets the bearer of the call.
// -----------------------------------------------------------------------------
//
TBuf8<KCCEBearerMaxSize> CCCECallParameters::Bearer() const 
    {
    return iBearer;
    }

// -----------------------------------------------------------------------------
// Sets the sub address of the dialled phone number.
// -----------------------------------------------------------------------------
//
void CCCECallParameters::SetSubAddress(TBuf<KCCESubAddressMaxSize> aSubAddress) 
    {
    iSubAddress = aSubAddress;
    }

// -----------------------------------------------------------------------------
// Gets the sub address of the dialled phone number.
// -----------------------------------------------------------------------------
//
TBuf<KCCESubAddressMaxSize> CCCECallParameters::SubAddress() const 
    {
    return iSubAddress;
    }

// -----------------------------------------------------------------------------
// Sets the origin of the call.
// -----------------------------------------------------------------------------
//
void CCCECallParameters::SetOrigin(CCCECallParameters::TCCECallOrigin aCallOrigin) 
    {
    iOrigin = aCallOrigin;
    }

// -----------------------------------------------------------------------------
// Gets the origin of the call.
// -----------------------------------------------------------------------------
//
CCCECallParameters::TCCECallOrigin CCCECallParameters::Origin() const 
    {
    return iOrigin;
    }

void CCCECallParameters::SetAlphaId(TBuf<KCCPAlphaIdMaxSize> aAlphaId)
    {
    iAlphaId = aAlphaId;
    }


TBuf<KCCPAlphaIdMaxSize> CCCECallParameters::AlphaId() const
    {
    return iAlphaId;
    }

// End of file
