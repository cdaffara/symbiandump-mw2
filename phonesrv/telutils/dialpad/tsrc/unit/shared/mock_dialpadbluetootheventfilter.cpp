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
#include <QDebug>
#include <QEvent>
#include <smcmockclassincludes.h>
#include "dialpadbluetootheventfilter.h"

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// DialpadBluetoothEventFilter::DialpadBluetoothEventFilter
// -----------------------------------------------------------------------------
//
DialpadBluetoothEventFilter::DialpadBluetoothEventFilter( 
        Dialpad * dialpad,
        QObject * parent )
    :
    QObject( parent )
    {
    Q_UNUSED(dialpad)
    }


// -----------------------------------------------------------------------------
// DialpadBluetoothEventFilter::~DialpadBluetoothEventFilter
// -----------------------------------------------------------------------------
//
DialpadBluetoothEventFilter::~DialpadBluetoothEventFilter(  )
    {
    
    }


// -----------------------------------------------------------------------------
// DialpadBluetoothEventFilter::eventFilter
// -----------------------------------------------------------------------------
//
bool DialpadBluetoothEventFilter::eventFilter( 
        QObject * watched,
        QEvent * event )
    {
    SMC_MOCK_METHOD2( bool, QObject *, watched, 
        QEvent *, event )
    }


// -----------------------------------------------------------------------------
// DialpadBluetoothEventFilter::toggleBluetooth
// -----------------------------------------------------------------------------
//
void DialpadBluetoothEventFilter::toggleBluetooth()
{
    SMC_MOCK_METHOD0( void )
}
