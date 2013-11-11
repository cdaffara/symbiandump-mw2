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
#include "dialpadvideomailboxeventfilter.h"

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// DialpadVideoMailboxEventFilter::DialpadVideoMailboxEventFilter
// -----------------------------------------------------------------------------
//
DialpadVideoMailboxEventFilter::DialpadVideoMailboxEventFilter( 
        Dialpad * dialpad,
        QObject * parent )
    :
    DialpadMailboxEventFilterBase( dialpad, parent )
    {
    
    }


// -----------------------------------------------------------------------------
// DialpadVideoMailboxEventFilter::~DialpadVideoMailboxEventFilter
// -----------------------------------------------------------------------------
//
DialpadVideoMailboxEventFilter::~DialpadVideoMailboxEventFilter(  )
    {
    
    }


// -----------------------------------------------------------------------------
// DialpadVideoMailboxEventFilter::eventFilter
// -----------------------------------------------------------------------------
//
bool DialpadVideoMailboxEventFilter::eventFilter( 
        QObject * watched,
        QEvent * event )
    {
    SMC_MOCK_METHOD2( bool, QObject *, watched, 
        QEvent *, event )
    }


// -----------------------------------------------------------------------------
// DialpadVideoMailboxEventFilter::handleCallButtonPress
// -----------------------------------------------------------------------------
//
bool DialpadVideoMailboxEventFilter::handleCallButtonPress(  )
    {
    SMC_MOCK_METHOD0( bool )
    }


// -----------------------------------------------------------------------------
// DialpadVideoMailboxEventFilter::handleMailboxOperation
// -----------------------------------------------------------------------------
//
void DialpadVideoMailboxEventFilter::handleMailboxOperation(  )
    {
    SMC_MOCK_METHOD0( void )
    }


// -----------------------------------------------------------------------------
// DialpadVideoMailboxEventFilter::handleLongKeyPress
// -----------------------------------------------------------------------------
//
void DialpadVideoMailboxEventFilter::handleLongKeyPress(  )
    {
    SMC_MOCK_METHOD0( void )
    }


