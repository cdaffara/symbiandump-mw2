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
#include "dialpadvtkeyhandler.h"
#include "dialpadkeysequenceeventfilter.h"
#include "dialpademergencycalleventfilter.h"
#include "qtphonesrvlog.h"

DialpadVtKeyHandler::DialpadVtKeyHandler(
    Dialpad *dialPad, HbMainWindow& mainWindow, QObject *parent) 
    : 
    QObject(parent),
    mMainWindow(mainWindow),
    mEmergencyCallFilter(0),
    mKeySequenceFilter(0)
{
    PHONE_TRACE;
    mEmergencyCallFilter = new DialpadEmergencyCallEventFilter(dialPad, this);
    mKeySequenceFilter = new DialpadKeySequenceEventFilter(dialPad, this);
    
    // Stack different event filters
    mMainWindow.installEventFilter(mKeySequenceFilter);
    mMainWindow.installEventFilter(mEmergencyCallFilter);
}

DialpadVtKeyHandler::~DialpadVtKeyHandler()
{
}
