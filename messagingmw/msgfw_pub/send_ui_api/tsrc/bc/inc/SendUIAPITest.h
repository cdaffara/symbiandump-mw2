/*
* Copyright (c) 2002-2009 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:         SendUIAPITest  declaration

*
*/










#ifndef SENDUIAPITEST_H
#define SENDUIAPITEST_H

//  INCLUDES
#include <StifLogger.h>
#include <TestScripterInternal.h>
#include <StifTestModule.h>
#include <TestclassAssert.h>
#include <CMessageData.h>

#include <bc_plat.h>

// CONSTANTS
//const ?type ?constant_var = ?constant;

// MACROS
//#define ?macro ?macro_def
#if  ! defined( __SERIES60_31__ ) && ! defined( __SERIES60_30__ )	
#define TEST_CLASS_VERSION_MAJOR 32
#else if
#define TEST_CLASS_VERSION_MAJOR 30
#endif

#define TEST_CLASS_VERSION_MINOR 9
#define TEST_CLASS_VERSION_BUILD 6

// Logging path
_LIT( KSendUIAPITestLogPath, "\\logs\\testframework\\SendUIAPITest\\" ); 
// Log file
_LIT( KSendUIAPITestLogFile, "SendUIAPITest.txt" ); 
_LIT( KSendUIAPITestLogFileWithTitle, "SendUIAPITest_[%S].txt" );

// FUNCTION PROTOTYPES
//?type ?function_name(?arg_list);

// FORWARD DECLARATIONS
//class ?FORWARD_CLASSNAME;
class CMessageAddress;
class CMessageData;
class CSendUIAPITest;
class CSendingServiceInfo;

// DATA TYPES
//enum ?declaration
//typedef ?declaration
//extern ?data_type;

// CLASS DECLARATION


/**
*  CSendUIAPITest test class for STIF Test Framework TestScripter.
*  ?other_description_lines
*
*  @lib ?library
*  @since ?Series60_version
*/
NONSHARABLE_CLASS(CSendUIAPITest) : public CScriptBase
    {
    public:  // Constructors and destructor

        /**
        * Two-phased constructor.
        */
        static CSendUIAPITest* NewL( CTestModuleIf& aTestModuleIf );

        /**
        * Destructor.
        */
        virtual ~CSendUIAPITest();

    public: // New functions

        /**
        * ?Function to create CMessageData object
        * @since ?Series60_version
        * @param ?void ?no input param
        * @CMessageData* ?CMessageData pointer object
        */
        CMessageData* InitL(void);
   

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
        CSendUIAPITest( CTestModuleIf& aTestModuleIf );

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
        * Example test method.
        * @since ?Series60_version
        * @param aItem Script line containing parameters.
        * @return Symbian OS error code.
        */
        virtual TInt ExampleL( CStifItemParser& aItem );
        
        /**
         * Method used to log version of test class
         */
        void SendTestClassVersion();

		/**
         * Method used to call SetSubjectL of CMessageData
         */
         virtual TInt MsgDataSetSubjectL( TPtrC& aSubject );
         
         /**
         * Method used to call SubjectL of CMessageData and Compare
         */
         virtual TInt MsgDataCompSubjectL( TPtrC& aSubject );
         
		/**
        * Test Case for Calling CMessageData's NewL fun
        * @since ?Series60_version
        * @param aItem Script line containing parameters.
        * @return Symbian OS error code.
        */
      	
    	virtual TInt TestMessageDataNewL( CStifItemParser& aItem );
    	
    	/**
        * Test Case for Calling CMessageData's SetSubjectL fun
        * @since ?Series60_version
        * @param aItem Script line containing parameters.
        * @return Symbian OS error code.
        */
    	virtual TInt TestMessageDataSetSubjectL( CStifItemParser& aItem );
    	
    	/**
        * Test Case for Calling CMessageData's Subject fun
        * @since ?Series60_version
        * @param aItem Script line containing parameters.
        * @return Symbian OS error code.
        */
    	virtual TInt TestMessageDataSubjectL( CStifItemParser& aItem );
    	
    	/**
        * Test Case for Calling CMessageData's AppendAttachmentL fun
        * @since ?Series60_version
        * @param aItem Script line containing parameters.
        * @return Symbian OS error code.
        */
    	virtual TInt TestMessageDataAppendAttachmentL( CStifItemParser& aItem );
    	
    	/**
        * Test Case for Calling CMessageData's AttachmentArray fun
        * @since ?Series60_version
        * @param aItem Script line containing parameters.
        * @return Symbian OS error code.
        */
    	virtual TInt TestMessageDataAttachmentArrayL( CStifItemParser& aItem );
    	
    	/**
        * Test Case for Calling CMessageData's ClearAttachmentArray fun
        * @since ?Series60_version
        * @param aItem Script line containing parameters.
        * @return Symbian OS error code.
        */
    	virtual TInt TestMessageDataClearAttachmentArrayL( CStifItemParser& aItem );
    	
    	/**
        * Test Case for Calling CMessageData's AppendToAddressL fun
        * @since ?Series60_version
        * @param aItem Script line containing parameters.
        * @return Symbian OS error code.
        */
     	virtual TInt TestMessageDataAppendToAddressL( CStifItemParser& aItem );
     	
     	/**
        * Test Case for Calling CMessageData's ToAddressArray fun
        * @since ?Series60_version
        * @param aItem Script line containing parameters.
        * @return Symbian OS error code.
        */
     	virtual TInt TestMessageDataToAddressArrayL( CStifItemParser& aItem );
    	
    	/**
        * Test Case for Calling CMessageData's AppendCcAddressL fun
        * @since ?Series60_version
        * @param aItem Script line containing parameters.
        * @return Symbian OS error code.
        */
     	virtual TInt TestMessageDataAppendCcAddressL( CStifItemParser& aItem );
     	
     	/**
        * Test Case for Calling CMessageData's CcAddressArray fun
        * @since ?Series60_version
        * @param aItem Script line containing parameters.
        * @return Symbian OS error code.
        */
     	virtual TInt TestMessageDataCcAddressArrayL( CStifItemParser& aItem );
     	
     	/**
        * Test Case for Calling CMessageData's AppendBccAddressL fun
        * @since ?Series60_version
        * @param aItem Script line containing parameters.
        * @return Symbian OS error code.
        */
     	virtual TInt TestMessageDataAppendBccAddressL( CStifItemParser& aItem );
     	
     	/**
        * Test Case for Calling CMessageData's BccAddressArray fun
        * @since ?Series60_version
        * @param aItem Script line containing parameters.
        * @return Symbian OS error code.
        */
     	virtual TInt TestMessageDataBccAddressArrayL( CStifItemParser& aItem );
     	
     	/**
        * Test Case for Calling CMessageData's SetBodyTextL fun
        * @since ?Series60_version
        * @param aItem Script line containing parameters.
        * @return Symbian OS error code.
        */
     	virtual TInt TestMessageDataSetBodyTextL( CStifItemParser& aItem );
     	
     	/**
        * Test Case for Calling CMessageData's BodyText fun
        * @since ?Series60_version
        * @param aItem Script line containing parameters.
        * @return Symbian OS error code.
        */
     	virtual TInt TestMessageDataBodyTextL( CStifItemParser& aItem );
     	
     	/**
        * Test Case for Calling CMessageData's AppendAttachmentHandleL fun
        * @since ?Series60_version
        * @param aItem Script line containing parameters.
        * @return Symbian OS error code.
        */
     	virtual TInt  TestAppendAttachmentHandleL( CStifItemParser& aItem );
     	
     	/**
        * Test Case for Calling CMessageData's AttachmentHandleArray fun
        * @since ?Series60_version
        * @param aItem Script line containing parameters.
        * @return Symbian OS error code.
        */
     	virtual TInt  TestAttachmentHandleArrayL( CStifItemParser& aItem );
     	
     	/**
        * Test Case for Calling CMessageData's DataType fun
        * @since ?Series60_version
        * @param aItem Script line containing parameters.
        * @return Symbian OS error code.
        */
     	virtual TInt  TestDataTypeL( CStifItemParser& aItem );
     	
     	/**
        * Test Case for Calling CMessageData's ClearAddresses fun
        * @since ?Series60_version
        * @param aItem Script line containing parameters.
        * @return Symbian OS error code.
        */
     	virtual TInt  ClearAddressesL( CStifItemParser& aItem );
     	
     	/**
        * Test Case for Calling CMessageData's ExternalizeL and InternalizeL funs
        * @since ?Series60_version
        * @param aItem Script line containing parameters.
        * @return Symbian OS error code.
        */
        virtual TInt  TestExternalizeLnInternalizeL( CStifItemParser& aItem );	
        	
     	/**
        * Test Case for Calling CMessageData's ExternalizeL fun
        * @since ?Series60_version
        * @param void
        * @return Symbian OS error code.
        */
     	virtual TInt ExternalizeL( void );
    	
     	/**
        * Test Case for Calling CMessageData's InternalizeL fun
        * @since ?Series60_version
        * @param void
        * @return Symbian OS error code.
        */
     	virtual TInt InternalizeL( void );
     	


     	/**
        * Test Case for Calling CMessageData's SetOpaqueDataL fun
        * @since ?Series60_version
        * @param aItem Script line containing parameters.
        * @return Symbian OS error code.
        */
     	virtual TInt  TestSetOpaqueDataL( CStifItemParser& aItem );
     	
     	/**
        * Test Case for Calling CMessageData's OpaqueDataL fun
        * @since ?Series60_version
        * @param aItem Script line containing parameters.
        * @return Symbian OS error code.
        */
     	virtual TInt  TestOpaqueDataL( CStifItemParser& aItem );
     	
     	/**
        * Test Case for Calling CMessageAddress's NewL Construction
        * @since ?Series60_version
        * @param aItem Script line containing parameters.
        * @return Symbian OS error code.
        */
     	virtual TInt  TestCMessageAddressNewL( CStifItemParser& aItem );
     	     	
     	/**
        * Test Case for Calling CMessageAddress's SetAddressL 
        * @since ?Series60_version
        * @param aItem Script line containing parameters.
        * @return Symbian OS error code.
        */
     	virtual TInt  TestMsgAdrsSetAddressL( CStifItemParser& aItem );
     	
     	/**
        * Test Case for Calling CMessageAddress's Address 
        * @since ?Series60_version
        * @param aItem Script line containing parameters.
        * @return Symbian OS error code.
        */
     	virtual TInt  TestMsgAdrsAddress( CStifItemParser& aItem );
     	
     	/**
        * Test Case for Calling CMessageAddress's SetAliasL 
        * @since ?Series60_version
        * @param aItem Script line containing parameters.
        * @return Symbian OS error code.
        */
     	virtual TInt  TestMsgAdrsSetAliasL( CStifItemParser& aItem );
     	
     	/**
        * Test Case for Calling CMessageAddress's AliasL 
        * @since ?Series60_version
        * @param aItem Script line containing parameters.
        * @return Symbian OS error code.
        */
     	virtual TInt  TestMsgAdrsAlias( CStifItemParser& aItem );
     	
     	
     	/**
        * Test Case for Calling TSendingCapabilities functions
        * @since ?Series60_version
        * @param aItem Script line containing parameters.
        * @return Symbian OS error code.
        */
     	virtual TInt  TestTSendingCapabilities( CStifItemParser& aItem );
     	
    	
     	/**
        * Test Case for Calling CMessageAddress's ExternalizeL 
        * @since ?Series60_version
        * @param aItem Script line containing parameters.
        * @return Symbian OS error code.
        */
     	virtual TInt  TestMsgAdrsExternalizeL( CStifItemParser& aItem );
     	
     	/**
        * Test Case for Calling CMessageAddress's InternalizeL 
        * @since ?Series60_version
        * @param aItem Script line containing parameters.
        * @return Symbian OS error code.
        */
     	virtual TInt  TestMsgAdrsInternalizeL( CStifItemParser& aItem );
     	
     	/**
        * Test Case for Calling CSendingServiceInfo's CopyLC functions
        * @since ?Series60_version
        * @param aItem Script line containing parameters.
        * @return Symbian OS error code.
        */
     	virtual TInt TestCopyLC( CStifItemParser& aItem );
     	
     	/**
        * Test Case for Calling CSendingServiceInfo's SetTechnologyTypeId functions
        * @since ?Series60_version
        * @param aItem Script line containing parameters.
        * @return Symbian OS error code.
        */
     	virtual TInt TestSetTechnologyTypeIdL( CStifItemParser& aItem );

     	/**
        * Test Case for Calling CSendingServiceInfo's ServiceID functions
        * @since ?Series60_version
        * @param aItem Script line containing parameters.
        * @return Symbian OS error code.
        */
     	virtual TInt TestServiceIDL( CStifItemParser& aItem );
     	
     	/**
        * Test Case for Calling CSendingServiceInfo's ServiceProviderID functions
        * @since ?Series60_version
        * @param aItem Script line containing parameters.
        * @return Symbian OS error code.
        */
     	virtual TInt TestServiceProviderIDL( CStifItemParser& aItem );
     	
     	/**
        * Test Case for Calling CSendingServiceInfo's ServiceCapabilities functions
        * @since ?Series60_version
        * @param aItem Script line containing parameters.
        * @return Symbian OS error code.
        */
     	virtual TInt TestServiceCapabilitiesL( CStifItemParser& aItem );
     	
        //ADD NEW METHOD DEC HERE
        //[TestMethods] - Do not remove

    public:     // Data
        // ?one_line_short_description_of_data
        //?data_declaration;

    protected:  // Data
        // ?one_line_short_description_of_data
        //?data_declaration;

    private:    // Data
    	//CMessageData memeber variable 
        CMessageData* iMessageData;	
        //Buffer to read and write from stream
        CBufBase* ibuffer;
        //CMessageAddress variable
        CMessageAddress* iMessageAddress;
        //Buffer to read and write from stream  
        CBufBase* iMsgAdrbuffer;
        //?data_declaration;

        // Reserved pointer for future extension
        //TAny* iReserved;

    public:     // Friend classes
        //?friend_class_declaration;
    protected:  // Friend classes
        //?friend_class_declaration;
    private:    // Friend classes
        //?friend_class_declaration;

    };
   
 

#endif      // SendUIAPITest_H

// End of File
