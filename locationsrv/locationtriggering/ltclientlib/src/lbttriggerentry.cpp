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
* Description: Base class implementation of trigger entries.
*
*/


#include <s32strm.h>
#include <lbterrors.h>
#include "lbttriggerentry.h"
#include "lbttriggerconditionbase.h"

// ======== LOCAL FUNCTIONS ========

// ---------------------------------------------------------------------------
// CLbtTriggerEntry::Id()
//
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//
EXPORT_C  const TLbtTriggerId&  CLbtTriggerEntry::Id() const 
    {
	return iId;
    }
// ---------------------------------------------------------------------------
// CLbtTriggerEntry::SetId()
//
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//
EXPORT_C void CLbtTriggerEntry::SetId( TLbtTriggerId aId )
    {
	iId=aId;
    }
// ---------------------------------------------------------------------------
// CLbtTriggerEntry::Name()
//
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//
EXPORT_C const TDesC& CLbtTriggerEntry::Name() const
    {
    if(!iName)
        {
        return KNullDesC;
        }
    else
        {
        return *iName;    
        }
    }
// ---------------------------------------------------------------------------
// CLbtTriggerEntry::SetNameL()
//
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//
EXPORT_C void CLbtTriggerEntry::SetNameL( const TDesC& aName )
    {
	if(aName.Length()>KLbtMaxNameLength)
		User::Leave(KErrArgument);
	else
    	{
    	if( iName )
    	    {
    	    delete iName;
    	    iName = NULL;
    	    }
    	iName=HBufC::NewL(aName.Length());
    	TPtr NamPtr=iName->Des();
    	NamPtr.Copy(aName);	
    	}
	}
// ---------------------------------------------------------------------------
// CLbtTriggerEntry::SetTimeToRearm()
//
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//    
EXPORT_C void CLbtTriggerEntry::SetTimeToRearm( TInt aSeconds )
    {
    if( aSeconds < KLbtMinTimeToRearm || aSeconds > KLbtMaxTimeToRearm )
        
        User::Panic(KLbtClientPanicCategory,ELbtErrArgument);
        
    iRearmTime = aSeconds;
    }	
// ---------------------------------------------------------------------------
// CLbtTriggerEntry::State()
//
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//
EXPORT_C CLbtTriggerEntry::TLbtTriggerState CLbtTriggerEntry::State() const
    {
  	return iState;
    }
// ---------------------------------------------------------------------------
// CLbtTriggerEntry::SetState()
//
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//
EXPORT_C void CLbtTriggerEntry::SetState( TLbtTriggerState aState )
    {
	iState=aState;
    }
// ---------------------------------------------------------------------------
// CLbtTriggerEntry::TimeToRearm()
//
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//    
EXPORT_C TInt CLbtTriggerEntry::TimeToRearm() const
    {
    return iRearmTime;
    }
    
// ---------------------------------------------------------------------------
// CLbtTriggerEntry::GetRequestorsL()
//
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//
EXPORT_C void CLbtTriggerEntry::GetRequestorsL( RRequestorStack& aRequestors ) const
    {
    aRequestors.Reset();
    TInt count = iRequestor.Count();
    for( TInt i=0;i<count;i++ )
        {
        CRequestor *Requestor = CRequestor::NewL(iRequestor[i]->RequestorType(),
                                                 iRequestor[i]->RequestorFormat(),
                                                 iRequestor[i]->RequestorData());
        aRequestors.Append( Requestor );
        }
    }
    
// ---------------------------------------------------------------------------
// CLbtTriggerEntry::SetRequestorsL()
//
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//
EXPORT_C void CLbtTriggerEntry::SetRequestorsL( const RRequestorStack& aRequestors ) 
    {
    TInt count = aRequestors.Count();
    if( count>0 )
        {
        for( TInt i=0;i<count;i++ )
            {
            CRequestor *Requestor = CRequestor::NewL(aRequestors[i]->RequestorType(),
                                                     aRequestors[i]->RequestorFormat(),
                                                     aRequestors[i]->RequestorData());
            iRequestor.Append( Requestor );
            }
        }
    else
        {
        User::Leave(KErrArgument);
        }
    }
// ---------------------------------------------------------------------------
// CLbtTriggerEntry::SetRequestorL()
//
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//
EXPORT_C void CLbtTriggerEntry::SetRequestorL( 
            CRequestor::TRequestorType aType,
            CRequestor::TRequestorFormat aFormat,
            const TDesC& aData )
    {
    CRequestor *Requestor=CRequestor::NewL(aType,aFormat,aData);
	iRequestor.Append(Requestor);
    }            
            

// ---------------------------------------------------------------------------
// CLbtTriggerEntry::ManagerUi()
//
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//
EXPORT_C TUid CLbtTriggerEntry::ManagerUi() const
    {
	return iManagerUi;
    }
// ---------------------------------------------------------------------------
// CLbtTriggerEntry::SetManagerUi()
//
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//
EXPORT_C void CLbtTriggerEntry::SetManagerUi( TUid aUid )
    {
    iManagerUi=aUid;
    }
// ---------------------------------------------------------------------------
// CLbtTriggerEntry::GetCondition()
//
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//
EXPORT_C CLbtTriggerConditionBase* CLbtTriggerEntry::GetCondition()
    {
	return iTriggerCondition;
    }

// ---------------------------------------------------------------------------
// CLbtTriggerEntry::GetCondition()
//
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//
EXPORT_C const CLbtTriggerConditionBase* CLbtTriggerEntry::GetCondition() const
    {
	return iTriggerCondition;
    }

// ---------------------------------------------------------------------------
// CLbtTriggerEntry::SetCondition()
//
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//
EXPORT_C void CLbtTriggerEntry::SetCondition( CLbtTriggerConditionBase* aCondition )
    {
  if( iTriggerCondition )
  	{
  	delete iTriggerCondition;	 
  	iTriggerCondition = NULL;
  	} 	
	iTriggerCondition=aCondition;
	}


// ---------------------------------------------------------------------------
// CLbtTriggerEntry::InternalizeL()
//
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//
EXPORT_C void CLbtTriggerEntry::InternalizeL( RReadStream& aStream )
    {
    iRequestor.InternalizeL( aStream );
    DoInternalizeL(aStream);
    }

// ---------------------------------------------------------------------------
// CLbtTriggerEntry::ExternalizeL()
//
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//
EXPORT_C void CLbtTriggerEntry::ExternalizeL( RWriteStream& aStream ) const
    {
    iRequestor.ExternalizeL( aStream );
    DoExternalizeL(aStream);
    }	

// ---------------------------------------------------------------------------
// CLbtTriggerEntry::CLbtTriggerEntry()
// Default constructor
// ---------------------------------------------------------------------------
//   
CLbtTriggerEntry::CLbtTriggerEntry()
    {
	iName = NULL;
	iTriggerCondition = NULL;
	iRearmTime = KLbtDefaultTimeToRearm;
    }

// ---------------------------------------------------------------------------
// CLbtTriggerEntry::~CLbtTriggerEntry()
// Destructor
// ---------------------------------------------------------------------------
// 
CLbtTriggerEntry::~CLbtTriggerEntry()
    {
    if( iTriggerCondition )
        {
        delete iTriggerCondition;
        iTriggerCondition = NULL;
        }
    if( iName )
        {
        delete iName;
        iName = NULL;
        }
	iRequestor.ResetAndDestroy();
	iRequestor.Close();
	}



