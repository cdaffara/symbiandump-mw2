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
* Description:  Active Scheduler Test Module
*
*/


#ifndef ACTIVE_SCHEDULED_TEST_MODULE_BASE_H
#define ACTIVE_SCHEDULED_TEST_MODULE_BASE_H

// INCLUDES
#if defined (_MSC_VER) && (_MSC_VER >= 1000)
#pragma once
#endif

#include <StifLogger.h>
#include <StifParser.h>
#include <StifTestModule.h>
#include "TestModuleConstants.h"
#include "OutputPrinter.h"

// MACROS
// None

// DATA TYPES
// None

// FUNCTION PROTOTYPES
// None

// FORWARD DECLARATIONS
// None

// CLASS DECLARATION


class CActiveScheduledTestModuleBase : public CTestModuleBase
	{
	public: // Enumerations

	private: // Enumerations

	protected: // Constructors and destructor

		/**
		* Two-phased constructor.
		*/
		//static CActiveScheduledTestModuleBase* NewL();
		CActiveScheduledTestModuleBase(TInt aMinTestCaseId, TInt aMaxTestCaseId, const TDesC &aTitleFormat);
		CActiveScheduledTestModuleBase(TInt aMinTestCaseId, TInt aMaxTestCaseId, const TDesC &aTitleFormat, const TDesC &aConsoleTraceDescription, const TDesC &aTracePrefix, TInt aPrintPriority=0);

		/**
		* Destructor.
		*/
		~CActiveScheduledTestModuleBase();

	public: // New functions

	public: // Functions from base classes

		/**
		* InitL is used to initialize the Test Module. The Test Modules
		* can use the configure file to read parameters for Test Module
		* initialization but they can also have their own configure file
		* or some other routine to initialize themselves.
		*/
		TInt InitL( TFileName& aIniFile, TBool aFirstTime );

		/**
		* Test cases are inquired from the Test Module by calling GetTestCasesL.
		* Test cases are read from the config file specified by aConfigFile.
		* If the Test Module does not use config files for test case definition
		* it does not use aConfigFile parameter. Test cases are appended
		* to CArrayPtrFlat<RTestEngine::TTestCaseInfo> that is a list
		* consisting of several TTestCaseInfo objects.
		* TTestCaseInfo class defines individual test cases and, if needed,
		* a test set where the test case belongs to. TTestCaseInfo is
		* defined in Table 9.
		* The Test Framework will be the owner of the data in the
		* RPointerArray and it does the memory deallocation.
		*/
		TInt GetTestCasesL( const TFileName& aConfigFile , RPointerArray<TTestCaseInfo>& aTestCases );

		/**
		* RunTestCaseL is used to run an individual test case specified
		* by aTestCase. Test cases that can be run may be requested from
		* Test Module by GetTestCasesL method before calling RunTestCaseL.
		* This method is synchronous and test case result is returned as
		* reference parameter aResult value. The function shall return
		* KErrNone if case is successfully started.
		* If test case is not available (i.e invalid configuration file or
		* or case number), then function shall return the error code (i.e
		* (KErrNotFound)
		*/
		virtual TInt RunTestCaseL( const TInt aCaseNumber , const TFileName& aConfig , TTestResult& aResult );

	protected: // New functions
		virtual TInt RunTestCaseFunctionL(TInt id, CStifSectionParser *section, TTestResult &aResult) = 0;

	protected: // Functions from base classes
		HBufC8 *GetDescriptor8LC(const TFileName &aFileName);
		TTimeIntervalMicroSeconds32 GetTimeIntervalL(CStifSectionParser *aSection, const TDesC &aTag, TBool &aUsingDefault, const TTimeIntervalMicroSeconds32 &aDefaultInterval, TBool aContinue=EFalse);
		TTimeIntervalMicroSeconds GetTimeIntervalL(CStifSectionParser *aSection, const TDesC &aTag, TBool &aUsingDefault, const TTimeIntervalMicroSeconds &aDefaultInterval=TTimeIntervalMicroSeconds(-1) , TBool aContinue=EFalse);
		TInt GetIntL(CStifSectionParser *aSection, const TDesC &aTag, TBool &aUsingDefault, const TInt &aDefaultInt, TBool aContinue=EFalse) ;
		TBool IsDurationInRange(TInt aRepeatTimes, TTimeIntervalMicroSeconds aToneDuration, TTimeIntervalMicroSeconds aRepeatSilence, TTimeIntervalMicroSeconds aErrorRange , TTime aStartTime , TTime aEndTime ) ;
		void ConstructL();

	private:
		void InitLoggerL( const TFileName& aLogPath , const TFileName& aLogFile );
		void InitLoggerL();

		//TTimeIntervalMicroSeconds GetDelayL(CStifSectionParser *aSection, const TDesC &aTag=KTagDelay);
	public: // Data
		// Constants

	private:
		CStifLogger *iLogger;
	protected: // Data
		const TDesC &iTitleFormat;
		COutputPrinter iOutputPrinter;
	private: // Data
		CActiveScheduler* iScheduler;
		TInt iMaxTestCaseId;
		TInt iMinTestCaseId;

	public: // Friend classes
	protected: // Friend classes
	private: // Friend classes
	};

#endif      // ACTIVE_SCHEDULED_TEST_MODULE_BASE_H
