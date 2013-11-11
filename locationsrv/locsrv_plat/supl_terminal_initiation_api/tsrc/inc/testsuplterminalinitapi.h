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
* Description:  Class declaration of Ctestsuplterminalinitapi
*
*/



#ifndef TESTSUPLTERMINALINITAPI_H
#define TESTSUPLTERMINALINITAPI_H

//  INCLUDES
#include <StifLogger.h>
#include <TestScripterInternal.h>
#include <StifTestModule.h>



// Logging path
_LIT( KtestsuplterminalinitapiLogPath, "\\logs\\testframework\\testsuplterminalinitapi\\" ); 
// Log file
_LIT( KtestsuplterminalinitapiLogFile, "testsuplterminalinitapi.txt" ); 


// FORWARD DECLARATIONS
//class ?FORWARD_CLASSNAME;
class CTestSuplTerminalInitApi;


// CLASS DECLARATION

/**
*  Ctestsuplterminalinitapi test class for STIF Test Framework TestScripter.
*  ?other_description_lines
*
*  @lib ?library
*  @since ?Series60_version
*/
NONSHARABLE_CLASS(CTestSuplTerminalInitApi) : public CScriptBase
    {
    public:  // Constructors and destructor

        /**
        * Two-phased constructor.
        */
        static CTestSuplTerminalInitApi* NewL( CTestModuleIf& aTestModuleIf );

        /**
        * Destructor.
        */
        virtual ~CTestSuplTerminalInitApi();

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
        * From CScriptBase Runs a script line.
        * @since ?Series60_version
        * @param aItem Script line containing method name and parameters
        * @return Symbian OS error code
        */
        virtual TInt RunMethodL( CStifItemParser& aItem );

    protected:  // New functions

        /**
        * ?member_description.
        * @since ?Series60_version
        * @param ?arg1 ?description
        * @return ?description
        */
        //?type ?member_function( ?type ?arg1 );

    protected:  // Functions from base classes

        /**
        * From ?base_class ?member_description
        */
        //?type ?member_function();

    private:

        /**
        * C++ default constructor.
        */
        CTestSuplTerminalInitApi( CTestModuleIf& aTestModuleIf );

        /**
        * By default Symbian 2nd phase constructor is private.
        */
        void ConstructL();

        // Prohibit copy constructor if not deriving from CBase.
        // ?classname( const ?classname& );
        // Prohibit assigment operator if not deriving from CBase.
        // ?classname& operator=( const ?classname& );

        /**
        * Frees all resources allocated from test methods.
        * @since ?Series60_version
        */
        void Delete();

        /**
        * Test methods are listed below. 
        */

                
        /**
        * SetL test method.
        * @since ?Series60_version
        * @param aItem Script line containing parameters.
        * @return Symbian OS error code.
        */
        virtual TInt SetL( CStifItemParser& aItem );
        
        /**
        * SetHorAccuracyL test method.
        * @since ?Series60_version
        * @param aItem Script line containing parameters.
        * @return Symbian OS error code.
        */
        virtual TInt SetHorAccuracyL( CStifItemParser& aItem );
        
        /**
        * SetVerAccuracyL test method.
        * @since ?Series60_version
        * @param aItem Script line containing parameters.
        * @return Symbian OS error code.
        */
        virtual TInt SetVerAccuracyL( CStifItemParser& aItem );
        
        /**
        * SetMaxLocationAgeL test method.
        * @since ?Series60_version
        * @param aItem Script line containing parameters.
        * @return Symbian OS error code.
        */
        virtual TInt SetMaxLocationAgeL( CStifItemParser& aItem );
        
        /**
        * SetDelayL test method.
        * @since ?Series60_version
        * @param aItem Script line containing parameters.
        * @return Symbian OS error code.
        */
        virtual TInt SetDelayL( CStifItemParser& aItem );
        
        /**
        * GetL test method.
        * @since ?Series60_version
        * @param aItem Script line containing parameters.
        * @return Symbian OS error code.
        */
        virtual TInt GetL( CStifItemParser& aItem );
        
        /**
        * GetHorAccuracyL test method.
        * @since ?Series60_version
        * @param aItem Script line containing parameters.
        * @return Symbian OS error code.
        */
        virtual TInt GetHorAccuracyL( CStifItemParser& aItem );
        
        /**
        * GetVerAccuracyL test method.
        * @since ?Series60_version
        * @param aItem Script line containing parameters.
        * @return Symbian OS error code.
        */
        virtual TInt GetVerAccuracyL( CStifItemParser& aItem );
        
        /**
        * GetMaxLocationAgeL test method.
        * @since ?Series60_version
        * @param aItem Script line containing parameters.
        * @return Symbian OS error code.
        */
        virtual TInt GetMaxLocationAgeL( CStifItemParser& aItem );
        
        /**
        * GetDelayL test method.
        * @since ?Series60_version
        * @param aItem Script line containing parameters.
        * @return Symbian OS error code.
        */
        virtual TInt GetDelayL( CStifItemParser& aItem );
        
        /**
        * ConnectCloseL test method.
        * @since ?Series60_version
        * @param aItem Script line containing parameters.
        * @return Symbian OS error code.
        */
        virtual TInt ConnectCloseL( CStifItemParser& aItem );
        
        /**
        * VersionL test method.
        * @since ?Series60_version
        * @param aItem Script line containing parameters.
        * @return Symbian OS error code.
        */
        virtual TInt VersionL( CStifItemParser& aItem );

		/**
        * OpenCloseL test method.
        * @since ?Series60_version
        * @param aItem Script line containing parameters.
        * @return Symbian OS error code.
        */
        virtual TInt OpenCloseL( CStifItemParser& aItem );
        
		
		/**
        * GetPositionL test method.
        * @since ?Series60_version
        * @param aItem Script line containing parameters.
        * @return Symbian OS error code.
        */
        virtual TInt GetPositionL( CStifItemParser& aItem );

		       
    };

#endif      // TESTSUPLTERMINALINITAPI_H

// End of File
