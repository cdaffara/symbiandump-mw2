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
// sisregistry - server classes
// 
//

/**
 @file 
 @test
 @internalComponent
*/
#ifndef __RTAMANAGERHANDLER_H__
#define __RTAMANAGERHANDLER_H__

#include <e32base.h>
#include "rtamessagehandler.h"
#include "clientserver.h"
#include "RTAserver.h"

namespace ReferenceTestAgent
	{
	class CRefTestAgentArchive;
	class CDrmFileContent;
	
	class CRtaManagerHandler: public CRtaMessageHandler
		{
	public:
		static CRtaManagerHandler* NewL(const CSession2& aSession);
		
		virtual void ServiceL(const RMessage2& aMessage);
		virtual ~CRtaManagerHandler();
	private:
		CRtaManagerHandler(const CSession2& aSession);
		
		void DeleteFileL(const RMessage2& aMessage);
		void CopyFileL(const RMessage2& aMessage);
		void CopyFileHandleL(const RMessage2& aMessage);
		void RenameFileL(const RMessage2& aMessage);
		void MkDirL(const RMessage2& aMessage);
		void MkDirAllL(const RMessage2& aMessage);
		void RenameDirL(const RMessage2& aMessage);
		void RmDirL(const RMessage2& aMessage);
		void GetDir1L(const RMessage2& aMessage);
		void GetDir2L(const RMessage2& aMessage);
		void GetDir3L(const RMessage2& aMessage);
		void GetAttributeL(const RMessage2& aMessage);
		void GetAttributeSetL(const RMessage2& aMessage);
		void GetStringAttributeL(const RMessage2& aMessage);
		void GetStringAttributeSetL(const RMessage2& aMessage);
		void NotifyStatusChangeL(const RMessage2& aMessage); 
		void CancelNotifyStatusChangeL(const RMessage2& aMessage); 	
		
		void OpenContentObjectL(const RMessage2& aMessage, TInt aUriParam, TInt aUniqueIdParam);
		void WriteCDirToMessageL(const RMessage2& aMessage, TInt aParam, CDir& aDir);
	private:
		RFile iFile;
		CRefTestAgentArchive* iArchive;
		
		// this pointer should not be deleted since it points to something owned by iArchive
		CDrmFileContent* iContentObject;
		};
		
	} //namespace

#endif 

