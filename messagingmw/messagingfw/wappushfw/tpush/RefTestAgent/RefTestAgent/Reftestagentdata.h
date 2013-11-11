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

#ifndef __REFTESTAGENTDATA_H__
#define __REFTESTAGENTDATA_H__

#include <caf/cafplatform.h>
#include <caf/agentinterface.h>
#include "rtaArchive.h"
#include "drmfilecontent.h"
#include "rtadata.h"

using namespace ContentAccess;
using namespace ReferenceTestAgent;

class CRefTestAgentData : public CAgentData
		{
	public:
		static CRefTestAgentData* NewL(const TVirtualPathPtr& aVirtualPath, TContentShareMode aShareMode);
		static CRefTestAgentData* NewLC(const TVirtualPathPtr& aVirtualPath, TContentShareMode aShareMode);

		static CRefTestAgentData* NewL(RFile& aFile, const TDesC& aUniqueId);
		static CRefTestAgentData* NewLC(RFile& aFile, const TDesC& aUniqueId);

		virtual ~CRefTestAgentData();

	public: // From CAgentData
		virtual TInt Read(TDes8& aDes);
		virtual TInt Read(TDes8& aDes,TInt aLength);
		virtual void Read(TDes8& aDes, TRequestStatus& aStatus);
		virtual void Read(TDes8& aDes, TInt aLength, TRequestStatus& aStatus);
		virtual void DataSizeL(TInt& aSize);
		virtual TInt Seek(TSeek aMode,TInt& aPos);
		virtual TInt SetProperty(TAgentProperty aProperty, TInt aValue);
		virtual TInt EvaluateIntent(TIntent aIntent);
		virtual TInt ExecuteIntent(TIntent aIntent);
		virtual TInt GetAttribute(TInt aAttribute, TInt& aValue);
		virtual TInt GetAttributeSet(RAttributeSet& aAttributeSet);
		virtual TInt GetStringAttribute(TInt aAttribute, TDes& aValue);
		virtual TInt GetStringAttributeSet(RStringAttributeSet& aStringAttributeSet);
		virtual TInt Read(TInt aPos, TDes8& aDes, TInt aLength, TRequestStatus& aStatus);
	private:
		CRefTestAgentData();
		void ConstructL(const TVirtualPathPtr& aVirtualPath, TContentShareMode aShareMode);
		void ConstructL(RFile& aFile, const TDesC& aUniqueId);
		
	private:
		RRtaData iServer;
		};

#endif // __REFTESTAGENTCONSUMER_H__
