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

#ifndef UT_SATAPPTONEPROVIDER_H
#define UT_SATAPPTONEPROVIDER_H

#include <QtTest/QtTest>
#include <QObject>
#include <msatuiobserver.h>


// Class forwards
class SatAppToneProvider;

// class declaration
class Ut_SatAppToneProvider : public QObject
{
    Q_OBJECT

public:

    Ut_SatAppToneProvider(QObject *parent = 0);

private slots:

    void testCreateProvider_data();
    void testCreateProvider();
    void testClearScreen();
    
    void testPlayTone_data();
    void testPlayTone();
    void testplayDoubleTone_data();
    void testplayDoubleTone();
    void testPlayToneAndClearScreen();

    void testMatoPrepareComplete_data();
    void testMatoPrepareComplete();
//    void testMatoPlayComplete_data();
//    void testMatoPlayComplete();
//    void testMapcPlayComplete_data();
//    void testMapcPlayComplete();
    void testMapcInitComplete_data();
    void testMapcInitComplete();
    void testRecognizeTone_data();
    void testRecognizeTone();
    void testPrepareAudioToneUtility();

};

// internal helper class
class ToneProviderTest: public QObject
{
    Q_OBJECT

public:

    ToneProviderTest();
    ~ToneProviderTest();
    static SatAppToneProvider* getToneInstance();
    bool isMessageBoxShown();
    bool isTimerActive();
    void ConstructAudioTone();
    void DestroyAudioTone();
    void ConstructAudioPlayer();
    void DestroyAudioPlayer();
    void ConstructTimer(bool active);
    void DestroyTimer();
    void SetPlayToneResult(TSatUiResponse error);
    void TestRecognizeTone(int tone, int duration);

public slots:

    void cleanup();

public:
    
    // test data
    SatAppToneProvider *mTone;

};

#endif // UT_SATAPPTONEPROVIDER_H
