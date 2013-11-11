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

#ifndef __MPXVIEWPLUGINRESOLVER_H__
#define __MPXVIEWPLUGINRESOLVER_H__

#include <QList>

#include <mpxviewpluginqt.h>

#ifdef BUILD_VIEWFRAMEWORK
#define VFDLL_EXPORT Q_DECL_EXPORT
#else
#define VFDLL_EXPORT Q_DECL_IMPORT
#endif

class MpxViewPluginResolverPrivate;

class VFDLL_EXPORT MpxViewPluginResolver
    {
public:
    // new functions
    MpxViewPluginResolver( const QList<int>& );
    virtual ~MpxViewPluginResolver();
    MpxViewPlugin* resolvePlugin();
private:
    MpxViewPluginResolverPrivate* impl;
    };

#endif  // __MPXVIEWPLUGINRESOLVER_H__

