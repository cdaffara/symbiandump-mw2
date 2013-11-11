/*
* Copyright (c) 2009 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  Base Class for Trigger Area
*
*/



#include <lbtgeoareabase.h>



// ======== LOCAL FUNCTIONS ========

// ---------------------------------------------------------------------------
// CLbtGeoAreaBase::CLbtGeoAreaBase()
// Default constructor
// ---------------------------------------------------------------------------
//
CLbtGeoAreaBase::CLbtGeoAreaBase()
    {
	
    }


// ---------------------------------------------------------------------------
// CLbtGeoAreaBase::~CLbtGeoAreaBase()
// Destructor
// ---------------------------------------------------------------------------
// 
CLbtGeoAreaBase::~CLbtGeoAreaBase()
    {
	
    }

// ---------------------------------------------------------------------------
// CLbtGeoAreaBase::InternalizeL()
// (other items were commented in a header).
// ---------------------------------------------------------------------------
// 
EXPORT_C void CLbtGeoAreaBase::InternalizeL( RReadStream& aStream )
    {
	DoInternalizeL(aStream);
    }

// ---------------------------------------------------------------------------
// CLbtGeoAreaBase::ExternalizeL()
// (other items were commented in a header).
// ---------------------------------------------------------------------------
// 
EXPORT_C void CLbtGeoAreaBase::ExternalizeL( RWriteStream& aStream ) const
    {
	DoExternalizeL(aStream);
    }

