/*
* Copyright (c) 2002 - 2007 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  HarvesterCollectionMediatorTest.h
*
*/



#ifndef HARVESTERCOLLECTIONMEDIATORTEST_H
#define HARVESTERCOLLECTIONMEDIATORTEST_H

//  INCLUDES
#include <StifLogger.h>
#include <TestScripterInternal.h>
#include <StifTestModule.h>
#include <TestclassAssert.h>

#include "mpxcollectionmediator.h"
#include <mpxcollectionutility.h>
#include <mpxmoveobserver.h>

// CONSTANTS
//const ?type ?constant_var = ?constant;
// MACROS
//#define ?macro ?macro_def
#define TEST_CLASS_VERSION_MAJOR 0
#define TEST_CLASS_VERSION_MINOR 0
#define TEST_CLASS_VERSION_BUILD 0

// Logging path
_LIT( KHarvesterCollectionMediatorTestLogPath, "\\logs\\testframework\\HarvesterCollectionMediatorTest\\" ); 
// Log file
_LIT( KHarvesterCollectionMediatorTestLogFile, "HarvesterCollectionMediatorTest.txt" ); 
_LIT( KHarvesterCollectionMediatorTestLogFileWithTitle, "HarvesterCollectionMediatorTest_[%S].txt" );
_LIT( KTestItemPath, "e:\\testing\\data\\test0000.mp3");
_LIT( KTestItemPath2, "e:\\testing\\data\\test0001.mp3");
_LIT( KTestItemPath3, "e:\\testing\\data\\test0002.mp3");
// FUNCTION PROTOTYPES
//?type ?function_name(?arg_list);

// FORWARD DECLARATIONS
//class ?FORWARD_CLASSNAME;
class CHarvesterCollectionMediatorTest;
class CMPXCollectionMediator;

// DATA TYPES
//enum ?declaration
//typedef ?declaration
//extern ?data_type;

// CLASS DECLARATION

/**
*  CHarvesterCollectionMediatorTest test class for STIF Test Framework TestScripter.
*  ?other_description_lines
*
*  @lib ?library
*  @since ?Series60_version
*/
NONSHARABLE_CLASS(CHarvesterCollectionMediatorTest) : public CScriptBase,
													  public MMPXMoveItemObserver
													  
													  
													  
    {
    public:  // Constructors and destructor

        /**
        * Two-phased constructor.
        */
        static CHarvesterCollectionMediatorTest* NewL( CTestModuleIf& aTestModuleIf );

        /**
        * Destructor.
        */
        virtual ~CHarvesterCollectionMediatorTest();

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
        
        //from MMPXMoveItemObserver
        virtual void HandleMoveCompleteL( TInt aErr );
       
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
        CHarvesterCollectionMediatorTest( CTestModuleIf& aTestModuleIf );

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
        
        
        
        //to test CMPXCollectionMediator::NewL(  )
        virtual TInt CMPXCollectionMediatorNewL( CStifItemParser& /*aItem*/ );      				//--debug pass
        //to test CMPXCollectionMediator::NewL( MMPXCollection& aColUtil, MMPXMediatorObserver* aObs = NULL )
        virtual TInt CMPXCollectionMediatorNewLWithMPXCollection( CStifItemParser& /*aItem*/ );		//--debug pass
        //to test CMPXCollectionMediator::~CMPXCollectionMediator()
        virtual TInt CMPXCollectionMediatorDestruction( CStifItemParser& /*aItem*/ );				//--debug pass
        //to test CMPXCollectionMediator::AddItemL( CMPXMedia*& aMediaProp)
        virtual TInt CMPXCollectionMediatorAddItemLMedia( CStifItemParser& /*aItem*/ );				//--debug pass
        //to test CMPXCollectionMediator::NewLC();
        virtual TInt CMPXCollectionMediatorNewLC( CStifItemParser& /*aItem*/ );						//--debug pass
        //to test CMPXCollectionMediator::AddItemL( CMPXMediaArray& aMPArray );
        virtual TInt CMPXCollectionAddItemLCMPXMediaArray( CStifItemParser& /*aItem*/ );			//--debug pass
        //to test CMPXCollectionMediator::SetItemL( CMPXMedia*& aMediaProp )
        virtual TInt CMPXCollectionSetItemLByCMPXMedia( CStifItemParser& /*aItem*/ );				//--debug pass
        //to test CMPXCollectionMediator::SetItemL( CMPXMediaArray& aMPArray );
        virtual TInt CMPXCollectionSetItemLByCMPXMediaArray( CStifItemParser& /*aItem*/ );			//--debug pass
        //to test CMPXCollectionMediator::SetItemL(TArray<CMPXMedia*>& aArray,CMPXCollectionPath& aPath,RArray<TInt>& aIndices );
        virtual TInt CMPXCollectionSetItemL( CStifItemParser& /*aItem*/ );							//--debug 			crash
        //to test CMPXCollectionMediator::CheckItemL( CMPXMedia& aMedia )
        virtual TInt CMPXCollectionCheckItemL( CStifItemParser& /*aItem*/ );						//--debug pass
        
        //to test CMPXCollectionMediator::MoveItemL( CMPXMedia*& aMedia,TUid aOldCollectionId,TUid aNewCollectionId )
        virtual TInt CMPXCollectionMoveItemLwith3paras( CStifItemParser& /*aItem*/ );				//--debug       	fail
        
        //to test CMPXCollectionMediator::MoveItemL( CMPXMedia*& aMedia,TUid aOldCollectionId,
        //                                           TUid aNewCollectionId,MMPXMoveItemObserver* aObs )
        virtual TInt CMPXCollectionMoveItemLwith4paras( CStifItemParser& /*aItem*/ );				//--debug pass
        
        virtual TInt CMPXCollectionMediatorWithRealCollection( CStifItemParser& /*aItem*/ );		//--debug pass
        
        /**
         * Method used to log version of test class
         */
        void SendTestClassVersion();

        //ADD NEW METHOD DEC HERE
        //[TestMethods] - Do not remove

    public:     // Data
        // ?one_line_short_description_of_data
        //?data_declaration;
    	
    	CMPXCollectionMediator* iMediator;

   
    private:    // Friend classes
        //?friend_class_declaration;
    	CMPXCollectionPath*     iCollectionPath;
    	MMPXCollectionUtility* iCollectionUtility;
    	
    };

#endif      // HARVESTERCOLLECTIONMEDIATORTEST_H

// End of File
