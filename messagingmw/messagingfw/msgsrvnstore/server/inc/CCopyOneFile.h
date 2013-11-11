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

#ifndef __CCOPYONEFILE_H__
#define __CCOPYONEFILE_H__

#include <e32base.h>
#include <f32file.h>

class CCopyOneFile : public CActive
/**
@internalComponent
@released
*/
	{
public:
	static CCopyOneFile* NewL(RFs &aFs);

	void Copy(const TFileName &aFrom, const TFileName &aTo,TRequestStatus& aStatus);

	void Copy(RFile& aFrom, RFile& aTo, TRequestStatus& aStatus);
	
	~CCopyOneFile();
private:
	// From CActive;
	void RunL();
	void DoCancel();
	TInt RunError(TInt aError);


	void ConstructL();
	CCopyOneFile(RFs &aFs);
	void CopyABuffer();

	void Complete(TInt aError);
private:
	// buffer allocated to copy the file, persists while the object persists
	// KCopyFileBufferSize bytes in length
	TDes8*	 iFileBuffer;
	RFs		&iFs;
	RFile	 iFrom;
	RFile	 iTo;
	// flag to tell RunL whether the last thing we did was read or write.
	TBool    iReading;
	TRequestStatus *iReportStatus;
	};

#endif // __CCOPYONEFILE_H__

