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

#ifndef __SENDPROXYCLIENT_H__
#define __SENDPROXYCLIENT_H__

#include "SendProxyClientServer.h"


//*****************************************
// Note we use a Proxy server so that 
// i) We do nothave to link to all the libs such as BAFL, etc that the main TesstMessagingServer does.
//		This avoids any problems with capability checking, such as DLL's not having correct caps.
// ii) Its easier to change the caps of the simple send server. The effects are more limited and controlled.
//*****************************************


class RProxyServerSession : public RSessionBase
	{
public:
	IMPORT_C TInt Connect();
	IMPORT_C TInt SendReceive(TInt aCommand,const TIpcArgs& aTIpcArgs);	
	IMPORT_C TInt SendReceiveProxyAsync(TInt aCommand,const TIpcArgs& aTIpcArgs);	
	IMPORT_C TInt SendReceive(TInt aCommand);	
	IMPORT_C TInt SendReceiveProxyAsync(TInt aCommand );
	IMPORT_C TInt GetServerSecureId( TSecureId& p );		
	IMPORT_C TInt GetServerCapabilities( TInt32& p );

private:
	IMPORT_C void SendReceive(TInt aCommand, const TIpcArgs& aTIpcArgs, TRequestStatus& aRequestStatus); 
	IMPORT_C void SendReceive(TInt aCommand, TRequestStatus& aRequestStatus); 

	};


TInt StartSendProxyServer(void);



#ifdef __SENDPROXYSERVER_NO_PROCESSES__
#include <e32math.h>
#endif

#endif	// __SENDPROXYCLIENT_H__
