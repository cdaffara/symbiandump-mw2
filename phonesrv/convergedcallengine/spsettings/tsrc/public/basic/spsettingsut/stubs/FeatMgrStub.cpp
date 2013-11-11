/*
* Copyright (c) 2003 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  Stub implementation for needed libraries.
*
*/



// INCLUDE FILES
#include "FeatMgr.h"
#include <bldvariant.hrh>
#include "TestData.h"

TBool FeatureManager::iBtAudioSupported = ETrue;
TBool FeatureManager::iHelpSupported = ETrue;
TBool FeatureManager::iFeatureIdCsVideoTelephony = ETrue;

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// FeatureManager::InitializeLibL
// 
// -----------------------------------------------------------------------------
void FeatureManager::InitializeLibL()
    {
    }

// -----------------------------------------------------------------------------
// FeatureManager::InitializeLibL
// 
// -----------------------------------------------------------------------------
void FeatureManager::UnInitializeLib()
    {
    }
    
// -----------------------------------------------------------------------------
// FeatureManager::FeatureSupported
// 
// -----------------------------------------------------------------------------
TBool FeatureManager::FeatureSupported( TInt aFeature )
    {
    TBool returnValue = EFalse;
    
    if ( aFeature == KFeatureIdCsVideoTelephony )
        {
        returnValue = iFeatureIdCsVideoTelephony;
        }
    else if ( aFeature == KFeatureIdBtAudio )
        {
        returnValue = iBtAudioSupported;
        }
    else if ( aFeature == KFeatureIdHelp )
        {
        returnValue = iHelpSupported;
        }
    else if ( aFeature == KFeatureIdAutoRedialForVideoCall )
        {
        returnValue = ETrue;
        }
    else if ( aFeature == KFeatureIdCommonVoip )
        {
        if ( TestData::iCheckSupportedCase )
            {
            returnValue = EFalse;
            }
        else
            {
            returnValue = ETrue;
            }
        }
    else 
        {
        // By default any other feature is not supported.
        returnValue = EFalse;
        }

    return returnValue;
    }

//  End of File  
