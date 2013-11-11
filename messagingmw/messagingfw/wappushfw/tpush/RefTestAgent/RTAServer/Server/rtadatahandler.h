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
#ifndef __RTADATAHANDLER_H__
#define __RTADATAHANDLER_H__

#include <e32base.h>
#include "clientserver.h"
#include "RTAserver.h"
#include "rtamessagehandler.h"

namespace ReferenceTestAgent
	{
	class CRefTestAgentArchive;
	class CDrmFileContent;
	
	class CRtaDataHandler : public CRtaMessageHandler
		{
	public:
		static CRtaDataHandler* NewL(const CSession2& aSession);

		virtual void ServiceL(const RMessage2& aMessage);
		virtual ~CRtaDataHandler();		
		
	private:
		CRtaDataHandler(const CSession2& aSession);
		
		// RRtaData functions
		void OpenFileL(const RMessage2& aMessage);
		void OpenFileHandleL(const RMessage2& aMessage);
		void EvaluateIntentL(const RMessage2& aMessage);
		void ExecuteIntentL(const RMessage2& aMessage);
		void ReadDataL(const RMessage2& aMessage);
		void ReadOffsetDataL(const RMessage2& aMessage);
		void SeekL(const RMessage2& aMessage);
		void DataSizeL(const RMessage2& aMessage);
		void GetAttributeL(const RMessage2& aMessage);
		void GetAttributeSetL(const RMessage2& aMessage);
		void GetStringAttributeL(const RMessage2& aMessage);
		void GetStringAttributeSetL(const RMessage2& aMessage);
	
	private:
		RFile iFile;
		CRefTestAgentArchive* iArchive;
		
		// this pointer should not be deleted since it points to something owned by iArchive
		CDrmFileContent* iContentObject;
		};
	} //namespace

#endif 
