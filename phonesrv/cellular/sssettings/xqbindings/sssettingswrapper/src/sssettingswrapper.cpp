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


#include <sssettingswrapper.h>
#include <rsssettings.h>
#include "sssettingswrapper_p.h"
#include "logging.h"

/*!
  SsSettingsWrapper::SsSettingsWrapper
 */
SsSettingsWrapper::SsSettingsWrapper(QObject *parent): 
    QObject(parent), m_Priv(new SsSettingsWrapperPrivate(*this))
{
    DPRINT << ": IN";
    
    // Initialize RSSSettings 
    m_ssSettings = new RSSSettings;
    int err = m_ssSettings->Open();
    DPRINT << "err: " << err;
    // Register for notifications
    m_ssSettings->RegisterAll( *m_Priv );  
    
    DPRINT << ": OUT";
}

/*!
  SsSettingsWrapper::~SsSettingsWrapper
 */
SsSettingsWrapper::~SsSettingsWrapper()
{
    DPRINT << ": IN";
    
    if(m_ssSettings){
        m_ssSettings->Close();
    }
    delete m_ssSettings;
    delete m_Priv;
    
    DPRINT << ": OUT";
}
    
/*!
  SsSettingsWrapper::get
 */
int SsSettingsWrapper::get(SsSettingsWrapperSettings setting, int &value)
{
    DPRINT << ": IN";
    
    int err = m_ssSettings->Get(static_cast<TSSSettingsSetting>(setting), value);
    
    DPRINT << ": err: " << err;
    DPRINT << ": setting: " << setting;
    DPRINT << ": value: " << value;
    DPRINT << ": OUT" ;
    return err;
}   

/*!
  SsSettingsWrapper::set
 */
int SsSettingsWrapper::set(SsSettingsWrapperSettings setting, int value)
{
    DPRINT << ": IN";
    
    int err = m_ssSettings->Set( static_cast<TSSSettingsSetting>(setting), value );
    
    DPRINT << ": err: " << err;
    DPRINT << ": setting: " << setting;
    DPRINT << ": value: " << value;
    DPRINT << ": OUT" ;
    return err;   
}     
    
// End of File.
