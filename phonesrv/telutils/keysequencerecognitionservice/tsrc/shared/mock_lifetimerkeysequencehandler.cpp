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
#include <smcmockclassincludes.h>
#include "lifetimerkeysequencehandler.h"

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// LifeTimerKeySequenceHandler::LifeTimerKeySequenceHandler
// -----------------------------------------------------------------------------
//
LifeTimerKeySequenceHandler::LifeTimerKeySequenceHandler( 
        QObject * parent )
    :
    KeySequenceHandler(parent),
    m_messageBox(0), 
    m_repository(0)
    {
    
    }


// -----------------------------------------------------------------------------
// LifeTimerKeySequenceHandler::~LifeTimerKeySequenceHandler
// -----------------------------------------------------------------------------
//
LifeTimerKeySequenceHandler::~LifeTimerKeySequenceHandler(  )
    {
    
    }


// -----------------------------------------------------------------------------
// LifeTimerKeySequenceHandler::executeKeySequence
// -----------------------------------------------------------------------------
//
bool LifeTimerKeySequenceHandler::executeKeySequence( 
        const QString & keySequence )
    {
    SMC_MOCK_METHOD1( bool, const QString &, keySequence )
    }


// -----------------------------------------------------------------------------
// LifeTimerKeySequenceHandler::destroyMessageBox
// -----------------------------------------------------------------------------
//
void LifeTimerKeySequenceHandler::destroyMessageBox()
{
    
}
