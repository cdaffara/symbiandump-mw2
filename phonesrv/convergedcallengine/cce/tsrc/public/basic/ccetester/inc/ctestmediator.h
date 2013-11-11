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


#ifndef CTESTMEDIATOR_H
#define CTESTMEDIATOR_H

#include <e32base.h>
#include <ccpdefs.h>
#include <callinformation.h>    //Call Information Mediator API
#include <MediatorEventConsumer.h>

class TMediatorEvent 
    {
public:
    TInt iCallIndex; 
    CCPCall::TCallType iCallType;
    CCPCall::TCallState iCallState; 
    TInt iServiceId;
    CCPCall::TCallDirection iCallDirection;
    
    };

NONSHARABLE_CLASS( CTestMediator ): public CBase, private MMediatorEventObserver
    {
public:
    static CTestMediator* NewL();
    static CTestMediator* NewLC();  

    /**
    * Destructor.
    */
    virtual ~CTestMediator();
    
    void AddExpectationL( 
        TInt aCallIndex, 
        CCPCall::TCallType aCallType,
        CCPCall::TCallState aCallState, 
        TInt aServiceId, 
        CCPCall::TCallDirection aCallDirection );
    
    TInt Verify();

private:

    CTestMediator();

    void ConstructL();
    
    void MediatorEventL( 
        TUid aDomain,
        TUid aCategory, 
        TInt aEventId, 
        const TDesC8& aData );
    
private: // data
    
    RPointerArray<TCallInfo> iExpectedEvents;
    RArray<TCallInfo> iReceivedEvents;
    
    CMediatorEventConsumer* iMediatorEvent;

    };

#endif // CTESTMEDIATOR_H
