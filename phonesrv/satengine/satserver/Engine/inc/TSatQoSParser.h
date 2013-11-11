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
* Description:  Maps QoS classes to correxponding Hex values
*
*/


#ifndef TSATQOSPARSER_H
#define TSATQOSPARSER_H

#include <e32std.h>
#include <etelsat.h>
#include <etelqos.h>

/**
 *  Parses QoS Classes coming from SIM in open channel to corresponding 
 *  values to use with RPacketQoS and vice versa.
 *
 *  Bearer params are in order:
 *   0 = Precedence class
 *   1 = Delay class
 *   2 = Reliability class
 *   3 = Peak throughput
 *   4 = Mean throughput
 *   (5 = protocol)
 *
 *
 *  @lib SatEngine.lib
 *  @since 3.0
 */
class TSatQoSParser
    {
    
public:
        
    /**  Enum for Maximum bit rate */
    enum TSatMaxBitRate
        {
        ESatMaxBitRateUnspecified = 0,
        ESatMaxBitRate1 = 8,
        ESatMaxBitRate2 = 16,
        ESatMaxBitRate3 = 32,
        ESatMaxBitRate4 = 64,
        ESatMaxBitRate5 = 128,
        ESatMaxBitRate6 = 256,
        ESatMaxBitRate7 = 512,
        ESatMaxBitRate8 = 1024,
        ESatMaxBitRate9 = 2048,
        };
        
    /**  Struct for converting QoS params */
    struct TQoSRel5
        {
        TInt iTrafficClass;             // Traffic Class
        TInt iTrafficHandlingPriority;  // Traffic handling priority
        TInt iSduErrorRatio;            // SDU error ratio            
        TInt iResidualBer;              // Residual bit error ratio
        TInt iDeliveryErroneousSdu;     // Delivery of Erroneous SDUs
        TInt iMaxBitRate;               // Maximum bitrate
        TInt iMaximumSDUSize;           // Maximum SDU size
        TInt iDeliveryOrder;            // Delivery Order
        };

// New functions
        
    /**
     * Converts requested QoS classes to corresponding Hex values used
     * by ETel Packet when requesting QoS parameters
     * 
     * @param aParams Requested QoS classes coming from SIM
     * @param aResult Output QoS values in RPacketQoS::TQoSGPRSRequested
     */
    static void GetRequestedQoSValues(
        RSat::TBearerParams& aParams, 
        RPacketQoS::TQoSGPRSRequested& aResult);
        
    /**
     * Converts negotiated QoS values to corresponding QoS class values used
     * in OpenChannel terminal response.
     * 
     * @param aNegParams Negotiated QoS parameters
     * @param aResult Output QoS class values in RSat::TBearerParams
     */ 
    static void GetNegotiatedQoSValues( 
        const RPacketQoS::TQoSGPRSNegotiated& aNegParams, 
        RSat::TBearerParams& aResult );
        
    /**
     * Convert QoS parameters from Rel97/98 format to Rel5 format
     * 
     * @param aReqParams: Requested QoS parameters in Rel97/98 format
     * @return QoS paramters in Rel5 format
     */ 
    static TQoSRel5 ConvertQoS( 
        const RPacketQoS::TQoSGPRSRequested& aReqParams );
            
    /**
     * Convert QoS parameters from Rel5 format to Rel97/98 format
     * 
     * @param aNegQoSRel5: Negotiated QoS parameters in Rel5 format
     * @return Negotiated QoS paramters in Rel97/98 format
     */ 
    static RPacketQoS::TQoSGPRSNegotiated ConvertNegotiatedQoS( 
        const TQoSRel5& aNegQoSRel5 );
        
private:

// New functions
        
    /**
     * Checks is requested index bigger than maximum lenght to prevent 
     * array overflows
     *
     * @param aReqIndex Requested index
     * @param aMaxIndex Last index, Requested cannot be bigger than this
     * @param Retuns aReqIndex, if it's not bigger that aMaxIndex, else 0
     */ 
    static TInt CheckIndex( TInt aReqIndex, TInt aMaxIndex );
    };

#endif      // TSATQOSPARSER_H

// End of File
