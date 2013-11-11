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
* Description:  ?Description
*
*/

// [INCLUDE FILES] - do not remove
#include <mdeobject.h>
#include <mdeobjectquery.h>
#include <mdeconstants.h>
#include <StifParser.h>
#include <StifTestInterface.h>
#include "MdETestScripter.h"

const TInt KRootCond = -1;

// -----------------------------------------------------------------------------
// CMdETestScripter::FindL
// -----------------------------------------------------------------------------
//
TInt CMdETestScripter::FindL( CStifItemParser& aItem )
	{
	TInt maxCount;
	User::LeaveIfError( aItem.GetNextInt( maxCount ) );
	TInt notifyCount;
	User::LeaveIfError( aItem.GetNextInt( notifyCount ) );
	
	iQuery->FindL( maxCount, notifyCount );
	return KErrNone;
	}


// -----------------------------------------------------------------------------
// CMdETestScripter::QueryObjectL
// -----------------------------------------------------------------------------
//
TInt CMdETestScripter::QueryObjectL( CStifItemParser& aItem )
    {
    TPtrC objectString;
    
    CMdENamespaceDef* space = SanitizeArrayIndexL( iNamespaceDefArray, *iNamespaceDefNameArray, aItem );
    
    User::LeaveIfError( aItem.GetNextString( objectString ));
	CMdEObjectDef& objdef = space->GetObjectDefL( objectString );
	
	delete iQuery;
	iQuery = NULL;
	iQuery = iMdeSession->NewObjectQueryL( *space, objdef, this );
	iQuery->SetResultMode( EQueryResultModeId );
	iQuery->FindL(1);

    return KErrNone;
    }
    
	
// -----------------------------------------------------------------------------
// CMdETestScripter::NewObjectQueryL
// -----------------------------------------------------------------------------
//
TInt CMdETestScripter::NewObjectQueryL( CStifItemParser& aItem )
	{
    TPtrC name;
	User::LeaveIfError( aItem.GetNextString( name )); 
    CMdENamespaceDef* space = SanitizeArrayIndexL( iNamespaceDefArray, *iNamespaceDefNameArray, aItem );	 		
    TPtrC modeString;
    User::LeaveIfError( aItem.GetNextString( modeString ));
    TQueryResultMode modeEnum = ResultModeL( modeString );
    
    TPtrC objectString;
    User::LeaveIfError( aItem.GetNextString( objectString ));

	CMdEObjectDef& objdef = space->GetObjectDefL( objectString );
	
	delete iQuery;
	iQuery = NULL;
	
	TPtrC type;
	User::LeaveIfError( aItem.GetNextString( type ));
	if ( type.Compare(_L("Object")) == 0 )
		{
		iQuery = iMdeSession->NewObjectQueryL( *space, objdef, this );			
		}		
	else if ( type.Compare(_L("LookupObject")) == 0 )
		{		
		RPointerArray<CMdEObjectDef> *objDefs = new (ELeave) RPointerArray<CMdEObjectDef>;
		TPtrC startMark;
		User::LeaveIfError( aItem.GetNextString( startMark ));
		if ( startMark.Compare(_L("{")) == 0)
			{
			TInt index;
			while( ETrue )
				{
				index = GetObjectDefIndexL( aItem );
				if ( index == KErrCompletion )
					{
					break;
					}
				objDefs->Append( iObjectDefArray[ index ] );
				}	
			}
		iQuery = iMdeSession->NewObjectQueryL( objdef, objDefs, this );	
		}
	else
        {
    	User::Leave( KErrArgument );
        }
	

	iQuery->SetResultMode( modeEnum );
	CMdELogicCondition* rootLogicCondition = &iQuery->Conditions();
    iLogicConditionArray.Append( rootLogicCondition );
    AppendNameL( iLogicConditionNameArray, name );		
	return KErrNone;	
	}

// -----------------------------------------------------------------------------
// CMdETestScripter::NewRelationQueryL
// -----------------------------------------------------------------------------
//
TInt CMdETestScripter::NewRelationQueryL( CStifItemParser& aItem )
	{	
	
    TPtrC name;
	User::LeaveIfError( aItem.GetNextString( name ));  	
	delete iQuery;
	iQuery = NULL;
	CMdENamespaceDef* space = SanitizeArrayIndexL( iNamespaceDefArray, *iNamespaceDefNameArray, aItem );
    TPtrC modeString;
    User::LeaveIfError( aItem.GetNextString( modeString ));
    TQueryResultMode modeEnum = ResultModeL( modeString );
    
	iQuery = iMdeSession->NewRelationQueryL( *space, this );
	iQuery->SetResultMode( modeEnum );
	CMdELogicCondition* rootLogicCondition = &iQuery->Conditions();
    iLogicConditionArray.Append( rootLogicCondition );
    AppendNameL( iLogicConditionNameArray, name );	
	return KErrNone;		
	}

// -----------------------------------------------------------------------------
// CMdETestScripter::NewEventQueryL
// -----------------------------------------------------------------------------
//
TInt CMdETestScripter::NewEventQueryL( CStifItemParser& aItem )
	{	
    TPtrC name;
	User::LeaveIfError( aItem.GetNextString( name ));	
	
	delete iQuery;
	iQuery = NULL;
    CMdENamespaceDef* space = SanitizeArrayIndexL( iNamespaceDefArray, *iNamespaceDefNameArray, aItem );	
    TPtrC modeString;
    User::LeaveIfError( aItem.GetNextString( modeString ));
    TQueryResultMode modeEnum = ResultModeL( modeString );

	iQuery = iMdeSession->NewEventQueryL( *space, this );
	iQuery->SetResultMode( modeEnum );
	CMdELogicCondition* rootLogicCondition = &iQuery->Conditions();
    iLogicConditionArray.Append( rootLogicCondition );
    AppendNameL( iLogicConditionNameArray, name );		
	return KErrNone;	
	}
	
// -----------------------------------------------------------------------------	
// CMdEObjectQuery::AddPropertyFilterL
// -----------------------------------------------------------------------------
// 
TInt CMdETestScripter::AddPropertyFilterL( CStifItemParser& aItem )
	{
	CMdEPropertyDef* propertyDef = SanitizeArrayIndexL( iPropertyDefArray, *iPropertyDefNameArray, aItem);
	((CMdEObjectQuery *)iQuery)->AddPropertyFilterL( propertyDef );
	return KErrNone;
	}	


// -----------------------------------------------------------------------------	
// CMdEObjectQuery::AppendOrderRuleL
// -----------------------------------------------------------------------------
// 
TInt CMdETestScripter::AppendOrderRuleL( CStifItemParser& aItem )
	{
	TInt ascending;
	User::LeaveIfError( aItem.GetNextInt( ascending ) );
	TInt orderRule;
	User::LeaveIfError( aItem.GetNextInt( orderRule ) );
	if ( orderRule == EOrderRuleTypeProperty )
		{
		CMdEPropertyDef* propertyDef = SanitizeArrayIndexL( iPropertyDefArray, *iPropertyDefNameArray, aItem);
		((CMdEObjectQuery *)iQuery)->AppendOrderRuleL( TMdEOrderRule( *propertyDef, ascending ));
		}
	else
		{
		((CMdEObjectQuery *)iQuery)->AppendOrderRuleL( TMdEOrderRule( (TOrderRuleType) orderRule, ascending ));	
		}
	return KErrNone;	
	}


// -----------------------------------------------------------------------------
// CMdETestScripter::HandleQueryNewResults
// -----------------------------------------------------------------------------
//    
void CMdETestScripter::HandleQueryNewResults( CMdEQuery& /*aQuery*/,
    TInt /*aFirstNewItemIndex*/, TInt aNewItemCount )
	{
	_LIT( KMessage, "Query new results count: " );
	_LIT( KMessage2, " new count: " );
	TBuf<128> buf( KMessage );
	buf.Append( KMessage2 );
	buf.AppendNum( aNewItemCount );

	}
	

// -----------------------------------------------------------------------------
// CMdETestScripter::QueryRelationL
// -----------------------------------------------------------------------------
//
TInt CMdETestScripter::QueryRelationL( CStifItemParser& aItem )
    {
    TPtrC relationString;
    User::LeaveIfError( aItem.GetNextString( relationString ));

	CMdERelationDef& reldef = iNamespaceDefArray[0]->GetRelationDefL( relationString );
	
	delete iQuery;
	iQuery = NULL;	
	iQuery = iMdeSession->NewRelationQueryL( *iNamespaceDefArray[0], this );
	iQuery->SetResultMode( EQueryResultModeId );
	iQueryRoot = &iQuery->Conditions();
	iQueryRoot->AddRelationConditionL( reldef );	
	iQuery->FindL(1);

    return KErrNone;
    }	
    
// -----------------------------------------------------------------------------
// CMdETestScripter::QueryEventL
// -----------------------------------------------------------------------------
//
TInt CMdETestScripter::QueryEventL( CStifItemParser& aItem )    
    {
    TPtrC eventString;
    User::LeaveIfError( aItem.GetNextString( eventString ));

	CMdEEventDef& evdef = iNamespaceDefArray[0]->GetEventDefL( eventString );
	
	delete iQuery;
	iQuery = NULL;
	iQuery = iMdeSession->NewEventQueryL( *iNamespaceDefArray[0], this );
	iQuery->SetResultMode( EQueryResultModeId );
	iQueryRoot = &iQuery->Conditions();
	iQueryRoot->AddEventConditionL( evdef );	
	iQuery->FindL(1);

    return KErrNone;
    }

_LIT(KAND , "AND");
_LIT(KOR , "OR");	
	
// -----------------------------------------------------------------------------	
// CMdETestScripter::NewLogicConditionL
// -----------------------------------------------------------------------------
//    
TInt CMdETestScripter::NewLogicConditionL( CStifItemParser& aItem )
	{
	TPtrC name;
	User::LeaveIfError( aItem.GetNextString( name ));
	TPtrC logicOperator;
	User::LeaveIfError( aItem.GetNextString( logicOperator ));
	TLogicConditionOperator logCondOperator = ELogicConditionOperatorAnd;
	if ( logicOperator.Compare( KAND ) == 0)
		{
		logCondOperator = ELogicConditionOperatorAnd;	
		}
	else if ( logicOperator.Compare( KOR ) == 0)
		{
		logCondOperator = ELogicConditionOperatorOr;		
		}
	else
        {
    	User::Leave( KErrArgument );
        }
	CMdELogicCondition* condition = CMdELogicCondition::NewL( logCondOperator );
	iLogicConditionArray.Append( condition );
	AppendNameL( iEventConditionNameArray, name );
	return KErrNone;	
	}


// -----------------------------------------------------------------------------	
// CMdETestScripter::RightL
// -----------------------------------------------------------------------------
//    
TInt CMdETestScripter::RightL( CStifItemParser& aItem )
	{
	TPtrC name;
	User::LeaveIfError( aItem.GetNextString( name ));
	
	CMdERelationCondition* relationCond = SanitizeArrayIndexL(
		 iRelationConditionArray, *iRelationConditionNameArray, aItem );			
	CMdELogicCondition* rightCond = &relationCond->RightL();
    iLogicConditionArray.Append( rightCond );
    AppendNameL( iLogicConditionNameArray, name );
    return KErrNone;
	}

// -----------------------------------------------------------------------------	
// CMdETestScripter::LeftL
// -----------------------------------------------------------------------------
//    
TInt CMdETestScripter::LeftL( CStifItemParser& aItem )
	{
	TPtrC name;
	User::LeaveIfError( aItem.GetNextString( name ));
	
	CMdERelationCondition* relationCond = SanitizeArrayIndexL( 
		iRelationConditionArray, *iRelationConditionNameArray, aItem );			
	CMdELogicCondition* leftCond = &relationCond->LeftL();
    iLogicConditionArray.Append( leftCond );
    AppendNameL( iLogicConditionNameArray, name );
    return KErrNone;
	}

// -----------------------------------------------------------------------------	
// CMdETestScripter::SetOperatorL
// -----------------------------------------------------------------------------
//    
TInt CMdETestScripter::SetOperatorL( CStifItemParser& aItem )
	{
	CMdELogicCondition* logicCondition = SanitizeArrayIndexL( iLogicConditionArray, *iLogicConditionNameArray, aItem );	
	TInt logicalOperator;
	User::LeaveIfError( aItem.GetNextInt( logicalOperator ));
	logicCondition->SetOperator( ( TLogicConditionOperator )logicalOperator );
    return KErrNone;
	}
	
// -----------------------------------------------------------------------------	
// CMdETestScripter::SetNegateL
// -----------------------------------------------------------------------------
//    
TInt CMdETestScripter::SetNegateL( CStifItemParser& aItem )
	{
	CMdEPropertyCondition* condition = SanitizeArrayIndexL( iPropertyConditionArray, *iPropertyConditionNameArray, aItem );	
	TInt boolean;
	User::LeaveIfError( aItem.GetNextInt( boolean ));
	((CMdECondition *)condition)->SetNegate( boolean );
    return KErrNone;
	}	
	
	

// -----------------------------------------------------------------------------	
// CMdETestScripter::AddLogicConditionL
// -----------------------------------------------------------------------------
//    
TInt CMdETestScripter::AddLogicConditionL( CStifItemParser& aItem )
	{
	TPtrC name;
	User::LeaveIfError( aItem.GetNextString( name ));			
	
	CMdELogicCondition* logicCondition = SanitizeArrayIndexL( iLogicConditionArray, *iLogicConditionNameArray, aItem );
	
	TPtrC logicOperator;
	User::LeaveIfError( aItem.GetNextString( logicOperator ));
	TLogicConditionOperator logCondOperator = ELogicConditionOperatorAnd;
	if ( logicOperator.Compare( KAND ) == 0)
		{
		logCondOperator = ELogicConditionOperatorAnd;	
		}
	else if ( logicOperator.Compare( KOR ) == 0)
		{
		logCondOperator = ELogicConditionOperatorOr;		
		}
	else
        {
    	User::Leave( KErrArgument );
        }
    CMdELogicCondition* condition = &logicCondition->AddLogicConditionL( logCondOperator );
    iLogicConditionArray.Append( condition );
    AppendNameL( iLogicConditionNameArray, name );
    return KErrNone;
	}


	_LIT(KRelationDef, "RelationDef");
	_LIT(KRelationDefRange, "RelationDefRange");
	
	_LIT(KAny, "Any");
	_LIT(KEqual, "Equal");
	_LIT(KNotEqual, "NotEqual");
	_LIT(KLess, "Less");
	_LIT(KLessOrEqual, "LessOrEqual");
	_LIT(KGreater, "Greater");
	_LIT(KGreaterOrEqual, "GreaterOrEqual");
	_LIT(KBetween, "Between");
	_LIT(KNotBetween, "NotBetween");

	_LIT(KObject , "Object");
	_LIT(KObjectDef , "ObjectDef");
	_LIT(KGuid , "Guid");

	_LIT(KNone , "None");
	_LIT(KId , "Id");
	_LIT(KIds , "Ids");
	_LIT(KUri , "Uri");
	_LIT(KUriBeginsWith , "UriBeginsWith");
	_LIT(KFreeText , "FreeText");
	_LIT(KFreeTextContains , "FreeTextContains");
	_LIT(KFreeTextBeginsWith , "FreeTextBeginsWith");
	_LIT(KFreeTextEndsWith , "FreeTextEndsWith");
	_LIT(KFreeTextUsageCount , "FreeTextUsageCount");


	_LIT(KCount , "Count");
	_LIT(KItem , "Item");
	_LIT(KDistinctValues , "DistinctValues");
	_LIT(KObjectWithFreetexts , "ObjectWithFreetexts");



TQueryResultMode CMdETestScripter::ResultModeL(TDesC& aModeString)
	{
	TQueryResultMode modeEnum = EQueryResultModeId;
	if ( aModeString.Compare( KId ) == 0 )
		{
		modeEnum = EQueryResultModeId;	
		}
	else if ( aModeString.Compare( KItem ) == 0 )
		{
		modeEnum = EQueryResultModeItem;	
		}
	else if ( aModeString.Compare( KCount ) == 0 )
		{
		modeEnum = EQueryResultModeCount;	
		}
	else if ( aModeString.Compare( KDistinctValues ) == 0 )
		{
		modeEnum = EQueryResultModeDistinctValues;	
		}
	else if ( aModeString.Compare( KObjectWithFreetexts ) == 0 )
		{
		modeEnum = EQueryResultModeObjectWithFreetexts;	
		}
	else
        {
    	User::Leave( KErrArgument );
        }
    return modeEnum;	
	}


CMdERelationDef& CMdETestScripter::GetRelationDefStrL( CStifItemParser& aItem )
	{
	TPtrC relationDefStr;
	User::LeaveIfError( aItem.GetNextString( relationDefStr ));
	TLex lex( relationDefStr );
	TInt integer;
	TInt err = lex.Val( integer );
	if ( err == KErrGeneral )
		{
		return iNamespaceDefArray[0]->GetRelationDefL( relationDefStr );		
		}
	else
		{
		return *iRelationDefArray[ integer ];	
		}
	}

TInt CMdETestScripter::GetObjectDefIndexL( CStifItemParser& aItem )
	{
	TPtrC objectDefStr;
	User::LeaveIfError( aItem.GetNextString( objectDefStr ));
	TLex lex( objectDefStr );
	TInt integer;
	TInt err = lex.Val( integer );
	if ( err == KErrGeneral )
		{
		if ( objectDefStr.Compare( _L("}")) == 0)
			{ 
			return KErrCompletion;	// indictes end of list
			}		
		for ( TInt i = 0; i < iObjectDefNameArray->Count(); i++)
			{
			if ( objectDefStr.Compare( (*iObjectDefNameArray)[i] ) == 0 )
				{
				return i;	
				}
			}
		return KErrNotFound;
		}
	else
		{
		return integer ;	
		}
	}
	
TInt CMdETestScripter::GetRelationDefIndexL( CStifItemParser& aItem )
	{
	TPtrC relationDefStr;
	User::LeaveIfError( aItem.GetNextString( relationDefStr ));
	TLex lex( relationDefStr );
	TInt integer;
	TInt err = lex.Val( integer );
	if ( err == KErrGeneral )
		{
		for ( TInt i = 0; i < iRelationDefNameArray->Count(); i++)
			{
			if ( relationDefStr.Compare( (*iRelationDefNameArray)[i] ) == 0 )
				{
				return i;	
				}
			}
		return KErrNotFound;
		}
	else
		{
		return integer ;	
		}
	}	

TInt CMdETestScripter::GetEventDefIndexL( CStifItemParser& aItem )
	{
	TPtrC eventDefStr;
	User::LeaveIfError( aItem.GetNextString( eventDefStr ));
	TLex lex( eventDefStr );
	TInt integer;
	TInt err = lex.Val( integer );
	if ( err == KErrGeneral )
		{
		for ( TInt i = 0; i < iEventDefNameArray->Count(); i++)
			{
			if ( eventDefStr.Compare( (*iEventDefNameArray)[i] ) == 0 )
				{
				return i;	
				}
			}
		return KErrNotFound;
		}
	else
		{
		return integer ;	
		}
	}	

TInt CMdETestScripter::GetObjectIndexL( CStifItemParser& aItem )
	{
	TPtrC objectStr;
	User::LeaveIfError( aItem.GetNextString( objectStr ));
	TLex lex( objectStr );
	TInt integer;
	TInt err = lex.Val( integer );
	if ( err == KErrGeneral )
		{
		if ( objectStr.Compare( _L("}")) == 0)
			{ 
			return KErrCompletion;	// indictes end of list
			}
		for ( TInt i = 0; i < iObjectNameArray->Count(); i++)
			{
			if ( objectStr.Compare( (*iObjectNameArray)[i] ) == 0 )
				{
				return i;	
				}
			}
		return KErrNotFound;
		}
	else
		{
		return integer ;	
		}
	}	


TInt CMdETestScripter::GetEventIndexL( CStifItemParser& aItem )
	{
	TPtrC eventStr;
	User::LeaveIfError( aItem.GetNextString( eventStr ));
	TLex lex( eventStr );
	TInt integer;
	TInt err = lex.Val( integer );
	if ( err == KErrGeneral )
		{
		if ( eventStr.Compare( _L("}")) == 0)
			{ 
			return KErrCompletion;	// indictes end of list
			}		
		for ( TInt i = 0; i < iEventNameArray->Count(); i++)
			{
			if ( eventStr.Compare( (*iEventNameArray)[i] ) == 0 )
				{
				return i;	
				}
			}
		return KErrNotFound;
		}
	else
		{
		return integer ;	
		}
	}

TInt CMdETestScripter::GetRelationIndexL( CStifItemParser& aItem )
	{
	TPtrC relationStr;
	User::LeaveIfError( aItem.GetNextString( relationStr ));
	TLex lex( relationStr );
	TInt integer;
	TInt err = lex.Val( integer );
	if ( err == KErrGeneral )
		{
		if ( relationStr.Compare( _L("}")) == 0)
			{ 
			return KErrCompletion;	// indictes end of list
			}		
		for ( TInt i = 0; i < iRelationNameArray->Count(); i++)
			{
			if ( relationStr.Compare( (*iRelationNameArray)[i] ) == 0 )
				{
				return i;	
				}
			}
		return KErrNotFound;
		}
	else
		{
		return integer ;	
		}
	}

TInt CMdETestScripter::GetLogicConditionIndexL( CStifItemParser& aItem )
	{
	TPtrC logicConditionStr;
	User::LeaveIfError( aItem.GetNextString( logicConditionStr ));
	TLex lex( logicConditionStr );
	TInt integer;
	TInt err = lex.Val( integer );
	if ( err == KErrGeneral )
		{
		for ( TInt i = 0; i < iLogicConditionNameArray->Count(); i++)
			{
			if ( logicConditionStr.Compare( (*iLogicConditionNameArray)[i] ) == 0 )
				{
				return i;	
				}
			}
		return KErrNotFound;
		}
	else
		{
		return integer ;	
		}
	}


TInt CMdETestScripter::GetRelationConditionIndexL( CStifItemParser& aItem )
	{
	TPtrC relationConditionStr;
	User::LeaveIfError( aItem.GetNextString( relationConditionStr ));
	TLex lex( relationConditionStr );
	TInt integer;
	TInt err = lex.Val( integer );
	if ( err == KErrGeneral )
		{
		for ( TInt i = 0; i < iRelationConditionNameArray->Count(); i++)
			{
			if ( relationConditionStr.Compare( (*iRelationConditionNameArray)[i] ) == 0 )
				{
				return i;	
				}
			}
		return KErrNotFound;
		}
	else
		{
		return integer ;	
		}
	}
	
	

TInt CMdETestScripter::GetPropertyDefIndexL( CStifItemParser& aItem )
	{
	TPtrC propertyDefStr;
	User::LeaveIfError( aItem.GetNextString( propertyDefStr ));
	TLex lex( propertyDefStr );
	TInt integer;
	TInt err = lex.Val( integer );
	if ( err == KErrGeneral )
		{
		if ( propertyDefStr.Compare( _L("}")) == 0)
			{ 
			return KErrCompletion;	// indictes end of list
			}
		for ( TInt i = 0; i < iPropertyDefNameArray->Count(); i++)
			{
			if ( propertyDefStr.Compare( (*iPropertyDefNameArray)[i] ) == 0 )
				{
				return i;	
				}
			}
		return KErrNotFound;
		}
	else
		{
		return integer ;	
		}
	}

TInt CMdETestScripter::GetPropertyIndexL( CStifItemParser& aItem )
	{
	TPtrC propertyStr;
	User::LeaveIfError( aItem.GetNextString( propertyStr ));
	TLex lex( propertyStr );
	TInt integer;
	TInt err = lex.Val( integer );
	if ( err == KErrGeneral )
		{
		for ( TInt i = 0; i < iPropertyNameArray->Count(); i++)
			{
			if ( propertyStr.Compare( (*iPropertyNameArray)[i] ) == 0 )
				{
				return i;	
				}
			}
		return KErrNotFound;
		}
	else
		{
		return integer ;	
		}
	}


CMdEObjectDef& CMdETestScripter::GetObjectDefStrL( CStifItemParser& aItem )
	{
	TPtrC objectDefStr;
	User::LeaveIfError( aItem.GetNextString( objectDefStr ));
	TLex lex( objectDefStr );
	TInt integer;
	TInt err = lex.Val( integer );
	if ( err == KErrGeneral )
		{
		return iNamespaceDefArray[0]->GetObjectDefL( objectDefStr );		
		}
	else
		{
		return *iObjectDefArray[ integer ];	
		}
	}

TObjectConditionCompareMethod CMdETestScripter::CompareMethodL(TDesC& aMethodString)
	{
	TObjectConditionCompareMethod methodEnum = EObjectConditionCompareNone;
	if ( aMethodString.Compare( KNone ) == 0 )
		{
		methodEnum = EObjectConditionCompareNone;	
		}
	else if ( aMethodString.Compare( KId ) == 0 )
		{
		methodEnum = EObjectConditionCompareId;	
		}
	else if ( aMethodString.Compare( KIds ) == 0 )
		{
		methodEnum = EObjectConditionCompareIds;	
		}
	else if ( aMethodString.Compare( KGuid ) == 0 )
		{
		methodEnum = EObjectConditionCompareGuid;	
		}
	else if ( aMethodString.Compare( KObjectDef ) == 0 )
		{
		methodEnum = EObjectConditionCompareObjectDef;	
		}
	else if ( aMethodString.Compare( KUri ) == 0 )
		{
		methodEnum = EObjectConditionCompareUri;	
		}
	else if ( aMethodString.Compare( KUriBeginsWith ) == 0 )
		{
		methodEnum = EObjectConditionCompareUriBeginsWith;	
		}
	else if ( aMethodString.Compare( KFreeText ) == 0 )
		{
		methodEnum = EObjectConditionCompareFreeText;	
		}
	else if ( aMethodString.Compare( KFreeTextContains ) == 0 )
		{
		methodEnum = EObjectConditionCompareFreeTextContains;	
		}
	else if ( aMethodString.Compare( KFreeTextBeginsWith ) == 0 )
		{
		methodEnum = EObjectConditionCompareFreeTextBeginsWith;	
		}					
	else if ( aMethodString.Compare( KFreeTextEndsWith ) == 0 )
		{
		methodEnum = EObjectConditionCompareFreeTextEndsWith;	
		}
	else if ( aMethodString.Compare( KFreeTextUsageCount ) == 0 )
		{
		methodEnum = EObjectConditionCompareUsageCount;	
		}
	else
        {
    	User::Leave( KErrArgument );
        }
    return methodEnum;	
	}


TMdERangeType CMdETestScripter::RangeTypeL(TDesC& aTypeString)
	{
	TMdERangeType rangeType = EMdERangeTypeAny;
	if ( aTypeString.Compare( KAny ) == 0 )
		{
		rangeType = EMdERangeTypeAny;	
		}
	else if ( aTypeString.Compare( KEqual ) == 0 )
		{
		rangeType = EMdERangeTypeEqual;	
		}
	else if ( aTypeString.Compare( KNotEqual ) == 0 )
		{
		rangeType = EMdERangeTypeNotEqual;	
		}
	else if ( aTypeString.Compare( KLess ) == 0 )
		{
		rangeType = EMdERangeTypeLess;	
		}
	else if ( aTypeString.Compare( KLessOrEqual ) == 0 )
		{
		rangeType = EMdERangeTypeLessOrEqual;	
		}
	else if ( aTypeString.Compare( KGreater ) == 0 )
		{
		rangeType = EMdERangeTypeGreater;	
		}
	else if ( aTypeString.Compare( KGreaterOrEqual ) == 0 )
		{
		rangeType = EMdERangeTypeGreaterOrEqual;	
		}
	else if ( aTypeString.Compare( KBetween ) == 0 )
		{
		rangeType = EMdERangeTypeBetween;	
		}
	else if ( aTypeString.Compare( KNotBetween ) == 0 )
		{
		rangeType = EMdERangeTypeNotBetween;	
		}
	else
        {
    	User::Leave( KErrArgument );
        }
    return rangeType;
	}
	

// -----------------------------------------------------------------------------	
// CMdETestScripter::AddObjectConditionL
// -----------------------------------------------------------------------------
//    
TInt CMdETestScripter::AddObjectConditionL( CStifItemParser& aItem )
	{
	TPtrC name;
	User::LeaveIfError( aItem.GetNextString( name ));	
	CMdEObjectCondition* newObjectCondition = NULL;
	
	
	CMdELogicCondition* logicCondition = SanitizeArrayIndexL( iLogicConditionArray, *iLogicConditionNameArray, aItem );
	
#if 0
	logicConditionIndex = GetLogicConditionIndexL( aItem );    
	CMdELogicCondition* logicCondition;
	if ( logicConditionIndex >= 0 )
		{
		logicCondition = iLogicConditionArray[ logicConditionIndex ];
		}
	else if ( logicConditionIndex == KRootCond )
		{
		logicCondition = &iQuery->Conditions();	
		}
	else
		{
		User::Leave( logicConditionIndex );	
		}

#endif

	TPtrC type;
	User::LeaveIfError( aItem.GetNextString( type ));

	_LIT(KObjectConditionCompare , "ObjectConditionCompare");

	_LIT(KRange , "Range");
	
	if ( type.Compare( KObject ) == 0)
		{
		TInt firstIndex = GetObjectIndexL( aItem );
		TInt objectCount;
		User::LeaveIfError( aItem.GetNextInt( objectCount ));
		if ( objectCount == 1 )
			{
			newObjectCondition = &logicCondition->AddObjectConditionL( iObjectArray[ firstIndex ]->Id() );	
			}
		else
			{
			RArray<TItemId> objectIdArray;
			for ( TInt i = firstIndex ; i < firstIndex + objectCount ; i++ )
				{
				objectIdArray.Append( iObjectArray[ i ]->Id() );
				}
			newObjectCondition = &logicCondition->AddObjectConditionL( objectIdArray );			
			objectIdArray.Reset();	
			}
		}
	else if ( type.Compare( KObjectDef ) == 0 )
		{
		newObjectCondition = &logicCondition->AddObjectConditionL( GetObjectDefStrL( aItem ) );
		}
	else if ( type.Compare( KGuid ) == 0 )
		{
	    TInt guidHigh;
	    TInt guidLow;
	    TInt64 guidHigh64;
	    TInt64 guidLow64;	
		User::LeaveIfError( aItem.GetNextInt( guidHigh ));
		guidHigh64 = guidHigh;
    	User::LeaveIfError( aItem.GetNextInt( guidLow ));
    	guidLow64 = guidLow;
		newObjectCondition = &logicCondition->AddObjectConditionL( guidHigh64, guidLow64 );
		}
	else if ( type.Compare( KObjectConditionCompare ) == 0 )
		{
		TPtrC method;
		User::LeaveIfError( aItem.GetNextString( method ));
		TObjectConditionCompareMethod methodEnum = CompareMethodL( method );

	    TPtrC string;
	    User::LeaveIfError( aItem.GetNextString( string ));
	    if ( methodEnum == EObjectConditionCompareId )
		    {
		    TLex lex( string );
			TInt integer;
			TInt err = lex.Val( integer );
			if ( err == KErrGeneral )
				{ // string is not a number so it should be a object name
				for ( TInt i = 0; i < iObjectNameArray->Count(); i++)
					{
					if ( string.Compare( (*iObjectNameArray)[i] ) == 0 )
						{
						TBuf<20> objIdString;
						objIdString.Num(iObjectArray[i]->Id());
						newObjectCondition = &logicCondition->AddObjectConditionL(
			    			EObjectConditionCompareId, 
			    			objIdString );
			    		break;
						}
					}					
				}
		    }
		else
	    	{
	    	newObjectCondition = &logicCondition->AddObjectConditionL(
    			methodEnum, 
    			string);
	    	}
		}
	else if ( type.Compare( KRange ) == 0 )
		{
		TPtrC type;
		User::LeaveIfError( aItem.GetNextString( type ));	
		TMdERangeType rangeType = RangeTypeL( type );
	    					
		TInt min;
		User::LeaveIfError( aItem.GetNextInt( min ) );
		TInt max;
		User::LeaveIfError( aItem.GetNextInt( max ) );
		newObjectCondition = &logicCondition->AddObjectConditionL( TMdEUintRange(min, max, rangeType));				
		}
	else
        {
    	User::Leave( KErrArgument );
        }
    iObjectConditionArray.Append( newObjectCondition );      
    AppendNameL( iObjectConditionNameArray, name );
          
	return KErrNone;	
	}	


_LIT(KTimeRange, "TimeRange");

// -----------------------------------------------------------------------------	
// CMdETestScripter::AddPropertyConditionL
// -----------------------------------------------------------------------------
//    
TInt CMdETestScripter::AddPropertyConditionL( CStifItemParser& aItem )
	{
	
	TPtrC name;
	User::LeaveIfError( aItem.GetNextString( name ));	
	
	CMdELogicCondition* logicCondition = SanitizeArrayIndexL( iLogicConditionArray, *iLogicConditionNameArray, aItem );
	
	CMdEPropertyDef* propertyDef = SanitizeArrayIndexL( iPropertyDefArray, *iPropertyDefNameArray, aItem);

	TPtrC type;
	User::LeaveIfError( aItem.GetNextString( type ));
	_LIT(KPropCond, "PropCond");
	_LIT(KBool, "Bool");
	_LIT(KIntRange, "IntRange");
	_LIT(KInt64Range, "Int64Range");
	_LIT(KUintRange, "UintRange");
	_LIT(KRealRange, "RealRange");

	_LIT(KTextCompare, "TextCompare");
	_LIT(KEquals, "Equals");
	_LIT(KContains, "Contains");	
	_LIT(KBeginsWith, "BeginsWith");
	_LIT(KEndsWith, "EndsWith");
	
	CMdEPropertyCondition* propertyCondition = NULL;
	if (type.Compare( KPropCond ) == 0)
		{
		
		propertyCondition = &logicCondition->AddPropertyConditionL( *propertyDef );	
		}
	else if (type.Compare( KBool ) == 0)
		{
		TInt boolean;
		User::LeaveIfError( aItem.GetNextInt( boolean ) );
		}
	else if (type.Compare( KIntRange ) == 0)
		{
		TPtrC type;
		User::LeaveIfError( aItem.GetNextString( type ));	
		TMdERangeType rangeType = RangeTypeL( type );
	    					
		TInt min;
		User::LeaveIfError( aItem.GetNextInt( min ) );
		TInt max;
		User::LeaveIfError( aItem.GetNextInt( max ) );
		propertyCondition = &logicCondition->AddPropertyConditionL( *propertyDef, TMdEIntRange( min, max, rangeType ) );	
		}
	else if (type.Compare( KInt64Range ) == 0)
		{		
		TPtrC type;
		User::LeaveIfError( aItem.GetNextString( type ));	
		TMdERangeType rangeType = RangeTypeL( type );
	    					
		TInt min;
		User::LeaveIfError( aItem.GetNextInt( min ) );
		TInt64 min64 = min ;
		TInt max;
		User::LeaveIfError( aItem.GetNextInt( max ) );
		TInt64 max64 = max ;
		propertyCondition = &logicCondition->AddPropertyConditionL( *propertyDef, TMdEInt64Range( min64, max64, rangeType ) );		
		}
	else if (type.Compare( KUintRange ) == 0)
		{
		TPtrC type;
		User::LeaveIfError( aItem.GetNextString( type ));	
		TMdERangeType rangeType = RangeTypeL( type );
	    					
		TUint min;
		User::LeaveIfError( aItem.GetNextInt( min ) );
		TUint max;
		User::LeaveIfError( aItem.GetNextInt( max ) );
		propertyCondition = &logicCondition->AddPropertyConditionL( *propertyDef, TMdEUintRange( min, max, rangeType ) );	
		}
	else if (type.Compare( KRealRange ) == 0)
		{
		TPtrC type;
		User::LeaveIfError( aItem.GetNextString( type ));

		TMdERangeType rangeType = RangeTypeL( type );
	    					
		TReal min;
		TPtrC minStr;
		User::LeaveIfError( aItem.GetNextString( minStr ) );
		TLex lex( minStr );
		lex.Val( min );
		
		TReal max;
		TPtrC maxStr;
		User::LeaveIfError( aItem.GetNextString( maxStr ) );
		lex.Assign( maxStr );
		lex.Val( max );
		
		propertyCondition = &logicCondition->AddPropertyConditionL( *propertyDef, TMdEIntRange( min, max, rangeType ) );	
		}
	else if (type.Compare( KTimeRange ) == 0)
		{
		TPtrC type;
		User::LeaveIfError( aItem.GetNextString( type ));

		TMdERangeType rangeType = RangeTypeL( type );
	    					
		TTime min;
		TPtrC minStr;
		User::LeaveIfError( aItem.GetNextString( minStr ) );
		min.Set( minStr );
		
		TTime max;
		TPtrC maxStr;
		User::LeaveIfError( aItem.GetNextString( maxStr ) );
		max.Set( maxStr );

		
		propertyCondition = &logicCondition->AddPropertyConditionL( *propertyDef, TMdETimeRange( min, max, rangeType ) );		
		}
	else if (type.Compare( KTextCompare ) == 0)
		{
		TPtrC type;
		User::LeaveIfError( aItem.GetNextString( type ));
		TTextPropertyConditionCompareMethod compareMethod = ETextPropertyConditionCompareEquals;
		if (type.Compare( KEquals ) == 0)
			{
			compareMethod = ETextPropertyConditionCompareEquals;
			}
		else if (type.Compare( KContains ) == 0)
			{
			compareMethod = ETextPropertyConditionCompareContains;
			}
		else if (type.Compare( KBeginsWith ) == 0)
			{
			compareMethod = ETextPropertyConditionCompareBeginsWith;
			}
		else if (type.Compare( KEndsWith ) == 0)
			{
			compareMethod = ETextPropertyConditionCompareEndsWith;
			}
		else
			{
			User::Leave( KErrArgument );			
			}
		TPtrC text;
		User::LeaveIfError( aItem.GetNextString( text ) );
		propertyCondition = &logicCondition->AddPropertyConditionL(
    		*propertyDef,
    		compareMethod,
            text);
		}
	else
		{
		User::Leave( KErrArgument );	
		}
		
		iPropertyConditionArray.Append( propertyCondition ); 	 

    AppendNameL(iPropertyConditionNameArray, name );		
	return KErrNone;		
	}
    

_LIT(KTimeAndEventCompare, "TimeAndEventCompare");

// -----------------------------------------------------------------------------	
// CMdETestScripter::AddRelationConditionL
// -----------------------------------------------------------------------------
//    
TInt CMdETestScripter::AddRelationConditionL( CStifItemParser& aItem )
	{
	_LIT(KRelationId, "RelationId");
	_LIT(KLeft, "Left");
	_LIT(KRight, "Right");
	_LIT(KEither, "Either");
	TPtrC name;
	User::LeaveIfError( aItem.GetNextString( name ));	

	CMdELogicCondition* logicCondition = SanitizeArrayIndexL( iLogicConditionArray, *iLogicConditionNameArray, aItem );

#if 0	
	TInt logicConditionIndex = GetLogicConditionIndexL( aItem );    
	CMdELogicCondition* logicCondition;
	if ( logicConditionIndex >= 0 )
		{
		logicCondition = iLogicConditionArray[ logicConditionIndex ];
		}
	else if ( logicConditionIndex == KRootCond )
		{
		logicCondition = &iQuery->Conditions();	
		}
	else
		{
		User::Leave( logicConditionIndex );	
		}
#endif

	CMdERelationCondition* newRelation = NULL;
	
	TPtrC side;
	User::LeaveIfError( aItem.GetNextString( side ));
	TRelationConditionSide condSide = ERelationConditionSideEither;
	if (side.Compare( KLeft ) == 0)
		{
		condSide = ERelationConditionSideLeft;
		}
	else if (side.Compare( KRight ) == 0)
		{
		condSide = ERelationConditionSideRight;
		}
	else if (side.Compare( KEither ) == 0)
		{
		condSide = ERelationConditionSideEither;
		}
	else
		{
		User::Leave( KErrArgument );	
		}	
	TPtrC type;
	User::LeaveIfError( aItem.GetNextString( type ));
	if (type.Compare( KRelationId ) == 0)
		{
		TInt id;

		TInt relationIdCount;
		User::LeaveIfError( aItem.GetNextInt( relationIdCount ));
		if ( relationIdCount == 1 )
			{
			User::LeaveIfError( aItem.GetNextInt( id ) );
			newRelation = &logicCondition->AddRelationConditionL( TItemId ( id ) , condSide);	
			}
		else
			{
			RArray<TItemId> relationIdArray;
			for ( TInt i = 0 ; i < relationIdCount ; i++ )
				{
				aItem.GetNextInt( id );
				relationIdArray.Append( TItemId ( id ));
				}
			newRelation = &logicCondition->AddRelationConditionL( TItemId ( id ), condSide );			
			relationIdArray.Reset();	
			}		
		
		}
	else if ( type.Compare( KRelationDef ) == 0)
		{
		CMdERelationDef& relationDef = GetRelationDefStrL( aItem );
		newRelation = &logicCondition->AddRelationConditionL( relationDef, condSide );
		}
	else if (type.Compare( KRelationDefRange ) == 0)
		{
		CMdERelationDef& relationDef = GetRelationDefStrL( aItem );
		TPtrC type;
		User::LeaveIfError( aItem.GetNextString( type ));	
		TMdERangeType rangeType = RangeTypeL( type );
	    					
		TInt min;
		User::LeaveIfError( aItem.GetNextInt( min ) );
		TInt max;
		User::LeaveIfError( aItem.GetNextInt( max ) );

		newRelation = &logicCondition->AddRelationConditionL( relationDef,
			 TMdEIntRange( min, max, rangeType ), condSide );
		}
	else
		{
		User::Leave( KErrArgument );			
		}
	
	iRelationConditionArray.Append( newRelation );
	AppendNameL( iRelationConditionNameArray, name );
	
	return KErrNone;			
	}

// -----------------------------------------------------------------------------	
// CMdETestScripter::AddEventConditionL
// -----------------------------------------------------------------------------
// 	
TInt CMdETestScripter::AddEventConditionL( CStifItemParser& aItem )
	{
	_LIT(KEventId, "EventId");
	_LIT(KNoParam, "NoParam");
	_LIT(KEventDef, "EventDef");
	_LIT(KEventCompare, "EventCompare");
	
	TPtrC name;
	User::LeaveIfError( aItem.GetNextString( name ));
	CMdEEventCondition* newEvent = NULL;
		
	TInt logicConditionIndex = GetLogicConditionIndexL( aItem );    
	CMdELogicCondition* logicCondition = NULL;
	if ( logicConditionIndex >= 0 )
		{
		logicCondition = iLogicConditionArray[ logicConditionIndex ];
		}
	else if ( logicConditionIndex == KRootCond )
		{
		logicCondition = &iQuery->Conditions();	
		}
	else
		{
		User::Leave( logicConditionIndex );	
		}
	
	TPtrC type;
	User::LeaveIfError( aItem.GetNextString( type ));
	if (type.Compare( KNoParam ) == 0)
		{
		logicCondition->AddEventConditionL();		
		}
	else if (type.Compare( KEventId ) == 0)
		{
		TInt id;
		User::LeaveIfError( aItem.GetNextInt( id ) );
		newEvent = &logicCondition->AddEventConditionL( TItemId(id) );		
		}
	else if (type.Compare( KEventDef ) == 0)
		{
	    TPtrC eventString;
	    User::LeaveIfError( aItem.GetNextString( eventString ));

		CMdEEventDef& evdef = iNamespaceDefArray[0]->GetEventDefL( eventString );
		newEvent = &logicCondition->AddEventConditionL( evdef );		
		}
	else if (type.Compare( KTimeRange ) == 0)
		{
		TPtrC type;
		User::LeaveIfError( aItem.GetNextString( type ));

		TMdERangeType rangeType = RangeTypeL( type );
	    					
		TInt64 min;
		TPtrC minStr;
		User::LeaveIfError( aItem.GetNextString( minStr ) );
		TLex lex( minStr );
		lex.Val( min );
		
		TInt64 max;
		TPtrC maxStr;
		User::LeaveIfError( aItem.GetNextString( maxStr ) );
		lex.Assign( maxStr );
		lex.Val( max );
		
		newEvent = &logicCondition->AddEventConditionL( TMdETimeRange( min, max, rangeType ) );		
		}		
	else if (type.Compare( KEventCompare ) == 0)
		{
		

		TPtrC type;
		User::LeaveIfError( aItem.GetNextString( type ));

		_LIT( KNone, "None");
		_LIT( KSourceURI, "SourceURI");
		_LIT( KParticipantURI, "ParticipantURI");
		_LIT( KId, "Id");
		
		TEventConditionCompareMethod compareMethod = EEventConditionCompareNone;
		if (type.Compare( KNone ) == 0)
			{
			compareMethod = EEventConditionCompareNone;	
			}
		else if (type.Compare( KSourceURI ) == 0)
			{
			compareMethod = EEventConditionCompareSourceURI;	
			}
		else if (type.Compare( KParticipantURI ) == 0)
			{
			compareMethod = EEventConditionCompareParticipantURI;	
			}
		else if (type.Compare( KId ) == 0)
			{
			compareMethod = EEventConditionCompareId;	
			}
		else 		
			{
			User::Leave( KErrArgument );			
			}
	
		TPtrC text;
		User::LeaveIfError( aItem.GetNextString( text ) );
		newEvent = &logicCondition->AddEventConditionL( compareMethod, text );
		}
	else if (type.Compare( KTimeAndEventCompare ) == 0)
		{

		_LIT( KNone, "None");
		_LIT( KSourceURI, "SourceURI");
		_LIT( KParticipantURI, "ParticipantURI");
		_LIT( KId, "Id");

		TPtrC type;
		User::LeaveIfError( aItem.GetNextString( type ));

		TMdERangeType rangeType = RangeTypeL( type );
	    					
		TInt64 min;
		TPtrC minStr;
		User::LeaveIfError( aItem.GetNextString( minStr ) );
		TLex lex( minStr );
		lex.Val( min );
		
		TInt64 max;
		TPtrC maxStr;
		User::LeaveIfError( aItem.GetNextString( maxStr ) );
		lex.Assign( maxStr );
		lex.Val( max );
		
		TEventConditionCompareMethod compareMethod = EEventConditionCompareNone;
		if (type.Compare( KNone ) == 0)
			{
			compareMethod = EEventConditionCompareNone;	
			}
		else if (type.Compare( KSourceURI ) == 0)
			{
			compareMethod = EEventConditionCompareSourceURI;	
			}
		else if (type.Compare( KParticipantURI ) == 0)
			{
			compareMethod = EEventConditionCompareParticipantURI;	
			}
		else if (type.Compare( KId ) == 0)
			{
			compareMethod = EEventConditionCompareId;	
			}
		else 		
			{
			User::Leave( KErrArgument );			
			}
	
		TPtrC text;
		User::LeaveIfError( aItem.GetNextString( text ) );
		newEvent = &logicCondition->AddEventConditionL( TMdETimeRange( min, max, rangeType ),
			 compareMethod, text );
		}

	AppendNameL( iEventConditionNameArray , name );
	iEventConditionArray.AppendL( newEvent );				
	return KErrNone;		
	}
    
// -----------------------------------------------------------------------------	
// CMdETestScripter::ObjectConditionsL
// -----------------------------------------------------------------------------
// 	
TInt CMdETestScripter::ObjectConditionsL( CStifItemParser& aItem )
	{
	TPtrC name;
	User::LeaveIfError( aItem.GetNextString( name ));
	CMdEEventCondition* eventCondition = SanitizeArrayIndexL( iEventConditionArray, *iEventConditionNameArray, aItem );
	CMdELogicCondition& objectConditions = eventCondition->ObjectConditionsL();
	AppendNameL( iLogicConditionNameArray , name );
	iLogicConditionArray.AppendL( &objectConditions );				
	return KErrNone;			
	}
    
#if 0    
    
void CMdETestScripter::HandleQueryCompleted(CMdEQuery& aQuery, TInt aError)
	{	
	TBuf<128> buf;
	
	Signal();
	
	if (aQuery.Type() == EQueryTypeObject)
		{
		iLog->Log(_L("Object query completed notification: Error=%d"), aError);

        TQueryResultMode mode = aQuery.ResultMode();        
        if ( mode == EQueryResultModeItem )
            {
			TInt cou = aQuery.Count();

			buf.Format(_L("  EModeItem, results=%d"), cou);
			iLog->Log(buf);

			for( TInt i = 0; i < cou; i++ )
				{
				const TInt64 rid = (TInt64)aQuery.ResultItem( i ).Id();
	            buf.Format(_L("  Id %d = %Ld"), i, rid);
    	        iLog->Log(buf);
				}
            }
        else if ( mode == EQueryResultModeId )
            {
            TInt cou = aQuery.Count();
            
            buf.Format(_L("  EModeId, results=%d"), cou);
            iLog->Log(buf);
			TInt i;
			TInt64 rid;
			RArray<TInt64> ridArray;
			for( i = 0; i < cou; i++ )
				{
				rid = (TInt64)aQuery.ResultId( i );
				ridArray.Append( rid );
				}

			for( i = cou; --i >= 0; )
				{
				for( TInt j = iResultIndices.Count(); --j >= 0 ; )
					{
					if ( iObjectArray[ iResultIndices[j]]->Id() == ridArray[i] )
						{ // matching result
						iResultIndices.Remove( j );
						ridArray.Remove( i );
						}
					}
				}
				
			if ( iResultIndices.Count() != 0 || iObjectArray.Count() != 0 )
				{ 
				_LIT( KTestMessage1, "Not expected object query result" );
				iLog->Log( KTestMessage1 );
				return;
				}
            }
        else if ( mode == EQueryResultModeCount )
            {
            TInt cou = aQuery.Count();
            buf.Format(_L("  EModeCount, results=%d"), cou);
            iLog->Log(buf);
            }
		}
	else if (aQuery.Type() == EQueryTypeRelation)
		{
		iLog->Log(_L("Relation query completed notification: Error=%d"), aError);
		
        TInt cou = aQuery.Count();
            
        buf.Format(_L("  results=%d"), cou);
        iLog->Log(buf);
		}
	else if (aQuery.Type() == EQueryTypeEvent)
		{
		iLog->Log(_L("Event query completed notification: Error=%d"), aError);

        TInt cou = aQuery.Count();
            
        buf.Format(_L("  results=%d"), cou);
        iLog->Log(buf);
		}
	else
		{
		_LIT( KTestMessage2, "Query completed notification of unknown query" );
		iLog->Log( KTestMessage2 );
		}
	}

#else

void CMdETestScripter::HandleQueryCompleted(CMdEQuery& aQuery, TInt /* aError */ )
	{	
	TBuf<128> buf;
	
	Signal();
	
	RArray<TInt64> expectedIdArray;
    RArray<TInt64> resultIdArray;	
	
	if (aQuery.Type() == EQueryTypeObject)
		{
		for( TInt j = iResultIndices.Count(); --j >= 0 ; )
			{
			expectedIdArray.Append( iObjectArray[ iResultIndices[j]]->Id() );
			}
		}
	else if (aQuery.Type() == EQueryTypeRelation)
		{
		for( TInt j = iResultIndices.Count(); --j >= 0 ; )
			{
			expectedIdArray.Append( iRelationArray[ iResultIndices[j]]->Id() );
			}
		}
	else if (aQuery.Type() == EQueryTypeEvent)
		{
		for( TInt j = iResultIndices.Count(); --j >= 0 ; )
			{
			expectedIdArray.Append( iEventArray[ iResultIndices[j]]->Id() );
			}
		}
	else
		{
		_LIT( KTestMessage3, "Query completed notification of unknown query" );
		iLog->Log( KTestMessage3 );
		}
		
	TQueryResultMode mode = aQuery.ResultMode(); 
	if ( mode == EQueryResultModeCount )
		{
		if (aQuery.Count() != iResultIndices.Count())
			{
			_LIT( KTestMessage4, "Not expected query result count" );
			iLog->Log( KTestMessage4 );
			expectedIdArray.Reset();
			return;
			}
		}
	else
		{
		for (TInt i = 0; i < aQuery.Count(); i++)
			{
			if (aQuery.ResultMode() == EQueryResultModeId)
				{
				_LIT(KFormat3,"ID: %Ld");
				const TInt64 id = (TInt64)aQuery.ResultId(i);
				resultIdArray.Append( id );
				iLog->Log(KFormat3, id);
				}
			else if (aQuery.ResultMode() == EQueryResultModeItem
					|| aQuery.ResultMode() == EQueryResultModeObjectWithFreetexts)
				{
				switch( aQuery.Type() )
					{
					case EQueryTypeObject:
						{
						_LIT(KFormat5o,"Object ID: %Ld URI: '%S' Def: '%S' Properties: %d Freetexts: %d UsageCount: %d");

						CMdEObject& obj = (CMdEObject&)aQuery.ResultItem(i);
						const TInt64 objId = (TInt64)obj.Id();
						resultIdArray.Append( objId );
						iLog->Log(KFormat5o, objId, &obj.Uri(), &obj.Def().Name(), obj.PropertyCount(), obj.FreeTextCount(), obj.UsageCount());
						}
						break;
					case EQueryTypeRelation:
						{
						_LIT(KFormat4r,"Relation ID: %Ld Def: '%S' Left ID: %Ld Right ID: %Ld Param: %d");

						CMdERelation& rel = (CMdERelation&)aQuery.ResultItem(i);
						const TInt64 relId = (TInt64)rel.Id();
						resultIdArray.Append( relId );
						const TInt64 relLId = (TInt64)rel.LeftObjectId();
						const TInt64 relRId = (TInt64)rel.RightObjectId();
						iLog->Log(KFormat4r, relId, &rel.Def().Name(), relLId, relRId, rel.Parameter());
						}
						break;
					case EQueryTypeEvent:
						{
						_LIT(KFormat4e,"Event ID: %Ld Def: '%S' Object ID: %Ld");

						CMdEEvent& eve = (CMdEEvent&)aQuery.ResultItem(i);
						const TInt64 eveId = (TInt64)eve.Id();
						resultIdArray.Append( eveId );
						const TInt64 eveOId = (TInt64)eve.ObjectId();
						iLog->Log(KFormat4e, eveId, &eve.Def().Name(), eveOId);
						}
						break;
					}
				}
			else if (aQuery.ResultMode() == EQueryResultModeDistinctValues)
				{
				_LIT(KFormat5,"Found with text: ");

				buf = KFormat5;
				buf.Append( aQuery.ResultDistinctValue(i) );
				iLog->Log( buf );
				}
			}
		
		
		if (aQuery.ResultMode() == EQueryResultModeId || aQuery.ResultMode() == EQueryResultModeItem
					|| aQuery.ResultMode() == EQueryResultModeObjectWithFreetexts )	
			{
			for(TInt i = resultIdArray.Count(); --i >= 0; )
				{
				for( TInt j = expectedIdArray.Count(); --j >= 0 ; )
					{
					if ( expectedIdArray[j] == resultIdArray[i] )
						{ // matching result
						expectedIdArray.Remove( j );
						resultIdArray.Remove( i );
						break;
						}
					}
				}
				
			if ( expectedIdArray.Count() != 0 || resultIdArray.Count() != 0 )
				{ 		
				expectedIdArray.Reset();
				resultIdArray.Reset();
				_LIT( KTestMessage5, "Not expected object query result" );
				iLog->Log( KTestMessage5 );
				return;
				}			
			}
		}	
	}

#endif	



TInt CMdETestScripter::SetEventQueryResultL( CStifItemParser& aItem )
	{
	iResultIndices.Reset();
	TPtrC startMark;
	User::LeaveIfError( aItem.GetNextString( startMark ));
	TLex lex;
	TPtrC indexStr;
	TInt index;

	if ( startMark.Compare(_L("{")) == 0)
		{
		while( ETrue )
			{
			index = GetEventIndexL( aItem );
			if ( index == KErrCompletion )
				{
				break;
				}
			iResultIndices.Append( index );
			}
		}
	return KErrNone;
	}

TInt CMdETestScripter::SetRelationQueryResultL( CStifItemParser& aItem )
	{
	iResultIndices.Reset();
	TPtrC startMark;
	User::LeaveIfError( aItem.GetNextString( startMark ));
	TLex lex;
	TPtrC indexStr;
	TInt index;

	if ( startMark.Compare(_L("{")) == 0)
		{
		while( ETrue )
			{
			index = GetRelationIndexL( aItem );
			if ( index == KErrCompletion )
				{
				break;
				}
			iResultIndices.Append( index );
			}
		}
	return KErrNone;	
	}
	
	
TInt CMdETestScripter::SetObjectQueryResultL( CStifItemParser& aItem )
	{
	iResultIndices.Reset();
	TPtrC startMark;
	User::LeaveIfError( aItem.GetNextString( startMark ));
	TLex lex;
	TPtrC indexStr;
	TInt index;

	if ( startMark.Compare(_L("{")) == 0)
		{
		while( ETrue )
			{
			index = GetObjectIndexL( aItem );
			if ( index == KErrCompletion )
				{
				break;
				}
			iResultIndices.Append( index );
			}
		}
	return KErrNone;
	}
	

// -----------------------------------------------------------------------------	
// CMdETestScripter::TestPopulateDbL
// -----------------------------------------------------------------------------
//    
TInt CMdETestScripter::TestPopulateDbL( CStifItemParser& /*aItem*/ )
	{
	CMdENamespaceDef& namespaceDef = iMdeSession->GetDefaultNamespaceDefL();
	CMdEObjectDef& objectDef = namespaceDef.GetObjectDefL( MdeConstants::Album::KAlbumObject );
	CMdEObject* albumObject = iMdeSession->NewObjectL(
            objectDef, MdeConstants::Object::KAutomaticUri );
	CMdEPropertyDef& creationDatePropertyDef = objectDef.GetPropertyDefL( MdeConstants::Object::KCreationDateProperty );
	TTime time( 0 );
    albumObject->AddTimePropertyL( creationDatePropertyDef, time );
    
	CMdEPropertyDef& lastModifiedDatePropertyDef = objectDef.GetPropertyDefL( MdeConstants::Object::KLastModifiedDateProperty );

    albumObject->AddTimePropertyL( lastModifiedDatePropertyDef, time );
    iMdeSession->AddObjectL( *albumObject );	    
    
	CMdEObjectDef& audioObjectDef = namespaceDef.GetObjectDefL( MdeConstants::Audio::KAudioObject );
	CMdEObject* audioObject = iMdeSession->NewObjectL(
            audioObjectDef, MdeConstants::Object::KAutomaticUri );

    audioObject->AddTimePropertyL( creationDatePropertyDef, time );
    
    audioObject->AddTimePropertyL( lastModifiedDatePropertyDef, time );
    iMdeSession->AddObjectL( *audioObject );
    CMdERelationDef& relationDef = namespaceDef.GetRelationDefL( MdeConstants::Relations::KContains );
    
    CMdERelation* relation = iMdeSession->NewRelationL( relationDef, albumObject->Id(), audioObject->Id(), 0 );
    iMdeSession->AddRelationL( *relation );
    
    CMdEObjectDef& objDef = namespaceDef.GetObjectDefL( 	MdeConstants::Object::KBaseObject );
    
    delete iQuery;
    iQuery = NULL;
	iQuery = iMdeSession->NewObjectQueryL( namespaceDef, objDef, this );

	CMdELogicCondition& rootCond = iQuery->Conditions();

	CMdERelationDef& containsRelDef = namespaceDef.GetRelationDefL( 
		MdeConstants::Relations::KContains );

	CMdERelationCondition& relCond = rootCond.AddRelationConditionL( 
		containsRelDef, ERelationConditionSideRight );
	CMdELogicCondition& leftRelCond = relCond.LeftL();

	leftRelCond.AddObjectConditionL( albumObject->Id() );

	iQuery->SetResultMode( EQueryResultModeId );

	iQuery->FindL();
	return KErrNone;
	}

