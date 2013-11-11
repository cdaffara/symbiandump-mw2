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
* Description: Contains implementation of the CMetaDataField class.
*              This class provides.
*
*/



// INCLUDE FILES
#include	"MetaDataField.h"
#ifdef _DEBUG
#include	<e32svr.h>
#endif

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CMetaDataField::CMetaDataField
// C++ default constructor can NOT contain any code, that
// might leave.
// -----------------------------------------------------------------------------
//
CMetaDataField::CMetaDataField()
    {
    }

// -----------------------------------------------------------------------------
// CMetaDataField::ConstructL
// Symbian 2nd phase constructor can leave.
// -----------------------------------------------------------------------------
//
void CMetaDataField::ConstructL(
	TMetaDataFieldId aId,
	const TDesC& aData )
    {
    iId = aId;
    iData = aData.AllocL();
    }

// -----------------------------------------------------------------------------
// CMetaDataField::ConstructL for 8 bit field data
// Symbian 2nd phase constructor can leave.
// -----------------------------------------------------------------------------
//
void CMetaDataField::ConstructL(
	TMetaDataFieldId aId,
	const TDesC8& aData )
    {
	#ifdef _DEBUG
		RDebug::Print(_L("CMetaDataField::ConstructL "));
	#endif
    iId = aId;
    iData8 = aData.AllocL();
    }

// -----------------------------------------------------------------------------
// CMetaDataField::NewLC
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
CMetaDataField* CMetaDataField::NewLC(
	TMetaDataFieldId aId,
	const TDesC& aData )
    {
	#ifdef _DEBUG
	RDebug::Print(_L("CMetaDataField::NewLC - [%d]"), aId);
	#endif
		CMetaDataField* self = new( ELeave ) CMetaDataField;
	    CleanupStack::PushL( self );
	    self->ConstructL( aId, aData );
	    return self;
    }

// -----------------------------------------------------------------------------
// CMetaDataField::NewLC
// Two-phased constructor for 8 bit data fields
// -----------------------------------------------------------------------------
//
CMetaDataField* CMetaDataField::NewLC(
	TMetaDataFieldId aId,
	const TDesC8& aData )
    {
	#ifdef _DEBUG
		RDebug::Print(_L("CMetaDataField::NewLC with 8bit"));
	#endif
		CMetaDataField* self = new( ELeave ) CMetaDataField;
    	CleanupStack::PushL( self );
    	self->ConstructL( aId, aData );
    	return self;
    }

// Destructor
CMetaDataField::~CMetaDataField()
	{
		delete iData;
		delete iData8;
	}

// -----------------------------------------------------------------------------
// CMetaDataField::Id
// -----------------------------------------------------------------------------
//
TMetaDataFieldId CMetaDataField::Id() const
    {
    return iId;
    }

// -----------------------------------------------------------------------------
// CMetaDataField::Data
// -----------------------------------------------------------------------------
//
TPtrC CMetaDataField::Data() const
    {
    if(iData)
    	{
    	return iData->Des();	
    	}
    return KNullDesC();
    }

// -----------------------------------------------------------------------------
// CMetaDataField::Data
// -----------------------------------------------------------------------------
//
TPtrC8 CMetaDataField::Data8() const
    {
    #ifdef _DEBUG	
		RDebug::Print(_L("CMetaDataField::Data8"));   
	#endif 	
    if (iData8)
    	{
    	return iData8->Des();
		}
	return KNullDesC8();
	}
//  End of File
