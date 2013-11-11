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
* Description:  Maps QoS parameters
*
*/


#include    "TSatQoSParser.h"
#include    "SatLog.h"

// Lenght of array of QoS from / to (U)SIM
const TUint8 KSimQoSSize = 6;

// Precedence class values
const TUint8 KPrecMaxSize = 4;
const TUint8 KPrecPos = 0;

const RPacketQoS::TQoSPrecedence TPrecedenceClass[KPrecMaxSize] =
    {
    RPacketQoS::EUnspecifiedPrecedence,
    RPacketQoS::EPriorityHighPrecedence,
    RPacketQoS::EPriorityMediumPrecedence,
    RPacketQoS::EPriorityLowPrecedence
    };

// Delay class values
const TUint8 KDelayMaxSize = 8;
const TUint8 KDelayPos = 1;
const RPacketQoS::TQoSDelay TDelayClass[KDelayMaxSize] =
    {
    RPacketQoS::EUnspecifiedDelayClass,
    RPacketQoS::EDelayClass1,
    RPacketQoS::EDelayClass2,
    RPacketQoS::EDelayClass3,
    RPacketQoS::EDelayClass4,
    // CDMA2000 specific values
    RPacketQoS::EDelay40ms,
    RPacketQoS::EDelay120ms,
    RPacketQoS::EDelay360ms
    };

// Reliability class values
const TUint8 KRelMaxSize = 6;
const TUint8 KRelPos = 2;
const RPacketQoS::TQoSReliability TReliablitityClass[KRelMaxSize] =
    {
    RPacketQoS::EUnspecifiedReliabilityClass,
    RPacketQoS::EReliabilityClass1,
    RPacketQoS::EReliabilityClass2,
    RPacketQoS::EReliabilityClass3,
    RPacketQoS::EReliabilityClass4,
    RPacketQoS::EReliabilityClass5
    };

// Peak throughput values
const TUint8 KPeakMaxSize = 10;
const TUint8 KPeakPos = 3;
const RPacketQoS::TQoSPeakThroughput TPeak[KPeakMaxSize] =
    {
    RPacketQoS::EUnspecifiedPeakThroughput,
    RPacketQoS::EPeakThroughput1000,
    RPacketQoS::EPeakThroughput2000,
    RPacketQoS::EPeakThroughput4000,
    RPacketQoS::EPeakThroughput8000,
    RPacketQoS::EPeakThroughput16000,
    RPacketQoS::EPeakThroughput32000,
    RPacketQoS::EPeakThroughput64000,
    RPacketQoS::EPeakThroughput128000,
    RPacketQoS::EPeakThroughput256000
    };

// Mean throughput values
const TUint8 KMeanMaxSize = 20;
const TUint8 KMeanPos = 4;
const TUint8 KMeanBestEffort = 0x1F;
const RPacketQoS::TQoSMeanThroughput TMean[KMeanMaxSize] =
    {
    RPacketQoS::EUnspecifiedMeanThroughput,
    RPacketQoS::EMeanThroughput100,
    RPacketQoS::EMeanThroughput200,
    RPacketQoS::EMeanThroughput500,
    RPacketQoS::EMeanThroughput1000,
    RPacketQoS::EMeanThroughput2000,
    RPacketQoS::EMeanThroughput5000,
    RPacketQoS::EMeanThroughput10000,
    RPacketQoS::EMeanThroughput20000,
    RPacketQoS::EMeanThroughput50000,
    RPacketQoS::EMeanThroughput100000,
    RPacketQoS::EMeanThroughput200000,
    RPacketQoS::EMeanThroughput500000,
    RPacketQoS::EMeanThroughput1000000,
    RPacketQoS::EMeanThroughput2000000,
    RPacketQoS::EMeanThroughput5000000,
    RPacketQoS::EMeanThroughput10000000,
    RPacketQoS::EMeanThroughput20000000,
    RPacketQoS::EMeanThroughput50000000,
    RPacketQoS::EMeanThroughputBestEffort
    };

// Maximum SDU size value
const TInt KMaximumSDUSize = 1500;

// ======== MEMBER FUNCTIONS ========

// ---------------------------------------------------------------------------
// GetRequestedQoSValues
// ---------------------------------------------------------------------------
//
void TSatQoSParser::GetRequestedQoSValues(
    RSat::TBearerParams& aParams, 
    RPacketQoS::TQoSGPRSRequested& aResult)
    {
    LOG( SIMPLE, "SATENGINE: TSatQoSParser::GetRequestedQoSValues calling" )

    // Get Precedence class value
    aResult.iReqPrecedence = TPrecedenceClass[
        CheckIndex( aParams[KPrecPos], KPrecMaxSize ) ];
    LOG2( SIMPLE, "  Precedence class:   %i", aParams[KPrecPos] )
    LOG2( SIMPLE, "  Precedence value:   0x%x", aResult.iReqPrecedence )

    // Get Delay class value
    aResult.iReqDelay = TDelayClass[
        CheckIndex( aParams[KDelayPos], KDelayMaxSize ) ];
    LOG2( SIMPLE, "  Delay class:        %i", aParams[KDelayPos] )
    LOG2( SIMPLE, "  Delay value:        0x%x", aResult.iReqDelay )

    // Get Reliablility value
    aResult.iReqReliability = TReliablitityClass[
        CheckIndex( aParams[KRelPos], KRelMaxSize ) ];
    LOG2( SIMPLE, "  Reliablility class: %i", aParams[KRelPos] )
    LOG2( SIMPLE, "  Reliablility value: 0x%x", aResult.iReqReliability )

    // Get Peak value
    aResult.iReqPeakThroughput = TPeak[
        CheckIndex( aParams[KPeakPos], KPeakMaxSize ) ];
    LOG2( SIMPLE, "  Peak througput:     %i", aParams[KPeakPos] )
    LOG2( SIMPLE, "  Peak througput:     0x%x", aResult.iReqPeakThroughput )

    // Get Mean value
    if ( KMeanBestEffort != aParams[KMeanPos] )
        {
        LOG( SIMPLE, "SATENGINE: TSatQoSParser::GetRequestedQoSValues \
        KMeanBestEffort != aParams[KMeanPos]" )
        aResult.iReqMeanThroughput = TMean[
            CheckIndex( aParams[KMeanPos], KMeanMaxSize ) ];
        }
    else
        {
        aResult.iReqMeanThroughput = RPacketQoS::EMeanThroughputBestEffort;
        }
    LOG2( SIMPLE, "  Mean througput:     %i", aParams[KMeanPos] )
    LOG2( SIMPLE, "  Mean througput(int):%i", aResult.iReqMeanThroughput )

    LOG( SIMPLE, "SATENGINE: TSatQoSParser::GetRequestedQoSValues exiting" )
    }

// ---------------------------------------------------------------------------
// GetRequestedQoSValues
// ---------------------------------------------------------------------------
//
void TSatQoSParser::GetNegotiatedQoSValues(
    const RPacketQoS::TQoSGPRSNegotiated& aNegParams, 
    RSat::TBearerParams& aResult )
    {
    LOG( SIMPLE, "SATENGINE: TSatQoSParser::GetNegotiatedQoSValues calling" )

    aResult.SetLength( KSimQoSSize );
    TInt index( 0 );

    // Get Precedence class by value
    for ( index = 0; index < KPrecMaxSize; index++ )
        {
        if ( TPrecedenceClass[index] == aNegParams.iPrecedence )
            {
             // Found, store index
            aResult[KPrecPos] = static_cast<TUint8>( index );
            index = KPrecMaxSize; // Stop loop
            LOG2( SIMPLE, "  Prececence class:  %i", aResult[KPrecPos] )
            LOG2( SIMPLE, "  Prececence value:  0x%x", aNegParams.iPrecedence )
            }
        }
    LOG2( SIMPLE, "SATENGINE: TSatQoSParser::GetNegotiatedQoSValues index:%i",
          index )
    // Get Delay class by value
    for ( index = 0; index < KDelayMaxSize; index++ )
        {
        if ( TDelayClass[index] == aNegParams.iDelay )
            {
             // Found, store index
            aResult[KDelayPos] = static_cast<TUint8>( index );
            index = KDelayMaxSize; // Stop loop
            LOG2( SIMPLE, "  Delay class:       %i", aResult[KDelayPos] )
            LOG2( SIMPLE, "  Delay value:       0x%x", aNegParams.iDelay )
            }
        }
    LOG2( SIMPLE, "SATENGINE: TSatQoSParser::GetNegotiatedQoSValues index:%i",
          index )
    // Get Reliability class by value
    for ( index = 0; index < KRelMaxSize; index++ )
        {
        if ( TReliablitityClass[index] == aNegParams.iReliability )
            {
             // Found, store index
            aResult[KRelPos] = static_cast<TUint8>( index );
            index = KRelMaxSize; // Stop loop
            LOG2( SIMPLE, "  Reliability class: %i", aResult[KRelPos] )
            LOG2( SIMPLE, "  Reliability value: 0x%x", aNegParams.iReliability )
            }
        }
    LOG2( SIMPLE, "SATENGINE: TSatQoSParser::GetNegotiatedQoSValues index:%i",
          index )
    // Get Peak by value
    for ( index = 0; index < KPeakMaxSize; index++ )
        {
        if ( TPeak[index] == aNegParams.iPeakThroughput )
            {
             // Found, store index
            aResult[KPeakPos] = static_cast<TUint8>( index );
            index = KPeakMaxSize; // Stop loop
            LOG2( SIMPLE, "  Peak class:        %i", aResult[KPeakPos] )
            LOG2( SIMPLE, "  Peak value:        0x%x", 
                aNegParams.iPeakThroughput )
            }
        }
    LOG2( SIMPLE, "SATENGINE: TSatQoSParser::GetNegotiatedQoSValues index:%i",
          index )
    // Get Mean by value
    for ( index = 0; index < KMeanMaxSize; index++ )
        {
        if ( TMean[index] == aNegParams.iMeanThroughput )
            {
            if ( RPacketQoS::EMeanThroughputBestEffort ==
                aNegParams.iMeanThroughput )
                {
                LOG( SIMPLE, "SATENGINE: TSatQoSParser::GetNegotiatedQoSValues \
                EMeanThroughputBestEffort" )
                // Best effort is not value '19', handle this case differently
                aResult[4] = KMeanBestEffort;
                }
            else
                {
                // All other values are linear from '0' to '18'
                // store index
                aResult[KMeanPos] = static_cast<TUint8>( index ); 
                }

            index = KMeanMaxSize; // Stop loop
            LOG2( SIMPLE, "  Mean class:        %i", aResult[KMeanPos] )
            LOG2( SIMPLE, "  Mean value:        0x%x", 
                aNegParams.iMeanThroughput )
            }
        }
    LOG2( SIMPLE, "SATENGINE: TSatQoSParser::GetNegotiatedQoSValues index:%i",
          index )
    LOG( SIMPLE, "SATENGINE: TSatQoSParser::GetNegotiatedQoSValues exiting" )
    }

// ---------------------------------------------------------------------------
// CheckIndex
// ---------------------------------------------------------------------------
//
TInt TSatQoSParser::CheckIndex( TInt aReqIndex, TInt aMaxIndex )
    {
    LOG( SIMPLE, "SATENGINE: TSatQoSParser::CheckIndex calling" )
    TInt retVal( aReqIndex );

    if ( aMaxIndex < aReqIndex )
        {
        LOG( SIMPLE, "SATENGINE: TSatQoSParser::CheckIndex \
        aMaxIndex < aReqIndex" )
        retVal = 0;
        }
    LOG( SIMPLE, "SATENGINE: TSatQoSParser::CheckIndex exiting" )
    return retVal;    
    }

// ---------------------------------------------------------------------------
// Convert QoS parameters from Rel97/98 format to Rel5 format
// ---------------------------------------------------------------------------
//
TSatQoSParser::TQoSRel5 TSatQoSParser::ConvertQoS( 
    const RPacketQoS::TQoSGPRSRequested& aReqParams )
    {
    LOG( SIMPLE, "SATENGINE: TSatQoSParser::ConvertQoS calling" )
    
    TQoSRel5 qos;
    LOG2( SIMPLE, "SATENGINE: TSatQoSParser::ConvertQoS \
          aReqParams.iReqDelay: %i", aReqParams.iReqDelay )
    // Map Delay class value to Traffic class/Traffic handling priority
    // Delay Class ----------- Traffic Class
    // 1,2,3                    Interactive
    // 4                        Background
    // Delay Class ----------- Traffic Handling priority
    // 1                            1
    // 2                            2
    // 3                            3
    // 4                            unspecified
    switch ( aReqParams.iReqDelay )
        {
        case RPacketQoS::EDelayClass1:
            {
            qos.iTrafficClass = RPacketQoS::ETrafficClassInteractive;
            qos.iTrafficHandlingPriority = RPacketQoS::ETrafficPriority1;
            break;
            }
        case RPacketQoS::EDelayClass2:
            {
            qos.iTrafficClass = RPacketQoS::ETrafficClassInteractive;
            qos.iTrafficHandlingPriority = RPacketQoS::ETrafficPriority2;
            break;
            }
        case RPacketQoS::EDelayClass3:
            {
            qos.iTrafficClass = RPacketQoS::ETrafficClassInteractive;
            qos.iTrafficHandlingPriority = RPacketQoS::ETrafficPriority3;
            break;
            }
        case RPacketQoS::EDelayClass4:
            {
            qos.iTrafficClass = RPacketQoS::ETrafficClassBackground;
            qos.iTrafficHandlingPriority = RPacketQoS::ETrafficPriorityUnspecified;
            break;    
            }
        default:
            {
            qos.iTrafficClass = RPacketQoS::ETrafficClassUnspecified;
            qos.iTrafficHandlingPriority = RPacketQoS::ETrafficPriorityUnspecified;
            break;    
            }
        }
    LOG2( SIMPLE, "SATENGINE: TSatQoSParser::ConvertQoS \
          aReqParams.iReqReliability: %i", aReqParams.iReqReliability )
    // Map Reliability class value to SDU error ratio/
    // Residual bit error ratio/Delivery of erroneous SDUs
    // Reliability Class ----------- SDU error ratio
    //  1,2                             0.000001
    //  3                               0.0001
    //  4,5                             0.001
    // Reliability Class ----------- Residual bit error ratio
    //  1,2,3,4                         0.00001
    //  5                               0.004  
    // Reliability Class ----------- Delivery of erroneous SDUs
    //  1,2,3,4                         No        
    //  5                               Yes
    switch ( aReqParams.iReqReliability )
        {
        case RPacketQoS::EReliabilityClass1:
            {
            qos.iSduErrorRatio = 
                RPacketQoS::ESDUErrorRatioOnePerMillion;
            qos.iResidualBer = RPacketQoS::EBEROnePerHundredThousand;
            qos.iDeliveryErroneousSdu = 
                RPacketQoS::EErroneousSDUDeliveryNotRequired;
            break;
            }
        case RPacketQoS::EReliabilityClass2:
            {
            qos.iSduErrorRatio = RPacketQoS::ESDUErrorRatioOnePerMillion;
            qos.iResidualBer = RPacketQoS::EBEROnePerHundredThousand;
            qos.iDeliveryErroneousSdu = 
                RPacketQoS::EErroneousSDUDeliveryNotRequired;
            break;
            }
        case RPacketQoS::EReliabilityClass3:
            {
            qos.iSduErrorRatio = 
                RPacketQoS::ESDUErrorRatioOnePerTenThousand;
            qos.iResidualBer = RPacketQoS::EBEROnePerHundredThousand;
            qos.iDeliveryErroneousSdu = 
                RPacketQoS::EErroneousSDUDeliveryNotRequired;
            break;
            }
        case RPacketQoS::EReliabilityClass4:
            {
            qos.iSduErrorRatio = RPacketQoS::ESDUErrorRatioOnePerThousand;
            qos.iResidualBer = RPacketQoS::EBEROnePerHundredThousand;
            qos.iDeliveryErroneousSdu = 
                RPacketQoS::EErroneousSDUDeliveryNotRequired;
            break;
            }
        case RPacketQoS::EReliabilityClass5:
            {
            qos.iSduErrorRatio = RPacketQoS::ESDUErrorRatioOnePerThousand;
            qos.iResidualBer = RPacketQoS::EBERFourPerThousand;
            qos.iDeliveryErroneousSdu = 
                RPacketQoS::EErroneousSDUDeliveryRequired;
            break;
            }    
        default:
            {
            qos.iSduErrorRatio = RPacketQoS::ESDUErrorRatioUnspecified;
            qos.iResidualBer = RPacketQoS::EBERUnspecified;
            qos.iDeliveryErroneousSdu = 
                RPacketQoS::EErroneousSDUDeliveryNotRequired;
            break;
            }
        }
    LOG2( SIMPLE, "SATENGINE: TSatQoSParser::ConvertQoS \
          aReqParams.iReqPeakThroughput: %i", aReqParams.iReqPeakThroughput )
    // Map Peak throughout class value to Maximum bitrate[kbps]
    // Peak throughout Class ---- Maximum bitrate[kbps]
    //      1                               8                               
    //      2                               16
    //      3                               32
    //      4                               64
    //      5                               128
    //      6                               256
    //      7                               512
    //      8                               1024    
    //      9                               2048
    switch ( aReqParams.iReqPeakThroughput )
        {
        case RPacketQoS::EPeakThroughput1000:
            {
            qos.iMaxBitRate = ESatMaxBitRate1;
            break;
            }
            
        case RPacketQoS::EPeakThroughput2000:
            {
            qos.iMaxBitRate = ESatMaxBitRate2;
            break;
            }
        case RPacketQoS::EPeakThroughput4000:
            {
            qos.iMaxBitRate = ESatMaxBitRate3;
            break;
            }
        case RPacketQoS::EPeakThroughput8000:
            {
            qos.iMaxBitRate = ESatMaxBitRate4;
            break;
            }
        case RPacketQoS::EPeakThroughput16000:
            {
            qos.iMaxBitRate = ESatMaxBitRate5;
            break;
            }
        case RPacketQoS::EPeakThroughput32000:
            {
            qos.iMaxBitRate = ESatMaxBitRate6;
            break;
            }
        case RPacketQoS::EPeakThroughput64000:
            {
            qos.iMaxBitRate = ESatMaxBitRate7;
            break;    
            }
        case RPacketQoS::EPeakThroughput128000:
            {
            qos.iMaxBitRate = ESatMaxBitRate8;
            break;    
            }
        case RPacketQoS::EPeakThroughput256000:
            {
            qos.iMaxBitRate = ESatMaxBitRate9;
            break;    
            }    
        default:
            {
            qos.iMaxBitRate = ESatMaxBitRateUnspecified;
            break;    
            }
        }
    
    // Fixed value
    qos.iMaximumSDUSize = KMaximumSDUSize;
    qos.iDeliveryOrder = RPacketQoS::EDeliveryOrderNotRequired;
    	
    LOG( SIMPLE, "SATENGINE: TSatQoSParser::ConvertQoS exiting" )
    
    return qos;
    }

// ---------------------------------------------------------------------------
// Convert QoS parameters from Rel5 format to Rel97/98 format
// ---------------------------------------------------------------------------
//
RPacketQoS::TQoSGPRSNegotiated TSatQoSParser::ConvertNegotiatedQoS(
    const TQoSRel5& aNegQoSRel5 )
    {
    LOG( SIMPLE, "SATENGINE: CSatBIPGPRSDataChannel::ConvertNegotiatedQoS \
        calling" )
    
    RPacketQoS::TQoSGPRSNegotiated negQos;
    LOG2( SIMPLE, "SATENGINE: TSatQoSParser::ConvertNegotiatedQoS \
          aNegQoSRel5.iTrafficClass: %i", aNegQoSRel5.iTrafficClass )
    // Map Traffic class/Traffic handling priority to Delay class value
    // Traffic Class/Traffic Handling priority----------- Delay Class
    // conversational                                       1
    // streaming                                            1
    // Interactive/1                                        1
    // Interactive/2                                        2
    // Interactive/3                                        3
    // Background                                           4
    
    switch ( aNegQoSRel5.iTrafficClass )
        {
        case RPacketQoS::ETrafficClassConversational:
            {
            negQos.iDelay = RPacketQoS::EDelayClass1;
            break;
            }
        case RPacketQoS::ETrafficClassStreaming:
            {
            negQos.iDelay = RPacketQoS::EDelayClass1;
            break;
            }
        case RPacketQoS::ETrafficClassInteractive:
            {
            LOG2( SIMPLE, "SATENGINE: TSatQoSParser::ConvertNegotiatedQoS \
            priority: %i", aNegQoSRel5.iTrafficHandlingPriority )
            switch ( aNegQoSRel5.iTrafficHandlingPriority )
                {
                case RPacketQoS::ETrafficPriority1:
                    {
                    negQos.iDelay = RPacketQoS::EDelayClass1;
                    break;
                    }
                case RPacketQoS::ETrafficPriority2:
                    {
                    negQos.iDelay = RPacketQoS::EDelayClass2;
                    break;
                    }
                case RPacketQoS::ETrafficPriority3:
                    {
                    negQos.iDelay = RPacketQoS::EDelayClass3;
                    break;
                    }
                default:
                    {
                    negQos.iDelay = RPacketQoS::EUnspecifiedDelayClass;
                    break;
                    }
                }
            break;
            }
        case RPacketQoS::ETrafficClassBackground:
            {
            negQos.iDelay = RPacketQoS::EDelayClass4;
            break;    
            }
        default:
            {
            negQos.iDelay = RPacketQoS::EUnspecifiedDelayClass;
            break;    
            }
        }
    LOG2( SIMPLE, "SATENGINE: TSatQoSParser::ConvertNegotiatedQoS \
          aNegQoSRel5.iSduErrorRatio: %i", aNegQoSRel5.iSduErrorRatio )
    // Map SDU error ratio/Residual bit error ratio to Reliability value
    // SDU error ratio/Residual bit error ratio -------- Reliability Class
    // 0.000001                                                     2
    // 0.00001                                                      2
    // 0.0001                                                       3
    // 0.001,0.007,0.01,0.1/0.0001,0.00001, 0.000001,0.00000006     4                         
    // 0.001,0.007,0.01,0.1/0.001,0.004,0.005,0.01,0.05             5
    switch ( aNegQoSRel5.iSduErrorRatio )
        {
        case RPacketQoS::ESDUErrorRatioOnePerHundredThousand:
        case RPacketQoS::ESDUErrorRatioOnePerMillion:
            {
            negQos.iReliability = RPacketQoS::EReliabilityClass2;
            break;
            }
        case RPacketQoS::ESDUErrorRatioOnePerTenThousand:
            {
            negQos.iReliability = RPacketQoS::EReliabilityClass3;
            break;
            }
        case RPacketQoS::ESDUErrorRatioOnePerThousand:
        case RPacketQoS::ESDUErrorRatioSevenPerThousand:
        case RPacketQoS::ESDUErrorRatioOnePerHundred:
        case RPacketQoS::ESDUErrorRatioOnePerTen:
            {
            LOG2( SIMPLE, "SATENGINE: TSatQoSParser::ConvertNegotiatedQoS \
            aNegQoSRel5.iResidualBer: %i", aNegQoSRel5.iResidualBer )
            switch( aNegQoSRel5.iResidualBer )
                {
                case RPacketQoS::EBEROnePerTenThousand:
                case RPacketQoS::EBEROnePerHundredThousand:
                case RPacketQoS::EBEROnePerMillion:
                case RPacketQoS::EBERSixPerHundredMillion:
                    {
                    negQos.iReliability = RPacketQoS::EReliabilityClass4;
                    break;
                    }
                case RPacketQoS::EBERFivePerHundred:
                case RPacketQoS::EBEROnePerHundred:
                case RPacketQoS::EBERFivePerThousand:
                case RPacketQoS::EBERFourPerThousand:
                case RPacketQoS::EBEROnePerThousand: 
                    {
                    negQos.iReliability = RPacketQoS::EReliabilityClass5;
                    break;
                    }
                default:
                    {
                    negQos.iReliability = RPacketQoS::EUnspecifiedReliabilityClass;
                    break;
                    }
                }
            break;
            }
        default:
            {
            negQos.iReliability = RPacketQoS::EUnspecifiedReliabilityClass;
            break;
            }
        }
        
    // Map Maximum bitrate [kbps] to Peak throughout class
    // Maximum bitrate[kbps] ---- Peak throughout Class
    // 
    //      < 16                               1                               
    //      16 <= x < 32                       2
    //      32 <= x < 64                       3
    //      64 <= x < 128                      4
    //      128 <= x < 256                     5
    //      256 <= x < 512                     6
    //      512 <= x < 1024                    7
    //      1024 <= x < 2048                   8    
    //      >= 2048                            9
    if ( aNegQoSRel5.iMaxBitRate < ESatMaxBitRate2 )
        {
        LOG( SIMPLE, "SATENGINE: CSatBIPGPRSDataChannel::\
             ConvertNegotiatedQoS rate: rate< 16" )
        negQos.iPeakThroughput = RPacketQoS::EPeakThroughput1000;
        }
    else if ( ( aNegQoSRel5.iMaxBitRate < ESatMaxBitRate3 ) && 
                ( aNegQoSRel5.iMaxBitRate >= ESatMaxBitRate2 ) )
        {
        LOG( SIMPLE, "SATENGINE: CSatBIPGPRSDataChannel::\
             ConvertNegotiatedQoS rate: 16 <= rate < 32" )
        negQos.iPeakThroughput = RPacketQoS::EPeakThroughput2000;
        }
    else if ( ( aNegQoSRel5.iMaxBitRate < ESatMaxBitRate4 ) && 
                ( aNegQoSRel5.iMaxBitRate >= ESatMaxBitRate3 ) )
        {
        LOG( SIMPLE, "SATENGINE: CSatBIPGPRSDataChannel::\
             ConvertNegotiatedQoS rate: 32 <= rate < 64" )
        negQos.iPeakThroughput = RPacketQoS::EPeakThroughput4000;
        }
    else if ( ( aNegQoSRel5.iMaxBitRate < ESatMaxBitRate5 ) && 
                ( aNegQoSRel5.iMaxBitRate >= ESatMaxBitRate4 ) )
        {
        LOG( SIMPLE, "SATENGINE: CSatBIPGPRSDataChannel::\
             ConvertNegotiatedQoS rate: 64 <= rate < 128" )
        negQos.iPeakThroughput = RPacketQoS::EPeakThroughput8000;
        }
    else if ( ( aNegQoSRel5.iMaxBitRate < ESatMaxBitRate6 ) && 
                ( aNegQoSRel5.iMaxBitRate >= ESatMaxBitRate5 ) )
        {
        LOG( SIMPLE, "SATENGINE: CSatBIPGPRSDataChannel::\
             ConvertNegotiatedQoS rate: 128 <= rate < 256" )
        negQos.iPeakThroughput = RPacketQoS::EPeakThroughput16000;
        }
    else if ( ( aNegQoSRel5.iMaxBitRate < ESatMaxBitRate7 ) && 
                ( aNegQoSRel5.iMaxBitRate >= ESatMaxBitRate6 ) )
        {
        LOG( SIMPLE, "SATENGINE: CSatBIPGPRSDataChannel::\
             ConvertNegotiatedQoS rate: 256 <= rate < 512" )
        negQos.iPeakThroughput = RPacketQoS::EPeakThroughput32000;
        }
    else if ( ( aNegQoSRel5.iMaxBitRate < ESatMaxBitRate8 ) && 
                ( aNegQoSRel5.iMaxBitRate >= ESatMaxBitRate7 ) )
        {
        LOG( SIMPLE, "SATENGINE: CSatBIPGPRSDataChannel::\
             ConvertNegotiatedQoS rate: 512 <= rate < 1024" )
        negQos.iPeakThroughput = RPacketQoS::EPeakThroughput64000;
        }
    else if ( ( aNegQoSRel5.iMaxBitRate < ESatMaxBitRate9 ) && 
                ( aNegQoSRel5.iMaxBitRate >= ESatMaxBitRate8 ) )
        {
        LOG( SIMPLE, "SATENGINE: CSatBIPGPRSDataChannel::\
             ConvertNegotiatedQoS rate: 1024 <= rate < 2048" )
        negQos.iPeakThroughput = RPacketQoS::EPeakThroughput128000;
        }
    else if ( aNegQoSRel5.iMaxBitRate >= ESatMaxBitRate9 )
        {
        LOG( SIMPLE, "SATENGINE: CSatBIPGPRSDataChannel::\
             ConvertNegotiatedQoS rate: rate >= 2048" )
        negQos.iPeakThroughput = RPacketQoS::EPeakThroughput256000;
        }
    else
        {
        LOG( SIMPLE, "SATENGINE: CSatBIPGPRSDataChannel::\
             ConvertNegotiatedQoS others" )
        negQos.iPeakThroughput = RPacketQoS::EUnspecifiedPeakThroughput;
        }
    
    // Fixed value
    // UE shall set the R97/98 precedence class to value "subscribed"  
    negQos.iPrecedence = RPacketQoS::EUnspecifiedPrecedence;
    // Always set to 31, best effort
    negQos.iMeanThroughput = RPacketQoS::EMeanThroughputBestEffort;  
    
    LOG( SIMPLE, "SATENGINE: CSatBIPGPRSDataChannel::ConvertNegotiatedQoS \
        exiting" )
    
    return negQos;
    }
    
