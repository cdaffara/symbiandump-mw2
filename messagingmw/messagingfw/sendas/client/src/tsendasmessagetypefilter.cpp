// Copyright (c) 2004-2009 Nokia Corporation and/or its subsidiary(-ies).
// All rights reserved.
// This component and the accompanying materials are made available
// under the terms of "Eclipse Public License v1.0"
// which accompanies this distribution, and is available
// at the URL "http://www.eclipse.org/legal/epl-v10.html".
//
// Initial Contributors:
// Nokia Corporation - initial contribution.
//
// Contributors:
//
// Description:
//


#include <tsendasmessagetypefilter.h>


EXPORT_C TSendAsMessageTypeFilter::TSendAsMessageTypeFilter() : 
	iMessageCapability(KNullUid), 
	iValue(0), 
	iCondition(RSendAs::ESendAsNoCondition)
	{
	}

EXPORT_C TSendAsMessageTypeFilter::TSendAsMessageTypeFilter(TUid aMsgCap) : 
	iMessageCapability(aMsgCap), 
	iValue(0), 
	iCondition(RSendAs::ESendAsNoCondition)
	{
	}

EXPORT_C TSendAsMessageTypeFilter::TSendAsMessageTypeFilter(TUid aMsgCap, TInt aVal, RSendAs::TSendAsConditionType aCond) : 
	iMessageCapability(aMsgCap), 
	iValue(aVal), 
	iCondition(aCond)
	{
	}

EXPORT_C void TSendAsMessageTypeFilter::Reset()
	{
	iMessageCapability = KNullUid;
	iValue = 0;
	iCondition = RSendAs::ESendAsNoCondition;
	}
