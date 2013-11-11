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
* Description: Audio Output Control Utility
*
*/



#ifndef CAudioOutputControlTestClass_H
#define CAudioOutputControlTestClass_H

//  INCLUDES
#include <StifLogger.h>
#include <TestScripterInternal.h>
#include <StifTestModule.h>

#include <e32svr.h>
#include <e32base.h>
#include <e32cons.h>
//#include <DataSourceAdapter.h>



#include <customInterfaceUtility.h>
#include <SoundDevice.h>
#include <ConfigurationComponentsFactory.h>
#include <AudioOutputControlUtility.h>
#include <DRMConfigIntfc.h>
#include <DataBufferSource.h>

#include <StreamControl.h>
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
#include <VolumeControl.h>

#include <StreamControlObserver.h>
#include <SourceControlObserver.h>
#include "TimeoutController.h"
using namespace multimedia;


// CONSTANTS
const TInt KErrBadTestParameter= -1000;         // Error on configuration file

const TInt KErrEventPending =-2000;             // Timeout and not callback
const TInt KErrCallbackErrorCode = -2001;       // Callback return error code
const TInt KErrUnexpectedValue =-2002;          // Unexpected value at setting
const TInt KErrTimeoutController = -2007;		// Unexpected notification

const TInt KShortTimeout = 1000;
const TInt KMediumTimeout = 4000;
const TInt KLongTimeout = 100000;

// Uid identifying Stream Control
/*const TUid KStreamControl = {0x10207B44};
const TUid KMMFAudioOutputSinkControl = {0x101F53B9};
// Uid identifying Volume Control
const TUid KVolumeEffectControl = {0x10207B75};*/

_LIT( KMsgBadTestParameters, "[Error] No valid test case parameters");

// General
_LIT( KTagG711, "G711");
_LIT( KTagOutput32Bit, "Output32Bit");
_LIT( KTagDownMixToStereo, "DownMixToStereo");
_LIT( KTagLostDataConcealment, "LostDataConcealment");
_LIT( KTagTrue, "True");
_LIT( KTagFalse, "False");

// MACROS
//#define ?macro ?macro_def
// Logging path
#ifdef __WINSCW__
_LIT( KAudioOutputControlTestClassLogPath, "\\logs\\testframework\\" );
#else
_LIT( KAudioOutputControlTestClassLogPath, "e:\\testing\\Log\\" );
#endif

// Log file
_LIT( KAudioOutputControlTestClassLogFile, "AudioOutputControlTestClass.txt" );


// FORWARD DECLARATIONS
class CAudioOutputControlTestClass;

class CConfigurationComponentsFactory;
class CMMFDevSound;
class CAudioOutputControlUtility;

using multimedia::MStreamControl;
using multimedia::MControlObserver;
using multimedia::MDataBufferSource;
using multimedia::MDescriptorSource;
using multimedia::MDataBuffer;
using multimedia::MControl;
using multimedia::CMultimediaFactory;
using multimedia::MSinkControl;
using multimedia::MFileSource;
using multimedia::MVolumeControl;

//_LIT(KRm1TestFile,"C:\\10sec_16Kbps_Voice_Wideband.rm");
_LIT(KRm1TestFile,"C:\\Sounds\\RM128k_RA10_stereo.rm");
_LIT(KPcmTestFile,"C:\\Sounds\\T48000mo.pcm");
_LIT(KWAVTestFile,"C:\\Sounds\\opera.wav");
_LIT(KMp3TestFile,"C:\\Sounds\\test.mp3");

// CLASS DECLARATION

/**
*  CRadioUtilityTestClass test class for STIF Test Framework TestScripter.
*  ?other_description_lines
*
*  @lib ?library
*  @since Series60_3_2
*/
NONSHARABLE_CLASS(CAudioOutputControlTestClass) : public CScriptBase,
											public MTimeoutObserver,
											public MDevSoundObserver,
											MControlObserver//,
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
        static CAudioOutputControlTestClass* NewL( CTestModuleIf& aTestModuleIf );

        /**
        * Destructor.
        */
        virtual ~CAudioOutputControlTestClass();

		enum TG711ExpectedEvent
		{
			EInitializeComplete,

		};

    public: // New functions

        /**
        * ?member_description.
        * @since ?Series60_version
        * @param ?arg1
        * @return
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


		// Required for MDevSoundObserver:
		void InitializeComplete(TInt /*aError*/);
		void ToneFinished(TInt /*aError*/);
		void BufferToBeFilled(CMMFBuffer* /*aBuffer*/);
		void PlayError(TInt /*aError*/);
		void BufferToBeEmptied(CMMFBuffer* /*aBuffer*/);
		void RecordError(TInt /*aError*/);
		void ConvertError(TInt /*aError*/);
		void DeviceMessage(TUid /*aMessageType*/, const TDesC8& /*aMsg*/);

	    // Audio Output Stream
    	//virtual void MaoscOpenComplete(TInt aError);
	    //virtual void MaoscBufferCopied(TInt aError, const TDesC8& aBuffer);
    	//virtual void MaoscPlayComplete(TInt aError);

	    // From MTimeoutObserver

		//MCONTROL OBSERVER
        virtual void Event( MControl* aObject,TUint aEvent, TAny* aData );


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
        * @param ?arg1
        * @return
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
        CAudioOutputControlTestClass( CTestModuleIf& aTestModuleIf );

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
	    void AddExpectedEvent(TG711ExpectedEvent event, TInt ms);


	    /**
	    * Unset an event as expected
	    * @since ?Series60_version
	    */
	    TBool RemoveExpectedEvent(TG711ExpectedEvent event);


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
	    void ProcessEvent(TG711ExpectedEvent aEvent, TInt aError);

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
//		TInt CreateAudioOutputControl( CStifItemParser& aItem );
		TInt CreateObject( CStifItemParser& aItem );
		TInt ConfigureAudioOutput( CStifItemParser& aItem );
		TInt SetDRMConfigIntfc( CStifItemParser& aItem );
		TInt CreateRestrictedAudioOutput(CStifItemParser& aItem );

		void SourceMenuResetEOFFlag();


		TInt SetDRMConfig( CStifItemParser& aItem );
		TInt CalcPref( CStifItemParser& aItem );


    public:     // Data
        // ?one_line_short_description_of_data
        //?data_declaration;

    protected:  // Data
        // ?one_line_short_description_of_data
        //?data_declaration;

    private:    // Data

    	// reference to TestModuleIf
    	CTestModuleIf& iTestModuleIf;

    	MDataSource* aSource;


    	CMMFDevSound*				iDevSound;
		//	CRestrictedAudioOutput* iRestrictedAudioOutput;
    	CAudioOutputControlUtility* iAudioOutputControlUtility;
    	MSourceControl* iSourceControl;

    	CConfigurationComponentsFactory* iFactory;
    	CMultimediaFactory* iFactorys;
		//CAudioOutputControl* iAudioOutputControl;

		// Active object with a timer to timeout the test case
		CSimpleTimeout * iTimeoutController;

		// Indicates if the test case use a normal exit reason
		TBool iNormalExitReason;

		// List of expected events
		RArray<TG711ExpectedEvent> iExpectedEvents;

		// List of notification event (by the callback)
		RArray<TG711ExpectedEvent> iOcurredEvents;

		CDRMConfigIntfc* iDRMConfigIntfc;

		 MDataBufferSource* iMDataBufferSource;
		 MStreamControl* iMStreamControl;
		 MVolumeControl* iMVolumeControl;
		 MSinkControl* iMAudioSink;

	        TBool        iIsEOFReached;
	        TInt         iBytesReadFromFile;
	        TInt iMaxVolume;
	        RFs             iFs;
	        RFile           iFile;



	     CRestrictedAudioOutput* iRestrictedAudioOutput;


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

#endif      // CAudioOutputControlTestClass_H

// End of File
