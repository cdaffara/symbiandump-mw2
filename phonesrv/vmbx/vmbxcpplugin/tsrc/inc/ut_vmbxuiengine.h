/*
* Copyright (c) 2009 - 2010 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  Implementation of the Ut_VmbxUiEngine class
*
*/


#ifndef UT_VMBXUIENGINE_H
#define UT_VMBXUIENGINE_H

#include <QObject>

// Class forwards
class VmbxUiEngine;

static TInt globalExpRet;
static QString globalNumber;

/*!
    Ut_VmbxUiEngine module test class.

    Tests Ut_VmbxUiEngine interface.
    See cpp file for more information.

    @lib Ut_VmbxUiEngine.lib
*/
class Ut_VmbxUiEngine : public QObject
{
    Q_OBJECT

private slots: 

    void initTestCase();
    void cleanupTestCase();
    
    void testCsVoice1Number();
    void testCsVideo1Number();      
    void testIsVideoSupport();
    void testHandleNotifyL();
    
private:  //data

    /*!
        Own.
    */
    VmbxUiEngine *mUiEngine;
};

#endif // _UT_VMBXUIENGINE_H
