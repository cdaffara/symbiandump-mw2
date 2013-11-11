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
* Description: Implementation of data class representing triggering
* system settings.
*
*/


#include "lbttriggeringsystemsettings.h"
#include "lbterrors.h"

// ======== LOCAL FUNCTIONS ========

// ---------------------------------------------------------
// TLbtTriggeringSystemSettings::TLbtTriggeringSystemSettings
//
// Default constructor
// ---------------------------------------------------------
//
EXPORT_C TLbtTriggeringSystemSettings::TLbtTriggeringSystemSettings()
    {
	iTriggeringState=ETriggeringMechanismOn;
    iMinimumTriggerAreaSize=0;
	}


// ---------------------------------------------------------
// TLbtTriggeringSystemSettings::Type
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
EXPORT_C TLbtTriggeringSystemSettings::TType TLbtTriggeringSystemSettings::Type()
    {
	return ETypeSystemSettings;
    }

// ---------------------------------------------------------
// TLbtTriggeringSystemSettings::MinimumTriggerAreaSize
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
EXPORT_C TReal TLbtTriggeringSystemSettings::MinimumTriggerAreaSize() const
    {
	return iMinimumTriggerAreaSize;
    }

// ---------------------------------------------------------
// TLbtTriggeringSystemSettings::SetMinimumTriggerAreaSize
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
EXPORT_C void TLbtTriggeringSystemSettings::SetMinimumTriggerAreaSize(
            TReal aSize )
    {
	if(aSize<0)
        User::Panic(KLbtClientPanicCategory,ELbtErrArgument);

	iMinimumTriggerAreaSize=aSize;
    }

// ---------------------------------------------------------
// TLbtTriggeringSystemSettings::TriggeringMechanismState
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
EXPORT_C TLbtTriggeringMechanismState 
            TLbtTriggeringSystemSettings::TriggeringMechanismState() const
    {
	return iTriggeringState;
    }
            
// ---------------------------------------------------------
// TLbtTriggeringSystemSettings::SetTriggeringMechanismState
//
// (other items were commented in a header).
// ---------------------------------------------------------
//            
EXPORT_C void TLbtTriggeringSystemSettings::SetTriggeringMechanismState(
            TLbtTriggeringMechanismState aState )
            
    {
	iTriggeringState=aState;	
    }

