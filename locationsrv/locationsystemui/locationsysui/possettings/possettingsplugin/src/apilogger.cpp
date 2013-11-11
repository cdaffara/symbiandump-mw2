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


#include "apilogger.h"
#include <qdatetime.h>

const char* debugFileName("c://logs//lbs//posSettingsEngineLog.txt");
ofstream logfile;

void ApiLogger::OpenLogFile() 
{   
    logfile.open(debugFileName, ios::app);   
}

void ApiLogger::CloseLogFile() 
{
    logfile.flush();
    logfile.close();
}

void ApiLogger::MyOutputHandler(QtMsgType type, const char *msg) 
{    
    switch (type) {
        case QtDebugMsg:
            logfile << QTime::currentTime().toString().toAscii().data() << " Debug: " << msg << "\n";            
            break;
        case QtCriticalMsg:
            logfile << QTime::currentTime().toString().toAscii().data() << " Critical: " << msg << "\n";
            break;
        case QtWarningMsg:
           // logfile << QTime::currentTime().toString().toAscii().data() << " Warning: " << msg << "\n";
            break;
        case QtFatalMsg:
            logfile << QTime::currentTime().toString().toAscii().data() << " Fatal: " << msg << "\n";
        default:
            break;
    }
    logfile.flush();
}
