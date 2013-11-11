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
#ifndef SATAPPTONEPROVIDER_H
#define SATAPPTONEPROVIDER_H

// includes
#include <QObject>
#include <QString>
#include <QByteArray>
#include <msatuiobserver.h> //for TSatUiResponse
#include <mdaaudiotoneplayer.h> //MMdaAudioToneObserver
#include <mdaaudiosampleplayer.h> //MMdaAudioPlayerCallback

// forward declarations
class QTimer;
class HbMessageBox;
class CMdaAudioToneUtility;
class CMdaAudioPlayerUtility;
class SatAppAction;


class SatAppToneProvider:
    public QObject,
    public MMdaAudioToneObserver,
    public MMdaAudioPlayerCallback
{
    Q_OBJECT

public:

    SatAppToneProvider(QObject *parent = 0);
    virtual ~SatAppToneProvider();

public slots:

    void playTone(SatAppAction& action);
    
    void clearScreen();

private:

    enum State {Idle,PlayStandardTone,PlayUserSelectedTone};

    State recognizeTone(
        int tone,
        QByteArray& sequence,
        int& duration);
    QByteArray charArrayToByteArray(
        const char tone[],
        int size);
    void playStandardTone(
        const QString& text,
        const QByteArray& sequence,
        int duration);
    void playUserSelectedTone(
        const QString& text,
        int tone,
        int duration);

public slots:
    /**
     * Resets the UI state within the tone provider,
     * commits pending server requests if any
     */
    void resetState();

private slots:

    void stopPlayTone();

// ****************************************************************************
// * ************************************************************************ *
// * *                          SYMBIAN PART                                * *
// * ************************************************************************ *
// ****************************************************************************

private:

    void SymbianPrepareAudioToneUtilityL(const TDesC8& sequence);
    void SymbianStopAudioToneUtility();
    void SymbianPrepareAudioPlayerL(int tone, int duration);
    void SymbianStopAudioPlayer();
    void GetProfileParamsL(
        int tone=ESatToneNotSet, TDes* toneName=NULL );

public: // from MMdaAudioToneObserver

    void MatoPrepareComplete( TInt aError );
    void MatoPlayComplete( TInt aError );

public: //from MMdaAudioPlayerCallback

    void MapcInitComplete( TInt aError,
        const TTimeIntervalMicroSeconds& aDuration );
    void MapcPlayComplete( TInt aError );


private: // Data

    State mState;
    SatAppAction* mCurrentAction;

    bool mWarningAndPlayTones;
    int mVolume;
    CMdaAudioToneUtility *mPlayer;
    TSatUiResponse mPlayToneError;
    QTimer *mTimer;
    HbMessageBox *mPermanentNote;
    CMdaAudioPlayerUtility *mAudioPlayer;

    // for unit testing
    friend class ToneProviderTest;
};

#endif //SATAPPTONEPROVIDER_H
