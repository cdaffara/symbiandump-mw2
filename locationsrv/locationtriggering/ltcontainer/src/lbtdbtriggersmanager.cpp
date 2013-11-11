/*
* Copyright (c) 2006 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  This file defines the class that handles the storage
*                of triggers in database.
*
*/


#include <sysutil.h>
#include <etelmm.h>
#include <lbtstartuptrigger.h>
#include <lbttriggerconditionarea.h>
#include <lbtgeocircle.h>
#include <lbttriggerfilterbase.h>
#include <lbttriggerfilterbyarea.h>
#include <lbttriggerfilterbyattribute.h>
#include <lbttriggerfiltercomposite.h>
#include <lbtgeorect.h>
#include <lbtgeocell.h>
#include <lbtgeohybrid.h>
#include <s32file.h>
#include <bautils.h>
#include "lbtdbtriggersmanager.h"
#include "lbtdboperationao.h"
#include "lbtcontainerextendedtriggerinfo.h"
#include "lbtcontainertriggerfilter.h"
#include "lbtcontainerfilterbase.h"
#include "lbtcontainerareafilter.h"
#include "lbtcontainerattrfilter.h"
#include "lbtcontainercompfilter.h"
#include "lbttriggerstoreobserver.h"
#include "lbtcontainerutilities.h"
#include "lbtserverconsts.h"
#include "lbtlogger.h"


_LIT(KLbtDbName,"lbt.db"); // Name of the Database file for storing triggers.
_LIT(KTriggersTable,"Triggers"); // Name of the Table for Triggers
_LIT(KCircularTriggersTable, "CircularTriggers");
_LIT(KCellTriggersTable, "CellTriggers");
_LIT(KHybridTriggersTable, "HybridTriggers");
_LIT(KTriggersIndexId,"TriggerIdIndex"); // Index based on Trigger Id column
_LIT(KCircTriggersIndexId,"CircularTriggerIdIndex");
_LIT(KCellTriggersIndexId,"CellularTriggerIdIndex");
_LIT(KHybridTriggersIndexId,"HybridTriggerIdIndex");

// Names for the fields in the table
_LIT(KLbtDbFieldId, "id");
_LIT(KLbtDbFieldName, "name");
_LIT(KLbtDbFieldType, "type");
_LIT(KLbtDbFieldDirection, "direction");
_LIT(KLbtDbFieldOwnerSid, "owner");
_LIT(KLbtDbFieldManagerUiSid, "managerui");
_LIT(KLbtDbFieldRearmTime, "rearmtime");
_LIT(KLbtDbFieldStartupSid, "stprocess");
_LIT(KLbtDbFieldStartupPath, "stprocesspath");
_LIT(KLbtDbFieldStartupCommandLine, "stprocesscmdline");
_LIT(KLbtDbFieldState, "state");
_LIT(KLbtDbFieldIsTriggerFired, "IsFired");
_LIT(KLbtDbFieldDistanceToLastLocation,"distance");
_LIT(KLbtDbFieldValidity, "validity");
_LIT(KLbtDbFieldIsTriggerFireOnCreation, "IsTriggerFireOnCreation");
_LIT(KLbtDbTriggerAreaType, "triggerareatype");

// Name for the fields in circular trigger table
_LIT(KLbtCircTrigDbFieldId, "idcirculartrigger");
_LIT(KLbtDbFieldCenterLat, "latitude");
_LIT(KLbtDbFieldCenterLong, "longitude");
_LIT(KLbtDbFieldRadius, "radius");
_LIT(KLbtDbFieldHysteresis, "hysteresis");
_LIT(KLbtDbFieldAreaLeft, "westlong");
_LIT(KLbtDbFieldAreaRight, "eastlong");
_LIT(KLbtDbFieldAreaTop, "northlat");
_LIT(KLbtDbFieldAreaBottom, "southlat");
_LIT(KLbtDbFieldTriggerFiredLat, "FiredLat");
_LIT(KLbtDbFieldTriggerFiredLong, "FiredLong");
_LIT(KLbtDbFieldTriggerFiredAlt, "FiredAlt");
_LIT(KLbtDbFieldTriggerFiredHorizAcc, "FiredHorizAcc");
_LIT(KLbtDbFieldTriggerFiredVertAcc, "FiredVertAcc");
_LIT(KLbtDbFieldCellIdStream,"cellidstream");  // 9.2TB Column additions start

// Name for the fields in cellular trigger table
_LIT(KLbtCellTrigDbFieldId, "idcelltrigger");
_LIT(KLbtCellTrigDbCountryCode, "countrycode");
_LIT(KLbtCellTrigDbNetworkId, "networkid");
_LIT(KLbtCellTrigDbNetworkType, "networktype");
_LIT(KLbtCellTrigDbLAC, "locationcode");
_LIT(KLbtCellTrigDbCellId, "cellid");

// Name for fields in Hybrid trigger table
_LIT(KLbtHybridTrigDbFieldId, "idhybridtrigger");
_LIT(KLbtHybridDataStream, "hybriddatastream"); // Added for 9.2 TB. Older fields not valid from 9.2 TB

// Constants for SQL Queries
_LIT(KWhere, " WHERE ");
_LIT(KLike, " = ");
_LIT(KLessThanEqualTo, " <= ");
_LIT(KGreaterThanEqualTo, " >= ");
_LIT(KAnd, " AND ");

// Enums for the coulumn ids in the common table
enum TCommonDbColNum
	{
	ELbtDbFieldId = 1,
    ELbtDbFieldName,
    ELbtDbFieldType,
    ELbtDbFieldDirection,
    ELbtDbFieldOwnerSid,
    ELbtDbFieldManagerUiSid,
    ELbtDbFieldRearmTime,
    ELbtDbFieldStartupSid, 
    ELbtDbFieldStartupPath, 
    ELbtDbFieldStartupCommandLine,
    ELbtDbFieldState,
    ELbtDbFieldIsTriggerFired,
    ELbtDbFieldDistanceToLastLocation,
    ELbtDbFieldValidity,
    ELbtDbFieldIsTriggerFireOnCreation,
    ELbtDbTriggerAreaType,
    };

// Enums for the column ids in the cicular trigger's db
enum TCircleTriggerDbColNum
	{
	ECircleDbFieldId  = 1,
    ECircleDbFieldCenterLat,
    ECircleDbFieldCenterLong,
    ECircleDbFieldRadius,
    ECircleDbFieldHysteresis,
    ECircleDbFieldAreaLeft,
    ECircleDbFieldAreaRight,
    ECircleDbFieldAreaTop,
    ECircleDbFieldAreaBottom,
    ECircleDbFieldTriggerFiredLat,
    ECircleDbFieldTriggerFiredLong,
    ECircleDbFieldTriggerFiredAlt,
    ECircleDbFieldTriggerFiredHorizAcc,
    ECircleDbFieldTriggerFiredVertAcc,
    };

// Enums for the column ids in the cellular trigger db
enum TCellTriggerDbColNum
	{
	ECellDbFieldId  = 1,
	ECellDbCountryCode,
	ECellDbNetworkId,
	ECellDbNetworkType,	
	ECellDbLocationAreaCode,
	ECellDbTriggerCellId,	
	};

// Enums added for 9.2 TB
enum THybridTriggersDbColNum
    {
    EHybridDbFieldId = 1,
    EHybridDataStream
    };	
	
// SQL Query related constants
const TInt KMaxSqlQueryLength = 256;

// If less than the below % of the db is used for legitimate data then compaction is needed
const TInt KDatabaseMinUsagePercentage = 60;
const TInt KDatabaseMaxWastageBytes = (50 * 1024); // More than 50 KB cannot be wasted
const TInt KStepCount(2); // Number of steps to be executed in one step for async operation

// Assumption that the worst case time needed for compaction is the amount in seconds defined 
// by KSecondsNeededForCompaction
const TTimeIntervalSeconds KSecondsNeededForCompaction(15);

_LIT(KSelect, "SELECT ");
_LIT(KFrom, " FROM ");
_LIT(KSelectAllFields, "* ");
_LIT(KDelete,"DELETE ");

#ifdef _DEBUG
    _LIT(KLbtCntDbPanic,"LBT: Db ");
    TInt KLbtCntDbSqlQueryError = -1;
#endif

// ======== MEMBER FUNCTIONS ========


// ---------------------------------------------------------------------------
// The Symbian 2 phase constructor
// ---------------------------------------------------------------------------
//
CLbtDbTriggersManager* CLbtDbTriggersManager::NewL( ) 
    {
    CLbtDbTriggersManager* self = new(ELeave)CLbtDbTriggersManager( );
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop( self );
    return self;
    }
    

// ---------------------------------------------------------------------------
// Destructor
// ---------------------------------------------------------------------------
//
CLbtDbTriggersManager::~CLbtDbTriggersManager()
    {
    FUNC_ENTER("CLbtDbTriggersManager::~CLbtDbTriggersManager");
    // Close all open Handles    
    delete iDbOperation;
    delete iSqlQuery;
    
    iIdArray.Close();
    
    if(iIsDbOpened)
    	{
    	// Compact the data base before going down
    	iDb.Compact();
    	}    
    iDb.Close();
    iFs.Close();
    }


// ---------------------------------------------------------------------------
// Specify the types of triggers supported by the Db Trigger Store
// ---------------------------------------------------------------------------
//
TTriggerTypeMask CLbtDbTriggersManager::SupportedTriggerTypes()
    {
    return ( CLbtTriggerEntry::ETypeStartup );
    }


// ---------------------------------------------------------------------------
// Specify the types of triggers supported by the Db Trigger Store
// ---------------------------------------------------------------------------
//
void CLbtDbTriggersManager::CreateTriggerL( CLbtContainerTriggerEntry &aEntry,
    										TRequestStatus& aStatus )
    {
    FUNC_ENTER("CLbtDbTriggersManager::CreateTriggerL-I");
    
    // This is just a safe check. While compaction is ongoing the
    // request is queued in container AO
    if(iDbOperation->IsCompactionOngoing())
    	{
        TRequestStatus* status = &aStatus;
    	User::RequestComplete(status, KErrServerBusy);
    	return;
    	}
 	
    // Save client data for create trigger
 	iCurrentOperation = EDbOpCreateTrigger;
    iEntry = &aEntry;
    iClientReqStatus = &aStatus;
    
    TInt size = 500;//ToDo: implement this GetSize( aEntry );
 	if ( SysUtil::FFSSpaceBelowCriticalLevelL( &iFs, size ) )
 	    {
 	    // Try Db Compaction in case we don't have enough disk space
        iDbOperation->CompactDb();
        iOperationState = EOpCompaction;
 	    }
 	else
 	    {
 	    iOperationState = EOpStateQuery;
 	    TInt error = AsyncPrepareViewOfCommonTable();
 	    
 	    if( error != KErrNone )
 	        {
 	        CompleteClientRequest( error );
 	        }
 	    }
    }

// ---------------------------------------------------------------------------
// CLbtDbTriggersManager::ResetCurrentStates
// ---------------------------------------------------------------------------
//
void CLbtDbTriggersManager::ResetCurrentStates()
    {
    iOperationState = EOpStateNone;
    iCurrentOperation = EDbOpNone;
    iEntry = NULL;
    iClientReqStatus = NULL;
    iFilter = NULL;
    delete iFilterBase;
    iFilterBase = NULL;
    iView.Close();
    }

// ---------------------------------------------------------------------------
// CLbtDbTriggersManager::HandleTriggerCreationEventL
// ---------------------------------------------------------------------------
//
void CLbtDbTriggersManager::HandleTriggerCreationEventL( )
    {
    FUNC_ENTER("CLbtDbTriggersManager::HandleTriggerCreationEventL");    
    
    switch(iOperationState)
        {
        case EOpStateQuery:
            {
            // Query is done. Now update the view
            TRAPD( err, CreateTriggerInViewL() );                        
            if( err == KErrNone )
                {
                CLbtTriggerEntry* trigger = iEntry->TriggerEntry();
                if((trigger->State() == CLbtTriggerEntry::EStateEnabled) &&
                   (iEntry->DynInfo()->iValidity == TLbtTriggerDynamicInfo::EValid))
                    {
                    iCountOfEnabledAndValidTrigger++;
                    }
                }
            CompleteClientRequest(err);
            CompactDbIfRequiredL();
            break;
            }
        case EOpCompaction:
            {
            TInt size = 500;//ToDo: implement this GetSize( aEntry );
            if ( !SysUtil::FFSSpaceBelowCriticalLevelL(&iFs, size) )
                {
                iOperationState = EOpStateQuery;
                TInt error = AsyncPrepareViewOfCommonTable();
                if( error != KErrNone )
                    {
                    CompleteClientRequest( KErrDiskFull );
                    }
                }
            else
                {
                CompleteClientRequest( KErrDiskFull );
                }
            break;
            }
        default:
            {
            CompleteClientRequest( KErrGeneral );
            break;
            }
        }
    }

// ---------------------------------------------------------------------------
// CLbtDbTriggersManager::HandleTriggerUpdationEventL
// ---------------------------------------------------------------------------
//
void CLbtDbTriggersManager::HandleTriggerUpdationEventL()
    {

    // Sanity checks to be done here
    __ASSERT_DEBUG(iView.CountL() < 2, User::Panic(KLbtCntDbPanic, KLbtCntDbSqlQueryError) );
    
    switch(iOperationState)
        {
        case EOpStateQuery:
            {
             
            // check if view is empty, return KErrNotFound
            if( !iView.FirstL() || iView.IsEmptyL() || !iView.AtRow() )
                {
                CompleteClientRequest( KErrNotFound );
                break;
                }       
            // Start iterating through all the triggers in view            
            iOperationState = EOpStateIterating;
                       
            }
            // Omitting break is intentional
       case EOpStateIterating:
            {  
            TPtr sql = iSqlQuery->Des();
            
            iView.GetL();
            CLbtContainerTriggerEntry* triggerEntry = RetrieveTriggerEntryFromDbL( iView );    
            TBool securityCheck = LbtContainerUtilities::RunSecurityPolicy( triggerEntry, iClientSecurityPolicy );
        
            if( securityCheck )
                {
                // if security check has passed then the trigger will be updated
                AppendTriggerInfo( triggerEntry );
                }
            else
                {
                CompleteClientRequest( KErrAccessDenied );
                return;
                }
            
            delete triggerEntry;
            iView.FirstL();
            iView.UpdateL();
        
            // Check the trigger type and update the appropriate view
            CLbtGeoAreaBase::TGeoAreaType triggerAreaType = static_cast<CLbtGeoAreaBase::TGeoAreaType>(iView.ColInt8(ELbtDbTriggerAreaType));
            CLbtStartupTrigger* trigger = static_cast<CLbtStartupTrigger*>(iEntry->TriggerEntry());
        
            // Check if the condition itself has to be replaced
            if( iAttrMask & CLbtTriggerEntry::EAttributeCondition )
                {
                CLbtTriggerConditionArea* cond=static_cast <CLbtTriggerConditionArea*> (trigger->GetCondition());
                CLbtGeoAreaBase* areaBase = cond->TriggerArea();
            
                // The area condition has changed
                if( triggerAreaType != areaBase->Type() )
                    {
                    // Check if the trigger area type itself has been updated
                    RemoveTriggerConditionFromDb( iEntry->TriggerEntry()->Id(), triggerAreaType );
                    switch(areaBase->Type())
                        {
                        case CLbtGeoAreaBase::ECircle:
                            {
                            AddGeoCircleIntoDbL(trigger);
                            break;
                            }
                        case CLbtGeoAreaBase::ECellular:
                            {
                            AddGeoCellIntoDbL(trigger);
                            break;
                            }
                        case CLbtGeoAreaBase::EHybrid:
                            {
                            AddGeoHybridIntoDbL(trigger);
                            break;
                            }
                        default:
                            {
                            CompleteClientRequest( KErrArgument );
                            break;
                            }
                        }
                    
                    // Write the area type
                    iView.SetColL(ELbtDbTriggerAreaType,static_cast<TInt8>(areaBase->Type()));
                
                    // Negate the Attribute condition so that it is not updated again
                    iAttrMask = iAttrMask & (~(CLbtTriggerEntry::EAttributeCondition));
                
                    // Update trigger area type
                    triggerAreaType = areaBase->Type();
                    }
            
                    CLbtTriggerConditionArea::TDirection currentTriggerDirection = 
                            static_cast<CLbtTriggerConditionArea::TDirection>(iView.ColInt8(ELbtDbFieldDirection));
                    
                    if( currentTriggerDirection != cond->Direction() )
                        {
                        iView.SetColL( ELbtDbFieldDirection, static_cast<TInt8>((cond->Direction())) );
                        }
                    }    
    
            CLbtTriggerEntry::TLbtTriggerState currentState = 
                        static_cast<CLbtTriggerEntry::TLbtTriggerState>(iView.ColInt8(ELbtDbFieldState));
        
            if( (iEntry->TriggerEntry()->State() == CLbtTriggerEntry::EStateEnabled) && 
                (currentState == CLbtTriggerEntry::EStateDisabled) )
                 {
                 iCountOfEnabledAndValidTrigger++;
                 }
            else if( (iEntry->TriggerEntry()->State() == CLbtTriggerEntry::EStateDisabled) && 
                     (currentState == CLbtTriggerEntry::EStateEnabled) )
                 {
                 iCountOfEnabledAndValidTrigger--;
                 }
        
            CLbtExtendedTriggerInfo* extdInfo = iEntry->ExtendedTriggerInfo();
            TLbtTriggerDynamicInfo* trigDynInfo = iEntry->DynInfo();
            
            //Check the masks and update the specified attributes
            if(trigger != NULL)
                {
                if( iAttrMask & CLbtTriggerEntry::EAttributeId )
                    {
                    iView.SetColL( ELbtDbFieldId, static_cast<TInt64>(trigger->Id()) );
                    }
        
                if( iAttrMask & CLbtTriggerEntry::EAttributeName )
                    {
                    iView.SetColL( ELbtDbFieldName, trigger->Name() );
                    }
        
                if( iAttrMask & CLbtTriggerEntry::EAttributeState )
                    {
                    iView.SetColL( ELbtDbFieldState, trigger->State() );
                    }
                
                if( iAttrMask & CLbtTriggerEntry::EAttributeRearmTime )
                    {
                    iView.SetColL( ELbtDbFieldRearmTime, trigger->TimeToRearm() );
                    }        
                
                if( iAttrMask & CLbtTriggerEntry::EAttributeStartUpCommandLine )
                    {
                    iView.SetColL( ELbtDbFieldStartupCommandLine, trigger->CommandLine() );          
                    }
              
                if( iAttrMask & CLbtTriggerEntry::EAttributeCondition )
                    {
                    CLbtTriggerConditionArea* cond=static_cast <CLbtTriggerConditionArea*> (trigger->GetCondition());
                    
                    switch( triggerAreaType )
                        {
                        case CLbtGeoAreaBase::ECircle:
                            {
                            //Since the trigger type is circular get the appropriate tuple from the circular trigger table
                            RDbView circView;
                            sql.Zero();
                            sql.Append( KSelect );
                            sql.Append( KSelectAllFields );
                            sql.Append( KFrom );
                            sql.Append( KCircularTriggersTable );
                            sql.Append( KWhere );
                            sql.Append( KLbtCircTrigDbFieldId );
                            sql.Append( KLike );
                            sql.AppendNum(iEntry->TriggerEntry()->Id());
                            
                            TInt error = iDbOperation->ExecuteSyncQuery( circView, sql );
                            if(error != KErrNone)
                                {
                                circView.Close();
                                User::Leave(error);
                                }
        
                            CleanupClosePushL(circView);
                            circView.FirstL();     
                            if ( circView.IsEmptyL() || !circView.AtRow() )
                                {
                                CleanupStack::PopAndDestroy(1); //circView
                                CompleteClientRequest( KErrNotFound );                                
                                return;
                                }
        
                            circView.UpdateL();
                            
                            // The area is of type circle
                            CLbtGeoCircle* area= static_cast <CLbtGeoCircle*> (cond->TriggerArea());
                    
                            circView.SetColL( ECircleDbFieldCenterLat, area->Center().Latitude() );
                            circView.SetColL( ECircleDbFieldCenterLong, area->Center().Longitude() );
                            circView.SetColL( ECircleDbFieldRadius, area->Radius() );
                            
                            if(extdInfo!=NULL)
                                {
                                if( iDataMask & CLbtContainerTriggerEntry::EContainerAttributeHysteresisRadius )
                                    {
                                    circView.SetColL( ECircleDbFieldHysteresis, extdInfo->HysteresisRadius() );
                                    }
                                
                                if( iDataMask & CLbtContainerTriggerEntry::EContainerAttributeRectTriggerArea )
                                    {                       
                                    circView.SetColL( ECircleDbFieldAreaLeft, extdInfo->TriggerReactangleArea().iTrigAreaWestLong );
                                    circView.SetColL( ECircleDbFieldAreaRight, extdInfo->TriggerReactangleArea().iTrigAreaEastLong );
                                    circView.SetColL( ECircleDbFieldAreaTop, extdInfo->TriggerReactangleArea().iTrigAreaNorthLat );
                                    circView.SetColL( ECircleDbFieldAreaBottom, extdInfo->TriggerReactangleArea().iTrigAreaSouthLat );
                                    }
                               
                                if( iDataMask & CLbtContainerTriggerEntry::EContainerAttributeFireInfo )
                                    {
                                    TLbtTriggerFireInfo fireInfo = extdInfo->GetFiredInfo();
                                    TPosition pos;
                                    fireInfo.iFiredPositionInfo.GetPosition(pos);
                                  
                                    circView.SetColL(ECircleDbFieldTriggerFiredLat, pos.Latitude());
                                    circView.SetColL(ECircleDbFieldTriggerFiredLong, pos.Longitude());
                                    circView.SetColL(ECircleDbFieldTriggerFiredAlt, pos.Altitude());
                                    circView.SetColL(ECircleDbFieldTriggerFiredHorizAcc, pos.HorizontalAccuracy());
                                    circView.SetColL(ECircleDbFieldTriggerFiredVertAcc, pos.VerticalAccuracy());
                                    }
                               }
                            circView.PutL();
                            CleanupStack::PopAndDestroy(1); // circView
                            break;
                            }
                        case CLbtGeoAreaBase::ECellular:
                            {
                            //Since the trigger type is cell get the appropriate tuple from the cell trigger table
                            RDbView cellView;
                            sql.Zero();
                            sql.Append( KSelect );
                            sql.Append( KSelectAllFields );
                            sql.Append( KFrom );
                            sql.Append( KCellTriggersTable );
                            sql.Append( KWhere );
                            sql.Append( KLbtCellTrigDbFieldId );
                            sql.Append( KLike );
                            sql.AppendNum( iEntry->TriggerEntry()->Id() );
                            
                            TInt error = iDbOperation->ExecuteSyncQuery( cellView, sql );      
                            if( error != KErrNone )
                                {
                                cellView.Close();
                                User::Leave(error);
                                }       
                            
                            CleanupClosePushL( cellView );
                            cellView.FirstL();     
                            if ( cellView.IsEmptyL() || !cellView.AtRow() )
                                {
                                CleanupStack::PopAndDestroy(); //cellView
                                CompleteClientRequest( KErrNotFound );                
                                return;
                                }
        
                            cellView.UpdateL();
                            
                            // The area is of type cell             
                            CLbtGeoCell* cell = static_cast <CLbtGeoCell*> (cond->TriggerArea());
                            
                            // Update the cell parameters
                            
                            // Update NCC
                            TLex lex( cell->NetworkCountryCode() );
                            TUint netCountryCode;
                            lex.Val( netCountryCode );
                            cellView.SetColL( ECellDbCountryCode, netCountryCode );
                            
                            // Update NIC
                            lex.Assign( cell->NetworkIdentityCode() );
                            TUint networkIdentityCode;
                            lex.Val( networkIdentityCode );             
                            cellView.SetColL( ECellDbNetworkId, networkIdentityCode );
                            
                            // Update Network Type
                            RMobilePhone::TMobilePhoneNetworkMode nwmode = cell->NetworkType();
                            if(nwmode == RMobilePhone::ENetworkModeGsm)
                                {
                                cellView.SetColL( ECellDbNetworkType, RMobilePhone::ENetworkModeGsm );
                                }                       
                            else if(nwmode == RMobilePhone::ENetworkModeWcdma) 
                                {
                                cellView.SetColL( ECellDbNetworkType, RMobilePhone::ENetworkModeWcdma );
                                }
                    
                            // Update LAC and Cell Id
                            cellView.SetColL( ECellDbLocationAreaCode, cell->LocationAreaCode());
                            cellView.SetColL( ECellDbTriggerCellId, cell->CellId());                    
                            cellView.PutL();
                            CleanupStack::PopAndDestroy(1); //cellView
                            break;
                            }
                        case CLbtGeoAreaBase::EHybrid:
                            {
                            RDbView hybridView;
                            sql.Zero();
                            sql.Append( KSelect );
                            sql.Append( KSelectAllFields );
                            sql.Append( KFrom );
                            sql.Append( KHybridTriggersTable );
                            sql.Append( KWhere );
                            sql.Append( KLbtHybridTrigDbFieldId );
                            sql.Append( KLike );
                            sql.AppendNum( iEntry->TriggerEntry()->Id() );
                            
                            TInt err = iDbOperation->ExecuteSyncQuery(hybridView, sql);    
                            if( err != KErrNone )
                                {
                                hybridView.Close();
                                User::Leave(err);
                                }
                            CleanupClosePushL(hybridView);                    
                            
                            hybridView.FirstL();     
                            if ( hybridView.IsEmptyL() || !hybridView.AtRow() )
                                {
                                CleanupStack::PopAndDestroy(); //hybridView
                                CompleteClientRequest( KErrNotFound );                        
                                return;
                                }
                            
                            hybridView.UpdateL();
                            
                            // cast to hybrid area
                            CLbtGeoHybrid* hybrid = static_cast<CLbtGeoHybrid *>(cond->TriggerArea());                    
                            
                            // Externalize the hybrid areas into the DB
                            hybridView.SetColNullL( EHybridDataStream );
                            RDbColWriteStream writeStream;
                            writeStream.OpenLC(hybridView, EHybridDataStream);            
                            const RPointerArray<CLbtGeoAreaBase>& hybridAreas = hybrid->HybridArea();            
                            // Write the count into the the stream
                            writeStream.WriteInt32L( hybridAreas.Count() );
                            for( TInt i=0; i<hybridAreas.Count(); ++i )
                                {
                                // Write the area type
                                writeStream.WriteInt32L( hybridAreas[i]->Type() );
                                
                                // Externalize the area data
                                hybridAreas[i]->ExternalizeL(writeStream);
                                }
                            writeStream.CommitL();
                            writeStream.Close();                    
                            hybridView.PutL();            
                            CleanupStack::PopAndDestroy( 2 ); // hybridView, writeStream
                            break;
                            }
                        default:
                            {
                            CompleteClientRequest( KErrArgument );
                            break;
                            }                    
                        }
                    }
                }
            
             if(extdInfo!=NULL)
                {                       
                if( iDataMask & CLbtContainerTriggerEntry::EContainerAttributeIsFired )
                    {
                    iView.SetColL(ELbtDbFieldIsTriggerFired, extdInfo->IsTriggerFired());
                    }
                
                if( iDataMask & CLbtContainerTriggerEntry::EContainerAttributeOwnerSid )
                    {
                    iView.SetColL( ELbtDbFieldOwnerSid, extdInfo->OwnerSid().iId );
                    }        
                    
                if( iDataMask & CLbtContainerTriggerEntry::EContainerAttributeIsTriggerFireOnCreation )
                    {
                    iView.SetColL( ELbtDbFieldIsTriggerFireOnCreation, extdInfo->IsTriggerFireOnCreation() );
                    }       
                }
        
            if(trigDynInfo != NULL)
                {
                if( iDataMask & CLbtContainerTriggerEntry::EContainerDynInfoAttributeValidity )
                    {
                    iView.SetColL( ELbtDbFieldValidity, static_cast<TInt8>(trigDynInfo->iValidity) );
                    }
                }
        
            // Flush the updates into the view/db
            iView.PutL();
            CompleteClientRequest( KErrNone );        
            CompactDbIfRequiredL();
            break;
            }
        default:
            {
            CompleteClientRequest( KErrGeneral );
            break;
            }
        }
    }

// ---------------------------------------------------------------------------
// CLbtDbTriggersManager::HandleTriggerStateUpdationEventL
// ---------------------------------------------------------------------------
//
void CLbtDbTriggersManager::HandleTriggerStateUpdationEventL()
    {
    FUNC_ENTER("CLbtDbTriggersManager::HandleTriggerStateUpdationEventL");    
    
    switch(iOperationState)
        {
        case EOpStateQuery:
            {
            // check if view is empty, return KErrNotFound
            if( iView.IsEmptyL() || !iView.FirstL())
                {
                CompleteClientRequest( KErrNotFound );
                break;
                }       
            // Start iterating through all the triggers in view            
            iOperationState = EOpStateIterating;
           
            }
            // Omitting break is intentional
        case EOpStateIterating:
            {            
            TUid ownerSid;
            TUid managerUi;
            TSecureId startupSid;
            TInt isTriggerPresent = 0; 
            TInt count = 0;
            while( iView.AtRow() && count < KStepCount )
                {
                ++count;
                iView.GetL();    
                // Check if the trigger satisfies the security policy
                ownerSid.iUid = iView.ColUint32(ELbtDbFieldOwnerSid);
                managerUi.iUid = iView.ColUint32(ELbtDbFieldManagerUiSid);
                startupSid.iId = iView.ColUint32(ELbtDbFieldStartupSid);
                
                if( LbtContainerUtilities::RunSecurityPolicy( ownerSid,
                                                              managerUi,
                                                              startupSid,
                                                              iClientSecurityPolicy ) )
                    {
                    CLbtContainerTriggerEntry* entry = RetrieveTriggerEntryFromDbL( iView );
                    CleanupStack::PushL(entry);
                    
                    TInt isFilterPresent = 0;
                    TBool isEntryRequested = EFalse;
                    
                    iFilterBase->ProcessFilter(entry, isFilterPresent, isEntryRequested);
                    
                    if(isFilterPresent > 0 && isEntryRequested)
                        {
                        AppendTriggerInfo(entry);
                        isTriggerPresent++;
                        
                        iView.UpdateL();
                        iView.SetColL( ELbtDbFieldState, iState );
                        
                        if( iFireOnUpdate == ELbtTrue )
                            {
                            iView.SetColL( ELbtDbFieldIsTriggerFireOnCreation, ETrue );
                            }
                        else if( iFireOnUpdate == ELbtFalse )
                            {
                            iView.SetColL( ELbtDbFieldIsTriggerFireOnCreation, EFalse );
                            }
                        iView.PutL();

                        CLbtTriggerEntry* trigEntry = entry->TriggerEntry();
                                
                        if( trigEntry->State() == CLbtTriggerEntry::EStateDisabled && 
                             iState == CLbtTriggerEntry::EStateEnabled )
                             {
                             iCountOfEnabledAndValidTrigger++;
                             }
                        else if( trigEntry->State() == CLbtTriggerEntry::EStateEnabled && 
                             iState == CLbtTriggerEntry::EStateDisabled)
                             {
                             iCountOfEnabledAndValidTrigger--;
                             }
                        }
                    CleanupStack::PopAndDestroy(entry);
                    }       
                 TBool res =iView.NextL();   
                 if (!res)
                 	break;
              }
            
            if( count >= KStepCount )
                {
                // Self complete to process next request
                SelfComplete();
                }
            else 
                {
                CompleteClientRequest( KErrNone );                               
                CompactDbIfRequiredL();
                }
            }
            break;
        default:
            {
            CompleteClientRequest( KErrGeneral );
            break;
            }
        }
    }

// ---------------------------------------------------------------------------
// CLbtDbTriggersManager::HandleTriggersDeletionEventL
// ---------------------------------------------------------------------------
//
void CLbtDbTriggersManager::HandleTriggersDeletionEventL()
    {
    switch( iOperationState )
        {
        case EOpStateQuery:
            {
            // check if view is empty, return KErrNotFound
            if( iView.IsEmptyL() || !iView.FirstL())
                {
                CompleteClientRequest( KErrNotFound );
                break;
                }
            
            // Start iterating through all the triggers in view            
            iOperationState = EOpStateIterating;
            
            
            }
            // Omitting break is intentional
        case EOpStateIterating:
            {
            TInt count = 0;
            while( count != KStepCount )
                {
                ++count;
                if( iView.AtRow() )
                    {
                    iView.GetL();                            
                    TUid ownerSid;
                    TUid managerUi;
                    TSecureId startupSid;
                    
                    ownerSid.iUid = iView.ColUint32( ELbtDbFieldOwnerSid );
                    managerUi.iUid = iView.ColUint32( ELbtDbFieldManagerUiSid );
                    startupSid.iId = iView.ColUint32( ELbtDbFieldStartupSid );
                    
                    if( LbtContainerUtilities::RunSecurityPolicy( ownerSid,
                                                                  managerUi,
                                                                  startupSid,
                                                                  iClientSecurityPolicy ) )
                        {
                        CLbtContainerTriggerEntry* entry = RetrieveTriggerEntryFromDbL( iView );
                        CleanupStack::PushL( entry );
                        TInt isFilterPresent = 0;
                        TBool isEntryRequested = EFalse;

                        iFilterBase->ProcessFilter(entry, isFilterPresent, isEntryRequested);
                        if( isFilterPresent > 0 && isEntryRequested )
                            {
                            AppendTriggerInfo(entry);
                            CLbtTriggerEntry* trigEntry = entry->TriggerEntry();
                            
                            if( trigEntry->State() == CLbtTriggerEntry::EStateEnabled && 
                                entry->DynInfo()->iValidity == TLbtTriggerDynamicInfo::EValid )
                                {
                                iCountOfEnabledAndValidTrigger--;
                                }
                            
                            // TODO: Check if this will crash
                            DeleteTriggerL( entry->TriggerEntry()->Id() );
                            }
                        CleanupStack::PopAndDestroy( entry );
                        }
                    
                     TBool res = iView.NextL();
                      if(!res)
            	         break;
                    }                
                }

            if( !iView.AtRow() )
                {
                CompleteClientRequest( KErrNone );
                CompactDbIfRequiredL();
                }
            else
                {
                SelfComplete();
                }
            break;
            }
        default:
            {
            CompleteClientRequest( KErrGeneral );
            break;
            }
        }
    }

//-------------------------------------------------------------------------------------------------
// CLbtDbTriggersManager::HandleGetTriggersEventL
//-------------------------------------------------------------------------------------------------
// 
void CLbtDbTriggersManager::HandleGetTriggersEventL( )
    {   
    /**
      * Once control reaches here it means that the Db Query
      * "Select * From table Triggers" has been executed and
      * the view contains all the rows.
      */
    
    TInt count = 0;
    while( iView.AtRow()  && count < KStepCount )
        {
        ++count;
        iView.GetL();
        TLbtTriggerId trigId = iView.ColInt64( ELbtDbFieldId );
        TInt index = iTriggerIds->Find(trigId);

        if( index != KErrNotFound )
            {
            CLbtContainerTriggerEntry* entry = RetrieveTriggerEntryFromDbL( iView );
            iTriggerIds->Remove( index );
        
            if( LbtContainerUtilities::RunSecurityPolicy( entry, iClientSecurityPolicy ) )
                {
                iTriggers->Append(entry);            
                }
            else
                {
                delete entry;
                }
            }

        if(!iView.NextL())
        	break;
        }

    if( count >= KStepCount )
        {
        // Self complete to process next request
        SelfComplete();
        }
    else // Operation is over
        {
        if(iTriggers->Count() == 0)
            {
            CompleteClientRequest( KErrNotFound );
            }
        else
            {
            CompleteClientRequest( KErrNone );
            }
        }
    }
// ---------------------------------------------------------------------------
// CLbtDbTriggersManager::CompleteClientRequest
// ---------------------------------------------------------------------------
//
void CLbtDbTriggersManager::CompleteClientRequest(TInt aError)
    {
    FUNC_ENTER("CLbtDbTriggersManager::CompleteClientRequest");
    if( iClientReqStatus )
        User::RequestComplete(iClientReqStatus,aError);
    ResetCurrentStates();
    }

// ---------------------------------------------------------------------------
// CLbtDbTriggersManager::AsyncPrepareViewOfCommonTable
// ---------------------------------------------------------------------------
//
TInt CLbtDbTriggersManager::AsyncPrepareViewOfCommonTable()
    {
    FUNC_ENTER("CLbtDbTriggersManager::PrepareViewsL");
    TPtr sql = iSqlQuery->Des();
    sql.Zero();
    sql.Append( KSelect );
    sql.Append( KSelectAllFields );
    sql.Append( KFrom );
    sql.Append( KTriggersTable );
    
    // First close the view just as a safe check
    iView.Close();
    
    TInt error = iView.Prepare( iDb, TDbQuery( sql, EDbCompareFolded) );
    if( error != KErrNone )
        {
        iView.Close();
        }
    else
        {
        iView.Evaluate(iStatus);
        SetActive();
        }    
    return error;
    }

// ---------------------------------------------------------------------------
// CLbtDbTriggersManager::RetrieveTriggerEntryFromDbL
// Retreives a trigger entry from the first row in the views
// ---------------------------------------------------------------------------
//
CLbtContainerTriggerEntry* CLbtDbTriggersManager::RetrieveTriggerEntryFromDbL( RDbView& aMainView )
	{
	/**
	 * This function is coded under the assumption that aMainView has been initialized
	 * and other views have been un-initialized.
 	 */
	FUNC_ENTER("CLbtDbTriggersManager::RetrieveTriggerEntryFromDbL");
	CLbtContainerTriggerEntry* entry = CLbtContainerTriggerEntry::NewL();
	CleanupStack::PushL(entry);
	
	CLbtStartupTrigger* clientEntry = CLbtStartupTrigger::NewL();
	CleanupStack::PushL(clientEntry);
	
	CLbtExtendedTriggerInfo* clientExtInfo = CLbtExtendedTriggerInfo::NewL();
	CleanupStack::PushL(clientExtInfo);

	CLbtTriggerConditionArea* cond = CLbtTriggerConditionArea::NewL();
	CleanupStack::PushL(cond);
	
	TLbtTriggerId trigId = aMainView.ColInt64(ELbtDbFieldId);
	clientEntry->SetId(trigId);
	clientEntry->SetNameL(aMainView.ColDes16(ELbtDbFieldName));
	CLbtTriggerEntry::TLbtTriggerState state = 
			static_cast <CLbtTriggerEntry::TLbtTriggerState>(aMainView.ColInt8(ELbtDbFieldState));
	clientEntry->SetState(state);
	TSecureId sid;
	sid.iId = aMainView.ColUint32(ELbtDbFieldStartupSid);
	
	TUid managerUi;
    managerUi.iUid=aMainView.ColUint32(ELbtDbFieldManagerUiSid);    
	clientEntry->SetManagerUi(managerUi);
	
	clientEntry->SetTimeToRearm( aMainView.ColInt32(ELbtDbFieldRearmTime) );
	
	clientEntry->SetProcessId(aMainView.ColDes16(ELbtDbFieldStartupPath),sid);
	
	clientEntry->SetCommandLineL( aMainView.ColDes16(ELbtDbFieldStartupCommandLine) ); 
	
	TInt fireOnCreation;
	fireOnCreation=aMainView.ColInt(ELbtDbFieldIsTriggerFireOnCreation);
    clientExtInfo->SetTriggerFireOnCreation(fireOnCreation); 
	cond->SetDirection(static_cast<CLbtTriggerConditionArea::TDirection>(aMainView.ColInt8(ELbtDbFieldDirection)));
	
	TUid ownerSid;
	ownerSid.iUid=aMainView.ColUint32(ELbtDbFieldOwnerSid);
	clientExtInfo->SetOwnerSid(ownerSid); 
   	clientExtInfo->SetTriggerFiredState(aMainView.ColInt(ELbtDbFieldIsTriggerFired)) ;
	
	TLbtTriggerFireInfo fireInfo;
	fireInfo.iAreaType = static_cast<CLbtGeoAreaBase::TGeoAreaType>(aMainView.ColInt8(ELbtDbTriggerAreaType));
	fireInfo.iTriggerId = aMainView.ColInt64(ELbtDbFieldId);
	clientExtInfo->SetFiredInfo(fireInfo);
	
	switch( aMainView.ColInt8(ELbtDbTriggerAreaType) )
	    {
	    case CLbtGeoAreaBase::ECircle:
	        {
	        TPtr sql = iSqlQuery->Des();
	        sql.Zero();
	        sql.Append( KSelect );
	        sql.Append( KSelectAllFields );
	        sql.Append( KFrom );
	        sql.Append( KCircularTriggersTable );
	        sql.Append( KWhere );
	        sql.Append( KLbtCircTrigDbFieldId );
	        sql.Append( KLike );
	        sql.AppendNum(trigId);    
	    
	        RDbView circView;
	        TInt error = iDbOperation->ExecuteSyncQuery( circView, sql );      
	        if(error != KErrNone)
	            {
	            circView.Close();
	            User::Leave(error);
	            }

	        CleanupClosePushL( circView );
	        circView.FirstL();
	        circView.GetL();
	        TCoordinate center;
	        center.SetCoordinate(circView.ColReal64(ECircleDbFieldCenterLat),circView.ColReal64(ECircleDbFieldCenterLong));
	        CLbtGeoCircle* circle=CLbtGeoCircle::NewL(center,circView.ColReal64(ECircleDbFieldRadius));
	        cond->SetTriggerArea(circle);       
	        clientExtInfo->SetHysteresisRadius(circView.ColReal64(ECircleDbFieldHysteresis));
	        
	        CLbtExtendedTriggerInfo::TLbtTriggerRectArea rect;
	        rect.iTrigAreaSouthLat=circView.ColReal64(ECircleDbFieldAreaBottom);
	        rect.iTrigAreaNorthLat=circView.ColReal64(ECircleDbFieldAreaTop);
	        rect.iTrigAreaEastLong=circView.ColReal64(ECircleDbFieldAreaRight);
	        rect.iTrigAreaWestLong=circView.ColReal64(ECircleDbFieldAreaLeft);
	        clientExtInfo->SetTriggerRectangleArea(rect); 
	        
	        TPosition pos;
	        pos.SetCoordinate( circView.ColReal64(ECircleDbFieldTriggerFiredLat), 
                               circView.ColReal64(ECircleDbFieldTriggerFiredLong),
                               circView.ColReal32(ECircleDbFieldTriggerFiredAlt) );
	                           
	        TReal32 horAcc = circView.ColReal32(ECircleDbFieldTriggerFiredHorizAcc);
	        TReal32 verAcc = circView.ColReal32(ECircleDbFieldTriggerFiredVertAcc);
	        
	        if ( horAcc != -1)
	            {
	            pos.SetHorizontalAccuracy(horAcc);  
	            }
	        if(verAcc != -1)
	            {
	            pos.SetVerticalAccuracy(verAcc);    
	            }
	        fireInfo.iFiredPositionInfo.SetPosition(pos);
	        CleanupStack::PopAndDestroy(1); // circView
	        break;
	        }
	    case CLbtGeoAreaBase::ECellular:
	        {
	        TPtr sql = iSqlQuery->Des();
	        sql.Zero();
	        sql.Append( KSelect );
	        sql.Append( KSelectAllFields );
	        sql.Append( KFrom );
	        sql.Append( KCellTriggersTable );
	        sql.Append( KWhere );
	        sql.Append( KLbtCellTrigDbFieldId );
	        sql.Append( KLike );
	        sql.AppendNum( trigId );
	        
	        RDbView cellView;
	        TInt error = iDbOperation->ExecuteSyncQuery( cellView, sql );
	        if(error != KErrNone)
	            {
	            cellView.Close();
	            User::Leave(error);
	            }
	        
	        CleanupClosePushL( cellView );
	        cellView.FirstL();
	        cellView.GetL();
	    
	        TUint32 countryCode = cellView.ColUint32(ECellDbCountryCode);
	        TUint32 networkID = cellView.ColUint32(ECellDbNetworkId);
	        RMobilePhone::TMobilePhoneNetworkMode netType = static_cast<RMobilePhone::TMobilePhoneNetworkMode>(cellView.ColInt8(ECellDbNetworkType));
	        TUint32 locAreaCode = cellView.ColUint32(ECellDbLocationAreaCode);
	        TUint32 cellid = cellView.ColUint32(ECellDbTriggerCellId);
	        
	        RMobilePhone::TMobilePhoneNetworkIdentity networkCode;
	        networkCode.AppendNum( networkID );
	        
	        RMobilePhone::TMobilePhoneNetworkCountryCode setCountryCode;
	        setCountryCode.AppendNum( countryCode );
	        
	        CLbtGeoCell* cell= CLbtGeoCell::NewL( netType,
	                                              setCountryCode,
	                                              networkCode, 
	                                              locAreaCode,cellid );
	        cond->SetTriggerArea(cell);
	        CleanupStack::PopAndDestroy(1); // cellView	        
	        break;
	        }
	    case CLbtGeoAreaBase::EHybrid:
            {
            TPtr sql = iSqlQuery->Des();
            RDbView view;
            sql.Zero();
            sql.Append( KSelect );
            sql.Append( KSelectAllFields );
            sql.Append( KFrom );
            sql.Append( KHybridTriggersTable );
            sql.Append( KWhere );
            sql.Append( KLbtHybridTrigDbFieldId );
            sql.Append( KLike );
            sql.AppendNum( trigId );
            
            TInt error = iDbOperation->ExecuteSyncQuery( view, sql );
            if(error != KErrNone)
                {
                view.Close();
                User::Leave(error);
                }
            CleanupClosePushL(view);
            
            view.FirstL();
            view.GetL();

            RPointerArray<CLbtGeoAreaBase> areaArray;
            CleanupClosePushL( areaArray );
            
            RDbColReadStream readStream;
            readStream.OpenLC(view, EHybridDataStream);
            
            // Get count of number of area base objects
            TInt count = readStream.ReadInt32L();            
            for( TInt i=0;i<count;++i )
                {
                // Create appropriate area type object
                CLbtGeoAreaBase* areaBase = NULL;
                switch( readStream.ReadInt32L() )
                    {
                    case CLbtGeoAreaBase::ECircle:
                        {
                        areaBase = CLbtGeoCircle::NewLC(); 
                        break;
                        }
                    case CLbtGeoAreaBase::ECellular:
                        {
                        areaBase = CLbtGeoCell::NewLC();
                        break;
                        }                    
                    default:
                        {
                        User::Leave( KErrArgument );
                        break;
                        }
                    }
                areaBase->InternalizeL(readStream);
                areaArray.AppendL(areaBase);
                CleanupStack::Pop(1); //areaBase                
                }
            
            CLbtGeoHybrid* hybrid = CLbtGeoHybrid::NewLC(areaArray);
            // Set the wlan area to the trigger
            cond->SetTriggerArea( hybrid ); // cond takes ownership of hybrid            
            CleanupStack::Pop(4); // hybrid, readStream, areaArray and view
            readStream.Close();
            view.Close();
            areaArray.Close(); // Ownership of pointers taken by CLbtGeoHybrid
            break;
            }
	    }
			
	clientEntry->SetCondition(cond);	
	TLbtTriggerDynamicInfo* info = new (ELeave) TLbtTriggerDynamicInfo;
	info->iValidity=static_cast <TLbtTriggerDynamicInfo::TLbtTriggerValidity> (aMainView.ColInt8(ELbtDbFieldValidity));
	info->iDistanceToLatestLocation= aMainView.ColReal32(ELbtDbFieldDistanceToLastLocation) ;
	TPosition firedPosition;
	fireInfo.iFiredPositionInfo.GetPosition( firedPosition );
	info->iFiredLocality = firedPosition;
	
	entry->SetTriggerEntry(clientEntry);    
	entry->SetExtendedTriggerInfo(clientExtInfo);    
	entry->SetDynInfo(info);
	CleanupStack::Pop(4); // entry, clientEntry, clientExtInfo and cond
	return entry;	
	}

// --------------------------------------------------------------------------
// CLbtDbTriggersManager::GetTriggersL
// ---------------------------------------------------------------------------
//
void CLbtDbTriggersManager::GetTriggersL( RArray<TLbtTriggerId>& aTriggerIds,
										  RPointerArray<CLbtContainerTriggerEntry>& aTriggers, 
										  TLbtSecurityPolicy& aSecurityPolicy,
										  TRequestStatus &aStatus )
	{
	FUNC_ENTER("CLbtDbTriggersManager::GetTriggersL");
	 aStatus = KRequestPending;
	 iClientReqStatus = &aStatus;
	// This is just a safe check. While compaction is ongoing the
    // request is queued in container AO
    if(iDbOperation->IsCompactionOngoing())
    	{
    	ERROR("COMPACTION ONGOING.THIS SHOULD NOT HAPPEN", KErrServerBusy);    	
    	CompleteClientRequest( KErrServerBusy );
    	return;
    	}
    
    iClientSecurityPolicy = aSecurityPolicy;
    iTriggers = &aTriggers;
    iTriggerIds = &aTriggerIds;
    iCurrentOperation = EDbOpGetTriggers;
    // Prepare view for triggers
    PrepareViewForTriggersL( aTriggerIds, iView );
 
    // rest of list triggers operation is in the method HandleGetTriggersEventL.
   if(!iView.FirstL())
   {
   	CompleteClientRequest( KErrNotFound );
	return;
	}
    SelfComplete();
	}

// ---------------------------------------------------------------------------
// List the triggers in database based on filter
// ---------------------------------------------------------------------------
//
void CLbtDbTriggersManager::ListTriggersL( CLbtContainerListOptions* aFilter,
    									   RPointerArray < CLbtContainerTriggerEntry >& aTriggers,
    									   TLbtSecurityPolicy& aSecurityPolicy,
    									   TRequestStatus &aStatus )
    {
    FUNC_ENTER("CLbtDbTriggersManager::ListTriggersL");
    /** 
     * Here is the alogirthm to do list triggers. The List Triggers
     * has filter based on the various attributes of the trigger like,
     * State, Validity, ManagerUi, area, Trigger Id etc.
     * In order to list triggers based on these various constraints 
     * we would have to construct a nested query. But since Symbian DBMS does
     * not support nested queries, the idea is to implement this as follows.
     *
     * 1. For i = 1 to number of filter attributes in aFilter
     * 2.      Construct a SQL query based on the attribute[i] value list.
     * 3.      Execute this SQL query on the previously obtained view
     * 4.      Obtain a view that contains only rows that have value matching
     *             the value list for attribute[i] in aFilter.
     * 5. Continue the loop till i < number of filter attributes in aFilter.
     * 
     * So the idea here is to repeatedly execute the query on the filtered
     * list of triggers. Each time the query is executed on the already 
     * updated db view.
     *
     * As of now this is not the Implementation. TODO: Fix this algo later.
     */
     
    // This is just a safe check. While compaction is ongoing the
    // request is queued in container AO
    aStatus = KRequestPending;
    iClientReqStatus = &aStatus;
    
    if(iDbOperation->IsCompactionOngoing())
    	{
    	CompleteClientRequest( KErrServerBusy );
    	return;
    	}

    if( aFilter == NULL )
        {
        CompleteClientRequest( KErrArgument );
        return;
        }

    // Check if this filter is application for the type of triggers supported by DB
    CLbtListTriggerOptions* options = aFilter->ListOptions();    
    if(options)
    	{
		if( !IsFilterForDBTriggersL(options->Filter()) )
	    	{
	    	CompleteClientRequest( KErrNotFound );
	        return;
	    	}
    	}
    
    // Store the parameters first for later references
    iFilter = aFilter;
    iTriggers = &aTriggers;
    iClientSecurityPolicy = aSecurityPolicy;
    iCurrentOperation = EDbOpListTriggers;

    // Prepare view for listing
    PrepareViewForListingL( iView );
    
    // rest of list triggers operation is in the method HandleListTriggersEventL.
    
	if(!iView.FirstL())
   {
   	CompleteClientRequest( KErrNotFound );
	return;
	}
    iFilterBase = LbtContainerUtilities::GetContainerFilterFromListOptionsLC(iFilter);
    CleanupStack::Pop(1); // iFilterBase
    
    SelfComplete();    
    }

//---------------------------------------------------------------------------
// CLbtDbTriggersManager::UpdateTriggerL
//---------------------------------------------------------------------------
//
void CLbtDbTriggersManager::UpdateTriggerL( CLbtContainerTriggerEntry& aEntry,
    										TLbtTriggerDataMask aDataMask,
    										TLbtTriggerAttributeFieldsMask aAttrMask,
    										TLbtSecurityPolicy& aSecurityPolicy,
    										TRequestStatus& aStatus )	
    {
    FUNC_ENTER("CLbtDbTriggersManager::UpdateTriggerL");
    aStatus = KRequestPending;
    iClientReqStatus = &aStatus;    
    
    // This is just a safe check. While compaction is ongoing the
    // request is queued in container AO
    if(iDbOperation->IsCompactionOngoing())
    	{
    	CompleteClientRequest( KErrServerBusy );
    	return;
    	}

    // Save user provided data
    iClientSecurityPolicy = aSecurityPolicy;
    iClientReqStatus = &aStatus;
    iEntry = &aEntry;
    iDataMask = aDataMask;
    iAttrMask = aAttrMask;
    
    // Save the current operation and its state
    iCurrentOperation = EDbOpUpdateTriggers;
    iOperationState = EOpStateQuery;       
    iIdArray.Reset();
    
    // Get the tuple from common db for which the primary key is the trigger id
    TPtr sql = iSqlQuery->Des();
    sql.Zero();
    sql.Append( KSelect );
    sql.Append( KSelectAllFields );
    sql.Append( KFrom );
    sql.Append( KTriggersTable );
    sql.Append( KWhere );
    sql.Append( KLbtDbFieldId );
    sql.Append( KLike );
    sql.AppendNum(aEntry.TriggerEntry()->Id());
    
    // First close the view just as a safe check
    iView.Close();
    
    TInt error = iView.Prepare( iDb, TDbQuery( sql, EDbCompareFolded) );
    if( error != KErrNone )
        {
        iView.Close();
        CompleteClientRequest( error );
        }
    else
        {
        iView.Evaluate(iStatus);
        SetActive();
        }    
    }

//---------------------------------------------------------------------------
// CLbtDbTriggersManager::RemoveTriggerConditionFromDb
//---------------------------------------------------------------------------
//        
void CLbtDbTriggersManager::RemoveTriggerConditionFromDb( TLbtTriggerId aTriggerId, 
                                                          CLbtGeoAreaBase::TGeoAreaType aAreaType )
	{
	RDbView view;
	TPtr sql = iSqlQuery->Des();
	
	sql.Zero();
    sql.Append( KDelete );
    sql.Append( KFrom );
    
    switch( aAreaType )
        {
        case CLbtGeoAreaBase::ECircle:
            {
            sql.Append( KCircularTriggersTable );
            sql.Append( KWhere );
            sql.Append( KLbtCircTrigDbFieldId );
            break;
            }
        case CLbtGeoAreaBase::ECellular:
            {
            sql.Append( KCellTriggersTable );
            sql.Append( KWhere );
            sql.Append( KLbtCellTrigDbFieldId );
            break;
            }
        case CLbtGeoAreaBase::EHybrid:
            {
            sql.Append( KHybridTriggersTable );
            sql.Append( KWhere );
            sql.Append( KLbtHybridTrigDbFieldId );
            break;
            }
        }
    
    sql.Append( KLike );
    sql.AppendNum( aTriggerId );
    
    // Execute the query to delete the entry
    TInt err =iDbOperation->ExecuteSyncQuery( view, sql );
    if( err != KErrNone )
    	{
    		LOG("err");
    	}
 	   	view.Close();
       }

//---------------------------------------------------------------------------
// CLbtDbTriggersManager::AddGeoCellIntoDbL
//---------------------------------------------------------------------------
//        
void CLbtDbTriggersManager::AddGeoCellIntoDbL( CLbtTriggerEntry* aTriggerEntry )
	{
	// Prepare view for cellular trigger table
	// Create the trigger	
	CLbtTriggerConditionArea* cond=static_cast <CLbtTriggerConditionArea*> (aTriggerEntry->GetCondition());
	if( cond->TriggerArea()->Type() != CLbtGeoAreaBase::ECellular )
	    {
	    User::Leave( KErrArgument );
	    }
	
	CLbtGeoCell* cell = static_cast<CLbtGeoCell* >(cond->TriggerArea());    
	RDbView view;
    TPtr cellSql = iSqlQuery->Des();
    cellSql.Zero();
    cellSql.Append( KSelect );
    cellSql.Append( KSelectAllFields );
    cellSql.Append( KFrom );
    cellSql.Append( KCellTriggersTable );
    
    TInt err = iDbOperation->ExecuteSyncQuery(view, cellSql);    
    if( err != KErrNone )
    	{
    	view.Close();
    	User::Leave(err);
    	return;	
    	}
    CleanupClosePushL(view);

	view.InsertL();
	view.SetColL( ECellDbFieldId, static_cast<TInt64>(aTriggerEntry->Id()) );
	
	// insert MNC
	TLex lex( cell->NetworkCountryCode() );
	TUint netCountryCode;
	lex.Val( netCountryCode );		
	view.SetColL( ECellDbCountryCode, netCountryCode );
	
	lex.Assign( cell->NetworkIdentityCode() );
	TUint networkIdentityCode;
	lex.Val( networkIdentityCode );		
	view.SetColL( ECellDbNetworkId, networkIdentityCode );
	
    RMobilePhone::TMobilePhoneNetworkMode nwmode = cell->NetworkType();
    if(nwmode == RMobilePhone::ENetworkModeGsm) 
        {
        view.SetColL( ECellDbNetworkType, RMobilePhone::ENetworkModeGsm);
        }
    else if(nwmode == RMobilePhone::ENetworkModeWcdma) 
        {
        view.SetColL( ECellDbNetworkType, RMobilePhone::ENetworkModeWcdma);
        }
	
    view.SetColL( ECellDbLocationAreaCode, cell->LocationAreaCode());	
    view.SetColL( ECellDbTriggerCellId, cell->CellId());
    view.PutL();
    CleanupStack::PopAndDestroy(1); // view
	}
	
//---------------------------------------------------------------------------
// CLbtDbTriggersManager::AddGeoCircleIntoDbL
//---------------------------------------------------------------------------
//        
void CLbtDbTriggersManager::AddGeoCircleIntoDbL( CLbtTriggerEntry* aTriggerEntry )
	{
	CLbtTriggerConditionArea* cond = static_cast<CLbtTriggerConditionArea* >(aTriggerEntry->GetCondition());
	CLbtGeoAreaBase * areaBase = cond->TriggerArea();
	if( areaBase->Type() != CLbtGeoAreaBase::ECircle )
	    {
        User::Leave( KErrArgument );
	    }
	
	// Prepare view from circlar triggers table
	RDbView view;
	TPtr circSql = iSqlQuery->Des();
    circSql.Zero();
    circSql.Append( KSelect );
    circSql.Append( KSelectAllFields );
    circSql.Append( KFrom );
    circSql.Append( KCircularTriggersTable );

    TInt err = iDbOperation->ExecuteSyncQuery(view, circSql);    
    if( err != KErrNone )
    	{
    	view.Close();
    	User::Leave(err);
    	return;	
    	}
    CleanupClosePushL(view);
	
    CLbtGeoCircle* area = static_cast<CLbtGeoCircle* >(cond->TriggerArea());
    
    view.InsertL();
    view.SetColL( ECircleDbFieldId, static_cast<TInt64>(aTriggerEntry->Id()) );	
    view.SetColL( ECircleDbFieldCenterLat, area->Center().Latitude() );
    view.SetColL( ECircleDbFieldCenterLong, area->Center().Longitude() );
    view.SetColL( ECircleDbFieldRadius, area->Radius() );

	TReal defaultValue = -1;
	TReal32 alt = -1;
	view.SetColL(ECircleDbFieldTriggerFiredLat,defaultValue);
	view.SetColL(ECircleDbFieldTriggerFiredLong,defaultValue);
	view.SetColL(ECircleDbFieldTriggerFiredAlt,alt);    	
	view.SetColL(ECircleDbFieldTriggerFiredHorizAcc,alt);
	view.SetColL(ECircleDbFieldTriggerFiredVertAcc,alt);	
	view.PutL(); // Complete insertion
	CleanupStack::PopAndDestroy(1); // view
	}

//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
// CLbtDbTriggersManager::AddGeoHybridIntoDbL
//---------------------------------------------------------------------------
//        
void CLbtDbTriggersManager::AddGeoHybridIntoDbL( CLbtTriggerEntry* aTriggerEntry )
    {
    CLbtTriggerConditionArea* cond = static_cast<CLbtTriggerConditionArea* >(aTriggerEntry->GetCondition());
    CLbtGeoAreaBase * areaBase = cond->TriggerArea();
    
    if( areaBase->Type() != CLbtGeoAreaBase::EHybrid )
        {
        User::Leave(KErrArgument);
        }
    
    RDbView view;
    TPtr hybridSql = iSqlQuery->Des();
    hybridSql.Zero();
    hybridSql.Append( KSelect );
    hybridSql.Append( KSelectAllFields );
    hybridSql.Append( KFrom );
    hybridSql.Append( KHybridTriggersTable );
    
    TInt err = iDbOperation->ExecuteSyncQuery(view, hybridSql);    
    if( err != KErrNone )
        {
        view.Close();
        User::Leave(err);
        }
    CleanupClosePushL(view);
    view.InsertL();
    
    // cast to hybrid area
    CLbtGeoHybrid* hybrid = static_cast<CLbtGeoHybrid *>(cond->TriggerArea());
    
    // Set Primary key, trigger id.
    view.SetColL( EHybridDbFieldId ,static_cast<TInt32>(aTriggerEntry->Id()) );
    
    // Externalize the hybrid areas into the DB
    view.SetColNullL( EHybridDataStream );
    RDbColWriteStream writeStream;
    writeStream.OpenLC(view, EHybridDataStream);            
    const RPointerArray<CLbtGeoAreaBase>& hybridAreas = hybrid->HybridArea();            
    // Write the count into the the stream
    writeStream.WriteInt32L( hybridAreas.Count() );
    for( TInt i=0; i<hybridAreas.Count(); ++i )
        {
        // Write the area type
        writeStream.WriteInt32L( hybridAreas[i]->Type() );
        
        // Externalize the area data
        hybridAreas[i]->ExternalizeL(writeStream);
        }
    writeStream.CommitL();
    writeStream.Close();    
    view.PutL();            
    CleanupStack::PopAndDestroy( 2 ); // view, writeStream    
    }

//---------------------------------------------------------------------------
// CLbtDbTriggersManager::UpdateTriggersState
//---------------------------------------------------------------------------
//        
void  CLbtDbTriggersManager::UpdateTriggersStateL( CLbtTriggerEntry::TLbtTriggerState aState,
												   CLbtContainerUpdateFilter* aFilter,
												   TLbtFireOnUpdate aFireOnUpdate,
												   TLbtSecurityPolicy& aSecurityPolicy,
												   TRequestStatus& aStatus )
    {
    FUNC_ENTER("CLbtDbTriggersManager::UpdateTriggersState");
    aStatus = KRequestPending;
    iClientReqStatus = &aStatus;   
    
    // This is just a safe check. While compaction is ongoing the
    // request is queued in container AO
    if( iDbOperation->IsCompactionOngoing() )
    	{    	 	
    	CompleteClientRequest( KErrServerBusy );
    	return;
    	}
   
    if(aFilter==NULL)
        {    
        CompleteClientRequest( KErrArgument );   
        return;
        }
    
    // Save user provided data
     iClientSecurityPolicy = aSecurityPolicy;    
     iState = aState;
     iFireOnUpdate = aFireOnUpdate;
     iFilterBase = LbtContainerUtilities::GetContainerFilterFromUpdateFilterLC( aFilter );
     CleanupStack::Pop(1); // iFilterBase
     
     // Save the current operation and its state
     iCurrentOperation = EDbOpUpdateTriggersState;
     iOperationState = EOpStateQuery;       
     iIdArray.Reset();
     
     TInt error = AsyncPrepareViewOfCommonTable();
            
     if( error != KErrNone )
        {
        CompleteClientRequest( error );
        }
    }

// --------------------------------------------------------------------------------------------
// CLbtDbTriggersManager::UpdateTriggersValidity
// --------------------------------------------------------------------------------------------
//                    
void CLbtDbTriggersManager::UpdateTriggersValidityL( TLbtTriggerDynamicInfo::TLbtTriggerValidity aValidity,
												     RArray <TLbtTriggerId>& aTriggerIds,
												     TLbtSecurityPolicy& aSecurityPolicy,
												     TRequestStatus& aStatus )
    {
    FUNC_ENTER("CLbtDbTriggersManager::UpdateTriggersValidity");
    TRequestStatus* status = &aStatus;
    aStatus = KRequestPending;
    
    // This is just a safe check. While compaction is ongoing the
    // request is queued in container AO
    if(iDbOperation->IsCompactionOngoing())
        {       
        User::RequestComplete(status, KErrServerBusy);
        return;
        }
    
    if(aTriggerIds.Count() == 0)
        {
        User::RequestComplete(status, KErrArgument);
        return;
        }

    iIdArray.Reset();
    RDbView view;
     CleanupClosePushL( view );
    PrepareViewForTriggersL( aTriggerIds, view );
    if(!view.FirstL())
	   {
	     CompleteClientRequest( KErrNotFound );
       return;
	   }
   
    
    while(view.AtRow())
        {
        view.GetL();
        TLbtTriggerId triggerId = view.ColInt64(ELbtDbFieldId);
        TInt index = aTriggerIds.Find(triggerId);
        
        if( KErrNotFound != index )
            {
            // This trigger is found in this db. So remove the trigger from the list
            aTriggerIds.Remove(index);

            // Check if the trigger satisfies the security policy
            TUid ownerSid;
            TUid managerUi;
            TSecureId startupSid;
            
            ownerSid.iUid = view.ColUint32(ELbtDbFieldOwnerSid);
            managerUi.iUid = view.ColUint32(ELbtDbFieldManagerUiSid);
            startupSid.iId = view.ColUint32(ELbtDbFieldStartupSid);
            
            if( LbtContainerUtilities::RunSecurityPolicy( ownerSid,
                                                          managerUi,
                                                          startupSid,
                                                          aSecurityPolicy ) )
                {
                TLbtTriggerDynamicInfo::TLbtTriggerValidity currentValidity= 
                        static_cast<TLbtTriggerDynamicInfo::TLbtTriggerValidity>(view.ColInt8(ELbtDbFieldValidity));
                
                if( currentValidity != aValidity )
                    {
                    view.UpdateL();
                    view.SetColL( ELbtDbFieldValidity, static_cast<TInt8>(aValidity) );
                    view.PutL();
                    
                    TLbtTriggerModifiedInfo info;
                    info.iTriggerId = triggerId;
                    info.iAreaType = static_cast<CLbtGeoAreaBase::TGeoAreaType>(view.ColInt8(ELbtDbTriggerAreaType));;
                    iIdArray.AppendL(info);
                    
                    if( currentValidity == TLbtTriggerDynamicInfo::EInvalid &&
                        aValidity == TLbtTriggerDynamicInfo::EValid)
                        {
                        iCountOfEnabledAndValidTrigger++;
                        }
                    else if(currentValidity == TLbtTriggerDynamicInfo::EValid &&
                        aValidity == TLbtTriggerDynamicInfo::EInvalid)
                        {
                        iCountOfEnabledAndValidTrigger--;
                        }
                    }
                }
            }
        if(!view.NextL())
        	break;
        }
    CleanupStack::PopAndDestroy(); // view
    User::RequestComplete( status, KErrNone );
    }


// --------------------------------------------------------------------------------------------
// CLbtDbTriggersManager::UpdateTriggerFiredStateL
// --------------------------------------------------------------------------------------------
//    
void CLbtDbTriggersManager::UpdateTriggerFiredStateL( RArray<TLbtTriggerId>& aTriggerIds,
													  TBool aFireBool,
								                      TRequestStatus& aStatus )
	{
	FUNC_ENTER("CLbtDbTriggersManager::UpdateTriggerFiredStateL");
    TRequestStatus* status = &aStatus;
    aStatus = KRequestPending;
    
    // This is just a safe check. While compaction is ongoing the
    // request is queued in container AO
    if(iDbOperation->IsCompactionOngoing())
    	{
    	User::RequestComplete(status, KErrServerBusy);
    	return;
    	}
    
    if(aTriggerIds.Count() == 0)
    	{
    	User::RequestComplete(status, KErrArgument);
    	return;
    	}
    
    RDbView view;
    PrepareViewForTriggersL( aTriggerIds, view );

  	TBool found = EFalse;
  	CleanupClosePushL( view );
  	
	if(!view.FirstL())
	{
	CompleteClientRequest( KErrNotFound );
    return;
	}
  	while(view.AtRow())
    	{
    	view.GetL();
    	TLbtTriggerId triggerId = view.ColInt64(ELbtDbFieldId);
    	
    	if( KErrNotFound != aTriggerIds.Find(triggerId) )
    		{
    		TBool currentFiredFlag = view.ColInt(ELbtDbFieldIsTriggerFired);
 	        found = ETrue;
	 	    if(currentFiredFlag != aFireBool)
	 	    	{
	 	    	view.UpdateL();	
	 	    	view.SetColL( ELbtDbFieldIsTriggerFired, aFireBool );
	 	    	view.PutL();
	 	    	}
    		}
    	if(!view.NextL())
    		break;
    			
    }
    CleanupStack::PopAndDestroy(); //view

    TInt error = KErrNone;
    if(!found)
    	{
    	error = KErrNotFound;
    	}
    User::RequestComplete(status, error);
	}
    
// --------------------------------------------------------------------------------------------
// CLbtDbTriggersManager::DeleteTriggersL
// --------------------------------------------------------------------------------------------
//
void CLbtDbTriggersManager::DeleteTriggersL( CLbtContainerUpdateFilter* aFilter,
											 TLbtSecurityPolicy& aSecurityPolicy,
    										 TRequestStatus& aStatus )
    {
    FUNC_ENTER("CLbtDbTriggersManager::DeleteTriggersL");
    
    // Save user provided data
    iClientSecurityPolicy = aSecurityPolicy;
    iClientReqStatus = &aStatus;
    
    // This is just a safe check. While compaction is ongoing the
    // request is queued in container AO
    if(iDbOperation->IsCompactionOngoing())
    	{
    	CompleteClientRequest( KErrServerBusy );
    	return;
    	}
    	
    if(aFilter == NULL)
        {        
        CompleteClientRequest( KErrArgument );
        return;
        }
    
    if( !IsFilterForDBTriggersL(aFilter->TrigInfoFilter()) )
    	{
    	CompleteClientRequest( KErrNotFound );
        return;
    	}
    
    TInt error = AsyncPrepareViewOfCommonTable();            
    if( error != KErrNone )
        {
        CompleteClientRequest( error );
        return;
        }
    
    
    iFilterBase = LbtContainerUtilities::GetContainerFilterFromUpdateFilterLC( aFilter );
    CleanupStack::Pop(1); // iFilterBase
    
    // Save the current operation and its state
    iCurrentOperation = EDbOpDeleteTriggers;
    iOperationState = EOpStateQuery;    
	iIdArray.Reset();    
    }

// --------------------------------------------------------------------------
// CLbtDbTriggersManager::SelfComplete
// --------------------------------------------------------------------------
// 
void CLbtDbTriggersManager::SelfComplete()
    {
    TRequestStatus* status = &iStatus;
    iStatus = KRequestPending;
    SetActive();
    User::RequestComplete(status, KErrNone);
    }
  

// --------------------------------------------------------------------------
// CLbtDbTriggersManager::DeleteTriggerL
// --------------------------------------------------------------------------
// 
void CLbtDbTriggersManager::DeleteTriggerL( TLbtTriggerId aTriggerId )
    {
    FUNC_ENTER("CLbtDbTriggersManager::DeleteTriggerL");
        
    CLbtGeoAreaBase::TGeoAreaType areaType = 
    			static_cast<CLbtGeoAreaBase::TGeoAreaType>( iView.ColInt8(ELbtDbTriggerAreaType) );
    
    switch( areaType )
        {
        case CLbtGeoAreaBase::ECircle:
            {
            TPtr circSql = iSqlQuery->Des();
            RDbView circView;
            circSql.Zero();
            circSql.Append( KSelect );
            circSql.Append( KSelectAllFields );
            circSql.Append( KFrom );
            circSql.Append( KCircularTriggersTable );
            circSql.Append( KWhere );
            circSql.Append( KLbtCircTrigDbFieldId );
            circSql.Append( KLike );
            circSql.AppendNum(aTriggerId);

            TInt error = iDbOperation->ExecuteSyncQuery( circView, circSql );      
            if(error != KErrNone)
                {
                circView.Close();
                User::Leave(error);
                }
            CleanupClosePushL(circView);
            circView.FirstL(); 
            circView.GetL();
            circView.DeleteL();
            circView.Close();
            CleanupStack::PopAndDestroy(1); // circView
            break;
            }
        case CLbtGeoAreaBase::ECellular:
            {
            TPtr cellSql = iSqlQuery->Des();
            cellSql.Zero();
            cellSql.Append( KSelect );
            cellSql.Append( KSelectAllFields );
            cellSql.Append( KFrom );
            cellSql.Append( KCellTriggersTable );
            cellSql.Append( KWhere );
            cellSql.Append( KLbtCellTrigDbFieldId );
            cellSql.Append( KLike );
            cellSql.AppendNum(aTriggerId);
            
            RDbView cellView;
            TInt error = iDbOperation->ExecuteSyncQuery( cellView, cellSql );      
            if(error != KErrNone)
                {
                cellView.Close();
                User::Leave(error);
                }
            CleanupClosePushL(cellView);
            cellView.FirstL();
            cellView.GetL();
            cellView.DeleteL();
            cellView.Close();
            CleanupStack::PopAndDestroy(1); // cellView
            break;
            }
        case CLbtGeoAreaBase::EHybrid:
            {
            TPtr cellSql = iSqlQuery->Des();
            cellSql.Zero();
            cellSql.Append( KSelect );
            cellSql.Append( KSelectAllFields );
            cellSql.Append( KFrom );
            cellSql.Append( KHybridTriggersTable );
            cellSql.Append( KWhere );
            cellSql.Append( KLbtHybridTrigDbFieldId );
            cellSql.Append( KLike );
            cellSql.AppendNum(aTriggerId);
            
            RDbView hybridView;
            TInt error = iDbOperation->ExecuteSyncQuery( hybridView, cellSql );      
            if(error != KErrNone)
                {
                hybridView.Close();
                User::Leave(error);
                }
            CleanupClosePushL(hybridView);
            if(hybridView.FirstL())
            	{
            hybridView.GetL();
            hybridView.DeleteL();
            hybridView.Close();
              }
            CleanupStack::PopAndDestroy(1); // hybridView            
            break;
            }
        }

    if(iIdArray.Count() == 0)
    	{
    	MLbtTriggerStore::TLbtTriggerModifiedInfo info;
    	info.iTriggerId = aTriggerId;
    	info.iAreaType = areaType;
    	iIdArray.AppendL(info);
    	}
    
    iView.DeleteL();
    }

// ---------------------------------------------------------------------------
// CLbtDbTriggersManager::RunL
// ---------------------------------------------------------------------------
//
void CLbtDbTriggersManager::RunL()
    {
    if( iStatus.Int() != KErrNone )
        {
        CompleteClientRequest( iStatus.Int() );
        return;
        }
    
    switch( iCurrentOperation )
        {
        case EDbOpCreateTrigger:
            {
            HandleTriggerCreationEventL();
            break;
            }
        case EDbOpDeleteTriggers:
            {
            HandleTriggersDeletionEventL();
            break;
            }
        case EDbOpListTriggers:
            {
            HandleListTriggersEventL();
            break;
            }
        case EDbOpGetTriggers:
           {
           HandleGetTriggersEventL();
           break;
           }
        case EDbOpUpdateTriggersState:
            {
            HandleTriggerStateUpdationEventL();
            break;
            }
        case EDbOpUpdateTriggers:
           {
           HandleTriggerUpdationEventL();
           break;
           }
        default:
            {
            //CompleteClientRequest( KErrGeneral );
            break;
            }
        }
    }

// ---------------------------------------------------------------------------
// CLbtDbTriggersManager::RunError
// ---------------------------------------------------------------------------
//
TInt CLbtDbTriggersManager::RunError( TInt aError )
    {
    CompleteClientRequest( aError );
    return KErrNone;
    }

// ---------------------------------------------------------------------------
// CLbtDbTriggersManager::DoCancel
// ---------------------------------------------------------------------------
//
void CLbtDbTriggersManager::DoCancel()
    {
    switch( iOperationState )
        {
        case EOpCompaction:
            {
            // Cancel compaction if ongoing
            iDbOperation->Cancel();
            break;
            }
        case EOpStateQuery:
            {
            iView.Cancel();
            break;
            }
        case EOpStateIterating:
        default:
            {
            break;
            }
        }
    }

// ---------------------------------------------------------------------------
// CLbtDbTriggersManager::PrepareViewForTriggersL
// ---------------------------------------------------------------------------
//    
void CLbtDbTriggersManager::PrepareViewForTriggersL( const RArray<TLbtTriggerId>& aTriggerIds, RDbView& aView )
	{
	FUNC_ENTER("CLbtDbTriggersManager::PrepareViewForTriggersL");
	TPtr sql = iSqlQuery->Des();
    sql.Zero();
    
    if( !aTriggerIds.Count() )
    	{
    	User::Leave( KErrArgument );
    	return;
    	}
	
	sql.Append( KSelect );
    sql.Append( KSelectAllFields );
    sql.Append( KFrom );
    sql.Append( KTriggersTable );
    sql.Append( KWhere );
	sql.Append( KLbtDbFieldId );
	
    if( aTriggerIds.Count() == 1 )
    	{
	    sql.Append( KLike );
	    sql.AppendNum( aTriggerIds[0] );
    	}
    else
    	{
    	// Find the max and min of trigger ids so that we get only thos triggers
    	// that fall into that bracket
    	TInt min = 0xFFFFFFFF;
    	TInt max = KLbtNullTriggerId;
    	
    	for(TInt i=0;i<aTriggerIds.Count();++i)
    		{
    		if(aTriggerIds[i] < min)
    			{
    			min = aTriggerIds[i];
    			}

    		if(aTriggerIds[i] > max)
    			{
    			max = aTriggerIds[i];
    			}
    		}
    	
    	sql.Append( KLessThanEqualTo );
    	sql.AppendNum( max );
    	sql.Append( KAnd );
    	sql.Append( KLbtDbFieldId );
    	sql.Append( KGreaterThanEqualTo );
    	sql.AppendNum( min );
    	}
    
    TInt err = iDbOperation->ExecuteSyncQuery(aView, sql);    
    if( err != KErrNone )
    	{
    	aView.Close();
    	User::Leave(err);
    	return;	
    	}
	}

// ---------------------------------------------------------------------------
// CLbtDbTriggersManager::AsyncPrepareViewForTriggersL
// ---------------------------------------------------------------------------
//    
TInt CLbtDbTriggersManager::AsyncPrepareViewForTriggers( const RArray<TLbtTriggerId>& aTriggerIds, RDbView& /*aView*/ )
    {
    FUNC_ENTER("CLbtDbTriggersManager::PrepareViewForTriggersL");
    TPtr sql = iSqlQuery->Des();
    sql.Zero();
    
    if( !aTriggerIds.Count() )
        {
        return KErrArgument;
        }
    
    sql.Append( KSelect );
    sql.Append( KSelectAllFields );
    sql.Append( KFrom );
    sql.Append( KTriggersTable );
    sql.Append( KWhere );
    sql.Append( KLbtDbFieldId );
    
    if( aTriggerIds.Count() == 1 )
        {
        sql.Append( KLike );
        sql.AppendNum( aTriggerIds[0] );
        }
    else
        {
        // Find the max and min of trigger ids so that we get only thos triggers
        // that fall into that bracket
        TInt min = 0xFFFFFFFF;
        TInt max = KLbtNullTriggerId;
        
        for(TInt i=0;i<aTriggerIds.Count();++i)
            {
            if(aTriggerIds[i] < min)
                {
                min = aTriggerIds[i];
                }

            if(aTriggerIds[i] > max)
                {
                max = aTriggerIds[i];
                }
            }
        
        sql.Append( KLessThanEqualTo );
        sql.AppendNum( max );
        sql.Append( KAnd );
        sql.Append( KLbtDbFieldId );
        sql.Append( KGreaterThanEqualTo );
        sql.AppendNum( min );
        }
   iView.Close();
   
   TInt err = iView.Prepare( iDb, TDbQuery(sql, EDbCompareFolded),KDbUnlimitedWindow );
   // This should evaluate the query fully
   if ( err == KErrNone )
       {
       iView.Evaluate(iStatus);
       SetActive();
       return KErrNone;
       }
   else
       {
       iView.Close();
       return err;
       }
    }

// ---------------------------------------------------------------------------
// CLbtDbTriggersManager::IsFilterForDBTriggersL
// ---------------------------------------------------------------------------
//    
TBool CLbtDbTriggersManager::IsFilterForDBTriggersL( CLbtTriggerFilterBase* aFilter )
	{
	if( aFilter == NULL )
		{
		return ETrue;
		}
	
	if( aFilter->Type() == CLbtTriggerFilterBase::EFilterByAttribute )
		{
		CLbtTriggerFilterByAttribute* attributeFilter = 
			static_cast<CLbtTriggerFilterByAttribute*>(aFilter);
		
		RArray<CLbtTriggerEntry::TType> triggerTypeArray;
		attributeFilter->GetTypeArrayL( triggerTypeArray );
		
		if( triggerTypeArray.Count() )
			{
			// If the trigger type does not have startup triggers then return EFalse
			if( !attributeFilter->IsTriggerTypeInFilter(CLbtTriggerEntry::ETypeStartup) )
				{
				return EFalse;
				}
			}
		triggerTypeArray.Close();
		}	
	return ETrue;
	}

// ---------------------------------------------------------------------------
// CLbtDbTriggersManager::PrepareViewForListingL
// ---------------------------------------------------------------------------
//    
void CLbtDbTriggersManager::PrepareViewForListingL( RDbView& aView )
	{
	FUNC_ENTER("CLbtDbTriggersManager::PrepareViewsL");
	TPtr sql = iSqlQuery->Des();
    sql.Zero();
    sql.Append( KSelect );
    sql.Append( KSelectAllFields );
    sql.Append( KFrom );
    sql.Append( KTriggersTable );
    // As of now,create trigger operation on db side is implemented synchronously.So using Synchronous version.
    
    TInt err = iDbOperation->ExecuteSyncQuery( aView, sql );    
    if( err != KErrNone )
    	{
    	aView.Close();
    	User::Leave(err);
    	return;	
    	}
	}
    
// ---------------------------------------------------------------------------
// Callback method that gets called when SQL Evaluation is completed.
// ---------------------------------------------------------------------------
//
void CLbtDbTriggersManager::DbSqlOperationCompletedL( TInt error )
    {
    FUNC_ENTER("CLbtDbTriggersManager::DbSqlOperationCompleted");
    switch( iCurrentOperation )
        {
        case EDbOpCreateTrigger:
            {
            if ( KErrNone == error )
                {
                TRAP(error, CreateTriggerInViewL());
                }
            User::RequestComplete(iClientReqStatus, error);
            iClientReqStatus = NULL;
            
            if(KErrNone == error)
            	{
            	CompactDbIfRequiredL();
            	}
            	
            iCurrentOperation = EDbOpNone;
            }
            break;
            
        default:
            break;
        }
    }

// ---------------------------------------------------------------------------
// CLbtDbTriggersManager::AppendTriggerInfo
//
// Appends the trigger info into the triggers modified array which is used for
// notifying observers
// ---------------------------------------------------------------------------
//
void CLbtDbTriggersManager::AppendTriggerInfo(CLbtContainerTriggerEntry* aEntry)
	{
	FUNC_ENTER("CLbtDbTriggersManager::AppendTriggerInfo");
	const CLbtTriggerConditionArea* conditionBase = 
			static_cast<const CLbtTriggerConditionArea*>(aEntry->TriggerEntry()->GetCondition());
	CLbtGeoAreaBase* areaBase = conditionBase->TriggerArea();
		    
    TLbtTriggerModifiedInfo info;
    info.iTriggerId = aEntry->TriggerEntry()->Id();
    info.iAreaType = areaBase->Type();
    info.iManagerUi = aEntry->TriggerEntry()->ManagerUi();
    
    TSecureId sid = aEntry->ExtendedTriggerInfo()->OwnerSid();
    TUid ownerUid;
    ownerUid.iUid = (TInt)(sid.iId);
    info.iOwner = ownerUid;
    
    CLbtTriggerEntry* triggerEntry = aEntry->TriggerEntry();	
	if( triggerEntry->Type() == CLbtTriggerEntry::ETypeStartup)
		{
		CLbtStartupTrigger* startupTrigger = static_cast<CLbtStartupTrigger*>(triggerEntry);
		TSecureId secureId;
		TBuf16<KMaxFileName> fileName;
		startupTrigger->GetProcessId(fileName, secureId);
		TUid startupUid;
		startupUid.iUid = (TInt)(sid.iId);
		info.iStartupProcess = startupUid;
		}
	else
		{
		info.iStartupProcess = KNullUid;
		}
        
    TInt error = iIdArray.Append(info);
    if( error != KErrNone )
        {
        LOG1("Failed to appenf info to the array:%d",error);
        }
	}
	

// -----------------------------------------------------------------------------
// CLbtDbTriggersManager::TriggersModified
// -----------------------------------------------------------------------------
// 
void CLbtDbTriggersManager::TriggersModified(RArray<TLbtTriggerModifiedInfo>& aArray)
	 {
	 for( TInt i = 0;i<iIdArray.Count();i++ )
		 {
		 TInt error = aArray.Append(iIdArray[i]);
	      if( error != KErrNone )
             {
             LOG1("Failed to append modified triggers:%d",error);
             return;
             }
	     }
	 iIdArray.Reset();
	 } 
 
// -----------------------------------------------------------------------------
// CLbtDbTriggersManager::ProcessListTriggers
// -----------------------------------------------------------------------------
//
void CLbtDbTriggersManager::ProcessListTriggers( CLbtContainerTriggerEntry* aEntry, 
 												 TLbtTriggerAttributeFieldsMask aAttrMask,
 												 TLbtTriggerDynamicInfoFieldsMask aDynInfoMask,
 												 TLbtTriggerDataMask aDataMask )
 	{
 	FUNC_ENTER("CLbtDbTriggersManager::ProcessListTriggers");
 	TLbtTriggerDynamicInfo* info = aEntry->DynInfo();
    
    CLbtStartupTrigger* clientEntry = static_cast<CLbtStartupTrigger*>( aEntry->TriggerEntry() );

    if( !(aDynInfoMask & TLbtTriggerDynamicInfo::EValidityStatus) )
        {
    	info->iValidity = TLbtTriggerDynamicInfo::EInvalid;
    	}
  
    if( !(aDynInfoMask & TLbtTriggerDynamicInfo::EDistanceToLatestLocation) )
        {
    	info->iDistanceToLatestLocation = 0;
    	}  
        
    if( !(aAttrMask & CLbtTriggerEntry::EAttributeId ) )
    	{
    	clientEntry->SetId(0);
    	}
                    	        
    if( !(aAttrMask & CLbtTriggerEntry::EAttributeName) )
    	{
    	TRAP_IGNORE(clientEntry->SetNameL(KNullDesC));    	
    	}   
                    	        
    if( !(aAttrMask & CLbtTriggerEntry::EAttributeState) )
    	{    	
    	clientEntry->SetState(CLbtTriggerEntry::EStateEnabled);    	
    	} 
    	
    if( !(aAttrMask & CLbtTriggerEntry::EAttributeRequestor) )
    	{
    	// TBD
    	}
                            	
    if( !(aAttrMask & CLbtTriggerEntry::EAttributeManagerUi) )
    	{
        TUid managerUi = TUid::Null();
    	clientEntry->SetManagerUi(managerUi);
    	}
                            	
    if( !(aAttrMask & CLbtTriggerEntry::EAttributeCondition) )
    	{
    	clientEntry->SetCondition(NULL);
    	}
                            	
	if( !(aAttrMask & CLbtTriggerEntry::EAttributeStartUpProcessId) )
		{
		TSecureId sid(0);		
	    clientEntry->SetProcessId( KNullDesC, sid );
		}

	if( !(aDataMask & CLbtContainerTriggerEntry::EContainerAttributeRectTriggerArea) )
		{
	
		}
 	}
 	
//-------------------------------------------------------------------------------------------------
// CLbtDbTriggersManager::CompactDbIfRequiredL
//-------------------------------------------------------------------------------------------------
//
void CLbtDbTriggersManager::CompactDbIfRequiredL()
	{
	FUNC_ENTER("CLbtDbTriggersManager::CompactDbIfRequiredL");
	// First update the stats to get the accurate db size information
	User::LeaveIfError(iDb.UpdateStats());
	RDbDatabase::TSize size = iDb.Size();
	TReal wastedBytes = size.iSize * static_cast<TReal>(100 - size.iUsage)/100;
			
	// Check if the amount of legitimate data present in less than the minimum usage percentage
	if( size.iUsage < KDatabaseMinUsagePercentage &&
	    wastedBytes > KDatabaseMaxWastageBytes )
		{
		TTime currentTime;
		
		// Get the current time
		currentTime.UniversalTime();
		
		// Find the seconds we have to compact
		TTimeIntervalSeconds seconds;
		
		TInt error = currentTime.SecondsFrom(iTimeTillCompaction, seconds);
		
		// If error is not equal to KErrNone that means that iTimeTillCompaction has not been set appropriately
		if(seconds > KSecondsNeededForCompaction || error != KErrNone)
			{
			// Compaction is needed
			iDbOperation->CompactDb();
			}
		}
	}

//-------------------------------------------------------------------------------------------------
// CLbtDbTriggersManager::HandleListTriggersEventL
//-------------------------------------------------------------------------------------------------
// 
void CLbtDbTriggersManager::HandleListTriggersEventL( )
    {    
    FUNC_ENTER("CLbtDbTriggersManager::HandleListTriggersEventL");
    /**
     * Once control reaches here it means that the Db Query
     * "Select * From table Triggers" has been executed and
     * the view contains all the rows.
     */

    CLbtListTriggerOptions* listOptions = iFilter->ListOptions();
    TLbtTriggerAttributeFieldsMask attrMask;
    TLbtTriggerDynamicInfoFieldsMask dynInfoMask;
    listOptions->GetRetrievedFields(attrMask,dynInfoMask);
    TLbtTriggerDataMask dataMask = iFilter->DataMask();    

    TUid ownerSid;
    TUid managerUi;
    TSecureId startupSid;
    TInt count = 0;
    while( iView.AtRow() && count < KStepCount )
    	{
    	++count;
    	iView.GetL();
		ownerSid.iUid = iView.ColUint32(ELbtDbFieldOwnerSid);
    	managerUi.iUid = iView.ColUint32(ELbtDbFieldManagerUiSid);
		startupSid.iId = iView.ColUint32(ELbtDbFieldStartupSid);
    	
    	if( LbtContainerUtilities::RunSecurityPolicy( ownerSid,
    												  managerUi,
    												  startupSid,
    												  iClientSecurityPolicy ) )
    		{
    		CLbtContainerTriggerEntry* entry = RetrieveTriggerEntryFromDbL( iView );    	
	    	TInt isFilterPresent = 0;
	    	TBool isEntryRequested = EFalse;	    	
	    	iFilterBase->ProcessFilter(entry, isFilterPresent, isEntryRequested);

	    	if(isFilterPresent > 0 && isEntryRequested)
	    		{
	    		ProcessListTriggers(entry,attrMask,dynInfoMask,dataMask);
	    		iTriggers->Append(entry);
	    		}
	    	else
	    		{
	    		delete entry;
	    		}
    		}

    	if(!iView.NextL())
    		break;
    	}
    
    if( count >= KStepCount )
        {
        // Self complete to process next request
        SelfComplete();
        }
    else // Operation is over
	    {
	    if(iTriggers->Count() == 0)
	        {
	        CompleteClientRequest( KErrNotFound );
	        }
	    else
	        {
	        CompleteClientRequest( KErrNone );
	        }
	    }
    }


// ---------------------------------------------------------------------------
// Callback method that is called when an Incremental operation is executed
// on the database (like database compaction, recovery)
// ---------------------------------------------------------------------------
//
void CLbtDbTriggersManager::DbIncrementalOperationCompleted( TInt /*aError*/ )
    {
    FUNC_ENTER("CLbtDbTriggersManager::DbIncrementalOperationCompleted");
    switch( iCurrentOperation )
        {
        case EDbOpCreateTrigger:
            {
            // Even if there is an error, it just means that Db Compaction
            // failed. So lets try Create trigger once more even in case of
            // failure. There is a remote possibility that some other process
            // might have done some cleanup and created enough space for this
            // trigger. So ignoring the error from compaction action here
            // deliberately.
            TInt error = KErrNone;
            TRAP( error, HandleTriggerCreationEventL() );
            if( error != KErrNone )
                {
                CompleteClientRequest( error );
                }
            }
            break;
        default:
        	{
        	// Notfy the client that we are ready to serve requests again
        	iObserver->NotifyTriggerStoreReadyToServe();
        	break;
        	}            
        }
    }

// ---------------------------------------------------------------------------
// Constructor
// ---------------------------------------------------------------------------
//
CLbtDbTriggersManager::CLbtDbTriggersManager( )
    : CActive(CActive::EPriorityStandard),
      iDbOperation( NULL ),
      iSqlQuery( NULL ),
      iCurrentOperation( EDbOpNone ),
      iOperationState( EOpStateNone ),
      iFilterBase( NULL )
    {
    CActiveScheduler::Add(this);
    iIdArray.Reset();
    iCountOfEnabledAndValidTrigger=0;
    }


// ---------------------------------------------------------------------------
// The 2nd phase Symbian Constructor
// ---------------------------------------------------------------------------
//
void CLbtDbTriggersManager::ConstructL()
    {
    FUNC_ENTER("CLbtDbTriggersManager::ConstructL");
    
    // db has not been opened yet
    iIsDbOpened = EFalse;
    
    // Can't do much if the file server open fails. Hence just leave
    // with that error code.
    User::LeaveIfError(iFs.Connect());
    
    // Obtain the Db file path
    TFileName dbFile;
    iFs.SessionPath(dbFile);

    // Create the Database Directory ie the private directory of the process
    iFs.MkDirAll(dbFile);
    
    // Generate the Db file
    dbFile.Append(KLbtDbName);
    
    // Initialize the Database
    TInt error = iDb.Open(iFs, dbFile);
        
    if ( error == KErrNotFound )
        {
        // The Db does not exist. Create the Db file
        User::LeaveIfError(iDb.Create(iFs, dbFile));
        CreateTablesInDbL();
        }
    else
        {
        User::LeaveIfError( error );
        }
    
    // Successfully opened db
    iIsDbOpened = ETrue;
    
    // Create the Db Operation Active Object
    iDbOperation = CLbtDbOperationAO::NewL(iDb, this);

    // Allocate the SQL Query String    
    iSqlQuery = HBufC::NewL(KMaxSqlQueryLength);
    
    iObserver = NULL;
    
    // List the set of all enabled and valid triggers    
    RDbView view;
    TPtr sql = iSqlQuery->Des();
    sql.Zero();
    sql.Append( KSelect );
    sql.Append( KSelectAllFields );
    sql.Append( KFrom );
    sql.Append( KTriggersTable );
    sql.Append( KWhere );
    sql.Append( KLbtDbFieldValidity );
    sql.Append( KLike );
    sql.AppendNum( TLbtTriggerDynamicInfo::EValid );
    sql.Append( KAnd );
    sql.Append( KLbtDbFieldState );
    sql.Append( KLike );
    sql.AppendNum( CLbtTriggerEntry::EStateEnabled );
    
    CleanupClosePushL( view );
    User::LeaveIfError( iDbOperation->ExecuteSyncQuery( view, sql ) );
    iCountOfEnabledAndValidTrigger = view.CountL();
    CleanupStack::PopAndDestroy(1); // view
    }


// ---------------------------------------------------------------------------
// Create the Table in the Database as part of initialization of Container
// ---------------------------------------------------------------------------
//
void CLbtDbTriggersManager::CreateTablesInDbL( )
    {
    FUNC_ENTER("CLbtDbTriggersManager::CreateTablesInDbL");
    CreateCommonTriggerAttributesTableL();
    CreateCoordinateTriggersTableL();
    CreateCellTriggersTableL();	
	CreateHybridTriggersTableL();
    }


// --------------------------------------------------------------------------------------------
// CLbtDbTriggersManager::CreateCommonTriggerAttributesTableL
// --------------------------------------------------------------------------------------------
void CLbtDbTriggersManager::CreateCommonTriggerAttributesTableL()
    {
    CDbColSet* triggerColSet = CDbColSet::NewLC();
        
    // Id
    triggerColSet->AddL(TDbCol(KLbtDbFieldId, EDbColInt64));
    
    // Name
    triggerColSet->AddL(TDbCol(KLbtDbFieldName, EDbColText16, KLbtMaxNameLength - 1));
    
    // Trigger Type
    triggerColSet->AddL(TDbCol(KLbtDbFieldType, EDbColInt8));
    
    // direction of Trigger (Entry type or Exit type)
    triggerColSet->AddL(TDbCol(KLbtDbFieldDirection, EDbColInt8));

   
    // SID of the Trigger Creator
    triggerColSet->AddL(TDbCol(KLbtDbFieldOwnerSid, EDbColUint32));
    
    // SID of the Manager UI for this trigger
    triggerColSet->AddL(TDbCol(KLbtDbFieldManagerUiSid, EDbColUint32));
    
    // Time to rearm a trigger
    triggerColSet->AddL(TDbCol(KLbtDbFieldRearmTime, EDbColInt32));
        
    // SID of the trigger handling process
    triggerColSet->AddL(TDbCol(KLbtDbFieldStartupSid, EDbColUint32));
    
    // Absolute path of the trigger handling process 
    triggerColSet->AddL(TDbCol(KLbtDbFieldStartupPath, EDbColText16));
    
    // Commandline parameters for the trigger handling process
    triggerColSet->AddL(TDbCol(KLbtDbFieldStartupCommandLine, EDbColText16));
    
    // State of the trigger
    triggerColSet->AddL(TDbCol(KLbtDbFieldState, EDbColInt8));

    // Bool for trigger fire event
    triggerColSet->AddL(TDbCol(KLbtDbFieldIsTriggerFired, EDbColBit));

    // The distance of this trigger from the last acquired location in strategy
    triggerColSet->AddL(TDbCol(KLbtDbFieldDistanceToLastLocation, EDbColReal32));

    // Validity of the trigger
    triggerColSet->AddL(TDbCol(KLbtDbFieldValidity, EDbColInt8));    
    
    // Trigger Fire on creation flag.
    triggerColSet->AddL(TDbCol(KLbtDbFieldIsTriggerFireOnCreation, EDbColBit));

    // Trigger Area type ( Cell/Circular)
    triggerColSet->AddL(TDbCol(KLbtDbTriggerAreaType, EDbColInt8));    
    
    User::LeaveIfError( iDb.CreateTable( KTriggersTable, *triggerColSet ) );
    
    // Create index key set based on Trigger Id
    CDbKey* index = CDbKey::NewLC();
    index->AddL(TDbKeyCol(KLbtDbFieldId));
    index->AddL(TDbKeyCol(KLbtDbFieldOwnerSid));

    User::LeaveIfError(iDb.CreateIndex(KTriggersIndexId, KTriggersTable, *index));
    CleanupStack::PopAndDestroy( 2 ); // index & triggerColSet
    }

// --------------------------------------------------------------------------------------------
// CLbtDbTriggersManager::CreateCoordinateTriggersTableL
// --------------------------------------------------------------------------------------------
void CLbtDbTriggersManager::CreateCoordinateTriggersTableL()
    {
    CDbColSet* circTriggerColSet = CDbColSet::NewLC();
        
    // Id
	circTriggerColSet->AddL(TDbCol(KLbtCircTrigDbFieldId, EDbColInt64));
	 // Trigger Condition center latitude
	circTriggerColSet->AddL(TDbCol(KLbtDbFieldCenterLat, EDbColReal64));

    // Trigger Condition center longitude
    circTriggerColSet->AddL(TDbCol(KLbtDbFieldCenterLong, EDbColReal64));

    // Trigger Condition radius
    circTriggerColSet->AddL(TDbCol(KLbtDbFieldRadius, EDbColReal64));

    // Trigger Condition Hysteresis radius
    circTriggerColSet->AddL(TDbCol(KLbtDbFieldHysteresis, EDbColReal64));

    // The Square approximation of the trigger condition area. Used by 
    // supervision for better supervision alogrithm.
    circTriggerColSet->AddL(TDbCol(KLbtDbFieldAreaLeft, EDbColReal64));
    
    circTriggerColSet->AddL(TDbCol(KLbtDbFieldAreaRight, EDbColReal64));
    
    circTriggerColSet->AddL(TDbCol(KLbtDbFieldAreaTop, EDbColReal64));
    
    circTriggerColSet->AddL(TDbCol(KLbtDbFieldAreaBottom, EDbColReal64));
    
    circTriggerColSet->AddL(TDbCol(KLbtDbFieldTriggerFiredLat, EDbColReal64));
    
    circTriggerColSet->AddL(TDbCol(KLbtDbFieldTriggerFiredLong, EDbColReal64));

	circTriggerColSet->AddL(TDbCol(KLbtDbFieldTriggerFiredAlt, EDbColReal32));
    circTriggerColSet->AddL(TDbCol(KLbtDbFieldTriggerFiredHorizAcc, EDbColReal32));
    
    circTriggerColSet->AddL(TDbCol(KLbtDbFieldTriggerFiredVertAcc, EDbColReal32));
    
    // Cell id stream for cell ids of the area. Hybrid info
    circTriggerColSet->AddL(TDbCol(KLbtDbFieldCellIdStream, EDbColLongText8));
    User::LeaveIfError( iDb.CreateTable( KCircularTriggersTable, *circTriggerColSet ) );

    // Create index key set based on Trigger Id
    CDbKey* circindex = CDbKey::NewLC();
    circindex->AddL( TDbKeyCol(KLbtCircTrigDbFieldId) );

    User::LeaveIfError( iDb.CreateIndex(KCircTriggersIndexId, KCircularTriggersTable, *circindex) );
    CleanupStack::PopAndDestroy( 2 ); // circindex & circTriggerColSet
    }

// --------------------------------------------------------------------------------------------
// CLbtDbTriggersManager::CreateCellTriggersTableL
// --------------------------------------------------------------------------------------------
void CLbtDbTriggersManager::CreateCellTriggersTableL( )
    {
    CDbColSet* cellTriggerColSet = CDbColSet::NewLC();

    // Cell Based Triggering : Create new col set for cell triggers and add fields
    
    // Id
    cellTriggerColSet->AddL(TDbCol(KLbtCellTrigDbFieldId, EDbColInt64));
    
    // MCC
 	cellTriggerColSet->AddL(TDbCol(KLbtCellTrigDbCountryCode, EDbColUint32));
 	
 	// MNC
 	cellTriggerColSet->AddL(TDbCol(KLbtCellTrigDbNetworkId, EDbColUint32));
 	
 	// Network Type (GSM/WCDMA)
 	cellTriggerColSet->AddL(TDbCol(KLbtCellTrigDbNetworkType, EDbColInt8));
 	
 	// LAC
 	cellTriggerColSet->AddL(TDbCol(KLbtCellTrigDbLAC, EDbColUint32));
 	
 	// Cell ID
 	cellTriggerColSet->AddL(TDbCol(KLbtCellTrigDbCellId, EDbColUint32)); 	
 	
 	User::LeaveIfError( iDb.CreateTable( KCellTriggersTable, *cellTriggerColSet ) );
	CleanupStack::PopAndDestroy( cellTriggerColSet );
  
	// Create index key set based on Trigger Id
	CDbKey* cellindex = CDbKey::NewLC();
	cellindex->AddL(TDbKeyCol(KLbtCellTrigDbFieldId));
	
    User::LeaveIfError(iDb.CreateIndex(KCellTriggersIndexId, KCellTriggersTable, *cellindex));
    CleanupStack::PopAndDestroy(cellindex);
    }

// --------------------------------------------------------------------------------------------
// CLbtDbTriggersManager::CreateHybridTriggersTableL
// --------------------------------------------------------------------------------------------
void CLbtDbTriggersManager::CreateHybridTriggersTableL()
    {
    // Hybrid Triggers table : Create new column set
    CDbColSet* hybridTriggerColSet = CDbColSet::NewLC();    

    // Id
    hybridTriggerColSet->AddL( TDbCol(KLbtHybridTrigDbFieldId, EDbColUint32) );
    
    // Hybrid area base stream
    hybridTriggerColSet->AddL( TDbCol(KLbtHybridDataStream, EDbColLongText8) );    
    
    User::LeaveIfError( iDb.CreateTable( KHybridTriggersTable, *hybridTriggerColSet ) );
    CleanupStack::PopAndDestroy( hybridTriggerColSet );
  
    // Create index key set based on Trigger Id
    CDbKey* cellindex = CDbKey::NewLC();
    cellindex->AddL( TDbKeyCol(KLbtHybridTrigDbFieldId) );

    User::LeaveIfError( iDb.CreateIndex(KHybridTriggersIndexId, KHybridTriggersTable, *cellindex) );
    CleanupStack::PopAndDestroy(cellindex);
    }    

// --------------------------------------------------------------------------------------------
// CLbtDbTriggersManager::CreateTriggerInViewL
// --------------------------------------------------------------------------------------------
void CLbtDbTriggersManager::CreateTriggerInViewL()
    {
    FUNC_ENTER("CLbtDbTriggersManager::CreateTriggerInViewL");
    
    CLbtStartupTrigger* trigger = static_cast<CLbtStartupTrigger* >(iEntry->TriggerEntry());
    TLbtTriggerDynamicInfo* dynInfo = iEntry->DynInfo();
    CLbtExtendedTriggerInfo* extdInfo = iEntry->ExtendedTriggerInfo();
    CLbtTriggerConditionArea* cond = static_cast<CLbtTriggerConditionArea* >(trigger->GetCondition());
    CLbtGeoAreaBase * areaBase = cond->TriggerArea();
    
    // Insert a Row in the View
    iView.InsertL();  // Insert a row. Column order matches sql select statement
	
	 /* Cell Based Triggering : add fields in the various views based on the field values*/
    iView.SetColL( ELbtDbFieldId, static_cast<TInt64>(trigger->Id()) );
    iView.SetColL( ELbtDbFieldName, trigger->Name() );
    iView.SetColL( ELbtDbFieldType, static_cast<TInt8>(trigger->Type()) );
    iView.SetColL( ELbtDbFieldDirection, static_cast<TInt8>((cond->Direction())) );
    iView.SetColL( ELbtDbFieldOwnerSid, extdInfo->OwnerSid().iId );
    iView.SetColL( ELbtDbFieldManagerUiSid, static_cast<TUint32>(trigger->ManagerUi().iUid) ); 	
    iView.SetColL( ELbtDbFieldRearmTime, trigger->TimeToRearm() );
 	
 	HBufC* startupProcFile = HBufC::NewLC( KMaxFileName );
    TPtr startupFile = startupProcFile->Des();
    TSecureId startupProcSid;
 	trigger->GetProcessId( startupFile, startupProcSid );
 	iView.SetColL( ELbtDbFieldStartupSid, startupProcSid );
 	iView.SetColL( ELbtDbFieldStartupPath, *startupProcFile );
 	CleanupStack::PopAndDestroy( startupProcFile );

 	iView.SetColL( ELbtDbFieldStartupCommandLine, trigger->CommandLine() );
 	iView.SetColL( ELbtDbFieldState, trigger->State() );
 	iView.SetColL(ELbtDbFieldIsTriggerFired, extdInfo->IsTriggerFired());
 	iView.SetColL(ELbtDbFieldDistanceToLastLocation, static_cast<TReal32>(0.0) );
 	iView.SetColL( ELbtDbFieldValidity, static_cast<TInt8>(dynInfo->iValidity) );
 	iView.SetColL( ELbtDbFieldIsTriggerFireOnCreation, extdInfo->IsTriggerFireOnCreation() );
 	
    /* add local enum */
 	iView.SetColL(ELbtDbTriggerAreaType,static_cast<TInt8>(areaBase->Type() ));
 	
 	iView.PutL(); // Complete insertion
 	iView.Close();
   
    switch(areaBase->Type())
        {
        case CLbtGeoAreaBase::ECircle:
            {
            AddGeoCircleIntoDbL(trigger);
            break;
            }
        case CLbtGeoAreaBase::ECellular:
            {
            AddGeoCellIntoDbL(trigger);
            break;
            }
        case CLbtGeoAreaBase::EHybrid:
            {
            AddGeoHybridIntoDbL(trigger);
            break;
            }        
        default:
            {
            break;
            }
        }
    }

//---------------------------------------------------------------------------
// CLbtDbTriggersManager::GetCountOfEnabledAndValidTriggers
//---------------------------------------------------------------------------
//
TInt CLbtDbTriggersManager::GetCountOfEnabledAndValidTriggers()
	{
	return iCountOfEnabledAndValidTrigger;
	}


//---------------------------------------------------------------------------
// CLbtDbTriggersManager::SetTimeTillCompaction
//---------------------------------------------------------------------------
//
void CLbtDbTriggersManager::SetTimeTillCompaction(TTime aTime)
	{
	iTimeTillCompaction = aTime;
	}

//---------------------------------------------------------------------------
// CLbtDbTriggersManager::IsStoreBusy
//---------------------------------------------------------------------------
//
TBool CLbtDbTriggersManager::IsStoreBusy()
	{
	return iDbOperation->IsCompactionOngoing();
	}

//---------------------------------------------------------------------------
// CLbtDbTriggersManager::SetStoreChangeObserver
//---------------------------------------------------------------------------
//
void CLbtDbTriggersManager::SetStoreChangeObserver(MLbtTriggerStoreObserver* aObserver)
	{
	iObserver = aObserver;
	}

//---------------------------------------------------------------------------
// CLbtDbTriggersManager::CancelCurrentOperation
//---------------------------------------------------------------------------
//
TInt CLbtDbTriggersManager::CancelCurrentOperation()
    {   
    TInt error = KErrCancel;
    switch( iCurrentOperation )
        {
        case EDbOpDeleteTriggers:
            {
            if( !iView.AtEnd() && !iView.AtBeginning() )
                {
                error = KLbtErrPartial;
                }
            break;
            }
        case EDbOpUpdateTriggersState:
            {
            if( !iView.AtEnd() && !iView.AtBeginning() )
                {
                error = KLbtErrPartial;
                }
            break;
            }
        }
    CompleteClientRequest( KErrCancel );
    Cancel();
    return error;
    }
	
// end of file


