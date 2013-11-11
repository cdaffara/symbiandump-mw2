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
* Description:  Derived plugin info class for playback tags
*
*/


// INCLUDE FILES
#include <e32base.h>
#include <mpxuser.h>
#include <mpxlog.h>
#include <mpxplugininfo.h>
#include "mpxplaybackplugininfo.h"

_LIT8(KMPXPluginSupportedInterfacesTag, "<z>" );

// ---------------------------------------------------------------------------
// Constructor
// ---------------------------------------------------------------------------
//
CMPXPlaybackPluginInfo::CMPXPlaybackPluginInfo()
    {
    
    }


// ---------------------------------------------------------------------------
// 2nd phase constructor
// ---------------------------------------------------------------------------
//
void CMPXPlaybackPluginInfo::ConstructL( const CImplementationInformation& aData )
    {
    iSupportedInterfaces = new(ELeave)CDesCArrayFlat(KMPXArrayGranularity);
    
    // Base constructor
    CMPXPluginInfo::ConstructL( aData );    
    }

// ---------------------------------------------------------------------------
// Two-Phased constructor
// ---------------------------------------------------------------------------
//
CMPXPlaybackPluginInfo* CMPXPlaybackPluginInfo::NewL( 
                                     const CImplementationInformation& aData )
    {
    CMPXPlaybackPluginInfo* self = new( ELeave ) CMPXPlaybackPluginInfo;
    CleanupStack::PushL( self );
    self->ConstructL( aData );
    CleanupStack::Pop( self );
    return self;
    }

// ---------------------------------------------------------------------------
// Two-phased constructor
// ---------------------------------------------------------------------------
//
CMPXPlaybackPluginInfo* CMPXPlaybackPluginInfo::NewLC( 
                                     const CImplementationInformation& aData )
    {
    CMPXPlaybackPluginInfo* self = new( ELeave ) CMPXPlaybackPluginInfo;
    CleanupStack::PushL( self );
    self->ConstructL( aData );
    return self;
    }

// ---------------------------------------------------------------------------
// Virtual destructor
// ---------------------------------------------------------------------------
//
CMPXPlaybackPluginInfo::~CMPXPlaybackPluginInfo()
    {
    delete iSupportedInterfaces;
    }

// ---------------------------------------------------------------------------
// Extended tag parsing functionality
// ---------------------------------------------------------------------------
//
void CMPXPlaybackPluginInfo::ProcessTaggedDataExL( const TDesC8& aTag,
                                                     const TDesC8& aData)
    {
    if( aTag == KMPXPluginSupportedInterfacesTag )
        {
        ExtractSupportedInterfacesL( aData );
        }
    }

// ---------------------------------------------------------------------------
// Get the list of supported interfaces
// ---------------------------------------------------------------------------
//
const CDesCArray& CMPXPlaybackPluginInfo::SupportedInterfaces() const
    {
    return *iSupportedInterfaces;
    }

// ---------------------------------------------------------------------------
// Process the non-cacheable attributes
// ---------------------------------------------------------------------------
//
void CMPXPlaybackPluginInfo::ExtractSupportedInterfacesL( const TDesC8& aData )
    {
    iSupportedInterfaces->Reset();
    ExtractIntoArrayL(aData, iSupportedInterfaces);
    }

// END OF FILE
