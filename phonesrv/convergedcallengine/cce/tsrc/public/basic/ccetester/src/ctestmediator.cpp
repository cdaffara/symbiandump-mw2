/*
* Copyright (c) 2008-2009 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  Tests mediator events sent by cce
*
*/


#include "ctestmediator.h"

#include <MediatorDomainUIDs.h>
#include "ccetesterlogger.h"

TBool NotEquals( TCallInfo* aInfo, TCallInfo* aInfo2 )
    {
    TBool dirNotOk = aInfo->iDirection != aInfo2->iDirection;
    TBool indexNotOk = aInfo->iIndex != aInfo2->iIndex;
    TBool serviceIdNotOk = aInfo->iServiceId != aInfo2->iServiceId;
    TBool stateNotOk = aInfo->iState != aInfo2->iState; 
    TBool typeNotOk = aInfo->iType != aInfo2->iType;
    return dirNotOk | indexNotOk | serviceIdNotOk | stateNotOk | typeNotOk;
    }


// ======== MEMBER FUNCTIONS ========

// ---------------------------------------------------------------------------
// Constructor
// ---------------------------------------------------------------------------
//
CTestMediator::CTestMediator()
    {
    }

// ---------------------------------------------------------------------------
// 2nd phase constructor
// ---------------------------------------------------------------------------
//
void CTestMediator::ConstructL()
    {
    iMediatorEvent = CMediatorEventConsumer::NewL( this ); 
    TInt err = iMediatorEvent->SubscribeEvent( 
        KMediatorTelephonyDomain,
        KCatCallInformation, 
        EChangesInCallStates, 
        TVersion( 
            KCallInformationVersionMajor,
            KCallInformationVersionMinor, 
            KCallInformationVersionBuild ) );   
    User::LeaveIfError( err );
    }

// ---------------------------------------------------------------------------
// From MMediatorEventObserver
// ---------------------------------------------------------------------------
//
void CTestMediator::MediatorEventL( 
    TUid /*aDomain*/,
    TUid /*aCategory*/, 
    TInt aEventId, 
    const TDesC8& aData )
    {
    
    if ( EChangesInCallStates == aEventId )
        {
        CCallInfos* infos = CCallInfos::NewLC();
        infos->InternalizeL( aData );
    
        const RPointerArray<TCallInfo>& callInfos = infos->CallInfos();
        for( TInt i = 0; i < callInfos.Count(); i++ )
            {
            TCallInfo *info = callInfos[i];
            iReceivedEvents.AppendL( *info );
            }    
        CleanupStack::PopAndDestroy();
        }
    else
        {
        CCETESTERLOG1("[CCETEST] CTestMediator::MediatorEventL; EventId %d", aEventId);             
        }  
    }

// ---------------------------------------------------------------------------
// Static constructor
// ---------------------------------------------------------------------------
//
CTestMediator* CTestMediator::NewL()
    {
    CTestMediator* self = CTestMediator::NewLC();
    CleanupStack::Pop( self );
    return self;
    }


// ---------------------------------------------------------------------------
// Static constructor
// ---------------------------------------------------------------------------
//
CTestMediator* CTestMediator::NewLC()
    {
    CTestMediator* self = new( ELeave ) CTestMediator;
    CleanupStack::PushL( self );
    self->ConstructL();
    return self;
    }


// ---------------------------------------------------------------------------
// Destructor
// ---------------------------------------------------------------------------
//
CTestMediator::~CTestMediator()
    {
    iExpectedEvents.ResetAndDestroy();
    iReceivedEvents.Close();
    iMediatorEvent->UnsubscribeEvent( 
        KMediatorTelephonyDomain,
        KCatCallInformation, 
        EChangesInCallStates );
        
    delete iMediatorEvent;
    }

// ---------------------------------------------------------------------------
// Adds expectations
// ---------------------------------------------------------------------------
//
void CTestMediator::AddExpectationL( 
    TInt aCallIndex, 
    CCPCall::TCallType aCallType,
    CCPCall::TCallState aCallState, 
    TInt aServiceId, 
    CCPCall::TCallDirection aCallDirection )
    {
    TCallInfo* expectation = new( ELeave )TCallInfo;    
    expectation->iIndex = aCallIndex;
    expectation->iType = aCallType;
    expectation->iState = aCallState;
    expectation->iServiceId = aServiceId;
    expectation->iDirection = aCallDirection;
    iExpectedEvents.AppendL( expectation );
    }

// ---------------------------------------------------------------------------
// Verifies the events
// ---------------------------------------------------------------------------
//
TInt CTestMediator::Verify()
    {   
    const TInt expectedEventCount = iExpectedEvents.Count();
    const TInt receivedEventCount = iReceivedEvents.Count();
    if ( expectedEventCount != receivedEventCount  )
        {
        CCETESTERLOG2("[CCETEST] CTestMediator::Verify failed: expected event count was %d, but received event count was %d", expectedEventCount, receivedEventCount  );
        //return KErrGeneral;
        }
    
    for ( TInt i = 0; i < receivedEventCount; i++ )
        {
        TCallInfo receivedEvent = iReceivedEvents[i];
        TCallInfo* expectedEvent = iExpectedEvents[i];
        if ( NotEquals( &receivedEvent, expectedEvent ) )
            {
            CCETESTERLOG1("[CCETEST] CTestMediator::Verify failed: expected event was not what was received at index %d", i);
            return KErrGeneral;
            }
        }
    
    return KErrNone;
    }
