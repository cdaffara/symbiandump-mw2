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
* Description: Base class implementation of trigger filter that is 
* used when listing triggers from location triggering server.
*
*/


#include "lbttriggerfilterbase.h"

// ======== LOCAL FUNCTIONS ========

// ---------------------------------------------------------------------------
// CLbtTriggerFilterBase::CLbtTriggerFilterBase()
// Default constructor
// ---------------------------------------------------------------------------
//
CLbtTriggerFilterBase::CLbtTriggerFilterBase()
    {
	
	}


// ---------------------------------------------------------------------------
// CLbtTriggerFilterBase::~CLbtTriggerFilterBase()
// Destructor
// ---------------------------------------------------------------------------
//
CLbtTriggerFilterBase::~CLbtTriggerFilterBase()
    {
	
    }

// ---------------------------------------------------------------------------
// CLbtTriggerFilterBase::InternalizeL()
// (other items were commented in a header).
// ---------------------------------------------------------------------------
// 
EXPORT_C void CLbtTriggerFilterBase::InternalizeL( RReadStream& aStream )
    {
	DoInternalizeL(aStream);
    }

// ---------------------------------------------------------------------------
// CLbtTriggerFilterBase::ExternalizeL()
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//
EXPORT_C void CLbtTriggerFilterBase::ExternalizeL( RWriteStream& aStream ) const
    {
	DoExternalizeL(aStream);
    }


