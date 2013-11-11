/*
* Copyright (c) 2006 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  Header of DRMConfigIntfcImpl object.
*
*/


#include "DRMConfigIntfcImpl.h"

// ---------------------------------------------------------------------------
// Contructor
// ---------------------------------------------------------------------------
CDRMConfigIntcfImpl::CDRMConfigIntcfImpl( CClientDataBufferSource& aDRMHandler )
:iDRMHandler(&aDRMHandler)
    {
    // No Impl
    }

// ---------------------------------------------------------------------------
// Destructor
// ---------------------------------------------------------------------------
CDRMConfigIntcfImpl::~CDRMConfigIntcfImpl()    
    {
    // No Impl
    }

// ---------------------------------------------------------------------------
// NewL Method
// ---------------------------------------------------------------------------
CDRMConfigIntcfImpl* CDRMConfigIntcfImpl::NewL( CClientDataBufferSource& aDRMHandler )
    {
    CDRMConfigIntcfImpl* self = new (ELeave)CDRMConfigIntcfImpl(aDRMHandler);
    CleanupStack::PushL(self);
    self->ConstructL();
    CleanupStack::Pop(self);
    return self;
    }

// ---------------------------------------------------------------------------
// Second Phase Contructor
// ---------------------------------------------------------------------------
void CDRMConfigIntcfImpl::ConstructL()
    {
    // No Impl
    }

// ---------------------------------------------------------------------------
// GetDRMType: This method gets the DRMType stored in the Client
// ---------------------------------------------------------------------------
TInt CDRMConfigIntcfImpl::GetDRMType( TDRMType& aDRMType )
    {
    return iDRMHandler->GetDRMType(aDRMType);
    }

// ---------------------------------------------------------------------------
// SetDRMType: This method Sets the DRMType stored in the Client
// ---------------------------------------------------------------------------
TInt CDRMConfigIntcfImpl::SetDRMType( TDRMType aDRMType )
    {
    return iDRMHandler->SetDRMType(aDRMType);
    }

// ---------------------------------------------------------------------------
// GetAllowedOutputDeviceCount
// ---------------------------------------------------------------------------
TInt CDRMConfigIntcfImpl::GetAllowedOutputDeviceCount( TInt& aCount )
    {
    return iDRMHandler->GetAllowedOutputDeviceCount(aCount);
    }


// ---------------------------------------------------------------------------
// GetAllowedOutputDevice
// ---------------------------------------------------------------------------
TInt CDRMConfigIntcfImpl::GetAllowedOutputDevice(
                TInt aIndex,
                TDRMAllowedOutputDevice& aOutputDevice )
    {
    return iDRMHandler->GetAllowedOutputDevice( aIndex, aOutputDevice );
    }

// ---------------------------------------------------------------------------
// AppendAllowedOutputDevice
// ---------------------------------------------------------------------------
TInt CDRMConfigIntcfImpl::AppendAllowedOutputDevice(
                        TDRMAllowedOutputDevice aOutputDevice )
    {
    return iDRMHandler->AppendAllowedOutputDevice(aOutputDevice);
    }

// ---------------------------------------------------------------------------
// RemoveAllowedOutputDevice
// ---------------------------------------------------------------------------
TInt CDRMConfigIntcfImpl::RemoveAllowedOutputDevice(
                        TDRMAllowedOutputDevice aOutputDevice )
    {
    return iDRMHandler->RemoveAllowedOutputDevice(aOutputDevice);
    }

// ---------------------------------------------------------------------------
// Reset
// ---------------------------------------------------------------------------
TInt CDRMConfigIntcfImpl::Reset()
    {
    return iDRMHandler->ResetDRMConfig();
    }

// ---------------------------------------------------------------------------
// Commit
// ---------------------------------------------------------------------------
TInt CDRMConfigIntcfImpl::Commit()
    {
    return iDRMHandler->CommitDRMConfig();
    }

// End of file
