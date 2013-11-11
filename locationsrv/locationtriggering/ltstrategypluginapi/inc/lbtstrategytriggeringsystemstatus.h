/*
* Copyright (c) 2006 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  Structure defines triggering system status.
*
*/


#ifndef T_LBTSTRATEGYSUPERVISIONDYNAMICINFO_H
#define T_LBTSTRATEGYSUPERVISIONDYNAMICINFO_H


#include "lbttriggeringsystemstatus.h"


/**
 * Structure defines triggering supervision process dynamic information.
 *
 * @since S60 4.0
 */
struct TLbtStrategySupervisionDynamicInfo

    {
    
    /**
     * Location acquisition status.
     */
    TLbtLocationAcquisitionStatus iLocationAcquisitionStatus;

    /**
     * Latest acquired position information.
     */
    TPositionInfo iLatestAcquiredPosInfo;

    /**
     * Trigger ID of the nearest trigger among
     * all enabled triggers in system.
     */
    TLbtTriggerId iNearestTriggerId;

    /**
     * Distance to the nearest trigger. In meters.
     */
    TReal iDistanceToNearestTrigger;
    
    };

#endif  // T_LBTSTRATEGYSUPERVISIONDYNAMICINFO_H
