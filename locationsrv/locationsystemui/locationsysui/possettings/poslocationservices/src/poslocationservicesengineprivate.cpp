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
 * Description: Implementation of PosLocationServicesEnginePrivate class.  
 *
 */
#include "poslocationservicesengineprivate.h"

#include <epos_csuplsettings.h>
#include <epos_csuplsettingparams.h>
#include <epos_csuplsettingsconstants.h>

//----------------------------------------------------------------------
// PosLocationServicesEnginePrivate::PosLocationServicesEnginePrivate
//----------------------------------------------------------------------
PosLocationServicesEnginePrivate::PosLocationServicesEnginePrivate(
        PosLocationServiceUpdateObserver& aObserver) :
    mObserver(aObserver)
    {
    QT_TRAP_THROWING( mSuplSettings = CSuplSettings::NewL();
            mSuplSettings->SetSessionObserverL( *this ) );
    }

//----------------------------------------------------------------------
// PosLocationServicesEnginePrivate::~PosLocationServicesEnginePrivate
//----------------------------------------------------------------------
PosLocationServicesEnginePrivate::~PosLocationServicesEnginePrivate()
    {
    mSuplSettings->RemoveSessionObserver();
    delete mSuplSettings;
    mSuplSettings = NULL;
    }

//----------------------------------------------------------------------
// PosLocationServicesEnginePrivate::GetLocationServices
//----------------------------------------------------------------------
int PosLocationServicesEnginePrivate::GetLocationServices(QList<
        PosLocationServiceInfo>& aLocationServiceList)
    {
    int err = 0;
    TRAP(err, GetLocationServicesL(aLocationServiceList));
    return err;
    }
//----------------------------------------------------------------------
// PosLocationServicesEnginePrivate::GetLocationServicesL
//----------------------------------------------------------------------
void PosLocationServicesEnginePrivate::GetLocationServicesL(QList<
        PosLocationServiceInfo>& aLocationServiceList)
    {
    RPointerArray<CTriggerParams> triggerParams;
    CleanupClosePushL(triggerParams);
    int error = mSuplSettings->GetTriggerParams(triggerParams);

    int count = triggerParams.Count();

    //if there are no trigger params then leave with KErrNotFound
    if (count <= 0)
        {
        User::Leave(KErrNotFound);
        }
    
    //for each entry in the paramlist populate the locationservice list
    for (int i = 0; i < count; ++i)
        {

        TInt64 sessionId;
        TUint64 outstandingTrigger;
        TUint64 interval;
        TTime endTime;
        TBool notificationPresent;
        TBool triggerNotificationStatus;
        CTriggerParams::TTriggerType triggerType;
        CTriggerParams::TRequestType requestType;
        HBufC* sessionName = HBufC::NewLC(
                KMaxTriggerSessionNameLen);

        error = triggerParams[i]->Get(sessionId, sessionName->Des(),
                notificationPresent, triggerNotificationStatus, triggerType,
                requestType, outstandingTrigger, interval);
        triggerParams[i]->GetTriggerEndTime(endTime);

        if (error == KErrNone)
            {
            PosLocationServiceInfo serviceInfo;
            serviceInfo.mServiceId = sessionId;
            QString sName((QChar*) (sessionName->Ptr()),
                    sessionName->Length());

            serviceInfo.mServiceName.append(sName);
            serviceInfo.mUpdateInterval = interval;
            serviceInfo.mEndDateTime.setDate(QDate(endTime.DateTime().Year(),
                    endTime.DateTime().Month() + 1, endTime.DateTime().Day()
                            + 1));
            serviceInfo.mEndDateTime.setTime(QTime(endTime.DateTime().Hour(),
                    endTime.DateTime().Minute(), endTime.DateTime().Second(),
                    endTime.DateTime().MicroSecond()));

            aLocationServiceList.append(serviceInfo);
            }
        CleanupStack::PopAndDestroy(sessionName);
        }
    CleanupStack::Pop(&triggerParams);
    triggerParams.ResetAndDestroy();
    
    //leave if there is an error
    User::LeaveIfError(error);
    }

//----------------------------------------------------------------------
// PosLocationServicesEnginePrivate::RemoveLocationService
//----------------------------------------------------------------------
int PosLocationServicesEnginePrivate::RemoveLocationService(int aServiceId)
    {
    return mSuplSettings->CancelTriggerSession(aServiceId);
    }

//----------------------------------------------------------------------
// PosLocationServicesEnginePrivate::HandleSuplSessionChangeL
//----------------------------------------------------------------------
void PosLocationServicesEnginePrivate::HandleSuplSessionChangeL(
        TSuplSessionEventType /*aEvent*/, TInt64 /*aSessionId*/)
    {
    mObserver.handleLocationServiceUpdate();
    }

// End of file
