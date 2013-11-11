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
* Description:   XDM Engine settings property
*
*/



#include "XdmSettingsApi.h"
#include "XdmSettingsProperty.h"

// ---------------------------------------------------------
// CXdmSettingsProperty::SetPropCollectionL
//
// ---------------------------------------------------------
//
CXdmSettingsProperty::CXdmSettingsProperty( TXdmSettingsProperty aPropertyName ) :
                                            iPropertyName( aPropertyName )
    {
    }

// ---------------------------------------------------------
// CXdmSettingsProperty::ConstructL
//
// ---------------------------------------------------------
//   
void CXdmSettingsProperty::ConstructL( const TDesC& aPropertyValue )
    {
    iPropertyValue = HBufC::NewL( aPropertyValue.Length() );
    iPropertyValue->Des().Copy( aPropertyValue );
    }

// ---------------------------------------------------------
// CXdmSettingsProperty::SetPropCollectionL
//
// ---------------------------------------------------------
//
CXdmSettingsProperty::~CXdmSettingsProperty()
    {
    delete iPropertyValue;
    }
        
// ---------------------------------------------------------
// CXdmSettingsProperty::SetPropCollectionL
//
// ---------------------------------------------------------
//
CXdmSettingsProperty* CXdmSettingsProperty::NewL( const TDesC& aPropertyValue,
                                                  TXdmSettingsProperty aPropertyName )
    {
    CXdmSettingsProperty* self = new ( ELeave ) CXdmSettingsProperty( aPropertyName );
    CleanupStack::PushL( self );
    self->ConstructL( aPropertyValue );
    CleanupStack::Pop();
    return self;
    }

// ---------------------------------------------------------
// CXdmSettingsProperty::SetPropertyL
//
// ---------------------------------------------------------
//   
EXPORT_C TXdmSettingsProperty CXdmSettingsProperty::PropertyName() const
    {
    return iPropertyName;
    }

// ---------------------------------------------------------
// CXdmSettingsProperty::SetPropertyL
//
// ---------------------------------------------------------
//   
EXPORT_C TPtrC CXdmSettingsProperty::PropertyValue() const
    {
    return iPropertyValue != NULL ? iPropertyValue->Des() : TPtrC();
    }
    


            
// End of File

