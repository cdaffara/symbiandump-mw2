/*
* Copyright (c)  Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  Service Provider Settings entry. 
*
*/



#include "spentry.h"
#include "spproperty.h"
#include "spdefaultvalues.h"


// ======== MEMBER FUNCTIONS ========

// ---------------------------------------------------------------------------
// Constructor
// ---------------------------------------------------------------------------
//
CSPEntry::CSPEntry()
    {
    }


// ---------------------------------------------------------------------------
// Constructor
// ---------------------------------------------------------------------------
//
void CSPEntry::ConstructL()
    {
    }


// ---------------------------------------------------------------------------
// Constructor
// ---------------------------------------------------------------------------
//
EXPORT_C CSPEntry* CSPEntry::NewL()
    {
    CSPEntry* self = CSPEntry::NewLC();
    CleanupStack::Pop( self );
    return self;
    }


// ---------------------------------------------------------------------------
// Constructor
// ---------------------------------------------------------------------------
//
EXPORT_C CSPEntry* CSPEntry::NewLC()
    {
    CSPEntry* self = new( ELeave ) CSPEntry;
    CleanupStack::PushL( self );
    self->ConstructL();
    return self;
    }


// ---------------------------------------------------------------------------
// Destructor
// ---------------------------------------------------------------------------
//
EXPORT_C CSPEntry::~CSPEntry()
    {
    iServiceName.Close();
    iPropertyArray.ResetAndDestroy();
    iPropertyArray.Close();
    }


// ---------------------------------------------------------------------------
// Returns service ID of this entry
// ---------------------------------------------------------------------------
//
EXPORT_C TServiceId CSPEntry::GetServiceId() const
    {
    return iServiceId;
    }


// ---------------------------------------------------------------------------
// Set service ID for this entry
// ---------------------------------------------------------------------------
//
void CSPEntry::SetServiceId( TServiceId aServiceId )
    {
    iServiceId = aServiceId;
    }


// ---------------------------------------------------------------------------
// Returns service name of this entry
// ---------------------------------------------------------------------------
//
EXPORT_C const TDesC& CSPEntry::GetServiceName() const
    {
    return iServiceName;
    }


// ---------------------------------------------------------------------------
// Sets service name for this settings entry.
// ---------------------------------------------------------------------------
//
EXPORT_C TInt CSPEntry::SetServiceName( const TDesC& aServiceName )
    {
    TInt err( KErrNone );
    TInt length( aServiceName.Length() );

    if ( 0 >= length ||  KSPMaxDesLength < length )
        {
        err = KErrArgument;
        }
    else
        {
        iServiceName.Close();
        err = iServiceName.Create( aServiceName );
        }

    return err;
    }


// ---------------------------------------------------------------------------
// Adds new property for this entry. Ownerhip transfers.
// ---------------------------------------------------------------------------
//
EXPORT_C TInt CSPEntry::AddPropertyL( const CSPProperty& aProperty )
    {
    TInt err( 0 );
    
    TInt index = SearchProperty( aProperty.GetName() );

    if ( KErrNotFound == index )
        {
        CSPProperty* property = CSPProperty::NewLC();
        property->CopyL( aProperty );
        User::LeaveIfError( iPropertyArray.Append( property ));
        CleanupStack::Pop( property );
        }
    else
        {
        err = KErrAlreadyExists;
        }

    return err;
    }


// ---------------------------------------------------------------------------
// Update the property value
// ---------------------------------------------------------------------------
//
EXPORT_C TInt CSPEntry::UpdateProperty( TServicePropertyName aPropertyName, TInt aValue )
    {
    TInt err( 0 );

    TInt index = SearchProperty( aPropertyName );

    if ( KErrNotFound < index )
        {
        CSPProperty* update = iPropertyArray[index];
        err = update->SetValue( aValue );
        }
    else
        {
        err = KErrNotFound;
        }

    return err;
    }


// ---------------------------------------------------------------------------
// Update the property value
// ---------------------------------------------------------------------------
//
EXPORT_C TInt CSPEntry::UpdateProperty( TServicePropertyName aPropertyName, const TDesC& aValue )
    {
    TInt err( 0 );

    TInt index = SearchProperty( aPropertyName );

    if ( KErrNotFound < index )
        {
        CSPProperty* update = iPropertyArray[index];
        err = update->SetValue( aValue );
        }
    else
        {
        err = KErrNotFound;
        }

    return err;
    }

// ---------------------------------------------------------------------------
// Update the property value
// ---------------------------------------------------------------------------
//
EXPORT_C TInt CSPEntry::UpdateProperty( TServicePropertyName aPropertyName, TOnOff aValue )
    {
    TInt err( 0 );

    TInt index = SearchProperty( aPropertyName );

    if ( KErrNotFound < index )
        {
        CSPProperty* update = iPropertyArray[index];
        err = update->SetValue( aValue );
        }
    else
        {
        err = KErrNotFound;
        }

    return err;
    }

// ---------------------------------------------------------------------------
// Returns property of this entry by name.
// ---------------------------------------------------------------------------
//
EXPORT_C TInt CSPEntry::GetProperty( const CSPProperty*& aProperty, 
                                    TServicePropertyName aPropertyName ) const
    {
    TInt index = SearchProperty( aPropertyName );

    if ( KErrNotFound != index )
        {
        aProperty = iPropertyArray[index];
        return KErrNone;
        }
    else
        {
        return KErrNotFound;
        }
    }


// ---------------------------------------------------------------------------
// Returns property of this entry by index.
// ---------------------------------------------------------------------------
//
EXPORT_C TInt CSPEntry::GetProperty( const CSPProperty*& aProperty, 
                                    TInt aIndex ) const
    {
    if ( iPropertyArray.Count() > aIndex && 0 <= aIndex )
        {
        aProperty = iPropertyArray[aIndex];
        return KErrNone;
        }
    else
        {
        return KErrArgument;
        }
    }


// ---------------------------------------------------------------------------
// Deletes property from this entry by index.
// ---------------------------------------------------------------------------
//
EXPORT_C TInt CSPEntry::DeleteProperty( TInt aIndex )
    {
    if ( iPropertyArray.Count() > aIndex && 0 <= aIndex )
        {
        delete iPropertyArray[aIndex];
        iPropertyArray[aIndex] = NULL;
        iPropertyArray.Remove( aIndex );
        return KErrNone;
        }
    else
        {
        return KErrArgument;
        }
    }


// ---------------------------------------------------------------------------
// Deletes property from this entry by name.
// ---------------------------------------------------------------------------
//
EXPORT_C TInt CSPEntry::DeleteProperty( TServicePropertyName aProperty )
    {
    TInt index = SearchProperty( aProperty );

    return DeleteProperty( index );
    }


// ---------------------------------------------------------------------------
// Returns reference to proporty array
// ---------------------------------------------------------------------------
//
EXPORT_C const RPropertyArray& CSPEntry::GetAllProperties() const
    {
    return iPropertyArray;    
    }


// ---------------------------------------------------------------------------
// Returns count of properties in this entry
// ---------------------------------------------------------------------------
//
EXPORT_C TInt CSPEntry::PropertyCount() const
    {
    return iPropertyArray.Count();
    }


// ---------------------------------------------------------------------------
// Search index of requested property in this entry
// ---------------------------------------------------------------------------
//
TInt CSPEntry::SearchProperty( TServicePropertyName aProperty ) const
    {
    TInt count( iPropertyArray.Count() );
    TInt index( KErrNotFound );

    TBool found( EFalse );
    for ( TInt i( 0 ); i < count && !found; i++ )
        {
        if ( aProperty == iPropertyArray[i]->GetName() )
            {
            index = i;
            found = ETrue;
            }
        }

    return index;
    }

// ---------------------------------------------------------------------------
// Reset the service entry, remove all properties
// ---------------------------------------------------------------------------
//
EXPORT_C void CSPEntry::Reset()
    {
    iServiceId = KSPNoId;
    iServiceName.Close();
    iPropertyArray.ResetAndDestroy();
    }

