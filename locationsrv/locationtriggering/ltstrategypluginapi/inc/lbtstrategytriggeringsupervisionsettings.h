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
* Description:  Location system settings related to triggering supervision.
*
*/


#ifndef T_LBTSTRATEGYTRIGGERINGSUPERVISIONSETTINGS_H
#define T_LBTSTRATEGYTRIGGERINGSUPERVISIONSETTINGS_H


#include "lbttriggeringsystemstatus.h"


/**
 * Structure defines location system settings related to triggering supervision.
 *
 * @since S60 4.0
 */
struct TLbtStrategyTriggeringSupervisionSettings

    {
    
    /**
     * Minimum location update interval. In seconds.
     */
    TInt iMinimumLocationUpdateInterval;

    /**
     * Maximum user speed. In meters per second.
     */
    TReal iMaximumUserSpeed;

    /**
     * Minimum location update interval when GPS fails. In seconds.
     */
    TInt iMinimumLocationUpdateIntervalWithoutGps;

    /**
     * Positioning technology to be used. This setting specifies
     * what positioning technology shall be used. KNullUid means
     * automatic selection of positioning technology.
     */
    TUid iPositioningTechnology;
    
    /**
     * GPS tracking mode interval. In seconds.
     */
    TInt iGpsTrackingModeInterval;

    /**
     * Location request timeout. In seconds.
     */
    TInt iLocationRequestTimeout;

    /**
     * Reserved
     */
    TUint iReserved[8];

    };

#endif  // T_LBTSTRATEGYTRIGGERINGSUPERVISIONSETTINGS_H
