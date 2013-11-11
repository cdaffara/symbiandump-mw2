/*
* Copyright (c) 2007 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  Active Scheduler Test Module
*
*/

#include "DTMFParameters.h"

CDTMFParameters::CDTMFParameters(TInt aAction, const TTimeIntervalMicroSeconds32 &aToneDuration,
		const TTimeIntervalMicroSeconds32 &aToneOffDuration,
		const TTimeIntervalMicroSeconds32 &aPauseDuration)
	: CParameters(aAction), iToneDuration(aToneDuration), iToneOffDuration(aToneOffDuration), iPauseDuration(aPauseDuration)
	{

	}

CDTMFParameters *CDTMFParameters::NewLC(TInt aAction, const TTimeIntervalMicroSeconds32 &aToneDuration,
			const TTimeIntervalMicroSeconds32 &aToneOffDuration,
			const TTimeIntervalMicroSeconds32 &aPauseDuration,
			const TDesC &aDTMFString)
	{
	CDTMFParameters *_self = new(ELeave)CDTMFParameters(aAction, aToneDuration, aToneOffDuration, aPauseDuration);
	CleanupStack::PushL(_self);
	_self->ConstructL(aDTMFString);
	return _self;
	}

CDTMFParameters *CDTMFParameters::NewLC(const CDTMFParameters &aParams)
	{
	CDTMFParameters *_self = new(ELeave)CDTMFParameters(aParams.iAction, aParams.iToneDuration, aParams.iToneOffDuration, aParams.iPauseDuration);
	CleanupStack::PushL(_self);
	_self->ConstructL(*aParams.iDTMFString);
	return _self;
	}

void CDTMFParameters::ConstructL(const TDesC &aDTMFString)
	{
	iDTMFString = aDTMFString.AllocL();
	}

HBufC *CDTMFParameters::GetDTMFString()
	{
	return iDTMFString;
	}

TTimeIntervalMicroSeconds32 CDTMFParameters::GetToneDuration()
	{
	return iToneDuration;
	}

TTimeIntervalMicroSeconds32 CDTMFParameters::GetToneOffDuration()
	{
	return iToneOffDuration;
	}

TTimeIntervalMicroSeconds32 CDTMFParameters::GetPauseDuration()
	{
	return iPauseDuration;
	}

CDTMFParameters::~CDTMFParameters ()
	{
	delete iDTMFString;
	}
