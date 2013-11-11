/*
* Copyright (c) 2003-2010 Nokia Corporation and/or its subsidiary(-ies).
* All rights reserved.
* This component and the accompanying materials are made available
* under the terms of "Eclipse Public License v1.0"
* which accompanies this distribution, and is available
* at the URL "http://www.eclipse.org/legal/epl-v10.html".
*
* Initial Contributors:
* Nokia Corporation - initial contribution.
*
* Contributors:
*
* Description:
* testagentconsumer.h
* Test agent implementation of the CAgentConsumer class
* 
*
*/



#ifndef __TESTAGENTDATA_H__
#define __TESTAGENTDATA_H__

#include <caf/cafplatform.h>
#include <caf/agentinterface.h>
#include "TestAgentFileInterface.h"

using namespace ContentAccess;
class CTestAgentData : public ContentAccess::CAgentData
		{
	public:
		static CTestAgentData* NewL(const TVirtualPathPtr& aVirtualPath, TContentShareMode aShareMode);
		static CTestAgentData* NewLC(const TVirtualPathPtr& aVirtualPath, TContentShareMode aShareMode);

		static CTestAgentData* NewL(RFile& aFile, const TDesC& aUniqueId);
		static CTestAgentData* NewLC(RFile& aFile, const TDesC& aUniqueId);

		virtual ~CTestAgentData();

	public: // From CAgentConsumer
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

	private:
		CTestAgentData();
		void ConstructL(const TVirtualPathPtr& aVirtualPath, TContentShareMode aShareMode);
		void ConstructL(RFile& aFile, const TDesC& aUniqueId);
		
	private:
		/* Calculate Data Size and Data offset */
		void CalculateDataSizeAndOffsetL();

		/* Format of a TUintvar is defined by the Open Mobile alliance in 
		*   Wireless Session Protocol OMA-WAP-WSP-v1_0-20020920-C
		*   @param aBuffer [in] buffer containing a TUintvar
		*   @param Offset [in] location of the first byte of the TUintvar within the buffer
		*   @param aNumBytes [out] number of bytes read from the buffer to decode the the TUintvar
		*   @return the value of the TUnitvar
		*/  
		TInt ReadTUintvarL(TDes8& aBuffer, TInt Offset, TInt& aNumBytes) const;

		// encapsulates and parses a DCF file
		CTestAgentFileInterface *iTestFileObject;

		// File session used when checking attributes
		RFs iFs;
		// File handle
		RFile iFile;

		CVirtualPath *iVirtualPath;
		};

#endif // __TESTAGENTCONSUMER_H__
