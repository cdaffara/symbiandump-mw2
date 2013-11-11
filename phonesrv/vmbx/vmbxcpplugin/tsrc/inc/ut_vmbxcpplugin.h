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
* Description:  Implementation of the Ut_vmbxCpPlugin class
*
*/


#ifndef UT_VMBXCPPLUGIN_H
#define UT_VMBXCPPLUGIN_H

// System includes
#include <QtTest/QtTest>
#include <QObject>

/*!
    Ut_vmbxCpPlugin module test class.

    Tests Ut_vmbxCpPlugin interface.
    See cpp file for more information.

    @lib Ut_vmbxCpPlugin.lib
*/

// Class declaration
class Ut_vmbxCpPlugin : public QObject
{
    Q_OBJECT

private slots: 

    /*!
        Test method in VmbxCpPlugin
    */
    void testCreateSettingFormItemData();
    
    /*!
        Test method in VmbxCpGroup
    */    
    void testCreateVmbxCpGroup();
};

#endif // _UT_VMBXCPPLUGIN_H
