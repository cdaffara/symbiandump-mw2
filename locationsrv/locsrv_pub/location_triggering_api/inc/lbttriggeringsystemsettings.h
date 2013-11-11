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
* Description:  Location triggering related system settings
*
*/


#ifndef LBTTRIGGERINGSYSTEMSETTINGS_H
#define LBTTRIGGERINGSYSTEMSETTINGS_H

#include <lbtcommon.h>

/**
 * Structure for location triggering related system settings.
 *
 * @lib lbt.lib
 *
 * @since S60 5.1
 */
class TLbtTriggeringSystemSettings
    {
public:
    /**
     * Type of system setting class.
     */
    enum TType
        {
        ///System settings
        ETypeSystemSettings     = 1,
        ///Management system settings
        ETypeManagementSystemSettings = 2
        };

    /**
     * Default constructor.
     */
    IMPORT_C TLbtTriggeringSystemSettings();
    
    /**
     * Gets the type of system setting class.
     */
    IMPORT_C virtual TType Type();

    /**
     * Returns minimum size of trigger area. 
     *
     * When creating a trigger with a circular geographical 
     * trigger area, the radius of the trigger area must be 
     * greater than this setting. 
     *
     * This function returns zero if the minimum trigger area
     * size is not set.
     *
     * @return The minimum size of trigger area. In meters.
     */
    IMPORT_C TReal MinimumTriggerAreaSize() const;

    /**
     * Sets the minum size of trigger area. This is for internal use only. 
     *
     * @panic LocTriggering ELbtErrArgument If aSize is negative.
     * @param aSize The minimum size of trigger area. In meters.
     */
    IMPORT_C void SetMinimumTriggerAreaSize(
        TReal aSize );

    /**
     * Returns the triggering mechanism state. 
     *
     * This function returns ETriggeringMechanismOn if
     * the triggering mechanism state is not set. 
     *
     * @return The triggering mechanism state. 
     */
    IMPORT_C TLbtTriggeringMechanismState 
        TriggeringMechanismState() const;

    /**
     * Sets the triggering mechanism state. This is for internal use only. 
     *
     * @param aState The triggering mechanism state.
     */
    IMPORT_C void SetTriggeringMechanismState(
        TLbtTriggeringMechanismState aState );

protected:
    /**
     * Type of the setting class
     */
    TType iType;
    
private: //data
    /**
     * The minimum size of trigger area.
     */
    TReal iMinimumTriggerAreaSize;

    /**
     * Triggering mechanism state. 
     */
    TLbtTriggeringMechanismState iTriggeringState;

    /**
     * For future use.
     */
    TUint8 iUnused[8];
    };

#endif //LBTCOMMON_H
