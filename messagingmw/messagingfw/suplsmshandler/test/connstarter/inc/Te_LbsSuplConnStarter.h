// Copyright (c) 2007-2009 Nokia Corporation and/or its subsidiary(-ies).
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
// The SUPL Ethernet Connection Starter plug-in class. 
// 
//

#include <e32base.h>
#include <es_sock.h>
#include <es_enum.h>

#if (!defined __TE_LBSSUPLCONNSTARTER_H__)
#define __TE_LBSSUPLCONNSTARTER_H__


class CTe_LbsSuplConnStarter : public CActive
	{
	
private:
	enum TState
		{
		EDisconnected,
		EConnecting,
		EConnected
		};	
	
public:
	static CTe_LbsSuplConnStarter* NewL(TAny* aWatcherParams);
	
	virtual ~CTe_LbsSuplConnStarter();
	
private:
	CTe_LbsSuplConnStarter();
	void ConstructL();

	void RunL();
	void DoCancel();
	
	void StartConnection();
	
private:
	TState iState;

	RSocketServ iSocketServ;
	RConnection iConnection;
	}; 
	
#endif //__TE_LBSSUPLCONNSTARTER_H__

