/**
* Copyright (c) 2004-2009 Nokia Corporation and/or its subsidiary(-ies).
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


#ifndef __CMTF_TEST_ACTION_UTILS_POP_SCRIPTS_H__
#define __CMTF_TEST_ACTION_UTILS_POP_SCRIPTS_H__

#include <e32std.h>


// Forward declarations
class CMtfTestCase;
class CMtfTestActionUtilsConfigFileParser;
class CImIAPPreferences;
class CImPop3Settings;
class CImSmtpSettings;

class CMtfTestActionUtilsPopScripts
{

public:		
	static void ReadPopSettingsFromConfigurationFileL(CMtfTestCase& aTestCase, const TDesC& aSettingsFile, 
											CImPop3Settings& aPopSet, CImIAPPreferences& aPopIapPref, 
											CImSmtpSettings& aSmtpSet, CImIAPPreferences& aSmtpIapPref);

private:
	static void ReadIAPAndAddL( CMtfTestCase& aTestCase, 
			CMtfTestActionUtilsConfigFileParser* aScriptFileParser , 
			const TDesC& aParamListName , CImIAPPreferences& aIapPref );
	static void _ReadIAPAndAddL( CMtfTestCase& aTestCase, 
			CMtfTestActionUtilsConfigFileParser* aScriptFileParser , 
			const TDesC& aFieldName , CImIAPPreferences& aIapPref );
	static TBool GetString( const TDesC& aStringPtr, TInt& aStart, 
			TInt aEnd, TPtrC& aBaseString );


private:
	CMtfTestActionUtilsPopScripts() {};
	~CMtfTestActionUtilsPopScripts() {};
};

#endif
