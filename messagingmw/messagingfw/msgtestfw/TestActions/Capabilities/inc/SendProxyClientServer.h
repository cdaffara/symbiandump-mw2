// Copyright (c) 2004-2009 Nokia Corporation and/or its subsidiary(-ies).
// All rights reserved.
// This component and the accompanying materials are made available
// under the terms of "Eclipse Public License v1.0"
// which accompanies this distribution, and is available
// at the URL "http://www.eclipse.org/legal/epl-v10.html".
//
// Initial Contributors:
// Nokia Corporation - initial contribution.
//
// Contributors:
//
// Description:
//

#ifndef __SENDPROXYCLIENTSERVER_H__
#define __SENDPROXYCLIENTSERVER_H__

#include <e32base.h>


_LIT(KSendProxyServerName,"SendProxyServer");
_LIT(KSendProxyServerImg,"SendProxyServer");		// DLL/EXE name
const TUid KServerUid3={0x1027406B};

#ifdef __SENDPROXYSERVER_NO_PROCESSES__
const TInt KSendProxyServerStackSize=0x2000;			//  8KB
const TInt KSendProxyServerInitHeapSize=0x1000;		//  4KB
const TInt KSendProxyServerMaxHeapSize=0x1000000;		// 16MB
#endif

const TInt KMaxSendProxyServerMessage=100;

enum TSendProxyMessages
	{
	ESend = 0x1000,
		// These need to way above the EMsvOperationData messages etc.
	ESendAsyncWait,
	EReceive,
	ECancelReceive,
	EGetServerSecureId,
	EGetServerCapabilities,
	ESendData,
	ENoCommandPending
	};


class CPackServerData
{
public:
	CPackServerData( TInt aCommand, const TIpcArgs* aTIpcArgs) : iCommand(aCommand) , iTIpcArgs(aTIpcArgs)
	{		
	};
	
public:	
	TInt iCommand;
	const TIpcArgs* iTIpcArgs;
};


	// This is the server that we are trying to start eventually.
	// We forward all messages to this.
_LIT(KMsvServerNameExe	,"MSExe");


#endif
