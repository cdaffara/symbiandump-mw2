/*
* Copyright (c) 2002 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  MmfTsTone test module.
*
*/



#ifndef MMFTSTONE_H
#define MMFTSTONE_H


// INCLUDES
#if defined (_MSC_VER) && (_MSC_VER >= 1000)
#pragma once
#endif

// INCLUDES
#include "StifTestModule.h"
#include <StifLogger.h>

#include "ActiveScheduledTestModuleBase.h"
#include "TonePlayer.h"
#include "MediaEvent.h"
#include "AudioToneTestModuleConstants.h"
// CONSTANTS
//const ?type ?constant_var = ?constant;

// MACROS
//#define ?macro ?macro_def

// Logging path
_LIT( KMmfTsToneLogPath, "\\logs\\testframework\\MmfTsTone\\" ); 
// Log file
_LIT( KMmfTsToneLogFile, "MmfTsTone.txt" ); 

// DATA TYPES
//enum ?declaration
//typedef ?declaration
//extern ?data_type;

// FUNCTION PROTOTYPES
//?type ?function_name(?arg_list);

// FORWARD DECLARATIONS
//class ?FORWARD_CLASSNAME;

// CLASS DECLARATION

/**
*  This a MmfTsTone class.
*  ?other_description_lines
*
*  @lib ?library
*  @since ?Series60_version
*/
NONSHARABLE_CLASS(CMmfTsTone) : public CActiveScheduledTestModuleBase
    {
    public:  // Constructors and destructor

        /**
        * Two-phased constructor.
        */
        static CMmfTsTone* NewL();

        /**
        * Destructor.
        */
        virtual ~CMmfTsTone();

    public: // New functions

        /**
        * ?member_description.
        * @since ?Series60_version
        * @param ?arg1 ?description
        * @return ?description
        */
        //?type ?member_function( ?type ?arg1 );

    public: // Functions from base classes

        /**
        * From CTestModuleBase InitL is used to initialize the MmfTsTone. 
        *       It is called once for every instance of MmfTsTone after
        *       its creation.
        * @since ?Series60_version
        * @param aIniFile Initialization file for the test module (optional)
        * @param aFirstTime Flag is true when InitL is executed for first 
        *               created instance of MmfTsTone.
        * @return Symbian OS error code
        */
       // TInt InitL( TFileName& aIniFile, TBool aFirstTime );

        /**
        * From CTestModuleBase GetTestCasesL is used to inquiry test cases 
        *   from MmfTsTone. 
        * @since ?Series60_version
        * @param aTestCaseFile Test case file (optional)
        * @param aTestCases  Array of TestCases returned to test framework
        * @return Symbian OS error code
        */
        //TInt GetTestCasesL( const TFileName& aTestCaseFile, 
                            //RPointerArray<TTestCaseInfo>& aTestCases );

        /**
        * From CTestModuleBase RunTestCaseL is used to run an individual 
        *   test case. 
        * @since ?Series60_version
        * @param aCaseNumber Test case number
        * @param aTestCaseFile Test case file (optional)
        * @param aResult Test case result returned to test framework (PASS/FAIL)
        * @return Symbian OS error code (test case execution error, which is 
        *           not reported in aResult parameter as test case failure).
        */   
      //  TInt RunTestCaseL( const TInt aCaseNumber, 
                         //  const TFileName& aTestCaseFile,
                          // TTestResult& aResult );

        /**
        * From CTestModuleBase; OOMTestQueryL is used to specify is particular
        * test case going to be executed using OOM conditions
        * @param aTestCaseFile Test case file (optional)
        * @param aCaseNumber Test case number (optional)
        * @param aFailureType OOM failure type (optional)
        * @param aFirstMemFailure The first heap memory allocation failure value (optional)
        * @param aLastMemFailure The last heap memory allocation failure value (optional)
        * @return TBool
        */
        virtual TBool OOMTestQueryL( const TFileName& /* aTestCaseFile */, 
                                     const TInt /* aCaseNumber */, 
                                     TOOMFailureType& /* aFailureType */,
                                     TInt& /* aFirstMemFailure */, 
                                     TInt& /* aLastMemFailure */ );

        /**
        * From CTestModuleBase; User may add implementation for OOM test 
        * environment initialization. Usually no implementation is required.
        * @param aTestCaseFile Test case file (optional)
        * @param aCaseNumber Test case number (optional)
        * @return None
        */
        virtual void OOMTestInitializeL( const TFileName& /* aTestCaseFile */, 
                                    const TInt /* aCaseNumber */ ); 

        /**
        * From CTestModuleBase; OOMHandleWarningL. User may add implementation 
        * for OOM test warning handling. Usually no implementation is required.
        * @param aTestCaseFile Test case file (optional)
        * @param aCaseNumber Test case number (optional)
        * @param aFailNextValue FailNextValue for OOM test execution (optional)
        * @return None
        */
        virtual void OOMHandleWarningL( const TFileName& /* aTestCaseFile */,
                                        const TInt /* aCaseNumber */, 
                                        TInt& /* aFailNextValue */);

        /**
        * From CTestModuleBase; OOMTestFinalizeL may be used to finalize OOM
        * test environment. Usually no implementation is required.
        * @param aTestCaseFile Test case file (optional)
        * @param aCaseNumber Test case number (optional)
        * @return None
        */
        virtual void OOMTestFinalizeL( const TFileName& /* aTestCaseFile */, 
                                       const TInt /* aCaseNumber */ );

    protected:  // New functions

        TInt RunTestCaseFunctionL(TInt id, CStifSectionParser *section, TTestResult &aResult) ;

    protected:  // Functions from base classes

        /**
        * From ?base_class ?member_description
        */
        //?type ?member_function();

    private:

        /**
        * C++ default constructor.
        */
        CMmfTsTone();

        /**
        * By default Symbian 2nd phase constructor is private.
        */
        void ConstructL();

        TInt TestCaseVolumeL(CStifSectionParser *section , TTestResult &aResult) ;
		TInt TestCaseVolumeRampL(CStifSectionParser *section , TTestResult &aResult) ;
		TInt TestCaseRepeatL(CStifSectionParser *section , TTestResult &aResult) ;
		TInt TestCaseFixedSecuencesL(CStifSectionParser *section , TTestResult &aResult) ;
		TInt TestCasePlayFileSequenceL(CStifSectionParser *section , TTestResult &aResult) ;
		TInt TestCaseBalanceL(CStifSectionParser *section , TTestResult &aResult) ;
		TInt TestCasePrioritiesL(CStifSectionParser *section , TTestResult &aResult) ;
		TInt TestCaseDTMFStringL(CStifSectionParser *section , TTestResult &aResult) ;
		TInt TestCaseDualToneL(CStifSectionParser *section , TTestResult &aResult) ;
		TInt TestCaseCancelPrepareL(CStifSectionParser *section , TTestResult &aResult) ;
		TInt TestCasePlayToneFileL(CStifSectionParser *section , TTestResult &aResult) ;
		TInt TestCasePlayToneRFileL(CStifSectionParser *section , TTestResult &aResult) ;
		TInt TestCaseCustomInterfaceL(CStifSectionParser *section , TTestResult &aResult) ;
		TInt TestCaseMaxVolumeL(CStifSectionParser *section , TTestResult &aResult);
		TInt TestCaseMaxVolumePlayL(CStifSectionParser *section , TTestResult &aResult);
		TInt TestCaseCancelPlayL(CStifSectionParser *section , TTestResult &aResult);
		TInt TestCaseVolumeChangeL(CStifSectionParser *section , TTestResult &aResult); 
		TInt TestCasePlayCorruptFileL(CStifSectionParser *section , TTestResult &aResult);
		TInt TestCaseCheckStateL(CStifSectionParser *section , TTestResult &aResult);
	public: // Data

	protected: // Data

	private: // Data
		static const TInt KToneMinCaseId;
		static const TInt KToneMaxCaseId;

		static const TInt KTestCaseVolume;
		static const TInt KTestCaseVolumeRamp;
		static const TInt KTestCaseRepeat;
		static const TInt KTestCaseFixedSequences;
		static const TInt KTestCasePlayFileSequence;
		static const TInt KTestCaseBalance;
		static const TInt KTestCasePriorities;
		static const TInt KTestCaseDTMFString;
		static const TInt KTestCaseCancelPrepare;
		static const TInt KTestCaseDualTone;
		static const TInt KTestCasePlayToneFile;
		static const TInt KTestCasePlayToneRFile; //JZ
		static const TInt KTestCaseCustomInterface; //JZ
		static const TInt KTestCaseMaxVolume;
		static const TInt KTestCaseMaxVolumePlay;
		static const TInt KTestCasePlayCancel;
		static const TInt KTestCasePlayChangeVolume;
		static const TInt KTestCaseInvalidFrequencyCheck;
		static const TInt KTestCaseCorruptFile;
		static const TInt KTestCaseCheckState;
		
		enum TVolume
		    {
		    EMaxVolume=11,
		    EAvgVolume,
		    EZeroVolume
		    };
    public:     // Data
        // ?one_line_short_description_of_data
        //?data_declaration;

    protected:  // Data
        // ?one_line_short_description_of_data
        //?data_declaration;

    private:    // Data
        // ?one_line_short_description_of_data
        //?data_declaration;

       // CStifLogger * iLog;

        // Reserved pointer for future extension
        //TAny* iReserved;

    public:     // Friend classes
        //?friend_class_declaration;
    protected:  // Friend classes
        //?friend_class_declaration;
    private:    // Friend classes
        //?friend_class_declaration;

    };

#endif      // MMFTSTONE_H

// End of File
