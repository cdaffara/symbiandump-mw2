// Copyright (c) 2004-2009 Nokia Corporation and/or its subsidiary(-ies).
// All rights reserved.
// This component and the accompanying materials are made available
// under the terms of "Eclipse Public License v1.0"
// which accompanies this distribution, and is available
// at the URL "http://www.eclipse.org/legal/epl-v10.html".
//
// Initial Contributors:
// Nokia Corporation - initial contribution.
//
// Contributors:
//
// Description:
// Header file: Declaration for Various Classes used for Audio Resource Notification tests.
// 
//

/**
 @file TestAudioClientUtilitiesARN.h
 */

#ifndef AUDIOPREMPTIONARN_H
#define AUDIOPREMPTIONARN_H

#include "mdaaudiosampleplayer.h"
#include "mdaaudiosampleeditor.h"
#include "TestCaseBase.h"
#include <mmf/common/mmfaudio.h>

//
//Forward class declarations
//

class CPreEmptionARNCallbackSupport;

//
//BASE CLASS DECLARATIONS//
//

/**
 * Observer class to identify and pass events to the test step class
 * 
 * @class MAudioPreEmptionARNCallback
 *
 */
class MAudioPreEmptionARNCallback
    {
public :
    virtual void MapcInitCompleteTest(TInt aError, const TTimeIntervalMicroSeconds& aDuration, CPreEmptionARNCallbackSupport& aMdaAudioPlayerCallbackSupport) = 0;
    virtual void MarncResourceAvailableTest(TUid aNotificationEventId, const TDesC8& aNotificationData, CPreEmptionARNCallbackSupport& aMdaAudioPlayerCallbackSupport) = 0;
    virtual void MapcPlayCompleteTest(TInt aError, CPreEmptionARNCallbackSupport& aMdaAudioPlayerCallbackSupport) = 0;
    };

/**
 * Support class to pass the events to the test step class using the MAudioPreEmptionARNCallback
 * 
 * @class CPreEmptionARNCallbackSupport
 *
 */
class CPreEmptionARNCallbackSupport : public CBase, public MMdaAudioPlayerCallback, public MMMFAudioResourceNotificationCallback
    {
public :
    static CPreEmptionARNCallbackSupport* NewL(MAudioPreEmptionARNCallback& aCallback);
    virtual void MapcInitComplete(TInt aError, const TTimeIntervalMicroSeconds& aDuration);
    virtual void MapcPlayComplete(TInt aError);
    virtual void MarncResourceAvailable(TUid aNotificationEventId, const TDesC8& aNotificationData);
protected:
    CPreEmptionARNCallbackSupport(MAudioPreEmptionARNCallback& aCallback);

private :
    MAudioPreEmptionARNCallback& iCallback;
    };


/**
 * Base Class for ARN functionality for most ARN audio player tests
 * 
 * @class CAudioPreEmptionARN
 *
 */
class CAudioPreEmptionARN : public CTestCaseBase, public MAudioPreEmptionARNCallback
    {
public:
    // Enum to describe the current state.
    enum TMmfAudioPlayerState
        {
        EStateBegin = 0,
        EStateOpenPlayer1,
        EStateOpenPlayer2,
        EStatePlaying1,
        EStatePlaying2,
        EStateEndPlay1,
        EStateEndPlay2,
        EStateErrInUsePlayer1,
        EStateErrInUsePlayer2,
        EStateNotifiedPlayer1,
        EStateNotifiedPlayer2,
        };

    // Enum to decide which Event has occured.
    enum TMmfAudioPlayerEvents
        {
        EIdlePlayer = 0,
        EMapcInitComplete1,
        EMapcInitComplete2,
        EMapcPlayComplete1,
        EMapcPlayComplete2,
        EMarncResourceAvailable1,
        EMarncResourceAvailable2,
        };
public :
    // Constructor
    CAudioPreEmptionARN(CTestModuleIf *aConsole, CStifLogger *aLogger);
    // from MAudioPreEmptionARNCallback
    virtual void MapcInitCompleteTest(TInt aError, const TTimeIntervalMicroSeconds& aDuration, CPreEmptionARNCallbackSupport& aMdaAudioPlayerCallbackSupport);
    virtual void MarncResourceAvailableTest(TUid aNotificationEventId, const TDesC8& aNotificationData, CPreEmptionARNCallbackSupport& aMdaAudioPlayerCallbackSupport);
    virtual void MapcPlayCompleteTest(TInt aError, CPreEmptionARNCallbackSupport& aMdaAudioPlayerCallbackSupport);
    // Centralised State Handler
    virtual TInt FsmL(TMmfAudioPlayerEvents aEventCode, TInt aError)=0;
    virtual TBool FsmCheck(TMmfAudioPlayerEvents aEventCode, TInt aError);
    virtual ~CAudioPreEmptionARN();
    TInt DoTestStepPreambleL(CTestModuleIf *aConsole, CStifLogger *aLogger, CStifSectionParser *aParser);
    void DoTestStepPostambleL();
    
protected : 

    void InitializeL();
    void StateInit();
    void Release();
    // StateChange Function
    void StateChange(TMmfAudioPlayerState aState, TMmfAudioPlayerEvents aExpectedEvent, TInt aExpectedError=KErrNone);
protected : 
    // CMdaAudioPlayerUtility instances
    CMdaAudioPlayerUtility* iTestAudioPlayer1;
    CMdaAudioPlayerUtility* iTestAudioPlayer2;

    // CPreEmptionARNCallbackSupport instances
    CPreEmptionARNCallbackSupport* iMdaAudioPlayerCallbackSupport1;
    CPreEmptionARNCallbackSupport* iMdaAudioPlayerCallbackSupport2;

    // Other class variable declarations

    TTimeIntervalMicroSeconds iPosition;
    TTimeIntervalMicroSeconds iDuration;
    TInt iError1;
    TTimeIntervalMicroSeconds iPlayer1Pos;
    TFileName iFileName1;
    TFileName iFileName2;
    TInt iError;
    TMmfAudioPlayerState iState;
    TRequestStatus iRequestStatus;
    //CCallBackTimer* iCallBackTimer;
    TMmfAudioPlayerEvents iExpectedEvent;
    TInt iExpectedError;
    TMmfAudioPlayerEvents iEventOccured;
    CActiveScheduler* iActiveScheduler; 
    };

/** 
 * Dont register for ARN, get pre-empted and then resume from same position.
 * Test case for CR Id: 
 * @class CAudioPreEmptionARN235
 * 
 */
class CAudioPreEmptionARN235 : public CAudioPreEmptionARN
    {
public:
    static CAudioPreEmptionARN235* NewL( CTestModuleIf *aConsole, CStifLogger *aLogger );

    static TInt RunTestL(CTestModuleIf *aConsole, CStifLogger *aLogger, CStifSectionParser *aParser, TInt* /*clipCounter*/);
    virtual TInt FsmL(TMmfAudioPlayerEvents aEventCode, TInt aError);

    TInt ConstructL(CTestModuleIf *aConsole, CStifLogger *aLogger, CStifSectionParser *aParser);

protected:
    CAudioPreEmptionARN235( CTestModuleIf *aConsole, CStifLogger *aLogger );
    
    };

/** 
 * Dont register for ARN, get pre-empted, stop and chk if position is reset, start play again.
 * Test case for CR Id: 
 * @class CAudioPreEmptionARN236
 * 
 */
class CAudioPreEmptionARN236 : public CAudioPreEmptionARN
    {
public:
    static CAudioPreEmptionARN236* NewL( CTestModuleIf *aConsole, CStifLogger *aLogger );
    // virtual TInt DoTestL(CTestModuleIf *aConsole, CStifLogger *aLogger, CStifSectionParser *aParser, TInt* /*clipCounter*/);

    static TInt RunTestL(CTestModuleIf *aConsole, CStifLogger *aLogger, CStifSectionParser *aParser, TInt* /*clipCounter*/);
    virtual TInt FsmL(TMmfAudioPlayerEvents aEventCode, TInt aError);
    TInt ConstructL(CTestModuleIf *aConsole, CStifLogger *aLogger, CStifSectionParser *aParser);

protected:
    CAudioPreEmptionARN236( CTestModuleIf *aConsole, CStifLogger *aLogger );
    
    };

/** 
 * Play higher priority first, then start playing low priority
 * Test case for CR Id: 
 * @class CAudioPreEmptionARN237
 * 
 */
class CAudioPreEmptionARN237 : public CAudioPreEmptionARN
    {
public:
    static CAudioPreEmptionARN237* NewL( CTestModuleIf *aConsole, CStifLogger *aLogger );
    //  virtual TInt DoTestL(CTestModuleIf *aConsole, CStifLogger *aLogger, CStifSectionParser *aParser, TInt* /*clipCounter*/);

    static TInt RunTestL(CTestModuleIf *aConsole, CStifLogger *aLogger, CStifSectionParser *aParser, TInt* /*clipCounter*/);
    virtual TInt FsmL(TMmfAudioPlayerEvents aEventCode, TInt aError);

    TInt ConstructL(CTestModuleIf *aConsole, CStifLogger *aLogger, CStifSectionParser *aParser);

protected:
    CAudioPreEmptionARN237( CTestModuleIf *aConsole, CStifLogger *aLogger );
     };

#endif

