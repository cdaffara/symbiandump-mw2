/*
* Copyright (c) 2010 Nokia Corporation and/or its subsidiary(-ies).
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
* Description: Unit test for satappmainhandler
*
*/

#include <QTimer>
#include "ut_satapptoneprovider.h"
#include "satapptoneprovider.h"
#include "satappaction.h"

// -----------------------------------------------------------------------------
// Ut_SatAppToneProvider::Ut_SatAppToneProvider
// -----------------------------------------------------------------------------
//
Ut_SatAppToneProvider::Ut_SatAppToneProvider(QObject *parent):QObject(parent) 
{
}

// -----------------------------------------------------------------------------
// Ut_SatAppToneProvider::testCreateProvider
// 
// Connects to test object signal and verifies received data.
// -----------------------------------------------------------------------------
void Ut_SatAppToneProvider::testCreateProvider_data()
{
    QTest::addColumn<bool>("timer");
    QTest::newRow("t01") << false;
    QTest::newRow("t01") << true;
}

// -----------------------------------------------------------------------------
// Ut_SatAppToneProvider::testCreateProvider
// 
// Connects to test object signal and verifies received data.
// -----------------------------------------------------------------------------
void Ut_SatAppToneProvider::testCreateProvider()
{
    QFETCH(bool, timer);
    ToneProviderTest test;
    if(timer){
        test.ConstructTimer(false);
    }
    QVERIFY(test.mTone); 
}

// -----------------------------------------------------------------------------
// Ut_SatAppToneProvider::testClearScreen
// 
// Connects to test object signal and verifies received data.
// -----------------------------------------------------------------------------
void Ut_SatAppToneProvider::testClearScreen()
{
    ToneProviderTest test;
    test.mTone->clearScreen();
    QVERIFY(!test.isMessageBoxShown());
    QVERIFY(!test.isTimerActive());
}


// -----------------------------------------------------------------------------
// Ut_SatAppToneProvider::testPlayTone_data
// 
// Connects to test object signal and verifies received data.
// -----------------------------------------------------------------------------
void Ut_SatAppToneProvider::testPlayTone_data()
{
    QTest::addColumn<QString>("text");
    QTest::addColumn<int>("tone");
    QTest::addColumn<int>("duration");
    QTest::addColumn<int>("exp_result");

    QTest::newRow("t01") << "Standard" << 0x01 << 0 << 0x32; // exp result changed
    QTest::newRow("t02") << "Standard" << 0x02 << 100 << 0;
    QTest::newRow("t03") << "Standard" << 0x03 << 1000 << 0;
    QTest::newRow("t04") << "Standard" << 0x04 << 0 << 0;
    QTest::newRow("t05") << "Standard" << 0x05 << 100 << 0;
    QTest::newRow("t06") << "Standard" << 0x07 << 1000 << 0;
    QTest::newRow("t07") << "" << 0x07 << 100 << 0;
    QTest::newRow("t08") << "" << 0x08 << 500 << 0;
    QTest::newRow("t09") << "" << 0x09 << 1000 << 0x32; // exp result changed
    QTest::newRow("t10") << "User" << 0x13 << 1000 << 0;
    QTest::newRow("t11") << "User" << 0x14 << 500 << 0;
    QTest::newRow("t12") << "Illegal" << 0x99 << 100 << 0x32;
}

// -----------------------------------------------------------------------------
// Ut_SatAppToneProvider::testPlayTone
// -----------------------------------------------------------------------------
void Ut_SatAppToneProvider::testPlayTone()
{
    QFETCH(QString, text);
    QFETCH(int, tone);
    QFETCH(int, duration);
    QFETCH(int, exp_result);

    ToneProviderTest test;
    TSatUiResponse resp(ESatSuccess);
    
    SatAppAction action(ESatPlayToneAction);
    action.set(KeyToneId,tone);
    action.set(KeyDuration,duration);
    action.set(KeyText,text);
    test.mTone->playTone(action);
    action.waitUntilCompleted();
    resp = action.response();

    QCOMPARE((int)resp, exp_result);
}

// -----------------------------------------------------------------------------
// Ut_SatAppToneProvider::testplayStandardTone_data
// 
// Connects to test object signal and verifies received data.
// -----------------------------------------------------------------------------
void Ut_SatAppToneProvider::testplayDoubleTone_data()
{
    QTest::addColumn<QString>("text");
    QTest::addColumn<int>("tone");
    QTest::addColumn<int>("duration");
    QTest::addColumn<bool>("timer");
    QTest::addColumn<bool>("active");

    QTest::newRow("t01") << "Standard" << 0x10 << 0 << false << false;
    QTest::newRow("t02") << "Standard" << 0x10 << 0 << true << true;
    QTest::newRow("t03") << "Standard" << 0x10 << 0 << true << false;
    QTest::newRow("t04") << "Standard" << 0x13 << 0 << false << false;
    QTest::newRow("t04") << "Standard" << 0x14 << 0 << true << true;
    QTest::newRow("t04") << "" << 0x13 << 0 << true << false;

}

// -----------------------------------------------------------------------------
// Ut_SatAppToneProvider::testplayStandardTone
// -----------------------------------------------------------------------------
void Ut_SatAppToneProvider::testplayDoubleTone()
{
    QFETCH(QString, text);
    QFETCH(int, tone);
    QFETCH(int, duration);
    QFETCH(bool, timer);
    QFETCH(bool, active);

    ToneProviderTest test;
    TSatUiResponse resp(ESatSuccess);
    if(timer){
        test.ConstructTimer(active);
    }
    
    SatAppAction action(ESatPlayToneAction);
    action.set(KeyToneId,tone);
    action.set(KeyDuration,duration);
    action.set(KeyText,text);
    test.mTone->playTone(action);
    action.waitUntilCompleted();
    resp = action.response();
}

// -----------------------------------------------------------------------------
// Ut_SatAppToneProvider::testPlayToneAndClearScreen
// -----------------------------------------------------------------------------
void Ut_SatAppToneProvider::testPlayToneAndClearScreen()
{
    ToneProviderTest test;

    // timer for clearing screen after 1 sec.
    QTimer::singleShot(1000, test.mTone, SLOT(clearScreen()));
    
    // start playing
    TSatUiResponse result(ESatFailure);
    SatAppAction action(ESatPlayToneAction);
    action.set(KeyToneId,0x01);
    action.set(KeyDuration,3000);
    action.set(KeyText,QString("play"));
    test.mTone->playTone(action);
    action.waitUntilCompleted();
    result = action.response();

//    QCOMPARE(result, ESatSuccessToneNotPlayed); TODO:which?
    QCOMPARE(result, ESatSuccess);
}

// -----------------------------------------------------------------------------
// Ut_SatAppToneProvider::testMatoPlayComplete_data
// 
// Connects to test object signal and verifies received data.
// -----------------------------------------------------------------------------
void Ut_SatAppToneProvider::testMatoPrepareComplete_data()
{
    QTest::addColumn<int>("error");
    QTest::addColumn<bool>("tone");
    QTest::newRow("error_0") << 0 << true;
    QTest::newRow("error_-1") << -1 << false;
    QTest::newRow("error_-2") << 0 << false;
}

// -----------------------------------------------------------------------------
// Ut_SatAppToneProvider::testMatoPlayComplete
// 
// Connects to test object signal and verifies received data.
// -----------------------------------------------------------------------------
void Ut_SatAppToneProvider::testMatoPrepareComplete()
{
    QFETCH(int, error);
    QFETCH(bool, tone);
    ToneProviderTest test;

    if(tone)
        test.ConstructAudioTone();

    test.mTone->MatoPrepareComplete(error);
    
    test.DestroyAudioTone();
}

//// -----------------------------------------------------------------------------
//// Ut_SatAppToneProvider::testMatoPlayComplete_data
//// 
//// Connects to test object signal and verifies received data.
//// -----------------------------------------------------------------------------
//void Ut_SatAppToneProvider::testMatoPlayComplete_data()
//{
//    QTest::addColumn<int>("error");
//    QTest::addColumn<bool>("tone");
//    QTest::newRow("error_0") << 0 << true;
//    QTest::newRow("error_-1") << -1 << false;
//    QTest::newRow("error_-2") << 0 << false;
//}
//
//// -----------------------------------------------------------------------------
//// Ut_SatAppToneProvider::testMatoPlayComplete
//// 
//// Connects to test object signal and verifies received data.
//// -----------------------------------------------------------------------------
//void Ut_SatAppToneProvider::testMatoPlayComplete()
//{
//    QFETCH(int, error);
//    QFETCH(bool, tone);
//    ToneProviderTest test;
//
//    if(tone)
//        test.ConstructAudioTone();
//
//    test.mTone->MatoPrepareComplete(error);
//    test.mTone->MatoPlayComplete(error);
//    
//    test.DestroyAudioTone();
//}
//
//// -----------------------------------------------------------------------------
//// Ut_SatAppToneProvider::testMapcPlayComplete_data
//// 
//// Connects to test object signal and verifies received data.
//// -----------------------------------------------------------------------------
//void Ut_SatAppToneProvider::testMapcPlayComplete_data()
//{
//    QTest::addColumn<bool>("player");
//    QTest::newRow("t_true") << true;
//    QTest::newRow("t_false") << false;
//}
//
//// -----------------------------------------------------------------------------
//// Ut_SatAppToneProvider::testMapcPlayComplete
//// 
//// Connects to test object signal and verifies received data.
//// -----------------------------------------------------------------------------
//void Ut_SatAppToneProvider::testMapcPlayComplete()
//{
//    QFETCH(bool, player);
//    TInt error = 0;
//    ToneProviderTest test;
//    
//    if(player)
//        test.ConstructAudioPlayer();
//    
//    test.mTone->MapcPlayComplete(error);
//    
//    test.DestroyAudioPlayer();
//}

// -----------------------------------------------------------------------------
// Ut_SatAppToneProvider::testMapcInitComplete_data
// 
// Connects to test object signal and verifies received data.
// -----------------------------------------------------------------------------
void Ut_SatAppToneProvider::testMapcInitComplete_data()
{
    QTest::addColumn<int>("error");
    QTest::addColumn<bool>("player");
    QTest::newRow("error_0") << 0 << false;
    QTest::newRow("error_-6") << -6 << true;
    QTest::newRow("error_-6") << -6 << false;
}

// -----------------------------------------------------------------------------
// Ut_SatAppToneProvider::testMapcInitComplete
// 
// Connects to test object signal and verifies received data.
// -----------------------------------------------------------------------------
void Ut_SatAppToneProvider::testMapcInitComplete()
{
    QFETCH(int, error);
    QFETCH(bool, player);
    ToneProviderTest test;
    
    if(player)
        test.ConstructAudioPlayer();
    
    test.mTone->MapcInitComplete(error, 0);
    
    test.DestroyAudioPlayer();
}

// -----------------------------------------------------------------------------
// Ut_SatAppToneProvider::testRecognizeTone_data
// 
// Connects to test object signal and verifies received data.
// -----------------------------------------------------------------------------
void Ut_SatAppToneProvider::testRecognizeTone_data()
{
    QTest::addColumn<int>("tone");
    QTest::addColumn<int>("duration");

    QTest::newRow("t01") << -1 << 0;
    QTest::newRow("t01") << -1 << 1;
    QTest::newRow("t01") << 0x01 << 0;
    QTest::newRow("t01") << 0x01 << 1;
    QTest::newRow("t01") << 0x02 << 0;
    QTest::newRow("t01") << 0x02 << 1;
    QTest::newRow("t01") << 0x03 << 0;
    QTest::newRow("t01") << 0x03 << 1;
    QTest::newRow("t01") << 0x04 << 0;
    QTest::newRow("t01") << 0x04 << 1;
    QTest::newRow("t01") << 0x05 << 0;
    QTest::newRow("t01") << 0x05 << 1;
    QTest::newRow("t01") << 0x06 << 0;
    QTest::newRow("t01") << 0x06 << 1;
    QTest::newRow("t01") << 0x07 << 0;
    QTest::newRow("t01") << 0x07 << 1;
    QTest::newRow("t01") << 0x08 << 0;
    QTest::newRow("t01") << 0x08 << 1;
    QTest::newRow("t01") << 0x09 << 0;
    
    QTest::newRow("t01") << 0x10 << 0;
    QTest::newRow("t01") << 0x10 << 1;
    QTest::newRow("t01") << 0x11 << 0;
    QTest::newRow("t01") << 0x11 << 1;
    QTest::newRow("t01") << 0x12 << 0;
    QTest::newRow("t01") << 0x12 << 1;
    QTest::newRow("t02") << 0x13 << 0;
    
    QTest::newRow("t02") << 0x14 << 0;
    
    
}

// -----------------------------------------------------------------------------
// Ut_SatAppToneProvider::testRecognizeTone
// 
// Connects to test object signal and verifies received data.
// -----------------------------------------------------------------------------
void Ut_SatAppToneProvider::testRecognizeTone()
{
    QFETCH(int, tone);
    QFETCH(int, duration);
    
    ToneProviderTest test;
    test.TestRecognizeTone(tone, duration);
}

// -----------------------------------------------------------------------------
// Ut_SatAppToneProvider::testPrepareAudioToneUtility
// 
// Connects to test object signal and verifies received data.
// -----------------------------------------------------------------------------
void Ut_SatAppToneProvider::testPrepareAudioToneUtility()
{
    ToneProviderTest test;
    test.ConstructAudioTone();
    test.ConstructAudioTone();
}

// ---------------------------------------------------------------
// internal helper class
// ---------------------------------------------------------------

ToneProviderTest::ToneProviderTest()
{
    mTone = new SatAppToneProvider(this);
}

ToneProviderTest::~ToneProviderTest()
{
    cleanup();
}

/**
 * Singleton Pattern - not used
 */
SatAppToneProvider* ToneProviderTest::getToneInstance()
{

}

bool ToneProviderTest::isMessageBoxShown()
{
    if (mTone->mPermanentNote) {
        return true;
    }
    return false;
}

bool ToneProviderTest::isTimerActive()
{
    if (mTone->mTimer && mTone->mTimer->isActive()) {
        return true;
    }
    return false;
}

void ToneProviderTest::ConstructAudioTone()
{
    QByteArray sequence;
    sequence = KNetToneSeqDial;
    TPtrC8 seq((unsigned char*)sequence.constData(),sequence.length());
    if(mTone)
        QT_TRAP_THROWING(mTone->SymbianPrepareAudioToneUtilityL(seq));
}

void ToneProviderTest::DestroyAudioTone()
{
    if(mTone)
        mTone->SymbianStopAudioToneUtility();
}

void ToneProviderTest::ConstructAudioPlayer()
{
    int tone = ESatToneNotSet;
    int duration = 0;
    if(mTone)
        QT_TRAP_THROWING(mTone->SymbianPrepareAudioPlayerL(tone,duration));
}

void ToneProviderTest::DestroyAudioPlayer()
{
    if(mTone)
        mTone->SymbianStopAudioPlayer();
}

void ToneProviderTest::ConstructTimer(bool active)
{
    mTone->mTimer = new QTimer(mTone);
    connect(mTone->mTimer, SIGNAL(timeout()),
            mTone, SLOT(stopPlayTone()));
    if(active)
        mTone->mTimer->start(3000);
}

void ToneProviderTest::DestroyTimer()
{
    
}

void ToneProviderTest::SetPlayToneResult(TSatUiResponse error)
{
    mTone->mPlayToneError = error;
}

void ToneProviderTest::TestRecognizeTone(int tone, int duration)
{
    QByteArray sequence;
    mTone->recognizeTone(tone, sequence, duration);
}

void ToneProviderTest::cleanup()
{
    delete mTone;
    mTone = 0;
}

// ****************************************************************************
// * ************************************************************************ *
// * *                      SYMBIAN MOC PART                                * *
// * ************************************************************************ *
// ****************************************************************************


// includes
#include <e32cmn.h>
#include <MProfile.h>
#include <MProfileEngine.h>
#include <MProfileTones.h>
#include <TProfileToneSettings.h>
#include <mdaaudiotoneplayer.h> //MMdaAudioToneObserver

class CMocProfileTones : public MProfileTones
    {
    public:
        CMocProfileTones() 
            {
            // Set TProfileToneSettings with specified value
            iToneSettings.iWarningAndGameTones = EFalse;
            TBuf<256> ringTone(_L("Z:\\Data\\Sounds\\Digital\\Nokia tune.aac"));
            TBuf<256> alertTone(_L("Z:\\Data\\Sounds\\Digital\\Message 1.aac"));
            iTone = ringTone;
            }
        virtual ~CMocProfileTones() {}

    public:
        virtual const TDesC& RingingTone1() const 
            {
            //DONE
            return iTone;
            }
        virtual const TDesC& RingingTone2() const 
            {
            //DONE
            return iTone;
            }
        virtual const TDesC& MessageAlertTone() const 
            {
            //DONE
            return iTone;
            }
        virtual const TProfileToneSettings& ToneSettings() const 
            {
            //DONE: Return TProfileToneSettings
            return iToneSettings;
            }
    private:
        TProfileToneSettings iToneSettings;
        TBuf<256> iTone;
    };

class CMocProFile : public MProfile
    {
    public:
        static CMocProFile* NewLC()
            {
            CMocProFile* self = new( ELeave ) CMocProFile;
            CleanupStack::PushL( self );
            self->ConstructL();
            return self;
            }
        virtual ~CMocProFile() {}
    protected:
        CMocProFile() {}
        void ConstructL()
            {
            iTones = new(ELeave) CMocProfileTones;
            }

    public: // New functions
        virtual void Release() {}
        virtual const TArray<TContactItemId> AlertForL() {}
        virtual TBool IsSilent() const { return EFalse; }
        virtual const MProfileName& ProfileName() const {}
        virtual const MProfileTones& ProfileTones() const 
            {
            //DONE: return an MProfileTones object
            return *iTones;
            }
        virtual const MProfileExtraTones& ProfileExtraTones() const {}
        virtual const MProfileExtraSettings& ProfileExtraSettings() const {}
        
    private:
        MProfileTones* iTones;
    };

class CMocProfileEngine : public MProfileEngine
    {
    public:
        CMocProfileEngine() {};
        virtual ~CMocProfileEngine() {};
    public:
        virtual void Release() {};
        virtual MProfile* ActiveProfileLC()
            {
            //DONE: Create an MProfile object
            MProfile* pf = CMocProFile::NewLC();
            return pf;
            }
        virtual MProfile* ActiveProfileL() { return NULL; };
        virtual TInt ActiveProfileId() { return 0; };
        virtual MProfilesNamesArray* ProfilesNamesArrayLC() { return NULL; };
        virtual void SetActiveProfileL( TInt /*aId*/ ) {};
        virtual void SetTempRingingVolumeL( TProfileRingingVolume /*aVolume*/ ) {};
        virtual TProfileRingingVolume TempRingingVolumeL() const {
            return EProfileRingingVolumeLevel1; 
        }
        virtual void SetTempMediaVolumeL( TProfileRingingVolume /*aVolume*/ ) {};
        virtual TProfileRingingVolume TempMediaVolumeL() const {
            return EProfileRingingVolumeLevel1; 
        }
        virtual TBool IsFeatureSupported(
            TProfileFeatureId /*aFeatureId*/ ) const { return EFalse; };
        virtual TBool IsActiveProfileTimedL() { return EFalse; };

    };

// ----------------------------------------------------------------------------
// CreateProfileEngineL moc
// create profile engine
// ----------------------------------------------------------------------------
//
//MProfileEngine* CreateProfileEngineL()
//    {
//    
//    MProfileEngine* pe = new( ELeave ) CMocProfileEngine;
//    return pe;
//    }

// End of file

