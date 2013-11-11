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
#include "keysequencehandler.h"

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// KeySequenceHandler::KeySequenceHandler
// -----------------------------------------------------------------------------
//
KeySequenceHandler::KeySequenceHandler( 
        QObject * parent )
    :
    QObject(parent),
    m_keySequenceValidator("")
    {
    
    }


// -----------------------------------------------------------------------------
// KeySequenceHandler::~KeySequenceHandler
// -----------------------------------------------------------------------------
//
KeySequenceHandler::~KeySequenceHandler(  )
    {
    
    }


// -----------------------------------------------------------------------------
// KeySequenceHandler::keySequenceValidator
// -----------------------------------------------------------------------------
//
QString KeySequenceHandler::keySequenceValidator(  ) const
    {
    SMC_MOCK_METHOD0( QString )
    }


// -----------------------------------------------------------------------------
// KeySequenceHandler::setKeySequenceValidator
// -----------------------------------------------------------------------------
//
void KeySequenceHandler::setKeySequenceValidator( 
        const QString & validator )
    {
    SMC_MOCK_METHOD1( void, const QString &, validator )
    }


