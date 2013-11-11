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
* Description:
*
*/

#ifndef UT_PSETNETWORKINFOCONVERTER_H
#define UT_PSETNETWORKINFOCONVERTER_H

#include <QtTest/QtTest>
#include <mockservice.h>


class UT_PSetNetworkInfoConverter : public QObject, public MockService
{
    Q_OBJECT

public:    
    UT_PSetNetworkInfoConverter();
    ~UT_PSetNetworkInfoConverter();
    
private slots:

    void init();
    void cleanup();
    
    void t_convertToSymbianTypeNetworkInfo();
    void t_convertToSymbianTypeNetworkId();
    void t_convertToSymbianTypeNetworkStatus();
    void t_convertToSymbianTypeNetworkSelectionMode();
    void t_convertToSymbianTypeNetworkAccessType();
    void t_convertToQtTypeNetworkSelectionMode();
};

#endif  // UT_PSETNETWORKINFOCONVERTER_H
