/**
* Copyright (c) 2005-2009 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:
*
*/



/**
 @file
*/
 
#ifndef __CSCHSENDSCRIPTSUTILS_H__
#define __CSCHSENDSCRIPTSUTILS_H__

#include <e32std.h>

// Forward declarations
class CMsvSysAgentActions;
class CMsvScheduleSettings;

/**
This utility class reads the settings for the two objects CMsvScheduleSettings
and CMsvSysAgentActions from the configuration file.

@internalTechnology
*/
class CSchSendScriptUtils
	{
	public:
		static void ReadScheduleSettingsFromConfigurationFileL(const TDesC& aSettingsFile, CMsvSysAgentActions& aSysAgentActions);
		static void ReadScheduleSettingsFromConfigurationFileL(const TDesC& aSettingsFile, CMsvScheduleSettings& aScheduleSettings);
	};

#endif //__CSCHSENDSCRIPTSUTILS_H__