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
#include "dialpadhasheventfilter.h"

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// DialpadHashEventFilter::DialpadHashEventFilter
// -----------------------------------------------------------------------------
//
DialpadHashEventFilter::DialpadHashEventFilter( 
        Dialpad * dialpad,
        QObject * parent )
    :
    DialpadMailboxEventFilterBase(dialpad, parent)
    {

    }


// -----------------------------------------------------------------------------
// DialpadHashEventFilter::~DialpadHashEventFilter
// -----------------------------------------------------------------------------
//
DialpadHashEventFilter::~DialpadHashEventFilter(  )
    {
    
    }


// -----------------------------------------------------------------------------
// DialpadHashEventFilter::eventFilter
// -----------------------------------------------------------------------------
//
bool DialpadHashEventFilter::eventFilter( 
        QObject * watched,
        QEvent * event )
    {
    SMC_MOCK_METHOD2( bool, QObject *, watched, 
        QEvent *, event )
    }

// -----------------------------------------------------------------------------
// DialpadHashEventFilter::handleCallButtonPress
// -----------------------------------------------------------------------------
//
bool DialpadHashEventFilter::handleCallButtonPress()
{
    return false;
}

// -----------------------------------------------------------------------------
// DialpadHashEventFilter::handleMailboxOperation
// -----------------------------------------------------------------------------
//
void DialpadHashEventFilter::handleMailboxOperation()
{
    
}

// -----------------------------------------------------------------------------
// DialpadHashEventFilter::handleLongKeyPress
// -----------------------------------------------------------------------------
//
void DialpadHashEventFilter::handleLongKeyPress()
{
    
}
