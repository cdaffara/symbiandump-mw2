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
#include "dialpadsymbianwrapper.h"

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// DialpadSymbianWrapper::DialpadSymbianWrapper
// -----------------------------------------------------------------------------
//
DialpadSymbianWrapper::DialpadSymbianWrapper( 
        QObject * parent ) : d_ptr(0)
    //:
    //QObject( /*parent*/ )
    {
    
    }


// -----------------------------------------------------------------------------
// DialpadSymbianWrapper::~DialpadSymbianWrapper
// -----------------------------------------------------------------------------
//
DialpadSymbianWrapper::~DialpadSymbianWrapper(  )
    {
    
    }


// -----------------------------------------------------------------------------
// DialpadSymbianWrapper::getMailboxNumber
// -----------------------------------------------------------------------------
//
int DialpadSymbianWrapper::getMailboxNumber( 
        QString & vmbxNumber )
    {
    SMC_MOCK_METHOD1( int, QString &, vmbxNumber )
    }


// -----------------------------------------------------------------------------
// DialpadSymbianWrapper::getVideoMailboxNumber
// -----------------------------------------------------------------------------
//
int DialpadSymbianWrapper::getVideoMailboxNumber( 
        QString & vmbxNumber )
    {
    SMC_MOCK_METHOD1( int, QString &, vmbxNumber )
    }


// -----------------------------------------------------------------------------
// DialpadSymbianWrapper::defineMailboxNumber
// -----------------------------------------------------------------------------
//
int DialpadSymbianWrapper::defineMailboxNumber( 
        QString & vmbxNumber )
    {
    SMC_MOCK_METHOD1( int, QString &, vmbxNumber )
    }


// -----------------------------------------------------------------------------
// DialpadSymbianWrapper::defineVideoMailboxNumber
// -----------------------------------------------------------------------------
//
int DialpadSymbianWrapper::defineVideoMailboxNumber( 
        QString & vmbxNumber )
    {
    SMC_MOCK_METHOD1( int, QString &, vmbxNumber )
    }


// -----------------------------------------------------------------------------
// DialpadSymbianWrapper::changeSilentModeState
// -----------------------------------------------------------------------------
//
bool DialpadSymbianWrapper::changeSilentModeState(  )
    {
    SMC_MOCK_METHOD0( bool )
    }


