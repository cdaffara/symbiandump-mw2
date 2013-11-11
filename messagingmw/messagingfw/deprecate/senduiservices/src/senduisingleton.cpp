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
* Description:   Singleton
*
*/



// INCLUDE FILES

#include <msvapi.h>     // CMsvSession
#include <mtclreg.h>    // CClientMtmRegistry
#include <mtuireg.h>    // CMtmUiRegistry
#include <mtudreg.h>    // CMtmUiDataRegistry

#include <senduisingleton.h>


// CONSTANTS

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CSendUiSingleton::CSendUiSingleton
// C++ default constructor can NOT contain any code, that
// might leave.
// -----------------------------------------------------------------------------
//
CSendUiSingleton::CSendUiSingleton()
    {
    }

// -----------------------------------------------------------------------------
// CSendUiSingleton::ConstructL
// Symbian 2nd phase constructor can leave.
// -----------------------------------------------------------------------------
//
void CSendUiSingleton::ConstructL()
    {
    }

// -----------------------------------------------------------------------------
// CSendUiSingleton::NewL
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
EXPORT_C CSendUiSingleton* CSendUiSingleton::NewL()
    {
    }

// -----------------------------------------------------------------------------
// CSendUiSingleton::~CSendUiSingleton
// Destructor
// -----------------------------------------------------------------------------
//
CSendUiSingleton::~CSendUiSingleton()
    {
   
    }

// -----------------------------------------------------------------------------
// CSendUiSingleton::MsvSessionL
// -----------------------------------------------------------------------------
//
EXPORT_C CMsvSession& CSendUiSingleton::MsvSessionL()
    {
   
    }

// -----------------------------------------------------------------------------
// CSendUiSingleton::ClientMtmRegistryL
// -----------------------------------------------------------------------------
//
EXPORT_C CClientMtmRegistry& CSendUiSingleton::ClientMtmRegistryL()
    {
    }

// -----------------------------------------------------------------------------
// CSendUiSingleton::MtmUiRegistryL
// -----------------------------------------------------------------------------
//
EXPORT_C CMtmUiRegistry& CSendUiSingleton::MtmUiRegistryL()
    {
   
    }

// -----------------------------------------------------------------------------
// CSendUiSingleton::MtmUiDataRegistryL
// -----------------------------------------------------------------------------
//
EXPORT_C CMtmUiDataRegistry& CSendUiSingleton::MtmUiDataRegistryL()
    {
   
    }

// -----------------------------------------------------------------------------
// CSendUiSingleton::MsvSessionL
// -----------------------------------------------------------------------------
//
void CSendUiSingleton::HandleSessionEventL(TMsvSessionEvent /*aEvent*/, TAny* /*aArg1*/, TAny* /*aArg2*/, TAny* /*aArg3*/)
    {
    }


//  End of File
