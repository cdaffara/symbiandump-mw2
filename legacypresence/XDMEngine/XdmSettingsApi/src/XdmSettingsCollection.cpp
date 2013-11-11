/*
* Copyright (c) 2005 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:   XDM Engine settings collection
*
*/



#include "XdmSettingsApi.h"
#include "XdmSettingsProperty.h"
#include "XdmSettingsCollection.h"

// ---------------------------------------------------------
// CXdmSettingsCollection::SetPropCollectionL
//
// ---------------------------------------------------------
//
EXPORT_C CXdmSettingsCollection::CXdmSettingsCollection()
    {
    }

// ---------------------------------------------------------
// CXdmSettingsCollection::~CXdmSettingsCollection
//
// ---------------------------------------------------------
//
EXPORT_C CXdmSettingsCollection::~CXdmSettingsCollection()
    {
    iPropCollection.ResetAndDestroy();
    }
        
// ---------------------------------------------------------
// CXdmSettingsCollection::Property
//
// ---------------------------------------------------------
//
EXPORT_C TPtrC CXdmSettingsCollection::Property( TXdmSettingsProperty aProperty ) const
    {
    CXdmSettingsProperty* property = Find( aProperty );
    return property ? property->PropertyValue(): TPtrC();
    }

// ---------------------------------------------------------
// CXdmSettingsCollection::Property
//
// ---------------------------------------------------------
//
EXPORT_C const CXdmSettingsProperty& CXdmSettingsCollection::Property( TInt aIndex ) const
    {
    __ASSERT_ALWAYS( aIndex >= 0 && aIndex < iPropCollection.Count(),
                     User::Panic( _L( "CXdmSettingsCollection" ), 0 ) );
    return *iPropCollection[aIndex];
    }
           
// ---------------------------------------------------------
// CXdmSettingsCollection::Count
//
// ---------------------------------------------------------
//
EXPORT_C TInt CXdmSettingsCollection::Count() const
    {
    return iPropCollection.Count();
    }
    
// ---------------------------------------------------------
// CXdmSettingsCollection::AppendL
//
// ---------------------------------------------------------
//
EXPORT_C void CXdmSettingsCollection::AppendL( const TDesC& aPropertyValue,
                                               TXdmSettingsProperty aPropertyName )
    {
    CXdmSettingsProperty* prop = CXdmSettingsProperty::NewL( aPropertyValue, aPropertyName );
    CleanupStack::PushL( prop );
    User::LeaveIfError( iPropCollection.Append( prop ) );
    CleanupStack::Pop();  //property
    }

// ---------------------------------------------------------
// CXdmSettingsCollection::RemoveL
//
// ---------------------------------------------------------
//
EXPORT_C void CXdmSettingsCollection::RemoveL( TXdmSettingsProperty aPropertyName )
    {
    CXdmSettingsProperty* property = Find( aPropertyName );
    if( property )
        {
        TInt index = iPropCollection.Find( property );
        iPropCollection.Remove( index );
        delete property;
        property = NULL;
        }   
    }

// ---------------------------------------------------------
// CXdmSettingsCollection::Find
//
// ---------------------------------------------------------
//
CXdmSettingsProperty* CXdmSettingsCollection::Find( TXdmSettingsProperty aProperty ) const
    {
    TBool found = EFalse;
    CXdmSettingsProperty* property = NULL;
    const TInt count = iPropCollection.Count();
    for( TInt i = 0;!found && i < count;i++ )
        {
        if( iPropCollection[i]->PropertyName() == aProperty )
            {
            found = ETrue;
            property = iPropCollection[i];
            }
        }
    return property;
    }
            
// End of File
