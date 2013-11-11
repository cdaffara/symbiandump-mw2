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
* Description:  Methods for call paramsclass.
*
*/


// INCLUDE FILES
#include <e32base.h>
#include <mccpcall.h>
#include <cccpcallparameters.h>




// ============================ MEMBER FUNCTIONS ===============================


// -----------------------------------------------------------------------------
// Ccallprovider::NewL
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
EXPORT_C CCCPCallParameters* CCCPCallParameters::NewL()
    {
    CCCPCallParameters* self = new(ELeave)CCCPCallParameters();
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop( self );
    return self;
    }

// -----------------------------------------------------------------------------
// Ccallprovider::NewLC
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
EXPORT_C CCCPCallParameters::~CCCPCallParameters()
    {
    }

CCCPCallParameters* CCCPCallParameters::CloneL() const
    {
    CCCPCallParameters* clone = CCCPCallParameters::NewL();
    clone->SetServiceId( iServiceId );
    clone->SetCallType( iCallType );
    return clone;    
    }

EXPORT_C void CCCPCallParameters::SetServiceId(TUint32 aServiceId)
    {
    iServiceId = aServiceId;
    }


EXPORT_C TUint32 CCCPCallParameters::ServiceId() const
    {
    return iServiceId;
    }


EXPORT_C void CCCPCallParameters::SetCallType(CCPCall::TCallType aCallType)
    {
    iCallType = aCallType;
    }


EXPORT_C CCPCall::TCallType CCCPCallParameters::CallType() const
    {
    return iCallType;
    }

EXPORT_C CCCPCallParameters::CCCPCallParameters()
    {
    }


EXPORT_C void CCCPCallParameters::ConstructL()
    {
    }

