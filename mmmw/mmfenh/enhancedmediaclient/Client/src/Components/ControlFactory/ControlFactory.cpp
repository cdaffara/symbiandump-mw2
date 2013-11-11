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
* Description:  Implementation of the ControlFactory class.
*
*/


#include <MMControlFactory.h>
#include "ControlFactoryImpl.h"

using namespace multimedia;

CMultimediaFactory::CMultimediaFactory()
    {
    // No Impl
    }

void CMultimediaFactory::ConstructL()
    {
    iCMultimediaFactoryImpl = CMultimediaFactoryImpl::NewL();
    }

EXPORT_C TInt CMultimediaFactory::CreateFactory( CMultimediaFactory*& aFactory )
    {
    TInt err(KErrNone);
    aFactory = NULL;
    // Create a new instance of factory
    aFactory = new CMultimediaFactory;
    if ( aFactory )
        {
        // Call second phase constructor
        TRAP( err, aFactory->ConstructL() );
        // If any error during second phase constructor,
        // delete the instance.
        if ( err != KErrNone )
            {
            delete aFactory;
            aFactory = NULL;
            }
        }
    else
        {
        err = KErrNoMemory;    
        }            
    return err;
    }
    
EXPORT_C CMultimediaFactory::~CMultimediaFactory()
    {
    delete iCMultimediaFactoryImpl;
    }

EXPORT_C TInt CMultimediaFactory::CreateStreamControl( TUid aType, MStreamControl*& aControl )
    {
    TInt status( KErrNotReady );
    if ( iCMultimediaFactoryImpl )
        {
        status = iCMultimediaFactoryImpl->CreateStreamControl( aType, aControl );
        }
    return status;
    }

EXPORT_C TInt CMultimediaFactory::DeleteStreamControl( MStreamControl*& aControl )
    {
    TInt status( KErrNotReady );
    if ( iCMultimediaFactoryImpl )
        {
        status = iCMultimediaFactoryImpl->DeleteStreamControl( aControl );
        }
    return status;
    }

EXPORT_C TInt CMultimediaFactory::CreateSourceControl( TUid aType, MSourceControl*& aControl )
    {
    TInt status( KErrNotReady );
    if ( iCMultimediaFactoryImpl )
        {
        status = iCMultimediaFactoryImpl->CreateSourceControl( aType, aControl );
        }
    return status;
    }

EXPORT_C TInt CMultimediaFactory::DeleteSourceControl( MSourceControl*& aControl )
    {
    TInt status( KErrNotReady );
    if ( iCMultimediaFactoryImpl )
        {
        status = iCMultimediaFactoryImpl->DeleteSourceControl( aControl );
        }
    return status;
    }

EXPORT_C TInt CMultimediaFactory::CreateSinkControl( TUid aType, MSinkControl*& aControl )
    {
    TInt status( KErrNotReady );
    if ( iCMultimediaFactoryImpl )
        {
        status = iCMultimediaFactoryImpl->CreateSinkControl( aType, aControl );
        }
    return status;
    }

EXPORT_C TInt CMultimediaFactory::DeleteSinkControl( MSinkControl*& aControl )
    {
    TInt status( KErrNotReady );
    if ( iCMultimediaFactoryImpl )
        {
        status = iCMultimediaFactoryImpl->DeleteSinkControl( aControl );
        }
    return status;
    }
    
EXPORT_C TInt CMultimediaFactory::CreateEffectControl( TUid aType, MEffectControl*& aControl )
    {
    TInt status( KErrNotReady );
    if ( iCMultimediaFactoryImpl )
        {
        status = iCMultimediaFactoryImpl->CreateEffectControl( aType, aControl );
        }
    return status;
    }

EXPORT_C TInt CMultimediaFactory::DeleteEffectControl( MEffectControl*& aControl )
    {
    TInt status( KErrNotReady );
    if ( iCMultimediaFactoryImpl )
        {
        status = iCMultimediaFactoryImpl->DeleteEffectControl( aControl );
        }
    return status;
    }

EXPORT_C TInt CMultimediaFactory::CreateDataBuffer( TUid aType, TUint aSize, MDataBuffer*& aBuffer )
    {
    TInt status( KErrNotReady );
    if ( iCMultimediaFactoryImpl )
        {
        status = iCMultimediaFactoryImpl->CreateDataBuffer( aType, aSize, aBuffer );
        }
    return status;
    }

EXPORT_C TInt CMultimediaFactory::DeleteDataBuffer( MDataBuffer*& aBuffer )
    {
    TInt status( KErrNotReady );
    if ( iCMultimediaFactoryImpl )
        {
        status = iCMultimediaFactoryImpl->DeleteDataBuffer( aBuffer );
        }
    return status;
    }
// End of file
