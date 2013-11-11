/*
* Copyright (c) 2004 Nokia Corporation and/or its subsidiary(-ies).
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
* Description: Contains implementation of the CMetaDataFieldContainer
*              class.This class provides.
*
*/



// INCLUDE FILES
#include	<MetaDataFieldContainer.h>
#include	"MetaDataField.h"
#ifdef _DEBUG
#include	<e32svr.h>
#endif

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CMetaDataFieldContainer::CMetaDataFieldContainer
// C++ default constructor can NOT contain any code, that
// might leave.
// -----------------------------------------------------------------------------
//
CMetaDataFieldContainer::CMetaDataFieldContainer()
    {
    iD16 = NULL;
    }

// -----------------------------------------------------------------------------
// CMetaDataFieldContainer::ConstructL
// Symbian 2nd phase constructor can leave.
// -----------------------------------------------------------------------------
//
void CMetaDataFieldContainer::ConstructL()
    {
    }

// -----------------------------------------------------------------------------
// CMetaDataFieldContainer::NewLC
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
CMetaDataFieldContainer* CMetaDataFieldContainer::NewL()
    {
#ifdef _DEBUG
	RDebug::Print(_L("CMetaDataFieldContainer::NewL"));
#endif
	CMetaDataFieldContainer* self = new( ELeave ) CMetaDataFieldContainer;
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop();
    return self;
    }

// Destructor
CMetaDataFieldContainer::~CMetaDataFieldContainer()
	{
	if(i16BitAlbumArt)
		{
		delete iD16;
		}
	iFields.ResetAndDestroy();
	iFields.Close();
	}

// -----------------------------------------------------------------------------
// CMetaDataFieldContainer::Count
// -----------------------------------------------------------------------------
//
EXPORT_C TInt CMetaDataFieldContainer::Count() const
    {
    return iFields.Count();
    }

// -----------------------------------------------------------------------------
// CMetaDataFieldContainer::Field
// -----------------------------------------------------------------------------
//
EXPORT_C TPtrC CMetaDataFieldContainer::Field(
	TMetaDataFieldId  aFieldId ) const
    {
	TInt count( iFields.Count() );
	for ( TInt i = 0; i < count; i++ )
		{
		if ( iFields[i]->Id() == aFieldId )
			{
			if(aFieldId != EMetaDataJpeg)
			    {
		    	return iFields[i]->Data();			    
			    }
			else
			    {
		    	TPtrC8 pic = iFields[i]->Data8();
			    TInt length = pic.Length();
		    	if( length )
			        {
			        if( iD16 )
			    	    {
				       	delete iD16;
				       	iD16 = NULL;
			        	}
				    TRAPD(err, iD16 = HBufC::NewL(length));
			    	if (err == KErrNone)
				        {
			        	i16BitAlbumArt = ETrue;
			        	TPtr unicode( iD16->Des() );
			        	unicode.Copy( pic );
			        	return unicode;
				        }
		        	}
	        	}
	    	}
    	}
	// Searched field Id wasn't found
	return KNullDesC();
	}



// -----------------------------------------------------------------------------
// CMetaDataFieldContainer::Field8
// -----------------------------------------------------------------------------
//
EXPORT_C TPtrC8 CMetaDataFieldContainer::Field8(
	TMetaDataFieldId  aFieldId ) const
    {
    #ifdef _DEBUG
	RDebug::Print(_L("CMetaDataFieldContainer::Field8 %d"), aFieldId);
	#endif
	TInt count( iFields.Count() );
	// 8 bit field is only valid for EMetaDataJpeg
		for ( TInt i = 0; i < count; i++ )
        {
		if ( iFields[i]->Id() == aFieldId )
			{
			if(aFieldId == EMetaDataJpeg)
				return iFields[i]->Data8();
			else
				return KNullDesC8(); //Only EMetaDataJpeg has valid 8 bit data
			}
		}
	// Searched field Id wasn't found
	return KNullDesC8();
	}

// -----------------------------------------------------------------------------
// CMetaDataFieldContainer::AppendL
// -----------------------------------------------------------------------------
//
EXPORT_C void CMetaDataFieldContainer::AppendL(
	TMetaDataFieldId  aFieldId,
	const TDesC& aData )
    {
    CMetaDataField* item = CMetaDataField::NewLC( aFieldId, aData );
    User::LeaveIfError( iFields.Append( item ) );
    CleanupStack::Pop();
    }

// -----------------------------------------------------------------------------
// CMetaDataFieldContainer::AppendL for 8-bit data
// -----------------------------------------------------------------------------
//
EXPORT_C void CMetaDataFieldContainer::AppendL(
	TMetaDataFieldId  aFieldId,
	const TDesC8& aData )
    {
	CMetaDataField* item = CMetaDataField::NewLC( aFieldId, aData );
	User::LeaveIfError( iFields.Append( item ) );
	CleanupStack::Pop();
    }

// -----------------------------------------------------------------------------
// CMetaDataFieldContainer::At
// -----------------------------------------------------------------------------
//
EXPORT_C TPtrC CMetaDataFieldContainer::At(
	TInt aPosition,
	TMetaDataFieldId& aFieldId ) const
    {
	TInt count( iFields.Count() );
	if ( count == 0 )
		{
		return KNullDesC();
		}

	if ( aPosition < 0 || aPosition > count )
		{
		return KNullDesC();
		}

	aFieldId = iFields[aPosition]->Id();
	if(aFieldId == EMetaDataJpeg)
		{
		TPtrC8 pic = iFields[aPosition]->Data8();
		TInt length = pic.Length();
		if( length )
			{
			if(iD16)
			    {
			   	delete iD16;
			   	iD16 = NULL;
			    }
			TRAPD(err, iD16 = HBufC::NewL(length));
			if(err != KErrNone)
		    	{
				return KNullDesC();
		    	}
			i16BitAlbumArt = ETrue;
			TPtr unicode( iD16->Des() );
			unicode.Copy( pic );
			return unicode;
			}
		}
	return iFields[aPosition]->Data();
    }

// -----------------------------------------------------------------------------
// CMetaDataFieldContainer::FieldIdAt
// -----------------------------------------------------------------------------
//
EXPORT_C void CMetaDataFieldContainer::FieldIdAt(
	TInt aPosition,
	TMetaDataFieldId& aFieldId ) const
    {
    TInt count( iFields.Count());
    if (count == 0 )
        {
        aFieldId = EUnknownMetaDataField;
        }
    if ( aPosition < 0 || aPosition > count )
        {
        aFieldId = EUnknownMetaDataField;
        }
    else
        {
        aFieldId = iFields[aPosition]->Id();
        }
    }
//  End of File
