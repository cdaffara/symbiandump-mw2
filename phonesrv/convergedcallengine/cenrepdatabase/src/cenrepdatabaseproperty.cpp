/*
* Copyright (c) 2008-2008 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  central repository database property class
*
*/


#include "cenrepdatabaseproperty.h"

const TInt KMaxIntLen = 15;

// ======== MEMBER FUNCTIONS ========

// ---------------------------------------------------------------------------
// Constructor
// ---------------------------------------------------------------------------
//
CCenRepDatabaseProperty::CCenRepDatabaseProperty()
    {
    }


// ---------------------------------------------------------------------------
// Constructor
// ---------------------------------------------------------------------------
//
void CCenRepDatabaseProperty::ConstructL()
    {
    }


// ---------------------------------------------------------------------------
// Constructor
// ---------------------------------------------------------------------------
//
EXPORT_C CCenRepDatabaseProperty* CCenRepDatabaseProperty::NewL()
    {
    CCenRepDatabaseProperty* self = CCenRepDatabaseProperty::NewLC();
    CleanupStack::Pop( self );
    return self;
    }


// ---------------------------------------------------------------------------
// Constructor
// ---------------------------------------------------------------------------
//
EXPORT_C CCenRepDatabaseProperty* CCenRepDatabaseProperty::NewLC()
    {
    CCenRepDatabaseProperty* self = new( ELeave ) CCenRepDatabaseProperty();
    CleanupStack::PushL( self );
    self->ConstructL();
    return self;
    }


// ---------------------------------------------------------------------------
// Destructor
// ---------------------------------------------------------------------------
//
EXPORT_C CCenRepDatabaseProperty::~CCenRepDatabaseProperty()
    {
    iPropertyValue.Close();
    }


// ---------------------------------------------------------------------------
// Returns name of property
// ---------------------------------------------------------------------------
//
EXPORT_C TUint32 CCenRepDatabaseProperty::GetName() const
    {
    return iPropertyName;
    }


// ---------------------------------------------------------------------------
// Sets the name of property
// ---------------------------------------------------------------------------
//
EXPORT_C void CCenRepDatabaseProperty::SetName( TUint32 aPropertyName )
    {
    iPropertyName = aPropertyName;
    }


// ---------------------------------------------------------------------------
// Returns value of property
// ---------------------------------------------------------------------------
//
EXPORT_C TInt CCenRepDatabaseProperty::GetValue( TInt& aValue ) const
    {
    TLex convert( iPropertyValue );
    TInt err = convert.Val( aValue );
    return err;
    }


// ---------------------------------------------------------------------------
// Sets value of property.
// ---------------------------------------------------------------------------
//
EXPORT_C TInt CCenRepDatabaseProperty::SetValue( TInt aValue )
    {
    TBuf<KMaxIntLen> val;
    val.AppendNum( aValue );
    iPropertyValue.Close();
    TInt ret = iPropertyValue.Create( val );

    return ret;
    }


// ---------------------------------------------------------------------------
// Returns value of property
// ---------------------------------------------------------------------------
//
EXPORT_C const TDesC& CCenRepDatabaseProperty::GetDesValue() const
    {
    return iPropertyValue;
    }


// ---------------------------------------------------------------------------
// Sets value of property.
// ---------------------------------------------------------------------------
//
EXPORT_C TInt CCenRepDatabaseProperty::SetValue( const TDesC& aValue )
    {
    TInt ret( 0 );
    if ( aValue.Length() > KCenRepMaxDesLength )
        {
        ret = KErrOverflow;
        }
    else
        {
        iPropertyValue.Close();
        ret = iPropertyValue.Create( aValue );
        }

    return ret;
    }



// ---------------------------------------------------------------------------
// Compare operator
// ---------------------------------------------------------------------------
//
EXPORT_C TBool CCenRepDatabaseProperty::operator==( const CCenRepDatabaseProperty& aProperty ) const
    {
    TBool result( EFalse );

    result = ( iPropertyName == aProperty.GetName() &&
               0 == iPropertyValue.Compare( aProperty.GetDesValue() ) );

    return result;
    }


