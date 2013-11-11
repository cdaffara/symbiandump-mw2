/*
* Copyright (c) 2002-2008 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  Maps extended error values to correct values.
*
*/


#include    "TSatExtErrorUtils.h"
#include    <exterror.h>
#include    "SatLog.h"

// In some cases return value needs to be zeroed
const TUint8 KSatZeroError( 0x00 );
const TUint8 KHighBitInByte( 0x80 );

// -----------------------------------------------------------------------------
// TSatExtErrorUtils::MapError
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
EXPORT_C TUint8 TSatExtErrorUtils::MapError(
    TInt aExtError,
    TBool aModifyMappedValue )
    {
    LOG2( SIMPLE, "SATENGINE: TSatExtErrorUtils::MapError calling value: %i",
       aExtError )

    TUint8 retValue( 0 );
    TUint8 nwValue( 0 );
    TInt baseError( 0 );

    if ( IsExtendedError( aExtError ) )
        {
        // Define the base error value
        if ( KErrGsmMobilityManagementBase < aExtError )
            {
            LOG( NORMAL, "SATENGINE: TSatExtErrorUtils::MapError Base error" )
            baseError = KErrEtelGsmBase; // Base error
            }
        else if ( KErrGsmCallControlBase < aExtError )
            {
            LOG( NORMAL, "SATENGINE: TSatExtErrorUtils::MapError MM error" )
            baseError = KErrGsmMobilityManagementBase; // MM error
            }
        else if ( KErrGsmSuppServiceBase < aExtError )
            {
            LOG( NORMAL, "SATENGINE: TSatExtErrorUtils::MapError CallControl \
                 error" )
            baseError = KErrGsmCallControlBase; // CallControl error
            }
        else if ( KErrGsmSmsBase < aExtError )
            {
            LOG( NORMAL, "SATENGINE: TSatExtErrorUtils::MapError SS error" )
            baseError = KErrGsmSuppServiceBase; // Ss error
            }
        else if ( KErrGsm0707Base < aExtError )
            {
            LOG( NORMAL, "SATENGINE: TSatExtErrorUtils::MapError Sms error" )
            baseError = KErrGsmSmsBase; // Sms error
            }
        else
            {
            LOG( NORMAL, "SATENGINE: TSatExtErrorUtils::MapError TE-TA error" )
            baseError = KErrGsm0707Base; // TE-TA error
            }
        // Base error resolved, calculate error value
        nwValue = static_cast<TUint8>( baseError - aExtError );
        }
    else
        {
        LOG( NORMAL, "  Value not extended error, not mapped" )
        }

    // Next, check specific cases
    CheckSpecificCases( aExtError, nwValue );

    // Check do we have to modify the value
    if ( aModifyMappedValue && nwValue > KSatZeroError )
        {
        LOG( NORMAL, "SATENGINE: TSatExtErrorUtils::MapError Modifying value" )
        retValue = static_cast<TUint8>( KHighBitInByte | nwValue );
        }
    else
        {
        retValue = nwValue;
        }

    LOG2( SIMPLE, "SATENGINE: TSatExtErrorUtils::MapError exiting value:%x",
        retValue )

    return retValue;
    }

// -----------------------------------------------------------------------------
// TSatExtErrorUtils::IsExtendedError
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
EXPORT_C TBool TSatExtErrorUtils::IsExtendedError( TInt aExtError )
    {
    LOG( SIMPLE, 
    "SATENGINE: TSatExtErrorUtils::IsExtendedError calling-exiting" )
    const TBool isExtError(
        ( KErrEtelGsmBase >= aExtError ) &&
        ( KErrGsm0707UnknownError <= aExtError ) );

    return isExtError;   
    }

// -----------------------------------------------------------------------------
// TSatExtErrorUtils::IsNetworkError
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
EXPORT_C TBool TSatExtErrorUtils::IsNetworkError( TInt aExtError )
    {
    LOG2( SIMPLE, "SATENGINE: TSatExtErrorUtils::IsNetworkError calling, \
         aExtError: %d", aExtError )
    TBool isNwError( ETrue );

    switch ( aExtError )
        {
        // Add errors here that are not supposed to handle as NW errors
        case KErrGsmSMSTemporaryFailure:
        case KErrGsmSSUnknownAlphabet:
        case KErrGsmSSSystemFailure:
        case KErrGsmSSUnexpectedDataValue:
        case KErrGsmSSIllegalOperation:
        case KErrGsm0707UnknownError:
            {
            isNwError = EFalse;
            break;
            }

        default:
            {
            LOG( NORMAL, "SATENGINE: TSatExtErrorUtils::IsNetworkError\
                Is Network error" )
            isNwError = ETrue;
            }
        }
    LOG2( SIMPLE, "SATENGINE: TSatExtErrorUtils::IsNetworkError exiting, \
         isNwError: %d", isNwError )
    return isNwError;    
    }

// -----------------------------------------------------------------------------
// TSatExtErrorUtils::CheckSpecificCases
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void TSatExtErrorUtils::CheckSpecificCases(
    TInt aExtError, TUint8& aNwError )
    {
    LOG( SIMPLE, "SATENGINE: TSatExtErrorUtils::CheckSpecificCases calling" )
    // Specific cases, where Extended error received by SAT differs from the
    // one that SIM expects
    if ( KErrGsmSMSCongestion == aExtError )
        {
        LOG( NORMAL, "SATENGINE: TSatExtErrorUtils::CheckSpecificCases\
                Changing Sms Congestion" )
        // Change this into KErrGsmMmCongestion
        aNwError = static_cast<TUint8>(
            KErrGsmMobilityManagementBase - KErrGsmMMCongestion );
        }
    else if ( KErrGsmSMSUnspecifiedProtocolError == aExtError ||
              KErrGsmCCResourceNotAvailable == aExtError ||
              KErrGsmSSIllegalOperation == aExtError ||
              KErrGsmSMSNoNetworkService == aExtError ||
              KErrGsmSSNotAvailable == aExtError ||
              KErrGsm0707UnknownError == aExtError )
        {
        LOG( NORMAL, "SATENGINE: TSatExtErrorUtils::CheckSpecificCases\
                Changing unspecified protocol error to 0x00" )
        // Change this into zero value
        aNwError = KSatZeroError;
        }
    else
        {
        LOG( NORMAL, "SATENGINE: TSatExtErrorUtils::CheckSpecificCases\
                Not a special case" )
        }
    LOG( SIMPLE, "SATENGINE: TSatExtErrorUtils::CheckSpecificCases exiting" )
    }

// End Of File
