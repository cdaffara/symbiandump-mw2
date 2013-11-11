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
* Description:  Implementation of positioning settings engine class. 
*
*/


#include "possettingsengine.h"
#include "possettingscrkey.h"
#include <QDebug>
#include <xqsettingsmanager.h>
#include <xqsettingskey.h>


//---------------------------------------------------------------------
// PosSettingsEngine::PosSettingsEngine()
// (other items were commented in a header).
//---------------------------------------------------------------------
PosSettingsEngine::PosSettingsEngine()
    {
    qDebug() << "+ PosSettingsEngine::PosSettingsEngine()";
    d_ptr = new PosSettingsEnginePrivate( *this );
    qDebug() << "- PosSettingsEngine::PosSettingsEngine()";
    }

//---------------------------------------------------------------------
// PosSettingsEngine::~PosSettingsEngine()
// (other items were commented in a header).
//---------------------------------------------------------------------
PosSettingsEngine::~PosSettingsEngine()
    {
    qDebug() << "+ PosSettingsEngine::~PosSettingsEngine()";
    delete d_ptr;
    d_ptr = NULL;
    qDebug() << "- PosSettingsEngine::~PosSettingsEngine()";
    }

//---------------------------------------------------------------------
// PosSettingsEngine::isPositionTechnologyAvailable()
// (other items were commented in a header).
//---------------------------------------------------------------------
bool PosSettingsEngine::isPositionTechnologyAvailable( 
                                      PositionTechnology posTechnology )
    {
    qDebug() << "+ PosSettingsEngine::isPositionTechnologyAvailable()";	
    bool err = d_ptr->isPositionTechnologyAvailable( posTechnology );   
    qDebug() << "- PosSettingsEngine::isPositionTechnologyAvailable() retvat: " << err;
    return err; 
    }

//---------------------------------------------------------------------
// PosSettingsEngine::setPositionTechnologyState()
// (other items were commented in a header).
//---------------------------------------------------------------------
int PosSettingsEngine::setPositionTechnologyState( 
                                       State posTechnologyState,
                                       PositionTechnology posTechnology )
    {
    qDebug() << "+ PosSettingsEngine::setPositionTechnologyState()";
    qDebug() << "posTechnologyState: " << posTechnologyState;
    int ret = d_ptr->setPositionTechnologyState( posTechnologyState, 
                                              posTechnology ); 
		qDebug() << "- PosSettingsEngine::setPositionTechnologyState()" << ret;       
		return ret;                                       
    }


//---------------------------------------------------------------------
// PosSettingsEngine::isPositionTechnologyEnabled()
// (other items were commented in a header).
//---------------------------------------------------------------------
bool PosSettingsEngine::isPositionTechnologyEnabled(
                                         PositionTechnology posTechnology )
    {
    qDebug() << "+ PosSettingsEngine::isPositionTechnologyEnabled()";
    bool ret = d_ptr->isPositionTechnologyEnabled( posTechnology );  
    qDebug() << "- PosSettingsEngine::isPositionTechnologyEnabled() retval= " << ret;  
    return ret;
    }

//---------------------------------------------------------------------
// PosSettingsEngine::setBackGroundPositioningState()
// (other items were commented in a header).
//---------------------------------------------------------------------
int PosSettingsEngine::setBackGroundPositioningState( 
                                             State backGroundPosState )
    {
    qDebug() << "+ PosSettingsEngine::setBackGroundPositioningState()";
    qDebug() << "backGroundPosState: " <<	backGroundPosState;
    int ret = d_ptr->setBackGroundPositioningState( backGroundPosState );    
    qDebug() << "- PosSettingsEngine::setBackGroundPositioningState() retval= " << ret;
    return ret;
    }

//---------------------------------------------------------------------
// PosSettingsEngine::isPositionTechnologyEnabled()
// (other items were commented in a header).
//---------------------------------------------------------------------
bool PosSettingsEngine::isBackGroundPositioningEnabled()
    {
    qDebug() << "+ PosSettingsEngine::isBackGroundPositioningEnabled()";
    bool ret = d_ptr->isBackGroundPositioningEnabled();    
    qDebug() << "- PosSettingsEngine::isBackGroundPositioningEnabled() retval= " << ret;
    return ret;
    }

// ---------------------------------------------------------------------------
// PosSettingsEngine::backgroundPositioningStatus
// ---------------------------------------------------------------------------
bool PosSettingsEngine::cRBackgroundPositioningStatus()
    {
    qDebug() << "+ PosSettingsEngine::isBackGroundPositioningEnabled()";
    // create a XQSettingsManager instance
    XQSettingsManager* manager = new XQSettingsManager();
    // create a XQSettingsKey object to read the cenrep key value
    XQSettingsKey cenrepKey(XQSettingsKey::TargetCentralRepository,
            KCRUidPositioningSettings.iUid, KPositioningSettingsBackgroundServicesStatus);
    // read the cenrep key value
    QVariant key(manager->readItemValue(cenrepKey,XQSettingsManager::TypeInt));
    // convert the key value to string
    int keyIntValue = key.toInt();
    
    delete manager;
    manager = 0;
    
    qDebug() << "- PosSettingsEngine::cRBackgroundPositioningStatus()";
    return keyIntValue;
    }

// ---------------------------------------------------------------------------
// PosSettingsEngine::setCRBackgroundPositioningStatus
// ---------------------------------------------------------------------------
void PosSettingsEngine::setCRBackgroundPositioningStatus(State backGroundPosState)
    {
    qDebug() << "+ PosSettingsEngine::isBackGroundPositioningEnabled()";
    // create a XQSettingsManager instance
    XQSettingsManager* manager = new XQSettingsManager();
    // create a XQSettingsKey object to read the cenrep key value
    XQSettingsKey cenrepKey(XQSettingsKey::TargetCentralRepository,
            KCRUidPositioningSettings.iUid, KPositioningSettingsBackgroundServicesStatus);
    
    // write the cenrep key value
    if(backGroundPosState == StateEnable)
        QVariant key(manager->writeItemValue(cenrepKey,QVariant(1)));
    else
        QVariant key(manager->writeItemValue(cenrepKey,QVariant(0)));
    
    delete manager;
    manager = 0;
    qDebug() << "- PosSettingsEngine::setCRBackgroundPositioningStatus()";
    }

//---------------------------------------------------------------------
// PosSettingsEngine::handlePositionTechnologyChange()
// (other items were commented in a header).
//---------------------------------------------------------------------
void PosSettingsEngine::handlePositionTechnologyChange()
    {
    qDebug() << "+ PosSettingsEngine::handlePositionTechnologyChange()";
    emit positionTechnologyChange();
    qDebug() << "- PosSettingsEngine::handlePositionTechnologyChange()";
    }
// End of file.
