/*
* Copyright (c) 2002-2005 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  Client-Server communication
*
*/


#include <e32std.h>
#include <s32strm.h>
#include <s32mem.h>

#include "lbt.h"
#include "lbtserver.h"
#include "lbttriggerinfo.h"
#include "lbterrors.h"
#include "lbtserverconsts.h"
#include "lbtsessiontrigger.h"
#include "lbtstartuptrigger.h"
#include "lbtclientrequester.h"
#include "lbtlisttriggeroptions.h"
#include "lbttriggerfilterbase.h"
#include "lbttriggerinfo.h"
#include "lbttriggerfilterbyattribute.h"
#include "lbttriggerconditionarea.h"
#include "lbtserverconsts.h"
#include "lbttriggeringsystemmanagementsettings.h"
#include "lbttriggerfilterbyarea.h"
#include "lbtgeocircle.h"
#include "lbtgeocell.h"
#include "lbtgeohybrid.h"
#include "lbtterminalptrholder.h"

// ---------------------------------------------------------------------------
// RLbt::RLbt()
//
// Default constructor.
// ---------------------------------------------------------------------------
// 
EXPORT_C RLbt::RLbt()
    { // Handling of these functions need to be discussed.
	TRAP_IGNORE(iPtrHolder = CLbtSubSessnPtrHolder::NewL(10, 10));
	TRAP_IGNORE(iClientRequester=CLbtClientRequester::NewL( *this ));
	TRAP_IGNORE(iTriggerCreationInfo=new( ELeave)TLbtTriggerCreationInfo);
	TRAP_IGNORE(iTriggerStateInfo = new( ELeave ) TLbtTriggerStateInfo);
	TRAP_IGNORE(iTriggerUpdationInfo = new( ELeave ) TLbtTriggerUpdationInfo);
	}

// ---------------------------------------------------------------------------
// RLbt::~RLbt
//
// Destructor.
// ---------------------------------------------------------------------------
// 	
EXPORT_C RLbt::~RLbt()
    {
    if(iPtrHolder)
        {
        delete iPtrHolder;
        iPtrHolder = NULL;
				}
    if(iClientRequester)
        {
        delete iClientRequester;
        iClientRequester = NULL;
        }
    if(iTriggerCreationInfo)    
    	{
    	delete iTriggerCreationInfo; 
     	iTriggerCreationInfo=NULL; 
    	} 
    if( iTriggerUpdationInfo )
        {
        delete iTriggerUpdationInfo;
        iTriggerUpdationInfo = NULL;
        }
    if( iTriggerStateInfo )
        {
        delete iTriggerStateInfo;
        iTriggerStateInfo = NULL;
        }
    iTriggerList.Close();	
    }


// ---------------------------------------------------------------------------
// RLbt::ConstructL
//Symbian 2nd phase constructor can leave.
// ---------------------------------------------------------------------------
//     
void RLbt::ConstructL()
    {
    iCreateIteratorFlag=EFalse;
    }


// ---------------------------------------------------------
// RLbt::Open
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
EXPORT_C TInt RLbt::Open( RLbtServer& aServer )
    {
    __ASSERT_ALWAYS(aServer.Handle() != 0,
		User::Panic(KLbtClientPanicCategory, ELbtServerBadHandle));
		
	__ASSERT_ALWAYS(! SubSessionHandle() , User::Panic  (KLbtClientPanicCategory, ELbtServerBadHandle));
	TRAPD(ret, ConstructL());
	if (ret == KErrNone)
		{
		ret = CreateSubSession(aServer, ELbtSubSessionOpen, TIpcArgs());
		}

	return ret;     
    }

// ---------------------------------------------------------
// RLbt::Open
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
EXPORT_C TInt RLbt::Open()
    {
    return 0;	
    }   

// ---------------------------------------------------------
// RLbt::Close
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
EXPORT_C void RLbt:: Close()
    {
    TBool isActive = iClientRequester-> IsActive();
    TInt status = iClientRequester->iStatus.Int();
	if(SubSessionHandle())
	    {
    	CloseSubSession(ELbtSubSessionClose);
	    }
	if( isActive && status == KRequestPending )
	    {
	    User::Panic(KLbtClientPanicCategory, ELbtRequestsNotCancelled );
	    }
    }

// ---------------------------------------------------------
// RLbt::CreateTrigger
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
EXPORT_C void RLbt:: CreateTrigger( 
            const CLbtTriggerEntry& aTrigger,
            TLbtTriggerId& aTriggerId,
            TBool aFireOnCreation,
            TRequestStatus& aStatus )
            
    {
    TRAPD( error, CreateTriggerL( aTrigger,aTriggerId,aFireOnCreation,aStatus ));
    if( KErrNone != error )
    	{
    	TRequestStatus* status = &aStatus;
    	User::RequestComplete(status,error);
    	}
    }

// ---------------------------------------------------------
// RLbt::CreateTriggerL
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void RLbt:: CreateTriggerL( 
            const CLbtTriggerEntry& aTrigger,
            TLbtTriggerId& aTriggerId,
            TBool aFireOnCreation,
            TRequestStatus& aStatus )
    {
	__ASSERT_ALWAYS(SubSessionHandle(), User::Panic(KLbtClientPanicCategory, ELbtServerBadHandle));
	
    aStatus=KRequestPending;
	TRequestStatus* status=&aStatus;
	if(iClientRequester->IsActive())
		User::Panic(KLbtClientPanicCategory, ELbtDuplicateRequest);
	
	if(aTrigger.GetCondition()==NULL || 
	   aTrigger.Name()==KNullDesC)
	    {
	    User::RequestComplete(status,KErrArgument);
	    return;
	    }
	
	RRequestorStack requestors;
	aTrigger.GetRequestorsL( requestors );
	TInt count = requestors.Count();
	requestors.ResetAndDestroy();
	
	if( !count )
	    {
	    User::Leave( KErrAccessDenied );
	    }
	
	if(aTrigger.Type()==CLbtTriggerEntry::ETypeStartup)
		{
		TBuf<256> fileName;
    	TSecureId secureId;
    	
    	const CLbtStartupTrigger* trigger=static_cast<const CLbtStartupTrigger*>
    							(&aTrigger);
    	trigger->GetProcessId(fileName,secureId);
    	if(fileName.Length()==0)
    		{
			User::RequestComplete(status,KErrArgument);
	    	return;    		
    		}
		}

    const CLbtTriggerConditionArea* cond =static_cast<const CLbtTriggerConditionArea*>( aTrigger.GetCondition()	);
    
     CLbtGeoAreaBase* area = cond->TriggerArea();
	if(!area)
	    {
	    if(aStatus.Int()== KRequestPending)
	        {
	        User::RequestComplete(status,KErrArgument);
	        }
	    return;    
	    }
	
	ValidateGeoAreaInformationL( area );
	
	if( area->Type() == CLbtGeoAreaBase::ECellular || area->Type() == CLbtGeoAreaBase::EHybrid )
        {        
        // In the current version of LBT, we dont support EXIT type cell/WLAN based triggers
        if( cond->Direction() == CLbtTriggerConditionArea::EFireOnExit )
            {
            User::RequestComplete( status,KErrNotSupported );
            return;
            }
        }
	
	TIpcArgs args;
	CBufFlat* buffer=NULL;
	RBufWriteStream writeStream;
	
	buffer = CBufFlat::NewL(512);
	CleanupStack::PushL(buffer);
	writeStream.Open(*buffer);
	CleanupClosePushL(writeStream);
	writeStream << aTrigger;
	writeStream.CommitL();
	CleanupStack::PopAndDestroy(&writeStream);
    
	TPtr8 ptr = buffer->Ptr(0);
	iPtrHolder->Ptr(0).Set(
		reinterpret_cast<TUint8*>(&aTriggerId),
		sizeof(aTriggerId),sizeof(aTriggerId));
	iPtrHolder->Ptr(1).Set(ptr);	
	
	iTriggerCreationInfo->iFireOnCreation=aFireOnCreation;
	iTriggerCreationInfo->iTriggerType=aTrigger.Type();
	
				
	iPtrHolder->Ptr(2).Set(reinterpret_cast<TUint8*>(iTriggerCreationInfo),
	sizeof(TLbtTriggerCreationInfo),sizeof(TLbtTriggerCreationInfo));	
	iClientRequester->SetStatus(aStatus);
	iClientRequester->SetBuffer(buffer);
	CleanupStack::Pop(buffer);
	
	args.Set(0,&iPtrHolder->Ptr(0));
	args.Set(1,&iPtrHolder->Ptr(1));
	args.Set(2,&iPtrHolder->Ptr(2));

	
	SendReceive(ELbtCreateTrigger, args, iClientRequester->iStatus );
	iClientRequester->Start();
	}

// ---------------------------------------------------------
// RLbt::CancelCreateTrigger
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
EXPORT_C void RLbt::CancelCreateTrigger()
    {
	__ASSERT_ALWAYS(SubSessionHandle(), User::Panic(KLbtClientPanicCategory, ELbtServerBadHandle));
	
	TInt error = SendReceive(ELbtCancelCreateTrigger);
	if( error == KErrNone )
	    {
	    iClientRequester->Cancel();
	    }
    }

// ---------------------------------------------------------
// RLbt::DeleteTriggerL
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
 EXPORT_C void RLbt::DeleteTriggerL( TLbtTriggerId aId )
    {
    __ASSERT_ALWAYS(SubSessionHandle(), User::Panic(KLbtClientPanicCategory, ELbtServerBadHandle));
		
	TIpcArgs args;
	TPckg<TLbtTriggerId> triggerId(aId);	
	args.Set(0, &triggerId);
	
	User::LeaveIfError(SendReceive(ELbtDeleteTrigger, args));
    }

// ---------------------------------------------------------
// RLbt::DeleteTriggerL
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
 EXPORT_C void RLbt::DeleteTriggersL( CLbtTriggerFilterBase* aFilter )
    {
	__ASSERT_ALWAYS(SubSessionHandle(), User::Panic(KLbtClientPanicCategory, ELbtServerBadHandle));
		
	TIpcArgs args;
	CBufFlat* buffer = CBufFlat::NewL(512);	
	CleanupStack::PushL(buffer);
	RBufWriteStream writeStream;
	writeStream.Open(*buffer);
	CleanupClosePushL(writeStream);
	CLbtTriggerFilterBase::TFilterType type;
	
	if(aFilter == NULL)
	    {
	    CLbtTriggerFilterByAttribute* filter = CLbtTriggerFilterByAttribute::NewL();
	    CleanupStack::PushL(filter);
		filter->AddTriggerStateL(CLbtTriggerEntry::EStateEnabled);
        filter->AddTriggerStateL(CLbtTriggerEntry::EStateDisabled);
        writeStream<< *filter;
        type=filter->Type();
        CleanupStack::PopAndDestroy(filter);
	    }
	else
        {   
        if( aFilter->Type() == CLbtTriggerFilterBase::EFilterByArea )
            {
            CLbtTriggerFilterByArea* areaFilter = static_cast<CLbtTriggerFilterByArea*> ( aFilter );
            ValidateGeoAreaInformationL( areaFilter->Area() );
            }
    	writeStream << *aFilter;
    	type=aFilter->Type();
        }	 
	
	writeStream.CommitL();
	TPtr8 ptr = buffer->Ptr(0);

	args.Set(KParamFilterType,type);
	args.Set(KParamFilter,&ptr);
		
	User::LeaveIfError( SendReceive(ELbtDeleteTriggers, args) );
    CleanupStack::PopAndDestroy(&writeStream);
    CleanupStack::PopAndDestroy(buffer);	
    }
    
// ---------------------------------------------------------
// RLbt::DeleteTriggers
//
// (other items were commented in a header).
// ---------------------------------------------------------
//

EXPORT_C void RLbt::DeleteTriggers( TRequestStatus& aStatus,
                        CLbtTriggerFilterBase* aFilter )
    {
    TRAPD( error,DeleteTriggersL( aFilter,aStatus ) );
    if( KErrNone != error )
    	{
    	TRequestStatus* status=&aStatus;
    	User::RequestComplete(status,error);
    	}
    }

// ---------------------------------------------------------
// RLbt::DeleteTriggersL
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void RLbt::DeleteTriggersL( 
            CLbtTriggerFilterBase* aFilter,
            TRequestStatus& aStatus )
    {
    __ASSERT_ALWAYS(SubSessionHandle(), User::Panic(KLbtClientPanicCategory, ELbtServerBadHandle));
	
	aStatus = KRequestPending;

	if(iClientRequester->IsActive())
		User::Panic(KLbtClientPanicCategory, ELbtDuplicateRequest);
	
	TIpcArgs args;
	
	CBufFlat* buffer=NULL;
	RBufWriteStream writeStream;

        buffer = CBufFlat::NewL(512);
	    CleanupStack::PushL(buffer);
	    writeStream.Open(*buffer);
	    CleanupClosePushL(writeStream);
	    
	    CLbtTriggerFilterBase::TFilterType type;
	if(aFilter == NULL)
        {
        CLbtTriggerFilterByAttribute* filter = CLbtTriggerFilterByAttribute::NewL();
        CleanupStack::PushL(filter);
    	filter->AddTriggerStateL(CLbtTriggerEntry::EStateEnabled);
        filter->AddTriggerStateL(CLbtTriggerEntry::EStateDisabled);
        writeStream<< *filter;
        type=filter->Type();
        CleanupStack::PopAndDestroy(filter);
        }
	else
        {  
        if( aFilter->Type() == CLbtTriggerFilterBase::EFilterByArea )
            {
            CLbtTriggerFilterByArea* areaFilter = static_cast<CLbtTriggerFilterByArea*> ( aFilter );
            ValidateGeoAreaInformationL( areaFilter->Area() );
            }
    	writeStream << *aFilter;
    	type=aFilter->Type();
        }	 
	
	  
	writeStream.CommitL();
	CleanupStack::PopAndDestroy(&writeStream);
    
    TPtr8 ptr = buffer->Ptr(0);
	iPtrHolder->Ptr(0).Set(ptr);
	
	args.Set(KParamFilterType,type);
	args.Set(KParamFilter,&iPtrHolder->Ptr(0));
	
	iClientRequester->SetStatus(aStatus);
	iClientRequester->SetBuffer(buffer);
	CleanupStack::Pop(buffer);
	SendReceive(ELbtDeleteTriggers, args,iClientRequester->iStatus);
	iClientRequester->Start();
    }

// ---------------------------------------------------------
// RLbt::DeleteTriggersL
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
EXPORT_C void RLbt::DeleteTriggersL( 
            const RArray<TLbtTriggerId> &aTriggerIdList )
            
    {
	__ASSERT_ALWAYS(SubSessionHandle(), User::Panic(KLbtClientPanicCategory, ELbtServerBadHandle));
		
	if(aTriggerIdList.Count()<1)
	    {
	    return;
	    }
	TIpcArgs args;
	TInt i=0;
	CBufFlat* buffer = CBufFlat::NewL(512);	
	CleanupStack::PushL(buffer);
	RBufWriteStream writeStream;
	writeStream.Open(*buffer);
	CleanupClosePushL(writeStream);
	TInt listCount=aTriggerIdList.Count();
	CLbtTriggerFilterByAttribute* filter = CLbtTriggerFilterByAttribute::NewL();
	CleanupStack::PushL(filter);

	while(i<listCount)
	    {
        filter->AddTriggerIdL(aTriggerIdList[i]);
        i++;
	    }
	writeStream<<*filter;
	writeStream.CommitL();

	TPtr8 ptr = buffer->Ptr(0);

	args.Set(KParamFilterType,filter->Type());
	
	CleanupStack::PopAndDestroy(filter);
	CleanupStack::PopAndDestroy(&writeStream);
	args.Set(KParamFilter,&ptr);
	CleanupStack::Pop(buffer);
	User::LeaveIfError(SendReceive(ELbtDeleteTriggers,args));	
	delete buffer;
    }

// ---------------------------------------------------------
// RLbt::DeleteTriggers
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
EXPORT_C void RLbt::DeleteTriggers( 
            const RArray<TLbtTriggerId>& aTriggerIdList,
            TRequestStatus& aStatus )
    {
    TRAPD( error,DeleteTriggersL( aTriggerIdList,aStatus ));
    if( KErrNone != error )
    	{
    	TRequestStatus* status=&aStatus;
    	User::RequestComplete( status,error );
    	}
    }

// ---------------------------------------------------------
// RLbt::DeleteTriggersL
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void RLbt::DeleteTriggersL( 
            const RArray<TLbtTriggerId>& aTriggerIdList,
            TRequestStatus& aStatus )
     {
	__ASSERT_ALWAYS(SubSessionHandle(), User::Panic(KLbtClientPanicCategory, ELbtServerBadHandle));
	
	aStatus = KRequestPending;
	TRequestStatus* status=&aStatus;
	if(iClientRequester->IsActive())
		User::Panic(KLbtClientPanicCategory, ELbtDuplicateRequest);

    if(aTriggerIdList.Count()<1)
	    {
	    User::RequestComplete(status,KErrNone);
	    return;
	    }
	TIpcArgs args;
	TInt i=0;

	
	CBufFlat* buffer=NULL;
	RBufWriteStream writeStream;

	 buffer = CBufFlat::NewL(512);
    CleanupStack::PushL(buffer);
    writeStream.Open(*buffer);
    CleanupClosePushL(writeStream);

	
    TInt listCount=aTriggerIdList.Count();
	CLbtTriggerFilterByAttribute* filter = CLbtTriggerFilterByAttribute::NewL();
	CleanupStack::PushL(filter);

	while(i<listCount)
	    {
        filter->AddTriggerIdL(aTriggerIdList[i]);
		i++;
	    }

	writeStream<<*filter;
	writeStream.CommitL();

	
	TPtr8 ptr = buffer->Ptr(0);
	iClientRequester->SetStatus(aStatus);
	iClientRequester->SetBuffer(buffer);

		

	iPtrHolder->Ptr(0).Set(ptr);
	
	args.Set(KParamFilterType,filter->Type());
	
	CleanupStack::PopAndDestroy(filter);
		
    CleanupStack::PopAndDestroy(&writeStream);
	
	CleanupStack::Pop(buffer);
	
	args.Set(KParamFilter,&iPtrHolder->Ptr(0));

	
	SendReceive(ELbtDeleteTriggers,args,iClientRequester->iStatus);
	
	iClientRequester->Start();
    }

// ---------------------------------------------------------
// RLbt::CancelDeleteTriggers
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
EXPORT_C void RLbt::CancelDeleteTriggers()
    {
	__ASSERT_ALWAYS(SubSessionHandle(), User::Panic(KLbtClientPanicCategory, ELbtServerBadHandle));
	TInt error = SendReceive(ELbtCancelDeleteTriggers);
    if( error == KErrNone )
        {
        iClientRequester->Cancel();
        }
	}



// ---------------------------------------------------------
// RLbt::GetTriggerLC
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
EXPORT_C CLbtTriggerInfo* RLbt::GetTriggerLC( TLbtTriggerId aId,TLbtTriggerAttributeFieldsMask aEntryFieldMask ,TLbtTriggerDynamicInfoFieldsMask  aDynInfoFieldMask )
                
    {
    __ASSERT_ALWAYS(SubSessionHandle(), User::Panic(KLbtClientPanicCategory, ELbtServerBadHandle));

	     
    CLbtListTriggerOptions* listOptions = CLbtListTriggerOptions::NewL();
    CleanupStack::PushL(listOptions);
    CLbtTriggerFilterByAttribute* attribFilter = CLbtTriggerFilterByAttribute::NewL();
    CleanupStack::PushL(attribFilter);
    attribFilter->AddTriggerIdL(aId);
    
    /* Add attribute Id in the fields to be retirieved. Specified in API*/
    TLbtTriggerAttributeFieldsMask entryMask = aEntryFieldMask|CLbtTriggerEntry::EAttributeId;
    listOptions->SetRetrievedFields(entryMask,aDynInfoFieldMask);
    listOptions->SetFilter(attribFilter);
    CBufFlat* buffer = CBufFlat::NewL(512);
    CleanupStack::PushL(buffer);
    TInt bufLength;
    GetTriggersInServerL(buffer,listOptions,bufLength);
    RBufReadStream stream(*buffer);
    CleanupClosePushL(stream);
    /* Neglect the number of triggers in this case, since it has to be one */
    TInt numOfTriggers = stream.ReadInt32L();

    CLbtTriggerInfo* triggerInfo = NULL;
    if(bufLength>0)
        {
        triggerInfo = CLbtTriggerInfo::NewL();
    	CleanupStack::PushL(triggerInfo);
    	triggerInfo->InternalizeL(stream);
        CleanupStack::Pop(triggerInfo);
        }

    CleanupStack::PopAndDestroy(4); // buffer, stream, 	filter,listoptions
    if(triggerInfo)
        {
        CleanupStack::PushL(triggerInfo); 
        }
    else
        {
        User::Leave(KErrNotFound);
        }    
    return triggerInfo;    
    }

// ---------------------------------------------------------
// RLbt::UpdateTriggerL
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
EXPORT_C void RLbt::UpdateTriggerL( const CLbtTriggerEntry& aTrigger,TLbtTriggerAttributeFieldsMask aFieldMask,
                                    TLbtFireOnUpdate aFireOnUpdate )
    {
    __ASSERT_ALWAYS(SubSessionHandle(), User::Panic(KLbtClientPanicCategory, ELbtServerBadHandle));

    if(((aFieldMask & CLbtTriggerEntry::EAttributeId) == CLbtTriggerEntry::EAttributeId)||((aFieldMask & CLbtTriggerEntry::EAttributeManagerUi) == CLbtTriggerEntry::EAttributeManagerUi)||((aFieldMask & CLbtTriggerEntry::EAttributeRequestor) == CLbtTriggerEntry::EAttributeRequestor))
        {
        User::Leave(KErrAccessDenied);
        }


    if(((aFieldMask & CLbtTriggerEntry::EAttributeCondition) == CLbtTriggerEntry::EAttributeCondition))
        {
        if(aTrigger.GetCondition()==NULL)
            {
            User::Leave(KErrArgument);
            }
            
        const CLbtTriggerConditionArea* area = static_cast< const CLbtTriggerConditionArea*> (aTrigger.GetCondition());    
        if(area->TriggerArea() == NULL)
            {
            User::Leave(KErrArgument);
            }
        }
    
    if(((aFieldMask & CLbtTriggerEntry::EAttributeName) == CLbtTriggerEntry::EAttributeName))
        {
        if(aTrigger.Name()==KNullDesC || (aTrigger.Name()).Length()>KLbtMaxNameLength)
            {
            User::Leave(KErrArgument);
            }
        } 
    
    if((aFieldMask & CLbtTriggerEntry::EAttributeStartUpProcessId) == CLbtTriggerEntry::EAttributeStartUpProcessId)    
        {
        User::Leave(KErrAccessDenied);
        }
           
    if((aFieldMask & CLbtTriggerEntry::EAttributeStartUpCommandLine) == CLbtTriggerEntry::EAttributeStartUpCommandLine)    
        {
        const CLbtStartupTrigger* trigger=static_cast<const CLbtStartupTrigger*>
    							(&aTrigger);
    	TPtrC commandLinePtr=trigger->CommandLine();
    	if(commandLinePtr.Length()==0)
    		{
    		User::Leave(KErrArgument);
        	}
        }
           
	TIpcArgs args;
	CBufFlat* buffer = CBufFlat::NewL(512);	
	CleanupStack::PushL(buffer);
	RBufWriteStream writeStream;
	writeStream.Open(*buffer);
	CleanupClosePushL(writeStream);
	writeStream << aTrigger;
	writeStream.CommitL();
	CleanupStack::PopAndDestroy(&writeStream);
	TPtr8 ptr1 = buffer->Ptr(0);

	CleanupStack::Pop(buffer);
	
	TLbtTriggerUpdationInfo triggerUpdationInfo;
	triggerUpdationInfo.iFireOnUpdate = aFireOnUpdate;
	triggerUpdationInfo.iTriggerType = aTrigger.Type();	
	
	TPckg<TLbtTriggerUpdationInfo> pckg(triggerUpdationInfo);
	args.Set( KParamTriggerEntry,&ptr1 );
	args.Set( KParamMask, aFieldMask);
	args.Set( KParamTriggerUpdationInfo,&pckg);
	User::LeaveIfError(SendReceive(ELbtUpdateTrigger, args));

	delete buffer;
    }


// ---------------------------------------------------------
// RLbt::UpdateTrigger
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
EXPORT_C void RLbt::UpdateTrigger( 
            const CLbtTriggerEntry& aTrigger,
            TLbtTriggerAttributeFieldsMask aFieldMask,
            TLbtFireOnUpdate aFireOnUpdate,
            TRequestStatus& aStatus )
    {
    TRAPD( error,UpdateTriggerL( aTrigger,aFieldMask,aFireOnUpdate,aStatus ));
    if( KErrNone != error )
        {
        TRequestStatus* status=&aStatus;
        User::RequestComplete( status,error );
        }
    }

// ---------------------------------------------------------
// RLbt::UpdateTriggerL
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void RLbt::UpdateTriggerL( 
            const CLbtTriggerEntry& aTrigger,
            TLbtTriggerAttributeFieldsMask aFieldMask,
            TLbtFireOnUpdate aFireOnUpdate,
            TRequestStatus& aStatus )
    {
    __ASSERT_ALWAYS(SubSessionHandle(), User::Panic(KLbtClientPanicCategory, ELbtServerBadHandle));

    if(((aFieldMask & CLbtTriggerEntry::EAttributeId) == CLbtTriggerEntry::EAttributeId)||((aFieldMask & CLbtTriggerEntry::EAttributeManagerUi) == CLbtTriggerEntry::EAttributeManagerUi)||((aFieldMask & CLbtTriggerEntry::EAttributeRequestor) == CLbtTriggerEntry::EAttributeRequestor))
        {
        User::Leave(KErrAccessDenied);
        }


    if(((aFieldMask & CLbtTriggerEntry::EAttributeCondition) == CLbtTriggerEntry::EAttributeCondition))
        {
        if(aTrigger.GetCondition()==NULL)
            {
            User::Leave(KErrArgument);
            }
            
        const CLbtTriggerConditionArea* area = static_cast< const CLbtTriggerConditionArea*> (aTrigger.GetCondition());    
        if(area->TriggerArea() == NULL)
            {
            User::Leave(KErrArgument);
            }
        }
    
    if(((aFieldMask & CLbtTriggerEntry::EAttributeName) == CLbtTriggerEntry::EAttributeName))
        {
        if(aTrigger.Name()==KNullDesC || (aTrigger.Name()).Length()>KLbtMaxNameLength)
            {
            User::Leave(KErrArgument);
            }
        } 
    
    if((aFieldMask & CLbtTriggerEntry::EAttributeStartUpProcessId) == CLbtTriggerEntry::EAttributeStartUpProcessId)    
        {
        User::Leave(KErrAccessDenied);
        }
           
    if((aFieldMask & CLbtTriggerEntry::EAttributeStartUpCommandLine) == CLbtTriggerEntry::EAttributeStartUpCommandLine)    
        {
        const CLbtStartupTrigger* trigger=static_cast<const CLbtStartupTrigger*>
                                (&aTrigger);
        TPtrC commandLinePtr=trigger->CommandLine();
        if(commandLinePtr.Length()==0)
            {
            User::Leave(KErrArgument);
            }
        }
           
    TIpcArgs args;
    CBufFlat* buffer = CBufFlat::NewL(512); 
    CleanupStack::PushL(buffer);
    RBufWriteStream writeStream;
    writeStream.Open(*buffer);
    CleanupClosePushL(writeStream);
    writeStream << aTrigger;
    writeStream.CommitL();
    CleanupStack::PopAndDestroy(&writeStream);
    TPtr8 ptr = buffer->Ptr(0);

    iPtrHolder->Ptr(0).Set(ptr);
        
    iClientRequester->SetStatus(aStatus);
    iClientRequester->SetBuffer(buffer);
    
    CleanupStack::Pop(buffer);
    
    iTriggerUpdationInfo->iFireOnUpdate = aFireOnUpdate;
    iTriggerUpdationInfo->iTriggerType = aTrigger.Type();
    iPtrHolder->Ptr(1).Set(reinterpret_cast<TUint8*>(iTriggerUpdationInfo),
                           sizeof(TLbtTriggerUpdationInfo),sizeof(TLbtTriggerUpdationInfo));    
    args.Set( KParamTriggerEntry,&iPtrHolder->Ptr(0) );
    args.Set( KParamMask, aFieldMask);
    args.Set( KParamTriggerUpdationInfo,&iPtrHolder->Ptr(1));
    SendReceive(ELbtUpdateTrigger, args,iClientRequester->iStatus);
    iClientRequester->Start();
    }

// ---------------------------------------------------------
// RLbt::CancelUpdateTrigger
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
EXPORT_C void RLbt::CancelUpdateTrigger()
    {
    __ASSERT_ALWAYS(SubSessionHandle(), User::Panic(KLbtClientPanicCategory, ELbtServerBadHandle));
        
    TInt error = SendReceive(ELbtCancelUpdateTrigger);    
    if( error == KErrNone )
        {
        iClientRequester->Cancel();
        }
    }

// ---------------------------------------------------------
// RLbt::SetTriggerStateL
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
EXPORT_C void RLbt::SetTriggerStateL( 
            TLbtTriggerId aId, 
            CLbtTriggerEntry::TLbtTriggerState aState,
            TLbtFireOnUpdate aFireOnUpdate )  
    {
    __ASSERT_ALWAYS(SubSessionHandle(), User::Panic(KLbtClientPanicCategory, ELbtServerBadHandle));

	TIpcArgs args;
	
	TPckg<TLbtTriggerId> triggerId(aId);
	TPckg<CLbtTriggerEntry::TLbtTriggerState> triggerState(aState);
	TPckg<TLbtFireOnUpdate> fireOnUpdate(aFireOnUpdate);
	
	args.Set(0,&triggerId);
	args.Set(1,&triggerState);
	args.Set(2,&fireOnUpdate);
	User::LeaveIfError(SendReceive(ELbtSetTriggerState, args));
    }

// ---------------------------------------------------------
// RLbt::SetTriggersStateL
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
EXPORT_C void RLbt::SetTriggersStateL( CLbtTriggerEntry::TLbtTriggerState aState,
                                       TLbtFireOnUpdate aFireOnUpdate,CLbtTriggerFilterBase* aFilter )
    {
    __ASSERT_ALWAYS(SubSessionHandle(), User::Panic(KLbtClientPanicCategory, ELbtServerBadHandle));

	TIpcArgs args;
	CBufFlat* buffer = CBufFlat::NewL(512);	
	CleanupStack::PushL(buffer);
	RBufWriteStream writeStream;
	writeStream.Open(*buffer);
	CleanupClosePushL(writeStream);
	CLbtTriggerFilterBase::TFilterType type;
	if(aFilter == NULL)
	    {
	    CLbtTriggerFilterByAttribute* filter = CLbtTriggerFilterByAttribute::NewL();
	    CleanupStack::PushL(filter);
		filter->AddTriggerStateL(CLbtTriggerEntry::EStateEnabled);
        filter->AddTriggerStateL(CLbtTriggerEntry::EStateDisabled);
        writeStream<< *filter;
        type = filter->Type();
        CleanupStack::PopAndDestroy(filter);
	    }
	else
        {
        if( aFilter->Type() == CLbtTriggerFilterBase::EFilterByArea )
            {
            CLbtTriggerFilterByArea* areaFilter = static_cast<CLbtTriggerFilterByArea*> ( aFilter );
            ValidateGeoAreaInformationL( areaFilter->Area() );      
            }
        type = aFilter->Type();
        writeStream << *aFilter;
        }	    
	writeStream.CommitL();
	CleanupStack::PopAndDestroy(&writeStream);
	TPtr8 ptr = buffer->Ptr(0);
   
	
	CleanupStack::Pop(buffer);
	
	TLbtTriggerStateInfo triggerStateInfo;
	triggerStateInfo.iFireOnUpdate = aFireOnUpdate;
	triggerStateInfo.iState = aState;
	
	TPckg<TLbtTriggerStateInfo> pckg(triggerStateInfo);	
	args.Set(KParamFilterType, type);
	args.Set(KParamFilter,&ptr);
	args.Set(KParamTriggerStateInfo,&pckg);
	
	User::LeaveIfError(SendReceive(ELbtSetTriggersState, args));
	delete buffer;
    }

// ---------------------------------------------------------
// RLbt::SetTriggersState
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
EXPORT_C void RLbt::SetTriggersState( 
            TRequestStatus& aStatus,
            CLbtTriggerEntry::TLbtTriggerState aState,
            TLbtFireOnUpdate aFireOnUpdate,    
            CLbtTriggerFilterBase* aFilter)

    {
    TRAPD( error,SetTriggersStateL( aState,aFilter,aFireOnUpdate,aStatus ) );
    if( KErrNone != error )
    	{
    	TRequestStatus* status=&aStatus;
    	User::RequestComplete( status,error );
    	}
    }
// ---------------------------------------------------------
// RLbt::SetTriggersStateL
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void RLbt::SetTriggersStateL( 
            CLbtTriggerEntry::TLbtTriggerState aState,
            CLbtTriggerFilterBase* aFilter,
            TLbtFireOnUpdate aFireOnUpdate,
            TRequestStatus& aStatus )
            
    {
    __ASSERT_ALWAYS(SubSessionHandle(), User::Panic(KLbtClientPanicCategory, ELbtServerBadHandle));
    aStatus = KRequestPending;
    
	if(iClientRequester->IsActive())
		User::Panic(KLbtClientPanicCategory, ELbtDuplicateRequest);
	TIpcArgs args;

    iState = aState;
	
	CBufFlat* buffer=NULL;
	RBufWriteStream writeStream;

    CLbtTriggerFilterBase::TFilterType type;
    buffer = CBufFlat::NewL(512);
    CleanupStack::PushL(buffer);
    writeStream.Open(*buffer);
    CleanupClosePushL(writeStream);
    if(aFilter == NULL)
	    {
	    CLbtTriggerFilterByAttribute* filter = CLbtTriggerFilterByAttribute::NewL();
	    CleanupStack::PushL(filter);
		filter->AddTriggerStateL(CLbtTriggerEntry::EStateEnabled);
        filter->AddTriggerStateL(CLbtTriggerEntry::EStateDisabled);
        writeStream<< *filter;
        type = filter->Type();
        CleanupStack::PopAndDestroy(filter);
	    }
	    
    else
	    {
	    if( aFilter->Type() == CLbtTriggerFilterBase::EFilterByArea )
            {
            CLbtTriggerFilterByArea* areaFilter = static_cast<CLbtTriggerFilterByArea*> ( aFilter );
            ValidateGeoAreaInformationL( areaFilter->Area() );
            }
	    writeStream << *aFilter;
	    type=aFilter->Type();
	    }
	writeStream.CommitL();
	 
	CleanupStack::PopAndDestroy(&writeStream);
	TPtr8 ptr = buffer->Ptr(0);

	iPtrHolder->Ptr(0).Set(ptr);
		
	iClientRequester->SetStatus(aStatus);
	iClientRequester->SetBuffer(buffer);
	CleanupStack::Pop(buffer);
	
	iTriggerStateInfo->iFireOnUpdate = aFireOnUpdate;
	iTriggerStateInfo->iState = aState;
    
    iPtrHolder->Ptr(1).Set(
    reinterpret_cast<TUint8*>(iTriggerStateInfo),
    sizeof(TLbtTriggerStateInfo),sizeof(TLbtTriggerStateInfo));
	
		
	args.Set(KParamFilterType, type);
	args.Set(KParamFilter,&iPtrHolder->Ptr(0));
	args.Set(KParamTriggerStateInfo,&iPtrHolder->Ptr(1));
	
	SendReceive(ELbtSetTriggersState, args,iClientRequester->iStatus);
	iClientRequester->Start();
	}

// ---------------------------------------------------------
// RLbt::CancelSetTriggersState
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
EXPORT_C void RLbt::CancelSetTriggersState()
    {
	__ASSERT_ALWAYS(SubSessionHandle(), User::Panic(KLbtClientPanicCategory, ELbtServerBadHandle));
	
	TInt error = SendReceive(ELbtCancelSetTriggersState);	
    if( error == KErrNone )
        {
        iClientRequester->Cancel();
        }	
    }

// ---------------------------------------------------------
// RLbt::ListTriggerIdsL
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
EXPORT_C void RLbt::ListTriggerIdsL( RArray < TLbtTriggerId >& aTriggerIdList,CLbtListTriggerOptions* aListOptions  )
    {
    __ASSERT_ALWAYS(SubSessionHandle(), User::Panic(KLbtClientPanicCategory, ELbtServerBadHandle));

		
	TIpcArgs args;
	TInt numOfTriggers;
	iPtrHolder->Ptr(0).Set(
	reinterpret_cast<TUint8*>(&numOfTriggers),
	sizeof(numOfTriggers),sizeof(numOfTriggers));
	args.Set(KTriggersSize,&iPtrHolder->Ptr(0));

	CBufFlat* buf = CBufFlat::NewL(512);	
	CleanupStack::PushL(buf);
	RBufWriteStream writeStream;
	writeStream.Open(*buf);
	CleanupClosePushL(writeStream);
    
    if(aListOptions)
        {
        CLbtTriggerFilterBase* filter = aListOptions->Filter();
        if( filter )
            {
            if( filter->Type() == CLbtTriggerFilterBase::EFilterByArea )
                {
                CLbtTriggerFilterByArea* areaFilter = static_cast<CLbtTriggerFilterByArea*> ( filter );
                ValidateGeoAreaInformationL( areaFilter->Area() );
                }
            }
        writeStream << *aListOptions;
        }
        
	if(!aListOptions)
		{
	    CLbtListTriggerOptions* listOptions = CLbtListTriggerOptions::NewL();
		CleanupStack::PushL(listOptions);
		CLbtTriggerFilterByAttribute* filter = CLbtTriggerFilterByAttribute::NewL();
		CleanupStack::PushL(filter);
		filter->AddTriggerStateL(CLbtTriggerEntry::EStateEnabled);
        filter->AddTriggerStateL(CLbtTriggerEntry::EStateDisabled);
        listOptions->SetFilter(filter);        
		writeStream << *listOptions;
		CleanupStack::PopAndDestroy(filter);
		CleanupStack::PopAndDestroy(listOptions);
		}
		
	writeStream.CommitL();
	CleanupStack::PopAndDestroy(&writeStream);
	TPtr8 ptr = buf->Ptr(0);
	args.Set(KParamFilter, &ptr);
	TInt error = SendReceive(ELbtGetListTriggerIdsSize,args);
	CleanupStack::PopAndDestroy(); //buf
	
	if( error != KErrNotFound )
	    {
	    User::LeaveIfError( error );
	    }
		
	if(numOfTriggers == 0)
		{
		// Zero triggers found for this client. So just return after resetting the array
		aTriggerIdList.Reset();
		return;
		}
	
	TInt sizeOfTriggerId = sizeof(TLbtTriggerId);
	
	CBufFlat* buffer = CBufFlat::NewL( 512 );
	CleanupStack::PushL(buffer);
	buffer->ResizeL(numOfTriggers * sizeOfTriggerId);
	TPtr8 bufPtr = buffer->Ptr(0);
	
	TIpcArgs ipcArgs;	
	ipcArgs.Set(KParamTriggerEntry, &bufPtr);
	User::LeaveIfError(SendReceive(ELbtListTriggerIds, ipcArgs));
	
	// Got response append the trigger info into the array
	RBufReadStream readStream;
	readStream.Open(*buffer);
	CleanupClosePushL(readStream);
	for(TInt i = 0 ;i<numOfTriggers; ++i)
		{
		TLbtTriggerId triggerId;
		TUint8* ptr = reinterpret_cast<TUint8*>(&triggerId);
		readStream.ReadL(ptr, sizeof(TLbtTriggerId));
		aTriggerIdList.Append(triggerId);
		}
	CleanupStack::PopAndDestroy(2);//buffer, readStream
	}

// ---------------------------------------------------------
// RLbt::ListTriggerIds
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
EXPORT_C void RLbt::ListTriggerIds( 
            TRequestStatus& aStatus,
            RArray < TLbtTriggerId >& aTriggerIdList,
            CLbtListTriggerOptions* aListOptions )

    {
    TRAPD( error,ListTriggerIdsL( aTriggerIdList,aListOptions,aStatus ) );
    if( KErrNone != error )
    	{
    	TRequestStatus* status=&aStatus;
    	User::RequestComplete( status,error );
    	}
    }

// ---------------------------------------------------------
// RLbt::ListTriggerIdsL
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
 void RLbt::ListTriggerIdsL( 
            RArray < TLbtTriggerId >& aTriggerIdList,
            CLbtListTriggerOptions* aListOptions,
            TRequestStatus& aStatus )
    {
    __ASSERT_ALWAYS(SubSessionHandle(), User::Panic(KLbtClientPanicCategory, ELbtServerBadHandle));
    
	if(iClientRequester->IsActive())
		User::Panic(KLbtClientPanicCategory, ELbtDuplicateRequest);
	
	aStatus = KRequestPending;
	aTriggerIdList.Reset();
	TIpcArgs args;
	TInt& numberOfTriggers = iClientRequester->ListTriggersSize();
	iPtrHolder->Ptr(0).Set(
		reinterpret_cast<TUint8*>(&numberOfTriggers),
		sizeof(numberOfTriggers),sizeof(numberOfTriggers));
	args.Set(KTriggersSize,&iPtrHolder->Ptr(0));
	
	
    CBufFlat* buf = CBufFlat::NewL(512);	
	CleanupStack::PushL(buf);
	RBufWriteStream writeStream;
	writeStream.Open(*buf);
	CleanupClosePushL(writeStream);
	
	if(!aListOptions)
		{
	    CLbtListTriggerOptions* listOptions = CLbtListTriggerOptions::NewL();
		CleanupStack::PushL(listOptions);
		CLbtTriggerFilterByAttribute* filter = CLbtTriggerFilterByAttribute::NewL();
		CleanupStack::PushL(filter);
		filter->AddTriggerStateL(CLbtTriggerEntry::EStateEnabled);
        filter->AddTriggerStateL(CLbtTriggerEntry::EStateDisabled);
        listOptions->SetFilter(filter);        
		writeStream << *listOptions;
		CleanupStack::PopAndDestroy(filter);
		CleanupStack::PopAndDestroy(listOptions);
		}
		
	else
	    {
	    CLbtTriggerFilterBase* filter = aListOptions->Filter();
        if( filter )
            {
            if( filter->Type() == CLbtTriggerFilterBase::EFilterByArea )
                {
                CLbtTriggerFilterByArea* areaFilter = static_cast<CLbtTriggerFilterByArea*> ( filter );
                ValidateGeoAreaInformationL( areaFilter->Area() );
                }
            }
	    writeStream << *aListOptions;    
	    }	
	
	writeStream.CommitL();
	CleanupStack::PopAndDestroy(&writeStream);
	CleanupStack::Pop(); //buf
	
	TPtr8 ptr = buf->Ptr(0);
	iPtrHolder->Ptr(1).Set(ptr);
	args.Set(KParamFilter,&iPtrHolder->Ptr(1));
	iClientRequester->SetListTriggerIdData(&aTriggerIdList);
	iClientRequester->SetBuffer(buf);
    iClientRequester->SetStatus(aStatus);
    iClientRequester->Start(CLbtClientRequester::EListTriggerIdsCount);
	SendReceive(ELbtGetListTriggerIdsSize,args,iClientRequester->iStatus);
	}

 // ---------------------------------------------------------
 // RLbt::HandleListTriggerIds
 //
 // (other items were commented in a header).
 // ---------------------------------------------------------
 //
 void RLbt::HandleListTriggerIdsL()
     {
     TInt numberOfTriggers = iClientRequester->ListTriggersSize();
     if( numberOfTriggers == 0 )
         {
         // Zero triggers found for this client. leave with KErrNotFound which will be 
         // handled in ClbtClientRequester
         User::Leave( KErrNotFound );
         }
     
     TInt sizeOfTriggerId = sizeof(TLbtTriggerId);
     CBufFlat* buffer = CBufFlat::NewL( 512 );
     CleanupStack::PushL(buffer);
     buffer->ResizeL(numberOfTriggers * sizeOfTriggerId);
     
     TPtr8 bufPtr = buffer->Ptr(0);
     TIpcArgs ipcArgs;
     
     ipcArgs.Set(KParamTriggerEntry ,&bufPtr);
     User::LeaveIfError(SendReceive(ELbtListTriggerIds, ipcArgs));
     
     RArray < TLbtTriggerId >* triggerIdArray = iClientRequester->ListTriggerIdData();
     RBufReadStream readStream(*buffer);
     CleanupClosePushL(readStream);
     for(TInt i = 0 ;i<numberOfTriggers; ++i)
         {
         TLbtTriggerId triggerId;
         TUint8* ptr = reinterpret_cast<TUint8*>(&triggerId);
         readStream.ReadL(ptr, sizeof(TLbtTriggerId));
         triggerIdArray->Append(triggerId);
         } 
     CleanupStack::PopAndDestroy(2);  // readStream,buffer
     }
 
 
// ---------------------------------------------------------
// RLbt::CancelListTriggerIds
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
EXPORT_C void RLbt::CancelListTriggerIds()
    {
	__ASSERT_ALWAYS(SubSessionHandle(), User::Panic(KLbtClientPanicCategory, ELbtServerBadHandle));
	
	TInt error = SendReceive(ELbtCancelListTriggerIds);	
    if( error == KErrNone )
        {
        iClientRequester->Cancel();
        }
    }

// ---------------------------------------------------------
// RLbt::GetTriggersInServerL
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void RLbt::GetTriggersInServerL(CBufFlat* aBuf,CLbtListTriggerOptions* aListOptions,TInt& aBufLength )
    {
    TIpcArgs args;
    TPtr8 ptr1( reinterpret_cast<TUint8*>(&aBufLength),
                sizeof(aBufLength));
	
	// Write stream to write the list options into a stream to send it to server
	
	RBufWriteStream writeStream;
	writeStream.Open(*aBuf);
	CleanupClosePushL(writeStream);
	
	if(aListOptions == NULL)
		{
		CLbtListTriggerOptions* listOptions = CLbtListTriggerOptions::NewL();
		CleanupStack::PushL(listOptions);
		CLbtTriggerFilterByAttribute* filter = CLbtTriggerFilterByAttribute::NewL();
		CleanupStack::PushL(filter);
		filter->AddTriggerStateL(CLbtTriggerEntry::EStateEnabled);
        filter->AddTriggerStateL(CLbtTriggerEntry::EStateDisabled);
        listOptions->SetFilter(filter);        
		writeStream << *listOptions;
		CleanupStack::PopAndDestroy(filter);
		CleanupStack::PopAndDestroy(listOptions);
		
		}
	else
		{
		CLbtTriggerFilterBase* filter = aListOptions->Filter();
        if( filter )
            {
            if( filter->Type() == CLbtTriggerFilterBase::EFilterByArea )
                {
                CLbtTriggerFilterByArea* areaFilter = static_cast<CLbtTriggerFilterByArea*> ( filter );
                ValidateGeoAreaInformationL( areaFilter->Area() );
                }
            }
		writeStream << *aListOptions;
		}
	writeStream.CommitL();
	CleanupStack::PopAndDestroy(&writeStream);
	TPtr8 ptr = aBuf->Ptr(0);
	
	args.Set(KTriggersSize,&ptr1);
	args.Set(KParamFilter,&ptr);
	TInt error = SendReceive(ELbtGetTriggersBufferSize,args);
	
    if(error == KErrNone && aBufLength> 0 )
    	{
    	aBuf->Reset();
    	aBuf->ResizeL(aBufLength);
    	
    	TPtr8 bufPtr = aBuf->Ptr(0);
    	TIpcArgs ipcArgs;
    	ipcArgs.Set(KParamTriggerEntry ,&bufPtr);
    	SendReceive(ELbtGetTriggers, ipcArgs);
    	}
	return;	
	}
	
// ---------------------------------------------------------
// RLbt::GetTriggersL
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
EXPORT_C void RLbt::GetTriggersL(  RPointerArray < CLbtTriggerInfo >& aTriggerList,CLbtListTriggerOptions* aListOptions  )            
    {
    __ASSERT_ALWAYS(SubSessionHandle(), User::Panic(KLbtClientPanicCategory, ELbtServerBadHandle));

	TIpcArgs args;
	TInt bufLength;
	TPtr8 ptr( reinterpret_cast<TUint8*>(&bufLength),
	            sizeof(bufLength));
		
	CBufFlat* buffer = CBufFlat::NewL(512);	
	CleanupStack::PushL(buffer);
	RBufWriteStream writeStream;
	writeStream.Open(*buffer);
	CleanupClosePushL(writeStream);
	if(aListOptions == NULL)
		{
		CLbtListTriggerOptions* listOptions = CLbtListTriggerOptions::NewL();
		CleanupStack::PushL(listOptions);
		CLbtTriggerFilterByAttribute* filter = CLbtTriggerFilterByAttribute::NewL();
		CleanupStack::PushL(filter);
		filter->AddTriggerStateL(CLbtTriggerEntry::EStateEnabled);
        filter->AddTriggerStateL(CLbtTriggerEntry::EStateDisabled);
        listOptions->SetFilter(filter);        
		writeStream << *listOptions;
		CleanupStack::PopAndDestroy(filter);
		CleanupStack::PopAndDestroy(listOptions);
		}
	else
		{
		CLbtTriggerFilterBase* filter = aListOptions->Filter();
        if( filter )
            {
            if( filter->Type() == CLbtTriggerFilterBase::EFilterByArea )
                {
                CLbtTriggerFilterByArea* areaFilter = static_cast<CLbtTriggerFilterByArea*> ( filter );
                ValidateGeoAreaInformationL( areaFilter->Area() );
                }
            }
		writeStream << *aListOptions;
		}
	writeStream.CommitL();
	CleanupStack::PopAndDestroy(&writeStream);
	TPtr8 ptr1 = buffer->Ptr(0);
	
	args.Set(KTriggersSize,&ptr);
	args.Set(KParamFilter,&ptr1);
	User::LeaveIfError(SendReceive(ELbtGetTriggersBufferSize,args));	
		
	if(bufLength>0)
    	{
    	buffer->Reset();
    	buffer->ResizeL(bufLength);
    	
    	TPtr8 bufPtr = buffer->Ptr(0);
    	TIpcArgs ipcArgs;
    	ipcArgs.Set(KParamTriggerEntry ,&bufPtr);
    	User::LeaveIfError(SendReceive(ELbtGetTriggers, ipcArgs));
    	
    	RBufReadStream stream(*buffer);
    	CleanupClosePushL(stream);
    	TInt numberOfTriggers = stream.ReadInt32L();
    	CleanupClosePushL(aTriggerList);
    	for(TInt i=0;i<numberOfTriggers; ++i)
    		{
    		CLbtTriggerInfo* triggerInfo = CLbtTriggerInfo::NewL();
    		CleanupStack::PushL(triggerInfo);
    		triggerInfo->InternalizeL(stream);
    		aTriggerList.Append(triggerInfo);		
    		CleanupStack::Pop(); // triggerInfo
    		}
    	CleanupStack::Pop(); //aTriggerList
    	CleanupStack::PopAndDestroy(); // stream
    	}
	CleanupStack::PopAndDestroy(); // buffer
	}

// ---------------------------------------------------------
// RLbt::GetTriggers
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
EXPORT_C void RLbt::GetTriggers( 
            TRequestStatus& aStatus,
            RPointerArray < CLbtTriggerInfo >& aTriggerList,
            CLbtListTriggerOptions* aListOptions )
    {
    TRAPD( error,GetTriggersL( aTriggerList,aListOptions,aStatus ) );
    if( KErrNone != error )
    	{
    	TRequestStatus* status=&aStatus;
    	User::RequestComplete( status,error );
    	}	
    }


// ---------------------------------------------------------
// RLbt::GetTriggersL
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void RLbt::GetTriggersL( 
            RPointerArray < CLbtTriggerInfo >& aTriggerList,
            CLbtListTriggerOptions* aListOptions,
            TRequestStatus& aStatus )
    {
    __ASSERT_ALWAYS(SubSessionHandle(), User::Panic(KLbtClientPanicCategory, ELbtServerBadHandle));
    
	if(iClientRequester->IsActive())
	User::Panic(KLbtClientPanicCategory, ELbtDuplicateRequest);
    
    aStatus = KRequestPending;

    TIpcArgs args;
    TInt& bufferSize = iClientRequester->ListTriggersSize();
	iPtrHolder->Ptr(0).Set(
	reinterpret_cast<TUint8*>(&bufferSize),
	sizeof(bufferSize),sizeof(bufferSize));
		
	CBufFlat* buffer = CBufFlat::NewL(512);	
	CleanupStack::PushL(buffer);
	RBufWriteStream writeStream;
	writeStream.Open(*buffer);
	CleanupClosePushL(writeStream);
	if(aListOptions == NULL)
		{
		CLbtListTriggerOptions* listOptions = CLbtListTriggerOptions::NewL();
		CleanupStack::PushL(listOptions);
		CLbtTriggerFilterByAttribute* filter = CLbtTriggerFilterByAttribute::NewL();
		CleanupStack::PushL(filter);
		filter->AddTriggerStateL(CLbtTriggerEntry::EStateEnabled);
        filter->AddTriggerStateL(CLbtTriggerEntry::EStateDisabled);
        listOptions->SetFilter(filter);        
		writeStream << *listOptions;
		CleanupStack::PopAndDestroy(filter);
		CleanupStack::PopAndDestroy(listOptions);
	    }
	else
		{
		CLbtTriggerFilterBase* filter = aListOptions->Filter();
        if( filter )
            {
            if( filter->Type() == CLbtTriggerFilterBase::EFilterByArea )
                {
                CLbtTriggerFilterByArea* areaFilter = static_cast<CLbtTriggerFilterByArea*> ( filter );
                ValidateGeoAreaInformationL( areaFilter->Area() );
                }
            }
		writeStream << *aListOptions;
		}
	writeStream.CommitL();
	CleanupStack::PopAndDestroy(&writeStream);
	CleanupStack::Pop();// buffer
	TPtr8 ptr = buffer->Ptr(0);
	iPtrHolder->Ptr(1).Set(ptr);
	iClientRequester->SetBuffer( buffer );
	iClientRequester->SetStatus( aStatus );
	iClientRequester->SetListTriggerData(&aTriggerList);
	
	args.Set(KTriggersSize,&iPtrHolder->Ptr(0));
	args.Set(KParamFilter,&iPtrHolder->Ptr(1));
	SendReceive(ELbtGetTriggersBufferSize,args,iClientRequester->iStatus);
	iClientRequester->Start( CLbtClientRequester::EListTriggersCount );
    }


void RLbt::HandleGetTriggersL()
    {
    TInt bufferSize = iClientRequester->ListTriggersSize();
    if( bufferSize <= 0 )
        {
        // Zero triggers found for this client. leave with KErrNotFound which will be 
        // handled in ClbtClientRequester
        User::Leave( KErrNotFound );
        }

    CBufFlat* buffer = CBufFlat::NewL(256);
    buffer->ResizeL(bufferSize);
    CleanupStack::PushL( buffer );
    
    TPtr8 bufPtr = buffer->Ptr(0);
    TIpcArgs ipcArgs;
    
    ipcArgs.Set(KParamTriggerEntry ,&bufPtr);
    User::LeaveIfError(SendReceive(ELbtGetTriggers,ipcArgs));
    
    RPointerArray<CLbtTriggerInfo>* triggerInfoList = iClientRequester->ListTriggerData();
    
    RBufReadStream stream(*buffer);
    CleanupClosePushL(stream);
    TInt numberOfTriggers = stream.ReadInt32L();
    for(TInt i=0;i<numberOfTriggers; ++i)
        {
        CLbtTriggerInfo* triggerInfo = CLbtTriggerInfo::NewL();
        CleanupStack::PushL(triggerInfo);
        triggerInfo->InternalizeL(stream);
        triggerInfoList->Append(triggerInfo);      
        CleanupStack::Pop(); // triggerInfo
        }
    CleanupStack::PopAndDestroy(2); // buffer,stream
    }
// ---------------------------------------------------------
// RLbt::CancelGetTriggers
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
EXPORT_C void RLbt::CancelGetTriggers()
    {
	__ASSERT_ALWAYS(SubSessionHandle(), User::Panic(KLbtClientPanicCategory, ELbtServerBadHandle));
	
	TInt error = SendReceive(ELbtCancelGetTriggers);	
    if( error == KErrNone )
        {
        iClientRequester->Cancel();
        }
    }

// ---------------------------------------------------------
// RLbt::CreateGetTriggerIterator
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
EXPORT_C void RLbt::CreateGetTriggerIterator( 
            TRequestStatus& aStatus,
            CLbtListTriggerOptions* aListOptions )
    {
    TRAPD( error,CreateGetTriggerIteratorL( aListOptions,aStatus ) );
    if( KErrNone != error )
    	{
    	TRequestStatus* status=&aStatus;
    	User::RequestComplete( status,error );
    	}	
    }

// ---------------------------------------------------------
// RLbt::CreateGetTriggerIteratorL
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void RLbt::CreateGetTriggerIteratorL( 
            CLbtListTriggerOptions* aListOptions,
            TRequestStatus& aStatus )
    {	
    __ASSERT_ALWAYS(SubSessionHandle(), User::Panic(KLbtClientPanicCategory, ELbtServerBadHandle));
	if(iClientRequester->IsActive())
		User::Panic(KLbtClientPanicCategory, ELbtDuplicateRequest);
	
	iCreateIteratorFlag=ETrue;
	iTriggerList.Reset();
	GetTriggers(aStatus,iTriggerList,aListOptions); 
    }


// ---------------------------------------------------------
// RLbt::CreateGetTriggerIteratorL
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
EXPORT_C void RLbt::CreateGetTriggerIteratorL( CLbtListTriggerOptions* aListOptions  )
    {
    __ASSERT_ALWAYS(SubSessionHandle(), User::Panic(KLbtClientPanicCategory, ELbtServerBadHandle));
	if(iClientRequester->IsActive())
		User::Panic(KLbtClientPanicCategory, ELbtDuplicateRequest);
    
    iCreateIteratorFlag=ETrue;
    iTriggerList.Reset();  
    GetTriggersL(iTriggerList,aListOptions);
    }

// ---------------------------------------------------------
// RLbt::CancelCreateGetTriggerIterator
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
EXPORT_C void RLbt::CancelCreateTriggerIterator()
    {
	__ASSERT_ALWAYS(SubSessionHandle(), User::Panic(KLbtClientPanicCategory, ELbtServerBadHandle));
	
	CancelGetTriggers();	
	if( iClientRequester->iStatus.Int() != KRequestPending )
        {
        iClientRequester->Cancel();
        }	
    }

// ---------------------------------------------------------
// RLbt::GetNextTriggerLC
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
EXPORT_C CLbtTriggerInfo* RLbt::GetNextTriggerLC()
    {
    __ASSERT_ALWAYS(SubSessionHandle(), User::Panic(KLbtClientPanicCategory, ELbtServerBadHandle));
	
	if(!iCreateIteratorFlag)
		{
		User::Panic(KLbtClientPanicCategory,ELbtIteratorNotCreated);
		}
	
	CLbtTriggerInfo* tempTriggerInfo;		
	if( iTriggerList.Count()==0 )
		{
		tempTriggerInfo=NULL;
		}
	else
		{
		tempTriggerInfo=iTriggerList[0];
		iTriggerList.Remove(0);
		}	
	
	CleanupStack::PushL(tempTriggerInfo);
	return(tempTriggerInfo);
    }

// ---------------------------------------------------------
// RLbt::NotifyTriggerChangeEvent
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
EXPORT_C void RLbt::NotifyTriggerChangeEvent(TLbtTriggerChangeEvent& aEvent, TRequestStatus& aStatus )
    {
    __ASSERT_ALWAYS(SubSessionHandle(), User::Panic(KLbtClientPanicCategory, ELbtServerBadHandle));
	if(iClientRequester->IsActive())
		User::Panic(KLbtClientPanicCategory, ELbtDuplicateRequest);

	TIpcArgs args;

	iPtrHolder->Ptr(0).Set(
		reinterpret_cast<TUint8*>(&aEvent),
		sizeof(aEvent),sizeof(aEvent));
		args.Set(0,&iPtrHolder->Ptr(0));
		
	iClientRequester->SetStatus(aStatus);
 
	SendReceive(ELbtNotifyTriggerChangeEvent, args,iClientRequester->iStatus);
	iClientRequester->Start();
    }

// ---------------------------------------------------------
// RLbt::CancelNotifyTriggerChangeEvent
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
EXPORT_C void RLbt::CancelNotifyTriggerChangeEvent()
    {
    __ASSERT_ALWAYS(SubSessionHandle(), User::Panic(KLbtClientPanicCategory, ELbtServerBadHandle));

    TInt error = SendReceive(ELbtCnclNotifyTriggerChangeEvent);
    if( error == KErrNone )
        {
        iClientRequester->Cancel();
        }
    }

// ---------------------------------------------------------
// RLbt::NotifySessionTriggerFired
//
// (other items were commented in a header).
// ---------------------------------------------------------
//	
EXPORT_C void RLbt::NotifyTriggerFired( TLbtTriggerFireInfo& aFireInfo, TRequestStatus& aStatus )
    {
    __ASSERT_ALWAYS(SubSessionHandle(), User::Panic(KLbtClientPanicCategory, ELbtServerBadHandle));

	aStatus=KRequestPending;
	if(iClientRequester->IsActive())
		User::Panic(KLbtClientPanicCategory, ELbtDuplicateRequest);
	TIpcArgs args;

	TPckg<TLbtTriggerFireInfo> fireInfo(aFireInfo);
       
    iPtrHolder->Ptr(0).Set(fireInfo);
    args.Set(KParamTriggerFireInfo,&iPtrHolder->Ptr(0));

    iClientRequester->SetStatus(aStatus);
 
	SendReceive(ELbtNotifyTriggerFired, args,iClientRequester->iStatus);
	iClientRequester->Start();
    }
// ---------------------------------------------------------
// RLbt::CancelNotifySessionTriggerFired
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
EXPORT_C void RLbt::CancelNotifyTriggerFired()
    {
    __ASSERT_ALWAYS(SubSessionHandle(), User::Panic(KLbtClientPanicCategory, ELbtServerBadHandle));
    TInt error = SendReceive(ELbtCnclNotifyTriggerFired);
    
    if( error == KErrNone )
        {
        iClientRequester->Cancel();
        }   
    }
    
// ---------------------------------------------------------
// RLbt::GetFiredStartupTriggersL
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
EXPORT_C void RLbt::GetFiredTriggersL( RArray < TLbtTriggerFireInfo >& aTriggerInfoList )
    {
    __ASSERT_ALWAYS(SubSessionHandle(), User::Panic(KLbtClientPanicCategory, ELbtServerBadHandle));

	TIpcArgs args;
	TInt numFiredTriggers;
	iPtrHolder->Ptr(0).Set(
	reinterpret_cast<TUint8*>(&numFiredTriggers),
	sizeof(numFiredTriggers),sizeof(numFiredTriggers));
	args.Set(KTriggersSize,&iPtrHolder->Ptr(0));
	SendReceive(EGetFiredTriggersCount,args);
	
	if(numFiredTriggers == 0)
		{
		// Zero triggers found for this client. So just return after resetting the array
		aTriggerInfoList.Reset();
		return;
		}
	TInt sizeOfFireInfo = sizeof(TLbtTriggerFireInfo);
	
	CBufFlat* buffer = CBufFlat::NewL( 512 );
	CleanupStack::PushL(buffer);
	buffer->ResizeL(numFiredTriggers * sizeOfFireInfo);
	TPtr8 ptr = buffer->Ptr(0);
	
	TIpcArgs ipcArgs;	
	ipcArgs.Set(KParamTriggerFireInfo, &ptr);
	User::LeaveIfError(SendReceive(ELbtGetFiredTriggers,ipcArgs));
	
	// Got response append the trigger info into the array
	RBufReadStream readStream;
	readStream.Open(*buffer);
	CleanupClosePushL(readStream);
	for(TInt i = 0 ;i<numFiredTriggers; ++i)
		{
		TLbtTriggerFireInfo fireInfo;
		TUint8* ptr = reinterpret_cast<TUint8*>(&fireInfo);
		readStream.ReadL(ptr, sizeof(TLbtTriggerFireInfo));
		aTriggerInfoList.Append(fireInfo);
		}
	CleanupStack::PopAndDestroy(2);//buffer, readStream
    }


// ---------------------------------------------------------
// RLbt::NotifyTriggeringSystemSettingChange
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
EXPORT_C void RLbt::NotifyTriggeringSystemSettingChange( TLbtTriggeringSystemSettings& aSettings,TRequestStatus& aStatus )
    {
    __ASSERT_ALWAYS(SubSessionHandle(), User::Panic(KLbtClientPanicCategory, ELbtServerBadHandle));
    if(iClientRequester->IsActive())
    	User::Panic(KLbtClientPanicCategory, ELbtDuplicateRequest);            
    TIpcArgs args;

    iPtrHolder->Ptr(0).Set(
   		reinterpret_cast<TUint8*>(&aSettings),
   		sizeof(TLbtTriggeringSystemSettings),sizeof(TLbtTriggeringSystemSettings));
   		args.Set(0,&iPtrHolder->Ptr(0));
   		
    iClientRequester->SetStatus(aStatus);
    SendReceive(ELbtNotifyTriggeringSysSettingChange, args,iClientRequester->iStatus);
    iClientRequester->Start();
    }
    
// ---------------------------------------------------------
// RLbt::CancelNotifyTriggeringSystemSettingChange
//
// (other items were commented in a header).
// ---------------------------------------------------------
//            
EXPORT_C void RLbt::CancelNotifyTriggeringSystemSettingChange()
    {
    __ASSERT_ALWAYS(SubSessionHandle(), User::Panic(KLbtClientPanicCategory, ELbtServerBadHandle));

    TInt error = SendReceive(ELbtCnclNotifyTriggeringSysSettingChange);
    if( error == KErrNone )
        {
        iClientRequester->Cancel();
        }
    }
// ---------------------------------------------------------
// RLbt::GetTriggeringSystemSettingL
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
EXPORT_C void RLbt::GetTriggeringSystemSettingsL( TLbtTriggeringSystemSettings& aSetting )
    {
    __ASSERT_ALWAYS(SubSessionHandle(), User::Panic(KLbtClientPanicCategory, ELbtServerBadHandle));
            
    TIpcArgs args;		
    TPckg<TLbtTriggeringSystemSettings> sysSettings(aSetting);
    args.Set(KParamSettings,&sysSettings);
	User::LeaveIfError(SendReceive(ELbtGetTriggeringSysSetting,args));
    }

// ---------------------------------------------------------
// RLbt::CancelAll
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
EXPORT_C void RLbt::CancelAll()
    {   
	__ASSERT_ALWAYS(SubSessionHandle(), User::Panic(KLbtClientPanicCategory, ELbtServerBadHandle));
	
	SendReceive(ELbtCancelAll);
	iClientRequester->Cancel();
    }

// ---------------------------------------------------------
// RLbt::ValidateGeoAreaInformationL
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void RLbt::ValidateGeoAreaInformationL( CLbtGeoAreaBase* aGeoArea )
	{
	switch( aGeoArea->Type() )
	    {
	    case CLbtGeoAreaBase::ECircle:
	        {
	        CLbtGeoCircle* geoCircle = static_cast<CLbtGeoCircle*>( aGeoArea );
	        geoCircle->ValidateCircleInformationL();
	        break;
	        }
	    case CLbtGeoAreaBase::ECellular:
            {
            CLbtGeoCell* geoCell = static_cast<CLbtGeoCell*> ( aGeoArea );                        
            geoCell->ValidateCellInformationL();                        
            break;
            }
	    case CLbtGeoAreaBase::EHybrid:
	        {
	        CLbtGeoHybrid* geoHybrid  = static_cast<CLbtGeoHybrid*> ( aGeoArea );   
	        geoHybrid->ValidateHybridInformationL();
            break;
	        }
	    default:
	        {
	        break;
	        }
	    }	
	}
