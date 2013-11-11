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

#ifndef DTMF_PARAMETERS_H
#define DTMF_PARAMETERS_H

#include "Parameters.h"

class CDTMFParameters : public CParameters {
private:
	CDTMFParameters(TInt aAction, const TTimeIntervalMicroSeconds32 &aToneDuration,
			const TTimeIntervalMicroSeconds32 &aToneOffDuration,
			const TTimeIntervalMicroSeconds32 &aPauseDuration);

	void ConstructL(const TDesC &aDTMFString);
	void operator=(const CDTMFParameters &aParams);

public:
	~CDTMFParameters();
	static CDTMFParameters *NewLC(TInt aAction, const TTimeIntervalMicroSeconds32 &aToneDuration,
			const TTimeIntervalMicroSeconds32 &aToneOffDuration,
			const TTimeIntervalMicroSeconds32 &aPauseDuration,
			const TDesC &aDTMFString);

	static CDTMFParameters *NewLC(const CDTMFParameters &aParams);
	HBufC *GetDTMFString();
	TTimeIntervalMicroSeconds32 GetToneDuration();
	TTimeIntervalMicroSeconds32 GetToneOffDuration();
	TTimeIntervalMicroSeconds32 GetPauseDuration();

private:
	TTimeIntervalMicroSeconds32 iToneDuration;
	TTimeIntervalMicroSeconds32 iToneOffDuration;
	TTimeIntervalMicroSeconds32 iPauseDuration;

	HBufC *iDTMFString;
};

#endif
