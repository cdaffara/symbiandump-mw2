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
* Description:   Common header file between TNE client and TNE server
*
*/



#ifndef _HX_TNE_Client_Server_Common_H_
#define  _HX_TNE_Client_Server_Common_H_

#include <e32base.h>
#include <gdi.h>
#include <fbs.h>


// server name

_LIT(KTneServerName, "HLXTNESERVER");

// A version must be specifyed when creating a session with the server

const TUint KTNEServMajorVersionNumber=0;
const TUint KTNEServMinorVersionNumber=1; 
const TUint KTNEServBuildVersionNumber=1;

TInt StartThread(RThread& aServerThread,  HBufC *pServerName);

// TNEMetaData is used by client to fetch metadata from the server
struct TNEMetaData
{
    TInt   iWidth;
    TInt   iHeight;
    TInt64 iFrameCount;
};

// TNEThumbRequest is used by client to send the GetThumb request to
// the server.

struct TNEThumbRequest
{
    TInt            iIndex;
    TUint8* 	    iYUVBuffer;    
};

// Function codes (opcodes) used in message passing between client and server
enum TTNEServRqst
    {
    EOpenFileRFmsg=0,
    EOpenFIleNamemsg,
    EGetThumbmsg,    
    EGetMetaDatamsg,
    ECancelThumbmsg
    };



#endif // _HX_TNE_Client_Server_Common_H_
