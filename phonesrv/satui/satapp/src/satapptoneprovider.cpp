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
* Description:
*
*/

// includes
#include <QTimer>
#include <QCoreApplication>
#include <QtDebug>
#include <hbmessagebox.h>//playtone note

#include "SatAppToneProvider.h"
#include "satappconstant.h"
#include "satappaction.h"

// ----------------------------------------------------------------------------
// SatAppToneProvider::SatAppToneProvider
// ----------------------------------------------------------------------------
//
SatAppToneProvider::SatAppToneProvider(QObject *parent) :
    QObject(parent)
{
    qDebug("SATAPP: SatAppToneProvider::SatAppToneProvider <>");
    mState = Idle;
    mWarningAndPlayTones = false;
    mVolume = 0;
    mPlayer = 0;
    mPlayToneError = ESatSuccess;
    mTimer = 0;
    mPermanentNote = 0;
    mAudioPlayer = 0;
    mCurrentAction = 0;
}

// ----------------------------------------------------------------------------
// SatAppToneProvider::~SatAppToneProvider
// ----------------------------------------------------------------------------
//
SatAppToneProvider::~SatAppToneProvider()
{
    qDebug("SATAPP: SatAppToneProvider::~SatAppToneProvider >");
    
    if (mCurrentAction) {
        stopPlayTone();
    }
    
    if (mPermanentNote) {
        qDebug("SatAppToneProvider::resetState delete mPermanentNote");
        delete mPermanentNote;
        mPermanentNote = 0;
    }
    
    if (mTimer) {
        qDebug("SatAppToneProvider::resetState delete timer");
        delete mTimer;
        mTimer = 0;
    }
    
    qDebug("SATAPP: SatAppToneProvider::~SatAppToneProvider <");
}

//-----------------------------------------------------------------------------
// SatAppToneProvider::playTone
//-----------------------------------------------------------------------------
void SatAppToneProvider::playTone(SatAppAction& action)
{
    qDebug("SATAPP: SatAppToneProvider::playTone >");
    resetState();
    
    mCurrentAction = &action;
    int tone = action.value(KeyToneId).toInt();
    int duration = action.value(KeyDuration).toInt();
    QString text = action.value(KeyText).toString();
    QByteArray sequence;
    mState = recognizeTone(tone,sequence,duration);
    if (mState == PlayStandardTone)
    {
        playStandardTone(text,sequence,duration);
    }
    else if (mState == PlayUserSelectedTone)
    {
        playUserSelectedTone(text,tone,duration);
    }
    else
    {
        mCurrentAction->complete(ESatCmdDataNotUnderstood);
        mCurrentAction = 0;
    }
    qDebug("SATAPP: SatAppToneProvider::playTone <");
}

//-----------------------------------------------------------------------------
// SatAppToneProvider::recognizeTone
// @param tone the tone code from server (see TSatTone in MSatUiObserver)
// @param sequence (out) standard tone play sequence
// @param duration (out) stanrard tone play duration (manipulated in some cases)
// @return suggested state for the tone provider (or Idle if tone is not recognised)
//-----------------------------------------------------------------------------
SatAppToneProvider::State SatAppToneProvider::recognizeTone(
    int tone,
    QByteArray& sequence,
    int& duration)
{
    qDebug("SATAPP: SatAppToneProvider::recognizeTone >");
    State ret = Idle;
    qDebug("SATAPP: SatAppToneProvider::recognizeTone tone: %d", tone);
    switch (tone)
    {
        case ESatGeneralBeep:
        case ESatPositiveTone:
        case ESatToneNotSet:
            {
            if (duration == 0) duration = KSatDur170ms; // 170ms
            sequence = charArrayToByteArray(KGeneralBeep, 
                                     sizeof(KGeneralBeep));
            ret = PlayStandardTone;
            break;
            }
        case ESatNegativeTone:
            {
            if (duration == 0) duration = KSatDur250ms; // 250ms
            sequence = charArrayToByteArray(KErrorTone, 
                                     sizeof(KErrorTone));
            ret = PlayStandardTone;
            break;
            }
        case ESatRadioPathNotAvailableCallDropped:
            {
            if (duration == 0) duration = KSatDur1200ms; // 1200ms
            sequence = charArrayToByteArray(KRadioPathNotAvailable, 
                                     sizeof(KRadioPathNotAvailable));
            ret = PlayStandardTone;
            break;
            }
        case ESatRadioPathAcknowledge:
            {
            if (duration == 0) duration = KSatDur200ms; // 200ms
            sequence = charArrayToByteArray(KRadioPathAck, 
                                     sizeof(KRadioPathAck));
            ret = PlayStandardTone;
            break;
            }
        case ESatDialTone:
            {
            if (duration == 0) break;
            sequence = charArrayToByteArray(KNetToneSeqDial, 
                                     sizeof(KNetToneSeqDial));
            ret = PlayStandardTone;
            break;
            }
        case ESatCalledSubscriberBusy:
            {
            if (duration == 0) break;
            sequence = charArrayToByteArray(KNetToneSeqNetBusy, 
                                     sizeof(KNetToneSeqNetBusy));
            ret = PlayStandardTone;
            break;
            }
        case ESatCongestion:
            {
            if (duration == 0) break;
            sequence = charArrayToByteArray(KNetToneSeqNetCongestion, 
                                     sizeof(KNetToneSeqNetCongestion));
            ret = PlayStandardTone;
            break;
            }
        case ESatErrorSpecialInfo:
            {
            if (duration == 0) break;
            sequence = charArrayToByteArray(KNetToneSeqNetSpecialInformation, 
                                     sizeof(KNetToneSeqNetSpecialInformation));
            ret = PlayStandardTone;
            break;
            }
        case ESatCallWaitingTone:
            {
            if (duration == 0) break;
            sequence = charArrayToByteArray(KNetToneSeqNetCallWaiting, 
                                     sizeof(KNetToneSeqNetCallWaiting));
            ret = PlayStandardTone;
            break;
            }
        case ESatRingingTone:
            {
            if (duration == 0) break;
            sequence = charArrayToByteArray(KNetToneSeqRingGoing, 
                                     sizeof(KNetToneSeqRingGoing));
            ret = PlayStandardTone;
            break;
            }
        case ESatUserSelectedToneIncomingSpeech:
        case ESatUserSelectedToneIncomingSms:
            {
            ret = PlayUserSelectedTone;
            break;
            }
        default:
            break;
    }
    
    qDebug("SATAPP: SatAppToneProvider::recognizeTone state=%d <", ret);
    return ret;
}

//-----------------------------------------------------------------------------
// SatAppToneProvider::charArrayToByteArray
//-----------------------------------------------------------------------------
QByteArray SatAppToneProvider::charArrayToByteArray(
    const char tone[], 
    int size)
{
    qDebug("SATAPP: SatAppToneProvider::charArrayToByteArray >");
    QByteArray seq;
    seq = QByteArray::fromRawData(tone, size);
    qDebug("SATAPP: SatAppToneProvider::charArrayToByteArray <");
    return seq;
}

//-----------------------------------------------------------------------------
// SatAppToneProvider::playStandardTone
//-----------------------------------------------------------------------------
void SatAppToneProvider::playStandardTone(
    const QString& text,
    const QByteArray& sequence,
    int duration)
{
    qDebug("SATAPP: SatAppToneProvider::playStandardTone >");

    if (text.length() > 0) {
        mPermanentNote = new HbMessageBox(HbMessageBox::MessageTypeInformation);
        SAT_ASSERT(connect(mPermanentNote, SIGNAL(aboutToClose()),
                this, SLOT(stopPlayTone())));
        mPermanentNote->setText(text);
        mPermanentNote->setStandardButtons(HbMessageBox::Cancel);
        if (duration > 0 ) {
            mPermanentNote->setTimeout(duration);
        }
        mPermanentNote->setDismissPolicy(HbPopup::TapAnywhere);
        qDebug("SatAppToneProvider::playStandardTone show before");
        mPermanentNote->show();
        qDebug("SatAppToneProvider::playStandardTone show after");
    }

    TPtrC8 seq((unsigned char*)sequence.constData(), sequence.length());
    qDebug("SATAPP: SatAppToneProvider::playStandardTone TPtrC8 seq: %S", &seq);
    QT_TRAP_THROWING(SymbianPrepareAudioToneUtilityL(seq));

    if (duration > 0) {
        mTimer = new QTimer(this);
        SAT_ASSERT(connect(mTimer, SIGNAL(timeout()),
                this, SLOT(stopPlayTone())));
        mTimer->start(duration);
    }
    
    qDebug("SATAPP: SatAppToneProvider::playStandardTone <");
}

//-----------------------------------------------------------------------------
// SatAppToneProvider::playUserSelectedTone
//-----------------------------------------------------------------------------
void SatAppToneProvider::playUserSelectedTone(
    const QString& text,
    int tone,
    int duration)
{
    qDebug("SatAppToneProvider::playUserSelectedTone >");

    // If several messages/calls arrive during a small amount of time and if the
    // message received or incoming call tone is already being played we do not 
    // restart it.
    if (mAudioPlayer) {
        qDebug( "SatAppToneProvider::\
            PlayUserSelectedToneL Error - already played" );
        mCurrentAction->complete(ESatFailure);
        mCurrentAction = 0;
        mState = Idle;
        return;
    }

    if (text.length() > 0) {
        mPermanentNote = new HbMessageBox(HbMessageBox::MessageTypeInformation);
        mPermanentNote->setText(text);
        mPermanentNote->setStandardButtons(HbMessageBox::Cancel);
        if (duration) {
            mPermanentNote->setTimeout(duration);
        }
        mPermanentNote->setDismissPolicy(HbPopup::TapAnywhere);
        qDebug("SatAppToneProvider::PlayUserSelectedToneL show before");
        mPermanentNote->show();
        qDebug("SatAppToneProvider::PlayUserSelectedToneL show after");
    }

    QT_TRAP_THROWING(SymbianPrepareAudioPlayerL(tone,duration));
    
    if (duration > 0) {
        qDebug( "SATAPP: SatAppToneProvider::PlayUserSelectedToneL\
            duration not 0" );
        mTimer = new QTimer(this);
        mTimer->start(duration);
        SAT_ASSERT(connect(mTimer, SIGNAL(timeout()), this, SLOT(stopPlayTone())));

        if (mPermanentNote) {
            SAT_ASSERT(connect(mPermanentNote, SIGNAL(aboutToClose()),
                    this, SLOT(stopPlayTone())));
        }
    }
    
    qDebug( "SatAppToneProvider::PlayUserSelectedToneL <" );
}


// ----------------------------------------------------------------------------
// SatAppToneProvider::clearScreen
// terminates all ongoing UI actions
// ----------------------------------------------------------------------------
//
void SatAppToneProvider::clearScreen()
{
    qDebug( "SatAppToneProvider::clearScreen >" );
    if (mCurrentAction) {
        stopPlayTone();
    }
    qDebug( "SatAppToneProvider::clearScreen <" );
}

// ----------------------------------------------------------------------------
// SatAppToneProvider::stopPlayTone
// Stops the music, frees blocked functions
// ----------------------------------------------------------------------------
//
void SatAppToneProvider::stopPlayTone()
{
    qDebug("SATAPP: SatAppToneProvider::stopPlayTone >");
    
    if (mPermanentNote) {
        qDebug("SatAppToneProvider::stopPlayTone close mPermanentNote");
        disconnect(mPermanentNote,SIGNAL(aboutToClose()),
                this, SLOT(stopPlayTone())); 
        mPermanentNote->close();
    }
    
    if (mTimer) {
        qDebug("SatAppToneProvider::stopPlayTone stop timer");
        disconnect(mTimer,SIGNAL(timeout()), this, SLOT(stopPlayTone()));
        if (mTimer->isActive()) {
            mTimer->stop();
        }
    }
    
    switch(mState) {
        case PlayStandardTone: {
            qDebug("SATAPP: SatAppToneProvider::stopPlayTone before play cancel");
            SymbianStopAudioToneUtility();

            //get warning and game tones setting from active profile
            QT_TRAP_THROWING(GetProfileParamsL());
            if( EFalse == mWarningAndPlayTones 
                && ESatSuccess == mPlayToneError ) {
                mPlayToneError = ESatSuccessToneNotPlayed;
            }
            break;
        }
        case PlayUserSelectedTone: {
            // Sample is played in forever loop for the duration.
            // After duration call back timers are destroyed       
            // If player exists, stop playing and delete player. MapcPlayComplete
            // is not called because CallBackTimer stops the playing.
            SymbianStopAudioPlayer();
            if( EFalse == mWarningAndPlayTones ) {
                mPlayToneError = ESatSuccessToneNotPlayed;
            }
            break;
        }
        default:
            break;
    }
    
    mState = Idle;
    if (mCurrentAction) {
        mCurrentAction->complete(mPlayToneError);
        mCurrentAction = 0;
    }
    
    qDebug("SATAPP: SatAppToneProvider::stopPlayTone <");
}

void SatAppToneProvider::resetState()
{
    qDebug("SATAPP: SatAppToneProvider::resetState >");
    if (mCurrentAction) {
        mCurrentAction->completeWithNoResponse();
        mCurrentAction = 0;
    }
    
    if (mPermanentNote) {
        qDebug("SatAppToneProvider::resetState delete mPermanentNote");
        delete mPermanentNote;
        mPermanentNote = 0;
    }
    
    if (mTimer) {
        qDebug("SatAppToneProvider::resetState delete timer");
        delete mTimer;
        mTimer = 0;
    }
    qDebug("SATAPP: SatAppToneProvider::resetState <");
}



// ****************************************************************************
// * ************************************************************************ *
// * *                          SYMBIAN PART                                * *
// * ************************************************************************ *
// ****************************************************************************


// includes
#include    <e32std.h> //TTimeIntervalMicroSeconds
#include    <MProfile.h>
#include    <MProfileEngine.h>
#include    <MProfileTones.h>
#include    <TProfileToneSettings.h>
#include    <AudioPreference.h> //KAudioPriorityLowLevel
#include    <mdaaudiotoneplayer.h> // for CMdaAudioToneUtility&CMdaAudioPlayerUtility
#include    <mda/common/resource.h> //KMdaRepeatForever

//constants

// Maximum length for sound file. 
const TInt KMaxSoundFileLength = 256;
// Used when dividing some values. Used only for preventing the use of magic 
// numbers
// Audio sample is repeated indefinitely.
const TInt KSoundPlayerRepeatForever = KMdaRepeatForever;

// The max volume value from settings.
// From TProfileToneSettings.h
const TInt KMaxVolumeFromProfile( 10 );

// ----------------------------------------------------------------------------
// SatAppToneProvider::SymbianPrepareAudioToneUtilityL
// creates and prepares CMdaAudioToneUtility
// ----------------------------------------------------------------------------
//
void SatAppToneProvider::SymbianPrepareAudioToneUtilityL(const TDesC8& sequence)
{
    qDebug( "SatAppToneProvider::SymbianPrepareAudioToneUtilityL >" );
    if (mPlayer) {
        delete mPlayer;
        mPlayer = 0;
    }
    mPlayer = CMdaAudioToneUtility::NewL(*this);
    TInt volume(mPlayer->MaxVolume());
    mPlayer->SetVolume(volume);
    mPlayer->PrepareToPlayDesSequence(sequence);
    qDebug( "SatAppToneProvider::SymbianPrepareAudioToneUtilityL <" );
}

// ----------------------------------------------------------------------------
// SatAppToneProvider::SymbianStopAudioToneUtility
// cancels and deletes CMdaAudioToneUtility
// ----------------------------------------------------------------------------
void SatAppToneProvider::SymbianStopAudioToneUtility()
{
    qDebug( "SatAppToneProvider::SymbianStopAudioToneUtility >" );
    if (mPlayer) {
        mPlayer->CancelPlay();
    }
    delete mPlayer;
    mPlayer = NULL;
    qDebug( "SatAppToneProvider::SymbianStopAudioToneUtility <" );
}

// ----------------------------------------------------------------------------
// SatAppToneProvider::SymbianPrepareAudioPlayerL
// cancels and deletes CMdaAudioToneUtility
// ----------------------------------------------------------------------------
void SatAppToneProvider::SymbianPrepareAudioPlayerL(
    int tone, int duration)
{
    qDebug( "SatAppToneProvider::SymbianPrepareAudioPlayerL >" );
    // This defines name for the current message alert or incoming call tone. 
    TBuf<KMaxSoundFileLength> soundFileName;
    GetProfileParamsL(tone, &soundFileName);

    // This defines the behaviour to be adopted by an
    // audio client if a higher priority client takes over the device.
    TInt mdaPriority( KAudioPrefIncomingCall );
    // This is used to resolve conflicts when more than one client tries to 
    // access the same sound output device simultaneously.
    TInt audioPriority( KAudioPriorityPhoneCall );

    qDebug( "SATAPP: SatAppToneProvider::PlayUserSelectedToneL\
        Volume is %d ", mVolume );

    // Creates the audio player.
    mAudioPlayer = CMdaAudioPlayerUtility::NewFilePlayerL( 
        soundFileName,
        *this,
        audioPriority,
        ( TMdaPriorityPreference ) mdaPriority );

    TTimeIntervalMicroSeconds no_silence(0);
    if (duration > 0)
    {
        // repeat forever in a loop
        mAudioPlayer->SetRepeats(KSoundPlayerRepeatForever, no_silence);
    }
    else
    {
        // play only once
        mAudioPlayer->SetRepeats(0, no_silence);
    }
    qDebug( "SatAppToneProvider::SymbianPrepareAudioPlayerL <" );

}

// ----------------------------------------------------------------------------
// SatAppToneProvider::SymbianStopAudioPlayer
// cancels and deletes CMdaAudioToneUtility
// ----------------------------------------------------------------------------
void SatAppToneProvider::SymbianStopAudioPlayer()
{
    qDebug( "SatAppToneProvider::SymbianStopAudioPlayer >" );
    if (mAudioPlayer) {
        qDebug( "SatAppToneProvider::SymbianStopAudioPlayer AudioPlayer stop" );
        mAudioPlayer->Stop();
    }
    delete mAudioPlayer;
    mAudioPlayer = 0;
    qDebug( "SatAppToneProvider::SymbianStopAudioPlayer <" );
}

// ---------------------------------------------------------
// SatAppCommandHandler::GetProfileParamsL
// Get and store settings from current profile 
// (other items were commented in a header).
// ---------------------------------------------------------
//
void SatAppToneProvider::GetProfileParamsL(
    int aTone /*= ESatToneNotSet*/,
    TDes* aToneName /*= NULL*/ )
{
    qDebug( "SatAppCommandHandler::GetProfileParamsL >" );
    MProfile* profile = NULL;
    MProfileEngine* engine = NULL;
    engine = CreateProfileEngineL();
    CleanupReleasePushL( *engine );

    profile = engine->ActiveProfileLC();

    const MProfileTones& tones = profile->ProfileTones();

    // Takes a current warning and gametones setting.
    const TProfileToneSettings& ts = tones.ToneSettings();
    mWarningAndPlayTones = ts.iWarningAndGameTones;
    mVolume = ts.iRingingVolume;
    
    qDebug( "SatAppCommandHandler::GetProfileParamsL \
        mVolume before mapping %d", mVolume );
    
    // Max volume from profile is KMaxVolumeFromProfile, Max volume from 
    // CMdaAudioToneUtility is different, maybe 10,000. So, 
    // rate = maxVolumeFromPlayer / KMaxVolumeFromProfile
    // User may never hear the TONE, because volume is too small.
    // iVolume times the rate make it can be heard.
    
    CMdaAudioToneUtility* toneUtl = CMdaAudioToneUtility::NewL( *this );
    TInt maxVolumeFromPlayer( toneUtl->MaxVolume() );
    mVolume *= maxVolumeFromPlayer / KMaxVolumeFromProfile;
    delete toneUtl;
    toneUtl = NULL;
    qDebug( "CSatUiViewAppUi::GetProfileParamsL \
        mVolume after mapping %d", mVolume );
    
    if ( ( ESatUserSelectedToneIncomingSms == aTone ) && ( aToneName ) )
        {
        qDebug("SatAppCommandHandler::GetProfileParamsL message tone");
        aToneName->Copy( tones.MessageAlertTone() );
    } else if( aToneName ) {
        qDebug("SatAppCommandHandler::GetProfileParamsL ring tone");
        aToneName->Copy( tones.RingingTone1() );
    }

    CleanupStack::PopAndDestroy(2); // engine, profile
    qDebug("SatAppCommandHandler::GetProfileParamsL <");
}

// ---------------------------------------------------------
// SatAppToneProvider::MatoPrepareComplete
// Preparation is complete -> Starts playing tone
// ---------------------------------------------------------
//
void SatAppToneProvider::MatoPrepareComplete(TInt aError)
{
    qDebug("SatAppToneProvider::MatoPrepareComplete \
        aError = %d >", aError);
    if (KErrNone == aError && mPlayer) {
        mPlayToneError = ESatSuccess;
        TTimeIntervalMicroSeconds zero(static_cast<TInt64>( 0 ));
        mPlayer->SetPriority(KAudioPriorityLowLevel,
               STATIC_CAST(TMdaPriorityPreference, KAudioPrefConfirmation));
        mPlayer->SetRepeats( KMdaAudioToneRepeatForever, zero );
        mPlayer->Play();
    } else {
        mPlayToneError = ESatFailure;
        stopPlayTone();
    }
    qDebug( "SatAppToneProvider::MatoPrepareComplete <" );
}

//-----------------------------------------------------------------------------
// SatAppToneProvider::MatoPlayComplete
// Play complete -> Frees blocking function
// (other items were commented in a header).
// ----------------------------------------------------------------------------
//
void SatAppToneProvider::MatoPlayComplete( TInt aError )
{
    qDebug("SatAppToneProvider::MatoPlayComplete >");
    if (KErrNone == aError && mPlayer) {
        qDebug("SatAppToneProvider::MatoPlayComplete play");
        mPlayer->Play();
    } else {
        qDebug( "SatAppToneProvider::MatoPlayComplete stop playing");
        // Stops playing if error.
        stopPlayTone();
    }
    qDebug("SatAppToneProvider::MatoPlayComplete error %d <", aError);
}

//-----------------------------------------------------------------------------
// SatAppToneProvider::MapcInitComplete
// Initialisation complete -> starts playing the tone.
// ----------------------------------------------------------------------------
void SatAppToneProvider::MapcInitComplete( TInt aError, 
    const TTimeIntervalMicroSeconds& /*aDuration*/ )
{
    qDebug( "SatAppToneProvider::MapcInitComplete >" );
    // Audio player is initialised, so if there is no error, we can start 
    // playing the tone.
    if (KErrNone == aError && mAudioPlayer) {
        const TInt volume( Max(0, Min(mVolume, 
                           mAudioPlayer->MaxVolume())));
        qDebug("CSatUiViewAppUi::\
            MapcInitComplete SetVolume %d", volume );
        // Set volume according Settings. 
        mAudioPlayer->SetVolume( volume );
        mAudioPlayer->Play();
        qDebug( "SatAppToneProvider::MapcInitComplete Play" );
    } else {
        // Otherwise, we delete the initialised audio player.
        if ( mAudioPlayer ) {
            qDebug( "SatAppToneProvider::MapcInitComplete mAudioPlayer \
                          true" );
            delete mAudioPlayer;
            mAudioPlayer = 0;
        }

        // Remove also the note for play tone
        // If there was a duration for play tone, stop waiting
        stopPlayTone();

        qDebug( "SatAppToneProvider::MapcInitComplete Error %d", aError );
    }
    qDebug( "SatAppToneProvider::MapcInitComplete <" );
}

//-----------------------------------------------------------------------------
// SatAppToneProvider::MapcPlayComplete
// Deletes audio player after playing is completed.
// ----------------------------------------------------------------------------
void SatAppToneProvider::MapcPlayComplete( TInt /*aError*/ )
{
    qDebug( "SatAppToneProvider::MapcPlayComplete >" );

    // When playing is completed, deleting the audio player.
    if ( mAudioPlayer ) {
        qDebug("SatAppToneProvider::MapcPlayComplete delete mAudioPlayer");
        delete mAudioPlayer;
        mAudioPlayer = 0;
    }
    // Remove note after play has completed.
    stopPlayTone();
    qDebug( "SatAppToneProvider::MapcPlayComplete <" );
}


//End of file
