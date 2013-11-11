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
* Description:  Implementation of the MmfAudioOutputSink class.
*
*/


#include "CMMFAudioOutputSink.h"
#include <e32std.h>

#define RETURN_IF_ERROR(x) if(x != KErrNone) return x

using namespace multimedia;

// CONSTANTS
CMmfAudioOutputSink::CMmfAudioOutputSink()
    {
    }

CMmfAudioOutputSink::~CMmfAudioOutputSink()
    {
    }

TInt CMmfAudioOutputSink::PostConstructor()
    {
    TInt status(KErrNone);
    return status;
    }

TInt CMmfAudioOutputSink::AddObserver( MControlObserver& /*aObserver*/ )
    {
    return KErrNotSupported;
    }

TInt CMmfAudioOutputSink::RemoveObserver( MControlObserver& /*aObserver*/ )
    {
    return KErrNotSupported;        
    }

TUid CMmfAudioOutputSink::Type()
    {
    return KMMFAudioOutputSinkControl;
    }

TControlType CMmfAudioOutputSink::ControlType()
    {
	return ESinkControl;
    }
    
    
TUid CMmfAudioOutputSink::GetSinkUid()
    {
    return KMMFAudioOutputSinkControl;
    }
    
// End of file
