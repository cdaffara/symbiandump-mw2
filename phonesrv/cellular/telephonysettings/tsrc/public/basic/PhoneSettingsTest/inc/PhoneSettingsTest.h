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
* Description: Declaration of CPhoneSettingsTest class.
*
*/



#ifndef PHONESETTINGSTEST_H
#define PHONESETTINGSTEST_H

// INCLUDES
#include <e32base.h>
#include <f32file.h>
#include <StifTestModule.h>
#include <StifLogger.h>
#include "PhoneSettingsTestVariation.h"
#include "PhoneSettingsTestParser.h" 
#include "MPhoneSettingsTestMessageHandler.h" 

// MACROS
// Logging path
_LIT( KPhoneSettingsTestLogPath, "\\logs\\testframework\\PhoneSettingsTest\\" ); 
// Log file
_LIT( KPhoneSettingsTestLogFile, "PhoneSettingsTest.txt" ); 

// Function pointer related internal definitions
// Rounding known bug in GCC
#ifndef __GCC32__
#define GETPTR
#else
#define GETPTR &
#endif  
#define ENTRY(str,func) {_S(str), GETPTR func,0,0,0}
#define FUNCENTRY(func) {_S(#func), GETPTR func,0,0,0}
#define OOM_ENTRY(str,func,a,b,c) {_S(str), GETPTR func,a,b,c}
#define OOM_FUNCENTRY(func,a,b,c) {_S(#func), GETPTR func,a,b,c}

// FORWARD DECLARATIONS
class CPhoneSettingsTest;
class CPhoneSettingsTestParser; 
class CPhoneSettingsTestCallBarring; 
class CPhoneSettingsTestCallDiverting; 
class CPhoneSettingsTestCallWaiting; 
class CPhoneSettingsTestCli; 
class CPhoneSettingsTestNetwork; 
class CPhoneSettingsTestCSP; 
class CPhoneSettingsTestSAObserver; 
class CPhoneSettingsTestSettingsContainer; 
class CPhoneSettingsTestRefreshHandler; 

// DATA TYPES
// TInt CPhoneSettingsTest::<NameOfFunction> ( TTestResult& aResult )
typedef TInt (CPhoneSettingsTest::* TestFunction)(TTestResult&);

// CLASS DECLARATION

/**
*  An internal structure containing a test case name and
*  the pointer to function doing the test
*
*  @lib ?library
*  @since ?Series60_version
*/
class TCaseInfoInternal
    {
    public:
        const TText*    iCaseName;
        TestFunction    iMethod;
        TBool           iIsOOMTest;
        TInt            iFirstMemoryAllocation;
        TInt            iLastMemoryAllocation;
    };

// CLASS DECLARATION

/**
*  A structure containing a test case name and
*  the pointer to function doing the test
*
*  @lib ?library
*  @since ?Series60_version
*/
class TCaseInfo
    {
    public:
        TPtrC iCaseName;
        TestFunction iMethod;
        TBool           iIsOOMTest;
        TInt            iFirstMemoryAllocation;
        TInt            iLastMemoryAllocation;

    TCaseInfo( const TText* a ) : iCaseName( (TText*) a )
        {
        };
    };

// CLASS DECLARATION

/**
*  This a PhoneSettingsTest class.
*  ?other_description_lines
*
*  @lib ?library
*  @since ?Series60_version
*/
NONSHARABLE_CLASS(CPhoneSettingsTest) : 
	public CTestModuleBase,
	public MPhoneSettingsTestMessageHandler 	
    {	
    public:  // Constructors and destructor

        /**
        * Two-phased constructor.
        */
        static CPhoneSettingsTest* NewL();

        /**
        * Destructor.
        */
        virtual ~CPhoneSettingsTest();

    public: // Functions from base classes

        /**
        * From CTestModuleBase InitL is used to initialize the 
        *       PhoneSettingsTest. It is called once for every instance of 
        *       PhoneSettingsTest after its creation.
        * @since 
        * @param aIniFile Initialization file for the test module (optional)
        * @param aFirstTime Flag is true when InitL is executed for first 
        *               created instance of PhoneSettingsTest.
        * @return Symbian OS error code
        */
        TInt InitL( TFileName& aIniFile, TBool aFirstTime );

        /**
        * From CTestModuleBase GetTestCasesL is used to inquiry test cases 
        *   from PhoneSettingsTest. 
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
                                     TOOMFailureType& aFailureType, 
                                     TInt& /* aFirstMemFailure */, 
                                     TInt& /* aLastMemFailure */ );

        /**
        * From CTestModuleBase; OOMTestInitializeL may be used to initialize OOM
        * test environment
        * @param aTestCaseFile Test case file (optional)
        * @param aCaseNumber Test case number (optional)
        * @return None
        */
        virtual void OOMTestInitializeL( const TFileName& /* aTestCaseFile */, 
                                    const TInt /* aCaseNumber */ ); 

        /**
        * From CTestModuleBase; OOMHandleWarningL
        * @param aTestCaseFile Test case file (optional)
        * @param aCaseNumber Test case number (optional)
        * @param aFailNextValue FailNextValue for OOM test execution (optional)
        * @return None
        *
        * User may add implementation for OOM test warning handling. Usually no
        * implementation is required.           
        */
        virtual void OOMHandleWarningL( const TFileName& /* aTestCaseFile */,
                                        const TInt /* aCaseNumber */, 
                                        TInt& /* aFailNextValue */); 

        /**
        * From CTestModuleBase; OOMTestFinalizeL may be used to finalize OOM
        * test environment
        * @param aTestCaseFile Test case file (optional)
        * @param aCaseNumber Test case number (optional)
        * @return None
        *
        */
        virtual void OOMTestFinalizeL( const TFileName& /* aTestCaseFile */, 
                                       const TInt /* aCaseNumber */ );

	public:	// Functions from MPhoneSettingsTestMessageHandler

	   /** 
        * From MPhoneSettingsTestMessageHandler. 
        * Outputs messages to Stif UI or log
        * @param aMessage Contains the message
        */        
		virtual void PrintMessage( const TDesC& aMessage );

       /** 
        * From MPhoneSettingsTestMessageHandler. 
        * Outputs messages to Stif UI or log
        * @param aMessage Contains the message
        */        
		virtual void PrintMessage( TRefByValue<const TDesC> aMessage,... ); 
									    
    private:

        /**
        * C++ default constructor.
        */
        CPhoneSettingsTest();

        /**
        * By default Symbian 2nd phase constructor is private.
        */
        void ConstructL();

        /**
        * Function returning test case name and pointer to test case function.
        * @since ?Series60_version
        * @param aCaseNumber test case number
        * @return TCaseInfo 
        */
        const TCaseInfo Case ( const TInt aCaseNumber ) const;

    public: // Call barring tests 
    
        /**
        * Test CPsetCallBarring::NewL and CPsetCallBarring::~CPsetCallBarring
        * Function type: Library
        */        
        TInt TestCallBarringCPsetCallBarringConstructAndDestructL( TTestResult& aResult );
    
        /**
        * Test CPsetCallBarring::ChangePasswordL
        * Function type: Library
        */        
        TInt TestCallBarringChangePasswordL( TTestResult& aResult );
		
        /**
        * Test CPsetCallBarring::TestSetBarringL 
        * Function type: Library
        */        
	    TInt TestCallBarringSetBarringL( TTestResult& aResult );
	
        /**
        * Test CPsetCallBarring::GetBarringStatusL
        * Function type: Library
        */        
	    TInt TestCallBarringGetBarringStatusL( TTestResult& aResult );
	    
        /**
        * Test CPsetCallBarring::TestSetRequestObserver
        * Function type: Library
        */        
	    TInt TestCallBarringSetRequestObserver( TTestResult& aResult );

        /**
        * Test CPsetCallBarring::CancelCurrentRequest
        * Function type: Library
        */        
	    TInt TestCallBarringCancelCurrentRequest( TTestResult& aResult );

        /**
        * Test CPsetCallBarring::NewL
        * Function type: Public
        */        
	    TInt TestCallBarringCPsetCallBarringNewL( TTestResult& aResult );
        
        /**
        * Test CPsetCallBarring::PhoneSettingChanged
        * Function type: Public
        */        
	    TInt TestCallBarringPhoneSettingChanged(TTestResult& aResult);
        
    public: // Call diverting tests 	                
    
        /**
        * Test CPsetCallDiverting::NewL and CPsetCallDiverting::~CPsetCallDiverting
        * Function type: Library 
        */        
        TInt TestCallDivertingCPsetCallDivertingConstructAndDestruct(TTestResult& aResult);

        /**
        * Test CPsetCallDiverting::GetUsedDataNumberLC        
        * Function type: Library 
        */        
        TInt TestCallDivertingGetUsedDataNumberLC(TTestResult& aResult);        

        /**
        * Test CPsetCallDiverting::GetUsedFaxNumberLC
        * Function type: Library 
        */        
        TInt TestCallDivertingGetUsedFaxNumberLC(TTestResult& aResult);        
	 
        /**
        * Test CPsetCallDiverting::SetUsedDataNumberL
        * Function type: Library 
        */   	 
	    TInt TestCallDivertingSetUsedDataNumberL(TTestResult& aResult);        
	    
        /**
        * Test CPsetCallDiverting::SetUsedFaxNumberL
        * Function type: Library 
        */   	 
	    TInt TestCallDivertingSetUsedFaxNumberL(TTestResult& aResult);        

        /**
        * Test CPsetCallDiverting::SetTimerValueL
        * Function type: Library 
        */   	 
	    TInt TestCallDivertingSetTimerValueL(TTestResult& aResult);        

        /**
        * Test CPsetCallDiverting::GetTimerValueL
        * Function type: Library 
        */   	 
	    TInt TestCallDivertingGetTimerValueL(TTestResult& aResult);        

        /**
        * Test CPsetCallDiverting::SetNewDefaultNumberL
        * Function type: Library 
        */   	 
	    TInt TestCallDivertingSetNewDefaultNumberL(TTestResult& aResult);        

        /**
        * Test CPsetCallDiverting::GetDefaultNumbersL
        * Function type: Library 
        */   	 
	    TInt TestCallDivertingGetDefaultNumbersL(TTestResult& aResult);        

        /**
        * Test CPsetCallDiverting::CancelCurrentRequest
        * Function type: Library 
        */   	 
	    TInt TestCallDivertingCancelCurrentRequest(TTestResult& aResult);        

        /**
        * Test CPsetCallDiverting::SetDivertingL
        * Function type: Library 
        */   	 
	    TInt TestCallDivertingSetDivertingL(TTestResult& aResult);        
	    
        /**
        * Test CPsetCallDiverting::GetDivertingStatusL
        * Function type: Library 
        */   	 
	    TInt TestCallDivertingGetDivertingStatusL(TTestResult& aResult);        

        /**
        * Test CPsetCallDiverting::SetRequestObserver
        * Function type: Library
        */        
	    TInt TestCallDivertingSetRequestObserver(TTestResult& aResult);
	    
        /**
        * Test CPsetCallDiverting::SwapDefaultNumberL
        * Function type: Library
        */        
	    TInt TestCallDivertingSwapDefaultNumberL(TTestResult& aResult);

        /**
        * Test CPsetCallDiverting::VoiceMailQueryL
        * Function type: Library
        */        
	    TInt TestCallDivertingVoiceMailQueryL(TTestResult& aResult);

        /**
        * Test CPsetCallDiverting::VideoMailQueryL
        * Function type: Library
        */        
	    TInt TestCallDivertingVideoMailQueryL(TTestResult& aResult);

        /**
        * Test TCallDivertSetting::Initialize
        * Function type: Public
        */        
        TInt TestCallDivertingTCallDivertSettingInitialize(TTestResult& aResult);

	public: // Call Waiting tests 	

        /**
        * Test CPsetCallWaiting::NewL and CPsetCallWaiting::~CPsetCallWaiting
        * Function type: Library
        */        
        TInt TestCallWaitingCPsetCallWaitingConstructAndDestructL(TTestResult& aResult);

        /**
        * Test CPsetCallWaiting::SetCallWaitingL
        * Function type: Library
        */        
        TInt TestCallWaitingSetCallWaitingL(TTestResult& aResult);

        /**
        * Test CPsetCallWaiting::GetCallWaitingStatusL
        * Function type: Library
        */        
        TInt TestCallWaitingGetCallWaitingStatusL(TTestResult& aResult);

        /**
        * Test CPsetCallWaiting::CancelProcess
        * Function type: Library
        */        
        TInt TestCallWaitingCancelProcess(TTestResult& aResult);

        /**
        * Test CPsetCallWaiting::SetRequestObserver
        * Function type: Library
        */        
        TInt TestCallWaitingSetRequestObserver(TTestResult& aResult);

        /**
        * Test CPsetCallWaiting::PhoneSettingChanged
        * Function type: Library
        */        
        TInt TestCallWaitingPhoneSettingChanged(TTestResult& aResult);

	public: // CLI tests 	

        /**
        * Test CPsetCli::NewL and CPsetCli::~CPsetCli  
        * Function type: Library
        */        
        TInt TestCliCPsetCliConstructAndDestructL(TTestResult& aResult);

        /**
        * Test CPsetCli::CancelAll
        * Function type: Library
        */        
        TInt TestCliCancelAll(TTestResult& aResult);

        /**
        * Test CPsetCli::GetClipModeL
        * Function type: Library
        */        
        TInt TestCliGetClipModeL(TTestResult& aResult);

        /**
        * Test CPsetCli::GetClirModeL
        * Function type: Library
        */        
        TInt TestCliGetClirModeL(TTestResult& aResult);

        /**
        * Test CPsetCli::GetCnapL
        * Function type: Library
        */        
        TInt TestCliGetCnapL(TTestResult& aResult);

        /**
        * Test CPsetCli::GetColpModeL
        * Function type: Library
        */        
        TInt TestCliGetColpModeL(TTestResult& aResult);

        /**
        * Test CPsetCli::GetColrModeL
        * Function type: Library
        */        
        TInt TestCliGetColrModeL(TTestResult& aResult);

        /**
        * Test CPsetCli::SetRequestObserver
        * Function type: Library
        */        
        TInt TestCliSetRequestObserver(TTestResult& aResult);

    public: // Settings Container test cases 

        /**
        * Test CPsetContainer::NewL and CPsetContainer::~CPsetContainer        
        * Function type: Library
        */        
        TInt TestSettingsContainerCPsetContainerConstructAndDestructL(TTestResult& aResult);

        /**
        * Test CPsetContainer::CreateCBObjectL
        * Function type: Library
        */        
        TInt TestSettingsContainerCreateCBObjectL(TTestResult& aResult);
        
        /**
        * Test CPsetContainer::CreateCFObjectL
        * Function type: Library
        */        
        TInt TestSettingsContainerCreateCFObjectL(TTestResult& aResult);
        
        /**
        * Test CPsetContainer::CreateCWObjectL
        * Function type: Library
        */        
        TInt TestSettingsContainerCreateCWObjectL(TTestResult& aResult);
        
        /**
        * Test CPsetContainer::CreateCliObjectL
        * Function type: Library
        */        
        TInt TestSettingsContainerCreateCliObjectL(TTestResult& aResult);

        /**
        * Test CPsetContainer::CreateNetworkModeObjectL
        * Function type: Library
        */        
        TInt TestSettingsContainerCreateNetworkModeObjectL(TTestResult& aResult);

        /**
        * Test CPsetContainer::CreateNetworkObjectL
        * Function type: Library
        */        
        TInt TestSettingsContainerCreateNetworkObjectL(TTestResult& aResult);

        /**
        * Test CPsetContainer::CreateRefreshHandlerL
        * Function type: Library
        */        
        TInt TestSettingsContainerCreateRefreshHandlerL(TTestResult& aResult);
        
        
    public: // Network test cases         
        
        /**
        * Test CPsetNetwork::NewL and CPsetNetwork::~CPsetNetwork
        * Function type: Library
        */        
        TInt TestNetworkCPsetNetworkConstructAndDestructL(TTestResult& aResult);        

        /**
        * Test CPsetNetwork::GetCurrentNetworkInfo
        * Function type: Library
        */        
        TInt TestNetworkGetCurrentNetworkInfo(TTestResult& aResult);
        
        /**
        * Test CPsetNetwork::GetNetworkSelectMode
        * Function type: Library
        */        
        TInt TestNetworkGetNetworkSelectMode(TTestResult& aResult);
        
        /**
        * Test CPsetNetwork::IsCallActive
        * Function type: Library
        */        
        TInt TestNetworkIsCallActive(TTestResult& aResult);
        
        /**
        * Test CPsetNetwork::IsGPRSConnected
        * Function type: Library
        */        
        TInt TestNetworkIsGPRSConnected(TTestResult& aResult);
        
        /**
        * Test CPsetNetwork::ResetNetworkSearch
        * Function type: Library
        */        
        TInt TestNetworkResetNetworkSearch(TTestResult& aResult);
        
        /**
        * Test CPsetNetwork::CancelProcess
        * Function type: Library
        */        
        TInt TestNetworkCancelProcess(TTestResult& aResult);
        
        /**
        * Test CPsetNetwork::GetAvailableNetworksL
        * Function type: Library
        */        
        TInt TestNetworkGetAvailableNetworksL(TTestResult& aResult);
        
        /**
        * Test CPsetNetwork::GetCurrentNetworkModeSelectionL
        * Function type: Library
        */        
        TInt TestNetworkGetCurrentNetworkModeSelectionL(TTestResult& aResult);
        
        /**
        * Test CPsetNetwork::SelectNetworkL
        * Function type: Library
        */        
        TInt TestNetworkSelectNetworkL(TTestResult& aResult);
        
        /**
        * Test CPsetNetwork::SetNetSAObserver
        * Function type: Library
        */        
        TInt TestNetworkSetNetSAObserver(TTestResult& aResult);

        /**
        * Test CPsetNetwork::SetNetworkModeObserver
        * Function type: Library
        */        
        TInt TestNetworkSetNetworkModeObserver(TTestResult& aResult);

        /**
        * Test CPsetNetwork::SetNetworkModeSelectionL
        * Function type: Library
        */        
        TInt TestNetworkSetNetworkModeSelectionL(TTestResult& aResult);
        
    public: // CSP test cases         

        /**
        * Test CPsetCustomerServiceProfile::NewL and 
        * CPsetCustomerServiceProfile::~CPsetCustomerServiceProfile
        * Function type: Library
        */        
        TInt TestCSPCPsetCustomerServiceProfileConstructAndDestructL(TTestResult& aResult);

        /**
        * Test CPsetCustomerServiceProfile::IsALSSupported
        * Function type: Library
        */        
        TInt TestCSPIsALSSupported(TTestResult& aResult);

        /**
        * Test CPsetCustomerServiceProfile::IsCBSupported
        * Function type: Library
        */        
        TInt TestCSPIsCBSupported(TTestResult& aResult);

        /**
        * Test CPsetCustomerServiceProfile::IsCFSupported
        * Function type: Library
        */        
        TInt TestCSPIsCFSupported(TTestResult& aResult);

        /**
        * Test CPsetCustomerServiceProfile::IsCWSupported
        * Function type: Library
        */        
        TInt TestCSPIsCWSupported(TTestResult& aResult);

        /**
        * Test CPsetCustomerServiceProfile::IsNetworkSelectionSupported
        * Function type: Library
        */        
        TInt TestCSPIsNetworkSelectionSupported(TTestResult& aResult);

        /**
        * Test CPsetCustomerServiceProfile::OpenCSProfileL
        * Function type: Library
        */        
        TInt TestCSPOpenCSProfileL(TTestResult& aResult);

        /**
        * Test CPsetCustomerServiceProfile::CheckIfCWSupported
        * Function type: Library
        */        
        TInt TestCSPCheckIfCWSupported(TTestResult& aResult);

        /**
        * Test CPsetCustomerServiceProfile::CheckIfCBSupported
        * Function type: Library
        */        
        TInt TestCSPCheckIfCBSupported(TTestResult& aResult);

        /**
        * Test CPsetCustomerServiceProfile::CheckIfCFSupported
        * Function type: Library
        */        
        TInt TestCSPCheckIfCFSupported(TTestResult& aResult);

        /**
        * Test CPsetCustomerServiceProfile::CheckIfAlsSupported
        * Function type: Library
        */        
        TInt TestCSPCheckIfAlsSupported(TTestResult& aResult);

        /**
        * Test CPsetCustomerServiceProfile::CheckIfNetworkSelectionSupported
        * Function type: Library
        */        
        TInt TestCSPCheckIfNetworkSelectionSupported(TTestResult& aResult);
        
    public: // SA Observer test cases         

        /**
        * Test CPsetSAObserver::NewL and CPsetSAObserver::~CPsetSAObserver
        * Function type: Library
        */        
        TInt TestSAOCPsetSAObserverConstructAndDestructL(TTestResult& aResult);

        /**
        * Test CPsetSAObserver::GetCurrentDivertStatus
        * Function type: Library
        */        
        TInt TestSAOGetCurrentDivertStatus(TTestResult& aResult);

        /**
        * Test CPsetSAObserver::IsCallActive
        * Function type: Library
        */        
        TInt TestSAOIsCallActive(TTestResult& aResult);

        /**
        * Test CPsetSAObserver::IsGPRSConnected
        * Function type: Library
        */        
        TInt TestSAOIsGPRSConnected(TTestResult& aResult);

        /**
        * Test CPsetSAObserver::NotifyDivertChange
        * Function type: Library
        */        
        TInt TestSAONotifyDivertChange(TTestResult& aResult);

        /**
        * Test CPsetSAObserver::SetNetObserver
        * Function type: Library
        */        
        TInt TestSAOSetNetObserver(TTestResult& aResult);

        /**
        * Test CPsetSAObserver::Initialize
        * Function type: Library
        */        
        TInt TestSAOInitialize(TTestResult& aResult);

    public: // Refresh handler test cases         

        /**
        * Test CPSetRefreshHandler::NewL and CPSetRefreshHandler::~CPSetRefreshHandler
        * Function type: Public/Pure Virtual
        */        
        TInt TestCPSetRefreshHandlerConstructAndDestructL(TTestResult& aResult);

    private:    // Data

        // Pointer to test (function) to be executed
        TestFunction iMethod;

        // Pointer to logger
        CStifLogger* iLogger; 

		// Logging output selector (Stif UI output VS. file logging)
		TMessageOption iMessageOption; 
		
		// Counter for STIF UI line number
		TInt iStifUiLineCounter; 		
		
		// Pointer to active scheduler
		CActiveScheduler* iTestScheduler;
		
		// Pointer to test parser
		CPhoneSettingsTestParser* iTestParser; 		
		
		// Tests for Phone Settings Call Barring 
		CPhoneSettingsTestCallBarring* iPhoneSettingsTestCallBarring;
		
		// Tests for Phone Settings Call Diverting
		CPhoneSettingsTestCallDiverting* iPhoneSettingsTestCallDiverting; 

		// Tests for Phone Settings Call Waiting
		CPhoneSettingsTestCallWaiting* iPhoneSettingsTestCallWaiting; 

		// Tests for Phone Settings Cli 
		CPhoneSettingsTestCli* iPhoneSettingsTestCli; 
	
		// Tests for Phone Settings Network 
	    CPhoneSettingsTestNetwork* iPhoneSettingsTestNetwork; 

        // Tests for Phone Settings CSP
        CPhoneSettingsTestCSP* iPhoneSettingsTestCSP; 
		
		// Tests for Phone Settings SA Observer
		CPhoneSettingsTestSAObserver* iPhoneSettingsTestSAObserver; 
		
		// Tests for Phone Settings Container
		CPhoneSettingsTestSettingsContainer* iPhoneSettingsTestContainer; 
		
		// Tests for Phone Settings Refresh Handler 
        CPhoneSettingsTestRefreshHandler* iPhoneSettingsTestRefreshHandler; 		
    };

#endif // PHONESETTINGSTEST_H

// End of File

