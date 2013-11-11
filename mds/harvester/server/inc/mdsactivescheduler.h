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
* Description:
*
*/

#include <e32base.h>

#ifndef MDSACTIVESCHEDULER_H_
#define MDSACTIVESCHEDULER_H_

class CMdsActiveScheduler : public CActiveScheduler
{
public:
	IMPORT_C virtual void WaitForAnyRequest();
	IMPORT_C void SetAllowedAOStatus(TRequestStatus& aStatus);
	IMPORT_C void RemoveAllowedAOStatus();
	IMPORT_C virtual void Error(TInt aError) const;
private:
	TRequestStatus* iAllowedAOStatus;
};

#endif /*MDSACTIVESCHEDULER_H_*/
