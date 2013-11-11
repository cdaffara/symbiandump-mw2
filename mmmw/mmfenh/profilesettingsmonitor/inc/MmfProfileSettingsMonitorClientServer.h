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
* Description:  This file contains definitions of ProfileSettings server.
*
*/




#ifndef __PROFILESETTINGSMONITORSTART_H
#define __PROFILESETTINGSMONITORSTART_H

#include <e32base.h>
#include <e32std.h>

_LIT(KProfileSettingsMonitorServerName,"ProfileSettingsMonitor");
const TInt KProfileSettingsMonitorServerStackSize=0x2000;			//  8KB
const TInt KProfileSettingsMonitorServerInitHeapSize=0x1000;		//  4KB
const TInt KProfileSettingsMonitorServerMaxHeapSize=0x1000000;		// 16MB


const TUint KProfileSettingsMonitorVersion=8;
const TUint KProfileSettingsMonitorMinorVersionNumber=0;
const TUint KProfileSettingsMonitorBuildVersionNumber=1;

#endif

