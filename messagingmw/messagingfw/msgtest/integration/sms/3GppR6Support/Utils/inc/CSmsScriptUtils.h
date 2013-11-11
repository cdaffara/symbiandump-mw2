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


#ifndef __CSMSSCRIPTSUTILS_H__
#define __CSMSSCRIPTSUTILS_H__

#include <smuthdr.h>

class CSmsScriptUtils
	{
	public:
		static void ReadSmsSettingsFromConfigurationFileL(const TDesC& settingsFile, CSmsSettings& smsSettings);
		static TInt SetSmsHeaderInfoFromConfigurationFileL(const TDesC& aSettingsFile, CSmsHeader& aSmsHeader);
		static TInt GetSmsMessageBodyL(const TDesC& aSettingsFile, TPtrC& aSmsMessageBody);
	private:
		static TBioMsgIdType GetTBioMsgIdType(TDesC& value);
		static CSmsSettings::TMobileSmsBearer GetMobileSmsBearer(TDesC& value);
		static TSmsDataCodingScheme::TSmsAlphabet GetSmsAlphabet(TDesC& value);
		static TSmsDelivery GetSmsDelivery(TDesC& value);
		static CSmsSettings::TSmsReportHandling GetSmsReportHandling(TDesC& value);
		static TSmsPIDConversion GetSmsPIDConversion(TDesC& value);
		static CSmsSettings::TSmsSettingsCommDbAction GetSmsSettingsCommDbAction (TDesC& value);
		static TSmsFirstOctet::TSmsValidityPeriodFormat GetSmsValidityPeriodFormat(TDesC& value);
		static TBioMsgIdType GetBioMsgIdType(TDesC& value);
		CSmsScriptUtils() {};
		~CSmsScriptUtils() {};
	};

#endif //__CSMSSCRIPTSUTILS_H__