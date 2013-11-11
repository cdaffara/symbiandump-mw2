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
#include "DataBufferDataSource.h"

CDRMConfigIntcfImpl::CDRMConfigIntcfImpl( CDataBufferMultimediaSource& aDRMHandler )
:iDRMHandler(&aDRMHandler)
    {
    // No Impl
    }

CDRMConfigIntcfImpl::~CDRMConfigIntcfImpl()    
    {
    // No Impl
    }

CDRMConfigIntcfImpl* CDRMConfigIntcfImpl::NewL( CDataBufferMultimediaSource& aDRMHandler )
    {
    CDRMConfigIntcfImpl* self = new (ELeave)CDRMConfigIntcfImpl(aDRMHandler);
    CleanupStack::PushL(self);
    self->ConstructL();
    CleanupStack::Pop(self);
    return self;
    }
void CDRMConfigIntcfImpl::ConstructL()
    {
    // No Impl
    }

TInt CDRMConfigIntcfImpl::GetDRMType( TDRMType& aDRMType )
    {
    return iDRMHandler->GetDRMType(aDRMType);
    }

TInt CDRMConfigIntcfImpl::GetAllowedOutputDeviceCount( TInt& aCount )
    {
    return iDRMHandler->GetAllowedOutputDeviceCount(aCount);
    }

TInt CDRMConfigIntcfImpl::GetAllowedOutputDevice(
                TInt aIndex,
                TDRMAllowedOutputDevice& aOutputDevice )
    {
    return iDRMHandler->GetAllowedOutputDevice( aIndex, aOutputDevice );
    }

// End of file
