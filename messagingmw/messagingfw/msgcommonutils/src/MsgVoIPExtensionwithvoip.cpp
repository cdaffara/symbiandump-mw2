/*
* Copyright (c) 2005, 2007-2008 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:   Get count of VoIP profiles.
*
*/


#include "MsgVoIPExtension.h"

#include <spsettings.h>

// ================= MEMBER FUNCTIONS =======================

// C++ default constructor can NOT contain any code, that
// might leave.
//
CMsgVoIPExtension::CMsgVoIPExtension()
    {
    }

// Two-phased constructor.
//
EXPORT_C CMsgVoIPExtension* CMsgVoIPExtension::NewL()
    {
    CMsgVoIPExtension *self =
        new ( ELeave ) CMsgVoIPExtension();
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop();
    return self;
    }

// ConstructL
//
void CMsgVoIPExtension::ConstructL()
    {
    // for handling services related data
    iSPSettings = CSPSettings::NewL();
    }

// Destructor
//
CMsgVoIPExtension::~CMsgVoIPExtension()
    {
    delete iSPSettings;
    }

// ----------------------------------------------------
// CMsgVoIPExtension::VoIPProfilesExistL
//
// ----------------------------------------------------
EXPORT_C TBool CMsgVoIPExtension::VoIPProfilesExistL() const
    {
    return iSPSettings->IsFeatureSupported( ESupportVoIPFeature ) &&
           iSPSettings->IsFeatureSupported( ESupportInternetCallFeature );
    }

// ----------------------------------------------------
// CMsgVoIPExtension::IsPreferredTelephonyVoIP
// 
// ----------------------------------------------------
EXPORT_C TBool CMsgVoIPExtension::IsPreferredTelephonyVoIP() const
    {
    return EFalse;
    }

// ----------------------------------------------------
// CMsgVoIPExtension::IsVoIPSupported
// 
// ----------------------------------------------------
EXPORT_C TBool CMsgVoIPExtension::IsVoIPSupported() const
    {
    return iSPSettings->IsFeatureSupported( ESupportVoIPFeature );
    }

//  End of File
