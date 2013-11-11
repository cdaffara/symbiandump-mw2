/*
* Copyright (c) 2009 -2010 Nokia Corporation and/or its subsidiary(-ies).
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

#ifndef __LOGGERUTIL_H
#define __LOGGERUTIL_H

//  INCLUDES
#include <QDebug>
#include <QFile>
#include <QDateTime>

/***************************
 * LOGGER SETTINGS
 ***************************/ 

#ifndef _DEBUG

/**
 * Logging method setting:
 * 0 = No logging
 * 1 = File logging, see additional comments later in this file!
 * 2 = Debug output
 */ 
#define _LOGGERUTIL_LOGGING_METHOD      0   // UREL BUILD

#else
    #ifdef __WINS__
        #define _LOGGERUTIL_LOGGING_METHOD      1   // UDEB BUILD, WINS
    #else
        #define _LOGGERUTIL_LOGGING_METHOD      2   // HW UDEB
    #endif // __WINS__
#endif // _DEBUG

// Update this prefix for your component-specific string
#define KDBGLOGPRE "VMBXUI: "   

#if _LOGGERUTIL_LOGGING_METHOD == 1
    #define KLOGGERUTILNEWLINE "\n"   
    // Log file location, file created if path exists
    const QString KLOGGERUTILLOGFILE("c:\\logs\\vmbx\\vmbxui.txt");
#elif _LOGGERUTIL_LOGGING_METHOD == 2
#endif //_LOGGERUTIL_LOGGING_METHOD

/***************************
 * FILE LOGGING IMPLEMENTATION
 ***************************/
#if _LOGGERUTIL_LOGGING_METHOD == 1
    class DbgLoggerUtil
    {
    public:    
        
        // Constructor opens a file and prints newline+timestamp
        //
        // NOTICE the file is not closed if only constructor is run,
        // other method(s) close the file later explicitly.
        //
        // NOTICE that file logging mode uses "append" so the file will grow 
        // indefinitely unless manually deleted. Reason for this is that each 
        // trace line opens&closes the file separately, so overwriting "truncate" 
        // mode would result in the file containing only the latest trace line 
        // and nothing more.
        DbgLoggerUtil() : mFile( KLOGGERUTILLOGFILE ), mDbg( &mFile )
        {
            if ( mFile.open(QFile::WriteOnly | QFile::Append/*Truncate*/) )
            {
                mDbg << KLOGGERUTILNEWLINE << KDBGLOGPRE 
                << QTime::currentTime().toString();
            }           
        }
        /* Debugging method, remove from production code.
        ~DbgLoggerUtil()
            {
            qDebug() << "~DbgLoggerUtil()";
            }
        */
        // This operator writes the argument to file.
        template<typename T>
        DbgLoggerUtil &operator << (const T &aParam1)         
        {
            // File may be already open so check first
            if ( QIODevice::NotOpen == mFile.openMode() )
            {
                /* result = */ mFile.open(
                    QFile::WriteOnly | QFile::Append/*Truncate*/);        
            }
            if ( QIODevice::NotOpen != mFile.openMode() )
            {
                mDbg << aParam1;
            }
            // In order to optimize the file is left open.
            // Close it after logging args is finished.
            return *this;
        }
        
        // This closes the file, dummy parameter not used.
        template<typename T>
        DbgLoggerUtil& operator ^ (const T &/*aParam1*/) 
        {             
            mFile.close(); // Also flushes the write buffer
            return *this;
        }
       
    private:
        Q_DISABLE_COPY(DbgLoggerUtil)
        QFile mFile;
        QDebug mDbg;
    };  
      
// ^ operator used just for closing the file, 0 is a dummy parameter  
#define _DBGLOG(AAA)            DbgLoggerUtil() << AAA ^ 0;
#define _DBGLOG2(AAA,BBB)       DbgLoggerUtil() << AAA << BBB ^ 0;
#define _DBGLOG3(AAA,BBB,CCC)   DbgLoggerUtil() << AAA << BBB << CCC ^ 0;

/***************************
 * DEBUG PORT LOGGING IMPLEMENTATION
 ***************************/
#elif _LOGGERUTIL_LOGGING_METHOD == 2

#define _DBGLOG(AAA)            qDebug() << KDBGLOGPRE << AAA;
#define _DBGLOG2(AAA,BBB)       qDebug() << KDBGLOGPRE << AAA << BBB;
#define _DBGLOG3(AAA,BBB,CCC)   qDebug() << KDBGLOGPRE << AAA << BBB << CCC;

#else    // _LOGGERUTIL_LOGGING_METHOD == 0 or invalid

/***************************
 * EMPTY MACROS LOGGING IS DISABLED
 ***************************/
#define _DBGLOG(AAA)
#define _DBGLOG2(AAA,BBB)
#define _DBGLOG3(AAA,BBB,CCC)

#endif  // _LOGGERUTIL_LOGGING_METHOD

#endif    // __LOGGERUTIL_H
// End of File
