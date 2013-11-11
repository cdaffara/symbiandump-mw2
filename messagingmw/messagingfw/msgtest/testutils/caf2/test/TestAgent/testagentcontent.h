// Copyright (c) 2004-2009 Nokia Corporation and/or its subsidiary(-ies).
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

#ifndef __TESTAGENTCONTENT_H__
#define __TESTAGENTCONTENT_H__

#include <f32file.h>
#include <caf/agentinterface.h>
#include "TestAgentFile.h"
#include "TestAgentDrmContent.h"


using namespace ContentAccess;

/**
Test agent implementation of the CAgentContent class used to browse
DCF files

*/
class CTestAgentContent : public ContentAccess::CAgentContent
	{
public:
	// Two phase constructor used when the file is opened 
	// using a file name
	static CTestAgentContent* NewL(const TDesC& aURI, TContentShareMode aShareMode);
	static CTestAgentContent* NewLC(const TDesC& aURI, TContentShareMode aShareMode);
		
	// Two phase constructor used when the file is opened with 
	// a file handle
	static CTestAgentContent* NewL(RFile& aFile);
	static CTestAgentContent* NewLC(RFile& aFile);

	virtual ~CTestAgentContent();

public: 
	// From CAgentContent
	virtual TInt OpenContainer(const TDesC& aUniqueId);
	virtual TInt CloseContainer();
	virtual void GetEmbeddedObjectsL(RStreamablePtrArray<CEmbeddedObject>& aArray);
	virtual void GetEmbeddedObjectsL(RStreamablePtrArray<CEmbeddedObject>& aArray, TEmbeddedType aType);
	virtual TInt Search(RStreamablePtrArray<CEmbeddedObject>& aArray, const TDesC8& aMimeType, TBool aRecursive);
	virtual TInt GetAttribute(TInt aAttribute, TInt& aValue, const TDesC& aUniqueId);
	virtual TInt GetAttributeSet(RAttributeSet& aAttributeSet, const TDesC& aUniqueId);
	virtual TInt GetStringAttribute(TInt aAttribute, TDes& aValue, const TDesC& aUniqueId);
	virtual TInt GetStringAttributeSet(RStringAttributeSet& aStringAttributeSet, const TDesC& aUniqueId);
	virtual TInt AgentSpecificCommand(TInt aCommand, const TDesC8& aInputBuffer, TDes8& aOutputBuffer);
	virtual void AgentSpecificCommand(TInt aCommand, const TDesC8& aInputBuffer, TDes8& aOutputBuffer, TRequestStatus& aStatus);
	virtual void NotifyStatusChange(TEventMask aMask, TRequestStatus& aStatus, const TDesC& aUniqueId);
	virtual TInt CancelNotifyStatusChange(TRequestStatus& aStatus, const TDesC& aUniqueId);
	virtual void RequestRights(TRequestStatus& aStatus, const TDesC& aUniqueId);
	virtual TInt CancelRequestRights(TRequestStatus& aStatus, const TDesC& aUniqueId);
	virtual void DisplayInfoL(TDisplayInfo aInfo, const TDesC& aUniqueId);
	virtual TInt SetProperty(TAgentProperty aProperty, TInt aValue);

private:
	CTestAgentContent();
	void ConstructL(const TDesC& aURI, TContentShareMode aShareMode);
	void ConstructL(RFile& aFile);
	TInt DoSearchL(RStreamablePtrArray<CEmbeddedObject>& aArray, const TDesC8& aMimeType);
		
private:
	/** Handle to the filesystem */
	RFs iFs;
		
	/** File handle used to browse the DCF file*/
	RFile iFile;

	CTestAgentFile *iTestFile;
	CTestAgentDrmContent *iTestDrmContent;

	HBufC *iURI;
	TContentShareMode iShareMode;
	TBool ConstructedWithFileHandle;
	TBool iDummyContainerOpen;
	};
#endif // __TestAgentContent_H__
