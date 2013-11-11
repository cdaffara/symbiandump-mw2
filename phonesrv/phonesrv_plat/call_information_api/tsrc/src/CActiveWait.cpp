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
#include "CActiveWait.h"

CActiveWait* CActiveWait::NewL()
    {
    CActiveWait* self = CActiveWait::NewLC();
    CleanupStack::Pop( self ); 
    return self;
    }
    
CActiveWait* CActiveWait::NewLC()
    {
    CActiveWait* self = new( ELeave )CActiveWait();
    CleanupStack::PushL( self );
    self->ConstructL();
    return self;
    }    
    
CActiveWait::~CActiveWait()
    {
    delete iWait;
    }
    
TRequestStatus& CActiveWait::Status()
    {
    return iStatus;
    }

void CActiveWait::WaitForRequest()
    {
    SetActive();
    iWait->Start();
    }

CActiveWait::CActiveWait() : CActive( EPriorityStandard )
    {
    CActiveScheduler::Add( this );
    }
    
void CActiveWait::ConstructL()
    {
    iWait = new( ELeave )CActiveSchedulerWait();
    }
    
void CActiveWait::DoCancel()
    {
    // Note that canceling this will cause stray signal if the waited service is not canceled.
    if( iWait->IsStarted() )
        {
        iWait->AsyncStop(); 
        }
    }
    
void CActiveWait::RunL()
    {
    if( iWait->IsStarted() )
        {
        iWait->AsyncStop();
        }
    }
