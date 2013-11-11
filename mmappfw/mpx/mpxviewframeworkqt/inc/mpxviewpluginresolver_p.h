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

// This file defines the API for MpxViewPluginResolver.dll

#ifndef __MPXVIEWPLUGINRESOLVERPRIVATE_H__
#define __MPXVIEWPLUGINRESOLVERPRIVATE_H__

#include <QList>
#include <xqpluginloader.h>
#include <xqplugininfo.h>

class MpxViewPlugin;
class MpxPluginViewInterface;

class MpxViewPluginResolverPrivate
{
public:
    // new functions
    MpxViewPluginResolverPrivate( const QList<int>& );
    ~MpxViewPluginResolverPrivate();
    MpxViewPlugin* resolvePlugin();
private:
    MpxPluginViewInterface* resolveAndLoadPlugins( const QList< XQPluginInfo >& impls );
private:
    const QList<int> requestedPlugins;
};
#endif  // __MPXVIEWPLUGINRESOLVERPRIVATE_H__

