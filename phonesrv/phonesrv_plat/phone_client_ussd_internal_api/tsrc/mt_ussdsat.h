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
* Description:  Implementation of the Mt_UssdSat class
*
*/


#ifndef _MT_USSDSAT_H
#define _MT_USSDSAT_H

#include <QtTest/QtTest>
#include <QString>
#include <QStringList>
#include <QSignalSpy>
#include <QMetaType>
#include <QObject>


// Class forwards
class CPhCltUssdSatClient;

/**
 *  Mt_UssdSat module test class.
 * 
 *  Tests Mt_UssdSat interface.
 *  See cpp file for more information. 
 *
 *  @lib phoneclient.lib
 *  @since S60 <TB10.1>
 */
class Mt_UssdSat : public QObject
{
    Q_OBJECT

public:
    Mt_UssdSat();

    virtual ~Mt_UssdSat();

private slots: 

     /**
     * Called before the first testfunction is executed.
     */
    void initTestCase();

     /**
     * Called after the last testfunction has been executed.
     */
    void cleanupTestCase();

    /**
	* Test send sat message
	*/
    void testSendSatMessage();
    
	/**
	* Test cancel send sat message
	*/    
    void testSendSatMessageCancel();
    
private:  //data

    CPhCltUssdSatClient* mUssdSat;
};

#endif // _MT_USSDSAT_H
