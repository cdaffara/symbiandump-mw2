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
* Description:  Declaration of settings api classes
*
*/



#ifndef SUPLSERVER_H
#define SUPLSERVER_H

//  INCLUDES
#include <StifLogger.h>
#include <TestScripterInternal.h>
#include <StifTestModule.h>
#include <epos_suplterminal.h>
#include <epos_CSuplSettings.h>
#include <epos_MSuplSettingsObserver.h>
#include <epos_csuplsettingparams.h>

//#include <epos_csuplsettingsengine.h>

class CObserverTest;
class MSuplSettingsObserver;
class CServerParamValues;
class CSuplSettingsInternal;
// CONSTANTS
//const ?type ?constant_var = ?constant;

// MACROS
//#define ?macro ?macro_def
// Logging path
_LIT( KsuplserverLogPath, "c:\\logs\\testframework\\suplsettings\\" ); 
// Log file
_LIT( KsuplserverLogFile, "suplsettings.txt" ); 

// FUNCTION PROTOTYPES
//?type ?function_name(?arg_list);

// FORWARD DECLARATIONS
//class ?FORWARD_CLASSNAME;
class CSettingsApi;
class CTerminalInitiatedRunSession;
class MSettingsObserver
{
	public:
	virtual void CompleteInitialize(TInt err)=0;
};
// DATA TYPES
//enum ?declaration
//typedef ?declaration
//extern ?data_type;

// CLASS DECLARATION

/**
*  CSettingsApi test class for STIF Test Framework TestScripter.
*  ?other_description_lines
*
*  @lib ?library
*  @since ?Series60_version
*/
NONSHARABLE_CLASS(CSettingsApi) : public CScriptBase, public MSuplSettingsObserver, public MSettingsObserver
    {
    public:  // Constructors and destructor
        
        /**
        * Two-phased constructor.
        */
        static CSettingsApi* NewL( CTestModuleIf& aTestModuleIf );
        
        /**
        * Destructor.
        */
        virtual ~CSettingsApi();

    public: // New functions
        
        /**
        * ?member_description.
        * @since ?Series60_version
        * @param ?arg1 ?description
        * @return ?description
        */
        //?type ?member_function( ?type ?arg1 );
      
		//	void 	HandleSuplSettingsChangeL(TSuplSettingsEventType aEvent,TBool aDBNotifier = EFalse);
			void HandleSuplSettingsChangeL(TSuplSettingsEventType aEvent,TInt aSlpId = -1 );
                       
    public: // Functions from base classes

        /**
        * From CScriptBase Runs a script line.
        * @since ?Series60_version
        * @param aItem Script line containing method name and parameters
        * @return Symbian OS error code
        */
        virtual TInt RunMethodL( CStifItemParser& aItem );
        void GetImsiTest(CSuplSettings*);
       
        void CompleteInitialize(TInt err);
                
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
        CSettingsApi( CTestModuleIf& aTestModuleIf );

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
        //TInt TestConnect( CStifItemParser& aItem );
       //priya
        TInt InitializeImsi();
        
        TInt ListenForChanges();
        TInt CancelInitializeTest();
        TInt GetHslpFromImsi();
        TInt IsImsiChangedTest();
        TInt RemoveObserver();
        TInt GetImsiWithoutInitialize();
        TInt RemoveBeforeSetObserver();
        TInt IsImsiChangedWithoutInitialize();
        TInt GenerateHslpFromImsiWithoutInitialize();
        TInt GenerateHslpWithoutInitialize();
			  TInt SetUsageAutomatic();
        TInt SetUsageAlwaysAsk();
        TInt SetUsageAutoInHMNw();
        TInt SetUsageDisabled();
        TInt GetUsageAutomatic();
        TInt GetUsageAlwaysAsk();
        TInt GetUsageAutoInHMNw();
        TInt GetUsageDisabled();
      	TInt SetUsageOtherThanAppropriate();
        TInt RemoveServer(CStifItemParser& aItem);
        TInt RemoveServer_Invalid_negative(CStifItemParser& aItem);
        TInt RemoveServer_Id_NotExisting(CStifItemParser& aItem);
        TInt RemoveServer_Invalid_zero(CStifItemParser& aItem);
        TInt ChangePriority_Increase(CStifItemParser& aItem);
        TInt ChangePriority_Decrease(CStifItemParser& aItem);
        TInt ChangePriority_invalid_positive(CStifItemParser& aItem);
        TInt ChangePriority_priority0(CStifItemParser& aItem);
        TInt ChangePriority_Negative(CStifItemParser& aItem);
        TInt ChangePriority_InvalidServer(CStifItemParser& aItem);
      	TInt ChangePriority_invalidserverpriority(CStifItemParser& aItem);
        TInt GetDefaultServer();
				TInt AddNewServerWithDefault(CStifItemParser& aItem);
				TInt AddNewServerWithoutDefault(CStifItemParser& aItem);
				TInt AddNewServer_Duplicate(CStifItemParser& aItem);
				TInt GetSlpInfoFromId(TInt64 Slpid);
				TInt GetSlpInfoFromId(CStifItemParser& aItem);
				TInt GetSlpInfoFromId_invalid_positive(CStifItemParser& aItem);
				TInt GetSlpInfoFromId_invalid_negative(CStifItemParser& aItem);
				TInt GetSlpInfoFromId_invalid_zero(CStifItemParser& aItem);
				TInt SetServerAddress(CStifItemParser& aItem);
				TInt SetServerAddress_invalidpositive(CStifItemParser& aItem);
				TInt SetServerAddress_invalidnegative(CStifItemParser& aItem);
				TInt SetServerAddress_invalidzero(CStifItemParser& aItem);
				TInt SetServerAddress_AddressNull(CStifItemParser& aItem);
				TInt SetServerAddress_InvalidAddressId(CStifItemParser& aItem);
				TInt GetServerAddress(CStifItemParser& aItem);
				TInt GetServerAddress_invalidpositive(CStifItemParser& aItem);
				TInt GetServerAddress_invalidnegative(CStifItemParser& aItem);
				TInt GetServerAddress_invalidzero(CStifItemParser& aItem);
				TInt GetServerAddress(TInt64 aSlpId1);
				TInt SetIAPName_InvalidAddressId(CStifItemParser& aItem);
				TInt SetIAPName_invalidzero(CStifItemParser& aItem);
				TInt SetIAPName_invalidnegative(CStifItemParser& aItem);
				TInt SetIAPName_invalidpositive(CStifItemParser& aItem);
				TInt SetIAPName(CStifItemParser& aItem);
				 TInt GetIAPName_invalidzero(CStifItemParser& aItem);
				 TInt GetIAPName_invalidnegative(CStifItemParser& aItem);
				 TInt GetIAPName_invalidpositive(CStifItemParser& aItem);
				 TInt GetIAPName(CStifItemParser& aItem);
				 TInt GetIAPName(TInt64 aSlpId1);
				 TInt SetServerEnabledFlag_Etrue(CStifItemParser& aItem);
				 TInt SetServerEnabledFlag_EFalse(CStifItemParser& aItem);
				 TInt SetServerEnabledFlag_id_negative(CStifItemParser& aItem);
				 TInt SetServerEnabledFlag_id_positive(CStifItemParser& aItem);
				  TInt SetServerEnabledFlag_id_zero(CStifItemParser& aItem);
				 TInt GetServerEnabledFlag_id_negative(CStifItemParser& aItem);
				  TInt GetServerEnabledFlag_id_positive(CStifItemParser& aItem);
				  TInt GetServerEnabledFlag_id_zero(CStifItemParser& aItem);
				  TInt GetServerEnabledFlag_EFalse(CStifItemParser& aItem);
				  TInt GetServerEnabledFlag_Etrue(CStifItemParser& aItem);
				  TInt GetUsageInHomeNwFlag(TInt64 aSlpId1);
				  TInt SetUsageInHomwNwFlag_Etrue(CStifItemParser& aItem);
				  TInt SetUsageInHomwNwFlag_EFalse(CStifItemParser& aItem);
				  TInt SetUsageInHomwNwFlag_id_negative(CStifItemParser& aItem);
				  TInt SetUsageInHomwNwFlag_id_positive(CStifItemParser& aItem);
				   TInt SetUsageInHomwNwFlag_id_zero(CStifItemParser& aItem);
					 TInt GetUsageInHomwNwFlag_Etrue(CStifItemParser& aItem);
				   TInt GetUsageInHomwNwFlag_EFalse(CStifItemParser& aItem);
				   TInt GetUsageInHomwNwFlag_id_negative(CStifItemParser& aItem);
				   TInt GetUsageInHomwNwFlag_id_positive(CStifItemParser& aItem);
				   TInt GetUsageInHomwNwFlag_id_zero(CStifItemParser& aItem);
				   TInt GetUsageInHomwNwFlag(TInt64 aSlpId1);
				 TInt AddNewServer_aServerAddress_Null(CStifItemParser& aItem);
				  TInt AddNewServer_aIapNAme_Null(CStifItemParser& aItem);
				 TInt SetDefaultServer_aServerAddress_Null(CStifItemParser& aItem);
	 TInt SetDefaultServerWithDefaultValues(CStifItemParser& aItem);
				 TInt SetAllParameter_aServerAddress_Null(CStifItemParser& aItem);
  TInt SetAllParameter_WithDefaultValues(CStifItemParser& aItem);
				 TInt SetAllParameter_Invalid_Positive(CStifItemParser& aItem);
				 TInt SetAllParameter_Invalid_Negative(CStifItemParser& aItem);
				 TInt SetAllParameter_Invalid_Zero(CStifItemParser& aItem);
				TInt GetServerEnabledFlag(TInt64 aSlpId1);
				TInt SetEditableFlag_Etrue(CStifItemParser& aItem);
				TInt SetEditableFlag_EFalse(CStifItemParser& aItem);
				TInt SetEditableFlag_id_negative(CStifItemParser& aItem);
				TInt SetEditableFlag_id_positive(CStifItemParser& aItem);
				TInt SetEditableFlag_id_zero(CStifItemParser& aItem);
				TBool IsSLPExists(CStifItemParser& aItem);
				TBool IsSLPExists_invalid_negative(CStifItemParser& aItem);
				TBool IsSLPExists_invalid_positive(CStifItemParser& aItem);
				TBool IsSLPExists_invalid_zero(CStifItemParser& aItem);
				TInt GetAllSlp_sync(CStifItemParser& aItem);
				TInt GetAllSlp_async(CStifItemParser& aItem);
				TInt SetSimChangeRemoveFlag_Etrue(CStifItemParser& aItem);
				TInt SetSimChangeRemoveFlag_EFalse(CStifItemParser& aItem);
				TInt SetSimChangeRemoveFlag_id_negative(CStifItemParser& aItem);
				TInt SetSimChangeRemoveFlag_id_positive(CStifItemParser& aItem);
				TInt SetSimChangeRemoveFlag_id_zero(CStifItemParser& aItem);
				TInt GetSimChangeRemoveFlag_Etrue(CStifItemParser& aItem);
				TInt GetSimChangeRemoveFlag(TInt64 aSlpId1);
				TInt GetSimChangeRemoveFlag_EFalse(CStifItemParser& aItem);
				TInt GetSimChangeRemoveFlag_id_negative(CStifItemParser& aItem);
				TInt GetSimChangeRemoveFlag_id_positive(CStifItemParser& aItem);
				TInt GetSimChangeRemoveFlag_id_zero(CStifItemParser& aItem);
				TInt GetEditableFlag_Etrue(CStifItemParser& aItem);
				TInt GetEditableFlag(TInt64 aSlpId1);
				TInt GetEditableFlag_EFalse(CStifItemParser& aItem);
				TInt GetEditableFlag_id_negative(CStifItemParser& aItem);
				TInt GetEditableFlag_id_positive(CStifItemParser& aItem);
				TInt GetEditableFlag_id_zero(CStifItemParser& aItem);
				TInt SetFallBack_ETrue();
				TInt SetFallBack_EFalse();
				TInt GetFallBack();
				TInt IsSLPExistsString();
				TInt IsSLPExistsString_Null();
				TInt IsSLPExistsString_invalid();
				TInt SlpCount();
				TInt GetSlpInfoFromAddress(CStifItemParser& aItem);
				TInt GetSlpInfoFromAddress_invalid(CStifItemParser& aItem);
		
				TInt SetGetTriggerParamsL( CStifItemParser& aItem ); 
				
                
        virtual TInt GetAllSessionSetApiL( CStifItemParser& aItem );
       
              
        
       
        virtual TInt SetSessionObserverL( CStifItemParser& aItem );
        virtual TInt SetSessionObserverTwiceL( CStifItemParser& aItem );
        virtual TInt RemoveSessionObserverL( CStifItemParser& aItem );
        virtual TInt RemoveSessionObserverWithoutSetL( CStifItemParser& aItem );
        virtual TInt CheckSessionChangeEventsL( CStifItemParser& aItem );
        
        virtual TInt GetASessionParamNullL( CStifItemParser& aItem );
        virtual TInt GetASessionParamInValidL( CStifItemParser& aItem );
        virtual TInt CancelTriggerSessionL( CStifItemParser& aItem );
        
       
        virtual TInt SetTriggerEmptySessionNameL( CStifItemParser& aItem );
        virtual TInt SetTriggerLongSessionNameL( CStifItemParser& aItem );

        TInt ChangeSuplTriggerServiceStatusON();
            

        TInt ChangeSuplTriggerServiceStatusOFF();
           

        TInt GetSuplTriggerServiceStatusON();


        TInt GetSuplTriggerServiceStatusOFF();
        
        TInt ProductConfiguredServer1();
        TInt ProductConfiguredServer2();
        TInt ProductConfiguredServer3();
        TInt ProductConfiguredServer4();

       // TInt CSettingsApi::SetstatusOtherThanAppropriate();
    public : // from MSuplSettingsObserver
        virtual void HandleSuplTriggerStatusChangeL(
                       CSuplSettings::TSuplTriggerStatus aSuplTriggerStatus );
      
				   
    public:     // Data
        // ?one_line_short_description_of_data
        //?data_declaration;
    
    protected:  // Data
        // ?one_line_short_description_of_data
        //?data_declaration;

    private:    // Data
        
        // ?one_line_short_description_of_data
        //?data_declaration;
         
        // Reserved pointer for future extension
        //TAny* iReserved;
		MSuplSettingsObserver *iObs;
    public:     // Friend classes
        //?friend_class_declaration;
    protected:  // Friend classes
        //?friend_class_declaration;
    private:    // Friend classes
        //?friend_class_declaration;
        CActiveScheduler *iScheduler;
        	CSuplSettings *iSettings;
        	CSuplSettingsInternal *cSettings;
        	CTerminalInitiatedRunSession *imyAO;
        	TInt iTestid;
        	TInt iErrCode;
        	TInt iSlpId;
        	CStifLogger* iLog;
        	
};
    

class CTerminalInitiatedRunSession : public CActive
	{
	public :

    CTerminalInitiatedRunSession();
	static CTerminalInitiatedRunSession* NewL(MSettingsObserver *aObserver);
		
	void ConstructL(MSettingsObserver *aObserver);
	
	~CTerminalInitiatedRunSession();
	void RunSession(CSuplSettings *aSettings);
	//void GetPositionL(TSuplPosition& aPosition);
	
	void RunL();
	void DoCancel();
	void CancelRunSession(CSuplSettings *aSettings);
	void StartListening();
	
	private:
	
   CSuplSettings *iSettings;
   CSettingsApi *iTest;
   	CSuplSettingsInternal *cSettings;
   MSettingsObserver *iObserver;
   RTimer iTimer;
    
	};
class CObserverTest: public MSuplSettingsObserver
{
	public:

	
	private:
	MSuplSettingsObserver *iObserver;
	
	
	 private:    // Constructor
        /**
        * C++ Constructor
        */
        CObserverTest(MSuplSettingsObserver *aObserver);

    public:     // Destructor
        /**
        * C++ Destructor
        */
        ~CObserverTest();

        /**
        * Two-phase constructor
        */
        static CObserverTest* NewL(MSuplSettingsObserver *aObserver);

    private:
        /**
        * EPOC default constructor
        */
        void ConstructL();

        // By default, prohibit copy constructor
        CObserverTest( const CObserverTest& );
	
};
#endif      // SUPLSERVER_H            
// End of File


