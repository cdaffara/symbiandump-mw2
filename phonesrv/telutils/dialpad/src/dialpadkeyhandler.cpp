/*!
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
* Description: Custom button
*
*/

#include <hbmainwindow.h>
#ifdef Q_OS_SYMBIAN
#include <featmgr.h>
#endif // Q_OS_SYMBIAN
#include "dialpad.h"
#include "dialpadkeyhandler.h"
#include "dialpadvoicemailboxeventfilter.h"
#include "dialpadvideomailboxeventfilter.h"
#include "dialpadbluetootheventfilter.h"
#include "dialpadkeysequenceeventfilter.h"
#include "dialpademergencycalleventfilter.h"
#include "dialpadhasheventfilter.h"
#ifdef _DEBUG
 #ifdef __WINSCW__
  #include "dialpadinternaleventfilter.h"
 #endif
#endif
#include "qtphonesrvlog.h"

DialpadKeyHandler::DialpadKeyHandler(
    Dialpad *dialPad, HbMainWindow& mainWindow, QObject *parent) 
    : 
    QObject(parent),
	mMainWindow(mainWindow),
	mVmbxFilter(0),
	mVideoVmbxFilter(0),
	mBtFilter(0),
	mKeySequenceFilter(0),
	mEmergencyCallFilter(0),
	mHashFilter(0)
{
    PHONE_TRACE;

    // Variations
    bool isVideoMailboxSupported = false;
#ifdef Q_OS_SYMBIAN
    isVideoMailboxSupported = FeatureManager::FeatureSupported(KFeatureIdCsVideoTelephony);
#endif // Q_OS_SYMBIAN
    
    mVmbxFilter.reset(new DialpadVoiceMailboxEventFilter(dialPad));
    if (isVideoMailboxSupported) {
        mVideoVmbxFilter.reset(new DialpadVideoMailboxEventFilter(dialPad));
    }
    mBtFilter.reset(new DialpadBluetoothEventFilter(dialPad));
    mKeySequenceFilter.reset(new DialpadKeySequenceEventFilter(dialPad));
    mHashFilter.reset(new DialpadHashEventFilter(dialPad));
    
#ifdef _DEBUG
 #ifdef __WINSCW__ 
    mInternalFilter.reset(new DialpadInternalEventFilter);
    mMainWindow.installEventFilter(mInternalFilter.data());
 #endif
#endif

    // Stack different event filters
    mMainWindow.installEventFilter(mVmbxFilter.data());
    if (isVideoMailboxSupported) {
        mMainWindow.installEventFilter(mVideoVmbxFilter.data());    
    }
    mMainWindow.installEventFilter(mBtFilter.data());
    mMainWindow.installEventFilter(mKeySequenceFilter.data());
    mMainWindow.installEventFilter(mHashFilter.data());
}


DialpadKeyHandler::DialpadKeyHandler(
    Dialpad *dialPad, 
    DialpadKeyHandler::DialpadKeyEventFilters filters, 
    QObject *parent) 
    : 
    QObject(parent),
    mMainWindow(*(dialPad->mainWindow())),
    mVmbxFilter(0),
    mVideoVmbxFilter(0),
    mBtFilter(0),
    mKeySequenceFilter(0),
    mEmergencyCallFilter(0),
    mHashFilter(0)
{
    PHONE_TRACE;
    
    if (filters.testFlag(VoiceMailbox)) {
        mVmbxFilter.reset(new DialpadVoiceMailboxEventFilter(dialPad));
        mMainWindow.installEventFilter(mVmbxFilter.data());
    }
    
    if (filters.testFlag(VideoMailBox)) {
        mVideoVmbxFilter.reset(
            new DialpadVideoMailboxEventFilter(dialPad));
        mMainWindow.installEventFilter(mVideoVmbxFilter.data());
    }
    
    if (filters.testFlag(Bluetooth)) {
        mBtFilter.reset(new DialpadBluetoothEventFilter(dialPad));
        mMainWindow.installEventFilter(mBtFilter.data());
    }
    
    if (filters.testFlag(KeySequence)) {
        mKeySequenceFilter.reset(
            new DialpadKeySequenceEventFilter(dialPad));
        mMainWindow.installEventFilter(mKeySequenceFilter.data());
    }
    
    if (filters.testFlag(EmergencyCall)) {
        mEmergencyCallFilter.reset(
            new DialpadEmergencyCallEventFilter(dialPad));
        mMainWindow.installEventFilter(mEmergencyCallFilter.data());
    }
    
    if (filters.testFlag(Hash)) {
        mHashFilter.reset(new DialpadHashEventFilter(dialPad));
        mMainWindow.installEventFilter(mHashFilter.data());
    }
}


DialpadKeyHandler::~DialpadKeyHandler()
{
    mMainWindow.removeEventFilter(mVmbxFilter.data());
    mMainWindow.removeEventFilter(mVideoVmbxFilter.data());
    mMainWindow.removeEventFilter(mBtFilter.data());
    mMainWindow.removeEventFilter(mKeySequenceFilter.data());
    mMainWindow.removeEventFilter(mEmergencyCallFilter.data());
    mMainWindow.removeEventFilter(mHashFilter.data());
#ifdef _DEBUG
 #ifdef __WINSCW__
    mMainWindow.removeEventFilter(mInternalFilter.data());
 #endif
#endif
}
