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
#ifndef __RTACONTENTHANDLER_H__
#define __RTACONTENTHANDLER_H__

#include <e32base.h>
#include "clientserver.h"
#include "RTAserver.h"
#include "rtamessagehandler.h"

namespace ReferenceTestAgent
	{
	class CRefTestAgentArchive;
	class CDrmFileContent;
	
	class CRtaContentHandler : public CRtaMessageHandler
		{
	public:
		static CRtaContentHandler* NewL(const CSession2& aSession);

		virtual void ServiceL(const RMessage2& aMessage);
		virtual ~CRtaContentHandler();		
		
	private:
		CRtaContentHandler(const CSession2& aSession);
		
		// RRtaContent functions
		void OpenFileL(const RMessage2& aMessage);
		void OpenFileHandleL(const RMessage2& aMessage);
		void OpenContainerL(const RMessage2& aMessage);
		void CloseContainerL(const RMessage2& aMessage);
		void GetEmbeddedObjects1L(const RMessage2& aMessage);
		void GetEmbeddedObjects2L(const RMessage2& aMessage);
		void SearchL(const RMessage2& aMessage);		
		void GetAttributeL(const RMessage2& aMessage);
		void GetAttributeSetL(const RMessage2& aMessage);
		void GetStringAttributeL(const RMessage2& aMessage);
		void GetStringAttributeSetL(const RMessage2& aMessage);
	
	private:
		RFile iFile;
		CRefTestAgentArchive* iArchive;
		};
	} //namespace

#endif 
