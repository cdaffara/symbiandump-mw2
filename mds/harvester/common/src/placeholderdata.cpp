/*
* Copyright (c) 2006-2009 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  Data transfer object for placeholder data
*
*/

#include <placeholderdata.h>

// CPlaceholderData implementation

//==========================================================================
// NewL
//==========================================================================
//
EXPORT_C CPlaceholderData* CPlaceholderData::NewL()
	{
	CPlaceholderData* self = new (ELeave) CPlaceholderData;
	CleanupStack::PushL( self );
	self->ConstructL();
	CleanupStack::Pop( self );

	return self;
	}

//==========================================================================
// ConstructL
//==========================================================================
//
void CPlaceholderData::ConstructL()
	{
	iUri = HBufC::New( 1 );
	iMediaId = 0;
	}

//==========================================================================
// Destructor
//==========================================================================
//
CPlaceholderData::~CPlaceholderData()
	{
		delete iUri;
		iUri = NULL;
	}

//==========================================================================
// Modified
//==========================================================================
//
EXPORT_C TTime CPlaceholderData::Modified()
	{
	return iModified;
	}

//==========================================================================
// SetModified
//==========================================================================
//
EXPORT_C void CPlaceholderData::SetModified( TTime aModified )
	{
	iModified = aModified;
	}

//==========================================================================
// Uri
//==========================================================================
//
EXPORT_C TPtr16 CPlaceholderData::Uri()
	{
	return iUri->Des();
	}

//==========================================================================
// SetUri
//==========================================================================
//
EXPORT_C void CPlaceholderData::SetUri( const TDesC& aUri )
	{
	delete iUri;
	iUri = NULL;
	iUri = aUri.Alloc();
	}

//==========================================================================
// FileSize
//==========================================================================
//
EXPORT_C TUint32 CPlaceholderData::FileSize()
	{
	return iFileSize;
	}

//==========================================================================
// SetFileSize
//==========================================================================
//
EXPORT_C void CPlaceholderData::SetFileSize( TUint32 aFileSize )
	{
	iFileSize = aFileSize;
	}

//==========================================================================
// MediaId
//==========================================================================
//
EXPORT_C TUint CPlaceholderData::MediaId()
	{
	return (TUint)iMediaId;
	}

//==========================================================================
// SetMediaId
//==========================================================================
//
EXPORT_C void CPlaceholderData::SetMediaId( TUint aMediaId )
	{
	iMediaId = aMediaId;
	}

//==========================================================================
// PresentState
//==========================================================================
//
EXPORT_C TInt CPlaceholderData::PresentState()
	{
	return iPresentState;
	}

//==========================================================================
// SetPresentState
//==========================================================================
//
EXPORT_C void CPlaceholderData::SetPresentState( TInt aState )
	{
	iPresentState = aState;
	}

//==========================================================================
// Preinstalled
//==========================================================================
//
EXPORT_C TInt CPlaceholderData::Preinstalled()
	{
	return iPreinstalled;
	}

//==========================================================================
// SetPreinstalled
//==========================================================================
//
EXPORT_C void CPlaceholderData::SetPreinstalled( TInt aValue )
	{
	iPreinstalled = aValue;
	}
