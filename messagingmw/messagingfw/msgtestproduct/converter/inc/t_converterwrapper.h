// Copyright (c) 1998-2009 Nokia Corporation and/or its subsidiary(-ies).
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
@test
@internalComponent

This contains CT_ConverterWrapper
*/

#if (!defined __TEF_INTEGRATION_TEST_WRAPPER_H__)
#define __TEF_INTEGRATION_TEST_WRAPPER_H__


#include <test\datawrapper.h>

#include <msvapi.h>

#include "SmsTestUtils.h"

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

class CT_ConverterWrapper : public CDataWrapper
	{
public:
	void SmsConversionValidationL();
	CT_ConverterWrapper();
	~CT_ConverterWrapper();

	static	CT_ConverterWrapper*	NewL();

	virtual TBool	DoCommandL(const TTEFFunction& aCommand, const TTEFSectionName& aSection, const TInt aAsyncErrorIndex);
	virtual TAny*	GetObject() { return iObject; }
	inline virtual void	SetObjectL(TAny* aObject)
		{
		DestroyData();
		iObject	= static_cast<TInt*> (aObject);
		}
		
	void RunL(CActive* aActive, TInt aIndex); 	
		
const TDesC& ConvertToStr(MMsvSessionObserver::TMsvSessionEvent& aEvent);

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
	CSmsTestUtils* iTestUtils;
	static void CleanupOperation(TAny* aAny)
		{
		TInt* number = static_cast<TInt*>(aAny);
		delete number;
		}
	TBool CaptureEvent(const TDesC& aSection);
	// Converter Operations
	
	inline void DoCmdNewL(const TDesC& aSection);
	inline void DoCmdDoConversion(const TDesC& aEntry);
	inline void doConversion(const TDesC& aEntry);
	inline void DoCmdGetDriveList (const TDesC& aEntry);
	inline void DoCmdRestartConversion(const TDesC& aEntry);
	inline void DoCmdCancelConversion(const TDesC& aSection);
	inline void DoCmdGetConversionStatus(const TDesC& aSection);
	inline void DoCmdHandleMultipleConversionRequests();
	inline void DoCmdCheckOutofMemory(const TDesC& aSection);
	inline void DoCleanup();
	TBool GetVerdictFromConfig(const TDesC& aSection, const TDesC& aParameterName, MMsvSessionObserver::TMsvSessionEvent& aEvent);
	TBool VerifyImHeaderPartsforImapL(const TDesC& aSection,CMsvEntrySelection* aEntrySelection);
	TBool VerifyMimeHeaderPartsforImapL(const TDesC& aSection,CMsvEntrySelection* aEntrySelection);

protected:
	TInt*						iObject;
	CMsvSession* 				iSession;
	CT_MediaSessionObserver*	iObserver;

private:
	CActiveCallback* iActiveCallback;
	CActiveCallback* iActiveCallback1;

	};

#endif /* __TEF_INTEGRATION_TEST_WRAPPER_H__ */

