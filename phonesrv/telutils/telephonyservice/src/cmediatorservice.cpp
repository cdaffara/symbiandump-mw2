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
* Description:  This class is responsible for communication towards Mediator.
*
*/

#include <MediatorCommandInitiator.h> 
#include <MediatorDomainUIDs.h> 
#include <callinformation.h>            //Call Information Mediator API
#include <callremotepartyinformation.h> //Call Remote Party Information Mediator API

#include "cmediatorservice.h"
#include "telsrvlogger.h"



CMediatorService::CMediatorService( TServiceCategory aService )
    : iServiceCategory( aService )
    {
    // No implementation required
    }

CMediatorService::~CMediatorService()
    {
    
    if (iMediatorEvent)
            {
            iMediatorEvent->UnsubscribeEvent( iDomain,
                                              iCategory, 
                                              iEventId );
            }
        
    delete iMediatorEvent;    
    delete iCommandInitiator;
    }

CMediatorService* CMediatorService::NewLC( TServiceCategory aService )    
    {
    CMediatorService* self = new (ELeave) CMediatorService( aService );
    CleanupStack::PushL(self);
    self->ConstructL();
    return self;
    }

CMediatorService* CMediatorService::NewL( TServiceCategory aService )
    {
    CMediatorService* self = CMediatorService::NewLC( aService );
    CleanupStack::Pop(self); 
    return self;
    }

// ---------------------------------------------------------------------------
// Sets the observer.
// ---------------------------------------------------------------------------
//
void CMediatorService::SetObserver( MMediatorServiceObserver* aObserver )
    {
    iObserver = aObserver;
    }


void CMediatorService::ConstructL()
    {
    iCommandInitiator = CMediatorCommandInitiator::NewL( this );
    iMediatorEvent = CMediatorEventConsumer::NewL( this ); 
    SetMediatorParams();
    }

// ---------------------------------------------------------------------------
// Sends a Mediator Service command.
// ---------------------------------------------------------------------------
//
TInt CMediatorService::SendCommand()
    {    
    TInt error = iCommandInitiator->IssueCommand(
                iDomain, 
                iCategory, 
                iCommandId, 
                iVersion,
                KNullDesC8 );
    
    return error;        
    }

// ---------------------------------------------------------------------------
// Subscribes to Mediator event.
// ---------------------------------------------------------------------------
//
TInt CMediatorService::SubscribeEvent()
    {
    TInt error = iMediatorEvent->SubscribeEvent( iDomain,
                                                 iCategory, 
                                                 iEventId, 
                                                 iVersion );
    return error;
    }

// ---------------------------------------------------------------------------
// From class MMediatorCommandResponseObserver.
// A response to a Mediator Service command.
// ---------------------------------------------------------------------------
//
void CMediatorService::CommandResponseL( TUid /*aDomain*/, TUid /*aCategory*/, 
  TInt aCommandId, TInt aStatus, const TDesC8& aData )
  {
  
  if( KErrNone != aStatus )
      {      
      TSLOGSTRING2("CMediatorService::CommandResponseL; status %d", aStatus);
      return;
      }
    
  if ( iCommandId == aCommandId )
      {
      if (iObserver)
          {
          iObserver->CommandResponseL(aData);
          }
      else // observer not set
          {
          TSLOGSTRING("CMediatorService::CommandResponseL; No observer");
          }
      }
  else // command ID doesn't match
      {
      TSLOGSTRING2("CMediatorService::CommandResponseL; CommandId %d", aCommandId);
      }      
  }

// ---------------------------------------------------------------------------
// From class MMediatorEventObserver.
// A Mediator Service event.
// ---------------------------------------------------------------------------
//
void CMediatorService::MediatorEventL( TUid /*aDomain*/, TUid /*aCategory*/, 
  TInt aEventId, const TDesC8& aData )
  {    
  
  if ( iEventId == aEventId )
      {
      if (iObserver)
          {
          iObserver->MediatorEventL(aData); 
          }
      else
          {
          TSLOGSTRING("CMediatorService::MediatorEventL; No observer");
          }        
      }
  else
      {
      TSLOGSTRING2("CMediatorService::MediatorEventL; EventId %d", aEventId);     
      }  
  }

// ---------------------------------------------------------------------------
// Sets the parameters used in Mediator commands and events
// ---------------------------------------------------------------------------
//
void CMediatorService::SetMediatorParams()
    {
    iDomain = KMediatorTelephonyDomain;
    
    switch ( iServiceCategory )
        {
        case ECallInfo:            
            iCategory = KCatCallInformation;
            iCommandId = EGetAllCallStates;
            iEventId = EChangesInCallStates;            
            iVersion.iMajor = KCallInformationVersionMajor;
            iVersion.iMinor = KCallInformationVersionMinor; 
            iVersion.iBuild = KCallInformationVersionBuild;            
            break;
        case ECallRemotePartyInfo:  
            iCategory = KCatCallRemotePartyInformation;
            iCommandId = EGetCallRemotePartyInformation;
            iEventId = EChangesInRemotePartyInformation;
            iVersion.iMajor = KCallRemotePartyInformationVersionMajor;
            iVersion.iMinor = KCallRemotePartyInformationVersionMinor; 
            iVersion.iBuild = KCallRemotePartyInformationVersionBuild;            
            break;
        default:
            TSLOGSTRING("CMediatorService::SetMediatorParams; unknown service category");
            break;
        }    
    }
