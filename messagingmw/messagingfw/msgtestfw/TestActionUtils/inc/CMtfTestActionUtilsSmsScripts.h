/**
* Copyright (c) 2003-2009 Nokia Corporation and/or its subsidiary(-ies).
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


#ifndef __CMTF_TEST_ACTION_UTILS_SMS_SCRIPTS_H__
#define __CMTF_TEST_ACTION_UTILS_SMS_SCRIPTS_H__

#include <e32std.h>


// Forward declarations
class CSmsSettings;
class CMtfTestCase;
class CSmsHeader;
class CMsvStore;

class CMtfTestActionUtilsSmsScripts
	{
	public:
		static void ReadSmsSettingsFromConfigurationFileL(CMtfTestCase& aTestCase, const TDesC& settingsFile, CSmsSettings& smsSettings);
		static TInt SetSmsHeaderInfoFromConfigurationFileL(CMtfTestCase& aTestCase, const TDesC& aSettingsFile, CSmsHeader& aSmsHeader);
		static TInt GetSmsMessageBodyL(const TDesC& aSettingsFile, TPtrC& aSmsMessageBody);
	private:
		CMtfTestActionUtilsSmsScripts() {};
		~CMtfTestActionUtilsSmsScripts() {};
	};

#endif
