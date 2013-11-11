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
* Description:  CCommonTestClass header for STIF Test Framework TestScripter.
*
*/



#ifndef COMMONTESTCLASS_H
#define COMMONTESTCLASS_H

//  INCLUDES
#include <StifLogger.h>
#include <TestScripterInternal.h>
#include <StifTestModule.h>
#include <aknviewappui.h>
#include "timeoutcontroller.h"
#include "debug.h"

#include <mpxpluginmonitor.h>
#include <mpxpluginmonitorobserver.h>
#include <mpxcenrepwatcher.h>
#include <mpxcenrepobserver.h>
#include <mpxparameter.h>
#include <mpxcollectiontype.h>
#include <mpxpskeywatcher.h>
#include <mpxpskeyobserver.h>
#include <mpxdrmmediautility.h>
#include <mpxmessage.h>
#include <mpxplaybackmessage.h>
#include <mpxmessagequeue.h>
#include <mpxattribute.h>
#include <mpxitemid.h>
#include <mpxsession.h>
#include <mpxmessagemonitor.h>
#include <mpxtaskqueue.h>
#include <mpxtaskqueueobserver.h>
#include <mpxuser.h>
#include <mpxclientlist.h>
#include <mpxviewplugin.h>
#include <mpxcustomcommandobserver.h>
#include <mpxplugininfo.h>
#include <mpxsubscription.h>

// CONSTANTS
const TInt KErrBadTestParameter= -1000;         // Error on configuration file
const TInt KErrEventPending =-2000;             // Timeout and not callback 
const TInt KErrCallbackErrorCode = -2001;       // Callback return error code
const TInt KErrUnexpectedValue =-2002;          // Unexpected value at setting    
const TInt KErrTimeoutController = -2007;		// Unexpected notification
const TInt KShortTimeout = 1000;
const TInt KMediumTimeout = 4000;
const TInt KLongTimeout = 100000;
const TInt KFailTOInstanceClass = -3000;



#ifdef __WINSCW__
_LIT( KCommonTestClassLogPath, "\\testing\\log\\" ); 
#else
_LIT( KCommonTestClassLogPath, "e:\\testing\\log\\" );
#endif

_LIT( KCommonTestClassLogFile, "CommonTestClass.log" ); 
_LIT( KCommonTestFilePath, "e:\\testing\\data\\" );
_LIT( KMsgBadTestParameters, "[Error] No valid test case parameters");
_LIT( KTagCompareNotEqual, "<>");
_LIT( KTagCompareEqual, "==");
_LIT( KTagvalue, "value");

// FORWARD DECLARATIONS
//class CMPXPluginMonitor;
//class CMPXCenRepWatcher;

// CLASS DECLARATION
class CMPXMedia;
class CMPXMediaArray;
class CMPXCollectionPath;
class CMPXSubscription;
/**
*  CCommonTestClass test class for STIF Test Framework TestScripter.
*
*  @lib RadioUtilityTestClass.lib
*  @since 5.0
*/
NONSHARABLE_CLASS(CCommonTestClass) : public CScriptBase, 
									public MTimeoutObserver,
									public MMPXPluginMonitorObserver,
									public MMPXCenRepObserver,
									public MMPXPSKeyObserver,
									public MMPXMessageObserver,
									public MMPXTaskQueueObserver,
									public MMPXCustomCommandObserver

    {
    public:  // Constructors and destructor

        /**
        * Two-phased constructor.
        */
        static CCommonTestClass* NewL( CTestModuleIf& aTestModuleIf );

        /**
        * Destructor.
        */
        virtual ~CCommonTestClass();

    public: // Functions from base classes

		enum TMPXTestExpectedEvent
		{
		};

        /**
        * From CScriptBase Runs a script line.
        * @since 3.2
        * @param aItem Script line containing method name and parameters
        * @return Symbian OS error code
        */
        virtual TInt RunMethodL( CStifItemParser& aItem );

	    // From MTimeoutObserver

	    /**
	    * @since 3.2
	    * Review if all the expected events have ocurred once the time is over
	    */
	    void HandleTimeout(TInt error);



    private:

        /**
        * C++ default constructor.
        */
        CCommonTestClass( CTestModuleIf& aTestModuleIf );

        /**
        * By default Symbian 2nd phase constructor is private.
        */
        void ConstructL();

        /**
        * Delete the resources before exit
        * @since S60 3.2
        * @param aItem Script line containing parameters.
        * @return Symbian OS error code.
        */
        void Delete();
        
	    /**
	    * Set an event as expected and set default timeout
	    * @since S60 3.2
	    */
	    void AddExpectedEvent(TMPXTestExpectedEvent event, TInt ms);


	    /**
	    * Unset an event as expected
	    * @since S60 3.2
	    */
	    TBool RemoveExpectedEvent(TMPXTestExpectedEvent event);


	    /**
	    * @since S60 3.2
	    * Removes all expected events
	    */
	    void RemoveAllExpectedEvents();

	    /**
	    * Verify that the event was expected, removes it from the list
	    * Signal the TestScripter with the returned error code
	    * @since S60 3.2
	    */
	    void ProcessEvent(TMPXTestExpectedEvent aEvent, TInt aError);

	    /**
	    * Maps a event with a descriptor with its name
	    * @since S60 3.2
	    */
	    TPtrC EventName( TInt aKey );

	    /**
	    * Sets a timeout different since the default
	    * @since S60 3.2
	    * @param aItem Script line containing parameters.
	    * @return Symbian OS error code.
	    */
	    TInt SetTimeout( CStifItemParser& aItem );

	    /**
	    *
	    * @since S60 3.2
	    * @param aItem Script line containing parameters.
	    * @return Symbian OS error code.
	    */
	    TInt SetExpectedEvents( CStifItemParser& aItem );

	    /**
	    *
	    * @since S60 3.2
	    * @param aItem Script line containing parameters.
	    * @return Symbian OS error code.
	    */
	    TInt SetAllowedPanic( CStifItemParser& aItem );
	    
	    /**
	    * To test CMPXPluginMonitor::NewL
	    *
	    * @since S60 5.0
	    * @return Symbian OS error code.
	    */
	    TInt CreatePluginMonitorL();///

	    /**
	    * To test CMPXPluginMonitor::~
	    *
	    * @since S60 5.0
	    * @return Symbian OS error code.
	    */
	    TInt DeletePluginMonitorL();

        /*
         * From MMPXPluginMonitorObserver
         */
        void PluginsChangedL();

        void HandlePluginUnload(const TUid& aPluginUid);////add
	    /**
	    * To test CMPXCenRepWatcher::NewL
	    *
	    * @since S60 5.0
	    * @return Symbian OS error code.
	    */
	    TInt CreateCenRepWatcherL();

	    /**
	    * To test CMPXCenRepWatcher::~
	    *
	    * @since S60 5.0
	    * @return Symbian OS error code.
	    */
	    TInt DeleteCenRepWatcherL();

        /*
         * From MMPXCenRepObserver
         *
         */
        void HandleSettingChange(const TUid& aRepositoryUid,
                                         TUint32 aSettingId);

        /**
        * Get current value as integer.
        *
        * @since 5.0
	    * @return Symbian OS error code.
        */
        TInt CenRepCurrentValueL(CStifItemParser& aItem);
        
        /**
        * Set integer value
        *
        * @since 5.0
	    * @return Symbian OS error code.
        */
        TInt CenRepSetValueL(CStifItemParser& aItem);


	    /**
	    * To test CMPXParameter::CMPXParameter()
	    *
	    * @since S60 5.0
	    * @return Symbian OS error code.
	    */
	    TInt CreateMPXParameter();

	    /**
	    * To test CMPXParameter::~
	    *
	    * @since S60 5.0
	    * @return Symbian OS error code.
	    */
	    TInt DeleteMPXParameter();
	    

	    /**
	    * To test CMPXParameter::InternalizeL
	    *
	    * @since S60 5.0
	    * @return Symbian OS error code.
	    */
	    TInt InternalizeMPXParameterL();


	    /**
	    * To test CMPXParameter::ExternalizeL
	    *
	    * @since S60 5.0
	    * @return Symbian OS error code.
	    */
	    TInt ExternalizeMPXParameterL();
	    
	    
	    /**
	    * To test CMPXCollectionType::MPXCollectionType(...)
	    *
	    * @since S60 5.0
	    * @return Symbian OS error code.
	    */
	    //TInt CreateMPXCollectionType();
	    

	    /**
	    * To test CMPXCollectionType::MPXCollectionType()
	    *
	    * @since S60 5.0
	    * @return Symbian OS error code.
	    */
	    TInt CreateMPXCollectionTypeWOParam();

	    
	    /**
	    * To test CMPXCollectionType::InternalizeMPXCollectionTypeL()
	    *
	    * @since S60 5.0
	    * @return Symbian OS error code.
	    */
	    TInt InternalizeMPXCollectionTypeL();

	    
	    /**
	    * To test CMPXCollectionType::ExternalizeMPXCollectionTypeL()
	    *
	    * @since S60 5.0
	    * @return Symbian OS error code.
	    */
	    TInt ExternalizeMPXCollectionTypeL();


	    /**
	    * To test CMPXPSKeyWatcherL::NewL()
	    *
	    * @since S60 5.0
	    * @return Symbian OS error code.
	    */
        TInt CreateCMPXPSKeyWatcherL();


	    /**
	    * To test CMPXPSKeyWatcherL::~
	    *
	    * @since S60 5.0
	    * @return Symbian OS error code.
	    */
        TInt DeleteCMPXPSKeyWatcherL();


	    /**
	    * To test CMPXPSKeyWatcherL::GetValue()
	    *
	    * @since S60 5.0
	    * @return Symbian OS error code.
	    */
        TInt GetTntValue();


	    /**
	    * To test CMPXPSKeyWatcherL::GetDes8Value()
	    *
	    * @since S60 5.0
	    * @return Symbian OS error code.
	    */
        TInt GetDes8Value();


	    /**
	    * To test CMPXPSKeyWatcherL::GetDes16Value()
	    *
	    * @since S60 5.0
	    * @return Symbian OS error code.
	    */
        TInt GetDes16Value();


	    /**
	    * To test CMPXPSKeyWatcherL::SetValue()
	    *
	    * @since S60 5.0
	    * @return Symbian OS error code.
	    */
        TInt SetTntValue();


	    /**
	    * To test CMPXPSKeyWatcherL::SetDes8Value()
	    *
	    * @since S60 5.0
	    * @return Symbian OS error code.
	    */
        TInt SetDes8Value();


	    /**
	    * To test CMPXPSKeyWatcherL::SetDes16Value()
	    *
	    * @since S60 5.0
	    * @return Symbian OS error code.
	    */
        TInt SetDes16Value();
        
	    /**
	    * From MMPXPSKeyObserver
	    *
	    * @since S60 5.0
	    * @return Symbian OS error code.
	    */
        void HandlePSEvent(TUid aUid, TInt aKey);

	    /**
	    * To test CMPXDrmMediaUtilityL::NewL()
	    *
	    * @since S60 5.0
	    * @return Symbian OS error code.
	    */
        TInt CreateCMPXDrmMediaUtilityL();

	    /**
	    * To test CMPXDrmMediaUtilityL::NewLC()
	    *
	    * @since S60 5.0
	    * @return Symbian OS error code.
	    */
        TInt CreateCMPXDrmMediaUtilityLC();

	    /**
	    * To test CMPXDrmMediaUtilityL::~
	    *
	    * @since S60 5.0
	    * @return Symbian OS error code.
	    */
        TInt DeleteCMPXDrmMediaUtilityL();

	    /**
	    * To test CMPXDrmMediaUtilityL::InitL
	    *
	    * @since S60 5.0
	    * @return Symbian OS error code.
	    */
        TInt InitFileNameL();

	    /**
	    * To test CMPXDrmMediaUtilityL::InitL
	    *
	    * @since S60 5.0
	    * @return Symbian OS error code.
	    */
        TInt InitRFileL();

	    /**
	    * To test CMPXDrmMediaUtilityL::Close
	    *
	    * @since S60 5.0
	    * @return Symbian OS error code.
	    */

        TInt Close();

	    /**
	    * To test CMPXDrmMediaUtilityL::GetMediaL
	    *
	    * @since S60 5.0
	    * @return Symbian OS error code.
	    */
        TInt GetMediaL();

	    /**
	    * To test CMPXDrmMediaUtilityL::ConsumeL
	    *
	    * @since S60 5.0
	    * @return Symbian OS error code.
	    */
        TInt ConsumeL();

	    /**
	    * To test TMPXMessage::TMPXMessageL(...)
	    *
	    * @since S60 5.0
	    * @return Symbian OS error code.
	    */
        TInt TMPXMessage3L();

	    /**
	    * To test TMPXMessage::TMPXMessageL(...)
	    *
	    * @since S60 5.0
	    * @return Symbian OS error code.
	    */
        TInt TMPXMessage1L();

	    /**
	    * To test TMPXMessage::TMPXMessageL()
	    *
	    * @since S60 5.0
	    * @return Symbian OS error code.
	    */
        TInt TMPXMessageL();

	    /**
	    * To test TMPXMessage::EqualTMPXMessage()
	    *
	    * @since S60 5.0
	    * @return Symbian OS error code.
	    */
        TInt EqualTMPXMessage();

	    /**
	    * To test TMPXMessage::Event()
	    *
	    * @since S60 5.0
	    * @return Symbian OS error code.
	    */
        TInt Event();

	    /**
	    * To test TMPXMessage::Type()
	    *
	    * @since S60 5.0
	    * @return Symbian OS error code.
	    */
        TInt Type();

	    /**
	    * To test TMPXMessage::Data()
	    *
	    * @since S60 5.0
	    * @return Symbian OS error code.
	    */
        TInt Data();

	    /**
	    * To test CMPXMessageQueueL::NewL()
	    *
	    * @since S60 5.0
	    * @return Symbian OS error code.
	    */
        TInt CreateCMPXMessageQueueL();

	    /**
	    * To test CMPXMessageQueueL::~
	    *
	    * @since S60 5.0
	    * @return Symbian OS error code.
	    */
        TInt DeleteCMPXMessageQueue();
        
	    /**
	    * To test CMPXMessageQueueL::Reset
	    *
	    * @since S60 5.0
	    * @return Symbian OS error code.
	    */
        TInt ResetCMPXMessageQueue();
        
	    /**
	    * To test CMPXMessageQueueL::SendNext
	    *
	    * @since S60 5.0
	    * @return Symbian OS error code.
	    */
        TInt SendNextCMPXMessageQueue();

	    /**
	    * To test CMPXMessageQueueL::Add
	    *
	    * @since S60 5.0
	    * @return Symbian OS error code.
	    */
        TInt AddCMPXMessageQueue();

        /**
	    * To test RMPXSession::RMPXSession()
	    *
	    * @since S60 5.0
	    * @return Symbian OS error code.
	    */
        TInt CreateRMPXSession();
        
        /**
	    * To test RMPXSession::~RMPXSession()
	    *
	    * @since S60 5.0
	    * @return Symbian OS error code.
	    */
       	TInt CloseAndDestructRMPXSession();
	    /**
	    * To test RMPXSession::Version()
	    *
	    * @since S60 5.0
	    * @return Symbian OS error code.
	    */        
        TInt VersionRMPXSession();
        
	    /**
	    * To test RMPXSession::Connect()
	    *
	    * @since S60 5.0
	    * @return Symbian OS error code.
	    */
        TInt ConnectRMPXSessionL();
        
	    /**
	    * To test RMPXSession::SendReceive(...)
	    *
	    * @since S60 5.0
	    * @return Symbian OS error code.
	    */
        TInt SendReceiveLRMPXSessionL();

        
	    /**
	    * To test RMPXSession::SendReceive(...)
	    *
	    * @since S60 5.0
	    * @return Symbian OS error code.
	    */
        TInt SendReceiveStatusRMPXSessionL();

	    /**
	    * To test RMPXSession::SendReceive(...)
	    *
	    * @since S60 5.0
	    * @return Symbian OS error code.
	    */
        TInt SendReceiveStatus3RMPXSessionL();
        
	    /**
	    * To test CMPXMessageMonitor::NewL()
	    *
	    * @since S60 5.0
	    * @return Symbian OS error code.
	    */
        TInt CreateCMPXMessageMonitorL();

	    /**
	    * To test CMPXMessageMonitor::~
	    *
	    * @since S60 5.0
	    * @return Symbian OS error code.
	    */
        TInt DeleteCMPXMessageMonitorL();

	    /**
	    * To test CMPXMessageMonitor::RunLCMPXMessageMonitorL()
	    *
	    * @since S60 5.0
	    * @return Symbian OS error code.
	    */
        TInt RunLCMPXMessageMonitorL();
        
        /**
         * From MMPXMessageObserver
         */
        void MessageReceived(TInt aMsgData, TInt aError);

	    /**
	    * To test CMPXActiveTaskQueue::NewL()
	    *
	    * @since S60 5.0
	    * @return Symbian OS error code.
	    */
        TInt CreateCMPXActiveTaskQueueL();

	    /**
	    * To test CMPXActiveTaskQueue::AddTaskL(...)
	    *
	    * @since S60 5.0
	    * @return Symbian OS error code.
	    */
        TInt AddTaskCMPXActiveTaskQueueL();
        
	    /**
	    * To test CMPXActiveTaskQueue::CancelRequests(...)
	    *
	    * @since S60 5.0
	    * @return Symbian OS error code.
	    */
        TInt CancelRequestsCMPXActiveTaskQueueL();
        
	    /**
	    * To test CMPXActiveTaskQueue::CompleteTask(...)
	    *
	    * @since S60 5.0
	    * @return Symbian OS error code.
	    */
        TInt CompleteTaskCMPXActiveTaskQueueL();
        
	    /**
	    * To test CMPXActiveTaskQueue::CompleteAllTasks(...)
	    *
	    * @since S60 5.0
	    * @return Symbian OS error code.
	    */
        TInt CompleteAllTasksCMPXActiveTaskQueueL();
        
	    /**
	    * To test CMPXTaskQueue::IsEmpty()
	    *
	    * @since S60 5.0
	    * @return Symbian OS error code.
	    */
        TInt IsEmptyCMPXTaskQueueL();
        
	    /**
	    * To test CMPXTaskQueue::AddTask()
	    *
	    * @since S60 5.0
	    * @return Symbian OS error code.
	    */
        TInt AddTaskCMPXTaskQueueL();
        
	    /**
	    * To test CMPXTaskQueue::RemoveTask(observer)
	    *
	    * @since S60 5.0
	    * @return Symbian OS error code.
	    */
        TInt RemoveTaskCMPXTaskQueueL();

	    /**
	    * To test CMPXTaskQueue::RemoveTask()
	    *
	    * @since S60 5.0
	    * @return Symbian OS error code.
	    */
        TInt RemoveTask2CMPXTaskQueueL();

	    /**
	    * To test CMPXTaskQueue::Callback()
	    *
	    * @since S60 5.0
	    * @return Symbian OS error code.
	    */
        TInt CallbackCMPXTaskQueueL();

	    /**
	    * To test CMPXTaskQueue::PtrData()
	    *
	    * @since S60 5.0
	    * @return Symbian OS error code.
	    */
        TInt PtrDataCMPXTaskQueueL();

	    /**
	    * To test CMPXTaskQueue::param()
	    *
	    * @since S60 5.0
	    * @return Symbian OS error code.
	    */
        TInt ParamCMPXTaskQueueL();

	    /**
	    * To test CMPXTaskQueue::BufData()
	    *
	    * @since S60 5.0
	    * @return Symbian OS error code.
	    */
        TInt BufDataCMPXTaskQueueL();

	    /**
	    * To test CMPXTaskQueue::Task()
	    *
	    * @since S60 5.0
	    * @return Symbian OS error code.
	    */
        TInt TaskCMPXTaskQueueL();

        /*
         * From MMPXTaskQueueObserver
         */
        void ExecuteTask(TInt aTask,
                             TInt aParamData, 
                             TAny* aPtrData,
                             const CBufBase& aBuf,
                             TAny* aCallback,
                             CBase* aCObject1,
                             CBase* aCObject2);	    

	    /**
	    * To test MPXUser::CompareUids
	    *
	    * @since S60 5.0
	    * @return Symbian OS error code.
	    */
        TInt CompareUids();

	    /**
	    * To test MPXUser::IsCallOngoing
	    *
	    * @since S60 5.0
	    * @return Symbian OS error code.
	    */
        TInt IsCallOngoing();

	    /**
	    * To test MPXUser::CompleteWithDllPath
	    *
	    * @since S60 5.0
	    * @return Symbian OS error code.
	    */
        TInt CompleteWithDllPath();

	    /**
	    * To test MPXUser::Alloc8L
	    *
	    * @since S60 5.0
	    * @return Symbian OS error code.
	    */
        TInt Alloc8L();

	    /**
	    * To test MPXUser::AllocL
	    *
	    * @since S60 5.0
	    * @return Symbian OS error code.
	    */
        TInt AllocL();

	    /**
	    * To test MPXUser::Ptr
	    *
	    * @since S60 5.0
	    * @return Symbian OS error code.
	    */
        TInt Ptr();

	    /**
	    * To test MPXUser::Ptr8
	    *
	    * @since S60 5.0
	    * @return Symbian OS error code.
	    */
        TInt Ptr8();
        
	    /**
	    * To test MPXUser::CreateBufferLC
	    *
	    * @since S60 5.0
	    * @return Symbian OS error code.
	    */
        TInt CreateBufferLC();

	    /**
	    * To test MPXUser::CreateBufferL
	    *
	    * @since S60 5.0
	    * @return Symbian OS error code.
	    */
        TInt CreateBufferDesCL();

	    /**
	    * To test MPXUser::CreateBufferL
	    *
	    * @since S60 5.0
	    * @return Symbian OS error code.
	    */
        TInt CreateBufferDesCArrayL();
        /*
         * to test IMPORT_C static void CreateBufferL(const RMessage2& aMessage,
         *                                 			  TInt aMsgSlot,
         *                                 			  CBufBase*& aBuffer);
        */
        TInt CreateBufferMsgL();

	    /**
	    * To test MPXUser::CreateBufferL
	    *
	    * @since S60 5.0
	    * @return Symbian OS error code.
	    */
        TInt CreateFromBufferL();

	    /**
	    * To test MPXUser::ExternalizeL
	    *
	    * @since S60 5.0
	    * @return Symbian OS error code.
	    */
        TInt ExternalizeL();

	    /**
	    * To test MPXUser::ExternalizeL
	    *
	    * @since S60 5.0
	    * @return Symbian OS error code.
	    */
        TInt ExternalizeArrayL();

	    /**
	    * To test MPXUser::InternalizeL
	    *
	    * @since S60 5.0
	    * @return Symbian OS error code.
	    */
        TInt InternalizeArrayL();

	    /**
	    * To test MPXUser::
	    * IMPORT_C static void InternalizeL(CDesCArray*& aArray,
        *                                  				     RReadStream& aStream);
	    * @since S60 5.0
	    * @return Symbian OS error code.
	    */
        TInt InternalizeDesCArrayL();
        
	    /**
	    * To test MPXUser::CopyArrayL
	    *
	    * @since S60 5.0
	    * @return Symbian OS error code.
	    */
        TInt CopyArrayL();

	    /**
	    * To test MPXUser::MergeArray
	    *
	    * @since S60 5.0
	    * @return Symbian OS error code.
	    */
        TInt MergeArray();

	    /**
	    * To test MPXUser::MergeAttributeL
	    *
	    * @since S60 5.0
	    * @return Symbian OS error code.
	    */
        TInt MergeAttributeL();

	    /**
	    * To test MPXUser::ProcessIdL
	    *
	    * @since S60 5.0
	    * @return Symbian OS error code.
	    */
        TInt ProcessIdL();

	    /**
	    * To test MPXUser::CompareOrderedUidArrays
	    *
	    * @since S60 5.0
	    * @return Symbian OS error code.
	    */
        TInt CompareOrderedUidArrays();


	    /**
	    * To test CMPXViewPlugin::NewL
	    *
	    * @since S60 5.0
	    * @return Symbian OS error code.
	    */
        TInt CreateCMPXViewPlugin();

	    /**
	    * To test CMPXViewPlugin::~
	    *
	    * @since S60 5.0
	    * @return Symbian OS error code.
	    */
        TInt DeleteCMPXViewPlugin();


        /**
         * From MMPXCustomCommandObserver
         */
        void HandleCustomCommand( 
            const TUid& aUid,
            const TInt aCommand, 
            const TDesC8& aData = KNullDesC8 );

	    /**
       * To test TMPXAttribute::TMPXAttribute()
       * 
       * @since S60 5.0
       * @param aItem Script line containing parameters.
       * @return Symbian OS error code.
       */
      TInt MpxAttDefaultConst(CStifItemParser& aItem);
	        
      /**
       * To test TMPXAttribute::TMPXAttribute(TInt aContentId, TUint aAttributeId)
       *
       * @since S60 5.0
       * @param aItem Script line containing parameters.
       * @return Symbian OS error code.
       */
      TInt MpxAttConst(CStifItemParser& aItem);
		        
      /**
       * To test TMPXAttribute::TMPXAttribute(const TMPXAttribute& aId)
       *
       * @since S60 5.0
       * @param aItem Script line containing parameters.
       * @return Symbian OS error code.
       */
      TInt MpxAttCopyConstAtt(CStifItemParser& aItem);
			        
      /**
       * To test TMPXAttribute::TMPXAttribute(const TMPXAttributeData& aData)
       *
       * @since S60 5.0
       * @param aItem Script line containing parameters.
       * @return Symbian OS error code.
       */
      TInt MpxAttCopyConstAttData(CStifItemParser& aItem);
				        
      /**
       * To test TMPXAttribute::operator=()
       *
       * @since S60 5.0
       * @param aItem Script line containing parameters.
       * @return Symbian OS error code.
       */
      TInt MpxAttOperatorAssignment(CStifItemParser& aItem);
                            
      /**
       * To test TMPXAttribute::operator==()
       *
       * @since S60 5.0
       * @param aItem Script line containing parameters.
       * @return Symbian OS error code.
       */
      TInt MpxAttOperatorEqual(CStifItemParser& aItem);
                             
      /**
       * To test TMPXAttribute::ExternalizeL() and
       *  TMPXAttribute::InternalizeL()
       *
       * @since S60 5.0
       * @param aItem Script line containing parameters.
       * @return Symbian OS error code.
       */
      TInt MpxAttStreamingL(CStifItemParser& aItem);
                              
      /**
       * To test TMPXAttribute::ContentId()
       *
       * @since S60 5.0
       * @param aItem Script line containing parameters.
       * @return Symbian OS error code.
       */
      TInt MpxAttContentId(CStifItemParser& aItem);

      /**
       * To test TMPXAttribute::AttributeId()
       *
       * @since S60 5.0
       * @param aItem Script line containing parameters.
       * @return Symbian OS error code.
       */
      TInt MpxAttAttributeId(CStifItemParser& aItem);

      /**
       * To test TMPXAttribute::Match()
       *
       * @since S60 5.0
       * @param aItem Script line containing parameters.
       * @return Symbian OS error code.
       */
      TInt MpxAttMatch(CStifItemParser& aItem);

      /**
       * To test TMPXAttribute::MatchContentId()
       *
       * @since S60 5.0
       * @param aItem Script line containing parameters.
       * @return Symbian OS error code.
       */
      TInt MpxAttMatchContentId(CStifItemParser& aItem);

      /**
       * To test TMPXAttributeData::operator& (const TMPXAttributeData& aData)
       *
       * @since S60 5.0
       * @param aItem Script line containing parameters.
       * @return Symbian OS error code.
       */
      TInt MpxAttOperatorAndAttData(CStifItemParser& aItem);
                                   
      /**
       * To test TMPXAttributeData::operator& (TUint& aData)
       *
       * @since S60 5.0
       * @param aItem Script line containing parameters.
       * @return Symbian OS error code.
       */
      TInt MpxAttOperatorAndData(CStifItemParser& aItem);
                                    
      /**
       * To test TMPXAttributeData::operator| ()
       *
       * @since S60 5.0
       * @param aItem Script line containing parameters.
       * @return Symbian OS error code.
       */
      TInt MpxAttOperatorOr(CStifItemParser& aItem);
                                     
      /**
       * To test CMPXMedia::NewL()
       *
       * @since S60 5.0
       * @param aItem Script line containing parameters.
       * @return Symbian OS error code.
       */
      TInt MpxMediaNewL(CStifItemParser& aItem);
                                      
      /**
       * To test CMPXMedia::NewL(aSupportedIds)
       *
       * @since S60 5.0
       * @param aItem Script line containing parameters.
       * @return Symbian OS error code.
       */
      TInt MpxMediaNewLSupportedIdsL(CStifItemParser& aItem);

      /**
       * To test CMPXMedia::NewL(aMedia)
       *
       * @since S60 5.0
       * @param aItem Script line containing parameters.
       * @return Symbian OS error code.
       */
      TInt MpxMediaNewLMediaL(CStifItemParser& aItem);
                                      
      /**
       * To test CMPXMedia::NewL(aDataHandle)
       *
       * @since S60 5.0
       * @param aItem Script line containing parameters.
       * @return Symbian OS error code.
       */
      TInt MpxMediaNewLDataHandleL(CStifItemParser& aItem);
                                      
      /**
       * To test CMPXMedia::NewL(aStream)
       *
       * @since S60 5.0
       * @param aItem Script line containing parameters.
       * @return Symbian OS error code.
       */
      TInt MpxMediaNewLStreamL(CStifItemParser& aItem);
                                      
      /**
       * To test CMPXMedia::operator=()
       *
       * @since S60 5.0
       * @param aItem Script line containing parameters.
       * @return Symbian OS error code.
       */
      TInt MpxMediaOperatorAssignment(CStifItemParser& aItem);
                                      
      /**
       * To test CMPXMedia::SupportedIds()
       *
       * @since S60 5.0
       * @param aItem Script line containing parameters.
       * @return Symbian OS error code.
       */
      TInt MpxMediaSupportedIds(CStifItemParser& aItem);

      /**
       * To test CMPXMedia::Attributes()
       *
       * @since S60 5.0
       * @param aItem Script line containing parameters.
       * @return Symbian OS error code.
       */
      TInt MpxMediaAttributes(CStifItemParser& aItem);

      /**
       * To test CMPXMedia::SetTObjectValueL()
       *
       * @since S60 5.0
       * @param aItem Script line containing parameters.
       * @return Symbian OS error code.
       */
      TInt MpxMediaSetTObjectValueL(CStifItemParser& aItem);

      /**
       * To test CMPXMedia::SetCObjectValueL()
       *
       * @since S60 5.0
       * @param aItem Script line containing parameters.
       * @return Symbian OS error code.
       */
      TInt MpxMediaSetCObjectValueL(CStifItemParser& aItem);

      /**
       * To test CMPXMedia::SetNoNewLCObjectL()
       *
       * @since S60 5.0
       * @param aItem Script line containing parameters.
       * @return Symbian OS error code.
       */
      TInt MpxMediaSetNoNewLCObjectL(CStifItemParser& aItem);

      /**
       * To test CMPXMedia::SetTextValueL()
       *
       * @since S60 5.0
       * @param aItem Script line containing parameters.
       * @return Symbian OS error code.
       */
      TInt MpxMediaSetTextValueL(CStifItemParser& aItem);

      /**
       * To test CMPXMedia::IsSupported()
       *
       * @since S60 5.0
       * @param aItem Script line containing parameters.
       * @return Symbian OS error code.
       */
      TInt MpxMediaIsSupported(CStifItemParser& aItem);

      /**
       * To test CMPXMedia::Count()
       *
       * @since S60 5.0
       * @param aItem Script line containing parameters.
       * @return Symbian OS error code.
       */
      TInt MpxMediaCount(CStifItemParser& aItem);

      /**
       * To test CMPXMedia::Attribute()
       *
       * @since S60 5.0
       * @param aItem Script line containing parameters.
       * @return Symbian OS error code.
       */
      TInt MpxMediaAttribute(CStifItemParser& aItem);

      /**
       * To test CMPXMedia::AttributesSet()
       *
       * @since S60 5.0
       * @param aItem Script line containing parameters.
       * @return Symbian OS error code.
       */
      TInt MpxMediaAttributesSet(CStifItemParser& aItem);

      /**
       * To test CMPXMedia::Index()
       *
       * @since S60 5.0
       * @param aItem Script line containing parameters.
       * @return Symbian OS error code.
       */
      TInt MpxMediaIndex(CStifItemParser& aItem);

      /**
       * To test CMPXMedia::Type(aIndex)
       *
       * @since S60 5.0
       * @param aItem Script line containing parameters.
       * @return Symbian OS error code.
       */
      TInt MpxMediaTypeIndex(CStifItemParser& aItem);

      /**
       * To test CMPXMedia::Type(aAttribute)
       *
       * @since S60 5.0
       * @param aItem Script line containing parameters.
       * @return Symbian OS error code.
       */
      TInt MpxMediaTypeAttribute(CStifItemParser& aItem);

      /**
       * To test CMPXMedia::Reset()
       *
       * @since S60 5.0
       * @param aItem Script line containing parameters.
       * @return Symbian OS error code.
       */
      TInt MpxMediaReset(CStifItemParser& aItem);

      /**
       * To test CMPXMedia::Delete(attribute)
       *
       * @since S60 5.0
       * @param aItem Script line containing parameters.
       * @return Symbian OS error code.
       */
      TInt MpxMediaDeleteAttribute(CStifItemParser& aItem);

      /**
       * To test CMPXMedia::Delete(index)
       *
       * @since S60 5.0
       * @param aItem Script line containing parameters.
       * @return Symbian OS error code.
       */
      TInt MpxMediaDeleteIndex(CStifItemParser& aItem);

      /**
       * To test CMPXMedia::ValueText()
       *
       * @since S60 5.0
       * @param aItem Script line containing parameters.
       * @return Symbian OS error code.
       */
      TInt MpxMediaValueText(CStifItemParser& aItem);

      /**
       * To test CMPXMedia::Value()
       *
       * @since S60 5.0
       * @param aItem Script line containing parameters.
       * @return Symbian OS error code.
       */
      TInt MpxMediaValue(CStifItemParser& aItem);

      /**
       * To test CMPXMedia::ValueTObjectL()
       *
       * @since S60 5.0
       * @param aItem Script line containing parameters.
       * @return Symbian OS error code.
       */
      TInt MpxMediaValueTObjectL(CStifItemParser& aItem);

      /**
       * To test CMPXMedia::ValueCObjectL()
       *
       * @since S60 5.0
       * @param aItem Script line containing parameters.
       * @return Symbian OS error code.
       */
      TInt MpxMediaValueCObjectL(CStifItemParser& aItem);

      /**
       * To test CMPXMedia::ValueNoNewLCObjectL()
       *
       * @since S60 5.0
       * @param aItem Script line containing parameters.
       * @return Symbian OS error code.
       */
      TInt MpxMediaValueNoNewLCObjectL(CStifItemParser& aItem);

      /**
       * To test CMPXMedia::ExternalizeL() and
       *  CMPXMedia::InternalizeL()
       *
       * @since S60 5.0
       * @param aItem Script line containing parameters.
       * @return Symbian OS error code.
       */
      TInt MpxMediaStreamingL(CStifItemParser& aItem);
                              
      /**
       * To test CMPXMediaArray::NewL()
       *
       * @since S60 5.0
       * @param aItem Script line containing parameters.
       * @return Symbian OS error code.
       */
      TInt MpxMediaArrayNewL(CStifItemParser& aItem);
                                        
      /**
       * To test CMPXMediaArray::NewL(MediaArray)
       *
       * @since S60 5.0
       * @param aItem Script line containing parameters.
       * @return Symbian OS error code.
       */
      TInt MpxMediaArrayNewLMediaArrayL(CStifItemParser& aItem);
                                        
      /**
       * To test CMPXMediaArray::Count()
       *
       * @since S60 5.0
       * @param aItem Script line containing parameters.
       * @return Symbian OS error code.
       */
      TInt MpxMediaArrayCount(CStifItemParser& aItem);
                   
      /**
       * To test CMPXMediaArray::IMPORT_C CMPXMedia* AtL(TInt aIndex) const;
       *
       * @since S60 5.0
       * @param aItem Script line containing parameters.
       * @return Symbian OS error code.
       */
      TInt MpxMediaArrayAtLByIndexL(CStifItemParser& aItem);
      
      
      /**
       * To test CMPXMediaArray::AppendL(Media*)
       *
       * @since S60 5.0
       * @param aItem Script line containing parameters.
       * @return Symbian OS error code.
       */
      TInt MpxMediaArrayAppendLByPtrL(CStifItemParser& aItem);
                                        
      /**
       * To test CMPXMediaArray::AppendL(Media&)
       *
       * @since S60 5.0
       * @param aItem Script line containing parameters.
       * @return Symbian OS error code.
       */
      TInt MpxMediaArrayAppendLByRefL(CStifItemParser& aItem);
                                        
      /**
       * To test CMPXMediaArray::Reset()
       *
       * @since S60 5.0
       * @param aItem Script line containing parameters.
       * @return Symbian OS error code.
       */
      TInt MpxMediaArrayReset(CStifItemParser& aItem);
                                        
      /**
       * To test CMPXMediaArray::Remove()
       *
       * @since S60 5.0
       * @param aItem Script line containing parameters.
       * @return Symbian OS error code.
       */
      TInt MpxMediaArrayRemove(CStifItemParser& aItem);
                                        
      /**
       * To test CMPXMediaArray::Insert(Media*)
       *
       * @since S60 5.0
       * @param aItem Script line containing parameters.
       * @return Symbian OS error code.
       */
      TInt MpxMediaArrayInsertByPtr(CStifItemParser& aItem);
                                        
      /**
       * To test CMPXMediaArray::Insert(Media&)
       *
       * @since S60 5.0
       * @param aItem Script line containing parameters.
       * @return Symbian OS error code.
       */
      TInt MpxMediaArrayInsertByRef(CStifItemParser& aItem);
                                        
      /**
       * To test CMPXMediaArray::InsertL(Media*)
       *
       * @since S60 5.0
       * @param aItem Script line containing parameters.
       * @return Symbian OS error code.
       */
      TInt MpxMediaArrayInsertLByPtrL(CStifItemParser& aItem);
                                        
      /**
       * To test CMPXMediaArray::InsertL(Media&)
       *
       * @since S60 5.0
       * @param aItem Script line containing parameters.
       * @return Symbian OS error code.
       */
      TInt MpxMediaArrayInsertLByRefL(CStifItemParser& aItem);
                                        
      /**
       * To test CMPXMediaArray::Set()
       *
       * @since S60 5.0
       * @param aItem Script line containing parameters.
       * @return Symbian OS error code.
       */
      TInt MpxMediaArraySet(CStifItemParser& aItem);
                                        
      /**
       * To test CMPXMediaArray::ExternalizeL() and
       *  CMPXMediaArray::InternalizeL()
       *
       * @since S60 5.0
       * @param aItem Script line containing parameters.
       * @return Symbian OS error code.
       */
      TInt MpxMediaArrayStreamingL(CStifItemParser& aItem);
                              
      /**
       * To test TMPXItemId::TMPXItemId()
       *
       * @since S60 5.0
       * @param aItem Script line containing parameters.
       * @return Symbian OS error code.
       */
      TInt MpxItemIdConst(CStifItemParser& aItem);
                                        
      /**
       * 
       *
       * @since S60 5.0
       * @param aItem Script line containing parameters.
       * @return Symbian OS error code.
       */
      TInt MpxItemIdReset(CStifItemParser& aItem);
                                        
      /**
       * To test CMPXCollectionPath::NewL()
       *
       * @since S60 5.0
       * @param aItem Script line containing parameters.
       * @return Symbian OS error code.
       */
      TInt MpxCollPathNewL(CStifItemParser& aItem);
                                        
      /**
       * To test CMPXCollectionPath::NewL(aPath)
       *
       * @since S60 5.0
       * @param aItem Script line containing parameters.
       * @return Symbian OS error code.
       */
      TInt MpxCollPathNewLPathL(CStifItemParser& aItem);
                                        
      /**
       * To test CMPXCollectionPath::NewL(aStream)
       *
       * @since S60 5.0
       * @param aItem Script line containing parameters.
       * @return Symbian OS error code.
       */
      TInt MpxCollPathNewLStreamL(CStifItemParser& aItem);
                                        
      /**
       * To test CMPXCollectionPath::AppendL(aId)
       *
       * @since S60 5.0
       * @param aItem Script line containing parameters.
       * @return Symbian OS error code.
       */
      TInt MpxCollPathAppendLIdL(CStifItemParser& aItem);
                                        
      /**
       * To test CMPXCollectionPath::AppendL(aIds)
       *
       * @since S60 5.0
       * @param aItem Script line containing parameters.
       * @return Symbian OS error code.
       */
      TInt MpxCollPathAppendLIdsL(CStifItemParser& aItem);
                                        
      /**
       * To test CMPXCollectionPath::Levels()
       *
       * @since S60 5.0
       * @param aItem Script line containing parameters.
       * @return Symbian OS error code.
       */
      TInt MpxCollPathLevels(CStifItemParser& aItem);
                                        
      /**
       * To test CMPXCollectionPath::Back()
       *
       * @since S60 5.0
       * @param aItem Script line containing parameters.
       * @return Symbian OS error code.
       */
      TInt MpxCollPathBack(CStifItemParser& aItem);
                                        
      /**
       * To test CMPXCollectionPath::Reset()
       *
       * @since S60 5.0
       * @param aItem Script line containing parameters.
       * @return Symbian OS error code.
       */
      TInt MpxCollPathReset(CStifItemParser& aItem);
                                        
      /**
       * To test CMPXCollectionPath::Index()
       *
       * @since S60 5.0
       * @param aItem Script line containing parameters.
       * @return Symbian OS error code.
       */
      TInt MpxCollPathIndex(CStifItemParser& aItem);
                                        
      /**
       * To test CMPXCollectionPath::Operator++()
       *
       * @since S60 5.0
       * @param aItem Script line containing parameters.
       * @return Symbian OS error code.
       */
      TInt MpxCollPathOperatorPlus(CStifItemParser& aItem);
                                        
      /**
       * To test CMPXCollectionPath::Operator--()
       *
       * @since S60 5.0
       * @param aItem Script line containing parameters.
       * @return Symbian OS error code.
       */
      TInt MpxCollPathOperatorMinus(CStifItemParser& aItem);
                                        
      /**
       * To test CMPXCollectionPath::SetToFirst()
       *
       * @since S60 5.0
       * @param aItem Script line containing parameters.
       * @return Symbian OS error code.
       */
      TInt MpxCollPathSetToFirst(CStifItemParser& aItem);
                                        
      /**
       * To test CMPXCollectionPath::SetToLast()
       *
       * @since S60 5.0
       * @param aItem Script line containing parameters.
       * @return Symbian OS error code.
       */
      TInt MpxCollPathSetToLast(CStifItemParser& aItem);
                                        
      /**
       * To test CMPXCollectionPath::Set(aIndex)
       *
       * @since S60 5.0
       * @param aItem Script line containing parameters.
       * @return Symbian OS error code.
       */
      TInt MpxCollPathSetIndex(CStifItemParser& aItem);
                                        
      /**
       * To test CMPXCollectionPath::Set(aId)
       *
       * @since S60 5.0
       * @param aItem Script line containing parameters.
       * @return Symbian OS error code.
       */
      TInt MpxCollPathSetId(CStifItemParser& aItem);
                                        
      /**
       * To test CMPXCollectionPath::Set(aMode)
       *
       * @since S60 5.0
       * @param aItem Script line containing parameters.
       * @return Symbian OS error code.
       */
      TInt MpxCollPathSetMode(CStifItemParser& aItem);
                                        
      /**
       * To test CMPXCollectionPath::SetL(aAttrs)
       *
       * @since S60 5.0
       * @param aItem Script line containing parameters.
       * @return Symbian OS error code.
       */
      TInt MpxCollPathSetLAttrsL(CStifItemParser& aItem);
                                        
      /**
       * To test CMPXCollectionPath::OpenNextMode()
       *
       * @since S60 5.0
       * @param aItem Script line containing parameters.
       * @return Symbian OS error code.
       */
      TInt MpxCollPathOpenNextMode(CStifItemParser& aItem);
                                        
      /**
       * To test CMPXCollectionPath::OpenPreviousMode()
       *
       * @since S60 5.0
       * @param aItem Script line containing parameters.
       * @return Symbian OS error code.
       */
      TInt MpxCollPathOpenPreviousMode(CStifItemParser& aItem);
                                        
      /**
       * To test CMPXCollectionPath::SelectL(aId)
       *
       * @since S60 5.0
       * @param aItem Script line containing parameters.
       * @return Symbian OS error code.
       */
      TInt MpxCollPathSelectLIdL(CStifItemParser& aItem);
                                        
      /**
       * To test CMPXCollectionPath::SelectL(aIndex)
       *
       * @since S60 5.0
       * @param aItem Script line containing parameters.
       * @return Symbian OS error code.
       */
      TInt MpxCollPathSelectLIndexL(CStifItemParser& aItem);
                                        
      /**
       * To test CMPXCollectionPath::SelectAllL()
       *
       * @since S60 5.0
       * @param aItem Script line containing parameters.
       * @return Symbian OS error code.
       */
      TInt MpxCollPathSelectAllL(CStifItemParser& aItem);
                                        
      /**
       * To test CMPXCollectionPath::Deselect(aId)
       *
       * @since S60 5.0
       * @param aItem Script line containing parameters.
       * @return Symbian OS error code.
       */
      TInt MpxCollPathDeselectId(CStifItemParser& aItem);
                                        
      /**
       * To test CMPXCollectionPath::Deselect(aIndex)
       *
       * @since S60 5.0
       * @param aItem Script line containing parameters.
       * @return Symbian OS error code.
       */
      TInt MpxCollPathDeselectIndex(CStifItemParser& aItem);
                                        
      /**
       * To test CMPXCollectionPath::DeselectAll()
       *
       * @since S60 5.0
       * @param aItem Script line containing parameters.
       * @return Symbian OS error code.
       */
      TInt MpxCollPathDeselectAll(CStifItemParser& aItem);
                                        
      /**
       * To test CMPXCollectionPath::Remove(aId)
       *
       * @since S60 5.0
       * @param aItem Script line containing parameters.
       * @return Symbian OS error code.
       */
      TInt MpxCollPathRemoveId(CStifItemParser& aItem);
                                        
      /**
       * To test CMPXCollectionPath::Remove(aIndex)
       *
       * @since S60 5.0
       * @param aItem Script line containing parameters.
       * @return Symbian OS error code.
       */
      TInt MpxCollPathRemoveIndex(CStifItemParser& aItem);
                                        
      /**
       * To test CMPXCollectionPath::IsSelected(aId)
       *
       * @since S60 5.0
       * @param aItem Script line containing parameters.
       * @return Symbian OS error code.
       */
      TInt MpxCollPathIsSelectedId(CStifItemParser& aItem);
                                        
      /**
       * To test CMPXCollectionPath::IsSelected(aIndex)
       *
       * @since S60 5.0
       * @param aItem Script line containing parameters.
       * @return Symbian OS error code.
       */
      TInt MpxCollPathIsSelectedIndex(CStifItemParser& aItem);

      /**
       * To test CMPXCollectionPath::ClearSelection
       *
       * @since S60 5.0
       * @param aItem Script line containing parameters.
       * @return Symbian OS error code.
       */
      TInt MpxCollPathClearSelection(CStifItemParser& aItem);

      /**
       * To test CMPXCollectionPath::Selection
       *
       * @since S60 5.0
       * @param aItem Script line containing parameters.
       * @return Symbian OS error code.
       */
      TInt MpxCollPathSelection(CStifItemParser& aItem);

      /**
       * To test CMPXCollectionPath::SelectionL
       *
       * @since S60 5.0
       * @param aItem Script line containing parameters.
       * @return Symbian OS error code.
       */
      TInt MpxCollPathSelectionL(CStifItemParser& aItem);

      /**
       * To test CMPXCollectionPath::Id
       *
       * @since S60 5.0
       * @param aItem Script line containing parameters.
       * @return Symbian OS error code.
       */
      TInt MpxCollPathId(CStifItemParser& aItem);

      /**
       * To test CMPXCollectionPath::Count
       *
       * @since S60 5.0
       * @param aItem Script line containing parameters.
       * @return Symbian OS error code.
       */
      TInt MpxCollPathCount(CStifItemParser& aItem);

      /**
       * To test CMPXCollectionPath::IndexOfId
       *
       * @since S60 5.0
       * @param aItem Script line containing parameters.
       * @return Symbian OS error code.
       */
      TInt MpxCollPathIndexOfId(CStifItemParser& aItem);

      /**
       * To test CMPXCollectionPath::IdOfIndex
       *
       * @since S60 5.0
       * @param aItem Script line containing parameters.
       * @return Symbian OS error code.
       */
      TInt MpxCollPathIdOfIndex(CStifItemParser& aItem);

      /**
       * To test CMPXCollectionPath::OpenAttributes
       *
       * @since S60 5.0
       * @param aItem Script line containing parameters.
       * @return Symbian OS error code.
       */
      TInt MpxCollPathOpenAttributes(CStifItemParser& aItem);

      /**
       * To test CMPXCollectionPath::IndexLevel
       *
       * @since S60 5.0
       * @param aItem Script line containing parameters.
       * @return Symbian OS error code.
       */
      TInt MpxCollPathIndexLevel(CStifItemParser& aItem);

      /**
       * To test CMPXCollectionPath::IdLevel
       *
       * @since S60 5.0
       * @param aItem Script line containing parameters.
       * @return Symbian OS error code.
       */
      TInt MpxCollPathIdLevel(CStifItemParser& aItem);

      /**
       * To test CMPXCollectionPath::ExternalizeL() and
       *  CMPXCollectionPath::InternalizeL()
       *
       * @since S60 5.0
       * @param aItem Script line containing parameters.
       * @return Symbian OS error code.
       */
      TInt MpxCollPathStreamingL(CStifItemParser& aItem);
                              
      /**
       * To test CMPXCollectionPath::HandleChange()
       *
       * @since S60 5.0
       * @param aItem Script line containing parameters.
       * @return Symbian OS error code.
       */
      TInt MpxCollPathHandleChange(CStifItemParser& aItem);
                              
      /**
       * To test CMPXClientList::NewL()
       *
       * @since S60 5.0
       * @param aItem Script line containing parameters.
       * @return Symbian OS error code.
       */
      TInt MpxClientListNewL(CStifItemParser& aItem);
                              
      /**
       * To test CMPXClientList::ClientCount()
       *
       * @since S60 5.0
       * @param aItem Script line containing parameters.
       * @return Symbian OS error code.
       */
      TInt MpxClientListClientCount(CStifItemParser& aItem);
                              
      /**
       * To test CMPXClientList::IsClient()
       *
       * @since S60 5.0
       * @param aItem Script line containing parameters.
       * @return Symbian OS error code.
       */
      TInt MpxClientListIsClient(CStifItemParser& aItem);
                              
      /**
       * To test CMPXClientList::Find(aMsgQueue)
       *
       * @since S60 5.0
       * @param aItem Script line containing parameters.
       * @return Symbian OS error code.
       */
      TInt MpxClientListFindByMsg(CStifItemParser& aItem);
                              
      /**
       * To test CMPXClientList::Find(aId)
       *
       * @since S60 5.0
       * @param aItem Script line containing parameters.
       * @return Symbian OS error code.
       */
      TInt MpxClientListFindById(CStifItemParser& aItem);
                              
      /**
       * To test CMPXPluginHandlerBase::SelectPluginL(aPluginUid)
       *
       * @since S60 5.0
       * @param aItem Script line containing parameters.
       * @return Symbian OS error code.
       */
      TInt MpxPluginHandlerBaseSelectPluginLIdL(CStifItemParser& aItem);
                              
      /**
       * To test CMPXPluginHandlerBase::SelectPluginL(aPluginName)
       *
       * @since S60 5.0
       * @param aItem Script line containing parameters.
       * @return Symbian OS error code.
       */
      TInt MpxPluginHandlerBaseSelectPluginLNameL(CStifItemParser& aItem);
                              
      /**
       * To test CMPXPluginHandlerBase::SelectPluginL(aUri, aDataType)
       *
       * @since S60 5.0
       * @param aItem Script line containing parameters.
       * @return Symbian OS error code.
       */
      TInt MpxPluginHandlerBaseSelectPluginLUriL(CStifItemParser& aItem);
                              
      /**
       * To test CMPXPluginHandlerBase::SelectPluginL(aPluginType)
       *
       * @since S60 5.0
       * @param aItem Script line containing parameters.
       * @return Symbian OS error code.
       */
      TInt MpxPluginHandlerBaseSelectPluginLTypeL(CStifItemParser& aItem);
                              
      /**
       * To test CMPXPluginHandlerBase::ClearSelectionCriteria()
       *
       * @since S60 5.0
       * @param aItem Script line containing parameters.
       * @return Symbian OS error code.
       */
      TInt MpxPluginHandlerBaseClearSelectionCriteria(CStifItemParser& aItem);
                              
      /**
       * To test CMPXPluginHandlerBase::PluginName(aType)
       *
       * @since S60 5.0
       * @param aItem Script line containing parameters.
       * @return Symbian OS error code.
       */
      TInt MpxPluginHandlerBasePluginNameType(CStifItemParser& aItem);
                              
      /**
       * To test CMPXPluginHandlerBase::PluginNamesL()
       *
       * @since S60 5.0
       * @param aItem Script line containing parameters.
       * @return Symbian OS error code.
       */
      TInt MpxPluginHandlerBasePluginNamesL(CStifItemParser& aItem);
                              
      /**
       * To test CMPXPluginHandlerBase::SupportedMimeTypesL()
       *
       * @since S60 5.0
       * @param aItem Script line containing parameters.
       * @return Symbian OS error code.
       */
      TInt MpxPluginHandlerBaseSupportedMimeTypesL(CStifItemParser& aItem);
                              
      /**
       * To test CMPXPluginHandlerBase::SupportedExtensionsL()
       *
       * @since S60 5.0
       * @param aItem Script line containing parameters.
       * @return Symbian OS error code.
       */
      TInt MpxPluginHandlerBaseSupportedExtensionsL(CStifItemParser& aItem);
                              
      /**
       * To test CMPXPluginHandlerBase::SupportedSchemasL()
       *
       * @since S60 5.0
       * @param aItem Script line containing parameters.
       * @return Symbian OS error code.
       */
      TInt MpxPluginHandlerBaseSupportedSchemasL(CStifItemParser& aItem);
                              
      /**
       * To test CMPXPluginHandlerBase::SupportedMimeTypesL(aUid)
       *
       * @since S60 5.0
       * @param aItem Script line containing parameters.
       * @return Symbian OS error code.
       */
      TInt MpxPluginHandlerBaseSupportedMimeTypesLUidL(CStifItemParser& aItem);

      /**
       * To test CMPXPluginHandlerBase::SupportedExtensionsL(aUid)
       *
       * @since S60 5.0
       * @param aItem Script line containing parameters.
       * @return Symbian OS error code.
       */
      TInt MpxPluginHandlerBaseSupportedExtensionsLUidL(CStifItemParser& aItem);
                              
      /**
       * To test CMPXPluginHandlerBase::GetPluginTypes()
       *
       * @since S60 5.0
       * @param aItem Script line containing parameters.
       * @return Symbian OS error code.
       */
      TInt MpxPluginHandlerBaseGetPluginTypes(CStifItemParser& aItem);
                              
      /**
       * To test CMPXPluginHandlerBase::GetPluginUids()
       *
       * @since S60 5.0
       * @param aItem Script line containing parameters.
       * @return Symbian OS error code.
       */
      TInt MpxPluginHandlerBaseGetPluginUids(CStifItemParser& aItem);
                              
      /**
       * To test CMPXPluginHandlerBase::GetPluginUidsL()
       *
       * @since S60 5.0
       * @param aItem Script line containing parameters.
       * @return Symbian OS error code.
       */
      TInt MpxPluginHandlerBaseGetPluginUidsL(CStifItemParser& aItem);
                              
      /**
       * To test CMPXPluginHandlerBase::PluginUid()
       *
       * @since S60 5.0
       * @param aItem Script line containing parameters.
       * @return Symbian OS error code.
       */
      TInt MpxPluginHandlerBasePluginUid(CStifItemParser& aItem);
                              
      /**
       * To test CMPXPluginHandlerBase::IndexOf()
       *
       * @since S60 5.0
       * @param aItem Script line containing parameters.
       * @return Symbian OS error code.
       */
      TInt MpxPluginHandlerBaseIndexOf(CStifItemParser& aItem);
                              
      /**
       * To test CMPXPluginHandlerBase::CreatePluginListL()
       *
       * @since S60 5.0
       * @param aItem Script line containing parameters.
       * @return Symbian OS error code.
       */
      TInt MpxPluginHandlerBaseCreatePluginListL(CStifItemParser& aItem);
                              
      /**
       * To test CMPXPluginHandlerBase::FindPlugin()
       *
       * @since S60 5.0
       * @param aItem Script line containing parameters.
       * @return Symbian OS error code.
       */
      TInt MpxPluginHandlerBaseFindPlugin(CStifItemParser& aItem);
      //add function to testmpxuser.h
      TInt RecreateBufferL();
      TInt CompleteWithDllPathOL();
      TInt FindWindowGroupIdL();
      TInt Alloc8ZL();
      //TInt CreateBufferRMessage();
      TInt CreateMPXCollectionType();
      //add function to testmpxmessagemonitor.cpp
      TInt GetNextMessageCMPXMessageMonitorL();
      TInt DoCancelCMPXMessageMonitorL();
      TInt RunErrorCMPXMessageMonitorL();
      //add function to testmpxclientlist.cpp
      TInt MpxClientListDestruction(CStifItemParser&);
      TInt MpxClientListAddClientL();
      TInt MpxClientListAddClientLUsingMode();
      TInt MpxClientListRemoveClient();
      TInt MpxClientListClientProcessList();
      TInt DeleteMpxClientList();
      TInt SendMsgLByMsgCMPXClientList(CStifItemParser& aItem);
      TInt MPXClientListConvertMsgLC(CStifItemParser& );
      TInt SendMsgLIndexAndTMPMsgCMPXClientList(CStifItemParser& aItem);
      TInt MpxClientListSendMsg(CStifItemParser& aItem);
      TInt MpxClientListSendMsgaIndex(CStifItemParser& aItem);
      TInt MpxClientListAddSubscriptionL(CStifItemParser& aItem);
      TInt MpxClientListRemoveSubscriptionL(CStifItemParser& aItem);
      TInt MpxClientListRemoveAllSubscriptionsL(CStifItemParser& aItem);
      
      //add function to testmpxcollectionpath.cpp 
      TInt MpxCollectionpathContainerPathL(CStifItemParser& /*aItem*/); 
      TInt MpxCollectionpathInsertL(CStifItemParser& aItem );     
      TInt MpxCollectionpathItems(CStifItemParser& /*aItem*/);       
      TInt MpxCollectionpathUpdate(CStifItemParser& aItem);      
      TInt MpxCollectionpathDelete(CStifItemParser& /*aItem*/);  
      TInt MpxCollectionpathCollPathSetAttrsL(CStifItemParser& /*aItem*/);
      
      TInt MpxClientListClientMode(CStifItemParser& /*aItem*/);
      //add function to testmpxcollectiontype.cpp 
      TInt TUidMPXCollectionType();
      TInt SendReceiveLTIpcArgsRMPXSessionL();
      TInt CMPXCollectionTypeDelete(CStifItemParser& aItem);
      //add function to testmpxpluginhandlerbase.cpp 
      TInt MpxPluginHandlerBaseSupportUids(CStifItemParser& /*aItem*/);
      //add function to testmpxpluginhandlerbase.cpp  
      TInt MpxMediaCopyL(CStifItemParser& aItem);           
      TInt MpxMediaDeleteL(CStifItemParser& aItem);          
      TInt MpxMediaHeapMemoryInfoL(CStifItemParser& aItem);  
      TInt MpxMediaMergeMediaL(CStifItemParser& aItem);      
      TInt MpxMediaMatchL(CStifItemParser& aItem);           
      TInt MpxMediaSetErrorL(CStifItemParser& aItem);        
      TInt MpxMediaErrorL(CStifItemParser& aItem);           
      TInt MpxMediaExternalizeL(CStifItemParser& aItem);    
      TInt MpxMediaInternalizeL(CStifItemParser& aItem);     
      //add these functions to testmpxpluginhandlerbase.cpp   
      TInt MpxPluginHandlerBaseDestruction(CStifItemParser& aItem);
      TInt MpxPluginHandlerBaseSelectPluginLRFile(CStifItemParser& aItem);
      TInt MpxPluginHandlerBasePluginNameByUid(CStifItemParser& /*aItem*/);  
      TInt MpxPluginHandlerBasePluginFlagsL(CStifItemParser& aItem);        
      TInt MpxPluginHandlerBasePluginType(CStifItemParser& /*aItem*/);         
      TInt MpxPluginHandlerBaseGetSelectionCriteria(CStifItemParser& aItem);  
      TInt MpxPluginHandlerBaseUsePluginTUid(CStifItemParser& aItem);
      TInt MpxPluginHandlerBaseReleasePluginTUid(CStifItemParser& aItem);
      TInt MpxPluginHandlerBaseCreate();
      TInt MpxPluginHandlerBaseSelectPluginLRFile64(CStifItemParser& aItem);
      //added tested functions for protected export functions of CMPXPluginHandlerBase
      /**
       * to test IMPORT_C void PluginsChangedL();
       **/
      TInt MpxPluginHBBasePluginsChangedL(CStifItemParser& aItem);
      TInt MpxPluginHBBaseIsPluginLoaded(CStifItemParser& aItem);
      TInt MpxPluginHBBaseLoadPluginL(CStifItemParser& aItem);
      TInt MpxPluginHBBaseHandlePluginUnload(CStifItemParser& aItem);
      
      
      
      TInt CreateCMPXTaskQueue();
      TInt DeleteCMPXTaskQueue();
      TInt ScheduleNextTaskCMPXTaskQueue();
      TInt ResetCMPXTaskQueue();
      TInt DeleteCMPXActiveTaskQueue();

      
      
      
      
      
      
      
      
      
      
      //add these functions to mpxplugininfo.h
      TInt CMPXPluginInfoDestructor();
      TInt CMPXPluginInfoImplementationUid(CStifItemParser& aItem);
      TInt CMPXPluginInfoFlags(CStifItemParser&);
      //added test functions to test protected export functions
      TInt CMPXPluginInfoProcessTaggedDataExL(CStifItemParser& aItem);
      TInt CMPXPluginInfoExtractIntoArrayL(CStifItemParser& aItem);


      //add test functions for CMPXSubscription of mpxsubscription.h
      TInt CMPXSubscriptionNewL(CStifItemParser& aItem);
      TInt CMPXSubscriptionNewLWithMediaArrayL(CStifItemParser& aItem);
      TInt CMPXSubscriptionAddItemL(CStifItemParser& aItem);
      TInt CMPXSubscriptionItemsL(CStifItemParser& aItem);
      TInt CMPXSubscriptionIsEqualOperator(CStifItemParser& aItem);

      TInt ReconnectRMPXSessionL();
      private:
        // reference to TestModuleIf
        CTestModuleIf& iTestModuleIf;           
        // Active object with a timer to timeout the test case
        CSimpleTimeout * iTimeoutController;
        // Indicates if the test case use a normal exit reason
        TBool iNormalExitReason;    
        // List of expected events
        RArray<TMPXTestExpectedEvent> iExpectedEvents;
        // List of notification event (by the callback)
        RArray<TMPXTestExpectedEvent> iOcurredEvents;


        CMPXPluginMonitor*   iPluginMonitor;
        CMPXCenRepWatcher*   iCenRepWatcher;
        CMPXParameter*       iParameter;
        CMPXCollectionType*  iType;
        CMPXPSKeyWatcher*    iPSWatcher;
        CMPXDrmMediaUtility* iDrmMediaUtility;
        CMPXMessageQueue*    iMessageQueue;        
        RArray<TMPXAttribute> iMPXAttArray;
        RArray<TMPXItemId>  iMPXItemIdArray;
        CMPXMediaArray*     iMPXMediaArray;
        CMPXCollectionPath* iMPXCollectionPath;
        TUid                iCollectionPathUid;
        CMPXClientList*     iClientList;
        CMPXMessageMonitor* iMsgMonitor;
        RMPXSession         iMessageSession;
        CMPXViewPlugin*     iViewPlugin;
        CMPXPluginInfo*     iPluginInfo;
        CMPXSubscription*  iSubscription;

    	
};

#endif      // COMMONTESTCLASS_H

// End of File
