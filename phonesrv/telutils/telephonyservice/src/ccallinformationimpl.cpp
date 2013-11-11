/*
* Copyright (c) 2007-2008 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  Provides implementation for CCallInformation -class.
*
*/


#include <e32base.h>
#include <callinformation.h>    //Call Information Mediator API

#include "ccallinformationimpl.h"
#include "ccallinfoiter.h"
#include "mcallinformationobserver.h"
#include "telsrvlogger.h"


// ======== MEMBER FUNCTIONS ========

// ---------------------------------------------------------------------------
// C++ constructor.
// ---------------------------------------------------------------------------
//
CCallInformationImpl::CCallInformationImpl() : CTelSrvBase( )
    {
         
    }


// ---------------------------------------------------------------------------
// Symbian second-phase constructor.
// ---------------------------------------------------------------------------
//
void CCallInformationImpl::ConstructL(CMediatorService* aMediatorService)
    {    
    CTelSrvBase::BaseConstructL( aMediatorService );    
    
    }


// ---------------------------------------------------------------------------
// Two-phased constructor.
// ---------------------------------------------------------------------------
//
CCallInformationImpl* CCallInformationImpl::NewL()
    {
    TSLOGSTRING("CCallInformationImpl::NewL <<");
    CCallInformationImpl* self = new (ELeave) CCallInformationImpl( );     
    CleanupStack::PushL( self );
    
    CMediatorService* mediatorService = 
        CMediatorService::NewL( CMediatorService::ECallInfo );    
    self->ConstructL(mediatorService);
    
    CleanupStack::Pop( self );
    TSLOGSTRING("CCallInformationImpl::NewL >>");
    return self;
    }

// ---------------------------------------------------------------------------
// Two-phased constructor for testing purposes.
// ---------------------------------------------------------------------------
//
CCallInformationImpl* CCallInformationImpl::NewL(CMediatorService* aMediatorService)
    {        
    CCallInformationImpl* self = new (ELeave) CCallInformationImpl( );    
    
    CleanupStack::PushL( self );
    self->ConstructL(aMediatorService);
    CleanupStack::Pop( self );
    
    return self;
    }


// ---------------------------------------------------------------------------
// Destructor
// ---------------------------------------------------------------------------
//
CCallInformationImpl::~CCallInformationImpl()
    {        
    delete iInfos;
    delete iCallIter;       
    delete iInfosInState;    
    delete iCallStateIter;    
    }


// ---------------------------------------------------------------------------
// Saves the observer call information changes.
// ---------------------------------------------------------------------------
//
void CCallInformationImpl::NotifyCallInformationChanges(
    MCallInformationObserver& aObserver )
    {
    TSLOGSTRING("CCallInformationImpl::NotifyCallInformationChanges <<");
    iObserver = &aObserver;    
    }
    
// ---------------------------------------------------------------------------
// Removes the observer.
// ---------------------------------------------------------------------------
//
void CCallInformationImpl::CancelNotification( )
    {
    TSLOGSTRING("CCallInformationImpl::CancelNotification <<");
    iObserver = NULL;    
    }


// ---------------------------------------------------------------------------
// From class CTelSrvBase.
// A response to a Mediator Service command.
// ---------------------------------------------------------------------------
//
void CCallInformationImpl::DoHandleCommandResponseL( const TDesC8& aData )
  {
  TSLOGSTRING("CCallInformationImpl::DoHandleCommandResponseL <<");  
  iInfos = CCallInfos::NewL();
  iInfos->InternalizeL( aData );                
  iCallIter = CCallInfoIter::NewL( *iInfos );
  
  TSLOGSTRING("CCallInformationImpl::DoHandleCommandResponseL >>");
  }

// ---------------------------------------------------------------------------
// From class MMediatorServiceObserver
// A Mediator Service event.
// ---------------------------------------------------------------------------
//
void CCallInformationImpl::DoHandleMediatorEventL( const TDesC8& aData )
  {  
  TSLOGSTRING("CCallInformationImpl::DoHandleMediatorEventL <<");  
  delete iInfos;
  iInfos = NULL;
      
  delete iCallIter;
  iCallIter = NULL;
      
  iInfos = CCallInfos::NewL();
  iInfos->InternalizeL( aData );      
  iCallIter = CCallInfoIter::NewL( *iInfos );      
        
  if( iObserver )
      {
      iObserver->CallInformationChanged();
      }  
  TSLOGSTRING("CCallInformationImpl::DoHandleMediatorEventL >>");
  }

// ---------------------------------------------------------------------------
// Returns info about all calls. 
// ---------------------------------------------------------------------------
//
CCallInfoIter& CCallInformationImpl::GetCallsL( )
    {    
    TSLOGSTRING("CCallInformationImpl::GetCallsL <<");
    if( !iCallIter )
        {
        TSLOGSTRING("CCallInformationImpl::GetCallsL; Leaves with KErrNoMemory");
        User::Leave( KErrNoMemory ); //memory allocation failed in CommandResponseL or MediatorEventL
        }
    
    TSLOGSTRING("CCallInformationImpl::GetCallsL >>");
    return *iCallIter;            
    }

// ---------------------------------------------------------------------------
// Returns information whether call in given state exist. 
// ---------------------------------------------------------------------------
//
TBool CCallInformationImpl::IsCallInState( CCPCall::TCallState aState ) const
    {
    TBool match ( EFalse );
    
    const RPointerArray<TCallInfo>& calls = iInfos->CallInfos();
    TInt count = calls.Count();
        
    for( int i = 0; i < count; i++ )
        {
        const TCallInfo& call = *calls[i];
        if( aState == call.CallState() )    
            {
            match = ETrue;
            break;
            }
        }           
    return match;
    }





