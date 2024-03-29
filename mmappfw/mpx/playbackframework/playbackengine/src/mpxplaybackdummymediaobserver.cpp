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
* Description:  Dummy observer class
*
*/


#include <e32base.h>
#include "mpxplaybackdummymediaobserver.h"


// ----------------------------------------------------------------------------
// Constructor
// ----------------------------------------------------------------------------
//
CMPXPlaybackDummyMediaObserver::CMPXPlaybackDummyMediaObserver()
    {
    }

// ----------------------------------------------------------------------------
// Destructor
// ----------------------------------------------------------------------------
//
CMPXPlaybackDummyMediaObserver::~CMPXPlaybackDummyMediaObserver()
    {
    }

// ----------------------------------------------------------------------------
// From MMPXCollectionMediaObserver
// ----------------------------------------------------------------------------
//
void CMPXPlaybackDummyMediaObserver::HandleCollectionMediaL(const CMPXMedia& /*aMedia*/,
                                                            TInt /*aError*/)
    {
    // DO NOTHING
    }

// END OF FILE
