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
* Description:  MPSettingsModel.cpp
*     
*
*/

// Version : %version: 4.1.2 %




// INCLUDE FILES
#include    <ecom/ecom.h>
#include    <mpsettingsmodel.h>

#define KMPSettEngUidPluginInterfaceMediaSettings 0x101F857C

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CMPSettingsModel::NewL
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
EXPORT_C CMPSettingsModel* CMPSettingsModel::NewL(TUid aUid)
    {
    return static_cast<CMPSettingsModel*>(REComSession::CreateImplementationL
        (aUid, _FOFF(CMPSettingsModel, iDtor_ID_Key)));
    }
    
// -----------------------------------------------------------------------------
// CMPSettingsModel::~CMPSettingsModel
// Destructor
// -----------------------------------------------------------------------------
//
EXPORT_C CMPSettingsModel::~CMPSettingsModel()
    {
    REComSession::FinalClose();
    REComSession::DestroyedImplementation(iDtor_ID_Key);
    }

// -----------------------------------------------------------------------------
// CMPSettingsModel::ListImplementationsL
// -----------------------------------------------------------------------------
//
EXPORT_C void CMPSettingsModel::ListImplementationsL(RImplInfoPtrArray& aImplInfoArray)
    {
    const TUid pluginViewInterfaceUID = {KMPSettEngUidPluginInterfaceMediaSettings};
    REComSession::ListImplementationsL(pluginViewInterfaceUID, aImplInfoArray);
    }

// ========================== OTHER EXPORTED FUNCTIONS =========================

//  End of File  
