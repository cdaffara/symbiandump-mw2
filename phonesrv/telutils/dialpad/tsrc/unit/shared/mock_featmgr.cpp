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
#include <smcmockclassincludes.h>
#include <featmgr/featmgr.h>
#include <featmgr/featurecontrol.h>
#include <e32std.h>

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// FeatureManager::FeatureManager
// -----------------------------------------------------------------------------
//
FeatureManager::FeatureManager(  )
    {
    
    }


// -----------------------------------------------------------------------------
// FeatureManager::InitializeLibL
// -----------------------------------------------------------------------------
//
void FeatureManager::InitializeLibL(  )
    {
    SMC_MOCK_METHOD0( void )
    }


// -----------------------------------------------------------------------------
// FeatureManager::UnInitializeLib
// -----------------------------------------------------------------------------
//
void FeatureManager::UnInitializeLib(  )
    {
    SMC_MOCK_METHOD0( void )
    }


// -----------------------------------------------------------------------------
// FeatureManager::FeatureSupported
// -----------------------------------------------------------------------------
//
TBool FeatureManager::FeatureSupported( 
        TInt aFeature )
    {
    SMC_MOCK_METHOD1( TBool, TInt, aFeature )
    }


