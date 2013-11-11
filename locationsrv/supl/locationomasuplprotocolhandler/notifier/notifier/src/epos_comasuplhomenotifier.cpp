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
#include "epos_comasuplhomenotifier.h"
#include "epos_omasuplnotifierinterface.h"

// System Includes
#include <StringLoader.h> 
#include <epos_omasuplnotifier.rsg>
#include <epos_omasuplquerycoverui.h>

// Constant Definition

// ------------------------- Member Function definition ----------------------

// ----------------------------------------------------------
// CSuplHomeNotifier::CSuplHomeNotifier()
// ----------------------------------------------------------
//
CSuplHomeNotifier::CSuplHomeNotifier()
    {
    }

// ----------------------------------------------------------
// CSuplHomeNotifier::~CSuplHomeNotifier()
// ----------------------------------------------------------
//
CSuplHomeNotifier::~CSuplHomeNotifier()
    {
    }

// ----------------------------------------------------------
// void CSuplHomeNotifier::ConstructL()
// ----------------------------------------------------------
//    
void CSuplHomeNotifier::ConstructL()
    {
    // Call the base class constructor
    CSuplNotifier::ConstructL();
    }

// ----------------------------------------------------------
// CSuplHomeNotifier* CSuplHomeNotifier::NewL()
// ----------------------------------------------------------
//
CSuplHomeNotifier* CSuplHomeNotifier::NewL()
    {
    CSuplHomeNotifier* self = new ( ELeave ) CSuplHomeNotifier;
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop( self );
    return self;
    }

// ----------------------------------------------------------
// HBufC* CSuplHomeNotifier::QueryTextL()
// ----------------------------------------------------------
//
HBufC* CSuplHomeNotifier::QueryTextL()
	{
    // Create the Query string for this notifier
    HBufC* queryText = StringLoader::LoadL( R_LOC_SUPL_CONNECTION );	
	return queryText;
	}

// ----------------------------------------------------------
// TInt CSuplHomeNotifier::SuplQueryId()
// ----------------------------------------------------------
//
TInt CSuplHomeNotifier::SuplQueryId()
    {
    return ECmdSuplHomeNetwork;
    }

// ----------------------------------------------------------
// CSuplHomeNotifier::TNotifierInfo CSuplHomeNotifier::RegisterL()
// ----------------------------------------------------------
//
CSuplHomeNotifier::TNotifierInfo CSuplHomeNotifier::RegisterL()
    {
    iInfo.iUid          = KSuplHomeNotifier;
    iInfo.iChannel      = KSuplHomeNotifierChannel;
    iInfo.iPriority     = ENotifierPriorityVHigh;
    return iInfo;    
    }
  
