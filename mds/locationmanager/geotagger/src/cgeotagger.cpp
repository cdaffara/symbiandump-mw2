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
* Description:  A class responsible for geotagging of media files in background.
*
*/

#include <geotagger.h>
#include "cinternalgeotagger.h"
#include "locationmanagerdebug.h"


// --------------------------------------------------------------------------
// CGeoTagger::NewL
// --------------------------------------------------------------------------
//
EXPORT_C CGeoTagger* CGeoTagger::NewL( MGeoTaggerObserver* aObserver,
        CMdESession* aMdeSession)
    {
    LOG("CGeoTagger::NewL, begin");
    CInternalGeoTagger* self = CInternalGeoTagger::NewL( aMdeSession, aObserver );
    
    return self;
    }
        


// End of file
