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
* Description:  Implementation of the ControlFactoryImpl class.
*
*/


#include <StreamControl.h>
#include <VolumeControl.h>
#include <BalanceControl.h>
#include <SinkControl.h>

#include "ControlFactoryImpl.h"
#include "ClientStreamControl.h"
#include "ClientDataBufferSource.h"
#include "ClientFileSource.h"
#include "ClientDescriptorSource.h"
#include "ClientDataBuffer.h"
#include "VolumeEffectImpl.h"
#include "BalanceEffectImpl.h"
#include "CMMFAudioOutputSink.h"

#include "EqualizerEffectImpl.h"
#include "ReverbEffectImpl.h"
#include "StereoWideningEffectImpl.h"
#include "LoudnessEffectImpl.h"
#include "RoomLevelEffectImpl.h"
#include "BassBoostEffectImpl.h"
#include "DistanceAttenuationEffectImpl.h"
#include "SourceLocationEffectImpl.h"
#include "ListenerLocationEffectImpl.h"
#include "SourceDopplerEffectImpl.h"
#include "ListenerDopplerEffectImpl.h"
#include "SourceOrientationEffectImpl.h"
#include "ListenerOrientationEffectImpl.h"

using namespace multimedia;

CMultimediaFactoryImpl::CMultimediaFactoryImpl()
    {
    // No impl
    }

CMultimediaFactoryImpl* CMultimediaFactoryImpl::NewL()
    {
    CMultimediaFactoryImpl* self = new ( ELeave ) CMultimediaFactoryImpl;
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop( self );
    return self;
    }

void CMultimediaFactoryImpl::ConstructL()
    {
    // No impl
    }

CMultimediaFactoryImpl::~CMultimediaFactoryImpl()
    {
    }
    
TInt CMultimediaFactoryImpl::CreateStreamControl( TUid aType, MStreamControl*& aControl )
    {
    TInt status(KErrNotFound);
    CStreamControl* objPtr(NULL);
    if ( aType == KStreamControl )
        {
        objPtr = new CStreamControl;
        status = KErrNone;
        if (!objPtr)
            status = KErrNoMemory;
        else
            status = objPtr->PostConstructor();
        }
    if ( status == KErrNone )
        {
        aControl = objPtr;
        }
    else
        {
        delete objPtr;
        }
    return status;
    }

TInt CMultimediaFactoryImpl::DeleteStreamControl( MStreamControl*& aControl )
    {
    TInt status(KErrNone);
    CBase* temp = dynamic_cast<CBase*>(aControl);
    delete temp;
    aControl = NULL;
    return status;
    }

TInt CMultimediaFactoryImpl::CreateSourceControl( TUid aType, MSourceControl*& aControl )
    {
    TInt status(KErrNotFound);

    if(aType == KDataBufferSourceControl)
        {
        CClientDataBufferSource* srcControl = new CClientDataBufferSource;
        if ( !srcControl )
            {
            status = KErrNoMemory;
            }
        else
            {
            status = srcControl->PostConstructor();
            }
        
        if ( status == KErrNone )
            {
            aControl = srcControl;
            }
        else
            {
            delete srcControl;
            }
        }
    
    if(aType == KFileSourceControl)
        {
        CClientFileSource* srcControl = new CClientFileSource;
        if ( !srcControl )
            {
            status = KErrNoMemory;
            }
        else
            {
            status = srcControl->PostConstructor();
            }
        
        if ( status == KErrNone )
            {
            aControl = srcControl;
            }
        else
            {
            delete srcControl;
            }
        }
    
    if(aType == KDescriptorSourceControl)
        {
        CClientDescriptorSource* srcControl = new CClientDescriptorSource;
        if ( !srcControl )
            {
            status = KErrNoMemory;
            }
        else
            {
            status = srcControl->PostConstructor();
            }
        
        if ( status == KErrNone )
            {
            aControl = srcControl;
            }
        else
            {
            delete srcControl;
            }
        }
        
    return status;
    }

TInt CMultimediaFactoryImpl::DeleteSourceControl( MSourceControl*& aControl )
    {
    TInt status(KErrNone);
    CBase* temp = dynamic_cast<CBase*>(aControl);
    delete temp;
    aControl = NULL;
    return status;
    }

TInt CMultimediaFactoryImpl::CreateSinkControl( TUid aType, MSinkControl*& aControl )
    {
    TInt status(KErrNotFound);
    if ( aType == KMMFAudioOutputSinkControl )
        {
        CMmfAudioOutputSink* control = new CMmfAudioOutputSink;
        if ( control == NULL )
            {
            status = KErrNoMemory;
            }
        else
            {
            status = control->PostConstructor();
            }

        if ( status == KErrNone )
            {
            aControl = control;
            }
        else
            {
            delete control;
            }
        }
    return status;
    }

TInt CMultimediaFactoryImpl::DeleteSinkControl( MSinkControl*& aControl )
    {
    TInt status(KErrNone);
    CBase* temp = dynamic_cast<CBase*>(aControl);
    delete temp;
    aControl = NULL;
    return status;
    }

TInt CMultimediaFactoryImpl::CreateEffectControl( TUid aType, MEffectControl*& aControl )
    {
    TInt status(KErrNotFound);
    if ( aType == KVolumeEffectControl )
        {
        CVolumeEffect* control = new CVolumeEffect;
        if ( control == NULL )
            {
            status = KErrNoMemory;
            }
        else
            {
            status = control->PostConstructor();
            }

        if ( status == KErrNone )
            {
            aControl = control;
            }
        else
            {
            delete control;
            }
        }
    else if ( aType == KBalanceEffectControl )
        {
        CBalanceEffect* control = new CBalanceEffect;
        if ( control == NULL )
            {
            status = KErrNoMemory;
            }
        else
            {
            status = control->PostConstructor();
            }

        if ( status == KErrNone )
            {
            aControl = control;
            }
        else
            {
            delete control;
            }

        }
    else if ( aType == KEqualizerEffectControl )
        {
        CEqualizerEffect* control = new CEqualizerEffect;
        if ( control == NULL )
            {
            status = KErrNoMemory;
            }
        else
            {
            status = control->PostConstructor();
            }

        if ( status == KErrNone )
            {
            aControl = control;
            }
        else
            {
            delete control;
            }

        }
    else if ( aType == KReverbEffectControl )
        {
        CReverbEffect* control = new CReverbEffect;
        if ( control == NULL )
            {
            status = KErrNoMemory;
            }
        else
            {
            status = control->PostConstructor();
            }

        if ( status == KErrNone )
            {
            aControl = control;
            }
        else
            {
            delete control;
            }

        }
    else if ( aType == KBassBoostEffectControl )
        {
        CBassBoostEffect* control = new CBassBoostEffect;
        if ( control == NULL )
            {
            status = KErrNoMemory;
            }
        else
            {
            status = control->PostConstructor();
            }

        if ( status == KErrNone )
            {
            aControl = control;
            }
        else
            {
            delete control;
            }

        }
    else if ( aType == KLoudnessEffectControl )
        {
        CLoudnessEffect* control = new CLoudnessEffect;
        if ( control == NULL )
            {
            status = KErrNoMemory;
            }
        else
            {
            status = control->PostConstructor();
            }

        if ( status == KErrNone )
            {
            aControl = control;
            }
        else
            {
            delete control;
            }

        }
    else if ( aType == KStereoWideningEffectControl )
        {
        CStereoWideningEffect* control = new CStereoWideningEffect;
        if ( control == NULL )
            {
            status = KErrNoMemory;
            }
        else
            {
            status = control->PostConstructor();
            }

        if ( status == KErrNone )
            {
            aControl = control;
            }
        else
            {
            delete control;
            }

        }
    else if ( aType == KRoomLevelEffectControl )
        {
        CRoomLevelEffect* control = new CRoomLevelEffect;
        if ( control == NULL )
            {
            status = KErrNoMemory;
            }
        else
            {
            status = control->PostConstructor();
            }

        if ( status == KErrNone )
            {
            aControl = control;
            }
        else
            {
            delete control;
            }

        }
    else if ( aType == KDistanceAttenuationEffectControl )
        {
        CDistanceAttenuationEffect* control = new CDistanceAttenuationEffect;
        if ( control == NULL )
            {
            status = KErrNoMemory;
            }
        else
            {
            status = control->PostConstructor();
            }

        if ( status == KErrNone )
            {
            aControl = control;
            }
        else
            {
            delete control;
            }

        }
    else if ( aType == KSourceLocationEffectControl )
        {
        CSourceLocationEffect* control = new CSourceLocationEffect;
        if ( control == NULL )
            {
            status = KErrNoMemory;
            }
        else
            {
            status = control->PostConstructor();
            }

        if ( status == KErrNone )
            {
            aControl = control;
            }
        else
            {
            delete control;
            }

        }
    else if ( aType == KListenerLocationEffectControl )
        {
        CListenerLocationEffect* control = new CListenerLocationEffect;
        if ( control == NULL )
            {
            status = KErrNoMemory;
            }
        else
            {
            status = control->PostConstructor();
            }

        if ( status == KErrNone )
            {
            aControl = control;
            }
        else
            {
            delete control;
            }

        }
    else if ( aType == KSourceOrientationEffectControl )
        {
        CSourceOrientationEffect* control = new CSourceOrientationEffect;
        if ( control == NULL )
            {
            status = KErrNoMemory;
            }
        else
            {
            status = control->PostConstructor();
            }

        if ( status == KErrNone )
            {
            aControl = control;
            }
        else
            {
            delete control;
            }

        }
    else if ( aType == KListenerOrientationEffectControl )
        {
        CListenerOrientationEffect* control = new CListenerOrientationEffect;
        if ( control == NULL )
            {
            status = KErrNoMemory;
            }
        else
            {
            status = control->PostConstructor();
            }

        if ( status == KErrNone )
            {
            aControl = control;
            }
        else
            {
            delete control;
            }

        }
    else if ( aType == KSourceDopplerEffectControl )
        {
        CSourceDopplerEffect* control = new CSourceDopplerEffect;
        if ( control == NULL )
            {
            status = KErrNoMemory;
            }
        else
            {
            status = control->PostConstructor();
            }

        if ( status == KErrNone )
            {
            aControl = control;
            }
        else
            {
            delete control;
            }

        }
    else if ( aType == KListenerDopplerEffectControl )
        {
        CListenerDopplerEffect* control = new CListenerDopplerEffect;
        if ( control == NULL )
            {
            status = KErrNoMemory;
            }
        else
            {
            status = control->PostConstructor();
            }

        if ( status == KErrNone )
            {
            aControl = control;
            }
        else
            {
            delete control;
            }

        }

    return status;
    }

TInt CMultimediaFactoryImpl::DeleteEffectControl( MEffectControl*& aControl )
    {
    TInt status(KErrNone);
    CBase* temp = dynamic_cast<CBase*>(aControl);
    delete temp;
    aControl = NULL;
    return status;
    }

TInt CMultimediaFactoryImpl::CreateDataBuffer( TUid aType, TUint aSize, MDataBuffer*& aBuffer )
    {
    TInt status(KErrNotFound);
 
    if ( aType == KDataBufferSourceControl )
        {
        CClientDataBuffer* srcBuffer=NULL;
        TRAP(status,srcBuffer = CClientDataBuffer::NewL(aSize));
        if ( !srcBuffer )
            {
            status = KErrNoMemory;
            }
        else
            {
            aBuffer = srcBuffer;
            }
        }
    return status;
    }

TInt CMultimediaFactoryImpl::DeleteDataBuffer( MDataBuffer*& aBuffer )
    {
    TInt status(KErrNone);
    CClientDataBuffer* temp = dynamic_cast<CClientDataBuffer*>(aBuffer);
    delete temp;
    aBuffer = NULL;
    return status;
    }
// End of file
