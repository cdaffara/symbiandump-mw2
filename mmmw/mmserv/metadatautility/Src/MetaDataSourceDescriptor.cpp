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
* Description:  This class implements the interface specified in CMetaDataSource
*				 when audio source is from a descriptor.
*
*/



// INCLUDE FILES
#include	"MetaDataSourceDescriptor.h"
#ifdef _DEBUG
#include	<e32svr.h>
#endif

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CMetaDataSourceDescriptor::CMetaDataSourceDescriptor
// C++ default constructor can NOT contain any code, that
// might leave.
// -----------------------------------------------------------------------------
//
CMetaDataSourceDescriptor::CMetaDataSourceDescriptor()
    {
    }

// -----------------------------------------------------------------------------
// CMetaDataSourceDescriptor::ConstructL
// Symbian 2nd phase constructor can leave.
// -----------------------------------------------------------------------------
//
void CMetaDataSourceDescriptor::ConstructL(
	const TDesC8& aDes )
    {
	iSource.Set(aDes);
	iLength = iSource.Length();
#ifdef _DEBUG
	RDebug::Print(_L("CMetaDataSourceDescriptor::ConstructL - Done"));
#endif
	}

// -----------------------------------------------------------------------------
// CMetaDataSourceDescriptor::NewL
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
CMetaDataSourceDescriptor* CMetaDataSourceDescriptor::NewL(
	const TDesC8& aDes )
    {
#ifdef _DEBUG
	RDebug::Print(_L("CMetaDataSourceDescriptor::NewL"));
#endif
	CMetaDataSourceDescriptor* self = new( ELeave ) CMetaDataSourceDescriptor;
    CleanupStack::PushL( self );
    self->ConstructL( aDes );
    CleanupStack::Pop();
    return self;
    }

// Destructor
CMetaDataSourceDescriptor::~CMetaDataSourceDescriptor()
	{
	}

// -----------------------------------------------------------------------------
// CMetaDataSourceDescriptor::ReadL
// -----------------------------------------------------------------------------
//
void CMetaDataSourceDescriptor::ReadL(
	TDes8& aDes )
   	{
	if ( aDes.Length() >= iLength )
		{
		aDes.Copy(iSource);
		}
	else
		{
		aDes.Copy(iSource.Ptr(), aDes.MaxLength());
		}
    }

// -----------------------------------------------------------------------------
// CMetaDataSourceDescriptor::ReadL
// -----------------------------------------------------------------------------
//
void CMetaDataSourceDescriptor::ReadL(
	TDes8& aDes,
	TInt aLength )
   	{
   	if(aLength > aDes.MaxLength())
   		{
   		aLength = aDes.MaxLength();
   		}
	aDes.Copy(iSource.Ptr(), aLength);
    }

// -----------------------------------------------------------------------------
// CMetaDataSourceDescriptor::ReadL
// -----------------------------------------------------------------------------
//
void CMetaDataSourceDescriptor::ReadL(
	TInt aPos,
	TDes8& aDes )
    {
    if(aPos + aDes.MaxLength() > iLength)
    	{
    	aDes.FillZ(aDes.MaxLength());
    	return;
    	}
	aDes.Copy(iSource.Mid(aPos, aDes.MaxLength()));
    }

// -----------------------------------------------------------------------------
// CMetaDataSourceDescriptor::ReadL
// -----------------------------------------------------------------------------
//
void CMetaDataSourceDescriptor::ReadL(
	TInt aPos,
	TDes8& aDes,
	TInt aLength )
    {
    if(aPos + aLength > iLength)
    	{
    	aDes.FillZ(aDes.MaxLength());
    	return;
    	}
    if(aLength > aDes.MaxLength())
   		{
   		aLength = aDes.MaxLength();
   		}
	aDes.Copy(iSource.Mid(aPos, aLength));
	}

// -----------------------------------------------------------------------------
// CMetaDataSourceDescriptor::Size
// -----------------------------------------------------------------------------
//
TInt CMetaDataSourceDescriptor::Size(
	TInt& aSize ) const
    {
	aSize = iLength;
    return KErrNone;
    }

//  End of File
