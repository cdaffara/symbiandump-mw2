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
* Description:  This class provides function that help in logging entry and exit of APIs of classes
*
*/


#include "apilogger.h"
#include <qdatetime.h>

#ifdef POSINDICATOR_NFT
const char* debugFileName("c://logs//lbs//posindicatornftlog.txt");
#else
const char* debugFileName("c://logs//lbs//posindicatorlog.txt");
#endif

ofstream logfile;
QFile file( debugFileName );

void ApiLogger::OpenLogFile() 
{   
#ifdef POSINDICATOR_NFT
    file.open( QIODevice::ReadWrite | QIODevice::Text ) ;
#else
    logfile.open(debugFileName, ios::app);
#endif
}

void ApiLogger::CloseLogFile() 
{
#ifdef POSINDICATOR_NFT
    file.close();
#else
    logfile.flush();
    logfile.close();
#endif    
    
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



void ApiLogger::NftLogger( const QString msg )
    {
    QString displayString( msg );	
    #ifdef POSINDICATOR_NFT
    QTextStream stream( &file );
    QString display( QTime::currentTime().toString("h:m:s:z") );
    display.append( displayString );
    stream << display;
    stream.flush();
    #endif
    }
    
