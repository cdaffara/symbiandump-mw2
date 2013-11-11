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
#include "manufacturerkeysequencehandler.h"

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// ManufacturerKeySequenceHandler::ManufacturerKeySequenceHandler
// -----------------------------------------------------------------------------
//
ManufacturerKeySequenceHandler::ManufacturerKeySequenceHandler( 
        QObject * parent )
    :
    KeySequenceHandler(parent),
    m_currentRequest(0)
    {
    
    }


// -----------------------------------------------------------------------------
// ManufacturerKeySequenceHandler::~ManufacturerKeySequenceHandler
// -----------------------------------------------------------------------------
//
ManufacturerKeySequenceHandler::~ManufacturerKeySequenceHandler(  )
    {
    
    }


// -----------------------------------------------------------------------------
// ManufacturerKeySequenceHandler::executeKeySequence
// -----------------------------------------------------------------------------
//
bool ManufacturerKeySequenceHandler::executeKeySequence( 
        const QString & keySequence )
    {
    SMC_MOCK_METHOD1( bool, const QString &, keySequence )
    }


// -----------------------------------------------------------------------------
// ManufacturerKeySequenceHandler::requestOk
// -----------------------------------------------------------------------------
//
void ManufacturerKeySequenceHandler::requestOk(
        const QVariant &/*returnValue*/)
    {
        
    }


// -----------------------------------------------------------------------------
// ManufacturerKeySequenceHandler::requestError
// -----------------------------------------------------------------------------
//
void ManufacturerKeySequenceHandler::requestError(
        int /*error*/, const QString& /*errorMessage*/)
    {
        
    }

