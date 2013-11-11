// Copyright (c) 2004-2010 Nokia Corporation and/or its subsidiary(-ies).
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

#ifndef __REFTESTAGENTMANAGER_H__
#define __REFTESTAGENTMANAGER_H__

#include <caf/cafplatform.h>
#include <caf/agentinterface.h>

#include "rtamanager.h"

using namespace ContentAccess;

class CRefTestAgentManager : public CAgentManager
		{
	public:
		static CRefTestAgentManager* NewL();
		static CRefTestAgentManager* NewLC();

		~CRefTestAgentManager(void);

	public:
		// From CAgentManagerBase
		virtual TInt DeleteFile(const TDesC &aFileName);
		virtual TInt CopyFile(const TDesC& aSource, const TDesC& aDestination);
		virtual TInt CopyFile(RFile& aSource, const TDesC& aDestination);
		virtual TInt RenameFile(const TDesC& aSource, const TDesC& aDestination);
		virtual TInt MkDir(const TDesC& aPath);
		virtual TInt MkDirAll(const TDesC& aPath);
		virtual TInt RmDir(const TDesC& aPath);
		virtual TInt GetDir(const TDesC& aName,TUint aEntryAttMask,TUint aEntrySortKey, CDir*& aEntryList) const;
		virtual TInt GetDir(const TDesC& aName,TUint aEntryAttMask,TUint aEntrySortKey, CDir*& aEntryList,CDir*& aDirList) const;
		virtual TInt GetDir(const TDesC& aName,const TUidType& aEntryUid,TUint aEntrySortKey, CDir*& aFileList) const;
		virtual TInt GetAttribute(TInt aAttribute, TInt& aValue, const TVirtualPathPtr& aVirtualPath);
		virtual TInt GetAttributeSet(RAttributeSet& aAttributeSet, const TVirtualPathPtr& aVirtualPath);
		virtual TInt GetStringAttributeSet(RStringAttributeSet& aAttributeSet, const TVirtualPathPtr& aVirtualPath);
		virtual TInt GetStringAttribute(TInt aAttribute, TDes& aValue, const TVirtualPathPtr& aVirtualPath);
		virtual void NotifyStatusChange(const TDesC& aURI, TEventMask aMask, TRequestStatus& aStatus);
		virtual TInt CancelNotifyStatusChange(const TDesC& aURI, TRequestStatus& aStatus);
		virtual TInt SetProperty(TAgentProperty aProperty, TInt aValue);
		virtual void DisplayInfoL(TDisplayInfo aInfo, const TVirtualPathPtr& aVirtualPath);
		
		// From CAgentManager
		virtual TBool IsRecognizedL(const TDesC& aURI, TContentShareMode aShareMode) const;
		virtual TBool IsRecognizedL(RFile& aFile) const;
		virtual TBool RecognizeFileL(const TDesC& aFileName, const TDesC8& aBuffer, TDes8& aFileMimeType, TDes8& aContentMimeType) const;
		
		virtual TInt AgentSpecificCommand(TInt aCommand, const TDesC8& aInputBuffer, TDes8& aOutputBuffer);
		virtual void AgentSpecificCommand(TInt aCommand, const TDesC8& aInputBuffer, TDes8& aOutputBuffer, TRequestStatus& aStatus);
		virtual void DisplayManagementInfoL();
		virtual void PrepareHTTPRequestHeaders(RStringPool& aStringPool, RHTTPHeaders& aRequestHeaders) const;
	private:
		CRefTestAgentManager();
		void ConstructL();
		void DeleteFileL(const TDesC &aFileName);
		void PrepareHTTPRequestHeadersL(RStringPool& aStringPool, RHTTPHeaders& aRequestHeaders) const;
	
		
	private:
		ReferenceTestAgent::RRtaManager iManagerSession;
		};

#endif // __REFTESTAGENTMANAGER_H__
