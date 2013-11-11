/*
* Copyright (c) 2009 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description: STIF testclass declaration
*
*/

#ifndef OPENMAXALTESTMODULE_H
#define OPENMAXALTESTMODULE_H

//  INCLUDES
#include <StifLogger.h>
#include <TestScripterInternal.h>
#include <StifTestModule.h>
#include <TestclassAssert.h>
#include <OpenMAXAL.h>
#include <xanokialinearvolumeitf.h>
#include <xanokiavolumeextitf.h>
#include <e32msgqueue.h>
#include "TimeoutController.h"
#include <w32std.h>
#include "profileutilmacro.h"

// CONSTANTS
//const ?type ?constant_var = ?constant;
// CONSTANTS
const TInt KErrBadTestParameter= -1000;         // Error on configuration file

const TInt KErrEventPending =-2000;             // Timeout and not callback 
const TInt KErrCallbackErrorCode = -2001;       // Callback return error code
const TInt KErrUnexpectedValue =-2002;          // Unexpected value at setting    
const TInt KErrTimeoutController = -2007;       // Unexpected notification

const TInt KShortTimeout = 1000;
const TInt KMediumTimeout = 4000;
const TInt KLongTimeout = 100000;


_LIT( KMsgBadTestParameters, "[Error] No valid test case parameters");
// MACROS
#define RET_ERR_IF_ERR(err) if (err!=KErrNone) return err;
//#define ?macro ?macro_def
#define TEST_CLASS_VERSION_MAJOR 0
#define TEST_CLASS_VERSION_MINOR 0
#define TEST_CLASS_VERSION_BUILD 0

// Logging path
_LIT( KOpenMAXALTestModuleLogPath, "\\logs\\testframework\\OpenMAXALTestModule\\" ); 
// Log file
_LIT( KOpenMAXALTestModuleLogFile, "OpenMAXALTestModule.txt" ); 
_LIT( KOpenMAXALTestModuleLogFileWithTitle, "OpenMAXALTestModule_[%S].txt" );

// FUNCTION PROTOTYPES
//?type ?function_name(?arg_list);

// FORWARD DECLARATIONS
//class ?FORWARD_CLASSNAME;
class COpenMAXALTestModule;

// DATA TYPES
#define MAX_NUMBER_OPTIONS 2
#define MAX_NUMBER_INTERFACES 20
//enum ?declaration
//typedef ?declaration
//extern ?data_type;

// CLASS DECLARATION

/**
*  COpenMAXALTestModule test class for STIF Test Framework TestScripter.
*  ?other_description_lines
*
*  @lib ?library
*  @since ?Series60_version
*/
NONSHARABLE_CLASS(COpenMAXALTestModule) : public CScriptBase,
                                            public MTimeoutObserver
    {
    public:  // Constructors and destructor

        /**
        * Two-phased constructor.
        */
        static COpenMAXALTestModule* NewL( CTestModuleIf& aTestModuleIf );

        /**
        * Destructor.
        */
        virtual ~COpenMAXALTestModule();

        virtual void HandlePlayItfCallback( XAPlayItf caller, XAuint32 event );
        virtual void HandleRadioItfCallback(XARadioItf caller, XAuint32 event );
        virtual void HandleObjectCallback(XAObjectItf caller,XAuint32 event,
                                XAresult result,XAuint32 param, void * pInterface);
        virtual void HandlePrefetchStatusItfCallback( XAPrefetchStatusItf caller, XAuint32 event );
        virtual void HandlePrefetchStatusItfTestEventCallback( XAPrefetchStatusItf caller, XAuint32 event );
        virtual void HandleStreamInfoItfCallback( XAStreamInformationItf caller, XAuint32 event, XAuint32 streamIndex );
     
        enum TOMXExpectedEvent
            {
            EEOSReceived = 1
            };
    
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

        // From MTimeoutObserver

        /**
        * @since ?Series60_version
        * @param none
        * Review if all the expected events have ocurred once the time is over
        */
        void HandleTimeout(TInt error);

        /**
        * Verify that the event was expected, removes it from the list
        * Signal the TestScripter with the returned error code
        * @since ?Series60_version
        */
        void ProcessEvent(TOMXExpectedEvent aEvent, TInt aError);        
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
        COpenMAXALTestModule( CTestModuleIf& aTestModuleIf );

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
        * Set an event as expected and set default timeout
        * @since ?Series60_version
        */
        void AddExpectedEvent(TOMXExpectedEvent event, TInt ms);


        /**
        * Unset an event as expected
        * @since ?Series60_version
        */
        TBool RemoveExpectedEvent(TOMXExpectedEvent event);


        /**
        * @since ?Series60_version
        * @param none
        * Removes all expected events
        */
        void RemoveAllExpectedEvents();



        /**
        * Maps a event with a descriptor with its name
        * @since ?Series60_version
        */
        TPtrC EventName( TInt aKey );

        /*
        * Test methods are listed below.
        */

        /**
        * Sets a timeout different since the default
        * @since Series60_3_2
        * @param aItem Script line containing parameters.
        * @return Symbian OS error code.
        */
        TInt SetTimeout( CStifItemParser& aItem );

        /**
        *
        * @since Series60_3_2
        * @param aItem Script line containing parameters.
        * @return Symbian OS error code.
        */
        TInt SetExpectedEvents( CStifItemParser& aItem );

        /**
        *
        * @since Series60_3_2
        * @param aItem Script line containing parameters.
        * @return Symbian OS error code.
        */
        TInt SetAllowedPanic( CStifItemParser& aItem );
        
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
        
        virtual TInt al_SetDataSource( CStifItemParser& aItem );
        virtual TInt al_SetDataSink( CStifItemParser& aItem );        
        virtual TInt al_SetDataLocator( CStifItemParser& aItem );        
        virtual TInt al_SetDataFormat( CStifItemParser& aItem );        
        virtual TInt al_CreateWindow( CStifItemParser& aItem );
        virtual TInt al_DeleteWindow( CStifItemParser& aItem );        
        virtual TInt CheckForNull(CStifItemParser& aItem, void*& paramPtr);
        virtual TInt CheckForNullParam(CStifItemParser& aItem, void*& paramPtr);
        
        virtual TInt al_createEngine( CStifItemParser& aItem );
        virtual TInt al_queryNumSupportedEngineInterfaces( CStifItemParser& aItem );
        virtual TInt al_querySupportedEngineInterfaces( CStifItemParser& aItem );
        virtual TInt al_objitf_Realize( CStifItemParser& aItem );
        virtual TInt al_objitf_Resume( CStifItemParser& aItem );
        virtual TInt al_objitf_GetState( CStifItemParser& aItem );
        virtual TInt al_objitf_GetInterface( CStifItemParser& aItem );
        virtual TInt al_objitf_RegisterCallback( CStifItemParser& aItem );
        virtual TInt al_objitf_AbortAsyncOperation( CStifItemParser& aItem );
        virtual TInt al_objitf_Destroy( CStifItemParser& aItem );
        virtual TInt al_objitf_SetPriority( CStifItemParser& aItem );
        virtual TInt al_objitf_GetPriority( CStifItemParser& aItem );
        virtual TInt al_objitf_SetLossOfControlInterfaces( CStifItemParser& aItem );
        
        virtual TInt al_dimitf_AddInterface( CStifItemParser& aItem );
        virtual TInt al_dimitf_RemoveInterface( CStifItemParser& aItem );
        virtual TInt al_dimitf_ResumeInterface( CStifItemParser& aItem );
        virtual TInt al_dimitf_RegisterCallback( CStifItemParser& aItem );
        
        
        virtual TInt al_engitf_CreateCameraDevice( CStifItemParser& aItem );
        virtual TInt al_engitf_CreateRadioDevice( CStifItemParser& aItem );
        virtual TInt al_engitf_CreateLEDDevice( CStifItemParser& aItem );
        virtual TInt al_engitf_CreateVibraDevice( CStifItemParser& aItem );
        virtual TInt al_engitf_CreateMediaPlayer( CStifItemParser& aItem );
        virtual TInt al_engitf_CreateMediaRecorder( CStifItemParser& aItem );
        virtual TInt al_engitf_CreateOutputMix( CStifItemParser& aItem );
        virtual TInt al_engitf_CreateMetadataExtractor( CStifItemParser& aItem );
        virtual TInt al_engitf_CreateExtensionObject( CStifItemParser& aItem );
        virtual TInt al_engitf_GetImplementationInfo( CStifItemParser& aItem );
        virtual TInt al_engitf_QuerySupportedProfiles( CStifItemParser& aItem );
        virtual TInt al_engitf_QueryNumSupportedInterfaces( CStifItemParser& aItem );
        virtual TInt al_engitf_QuerySupportedInterfaces( CStifItemParser& aItem );
        virtual TInt al_engitf_QueryNumSupportedExtension( CStifItemParser& aItem );
        virtual TInt al_engitf_QuerySupportedExtension( CStifItemParser& aItem );
        virtual TInt al_engitf_IsExtensionSupported( CStifItemParser& aItem );
        virtual TInt al_engitf_QueryLEDCapabilities( CStifItemParser& aItem );
        virtual TInt al_engitf_QueryVibraCapabilities( CStifItemParser& aItem );
        virtual TInt al_audioiodevcapitf_GetAvailableAudioInputs( CStifItemParser& aItem );
        virtual TInt al_audioiodevcapitf_QueryAudioInputCapabilities( CStifItemParser& aItem );
        virtual TInt al_audioiodevcapitf_RegisterAvailableAudioInputsChangedCallback( CStifItemParser& aItem );
        virtual TInt al_audioiodevcapitf_GetAvailableAudioOutputs( CStifItemParser& aItem );
        virtual TInt al_audioiodevcapitf_QueryAudioOutputCapabilities( CStifItemParser& aItem );
        virtual TInt al_audioiodevcapitf_RegisterAvailableAudioOutputsChangedCallback( CStifItemParser& aItem );
        virtual TInt al_audioiodevcapitf_RegisterDefaultDeviceIDMapChangedCallback( CStifItemParser& aItem );
        virtual TInt al_audioiodevcapitf_GetAssociatedAudioInputs( CStifItemParser& aItem );
        virtual TInt al_audioiodevcapitf_GetAssociatedAudioOutputs( CStifItemParser& aItem );
        virtual TInt al_audioiodevcapitf_GetDefaultAudioDevices( CStifItemParser& aItem );
        virtual TInt al_audioiodevcapitf_QuerySampleFormatsSupported( CStifItemParser& aItem );
        virtual TInt al_audioencodercapitf_GetAudioEncoders( CStifItemParser& aItem );
        virtual TInt al_audioencodercapitf_GetAudioEncoderCapabilities( CStifItemParser& aItem );
        virtual TInt al_recorditf_SetRecordState( CStifItemParser& aItem );
        virtual TInt al_recorditf_GetRecordState( CStifItemParser& aItem );
        virtual TInt al_recorditf_SetDurationLimit( CStifItemParser& aItem );
        virtual TInt al_recorditf_GetPosition( CStifItemParser& aItem );
        virtual TInt al_recorditf_RegisterCallback( CStifItemParser& aItem );
        virtual TInt al_recorditf_SetCallbackEventsMask( CStifItemParser& aItem );
        virtual TInt al_recorditf_GetCallbackEventsMask( CStifItemParser& aItem );
        virtual TInt al_recorditf_SetMarkerPosition( CStifItemParser& aItem );
        virtual TInt al_recorditf_ClearMarkerPosition( CStifItemParser& aItem );
        virtual TInt al_recorditf_GetMarkerPosition( CStifItemParser& aItem );
        virtual TInt al_recorditf_SetPositionUpdatePeriod( CStifItemParser& aItem );
        virtual TInt al_recorditf_GetPositionUpdatePeriod( CStifItemParser& aItem );
        virtual TInt al_audioencoderitf_SetEncoderSettings( CStifItemParser& aItem );
        virtual TInt al_audioencoderitf_GetEncoderSettings( CStifItemParser& aItem );
        virtual TInt al_metadatainsertionitf_CreateChildNode( CStifItemParser& aItem );
        virtual TInt al_metadatainsertionitf_GetSupportedKeysCount( CStifItemParser& aItem );
        virtual TInt al_metadatainsertionitf_GetKeySize( CStifItemParser& aItem );
        virtual TInt al_metadatainsertionitf_GetKey( CStifItemParser& aItem );
        virtual TInt al_metadatainsertionitf_GetFreeKeysEncoding( CStifItemParser& aItem );
        virtual TInt al_metadatainsertionitf_InsertMetadataItem( CStifItemParser& aItem );
        virtual TInt al_metadatainsertionitf_RegisterCallback( CStifItemParser& aItem );
        
        virtual TInt al_playitf_SetPlayState( CStifItemParser& aItem );
        virtual TInt al_playitf_GetPlayState( CStifItemParser& aItem );
        virtual TInt al_playitf_GetDurationNullParam( CStifItemParser& aItem );
        virtual TInt al_playitf_GetDuration( CStifItemParser& aItem );
        virtual TInt al_playitf_GetPositionNullParam( CStifItemParser& aItem );
        virtual TInt al_playitf_GetPosition( CStifItemParser& aItem );
        virtual TInt al_playitf_RegisterCallback( CStifItemParser& aItem );
        virtual TInt al_playitf_SetCallbackEventMask( CStifItemParser& aItem );
        virtual TInt al_playitf_GetCallbackEventMaskNullParam( CStifItemParser& aItem );
        virtual TInt al_playitf_GetCallbackEventMask( CStifItemParser& aItem );
        virtual TInt al_playitf_SetMarkerPosition( CStifItemParser& aItem );
        virtual TInt al_playitf_ClearMarkerPosition( CStifItemParser& aItem );
        virtual TInt al_playitf_GetMarkerPositionNullParam( CStifItemParser& aItem );
        virtual TInt al_playitf_GetMarkerPosition( CStifItemParser& aItem );
        virtual TInt al_playitf_SetPositionUpdatePeriod( CStifItemParser& aItem );
        virtual TInt al_playitf_GetPositionUpdatePeriodNullParam( CStifItemParser& aItem );
        virtual TInt al_playitf_GetPositionUpdatePeriod( CStifItemParser& aItem );

        virtual TInt al_seekitf_SetPosition( CStifItemParser& aItem );
        virtual TInt al_seekitf_SetLoop( CStifItemParser& aItem );
        virtual TInt al_seekitf_GetLoop( CStifItemParser& aItem );
        
        virtual TInt al_dynsrcitf_SetSource( CStifItemParser& aItem );
        
        virtual TInt al_radioitf_SetFreqRange(CStifItemParser& aItem );
        virtual TInt al_radioitf_GetFreqRange(CStifItemParser& aItem );
        virtual TInt al_radioitf_IsFreqRangeSupported(CStifItemParser& aItem );
        virtual TInt al_radioitf_GetFreqRangeProperties(CStifItemParser& aItem );
        virtual TInt al_radioitf_SetFrequency(CStifItemParser& aItem );
        virtual TInt al_radioitf_CancelSetFrequency(CStifItemParser& aItem );
        virtual TInt al_radioitf_GetFrequency(CStifItemParser& aItem );
        virtual TInt al_radioitf_SetSquelch(CStifItemParser& aItem );
        virtual TInt al_radioitf_GetSquelch(CStifItemParser& aItem );
        virtual TInt al_radioitf_SetStereoMode(CStifItemParser& aItem );
        virtual TInt al_radioitf_GetStereoMode(CStifItemParser& aItem );
        virtual TInt al_radioitf_GetSignalStrength(CStifItemParser& aItem );
        virtual TInt al_radioitf_Seek(CStifItemParser& aItem );
        virtual TInt al_radioitf_StopSeeking(CStifItemParser& aItem ); 
       	virtual TInt al_radioitf_RegisterRadioCallback(CStifItemParser& aItem ); 
        virtual TInt al_radioitf_GetNumberOfPresets( CStifItemParser& aItem);
        virtual TInt al_radioitf_SetPreset( CStifItemParser& aItem);
        virtual TInt al_radioitf_GetPreset( CStifItemParser& aItem);

        virtual TInt al_strminfoitf_QueryMediaContainerInformation( CStifItemParser& aItem );
        virtual TInt al_strminfoitf_QueryStreamType( CStifItemParser& aItem );
        virtual TInt al_strminfoitf_QueryStreamInformation( CStifItemParser& aItem );
        virtual TInt al_strminfoitf_QueryStreamName( CStifItemParser& aItem );
        virtual TInt al_strminfoitf_RegisterStreamChangeCallback( CStifItemParser& aItem );
        virtual TInt al_strminfoitf_QueryActiveStreams( CStifItemParser& aItem );
        virtual TInt al_strminfoitf_SetActiveStream( CStifItemParser& aItem );        
        virtual TInt al_strminfoitf_SetStreamInfoIndex(CStifItemParser& aItem);
        
        virtual TInt al_volumeitf_SetVolumeLevel( CStifItemParser& aItem );
        virtual TInt al_volumeitf_GetVolumeLevel( CStifItemParser& aItem );
        virtual TInt al_volumeitf_GetMaxVolumeLevel( CStifItemParser& aItem );
        virtual TInt al_volumeitf_SetMute( CStifItemParser& aItem );
        virtual TInt al_volumeitf_GetMute( CStifItemParser& aItem );
        virtual TInt al_volumeitf_EnableStereoPosition( CStifItemParser& aItem );
        virtual TInt al_volumeitf_IsEnabledStereoPosition( CStifItemParser& aItem );
        virtual TInt al_volumeitf_SetStereoPosition( CStifItemParser& aItem );
        virtual TInt al_volumeitf_GetStereoPosition( CStifItemParser& aItem );

        virtual TInt al_nokiavolumeextitf_SetVolumeLevel( CStifItemParser& aItem ); 
        virtual TInt al_nokiavolumeextitf_GetVolumeLevel( CStifItemParser& aItem ); 
        virtual TInt al_nokiavolumeextitf_GetMaxVolumeLevel( CStifItemParser& aItem ); 
        virtual TInt al_nokiavolumeextitf_SetMute( CStifItemParser& aItem ); 
        virtual TInt al_nokiavolumeextitf_GetMute( CStifItemParser& aItem ); 
        virtual TInt al_nokiavolumeextitf_EnableStereoPosition( CStifItemParser& aItem ); 
        virtual TInt al_nokiavolumeextitf_IsEnabledStereoPosition( CStifItemParser& aItem ); 
        virtual TInt al_nokiavolumeextitf_SetStereoPosition( CStifItemParser& aItem ); 
        virtual TInt al_nokiavolumeextitf_GetStereoPosition( CStifItemParser& aItem ); 
        virtual TInt al_nokiavolumeextitf_RegisterVolumeCallback( CStifItemParser& aItem ); 
        virtual TInt al_nokiavolumeextitf_SetCallbackEventsMask( CStifItemParser& aItem ); 
        virtual TInt al_nokiavolumeextitf_GetCallbackEventsMask( CStifItemParser& aItem ); 

        virtual TInt al_nokialinearvolumeitf_SetVolumeLevel( CStifItemParser& aItem );
        virtual TInt al_nokialinearvolumeitf_GetVolumeLevel( CStifItemParser& aItem );
        virtual TInt al_nokialinearvolumeitf_SetGetVolumeLevel( CStifItemParser& aItem );        
        virtual TInt al_nokialinearvolumeitf_GetStepCount( CStifItemParser& aItem );
        virtual TInt al_nokialinearvolumeitf_RegisterVolumeCallback( CStifItemParser& aItem );
        virtual TInt al_nokialinearvolumeitf_SetCallbackEventsMask( CStifItemParser& aItem );
        virtual TInt al_nokialinearvolumeitf_GetCallbackEventsMask( CStifItemParser& aItem );

        virtual TInt al_metadataextractionitf_GetItemCount( CStifItemParser& aItem );
		virtual TInt al_metadataextractionitf_GetKeySize( CStifItemParser& aItem );
		virtual TInt al_metadataextractionitf_GetKey( CStifItemParser& aItem );
		virtual TInt al_metadataextractionitf_GetValueSize( CStifItemParser& aItem );
		virtual TInt al_metadataextractionitf_GetValue( CStifItemParser& aItem );
		virtual TInt al_metadataextractionitf_AddKeyFilter( CStifItemParser& aItem );
		virtual TInt al_metadataextractionitf_ClearKeyFilter( CStifItemParser& aItem );
		virtual TInt al_metadataextractionitf_GetCoverArt( CStifItemParser& aItem );
		
		virtual TInt al_playbackrateitf_SetRate( CStifItemParser& aItem );
		virtual TInt al_playbackrateitf_GetRate( CStifItemParser& aItem );
		virtual TInt al_playbackrateitf_SetPropertyConstraints( CStifItemParser& aItem );
		virtual TInt al_playbackrateitf_GetProperties( CStifItemParser& aItem );
		virtual TInt al_playbackrateitf_GetCapabilitiesOfRate( CStifItemParser& aItem );
		virtual TInt al_playbackrateitf_GetRateRange( CStifItemParser& aItem );

        virtual TInt al_prefetchstatusitf_GetFillLevel( CStifItemParser& aItem ) ;
        virtual TInt al_prefetchstatusitf_GetPrefetchStatus( CStifItemParser& aItem );
        virtual TInt al_prefetchstatusitf_RegisterCallback( CStifItemParser& aItem );
        virtual TInt al_prefetchstatusitf_RegisterTestEventCallback( CStifItemParser& aItem );
        virtual TInt al_prefetchstatusitf_SetCallbackEventMask( CStifItemParser& aItem );
        virtual TInt al_prefetchstatusitf_GetCallbackEventMaskNullParam( CStifItemParser& aItem );
        virtual TInt al_prefetchstatusitf_GetCallbackEventMask( CStifItemParser& aItem );
        virtual TInt al_prefetchstatusitf_SetFillUpdatePeriod( CStifItemParser& aItem );
        virtual TInt al_prefetchstatusitf_GetFillUpdatePeriodNullParam( CStifItemParser& aItem );
        virtual TInt al_prefetchstatusitf_GetFillUpdatePeriod( CStifItemParser& aItem );
        virtual TInt al_prefetchstatusitf_TestEvent( CStifItemParser& aItem );
		
		virtual TInt al_videoppitf_SetRotation( CStifItemParser& aItem );
		virtual TInt al_videoppitf_IsArbitraryRotationSupported( CStifItemParser& aItem );
		virtual TInt al_videoppitf_SetScaleOptions( CStifItemParser& aItem );
		virtual TInt al_videoppitf_SetSourceRectangle( CStifItemParser& aItem );
		virtual TInt al_videoppitf_SetDestinationRectangle( CStifItemParser& aItem );
		virtual TInt al_videoppitf_SetMirror( CStifItemParser& aItem );
		virtual TInt al_videoppitf_Commit( CStifItemParser& aItem );

        /**
         * Method used to log version of test class
         */
        void SendTestClassVersion();

        //ADD NEW METHOD DEC HERE
        TInt validateAudioCodecDescriptorAtIndex(
                XAuint32 aAudioCodecId,
                XAAudioCodecDescriptor& aDesc,
                CStifItemParser& aItem );
        //[TestMethods] - Do not remove

    public:     // Data
        // ?one_line_short_description_of_data
        //?data_declaration;
        enum TObjectType
            {
            EEngine = 1,
            EMediaRecorder,
/*
            ERecordItf,
            EEngineItf
*/
            EMediaPlayer = 4
            };

    protected:  // Data
        TInt MapErr(XAresult xa_res);
        XAInterfaceID MapInterface(TInt interface);
        XAObjectItf COpenMAXALTestModule::GetObject(TInt object);
        TInt StoreInterface(TInt interface, void* id);
        xaObjectCallback GetCallbackFunc(TInt object);
        // ?one_line_short_description_of_data
        //?data_declaration;

    private:    // Data
        
        // reference to TestModuleIf
        CTestModuleIf& iTestModuleIf;
        // Active object with a timer to timeout the test case
        CSimpleTimeout * iTimeoutController;

        // Indicates if the test case use a normal exit reason
        TBool iNormalExitReason;

        // List of expected events
        RArray<TOMXExpectedEvent> iExpectedEvents;

        // List of notification event (by the callback)
        RArray<TOMXExpectedEvent> iOcurredEvents;
        
        
        XAboolean     required[MAX_NUMBER_INTERFACES]; 
        XAInterfaceID iidArray[MAX_NUMBER_INTERFACES]; 
        XAEngineOption EngineOption[MAX_NUMBER_OPTIONS];
                                    
        XAObjectItf m_EOEngine;
        XAObjectItf m_MORecorder;
        XAObjectItf m_MOPlayer;
        XARecordItf m_RecordItf;
        XAObjectItf m_MOOutputMix;
        XAObjectItf m_MOMetadataExtractor;
        XAObjectItf m_MOExtnObject;
        XAObjectItf m_MOVibraObject;
        XAObjectItf m_MOLEDObject;
        XAObjectItf m_MORadioObject;
        XAObjectItf m_MOCameraObject;

        XAPlayItf m_PlayItf;
        XASeekItf m_SeekItf;
        XARadioItf m_RadioItf;
        XADynamicSourceItf m_DynSrcItf;
        XAPlaybackRateItf m_PlaybackRateItf;
        XAVideoPostProcessingItf m_VideoPP;
        
        XAVolumeItf m_VolumeItf;
        XANokiaLinearVolumeItf  m_NokiaLinearVolumeItf;
        XANokiaVolumeExtItf m_NokiaVolumeExtItf;
        
        XADynamicInterfaceManagementItf m_DIMItf;
        XAStreamInformationItf m_StrInfoItf;
        
        XAEngineItf m_EngineItf;
        XAAudioIODeviceCapabilitiesItf m_AIODevCapItf;
        XAAudioEncoderCapabilitiesItf m_AEncCapItf;
        XAAudioEncoderItf m_AudEncItf;
        XAMetadataInsertionItf m_MetadataInsertionItf;
        XAMetadataExtractionItf m_MetadataExtractionItf;
        XAPrefetchStatusItf m_PrefetchStatusItf;
        
        /*Audio Source*/
        XADataSource m_AudioSource;
        XADataLocator_IODevice  m_SrcIODevice;
        XADataLocator_IODevice  m_SinkIODevice;

        /*Image/Video Source*/
        XADataSource m_VideoSource;
       
        /*MetadataExtractor Source*/
        XADataSource m_MOMetadataExtractorSource;

        /*Dynamic Source*/
        XADataSource m_DynamicSource;
        
        /*Data Sink*/
        XADataSink m_DataSink;
        XADataSink m_AudioSink;
        XADataSink m_VideoSink;
        XADataLocator_NativeDisplay m_NativeDisplay;
        XADataSink m_LEDSink;
        XADataSink m_VibraSink;
        
        
        XADataLocator_URI m_Uri;
        HBufC8* m_URIName;
        
        XADataFormat_MIME m_Mime;
        HBufC8* m_MimeType;

        RWsSession iRwSession;
        RWindowGroup iRwGroup;
        RWindow iRWindow;
        CWsScreenDevice* iDevice;
        //prefetch status variables
        TInt iStatusEventCount;
        TInt iFillEventCount;
        TInt iFillUpdateIncrement;
        TInt iLastFillIncrement;
        TBool iPrefetchEventTesting;
        TBool iPrefetchEventError;
		
        XAAudioEncoderSettings m_audioensettings;
		
		TInt iStreamInfoItfIndex; //expected streamIndex for callback event
        // ?one_line_short_description_of_data
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

#endif      // OPENMAXALTESTMODULE_H

// End of File
