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

#ifndef __PUSHSERVERMTM_H__
#define __PUSHSERVERMTM_H__


#include <mtsr.h>
#include <msventry.h>
#include <pushentry.h>

/**
 * Server MTM for the WAP Push Service.  This is required by the MTM architecture, so that 
 * Push Messages can be saved under a single WAP Push Service Entry. Only basic functionality
 * has been implemented, create, change, delete and progress functions. Other functionality such
 * as copy from local to a service, etc., is not applicable to WAP Push so returns KErrNotSupported.
 * 
 * The Progress class for this MTM is defined in the header file for the Push Entry class to 
 * minimise the number of exported files for the WAP Push projects.
 */

//##ModelId=3B659E4001F8
class CWapPushServerMtm : public CBaseServerMtm
/**
@internalTechnology
@released
*/
	{

public:
/**
@internalComponent
@released
*/
	enum TWapPushServerState	{ 
								EPushWaiting, 
								EPushCreatingEntries,
								EPushChangingEntries,
								EPushDeletingEntries,
								EPushComplete 
								};



	//##ModelId=3B659E40034D
	IMPORT_C static CWapPushServerMtm* NewL(CRegisteredMtmDll& aRegisteredMtmDll, CMsvServerEntry* aInitialEntry);
	//##ModelId=3B659E40034C
	~CWapPushServerMtm();

	//##ModelId=3B659E400338
	void ChangeL(TMsvEntry aNewEntry, TRequestStatus& aStatus);
	//##ModelId=3B659E400324
	void CopyToLocalL(const CMsvEntrySelection& aSelection,TMsvId aDestination, TRequestStatus& aStatus);
	//##ModelId=3B659E400310
	void CopyFromLocalL(const CMsvEntrySelection& aSelection,TMsvId aDestination, TRequestStatus& aStatus);
	//##ModelId=3B659E4002FC
	void CreateL(TMsvEntry aNewEntry, TRequestStatus& aStatus);
	//##ModelId=3B659E4002E8
	void CopyWithinServiceL(const CMsvEntrySelection& aSelection,TMsvId aDestination, TRequestStatus& aStatus);
	//##ModelId=3B659E4002D5
	void DeleteAllL(const CMsvEntrySelection& aSelection, TRequestStatus& aStatus);
	//##ModelId=3B659E4002C0
	void MoveFromLocalL(const CMsvEntrySelection& aSelection,TMsvId aDestination, TRequestStatus& aStatus);
	//##ModelId=3B659E4002AC
	void MoveToLocalL(const CMsvEntrySelection& aSelection,TMsvId aDestination, TRequestStatus& aStatus);
	//##ModelId=3B659E400298
	void MoveWithinServiceL(const CMsvEntrySelection& aSelection,TMsvId aDestination, TRequestStatus& aStatus);

	//##ModelId=3B659E40027A
	void StartCommandL(CMsvEntrySelection& aSelection, TInt aCommand, const TDesC8& aParameter, TRequestStatus& aStatus);
	//
	//##ModelId=3B659E400273
	TBool CommandExpected();
	//
	//##ModelId=3B659E400272
	const TDesC8& Progress();

protected:
	//RunL stuff
	//##ModelId=3B659E400271
	void DoCancel();
	//##ModelId=3B659E400270
	void DoRunL();
	//##ModelId=3B659E400267
	void Complete();

	//##ModelId=3B659E40025E
	void DoComplete(TInt aError);
	//##ModelId=3B659E40025C
	TInt RunError(TInt aError);


private:
	//##ModelId=3B659E400249
	CWapPushServerMtm(CRegisteredMtmDll& aRegisteredMtmDll, CMsvServerEntry* aInitialEntry);
	//##ModelId=3B659E400248
	void Construct();
	//##ModelId=3B659E400241
	void ResetProgress();

	//##ModelId=3B659E40023E
	const CMsvEntrySelection*	iSelection;
	//##ModelId=3B659E400234
	TPushMTMProgressBuf			iProgressBuf;
	//##ModelId=3B659E400222
	TPushMTMProgress			iProgress;


	//##ModelId=3B659E400218
	TInt						iError;
	//##ModelId=3B659E40020E
	TRequestStatus*				iReportStatus;

	//##ModelId=3B659E400205
	TPushServerMTMState			iState;
	};

#endif
