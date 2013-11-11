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


#ifndef __CMTF_CONFIGURATION_TYPE_H__
#define __CMTF_CONFIGURATION_TYPE_H__

#include <e32base.h>

/** CMtfConfigurationType defines a configuration type, which is a set of
filenames associated with a type. It is used to specify all filenames of a particular type 
(e.g., all filenames containing SMS messages) that will be used in a particular test case. */  
class CMtfConfigurationType: public CBase
{
public:

	/** Enumerator TMtfConfigurationType defines all possible configuration file types. */
	enum TMtfConfigurationType
	{	
		EMtfDataFile,
		EMtfCommDb,
		EMtfSmsSettings,
		EMtfMmsSettings,
		EMtfImap4Settings,
		EMtfPop3Settings,
		EMtfSmtpSettings,
		EMtfSmsMessage,
		EMtfSmilFile,
		EMtfEmailFile,
		EMtfInvalidSmsMessage,
		EMtfInvalidSmilFile,
		EMtfInvalidEmailFile,
		EMtfMmsHeader,
		EMtfEmailHeader
	};
	
	/** Creates a new instance of a particular type */
	static CMtfConfigurationType* NewL(const TMtfConfigurationType& aConfigurationType);
	
	/** Finds the configuration type with the required type in a set of configuration types. */
	static TInt FindConfigurationL(const TMtfConfigurationType& aType, const RPointerArray<CMtfConfigurationType>& aConfigurations);
	
	/** Adds a configuration type to a set of configuration types. 
	This function takes ownership of aConfiguration immediately. */
	static void SetConfigurationTypeL(CMtfConfigurationType* aConfiguration,
		RPointerArray<CMtfConfigurationType>& aConfigurations);

	virtual ~CMtfConfigurationType();
	
	/** Returns the type */
	const TMtfConfigurationType& ConfigurationType() const;
	
	/** Adds a filename to the set of filenames */
	void AddConfigurationFilenameL(const TDesC& aFilename);
	
	/** Returns the filename identified by an index */
	const TDesC& ConfigurationFileL(TInt aIndex) const;
	
	/** Returns the number of filenames. */
	TInt Count() const;
	
private:
	CMtfConfigurationType(const TMtfConfigurationType& aType);
			
private:
	TMtfConfigurationType	iConfigurationType;
	RPointerArray<HBufC>	iFilenames;
};

#endif
