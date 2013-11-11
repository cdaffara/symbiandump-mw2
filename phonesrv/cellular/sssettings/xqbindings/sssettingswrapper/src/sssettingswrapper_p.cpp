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

#include "sssettingswrapper_p.h"
#include "sssettingswrapper.h"
#include "logging.h"
#include <rsssettings.h>

/*!
  SsSettingsWrapperPrivate::SsSettingsWrapperPrivate
 */
SsSettingsWrapperPrivate::SsSettingsWrapperPrivate( SsSettingsWrapper &owner ) : m_Owner(owner)
{
    DPRINT;
}

/*!
  SsSettingsWrapperPrivate::~SsSettingsWrapperPrivate
 */
SsSettingsWrapperPrivate::~SsSettingsWrapperPrivate()
{
    DPRINT;
}

/*!
  SsSettingsWrapperPrivate::PhoneSettingChanged
 */
void SsSettingsWrapperPrivate::PhoneSettingChanged( 
    TSSSettingsSetting aSetting, 
    TInt aNewValue )
{
    DPRINT << ": IN";
    DPRINT << ": aSetting: " << static_cast<SsSettingsWrapperSettings>(aSetting);
    DPRINT << ": aNewValue: " << aNewValue;
        
    emit m_Owner.phoneSettingChanged( 
        static_cast<SsSettingsWrapperSettings>(aSetting),
        static_cast<int>(aNewValue));
        
    DPRINT << ": OUT";
}
    
// End of File. 
