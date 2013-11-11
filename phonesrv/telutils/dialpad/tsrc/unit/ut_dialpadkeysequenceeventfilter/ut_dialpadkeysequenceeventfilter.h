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

#ifndef UT_DIALPADKEYSEQUENCEEVENTFILTER_H
#define UT_DIALPADKEYSEQUENCEEVENTFILTER_H

#include <QtTest/QtTest>
#include <mockservice.h>

class Dialpad;
class DialpadKeySequenceEventFilter;
class HbLineEdit;

class UT_DialpadKeySequenceEventFilter : public QObject, public MockService
{
    Q_OBJECT

public:    
    
    UT_DialpadKeySequenceEventFilter();
    ~UT_DialpadKeySequenceEventFilter();
    
private slots:

    void init();
    void cleanup();
#ifdef Q_OS_SYMBIAN
    void eventFilterValidKeySequence();
    void eventFilterNotAKeyEvent();
    void eventFilterNotAHashKey();
    void eventFilterNotValidKeySequence();
    void eventFilterServiceRequestFails();
#endif
    
private:
    Dialpad *m_dialPad;
    DialpadKeySequenceEventFilter *m_eventFilter;
    HbLineEdit *m_lineEditMock;
};

#endif  // UT_DIALPADKEYSEQUENCEEVENTFILTER_H
