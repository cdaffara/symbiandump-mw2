/*
* Copyright (c) 2009 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  Implementation of the Mt_Ussd class
*
*/


#ifndef _MT_USSD_H
#define _MT_USSD_H

#include <QtTest/QtTest>
#include <QString>
#include <QStringList>
#include <QSignalSpy>
#include <QMetaType>
#include <QObject>


// Class forwards
class CPhCltUssd;

/**
 *  Mt_Ussd module test class.
 * 
 *  Tests Mt_Ussd interface.
 *  See cpp file for more information. 
 *
 *  @lib phoneclient.lib
 *  @since S60 <TB10.1>
 */
class Mt_Ussd : public QObject
{
    Q_OBJECT

public:
    Mt_Ussd();

    virtual ~Mt_Ussd();

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
	* Test SendUssd with Unicode string
	*/
    void testSendUssdUnicode();
    
	/**
	* Test sendUssd
	*/    
    void testSendUssd();
    
	/**
	* Test sendUssd with given DCS
	*/    
    void testSendUssdWithDCS();
    
	/**
	* Test sendUssd by BTN string
	*/    
    void testSendUssdBtn();

	/**
	* Test sendUssd by CMCC string
	*/    
    void testSendUssdCmcc();    
    
	
private:  //data

    CPhCltUssd* mUssd;
};

#endif // _MT_VMBXENGINE_H
