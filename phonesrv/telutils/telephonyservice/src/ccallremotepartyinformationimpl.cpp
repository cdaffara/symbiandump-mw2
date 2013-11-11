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
* Description: 
*
*/

#include <callremotepartyinformation.h>
#include <mcallremotepartyinfoobserver.h>

#include "ccallremotepartyinformationimpl.h"
#include "ccallremotepartyinfoiter.h"
#include "telsrvlogger.h"

// ======== MEMBER FUNCTIONS ========

// ---------------------------------------------------------------------------
// C++ constructor
// ---------------------------------------------------------------------------
//
CCallRemotePartyInformationImpl::CCallRemotePartyInformationImpl()
    : CTelSrvBase( )
    {
    }

// ---------------------------------------------------------------------------
// Symbian second-phase constructor.
// ---------------------------------------------------------------------------
//
void CCallRemotePartyInformationImpl::
    ConstructL(CMediatorService* aMediatorService)
    {    
    TSLOGSTRING("CCallInformationImpl::ConstructL >");
    CTelSrvBase::BaseConstructL( aMediatorService );    
    
    TSLOGSTRING("CCallInformationImpl::ConstructL <");
    }


// ---------------------------------------------------------------------------
// Static constructor
// ---------------------------------------------------------------------------
//
CCallRemotePartyInformationImpl* CCallRemotePartyInformationImpl::NewL()
    {
    CCallRemotePartyInformationImpl* self =
        CCallRemotePartyInformationImpl::NewLC();          
            
    CleanupStack::Pop( self );        
    return self;
    }


// ---------------------------------------------------------------------------
// Static constructor
// ---------------------------------------------------------------------------
//
CCallRemotePartyInformationImpl* CCallRemotePartyInformationImpl::NewLC()
    {
    CCallRemotePartyInformationImpl* self =
        new( ELeave ) CCallRemotePartyInformationImpl;
    CleanupStack::PushL( self );
    
    CMediatorService* mediatorService =
        CMediatorService::NewLC( CMediatorService::ECallRemotePartyInfo );    
            
    self->ConstructL(mediatorService);    
    CleanupStack::Pop( mediatorService );    
    return self;
    }


// ---------------------------------------------------------------------------
// Destructor
// ---------------------------------------------------------------------------
//
CCallRemotePartyInformationImpl::~CCallRemotePartyInformationImpl()
    {
    delete iRemotePartyInfos;
    delete iRemotePartyInfoIter;
    }

// ---------------------------------------------------------------------------
// Saves the observer call information changes.
// ---------------------------------------------------------------------------
//
void CCallRemotePartyInformationImpl::NotifyChanges(
    MCallRemotePartyInformationObserver& aObserver )
    {
    TSLOGSTRING("CCallInformationImpl::NotifyChanges <<");
    iObserver = &aObserver;    
    }
    
// ---------------------------------------------------------------------------
// Removes the observer.
// ---------------------------------------------------------------------------
//
void CCallRemotePartyInformationImpl::CancelNotification( )
    {
    TSLOGSTRING("CCallRemotePartyInformationImpl::CancelNotification <<");
    iObserver = NULL;    
    }

// ---------------------------------------------------------------------------
// 
// ---------------------------------------------------------------------------
//
CCallRemotePartyInfoIter&
    CCallRemotePartyInformationImpl::GetRemotePartyInformationL( )
    {
    TSLOGSTRING("CCallRemotePartyInformationImpl::GetRemotePartyInfoL <<");
    if( !iRemotePartyInfoIter )
        {
        TSLOGSTRING("GetRemotePartyInformationL; Leaves with KErrNoMemory");
        //memory allocation failed in CommandResponseL or MediatorEventL
        User::Leave( KErrNoMemory ); 
        }
            
    TSLOGSTRING("CCallRemotePartyInformationImpl::GetRemotePartyInfoL >>");
    return *iRemotePartyInfoIter; 
    }

// ---------------------------------------------------------------------------
// 
// ---------------------------------------------------------------------------
//
const MCallRemotePartyInfo* 
    CCallRemotePartyInformationImpl::GetRemotePartyInformationL( TInt aCallIndex )
    {
    TSLOGSTRING("CCallRemotePartyInformationImpl::GetRemotePartyInfoL(aCallIndex) <<");
    if( !iRemotePartyInfoIter )
        {
        TSLOGSTRING("GetRemotePartyInformationL; Leaves with KErrNoMemory");
        //memory allocation failed in CommandResponseL or MediatorEventL
        User::Leave( KErrNoMemory ); 
        }    
    
    for( iRemotePartyInfoIter->First(); !iRemotePartyInfoIter->IsDone(); iRemotePartyInfoIter->Next() )
        {
        const MCallRemotePartyInfo& info( iRemotePartyInfoIter->Current() );
        if ( info.CallIndex() == aCallIndex )
            {
            return &info; 
            }         
        }    
       
    return NULL;    
    }

// ---------------------------------------------------------------------------
// From class CTelSrvBase.
// A response to a Mediator Service command.
// ---------------------------------------------------------------------------
//
void CCallRemotePartyInformationImpl::
    DoHandleCommandResponseL( const TDesC8& aData )
    {
    TSLOGSTRING("CCallRemotePartyInformationImpl::DoHandleCommandResponseL <<");    
  
    iRemotePartyInfos = CCallRemotePartyInfos::NewL();
    iRemotePartyInfos->InternalizeL( aData );
  
    iRemotePartyInfoIter = CCallRemotePartyInfoIter::NewL( *iRemotePartyInfos );
  
    TSLOGSTRING("CCallRemotePartyInformationImpl::DoHandleCommandResponseL >>");
    }

// ---------------------------------------------------------------------------
// From class MMediatorServiceObserver
// A Mediator Service event.
// ---------------------------------------------------------------------------
//
void CCallRemotePartyInformationImpl::DoHandleMediatorEventL( const TDesC8& aData )
    {  
    TSLOGSTRING("CCallRemotePartyInformationImpl::DoHandleMediatorEventL <<");
  
    delete iRemotePartyInfos;
    iRemotePartyInfos = NULL;
        
    delete iRemotePartyInfoIter;
    iRemotePartyInfoIter = NULL;
        
    iRemotePartyInfos = CCallRemotePartyInfos::NewL();
    iRemotePartyInfos->InternalizeL( aData ); 
  
    iRemotePartyInfoIter = CCallRemotePartyInfoIter::NewL( *iRemotePartyInfos );      
          
    if( iObserver )
        {
        iObserver->CallRemotePartyInformationChanged();
        } 
  
    TSLOGSTRING("CCallRemotePartyInformationImpl::DoHandleMediatorEventL >>");
    }
