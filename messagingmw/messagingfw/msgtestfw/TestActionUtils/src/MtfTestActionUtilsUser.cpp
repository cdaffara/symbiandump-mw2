// Copyright (c) 2003-2009 Nokia Corporation and/or its subsidiary(-ies).
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

/**
 @file
*/


#include "MtfTestActionUtilsUser.h"

#include <msvuids.h>


TInt MtfTestActionUtilsUser::CopyDescriptor16To8(const TDesC16& aDesc16,TDes8& aDesc8)
	{
	for (TInt i=0; i<aDesc16.Length(); i++)
		if (aDesc16[i] > 255)
			return KErrOverflow;
	aDesc8.Copy(aDesc16);
	return KErrNone;
	}


TInt MtfTestActionUtilsUser::FinalProgressStatus(CMsvOperation& aOperation,const TRequestStatus& aStatus)
	{
	TInt err;

	if (aOperation.Mtm() == KUidMsvLocalServiceMtm)
		{
		TPckgBuf<TMsvLocalOperationProgress> progress;
		progress.Copy(aOperation.FinalProgress());
		err = progress().iError;
		}
	else
		err = aStatus.Int();

	return (err);
	}

