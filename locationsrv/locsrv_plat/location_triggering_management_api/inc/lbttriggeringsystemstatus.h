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
* Description:  Structure for location triggering status
*
*/



#ifndef LBTTRIGGERINGSYSTEMSTATUS_H
#define LBTTRIGGERINGSYSTEMSTATUS_H

#include <lbtcommon.h>
#include "lbttriggeringsystemstatus.h"

/**
 * Location acquisition status.
 *
 * @since S60 5.1
 */
enum TLbtLocationAcquisitionStatus
    {
    ELocationAcquisitionActive,         ///<Location acquisition is active.
    ELocationAcquisitionInactive,       ///<Location acquisition is inactive, 
                                        ///<for example, there is no enabled 
                                        ///<trigger in the system.
    ELocationAcquisitionOff,            ///<Triggering mechanism is switched off
    ELocationAcquisitionNotReady,       ///<For the situation in system boot-up time
                                        ///<when Location Triggering Server is not
                                        ///<yet ready to start supervision process.
    ELocationAcquisitionPrivacyReject,  ///<Location acquistion is rejected 
                                        ///<because of privacy checking failure.
    ELocationAcquisitionNoMethod,       ///<Location acquisition failed because 
                                        ///<no positioning method is available.
                                        ///<This information is published only when 
                                        ///<subsequent location request is made by the 
																				///<location triggering server. 
    ELocationAcquisitionFailure,        ///<Positioning failed because of other reason.
    };

/**
 * Structure for location triggering system status.
 *
 * @since S60 5.1
 */
struct TLbtTriggeringSystemStatus
    {
    /**
     * Location acquisition status.
     */
    TLbtLocationAcquisitionStatus iLocationAcquisitionStatus;
        
    /**
     * Uid of the last used positioning method.
     */
    TUid iPositioningMethod;

	/**
	 * Name of the last used positioning method.
	 */
	TBuf<KPositionMaxModuleName> iPositioningMethodName;

    /**
     * Timestamp of the latest location update used 
     * by Location Triggering Server.
     */
    TTime iLatestLocationUpdate;
    
    /**
     * For future use.
     */
    TUint8 iUnused[8];
    };


#endif // LBTTRIGGERINGSYSTEMSTATUS_H
