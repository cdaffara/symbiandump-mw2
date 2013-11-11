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
* Description: Implementation of connection timer class.
*
*/

#include "posrevgeocodeconnectiontimer.h"
#include "posrevgeocodelogger.h"

//------------------------------------------------------------------------------
// CPosRevGeoCodeConnectionTimer::NewL
//------------------------------------------------------------------------------
CPosRevGeoCodeConnectionTimer* CPosRevGeoCodeConnectionTimer::NewL(
            MPosRevGeoCodeTimeoutObserver& aConnectionTimeoutObserver )
    {
    FUNC("CPosRevGeoCodeConnectionTimer::NewL");
    CPosRevGeoCodeConnectionTimer* self = new ( ELeave ) 
                    CPosRevGeoCodeConnectionTimer( aConnectionTimeoutObserver );
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop();
    return self;
    }

//------------------------------------------------------------------------------
// CPosRevGeoCodeConnectionTimer::CPosRevGeoCodeConnectionTimer
//------------------------------------------------------------------------------
CPosRevGeoCodeConnectionTimer::CPosRevGeoCodeConnectionTimer(
        MPosRevGeoCodeTimeoutObserver& aConnectionTimeoutObserver ):
                        CTimer( EPriorityStandard ),
                        iConnectionTimeoutObserver ( aConnectionTimeoutObserver )
    {
    FUNC("CPosRevGeoCodeConnectionTimer::CPosRevGeoCodeConnectionTimer");
    CActiveScheduler::Add(this);
    }

//------------------------------------------------------------------------------
// CPosRevGeoCodeConnectionTimer::~CPosRevGeoCodeConnectionTimer
//------------------------------------------------------------------------------
CPosRevGeoCodeConnectionTimer::~CPosRevGeoCodeConnectionTimer()
   {
   FUNC("CPosRevGeoCodeConnectionTimer::~CPosRevGeoCodeConnectionTimer");
   Cancel();
   }


//------------------------------------------------------------------------------
// CPosRevGeoCodeConnectionTimer::ConstructL
//------------------------------------------------------------------------------

void CPosRevGeoCodeConnectionTimer::ConstructL()
    {
    FUNC("CPosRevGeoCodeConnectionTimer::ConstructL");
    CTimer::ConstructL();
    }

//------------------------------------------------------------------------------
// CPosRevGeoCodeConnectionTimer::StartTimer
//------------------------------------------------------------------------------
TInt CPosRevGeoCodeConnectionTimer::StartTimer(const TInt aTimeoutVal)
    {
    FUNC("CPosRevGeoCodeConnectionTimer::StartTimer");
    if( IsActive() )
        {
        // If the timer is already running, return error code.
        return KErrInUse;
        }
    After(aTimeoutVal);
    return KErrNone;
    }

//------------------------------------------------------------------------------
// CPosRevGeoCodeConnectionTimer::NewL
//------------------------------------------------------------------------------

void CPosRevGeoCodeConnectionTimer::RunL( )
    {
    FUNC("CPosRevGeoCodeConnectionTimer::RunL");
    iConnectionTimeoutObserver.HandleTimedoutEvent( iStatus.Int() );
    }
    
// End of file
