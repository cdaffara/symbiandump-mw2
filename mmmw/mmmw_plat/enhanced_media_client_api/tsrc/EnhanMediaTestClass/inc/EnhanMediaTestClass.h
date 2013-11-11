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
* Description:  EMC Test App
*
*/



#ifndef CEnhanMediaTestClass_H
#define CEnhanMediaTestClass_H

//  INCLUDES
#include <StifLogger.h>
#include <TestScripterInternal.h>
#include <StifTestModule.h>

#include <e32svr.h>
#include <e32base.h>
#include <e32cons.h>
//#include <customInterfaceUtility.h>
//#include <SoundDevice.h>
//#include <AacDecoderConfig.h>
//#include <MdaAudioOutputStream.h>
//#include <Mda\Common\Audio.h>
#include <StreamControl.h>
#include <SourceControl.h>
#include <ControlObserver.h>
#include <Events.h>
#include <DataBufferSource.h>
#include <FileSource.h>
#include <DataBuffer.h>
#include <MMControlFactory.h>
#include <SinkControl.h>
#include <badesca.h>
#include <MimeTypes.h>
#include <DescriptorSource.h>
#include <MultimediaDataSource.h>
#include <EffectControl.h>
#include <VolumeControl.h>
#include <BalanceControl.h>

#include <EqualizerControl.h>
#include <ReverbControl.h>
#include "CEMCConstants.h"

#include<StereoWideningControl.h>
#include<LoudnessControl.h>
#include<BassBoostControl.h>
#include<RoomLevelControl.h>
#include<DistanceAttenuationControl.h>
#include<ListenerLocationControl.h>
#include<SourceLocationControl.h>
#include<ListenerDopplerControl.h>
#include<SourceDopplerControl.h>
#include<SourceOrientationControl.h>
#include<ListenerOrientationControl.h>
//#include <CacheDataSource.h>
//#include <DataSourceConfigIntfcImpl.h>

#include "CEMCConstants.h"

using multimedia::MStreamControl;
using multimedia::MControlObserver;
using multimedia::MDataBufferSource;
using multimedia::MDescriptorSource;
using multimedia::MDataBuffer;
using multimedia::MControl;
using multimedia::CMultimediaFactory;
using multimedia::MSinkControl;
using multimedia::MFileSource;
using multimedia::MSourceControl;
using multimedia::MVolumeControl;
using multimedia::MBalanceControl;
using multimedia::MEffectControl;
using multimedia::MEqualizerControl;
using multimedia::MReverbControl;

using multimedia::MStereoWideningControl;
using multimedia::MLoudnessControl;
using multimedia::MBassBoostControl;
using multimedia::MRoomLevelControl;
using multimedia::MDistanceAttenuationControl;
using multimedia::MListenerLocationControl;
using multimedia::MSourceLocationControl;
using multimedia::MListenerDopplerControl;
using multimedia::MSourceDopplerControl;
using multimedia::MSourceOrientationControl;
using multimedia::MListenerOrientationControl;
#include "TimeoutController.h"



// FORWARD DECLARATIONS
class CEnhanMediaTestClass;
class CDRMConfigIntfc;

// CLASS DECLARATION

/**
*  CRadioUtilityTestClass test class for STIF Test Framework TestScripter.
*  ?other_description_lines
*
*  @lib ?library
*  @since Series60_3_2
*/
NONSHARABLE_CLASS(CEnhanMediaTestClass) : public CScriptBase,
										  public MTimeoutObserver
										  ,public MControlObserver
											//public MDevSoundObserver,
											//public MMdaAudioOutputStreamCallback
										//	public MRadioFmTunerObserver,
										//	public MRadioPlayerObserver,
										//	public MRadioRdsObserver
											/*public MRadioPresetObserver*/
    {
    public:  // Constructors and destructor
        /**
        * Two-phased constructor.
        */
        static CEnhanMediaTestClass* NewL( CTestModuleIf& aTestModuleIf );

        /**
        * Destructor.
        */
        virtual ~CEnhanMediaTestClass();

        void Event( MControl* aObject,TUint aEvent, TAny* aData );

		enum TAEnhsExpectedEvent
		{
			EEDBSOpen,
			EEStreamPause,
			EEDeleteBuffer,
			EEDBSGetInterface,
			EEWriteToStream,
			EEDataBuffer,
			EEStreamStart,
			EEStreamClose,
			EEStreamAddEffect,
			EEStreamOpen,
			EEStreamPrime,
			EEPDLSOpen,

		};

	    enum TControlTy
	        {
	            EStreamControl,
	            ESourceControl,
	            ESinkControl,
	            EEffectControl
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
        CEnhanMediaTestClass( CTestModuleIf& aTestModuleIf );

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

	    /**
	    * Set an event as expected and set default timeout
	    * @since ?Series60_version
	    */
	    void AddExpectedEvent(TAEnhsExpectedEvent event, TInt ms);


	    /**
	    * Unset an event as expected
	    * @since ?Series60_version
	    */
	    TBool RemoveExpectedEvent(TAEnhsExpectedEvent event);


	    /**
	    * @since ?Series60_version
	    * @param none
	    * Removes all expected events
	    */
	    void RemoveAllExpectedEvents();

	    /**
	    * Verify that the event was expected, removes it from the list
	    * Signal the TestScripter with the returned error code
	    * @since ?Series60_version
	    */
	    void ProcessEvent(TAEnhsExpectedEvent aEvent, TInt aError);

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
	    * Connect to the default client to the server
	    * @since ?Series60_version
	    * @param aItem Script line containing parameters.
	    * @return Symbian OS error code.
	    */

		TInt ExampleL( CStifItemParser& aItem );
		TInt CreateFactory( CStifItemParser& aItem );
		TInt DeleteFactory( CStifItemParser& aItem );
		TInt CreateSourceControl( CStifItemParser& aItem );
		TInt DeleteSource(CStifItemParser& /*aItem*/);
		TInt CloseSource(CStifItemParser& /*aItem*/);
		TInt CreateSink(CStifItemParser& aItem);
		TInt StreamAddSink(CStifItemParser& /*aItem*/);
		TInt StreamRemoveSink(CStifItemParser& /*aItem*/);
		TInt DeleteSink(CStifItemParser& aItem);
		TInt CreateEffect(CStifItemParser& aItem);
		TInt DeleteEffect(CStifItemParser& aItem);
		TInt RemoveEffect(CStifItemParser& aItem);
		TInt CreateStream(CStifItemParser& aItem);
		TInt DeleteStream(CStifItemParser& aItem);
		TInt StreamAddEffect(CStifItemParser& aItem);
		TInt StreamAddSource(CStifItemParser& aItem);
		TInt StreamRemoveSource(CStifItemParser& aItem);
		TInt AddObserver(CStifItemParser& aItem);
		TInt RemoveObserver(CStifItemParser& aItem);
		TInt FileSOpen(CStifItemParser& aItem);
		TInt FileSGetBitRate(CStifItemParser& aItem);
		TInt FileSGetSize(CStifItemParser& aItem);
		TInt Source_GetMimeType(CStifItemParser& aItem);
		TInt Source_GetSize(CStifItemParser& aItem);
		TInt SourceClose (CStifItemParser& aItem);
		TInt FileGenOpen(CStifItemParser& aItem);
		TInt DescSOpen(CStifItemParser& aItem);
		TInt DBSOpen(CStifItemParser& aItem);
		TInt DBSClose(CStifItemParser& aItem);
		TInt DBSSetSize(CStifItemParser& aItem);
		TInt DBSMinBufSize(CStifItemParser& aItem);
		void WriteLoop(CStifItemParser& aItem);
		TInt GetBuffConfig(CStifItemParser& aItem);
		TInt DBSSetBuffConfig(CStifItemParser& aItem);
		TInt DBSGetBuffType(CStifItemParser& aItem);
		TInt DBSGetInterface(CStifItemParser& aItem);
		TInt DBSGetBitRate(CStifItemParser& aItem);
		TInt DBSGetSize(CStifItemParser& aItem);
		TInt DSGetSize(CStifItemParser& aItem);
		TInt StreamOpen(CStifItemParser& aItem);
		TInt StreamClose(CStifItemParser& /*aItem*/);
		TInt StreamPrime(CStifItemParser& /*aItem*/);
		TInt StreamStart(CStifItemParser& /*aItem*/);
		TInt StreamPause(CStifItemParser& /*aItem*/);
		TInt StreamStop(CStifItemParser& /*aItem*/);
		TInt Stream_GetDuration(CStifItemParser& /*aItem*/);
		TInt Stream_SetPriority(CStifItemParser& /*aItem*/);
		TInt StreamGetSSL(CStifItemParser& aItem);
		TInt StreamGetRSSL(CStifItemParser& aItem);
		TInt EqualizerBandLevel(CStifItemParser& aItem);
		TInt EqualizerNumBands (CStifItemParser& aItem);
		TInt EqualizerBandWidth (CStifItemParser& aItem);
		TInt EqualizerCenterFrequency (CStifItemParser& aItem);
		TInt EqualizerCrossoverFrequency (CStifItemParser& aItem);
		TInt EqualizerDbLevelLimits (CStifItemParser& aItem);
		TInt EqualizerSetBandLevel (CStifItemParser& aItem);
		TInt StreamGetStateL(CStifItemParser& aItem);
		TInt StreamGetCIL(CStifItemParser& aItem);
		TInt StreamGetPositionL(CStifItemParser& aItem);
		TInt StreamSetPositionL(CStifItemParser& aItem);
		TInt StreamCustomInterface(CStifItemParser& aItem);
		TInt ControlTypeL(CStifItemParser& aItem);
		TInt ControlControlTypeL(CStifItemParser& aItem);
		TInt DescSGetBitRate(CStifItemParser& aItem);
		TInt CreateDataBuffer(CStifItemParser& aItem);
		TInt DeleteDataBuffer(CStifItemParser& aItem);
		TInt VolGetMinVolume(CStifItemParser& aItem);
		TInt VolGetMaxVolume(CStifItemParser& aItem);
		TInt VolGetVolume(CStifItemParser& aItem);
		TInt VolGetDefaultVolume(CStifItemParser& aItem);
		TInt VolSetVolume(CStifItemParser& aItem);
		TInt BalGetBalance(CStifItemParser& aItem);
		TInt BalSetBalance(CStifItemParser& aItem);
		TInt EffectApply(CStifItemParser& aItem);
		TInt VolSetVolumeRamp(CStifItemParser& aItem);


		TInt ReadFromFileAndWriteToStream(TInt index);

	    TInt DRMGetType( CStifItemParser& aItem );
	    TInt DRMSetType( CStifItemParser& aItem);
	    TInt DRMGetAllowedOutputDeviceCount( CStifItemParser& aItem );
	    TInt DRMGetAllowedOutputDevice(CStifItemParser& aItem );
	    TInt DRMAppendAllowedOutputDevice(CStifItemParser& aItem );
	    TInt DRMRemoveAllowedOutputDevice(CStifItemParser& aItem );
	    TInt DRMReset();
	    TInt DRMCommit(CStifItemParser& aItem);

	    TInt CreateSeekableData(CStifItemParser& aItem);

	    TInt AudioEffectIsEnabled (CStifItemParser& aItem);

	    TInt HandleAudioEffectEnableL(CStifItemParser& aItem);
	    TInt HandleAudioEffectDisableL(CStifItemParser& aItem);
	    TInt AudioEffectIsEnforced (CStifItemParser& aItem);
//added
	    TInt  AudioCreateEffect (CStifItemParser& aItem);
	    
	    TInt AudioEffectUid (CStifItemParser& aItem);
	    TInt AudioEffectHaveUpdateRights (CStifItemParser& aItem);
	    TInt AudioEffectEnforce (CStifItemParser& aItem);

	    TInt HandleReverbDecayHFRatioL(CStifItemParser& aItem);
	    TInt HandleReverbDecayHFRatioRangeL(CStifItemParser& aItem);
	    TInt HandleReverbDecayTimeL(CStifItemParser& aItem);
	    TInt HandleReverbDecayTimeRangeL(CStifItemParser& aItem);
	    TInt HandleReverbDensityL(CStifItemParser& aItem);
	    TInt HandleReverbDiffusionL(CStifItemParser& aItem);
	    TInt HandleReverbReflectionsDelayL(CStifItemParser& aItem);
	    TInt HandleReverbReflectionsDelayMaxL(CStifItemParser& aItem);
	    TInt HandleReverbReflectionsLevelL(CStifItemParser& aItem);
	    TInt HandleReverbReflectionLevelRangeL(CStifItemParser& aItem);
	    TInt HandleReverbReverbDelayL(CStifItemParser& aItem);
	    TInt HandleReverbReverbDelayMaxL(CStifItemParser& aItem);
	    TInt HandleReverbReverbLevelL(CStifItemParser& aItem);
	    TInt HandleReverbReverbLevelRangeL(CStifItemParser& aItem);
	    TInt HandleReverbRoomHFLevelL(CStifItemParser& aItem);
	    TInt HandleReverbRoomHFLevelRangeL(CStifItemParser& aItem);
	    TInt HandleReverbRoomLevelL(CStifItemParser& aItem);
	    TInt HandleReverbRoomLevelRangeL(CStifItemParser& aItem);
	    TInt HandleReverbSetDecayHFRatioL(CStifItemParser& aItem);
	    TInt HandleReverbSetDecayTimeL(CStifItemParser& aItem);
	    TInt HandleReverbSetDensityL(CStifItemParser& aItem);
	    TInt HandleReverbSetDiffusionL(CStifItemParser& aItem);
	    TInt HandleReverbSetReflectionsDelayL(CStifItemParser& aItem);
	    TInt HandleReverbSetReflectionsLevelL(CStifItemParser& aItem);
	    TInt HandleReverbSetReverbDelayL(CStifItemParser& aItem);
	    TInt HandleReverbSetReverbLevelL(CStifItemParser& aItem);
	    TInt HandleReverbSetRoomHFLevelL(CStifItemParser& aItem);
	    TInt HandleReverbSetRoomLevelL(CStifItemParser& aItem);
	    TInt HandleReverbDelayMaxL(CStifItemParser& aItem);

	    TInt HandleDelayMaxL(CStifItemParser& aItem);

	    TInt HandleSWIsContinuousLevelSupportedL(CStifItemParser& aItem);
	    TInt HandleSWSetStereoWideningLevelL(CStifItemParser& aItem);
	    TInt HandleSWStereoWideningLevelL(CStifItemParser& aItem);

	    TInt HandleRLLevelRangeL(CStifItemParser& aItem);
	    TInt HandleRLSetRoomLevelL(CStifItemParser& aItem);
	    TInt HandleRLLevelL(CStifItemParser& aItem);

	    TInt HandleDADistanceAttenuationL(CStifItemParser& aItem);
	    TInt HandleDARollOffFactorMaxL(CStifItemParser& aItem);
	    TInt HandleDARoomRollOffFactorMaxL(CStifItemParser& aItem);

	    TInt HandleSLLocationCartesianL(CStifItemParser& aItem);
	    TInt HandleSLLocationSphericalL(CStifItemParser& aItem);
	    TInt HandleSLSetLocationCartesianL(CStifItemParser& aItem);
	    TInt HandleSLSetLocationSphericalL(CStifItemParser& aItem);
	    TInt LO_OrientationL (CStifItemParser& aItem);
	    TInt LO_OrientationVectorsL (CStifItemParser& aItem);

	    TInt HandleControlTypeL (CStifItemParser& aItem);
	    TInt HandleControlControlTypeL (CStifItemParser& aItem);


	    TInt HandleLDSetSphericalVelocityL (CStifItemParser& aItem);
	    TInt HandleLDSphericalVelocityL (CStifItemParser& aItem);

	    TInt HandleSDSetFactorL(CStifItemParser& aItem);
	    TInt HandleSDSetSphericalVelocityL(CStifItemParser& aItem);
	    TInt HandleSDSphericalVelocityL(CStifItemParser& aItem);
	    TInt HandleSDCartesianVelocityL(CStifItemParser& aItem);
	    TInt HandleSDFactorL(CStifItemParser& aItem);
	    TInt HandleSDFactorMaxL(CStifItemParser& aItem);
	    TInt HandleSDSetCartesianVelocityL(CStifItemParser& aItem);

		TInt AttachReverb(CStifItemParser& /*aItem*/);
		TInt DetachReverb(CStifItemParser& /*aItem*/);


    public:     // Data
        // ?one_line_short_description_of_data
        //?data_declaration;

    protected:  // Data
        // ?one_line_short_description_of_data
        //?data_declaration;

    private:    // Data

    	// reference to TestModuleIf
    	CTestModuleIf& iTestModuleIf;

    	//CMMFDevSound*	iDevSound;
    	//CMdaAudioOutputStream*    iAudioOutputStream;

    	//CAacDecoderConfig* iAacDecoder;

    	// Active object with a timer to timeout the test case
    	CSimpleTimeout * iTimeoutController;

    	// Indicates if the test case use a normal exit reason
    	TBool iNormalExitReason;

    	// List of expected events
    	RArray<TAEnhsExpectedEvent> iExpectedEvents;

    	// List of notification event (by the callback)
    	//RArray<TAEnhsExpectedEvent> iOcurredEvents;

    	//RArray<TUint> iSupportedBitrates;


    	CMultimediaFactory* iFactory;
    	MStreamControl* iMStreamControl;
    	MDataBufferSource* iMDataBufferSource;
        MDescriptorSource* iMDescriptorSource;
        MSinkControl* iMAudioSink;

        MFileSource* iMFileSource;
        MSourceControl* iSourceControl;
        RPointerArray<MDataBuffer>  iBuffers;
        RArray<TInt> iAvailable;
        MEqualizerControl* iMEqualizerControl;
        MBassBoostControl* iMBassBoostControl;
        MVolumeControl* iMVolumeControl;
        MBalanceControl* iMBalanceControl;
        MEffectControl* iEffectControl;
        MReverbControl* iMReverbControl;

        MStereoWideningControl* iMStereoWideningControl;
        MLoudnessControl* iMLoudnessControl;
        MRoomLevelControl* iMRoomLevelControl;
        MDistanceAttenuationControl* iMDistanceAttenuationControl;

        MListenerLocationControl* iMListenerLocationControl;
        MSourceLocationControl* iMSourceLocationControl;

        MListenerDopplerControl* iMListenerDopplerControl;
        MSourceDopplerControl* iMSourceDopplerControl;

        MSourceOrientationControl* iMSourceOrientationControl;
        MListenerOrientationControl* iMListenerOrientationControl;

        HBufC* iFileName;
        HBufC8* iMimeType;
        RFs             iFs;
        RFile           iFile;
        /*****************************/
        //CF32File* iF32File;
        /*****************************/
        HBufC8* iDescData;
        CDRMConfigIntfc*            iDRMConfigIntfc;
        //CDataSourceConfigIntfcImpl* iDataSourceConfigIntfcImpl;
        TBool                       iIsEOFReached;
        TBool iAutoWriteBuffer;

        TInt         iBytesReadFromFile;
        TInt         iFileSizeInBytes;

        TInt iAllowedDevice;

        CActiveSchedulerWait* iActive;
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
#endif      // CEnhanMediaTestClass_H

// End of File
