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
* Description:  Definitions for progress messages
*
*/


#ifndef MPXMESSAGEPROGRESSDEFS_H
#define MPXMESSAGEPROGRESSDEFS_H

#include <e32base.h>

/**
* Content ID identifying progress messages
*/
const TInt KMPXMessageContentIdProgress = 0x10282964;

/**
*  Attributes supported for KMPXMessageContentIdProgress
*/
enum
    {
    /**
    * Integer value of currently completed steps
    */
    EMPXMessageProgressCurrentCount = 0x01,

    /**
    * Integer value of total steps to completion
    */
    EMPXMessageProgressTotalCount = 0x02
    };

#endif // MPXMESSAGEPROGRESSDEFS_H
