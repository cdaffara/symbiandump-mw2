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

#ifndef UT_SATAPPCONFIRMPROVIDER_H
#define UT_SATAPPCONFIRMPROVIDER_H

// includes
#include <QtTest/QtTest>
#include <QObject>


// forward declarations
class SatAppConfirmProvider;



// QTest class
class Ut_SatAppConfirmProvider : public QObject
{
    Q_OBJECT

public:

    Ut_SatAppConfirmProvider(
            QObject *parent = 0);

private slots:

    void testConfirmCommand_data();
    void testConfirmCommand();
    void testConfirmFirstAction();
    void testConfirmSecondAction();
    void testConfirmAndClearScreen();

};


// internal helper class
class ConfirmProviderTest: public QObject
{
    Q_OBJECT

public:

    ConfirmProviderTest();
    ~ConfirmProviderTest();

public slots:

    void simulateFirstActionClicked();
    void simulateSecondActionClicked();
    void simulateClearScreen();
    void cleanup();

public:
    
    // test data
    SatAppConfirmProvider *mConfirm;

};


#endif // UT_SATAPPCONFIRMPROVIDER_H
