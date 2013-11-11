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
* Description:   Client-Server Request codes
*
*/



#ifndef __EPOS_SUPLNETWORKIPC_H__
#define __EPOS_SUPLNETWORKIPC_H__

// 0x00000000 to 0x00000FFF not used
#define ESuplNWIpcOffset           0x5000

// 0x00005000 to 0x00005FFF reserved for RSuplNetworkServer requests
#define ESuplServerNWIpcOffset        (ESuplNWIpcOffset)

// 0x00006000 to 0x00007FFF reserved for subsession requests
#define ESuplSubsessnNWIpcOffset      (ESuplServerNWIpcOffset + 0x1000)

// 0x60000000 to 0x7FFFFFFF reserved for 3rd party extensions to the SUPL API.
// All extensions implemented by 3rd parties should be implemented in this range.
#define ESuplNWCustomIpcOffset      0x60000000


enum TSuplNWIpcId
    {
    //
    // Request IDs
    ESuplNetworkForwardSuplMsg               = ESuplServerNWIpcOffset,

    ESuplNWCustomCodesBegin         = ESuplNWCustomIpcOffset,
    };

#endif // __EPOS_SUPLNETWORKIPC_H__
