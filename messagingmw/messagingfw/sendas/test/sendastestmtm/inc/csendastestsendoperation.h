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

#ifndef __CSENDASTESTSENDOPERATION_H__
#define __CSENDASTESTSENDOPERATION_H__

#include <msvapi.h>

#ifndef SYMBIAN_ENABLE_SPLIT_HEADERS  
#include "msvconsts.h"
#endif

class CSendAsTestSendOperation : public CMsvSendOperation
	{
public:
	static CSendAsTestSendOperation* NewL(CMsvSession&				aMsvSession, 
										  const CMsvEntrySelection&	aSelection, 
										  const TDesC8& 			aParameter, 
										  TRequestStatus& 			aRequestStatus
										  );
	virtual ~CSendAsTestSendOperation();
private:	// methods from CMsvSendOperation
	virtual const TDesC8& TranslateProgress(const TDesC8& aProgress);
private:
	CSendAsTestSendOperation(CMsvSession& aMsvSession, TRequestStatus& aRequestStatus);
	void ConstructL(const CMsvEntrySelection& aSelection, const TDesC8& aParameter);
	};

#endif	//  __CSENDASTESTSENDOPERATION_H__
