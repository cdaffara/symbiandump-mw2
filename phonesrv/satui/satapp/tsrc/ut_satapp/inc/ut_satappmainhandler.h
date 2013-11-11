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

#ifndef UT_SATAPPMAINHANDLER_H
#define UT_SATAPPMAINHANDLER_H

#include <QtTest/QtTest>
#include <QObject>


// Class forwards
class HbMainWindow;
class SatAppMainHandler;

/**
 *  Satapp unit test class.
 * 
 *  @since S60 <TB10.1>
 */
class Ut_SatAppMainHandler : public QObject
{
    Q_OBJECT

public:

    Ut_SatAppMainHandler(
            HbMainWindow* mainWindow, QObject *parent = 0);

    virtual ~Ut_SatAppMainHandler();

private slots:
     /**
     * Called before the first testfunction is executed.
     * Handles the SetUpMenu command.
     */
    void initTestCase();

     /**
     * Called after the last testfunction has been executed.
     * 
     */
    void cleanupTestCase();

     /**
     * Test SatAppMainHandler's constructor.
     * 
     */
    void testCreateMainHandler();
    
    /**
     * Test SatAppMainHandler's constructor in offline.
     * 
     */
    void testCreateMainHandler_OfflineMode();

private:
    
    HbMainWindow* mMainWindow; // not own
    SatAppMainHandler *mMainHandler;

};

#endif // UT_SATAPPMAINHANDLER_H
