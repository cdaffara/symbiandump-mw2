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

#ifndef UT_SATAPPPOPUPPROVIDER_H
#define UT_SATAPPPOPUPPROVIDER_H

// includes
#include <QtTest/QtTest>
#include <QObject>


// forward declarations
class SatAppPopupProvider;



// QTest class
class Ut_SatAppPopupProvider : public QObject
{
    Q_OBJECT

public:

    Ut_SatAppPopupProvider(
            QObject *parent = 0);

private slots:


    void testDisplayTextAndOk_data();
    void testDisplayTextAndOk();
    
    void testDisplayTextAndCancel_data();
    void testDisplayTextAndCancel();
  
    void testDisplayTextAndTimeout();
    
    void testNotificationWithCancel_data();
    void testNotificationWithCancel();
  
    void testNotification_data();
    void testNotification();

};


// internal helper class
class PopupProviderTest: public QObject
{
    Q_OBJECT

public:

    PopupProviderTest();
    ~PopupProviderTest();

public slots:

    bool simulateOkClicked();
    bool simulateCancelClicked();
    void cleanup();

public:
    
    // test data
    SatAppPopupProvider *mPopup;

};


#endif // UT_SATAPPPOPUPPROVIDER_H
