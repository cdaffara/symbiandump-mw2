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
* Description: Implementation of trigger condition base class.
*
*/


#include "lbttriggerconditionbase.h"


// ======== LOCAL FUNCTIONS ========

// ---------------------------------------------------------------------------
// CLbtTriggerConditionBase::CLbtTriggerConditionBase()
// Default constructor
// ---------------------------------------------------------------------------
//
CLbtTriggerConditionBase::CLbtTriggerConditionBase()
    {
	
	}

// ---------------------------------------------------------------------------
// CLbtTriggerConditionBase::~CLbtTriggerConditionBase()
// Destructor
// ---------------------------------------------------------------------------
//
CLbtTriggerConditionBase::~CLbtTriggerConditionBase()
    {
	
	}

// ---------------------------------------------------------------------------
// CLbtTriggerConditionBase::InternalizeL()
// (other items were commented in a header).
// ---------------------------------------------------------------------------
// 
EXPORT_C void CLbtTriggerConditionBase::InternalizeL( RReadStream& aStream )
    {
 	DoInternalizeL(aStream);
 	}

// ---------------------------------------------------------------------------
// CLbtTriggerConditionBase::ExternalizeL()
// (other items were commented in a header).
// ---------------------------------------------------------------------------
// 
EXPORT_C void CLbtTriggerConditionBase::ExternalizeL( RWriteStream& aStream ) const
    {
  	DoExternalizeL(aStream);
    }

