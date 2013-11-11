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
* Description: 
*
*/


#ifndef LOCATIONMANAGERTESTSCRIPTER_H
#define LOCATIONMANAGERTESTSCRIPTER_H

//  INCLUDES
#include <StifLogger.h>
#include <TestScripterInternal.h>
#include <StifTestModule.h>
#include <locationeventdef.h>
#include <rlocationtrail.h>
#include <rtracklog.h>
#include "locationmanagerdefs.h"
#include "rlocationobjectmanipulator.h"
#include "mdccommon.h"
#include "mdesession.h"

#ifdef LOC_REVERSEGEOCODE
#include "rlocationgeotagger.h"
#endif //LOC_REVERSEGEOCODE

// CONSTANTS
// Logging path
_LIT( KLocationManagerTestScripterLogPath, "\\logs\\testframework\\LocationManagerTestScripter\\" ); 
// Log file
_LIT( KLocationManagerTestScripterLogFile, "LocationManagerTestScripter.txt" ); 
_LIT( KLocationManagerTestScripterLogFileWithTitle, "LocationManagerTestScripter_[%S].txt" );


// FORWARD DECLARATIONS
class CMdEObject;
class CLocationManagerTestScripter;


// CLASS DECLARATION
NONSHARABLE_CLASS(CLocationManagerTestScripter) : public CScriptBase,
												  public MTrackLogObserver,
												  public MMdESessionObserver
    {
    public:  // Constructors and destructor

        /**
        * Two-phased constructor.
        */
        static CLocationManagerTestScripter* NewL( CTestModuleIf& aTestModuleIf );

        /**
        * Destructor.
        */
        virtual ~CLocationManagerTestScripter();

    public: // Functions from base classes

        /**
        * From CScriptBase Runs a script line.
        * @since ?Series60_version
        * @param aItem Script line containing method name and parameters
        * @return Symbian OS error code
        */
        virtual TInt RunMethodL( CStifItemParser& aItem );
 
    public: // From MdESessionObserver
		virtual void HandleSessionOpened(CMdESession& aSession, TInt aError);
		virtual void HandleSessionError(CMdESession& aSession, TInt aError);
		
		virtual void GPSFixQualityChanged(TPositionSatelliteInfo& aFixQuality, 
		                                                        TBool& aTrackingActive );

    private:

        /**
        * C++ default constructor.
        */
        CLocationManagerTestScripter( CTestModuleIf& aTestModuleIf );

        /**
        * By default Symbian 2nd phase constructor is private.
        */
        void ConstructL();

        /**
        * Frees all resources allocated from test methods.
        */
        void Delete();

        CMdEObject* CreateMetadataObjectL();
        void TestLocationData( TLocationData& aLocationData );
        
        TInt PrepareSessionL( CStifItemParser& aItem );
        TInt SetupOML( CStifItemParser& aItem );
        TInt LocationSnapshotL( CStifItemParser& aItem );
        TInt RemoveLocationObjectL( CStifItemParser& aItem );
        TInt TearDownOML( CStifItemParser& aItem );
        TInt CloseOML( CStifItemParser& aItem );
        virtual TInt RLocationObjectManipulatorTest_CopyByIDL( CStifItemParser& aItem );
        virtual TInt RLocationObjectManipulatorTest_CopyByURIL( CStifItemParser& aItem );
        TInt RLocationObjectManipulatorTest_CopyByURI_DisconnectedL( CStifItemParser& aItem );
        TInt RLocationObjectManipulatorTest_CopyByID_DisconnectedL( CStifItemParser& aItem );
        
        virtual TInt ConnectL( CStifItemParser& aItem );
        virtual TInt CloseL( CStifItemParser& aItem );
        virtual TInt TrailConnectL( CStifItemParser& aItem );
        virtual TInt TrailCloseL( CStifItemParser& aItem );
        virtual TInt StartTrailL( CStifItemParser& aItem );
        virtual TInt StopTrailL( CStifItemParser& aItem );
        virtual TInt StartTrailTestsL( CStifItemParser& aItem );
        virtual TInt GetTrailStateL( CStifItemParser& aItem );
        virtual TInt LocationTrailNotifyTestL( CStifItemParser& aItem );
        virtual TInt RetrieveLocationL( CStifItemParser& aItem );
        virtual TInt CurrentLocationL( CStifItemParser& aItem );
        
        TInt SetupTrackLogL( CStifItemParser& aItem );
        TInt TearDownTrackLog( CStifItemParser& aItem );
        TInt RTrackLogTest_AddObserverL( CStifItemParser& aItem );
        TInt RTrackLogTest_RemoveObserverL( CStifItemParser& aItem );
        virtual TInt RTrackLogTest_StartTrackLogL( CStifItemParser& aItem );
        virtual TInt RTrackLogTest_RecordingL( CStifItemParser& aItem );
        virtual TInt RTrackLogTest_StopTrackLogL( CStifItemParser& aItem );
        virtual TInt RTrackLogTest_NotRecordingL( CStifItemParser& aItem );
        TInt CrashLocationManagerL( CStifItemParser& aItem );
        TInt GetFileL( CStifItemParser& aItem );
        TInt DeleteFileL( CStifItemParser& aItem );
        TInt GetStatusL( CStifItemParser& aItem );
        TInt GetStatus2L( CStifItemParser& aItem );
        //ADD NEW METHOD DEC HERE
        //[TestMethods] - Do not remove

#ifdef LOC_REVERSEGEOCODE
         TInt ConnectGeoTaggerL( CStifItemParser& /*aItem*/ );
         TInt CloseGeoTaggerL( CStifItemParser& /*aItem*/ );
         TInt StartGeoTaggingL( CStifItemParser& aItem );
                 
         TInt TestGeoTagCompleteStatusL( CStifItemParser& aItem );
         TInt TestGeoTagPendingStatusL( CStifItemParser& aItem );
         TInt TestGeoTagGoingOnStatusL( CStifItemParser& aItem );
         
         TInt CancelGeoTaggingL( CStifItemParser& aItem ) ;
#endif //LOC_REVERSEGEOCODE
         
    public: // From MTrackLogObserver
    	void TrackLogStarted(TInt aError);
    	void TrackLogStopped(TInt aError);

    private:    // Data
        
        RLocationManager iLocationManager;
        RLocationTrail iLocationTrail;
        
     	TBool iRunning;
     	RTrackLog iTrackLog;
     	RLocationObjectManipulator iOM;
     	
     	CMdEObject* iSourceObject;
     	CMdEObject* iTargetObject;
     	CMdESession* iMdeSession;
     	
     	CActiveSchedulerWait* iASW;
     	
#ifdef LOC_REVERSEGEOCODE
     	RLocationGeoTagger iLocationGeoTagger;
     	TBool iLocGeoTaggerConnected;
#endif //LOC_REVERSEGEOCODE
    };

#endif      // LOCATIONMANAGERTESTSCRIPTER_H

// End of File
