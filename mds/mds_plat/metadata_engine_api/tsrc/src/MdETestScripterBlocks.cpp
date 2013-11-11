/*
* Copyright (c) 2002-2009 Nokia Corporation and/or its subsidiary(-ies). 
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

// [INCLUDE FILES] - do not remove
#include <e32svr.h>
#include <mdeobject.h>
#include <mdeobjectdef.h>
#include <mdesession.h>
#include <mdeconstants.h>
#include <StifParser.h>
#include <Stiftestinterface.h>
#include <StifTestEventInterface.h>
#include "MdETestScripter.h"
#include "MdETestScripterAO.h"

// ============================= LOCAL FUNCTIONS ===============================

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CMdETestScripter::Delete
// Delete here all resources allocated and opened from test methods. 
// Called from destructor. 
// -----------------------------------------------------------------------------
//
void CMdETestScripter::Delete() 
    {
    iObjectDefArray.Reset();
    iRelationDefArray.Reset();
    iEventDefArray.Reset();
    
    iLogicConditionArray.Reset();
    iRelationConditionArray.Reset();
    iNotificationRelationArray.Reset();

	RArray<TItemId> objectIds;
	RArray<TItemId> resultObjectIds;
	RArray<TItemId> eventIds;
	RArray<TItemId> resultEventIds;
	RArray<TItemId> relationIds;
	RArray<TItemId> resultRelationIds;
	
	TInt count = iObjectArray.Count();
	for (TInt i = 0; i < count; i++)
		{
		objectIds.Append(iObjectArray[i]->Id());	
		}
		
	count = iEventArray.Count();
	for (TInt i = 0; i < count; i++)
		{
		eventIds.Append(iEventArray[i]->Id());	
		}
		
	count = iRelationArray.Count();
	for (TInt i = 0; i < count; i++)
		{
		relationIds.Append(iRelationArray[i]->Id());	
		}			
	
	if (objectIds.Count())
		{
		TRAP_IGNORE( iMdeSession->RemoveObjectsL( objectIds, resultObjectIds) );	
		}

    iObjectArray.ResetAndDestroy();
    
	if (eventIds.Count())
		{    
		TRAP_IGNORE( iMdeSession->RemoveEventsL( eventIds, resultEventIds) );
		}
    iEventArray.ResetAndDestroy();
        
	if (relationIds.Count())
		{          
		TRAP_IGNORE( iMdeSession->RemoveRelationsL( relationIds, resultRelationIds) );
		}
    iRelationArray.ResetAndDestroy();
    
    TInt allRemoved = ETrue;
	count = iObjectArray.Count();
	for (TInt i = 0; i < count; i++)
		{
		if (resultObjectIds[i] == KNoId)
			{
			allRemoved = EFalse;
			break;	
			}			
		}
	if (!allRemoved)
		{
		iLog->Log(_L( "All objects not removed from DB!!"));
		}

    allRemoved = ETrue;		
	count = iEventArray.Count();
	for (TInt i = 0; i < count; i++)
		{
		if (resultEventIds[i] == KNoId)
			{
			allRemoved = EFalse;
			break;	
			}		
		}
	if (!allRemoved)
		{
		iLog->Log(_L( "All events not removed from DB!!"));
		}
    iEventArray.ResetAndDestroy();
		
    allRemoved = ETrue;		
	count = iRelationArray.Count();
	for (TInt i = 0; i < count; i++)
		{
		if (resultRelationIds[i] == KNoId)
			{
			allRemoved = EFalse;
			break;	
			}		
		}
	if (!allRemoved)
		{
		iLog->Log(_L( "All relations not removed from DB!!"));
		}
				
	objectIds.Close();
	resultObjectIds.Close();
	eventIds.Close();
	resultEventIds.Close();
	relationIds.Close();
	resultRelationIds.Close();

	iPropertyDefArray.Reset();

    delete iObjectDefNameArray;
    delete iRelationDefNameArray;
	delete iEventDefNameArray;
	delete iPropertyDefNameArray;
	delete iNamespaceDefNameArray;
	
	delete iObjectNameArray;	
    delete iRelationNameArray;
    delete iEventNameArray;
    
    delete iPropertyNameArray;
    
	delete iLogicConditionNameArray;
	delete iRelationConditionNameArray;
	delete iObjectConditionNameArray;
	
	delete iPropertyConditionNameArray;
	delete iEventConditionNameArray;	

	delete iQuery;
    delete iMdeSession;
    iMdeSession = NULL;
    delete iAsyncHandler;
    iAsyncHandler = NULL;
    iMdEDataBuf.Close();
    iItemArray.Close();//Do not need call destroy because it does not own its pointers
    }

// -----------------------------------------------------------------------------
// CMdETestScripter::RunMethodL
// Run specified method. Contains also table of test mothods and their names.
// -----------------------------------------------------------------------------
//
TInt CMdETestScripter::RunMethodL( CStifItemParser& aItem ) 
    {
    static TStifFunctionInfo const KFunctions[] =
        {  
        // Observer add/remove methods
        ENTRY( "AddSchemaObserverL", CMdETestScripter::AddSchemaObserverL ),
        ENTRY( "RemoveSchemaObserverL", CMdETestScripter::RemoveSchemaObserverL ),
        ENTRY( "AddObjectObserverL", CMdETestScripter::AddObjectObserverL ),
        ENTRY( "RemoveObjectObserverL", CMdETestScripter::RemoveObjectObserverL ),
        ENTRY( "AddObjectObserverWithUriL", CMdETestScripter::AddObjectObserverWithUriL ),
        ENTRY( "RemoveObjectObserverWithUriL", CMdETestScripter::RemoveObjectObserverWithUriL ),
        ENTRY( "AddObjectPresentObserverL", CMdETestScripter::AddObjectPresentObserverL ),
        ENTRY( "RemoveObjectPresentObserverL", CMdETestScripter::RemoveObjectPresentObserverL ),
        ENTRY( "AddRelationObserverL", CMdETestScripter::AddRelationObserverL ),
        ENTRY( "RemoveRelationObserverL", CMdETestScripter::RemoveRelationObserverL ),
        ENTRY( "AddRelationItemObserverL", CMdETestScripter::AddRelationItemObserverL ),
        ENTRY( "RemoveRelationItemObserverL", CMdETestScripter::RemoveRelationItemObserverL ),
        ENTRY( "AddRelationPresentObserverL", CMdETestScripter::AddRelationPresentObserverL ),
        ENTRY( "RemoveRelationPresentObserverL", CMdETestScripter::RemoveRelationPresentObserverL ),
        ENTRY( "AddEventObserverL", CMdETestScripter::AddEventObserverL ),
        ENTRY( "RemoveEventObserverL", CMdETestScripter::RemoveEventObserverL ),

        // Notification resulated test primitives
        ENTRY( "VerifyTItemIdsL", CMdETestScripter::VerifyTItemIdsL ),
        ENTRY( "VerifyMdERelationsL", CMdETestScripter::VerifyMdERelationsL ),
        ENTRY( "VerifyObserverNotificationTypeL", CMdETestScripter::VerifyObserverNotificationTypeL ),

        // Metadata/schema/namespace test primitives.
        ENTRY( "ObjectDefCountL", CMdETestScripter::ObjectDefCountL ),
        ENTRY( "ObjectDefL", CMdETestScripter::ObjectDefL ),
        ENTRY( "GetObjectDefL", CMdETestScripter::GetObjectDefL ),
        ENTRY( "RelationDefCountL", CMdETestScripter::RelationDefCountL ),
        ENTRY( "RelationDefL", CMdETestScripter::RelationDefL ),
        ENTRY( "GetRelationDefL", CMdETestScripter::GetRelationDefL ),
        ENTRY( "EventDefCountL", CMdETestScripter::EventDefCountL ),
        ENTRY( "EventDefL", CMdETestScripter::EventDefL ),
        ENTRY( "GetEventDefL", CMdETestScripter::GetEventDefL ),

        ENTRY( "ExportMetadataL", CMdETestScripter::ExportMetadataL ),
        ENTRY( "ImportMetadataL", CMdETestScripter::ImportMetadataL ),
        ENTRY( "ImportSchemaL", CMdETestScripter::ImportSchemaL ),
        ENTRY( "NamespaceDefCountL", CMdETestScripter::NamespaceDefCountL ),
        ENTRY( "NamespaceDefL", CMdETestScripter::NamespaceDefL ),
        ENTRY( "GetNamespaceDefL", CMdETestScripter::GetNamespaceDefL ),
        ENTRY( "GetDefaultNamespaceDefL", CMdETestScripter::GetDefaultNamespaceDefL ),
        ENTRY( "ResetNamespaceDefArray", CMdETestScripter::ResetNamespaceDefArray ),
        ENTRY( "LoadSchemaL", CMdETestScripter::LoadSchemaL ),
        ENTRY( "EngineSessionL", CMdETestScripter::EngineSessionL ),
        ENTRY( "GetSchemaVersionL", CMdETestScripter::GetSchemaVersionL ),
        ENTRY( "SetObjectToPresentByGuidL", CMdETestScripter::SetObjectToPresentByGuidL ),

        // Item / InstanceItem test primitives.
        ENTRY( "BelongsToSessionL", CMdETestScripter::BelongsToSessionL ),
        ENTRY( "SessionL", CMdETestScripter::SessionL ),
        ENTRY( "InDatabaseL", CMdETestScripter::InDatabaseL ),
        ENTRY( "AddItemsL", CMdETestScripter::AddItemsL ),
        ENTRY( "UpdateItemsL", CMdETestScripter::UpdateItemsL ),

        // Definition test primitives.
        ENTRY( "AddRelationDefL", CMdETestScripter::AddRelationDefL ),
        ENTRY( "AddEventDefL", CMdETestScripter::AddEventDefL ),
        ENTRY( "PropertyDefCountL", CMdETestScripter::PropertyDefCountL ),
        ENTRY( "PropertyDefL", CMdETestScripter::PropertyDefL ),
        ENTRY( "GetPropertyDefL", CMdETestScripter::GetPropertyDefL ),
        ENTRY( "MandatoryL", CMdETestScripter::MandatoryL ),
        ENTRY( "MinInt32ValueL", CMdETestScripter::MinInt32ValueL ),
        ENTRY( "MaxInt32ValueL", CMdETestScripter::MaxInt32ValueL ),
        ENTRY( "MinUint32ValueL", CMdETestScripter::MinUint32ValueL ),
        ENTRY( "MaxUint32ValueL", CMdETestScripter::MaxUint32ValueL ),
        ENTRY( "MinInt64ValueL", CMdETestScripter::MinInt64ValueL ),
        ENTRY( "MaxInt64ValueL", CMdETestScripter::MaxInt64ValueL ),
        ENTRY( "MinRealValueL", CMdETestScripter::MinRealValueL ),
        ENTRY( "MaxRealValueL", CMdETestScripter::MaxRealValueL ),
        ENTRY( "MinTimeValueL", CMdETestScripter::MinTimeValueL ),
        ENTRY( "MaxTimeValueL", CMdETestScripter::MaxTimeValueL ),
        ENTRY( "MinTextLengthL", CMdETestScripter::MinTextLengthL ),
        ENTRY( "MaxTextLengthL", CMdETestScripter::MaxTextLengthL ),
        ENTRY( "CompareL", CMdETestScripter::CompareL ),
        ENTRY( "ParentL", CMdETestScripter::ParentL ),
        ENTRY( "NameL", CMdETestScripter::NameL ),
        ENTRY( "ReadOnlyL", CMdETestScripter::ReadOnlyL ),
        ENTRY( "DefL", CMdETestScripter::DefL ),

        // Object test primitives.
        ENTRY( "SetDefL", CMdETestScripter::SetDefL ),
        ENTRY( "UriL", CMdETestScripter::UriL ),
        ENTRY( "SetUriL", CMdETestScripter::SetUriL ),
        ENTRY( "ConfidentialL", CMdETestScripter::ConfidentialL ),
        ENTRY( "SetConfidentialL", CMdETestScripter::SetConfidentialL ),
        ENTRY( "PlaceholderL", CMdETestScripter::PlaceholderL ),
        ENTRY( "SetPlaceholderL", CMdETestScripter::SetPlaceholderL ),
        ENTRY( "MediaIdL", CMdETestScripter::MediaIdL ),
        ENTRY( "SetMediaIdL", CMdETestScripter::SetMediaIdL ),
        ENTRY( "UsageCountL", CMdETestScripter::UsageCountL ),
        ENTRY( "GuidL", CMdETestScripter::GuidL ),
        ENTRY( "SetGuidL", CMdETestScripter::SetGuidL ),
        ENTRY( "OpenForModificationsL", CMdETestScripter::OpenForModificationsL ),
        ENTRY( "NewObjectL", CMdETestScripter::NewObjectL ),
        ENTRY( "AddObjectL", CMdETestScripter::AddObjectL ),
        ENTRY( "AddObjectsL", CMdETestScripter::AddObjectsL ),
        ENTRY( "GetObjectL", CMdETestScripter::GetObjectL ),
        ENTRY( "GetFullObjectL", CMdETestScripter::GetFullObjectL ),
        ENTRY( "CheckObjectL", CMdETestScripter::CheckObjectL ),
        ENTRY( "RemoveObjectL", CMdETestScripter::RemoveObjectL ),
        ENTRY( "RemoveObjectsL", CMdETestScripter::RemoveObjectsL ),
        ENTRY( "RemoveAllObjectsL", CMdETestScripter::RemoveAllObjectsL ),
        ENTRY( "OpenObjectL", CMdETestScripter::OpenObjectL ),
        ENTRY( "OpenFullObjectL", CMdETestScripter::OpenFullObjectL ),
        ENTRY( "CommitObjectL", CMdETestScripter::CommitObjectL ),
        ENTRY( "CommitObjectsL", CMdETestScripter::CommitObjectsL ),
        ENTRY( "CancelObjectL", CMdETestScripter::CancelObjectL ),

        ENTRY( "PropertyL", CMdETestScripter::PropertyL ),
        ENTRY( "PropertyCountL", CMdETestScripter::PropertyCountL ),
        ENTRY( "AddBoolPropertyL", CMdETestScripter::AddBoolPropertyL ),
        ENTRY( "AddInt8PropertyL", CMdETestScripter::AddInt8PropertyL ),
        ENTRY( "AddUint8PropertyL", CMdETestScripter::AddUint8PropertyL ),
        ENTRY( "AddInt16PropertyL", CMdETestScripter::AddInt16PropertyL ),
        ENTRY( "AddUint16PropertyL", CMdETestScripter::AddUint16PropertyL ),
        ENTRY( "AddInt32PropertyL", CMdETestScripter::AddInt32PropertyL ),
        ENTRY( "AddUint32PropertyL", CMdETestScripter::AddUint32PropertyL ),
        ENTRY( "AddInt64PropertyL", CMdETestScripter::AddInt64PropertyL ),
        ENTRY( "AddReal32PropertyL", CMdETestScripter::AddReal32PropertyL ),
        ENTRY( "AddReal64PropertyL", CMdETestScripter::AddReal64PropertyL ),
        ENTRY( "AddTextPropertyL", CMdETestScripter::AddTextPropertyL ),
        ENTRY( "AddTimePropertyL", CMdETestScripter::AddTimePropertyL ),

        // Property test primitives.
        ENTRY( "ObjectL", CMdETestScripter::ObjectL ),
        ENTRY( "BoolValueL", CMdETestScripter::BoolValueL ),
        ENTRY( "Int8ValueL", CMdETestScripter::Int8ValueL ),
        ENTRY( "Uint8ValueL", CMdETestScripter::Uint8ValueL ),
        ENTRY( "Int16ValueL", CMdETestScripter::Int16ValueL ),
        ENTRY( "Uint16ValueL", CMdETestScripter::Uint16ValueL ),
        ENTRY( "Int32ValueL", CMdETestScripter::Int32ValueL ),
        ENTRY( "Uint32ValueL", CMdETestScripter::Uint32ValueL ),
        ENTRY( "Int64ValueL", CMdETestScripter::Int64ValueL ),
        ENTRY( "Real32ValueL", CMdETestScripter::Real32ValueL ),
        ENTRY( "Real64ValueL", CMdETestScripter::Real64ValueL ),
        ENTRY( "TimeValueL", CMdETestScripter::TimeValueL ),
        ENTRY( "TextValueL", CMdETestScripter::TextValueL ),
        ENTRY( "SetBoolValueL", CMdETestScripter::SetBoolValueL ),
        ENTRY( "SetInt8ValueL", CMdETestScripter::SetInt8ValueL ),
        ENTRY( "SetUint8ValueL", CMdETestScripter::SetUint8ValueL ),
        ENTRY( "SetInt16ValueL", CMdETestScripter::SetInt16ValueL ),
        ENTRY( "SetUint16ValueL", CMdETestScripter::SetUint16ValueL ),
        ENTRY( "SetInt32ValueL", CMdETestScripter::SetInt32ValueL ),
        ENTRY( "SetUint32ValueL", CMdETestScripter::SetUint32ValueL ),
        ENTRY( "SetInt64ValueL", CMdETestScripter::SetInt64ValueL ),
        ENTRY( "SetReal32ValueL", CMdETestScripter::SetReal32ValueL ),
        ENTRY( "SetReal64ValueL", CMdETestScripter::SetReal64ValueL ),
        ENTRY( "SetTimeValueL", CMdETestScripter::SetTimeValueL ),
        ENTRY( "SetTextValueL", CMdETestScripter::SetTextValueL ),
        ENTRY( "RemovePropertyL", CMdETestScripter::RemovePropertyL ),
        ENTRY( "MovePropertiesL", CMdETestScripter::MovePropertiesL ),

        // Free text test primitives.
        ENTRY( "FreeTextCountL", CMdETestScripter::FreeTextCountL ),
        ENTRY( "FreeTextL", CMdETestScripter::FreeTextL ),
        ENTRY( "FreeTextIndexL", CMdETestScripter::FreeTextIndexL ),
        ENTRY( "AddFreeTextL", CMdETestScripter::AddFreeTextL ),
        ENTRY( "RemoveFreeTextL", CMdETestScripter::RemoveFreeTextL ),

        // Relation test primitives.
        ENTRY( "NewRelationL", CMdETestScripter::NewRelationL ),
        ENTRY( "AddRelationL", CMdETestScripter::AddRelationL ),
        ENTRY( "LeftObjectIdL", CMdETestScripter::LeftObjectIdL ),
        ENTRY( "RightObjectIdL", CMdETestScripter::RightObjectIdL ),
        ENTRY( "ParameterL", CMdETestScripter::ParameterL ),
        ENTRY( "SetParameterL", CMdETestScripter::SetParameterL ),
        ENTRY( "SetLeftObjectIdL", CMdETestScripter::SetLeftObjectIdL ),
        ENTRY( "SetRightObjectIdL", CMdETestScripter::SetRightObjectIdL ),
        ENTRY( "LastModifiedDateL", CMdETestScripter::LastModifiedDateL ),
        ENTRY( "SetLastModifiedDateL", CMdETestScripter::SetLastModifiedDateL ),
        ENTRY( "GetRelationL", CMdETestScripter::GetRelationL ),
        ENTRY( "UpdateRelationL", CMdETestScripter::UpdateRelationL ),
        ENTRY( "RemoveRelationL", CMdETestScripter::RemoveRelationL ),
        ENTRY( "RemoveRelationsL", CMdETestScripter::RemoveRelationsL ),

        // Event test primitives.
        ENTRY( "NewEventL", CMdETestScripter::NewEventL ),
        ENTRY( "AddEventL", CMdETestScripter::AddEventL ),
        ENTRY( "ObjectIdL", CMdETestScripter::ObjectIdL ),
        ENTRY( "TimeL", CMdETestScripter::TimeL ),
        ENTRY( "SourceL", CMdETestScripter::SourceL ),
        ENTRY( "ParticipantL", CMdETestScripter::ParticipantL ),
        ENTRY( "GetEventL", CMdETestScripter::GetEventL ),
        ENTRY( "RemoveEventL", CMdETestScripter::RemoveEventL ),
        ENTRY( "RemoveEventsL", CMdETestScripter::RemoveEventsL ),
        ENTRY( "NewObjectQueryL", CMdETestScripter::NewObjectQueryL ),
        ENTRY( "NewRelationQueryL", CMdETestScripter::NewRelationQueryL ),
        ENTRY( "NewEventQueryL", CMdETestScripter::NewEventQueryL ),
                
        ENTRY( "QueryObjectL", CMdETestScripter::QueryObjectL ),
        ENTRY( "QueryRelationL", CMdETestScripter::QueryRelationL ),
        ENTRY( "QueryEventL", CMdETestScripter::QueryEventL ),
        ENTRY( "AddPropertyFilterL", CMdETestScripter::AddPropertyFilterL ),
        ENTRY( "AppendOrderRuleL", CMdETestScripter::AppendOrderRuleL ),
        ENTRY( "NewLogicConditionL", CMdETestScripter::NewLogicConditionL ),
        ENTRY( "AddLogicConditionL", CMdETestScripter::AddLogicConditionL ),
        ENTRY( "AddObjectConditionL", CMdETestScripter::AddObjectConditionL ),
        ENTRY( "AddPropertyConditionL", CMdETestScripter::AddPropertyConditionL ),
        ENTRY( "AddRelationConditionL", CMdETestScripter::AddRelationConditionL ),
        ENTRY( "AddEventConditionL", CMdETestScripter::AddEventConditionL ),
        ENTRY( "FindL", CMdETestScripter::FindL ),
        ENTRY( "SetObjectQueryResultL", CMdETestScripter::SetObjectQueryResultL ),
        ENTRY( "SetEventQueryResultL", CMdETestScripter::SetEventQueryResultL ),
        ENTRY( "SetRelationQueryResultL", CMdETestScripter::SetRelationQueryResultL ),
        ENTRY( "RightL", CMdETestScripter::RightL ),
        ENTRY( "LeftL", CMdETestScripter::LeftL ),
        ENTRY( "SetOperatorL", CMdETestScripter::SetOperatorL ), 
        ENTRY( "SetNegateL", CMdETestScripter::SetNegateL ),
        ENTRY( "ObjectConditionsL", CMdETestScripter::ObjectConditionsL ),     
        ENTRY( "TestPopulateDbL", CMdETestScripter::TestPopulateDbL )
        
        // [test cases entries] - Do not remove
        };

    const TInt count = sizeof( KFunctions ) / sizeof( TStifFunctionInfo );

    return RunInternalL( KFunctions, count, aItem );
    }

// -----------------------------------------------------------------------------
// CHarvesterPluginTestScripter::HandleObjectNotification
// -----------------------------------------------------------------------------
//
void CMdETestScripter::HandleObjectNotification( CMdESession& aSession, 
    TObserverNotificationType aType,
    const RArray<TItemId>& aObjectIdArray )
    {
    iLog->Log(_L( "HandleObjectNotification" ));
    
    iNotificationSession = &aSession;
    iNotificationType = aType;
    for ( TInt i = 0; i < aObjectIdArray.Count(); ++i )
        {
        iNotificationItemIdArray.Append( aObjectIdArray[ i ] );
        }

    Signal();
    }

// -----------------------------------------------------------------------------
// CHarvesterPluginTestScripter::HandleUriObjectNotification
// -----------------------------------------------------------------------------
//
void CMdETestScripter::HandleUriObjectNotification( CMdESession& aSession, 
    TObserverNotificationType aType,
    const RArray<TItemId>& aObjectIdArray,
    const RPointerArray<HBufC>& /* aObjectUriArray */ )
    {
    iLog->Log(_L( "HandleUriObjectNotification" ));
    
    iNotificationSession = &aSession;
    iNotificationType = aType;
    for ( TInt i = 0; i < aObjectIdArray.Count(); ++i )
        {
        iNotificationItemIdArray.Append( aObjectIdArray[ i ] );
        }

    Signal();
    }

// -----------------------------------------------------------------------------
// CHarvesterPluginTestScripter::HandleObjectPresentNotification
// -----------------------------------------------------------------------------
//
void CMdETestScripter::HandleObjectPresentNotification( CMdESession& /* aSession */, 
    TBool /* aPresent */, const RArray<TItemId>& /* aObjectIdArray */ )
    {

    }

// -----------------------------------------------------------------------------
// CHarvesterPluginTestScripter::HandleSessionOpened
// -----------------------------------------------------------------------------
//
void CMdETestScripter::HandleSessionOpened( CMdESession& /* aSession */, TInt aError )
    {
    iLog->Log(_L( "HandleSessionOpened, aError == %d"), aError );
    
    Signal();

    iError = aError;
    }

// -----------------------------------------------------------------------------
// CHarvesterPluginTestScripter::HandleSessionError
// -----------------------------------------------------------------------------
//
void CMdETestScripter::HandleSessionError( CMdESession& /*aSession*/, TInt aError )
    {
    iLog->Log(_L( "HandleSessionError, aError == %d"), aError );

    iError = aError;
    }

// -----------------------------------------------------------------------------
// CHarvesterPluginTestScripter::HandleSchemaModified
// -----------------------------------------------------------------------------
//
void CMdETestScripter::HandleSchemaModified()
    {
    iLog->Log(_L( "HandleSchemaModified" ));

    TEventIf event( TEventIf::ESetEvent, TName( KMdESchemaModified ) );
    TestModuleIf().Event( event );

    Signal();// This can be removed after modify all related cases.
    }

// -----------------------------------------------------------------------------
// CHarvesterPluginTestScripter::HandleRelationNotification
// -----------------------------------------------------------------------------
//
void CMdETestScripter::HandleRelationNotification( CMdESession& aSession, 
    TObserverNotificationType aType,
    const RArray<TItemId>& aRelationIdArray )
    {
    iLog->Log(_L( "HandleRelationNotification" ));
    
    iNotificationSession = &aSession;
    iNotificationType = aType;
    for ( TInt i = 0; i < aRelationIdArray.Count(); ++i )
        {
        iNotificationItemIdArray.Append( aRelationIdArray[ i ] );
        }

    Signal();
    }

// -----------------------------------------------------------------------------
// CHarvesterPluginTestScripter::HandleRelationItemNotification
// -----------------------------------------------------------------------------
//
void CMdETestScripter::HandleRelationItemNotification(CMdESession& aSession, 
    TObserverNotificationType aType,
    const RArray<TMdERelation>& aRelationArray )
    {
    iLog->Log(_L( "HandleRelationItemNotification" ));

    iNotificationSession = &aSession;
    iNotificationType = aType;
    for ( TInt i = 0; i < aRelationArray.Count(); ++i )
        {
        iNotificationRelationArray.Append( aRelationArray[ i ] );
        }

    Signal();
    }

// -----------------------------------------------------------------------------
// CHarvesterPluginTestScripter::HandleRelationPresentNotification
// -----------------------------------------------------------------------------
//
void CMdETestScripter::HandleRelationPresentNotification(CMdESession& /* aSession */,
    TBool /* aPresent */,
    const RArray<TItemId>& /* aRelationIdArray */ )
    {

    }

// -----------------------------------------------------------------------------
// CHarvesterPluginTestScripter::HandleEventNotification
// -----------------------------------------------------------------------------
//
void CMdETestScripter::HandleEventNotification( CMdESession& aSession, 
    TObserverNotificationType aType,
    const RArray<TItemId>& aEventIdArray )
    {
    iLog->Log(_L( "HandleEventNotification" ));
    
    iNotificationSession = &aSession;
    iNotificationType = aType;
    for ( TInt i = 0; i < aEventIdArray.Count(); ++i )
        {
        iNotificationItemIdArray.Append( aEventIdArray[ i ] );
        }

    Signal();
    }

// -----------------------------------------------------------------------------
// CMdETestScripter::GetInstanceItemByArrayIndexL
// -----------------------------------------------------------------------------
//
CMdEInstanceItem* CMdETestScripter::GetInstanceItemByArrayIndexL(
    const TInt aMode, CStifItemParser& aItem )
    {
    CMdEInstanceItem* ret = NULL;

    if ( aMode == EObject )
        {
        ret = SanitizeArrayIndexL( iObjectArray, *iObjectNameArray, aItem );
        TL( ret->InstanceType() == EMdETypeObject )
        }
    else if ( aMode == ERelation )
        {
        ret = SanitizeArrayIndexL( iRelationArray, *iRelationNameArray, aItem );
        TL( ret->InstanceType() == EMdETypeRelation )
        }
    else if ( aMode == EEvent )
        {
        ret = SanitizeArrayIndexL( iEventArray, *iEventNameArray, aItem );
        TL( ret->InstanceType() == EMdETypeEvent )
        }
    else if ( aMode == EProperty )
        {
        ret = SanitizeArrayIndexL( iPropertyArray, *iPropertyNameArray, aItem );
        TL( ret->InstanceType() == EMdETypeProperty )
        }
    else
        {
    	User::Leave( KErrArgument );
        }

    return ret;
    }

// -----------------------------------------------------------------------------
// CMdETestScripter::CheckTMdEObjectL
// -----------------------------------------------------------------------------
//
void CMdETestScripter::CheckTMdEObjectL( const TMdEObject& aTObject, const CMdEObject& aObject )
    {
    TL( aObject.Id() == aTObject.Id());
    TL( aObject.Def().Compare( aTObject.DefL()) == 0 );
    TL( aObject.Confidential() == aTObject.Confidential());
    TL( aObject.Placeholder() == aTObject.Placeholder());
    }

// -----------------------------------------------------------------------------
// CMdETestScripter::CheckTMdERelationL
// -----------------------------------------------------------------------------
//
void CMdETestScripter::CheckTMdERelationL( const TMdERelation& aTRelation, const CMdERelation& aRelation )
    {
    TL( aRelation.Id() == aTRelation.Id());
    TL( aRelation.Def().Compare( aTRelation.DefL()) == 0 );
    TL( aRelation.LeftObjectId() == aTRelation.LeftObjectId())
    TL( aRelation.RightObjectId() == aTRelation.RightObjectId()) 
    }

// -----------------------------------------------------------------------------
// CMdETestScripter::CheckTMdEObjectArrayL
// -----------------------------------------------------------------------------
//
void CMdETestScripter::CheckTMdEObjectArrayL(
    const RArray<TMdEObject>& aTObjects, const TInt aIndex, const TInt aCount )
    {
    TL( aTObjects.Count() == aCount );

    for ( TInt i = 0; i < aTObjects.Count(); ++i )
        {
        const CMdEObject* object = NULL;
        TInt j;

        for ( j = 0; aIndex + j < iObjectArray.Count(); ++j )
            {
            if ( iObjectArray[ aIndex + j ]->Id() == aTObjects[ i ].Id())
                {
                object = iObjectArray[ aIndex + j ];

                CheckTMdEObjectL( aTObjects[ i ], *object );
                break;
                }
            }
        TL( object );
        }
    }

// -----------------------------------------------------------------------------
// CMdETestScripter::CheckTMdERelationArrayL
// -----------------------------------------------------------------------------
//
void CMdETestScripter::CheckTMdERelationArrayL(
    const RArray<TMdERelation>& aTRelations, const TInt aIndex, const TInt aCount )
    {
    TL( aTRelations.Count() == aCount );

    for ( TInt i = 0; i < aTRelations.Count(); ++i )
        {
        const CMdERelation* relation = NULL;
        TInt j;

        for ( j = 0; aIndex + j < iRelationArray.Count(); ++j )
            {
            if ( iRelationArray[ aIndex + j ]->Id() == aTRelations[ i ].Id())
                {
                relation = iRelationArray[ aIndex + j ];

                CheckTMdERelationL( aTRelations[ i ], *relation );
                break;
                }
            }
        TL( relation );
        }
    }

// -----------------------------------------------------------------------------
// CMdETestScripter::CheckTItemIdArrayL
// -----------------------------------------------------------------------------
//
template <class T> void CMdETestScripter::CheckTItemIdArrayL(
    const RArray<TItemId>& aTItemIds, const RPointerArray<T>& aArray,
    const TInt aIndex, const TInt aCount )
    {
    TL( aTItemIds.Count() == aCount );

    for ( TInt i = 0; i < aTItemIds.Count(); ++i )
        {
        const T* t = NULL;
        TInt j;

        for ( j = 0; aIndex + j < aArray.Count(); ++j )
            {
            if ( aArray[ aIndex + j ]->Id() == aTItemIds[ i ] )
                {
                t = aArray[ aIndex + j ];

                break;
                }
            }
        TL( t );
        }
    }

// -----------------------------------------------------------------------------
// CMdETestScripter::CheckAsyncResultArrayL
// -----------------------------------------------------------------------------
//
TInt CMdETestScripter::CheckAsyncResultArrayL( TEventIf aEvent )
    {
    TInt err = KErrNone;
    if ( 0 == aEvent.Name().Compare( KMdEItemsAsyncAdd ) 
        || 0 == aEvent.Name().Compare( KMdEItemsAsyncUpdate ) )
        {
        //Check result item ids count of iMdEDataBuf
        //Update ids of iItemArray( iObjectArray, iRelationArray, iEventsArray ) to clear temp test data in delete() later.
        err = iMdeSession->DeserializeItemsL( iMdEDataBuf, iItemArray );
        }
    else if ( 0 == aEvent.Name().Compare( KMdEObjectsAsyncRemoved )
        || 0 == aEvent.Name().Compare( KMdEEventsAsyncRemoved )
        || 0 == aEvent.Name().Compare( KMdERelationsAsyncRemoved ) )
        {
        RArray<TItemId> objectIds, eventIds, relationIds;
        err = iMdeSession->DeserializeIdsL( iMdEDataBuf, &objectIds, &eventIds, &relationIds );
        
        //Check result item ids count of iMdEDataBuf
        if ( 0 == aEvent.Name().Compare( KMdEObjectsAsyncRemoved ) )
            {
            TL( objectIds.Count() == iItemCount );
            }
        else if ( 0 == aEvent.Name().Compare( KMdEEventsAsyncRemoved ) )
            {
            TL( eventIds.Count() == iItemCount );
            }
        else if ( 0 == aEvent.Name().Compare( KMdERelationsAsyncRemoved ) )
            {
            TL( relationIds.Count() == iItemCount );
            }
        objectIds.Close();
        eventIds.Close();
        relationIds.Close();
        }
    iItemCount = 0;
    iItemArray.Reset();
    iMdEDataBuf.Close();//Clean mde buffer
    return err;
    }

// -----------------------------------------------------------------------------
// CMdETestScripter::FormInstanceItemArrayL
// -----------------------------------------------------------------------------
//
void CMdETestScripter::FormInstanceItemArrayL(
    CStifItemParser& aItem, RPointerArray<CMdEInstanceItem>& aItemArray ) const
    {
    TInt mode2;
    TInt index;
    TInt count;

    User::LeaveIfError( aItem.GetNextInt( mode2 ));
    User::LeaveIfError( aItem.GetNextInt( index ));
    User::LeaveIfError( aItem.GetNextInt( count ));

    for ( TInt i = 0; i < count; ++i )
        {
        if ( mode2 == EObject )
            {
            aItemArray.Append( iObjectArray[ index + i ] );
            }
        else if ( mode2 == ERelation )
            {
            aItemArray.Append( iRelationArray[ index + i ] );
            }
        else if ( mode2 == EEvent )
            {
            aItemArray.Append( iEventArray[ index + i ] );
            }
        else if ( mode2 == EProperty )
            {
            aItemArray.Append( iPropertyArray[ index + i ] );
            }
        }
    }

// -----------------------------------------------------------------------------
// CMdETestScripter::SanitizeArrayIndexL
// -----------------------------------------------------------------------------
//
template <class T> T* CMdETestScripter::SanitizeArrayIndexL(
    RPointerArray<T>& aArray, const CDesCArrayFlat& aNameArray, 
    CStifItemParser& aItem )
    {
    __ASSERT_ALWAYS( aArray.Count() == aNameArray.Count(),
                     User::Panic( KMdETestScripter, KErrGeneral ));

    TPtrC nameOrIndex;
    User::LeaveIfError( aItem.GetNextString( nameOrIndex ));
	TLex lex( nameOrIndex );
	TInt index;
	TInt err = lex.Val( index );
	if ( err == KErrGeneral )
		{ // string is name identifier
		for ( TInt nameIndex = 0; nameIndex < aNameArray.Count(); nameIndex++)
			{
			if ( nameOrIndex.Compare( aNameArray[ nameIndex ] ) == 0 )
				{
				return aArray[ nameIndex ];	
				}
			}
		User::Leave( KErrNotFound );
		return NULL; // suppress compile warning
		}
	else
		{
		if ( index == KLast )
    		{
    		index = aArray.Count() - 1;
    		}

		TL( aArray.Count() > index );
		return aArray[ index ];
		}		
    }

// ========================== TEMPLATE METHOD INSTANTIATIONS ===================

// -----------------------------------------------------------------------------
// CMdETestScripter::SanitizeArrayIndexL
// -----------------------------------------------------------------------------
//
template CMdEObjectDef* CMdETestScripter::SanitizeArrayIndexL(
    RPointerArray<CMdEObjectDef>& aArray, const CDesCArrayFlat& aNameArray, CStifItemParser& aItem );
template CMdERelationDef* CMdETestScripter::SanitizeArrayIndexL(
    RPointerArray<CMdERelationDef>& aArray, const CDesCArrayFlat& aNameArray, CStifItemParser& aItem );
template CMdEEventDef* CMdETestScripter::SanitizeArrayIndexL(
    RPointerArray<CMdEEventDef>& aArray, const CDesCArrayFlat& aNameArray, CStifItemParser& aItem );
template CMdEPropertyDef* CMdETestScripter::SanitizeArrayIndexL(
    RPointerArray<CMdEPropertyDef>& aArray, const CDesCArrayFlat& aNameArray, CStifItemParser& aItem );
template CMdENamespaceDef* CMdETestScripter::SanitizeArrayIndexL(
    RPointerArray<CMdENamespaceDef>& aArray, const CDesCArrayFlat& aNameArray, CStifItemParser& aItem );
template CMdEObject* CMdETestScripter::SanitizeArrayIndexL(
    RPointerArray<CMdEObject>& aArray, const CDesCArrayFlat& aNameArray, CStifItemParser& aItem );
template CMdERelation* CMdETestScripter::SanitizeArrayIndexL(
    RPointerArray<CMdERelation>& aArray, const CDesCArrayFlat& aNameArray, CStifItemParser& aItem );
template CMdEEvent* CMdETestScripter::SanitizeArrayIndexL(
    RPointerArray<CMdEEvent>& aArray, const CDesCArrayFlat& aNameArray, CStifItemParser& aItem );
template CMdEProperty* CMdETestScripter::SanitizeArrayIndexL(
    RPointerArray<CMdEProperty>& aArray, const CDesCArrayFlat& aNameArray, CStifItemParser& aItem );
template CMdERelationCondition* CMdETestScripter::SanitizeArrayIndexL(
    RPointerArray<CMdERelationCondition>& aArray, const CDesCArrayFlat& aNameArray, CStifItemParser& aItem );
template CMdELogicCondition* CMdETestScripter::SanitizeArrayIndexL(
    RPointerArray<CMdELogicCondition>& aArray, const CDesCArrayFlat& aNameArray, CStifItemParser& aItem );
template CMdEPropertyCondition* CMdETestScripter::SanitizeArrayIndexL(
    RPointerArray<CMdEPropertyCondition>& aArray, const CDesCArrayFlat& aNameArray, CStifItemParser& aItem );
template CMdEEventCondition* CMdETestScripter::SanitizeArrayIndexL(
    RPointerArray<CMdEEventCondition>& aArray, const CDesCArrayFlat& aNameArray, CStifItemParser& aItem );

// -----------------------------------------------------------------------------
// CMdETestScripter::CheckTItemIdArrayL
// -----------------------------------------------------------------------------
//
template void CMdETestScripter::CheckTItemIdArrayL(
    const RArray<TItemId>& aTItemIds, const RPointerArray<CMdEObject>& aArray,
    const TInt aIndex, const TInt aCount );
template void CMdETestScripter::CheckTItemIdArrayL(
    const RArray<TItemId>& aTItemIds, const RPointerArray<CMdEEvent>& aArray,
    const TInt aIndex, const TInt aCount );
template void CMdETestScripter::CheckTItemIdArrayL(
    const RArray<TItemId>& aTItemIds, const RPointerArray<CMdERelation>& aArray,
    const TInt aIndex, const TInt aCount );

// ========================== OTHER EXPORTED FUNCTIONS =========================
// None

//  [End of File] - Do not remove
