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
* Description:  Factory method to create playback utility object
*
*/


#include"mpxplaybackutilityimpl.h"


// ============================== MEMBER FUNCTIONS ============================

// ----------------------------------------------------------------------------
// Create the playback utility object
// ----------------------------------------------------------------------------
//
EXPORT_C MMPXPlaybackUtility* MMPXPlaybackUtility::NewL(const TUid& aModeId,
                                                    MMPXPlaybackObserver* aObs)
    {
    return CMPXPlaybackUtility::NewL(aModeId,aObs, EMPXCategoryMusic);
    }

// ----------------------------------------------------------------------------
// Create the playback utility object
// ----------------------------------------------------------------------------
//

EXPORT_C MMPXPlaybackUtility* MMPXPlaybackUtility::NewL(const TMPXCategory aCategory,
                                                    const TUid& aModeId,
                                                    MMPXPlaybackObserver* aObs)
    {
    return CMPXPlaybackUtility::NewL(aModeId,aObs,aCategory);
    }
	
// ----------------------------------------------------------------------------
// Creates the playback utility object if not already created
// ----------------------------------------------------------------------------
//
EXPORT_C MMPXPlaybackUtility* MMPXPlaybackUtility::UtilityL( const TUid& aModeId )
    {
    return CMPXPlaybackUtility::UtilityL( aModeId, EMPXCategoryMusic );
    }
	
// ----------------------------------------------------------------------------
// Creates the playback utility object if not already created
// ----------------------------------------------------------------------------
//

EXPORT_C MMPXPlaybackUtility* MMPXPlaybackUtility::UtilityL(const TMPXCategory aCategory,
                                                const TUid& aModeId )
    {
    return CMPXPlaybackUtility::UtilityL( aModeId, aCategory );
    }
