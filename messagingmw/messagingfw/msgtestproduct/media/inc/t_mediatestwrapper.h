// Copyright (c) 2008-2009 Nokia Corporation and/or its subsidiary(-ies).
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
// @file
// This contains CT_MediaTestWrapper
// 
//

/**
 @test
 @internalComponent
*/

#if (!defined __TEF_INTEGRATION_TEST_WRAPPER_H__)
#define __TEF_INTEGRATION_TEST_WRAPPER_H__


#include <test\datawrapper.h>

#include <msvapi.h>


class CT_MediaSessionObserver : public CBase, public MMsvSessionObserver
	{
public:
	void HandleSessionEventL(TMsvSessionEvent aEvent, TAny*, TAny*, TAny*);
public:
	CMsvSession* iSession;
	TMsvSessionEvent iType;
	TInt iArg1;
	TInt iArg2;
	};

class CT_MediaTestWrapper : public CDataWrapper
	{
public:
	CT_MediaTestWrapper();
	~CT_MediaTestWrapper();

	static	CT_MediaTestWrapper*	NewL();

	virtual TBool	DoCommandL(const TTEFFunction& aCommand, const TTEFSectionName& aSection, const TInt aAsyncErrorIndex);
	virtual TAny*	GetObject() { return iObject; }
	inline virtual void	SetObjectL(TAny* aObject)
		{
		DestroyData();
		iObject	= static_cast<TInt*> (aObject);
		}

	inline virtual void	DisownObjectL()
		{
		iObject = NULL;
		}

	void DestroyData()
		{
		delete iObject;
		iObject=NULL;
		}

	inline virtual TCleanupOperation CleanupOperation()
		{
		return CleanupOperation;
		}

protected:
	void ConstructL();
private:
	static void CleanupOperation(TAny* aAny)
		{
		TInt* number = static_cast<TInt*>(aAny);
		delete number;
		}

	// Drive commands
	inline void DoCmdNewL(const TDesC& aEntry);
	inline void DoCmdAddDrive(const TDesC& aEntry);
	inline void DoCmdRemoveDrive(const TDesC& aSection);
	inline void DoCmdUpdateDrive(const TDesC& aSection);
	inline void DoCmdGetCurrentDrive(const TDesC& aSection);
	inline void DoCmdGetDriveList(const TDesC& aSection);
	inline void DoCmdGetAvailableDriveList(const TDesC& aSection);
	inline void DoCmdChildrenOfAvailableDrives(const TDesC& aSection);
	inline void DoCmdIsMessageStoreSupported(const TDesC& aSection);
	inline void DoCmdGetChildIdsAllL(const TDesC& aSection);
	void DoCmdVerifyCount(const TDesC& aSection);	
	void DoCmdCreateSMS();
	void DoCmdCreateSMSAccount();
	void DoCmdDeleteMessages();
	void DoCmdSearchMessages(const TDesC& aSection);
	void DoCmdRemoveMMC();
	void DoCmdCreateBulkSMS();
 	void DoCmdCopyStore(const TDesC& aSection);	
 	void DoCmdChangeDriveL(const TDesC& aSection);
 	
	// Disk commands
	inline void DoCmdMountDrive(const TDesC& aSection);
	inline void DoCmdDisMountDrive(const TDesC& aSection);
	
	// utilities
	TBool GetVerdictFromConfig(const TDesC& aSection, const TDesC& aParameterName, MMsvSessionObserver::TMsvSessionEvent& aEvent);
	TBool CaptureEvent(const TDesC& aSection);
	const TDesC& ConvertToStr(MMsvSessionObserver::TMsvSessionEvent& aEvent);
protected:
	TInt*						iObject;
	CMsvSession* 				iSession;
	CT_MediaSessionObserver*	iObserver;
	};

#endif /* __TEF_INTEGRATION_TEST_WRAPPER_H__ */

