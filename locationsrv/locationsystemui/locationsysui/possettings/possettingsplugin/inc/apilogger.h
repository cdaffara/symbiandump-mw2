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
* Description:  This class provides function that help in logging entry and exit of APIs of classes
*
*/

#ifndef APILOGGER_H
#define APILOGGER_H

#include <iostream>
#include <fstream> 
#include <QDebug>
#include <qfile.h>
#include <qtextstream.h>

using namespace std;


class ApiLogger
    {
    public:
    static void OpenLogFile();
    static void CloseLogFile();
    static void MyOutputHandler(QtMsgType type, const char *msg);
    };
    
#endif // APILOGGER_H
