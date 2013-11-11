/*
* Copyright (c) 2010 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  Positioning Indicators Info - QT Wrapper
*
*/

#include "posindicatorinfo.h"
#include "posindicatorinfo_p.h"

// ---------------------------------------------------------------------------
// PositionIndicatorInfo::PositionIndicatorInfo()
// ---------------------------------------------------------------------------
 PosIndicatorInfo::PosIndicatorInfo()
    {
    FUNC("+ PosIndicatorInfo::PosIndicatorInfo");
    d_ptr = new PosIndicatorInfoPrivate(this);
    FUNC("- PosIndicatorInfo::PosIndicatorInfo");
    }

// ---------------------------------------------------------------------------
// PosIndicatorInfo::~PosIndicatorInfo()
// ---------------------------------------------------------------------------
 PosIndicatorInfo::~PosIndicatorInfo()
    {
    FUNC("+ PosIndicatorInfo::~PosIndicatorInfo");
    delete d_ptr;
    d_ptr = NULL;
    FUNC("- PosIndicatorInfo::~PosIndicatorInfo");
    }

// ---------------------------------------------------------------------------
// PosIndicatorInfo::requestPosInfo()
// ---------------------------------------------------------------------------
 void PosIndicatorInfo::requestPosInfo()
    {
    FUNC("+ PosIndicatorInfo::requestPosInfo");
    d_ptr->RequestPosInfo();
    FUNC("- PosIndicatorInfo::requestPosInfo");
    }
 
 // ---------------------------------------------------------------------------
 // PosIndicatorInfo::cancelPosInfo()
 // ---------------------------------------------------------------------------
  void PosIndicatorInfo::cancelPosInfo()
     {
     FUNC("+ PosIndicatorInfo::cancelPosInfo");
     d_ptr->CancelPosInfo();
     FUNC("- PosIndicatorInfo::cancelPosInfo");
     }

// End of file
