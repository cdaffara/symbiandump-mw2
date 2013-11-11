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

#include "welcomeviewplugin.h"
#include "welcomeview.h"

#include <xqplugin.h>

WelcomeViewPlugin::WelcomeViewPlugin()
{
}
    
WelcomeViewPlugin::~WelcomeViewPlugin()
{
}

void WelcomeViewPlugin::createView()  
{
}

void WelcomeViewPlugin::destroyView() 
{
}

void WelcomeViewPlugin::activateView()
{
}

void WelcomeViewPlugin::deactivateView()
{
}

QGraphicsWidget* WelcomeViewPlugin::getView()
{
}

void WelcomeViewPlugin::orientationChange(Qt::Orientation orientation)
{
}

void WelcomeViewPlugin::back()
{
}

XQ_EXPORT_PLUGIN2( welcomeviewplugin, WelcomeViewPlugin );
