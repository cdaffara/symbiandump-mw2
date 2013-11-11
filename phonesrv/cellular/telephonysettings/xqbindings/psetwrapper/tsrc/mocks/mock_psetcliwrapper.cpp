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
#include <QtGlobal>
#include <QDebug>
#include <smcmockclassincludes.h>
#include "psetcliwrapper.h"

// ============================ MEMBER FUNCTIONS ===============================
class PSetCliWrapperPrivate
{
public:
    PSetCliWrapperPrivate() {}
};


// -----------------------------------------------------------------------------
// PSetCliWrapper::PSetCliWrapper
// -----------------------------------------------------------------------------
//
PSetCliWrapper::PSetCliWrapper( 
        CPsetContainer & /*psetContainer*/,
        QObject * parent )
    :
    QObject(parent)
    {
    
    }


// -----------------------------------------------------------------------------
// PSetCliWrapper::~PSetCliWrapper
// -----------------------------------------------------------------------------
//
PSetCliWrapper::~PSetCliWrapper(  )
    {
    
    }


// -----------------------------------------------------------------------------
// PSetCliWrapper::getColpMode
// -----------------------------------------------------------------------------
//
void PSetCliWrapper::getColpMode(  )
    {
    SMC_MOCK_METHOD0( void )
    }


// -----------------------------------------------------------------------------
// PSetCliWrapper::getClipMode
// -----------------------------------------------------------------------------
//
void PSetCliWrapper::getClipMode(  )
    {
    SMC_MOCK_METHOD0( void )
    }


// -----------------------------------------------------------------------------
// PSetCliWrapper::getClirMode
// -----------------------------------------------------------------------------
//
void PSetCliWrapper::getClirMode(  )
    {
    SMC_MOCK_METHOD0( void )
    }


// -----------------------------------------------------------------------------
// PSetCliWrapper::getColrMode
// -----------------------------------------------------------------------------
//
void PSetCliWrapper::getColrMode(  )
    {
    SMC_MOCK_METHOD0( void )
    }


// -----------------------------------------------------------------------------
// PSetCliWrapper::getCnap
// -----------------------------------------------------------------------------
//
void PSetCliWrapper::getCnap(  )
    {
    SMC_MOCK_METHOD0( void )
    }


// -----------------------------------------------------------------------------
// PSetCliWrapper::cancelAll
// -----------------------------------------------------------------------------
//
void PSetCliWrapper::cancelAll(  )
    {
    SMC_MOCK_METHOD0( void )
    }
