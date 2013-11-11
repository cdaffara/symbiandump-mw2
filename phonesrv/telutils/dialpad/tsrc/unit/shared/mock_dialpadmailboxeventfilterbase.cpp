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
#include "dialpadmailboxeventfilterbase.h"

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// DialpadMailboxEventFilterBase::DialpadMailboxEventFilterBase
// -----------------------------------------------------------------------------
//
DialpadMailboxEventFilterBase::DialpadMailboxEventFilterBase( 
        Dialpad * dialpad,
        QObject * parent )
    :
    QObject( parent )
    {
    Q_UNUSED(dialpad)
    }


// -----------------------------------------------------------------------------
// DialpadMailboxEventFilterBase::~DialpadMailboxEventFilterBase
// -----------------------------------------------------------------------------
//
DialpadMailboxEventFilterBase::~DialpadMailboxEventFilterBase(  )
    {
    
    }


// -----------------------------------------------------------------------------
// DialpadMailboxEventFilterBase::eventFilter
// -----------------------------------------------------------------------------
//
bool DialpadMailboxEventFilterBase::eventFilter( 
        QObject * watched,
        QEvent * event )
    {
    SMC_MOCK_METHOD2( bool, QObject *, watched, 
        QEvent *, event )
    }


// -----------------------------------------------------------------------------
// DialpadMailboxEventFilterBase::isLongKeyPressSupported
// -----------------------------------------------------------------------------
//
bool DialpadMailboxEventFilterBase::isLongKeyPressSupported( 
        const int key )
    {
    SMC_MOCK_METHOD1( bool, const int, key )
    }


// -----------------------------------------------------------------------------
// DialpadMailboxEventFilterBase::checkIfSendEventAndConsumeEvent
// -----------------------------------------------------------------------------
//
bool DialpadMailboxEventFilterBase::checkIfSendEventAndConsumeEvent( 
        const int pressedKey,
        const int eventType )
    {
    SMC_MOCK_METHOD2( bool, const int, pressedKey, 
        const int, eventType )
    }


// -----------------------------------------------------------------------------
// DialpadMailboxEventFilterBase::handleCallButtonPress
// -----------------------------------------------------------------------------
//
bool DialpadMailboxEventFilterBase::handleCallButtonPress(  )
    {
    SMC_MOCK_METHOD0( bool )
    }


// -----------------------------------------------------------------------------
// DialpadMailboxEventFilterBase::clearEditor
// -----------------------------------------------------------------------------
//
void DialpadMailboxEventFilterBase::clearEditor(  )
    {
    SMC_MOCK_METHOD0( void )
    }


// -----------------------------------------------------------------------------
// DialpadMailboxEventFilterBase::createCall
// -----------------------------------------------------------------------------
//
void DialpadMailboxEventFilterBase::createCall( 
        const QString & phoneNumber,
        bool createVideoCall )
    {
    SMC_MOCK_METHOD2( void, const QString &, phoneNumber, 
        bool, createVideoCall )
    }


// -----------------------------------------------------------------------------
// DialpadMailboxEventFilterBase::handleMailboxOperation
// -----------------------------------------------------------------------------
//
void DialpadMailboxEventFilterBase::handleMailboxOperation(  )
    {
    SMC_MOCK_METHOD0( void )
    }


// -----------------------------------------------------------------------------
// DialpadMailboxEventFilterBase::handleLongKeyPress
// -----------------------------------------------------------------------------
//
void DialpadMailboxEventFilterBase::handleLongKeyPress(  )
    {
    SMC_MOCK_METHOD0( void )
    }


