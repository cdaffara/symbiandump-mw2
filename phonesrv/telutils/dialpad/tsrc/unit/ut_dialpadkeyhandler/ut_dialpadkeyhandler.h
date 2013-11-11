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

#ifndef UT_DIALPADKEYHANDLER_H
#define UT_DIALPADKEYHANDLER_H

#include <QtTest/QtTest>
#include <mockservice.h>

class Dialpad;
class DialpadKeyHandler;

class UT_DialpadKeyHandler : public QObject, public MockService
{
    Q_OBJECT

public:    
    
    UT_DialpadKeyHandler();
    ~UT_DialpadKeyHandler();
    
private slots:

    void init();
    void cleanup();
    
    void t_constructionWithAllFilters();
    void t_constructionWithNoFilters();
    void t_deprecatedConstructionWithVideoMailbox();
    void t_deprecatedConstructionWithoutVideoMailbox();
    
private:
    Dialpad *m_dialPad;
    DialpadKeyHandler *m_keyHandler;
};

#endif  // UT_DIALPADKEYHANDLER_H
