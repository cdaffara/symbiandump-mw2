// Copyright (c) 2005-2009 Nokia Corporation and/or its subsidiary(-ies).
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

#ifndef __CSENDASEDITUTILS_H__
#define __CSENDASEDITUTILS_H__

#include <e32base.h>
#include <msvstd.h>

const TUid KUidSendAsEditUtils = { 0x1020806B };

/** 

@publishedPartner
@released
*/
class CSendAsEditUtils : public CBase
	{
public:
	inline static CSendAsEditUtils* NewL(TUid aUid);
	inline virtual ~CSendAsEditUtils();

	virtual void LaunchEditorL(TMsvId aId, TRequestStatus& aStatus) =0;
	virtual void LaunchEditorAndWaitL(TMsvId aId) =0;
	virtual void Cancel() =0;
	
protected:
	inline CSendAsEditUtils();
	
private:
	TUid	iDtor_ID_Key;
	};

#include <csendaseditutils.inl>

#endif	// __CSENDASEDITUTILS_H__
