// Copyright (c) 2005-2009 Nokia Corporation and/or its subsidiary(-ies).
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
// CPP file: Audio Resource Notification tests for Audio Player.
// 
//

/**
 @file TestPlayerUtilsARN.cpp
 */

#include "AudioPreempionARN.h"
//
//CPreEmptionARNCallbackSupport
//

/**
 * NewL
 */
CPreEmptionARNCallbackSupport* CPreEmptionARNCallbackSupport::NewL(MAudioPreEmptionARNCallback& aCallback)
    {
    return new(ELeave) CPreEmptionARNCallbackSupport(aCallback);
    }

/**
 * MapcInitComplete
 */
void CPreEmptionARNCallbackSupport::MapcInitComplete(TInt aError, const TTimeIntervalMicroSeconds& aDuration)
    {
    iCallback.MapcInitCompleteTest(aError, aDuration, *this);
    }

/**
 * MapcPlayComplete
 */
void CPreEmptionARNCallbackSupport::MapcPlayComplete(TInt aError)
    {
    iCallback.MapcPlayCompleteTest(aError, *this);
    }

/**
 * MarncResourceAvailable
 */
void CPreEmptionARNCallbackSupport::MarncResourceAvailable(TUid aNotificationEventId, const TDesC8& aNotificationData)
    {
    iCallback.MarncResourceAvailableTest(aNotificationEventId, aNotificationData, *this);
    }

/**
 * Constructor
 */
CPreEmptionARNCallbackSupport::CPreEmptionARNCallbackSupport(MAudioPreEmptionARNCallback& aCallback):iCallback(aCallback)
                {
 
                }

//
//CAudioPreEmptionARN
//

/**
 * Constructor
 */
CAudioPreEmptionARN::CAudioPreEmptionARN(CTestModuleIf *aConsole, CStifLogger *aLogger):iError1(KErrNone),iError(KErrNone)
                    {
                    // store the name of this test case
                    console = aConsole;
                    logger = aLogger;
                    }


/**
 * MapcInitCompleteTest
 */
void CAudioPreEmptionARN::MapcInitCompleteTest(TInt aError, const TTimeIntervalMicroSeconds& aDuration, CPreEmptionARNCallbackSupport& aMdaAudioPlayerCallbackSupport)
    {
    logger->Log(_L("CAudioPreEmptionARN::MapcInitCompleteTest is called aError =%d"), aError);
    iDuration = aDuration;
    if (&aMdaAudioPlayerCallbackSupport == iMdaAudioPlayerCallbackSupport1)
        {
        TRAP(iError1, FsmL(EMapcInitComplete1, aError));
        if(iError1 != KErrNone)
             {
             CActiveScheduler::Stop();
             }
        }
    else if (&aMdaAudioPlayerCallbackSupport == iMdaAudioPlayerCallbackSupport2)
        {
        TRAP(iError1, FsmL(EMapcInitComplete2, aError));
        if(iError1 != KErrNone)
             {
             CActiveScheduler::Stop();
             }
        }
    }

/**
 * MapcPlayCompleteTest
 */
void CAudioPreEmptionARN::MapcPlayCompleteTest(TInt aError, CPreEmptionARNCallbackSupport& aMdaAudioPlayerCallbackSupport)
    {
    if (&aMdaAudioPlayerCallbackSupport == iMdaAudioPlayerCallbackSupport1)
        {
        TRAP(iError1, FsmL(EMapcPlayComplete1, aError));
        if(iError1 != KErrNone)
            {
            CActiveScheduler::Stop();
            }

        }
    else if (&aMdaAudioPlayerCallbackSupport == iMdaAudioPlayerCallbackSupport2)
        {
        TRAP(iError1, FsmL(EMapcPlayComplete2, aError));
        if(iError1 != KErrNone)
             {
             CActiveScheduler::Stop();
             }
        }
    }


/**
 * MarncResourceAvailableTest
 */
void CAudioPreEmptionARN::MarncResourceAvailableTest(TUid aNotificationEventId, const TDesC8& aNotificationData, CPreEmptionARNCallbackSupport& aMdaAudioPlayerCallbackSupport)
    {
    if (aNotificationEventId == KMMFEventCategoryAudioResourceAvailable)
        {
        //Convert TDesc8 to TInt64
        TMMFTimeIntervalMicroSecondsPckg timeIntvbuf;
        timeIntvbuf.Copy(aNotificationData);
        TTimeIntervalMicroSeconds intPos = timeIntvbuf();
        iPosition = intPos;
        if (&aMdaAudioPlayerCallbackSupport == iMdaAudioPlayerCallbackSupport1)
            {
            TRAP(iError1, FsmL(EMarncResourceAvailable1, KErrNone));
            }
        else if (&aMdaAudioPlayerCallbackSupport == iMdaAudioPlayerCallbackSupport2)
            {
            TRAP(iError1, FsmL(EMarncResourceAvailable2, KErrNone));
            }

        }
    else
        {

        }
    }

/**
 * StateInit
 */
void CAudioPreEmptionARN::StateInit()
    {
    iExpectedEvent=EIdlePlayer;
    iExpectedError=KErrNone;
    iState=EStateBegin;
    }

/**
 * StateChange
 */
void CAudioPreEmptionARN::StateChange(TMmfAudioPlayerState aState, TMmfAudioPlayerEvents aExpectedEvent, TInt aExpectedError)
    {
    iExpectedEvent=aExpectedEvent;
    iExpectedError=aExpectedError;
    iState=aState;
    }

/**
 * FsmCheck
 */
TBool CAudioPreEmptionARN::FsmCheck(TMmfAudioPlayerEvents aEventCode, TInt aError)
    {
    TBool retFsmCheck = EFalse;
    if (((aEventCode == EMapcPlayComplete1) || (aEventCode == EMapcPlayComplete2)) && (aError == KErrUnderflow))
        {

        }
    else if (iExpectedEvent != aEventCode)
        {
        CActiveScheduler::Stop();
        }
    else if (iExpectedError != aError)
        {
        CActiveScheduler::Stop();
        }
    else
        {
        retFsmCheck = ETrue;
        }
    return retFsmCheck;
    }

//
//CAudioPreEmptionARN235 //test to cover CR id: 
//

/**
 * Constructor
 */
CAudioPreEmptionARN235::CAudioPreEmptionARN235(CTestModuleIf *aConsole, CStifLogger *aLogger): CAudioPreEmptionARN(aConsole, aLogger)
                {
                console = aConsole;
                logger = aLogger;
                }

/**
 * NewL
 */
CAudioPreEmptionARN235* CAudioPreEmptionARN235::NewL(CTestModuleIf *aConsole, CStifLogger *aLogger)
    {
    CAudioPreEmptionARN235* self = new (ELeave) CAudioPreEmptionARN235(aConsole, aLogger);
    return self;
    }


/**
 * FsmL
 */
TInt CAudioPreEmptionARN235::FsmL(TMmfAudioPlayerEvents aEventCode, TInt aError)
    {
    if (FsmCheck(aEventCode, aError))
        {
        TTimeIntervalMicroSeconds32 wait2secs(2000000);
        TTimeIntervalMicroSeconds oneNhalf(MAKE_TINT64(0,1500000));

        switch (iState)
            {
            case EStateBegin:    
                // Open iTestAudioPlayer#1 // next event to play iTestAudioPlayer#1
                iTestAudioPlayer1->OpenFileL(iFileName1); // KWav

                // Set ExpectedEvent and change the State
                StateChange(EStatePlaying1, EMapcInitComplete1);
                break;
            case EStatePlaying1:
                // Play iTestAudioPlayer#1
                iTestAudioPlayer1->Play();
                // Open iTestAudioPlayer#2
                iTestAudioPlayer2->OpenFileL(iFileName2);
                // Set ExpectedEvent and change the State
                StateChange(EStatePlaying2, EMapcInitComplete2);
                break;
            case EStatePlaying2:
                // Play iTestAudioPlayer#2
                User::After(wait2secs);
                iTestAudioPlayer2->Play();
                // Set ExpectedEvent and change the State
                StateChange(EStateErrInUsePlayer1, EMapcPlayComplete1, KErrInUse);
                break;
            case EStateErrInUsePlayer1:
                iError = iTestAudioPlayer1->GetPosition(iPlayer1Pos);
                if(iError != KErrNone)
                    {
                    CActiveScheduler::Stop();
                    break;
                    }
                //using 1.5 sec for comparison as sometimes playback is slow.
                if(iPlayer1Pos < oneNhalf)
                    {
                    logger->Log(_L("Test case failed "));
                    iError = KErrGeneral;
                    CActiveScheduler::Stop();
                    break;
                    }
                // Set ExpectedEvent and change the State
                StateChange(EStateEndPlay2, EMapcPlayComplete2 );
                break;
            case EStateEndPlay2:
                iError = KErrNone;
                // Play iTestAudioPlayer#1 again
                TTimeIntervalMicroSeconds pos;
                iError = iTestAudioPlayer1->GetPosition(pos);
                if(iError != KErrNone)
                    {
                    logger->Log(_L("Test case failed "));
                    CActiveScheduler::Stop();
                    break;
                    }
                if(pos == 0 || pos != iPlayer1Pos )
                    {
                    logger->Log(_L("Test case failed "));
                    iError = KErrGeneral;
                    CActiveScheduler::Stop();
                    break;
                    }
                iTestAudioPlayer1->Play();
                iError = iTestAudioPlayer1->GetPosition(pos);
                if(iError != KErrNone)
                    {
                    logger->Log(_L("Test case failed "));
                    CActiveScheduler::Stop();
                    break;
                    }
                if(pos < iPlayer1Pos)
                    {
                    logger->Log(_L("Test case failed "));
                    iError = KErrGeneral;
                    CActiveScheduler::Stop();
                    break;
                    }
                logger->Log(_L("CAudioPreEmptionARN235::test passed "));
                iTestAudioPlayer1->Stop();

                CActiveScheduler::Stop();
                break;
            }
        logger->Log(_L("CAudioPreEmptionARN235::RunTestL %d"), iError);
        }
    return iError;
    }

/**
 * RunTestL
 */
TInt CAudioPreEmptionARN235::RunTestL(CTestModuleIf *aConsole, CStifLogger *aLogger, CStifSectionParser *aParser, TInt* /*clipCounter*/)
    {
    TInt error = KErrNone;
    // Initialise the state variables
    CAudioPreEmptionARN235* self = CAudioPreEmptionARN235::NewL(aConsole,aLogger);
    CleanupStack::PushL(self);
    error = self->ConstructL(aConsole,aLogger,aParser);
    CleanupStack::PopAndDestroy(self);
    aLogger->Log(_L("CAudioPreEmptionARN235::RunTestL %d"), error);
    return error;
    }


//
//CAudioPreEmptionARN236 //test to cover CR id: 
//

/**
 * Constructor
 */
CAudioPreEmptionARN236::CAudioPreEmptionARN236(CTestModuleIf *aConsole, CStifLogger *aLogger)
: CAudioPreEmptionARN(aConsole, aLogger)
                {
                console = aConsole;
                logger = aLogger;
                }

/**
 * NewL
 */
CAudioPreEmptionARN236* CAudioPreEmptionARN236::NewL(CTestModuleIf *aConsole, CStifLogger *aLogger)
    {
    CAudioPreEmptionARN236* self = new (ELeave) CAudioPreEmptionARN236(aConsole, aLogger);
    return self;
    }


/**
 * FsmL
 */
TInt CAudioPreEmptionARN236::FsmL(TMmfAudioPlayerEvents aEventCode, TInt aError)
    {
    if (FsmCheck(aEventCode, aError))
        {
        TTimeIntervalMicroSeconds32 wait2secs(2000000);
        TTimeIntervalMicroSeconds oneNhalf(MAKE_TINT64(0,1500000));
        TTimeIntervalMicroSeconds zeroSecs(MAKE_TINT64(0,0));
        iError = KErrNone;
        switch (iState)
            {
            case EStateBegin:
                // Open iTestAudioPlayer#1 // next event to play iTestAudioPlayer#1
                iTestAudioPlayer1->OpenFileL(iFileName1);
                // Set ExpectedEvent and change the State
                StateChange(EStatePlaying1, EMapcInitComplete1);
                break;
            case EStatePlaying1:
                // Play iTestAudioPlayer#1
                iTestAudioPlayer1->Play();
                // Open iTestAudioPlayer#2
                iTestAudioPlayer2->OpenFileL(iFileName2);
                // Set ExpectedEvent and change the State
                StateChange(EStatePlaying2, EMapcInitComplete2);
                break;
            case EStatePlaying2:
                // Play iTestAudioPlayer#2
                User::After(wait2secs);
                iTestAudioPlayer2->Play();
                // Set ExpectedEvent and change the State
                StateChange(EStateErrInUsePlayer1, EMapcPlayComplete1, KErrInUse);
                break;
            case EStateErrInUsePlayer1:
                iError = iTestAudioPlayer1->GetPosition(iPlayer1Pos);
                if(iError != KErrNone)
                    {
                    logger->Log(_L("Test case failed "));
                    CActiveScheduler::Stop();
                    break;
                    }
                //using 1.5 sec for comparison as sometimes playback is slow.
                if(iPlayer1Pos < oneNhalf)
                    {
                    logger->Log(_L("Test case failed "));
                    iError = KErrGeneral;
                    CActiveScheduler::Stop();
                    break;
                    }
                iTestAudioPlayer1->Stop();
                iError = iTestAudioPlayer1->GetPosition(iPlayer1Pos);
                if(iError != KErrNone)
                    {
                    logger->Log(_L("Test case failed "));
                    CActiveScheduler::Stop();
                    break;
                    }
                if(iPlayer1Pos != zeroSecs)
                    {
                    logger->Log(_L("Test case failed "));
                    iError = KErrGeneral;
                    CActiveScheduler::Stop();
                    break;
                    }
                logger->Log(_L("CAudioPreEmptionARN236::test passed "));
                // Set ExpectedEvent and change the State
                StateChange(EStateEndPlay2, EMapcPlayComplete2 );
                break;
            case EStateEndPlay2:

                // Play iTestAudioPlayer#1 again
                iTestAudioPlayer1->Play();
                User::After(wait2secs);
                iTestAudioPlayer1->Stop();
                CActiveScheduler::Stop();
                // Set ExpectedEvent and change the State
                break;
            }
        logger->Log(_L("CAudioPreEmptionARN236::RunTestL %d"), iError);
        
        }
    return iError;
    }

/**
 * RunTestL
 */
TInt CAudioPreEmptionARN236::RunTestL(CTestModuleIf *aConsole, CStifLogger *aLogger, CStifSectionParser *aParser, TInt* /*clipCounter*/)
    {
    TInt error = KErrNone;
    // Initialise the state variables
    CAudioPreEmptionARN236* self = CAudioPreEmptionARN236::NewL(aConsole,aLogger);
    CleanupStack::PushL(self);
    error = self->ConstructL(aConsole,aLogger,aParser);
    CleanupStack::PopAndDestroy(self);
    aLogger->Log(_L("CAudioPreEmptionARN236::RunTestL %d"), error);
    return error;
    }

TInt CAudioPreEmptionARN236::ConstructL(CTestModuleIf *aConsole, CStifLogger *aLogger, CStifSectionParser *aParser)
    {
    // Initialise the state variables
    StateInit();
    InitializeL();
    iError = DoTestStepPreambleL(aConsole,aLogger,aParser);
    iError = FsmL(EIdlePlayer, KErrNone);
    CActiveScheduler::Start();
    DoTestStepPostambleL();
    Release();
    logger->Log(_L("CAudioPreEmptionARN236::ConstructL %d"), iError);
    return iError;
    }
//
//CAudioPreEmptionARN237 //test to cover CR id: 
//

/**
 * Constructor
 */
CAudioPreEmptionARN237::CAudioPreEmptionARN237(CTestModuleIf *aConsole, CStifLogger *aLogger)
: CAudioPreEmptionARN(aConsole, aLogger)
                {
                console = aConsole;
                logger = aLogger;
                }

/**
 * NewL
 */
CAudioPreEmptionARN237* CAudioPreEmptionARN237::NewL(CTestModuleIf *aConsole, CStifLogger *aLogger)
    {
    CAudioPreEmptionARN237* self = new (ELeave) CAudioPreEmptionARN237(aConsole, aLogger);

    return self;
    }


/**
 * FsmL
 */
TInt CAudioPreEmptionARN237::FsmL(TMmfAudioPlayerEvents aEventCode, TInt aError)
    {
    logger->Log(_L("CAudioPreEmptionARN237::FsmL-starting "));
    if (FsmCheck(aEventCode, aError))
        {
        TTimeIntervalMicroSeconds32 wait1secs(1000000);
        TTimeIntervalMicroSeconds zeroSecs(MAKE_TINT64(0,0));
        iError = KErrNone;
        switch (iState)
            {
            case EStateBegin:
                // Open iTestAudioPlayer#2 // next event to play iTestAudioPlayer#2
                iTestAudioPlayer2->OpenFileL(iFileName1);
                // Set ExpectedEvent and change the State
                StateChange(EStatePlaying2, EMapcInitComplete2);
                break;
            case EStatePlaying2:
                // Play iTestAudioPlayer#1
                iTestAudioPlayer2->Play();
                // Open iTestAudioPlayer#1
                iTestAudioPlayer1->OpenFileL(iFileName2);
                // Set ExpectedEvent and change the State
                StateChange(EStatePlaying1, EMapcInitComplete1);
                break;
            case EStatePlaying1:
                // Play iTestAudioPlayer#1
                User::After(wait1secs);
                iTestAudioPlayer1->Play();
                // Set ExpectedEvent and change the State
                StateChange(EStateErrInUsePlayer1, EMapcPlayComplete1, KErrInUse);
                break;
            case EStateErrInUsePlayer1:
                iError = iTestAudioPlayer1->GetPosition(iPlayer1Pos);
                if(iError != KErrNone)
                    {
                    logger->Log(_L("Test case failed "));
                    CActiveScheduler::Stop();
                    break;
                    }
                if(iPlayer1Pos != zeroSecs)
                    {
                    logger->Log(_L("Test case failed "));
                    iError = KErrGeneral;
                    CActiveScheduler::Stop();
                    break;
                    }
                logger->Log(_L("CAudioPreEmptionARN237::test passed "));
                //stop the iTestAudioPlayer#1 after pre-emption and check if the position is reset.
                iTestAudioPlayer2->Stop();
                iTestAudioPlayer1->Play();

                // Set ExpectedEvent and change the State
                StateChange(EStateEndPlay1, EMapcPlayComplete1 );
                break;
            case EStateEndPlay1:
                // Play iTestAudioPlayer#1 again
                CActiveScheduler::Stop();
                break;
            }
        logger->Log(_L("CAudioPreEmptionARN237::FsmL %d"), iError);
        
        }
    return iError;
    }

/**
 * RunTestL
 */
TInt CAudioPreEmptionARN237::RunTestL(CTestModuleIf *aConsole, CStifLogger *aLogger, CStifSectionParser *aParser, TInt* /*clipCounter*/)
    {
    TInt error = KErrNone;
    // Initialise the state variables
    CAudioPreEmptionARN237* self = CAudioPreEmptionARN237::NewL(aConsole,aLogger);
    CleanupStack::PushL(self);
    error = self->ConstructL(aConsole,aLogger,aParser);
    CleanupStack::PopAndDestroy(self);
    aLogger->Log(_L("CAudioPreEmptionARN237::RunTestL %d"), error);
    return error;
    }

TInt CAudioPreEmptionARN237::ConstructL(CTestModuleIf *aConsole, CStifLogger *aLogger, CStifSectionParser *aParser)
    {
    // Initialise the state variables
    StateInit();
    InitializeL();
    iError = DoTestStepPreambleL(aConsole,aLogger,aParser);
    iError = FsmL(EIdlePlayer, KErrNone);
    CActiveScheduler::Start();
    DoTestStepPostambleL();
    Release();
    logger->Log(_L("CAudioPreEmptionARN237::ConstructL %d"), iError);
    return iError;
    }

TInt CAudioPreEmptionARN235::ConstructL(CTestModuleIf *aConsole, CStifLogger *aLogger, CStifSectionParser *aParser)
    {
    StateInit();
    InitializeL();
    iError = DoTestStepPreambleL(aConsole,aLogger,aParser);

    // Begin the process
    iError = FsmL(EIdlePlayer, KErrNone);
    CActiveScheduler::Start();
    DoTestStepPostambleL();
    Release();
    logger->Log(_L("CAudioPreEmptionARN235::ConstructL %d"), iError);
    return iError;
    }

/**
 * DoTestStepPostambleL
 */
void CAudioPreEmptionARN::DoTestStepPostambleL()
    {
    // Delete all the iTestAudioPlayer utilities.
    logger->Log(_L("Delete all the iTestAudioPlayer utilities"));
    iTestAudioPlayer1->Close();
    delete iTestAudioPlayer1;
    iTestAudioPlayer1=NULL;
    iTestAudioPlayer2->Close();
    delete iTestAudioPlayer2;
    iTestAudioPlayer2=NULL;

    // Delete CPreEmptionARNCallbackSupport
    delete iMdaAudioPlayerCallbackSupport1;
    delete iMdaAudioPlayerCallbackSupport2;
    }

void CAudioPreEmptionARN::InitializeL()
    {
    logger->Log(_L("Creating scheduler"));
    iActiveScheduler = new(ELeave) CActiveScheduler;
    CActiveScheduler::Install(iActiveScheduler);
    }


void CAudioPreEmptionARN::Release()
    {
    logger->Log(_L("Deleting scheduler"));
    delete iActiveScheduler;
    iActiveScheduler = NULL;
    }

CAudioPreEmptionARN::~CAudioPreEmptionARN()
    {
    }

/**
 * DoTestStepPreambleL
 */
TInt CAudioPreEmptionARN::DoTestStepPreambleL(CTestModuleIf* /*aConsole*/, CStifLogger* /*aLogger*/, CStifSectionParser *aParser)
    {
    logger->Log(_L("Initializing all the iTestAudioPlayer utilities"));
    iError = KErrNone;
    TPtrC FileNamePtr1;
    if ( !aParser->GetLine(KFileName1, FileNamePtr1, ENoTag) )
        {
        iFileName1 = FileNamePtr1;
        }
    else
        {
        logger->Log(_L("Invalid file name"));
        return KErrSyntax;
        }
    iMdaAudioPlayerCallbackSupport1 = CPreEmptionARNCallbackSupport::NewL(*this);
    iTestAudioPlayer1 = CMdaAudioPlayerUtility::NewL(*iMdaAudioPlayerCallbackSupport1, EMdaPriorityNormal);
    TPtrC FileNamePtr2;
    if ( !aParser->GetLine(KFileName3, FileNamePtr2, ENoTag) )
        {
        iFileName2 = FileNamePtr2;
        }
    else
        {
        logger->Log(_L("Invalid file name"));
        return KErrSyntax;
        }
    // Initialise 2nd Audio Player
    iMdaAudioPlayerCallbackSupport2 = CPreEmptionARNCallbackSupport::NewL(*this);
    //  CActiveScheduler::Start();
    iTestAudioPlayer2 = CMdaAudioPlayerUtility::NewL(*iMdaAudioPlayerCallbackSupport2, EMdaPriorityMax);

    return iError;
    }
