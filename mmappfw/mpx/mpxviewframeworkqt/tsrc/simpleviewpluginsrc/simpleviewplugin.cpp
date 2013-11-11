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

#include "simpleviewplugin.h"
#include "simpleview.h"

#include <xqplugin.h>

/*
#ifndef SIMPLE_VIEW_NO
#define SIMPLE_VIEW_NO 0;
#endif
*/

SimpleViewPlugin::SimpleViewPlugin() : MpxViewPlugin(), viewInstance(0)
{
}
    
SimpleViewPlugin::~SimpleViewPlugin()
{
    delete viewInstance;
}

MpxViewPlugin *SimpleViewPlugin::viewPlugin()
{
    return this;
}

void SimpleViewPlugin::createView()  
{
}

void SimpleViewPlugin::destroyView() 
{
}

void SimpleViewPlugin::activateView()
{
}

void SimpleViewPlugin::deactivateView()
{
}

QGraphicsWidget* SimpleViewPlugin::getView()
{
    if (viewInstance==0) {
        viewInstance = new SimpleView(QString("Simpleview#%1").arg(SIMPLE_VIEW_NO));
    }
    return viewInstance;
}

void SimpleViewPlugin::orientationChange(Qt::Orientation orientation)
{
}

void SimpleViewPlugin::back()
{
}

XQ_EXPORT_PLUGIN2(SVIEW_PLUGIN_ID, SimpleViewPlugin);
