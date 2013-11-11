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
* Description:  This file contains definitions for CBS MCN Clients.
*
*/



#ifndef CBSMCNCOMMON_H
#define CBSMCNCOMMON_H

//  INCLUDES
#include <e32base.h>
#include <s32std.h>

// CONSTANTS

// DATA TYPES  

// MCN message max size
const TInt KCbsMcnFullMsgSize = 1320;
// MCN topic number max size
const TInt KCbsTopicNumberPageSize = 5; 

// Network in which current message was received in.
enum TCbsNetworkMode
    {
    ECbsNetworkGsm,
    ECbsNetworkWcdma,
    ECbsNetworkNotDefined
    };
    
    
// Class for MCN messages over IPC
class TCbsMcnMessage
    {
public:
    TCbsMcnMessage() :  iBuffer( NULL ), 
                        iBuffer8( NULL ), 
                        iTopicNumber( 0 ),
                        iNetworkMode( ECbsNetworkNotDefined ) {};

public: // Data
    // Buffer for the Mcn message.
    TBuf<KCbsMcnFullMsgSize> iBuffer;

    // 8-bit Buffer for MCN/Livecast messages
    TBuf8<KCbsMcnFullMsgSize> iBuffer8;
       
    //Buffer for the Topic number of the Mcn message.
    //TBuf<KCbsTopicNumberPageSize> iTopicNumber;
    TUint iTopicNumber;

    // Network in which current message was received
    TCbsNetworkMode iNetworkMode;
    };

// Livecast topic range
const TUint KCbsLivecastTopicMin = 40960;
const TUint KCbsLivecastTopicMax = 45055;

#endif      // CBSMCNCOMMON_H   
            
// End of File

