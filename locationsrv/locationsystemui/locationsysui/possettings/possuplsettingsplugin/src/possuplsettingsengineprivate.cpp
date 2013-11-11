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
 * Description:  Implementation of positioning settings engine private class.
 *
 */
#include "possuplsettingsengineprivate.h"

#include <epos_csuplsettings.h>
#include <epos_csuplsettingparams.h>
#include <QDebug>

//---------------------------------------------------------------------
// PosSuplSettingsEnginePrivate::PosSuplSettingsEnginePrivate()
// (other items were commented in a header).
//---------------------------------------------------------------------
PosSuplSettingsEnginePrivate::PosSuplSettingsEnginePrivate() :
    mPosSuplSettingsObserver(NULL), mSuplSettings(NULL)
    {
    QT_TRAP_THROWING( mSuplSettings = CSuplSettings::NewL();
            mSuplSettings->SetObserverL(*this);
    );
    }

//---------------------------------------------------------------------
// PosSuplSettingsEnginePrivate::~PosSuplSettingsEnginePrivate()
// (other items were commented in a header).
//---------------------------------------------------------------------
PosSuplSettingsEnginePrivate::~PosSuplSettingsEnginePrivate()
    {
    
    
    mServerList.ResetAndDestroy();    
    
    if (mSuplSettings)
        {
        mSuplSettings->RemoveObserver();
        delete mSuplSettings;
        mSuplSettings = NULL;
        }
    }

//---------------------------------------------------------------------
// PosSuplSettingsEnginePrivate::GetServerList()
// (other items were commented in a header).
//---------------------------------------------------------------------
int PosSuplSettingsEnginePrivate::GetServerList(
        QList<PosServerParams> & aParamValues)
    {
    TInt64 slpId;
    HBufC* serverAddress = HBufC::New(KMaxHSLPAddrLen);
    HBufC* iapName = HBufC::New(KMaxIAPLen);
    TBool serverEnabled;
    TBool simChangeRemove;
    TBool usageInHomeNw;
    TBool editable;

    if (!serverAddress || !iapName)
        {
        delete serverAddress;
        delete iapName;
        return KErrNoMemory;
        }
    
    mServerList.ResetAndDestroy();

    TInt error = mSuplSettings->GetAllSlp(mServerList); //handle return value...
    if (error!=KErrNone)
        {
        delete serverAddress;
        delete iapName;
        mServerList.Close();
        mServerList.ResetAndDestroy();
        return error;
        }
    
    TInt count = mServerList.Count();
    for(TInt i = 0; i < count; ++i )
        {        
        PosServerParams params;
        error = mServerList[i]->Get(
                            slpId,
                            serverAddress->Des(),
                            iapName->Des(),
                            serverEnabled,
                            simChangeRemove,
                            usageInHomeNw,
                            editable);
                            
        if (error == 0)
            {
//            QString sName((QChar*) (serverAddress->Ptr()),
//                    serverAddress->Length());            
            
            QString sName = QString::fromUtf16(serverAddress->Ptr(), serverAddress->Length()); 
            
            params.slpId = slpId;
            params.serverAdd = sName;
            params.serverEnabled =  serverEnabled;
            
            aParamValues.append(params);
            }
        }
    
    delete serverAddress;
    delete iapName; 

    return KErrNone;
    }

//---------------------------------------------------------------------
// PosSuplSettingsEnginePrivate::SetServerStatus()
// (other items were commented in a header).
//---------------------------------------------------------------------
int PosSuplSettingsEnginePrivate::SetServerStatus(int aSlpId, bool aEnableFlg)
    {
    return (mSuplSettings->SetServerEnabledFlag(aSlpId, aEnableFlg));
    }


//---------------------------------------------------------------------
// PosSuplSettingsEnginePrivate::RemoveServer()
// (other items were commented in a header).
//---------------------------------------------------------------------
int PosSuplSettingsEnginePrivate::RemoveServer(int aSlpId)
    {
    return (mSuplSettings->RemoveServer(aSlpId));
    }

//---------------------------------------------------------------------
// PosSuplSettingsEnginePrivate::GetRemovableServerList()
// (other items were commented in a header).
//---------------------------------------------------------------------
int PosSuplSettingsEnginePrivate::GetRemovableServerList(QList<
        PosServerParams>& aParamValues)
    {
    qDebug() << "+  PosSuplSettingsEnginePrivate::getRemovableServerList()";
    TInt64 slpId;
    HBufC* serverAddress = HBufC::New(KMaxHSLPAddrLen);
    HBufC* iapName = HBufC::New(KMaxIAPLen);
    TBool serverEnabled;
    TBool simChangeRemove;
    TBool usageInHomeNw;
    TBool editable;
    TBool productConfigured;

    if (!serverAddress || !iapName)
        {
        delete serverAddress;
        delete iapName;
        return KErrNoMemory;
        }
    
    mServerList.ResetAndDestroy();

    TInt error = mSuplSettings->GetAllSlp(mServerList); //handle return value...
    if (error!=KErrNone)
        {
        delete serverAddress;
        delete iapName;
        mServerList.Close();
        mServerList.ResetAndDestroy();
        return error;
        }
    
    TInt count = mServerList.Count();
    qDebug() << "Deleteable server list count = " << count;
    for(TInt i = 0; i < count; ++i )
        {        

        PosServerParams params;
        error = mServerList[i]->Get(slpId,
                                    serverAddress->Des(),
                                    iapName->Des(),
                                    serverEnabled,
                                    simChangeRemove,
                                    usageInHomeNw,
                                    editable);
         TInt err = mServerList[i]->GetServerConfigurationType(productConfigured);
         qDebug()<< "GetServerConfigurationType returns error = "<< err;                   
        if (error == 0)
            {
           QString sName = QString::fromUtf16(serverAddress->Ptr(), serverAddress->Length()); 
            
            params.slpId = slpId;
            params.serverAdd = sName;
            params.serverEnabled =  serverEnabled;
            
            if (!productConfigured)
                {
                aParamValues.append(params);
                }            
            }
        }
    
    delete serverAddress;
    delete iapName; 
    qDebug() << "-  PosSuplSettingsEnginePrivate::getRemovableServerList()";
    return KErrNone;

    }

//---------------------------------------------------------------------
// PosSuplSettingsEnginePrivate::SetObserver()
// (other items were commented in a header).
//---------------------------------------------------------------------
int PosSuplSettingsEnginePrivate::SetObserver(
        PosSuplSettingsChangeObserver* aObserver)
    {
    TInt ret;
    if (!mPosSuplSettingsObserver)
        {
        mPosSuplSettingsObserver = aObserver;
        ret = KErrNone;
        }
    else
        {
        ret = KErrAlreadyExists;
        }
    return ret;
    }

//---------------------------------------------------------------------
// PosSuplSettingsEnginePrivate::HandleSuplSettingsChangeL()
// (other items were commented in a header).
//---------------------------------------------------------------------
void PosSuplSettingsEnginePrivate::HandleSuplSettingsChangeL(
        MSuplSettingsObserver::TSuplSettingsEventType aEvent, TInt aSlpId)
    {
    if (aEvent == MSuplSettingsObserver::ESuplSettingsDBAddEvent || aEvent
            == MSuplSettingsObserver::ESuplSettingsDBDeleteEvent)
        {
        if (mPosSuplSettingsObserver)
            {
            mPosSuplSettingsObserver->handleSuplSettingsChange(aEvent, aSlpId);
            }
        }
    }

//---------------------------------------------------------------------
// PosSuplSettingsEnginePrivate::HandleSuplTriggerStatusChangeL()
// (other items were commented in a header).
//---------------------------------------------------------------------
void PosSuplSettingsEnginePrivate::HandleSuplTriggerStatusChangeL(
        CSuplSettings::TSuplTriggerStatus /*aSuplTriggerStatus*/)
    {
    }

// End of file
