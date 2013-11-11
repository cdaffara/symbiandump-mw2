/*
* Copyright (c) 2005-2006 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  "testclass" Test module for STIF FW
*
*/


#ifndef C_COMASUPLPOSTESTERMODULECFG_H
#define C_COMASUPLPOSTESTERMODULECFG_H

#include <stiflogger.h>
#include <testscripterinternal.h>
#include <stiftestmodule.h>
#include "epos_comasuplpostestercategory.h"

/** Logging path */
_LIT( KPosTesterModuleCfgLogPath, "\\logs\\testframework\\PosTesterModuleCfg\\" ); 
/** Log file */
_LIT( KPosTesterModuleCfgLogFile, "PosTesterModuleCfg.txt" ); 

class COMASuplPosTesterModuleCfg;
class COMASuplEComHandler;
class COMASuplConsoleManager;
class CImplementationInformation;
class COMASuplTestHandler;
class COMASuplPosTesterLogger;
class COMASuplPosHandlerBase;
class COMASuplPosTesterCategory;
class COMASuplThreadProcessTest;
/**
 *  COMASuplPosTesterModuleCfg test class for STIF Test Framework TestScripter.
 *  Execites tests, sequence is controlled by cfg file.
 *
 *  @lib epos_comasuplpostestermodulecfg.lib
 *  @since S60 v3.1u
 */
NONSHARABLE_CLASS(COMASuplPosTesterModuleCfg) : public CScriptBase
    {
    public:  // Constructors and destructor

        /**
         * Two-phased constructor.
         */
        static COMASuplPosTesterModuleCfg* NewL( CTestModuleIf& aTestModuleIf );

        /**
         * Destructor.
         */
        virtual ~COMASuplPosTesterModuleCfg();

    public: // Functions from base classes
		/**
	     * From CScriptBase
	     * Runs a script line.
	     *
	     * @since S60 v3.1u
	     * @param aItem Script line containing method name and parameters
	     */
        virtual TInt RunMethodL( CStifItemParser& aItem );

    private:

        /**
         * C++ default constructor.
         */
        COMASuplPosTesterModuleCfg( CTestModuleIf& aTestModuleIf );

        /**
         * By default Symbian 2nd phase constructor is private.
         */
        void ConstructL();

        /**
         * Frees all resources allocated from test methods.
         * @since S60 v3.1
         * @return void
         */
        void Delete();

        /**
         * Test methods are listed below. 
         */

        /**
         * Test method.
         * @since S60 v3.1
         * @param aItem Script line containing parameters.
         * @return Symbian OS error code.
         */
        virtual TInt SuplGatewayTestL( CStifItemParser& aItem );

        /**
         * Test method.
         * @since S60 v3.1
         * @param aItem Script line containing parameters.
         * @return Symbian OS error code.
         */
        virtual TInt GetUserSelectionL( CStifItemParser& aItem );
        
        /**
         * Test method.
         * @since S60 v3.1
         * @param aItem Script line containing parameters.
         * @return Symbian OS error code.
         */
		virtual TInt Uid_ECom_TestL( CStifItemParser& aItem );
		
		/**
         * Test method.
         * @since S60 v3.1
         * @param aItem Script line containing parameters.
         * @return Symbian OS error code.
         */
		virtual TInt PosHandlerCreationL( CStifItemParser& aItem );
		
	 	/**
         * Test method.
         * @since S60 v3.1
         * @param aItem Script line containing parameters.
         * @return Symbian OS error code.
         */
		virtual TInt PosSessionCreationL( CStifItemParser& aItem );
		
		/**
         * Test method.
         * @since S60 v3.1
         * @param aItem Script line containing parameters.
         * @return Symbian OS error code.
         */
		virtual TInt GetSuplInfoTestL(CStifItemParser& aItem);
		
		/**
         * Test method.
         * @since S60 v3.1
         * @param aItem Script line containing parameters.
         * @return Symbian OS error code.
         */
		virtual TInt GetSuplInfoCancelTestL( CStifItemParser& aItem);

		/**
         * Test method.
         * @since S60 v3.1
         * @param aItem Script line containing parameters.
         * @return Symbian OS error code.
         */
		virtual TInt MessageReaderL(CStifItemParser& aItem);

		/**
         * Test method.
         * @since S60 v3.1
         * @param aItem Script line containing parameters.
         * @return Symbian OS error code.
         */
		virtual TInt PositionVelocityTestL(CStifItemParser& aItem);

		/**
         * Test method.
         * @since S60 v3.1
         * @param aItem Script line containing parameters.
         * @return Symbian OS error code.
         */
		virtual TInt PositionCancellationTestL(CStifItemParser& aItem);

		/**
         * Test method.
         * @since S60 v3.1
         * @param aItem Script line containing parameters.
         * @return Symbian OS error code.
         */
		virtual TInt InitializeCancelTestL(CStifItemParser& aItem);

		/**
         * Test method.
         * @since S60 v3.1
         * @param aItem Script line containing parameters.
         * @return Symbian OS error code.
         */
		virtual TInt PositionTestL(CStifItemParser& aItem);

		/**
         * Test method.
         * @since S60 v3.1
         * @param aItem Script line containing parameters.
         * @return Symbian OS error code.
         */
		virtual TInt MultipleSessionTestL(CStifItemParser& /*aItem*/);
		/**
         * Test method.
         * @since S60 v3.1
         * @param aItem Script line containing parameters.
         * @return Symbian OS error code.
         */
		virtual TInt WriteSummaryL (CStifItemParser& aItem);

		/**
         * Test method.
         * @since S60 v3.1
         * @param aItem Script line containing parameters.
         * @return Symbian OS error code.
         */
		virtual TInt SessionTestL(CStifItemParser& aItem);

	    /**
	     * Prepares filename from the displayname of the implementation.
	     *
	     * @since S60 v3.1
	     * @param aOutputFileName Name of output file.
	     * @param aImplInfo Implementation.
	     * @return void
	     */
		void PrepareFileName(TFileName& aOutputFileName, 
							const CImplementationInformation& aImplInfo);
		
		/**
	     * Prepares filename from the displayname of the implementation.
	     *
	     * @since S60 v3.1
	     * @param aChar Character is checked if it can be used in file name.
	     * @return TBool True if character can be used in a file name.
	     */					
		TBool IsValidFileNameChar(const TUint16* aChar);
		
		/**
	     * Adds number of info, warning and error messages to member variables.
	     *
	     * @since S60 v3.1
	     * @param aTestCat Test class whose summary is to be added.
	     * @return void
	     */
		void UpdateSummary(COMASuplPosTesterCategory* aTestCat);
		
    private:
    	/**
	     * It is used to identify all implementations
	     * Own.
	     */
        COMASuplEComHandler* iEComHandler;
    	/**
	     * Controls display of list and selection.
	     * Own.
	     */
		COMASuplConsoleManager* iConsoleManager;
    	/**
	     * provides logging utility.
	     * Own.
	     */
		COMASuplPosTesterLogger* iPosLogger;
		/**
	     * Name of the logfile prepared from Display name.
	     */
		TFileName iLogFileName;
    	/**
	     * Points to the implementation selected by the user.
	     * If there is only one implementation, it is selected.
	     * Own.
	     */
		CImplementationInformation* iSelectedImpl;
    	/**
	     * Controls tests and monitors user inputs/request for cancellation.
	     * Own.
	     */
		COMASuplTestHandler* iTestHandler;
		
		/**
	     * ETrue if testing is canceled by user.
	     */
		TBool iTestingCancelled;
    	/**
	     * POS handler provided to tests .
	     * Own.
	     */
		COMASuplPosHandlerBase* iPosHandler;
		
		/**
	     * Total number of infos during a test cycle.
	     */
		TInt iNetInfos;
		
		/**
	     * Total number of warnings during a test cycle.
	     */
		TInt iNetWarnings;
		
		/**
	     * Total number of errors during a test cycle.
	     */
		TInt iNetErrors;
		
		/**
	     * Status of testing.
	     */
		TTestingStatus iTestingStatus;
		
		/**
	     * Object of test class for Thread and process thread.
	     */
		COMASuplThreadProcessTest* iThreadTester;
    };

#endif
