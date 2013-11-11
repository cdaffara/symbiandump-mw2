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
* Description:  This file implements the Location triggering Management Settings
*                interface
*
*/


#include <lbttriggeringsystemsettings.h>
#include "lbttriggeringsystemmanagementsettings.h"

// ---------------------------------------------------------
// TLbtTriggeringSystemManagementSettings::TLbtTriggeringSystemManagementSettings
//
// (other items were commented in a header).
// ---------------------------------------------------------
// 
EXPORT_C TLbtTriggeringSystemManagementSettings::TLbtTriggeringSystemManagementSettings()
    {
    iMinimumLocationUpdateInterval = 0;
    iMinimumLocationUpdateIntervalWhenGpsFails = 0;
    iUsedPositioningmodule = KLbtAutomaticModuleSelection;
    iMaximumUserSpeed = 0;
    }
    
// ---------------------------------------------------------
// TLbtTriggeringSystemManagementSettings::Type
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
EXPORT_C  TLbtTriggeringSystemSettings::TType TLbtTriggeringSystemManagementSettings::Type()
    {
    return TLbtTriggeringSystemSettings::ETypeManagementSystemSettings;
    }

// ---------------------------------------------------------
// TLbtTriggeringSystemManagementSettings::MinimumLocationUpdateInterval
//
// (other items were commented in a header).
// ---------------------------------------------------------
//    
EXPORT_C TInt TLbtTriggeringSystemManagementSettings::MinimumLocationUpdateInterval() const
    {
    return iMinimumLocationUpdateInterval;
    }
    
// ---------------------------------------------------------
// TLbtTriggeringSystemManagementSettings::SetMinimumLocationUpdateInterval
//
// (other items were commented in a header).
// ---------------------------------------------------------
//    
EXPORT_C void TLbtTriggeringSystemManagementSettings::SetMinimumLocationUpdateInterval(
            TInt aInterval )
    {
    iMinimumLocationUpdateInterval = aInterval;
    }
    
// ---------------------------------------------------------
// TLbtTriggeringSystemManagementSettings::MinimumLocationUpdateIntervalWhenGpsFails
//
// (other items were commented in a header).
// ---------------------------------------------------------
//    
EXPORT_C TInt TLbtTriggeringSystemManagementSettings::MinimumLocationUpdateIntervalWhenGpsFails() const
    {
    return iMinimumLocationUpdateIntervalWhenGpsFails;
    }
  
// ---------------------------------------------------------
// TLbtTriggeringSystemManagementSettings::SetMinimumLocationUpdateIntervalWhenGpsFails
//
// (other items were commented in a header).
// ---------------------------------------------------------
//  
EXPORT_C void TLbtTriggeringSystemManagementSettings::SetMinimumLocationUpdateIntervalWhenGpsFails(
            TInt aInterval )
    {
    iMinimumLocationUpdateIntervalWhenGpsFails = aInterval;
    }
// ---------------------------------------------------------
// TLbtTriggeringSystemManagementSettings::UsedPositioningModule
//
// (other items were commented in a header).
// ---------------------------------------------------------
//    
EXPORT_C TPositionModuleId TLbtTriggeringSystemManagementSettings::UsedPositioningModule() const
    {
    return iUsedPositioningmodule;
    }
// ---------------------------------------------------------
// TLbtTriggeringSystemManagementSettings::SetUsedPositioningModule
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
EXPORT_C void TLbtTriggeringSystemManagementSettings::SetUsedPositioningModule(
            TPositionModuleId aUid )
    {
    iUsedPositioningmodule = aUid;
    }
// ---------------------------------------------------------
// TLbtTriggeringSystemManagementSettings::MaximumUserSpeed
//
// (other items were commented in a header).
// ---------------------------------------------------------
//            
EXPORT_C TReal TLbtTriggeringSystemManagementSettings::MaximumUserSpeed() const
    {
    return iMaximumUserSpeed;
    }
// ---------------------------------------------------------
// TLbtTriggeringSystemManagementSettings::SetMaximumUserSpeed
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
EXPORT_C void TLbtTriggeringSystemManagementSettings::SetMaximumUserSpeed(
            TReal aSpeed )
    {
    iMaximumUserSpeed = aSpeed;
    }
