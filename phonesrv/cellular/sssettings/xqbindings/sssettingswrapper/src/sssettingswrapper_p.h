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

#ifndef SSSETTINGSWRAPPER_P_H
#define SSSETTINGSWRAPPER_P_H

#include <rsssettings.h>
#include <msssettingsobserver.h>


class SsSettingsWrapper;

class SsSettingsWrapperPrivate: public MSSSettingsObserver
{
public:
    SsSettingsWrapperPrivate( SsSettingsWrapper &owner );
    
    ~SsSettingsWrapperPrivate();
    
public: // From MSSSettingsObserver:
    void PhoneSettingChanged( 
        TSSSettingsSetting aSetting, 
        TInt aNewValue ); 

private: // Data: 
    
    SsSettingsWrapper &m_Owner;
    
};

#endif // SSSETTINGSWRAPPER_P

