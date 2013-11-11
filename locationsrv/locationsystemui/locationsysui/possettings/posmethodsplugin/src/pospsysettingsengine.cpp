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
 * Description:  Implementation of psy settings engine class. 
 *
 */

#include "pospsysettingsengine.h"
#include <QDebug>

//---------------------------------------------------------------------
// PosPsySettingsEngine::PosPsySettingsEngine()
// Constructor
//---------------------------------------------------------------------
PosPsySettingsEngine::PosPsySettingsEngine()
    {
    qDebug() << "+ PosSettingsEngine::PosPsySettingsEngine()";		
    d_ptr = new PosPsySettingsEnginePrivate(*this);
    qDebug() << "- PosSettingsEngine::PosPsySettingsEngine()";	
    }

//---------------------------------------------------------------------
// PosPsySettingsEngine::~PosPsySettingsEngine()
// Destructor
//---------------------------------------------------------------------
PosPsySettingsEngine::~PosPsySettingsEngine()
    {
    qDebug() << "+ PosSettingsEngine::~PosPsySettingsEngine()";	
    delete d_ptr;
    d_ptr = NULL;      
    qDebug() << "- PosSettingsEngine::~PosPsySettingsEngine()";	  
    }

//---------------------------------------------------------------------
// PosPsySettingsEngine::listPsys( QList<PosPsyInfo>& psyList )
// 
//---------------------------------------------------------------------
int PosPsySettingsEngine::listPsys(QList<PosPsyInfo>& psyList)
    {
    qDebug() << "+ PosSettingsEngine::listPsys()";	
    int ret = d_ptr->listPsys(psyList);
    qDebug() << "- PosSettingsEngine::listPsys()" << ret;	
    return ret;
    }

//---------------------------------------------------------------------
// PosPsySettingsEngine::changePsyState( TUid psyModuleId, PosPsyState psyState )
// 
//---------------------------------------------------------------------
int PosPsySettingsEngine::changePsyState(TUid psyModuleId,
        PosPsyState psyState)
    {
    qDebug() << "+ PosSettingsEngine::changePsyState()";	
    qDebug() << "psystate = " << psyState;
    int ret = d_ptr->changePsyState(psyModuleId, psyState);
    qDebug() << "- PosSettingsEngine::changePsyState()" << ret;	
    return ret;
    }

//---------------------------------------------------------------------
// PosPsySettingsEngine::handlePositionTechnologyChange
// 
//---------------------------------------------------------------------
void PosPsySettingsEngine::handlePositionTechnologyChange()
    {
    qDebug() << "+ PosSettingsEngine::handlePositionTechnologyChange()";   
    // emit the signal when there is a change in the list of available psys
    emit PSYChange();
    qDebug() << "- PosSettingsEngine::handlePositionTechnologyChange()";   
    }

