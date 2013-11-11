/*
* Copyright (c) 2002 - 2007 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:    Test module to implement Firing of startup trigger and Listing of trigger related test cases
*
*/





#include "t_trigIterator.h" 
#include <lbttriggerfiringeventnotifier.h>
#include <lbtsessiontrigger.h>
#include <lbtstartuptrigger.h>
#include <lbttriggerentry.h>



CTrigIterator* CTrigIterator::NewL( RLbt& aLbt, CActiveSchedulerWait* aWait )
    {
    CTrigIterator* self = new ( ELeave ) CTrigIterator( aLbt, aWait );
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop( self );
    return self;
    }

CTrigIterator::CTrigIterator( RLbt& aLbt, CActiveSchedulerWait* aWait )
    : CIdle( CActive::EPriorityStandard ),
      iLbt( aLbt ),
      iWait( aWait )
    {
    }

void CTrigIterator::ConstructL()
    {
    //Create iterator in Location Triggering Server
    iLbt.CreateGetTriggerIteratorL();
    
    //Start the idle task
    Start( TCallBack( CallbackL, this ) );
    }

TInt CTrigIterator::CallbackL( TAny* aAny )
    {
    return reinterpret_cast < CTrigIterator* > ( aAny )->StepL();
    }

TInt CTrigIterator::StepL()
    {
    //Get a trigger object
    CLbtTriggerInfo* trigInfo = iLbt.GetNextTriggerLC(); 
    CLbtTriggerEntry* entry =NULL;//= trigInfo->TriggerEntry();
     
    TInt ret = ETrue; //return ETrue will make the active object scheduled to run again
    if ( entry != NULL )
        {
        //Do something with entry
        }
    else
        {
        //All triggers are retrieved
        if ( iWait )
            {
            iWait->AsyncStop();
            }
        ret = EFalse; //Not further callback
        }
    CleanupStack::PopAndDestroy( trigInfo );
    return ret;
    }

