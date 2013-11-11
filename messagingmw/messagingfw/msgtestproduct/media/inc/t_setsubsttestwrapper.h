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
// This contains CT_SetSubstTestWrapper
// 
//

/**
 @test
 @internalComponent
*/

#if (!defined __TEF_SETSUBST_TEST_WRAPPER_H__)
#define __TEF_SETSUBST_TEST_WRAPPER_H__


#include <test\datawrapper.h>

#include <f32file.h>


class CT_SetSubstTestWrapper : public CDataWrapper
	{
public:
	CT_SetSubstTestWrapper();
	~CT_SetSubstTestWrapper();

	static	CT_SetSubstTestWrapper*	NewL();

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

	// Central Repository commands
	inline void DoCmdCreateDrive(const TDesC& aSection);
	inline void DoCmdDeleteDrive(const TDesC& aSection);

protected:
	TInt*						iObject;
	RFs							iFs;
	};

#endif /* __TEF_SETSUBST_TEST_WRAPPER_H__ */

