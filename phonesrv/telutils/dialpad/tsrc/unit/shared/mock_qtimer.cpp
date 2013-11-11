/** Copyright (c) 2010 Nokia Corporation and/or its subsidiary(-ies).
* All rights reserved.
* This component and the accompanying materials are made available
* under the terms of the License "Eclipse Public License v1.0"
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
#include <QDebug>
#include <smcmockclassincludes.h>
#include <qtimer.h>

#include <QTimerEvent>

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// QTimer::QTimer
// -----------------------------------------------------------------------------
//
QTimer::QTimer( 
        QObject * parent )
    //:
    //QObject( /*parent*/ )
    {
    
    }


// -----------------------------------------------------------------------------
// QTimer::~QTimer
// -----------------------------------------------------------------------------
//
QTimer::~QTimer(  )
    {
    
    }


// -----------------------------------------------------------------------------
// QTimer::setInterval
// -----------------------------------------------------------------------------
//
void QTimer::setInterval( 
        int msec )
    {
    SMC_MOCK_METHOD1( void, int, msec )
    }


// -----------------------------------------------------------------------------
// QTimer::singleShot
// -----------------------------------------------------------------------------
//
void QTimer::singleShot( 
        int msec,
        QObject * receiver,
        const char * member )
    {
    SMC_MOCK_METHOD3( void, int, msec, 
        QObject *, receiver, 
        const char *, member )
    }


// -----------------------------------------------------------------------------
// QTimer::start
// -----------------------------------------------------------------------------
//
void QTimer::start( 
        int msec )
    {
    SMC_MOCK_METHOD1( void, int, msec )
    }


// -----------------------------------------------------------------------------
// QTimer::start
// -----------------------------------------------------------------------------
//
void QTimer::start(  )
    {
    SMC_MOCK_METHOD0( void )
    }


// -----------------------------------------------------------------------------
// QTimer::stop
// -----------------------------------------------------------------------------
//
void QTimer::stop(  )
    {
    SMC_MOCK_METHOD0( void )
    }


// -----------------------------------------------------------------------------
// QTimer::timerEvent
// -----------------------------------------------------------------------------
//
void QTimer::timerEvent( 
        QTimerEvent * ev)
    {
    SMC_MOCK_METHOD1( void, QTimerEvent *, ev )
    }


