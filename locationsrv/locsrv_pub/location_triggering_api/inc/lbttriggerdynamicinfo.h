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
* Description:  Data class for trigger's dynamic system information.
*
*/


#ifndef LBTTRIGGERDYNAMICINFO_H
#define LBTTRIGGERDYNAMICINFO_H

#include <lbtcommon.h>

/**
 * Data class for trigger's dynamic system information, such as 
 * trigger validity and distance to the latest acquired location.
 * 
 * Client application can get following dynamic information of a trigger
 * after it's created. 
 *
 * -  <B>Validity</B>. A trigger is set as invalid when the specified 
 * manager UI, owner process or trigger handling process( for start-up trigger )
 * is removed from the system. The process can be removed for example when 
 * it resides in removable media( like MMC ), and that media is removed from 
 * the terminal. An invalid trigger is not included in the 
 * trigger supervision process. When the removable media with the specified
 * process is attached back to the terminal, the trigger is set as 
 * valid and included in the trigger supervision process, provided 
 * that the trigger is not disabled. When a trigger has been invalid longer
 * than the system clean-up time, the trigger is removed from the system.  
 * The system clean-up time is defined by the system and can't be accessed
 * by client application. Note, if a trigger's owner process, manager UI
 * or trigger handling process( for start-up trigger ) is uninstalled
 * from the system, the trigger will be removed by Location Triggering
 * Server.
 *
 * -  <B>Distance to the Latest Acquired Location</B>. 
 *
 *  @lib lbt.lib
 *  @since S60 5.1
 */
struct TLbtTriggerDynamicInfo
    {
    /**
     * Attributes of a trigger's dynamic information.
     */
    enum TLbtDynamicInfoAttribute
        {
        /**
         * Validity status attribute.
         */
        EValidityStatus = 0x01,           
        
        /**
         * Distance to latest acquired location fix.
         */ 
        EDistanceToLatestLocation = 0x02,
        
        /**
         * Last fired location information.
         */
        EFiredLocality = 0x04   
        };
    
    /**
     * Defines the trigger validity status.
     */
    enum TLbtTriggerValidity
        {
        /**
         * Invalid
         */
        EInvalid = 1,       
        /**
         * Valid
         */
        EValid = 2          
        };

    /**
     * Defines the trigger validity status.
     */
    TLbtTriggerValidity iValidity;

    /**
     * The distance in meters from the trigger to latest acquired
     * location.
     */
    TReal iDistanceToLatestLocation;
    
    /**
     * Information of the location where the trigger was last fired. The 
     * information available will contain the latitude, longitude and 
     * horizontal accuracy values. Other details in TLocality will not 
     * be updated.
     */
    TLocality iFiredLocality;

    /**
     * For future use
     */
    TUint8 iUnused[8];
    };

#endif // LBTTRIGGERDYNAMICINFO_H
