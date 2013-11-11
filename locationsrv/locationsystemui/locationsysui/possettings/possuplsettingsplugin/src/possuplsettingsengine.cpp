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
 * Description: This is the engine for Positioning Supl Settings plugin 
 *							for Positioning Settings's Advanced view
 *
 */
#include "possuplsettingsengine.h"
#include "possuplsettingsengineprivate.h"

#include <QDebug>

//-----------------------------------------------------------------
//PosSuplSettingsEngine::PosSuplSettingsEngine()
//-----------------------------------------------------------------
PosSuplSettingsEngine::PosSuplSettingsEngine() :
    d_ptr(0)
    {
    qDebug() << "+ PosSuplSettingsEngine::PosSuplSettingsEngine()";
    d_ptr = new PosSuplSettingsEnginePrivate();
    //Set the PosSuplSettingsChangeObserver
    if (d_ptr)
        {
        d_ptr->SetObserver(this);
        }
    qDebug() << "- PosSuplSettingsEngine::PosSuplSettingsEngine()";    
    }

//-----------------------------------------------------------------
//PosSuplSettingsEngine::~PosSuplSettingsEngine()
//-----------------------------------------------------------------
PosSuplSettingsEngine::~PosSuplSettingsEngine()
    {
    qDebug() << "+ PosSuplSettingsEngine::~PosSuplSettingsEngine()";    	
    if (d_ptr)
        {
        delete d_ptr;
        d_ptr = 0;
        }
    qDebug() << "- PosSuplSettingsEngine::~PosSuplSettingsEngine()";            
    }
//-----------------------------------------------------------------
//PosSuplSettingsEngine::suplServerList
//-----------------------------------------------------------------
int PosSuplSettingsEngine::suplServerList(QList<PosServerParams>& serverList)
    {
    qDebug() << "+ PosSuplSettingsEngine::suplServerList()";    	
    int result = 0;
    //Clear the server list to remove any junk
    //serverList.clear();
    if (d_ptr)
        {
        result = d_ptr->GetServerList(serverList);
        }
    qDebug() << "- PosSuplSettingsEngine::suplServerList() result = %d" << result;        
    return result;
    }

//-----------------------------------------------------------------
//PosSuplSettingsEngine::deletableServerList
//-----------------------------------------------------------------
int PosSuplSettingsEngine::deletableServerList(
        QList<PosServerParams>& serverList)
    {
    qDebug() << "+ PosSuplSettingsEngine::deletableServerList()";    	
    int result = 0;
    //Clear the server list to remove any junk
    serverList.clear();
    if (d_ptr)
        {
        result = d_ptr->GetRemovableServerList(serverList);
        }
    qDebug() << "- PosSuplSettingsEngine::deletableServerList() result =%d" << result;        
    return result;
    }
//-----------------------------------------------------------------
//PosSuplSettingsEngine::enableServer
//-----------------------------------------------------------------
int PosSuplSettingsEngine::enableServer(int serverIndex, bool value)
    {
    qDebug() << "+ PosSuplSettingsEngine::enableServer()";    	
    int result = 0;
    if (d_ptr)
        {
        result = d_ptr->SetServerStatus(serverIndex, value);
        }
    qDebug() << "- PosSuplSettingsEngine::enableServer() result =%d" << result;      
    return result;
    }
//-----------------------------------------------------------------
//PosSuplSettingsEngine::deleteServer
//-----------------------------------------------------------------
int PosSuplSettingsEngine::deleteServer(int serverIndex)
    {
    qDebug() << "+ PosSuplSettingsEngine::deleteServer()"; 	
    int result = 0;
    if (d_ptr)
        {
        result = d_ptr->RemoveServer(serverIndex);
        }
    qDebug() << "- PosSuplSettingsEngine::deleteServer() result =%d" << result;           
    //Check where to return KErrPermissionDenied
    return result;
    }
//-----------------------------------------------------------------
//PosSuplSettingsEngine::handleSuplSettingsChange
//-----------------------------------------------------------------
void PosSuplSettingsEngine::handleSuplSettingsChange(
        MSuplSettingsObserver::TSuplSettingsEventType /*event*/, int /**slpId*/)
    {
    qDebug() << "+ PosSuplSettingsEngine::handleSuplSettingsChange()"; 		
    emit serverListUpdated();
    qDebug() << "- PosSuplSettingsEngine::handleSuplSettingsChange()"; 	
    }

