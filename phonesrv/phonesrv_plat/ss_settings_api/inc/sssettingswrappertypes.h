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
 * Description:
 *
 */

#ifndef SSSETTINGSWRAPPERTYPES_H_
#define SSSETTINGSWRAPPERTYPES_H_

    
/**
* It enumerates all SSSettings settings.
*
* Cug         - closed user group setting,
*                     for some values see TSSSettingsCugValue.
* Clir        - default setting for calling line identification
*                     restriction, see SsSettingsWrapperClirValue.
* Als         - selected line, see SsSettingsWrapperAlsValue.
* AlsBlocking - line selection blocking, see SsSettingsWrapperAlsBlockingValue.
* DefaultCug - value for cug default.
*/
typedef enum {
    Cug = 0,  // == ESSSettingsCug
    Clir = 1,   // == ESSSettingsClir
    Als = 2,    // == ESSSettingsAls
    AlsBlocking = 3,  // == ESSSettingsAlsBlocking
    DefaultCug = 4    // == ESSSettingsCugDefault
    } SsSettingsWrapperSettings;
    
/**
* Translation table for TSSSettingsAlsValue
* It enumerates all values for ALS.
*
* AlsNotSupported - ALS not supported, always primary line.
* AlsPrimary - ALS supported, primary line selected.
* AlsAlternate - ALS supported, alternate line selected.
*/
typedef enum  {
    AlsNotSupported = 0, // == TSSSettingsAlsValue::ESSSettingsAlsNotSupported
    AlsPrimary = 1, // == TSSSettingsAlsValue::ESSSettingsAlsPrimary
    AlsAlternate = 2 // == TSSSettingsAlsValue::ESSSettingsAlsAlternate
    } SsSettingsWrapperAlsValue;   

/**
* Translation table for TSSSettingsAlsBlockingValue
* It enumerates all values for ALS blocking.
*
* AlsBlockingNotSupported - ALS blocking not supported.
* AlsBlockingOff - ALS blocking off.
* AlsBlockingOn - ALS blocking on.
*/
typedef enum {
    AlsBlockingNotSupported = 0, // == ESSSettingsAlsBlockingNotSupported,
    AlsBlockingOff, // == ESSSettingsAlsBlockingOff
    AlsBlockingOn // == ESSSettingsAlsBlockingOn
    } SsSettingsWrapperAlsBlockingValue;

/**
* It enumerates all values for CLIR.
*
* ClirNetworkDefault - use network default
* ClirExplicitInvoke - by default, invoke CLIR
* ClirExplicitSuppress - by default, suppress CLIR
*/
typedef enum {
    ClirNetworkDefault,    // == ESSSettingsClirNetworkDefault
    ClirExplicitInvoke,    // == ESSSettingsClirExplicitInvoke
    ClirExplicitSuppress    // == ESSSettingsClirExplicitSuppress
    } SsSettingsWrapperClirValue;
    
#endif /* SSSETTINGSWRAPPERTYPES_H_ */
