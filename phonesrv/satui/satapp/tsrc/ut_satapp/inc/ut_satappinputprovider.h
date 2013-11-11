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

#ifndef UT_SATAPPINPUTPROVIDER_H
#define UT_SATAPPINPUTPROVIDER_H

// includes
#include <QtTest/QtTest>
#include <QObject>


// forward declarations
class SatAppInputProvider;



// QTest class
class Ut_SatAppInputProvider : public QObject
{
    Q_OBJECT

public:

    Ut_SatAppInputProvider(
            QObject *parent = 0);

private slots:

    void testResetState();

    void testGetInkeyAndOk_data();
    void testGetInkeyAndOk();
    void testGetInkeyAndCancel_data();
    void testGetInkeyAndCancel();
    void testGetInkeyAndTimeout();
    void testGetInkeyAndReset();
    void testGetInkeyAndDelete();

    void testGetInkeyImmediateAndOk();
    void testGetInkeyImmediateAndCancel();
    
    void testGetInputAndOk_data();
    void testGetInputAndOk();
    void testGetInputAndCancel_data();
    void testGetInputAndCancel();
    void testGetInputAndTimeout();
    void testGetInputAndReset();
    void testGetInputAndDelete();

private:


};


// internal helper class
class InputProviderTest: public QObject
{
    Q_OBJECT

public:

    InputProviderTest();
    ~InputProviderTest();

public slots:

    void simulateFirstActionClicked();
    void simulateSecondActionClicked();
    void simulateImmediateKeypress(QString s);
    void cleanup();

public:

    
    // test data
    SatAppInputProvider *mInput;

};


#endif // UT_SATAPPINPUTPROVIDER_H
