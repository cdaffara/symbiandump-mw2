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
#include "MdETestScripter.h"

// -----------------------------------------------------------------------------
// CMdETestScripter::AddSchemaObserverL
// -----------------------------------------------------------------------------
//
TInt CMdETestScripter::AddSchemaObserverL( CStifItemParser& /* aItem */ )
    {
    iMdeSession->AddSchemaObserverL( *this );

    return KErrNone;
    }

// -----------------------------------------------------------------------------
// CMdETestScripter::RemoveSchemaObserverL
// -----------------------------------------------------------------------------
//
TInt CMdETestScripter::RemoveSchemaObserverL( CStifItemParser& /* aItem */ )
    {
    iMdeSession->RemoveSchemaObserverL( *this );

    return KErrNone;
    }

// -----------------------------------------------------------------------------
// CMdETestScripter::AddObjectObserverL
// -----------------------------------------------------------------------------
//
TInt CMdETestScripter::AddObjectObserverL( CStifItemParser& /* aItem */ )
    {
    iMdeSession->AddObjectObserverL( *this );

    return KErrNone;
    }

// -----------------------------------------------------------------------------
// CMdETestScripter::RemoveObjectObserverL
// -----------------------------------------------------------------------------
//
TInt CMdETestScripter::RemoveObjectObserverL( CStifItemParser& /* aItem */ )
    {
    iMdeSession->RemoveObjectObserverL( *this );

    return KErrNone;
    }

// -----------------------------------------------------------------------------
// CMdETestScripter::AddObjectObserverWithUriL
// -----------------------------------------------------------------------------
//
TInt CMdETestScripter::AddObjectObserverWithUriL( CStifItemParser& /* aItem */ )
    {
    iMdeSession->AddObjectObserverWithUriL( *this );

    return KErrNone;
    }

// -----------------------------------------------------------------------------
// CMdETestScripter::RemoveObjectObserverWithUriL
// -----------------------------------------------------------------------------
//
TInt CMdETestScripter::RemoveObjectObserverWithUriL( CStifItemParser& /* aItem */ )
    {
    iMdeSession->RemoveObjectObserverWithUriL( *this );

    return KErrNone;
    }

// -----------------------------------------------------------------------------
// CMdETestScripter::AddObjectPresentObserverL
// -----------------------------------------------------------------------------
//
TInt CMdETestScripter::AddObjectPresentObserverL( CStifItemParser& /* aItem */ )
    {
    iMdeSession->AddObjectPresentObserverL( *this );

    return KErrNone;
    }

// -----------------------------------------------------------------------------
// CMdETestScripter::RemoveObjectPresentObserverL
// -----------------------------------------------------------------------------
//
TInt CMdETestScripter::RemoveObjectPresentObserverL( CStifItemParser& /* aItem */ )
    {
    iMdeSession->RemoveObjectPresentObserverL( *this );

    return KErrNone;
    }

// -----------------------------------------------------------------------------
// CMdETestScripter::AddRelationObserverL
// -----------------------------------------------------------------------------
//
TInt CMdETestScripter::AddRelationObserverL( CStifItemParser& /* aItem */ )
    {
    iMdeSession->AddRelationObserverL( *this );

    return KErrNone;
    }

// -----------------------------------------------------------------------------
// CMdETestScripter::RemoveRelationObserverL
// -----------------------------------------------------------------------------
//
TInt CMdETestScripter::RemoveRelationObserverL( CStifItemParser& /* aItem */ )
    {
    iMdeSession->RemoveRelationObserverL( *this );

    return KErrNone;
    }

// -----------------------------------------------------------------------------
// CMdETestScripter::AddRelationItemObserverL
// -----------------------------------------------------------------------------
//
TInt CMdETestScripter::AddRelationItemObserverL( CStifItemParser& /* aItem */ )
    {
    iMdeSession->AddRelationItemObserverL( *this );

    return KErrNone;
    }

// -----------------------------------------------------------------------------
// CMdETestScripter::RemoveRelationItemObserverL
// -----------------------------------------------------------------------------
//
TInt CMdETestScripter::RemoveRelationItemObserverL( CStifItemParser& /* aItem */ )
    {
    iMdeSession->RemoveRelationItemObserverL( *this );

    return KErrNone;
    }

// -----------------------------------------------------------------------------
// CMdETestScripter::AddRelationPresentObserverL
// -----------------------------------------------------------------------------
//
TInt CMdETestScripter::AddRelationPresentObserverL( CStifItemParser& /* aItem */ )
    {
    iMdeSession->AddRelationPresentObserverL( *this );

    return KErrNone;
    }

// -----------------------------------------------------------------------------
// CMdETestScripter::RemoveRelationPresentObserverL
// -----------------------------------------------------------------------------
//
TInt CMdETestScripter::RemoveRelationPresentObserverL( CStifItemParser& /* aItem */ )
    {
    iMdeSession->RemoveRelationPresentObserverL( *this );

    return KErrNone;
    }

// -----------------------------------------------------------------------------
// CMdETestScripter::AddEventObserverL
// -----------------------------------------------------------------------------
//
TInt CMdETestScripter::AddEventObserverL( CStifItemParser& /* aItem */ )
    {
    iMdeSession->AddEventObserverL( *this );

    return KErrNone;
    }

// -----------------------------------------------------------------------------
// CMdETestScripter::RemoveEventObserverL
// -----------------------------------------------------------------------------
//
TInt CMdETestScripter::RemoveEventObserverL( CStifItemParser& /* aItem */ )
    {
    iMdeSession->RemoveEventObserverL( *this );

    return KErrNone;
    }

// -----------------------------------------------------------------------------
// CMdETestScripter::VerifyTItemIdsL
// -----------------------------------------------------------------------------
//
TInt CMdETestScripter::VerifyTItemIdsL( CStifItemParser& aItem )
    {
    TInt mode;
    TInt index;
    TInt count;

    User::LeaveIfError( aItem.GetNextInt( mode ));
    User::LeaveIfError( aItem.GetNextInt( index ));
    User::LeaveIfError( aItem.GetNextInt( count ));

    if ( mode == EObject )
        {
        CheckTItemIdArrayL( iNotificationItemIdArray, iObjectArray, index, count );
        }
    else if ( mode == EEvent )
        {
        CheckTItemIdArrayL( iNotificationItemIdArray, iEventArray, index, count );
        }
    else if ( mode == ERelation )
        {
        CheckTItemIdArrayL( iNotificationItemIdArray, iRelationArray, index, count );
        }
    else
        {
        User::Leave( KErrArgument );
        }

    iNotificationItemIdArray.Reset();

    return KErrNone;
    }

// -----------------------------------------------------------------------------
// CMdETestScripter::VerifyMdERelationsL
// -----------------------------------------------------------------------------
//
TInt CMdETestScripter::VerifyMdERelationsL( CStifItemParser& aItem )
    {
    TInt index;
    TInt count;

    User::LeaveIfError( aItem.GetNextInt( index ));
    User::LeaveIfError( aItem.GetNextInt( count ));

    CheckTMdERelationArrayL( iNotificationRelationArray, index, count );
    iNotificationRelationArray.Reset();

    return KErrNone;
    }

// -----------------------------------------------------------------------------
// CMdETestScripter::VerifyObserverNotificationTypeL
// -----------------------------------------------------------------------------
//
TInt CMdETestScripter::VerifyObserverNotificationTypeL( CStifItemParser& aItem )
    {
    TInt type;

    User::LeaveIfError( aItem.GetNextInt( type ));

    TL( iNotificationType == type );

    return KErrNone;
    }

// -----------------------------------------------------------------------------
// CMdETestScripter::ParentL
// -----------------------------------------------------------------------------
//
TInt CMdETestScripter::ParentL( CStifItemParser& aItem )
    {
    CMdEObjectDef* objectDef = SanitizeArrayIndexL( iObjectDefArray, *iObjectDefNameArray, aItem );
    CMdEObjectDef* parentDef = SanitizeArrayIndexL( iObjectDefArray, *iObjectDefNameArray, aItem );
    TL( objectDef->ParentL() == parentDef );

    return KErrNone;
    }

// -----------------------------------------------------------------------------
// CMdETestScripter::NameL
// -----------------------------------------------------------------------------
//
TInt CMdETestScripter::NameL( CStifItemParser& aItem )
    {
    TInt mode;
    TPtrC name;

    User::LeaveIfError( aItem.GetNextInt( mode ));
    if ( mode == EPropertyDef )
        {
        CMdEPropertyDef* propDef = SanitizeArrayIndexL( iPropertyDefArray, *iPropertyDefNameArray, aItem );
        aItem.SetParsingType( CStifItemParser::EQuoteStyleParsing );
        User::LeaveIfError( aItem.GetNextString( name ));
        TL( name.Compare( propDef->Name()) == 0 );           
        }
    else if ( mode == ENamespaceDef )
        {
        CMdENamespaceDef* spaceDef = SanitizeArrayIndexL( iNamespaceDefArray, *iNamespaceDefNameArray, aItem );
        aItem.SetParsingType( CStifItemParser::EQuoteStyleParsing );
        User::LeaveIfError( aItem.GetNextString( name ));
        TL( name.Compare( spaceDef->Name()) == 0 );
        }
    else
        {
        User::Leave( KErrArgument );
        }

    return KErrNone;
    }

// -----------------------------------------------------------------------------
// CMdETestScripter::ObjectDefCountL
// -----------------------------------------------------------------------------
//
TInt CMdETestScripter::ObjectDefCountL( CStifItemParser& aItem )
    {
    TInt count;

    CMdENamespaceDef* space = SanitizeArrayIndexL( iNamespaceDefArray, *iNamespaceDefNameArray, aItem );
    User::LeaveIfError( aItem.GetNextInt( count ));

    TL( space->ObjectDefCount() == count );

    return KErrNone;
    }

// -----------------------------------------------------------------------------
// CMdETestScripter::ObjectDefL
// -----------------------------------------------------------------------------
//
TInt CMdETestScripter::ObjectDefL( CStifItemParser& aItem )
    {
    TInt mode;
    TPtrC name;
    TInt index;
    CMdEObjectDef* objectDef = NULL;

    User::LeaveIfError( aItem.GetNextInt( mode ));
    User::LeaveIfError( aItem.GetNextString( name ));
    if ( mode == ENamespaceDef )
        {
        CMdENamespaceDef* space = SanitizeArrayIndexL( iNamespaceDefArray, *iNamespaceDefNameArray, aItem );
        User::LeaveIfError( aItem.GetNextInt( index ));
        objectDef = &space->ObjectDefL( index );
        }
    else if ( mode == EPropertyDef )
        {
        CMdEPropertyDef* propDef = SanitizeArrayIndexL( iPropertyDefArray, *iPropertyDefNameArray, aItem );
        objectDef = &propDef->ObjectDef();
        }
    else
        {
        User::Leave( KErrArgument );
        }

    TL( objectDef );
    iObjectDefArray.Append( objectDef );
    AppendNameL( iObjectDefNameArray, name );

    return KErrNone;
    }

// -----------------------------------------------------------------------------
// CMdETestScripter::GetObjectDefL
// -----------------------------------------------------------------------------
//
TInt CMdETestScripter::GetObjectDefL( CStifItemParser& aItem )
    {
    TPtrC name;
    User::LeaveIfError( aItem.GetNextString( name ));    
    
    TPtrC objectString;
    CMdENamespaceDef* space = SanitizeArrayIndexL( iNamespaceDefArray, *iNamespaceDefNameArray, aItem );
    User::LeaveIfError( aItem.GetNextString( objectString ));
    CMdEObjectDef& objectDef = space->GetObjectDefL( objectString );
    TL( &objectDef );
    iObjectDefArray.Append( &objectDef );
    AppendNameL( iObjectDefNameArray, name );

    return KErrNone;
    }

// -----------------------------------------------------------------------------
// CMdETestScripter::RelationDefCountL
// -----------------------------------------------------------------------------
//
TInt CMdETestScripter::RelationDefCountL( CStifItemParser& aItem )
    {
    TInt count;

    CMdENamespaceDef* space = SanitizeArrayIndexL( iNamespaceDefArray, *iNamespaceDefNameArray, aItem );
    User::LeaveIfError( aItem.GetNextInt( count ));

    TL( space->RelationDefCount() == count );

    return KErrNone;
    }

// -----------------------------------------------------------------------------
// CMdETestScripter::RelationDefL
// -----------------------------------------------------------------------------
//
TInt CMdETestScripter::RelationDefL( CStifItemParser& aItem )
    {

    TPtrC name;
    User::LeaveIfError( aItem.GetNextString( name ));  
    TInt index;

    CMdENamespaceDef* space = SanitizeArrayIndexL( iNamespaceDefArray, *iNamespaceDefNameArray, aItem );
    User::LeaveIfError( aItem.GetNextInt( index ));
    
    CMdERelationDef& relationDef = space->RelationDefL( index );
    TL( &relationDef );
    iRelationDefArray.Append( &relationDef );
    AppendNameL( iRelationDefNameArray, name );
    return KErrNone;
    }

// -----------------------------------------------------------------------------
// CMdETestScripter::GetRelationDefL
// -----------------------------------------------------------------------------
//
TInt CMdETestScripter::GetRelationDefL( CStifItemParser& aItem )
    {
    TPtrC name;
    User::LeaveIfError( aItem.GetNextString( name ));      
    TPtrC relationString;

    CMdENamespaceDef* space = SanitizeArrayIndexL( iNamespaceDefArray, *iNamespaceDefNameArray, aItem );
    User::LeaveIfError( aItem.GetNextString( relationString ));

    CMdERelationDef& relationDef = space->GetRelationDefL( relationString );
    TL( &relationDef );
    iRelationDefArray.Append( &relationDef );
    AppendNameL( iRelationDefNameArray, name );
    return KErrNone;
    }

// -----------------------------------------------------------------------------
// CMdETestScripter::EventDefCountL
// -----------------------------------------------------------------------------
//
TInt CMdETestScripter::EventDefCountL( CStifItemParser& aItem )
    {
    TInt count;

    CMdENamespaceDef* space = SanitizeArrayIndexL( iNamespaceDefArray, *iNamespaceDefNameArray, aItem );
    User::LeaveIfError( aItem.GetNextInt( count ));

    TL( space->EventDefCount() == count );

    return KErrNone;
    }

// -----------------------------------------------------------------------------
// CMdETestScripter::EventDefL
// -----------------------------------------------------------------------------
//
TInt CMdETestScripter::EventDefL( CStifItemParser& aItem )
    {
    
    TPtrC name;
    User::LeaveIfError( aItem.GetNextString( name ));     
    TInt index;

    CMdENamespaceDef* space = SanitizeArrayIndexL( iNamespaceDefArray, *iNamespaceDefNameArray, aItem );
    User::LeaveIfError( aItem.GetNextInt( index ));

    CMdEEventDef& eventDef = space->EventDefL( index );
    TL( &eventDef );
    iEventDefArray.Append( &eventDef );
    AppendNameL( iEventDefNameArray, name );
    return KErrNone;
    }

// -----------------------------------------------------------------------------
// CMdETestScripter::GetEventDefL
// -----------------------------------------------------------------------------
//
TInt CMdETestScripter::GetEventDefL( CStifItemParser& aItem )
    {
    TPtrC name;
    User::LeaveIfError( aItem.GetNextString( name ));   
    TPtrC eventString;

    CMdENamespaceDef* space = SanitizeArrayIndexL( iNamespaceDefArray, *iNamespaceDefNameArray, aItem );
    User::LeaveIfError( aItem.GetNextString( eventString ));
    CMdEEventDef& eventDef = space->GetEventDefL( eventString );
    TL( &eventDef );
    iEventDefArray.Append( &eventDef );
    AppendNameL( iEventDefNameArray, name );
    return KErrNone;
    }

// -----------------------------------------------------------------------------
// CMdETestScripter::ExportMetadataL
// -----------------------------------------------------------------------------
//
TInt CMdETestScripter::ExportMetadataL( CStifItemParser& aItem )
    {
    TPtrC path;
    TInt mode;
    TInt useObjectDefs;
    TInt useRelationDefs;
    TInt useEventDefs;

    const RPointerArray<CMdEObjectDef>* objectDefs;
    const RPointerArray<CMdERelationDef>* relationDefs;
    const RPointerArray<CMdEEventDef>* eventDefs;

    CMdENamespaceDef* space = SanitizeArrayIndexL( iNamespaceDefArray, *iNamespaceDefNameArray, aItem );
    User::LeaveIfError( aItem.GetNextString( path ));
    User::LeaveIfError( aItem.GetNextInt( mode ));
    User::LeaveIfError( aItem.GetNextInt( useObjectDefs ));
    User::LeaveIfError( aItem.GetNextInt( useRelationDefs ));
    User::LeaveIfError( aItem.GetNextInt( useEventDefs ));

    objectDefs = (useObjectDefs) ? &iObjectDefArray : NULL;
    relationDefs = (useRelationDefs) ? &iRelationDefArray : NULL;
    eventDefs = (useEventDefs) ? &iEventDefArray : NULL;

    TFileName filePath( iFilePath );
    filePath.Append( path );
    filePath.ZeroTerminate();

    if ( mode == KAsync )
        {
        TRequestStatus status = KRequestPending;
        RMdEDataBuffer buf;

        CleanupClosePushL( buf );
        iMdeSession->ExportMetadataL( filePath, status, buf, space,
            objectDefs, relationDefs, eventDefs );
        User::WaitForRequest( status );

        CleanupStack::PopAndDestroy( &buf );
        }
    else if ( mode == KSync )
        {
        iMdeSession->ExportMetadataL( filePath, space,
            objectDefs, relationDefs, eventDefs );
        }
    else
        {
        User::Leave( KErrArgument );
        }
    iLog->Log( _L( "ExportMetadataL End"));

    return KErrNone;
    }

// -----------------------------------------------------------------------------
// CMdETestScripter::ImportMetadataL
// -----------------------------------------------------------------------------
//
TInt CMdETestScripter::ImportMetadataL( CStifItemParser& aItem )
    {            
    TPtrC path;
    TInt mode;
    TInt ret = KErrNone;

    User::LeaveIfError( aItem.GetNextString( path ));
    User::LeaveIfError( aItem.GetNextInt( mode ));

    TFileName filePath( iFilePath );
    filePath.Append( path );
    filePath.ZeroTerminate();

    if ( mode == KAsync )
        {
        TRequestStatus status = KRequestPending;
        TPckgBuf<TInt> buf( ret );

        iMdeSession->ImportMetadata( filePath, buf, status );
        User::WaitForRequest( status );
        ret = buf();
        }
    else if ( mode == KSync )
        {
        ret = iMdeSession->ImportMetadataL( filePath );
        }
    else
        {
        User::Leave( KErrArgument );
        }
       iLog->Log( _L( "ImportMetadataL ret: %d" ), ret );
    TL ( ret >= 0 );

    return KErrNone;
    }

// -----------------------------------------------------------------------------
// CMdETestScripter::ImportSchemaL
// -----------------------------------------------------------------------------
//
TInt CMdETestScripter::ImportSchemaL( CStifItemParser& aItem )
    {
    TPtrC path;
    User::LeaveIfError( aItem.GetNextString( path ));

    TFileName filePath( iFilePath );
    filePath.Append( path );
    filePath.ZeroTerminate();

    iMdeSession->ImportSchemaL( filePath );

    return KErrNone;
    }

// -----------------------------------------------------------------------------
// CMdETestScripter::NamespaceDefCountL
// -----------------------------------------------------------------------------
//
TInt CMdETestScripter::NamespaceDefCountL( CStifItemParser& aItem )
    {
    TInt count;

    User::LeaveIfError( aItem.GetNextInt( count ));

    TInt state = iMdeSession->NamespaceDefCount();
    TL( count == state );

    return KErrNone;
    }

// -----------------------------------------------------------------------------
// CMdETestScripter::NamespaceDefL
// -----------------------------------------------------------------------------
//
TInt CMdETestScripter::NamespaceDefL( CStifItemParser& aItem )
    {
    TPtrC name;
    User::LeaveIfError( aItem.GetNextString( name ));
        
    TInt mode;
    User::LeaveIfError( aItem.GetNextInt( mode ));

    CMdENamespaceDef* namespaceDef = NULL;
    if ( mode == ESession )
        {
        TInt index;

        User::LeaveIfError( aItem.GetNextInt( index ));
        namespaceDef = &iMdeSession->NamespaceDefL( index );
        }
    if ( mode == EEventDef )
        {
        CMdEEventDef* eventDef = SanitizeArrayIndexL( iEventDefArray, *iEventDefNameArray, aItem );
        namespaceDef = &eventDef->NamespaceDef();
        }
    else if ( mode == EObjectDef )
        {
        CMdEObjectDef* objectDef = SanitizeArrayIndexL( iObjectDefArray, *iObjectDefNameArray, aItem );
        namespaceDef = &objectDef->NamespaceDef();
        }
    else if ( mode == EPropertyDef )
        {
        CMdEPropertyDef* propertyDef = SanitizeArrayIndexL( iPropertyDefArray, *iPropertyDefNameArray, aItem );        
        namespaceDef = &propertyDef->NamespaceDef();
        }
    else if ( mode == ERelationDef )
        {
        CMdERelationDef* relationDef = SanitizeArrayIndexL( iRelationDefArray, *iRelationDefNameArray, aItem );
        namespaceDef = &relationDef->NamespaceDef();
        }

    iNamespaceDefArray.Append( namespaceDef );
    AppendNameL( iNamespaceDefNameArray, name );

    return KErrNone;
    }

// -----------------------------------------------------------------------------
// CMdETestScripter::GetNamespaceDefL
// -----------------------------------------------------------------------------
//
TInt CMdETestScripter::GetNamespaceDefL( CStifItemParser& aItem )
    {
    TPtrC name;
    User::LeaveIfError( aItem.GetNextString( name ));    

    aItem.SetParsingType( CStifItemParser::EQuoteStyleParsing );
    TPtrC nsName;
    User::LeaveIfError( aItem.GetNextString( nsName ));

    CMdENamespaceDef& namespaceDef = iMdeSession->GetNamespaceDefL( nsName );
    iNamespaceDefArray.Append( &namespaceDef );
    AppendNameL( iNamespaceDefNameArray, name );
    return KErrNone;
    }

// -----------------------------------------------------------------------------
// CMdETestScripter::GetDefaultNamespaceDefL
// -----------------------------------------------------------------------------
//
TInt CMdETestScripter::GetDefaultNamespaceDefL( CStifItemParser& aItem )
    {
    TPtrC name;
    User::LeaveIfError( aItem.GetNextString( name ));    
        
    CMdENamespaceDef& namespaceDef = iMdeSession->GetDefaultNamespaceDefL();
    iNamespaceDefArray.Append( &namespaceDef );
    AppendNameL( iNamespaceDefNameArray, name );
    return KErrNone;
    }

// -----------------------------------------------------------------------------
// CMdETestScripter::LoadSchemaL
// -----------------------------------------------------------------------------
//
TInt CMdETestScripter::LoadSchemaL( CStifItemParser& /* aItem */ )
    {
    iMdeSession->LoadSchemaL();

    return KErrNone;
    }

// -----------------------------------------------------------------------------
// CMdETestScripter::EngineSessionL
// -----------------------------------------------------------------------------
//
TInt CMdETestScripter::EngineSessionL( CStifItemParser& /* aItem */ )
    {
    RMdEEngineSession& session = iMdeSession->EngineSession();
    TL( &session );

    return KErrNone;
    }

// -----------------------------------------------------------------------------
// CMdETestScripter::GetSchemaVersionL
// -----------------------------------------------------------------------------
//
TInt CMdETestScripter::GetSchemaVersionL( CStifItemParser& aItem )
    {
    TInt majorVersion;
    TInt minorVersion;
    TInt expectedMajorVersion;
    TInt expectedMinorVersion;

    User::LeaveIfError( aItem.GetNextInt( expectedMajorVersion ));
    User::LeaveIfError( aItem.GetNextInt( expectedMinorVersion ));

    iMdeSession->GetSchemaVersionL( majorVersion, minorVersion );
    TL( majorVersion == expectedMajorVersion );
    TL( minorVersion == expectedMinorVersion );

    return KErrNone;
    }

// -----------------------------------------------------------------------------
// CMdETestScripter::SetObjectToPresentByGuidL
// -----------------------------------------------------------------------------
//
TInt CMdETestScripter::SetObjectToPresentByGuidL( CStifItemParser& aItem )
    {
    TInt guidHigh;
    TInt guidLow;

    User::LeaveIfError( aItem.GetNextInt( guidHigh ));
    User::LeaveIfError( aItem.GetNextInt( guidLow ));

    iMdeSession->SetObjectToPresentByGuidL( guidHigh, guidLow );

    return KErrNone;
    }

// -----------------------------------------------------------------------------
// CMdETestScripter::ClearNamespaceDefArray
// -----------------------------------------------------------------------------
TInt CMdETestScripter::ResetNamespaceDefArray( CStifItemParser& /*aItem*/ )
    {
    iNamespaceDefArray.Reset();
    iNamespaceDefNameArray->Reset();
    return KErrNone;
    }

// End of File
