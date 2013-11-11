// Copyright (c) 2000-2009 Nokia Corporation and/or its subsidiary(-ies).
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

#ifndef __CCOPYFILES_H__
#define __CCOPYFILES_H__

#include <e32base.h>
#include <f32file.h>

class CCopyOneFile;

class CCopyFiles : public CActive
/**
@internalComponent
@released
*/
	{
public:
	static CCopyFiles* NewL(RFs &aFs);

	void CopyDir(const TFileName &aFrom, const TFileName &aTo,TRequestStatus& aStatus);

	~CCopyFiles();

private:
	// From CActive;
	void RunL();
	void DoCancel();
	TInt RunError(TInt aError);


	void ConstructL();
	CCopyFiles(RFs &aFs);
	void NextDirL();
	void CopyFileL();
	void Complete(TInt aError);
	void InitDirCopy();

private:
	RFs		&iFs;
	CCopyOneFile *iCopyOneFile;
	CDirScan     *iDirScan;
	CDir		 *iNext;
	TInt		  iPos;

	// lets us know if this is the first time into the RunL
	TBool		  iStartedCopy;

	TFileName       *iFrom;
	TFileName       *iTo;
	const TFileName	 *iToPath;
	const TFileName	 *iFromPath;

	TRequestStatus *iReportStatus;
	};

#endif // __CCOPYFILES_H__
