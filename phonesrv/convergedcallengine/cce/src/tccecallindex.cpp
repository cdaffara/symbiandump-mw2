/*
* Copyright (c) 2008-2008 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  Keeps up the call index
*
*/


#include "tccecallindex.h"

#include "mccecallarray.h"
#include "mccecallinfo.h"
#include "cccelogger.h"

// ======== MEMBER FUNCTIONS ========

// ---------------------------------------------------------------------------
// Constructor
// ---------------------------------------------------------------------------
//
TCCECallIndex::TCCECallIndex( MCCECallArray& aCallArray ) : 
    iCallArray( aCallArray )
    {
    }

// ---------------------------------------------------------------------------
// Checks smallest available call index
// ---------------------------------------------------------------------------
//
TInt TCCECallIndex::CurrentCallIndex() const
    {
    CCELOGSTRING( "TCCECallIndex::CurrentCallIndex(): In" );
    TInt smallestAvailableCallIndex = 1; // Smallest call index is 1
    
    // Iterate until call index value has not changed. 
    // TODO: There has to be better way to solve call index. 
    TBool callIndexChanged( ETrue );
    while( callIndexChanged )
        {
        // Iterate over the calls and check if smallestAvailableCallIndex is in use
        // with some call.
        callIndexChanged = EFalse;
        for ( TInt i = 0; i < iCallArray.MaxNumberOfCalls(); i++ )
            {
            if( CheckAndIncrementCallIndex( iCallArray.CallInfo( i ), smallestAvailableCallIndex ) )
                {
                callIndexChanged = ETrue;
                }
            }
            if ( CheckAndIncrementCallIndex( iCallArray.EmergencyCallInfo(), smallestAvailableCallIndex ) )
                {
                callIndexChanged = ETrue;
                }
        }
    CCELOGSTRING2( "TCCECallIndex::CurrentCallIndex(): Smallest avail call index = %d", smallestAvailableCallIndex );
    CCELOGSTRING( "TCCECallIndex::CurrentCallIndex(): Out" );
    return smallestAvailableCallIndex;
    }
 
// ---------------------------------------------------------------------------
// Checks and increments the call index.
// ---------------------------------------------------------------------------
//
TBool TCCECallIndex::CheckAndIncrementCallIndex( 
    MCCECallInfo* aCallInfo, 
    TInt& aCallIndex ) const
    {
    if ( aCallInfo && aCallInfo->State() != CCPCall::EStateIdle )
        {
        if ( aCallIndex == aCallInfo->CallIndex() )
            {
            aCallIndex++;
            return ETrue;
            }
        }
    return EFalse;
    }

