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
#include "dialpadvoicemailboxeventfilter.h"

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// DialpadVoiceMailboxEventFilter::DialpadVoiceMailboxEventFilter
// -----------------------------------------------------------------------------
//
DialpadVoiceMailboxEventFilter::DialpadVoiceMailboxEventFilter( 
        Dialpad * dialpad,
        QObject * parent )
    :
    DialpadMailboxEventFilterBase( dialpad, parent )
    {
    
    }


// -----------------------------------------------------------------------------
// DialpadVoiceMailboxEventFilter::~DialpadVoiceMailboxEventFilter
// -----------------------------------------------------------------------------
//
DialpadVoiceMailboxEventFilter::~DialpadVoiceMailboxEventFilter(  )
    {
    
    }


// -----------------------------------------------------------------------------
// DialpadVoiceMailboxEventFilter::eventFilter
// -----------------------------------------------------------------------------
//
bool DialpadVoiceMailboxEventFilter::eventFilter( 
        QObject * watched,
        QEvent * event )
    {
    SMC_MOCK_METHOD2( bool, QObject *, watched, 
        QEvent *, event )
    }


// -----------------------------------------------------------------------------
// DialpadVoiceMailboxEventFilter::handleCallButtonPress
// -----------------------------------------------------------------------------
//
bool DialpadVoiceMailboxEventFilter::handleCallButtonPress(  )
    {
    SMC_MOCK_METHOD0( bool )
    }


// -----------------------------------------------------------------------------
// DialpadVoiceMailboxEventFilter::handleMailboxOperation
// -----------------------------------------------------------------------------
//
void DialpadVoiceMailboxEventFilter::handleMailboxOperation(  )
    {
    SMC_MOCK_METHOD0( void )
    }


// -----------------------------------------------------------------------------
// DialpadVoiceMailboxEventFilter::handleLongKeyPress
// -----------------------------------------------------------------------------
//
void DialpadVoiceMailboxEventFilter::handleLongKeyPress(  )
    {
    SMC_MOCK_METHOD0( void )
    }


