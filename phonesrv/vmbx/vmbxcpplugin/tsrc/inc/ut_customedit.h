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
* Description:  Implementation of the Ut_vmbxCpPlugin class
*
*/


#ifndef UT_CUSTOMEDIT_H
#define UT_CUSTOMEDIT_H

#include <QObject>
#include <QtTest/QtTest>

class Ut_CustomEdit : public QObject
{
    Q_OBJECT
    
private slots:
    void testFocusInEvent();
    
};

#endif // UT_CUSTOMEDIT_H
