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
* Description: Implementation of class representing session trigger
* entries.
*
*/


#include <s32strm.h>
#include "lbtsessiontrigger.h"
#include "lbttriggerentry.h"
#include "lbttriggerconditionbase.h"
#include "lbttriggerconditionarea.h"

const TInt EnabledState=1;
const TInt DisabledState=2;
const TInt ConditionArea=1;
// ======== LOCAL FUNCTIONS ========

// ---------------------------------------------------------------------------
// CLbtSessionTrigger::NewL()
//
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//
EXPORT_C  CLbtSessionTrigger* CLbtSessionTrigger::NewL()
    {
  	CLbtSessionTrigger* self = new( ELeave ) CLbtSessionTrigger;
    CleanupStack::PushL( self );
    self->ConstructL();
	CleanupStack::Pop( self );    
    return self;
    }
    
    
// ---------------------------------------------------------------------------
// CLbtSessionTrigger::NewL()
//
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//
EXPORT_C  CLbtSessionTrigger* CLbtSessionTrigger::NewLC()
    {
  	CLbtSessionTrigger* self = new( ELeave ) CLbtSessionTrigger;
    CleanupStack::PushL( self );
    self->ConstructL();
	return self;
    }
// ---------------------------------------------------------------------------
// CLbtSessionTrigger::NewL(TCoordinate aCenter,
//            TReal aRadius)
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//
EXPORT_C  CLbtSessionTrigger* CLbtSessionTrigger::NewL( 
            const TDesC& aName,
            TLbtTriggerState aState,
            CRequestor::TRequestorType aRequestorType,
            CRequestor::TRequestorFormat aRequestorFormat,
            const TDesC& aRequestorData,
            TUid aManagerUi,
            CLbtTriggerConditionBase* aCondition)
    {
	CLbtSessionTrigger* self = new( ELeave ) CLbtSessionTrigger;
    CleanupStack::PushL( self );
    self->ConstructL(aName,aState,aRequestorType,aRequestorFormat,aRequestorData,aManagerUi,aCondition);
	CleanupStack::Pop( self );    
    return self;
    }

// ---------------------------------------------------------------------------
// CLbtSessionTrigger::NewL()
//
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//
EXPORT_C  CLbtSessionTrigger* CLbtSessionTrigger::NewL( 
            const TDesC& aName,
            CLbtTriggerEntry::TLbtTriggerState aState,
            const RRequestorStack& aRequestors,
            TUid aManagerUi,
            CLbtTriggerConditionBase* aCondition )
            
    {
 	CLbtSessionTrigger* self = new( ELeave ) CLbtSessionTrigger;
    CleanupStack::PushL( self );
    self->ConstructL(aName,aState, aRequestors, aManagerUi, aCondition);
 	CleanupStack::Pop( self );    
    return self;
    }
 
// ---------------------------------------------------------------------------
// CLbtSessionTrigger::Type()
//
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//        
EXPORT_C CLbtTriggerEntry::TType CLbtSessionTrigger::Type() const
    {
	return CLbtTriggerEntry::ETypeSession;
    }

// ---------------------------------------------------------------------------
// CLbtSessionTrigger::CLbtSessionTrigger()
// Default constructor
// ---------------------------------------------------------------------------
//
CLbtSessionTrigger::CLbtSessionTrigger()
    {
	
    }

// ---------------------------------------------------------------------------
// CLbtSessionTrigger::~CLbtSessionTrigger()
// Destructor
// ---------------------------------------------------------------------------
//
CLbtSessionTrigger::~CLbtSessionTrigger()
    {
	
    }

// ---------------------------------------------------------------------------
// CLbtSessionTrigger::ConstructL()
// Symbian 2nd phase constructor can leave.
// ---------------------------------------------------------------------------
//
void CLbtSessionTrigger::ConstructL(const TDesC& aName,TLbtTriggerState aState,
            CRequestor::TRequestorType aRequestorType,
            CRequestor::TRequestorFormat aRequestorFormat,
            const TDesC& aRequestorData,
            TUid aManagerUi,
            CLbtTriggerConditionBase* aCondition)
    {
	SetNameL(aName);
	SetState(aState);
    SetRequestorL(aRequestorType,aRequestorFormat,aRequestorData);
	SetCondition(aCondition);
	SetManagerUi(aManagerUi);
    }

// ---------------------------------------------------------------------------
// CLbtSessionTrigger::ConstructL()
// Symbian 2nd phase constructor can leave.
// ---------------------------------------------------------------------------
//
void CLbtSessionTrigger::ConstructL(const TDesC& aName,
            CLbtTriggerEntry::TLbtTriggerState aState,
            const RRequestorStack& aRequestors,
            TUid aManagerUi,
            CLbtTriggerConditionBase* aCondition)
            
    {
	SetNameL(aName);
	SetState(aState);
	SetManagerUi(aManagerUi);
	SetCondition(aCondition);
	SetRequestorsL(aRequestors);
	} 

// ---------------------------------------------------------------------------
// CLbtSessionTrigger::DoInternalizeL()
// (other items were commented in a header).
// ---------------------------------------------------------------------------
// 
void CLbtSessionTrigger::DoInternalizeL(RReadStream& aStream) 
    {
	SetId(aStream.ReadInt32L());
	TBuf16<KLbtMaxNameLength> name;
	TInt namLength=aStream.ReadInt32L();
	aStream.ReadL(name,namLength);
	SetNameL(name);
	TInt state;
	state=aStream.ReadInt8L();
	TLbtTriggerState trigState=CLbtTriggerEntry::EStateEnabled;
	if(state==EnabledState)
	trigState=CLbtTriggerEntry::EStateEnabled;
	else if(state==DisabledState) 
	trigState=CLbtTriggerEntry::EStateDisabled;
	SetState(trigState);
	TInt rearmTime = aStream.ReadInt32L();
	SetTimeToRearm( rearmTime );
	TUid managerUid;
	managerUid.iUid=aStream.ReadInt32L();
	SetManagerUi(managerUid);
	TInt condType=aStream.ReadInt8L();
	if(condType==ConditionArea)
	    {
		CLbtTriggerConditionArea *area=CLbtTriggerConditionArea::NewL();
		area->InternalizeL(aStream);
		SetCondition(area);
    	} 
    }

// ---------------------------------------------------------------------------
// CLbtSessionTrigger::DoExternalizeL()
// (other items were commented in a header).
// ---------------------------------------------------------------------------
// 
void CLbtSessionTrigger::DoExternalizeL(RWriteStream& aStream) const 
    {
	aStream.WriteInt32L(Id());
	const TDesC& name=Name();
	TInt len=name.Length();
	aStream.WriteInt32L(len);
	aStream.WriteL(name,len);
	TLbtTriggerState state=State();
	
	if(state==CLbtTriggerEntry::EStateEnabled)
		aStream.WriteInt8L(EnabledState);
	else
		aStream.WriteInt8L(DisabledState);
	TInt rearmTime = TimeToRearm();
	aStream.WriteInt32L( rearmTime );
	aStream.WriteInt32L(ManagerUi().iUid);
	const CLbtTriggerConditionBase* Cond=GetCondition();
	CLbtTriggerConditionBase::TType condType;
	if(Cond!=NULL)
	    {
	    condType=Cond->Type();
	    if(condType==CLbtTriggerConditionBase::ETriggerConditionArea)
			aStream.WriteInt8L(ConditionArea);
	    Cond->ExternalizeL(aStream);
	    }
	else
	    {
	    aStream.WriteInt8L(0);
	    }    
	}

// ---------------------------------------------------------------------------
// CLbtSessionTrigger::ConstructL()
// Symbian 2nd phase constructor can leave.
// ---------------------------------------------------------------------------
//
void CLbtSessionTrigger::ConstructL()
    {
	SetState(CLbtTriggerEntry::EStateEnabled);
	SetId(KLbtNullTriggerId);
	SetCondition(NULL);
	SetNameL(KNullDesC());
    }

