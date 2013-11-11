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
* Description:  location triggering server client library
*
*/



#ifndef LBTTRIGGERINGSYSTEMMANAGEMENTSETTINGS_H
#define LBTTRIGGERINGSYSTEMMANAGEMENTSETTINGS_H


#include <e32std.h>
#include <lbtcommon.h>


const TPositionModuleId KLbtAutomaticModuleSelection = {0x10206911};

typedef TUint32 TLbtManagementSettingsMask; 

enum TLbtManagementSettings
    {
    EMinimumLocationUpdateInterval = 0x0001,
       
    EMinimumLocationUpdateIntervalWhenGpsFails = 0x0002,
      
    EUsedPositioningmodule = 0x0004,
    
    EMaximumUserSpeed = 0x0008,
    
    ETriggeringMechanismState = 0x0010
    };

/**
* Data class for triggering system management settings.
*
* @since S60 4.0
*/
class TLbtTriggeringSystemManagementSettings : public TLbtTriggeringSystemSettings
    {
public:
    /**
     * Constructor.
     *
     * After initiation
     * 
     * - minimum location update interval is set to zero
     * - minimum location update interval when GPS fails is set to zero
     * - maximum user speed is set to zero
     * - the used positioning module is set to KLbtAutomaticModuleSelection, 
     * which means automatic module selection. 
     */
    IMPORT_C TLbtTriggeringSystemManagementSettings();
    
    
    /**
     * Derived from TLbtTriggeringSystemSettings. Gets the type 
     * of system settings class.
     *
     * @return The type of system settings class 
     * defined in TLbtTriggeringSystemSettings.
     */
    IMPORT_C virtual TType Type();

    /**
     * Returns minimum location update interval. 
     *
     * This setting defines minimum location update interval when 
     * Location Triggering Server requests location information.
     *
     * @return Minimum location update interval. In seconds.
     */
    IMPORT_C TInt MinimumLocationUpdateInterval() const;

    /**
     * Set the minimum location update interval. 
     *
     * This setting defines minimum location update interval when 
     * Location Triggering Server requests location information.
     *
     * @param[in] aInterval Minimum location update interval. In seconds.
     *
     * @panic LocTriggering ELbtErrArgument If aInterval is 
     * negative.
     */
    IMPORT_C void SetMinimumLocationUpdateInterval(
        TInt aInterval );

    /**
     * Returns minimum location update interval the 
     * Location Triggering Server uses in case of GPS 
     * technology fails. 
     *
     * This setting defines minimum location update interval when GPS 
     * technology was tried but it failed to give a location fix.
     *
     * @return Minimum location update interval when GPS 
     * fails. In seconds.
     */
    IMPORT_C TInt MinimumLocationUpdateIntervalWhenGpsFails() const;

    /**
     * Sets minimum location update interval the 
     * Location Triggering Server uses in case of GPS 
     * technology fails. 
     *
     * This setting defines minimum location update interval when GPS 
     * technology was tried but it failed to give a location fix.
     * 
     * @param[in] aInterval Minimum location update interval when GPS 
     * fails. In seconds.
     *
     * @panic LocTriggering ELbtErrArgument If aInterval is 
     * negative.
     */
    IMPORT_C void SetMinimumLocationUpdateIntervalWhenGpsFails(
        TInt aInterval );

    /**
     * Returns the used positioning module. This setting can either 
     * indicate a specific positioning module (i.e. a PSY), 
     * or automatic selection of positioning module.
     * Default value of this setting is 
     * KLbtAutomaticModuleSelection, which means automatic selection 
     * of positioning module. 
     *
     * @return Uid of the used positioning module.
     */
    IMPORT_C TPositionModuleId UsedPositioningModule() const;

    /**
     * Sets the used positioning module. This setting can either 
     * indicate a specific positioning module (i.e. a PSY), 
     * or automatic selection of positioning module (i.e. 
     * Default Proxy). Default value of this setting is 
     * KLbtAutomaticModuleSelection, which means automatic 
     * selection of positioning module. 
     *
     * @param[in] aUid The UID of the used positioning module.
     * KLbtAutomaticModuleSelection means automatic selection of 
     * positioning module.
     */
    IMPORT_C void SetUsedPositioningModule(
        TPositionModuleId aUid );

    /**
     * Returns maximum user speed. 
     *
     * This setting defines maximum speed that user may move.
     *
     * @return The maximum user speed in meters per second.
     */
    IMPORT_C TReal MaximumUserSpeed() const;

    /**
     * Sets maximum user speed. 
     *
     * This setting defines maximum speed that user may move.
     *
     * @param[in] aSpeed The maximum user speed, In meters per second.
     * 
     * @panic LocTriggering ELbtErrArgument If aSpeed is 
     * zero or negative.
     */
    IMPORT_C void SetMaximumUserSpeed(
        TReal aSpeed );


private: //data
    /**
     * Minimum location update interval. In seconds.
     */
    TInt iMinimumLocationUpdateInterval;

    /**
     * Minimum location update interval when GPS fails. In seconds.
     */
    TInt iMinimumLocationUpdateIntervalWhenGpsFails;

    /**
     * The used positioning module. 
     */
    TUid iUsedPositioningmodule;

    /**
     * Maximum user speed. In meters per second.
     */
    TReal iMaximumUserSpeed;
    
    /**
     * For future use.
     */
    TUint8 iUnused[8];
    };

#endif // LBTTRIGGERINGSYSTEMMANAGEMENTSETTINGS_H
