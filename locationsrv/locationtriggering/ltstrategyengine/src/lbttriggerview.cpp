/*
* Copyright (c) 2006, 2007 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  Implementation of Trigger View
*
*/



#include <lbtlisttriggeroptions.h>

#include "lbtlogger.h"
#include "lbttriggerview.h"
#include "lbttriggerconditionarea.h"
#include "lbtgeohybrid.h"


// ======== MEMBER FUNCTIONS ========

// -----------------------------------------------------------------------------
// CLbtTriggerView::NewL
// 
// -----------------------------------------------------------------------------
//
CLbtTriggerView* CLbtTriggerView::NewL( MLbtTriggerViewObserver* aObserver )
    {    
    FUNC_ENTER("CLbtTriggerView::NewL");
    
    CLbtTriggerView* self = new ( ELeave ) CLbtTriggerView;
    CleanupStack::PushL( self );
    self->ConstructL( aObserver );
    CleanupStack::Pop();
    return self;
    
    }


// -----------------------------------------------------------------------------
// CLbtTriggerView::CheckRefreshRequests
// 
// -----------------------------------------------------------------------------
//
void CLbtTriggerView::CheckRefreshRequests()
    {
    FUNC_ENTER("CLbtTriggerView::CheckRefreshRequests");
    if( iRefRequestList.Count() > 0 )
        {
        iState = ERefreshView;
        SetEvent();
        }
    else
        {
        // switch back to idle state
        iState = EIdle;
        }
    }
    
// -----------------------------------------------------------------------------
// CLbtTriggerView::Refresh
// 
// -----------------------------------------------------------------------------
//
void CLbtTriggerView::Refresh( TPositionInfo& aPosInfo, 
                               TLbtTriggerEventMask aEventMask,
                               RArray<TLbtTriggerId>& aTriggerID )
    {
    FUNC_ENTER("CLbtTriggerView::Refresh");
    
        // Append refresh requests to the queue to be processed by trigger view
        if( aTriggerID.Count() != 0 )
            {            
            TRefRequestParam req;
            req.posInfo = aPosInfo;
            req.eventMask = aEventMask;
            
            RArray< TLbtTriggerId >* triggerID = NULL;
            TRAPD( err,  triggerID = 
                new( ELeave ) RArray< TLbtTriggerId > );
            if( KErrNone == err )
                {
                for( TInt i = 0; i < aTriggerID.Count(); i++ )
                    {
                    triggerID->Append( aTriggerID[i] );
                    }
                req.triggerID = triggerID;
                }
            else
                {
                // Memory allocation failure
                LOG1("Mem Error: %d", err);
                // ToDo: Initiate termination
                return;
                }

            err = iRefRequestList.Append( req );
            if( err != KErrNone )
                {
                LOG1("Failed to append req to the array:%d",err);
                return;
                }
            }
            
        LOG1( "View State is %d", iState );
        // Initiate processing of request queue only if the current state is
        // idle. Processing of subsequent requests is automatically handled 
        // by the ProcessRefreshRequests() method when state is ERefreshView
        if( EIdle == iState )
            {
            LOG("Resetting modified trigger list");
            iModifiedTrgList.Reset();
            iState = ERefreshView;
            SetEvent();
            }
    }


// -----------------------------------------------------------------------------
// CLbtTriggerView::GetModifiedTriggers
// 
// -----------------------------------------------------------------------------
//
void CLbtTriggerView::GetModifiedTriggers( RArray< TLbtTriggerId >& aList )
    {
    FUNC_ENTER("CLbtTriggerView::GetModifiedTriggers");
    
    for( TInt index = 0; index < iModifiedTrgList.Count(); index++ )
        {
        TInt error = aList.Append( iModifiedTrgList[index] );
        if( error != KErrNone )
            {
            LOG1("Failed to append trigger id:%d",error);
            return;
            }
        }
    
    // This is done here because strategy engine invokes this method and evaluates all modified triggers
    iModifiedTrgList.Reset();
    
    }


// -----------------------------------------------------------------------------
// CLbtTriggerView::GetTriggerByID
// 
// -----------------------------------------------------------------------------
//
TBool CLbtTriggerView::GetTriggerByID( TLbtTriggerId aID, 
                                     CLbtContainerTriggerEntry*& aTriggerInfo )
    {
    FUNC_ENTER("CLbtTriggerView::GetTriggerByID");
    
    TBtreePos pos;
    TBool found = EFalse;
    TRAPD( err, 
        found = iTriggerList->FindL( pos, aID ); 
        if( found )
            {
            TTrigger tEntry;
            iTriggerList->ExtractAtL( pos, tEntry );
            aTriggerInfo = tEntry.info;
            } 
    );
    if( KErrNone != err )
        {
        found = EFalse;
        }
     
    return found;
    
    }
    

// -----------------------------------------------------------------------------
// CLbtTriggerView::IsEmpty
// 
// -----------------------------------------------------------------------------
//
TBool CLbtTriggerView::IsEmpty()
    {
    return iTriggerList->IsEmpty();
    }

       
// -----------------------------------------------------------------------------
// CLbtTriggerView::SetContainerObserver
// 
// -----------------------------------------------------------------------------
//
void CLbtTriggerView::SetContainerObserver( 
                                MLbtContainerChangeEventObserver* aObserver )
    {
    FUNC_ENTER("CLbtTriggerView::SetContainerObserver");
    
    // Set observer for trigger change notifications from container
    TLbtTriggerEventMask event = CLbtContainer::ELbtConTriggerEnabledAndValidCreated | 
		                         CLbtContainer::ELbtConTriggerDeleted |
		                         CLbtContainer::ELbtConTriggerStateFieldChanged |
		                         CLbtContainer::ELbtConTriggerConditionFieldChanged |
		                         CLbtContainer::ELbtConTriggerValidityFieldChanged;

    iTrgContainer->SetChangeObserver( aObserver, event );
    iContainerObserver = aObserver;    
    }


// -----------------------------------------------------------------------------
// CLbtTriggerView::SetTriggerStatusObserver
// 
// -----------------------------------------------------------------------------
//
void CLbtTriggerView::SetTriggerStatusObserver( 
                                MLbtTriggerStatusObsrvr* aObserver )
    {
    FUNC_ENTER("CLbtTriggerView::SetTriggerStatusObserver");
    
    iTriggerStatusObserver = aObserver;
    
    }


// -----------------------------------------------------------------------------
// CLbtTriggerView::UpdateTriggerInfo
// 
// -----------------------------------------------------------------------------
//
void CLbtTriggerView::UpdateTriggerInfo( CLbtGeoAreaBase::TGeoAreaType aAreaType,
                                         TPositionInfo aPosInfo,
                                         CLbtContainerTriggerEntry* aTrigger, 
                                         TLbtTriggerDataMask aDataMask )
    {
    FUNC_ENTER("CLbtTriggerView::UpdateTriggerInfo");
    
    TUpdateRequestParam reqParam;
    reqParam.areaType = aAreaType;
    reqParam.posInfo = aPosInfo;
    reqParam.trigger = aTrigger;
    reqParam.dataMask = aDataMask;
    TInt error = iUpdateRequestList.Append( reqParam );
    if( error != KErrNone )
        {
        LOG1("Failed to appenf reqParam:%d",error);
        }
    if( EIdle == iState )
        {
        iState = ERefreshContainer;
        SetEvent();
        }
        
    }


// -----------------------------------------------------------------------------
// CLbtTriggerView::RemContainerObserver
// 
// -----------------------------------------------------------------------------
//
void CLbtTriggerView::DetermineNearestTriggerChanges( RArray< TNearestTrigger >* aNearestTriggers )
    {
    FUNC_ENTER("CLbtTriggerView::DetermineNearestTriggerChanges");
    TLinearOrder< TLbtTriggerId > byTriggerID( CompareByID );
    TInt pos = 0;
    TInt count = aNearestTriggers->Count();
    for( TInt index = count - 1; index >= 0; index-- )
        {
        pos = iModifiedTrgList.FindInOrder( ( *aNearestTriggers )[0].id, byTriggerID );
        if( KErrNotFound != pos )
            {
            aNearestTriggers->Remove( index );
            }
        }
    }

    
// -----------------------------------------------------------------------------
// CLbtTriggerView::RemContainerObserver
// 
// -----------------------------------------------------------------------------
//
void CLbtTriggerView::RemContainerObserver()
    {
    FUNC_ENTER("CLbtTriggerView::RemContainerObserver");
    
    iTrgContainer->RemoveObserver( iContainerObserver );
    
    }


// -----------------------------------------------------------------------------
// CLbtTriggerView::InvalidateTrigger
// 
// -----------------------------------------------------------------------------
//
void CLbtTriggerView::InvalidateTrigger( TLbtTriggerId aKey )
    {
    FUNC_ENTER("CLbtTriggerView::InvalidateTrigger");
    
    TBtreePos pos;
    TBool found = EFalse;
    TRAPD( err, 
        found = iTriggerList->FindL( pos, aKey );
        if( found )
            {
            TTrigger tEntry;
            iTriggerList->ExtractAtL( pos, tEntry );
            tEntry.validity = EFalse;
            
            // Remove entry from tree and re-insert the modified entry
            iTriggerList->DeleteL( aKey );
            iTriggerList->InsertL( pos, tEntry );
            } 
    );
    if( KErrNone != err )
        {
        LOG1("Invalidation of trigger %d failed",aKey );
        LOG1("Reason: %d", err);
        }
    else
        {
        LOG1("Trigger %d will no longer be supervised as it is invalidated", aKey);
        }
    }


// -----------------------------------------------------------------------------
// CLbtTriggerView::BeginL
// 
// -----------------------------------------------------------------------------
//
CLbtTriggerView::TTriggerViewIter CLbtTriggerView::BeginL(CLbtGeoAreaBase::TGeoAreaType aPrimaryTriggerArea)
    {
    FUNC_ENTER("CLbtTriggerView::BeginL");
    
     TTriggerViewIter iter = TTriggerViewIter( iTriggerList,aPrimaryTriggerArea );
     TInt err = iter.Reset();
     if( KErrNone != err && KErrNotFound != err )
        {
        User::Leave( err );
        }
     return iter;
     
    }

// -----------------------------------------------------------------------------
// CLbtTriggerView::BeginL
// 
// -----------------------------------------------------------------------------
//
CLbtTriggerView::TTriggerViewIter CLbtTriggerView::BeginL(CLbtGeoAreaBase::TGeoAreaType aPrimaryTriggerArea,
                                                          CLbtGeoAreaBase::TGeoAreaType aSecondaryTriggerArea)
    {
    FUNC_ENTER("CLbtTriggerView::BeginL");
    
     TTriggerViewIter iter = TTriggerViewIter( iTriggerList,aPrimaryTriggerArea,
                                               aSecondaryTriggerArea);
     TInt err = iter.Reset();
     if( KErrNone != err && KErrNotFound != err )
        {
        User::Leave( err );
        }
     return iter;
     
    }


// -----------------------------------------------------------------------------
// CLbtTriggerView::Begin
// 
// -----------------------------------------------------------------------------
//
CLbtTriggerView::TTriggerViewIter CLbtTriggerView::Begin( 
                                                RArray< TLbtTriggerId >* aID,
                                                CLbtGeoAreaBase::TGeoAreaType aPrimaryTriggerArea)
    {
    FUNC_ENTER("CLbtTriggerView::CLbtTriggerView::Begin");
    
    TTriggerViewIter iter = TTriggerViewIter( iTriggerList, aID ,aPrimaryTriggerArea );
    ( void )iter.Reset();
    return iter;
    
    }
    
    
// -----------------------------------------------------------------------------
// CLbtTriggerView::~CLbtTriggerView
// 
// -----------------------------------------------------------------------------
//
CLbtTriggerView::~CLbtTriggerView()
    {
    FUNC_ENTER("CLbtTriggerView::~CLbtTriggerView");
    
    TBtreeMark index;
    TBool next = EFalse;
    
    if (IsActive())
        {
        Cancel();
        }
    
    if( NULL != iTriggerList )
        {
        TRAPD( err, 
        next = iTriggerList->ResetL( index );
        TTrigger tEntry;
        while( next ) 
            {
            iTriggerList->ExtractAtL( index, tEntry );
            delete tEntry.info;
            iTriggerList->DeleteL( tEntry.key );
            next = iTriggerList->ResetL( index );
            } );
        if( KErrNone != err )
            {
            LOG("Entries could not be deleted");
            }
        }

    iList.ResetAndDestroy();
    delete iTriggerList;
    delete iPool;
    if( NULL != iTrgContainer )
        {
        iTrgContainer->Destroy();
        }
    iModifiedTrgList.Close();
    iUpdateRequestList.Close();
    
    for(TInt i=0;i<iRefRequestList.Count();++i)
    	{
    	if( iRefRequestList[i].triggerID != NULL )
    		{
    		iRefRequestList[i].triggerID->Close();
    		delete (iRefRequestList[i].triggerID);
    		}    	
    	}
    iRefRequestList.Close();
    }
    

// -----------------------------------------------------------------------------
// CLbtTriggerView::TTriggerViewIter::TTriggerViewIter
// 
// -----------------------------------------------------------------------------
//
CLbtTriggerView::TTriggerViewIter::TTriggerViewIter()
    {
    
    }


// -----------------------------------------------------------------------------
// CLbtTriggerView::TTriggerViewIter::TTriggerViewIter
// 
// -----------------------------------------------------------------------------
//
CLbtTriggerView::TTriggerViewIter::TTriggerViewIter( 
                           TBtreeFix< TTrigger, TLbtTriggerId >* aTriggerList,
                           CLbtGeoAreaBase::TGeoAreaType aPrimaryTriggerArea )
    {
    FUNC_ENTER("CLbtTriggerView::TTriggerViewIter");
    iTrgArray = NULL;
    iLast = ETrue;    
    iTree = aTriggerList;
    iPrimaryAreaType = aPrimaryTriggerArea;
    }

// -----------------------------------------------------------------------------
// CLbtTriggerView::TTriggerViewIter::TTriggerViewIter
// 
// -----------------------------------------------------------------------------
//
CLbtTriggerView::TTriggerViewIter::TTriggerViewIter( 
                           TBtreeFix< TTrigger, TLbtTriggerId >* aTriggerList,
                           CLbtGeoAreaBase::TGeoAreaType aPrimaryTriggerArea,
                           CLbtGeoAreaBase::TGeoAreaType aSecondaryTriggerArea)
    {
    FUNC_ENTER("CLbtTriggerView::TTriggerViewIter");
    iTrgArray = NULL;
    iLast = ETrue;    
    iTree = aTriggerList;
    iPrimaryAreaType = aPrimaryTriggerArea;
    iSecondaryAreaType = aSecondaryTriggerArea;
    }


// -----------------------------------------------------------------------------
// CLbtTriggerView::TTriggerViewIter::TTriggerViewIter
// 
// -----------------------------------------------------------------------------
//
CLbtTriggerView::TTriggerViewIter::TTriggerViewIter( 
                                TBtreeFix< TTrigger, TLbtTriggerId >* aTriggerList,
                                RArray< TLbtTriggerId >* aTriggerIDs,
                                CLbtGeoAreaBase::TGeoAreaType aPrimaryTriggerArea )
    {
    FUNC_ENTER("CLbtTriggerView::TTriggerViewIter");
    iTrgArray = aTriggerIDs;
    iLast = ETrue;    
    iTree = aTriggerList; 
    iPrimaryAreaType = aPrimaryTriggerArea;   
    }

// -----------------------------------------------------------------------------
// CLbtTriggerView::TTriggerViewIter::TTriggerViewIter
// 
// -----------------------------------------------------------------------------
//
CLbtTriggerView::TTriggerViewIter::TTriggerViewIter( 
                                TBtreeFix< TTrigger, TLbtTriggerId >* aTriggerList,
                                RArray< TLbtTriggerId >* aTriggerIDs,
                                CLbtGeoAreaBase::TGeoAreaType aPrimaryTriggerArea,
                                CLbtGeoAreaBase::TGeoAreaType aSecondaryTriggerArea)
    {
    FUNC_ENTER("CLbtTriggerView::TTriggerViewIter");
    iTrgArray = aTriggerIDs;
    iLast = ETrue;    
    iTree = aTriggerList; 
    iPrimaryAreaType = aPrimaryTriggerArea;   
    iSecondaryAreaType = aSecondaryTriggerArea;   
    }

// -----------------------------------------------------------------------------
// CLbtTriggerView::TTriggerViewIter::ResetL
// 
// -----------------------------------------------------------------------------
//
TInt CLbtTriggerView::TTriggerViewIter::Reset()
    {
    FUNC_ENTER("CLbtTriggerView::Reset");
    
    if( iTrgArray && iTrgArray->Count() )
        {
        iIndex = 0;
        }
    else
        {
        TBool exist = EFalse;
        TRAPD( err, exist = iTree->ResetL( iPos ) );
        if( KErrNone != err )
            {
            return err;
            }
        else if( !exist )
            {
            return KErrNotFound;
            }
        }
    iLast = EFalse;
    return KErrNone;
    
    }


// -----------------------------------------------------------------------------
// CLbtTriggerView::TTriggerViewIter::NextL
// 
// -----------------------------------------------------------------------------
//
 TBool CLbtTriggerView::TTriggerViewIter::NextL( 
                                     CLbtContainerTriggerEntry*& aTriggerInfo )
    {
    FUNC_ENTER("CLbtTriggerView::NextL");
    TInt err;
    TInt count = 0;
    if( iTrgArray )
        {
        count = iTrgArray->Count();
        }
    
    // This section of code is for triggers whose IDs are specified. This 
    // section is executed only when there are trigger changes ( addition /
    // modification / deletion ) in the system
    if( count )
        {
        while( iIndex < count )
            {
            TBtreePos pos;
            TBool found = EFalse;
            
            TRAP( err,
                found = iTree->FindL( pos, ( *iTrgArray )[iIndex] ) );
            if( KErrNone == err )
                {
                if( found )
                    {
                    CLbtTriggerView::TTrigger tEntry;
                    TRAP( err, iTree->ExtractAtL( pos, tEntry ) );
                    if( KErrNone == err && tEntry.validity  && 
                        ( tEntry.type == iPrimaryAreaType || tEntry.type == iSecondaryAreaType ) )
                        {
                        aTriggerInfo = tEntry.info;
                        ++iIndex;
                        return ETrue;
                        }
                    }
                }
            LOG1("TTriggerViewIter::NextL - trigger %d not found in repository",( *iTrgArray )[iIndex]);    
            // This step only ignores missing trigger information and proceeds 
            // with the next entry
            ++iIndex;
            continue;
            }   // end of while
            
        return EFalse;
        }

    // This section is for all triggers in the BTree
    else
        {
        while( ETrue )
            {
            if( !iLast )
                {
                CLbtTriggerView::TTrigger tEntry;
                iTree->ExtractAtL( iPos, tEntry );
                if( tEntry.validity && 
                    ( tEntry.type == iPrimaryAreaType || tEntry.type == iSecondaryAreaType ) )
                    {
                    aTriggerInfo = tEntry.info;
                    }
                else
                    {
                    if( iTree->NextL( iPos ) )
                        {
                        continue;
                        }
                    else
                        {
                        aTriggerInfo = NULL;
                        iLast = ETrue;
                        return !iLast;
                        }
                    }
                    
                if( iTree->NextL( iPos ) )
                    {
                    return !iLast;
                    }
                else
                    {
                    iLast = ETrue;
                    return iLast;
                    }    
                }
            else
                {
                return !iLast;
                }
            }
        }
    }


// -----------------------------------------------------------------------------
// CLbtTriggerView::CLbtTriggerView
// 
// -----------------------------------------------------------------------------
//
CLbtTriggerView::CLbtTriggerView() : 
            CActive( EPriorityStandard )
    {
    CActiveScheduler::Add( this );
    }


// -----------------------------------------------------------------------------
// CLbtTriggerView::ConstructL
// 
// -----------------------------------------------------------------------------
//
void CLbtTriggerView::ConstructL( MLbtTriggerViewObserver* aObserver )
    {
    FUNC_ENTER("CLbtTriggerView::ConstructL");
    iObserver = aObserver;
    iRefReqCounter = 0;
    iUpdateReqCounter = 0;
    iCellTriggerCount = 0;
    iCoordTriggerCount = 0;
    iRequestInitiated = EFalse;
    iDelayedInitialization = EFalse;
    
    
    iTriggerList = new ( ELeave ) 
                            TBtreeFix< TTrigger, TLbtTriggerId >( EBtreeFast );
    iPool = CMemPagePool::NewL();
    iTriggerList->Connect( iPool, &iKey );
    
    iTrgContainer = CLbtContainer::NewL();
    iState = EInitialize;
    // Retrieve triggers from container
    ListEnabledAndValidTriggersL();
    }


// -----------------------------------------------------------------------------
// CLbtTriggerView::InitializeList
// 
// -----------------------------------------------------------------------------
//
void CLbtTriggerView::InitializeList()
    {
    FUNC_ENTER("CLbtTriggerView::InitializeList");
    
    TInt err;
    TInt count = iList.Count();
    for( TInt index = count - 1; index >= 0; index-- )
        {
        TRAP( err, AddToTreeL( ( iList[index] )->TriggerEntry()->Id(), 
                                                    iList[index] ) );
        if( KErrNone != err )
            {
            LOG1("Initializing trigger repository failed. Reason: %d", err );
            iObserver->OperationComplete( 
                                MLbtTriggerViewObserver::EOperationError );
            iList.ResetAndDestroy();
            return;
            }
            
        iList.Remove( index );
        }

    // Reset the trigger list as this would be used later to retrieve 
    // trigger information from container during refresh operations
    iList.Reset();
    
    // Check if there have been any refresh requests queued while view was
    // being initialized. Process these requests and delay notification to
    // strategy engine.
    if( iRefRequestList.Count() )
        {
        iDelayedInitialization = ETrue;
        iState = ERefreshView;
        SetEvent();
        }
    else
        {
        iState = EIdle;
        iObserver->OperationComplete( MLbtTriggerViewObserver::EViewInitialized );
        }
    }
    
    
// -----------------------------------------------------------------------------
// CLbtTriggerView::ProcessRefreshRequests
// 
// -----------------------------------------------------------------------------
//
void CLbtTriggerView::ProcessRefreshRequests()
    {
    FUNC_ENTER("CLbtTriggerView::ProcessRefreshRequests");
    
	// TODO: Implement relevance here! Currently the list contains
	//       all enabled and valid triggers in the system
    while( iRefRequestList.Count() > 0 )
        {
        TRefRequestParam req = iRefRequestList[iRefReqCounter];
    	if( !req.eventMask )
    	    {
    	    // This is a special case when strategy engine requests refresh of 
    	    // trigger view based on position info
    	    // Current implementation only removes request from queue and 
    	    // notifies strategy engine
    	    req.triggerID->Close();
    	    delete (req.triggerID);
            iRefRequestList.Remove( iRefReqCounter );
            
            LOG("Event mask absent for process refresh requests");
            continue;
    	    }
        
        if( req.eventMask & CLbtContainer::ELbtConTriggerDeleted )
            {
            LOG("Event Mask : Trigger Deleted");
            // Trigger(s) deleted - remove entry from tree
            iRefRequestList.Remove( iRefReqCounter );
            HandleTriggerDeletionEvent( req.triggerID );
            ( req.triggerID )->Reset();
            delete (req.triggerID);
            }
        else
            {
            // Trigger(s) created or modified
            if( !iRequestInitiated && iList.Count() == 0 )
                {
                iRequestInitiated = ETrue;
                // Retrieve modified triggers from container
                TRAPD( err, GetTriggers( req.triggerID ) );
                if( KErrNone != err )
                    {
                    iRefRequestList.Remove( iRefReqCounter );
                    // Invalidate triggers that have been modified
                    if( !( req.eventMask & CLbtContainer::ELbtConTriggerEnabledAndValidCreated ) &&
                        !( req.eventMask & CLbtContainer:: ELbtConTriggerDeleted) )
                        {
                        TInt index = 0;
                        while ( index < ( req.triggerID )->Count() )
                            {
                            InvalidateTrigger( ( *req.triggerID )[index] );
                            index++;
                            }
                        }
                    ( req.triggerID )->Reset();
                    delete req.triggerID;
                    
                    continue;
                    }
                    
                break;
                }
            else
                {
                iRequestInitiated = EFalse;
                
                // Triggers with the specified IDs were not found in container
                if( iList.Count() == 0 )
                    {
                    LOG("Zombie Triggers!!");
                    iRefRequestList.Remove( iRefReqCounter );
                    ( req.triggerID )->Reset();
                    delete req.triggerID;
                    }
                    
                // Update tree with triggers retrieved from container
                else if( req.eventMask & CLbtContainer::ELbtConTriggerEnabledAndValidCreated )
                    {
                    LOG("Event Mask : Trigger Created");
                    // Add new triggers to view
                    iRefRequestList.Remove( iRefReqCounter );
                    HandleTriggerCreationEvent();
                    ( req.triggerID )->Reset();
                    delete req.triggerID;
                    }  
                else
                    {
                    LOG("Event Mask : Trigger Modified");
                    // Check for trigger modifications
                    iRefRequestList.Remove( iRefReqCounter );
                    HandleTriggerModificationEvent();
                    ( req.triggerID )->Reset();
                    delete req.triggerID;
                    }
                }
            }
            
        continue;
        }// end while
        
    if( iRefRequestList.Count() <= 0 )
        {
        LOG("End of ProcessRefreshReqs");

        // Reset request list to be reused
        iRefRequestList.Reset();
        iState = EIdle;
        
        // If a result of delayed initialization caused by triggers being created
        // before the initialization process is complete, notify initialization
        // operation complete now.
        if( iDelayedInitialization )
            {
            iDelayedInitialization = EFalse;
            iObserver->OperationComplete( MLbtTriggerViewObserver::EViewInitialized );
            }
        else
            {
            iObserver->OperationComplete( MLbtTriggerViewObserver::EViewRefreshed );
            }
        
        
        // Check if there are any update requests pending. If so, switch to
        // processing of updates
        if( iUpdateRequestList.Count() > 0 )
            {
            LOG("ERefreshContainer");
            iState = ERefreshContainer;
            SetEvent();
            }
        else
            {
            iState = EIdle;
            }
        }
    }


// -----------------------------------------------------------------------------
// CLbtTriggerView::ProcessUpdateRequests
// 
// -----------------------------------------------------------------------------
//
void CLbtTriggerView::ProcessUpdateRequests()
    {
    FUNC_ENTER("CLbtTriggerView::ProcessUpdateRequests");
    
    if( iUpdateReqCounter )
        {
        if( KErrNone != iStatus.Int() )
            {
            LOG1("Updating information of trigger %d in the container failed.", 
                    iUpdateRequestList[iUpdateReqCounter - 1].trigger->TriggerEntry()->Id());
            LOG1("Reason: %d", iStatus.Int() );      
            // Error in updating container
            }
        else if( iUpdateRequestList[iUpdateReqCounter - 1].dataMask & 
                    CLbtContainerTriggerEntry::EContainerAttributeIsFired )
            {
            iTriggerStatusObserver->TriggerFired( 
            iUpdateRequestList[iUpdateReqCounter - 1].areaType,
            iUpdateRequestList[iUpdateReqCounter - 1].trigger->TriggerEntry()->Id(), 
            iUpdateRequestList[iUpdateReqCounter - 1].posInfo );
            }
        }

    if( iUpdateReqCounter < iUpdateRequestList.Count() )
        {
        // Update trigger information in container. 
        // TODO: May need to revisit this TRAP_IGNORE part if container decides 
        // to leave. Current implementation, there aint no leave!
        TRAP_IGNORE( iTrgContainer->UpdateTriggerL( 
                            *( iUpdateRequestList[iUpdateReqCounter].trigger ),
                            iUpdateRequestList[iUpdateReqCounter].dataMask, 0, 
                            iRequestID, iStatus ) );
        SetActive();
        iUpdateReqCounter++;
        }
    else
        {
        // Reset request list to be reused
        iUpdateRequestList.Reset();
        iUpdateReqCounter = 0;
        
        // Check for any refresh requests from container
        CheckRefreshRequests();
        }
    }


// -----------------------------------------------------------------------------
// CLbtTriggerView::HandleTriggerCreationEvent
// 
// -----------------------------------------------------------------------------
//
void CLbtTriggerView::HandleTriggerCreationEvent()
    {
    FUNC_ENTER("CLbtTriggerView::HandleTriggerCreationEvent");
    TInt count = iList.Count();
    for( TInt index = count - 1; index >= 0; index-- )
        {
        TRAPD( err, AddToTreeL( ( iList[index] )->TriggerEntry()->Id(),  
                                                    iList[index] ) );
        if( KErrNone == err )
            {
            LOG1("Added Trigger to tree: %d", ( iList[index] )->TriggerEntry()->Id() );
            TLinearOrder< TLbtTriggerId > byTriggerID( CompareByID );
            iModifiedTrgList.InsertInOrder( ( iList[index] )->TriggerEntry()->Id(), byTriggerID );
            }
        else
            {
            LOG1("Including trigger %d in view failed.",( iList[index] )->TriggerEntry()->Id());
            LOG1("Reason:%d",err);
            continue;
            }
        
        iList.Remove( index );
        }
        
    // Reset list to be reused for subsequent refresh operations
    iList.ResetAndDestroy();
    }

    
// -----------------------------------------------------------------------------
// CLbtTriggerView::HandleTriggerModificationEvent
// 
// -----------------------------------------------------------------------------
//
void CLbtTriggerView::HandleTriggerModificationEvent()
    {
    FUNC_ENTER("CLbtTriggerView::HandleTriggerModificationEvent");
    TInt count = iList.Count();
    TBool found = EFalse;
    for( TInt index = count - 1; index >= 0; index-- )
        {
        // Check for triggers that have been rendered invalid or disabled and 
        // remove them from view
        if( ( iList[index] )->TriggerEntry()->State() == 
                            CLbtTriggerEntry::EStateDisabled || 
            ( iList[index] )->DynInfo()->iValidity == 
                            TLbtTriggerDynamicInfo::EInvalid )
            {
            TLbtTriggerId id = ( iList[index] )->TriggerEntry()->Id();
            
            LOG1("Trigger deleted / set invalid: %d", id );
            TRAPD( err, found = DeleteFromTreeL( id ) );
            if( found )
                {
                if( KErrNone == err )
                    {
                    TLinearOrder< TLbtTriggerId > byTriggerID( CompareByID );
                    iModifiedTrgList.InsertInOrder( id, byTriggerID );
                    }
                else
                    {
                    LOG1("Excluding trigger %d from view failed.",id);
                    LOG1("This trigger is invalidated and shall not be supervised. Reason: %d",err);
                    InvalidateTrigger( id );
                    }
                }
            }
        else
            {
            TLbtTriggerId id = ( iList[index] )->TriggerEntry()->Id();            
            TTrigger trigger;
            
            // Find trigger information from tree if it exists
            TRAPD( err, found = FindInTreeL( id, trigger ) );
            if( found && (KErrNone == err) )
            	{
            	TBool copyStrategyData = ETrue;

            	CLbtTriggerConditionArea* cond = 
            	        static_cast<CLbtTriggerConditionArea*>(iList[index]->TriggerEntry()->GetCondition());
            	CLbtTriggerConditionArea::TDirection oldDirection = 
	            	    cond->Direction();
            	
            	CLbtContainerTriggerEntry* triggerEntry = trigger.info;
            	cond = static_cast<CLbtTriggerConditionArea*>(triggerEntry->TriggerEntry()->GetCondition());
            	CLbtTriggerConditionArea::TDirection newDirection = 
	            	    cond->Direction();
	            	    
	           	if( triggerEntry->ExtendedTriggerInfo()->IsTriggerFireOnCreation() )
	           		{
	           		copyStrategyData = EFalse;
	           		}
	           	else if( newDirection != oldDirection )
	           		{
	           		copyStrategyData = EFalse;
	           		}
            	
            	if( copyStrategyData )
            	    {
            	    // The trigger is already present in the tree. Save the strategy information
                    // since it is required for regular supervision             
                    TStrategyData strategyData = triggerEntry->StrategyData();
                    iList[index]->SetStrategyData( strategyData );
            	    }            	
            	}
            
            found = EFalse;
            
            // Remove trigger information from tree if it exists
            TRAP( err, found = DeleteFromTreeL( id ) );
            if( found && KErrNone != err )
                {
                InvalidateTrigger( id );
                }
            else
                {
                LOG1("Added modified trigger to tree: %d", id );
                
                // Add modified trigger information to tree
                TRAP( err, AddToTreeL( id, iList[index] ) );
                if( KErrNone == err )
                    {
                    TLinearOrder< TLbtTriggerId > byTriggerID( CompareByID );
                    iModifiedTrgList.InsertInOrder( id, byTriggerID );
                    }
                else
                    {
                    LOG1("Excluding trigger %d from view failed.",id);
                    LOG1("This trigger is invalidated and shall not be supervised. Reason: %d",err);
                    InvalidateTrigger( id );
                    }
                }
            }
        
        iList.Remove( index );
        }
        
    // Reset list to be reused for subsequent refresh operations        
    iList.ResetAndDestroy();
    }


// -----------------------------------------------------------------------------
// CLbtTriggerView::HandleTriggerDeletionEvent
// 
// -----------------------------------------------------------------------------
//
void CLbtTriggerView::HandleTriggerDeletionEvent( 
                                        RArray< TLbtTriggerId >* aIDs )
    {
    FUNC_ENTER("CLbtTriggerView::HandleTriggerDeletionEvent");
    TBool found = EFalse;
    for( TInt index = 0; index < aIDs->Count(); index++ )
        {
        TRAPD( err, found = DeleteFromTreeL( ( *aIDs )[index] ) );
        if( found )
            {
            if( KErrNone == err )
                {
                TLinearOrder< TLbtTriggerId > byTriggerID( CompareByID );
                iModifiedTrgList.InsertInOrder( ( *aIDs )[index], byTriggerID );
                }
            else
                {
                LOG1("Invalid trigger ID:%d",( *aIDs )[index]);
                InvalidateTrigger( ( *aIDs )[index] );
                }
            }
        }
    }


// -----------------------------------------------------------------------------
// CLbtTriggerView::CompareByID
// 
// -----------------------------------------------------------------------------
//
TInt CLbtTriggerView::CompareByID( const TLbtTriggerId& src, const TLbtTriggerId& dst )
    {
    if( src > dst )
        {
        return 1;
        }
    else if( src < dst )
        {
        return -1;
        }
    return 0;
    }


// -----------------------------------------------------------------------------
// CLbtTriggerView::ListEnabledAndValidTriggersL
// 
// -----------------------------------------------------------------------------
//
void CLbtTriggerView::ListEnabledAndValidTriggersL()
	{
	FUNC_ENTER("CLbtTriggerView::ListEnabledAndValidTriggersL");
    
    CLbtTriggerFilterByAttribute* filter = 
                        CLbtTriggerFilterByAttribute::NewL();
    CleanupStack::PushL( filter );
        
    filter->AddTriggerTypeL( CLbtTriggerEntry::ETypeSession );
    filter->AddTriggerTypeL( CLbtTriggerEntry::ETypeStartup );
    filter->AddTriggerStateL( CLbtTriggerEntry::EStateEnabled );
    filter->AddTriggerValidityL( TLbtTriggerDynamicInfo::EValid );    
    
    // Set fields to retrieve
    TLbtTriggerDataMask dataMask = 
               CLbtContainerTriggerEntry::EContainerAttributeHysteresisRadius |
               CLbtContainerTriggerEntry::EContainerAttributeRectTriggerArea | 
               CLbtContainerTriggerEntry::EContainerAttributeIsFired |
               CLbtContainerTriggerEntry::EContainerAttributeStrategyData |
               CLbtContainerTriggerEntry::EContainerAttributeIsTriggerFireOnCreation;
            
       
    TLbtTriggerAttributeFieldsMask attribMask = CLbtTriggerEntry::EAttributeId |
                                         CLbtTriggerEntry::EAttributeCondition |
                                         CLbtTriggerEntry::EAttributeState |
                                         CLbtTriggerEntry::EAttributeName;
                                         
    TLbtTriggerDynamicInfoFieldsMask dynInfoMask = 
                                      TLbtTriggerDynamicInfo::EValidityStatus;
    
    CLbtListTriggerOptions* listOpt = CLbtListTriggerOptions::NewL();
    listOpt->SetRetrievedFields( attribMask, dynInfoMask );
    listOpt->SetFilter( filter );
    CleanupStack::PushL( listOpt );
    
    CLbtContainerListOptions* conOpt = CLbtContainerListOptions::NewL( 
                                                               listOpt, NULL );
    conOpt->SetDataMask( dataMask );
    
    CleanupStack::Pop();    // listOpt
    CleanupStack::Pop();    // filter
    
    iTrgContainer->ListTriggers( conOpt, iList, iRequestID, iStatus );
    SetActive(); 
	}



// -----------------------------------------------------------------------------
// CLbtTriggerView::GetTriggers
// 
// -----------------------------------------------------------------------------
//
void CLbtTriggerView::GetTriggers( RArray< TLbtTriggerId >* aIDs )
    {
    FUNC_ENTER("CLbtTriggerView::GetTriggers");
    iTrgContainer->GetTriggers( *aIDs,
    							iList,
    							iRequestID,
    							iStatus );
    SetActive();
    }


// -----------------------------------------------------------------------------
// CLbtTriggerView::AddToTreeL
// 
// -----------------------------------------------------------------------------
//    
void CLbtTriggerView::AddToTreeL( TLbtTriggerId aKey, 
                                    CLbtContainerTriggerEntry* aEntry )
    {
    FUNC_ENTER("CLbtTriggerView::AddToTreeL");
    TBtreePos pos;
    TTrigger tEntry;
    tEntry.info = aEntry;
    tEntry.key = aKey;
    
    CLbtTriggerEntry* trgEntry = aEntry->TriggerEntry();
    CLbtTriggerConditionBase* conditionBase = trgEntry->GetCondition();
    CLbtTriggerConditionArea* conditionArea = 
            static_cast< CLbtTriggerConditionArea* >( conditionBase );
    CLbtGeoAreaBase* areaBase = conditionArea->TriggerArea();    

    tEntry.type = areaBase->Type();
    tEntry.validity = ETrue;
    
    iTriggerList->InsertL( pos, tEntry );

    if( CLbtGeoAreaBase::ECellular == tEntry.type )
        {
        ++iCellTriggerCount;
        }
    else if( CLbtGeoAreaBase::ECircle == tEntry.type )
        {
        ++iCoordTriggerCount;
        }
    else if( CLbtGeoAreaBase::EHybrid == tEntry.type )
        {
        CLbtGeoHybrid* hybridArea = static_cast<CLbtGeoHybrid*> ( areaBase );
        RPointerArray<CLbtGeoAreaBase> hybridAreaArray = hybridArea->HybridArea();
        
        if( hybridAreaArray.Count() )
            {
            // Since hybrid area can hold similar area types, it is enough to 
            // check the first element in the array
            if( hybridAreaArray[0]->Type() == CLbtGeoAreaBase::ECircle )
                {
                ++iCoordTriggerCount;
                }
            else if( hybridAreaArray[0]->Type() == CLbtGeoAreaBase::ECellular )
                {
                ++iCellTriggerCount;
                }
            }
        }
    }
    

// -----------------------------------------------------------------------------
// CLbtTriggerView::FindInTreeL
// 
// -----------------------------------------------------------------------------
//
TBool CLbtTriggerView::FindInTreeL( TLbtTriggerId aKey, TTrigger& aEntry )
	{
	FUNC_ENTER("CLbtTriggerView::FindInTreeL");
	TBtreePos pos;
    TBool found = EFalse;
    found = iTriggerList->FindL( pos, aKey );
    if( found )
        {
        iTriggerList->ExtractAtL( pos, aEntry );
        }
    return found;
	}    

// -----------------------------------------------------------------------------
// CLbtTriggerView::DeleteFromTreeL
// 
// -----------------------------------------------------------------------------
//
TBool CLbtTriggerView::DeleteFromTreeL( TLbtTriggerId aKey )
    {
    FUNC_ENTER("CLbtTriggerView::DeleteFromTreeL");
    TBtreePos pos;
    TBool found = EFalse;
    found = iTriggerList->FindL( pos, aKey );
    if( found )
        {
        TTrigger tEntry;
        iTriggerList->ExtractAtL( pos, tEntry );
        
        if( CLbtGeoAreaBase::ECellular == tEntry.type && iCellTriggerCount != 0 )
            {
            --iCellTriggerCount;
            }
        else if( CLbtGeoAreaBase::ECircle == tEntry.type && iCoordTriggerCount != 0 )
            {
            --iCoordTriggerCount;
            }
        else if( CLbtGeoAreaBase::EHybrid == tEntry.type )
            {
            CLbtTriggerEntry* trgEntry = tEntry.info->TriggerEntry();
            CLbtTriggerConditionBase* conditionBase = trgEntry->GetCondition();
            CLbtTriggerConditionArea* conditionArea = 
                    static_cast< CLbtTriggerConditionArea* >( conditionBase );
            CLbtGeoAreaBase* areaBase = conditionArea->TriggerArea();    

            CLbtGeoHybrid* hybridArea = static_cast<CLbtGeoHybrid*> ( areaBase );
            RPointerArray<CLbtGeoAreaBase> hybridAreaArray = hybridArea->HybridArea();
            
            if( hybridAreaArray.Count() )
                {
                // Since hybrid area can hold similar area types, it is enough to 
                // check the first element in the array
                if( hybridAreaArray[0]->Type() == CLbtGeoAreaBase::ECircle && iCoordTriggerCount != 0)
                    {
                    --iCoordTriggerCount;
                    }
                else if( hybridAreaArray[0]->Type() == CLbtGeoAreaBase::ECellular && iCellTriggerCount != 0 )
                    {
                    --iCellTriggerCount;
                    }
                }
            }
        delete tEntry.info;
        iTriggerList->DeleteL( aKey );
        }
    return found;
    }


// -----------------------------------------------------------------------------
// CLbtTriggerView::SetEvent
// 
// -----------------------------------------------------------------------------
//
void CLbtTriggerView::SetEvent()
    {
    FUNC_ENTER("CLbtTriggerView::SetEvent");
    if ( !IsActive() )
    	{
    	TRequestStatus* lStatus = &iStatus;
    	User::RequestComplete( lStatus, KErrNone );
    	LOG("Calling Set Active !!");
        SetActive();
    	}    
    }


// -----------------------------------------------------------------------------
// CLbtTriggerView::RunL
// 
// -----------------------------------------------------------------------------
//
void CLbtTriggerView::RunL()
    {
    FUNC_ENTER("CLbtTriggerView::RunL");
    if( KErrNone != iStatus.Int() )
        {
        LOG1("iStatus.Int = %d",iStatus.Int() );
        // TODO: Check if error needs to be reported through the 
        //       observer interface
        }
        
    switch( iState )
        {
        
    case EInitialize:
       InitializeList();
       break;
    
    case ERefreshView:
        ProcessRefreshRequests();
        break;

    case ERefreshContainer:
        ProcessUpdateRequests();
        break;
    
    default:
        break;
        
        }
    }


// -----------------------------------------------------------------------------
// CLbtTriggerView::DoCancel
// 
// -----------------------------------------------------------------------------
//
void CLbtTriggerView::DoCancel()
    {
    FUNC_ENTER("CLbtTriggerView::DoCancel");
    if( IsActive() )
        {
        iTrgContainer->CancelAsyncOperation( iRequestID );
        }
    
    // Reset the state the idle
    iState = EIdle;
    }

// end of file

