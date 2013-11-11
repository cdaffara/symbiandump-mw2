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
#include "simcontrolkeysequencehandler.h"

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// SimControlKeySequenceHandler::SimControlKeySequenceHandler
// -----------------------------------------------------------------------------
//
SimControlKeySequenceHandler::SimControlKeySequenceHandler( 
        QObject * parent )
    :
    KeySequenceHandler( parent )
    {
    
    }


// -----------------------------------------------------------------------------
// SimControlKeySequenceHandler::~SimControlKeySequenceHandler
// -----------------------------------------------------------------------------
//
SimControlKeySequenceHandler::~SimControlKeySequenceHandler(  )
    {
    
    }


// -----------------------------------------------------------------------------
// SimControlKeySequenceHandler::executeKeySequence
// -----------------------------------------------------------------------------
//
bool SimControlKeySequenceHandler::executeKeySequence( 
        const QString & keySequence )
    {
    SMC_MOCK_METHOD1( bool, const QString &, keySequence )
    }


