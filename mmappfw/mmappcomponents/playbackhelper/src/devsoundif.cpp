/*
* Copyright (c) 2008 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  Implementation of CDevSoundIf
*
*/

// Version : %version: 5 %



// INCLUDE FILES
#include <mmf/server/sounddevice.h>
#include <devsoundif.h>

#include "playbackhelper_log.h"

// CONSTANTS
const TInt KTenStepsVolume = 10;
const TInt KTwentyStepsVolume = 20;


// ============================ MEMBER FUNCTIONS ===================================================

// -------------------------------------------------------------------------------------------------
// CDevSoundIf::CDevSoundIf
// C++ default constructor can NOT contain any code, that might leave.
// -------------------------------------------------------------------------------------------------
//
CDevSoundIf::CDevSoundIf()
{
    PLAYBACKHELPER_DEBUG(_L("CDevSoundIf::CDevSoundIf()"));
}

// -------------------------------------------------------------------------------------------------
// CDevSoundIf::ConstructL
// Symbian 2nd phase constructor can leave.
// -------------------------------------------------------------------------------------------------
//
void CDevSoundIf::ConstructL()
{
    PLAYBACKHELPER_DEBUG(_L("CDevSoundIf::ConstructL()"));

    iDevSound = CMMFDevSound::NewL();
}

// -------------------------------------------------------------------------------------------------
// CDevSoundIf::NewL
// Two-phased constructor.
// -------------------------------------------------------------------------------------------------
//
EXPORT_C CDevSoundIf* CDevSoundIf::NewL()
{
    PLAYBACKHELPER_DEBUG(_L("CDevSoundIf::NewL()"));

    CDevSoundIf* self = new( ELeave ) CDevSoundIf;
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop();
    return self;
}

// -------------------------------------------------------------------------------------------------
// CDevSoundIf::~CDevSoundIf
// Destructor
// -------------------------------------------------------------------------------------------------
//
EXPORT_C CDevSoundIf::~CDevSoundIf()
{
    PLAYBACKHELPER_DEBUG(_L("CDevSoundIf::~CDevSoundIf()"));

    if ( iDevSound ) 
    {
        delete iDevSound;
    }

}

// -------------------------------------------------------------------------------------------------
// CDevSoundIf::GetNumberOfVolumeSteps
// -------------------------------------------------------------------------------------------------
//
EXPORT_C TInt CDevSoundIf::GetNumberOfVolumeSteps()
{
    // 
    // set default 10-steps volume
    //
    TInt volumeSteps = KTenStepsVolume;
	
    if ( iDevSound && 
         iDevSound->MaxVolume() >= KTwentyStepsVolume ) 
    {
        //
        // set 20-steps volume
        //
        volumeSteps = KTwentyStepsVolume;
    }	 

    PLAYBACKHELPER_DEBUG(_L("CDevSoundIf::GetNumberOfVolumeSteps() returns [%d]"), volumeSteps);

    return volumeSteps;
}

//  End of File
