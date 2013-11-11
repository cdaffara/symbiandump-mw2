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


#include "sssettingswrapper.h"
#include "logging.h"


SsSettingsWrapper::SsSettingsWrapper(QObject *parent): 
    QObject(parent)
{
    DPRINT << "DUMMY WRAPPER";
}

SsSettingsWrapper::~SsSettingsWrapper()
{
    DPRINT << "DUMMY WRAPPER";
}
    
    
int SsSettingsWrapper::get(SsSettingsWrapperSettings setting, int &value)
{
    DPRINT << "DUMMY WRAPPER";
    return 0;
}   

int SsSettingsWrapper::set(SsSettingsWrapperSettings setting, int value)
{
    DPRINT << "DUMMY WRAPPER";
    return 0;
}     
    
// End of File. 
