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
#include <mdeconstants.h>
#include <StifParser.h>
#include <StifTestInterface.h>
#include <StifTestEventInterface.h>
#include "MdETestScripter.h"
#include "MdETestScripterAO.h"

// -----------------------------------------------------------------------------
// CMdETestScripter::BelongsToSessionL
// -----------------------------------------------------------------------------
//
TInt CMdETestScripter::BelongsToSessionL( CStifItemParser& aItem )
    {
    TInt mode;
    TInt state;

    User::LeaveIfError( aItem.GetNextInt( mode ));
    CMdEInstanceItem* instanceItem = GetInstanceItemByArrayIndexL( mode, aItem );
    User::LeaveIfError( aItem.GetNextInt( state ));

    TL( instanceItem->BelongsToSession() == (TBool)state );

    return KErrNone;
    }

// -----------------------------------------------------------------------------
// CMdETestScripter::SessionL
// -----------------------------------------------------------------------------
//
TInt CMdETestScripter::SessionL( CStifItemParser& aItem )
    {
    TInt mode;

    User::LeaveIfError( aItem.GetNextInt( mode ));
    CMdEInstanceItem* instanceItem = GetInstanceItemByArrayIndexL( mode, aItem );
    TL( iMdeSession == &instanceItem->Session());

    return KErrNone;
    }

// -----------------------------------------------------------------------------
// CMdETestScripter::InDatabaseL
// -----------------------------------------------------------------------------
//
TInt CMdETestScripter::InDatabaseL( CStifItemParser& aItem )
    {
    TInt mode;
    TInt state;

    User::LeaveIfError( aItem.GetNextInt( mode ));
    CMdEInstanceItem* instanceItem = GetInstanceItemByArrayIndexL( mode, aItem );
    User::LeaveIfError( aItem.GetNextInt( state ));
    TL( instanceItem->InDatabase() == (TBool)state );

    return KErrNone;
    }

// -----------------------------------------------------------------------------
// CMdETestScripter::AddItemsL
// -----------------------------------------------------------------------------
//
TInt CMdETestScripter::AddItemsL( CStifItemParser& aItem )
    {
    TInt mode1, err = KErrNone;
    User::LeaveIfError( aItem.GetNextInt( mode1 ));

    if ( mode1 == KSync )
        {
        RPointerArray<CMdEInstanceItem> itemArray;
        FormInstanceItemArrayL( aItem, itemArray );
        err = iMdeSession->AddItemsL( itemArray );
        }
    else if ( mode1 == KAsync )
        {
        FormInstanceItemArrayL( aItem, iItemArray );
        TEventIf event( TEventIf::ESetEvent, TName( KMdEItemsAsyncAdd ) );
        iAsyncHandler->Start( this, event );
        iMdEDataBuf.Close();// make sure the last async request buffer cleaned before using again.
        iMdeSession->AddItemsAsyncL( iItemArray, iAsyncHandler->iStatus, iMdEDataBuf );
        }
    else
        {
        User::Leave( KErrArgument );
        }

    return err;
    }

// -----------------------------------------------------------------------------
// CMdETestScripter::UpdateItemsL
// -----------------------------------------------------------------------------
//
TInt CMdETestScripter::UpdateItemsL( CStifItemParser& aItem )
    {
    TInt mode1, err = KErrNone;
    User::LeaveIfError( aItem.GetNextInt( mode1 ));

    if ( mode1 == KSync )
        {
        RPointerArray<CMdEInstanceItem> itemArray;
        FormInstanceItemArrayL( aItem, itemArray );
        err = iMdeSession->UpdateItemsL( itemArray );
        }
    else if ( mode1 == KAsync )
        {
        FormInstanceItemArrayL( aItem, iItemArray );
        TEventIf event( TEventIf::ESetEvent, TName( KMdEItemsAsyncUpdate ) );
        iAsyncHandler->Start( this, event );
        iMdEDataBuf.Close();// make sure the last async request buffer cleaned before using again.
        iMdeSession->UpdateItemsAsyncL( iItemArray, iAsyncHandler->iStatus, iMdEDataBuf );
        }
    else
        {
        User::Leave( KErrArgument );
        }

    return err;
    }

// -----------------------------------------------------------------------------
// CMdETestScripter::AddRelationDefL
// -----------------------------------------------------------------------------
//
TInt CMdETestScripter::AddRelationDefL( CStifItemParser& aItem )
    {
    TPtrC relationString;

    CMdENamespaceDef* namespaceDef = SanitizeArrayIndexL( iNamespaceDefArray, *iNamespaceDefNameArray, aItem );
    User::LeaveIfError( aItem.GetNextString( relationString ));
    TRAPD( err, iMdeSession->AddRelationDefL( *namespaceDef, relationString ) );
    if( KErrAlreadyExists == err )
        {
        TEventIf event( TEventIf::ESetEvent, TName( KMdESchemaModified ) );
        TestModuleIf().Event( event );
        }
    return err;
    }

// -----------------------------------------------------------------------------
// CMdETestScripter::AddEventDefL
// -----------------------------------------------------------------------------
//
TInt CMdETestScripter::AddEventDefL( CStifItemParser& aItem )
    {
    TPtrC eventString;

    CMdENamespaceDef* namespaceDef = SanitizeArrayIndexL( iNamespaceDefArray, *iNamespaceDefNameArray, aItem );
    User::LeaveIfError( aItem.GetNextString( eventString ));
    TRAPD( err, iMdeSession->AddEventDefL( *namespaceDef, eventString ) );
    if( KErrAlreadyExists == err )
        {
        TEventIf event( TEventIf::ESetEvent, TName( KMdESchemaModified ) );
        TestModuleIf().Event( event );
       	}
    return err;
    }

// -----------------------------------------------------------------------------
// CMdETestScripter::PropertyDefCountL
// -----------------------------------------------------------------------------
//
TInt CMdETestScripter::PropertyDefCountL( CStifItemParser& aItem )
    {
    TInt count;

    CMdEObjectDef* objectDef = SanitizeArrayIndexL( iObjectDefArray, *iObjectDefNameArray, aItem);
    User::LeaveIfError( aItem.GetNextInt( count ));
    TL( objectDef->PropertyDefCount() == count );

    return KErrNone;
    }

// -----------------------------------------------------------------------------
// CMdETestScripter::PropertyDefL
// -----------------------------------------------------------------------------
//
TInt CMdETestScripter::PropertyDefL( CStifItemParser& aItem )
    {
    TInt index;

    TPtrC name;
    User::LeaveIfError( aItem.GetNextString( name ));   

    CMdENamespaceDef* space = SanitizeArrayIndexL( iNamespaceDefArray, *iNamespaceDefNameArray, aItem);
    CMdEObjectDef* objectDef = SanitizeArrayIndexL( iObjectDefArray, *iObjectDefNameArray, aItem);
    User::LeaveIfError( aItem.GetNextInt( index ));
    CMdEPropertyDef& propertyDef = objectDef->PropertyDefL( index );
    TL( &propertyDef );
    iPropertyDefArray.Append( &propertyDef );
    AppendNameL( iPropertyDefNameArray, name );

    return KErrNone;
    }

// -----------------------------------------------------------------------------
// CMdETestScripter::GetPropertyDefL
// -----------------------------------------------------------------------------
//
TInt CMdETestScripter::GetPropertyDefL( CStifItemParser& aItem )
    {
    TPtrC propertyString;
    
    TPtrC name;
    User::LeaveIfError( aItem.GetNextString( name ));   

    CMdEObjectDef* objectDef = SanitizeArrayIndexL( iObjectDefArray, *iObjectDefNameArray, aItem);
    User::LeaveIfError( aItem.GetNextString( propertyString ));
    CMdEPropertyDef& propertyDef = objectDef->GetPropertyDefL( propertyString );
    iPropertyDefArray.Append( &propertyDef );
    AppendNameL( iPropertyDefNameArray, name );

    return KErrNone;
    }

// -----------------------------------------------------------------------------
// CMdETestScripter::CompareL
// -----------------------------------------------------------------------------
//
TInt CMdETestScripter::CompareL( CStifItemParser& aItem )
    {
    TInt mode;

    TInt expected;

    User::LeaveIfError( aItem.GetNextInt( mode ));
    TInt result( -1 );

    if ( mode == EObjectDef )
        {
        CMdEObjectDef* item1 = SanitizeArrayIndexL( iObjectDefArray, *iObjectDefNameArray, aItem );
        CMdEObjectDef* item2 = SanitizeArrayIndexL( iObjectDefArray, *iObjectDefNameArray, aItem );
        result = item1->Compare( *item2 );
        }
    else if ( mode == ERelationDef )
        {
        CMdERelationDef* item1 = SanitizeArrayIndexL( iRelationDefArray, *iRelationDefNameArray, aItem );
        CMdERelationDef* item2 = SanitizeArrayIndexL( iRelationDefArray, *iRelationDefNameArray, aItem );
        result = item1->Compare( *item2 );
        }
    else if ( mode == EEventDef )
        {
        CMdEEventDef* item1 = SanitizeArrayIndexL( iEventDefArray, *iEventDefNameArray, aItem );
        CMdEEventDef* item2 = SanitizeArrayIndexL( iEventDefArray, *iEventDefNameArray, aItem );
        result = item1->Compare( *item2 );
        }
    else if ( mode == EPropertyDef )
        {
        CMdEPropertyDef* item1 = SanitizeArrayIndexL( iPropertyDefArray, *iPropertyDefNameArray, aItem );
        CMdEPropertyDef* item2 = SanitizeArrayIndexL( iPropertyDefArray, *iPropertyDefNameArray, aItem );
        result = item1->Compare( *item2 );
        }
    else if ( mode == ENamespaceDef )
        {
        CMdENamespaceDef* item1 = SanitizeArrayIndexL( iNamespaceDefArray, *iNamespaceDefNameArray, aItem );
        CMdENamespaceDef* item2 = SanitizeArrayIndexL( iNamespaceDefArray, *iNamespaceDefNameArray, aItem );
        result = item1->Compare( *item2 );
        }
    else
        {
        User::Leave( KErrArgument );
        }
        
    User::LeaveIfError( aItem.GetNextInt( expected ));
    TL( result == expected );

    return KErrNone;
    }

// -----------------------------------------------------------------------------
// CMdETestScripter::ReadOnlyL
// -----------------------------------------------------------------------------
//
TInt CMdETestScripter::ReadOnlyL( CStifItemParser& aItem )
    {
    TInt mode;
    TBool state;

    User::LeaveIfError( aItem.GetNextInt( mode ));

    if ( mode == EPropertyDef )
        {
        CMdEPropertyDef* propertyDef = SanitizeArrayIndexL( iPropertyDefArray, *iPropertyDefNameArray, aItem );
        User::LeaveIfError( aItem.GetNextInt( state ));
        TL( propertyDef->ReadOnly() == (TBool)state );
        }
    else if ( mode == ENamespaceDef )
        {
        CMdENamespaceDef* namespaceDef = SanitizeArrayIndexL( iNamespaceDefArray, *iNamespaceDefNameArray, aItem );
        User::LeaveIfError( aItem.GetNextInt( state ));
        TL( namespaceDef->ReadOnly() == state );
        }

    return KErrNone;
    }

// -----------------------------------------------------------------------------
// CMdETestScripter::MandatoryL
// -----------------------------------------------------------------------------
//
TInt CMdETestScripter::MandatoryL( CStifItemParser& aItem )
    {
    TBool state;

    CMdEPropertyDef* propertyDef = SanitizeArrayIndexL( iPropertyDefArray, *iPropertyDefNameArray, aItem );
    User::LeaveIfError( aItem.GetNextInt( state ));
    TL( propertyDef->Mandatory() == (TBool)state );

    return KErrNone;
    }    

// -----------------------------------------------------------------------------
// CMdETestScripter::MinInt32ValueL
// -----------------------------------------------------------------------------
//
TInt CMdETestScripter::MinInt32ValueL( CStifItemParser& aItem )
    {
    TPtrC value;

    CMdEPropertyDef* propertyDef = SanitizeArrayIndexL( iPropertyDefArray, *iPropertyDefNameArray, aItem );
    User::LeaveIfError( aItem.GetNextString( value ));
    TLex lex( value );
    TInt32 valueU;
    lex.BoundedVal( valueU, 0x7FFFFFFF );
    TL( propertyDef->MinInt32ValueL() == valueU );

    return KErrNone;
    }

// -----------------------------------------------------------------------------
// CMdETestScripter::MaxInt32ValueL
// -----------------------------------------------------------------------------
//
TInt CMdETestScripter::MaxInt32ValueL( CStifItemParser& aItem )
    {
    TPtrC value;

    CMdEPropertyDef* propertyDef = SanitizeArrayIndexL( iPropertyDefArray, *iPropertyDefNameArray, aItem );
    User::LeaveIfError( aItem.GetNextString( value ));
    TLex lex( value );
    TInt32 valueU;
    lex.BoundedVal( valueU, 0x7FFFFFFF );
    TL( propertyDef->MaxInt32ValueL() == valueU );

    return KErrNone;
    }

// -----------------------------------------------------------------------------
// CMdETestScripter::MinUint32ValueL
// -----------------------------------------------------------------------------
//
TInt CMdETestScripter::MinUint32ValueL( CStifItemParser& aItem )
    {
    TPtrC value;

    CMdEPropertyDef* propertyDef = SanitizeArrayIndexL( iPropertyDefArray, *iPropertyDefNameArray, aItem );
    User::LeaveIfError( aItem.GetNextString( value ));
    TLex lex( value );
    TUint32 valueU;
    lex.BoundedVal( valueU, EDecimal, 0xFFFFFFFF );
    TL( propertyDef->MinUint32ValueL() == valueU );

    return KErrNone;
    }

// -----------------------------------------------------------------------------
// CMdETestScripter::MaxUint32ValueL
// -----------------------------------------------------------------------------
//
TInt CMdETestScripter::MaxUint32ValueL( CStifItemParser& aItem )
    {
    TPtrC value;

    CMdEPropertyDef* propertyDef = SanitizeArrayIndexL( iPropertyDefArray, *iPropertyDefNameArray, aItem );
    User::LeaveIfError( aItem.GetNextString( value ));
    TLex lex( value );
    TUint32 valueU;
    lex.BoundedVal( valueU, EDecimal, 0xFFFFFFFF );
    TL( propertyDef->MaxUint32ValueL() == valueU );

    return KErrNone;
    }

// -----------------------------------------------------------------------------
// CMdETestScripter::MinInt64ValueL
// -----------------------------------------------------------------------------
//
TInt CMdETestScripter::MinInt64ValueL( CStifItemParser& aItem )
    {
    TPtrC value;

    CMdEPropertyDef* propertyDef = SanitizeArrayIndexL( iPropertyDefArray, *iPropertyDefNameArray, aItem );
    User::LeaveIfError( aItem.GetNextString( value ));
    TLex lex( value );
    TInt64 value64;
    lex.Val( value64, 0x7FFFFFFFFFFFFFFF );
    TL( propertyDef->MinInt64ValueL() == value64 );

    return KErrNone;
    }

// -----------------------------------------------------------------------------
// CMdETestScripter::MaxInt64ValueL
// -----------------------------------------------------------------------------
//
TInt CMdETestScripter::MaxInt64ValueL( CStifItemParser& aItem )
    {
    TPtrC value;

    CMdEPropertyDef* propertyDef = SanitizeArrayIndexL( iPropertyDefArray, *iPropertyDefNameArray, aItem );
    User::LeaveIfError( aItem.GetNextString( value ));
    TLex lex( value );
    TInt64 value64;
    lex.Val( value64, 0x7FFFFFFFFFFFFFFF );
    TL( propertyDef->MaxInt64ValueL() == value64 );

    return KErrNone;
    }

// -----------------------------------------------------------------------------
// CMdETestScripter::MinRealValueL
// -----------------------------------------------------------------------------
//
TInt CMdETestScripter::MinRealValueL( CStifItemParser& aItem )
    {
    TPtrC value;

    CMdEPropertyDef* propertyDef = SanitizeArrayIndexL( iPropertyDefArray, *iPropertyDefNameArray, aItem );
    aItem.SetParsingType( CStifItemParser::EQuoteStyleParsing );
    User::LeaveIfError( aItem.GetNextString( value ));
    TLex lex( value );
    TReal valueR;
    lex.Val( valueR );
    TL( propertyDef->MinRealValueL() == valueR );

    return KErrNone;
    }

// -----------------------------------------------------------------------------
// CMdETestScripter::MaxRealValueL
// -----------------------------------------------------------------------------
//
TInt CMdETestScripter::MaxRealValueL( CStifItemParser& aItem )
    {
    TPtrC value;

    CMdEPropertyDef* propertyDef = SanitizeArrayIndexL( iPropertyDefArray, *iPropertyDefNameArray, aItem );
    aItem.SetParsingType( CStifItemParser::EQuoteStyleParsing );
    User::LeaveIfError( aItem.GetNextString( value ));
    TLex lex( value );
    TReal valueR;
    lex.Val( valueR );
    TL( propertyDef->MaxRealValueL() == valueR );

    return KErrNone;
    }

// -----------------------------------------------------------------------------
// CMdETestScripter::MinTimeValueL
// -----------------------------------------------------------------------------
//
TInt CMdETestScripter::MinTimeValueL( CStifItemParser& aItem )
    {
    TPtrC value;

    CMdEPropertyDef* propertyDef = SanitizeArrayIndexL( iPropertyDefArray, *iPropertyDefNameArray, aItem );
    User::LeaveIfError( aItem.GetNextString( value ));
    TTime time;
    TL( time.Set( value ) == KErrNone );
    TL( propertyDef->MinTimeValueL() == time );

    return KErrNone;
    }

// -----------------------------------------------------------------------------
// CMdETestScripter::MaxTimeValueL
// -----------------------------------------------------------------------------
//
TInt CMdETestScripter::MaxTimeValueL( CStifItemParser& aItem )
    {
    TPtrC value;

    CMdEPropertyDef* propertyDef = SanitizeArrayIndexL( iPropertyDefArray, *iPropertyDefNameArray, aItem );
    User::LeaveIfError( aItem.GetNextString( value ));
    TTime time;
    TL( time.Set( value ) == KErrNone );
    TL( propertyDef->MaxTimeValueL() == time );

    return KErrNone;
    }

// -----------------------------------------------------------------------------
// CMdETestScripter::MinTextLengthL
// -----------------------------------------------------------------------------
//
TInt CMdETestScripter::MinTextLengthL( CStifItemParser& aItem )
    {
    TInt value;

    CMdEPropertyDef* propertyDef = SanitizeArrayIndexL( iPropertyDefArray, *iPropertyDefNameArray, aItem );
    User::LeaveIfError( aItem.GetNextInt( value ));
    TL( propertyDef->MinTextLengthL() == value );

    return KErrNone;
    }

// -----------------------------------------------------------------------------
// CMdETestScripter::MaxTextLengthL
// -----------------------------------------------------------------------------
//
TInt CMdETestScripter::MaxTextLengthL( CStifItemParser& aItem )
    {
    TInt value;

    CMdEPropertyDef* propertyDef = SanitizeArrayIndexL( iPropertyDefArray, *iPropertyDefNameArray, aItem );
    User::LeaveIfError( aItem.GetNextInt( value ));
    TL( propertyDef->MaxTextLengthL() == value );

    return KErrNone;
    }

// -----------------------------------------------------------------------------
// CMdETestScripter::DefL
// -----------------------------------------------------------------------------
//
TInt CMdETestScripter::DefL( CStifItemParser& aItem )
    {
    TInt mode;
    TPtrC name;
    User::LeaveIfError( aItem.GetNextString( name ));

    User::LeaveIfError( aItem.GetNextInt( mode ));

    if ( mode == EObjectDef )
        {
           CMdEObject* object = SanitizeArrayIndexL( iObjectArray, *iObjectNameArray, aItem);
           CMdEObjectDef& objDef = object->Def();
        TL( &objDef );
        iObjectDefArray.Append( &objDef );
        AppendNameL( iObjectDefNameArray, name );
        }
    else if ( mode == ERelationDef )
        {
           CMdERelation* relation = SanitizeArrayIndexL( iRelationArray, *iRelationNameArray, aItem);
           CMdERelationDef& relDef = relation->Def();        
        TL( &relDef );
        iRelationDefArray.Append( &relDef );
        AppendNameL( iRelationDefNameArray, name );
        }
    else if ( mode == EPropertyDef )
        {
           CMdEProperty* property = SanitizeArrayIndexL( iPropertyArray, *iPropertyNameArray, aItem);
           const CMdEPropertyDef& propDef = property->Def();      
        TL( &propDef );
        iPropertyDefArray.Append( &propDef );
        AppendNameL( iPropertyDefNameArray, name );
        }
    else if ( mode == EEventDef )
        {
        CMdEEvent* event = SanitizeArrayIndexL( iEventArray, *iEventNameArray, aItem );
        CMdEEventDef& eventDef = event->Def();
        TL( &eventDef );
        iEventDefArray.Append( &eventDef );
        AppendNameL( iEventDefNameArray, name );
        }
    else
        {
        User::Leave( KErrArgument );
        }

    return KErrNone;
    }

// -----------------------------------------------------------------------------
// CMdETestScripter::SetDefL
// -----------------------------------------------------------------------------
//
TInt CMdETestScripter::SetDefL( CStifItemParser& aItem )
    {

    CMdEObject* object = SanitizeArrayIndexL( iObjectArray, *iObjectNameArray, aItem );
    CMdEObjectDef* objectDef = SanitizeArrayIndexL( iObjectDefArray, *iObjectDefNameArray, aItem );
    object->SetDefL( *objectDef );

    return KErrNone;
    }

// -----------------------------------------------------------------------------
// CMdETestScripter::UriL
// -----------------------------------------------------------------------------
//
TInt CMdETestScripter::UriL( CStifItemParser& aItem )
    {

    TPtrC uri;

    CMdEObject* object = SanitizeArrayIndexL( iObjectArray, *iObjectNameArray, aItem );
    User::LeaveIfError( aItem.GetNextString( uri ));
    TL( object->Uri().Compare( uri ) == 0 );

    return KErrNone;
    }

// -----------------------------------------------------------------------------
// CMdETestScripter::SetUriL
// -----------------------------------------------------------------------------
//
TInt CMdETestScripter::SetUriL( CStifItemParser& aItem )
    {

    TPtrC uri;

    CMdEObject* object = SanitizeArrayIndexL( iObjectArray, *iObjectNameArray, aItem );
    User::LeaveIfError( aItem.GetNextString( uri ));
    object->SetUriL( uri );

    return KErrNone;
    }

// -----------------------------------------------------------------------------
// CMdETestScripter::ConfidentialL
// -----------------------------------------------------------------------------
//
TInt CMdETestScripter::ConfidentialL( CStifItemParser& aItem )
    {
    TInt state;

    CMdEObject* object = SanitizeArrayIndexL( iObjectArray, *iObjectNameArray, aItem );
    User::LeaveIfError( aItem.GetNextInt( state ));
    TL( object->Confidential() == (TBool)state );

    return KErrNone;
    }

// -----------------------------------------------------------------------------
// CMdETestScripter::SetConfidentialL
// -----------------------------------------------------------------------------
//
TInt CMdETestScripter::SetConfidentialL( CStifItemParser& aItem )
    {
    TInt state;

    CMdEObject* object = SanitizeArrayIndexL( iObjectArray, *iObjectNameArray, aItem );
    User::LeaveIfError( aItem.GetNextInt( state ));

    object->SetConfidential( (TBool)state );

    return KErrNone;
    }

// -----------------------------------------------------------------------------
// CMdETestScripter::PlaceholderL
// -----------------------------------------------------------------------------
//
TInt CMdETestScripter::PlaceholderL( CStifItemParser& aItem )
    {
    TInt state;

    CMdEObject* object = SanitizeArrayIndexL( iObjectArray, *iObjectNameArray, aItem );
    User::LeaveIfError( aItem.GetNextInt( state ));
    TL( object->Placeholder() == (TBool)state );

    return KErrNone;
    }

// -----------------------------------------------------------------------------
// CMdETestScripter::SetPlaceholderL
// -----------------------------------------------------------------------------
//
TInt CMdETestScripter::SetPlaceholderL( CStifItemParser& aItem )
    {
    TInt state;

    CMdEObject* object = SanitizeArrayIndexL( iObjectArray, *iObjectNameArray, aItem );
    User::LeaveIfError( aItem.GetNextInt( state ));

    object->SetPlaceholder( (TBool)state );

    return KErrNone;
    }

// -----------------------------------------------------------------------------
// CMdETestScripter::MediaIdL
// -----------------------------------------------------------------------------
//
TInt CMdETestScripter::MediaIdL( CStifItemParser& aItem )
    {
    TInt mediaId;

    CMdEObject* object = SanitizeArrayIndexL( iObjectArray, *iObjectNameArray, aItem );
    User::LeaveIfError( aItem.GetNextInt( mediaId ));

    TL( object->MediaId() == mediaId );

    return KErrNone;
    }

// -----------------------------------------------------------------------------
// CMdETestScripter::SetMediaIdL
// -----------------------------------------------------------------------------
//
TInt CMdETestScripter::SetMediaIdL( CStifItemParser& aItem )
    {
    TInt mediaId;

    CMdEObject* object = SanitizeArrayIndexL( iObjectArray, *iObjectNameArray, aItem );
    User::LeaveIfError( aItem.GetNextInt( mediaId ));

    object->SetMediaId( mediaId );

    return KErrNone;
    }

// -----------------------------------------------------------------------------
// CMdETestScripter::UsageCountL
// -----------------------------------------------------------------------------
//
TInt CMdETestScripter::UsageCountL( CStifItemParser& aItem )
    {

    TInt count;

    CMdEObject* object = SanitizeArrayIndexL( iObjectArray, *iObjectNameArray, aItem );
    User::LeaveIfError( aItem.GetNextInt( count ));

    TL( object->UsageCount() == count );

    return KErrNone;
    }

// -----------------------------------------------------------------------------
// CMdETestScripter::GuidL
// -----------------------------------------------------------------------------
//
TInt CMdETestScripter::GuidL( CStifItemParser& aItem )
    {
    TInt mode;

    TInt guidHighExpected;
    TInt guidLowExpected;
    TInt64 guidHigh;
    TInt64 guidLow;

    User::LeaveIfError( aItem.GetNextInt( mode ));
    if ( mode == EObject )
        {
        CMdEObject* object = SanitizeArrayIndexL( iObjectArray, *iObjectNameArray, aItem );
        User::LeaveIfError( aItem.GetNextInt( guidHighExpected ));
        User::LeaveIfError( aItem.GetNextInt( guidLowExpected ));        
        object->Guid( guidHigh, guidLow );
        }
    else if ( mode == ERelation )
        {
        CMdERelation* relation = SanitizeArrayIndexL( iRelationArray, *iRelationNameArray, aItem );
        User::LeaveIfError( aItem.GetNextInt( guidHighExpected ));
        User::LeaveIfError( aItem.GetNextInt( guidLowExpected ));     
        relation->Guid( guidHigh, guidLow );
        }
          
    TL( guidHighExpected == guidHigh );
    TL( guidLowExpected == guidLow );

    return KErrNone;
    }

// -----------------------------------------------------------------------------
// CMdETestScripter::SetGuidL
// -----------------------------------------------------------------------------
//
TInt CMdETestScripter::SetGuidL( CStifItemParser& aItem )
    {
    TInt mode;
    
    TInt guidHigh;
    TInt guidLow;

    User::LeaveIfError( aItem.GetNextInt( mode ));
    if ( mode == EObject )
        {
        CMdEObject* object = SanitizeArrayIndexL( iObjectArray, *iObjectNameArray, aItem );
        User::LeaveIfError( aItem.GetNextInt( guidHigh ));
        User::LeaveIfError( aItem.GetNextInt( guidLow ));        
        object->SetGuid( guidHigh, guidLow );
        }
    else if ( mode == ERelation )
        {
        CMdERelation* relation = SanitizeArrayIndexL( iRelationArray, *iRelationNameArray, aItem );
        User::LeaveIfError( aItem.GetNextInt( guidHigh ));
        User::LeaveIfError( aItem.GetNextInt( guidLow ));     
        relation->SetGuid( guidHigh, guidLow );
        }
        
    return KErrNone;
    }

// -----------------------------------------------------------------------------
// CMdETestScripter::OpenForModifications
// -----------------------------------------------------------------------------
//
TInt CMdETestScripter::OpenForModificationsL( CStifItemParser& aItem )
    {
    TInt open;

    CMdEObject* object = SanitizeArrayIndexL( iObjectArray, *iObjectNameArray, aItem );
    User::LeaveIfError( aItem.GetNextInt( open ));

    TBool state = object->OpenForModifications();
    TL( (TBool)open == state );

    return KErrNone;
    }

// -----------------------------------------------------------------------------
// CMdETestScripter::NewObjectL
// -----------------------------------------------------------------------------
//
TInt CMdETestScripter::NewObjectL( CStifItemParser& aItem )
    {
    TInt mode;
    CMdEObject* object = NULL;

    TPtrC name;
    User::LeaveIfError( aItem.GetNextString( name ));    
    
    CMdEObjectDef* objectDef = SanitizeArrayIndexL( iObjectDefArray, *iObjectDefNameArray, aItem );
    User::LeaveIfError( aItem.GetNextInt( mode ));

    if ( mode == KL )
        {
        object = iMdeSession->NewObjectL(
            *objectDef, MdeConstants::Object::KAutomaticUri );
        }
    else if ( mode == KLC )
        {
        object = iMdeSession->NewObjectLC(
            *objectDef, MdeConstants::Object::KAutomaticUri );
        CleanupStack::Pop( object );
        }
    else
        {
        User::Leave( KErrArgument );
        }

    TL( object );
    iObjectArray.Append( object );
    AppendNameL( iObjectNameArray, name );    

    return KErrNone;
    }

// -----------------------------------------------------------------------------
// CMdETestScripter::AddObjectL
// -----------------------------------------------------------------------------
//
TInt CMdETestScripter::AddObjectL( CStifItemParser& aItem )
    {
    CMdEObject* object = SanitizeArrayIndexL( iObjectArray, *iObjectNameArray, aItem );

    TItemId id = iMdeSession->AddObjectL( *object );
    TL( id != KNoId );
    TL( object->Id() == id );

    return KErrNone;
    }

// -----------------------------------------------------------------------------
// CMdETestScripter::AddObjectsL
// -----------------------------------------------------------------------------
//
TInt CMdETestScripter::AddObjectsL( CStifItemParser& aItem )
    {
    TInt count;
    RPointerArray<CMdEObject> objectArray;

    TInt firstIndex = GetObjectIndexL( aItem );
    User::LeaveIfError( aItem.GetNextInt( count ));

    for ( TInt i = 0; i < count; ++i )
        {
        objectArray.Append( iObjectArray[ firstIndex + i ] );
        }

    TInt err = iMdeSession->AddObjectsL( objectArray );
    objectArray.Reset();
    T1L ( err, KErrNone );

    return KErrNone;
    }

// -----------------------------------------------------------------------------
// CMdETestScripter::GetObjectL
// -----------------------------------------------------------------------------
//
TInt CMdETestScripter::GetObjectL( CStifItemParser& aItem )
    {
    TPtrC name;
    User::LeaveIfError( aItem.GetNextString( name ));

    TInt mode1;
    TInt mode2;
    CMdEObject* newObject = NULL;
    CMdEObjectDef* objectDef = NULL;
    CMdENamespaceDef* namespaceDef = NULL;
    TInt64 guidHigh = 0;
    TInt64 guidLow = 0;

    CMdEObject* object = SanitizeArrayIndexL( iObjectArray, *iObjectNameArray, aItem );

    User::LeaveIfError( aItem.GetNextInt( mode1 ));
    User::LeaveIfError( aItem.GetNextInt( mode2 ));

    if ( mode2 == EObjectDef )
        {
        objectDef = SanitizeArrayIndexL( iObjectDefArray, *iObjectDefNameArray, aItem );
        }
        else if ( mode2 == ENamespaceDef )
        {
        namespaceDef = SanitizeArrayIndexL( iNamespaceDefArray, *iNamespaceDefNameArray, aItem );
        }

    if ( mode1 == KById && mode2 == EObjectDef )
        {
        newObject = iMdeSession->GetObjectL( object->Id(), *objectDef );
        }
    else if ( mode1 == KById && mode2 == ENamespaceDef )
        {
        newObject = iMdeSession->GetObjectL( object->Id(), namespaceDef );
        }
    else if ( mode1 == KByUri && mode2 == EObjectDef )
        {
        newObject = iMdeSession->GetObjectL( object->Uri(), *objectDef );
        }
    else if ( mode1 == KByUri && mode2 == ENamespaceDef )
        {
        newObject = iMdeSession->GetObjectL( object->Uri(), namespaceDef );
        }
    else if ( mode1 == KByGuid && mode2 == EObjectDef )
        {
        object->Guid( guidHigh, guidLow );
        newObject = iMdeSession->GetObjectL( guidHigh, guidLow, *objectDef );
        }
    else if ( mode1 == KByGuid && mode2 == ENamespaceDef )
        {
        object->Guid( guidHigh, guidLow );
        newObject = iMdeSession->GetObjectL( guidHigh, guidLow, namespaceDef );
        }
    else
        {
        User::Leave( KErrArgument );
        }

    TL( newObject );
    iObjectArray.Append( newObject );   
    AppendNameL( iObjectNameArray, name );

    iLog->Log( _L( "Object Id = %d URI = %S Propertycount = %d" ),
        newObject->Id(), &newObject->Uri(), newObject->PropertyCount() );

    return KErrNone;
    }

// -----------------------------------------------------------------------------
// CMdETestScripter::GetFullObjectL
// -----------------------------------------------------------------------------
//
TInt CMdETestScripter::GetFullObjectL( CStifItemParser& aItem )
    {
    TInt mode;
    CMdEObject* newObject = NULL;
    TPtrC name;
    User::LeaveIfError( aItem.GetNextString( name ));
    
    CMdEObjectDef* objectDef = SanitizeArrayIndexL( iObjectDefArray, *iObjectDefNameArray, aItem );
    CMdEObject* object = SanitizeArrayIndexL( iObjectArray, *iObjectNameArray, aItem );
    User::LeaveIfError( aItem.GetNextInt( mode ));
    if ( mode == KById )
        {
        newObject = iMdeSession->GetFullObjectL( object->Id());
       }
    else if ( mode == KByUri )
        {
        newObject = iMdeSession->GetFullObjectL( object->Uri());
        }
    else if ( mode == KByGuid )
        {
        TInt64 guidHigh;
        TInt64 guidLow;

        object->Guid( guidHigh, guidLow );
        newObject = iMdeSession->GetFullObjectL( guidHigh, guidLow );
        }
    else
        {
        User::Leave( KErrArgument );
        }
    TL( newObject );
    iObjectArray.Append( newObject );
    AppendNameL( iObjectNameArray, name );

    return KErrNone;
    }

// -----------------------------------------------------------------------------
// CMdETestScripter::CheckObjectL
// -----------------------------------------------------------------------------
//
TInt CMdETestScripter::CheckObjectL( CStifItemParser& aItem )
    {
    TInt objIndex;
    TInt mode;
    TMdEObject tObject;

    objIndex = GetObjectIndexL( aItem );
    User::LeaveIfError( aItem.GetNextInt( mode ));

    if ( mode == KById )
        {
        iMdeSession->CheckObjectL( tObject, iObjectArray[ objIndex ]->Id() );
        CheckTMdEObjectL( tObject, *iObjectArray[ objIndex ] );
        }
    else if ( mode == KByUri )
        {
        iMdeSession->CheckObjectL( tObject, iObjectArray[ objIndex ]->Uri() );
        CheckTMdEObjectL( tObject, *iObjectArray[ objIndex ] );
        }
    else if ( mode == KByIds )
        {
        TInt count;
        RArray<TItemId> ids;
        RArray<TMdEObject> tObjects;

        User::LeaveIfError( aItem.GetNextInt( count ));
        TL( iObjectArray.Count() > objIndex + count - 1 );

        for ( TInt i = 0; i < count; ++i )
            {
            ids.Append( iObjectArray[ objIndex + i ]->Id() );
            }
        iMdeSession->CheckObjectL( tObjects, ids );
        CheckTMdEObjectArrayL( tObjects, objIndex, count );

        tObjects.Reset();
        ids.Reset();
        }
    else
        {
        User::Leave( KErrArgument );
        }

    return KErrNone;
    }

// -----------------------------------------------------------------------------
// CMdETestScripter::RemoveObjectL
// -----------------------------------------------------------------------------
//
TInt CMdETestScripter::RemoveObjectL( CStifItemParser& aItem )
    {
    TInt mode;

    CMdEObject* object = SanitizeArrayIndexL( iObjectArray, *iObjectNameArray, aItem );
    User::LeaveIfError( aItem.GetNextInt( mode ));

    TItemId id = KNoId;

    if ( mode == KById )
        {
        id = iMdeSession->RemoveObjectL( object->Id() );
        }
    else if ( mode == KByUri )
        {
        id = iMdeSession->RemoveObjectL( object->Uri() );
        }
    else
        {
        User::Leave( KErrArgument );
        }

    TL( id != KNoId );

    return KErrNone;
    }

// -----------------------------------------------------------------------------
// CMdETestScripter::RemoveObjectsL
// -----------------------------------------------------------------------------
//
TInt CMdETestScripter::RemoveObjectsL( CStifItemParser& aItem )
    {
    TInt mode1;
    TInt mode2;
    TInt objIndex;
    TInt count;
    TInt needMacroVerifyResult;
    RArray<TItemId> objectIdArray;
    RPointerArray<TDesC> objectUriArray;
    RArray<TItemId> successful;
    TRequestStatus status = KRequestPending;

    User::LeaveIfError( aItem.GetNextInt( mode1 ));
    User::LeaveIfError( aItem.GetNextInt( mode2 ));
    objIndex = GetObjectIndexL( aItem );
    User::LeaveIfError( aItem.GetNextInt( count ));
    TL( iObjectArray.Count() > objIndex + count - 1 );
    TRAPD( err, aItem.GetNextInt( needMacroVerifyResult ) );
    if ( KErrNone != err )
        {
        needMacroVerifyResult = 1;
        }

    for ( TInt i = 0; i < count; ++i )
        {
        if ( mode2 == KById )
            {
            objectIdArray.Append( iObjectArray[ objIndex + i ]->Id() );
            }
        else if ( mode2 == KByUri )
            {
            objectUriArray.Append( &iObjectArray[ objIndex + i ]->Uri() );
            }
        }

    if ( mode1 == KSync && mode2 == KById )
        {
        iMdeSession->RemoveObjectsL( objectIdArray, successful );
        }
    else if ( mode1 == KSync && mode2 == KByUri )
        {
        iMdeSession->RemoveObjectsL( objectUriArray, successful );
        }
    else if ( mode1 == KAsync && mode2 == KById )
        {
        iItemCount = count;
        TEventIf event( TEventIf::ESetEvent, TName( KMdEObjectsAsyncRemoved ) );
        iAsyncHandler->Start( this, event );
        iMdEDataBuf.Close();// make sure the last async request buffer cleaned before using again.
        iMdeSession->RemoveObjectsAsyncL( objectIdArray, iAsyncHandler->iStatus, iMdEDataBuf );
        }
    else if ( mode1 == KAsync && mode2 == KByUri )
        {
        iItemCount = count;
        TEventIf event( TEventIf::ESetEvent, TName( KMdEObjectsAsyncRemoved ) );
        iAsyncHandler->Start( this, event );
        iMdEDataBuf.Close();// make sure the last async request buffer cleaned before using again.
        iMdeSession->RemoveObjectsAsyncL( objectUriArray, iAsyncHandler->iStatus, iMdEDataBuf );
        }
    else
        {
        User::Leave( KErrArgument );
        }

    if( needMacroVerifyResult && KSync == mode1 )
        {
        TL( successful.Count() == count );
        for ( TInt i = 0; i < successful.Count(); ++i )
            {
            TL( successful[ i ] != KNoId );
            }
        }

    successful.Reset();
    objectIdArray.Reset();
    
    return KErrNone;
    }

// -----------------------------------------------------------------------------
// CMdETestScripter::RemoveAllObjectsL
// -----------------------------------------------------------------------------
//
TInt CMdETestScripter::RemoveAllObjectsL( CStifItemParser& /* aItem */ )
    {
    TInt count = iObjectArray.Count();
    RArray<TItemId> successful;
    RArray<TItemId> objectIdArray;

    for ( TInt i = 0; i < count; ++i )
        {
        objectIdArray.Append( iObjectArray[ i ]->Id() );
        }

    if (count)
        {
        iMdeSession->RemoveObjectsL( objectIdArray, successful );
        }
    objectIdArray.Reset();
    successful.Reset();

    return KErrNone;
    }
// -----------------------------------------------------------------------------
// CMdETestScripter::OpenObjectL
// -----------------------------------------------------------------------------
//
TInt CMdETestScripter::OpenObjectL( CStifItemParser& aItem )
    {
    _LIT( KMsg1, "Enter OpenObjectL" );
    iLog->Log( KMsg1 );
    RDebug::Print( KMsg1 );
    
    TPtrC name;
    User::LeaveIfError( aItem.GetNextString( name ));      

    TInt mode1;
    TInt mode2;
    CMdEObject* newObject = NULL;
    CMdEObjectDef* objectDef = NULL;
    CMdENamespaceDef* namespaceDef = NULL;
    TInt64 guidHigh = 0;
    TInt64 guidLow = 0;

    CMdEObject* object = SanitizeArrayIndexL( iObjectArray, *iObjectNameArray, aItem );
    User::LeaveIfError( aItem.GetNextInt( mode1 ));
    User::LeaveIfError( aItem.GetNextInt( mode2 ));

    if ( mode2 == EObjectDef )
        {
        objectDef = SanitizeArrayIndexL( iObjectDefArray, *iObjectDefNameArray, aItem );
        }
        else if ( mode2 == ENamespaceDef )
        {
        namespaceDef = SanitizeArrayIndexL( iNamespaceDefArray, *iNamespaceDefNameArray, aItem );
        }

    if ( mode1 == KById && mode2 == EObjectDef )
        {
        newObject = iMdeSession->OpenObjectL( object->Id(), *objectDef );
        }
    else if ( mode1 == KById && mode2 == ENamespaceDef )
        {
        newObject = iMdeSession->OpenObjectL( object->Id(), namespaceDef );
        }
    else if ( mode1 == KByUri && mode2 == EObjectDef )
        {
        newObject = iMdeSession->OpenObjectL( object->Uri(), *objectDef );
        }
    else if ( mode1 == KByUri && mode2 == ENamespaceDef )
        {
        newObject = iMdeSession->OpenObjectL( object->Uri(), namespaceDef );
        }
    else if ( mode1 == KByGuid && mode2 == EObjectDef )
        {
        object->Guid( guidHigh, guidLow );
        newObject = iMdeSession->OpenObjectL( guidHigh, guidLow, *objectDef );
        }
    else if ( mode1 == KByGuid && mode2 == ENamespaceDef )
        {
        object->Guid( guidHigh, guidLow );
        newObject = iMdeSession->OpenObjectL( guidHigh, guidLow, namespaceDef );
        }
    else
        {
        User::Leave( KErrArgument );
        }
    TL( newObject );
    iObjectArray.Append( newObject );
    AppendNameL( iObjectNameArray, name );

    _LIT( KMsg2, "Exit OpenObjectL" );
    iLog->Log( KMsg2 );
    RDebug::Print( KMsg2 );
    
    return KErrNone;
    }

// -----------------------------------------------------------------------------
// CMdETestScripter::OpenFullObjectL
// -----------------------------------------------------------------------------
//
TInt CMdETestScripter::OpenFullObjectL( CStifItemParser& aItem )
    {
    TInt mode;
    TPtrC name;
    User::LeaveIfError( aItem.GetNextString( name ));  
    CMdEObject* newObject = NULL;

    CMdEObject* object = SanitizeArrayIndexL( iObjectArray, *iObjectNameArray, aItem );
    
    User::LeaveIfError( aItem.GetNextInt( mode ));

    if ( mode == KById )
        {
        newObject = iMdeSession->OpenFullObjectL( object->Id() );
        }
    else if ( mode == KByUri )
        {
        newObject = iMdeSession->OpenFullObjectL( object->Uri() );
        }
    else if ( mode == KByGuid )
        {
        TInt64 guidHigh;
        TInt64 guidLow;

        object->Guid( guidHigh, guidLow );
        newObject = iMdeSession->OpenFullObjectL( guidHigh, guidLow );
        }
    else
        {
        User::Leave( KErrArgument );
        }
    TL( newObject );
    if ( !newObject->OpenForModifications() )
        {
        return KErrLocked;
        }
    iObjectArray.Append( newObject );
    AppendNameL( iObjectNameArray, name );
    return KErrNone;
    }

// -----------------------------------------------------------------------------
// CMdETestScripter::CommitObjectL
// -----------------------------------------------------------------------------
//
TInt CMdETestScripter::CommitObjectL( CStifItemParser& aItem )
    {
    CMdEObject* object = SanitizeArrayIndexL( iObjectArray, *iObjectNameArray, aItem );
    iMdeSession->CommitObjectL( *object );

    return KErrNone;
    }

// -----------------------------------------------------------------------------
// CMdETestScripter::CommitObjectsL
// -----------------------------------------------------------------------------
//
TInt CMdETestScripter::CommitObjectsL( CStifItemParser& aItem )
    {
    TInt objIndex;
    TInt count;
    RPointerArray<CMdEObject> objectArray;

    objIndex = GetObjectIndexL( aItem );

    User::LeaveIfError( aItem.GetNextInt( count ));

    for ( TInt i = 0; i < count; ++i )
        {
        objectArray.Append( iObjectArray[ objIndex + i ] );
        }

    iMdeSession->CommitObjectsL( objectArray );
    
    objectArray.Reset();

    return KErrNone;
    }

// -----------------------------------------------------------------------------
// CMdETestScripter::CancelObjectL
// -----------------------------------------------------------------------------
//
TInt CMdETestScripter::CancelObjectL( CStifItemParser& aItem )
    {
    CMdEObject* object = SanitizeArrayIndexL( iObjectArray, *iObjectNameArray, aItem );

    iMdeSession->CancelObjectL( *object );

    return KErrNone;
    }

// -----------------------------------------------------------------------------
// CMdETestScripter::PropertyL
// -----------------------------------------------------------------------------
//
TInt CMdETestScripter::PropertyL( CStifItemParser& aItem )
    {
    TInt mode;
    TPtrC name;
    User::LeaveIfError( aItem.GetNextString( name ));  
    User::LeaveIfError( aItem.GetNextInt( mode ));

    const CMdEObject* object = SanitizeArrayIndexL( iObjectArray, *iObjectNameArray, aItem );    
    CMdEProperty* property;

    if ( mode == KByIndex )
        {
        TInt index;

        User::LeaveIfError( aItem.GetNextInt( index ));
        property = &object->PropertyL( index );
        }
    else if ( mode == KByDef )
        {
        CMdEPropertyDef* propertyDef = SanitizeArrayIndexL( iPropertyDefArray, *iPropertyDefNameArray, aItem );
        TInt startAt;
        User::LeaveIfError( aItem.GetNextInt( startAt ));        
        TL( object->Property( *propertyDef, property, startAt ) != KErrNotFound );
        }
    else
        {
        User::Leave( KErrArgument );
        }
    TL( property );

    iPropertyArray.Append ( property );
    AppendNameL( iPropertyNameArray, name );
    return KErrNone;
    }

// -----------------------------------------------------------------------------
// CMdETestScripter::PropertyCountL
// -----------------------------------------------------------------------------
//
TInt CMdETestScripter::PropertyCountL( CStifItemParser& aItem )
    {
    TInt mode;
    TInt count;
    TInt state = 0;

    User::LeaveIfError( aItem.GetNextInt( mode ));
    const CMdEObject* object = SanitizeArrayIndexL( iObjectArray, *iObjectNameArray, aItem );

    if ( mode == EObject )
        {
        state = object->PropertyCount();
        }
    else if ( mode == EPropertyDef )
        {
        CMdEPropertyDef* propertyDef = SanitizeArrayIndexL( iPropertyDefArray, *iPropertyDefNameArray, aItem );
        state = object->PropertyCount( *propertyDef );
        }
    else
        {
        User::Leave( KErrArgument );
        }

    User::LeaveIfError( aItem.GetNextInt( count ));
    TL( count == state );

    return KErrNone;
    }

// -----------------------------------------------------------------------------
// CMdETestScripter::AddBoolPropertyL
// -----------------------------------------------------------------------------
//
TInt CMdETestScripter::AddBoolPropertyL( CStifItemParser& aItem )
    {
    TInt value;

    CMdEObject* object = SanitizeArrayIndexL( iObjectArray, *iObjectNameArray, aItem );
    CMdEPropertyDef* propertyDef = SanitizeArrayIndexL( iPropertyDefArray, *iPropertyDefNameArray, aItem );
    User::LeaveIfError( aItem.GetNextInt( value ));

    CMdEProperty& prop = object->AddBoolPropertyL( *propertyDef, (TBool)value );
    TL( &prop );

    return KErrNone;
    }

// -----------------------------------------------------------------------------
// CMdETestScripter::AddInt8PropertyL
// -----------------------------------------------------------------------------
//
TInt CMdETestScripter::AddInt8PropertyL( CStifItemParser& aItem )
    {
    TInt value;
    CMdEObject* object = SanitizeArrayIndexL( iObjectArray, *iObjectNameArray, aItem );
    
    CMdEPropertyDef* propertyDef = SanitizeArrayIndexL( iPropertyDefArray, *iPropertyDefNameArray, aItem );

    User::LeaveIfError( aItem.GetNextInt( value ));

    CMdEProperty& prop = object->AddInt8PropertyL( *propertyDef, value );
    TL( &prop );

    return KErrNone;
    }

// -----------------------------------------------------------------------------
// CMdETestScripter::AddUint8PropertyL
// -----------------------------------------------------------------------------
//
TInt CMdETestScripter::AddUint8PropertyL( CStifItemParser& aItem )
    {
    TInt value;

    CMdEObject* object = SanitizeArrayIndexL( iObjectArray, *iObjectNameArray, aItem );
    CMdEPropertyDef* propertyDef = SanitizeArrayIndexL( iPropertyDefArray, *iPropertyDefNameArray, aItem );
    User::LeaveIfError( aItem.GetNextInt( value ));

    CMdEProperty& prop = object->AddUint8PropertyL( *propertyDef, value );
    TL( &prop );

    return KErrNone;
    }

// -----------------------------------------------------------------------------
// CMdETestScripter::AddInt16PropertyL
// -----------------------------------------------------------------------------
//
TInt CMdETestScripter::AddInt16PropertyL( CStifItemParser& aItem )
    {
    TInt value;

    CMdEObject* object = SanitizeArrayIndexL( iObjectArray, *iObjectNameArray, aItem );
    CMdEPropertyDef* propertyDef = SanitizeArrayIndexL( iPropertyDefArray, *iPropertyDefNameArray, aItem );
    User::LeaveIfError( aItem.GetNextInt( value ));

    CMdEProperty& prop = object->AddInt16PropertyL( *propertyDef, value );
    TL( &prop );

    return KErrNone;
    }

// -----------------------------------------------------------------------------
// CMdETestScripter::AddUint16PropertyL
// -----------------------------------------------------------------------------
//
TInt CMdETestScripter::AddUint16PropertyL( CStifItemParser& aItem )
    {
    TInt value;

    CMdEObject* object = SanitizeArrayIndexL( iObjectArray, *iObjectNameArray, aItem );
    CMdEPropertyDef* propertyDef = SanitizeArrayIndexL( iPropertyDefArray, *iPropertyDefNameArray, aItem );
    User::LeaveIfError( aItem.GetNextInt( value ));    
    
    CMdEProperty& prop = object->AddUint16PropertyL( *propertyDef, value );
    TL( &prop );

    return KErrNone;
    }

// -----------------------------------------------------------------------------
// CMdETestScripter::AddInt32PropertyL
// -----------------------------------------------------------------------------
//
TInt CMdETestScripter::AddInt32PropertyL( CStifItemParser& aItem )
    {
    TInt value;

    CMdEObject* object = SanitizeArrayIndexL( iObjectArray, *iObjectNameArray, aItem );
    CMdEPropertyDef* propertyDef = SanitizeArrayIndexL( iPropertyDefArray, *iPropertyDefNameArray, aItem );
    User::LeaveIfError( aItem.GetNextInt( value ));

    CMdEProperty& prop = object->AddInt32PropertyL( *propertyDef, value );
    TL( &prop );

    return KErrNone;
    }

// -----------------------------------------------------------------------------
// CMdETestScripter::AddUint32PropertyL
// -----------------------------------------------------------------------------
//
TInt CMdETestScripter::AddUint32PropertyL( CStifItemParser& aItem )
    {
    TInt value;
    CMdEObject* object = SanitizeArrayIndexL( iObjectArray, *iObjectNameArray, aItem );
    CMdEPropertyDef* propertyDef = SanitizeArrayIndexL( iPropertyDefArray, *iPropertyDefNameArray, aItem );
    User::LeaveIfError( aItem.GetNextInt( value ));

    CMdEProperty& prop = object->AddUint32PropertyL( *propertyDef, value );
    TL( &prop );

    return KErrNone;
    }

// -----------------------------------------------------------------------------
// CMdETestScripter::AddInt64PropertyL
// -----------------------------------------------------------------------------
//
TInt CMdETestScripter::AddInt64PropertyL( CStifItemParser& aItem )
    {
    TInt value;
    CMdEObject* object = SanitizeArrayIndexL( iObjectArray, *iObjectNameArray, aItem );
    CMdEPropertyDef* propertyDef = SanitizeArrayIndexL( iPropertyDefArray, *iPropertyDefNameArray, aItem );
    User::LeaveIfError( aItem.GetNextInt( value ));

    CMdEProperty& prop = object->AddInt64PropertyL( *propertyDef, value );
    TL( &prop );

    return KErrNone;
    }

// -----------------------------------------------------------------------------
// CMdETestScripter::AddReal32PropertyL
// -----------------------------------------------------------------------------
//
TInt CMdETestScripter::AddReal32PropertyL( CStifItemParser& aItem )
    {
    TInt value;

    CMdEObject* object = SanitizeArrayIndexL( iObjectArray, *iObjectNameArray, aItem );
    CMdEPropertyDef* propertyDef = SanitizeArrayIndexL( iPropertyDefArray, *iPropertyDefNameArray, aItem );
    User::LeaveIfError( aItem.GetNextInt( value ));

    CMdEProperty& prop = object->AddReal32PropertyL( *propertyDef, value );
    TL( &prop );

    return KErrNone;
    }

// -----------------------------------------------------------------------------
// CMdETestScripter::AddReal64PropertyL
// -----------------------------------------------------------------------------
//
TInt CMdETestScripter::AddReal64PropertyL( CStifItemParser& aItem )
    {
    TInt value;

    CMdEObject* object = SanitizeArrayIndexL( iObjectArray, *iObjectNameArray, aItem );
    CMdEPropertyDef* propertyDef = SanitizeArrayIndexL( iPropertyDefArray, *iPropertyDefNameArray, aItem );

    User::LeaveIfError( aItem.GetNextInt( value ));

    CMdEProperty& prop = object->AddReal64PropertyL( *propertyDef, value );
    TL( &prop );

    return KErrNone;
    }

// -----------------------------------------------------------------------------
// CMdETestScripter::AddTextPropertyL
// -----------------------------------------------------------------------------
//
TInt CMdETestScripter::AddTextPropertyL( CStifItemParser& aItem )
    {
    TPtrC value;

    CMdEObject* object = SanitizeArrayIndexL( iObjectArray, *iObjectNameArray, aItem );
    CMdEPropertyDef* propertyDef = SanitizeArrayIndexL( iPropertyDefArray, *iPropertyDefNameArray, aItem );
 
    aItem.SetParsingType( CStifItemParser::EQuoteStyleParsing );
    User::LeaveIfError( aItem.GetNextString( value ));

    CMdEProperty& prop = object->AddTextPropertyL( *propertyDef, value );
    TL( &prop );

    return KErrNone;
    }

// -----------------------------------------------------------------------------
// CMdETestScripter::AddTimePropertyL
// -----------------------------------------------------------------------------
//
TInt CMdETestScripter::AddTimePropertyL( CStifItemParser& aItem )
    {
    TPtrC value;

    CMdEObject* object = SanitizeArrayIndexL( iObjectArray, *iObjectNameArray, aItem );
    CMdEPropertyDef* propertyDef = SanitizeArrayIndexL( iPropertyDefArray, *iPropertyDefNameArray, aItem );  
    User::LeaveIfError( aItem.GetNextString( value ));

    TTime time;
    TL( time.Set( value ) == KErrNone );
    CMdEProperty& prop = object->AddTimePropertyL( *propertyDef, time );
    TL( &prop );

    return KErrNone;
    }

// -----------------------------------------------------------------------------
// CMdETestScripter::ObjectL
// -----------------------------------------------------------------------------
//
TInt CMdETestScripter::ObjectL( CStifItemParser& aItem )
    {
    CMdEProperty* property = SanitizeArrayIndexL( iPropertyArray, *iPropertyNameArray, aItem );
    CMdEObject* object = SanitizeArrayIndexL( iObjectArray, *iObjectNameArray, aItem );
    TL( object == &property->Object() );

    return KErrNone;
    }

// -----------------------------------------------------------------------------
// CMdETestScripter::BoolValueL
// -----------------------------------------------------------------------------
//
TInt CMdETestScripter::BoolValueL( CStifItemParser& aItem )
    {
    TInt value;

    CMdEProperty* property = SanitizeArrayIndexL( iPropertyArray, *iPropertyNameArray, aItem );  
    User::LeaveIfError( aItem.GetNextInt( value ));

    TL( property->BoolValueL() == value );

    return KErrNone;
    }
// -----------------------------------------------------------------------------
// CMdETestScripter::Int8ValueL
// -----------------------------------------------------------------------------
//
TInt CMdETestScripter::Int8ValueL( CStifItemParser& aItem )
    {
    TInt value;

    CMdEProperty* property = SanitizeArrayIndexL( iPropertyArray, *iPropertyNameArray, aItem );  
    User::LeaveIfError( aItem.GetNextInt( value ));
    TL( property->Int8ValueL() == value );

    return KErrNone;
    }

// -----------------------------------------------------------------------------
// CMdETestScripter::Uint8ValueL
// -----------------------------------------------------------------------------
//
TInt CMdETestScripter::Uint8ValueL( CStifItemParser& aItem )
    {
    TInt value;

    CMdEProperty* property = SanitizeArrayIndexL( iPropertyArray, *iPropertyNameArray, aItem );  
    User::LeaveIfError( aItem.GetNextInt( value ));
    TL( property->Uint8ValueL() == value );

    return KErrNone;
    }

// -----------------------------------------------------------------------------
// CMdETestScripter::Int16ValueL
// -----------------------------------------------------------------------------
//
TInt CMdETestScripter::Int16ValueL( CStifItemParser& aItem )
    {
    TInt value;

    CMdEProperty* property = SanitizeArrayIndexL( iPropertyArray, *iPropertyNameArray, aItem );  
    User::LeaveIfError( aItem.GetNextInt( value ));
    TL( property->Int16ValueL() == value );

    return KErrNone;
    }

// -----------------------------------------------------------------------------
// CMdETestScripter::Uint16ValueL
// -----------------------------------------------------------------------------
//
TInt CMdETestScripter::Uint16ValueL( CStifItemParser& aItem )
    {
    TInt value;
    CMdEProperty* property = SanitizeArrayIndexL( iPropertyArray, *iPropertyNameArray, aItem );  
    User::LeaveIfError( aItem.GetNextInt( value ));
    TL( property->Uint16ValueL() == value );

    return KErrNone;
    }

// -----------------------------------------------------------------------------
// CMdETestScripter::Int32ValueL
// -----------------------------------------------------------------------------
//
TInt CMdETestScripter::Int32ValueL( CStifItemParser& aItem )
    {
    TInt value;
    CMdEProperty* property = SanitizeArrayIndexL( iPropertyArray, *iPropertyNameArray, aItem );  
    User::LeaveIfError( aItem.GetNextInt( value ));
    TL( property->Int32ValueL() == value );

    return KErrNone;
    }

// -----------------------------------------------------------------------------
// CMdETestScripter::Uint32ValueL
// -----------------------------------------------------------------------------
//
TInt CMdETestScripter::Uint32ValueL( CStifItemParser& aItem )
    {
    TInt value;
    CMdEProperty* property = SanitizeArrayIndexL( iPropertyArray, *iPropertyNameArray, aItem );  
    User::LeaveIfError( aItem.GetNextInt( value ));
    TL( property->Uint32ValueL() == value );

    return KErrNone;
    }

// -----------------------------------------------------------------------------
// CMdETestScripter::Int64ValueL
// -----------------------------------------------------------------------------
//
TInt CMdETestScripter::Int64ValueL( CStifItemParser& aItem )
    {
    TInt value;
    CMdEProperty* property = SanitizeArrayIndexL( iPropertyArray, *iPropertyNameArray, aItem );  
    User::LeaveIfError( aItem.GetNextInt( value ));
    TL( property->Int64ValueL() == value );

    return KErrNone;
    }

// -----------------------------------------------------------------------------
// CMdETestScripter::Real32ValueL
// -----------------------------------------------------------------------------
//
TInt CMdETestScripter::Real32ValueL( CStifItemParser& aItem )
    {
    TInt value;
    CMdEProperty* property = SanitizeArrayIndexL( iPropertyArray, *iPropertyNameArray, aItem );  
    User::LeaveIfError( aItem.GetNextInt( value ));
    TL( property->Real32ValueL() == value );

    return KErrNone;
    }

// -----------------------------------------------------------------------------
// CMdETestScripter::Real64ValueL
// -----------------------------------------------------------------------------
//
TInt CMdETestScripter::Real64ValueL( CStifItemParser& aItem )
    {
    TInt value;
    CMdEProperty* property = SanitizeArrayIndexL( iPropertyArray, *iPropertyNameArray, aItem );  
    User::LeaveIfError( aItem.GetNextInt( value ));
    TL( property->Real64ValueL() == value );

    return KErrNone;
    }

// -----------------------------------------------------------------------------
// CMdETestScripter::TimeValueL
// -----------------------------------------------------------------------------
//
TInt CMdETestScripter::TimeValueL( CStifItemParser& aItem )
    {
    TPtrC value;
    CMdEProperty* property = SanitizeArrayIndexL( iPropertyArray, *iPropertyNameArray, aItem );  
    User::LeaveIfError( aItem.GetNextString( value ));

    TTime time;
    TL( time.Set( value ) == KErrNone );
    TL( property->TimeValueL() == time );

    return KErrNone;
    }

// -----------------------------------------------------------------------------
// CMdETestScripter::TextValueL
// -----------------------------------------------------------------------------
//
TInt CMdETestScripter::TextValueL( CStifItemParser& aItem )
    {
    TPtrC value;

    CMdEProperty* property = SanitizeArrayIndexL( iPropertyArray, *iPropertyNameArray, aItem );  
    aItem.SetParsingType( CStifItemParser::EQuoteStyleParsing );
    User::LeaveIfError( aItem.GetNextString( value ));

    TL( value.Compare( property->TextValueL()) == 0 );

    return KErrNone;
    }

// -----------------------------------------------------------------------------
// CMdETestScripter::SetBoolValueL
// -----------------------------------------------------------------------------
//
TInt CMdETestScripter::SetBoolValueL( CStifItemParser& aItem )
    {
    TInt value;
    CMdEProperty* property = SanitizeArrayIndexL( iPropertyArray, *iPropertyNameArray, aItem ); 
    User::LeaveIfError( aItem.GetNextInt( value ));

    property->SetBoolValueL( value );

    return KErrNone;
    }

// -----------------------------------------------------------------------------
// CMdETestScripter::SetInt8ValueL
// -----------------------------------------------------------------------------
//
TInt CMdETestScripter::SetInt8ValueL( CStifItemParser& aItem )
    {
    TInt value;
    CMdEProperty* property = SanitizeArrayIndexL( iPropertyArray, *iPropertyNameArray, aItem ); 
    User::LeaveIfError( aItem.GetNextInt( value ));

    property->SetInt8ValueL( value );

    return KErrNone;
    }

// -----------------------------------------------------------------------------
// CMdETestScripter::SetUint8ValueL
// -----------------------------------------------------------------------------
//
TInt CMdETestScripter::SetUint8ValueL( CStifItemParser& aItem )
    {
    TInt value;
    CMdEProperty* property = SanitizeArrayIndexL( iPropertyArray, *iPropertyNameArray, aItem ); 
    User::LeaveIfError( aItem.GetNextInt( value ));
    property->SetUint8ValueL( value );

    return KErrNone;
    }

// -----------------------------------------------------------------------------
// CMdETestScripter::SetInt16ValueL
// -----------------------------------------------------------------------------
//
TInt CMdETestScripter::SetInt16ValueL( CStifItemParser& aItem )
    {
    TInt value;
    CMdEProperty* property = SanitizeArrayIndexL( iPropertyArray, *iPropertyNameArray, aItem ); 
    User::LeaveIfError( aItem.GetNextInt( value ));
    property->SetInt16ValueL( value );

    return KErrNone;
    }

// -----------------------------------------------------------------------------
// CMdETestScripter::SetUint16ValueL
// -----------------------------------------------------------------------------
//
TInt CMdETestScripter::SetUint16ValueL( CStifItemParser& aItem )
    {
    TInt value;
    CMdEProperty* property = SanitizeArrayIndexL( iPropertyArray, *iPropertyNameArray, aItem );
    User::LeaveIfError( aItem.GetNextInt( value ));
    property->SetUint16ValueL( value );

    return KErrNone;
    }

// -----------------------------------------------------------------------------
// CMdETestScripter::SetInt32ValueL
// -----------------------------------------------------------------------------
//
TInt CMdETestScripter::SetInt32ValueL( CStifItemParser& aItem )
    {
    TInt value;
    CMdEProperty* property = SanitizeArrayIndexL( iPropertyArray, *iPropertyNameArray, aItem ); 
    User::LeaveIfError( aItem.GetNextInt( value ));
    property->SetInt32ValueL( value );

    return KErrNone;
    }

// -----------------------------------------------------------------------------
// CMdETestScripter::SetUint32ValueL
// -----------------------------------------------------------------------------
//
TInt CMdETestScripter::SetUint32ValueL( CStifItemParser& aItem )
    {
    TInt value;
    CMdEProperty* property = SanitizeArrayIndexL( iPropertyArray, *iPropertyNameArray, aItem ); 
    User::LeaveIfError( aItem.GetNextInt( value ));
    property->SetUint32ValueL( value );

    return KErrNone;
    }

// -----------------------------------------------------------------------------
// CMdETestScripter::SetInt64ValueL
// -----------------------------------------------------------------------------
//
TInt CMdETestScripter::SetInt64ValueL( CStifItemParser& aItem )
    {
    TInt value;
    CMdEProperty* property = SanitizeArrayIndexL( iPropertyArray, *iPropertyNameArray, aItem ); 
    User::LeaveIfError( aItem.GetNextInt( value ));

    property->SetInt64ValueL( value );

    return KErrNone;
    }

// -----------------------------------------------------------------------------
// CMdETestScripter::SetReal32ValueL
// -----------------------------------------------------------------------------
//
TInt CMdETestScripter::SetReal32ValueL( CStifItemParser& aItem )
    {
    TInt value;
    CMdEProperty* property = SanitizeArrayIndexL( iPropertyArray, *iPropertyNameArray, aItem );
    User::LeaveIfError( aItem.GetNextInt( value ));
    property->SetReal32ValueL( value );

    return KErrNone;
    }

// -----------------------------------------------------------------------------
// CMdETestScripter::SetReal64ValueL
// -----------------------------------------------------------------------------
//
TInt CMdETestScripter::SetReal64ValueL( CStifItemParser& aItem )
    {
    TInt value;
    CMdEProperty* property = SanitizeArrayIndexL( iPropertyArray, *iPropertyNameArray, aItem ); 
    User::LeaveIfError( aItem.GetNextInt( value ));

    property->SetReal64ValueL( value );

    return KErrNone;
    }

// -----------------------------------------------------------------------------
// CMdETestScripter::SetTimeValueL
// -----------------------------------------------------------------------------
//
TInt CMdETestScripter::SetTimeValueL( CStifItemParser& aItem )
    {
    TPtrC value;
    CMdEProperty* property = SanitizeArrayIndexL( iPropertyArray, *iPropertyNameArray, aItem );
    User::LeaveIfError( aItem.GetNextString( value ));
    TTime time;
    TL( time.Set( value ) == KErrNone );
    property->SetTimeValueL( time );
    return KErrNone;
    }

// -----------------------------------------------------------------------------
// CMdETestScripter::SetTextValueL
// -----------------------------------------------------------------------------
//
TInt CMdETestScripter::SetTextValueL( CStifItemParser& aItem )
    {
    TPtrC value;
    CMdEProperty* property = SanitizeArrayIndexL( iPropertyArray, *iPropertyNameArray, aItem );
    aItem.SetParsingType( CStifItemParser::EQuoteStyleParsing );
    User::LeaveIfError( aItem.GetNextString( value ));
    property->SetTextValueL( value );

    return KErrNone;
    }

// -----------------------------------------------------------------------------
// CMdETestScripter::RemovePropertyL
// -----------------------------------------------------------------------------
//
TInt CMdETestScripter::RemovePropertyL( CStifItemParser& aItem )
    {
    TPtrC propertyDefName;

    CMdEObject* object = SanitizeArrayIndexL( iObjectArray, *iObjectNameArray, aItem );
    User::LeaveIfError( aItem.GetNextString( propertyDefName ));

    CMdEPropertyDef& propertyDef = object->Def().GetPropertyDefL( propertyDefName );
    CMdEProperty* property = NULL;
    TInt propIndex = object->Property( propertyDef, property );
    TL( propIndex );
    object->RemoveProperty( propIndex );

    return KErrNone;
    }

// -----------------------------------------------------------------------------
// CMdETestScripter::FreeTextCountL
// -----------------------------------------------------------------------------
//
TInt CMdETestScripter::FreeTextCountL( CStifItemParser& aItem )
    {
    TInt count;

    CMdEObject* object = SanitizeArrayIndexL( iObjectArray, *iObjectNameArray, aItem );
    User::LeaveIfError( aItem.GetNextInt( count ));

    TInt state = object->FreeTextCount();
    TL( count == state );

    return KErrNone;
    }

// -----------------------------------------------------------------------------
// CMdETestScripter::FreeTextL
// -----------------------------------------------------------------------------
//
TInt CMdETestScripter::FreeTextL( CStifItemParser& aItem )
    {
    TInt index;
    TPtrC text;

    CMdEObject* object = SanitizeArrayIndexL( iObjectArray, *iObjectNameArray, aItem );
    User::LeaveIfError( aItem.GetNextInt( index ));
    User::LeaveIfError( aItem.GetNextString( text ));

    TPtrC state = object->FreeText( index );
    TL( text.Compare( state ) == 0 );

    return KErrNone;
    }

// -----------------------------------------------------------------------------
// CMdETestScripter::FreeTextIndexL
// -----------------------------------------------------------------------------
//
TInt CMdETestScripter::FreeTextIndexL( CStifItemParser& aItem )
    {
    TInt index;
    TPtrC value;

    CMdEObject* object = SanitizeArrayIndexL( iObjectArray, *iObjectNameArray, aItem );
    User::LeaveIfError( aItem.GetNextInt( index ));
    User::LeaveIfError( aItem.GetNextString( value ));

    TInt state = object->FreeTextIndex( value );
    TA1L( state, index );

    return KErrNone;
    }

// -----------------------------------------------------------------------------
// CMdETestScripter::AddFreeTextL
// -----------------------------------------------------------------------------
//
TInt CMdETestScripter::AddFreeTextL( CStifItemParser& aItem )
    {
    TPtrC value;

    CMdEObject* object = SanitizeArrayIndexL( iObjectArray, *iObjectNameArray, aItem );
    User::LeaveIfError( aItem.GetNextString( value ));

    object->AddFreeTextL( value );

    return KErrNone;
    }

// -----------------------------------------------------------------------------
// CMdETestScripter::RemoveFreeTextL
// -----------------------------------------------------------------------------
//
TInt CMdETestScripter::RemoveFreeTextL( CStifItemParser& aItem )
    {
    TInt index;

    CMdEObject* object = SanitizeArrayIndexL( iObjectArray, *iObjectNameArray, aItem );
    User::LeaveIfError( aItem.GetNextInt( index ));

    object->RemoveFreeText( index );

    return KErrNone;
    }

// -----------------------------------------------------------------------------
// CMdETestScripter::MovePropertiesL
// -----------------------------------------------------------------------------
//
TInt CMdETestScripter::MovePropertiesL( CStifItemParser& aItem )
    {
    CMdEObject* sourceObj = SanitizeArrayIndexL( iObjectArray, *iObjectNameArray, aItem );
    CMdEObject* targetObj = SanitizeArrayIndexL( iObjectArray, *iObjectNameArray, aItem );

    targetObj->MovePropertiesL( *sourceObj );

    return KErrNone;
    }

// -----------------------------------------------------------------------------
// CMdETestScripter::NewRelationL
// -----------------------------------------------------------------------------
//
TInt CMdETestScripter::NewRelationL( CStifItemParser& aItem )
    {
    TPtrC name;
    User::LeaveIfError( aItem.GetNextString( name ));  
    TPtrC relationString;
    TInt parameter;
    TInt mode;

    CMdEObject* sourceObj = SanitizeArrayIndexL( iObjectArray, *iObjectNameArray, aItem );
    CMdERelationDef* relationDef = SanitizeArrayIndexL( iRelationDefArray, *iRelationDefNameArray, aItem );
    CMdEObject* targetObj = SanitizeArrayIndexL( iObjectArray, *iObjectNameArray, aItem );
    User::LeaveIfError( aItem.GetNextInt( parameter ));
    User::LeaveIfError( aItem.GetNextInt( mode ));

    CMdERelation* relation = NULL;

    if ( mode == KL )
        {
        relation = iMdeSession->NewRelationL( *relationDef, sourceObj->Id(), targetObj->Id(), parameter );
        }
    else if ( mode == KLC )
        {
        relation = iMdeSession->NewRelationLC( *relationDef, sourceObj->Id(), targetObj->Id(), parameter );
        CleanupStack::Pop( relation );
        }
    else
        {
        User::Leave( KErrArgument );
        }

    TL( relation );
    iRelationArray.Append( relation );
    AppendNameL( iRelationNameArray, name );

    return KErrNone;
    }

// -----------------------------------------------------------------------------
// CMdETestScripter::AddRelationL
// -----------------------------------------------------------------------------
//
TInt CMdETestScripter::AddRelationL( CStifItemParser& aItem )
    {
    CMdERelation* relation = SanitizeArrayIndexL( iRelationArray, *iRelationNameArray, aItem );

    TItemId id = iMdeSession->AddRelationL( *relation );
    TL( id != KNoId );
    TL( relation->Id() == id );

    return KErrNone;
    }

// -----------------------------------------------------------------------------
// CMdETestScripter::LeftObjectIdL
// -----------------------------------------------------------------------------
//
TInt CMdETestScripter::LeftObjectIdL( CStifItemParser& aItem )
    {
    CMdERelation* relation = SanitizeArrayIndexL( iRelationArray, *iRelationNameArray, aItem );
    CMdEObject* object = SanitizeArrayIndexL( iObjectArray, *iObjectNameArray, aItem );

    TL( relation->LeftObjectId() == object->Id() );

    return KErrNone;
    }

// -----------------------------------------------------------------------------
// CMdETestScripter::RightObjectIdL
// -----------------------------------------------------------------------------
//
TInt CMdETestScripter::RightObjectIdL( CStifItemParser& aItem )
    {
    CMdERelation* relation = SanitizeArrayIndexL( iRelationArray, *iRelationNameArray, aItem );
    CMdEObject* object = SanitizeArrayIndexL( iObjectArray, *iObjectNameArray, aItem );

    TL( relation->RightObjectId() == object->Id() );

    return KErrNone;
    }

// -----------------------------------------------------------------------------
// CMdETestScripter::ParameterL
// -----------------------------------------------------------------------------
//
TInt CMdETestScripter::ParameterL( CStifItemParser& aItem )
    {
    TInt parameter;

    CMdERelation* relation = SanitizeArrayIndexL( iRelationArray, *iRelationNameArray, aItem );
    User::LeaveIfError( aItem.GetNextInt( parameter ));

    TL( relation->Parameter() == parameter );

    return KErrNone;
    }

// -----------------------------------------------------------------------------
// CMdETestScripter::SetParameterL
// -----------------------------------------------------------------------------
//
TInt CMdETestScripter::SetParameterL( CStifItemParser& aItem )
    {
    TInt parameter;

    CMdERelation* relation = SanitizeArrayIndexL( iRelationArray, *iRelationNameArray, aItem );
    User::LeaveIfError( aItem.GetNextInt( parameter ));

    relation->SetParameter( parameter );

    return KErrNone;
    }

// -----------------------------------------------------------------------------
// CMdETestScripter::SetLeftObjectIdL
// -----------------------------------------------------------------------------
//
TInt CMdETestScripter::SetLeftObjectIdL( CStifItemParser& aItem )
    {
    CMdERelation* relation = SanitizeArrayIndexL( iRelationArray, *iRelationNameArray, aItem );
    CMdEObject* object = SanitizeArrayIndexL( iObjectArray, *iObjectNameArray, aItem );

    relation->SetLeftObjectIdL( object->Id() );

    return KErrNone;
    }

// -----------------------------------------------------------------------------
// CMdETestScripter::SetRightObjectIdL
// -----------------------------------------------------------------------------
//
TInt CMdETestScripter::SetRightObjectIdL( CStifItemParser& aItem )
    {
    CMdERelation* relation = SanitizeArrayIndexL( iRelationArray, *iRelationNameArray, aItem );
    CMdEObject* object = SanitizeArrayIndexL( iObjectArray, *iObjectNameArray, aItem );

    relation->SetRightObjectIdL( object->Id() );

    return KErrNone;
    }

// -----------------------------------------------------------------------------
// CMdETestScripter::LastModifiedDateL
// -----------------------------------------------------------------------------
//
TInt CMdETestScripter::LastModifiedDateL( CStifItemParser& aItem )
    {
    TPtrC value;

    CMdERelation* relation = SanitizeArrayIndexL( iRelationArray, *iRelationNameArray, aItem );
    User::LeaveIfError( aItem.GetNextString( value ));

    TTime time;
    TL( time.Set( value ) == KErrNone );

    TL( relation->LastModifiedDate() == time );

    return KErrNone;
    }

// -----------------------------------------------------------------------------
// CMdETestScripter::SetLastModifiedDateL
// -----------------------------------------------------------------------------
//
TInt CMdETestScripter::SetLastModifiedDateL( CStifItemParser& aItem )
    {
    TPtrC value;

    CMdERelation* relation = SanitizeArrayIndexL( iRelationArray, *iRelationNameArray, aItem );
    User::LeaveIfError( aItem.GetNextString( value ));

    TTime time;
    TL( time.Set( value ) == KErrNone );

    relation->SetLastModifiedDate( time );

    return KErrNone;
    }

// -----------------------------------------------------------------------------
// CMdETestScripter::GetRelationL
// -----------------------------------------------------------------------------
//
TInt CMdETestScripter::GetRelationL( CStifItemParser& aItem )
    {
    TPtrC name;
    User::LeaveIfError( aItem.GetNextString( name ));      
    
    CMdERelation* relation = SanitizeArrayIndexL( iRelationArray, *iRelationNameArray, aItem );

    CMdERelation* newRelation = iMdeSession->GetRelationL( relation->Id());

    TL( newRelation );
    iRelationArray.Append( newRelation );
    AppendNameL( iRelationNameArray, name );

    return KErrNone;
    }

// -----------------------------------------------------------------------------
// CMdETestScripter::UpdateRelationL
// -----------------------------------------------------------------------------
//
TInt CMdETestScripter::UpdateRelationL( CStifItemParser& aItem )
    {
    CMdERelation* relation = SanitizeArrayIndexL( iRelationArray, *iRelationNameArray, aItem );

    TItemId id = iMdeSession->UpdateRelationL( *relation );
    TL( id != KNoId );

    return KErrNone;
    }

// -----------------------------------------------------------------------------
// CMdETestScripter::RemoveRelationL
// -----------------------------------------------------------------------------
//
TInt CMdETestScripter::RemoveRelationL( CStifItemParser& aItem )
    {
    CMdERelation* relation = SanitizeArrayIndexL( iRelationArray, *iRelationNameArray, aItem );

    TItemId id = iMdeSession->RemoveRelationL( relation->Id() );
    TL( id != KNoId );

    return KErrNone;
    }

// -----------------------------------------------------------------------------
// CMdETestScripter::RemoveRelationsL
// -----------------------------------------------------------------------------
//
TInt CMdETestScripter::RemoveRelationsL( CStifItemParser& aItem )
    {
    TInt mode;
    TInt relIndex;
    TInt count;
    RArray<TItemId> relationIdArray;

    User::LeaveIfError( aItem.GetNextInt( mode ));
    relIndex = GetRelationIndexL( aItem );

    User::LeaveIfError( aItem.GetNextInt( count ));
    TL( iRelationArray.Count() > relIndex + count - 1 );

    for ( TInt i = 0; i < count; ++i )
        {
        relationIdArray.Append( iRelationArray[ relIndex + i ]->Id() );
        }

    if ( mode == KSync )
        {
        RArray<TItemId> successful;

        iMdeSession->RemoveRelationsL( relationIdArray, successful );
        }
    else if ( mode == KAsync )
        {
        iItemCount = count;
        TEventIf event( TEventIf::ESetEvent, TName( KMdERelationsAsyncRemoved ) );
        iAsyncHandler->Start( this, event );
        iMdEDataBuf.Close();// make sure the last async request buffer cleaned before using again.
        iMdeSession->RemoveRelationsAsyncL( relationIdArray, iAsyncHandler->iStatus, iMdEDataBuf );
        }
    else
        {
        User::Leave( KErrArgument );
        }

    return KErrNone;
    }

// -----------------------------------------------------------------------------
// CMdETestScripter::NewEventL
// -----------------------------------------------------------------------------
//
TInt CMdETestScripter::NewEventL( CStifItemParser& aItem )
    {
    TPtrC name;
    User::LeaveIfError( aItem.GetNextString( name ));  

    TPtrC eventSource;
    TPtrC eventParticipant;
    TPtrC value;
    TInt mode;

    CMdEObject* object = SanitizeArrayIndexL( iObjectArray, *iObjectNameArray, aItem );
    CMdEEventDef* eventDef = SanitizeArrayIndexL( iEventDefArray, *iEventDefNameArray, aItem );
    
    User::LeaveIfError( aItem.GetNextString( eventSource ));
    User::LeaveIfError( aItem.GetNextString( eventParticipant ));
    User::LeaveIfError( aItem.GetNextString( value ));
    User::LeaveIfError( aItem.GetNextInt( mode ));

    TTime time;
    TL( time.Set( value ) == KErrNone );
    CMdEEvent* event = NULL;

    if ( mode == KL )
        {
        event = iMdeSession->NewEventL(
            *eventDef, object->Id(),
            time, &eventSource, &eventParticipant );
        }
    else if ( mode == KLC )
        {
        event = iMdeSession->NewEventLC(
            *eventDef, object->Id(),
            time, &eventSource, &eventParticipant );
        CleanupStack::Pop( event );
        }
    else
        {
        User::Leave( KErrArgument );
        }

    TL( event );
    iEventArray.Append( event );
    AppendNameL( iEventNameArray, name );

    return KErrNone;
    }

// -----------------------------------------------------------------------------
// CMdETestScripter::AddEventL
// -----------------------------------------------------------------------------
//
TInt CMdETestScripter::AddEventL( CStifItemParser& aItem )
    {
    CMdEEvent* event = SanitizeArrayIndexL( iEventArray, *iEventNameArray, aItem );

    TItemId id = iMdeSession->AddEventL( *event );
    TL( id != KNoId );
    TL( event->Id() == id );

    return KErrNone;
    }

// -----------------------------------------------------------------------------
// CMdETestScripter::ObjectIdL
// -----------------------------------------------------------------------------
//
TInt CMdETestScripter::ObjectIdL( CStifItemParser& aItem )
    {
    CMdEEvent* event = SanitizeArrayIndexL( iEventArray, *iEventNameArray, aItem );
    CMdEObject* object = SanitizeArrayIndexL( iObjectArray, *iObjectNameArray, aItem );

    TL( event->ObjectId() == object->Id() );

    return KErrNone;
    }

// -----------------------------------------------------------------------------
// CMdETestScripter::TimeL
// -----------------------------------------------------------------------------
//
TInt CMdETestScripter::TimeL( CStifItemParser& aItem )
    {
    TPtrC value;

    CMdEEvent* event = SanitizeArrayIndexL( iEventArray, *iEventNameArray, aItem );
    User::LeaveIfError( aItem.GetNextString( value ));

    TTime time;
    TL( time.Set( value ) == KErrNone );

    TL( event->Time() == time );

    return KErrNone;
    }

// -----------------------------------------------------------------------------
// CMdETestScripter::SourceL
// -----------------------------------------------------------------------------
//
TInt CMdETestScripter::SourceL( CStifItemParser& aItem )
    {
    TPtrC source;
    TInt expected;

    CMdEEvent* event = SanitizeArrayIndexL( iEventArray, *iEventNameArray, aItem );
    User::LeaveIfError( aItem.GetNextString( source ));
    User::LeaveIfError( aItem.GetNextInt( expected ));

    TInt result = event->Source()->Compare( source );
    TL( result == expected );

    return KErrNone;
    }

// -----------------------------------------------------------------------------
// CMdETestScripter::ParticipantL
// -----------------------------------------------------------------------------
//
TInt CMdETestScripter::ParticipantL( CStifItemParser& aItem )
    {
    TPtrC participant;
    TInt expected;

    CMdEEvent* event = SanitizeArrayIndexL( iEventArray, *iEventNameArray, aItem );
    User::LeaveIfError( aItem.GetNextString( participant ));
    User::LeaveIfError( aItem.GetNextInt( expected ));

    TInt result = event->Participant()->Compare( participant );
    TL( result == expected );

    return KErrNone;
    }

// -----------------------------------------------------------------------------
// CMdETestScripter::GetEventL
// -----------------------------------------------------------------------------
//
TInt CMdETestScripter::GetEventL( CStifItemParser& aItem )
    {
    TPtrC name;
    User::LeaveIfError( aItem.GetNextString( name ));    
    CMdEEvent* event = SanitizeArrayIndexL( iEventArray, *iEventNameArray, aItem );

    CMdEEvent* newEvent = iMdeSession->GetEventL( event->Id());

    TL( event );
    iEventArray.Append( newEvent );
    AppendNameL( iEventNameArray, name );
    return KErrNone;
    }

// -----------------------------------------------------------------------------
// CMdETestScripter::RemoveEventL
// -----------------------------------------------------------------------------
//
TInt CMdETestScripter::RemoveEventL( CStifItemParser& aItem )
    {
    CMdEEvent* event = SanitizeArrayIndexL( iEventArray, *iEventNameArray, aItem );
    TItemId id = iMdeSession->RemoveEventL( event->Id() );
    TL( id != KNoId );

    return KErrNone;
    }

// -----------------------------------------------------------------------------
// CMdETestScripter::RemoveEventsL
// -----------------------------------------------------------------------------
//
TInt CMdETestScripter::RemoveEventsL( CStifItemParser& aItem )
    {
    TInt mode;
    TInt eventIndex;
    TInt count;
    RArray<TItemId> successful;
    RArray<TItemId> eventIdArray;

    User::LeaveIfError( aItem.GetNextInt( mode ));
    eventIndex = GetEventIndexL( aItem );
    User::LeaveIfError( eventIndex );
    TL( iEventArray.Count() > eventIndex );
    User::LeaveIfError( aItem.GetNextInt( count ));
    TL( iEventArray.Count() > eventIndex + count - 1 );

    for ( TInt i = 0; i < count; ++i )
        {
        eventIdArray.Append( iEventArray[ eventIndex + i ]->Id() );
        }

    if ( mode == KSync )
        {
        iMdeSession->RemoveEventsL( eventIdArray, successful );
        }
    else if ( mode == KAsync )
        {
        iItemCount = count;
        TEventIf event( TEventIf::ESetEvent, TName( KMdEEventsAsyncRemoved ) );
        iAsyncHandler->Start( this, event );
        iMdEDataBuf.Close();// make sure the last async request buffer cleaned before using again.
        iMdeSession->RemoveEventsAsyncL( eventIdArray, iAsyncHandler->iStatus, iMdEDataBuf );
        }
    else
        {
        User::Leave( KErrArgument );
        }

    successful.Reset();
    eventIdArray.Reset();

    return KErrNone;
    }

// End of File
