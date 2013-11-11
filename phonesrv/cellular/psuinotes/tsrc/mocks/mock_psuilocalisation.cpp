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
#include "psuilocalisation.h"

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// PsUiLocalisation::PsUiLocalisation
// -----------------------------------------------------------------------------
//
PsUiLocalisation::PsUiLocalisation( 
        QObject * parent )
    //:
    //QObject( /*parent*/ )
    {
    
    }


// -----------------------------------------------------------------------------
// PsUiLocalisation::~PsUiLocalisation
// -----------------------------------------------------------------------------
//
PsUiLocalisation::~PsUiLocalisation(  )
    {
    
    }


// -----------------------------------------------------------------------------
// PsUiLocalisation::installTranslator
// -----------------------------------------------------------------------------
//
bool PsUiLocalisation::installTranslator( 
        TranslationFileId translationFileId )
    {
    SMC_MOCK_METHOD1( bool, TranslationFileId, translationFileId )
    }


// -----------------------------------------------------------------------------
// PsUiLocalisation::removeTranslators
// -----------------------------------------------------------------------------
//
void PsUiLocalisation::removeTranslators(  )
    {
    SMC_MOCK_METHOD0( void )
    }


