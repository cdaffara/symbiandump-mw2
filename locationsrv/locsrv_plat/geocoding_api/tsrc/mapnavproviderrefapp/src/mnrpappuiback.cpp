/*
* Copyright (c) 2006 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  CMnrpAppUiBack classimplementation
*
*/


#include <aknnavi.h>
#include <aknnavide.h>

#include "mnrpengine.h"
#include "mnrpappuiback.h"

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
void CMnrpAppUiBack::ConstructL()
    {
    BaseConstructL( EAknEnableSkin );
    // no view is created as this application is intended to be run
    // only as hidden server
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
void CMnrpAppUiBack::HandleCommandL(TInt aCommand)
    {
    switch(aCommand)
        {
        case EEikCmdExit:
        case EAknSoftkeyExit:
            Exit();
            break;
        default:
            break;
        }
    }

// ---------------------------------------------------------------------------
// ---------------------------------------------------------------------------
//
void CMnrpAppUiBack::MoveForeground()
    {
    RWindowGroup& window = iEikonEnv->RootWin();
    window.SetOrdinalPosition( 0 );
    }

// ---------------------------------------------------------------------------
// ---------------------------------------------------------------------------
//
void CMnrpAppUiBack::MoveBackground()
    {
    RWindowGroup& window = iEikonEnv->RootWin();
    window.SetOrdinalPosition( -1 );
    }
