/*
* Copyright (c) 2007 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:   SUPL Notifier for the user in his home network
*
*/



// User Includes
#include "epos_comasuplroamingnotifier.h"
#include "epos_omasuplnotifierinterface.h"

// System Includes
#include <StringLoader.h> 
#include <epos_omasuplnotifier.rsg>
#include <epos_omasuplquerycoverui.h>

// Constant Definition

// ------------------------- Member Function definition ----------------------

// ----------------------------------------------------------
// CSuplRoamingNotifier::CSuplRoamingNotifier()
// ----------------------------------------------------------
//
CSuplRoamingNotifier::CSuplRoamingNotifier()
    {
    }

// ----------------------------------------------------------
// CSuplRoamingNotifier::~CSuplRoamingNotifier()
// ----------------------------------------------------------
//
CSuplRoamingNotifier::~CSuplRoamingNotifier()
    {
    }

// ----------------------------------------------------------
// void CSuplRoamingNotifier::ConstructL()
// ----------------------------------------------------------
//    
void CSuplRoamingNotifier::ConstructL()
    {
    // Call the base class constructor
    CSuplNotifier::ConstructL();
    }

// ----------------------------------------------------------
// CSuplRoamingNotifier* CSuplRoamingNotifier::NewL()
// ----------------------------------------------------------
//
CSuplRoamingNotifier* CSuplRoamingNotifier::NewL()
    {
    CSuplRoamingNotifier* self = new ( ELeave ) CSuplRoamingNotifier;
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop( self );
    return self;
    }

// ----------------------------------------------------------
// HBufC* CSuplRoamingNotifier::QueryTextL()
// ----------------------------------------------------------
//
HBufC* CSuplRoamingNotifier::QueryTextL()
	{
    // Create the Query string for this notifier
    HBufC* queryText = StringLoader::LoadL( R_LOC_SUPL_ROAMING_CONNECTION );	
	return queryText;
	}

// ----------------------------------------------------------
// TInt CSuplRoamingNotifier::SuplQueryId()
// ----------------------------------------------------------
//
TInt CSuplRoamingNotifier::SuplQueryId()
    {
    return ECmdSuplRoamingNetwork;
    }
    	
// ----------------------------------------------------------
// CSuplRoamingNotifier::TNotifierInfo CSuplRoamingNotifier::RegisterL()
// ----------------------------------------------------------
//
CSuplRoamingNotifier::TNotifierInfo CSuplRoamingNotifier::RegisterL()
    {
    iInfo.iUid          = KSuplRoamingNotifier;
    iInfo.iChannel      = KSuplRoamingNotifierChannel;
    iInfo.iPriority     = ENotifierPriorityVHigh;
    return iInfo;    
    }
