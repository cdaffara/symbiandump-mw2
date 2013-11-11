/*
* Copyright (c) 2007 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  This file contains definitions of ProfileSettingsMonitorServer.
*
*/

#ifndef __PROFILESETTINGSMONITORSERVER_H__
#define __PROFILESETTINGSMONITORSERVER_H__

#include <e32base.h>
#include <e32std.h>

class CProfileSettingsMonitorServerImpl;

class CProfileSettingsMonitorServer: public CServer2
	{
public:
	static CProfileSettingsMonitorServer* NewL(); 
	virtual CSession2* NewSessionL(const TVersion& aVersion,const RMessage2& aMessage) const;
	~CProfileSettingsMonitorServer();

private:
	
	CProfileSettingsMonitorServer();
	void ConstructL();

private:
    CProfileSettingsMonitorServerImpl *iServerImpl;
	};

#endif
