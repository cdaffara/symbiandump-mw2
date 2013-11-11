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
* Description: This is the first view for Positioning Settings plugin 
*
*/

#include "possettingsview.h"
#include "possettingsengine.h"
#include "possettingsform.h"
#include "possettingspushbuttonitem.h"
#include <QDebug>

// ---------------------------------------------------------------------------
// PosSettingsView::PosSettingsView
// ---------------------------------------------------------------------------
//
PosSettingsView::PosSettingsView(QGraphicsItem 
                                       *parent):CpBaseSettingView(0,parent)
    {   
    qDebug() << "================ Positiong settings is launched ==============================";
    qDebug() << "+ PosSettingsView::PosSettingsView()";
    mSettingsEngine = new PosSettingsEngine;
    if(!mSettingsEngine){
        qDebug() << "- PosSettingsView::PosSettingsView()";
        return;
        }
        
    mSettingsForm = new PosSettingsForm( *mSettingsEngine, this );  
    if(mSettingsForm)
    		this->setWidget( mSettingsForm );   
    qDebug() << "- PosSettingsView::PosSettingsView()";
    }

// ---------------------------------------------------------------------------
// PosSettingsView::~PosSettingsViewl
// ---------------------------------------------------------------------------
//
PosSettingsView::~PosSettingsView()
    {
    qDebug() << "+ PosSettingsView::~PosSettingsView()";
    delete mSettingsEngine;
    mSettingsEngine = NULL;
    
    delete mSettingsForm;
    mSettingsForm = NULL;
    
    qDebug() << "- PosSettingsView::~PosSettingsView()";            
    qDebug() << "================ Positiong settings is closed ==============================";	   
    }

//EOF

