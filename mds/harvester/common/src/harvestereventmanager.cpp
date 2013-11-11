/*
* Copyright (c) 2006-2009 Nokia Corporation and/or its subsidiary(-ies). 
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
#include <e32svr.h>

#include "harvestereventmanager.h"
#include "harvesterlog.h"
#include "OstTraceDefinitions.h"
#ifdef OST_TRACE_COMPILER_IN_USE
#include "harvestereventmanagerTraces.h"
#endif


const TInt KHarvesterEventManagerTLSKey = 0x200104D9;

CHarvesterEventManager::CHarvesterEventManager()
	{
	}

void CHarvesterEventManager::ConstructL()
	{
	}

CHarvesterEventManager::~CHarvesterEventManager()
	{
	for ( TInt i = 0; i < iEventQueues.Count(); i++)
		{
		iEventQueues[i]->Close();
		}
	iEventQueues.ResetAndDestroy();
	iEventQueues.Close();
	
	iRegisteredObservers.ResetAndDestroy();
	iRegisteredObservers.Close();
	
	iEventStatuses.Close();
	}

EXPORT_C CHarvesterEventManager* CHarvesterEventManager::GetInstanceL()
	{
	WRITELOG( "CHarvesterEventManager::GetInstanceL" );
	OstTrace0( TRACE_NORMAL, CHARVESTEREVENTMANAGER_GETINSTANCEL, "CHarvesterEventManager::GetInstanceL" );	

	CHarvesterEventManagerStaticData* data = 
		static_cast<CHarvesterEventManagerStaticData*>( 
				UserSvr::DllTls( KHarvesterEventManagerTLSKey ) );

	CHarvesterEventManager* instance = NULL;

	if ( !data )
        {
        instance = new (ELeave) CHarvesterEventManager();
        CleanupStack::PushL( instance );
        instance->ConstructL();
        UserSvr::DllSetTls( KHarvesterEventManagerTLSKey,
        	new (ELeave) CHarvesterEventManagerStaticData(instance) );
        CleanupStack::Pop( instance );
        }
    else
        {
        instance = data->iHEM;
        data->iRefCount++;
        }

    return instance;
	}

EXPORT_C void CHarvesterEventManager::ReleaseInstance()
    {
    WRITELOG( "CHarvesterEventManager::ReleaseInstance" );
    OstTrace0( TRACE_NORMAL, CHARVESTEREVENTMANAGER_RELEASEINSTANCE, "CHarvesterEventManager::ReleaseInstance" );
    
    CHarvesterEventManagerStaticData* data =
        static_cast<CHarvesterEventManagerStaticData*>( 
        		UserSvr::DllTls( KHarvesterEventManagerTLSKey ) );
    if ( data )
        {
        data->iRefCount--;
        if ( data->iRefCount <= 0 )
            {
            // destroy the singleton and free TLS
            delete data;
            data = NULL;
            UserSvr::DllFreeTls( KHarvesterEventManagerTLSKey );
            }
        }
    }

EXPORT_C void CHarvesterEventManager::IncreaseItemCount( 
		HarvesterEventObserverType aHEObserverType, TUint aCount )
	{
	TEventStatus* eventStatus = GetEventStatus( aHEObserverType );
	WRITELOG2( "CHarvesterEventManager::IncreaseItemCount(%d) type = %d", aCount, aHEObserverType); 
	
#ifdef _DEBUG
	switch(aHEObserverType)
	    {
	    case EHEObserverTypePlaceholder:
	        {
	        WRITELOG( "CHarvesterEventManager::IncreaseItemCount() EHEObserverTypePlaceholder");
	        OstTrace0( TRACE_NORMAL, CHARVESTEREVENTMANAGER_INCREASEITEMCOUNT, "CHarvesterEventManager::IncreaseItemCount EHEObserverTypePlaceholder" );	        
	        break;
	        }	        
	    case EHEObserverTypeMMC:
	        {
	        WRITELOG( "CHarvesterEventManager::IncreaseItemCount() EHEObserverTypeMMC");
	        OstTrace0( TRACE_NORMAL, DUP1_CHARVESTEREVENTMANAGER_INCREASEITEMCOUNT, "CHarvesterEventManager::IncreaseItemCount EHEObserverTypeMMC" );	        
	        break;
	        }
	    case EHEObserverTypeOverall:
	        {
	        WRITELOG( "CHarvesterEventManager::IncreaseItemCount() EHEObserverTypeOverall");
	        OstTrace0( TRACE_NORMAL, DUP2_CHARVESTEREVENTMANAGER_INCREASEITEMCOUNT, "CHarvesterEventManager::IncreaseItemCount EHEObserverTypeOverall" );	        
	        break;
	        }
	    default:
	        {
	        WRITELOG( "CHarvesterEventManager::IncreaseItemCount() Unknown type!");
	        OstTrace0( TRACE_NORMAL, DUP3_CHARVESTEREVENTMANAGER_INCREASEITEMCOUNT, "CHarvesterEventManager::IncreaseItemCount Unknown type!" );
	        }
	    };
#endif
	
	if( eventStatus )
		{
		WRITELOG1( "CHarvesterEventManager::IncreaseItemCount() itemsleft = %d old ", eventStatus->iItemsLeft);
		OstTrace1( TRACE_NORMAL, DUP4_CHARVESTEREVENTMANAGER_INCREASEITEMCOUNT, "CHarvesterEventManager::IncreaseItemCount) itemsleft = %d old", eventStatus->iItemsLeft );
		
		eventStatus->iItemsLeft += aCount;
		WRITELOG1( "CHarvesterEventManager::IncreaseItemCount() itemsleft = %d ", eventStatus->iItemsLeft);
		OstTrace1( TRACE_NORMAL, DUP5_CHARVESTEREVENTMANAGER_INCREASEITEMCOUNT, "CHarvesterEventManager::IncreaseItemCount itemsleft = %d", eventStatus->iItemsLeft );
		
		}
	else
		{
		TEventStatus eventStatus;
		eventStatus.iCurrentState = EHEStateStarted;
		WRITELOG1( "CHarvesterEventManager::IncreaseItemCount() itemsleft = %d old", aCount);
		OstTrace1( TRACE_NORMAL, DUP6_CHARVESTEREVENTMANAGER_INCREASEITEMCOUNT, "CHarvesterEventManager::IncreaseItemCount itemsleft = %d old", aCount );
		
		eventStatus.iItemsLeft = aCount;
		WRITELOG1( "CHarvesterEventManager::IncreaseItemCount() itemsleft = %d ", aCount);
		OstTrace1( TRACE_NORMAL, DUP7_CHARVESTEREVENTMANAGER_INCREASEITEMCOUNT, "CHarvesterEventManager::IncreaseItemCount itemsleft = %d", aCount );
		
		eventStatus.iObserverType = aHEObserverType;
		iEventStatuses.Append( eventStatus );
		}
	}

EXPORT_C TBool CHarvesterEventManager::DecreaseItemCountL( 
		HarvesterEventObserverType aHEObserverType, TUint aCount )
	{
	TEventStatus* eventStatus = GetEventStatus( aHEObserverType );
	WRITELOG2( "CHarvesterEventManager::DecreaseItemCountL(%d) type = %d ", aCount, aHEObserverType);
	
#ifdef _DEBUG
    switch(aHEObserverType)
        {
        case EHEObserverTypePlaceholder:
            {
            WRITELOG( "CHarvesterEventManager::DecreaseItemCountL() EHEObserverTypePlaceholder");
            OstTrace0( TRACE_NORMAL, CHARVESTEREVENTMANAGER_DECREASEITEMCOUNTL, "CHarvesterEventManager::DecreaseItemCountL EHEObserverTypePlaceholder" );            
            break;
            }
        case EHEObserverTypeMMC:
            {
            WRITELOG( "CHarvesterEventManager::DecreaseItemCountL() EHEObserverTypeMMC");
            OstTrace0( TRACE_NORMAL, DUP1_CHARVESTEREVENTMANAGER_DECREASEITEMCOUNTL, "CHarvesterEventManager::DecreaseItemCountL EHEObserverTypeMMC" );            
            break;
            }
        case EHEObserverTypeOverall:
            {
            WRITELOG( "CHarvesterEventManager::DecreaseItemCountL() EHEObserverTypeOverall");
            OstTrace0( TRACE_NORMAL, DUP2_CHARVESTEREVENTMANAGER_DECREASEITEMCOUNTL, "CHarvesterEventManager::DecreaseItemCountL EHEObserverTypeOverall" );            
            break;
            }
        default:
            {
            WRITELOG( "CHarvesterEventManager::DecreaseItemCountL() Unknown type!");
            OstTrace0( TRACE_NORMAL, DUP3_CHARVESTEREVENTMANAGER_DECREASEITEMCOUNTL, "CHarvesterEventManager::DecreaseItemCountL Unknown type!" );
            }
        };
#endif
	
	if( eventStatus )
		{
		WRITELOG1( "CHarvesterEventManager::DecreaseItemCountL() iItemsLeft = %d old", eventStatus->iItemsLeft);
		OstTrace1( TRACE_NORMAL, DUP4_CHARVESTEREVENTMANAGER_DECREASEITEMCOUNTL, "CHarvesterEventManager::DecreaseItemCountL iItemsLeft = %d old", eventStatus->iItemsLeft );
		
		TUint newCount = eventStatus->iItemsLeft - aCount;

		// check for wrap
		if( newCount > eventStatus->iItemsLeft )
			{
			newCount = 0;
			}

		// event doesn't need to be sent, if count hasn't changed 
		if( newCount == eventStatus->iItemsLeft )
			{
			return EFalse;
			}

		eventStatus->iItemsLeft = newCount;
		}
	else
		{
		return EFalse;
		}

	WRITELOG1( "CHarvesterEventManager::DecreaseItemCountL() iItemsLeft = %d", eventStatus->iItemsLeft );
	OstTrace1( TRACE_NORMAL, DUP5_CHARVESTEREVENTMANAGER_DECREASEITEMCOUNTL, "CHarvesterEventManager::DecreaseItemCountL iItemsLeft = %d", eventStatus->iItemsLeft );
	
	// send finished event to all matching observers
	if ( eventStatus->iItemsLeft <= 0 )
		{
		eventStatus->iItemsLeft = 0;
		eventStatus->iCurrentState = EHEStateFinished;

		// EHEObserverTypeOverall state is handled directly in harvesterao during harvesting
        if( aHEObserverType != EHEObserverTypeOverall )
		    {
		    const TInt err = SendEventL( aHEObserverType, eventStatus->iCurrentState, eventStatus->iItemsLeft );
		    return err == KErrNone;
		    }
		}

	// still harvesting
	eventStatus->iCurrentState = EHEStateHarvesting;
	
	// send harvesting event to all matching observers
	TBool wasSent = EFalse;
	TInt count = iRegisteredObservers.Count();
	for ( TInt i = count; --i >= 0; )
		{
		THarvesterEventObserverInfo& observer = *(iRegisteredObservers[i]);

		if( CheckObserverType( observer.iObserverType, aHEObserverType ) )
			{
			observer.iDelta += aCount;

			// observers interval full
			if ( observer.iDelta >= observer.iNotificationInterval )
				{
				TInt err = SendSingleEvent( observer, aHEObserverType, 
						eventStatus->iCurrentState, eventStatus->iItemsLeft );
				if ( err == KErrNone )
					{
					wasSent = ETrue;
					}
				}
			}
		}

	return wasSent;
	}

TInt CHarvesterEventManager::SendSingleEvent( 
		THarvesterEventObserverInfo& aEventObserverInfo, 
		HarvesterEventObserverType aObserverType, 
		HarvesterEventState aEventState, TUint aItemsLeft )
	{
	aEventObserverInfo.iDelta = 0;

	THarvesterEventNotification notification;
	notification.iObserverId = aEventObserverInfo.iObserverId;
	notification.iObserverType = aObserverType;
	notification.iCurrentState = aEventState;
	notification.iItemsLeft = aItemsLeft;

	THarvesterEventQueue& eventQueue = *(aEventObserverInfo.iQueuePtr);
	TInt err = eventQueue.Send( notification );
	return err;
	}

EXPORT_C TUint CHarvesterEventManager::ItemCount( 
		HarvesterEventObserverType aHEObserverType )
	{
	TEventStatus* eventStatus = GetEventStatus( aHEObserverType );
	if( eventStatus )
		{
		return eventStatus->iItemsLeft;
		}
	
	return 0;
	}

EXPORT_C TInt CHarvesterEventManager::SendEventL( 
		HarvesterEventObserverType aHEObserverType,
		HarvesterEventState aHEState, TUint aItemsLeft )
	{
	TEventStatus* eventStatus = GetEventStatus( aHEObserverType );
	if( eventStatus )
		{
		eventStatus->iCurrentState = aHEState;
		}

	THarvesterEventNotification notification;
	notification.iObserverType = aHEObserverType;
	notification.iCurrentState = aHEState;
	notification.iItemsLeft = aItemsLeft;

	TInt err = KErrNone;
	TInt count = iRegisteredObservers.Count();
	for ( TInt i = count; --i >= 0; )
		{
		THarvesterEventObserverInfo& observer = *(iRegisteredObservers[i]);

		if( CheckObserverType( observer.iObserverType, aHEObserverType ) )
			{
			notification.iObserverId = observer.iObserverId;

			THarvesterEventQueue& eventQueue = *(observer.iQueuePtr);
			TInt sendErr = eventQueue.Send( notification );
			// Take first error
			if( err == KErrNone )
				{
				err = sendErr;
				}
			}
		}
	return err;
	}

EXPORT_C void CHarvesterEventManager::RegisterEventObserverL( const RMessage2& aMessage )
	{
	THarvesterEventObserverInfo* observerInfo = new (ELeave) THarvesterEventObserverInfo;
	CleanupStack::PushL( observerInfo );

    TPckg<THarvesterEventObserverInfo> pckgObserverInfo( *observerInfo );
    TInt err = aMessage.Read( 0, pckgObserverInfo );

    // Init server side values
    observerInfo->iQueuePtr = NULL;
    observerInfo->iDelta = 0;
    observerInfo->iProcessUid = aMessage.Identity().iUid;

    // Check if observer ID is not yet used
    // and if event queue already exists
    TBool found = EFalse;
	for(TInt i = iRegisteredObservers.Count(); --i >= 0;)
		{
		THarvesterEventObserverInfo* info = iRegisteredObservers[i];
		if( info->iProcessUid == observerInfo->iProcessUid && 
			info->iQueueHandle == observerInfo->iQueueHandle )
			{
			if( info->iObserverId == observerInfo->iObserverId )
				{
				User::Leave( KErrAlreadyExists );
				}

			observerInfo->iQueuePtr = info->iQueuePtr;
			found = ETrue;
			}
		}

	// create new event queue if no match was found 
	if ( !found )
		{
		THarvesterEventQueue* msgQueue = new (ELeave) THarvesterEventQueue;
		CleanupStack::PushL( msgQueue );

		User::LeaveIfError( msgQueue->Open( aMessage, 1 ) );

		User::LeaveIfError( iEventQueues.InsertInOrderAllowRepeats(msgQueue, 
		                                 TLinearOrder<THarvesterEventQueue>(CHarvesterEventManager::CompareProperties))); 
		observerInfo->iQueuePtr = msgQueue;
		
		CleanupStack::Pop( msgQueue );
		}	

	iRegisteredObservers.AppendL( observerInfo );

	// send event if register is coming in the middle of harvesting
	for( TInt i = iEventStatuses.Count(); --i >= 0; )
		{
		TEventStatus& eventStatus = iEventStatuses[i];
		if( CheckObserverType( observerInfo->iObserverType, 
				eventStatus.iObserverType) )
			{
			TRAP_IGNORE( SendEventL( eventStatus.iObserverType, 
					eventStatus.iCurrentState, eventStatus.iItemsLeft ) );
			}
		}
	
	//no events in queue, signal registered client anyways 
	if( !iEventStatuses.Count() )
	    {
        if(observerInfo->iObserverType & EHEObserverTypeOverall)
            {
            SendSingleEvent(*observerInfo, EHEObserverTypeOverall, EHEStateUninitialized, 0);
            }
        
        if(observerInfo->iObserverType & EHEObserverTypeMMC)
            {
            SendSingleEvent(*observerInfo, EHEObserverTypeMMC, EHEStateUninitialized, 0);
            }
        
        if(observerInfo->iObserverType & EHEObserverTypePlaceholder)
            {
            SendSingleEvent(*observerInfo, EHEObserverTypePlaceholder, EHEStateUninitialized, 0);
            }
	    }
	
	CleanupStack::Pop( observerInfo );
	}

EXPORT_C TInt CHarvesterEventManager::UnregisterEventObserver( const RMessage2& aMessage )
	{
	TUint observerId = (TUint)aMessage.Int0();
	TInt queueHandle = aMessage.Int1();

	TInt serverQueueHandle = KNullHandle;
	TUint processUid = aMessage.Identity().iUid;
	
	TBool otherObserverFound = EFalse;
	for(TInt i = iRegisteredObservers.Count(); --i >= 0;)
		{
		THarvesterEventObserverInfo* observer = iRegisteredObservers[i];
		if( observer->iProcessUid == processUid && 
			observer->iQueueHandle == queueHandle )
			{
			// Remove registered observer
			if( observer->iObserverId == observerId )
				{
				serverQueueHandle = observer->iQueuePtr->Handle();
				
				iRegisteredObservers.Remove( i );
				delete observer;
				observer = NULL;
				}
			// Find if any other observer is using the same queue
			else
				{
				otherObserverFound = ETrue;
				}
			}
		}

	if( serverQueueHandle )
		{
		// Remove the queue if removed observer 
		// was the last observer which used it
		if( !otherObserverFound )
			{
	        TInt low( 0 );
	        TInt high( iEventQueues.Count() );
	    
	        while( low < high )
	            {
	            TInt mid( (low+high)>>1 );
	        
	            const TInt compare( serverQueueHandle - iEventQueues[mid]->Handle() );
	            if( compare == 0 )
	                {
	                THarvesterEventQueue* queue = iEventQueues[mid];
                    iEventQueues.Remove( mid );
                    queue->Close();
                    delete queue;
                    queue = NULL;
                    break;
	                }
	            else if( compare > 0 )
	                {
	                low = mid + 1;
	                }
	            else
	                {
	                high = mid;
	                }
	            }
			iEventQueues.Compress();
			}

		iRegisteredObservers.Compress();
		return KErrNone;
		}
	else
		{
		return KErrNotFound;
		}
	}

EXPORT_C HarvesterEventState CHarvesterEventManager::CurrentState( 
		HarvesterEventObserverType aHEObserverType )
	{
	TInt count = iEventStatuses.Count();
	for( TInt i = count; --i >= 0; )
		{
		TEventStatus& eventStatus = iEventStatuses[i];
		if( eventStatus.iObserverType == aHEObserverType )
			{
			return eventStatus.iCurrentState;
			}
		}
	return EHEStateUninitialized;
	}

TBool CHarvesterEventManager::CheckObserverType( TInt aObserverType, TInt aEventType )
	{
	return aObserverType & aEventType;
	}

CHarvesterEventManager::TEventStatus* CHarvesterEventManager::GetEventStatus( 
		HarvesterEventObserverType aHEObserverType )
	{
	TInt count = iEventStatuses.Count();
	for(TInt i = count; --i >= 0; )
		{
		TEventStatus& eventStatus = iEventStatuses[i];
		if( eventStatus.iObserverType == aHEObserverType )
			{
			return &eventStatus;
			}
		}

	return NULL;
	}

EXPORT_C TUint CHarvesterEventManager::GetLastClientId()
	{
	// deprecated method, just return something
	return 0;
	}

TInt CHarvesterEventManager::CompareProperties(const THarvesterEventQueue& aFirst, const THarvesterEventQueue& aSecond)
    {
    return aFirst.Handle() - aSecond.Handle();
    }

