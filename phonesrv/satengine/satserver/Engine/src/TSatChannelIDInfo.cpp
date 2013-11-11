/*
* Copyright (c) 2002-2005 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  Information class for Channel IDs.
*
*/


#include "TSatChannelIDInfo.h"
#include "SatLog.h"

// -----------------------------------------------------------------------------
// TSatChannelIdInfo::TSatChannelIdInfo
// C++ default constructor can NOT contain any code, that
// might leave.
// -----------------------------------------------------------------------------
//
TSatChannelIdInfo::TSatChannelIdInfo( TInt aChannelId, TUint8 aShort ) :
    iChannelId( aChannelId ),
    iShortChannelId( aShort ),
    iActive( EFalse )
    {
    LOG( SIMPLE, "SATENGINE: TSatChannelIdInfo::TSatChannelIdInfo \
        calling-exiting" )
    }

// -----------------------------------------------------------------------------
// TSatChannelIdInfo::ChannelId
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TInt TSatChannelIdInfo::ChannelId() const
    {
    LOG2( SIMPLE, "SATENGINE: TSatChannelIdInfo::ChannelId \
        calling-exiting with: %i", iChannelId )
    return iChannelId;
    }

// -----------------------------------------------------------------------------
// TSatChannelIdInfo::ShortChannelId
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TUint8 TSatChannelIdInfo::ShortChannelId() const
    {
    LOG2( SIMPLE, "SATENGINE: TSatChannelIdInfo::ShortChannelId \
        calling-exiting with: %i", iShortChannelId )
    return iShortChannelId;
    }
 
// -----------------------------------------------------------------------------
// TSatChannelIdInfo::IsReserved
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TBool TSatChannelIdInfo::IsReserved() const
    {
    LOG2( SIMPLE, "SATENGINE: TSatChannelIdInfo::IsReserved \
        calling-exiting with: %i", iActive )
    return iActive;
    }

// -----------------------------------------------------------------------------
// TSatChannelIdInfo::ReserveChannel
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void TSatChannelIdInfo::ReserveChannel()
    {
    LOG( SIMPLE, "SATENGINE: TSatChannelIdInfo::ReserveChannel \
        calling-exiting" )
    iActive = ETrue;
    }

// -----------------------------------------------------------------------------
// TSatChannelIdInfo::ReleaseChannel
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void TSatChannelIdInfo::ReleaseChannel()
    {
    LOG( SIMPLE, "SATENGINE: TSatChannelIdInfo::ReleaseChannel \
        calling-exiting" )
    iActive = EFalse;
    }

// End of File
