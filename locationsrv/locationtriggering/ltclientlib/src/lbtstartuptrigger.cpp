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
* Description: Implementation of class representing startup trigger
* entries.
*
*/


#include "lbtstartuptrigger.h"
#include "lbttriggerentry.h"
#include "lbttriggerconditionbase.h"
#include "lbttriggerconditionarea.h"
#include "lbterrors.h"

const TInt EnabledState=1;
const TInt DisabledState=2;
const TInt ConditionArea=1;



// ======== LOCAL FUNCTIONS ========

// ---------------------------------------------------------------------------
// CLbtStartupTrigger::NewL()
//
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//
EXPORT_C  CLbtStartupTrigger* CLbtStartupTrigger::NewL()
    {
  	CLbtStartupTrigger* self = new( ELeave ) CLbtStartupTrigger;
    CleanupStack::PushL( self );
    self->ConstructL();
	CleanupStack::Pop( self );    
    return self;
    }
    
    
// ---------------------------------------------------------------------------
// CLbtStartupTrigger::NewL()
//
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//
EXPORT_C  CLbtStartupTrigger* CLbtStartupTrigger::NewLC()
    {
  	CLbtStartupTrigger* self = new( ELeave ) CLbtStartupTrigger;
    CleanupStack::PushL( self );
    self->ConstructL();
	return self;
    }    
// ---------------------------------------------------------------------------
// CLbtStartupTrigger::NewL()
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//
EXPORT_C  CLbtStartupTrigger* CLbtStartupTrigger::NewL( 
            const TDesC& aName,
            TLbtTriggerState aState,
            CRequestor::TRequestorType aRequestorType,
            CRequestor::TRequestorFormat aRequestorFormat,
            const TDesC& aRequestorData,
            TUid aManagerUi,
            CLbtTriggerConditionBase* aCondition,
            const TDesC& aFileName, 
            const TSecureId& aSecureId ,
            const TDesC& aCommandLine)
    {
	CLbtStartupTrigger* self = new( ELeave ) CLbtStartupTrigger;
    CleanupStack::PushL( self );
    self->ConstructL(aName,aState,aRequestorType,aRequestorFormat,aRequestorData
    				,aManagerUi,aCondition,aFileName,aSecureId,aCommandLine);
	CleanupStack::Pop( self );    
    return self;
    }

// ---------------------------------------------------------------------------
// CLbtStartupTrigger::NewL()
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//
EXPORT_C  CLbtStartupTrigger* CLbtStartupTrigger::NewL( 
            const TDesC& aName,
            CLbtTriggerEntry::TLbtTriggerState aState,
            const RRequestorStack& aRequestors,
            TUid aManagerUi,
            CLbtTriggerConditionBase* aCondition,
            const TDesC& aFileName, 
            const TSecureId& aSecureId ,
            const TDesC& aCommandLine )
    {
 	CLbtStartupTrigger* self = new( ELeave ) CLbtStartupTrigger;
 	CleanupStack::PushL( self );
 	self->ConstructL(aName,aState,aRequestors,
    				aManagerUi,aCondition,aFileName,aSecureId,aCommandLine);
 	CleanupStack::Pop( self );    
 	return self;
    }
 
// ---------------------------------------------------------------------------
// CLbtTriggerEntry::TType CLbtStartupTrigger::Type()
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//
EXPORT_C CLbtTriggerEntry::TType CLbtStartupTrigger::Type() const
    {
	return CLbtTriggerEntry::ETypeStartup;
    }

// ---------------------------------------------------------------------------
// CLbtStartupTrigger::GetProcessId( 
//            TDes& aFileName, 
//            TUidType& aUidType )
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//

EXPORT_C void CLbtStartupTrigger::GetProcessId( 
            TDes& aFileName, 
            TSecureId& aSecureId ) const
    {
	aFileName = iProcessFileName;
	aSecureId=iSecureId;
    }            

// ---------------------------------------------------------------------------
// CLbtStartupTrigger::SetProcessId( 
//            TDes& aFileName, 
//            TUidType& aUidType )
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//
EXPORT_C void CLbtStartupTrigger::SetProcessId( 
            const TDesC& aFileName, 
            const TSecureId& aSecureId)
    {	
    if( aFileName.Length() > KMaxFileName )
        User::Panic(KLbtClientPanicCategory,ELbtErrArgument);
    
	iProcessFileName.Copy(aFileName);
	iSecureId=aSecureId;
    }

// ---------------------------------------------------------------------------
// CLbtStartupTrigger::SetCommandLineL( 
//            const TDesC& aCommandLine )
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//

 EXPORT_C void CLbtStartupTrigger::SetCommandLineL( const TDesC& aCommandLine )
    {
	if(aCommandLine.Length()>KLbtMaxNameLength)
		User::Leave(KErrArgument);
	
	else if( aCommandLine.Length() )
    	{
    	if( iCommandLine )
    	    {
    	    delete iCommandLine;
    	    iCommandLine = NULL;
    	    }
    	iCommandLine=HBufC::NewL(aCommandLine.Length());
    	TPtr commLine=iCommandLine->Des();
    	commLine.Copy(aCommandLine);	
    	}
 	}
// ---------------------------------------------------------------------------
// CLbtStartupTrigger::CommandLine()
// (other items were commented in a header).
// ---------------------------------------------------------------------------
// 
EXPORT_C const TDesC& CLbtStartupTrigger::CommandLine() const
    {
	if(!iCommandLine)
	    {
	    return KNullDesC;
	    }
	else
	    {
	    return *iCommandLine;    
	    }    
 	}

// -----------------------------------------------------------------------------
// CLbtStartupTrigger::ConstructL
// Symbian 2nd phase constructor can leave.
// -----------------------------------------------------------------------------
// 
void CLbtStartupTrigger::ConstructL(const TDesC& aName,
            TLbtTriggerState aState,
            CRequestor::TRequestorType aRequestorType,
            CRequestor::TRequestorFormat aRequestorFormat,
            const TDesC& aRequestorData,
            TUid aManagerUi,
            CLbtTriggerConditionBase* aCondition,
            const TDesC& aFileName, 
            const TSecureId& aSecureId ,
            const TDesC& aCommandLine)
    {
	SetNameL(aName);
	SetState(aState);
	SetManagerUi(aManagerUi);
	SetCondition(aCondition);
	SetProcessId(aFileName,aSecureId);
	SetCommandLineL(aCommandLine);
	SetRequestorL(aRequestorType,aRequestorFormat,aRequestorData);
    }
    
// -----------------------------------------------------------------------------
// CLbtStartupTrigger::ConstructL
// Symbian 2nd phase constructor can leave.
// -----------------------------------------------------------------------------
// 
void CLbtStartupTrigger::ConstructL(const TDesC& aName,
            CLbtTriggerEntry::TLbtTriggerState aState,
            const RRequestorStack& aRequestors,
            TUid aManagerUi,
            CLbtTriggerConditionBase* aCondition,
            const TDesC& aFileName, 
            const TSecureId& aSecureId ,
            const TDesC& aCommandLine)
    {
	SetNameL(aName);
	SetState(aState);
	SetManagerUi(aManagerUi);
	SetCondition(aCondition);
	SetProcessId(aFileName,aSecureId);
	SetCommandLineL(aCommandLine);
	SetRequestorsL(aRequestors);
    }

// ---------------------------------------------------------------------------
// CLbtStartupTrigger::CLbtStartupTrigger()
// Default constructor
// ---------------------------------------------------------------------------
// 
CLbtStartupTrigger::CLbtStartupTrigger()
    {
	
	}

// ---------------------------------------------------------------------------
// CLbtStartupTrigger::~CLbtStartupTrigger()
// Destructor
// ---------------------------------------------------------------------------
//
CLbtStartupTrigger::~CLbtStartupTrigger()
    {
    if( iCommandLine )
    	{
    	delete iCommandLine;
    	iCommandLine=NULL;
    	}		
    }

// ---------------------------------------------------------------------------
// CLbtStartupTrigger::DoInternalizeL()
// (other items were commented in a header).
// ---------------------------------------------------------------------------
// 
void CLbtStartupTrigger::DoInternalizeL(RReadStream& aStream)
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
	TInt filenameLen=aStream.ReadInt32L();
	RBuf fileName;	
	User::LeaveIfError(fileName.Create(filenameLen));
	fileName.CleanupClosePushL( );
	aStream.ReadL(fileName,filenameLen);
	TInt secureId=aStream.ReadInt32L();
	SetProcessId(fileName,secureId);
	CleanupStack::PopAndDestroy( ); //fileName
	TInt isCommandLine = aStream.ReadInt32L();
	if( isCommandLine )
	    {
	    TInt commandLine = aStream.ReadInt32L();
	    RBuf commandlineBuffer;	    
	    User::LeaveIfError(commandlineBuffer.Create( commandLine ));
	    commandlineBuffer.CleanupClosePushL();
	    aStream.ReadL(commandlineBuffer,commandLine);
	    SetCommandLineL( commandlineBuffer );
	    CleanupStack::PopAndDestroy( ); //commandlineBuffer
	    }
	TInt condType=aStream.ReadInt8L();
	if(condType==ConditionArea)
	    {
		CLbtTriggerConditionArea *area=CLbtTriggerConditionArea::NewL();
		area->InternalizeL(aStream);
		SetCondition(area);
	    }	
    }   

// ---------------------------------------------------------------------------
// CLbtStartupTrigger::DoExternalizeL()
// (other items were commented in a header).
// ---------------------------------------------------------------------------
// 
void CLbtStartupTrigger::DoExternalizeL(RWriteStream& aStream) const
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
	aStream.WriteInt32L(iProcessFileName.Length());
	aStream.WriteL(iProcessFileName,iProcessFileName.Length());
	aStream.WriteInt32L(iSecureId.iId);
	if( iCommandLine )
	    {
	    aStream.WriteInt32L(1);
	    TInt commandLine = iCommandLine->Length();
	    aStream.WriteInt32L(commandLine);
	    aStream.WriteL(*iCommandLine,commandLine);
	    }
	else
	    {
	    aStream.WriteInt32L(0);
	    }    
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
// CLbtStartupTrigger::ConstructL()
// Symbian 2nd phase constructor can leave.
// ---------------------------------------------------------------------------
//
void CLbtStartupTrigger::ConstructL()
    {
	SetState(CLbtTriggerEntry::EStateEnabled);
	SetId(KLbtNullTriggerId);
	// Set it to null descriptor by default
	SetCommandLineL(KNullDesC());
	SetCondition(NULL);
	SetNameL(KNullDesC());
    }


