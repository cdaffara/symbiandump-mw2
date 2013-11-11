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
* Description:  Provides implementation for CTelSrvBase -class.
*
*/

#include "ctelsrvbase.h"
#include "telsrvlogger.h"

// ======== MEMBER FUNCTIONS ========

// ---------------------------------------------------------------------------
// C++ constructor.
// ---------------------------------------------------------------------------
//
CTelSrvBase::CTelSrvBase()
    {
    // No implementation required
    }

// ---------------------------------------------------------------------------
// Destructor
// ---------------------------------------------------------------------------
//
CTelSrvBase::~CTelSrvBase()
    {
    delete iCommandBuffer;
    delete iWaitForCommandResponse;
    delete iMediatorService;
    }

// ---------------------------------------------------------------------------
// Symbian second-phase constructor.
// ---------------------------------------------------------------------------
//
void CTelSrvBase::BaseConstructL( CMediatorService* aMediatorService )
    {
    iMediatorService = aMediatorService;
    iWaitForCommandResponse = new( ELeave )CActiveSchedulerWait();
    iCommandBuffer = CIdle::NewL( CActive::EPriorityHigh );
    
    iMediatorService->SetObserver(this);    
    //event might come while getting initial state so subscribe it first.
    SubscribeMediatorEventL();    
    Initialize();
    }

// ---------------------------------------------------------------------------
// From class MMediatorCommandResponseObserver.
// A response to a Mediator Service command.
// ---------------------------------------------------------------------------
//
void CTelSrvBase::CommandResponseL( const TDesC8& aData )
  {
  TSLOGSTRING("CTelSrvBase::CommandResponseL <<");
  if( iWaitForCommandResponse->IsStarted() )
      {
      iWaitForCommandResponse->AsyncStop();
      DoHandleCommandResponseL( aData );
      }  
  }

// ---------------------------------------------------------------------------
// From class MMediatorCommandResponseObserver.
// for handling a Mediator Service event.
// ---------------------------------------------------------------------------
//
void CTelSrvBase::MediatorEventL( const TDesC8& aData )
    {
    TSLOGSTRING("CTelSrvBase::MediatorEventL <<");
    
    // Command response can be discarded if event is received
    // while waiting for response.
    if( iWaitForCommandResponse->IsStarted() )
        {
        iWaitForCommandResponse->AsyncStop();
        }  
    
    DoHandleMediatorEventL( aData );
    }
 


// ---------------------------------------------------------------------------
// Get the current info through Mediator.
// ---------------------------------------------------------------------------
//
void CTelSrvBase::Initialize(  )
  {
  TSLOGSTRING("CTelSrvBase::Initialize <<");
  TInt error = iMediatorService->SendCommand();    
      
  if( KErrInUse == error )    //Command needs to be re-send
      {        
      if ( !iCommandBuffer->IsActive() )
          {
          iCommandBuffer->Start( TCallBack( DoReSendCommand, this ) );          
          }
      }
    
  if( !iWaitForCommandResponse->IsStarted() )
      {
      iWaitForCommandResponse->Start();
      }
  }

// ---------------------------------------------------------------------------
// Starts re-sending mediator command.
// ---------------------------------------------------------------------------
//
TInt CTelSrvBase::DoReSendCommand(TAny* aPtr)
    {    
    TSLOGSTRING("CTelSrvBase::DoReSendCommand <<");
    CTelSrvBase* self = 
            static_cast< CTelSrvBase* >( aPtr );            
            
    self->Initialize();         
    return KErrNone;        
    }

// ---------------------------------------------------------------------------
// Subscribes to Mediator event.
// ---------------------------------------------------------------------------
//
void CTelSrvBase::SubscribeMediatorEventL()
    {
    TSLOGSTRING("CTelSrvBase::SubscribeMediatorEventL <<");
    TInt err = iMediatorService->SubscribeEvent();                                                                
    User::LeaveIfError( err );
    TSLOGSTRING("CTelSrvBase::SubscribeMediatorEventL >>");
    }

  
