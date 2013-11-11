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

#ifndef MPXVIEWFRAMEWORK_H
#define MPXVIEWFRAMEWORK_H

#include <QObject>
#include <QList>
#include <qglobal.h>
#include <hbmainwindow.h>

class MpxViewPlugin;
class HbView;

#ifdef BUILD_VIEWFRAMEWORK
#define VFDLL_EXPORT Q_DECL_EXPORT
#else
#define VFDLL_EXPORT Q_DECL_IMPORT
#endif

class VFDLL_EXPORT MpxViewFramework : public HbMainWindow
    {
    Q_OBJECT
    
public:
    MpxViewFramework(QWidget *parent = 0, Hb::WindowFlags windowFlags = Hb::WindowFlagNone);
    
    virtual ~MpxViewFramework();

    virtual MpxViewPlugin *resolvePlugin(const QList<int>& requestedPlugins);
    
private:
    
    Q_DISABLE_COPY(MpxViewFramework)    
    
    };

#endif //VIEWFRAMEWORK_H
