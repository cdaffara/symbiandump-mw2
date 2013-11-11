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
* Description:  Implementation of drm media agent base class
*
*/


#include <caf/data.h>
#include "mpxmedia.h"
#include "mpxmediadrmdefs.h"
#include "mpxdrmmediaagent.h"
#include "mpxlog.h"

// CONSTANTS

// ----------------------------------------------------------------------------
// Closes and uninitializes the agent and cleans up member variables
// ----------------------------------------------------------------------------
//
void CMPXDrmMediaAgent::Close()
    {
    MPX_DEBUG1("CMPXDrmMediaAgent::Close() entering");
    delete iMedia;
    iMedia = NULL;
    delete iData;
    iData = NULL;
    MPX_DEBUG1("CMPXDrmMediaAgent::Close() exiting");
    }

// ----------------------------------------------------------------------------
// Creates a new media object
// ----------------------------------------------------------------------------
//
void CMPXDrmMediaAgent::CreateMediaL()
    {
    MPX_DEBUG1("CMPXDrmMediaAgent::CreateMediaL() entering");

    // Create media
    RArray<TInt> supportedIds;
    CleanupClosePushL( supportedIds );
    supportedIds.AppendL(KMPXMediaIdDrm);
    iMedia = CMPXMedia::NewL( supportedIds.Array() );
    CleanupStack::PopAndDestroy( &supportedIds );
    
    MPX_DEBUG1("CMPXDrmMediaAgent::CreateMediaL() exiting");
    }

// ----------------------------------------------------------------------------
// Gets the protected attribute if not already obtained
// ----------------------------------------------------------------------------
//
void CMPXDrmMediaAgent::GetProtectedL()
    {
    MPX_DEBUG1("CMPXDrmMediaAgent::GetProtectedL() entering");
    
    if ( !iMedia->IsSupported( KMPXMediaDrmProtected ))
        {
        TBool prot( EFalse );
        User::LeaveIfError( iData->GetAttribute( EIsProtected, prot ));
        iMedia->SetTObjectValueL( KMPXMediaDrmProtected,
                                  prot );
        MPX_DEBUG2("CMPXDrmMediaAgent::GetProtectedL() protected = %d", prot);
        }
    
    MPX_DEBUG1("CMPXDrmMediaAgent::GetProtectedL() exiting");
    }


// end of file
