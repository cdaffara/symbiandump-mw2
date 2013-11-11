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

#ifndef SIMPLEVIEWLUGIN_H_
#define SIMPLEVIEWPLUGIN_H_

#include <QObject>
#include <mpxviewpluginqt.h>

class HbView;

class SimpleViewPlugin : public MpxViewPlugin, public MpxPluginViewInterface
    {
    Q_INTERFACES(MpxPluginViewInterface)
    Q_OBJECT
    
public:

	SimpleViewPlugin();
	
	virtual ~SimpleViewPlugin();
	
    virtual void createView();  

    virtual void destroyView();

    virtual void activateView();

    virtual void deactivateView();

    virtual QGraphicsWidget* getView();

    virtual MpxViewPlugin *viewPlugin();

public slots:

    virtual void orientationChange(Qt::Orientation orientation);

    virtual void back();
	
private:
	
	HbView *viewInstance;
    
    };

#endif /* SIMPLEVIEWLUGIN_H_ */
