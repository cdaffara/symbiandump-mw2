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


#ifndef __TMTFTESTACTIONUTILSSCHSENDSCRIPTS_H__
#define __TMTFTESTACTIONUTILSSCHSENDSCRIPTS_H__

#include <e32std.h>

// Forward declarations
class CMsvSysAgentActions;
class CMsvScheduleSettings;
class CMtfTestCase;

/**
This untility class reads the settings for the two objects CMsvScheduleSettings
and CMsvSysAgentActions from the configuration file.

@internalTechnology
*/
class TMtfTestActionUtilsSchSendScripts
	{
	public:
		static void ReadScheduleSettingsFromConfigurationFileL(CMtfTestCase& aTestCase, const TDesC& aSettingsFile, CMsvSysAgentActions& aSysAgentActions);
		static void ReadScheduleSettingsFromConfigurationFileL(CMtfTestCase& aTestCase, const TDesC& aSettingsFile, CMsvScheduleSettings& aScheduleSettings);
	};

#endif
