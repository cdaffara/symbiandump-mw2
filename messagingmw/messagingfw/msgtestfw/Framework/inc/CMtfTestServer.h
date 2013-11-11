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


#ifndef __CMTF_TEST_SERVER_H__
#define __CMTF_TEST_SERVER_H__

#include <test/testexecuteserverbase.h>
#include "CMtfTestCase.h"

class CMtfTestAction;
class CMtfTestActionNamePair;
class CMtfTestCaseNamePair;
class CMtfTestActionParameters;
class CMtfEnumeratorConverter;

/** Messaging test server class. Handles Messaging test scripts. */
class CMtfTestServer : public CTestServer
{
public:
	virtual ~CMtfTestServer();
	
	static CMtfTestServer* NewL();
	virtual CTestStep* CreateTestStep(const TDesC& aStepName);
	void SetCurrentTestCase(CMtfTestCase* aTestCase);
	CMtfTestCase& CurrentTestCase() const;
	void AddTestActionL(CMtfTestActionNamePair* aPair);
	void AddTestCaseL(CMtfTestCaseNamePair* aPair);
	const CMtfTestAction& CreateTestActionL(const TDesC& aActionName,
			CMtfTestActionParameters* aActionParameters) const;
	const CMtfTestAction& CreateTestActionL(const TDesC& aActionName,
			CMtfTestActionParameters* aActionParameters, const TDesC& aActionId) const;
	const TDesC& GetDefaultConfigurationFileL(const CMtfConfigurationType::TMtfConfigurationType& aType) const;
	const TDesC& GetMainScriptConfigurationFileL(const CMtfConfigurationType::TMtfConfigurationType& aType, TInt aIndex) const;
	TInt ConvertEnumeratorL(const TDesC& aParameter) const;
	void SetMainScriptConfigurationTypeL(CMtfConfigurationType* aConfiguration);
	
private:
	CMtfTestServer();
	void ConstructL();
	CMtfTestAction* CreateTestActionPtrL(const TDesC& aActionName,
			CMtfTestActionParameters* aActionParameters) const;
	CTestStep* CreateTestStepL(const TDesC& aStepName);
	CMtfTestCase* CreateTestCaseL(const TDesC& aCaseName) const;
	void SetDefaultConfigurationTypeL(CMtfConfigurationType* aConfigurationType);
	void AddEnumeratorConverterL(CMtfEnumeratorConverter* aEnumeratorConverter);
	TBool EnumeratorConverterExists(const CMtfEnumeratorConverter& aConverter) const;
	const CMtfEnumeratorConverter& FindEnumeratorConverterL(const TDesC& aParameter) const;
	
private:
	CMtfTestCase* 							iCurrentTestCase;
	TBool 									iInsideScriptedTestCase;
	CMtfTestCase::TMtfTestCaseState 		iScriptState;
	RPointerArray<CMtfTestActionNamePair>	iAvailableTestActions;
	RPointerArray<CMtfTestCaseNamePair>		iAvailableTestCases;
	RPointerArray<CMtfConfigurationType>	iDefaultConfigurations;
	RPointerArray<CMtfConfigurationType> 	iMainScriptConfigurations;
	RPointerArray<CMtfEnumeratorConverter>	iEnumeratorConverters;
};

#endif
