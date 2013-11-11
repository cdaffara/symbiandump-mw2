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
* Description: Base class implementation of container filter
*
*/


#include <s32strm.h>
#include <lbterrors.h>
#include <lbttriggerentry.h>
#include <lbttriggerfilterbase.h>
#include "lbtcontainerfilterbase.h"
#include "lbtcontainertriggerfilter.h"



// ======== LOCAL FUNCTIONS ========


// ---------------------------------------------------------------------------
// CLbtTriggerEntry::SetState()
//
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//
 void CLbtContainerFilterBase::SetTrigFilter(CLbtTriggerFilterBase* aTrigFilter)
	{
	iTrigFilter = aTrigFilter;
	}

// ---------------------------------------------------------------------------
// CLbtTriggerEntry::SetRequestorsL()
//
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//
 void CLbtContainerFilterBase::SetContFilter(CLbtContainerFilter* aContFilter)
	{
	iContFilter = aContFilter;
	}
	
// ---------------------------------------------------------------------------
// CLbtTriggerEntry::SetRequestorL()
//
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//
 CLbtTriggerFilterBase* CLbtContainerFilterBase::TrigFilter()
	{
	return iTrigFilter;
	}

// ---------------------------------------------------------------------------
// CLbtTriggerEntry::ManagerUi()
//
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//
 CLbtContainerFilter* CLbtContainerFilterBase::ContFilter()
	{
	return iContFilter;
	} 

  
CLbtContainerFilterBase::~CLbtContainerFilterBase()
	{
	delete iContFilter;
	delete iTrigFilter;
	}

