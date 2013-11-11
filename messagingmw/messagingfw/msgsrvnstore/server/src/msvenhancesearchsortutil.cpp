// Copyright (c) 2007-2009 Nokia Corporation and/or its subsidiary(-ies).
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
// msvenhancesearchsortutil..CPP
// 
//

#include <msvenhancesearchsortutil.h>


EXPORT_C TMsvEnhanceSearchSortUtil::TMsvEnhanceSearchSortUtil()
{
	iPartList=0;
}

/*
Sets the flag in iPartList variable
*/
EXPORT_C void TMsvEnhanceSearchSortUtil :: SetSearchSortSetting(TMsvEnhanceSearchSortSetting aPartList)
{
	iPartList |= aPartList;
}

/*
Returns iPartList TInt32 variable
*/
EXPORT_C TInt32 TMsvEnhanceSearchSortUtil :: GetSearchSortSetting()
{
	return iPartList;
}
/*
Removes the specified flag from iPartList variable
*/

void TMsvEnhanceSearchSortUtil :: RemoveSearchSortSetting(TMsvEnhanceSearchSortSetting aPartList)
{
	iPartList &= ~aPartList;
}





