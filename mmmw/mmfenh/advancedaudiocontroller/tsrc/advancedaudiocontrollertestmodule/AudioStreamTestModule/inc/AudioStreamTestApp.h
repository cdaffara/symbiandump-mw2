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
* Description:  AudioInputStreamTestApp test module.
*
*/



#ifndef AUDIO_STREAM_TESTAPP_H
#define AUDIO_STREAM_TESTAPP_H

// INCLUDES
#include "StifTestModule.h"
#include <StifLogger.h>
#include <StifParser.h>
#include <f32file.h>
#include <s32file.h>

#include <MdaAudioTonePlayer.h>
#include "AudioStreamConstants.h"

// CONSTANTS
//const ?type ?constant_var = ?constant;

    
// MACROS
//#define ?macro ?macro_def

// Logging path
//_LIT( KAudioInputStreamTestAppLogPath, "\\logs\\testframework\\AudioInputStreamTestApp\\" ); 
// Log file
//_LIT( KAudioInputStreamTestAppLogFile, "AudioInputStreamTestApp.txt" ); 

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
*  This a AudioInputStreamTestApp class.
*  ?other_description_lines
*
*  @lib ?library
*  @since ?Series60_version
*/
NONSHARABLE_CLASS(CAudioStreamTestApp) : public CTestModuleBase
    {
    public:  // Constructors and destructor

        /**
        * Two-phased constructor.
        */
        static CAudioStreamTestApp* NewL();

        /**
        * Destructor.
        */
        virtual ~CAudioStreamTestApp();

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
        * From CTestModuleBase InitL is used to initialize the AudioInputStreamTestApp. 
        *       It is called once for every instance of AudioInputStreamTestApp after
        *       its creation.
        * @since ?Series60_version
        * @param aIniFile Initialization file for the test module (optional)
        * @param aFirstTime Flag is true when InitL is executed for first 
        *               created instance of AudioInputStreamTestApp.
        * @return Symbian OS error code
        */
        TInt InitL( TFileName& aIniFile, TBool aFirstTime );

        /**
        * From CTestModuleBase GetTestCasesL is used to inquiry test cases 
        *   from AudioInputStreamTestApp. 
        * @since ?Series60_version
        * @param aTestCaseFile Test case file (optional)
        * @param aTestCases  Array of TestCases returned to test framework
        * @return Symbian OS error code
        */
        TInt GetTestCasesL( const TFileName& aTestCaseFile, 
                            RPointerArray<TTestCaseInfo>& aTestCases );

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
        TInt RunTestCaseL( const TInt aCaseNumber, 
                           const TFileName& aTestCaseFile,
                           TTestResult& aResult );

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

        /**
        * ?member_description.
        * @since ?Series60_version
        * @param ?arg1 ?description
        * @return ?description
        */
        //?type ?member_function( ?type ?arg1 );
        
		HBufC8 *GetDescriptor8LC(const TFileName &aFileName);
		TInt GetIntL(CStifSectionParser *aSection, const TDesC &aTag, TBool &aUsingDefault, const TInt &aDefaultInt, TBool aContinue=EFalse) ;
		TTimeIntervalMicroSeconds32 GetTimeIntervalL(CStifSectionParser *aSection, const TDesC &aTag, TBool &aUsingDefault, const TTimeIntervalMicroSeconds32 &aDefaultInterval, TBool aContinue=EFalse);
        void InitLoggerL( const TFileName& aLogPath , const TFileName& aLogFile );
		void InitLoggerL();
		TInt RunTestCaseFunctionL(TInt id, CStifSectionParser *aSection, TTestResult &aResult);

        TInt TestCaseInputNewL(CStifSectionParser *aSection, TTestResult & aResult);
        TInt TestCaseInputNewWithParamL(CStifSectionParser *aSection, TTestResult & aResult);
        TInt TestCaseInputSetPropL(CStifSectionParser *aSection, TTestResult & aResult);
        TInt TestCaseInputSetGainL(CStifSectionParser *aSection, TTestResult & aResult);
        TInt TestCaseInputSetBalanceL(CStifSectionParser *aSection, TTestResult & aResult);
        TInt TestCaseInputSetPriorityL(CStifSectionParser *aSection, TTestResult & aResult);
        TInt TestCaseInputSetDataTypeL(CStifSectionParser *aSection, TTestResult & aResult);
        TInt TestCaseInputOpenL(CStifSectionParser *aSection, TTestResult & aResult);
        TInt TestCaseInputRecordL(CStifSectionParser *aSection, TTestResult & aResult);
        TInt TestCaseInputBitRateL(CStifSectionParser *aSection, TTestResult & aResult);

        TInt TestCaseOutputNewL(CStifSectionParser *aSection, TTestResult & aResult);
        TInt TestCaseOutputNewWithParamL(CStifSectionParser *aSection, TTestResult & aResult);
        TInt TestCaseOutputSetPropL(CStifSectionParser *aSection, TTestResult & aResult);
        TInt TestCaseOutputOpenL(CStifSectionParser *aSection, TTestResult & aResult);
        TInt TestCaseOutputSetVolumeL(CStifSectionParser *aSection, TTestResult & aResult);
        TInt TestCaseOutputSetPriorityL(CStifSectionParser *aSection, TTestResult & aResult);
        TInt TestCaseOutputPlayL(CStifSectionParser *aSection, TTestResult & aResult);
        TInt TestCaseOutputSetBalanceL(CStifSectionParser *aSection, TTestResult & aResult);
        TInt TestCaseOutputSetDataTypeL(CStifSectionParser *aSection, TTestResult & aResult);
        TInt TestCaseOutputStopL(CStifSectionParser *aSection, TTestResult & aResult);

		//void testPlay (COutputStreamer *os1, CStifSectionParser *section, TTestResult & aResult);

    protected:  // Functions from base classes

        /**
        * From ?base_class ?member_description
        */
        //?type ?member_function();

    private:

        /**
        * C++ default constructor.
        */
        CAudioStreamTestApp();

        /**
        * By default Symbian 2nd phase constructor is private.
        */
        void ConstructL();

        // Prohibit copy constructor if not deriving from CBase.
        // ?classname( const ?classname& );
        // Prohibit assigment operator if not deriving from CBase.
        // ?classname& operator=( const ?classname& );
		TMdaPriorityPreference IntToPriorityPreference (TInt aPriorityPreference) ;
		TDesC& GetErrorDes(TInt aErr);

    public:     // Data
        // ?one_line_short_description_of_data
        //?data_declaration;
    	RFs				iFs;
		RFile			iFile;

    protected:  // Data
        // ?one_line_short_description_of_data
        //?data_declaration;

    private:    // Data
        // ?one_line_short_description_of_data
        //?data_declaration;
        static const TInt MAX_TESTCASE_ID;

		static const TInt KToneMinCaseId;
		static const TInt KToneMaxCaseId;

		static const TInt KTestCaseCallbacks;
		static const TInt KTestCaseVolume;
		static const TInt KTestCaseBalance;
		static const TInt KTestCaseOutputPriorities;
		static const TInt KTestCaseInputPriorities;

        static const TInt KTestCaseInputNew;
        static const TInt KTestCaseInputNewWithParam;
        static const TInt KTestCaseInputSetAudioProperties;
        static const TInt KTestCaseInputSetGain;
        static const TInt KTestCaseInputSetBalance;
        static const TInt KTestCaseInputSetPriority;
        static const TInt KTestCaseInputSetDataType;
        static const TInt KTestCaseInputOpen;
        static const TInt KTestCaseInputRecord;
        static const TInt KTestCaseInputBitRate;
        
        static const TInt KTestCaseOutputNew;
        static const TInt KTestCaseOutputNewWithParam;
        static const TInt KTestCaseOutputSetAudioProperties;
        static const TInt KTestCaseOutputOpen;
        static const TInt KTestCaseOutputSetVolume;
        static const TInt KTestCaseOutputSetPriority;
        static const TInt KTestCaseOutputPlay;
        static const TInt KTestCaseOutputSetBalance;
        static const TInt KTestCaseOutputSetDataType;
        static const TInt KTestCaseOutputStop;

        CStifLogger * iLogger;
		CActiveScheduler* iScheduler;

		TBuf<50>	iErrName[47];
		TBuf<50>    iErrBuf;
		
        // Reserved pointer for future extension
        //TAny* iReserved;

    public:     // Friend classes
        //?friend_class_declaration;
    protected:  // Friend classes
        //?friend_class_declaration;
    private:    // Friend classes
        //?friend_class_declaration;

    };

#endif      // AUDIO_STREAM_TESTAPP_H

// End of File
