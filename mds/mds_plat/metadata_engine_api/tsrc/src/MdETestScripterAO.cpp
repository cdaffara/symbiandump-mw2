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

#include "MdETestScripterAO.h"
#include "MdETestScripter.h"
#include <e32debug.h>

CMdETestScripterAO::CMdETestScripterAO() : CActive( EPriorityStandard )
    {
    }

CMdETestScripterAO* CMdETestScripterAO::NewLC()
    {
    CMdETestScripterAO* self = new (ELeave) CMdETestScripterAO();
    CleanupStack::PushL( self );
    self->ConstructL();
    return self;
    }

CMdETestScripterAO* CMdETestScripterAO::NewL()
    {
    CMdETestScripterAO* self = CMdETestScripterAO::NewLC();
    CleanupStack::Pop();
    return self;
    }

void CMdETestScripterAO::ConstructL()
    {
    CActiveScheduler::Add( this );
    }

CMdETestScripterAO::~CMdETestScripterAO()
    {
    Cancel();
    }

void CMdETestScripterAO::Start( CMdETestScripter* aScripter, TEventIf aEvent )
    {
    Cancel();
    iScripter = aScripter;
    iEvent = aEvent;
    SetActive();
    }

void CMdETestScripterAO::RunL()
    {
    iScripter->CheckAsyncResultArrayL( iEvent );
    iScripter->TestModuleIf().Event( iEvent );
    }

void CMdETestScripterAO::DoCancel()
    {
    }

TInt CMdETestScripterAO::RunError(TInt aError)
    {
    return aError;
    }
