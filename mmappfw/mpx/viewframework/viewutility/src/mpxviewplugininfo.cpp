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
* Description:  Implementation of view plugin info
*
*/



// INCLUDE FILES
#include <mpxviewplugin.hrh>

#include "mpxviewutilityhelper.h"
#include "mpxviewplugininfo.h"
#include "mpxlog.h"


// CONSTANTS
_LIT8( KMPXPluginUidMarkerStart, "<p>" );
_LIT8( KMPXPluginUidMarkerEnd, "</p>" );
_LIT8( KMPXPluginTypeMarkerStart, "<t>" );
_LIT8( KMPXPluginTypeMarkerEnd, "</t>" );
_LIT8( KMPXPluginFlagMarkerStart, "<f>" );
_LIT8( KMPXPluginFlagMarkerEnd, "</f>" );
_LIT8( KMPXPluginPriorityMarkerStart, "<i>" );
_LIT8( KMPXPluginPriorityMarkerEnd, "</i>" );

// ======== MEMBER FUNCTIONS ========

// ---------------------------------------------------------------------------
// C++ default constructor can NOT contain any code, that
// might leave.
// ---------------------------------------------------------------------------
//
CMPXViewPluginInfo::CMPXViewPluginInfo()
    {
    }

// ---------------------------------------------------------------------------
// By default Symbian 2nd phase constructor is private.
// ---------------------------------------------------------------------------
//
void CMPXViewPluginInfo::ConstructL( 
    const CImplementationInformation& aData )
    {
    iPluginName = aData.DisplayName().AllocL();
    iPluginUid = aData.ImplementationUid();
    iPluginTypeUid = ExtractPluginType( aData.OpaqueData() );
    iFlags = ExtractPluginFlags( aData.OpaqueData() );
    iPriority = ExtractPluginPriority( aData.OpaqueData() );
    ExtractPluginUidsL( aData.OpaqueData(), iSupportedUids );
    }

// ---------------------------------------------------------------------------
// Two-phased constructor.
// ---------------------------------------------------------------------------
//
CMPXViewPluginInfo* CMPXViewPluginInfo::NewL( 
    const CImplementationInformation& aData )
    {
    CMPXViewPluginInfo* self = CMPXViewPluginInfo::NewLC( aData );
    CleanupStack::Pop( self );

    return self;
    }

// ---------------------------------------------------------------------------
// Two-phased constructor.
// ---------------------------------------------------------------------------
//
CMPXViewPluginInfo* CMPXViewPluginInfo::NewLC( 
    const CImplementationInformation& aData )
    {
    CMPXViewPluginInfo* self = new ( ELeave ) CMPXViewPluginInfo();
    CleanupStack::PushL( self );
    self->ConstructL( aData );

    return self;
    }

// ---------------------------------------------------------------------------
// Destructor
// ---------------------------------------------------------------------------
//
CMPXViewPluginInfo::~CMPXViewPluginInfo()
    {
    delete iPluginName;
    iSupportedUids.Close();
    }

// ---------------------------------------------------------------------------
// Returns plugin's display name.
// ---------------------------------------------------------------------------
//
const TDesC& CMPXViewPluginInfo::DisplayName() const
    {
    return iPluginName ? static_cast<const TDesC&>( *iPluginName ) : KNullDesC;
    }

// ---------------------------------------------------------------------------
// Returns plugin's implementation uid.
// ---------------------------------------------------------------------------
//
const TUid& CMPXViewPluginInfo::ImplementationUid() const
    {
    return iPluginUid;
    }

// ---------------------------------------------------------------------------
// Returns plugin's supported uids.
// ---------------------------------------------------------------------------
//
const RArray<TUid> CMPXViewPluginInfo::SupportedUids() const
    {
    return iSupportedUids;
    }

// ---------------------------------------------------------------------------
// Returns plugin's plugin type.
// ---------------------------------------------------------------------------
//
const TUid& CMPXViewPluginInfo::PluginType() const
    {
    return iPluginTypeUid;
    }

// ---------------------------------------------------------------------------
// Returns plugin's view priority.
// ---------------------------------------------------------------------------
//
TInt CMPXViewPluginInfo::Priority() const
    {
    return iPriority;
    }

// ---------------------------------------------------------------------------
// Test whether the plugin is user selectable.
// ---------------------------------------------------------------------------
//
TBool CMPXViewPluginInfo::IsUserSelectable() const
    {
    return iFlags & KMPXVpFeatureUserSelectable;
    }

// ---------------------------------------------------------------------------
// Test whether the plugin needs to be preloaded.
// ---------------------------------------------------------------------------
//
TBool CMPXViewPluginInfo::IsPreLoad() const
    {
    return iFlags & KMPXVpFeaturePreLoaded;
    }

// ---------------------------------------------------------------------------
// Extracts plugin priority from data.
// ---------------------------------------------------------------------------
//
TInt CMPXViewPluginInfo::ExtractPluginPriority( 
    const TDesC8& aData )
    {
    TInt ignore( 0 );
    TInt priority( 0 );
    TPtrC8 string =
        CMPXViewUtilityHelper::Extract( 
            aData, 
            KMPXPluginPriorityMarkerStart, 
            KMPXPluginPriorityMarkerEnd );

    CMPXViewUtilityHelper::ExtractNum( string, ignore, priority );

    return priority;
    }

// ---------------------------------------------------------------------------
// Extracts plugin supported Uids from data.
// ---------------------------------------------------------------------------
//
void CMPXViewPluginInfo::ExtractPluginUidsL( 
    const TDesC8& aData,
    RArray<TUid>& aArray )
    {
    CleanupClosePushL(aArray);
    aArray.Reset();

    TInt nextPos( 0 );
    TInt pluginUid( 0 );
    TPtrC8 string =
        CMPXViewUtilityHelper::Extract( 
            aData, KMPXPluginUidMarkerStart, KMPXPluginUidMarkerEnd );

    do
        {
        if ( KErrNone == 
            CMPXViewUtilityHelper::ExtractNum( string, nextPos, pluginUid ) )
            {
            aArray.AppendL( TUid::Uid( pluginUid ) );
            }
        } while ( KErrNotFound != nextPos );
    CleanupStack::Pop(); 
    }

// ---------------------------------------------------------------------------
// Extracts plugin type from data.
// ---------------------------------------------------------------------------
//
TUid CMPXViewPluginInfo::ExtractPluginType( 
    const TDesC8& aData )
    {
    TInt ignore( 0 );
    TInt type( 0 );
    TPtrC8 string =
        CMPXViewUtilityHelper::Extract( 
            aData, KMPXPluginTypeMarkerStart, KMPXPluginTypeMarkerEnd );

    CMPXViewUtilityHelper::ExtractNum( string, ignore, type );

    return TUid::Uid( type );
    }

// ---------------------------------------------------------------------------
// Extracts plugin feature flags from data.
// ---------------------------------------------------------------------------
//
TInt CMPXViewPluginInfo::ExtractPluginFlags( 
    const TDesC8& aData )
    {
    TInt ignore( 0 );
    TInt flags( 0 );
    TPtrC8 string =
        CMPXViewUtilityHelper::Extract( 
            aData, KMPXPluginFlagMarkerStart, KMPXPluginFlagMarkerEnd );

    CMPXViewUtilityHelper::ExtractNum( string, ignore, flags );

    return flags;
    }

//  End of File
