/**
* Copyright (c) 2006-2009 Nokia Corporation and/or its subsidiary(-ies).
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
* This is the header file for CT_MsgUtilsReadEmailSettingsFromConfigFile
* 
*
*/



/**
 @file
*/

 
#ifndef __T_READ_EMAIL_SETTINGS_FROM_CONFIG_FILE_H__
#define __T_READ_EMAIL_SETTINGS_FROM_CONFIG_FILE_H__

_LIT(KNone, "NONE");

// Forward declarations
class CImIAPPreferences;
class CImImap4Settings;
class CImPop3Settings;
class CImSmtpSettings;
class CT_MsgUtilsConfigFileParserUtility;
class TImapAccount;


/**
CT_MsgUtilsReadEmailSettingsFromConfigFile is the utility class used to read the
fields from the configuration/setting file and and update the set the data members of 
CImImap4Settings, CImPop3Settings, CImSmtpSettings and CImIAPPreferences objects.
*/
class CT_MsgUtilsReadEmailSettingsFromConfigFile
	{
public:	
	CT_MsgUtilsReadEmailSettingsFromConfigFile() {};
	
	~CT_MsgUtilsReadEmailSettingsFromConfigFile() {};
	
	IMPORT_C static TBool CompareIapPrefs(CImIAPPreferences& prefs1, CImIAPPreferences& prefs2);
	
	IMPORT_C static void ReadImapSettingsFromConfigurationFileL(TDesC& aSettingsFile,
							CImImap4Settings& aImapSet, CImIAPPreferences& aImapIapPref, const TDesC& aPtrEmailAdd = KNone());
										
	IMPORT_C static void ReadPopSettingsFromConfigurationFileL(TDesC& aSettingsFile,
							CImPop3Settings& aPopSet, CImIAPPreferences& aPopIapPref, const TDesC& aPtrEmailAdd = KNone());
										
	IMPORT_C static void ReadSmtpSettingsFromConfigurationFileL(TDesC& aSettingsFile,
							CImSmtpSettings& aSmtpSet, CImIAPPreferences& aSmtpIapPref,const TDesC& aPtrEmailAdd = KNone());
	
private:
	IMPORT_C static void ReadImapIAPAndAddL(CT_MsgUtilsConfigFileParserUtility* aScriptFileParser,
			const TDesC& aParamListName, CImIAPPreferences& aIapPref);
			
	IMPORT_C static void ReadImapIAPAndAddFieldL(CT_MsgUtilsConfigFileParserUtility* aScriptFileParser, 
			const TDesC& aFieldName, CImIAPPreferences& aIapPref);
	
	IMPORT_C static void ReadPopIAPAndAddL(CT_MsgUtilsConfigFileParserUtility* aScriptFileParser,
			const TDesC& aParamListName, CImIAPPreferences& aIapPref);
			
	IMPORT_C static void ReadPopIAPAndAddFieldL(CT_MsgUtilsConfigFileParserUtility* aScriptFileParser, 
			const TDesC& aFieldName, CImIAPPreferences& aIapPref);
			
	IMPORT_C static void ReadSmtpIAPAndAddL(CT_MsgUtilsConfigFileParserUtility* aScriptFileParser, 
			const TDesC& aParamListName, CImIAPPreferences& aSmtpIapPref);
			
	IMPORT_C static void ReadSmtpIAPAndAddFieldL(CT_MsgUtilsConfigFileParserUtility* aScriptFileParser, 
			const TDesC& aFieldName, CImIAPPreferences& aIapPref);
			
	IMPORT_C static TBool GetString(TDesC& aStringPtr, TInt& aStart, TInt aEnd, TPtrC& aBaseString);
	};
#endif //__T_READ_EMAIL_SETTINGS_FROM_CONFIG_FILE_H__
