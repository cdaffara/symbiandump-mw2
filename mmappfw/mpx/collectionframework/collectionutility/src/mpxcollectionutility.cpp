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
* Description:  Factory method to create collection utility object
*
*/


#include"mpxcollectionutilityimpl.h"


// ============================== MEMBER FUNCTIONS ============================

// ----------------------------------------------------------------------------
// Create the collection utility object by mode
// ----------------------------------------------------------------------------
//
EXPORT_C MMPXCollectionUtility* MMPXCollectionUtility::NewL(MMPXCollectionObserver* aObs,
                                                            const TUid& aModeId)
    {
    return CMPXCollectionUtility::NewL(aModeId,aObs);
    }
