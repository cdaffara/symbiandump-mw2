/*
* Copyright (c) 2010 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  Implementation of positioning settings plugin helper class. 
*
*/

#include "possettingspluginhelper.h"
#include "possettingspluginhelper_p.h"
#include <QDebug>

//-----------------------------------------------------------------
//PosSettingsPluginHelper::PosSettingsPluginHelper()
//-----------------------------------------------------------------
PosSettingsPluginHelper::PosSettingsPluginHelper() :
    d_ptr(new PosSettingsPluginHelperPrivate())
    {
    qDebug() << "+ PosSettingsPluginHelper::PosSettingsPluginHelper()";
    qDebug() << "- PosSettingsPluginHelper::PosSettingsPluginHelper()";    
    }

//-----------------------------------------------------------------
//PosSettingsPluginHelper::~PosSettingsPluginHelper()
//-----------------------------------------------------------------
PosSettingsPluginHelper::~PosSettingsPluginHelper()
    {
    qDebug() << "+ PosSettingsPluginHelper::~PosSettingsPluginHelper()";    	
    if (d_ptr)
        {
        delete d_ptr;
        }
    qDebug() << "- PosSettingsPluginHelper::~PosSettingsPluginHelper()";            
    }
    
//---------------------------------------------------------------------
// PosSettingsPluginHelper::populatePsyModuleInfoListL()
// (other items were commented in a header).
//---------------------------------------------------------------------
bool PosSettingsPluginHelper::isPositionPsyAvailable()
    {
    qDebug() << "+ PosSettingsPluginHelper::isPositionPsyAvailable()";
    bool ret = d_ptr->isPositionPsyAvailable();
    qDebug() << "- PosSettingsPluginHelper::isPositionPsyAvailable() ret =" << ret;
    return ret;
    }

//---------------------------------------------------------------------
// PosSettingsPluginHelperPrivate::isFeatureFlagEnabledL()
// (other items were commented in a header).
//---------------------------------------------------------------------
bool PosSettingsPluginHelper::isFeatureFlagEnabled()    
    {   
    qDebug() << "+ PosSettingsPluginHelper::isFeatureFlagEnabled()";	
    bool ret = d_ptr->isFeatureFlagEnabled();
    qDebug() << "- PosSettingsPluginHelper::isFeatureFlagEnabled() ret =" << ret;
    return ret;
		}
		
// End of file
