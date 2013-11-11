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

#include <hbmainwindow.h>
#include <hbview.h>

#include <mpxviewframeworkqt.h>
#include <mpxviewpluginresolver.h>
#include <mpxviewpluginqt.h>

MpxViewFramework::MpxViewFramework(QWidget *parent, Hb::WindowFlags windowFlags)
:
HbMainWindow(parent, windowFlags)
{
}

MpxViewFramework::~MpxViewFramework()
{
}

MpxViewPlugin *MpxViewFramework::resolvePlugin(const QList<int>& requestedPlugins)
{
    MpxViewPlugin *viewPlugin = 0;
    MpxViewPluginResolver *viewResolver = new MpxViewPluginResolver(requestedPlugins);
    if (viewResolver!=0) {
        viewPlugin = viewResolver->resolvePlugin();
        delete viewResolver;
    }
    return viewPlugin;
}

