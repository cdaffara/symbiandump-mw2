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

#ifndef __CSENDASTESTUIMTM_H__
#define __CSENDASTESTUIMTM_H__

#include <mtmuibas.h>

class CSendAsTestUiMtm : public CBaseMtmUi
	{
public:
	IMPORT_C static CSendAsTestUiMtm* NewL(CBaseMtm& aBaseMtm, CRegisteredMtmDll& aRegisteredMtmDll);
	virtual ~CSendAsTestUiMtm();
private:	// methods from CBaseMtmUi
	virtual CMsvOperation* OpenL(TRequestStatus& aStatus); 
	virtual CMsvOperation* CloseL(TRequestStatus& aStatus);
	virtual CMsvOperation* EditL(TRequestStatus& aStatus);// Launches editor/settings dialog as appropriate  
	virtual CMsvOperation* ViewL(TRequestStatus& aStatus);// Launches viewer/settings dialog as appropriate  
	virtual CMsvOperation* OpenL(TRequestStatus& aStatus, const CMsvEntrySelection& aSelection); 
	virtual CMsvOperation* CloseL(TRequestStatus& aStatus, const CMsvEntrySelection& aSelection);
	virtual CMsvOperation* EditL(TRequestStatus& aStatus, const CMsvEntrySelection& aSelection);// Launches editor/settings dialog as appropriate  
	virtual CMsvOperation* ViewL(TRequestStatus& aStatus, const CMsvEntrySelection& aSelection);// Launches viewer/settings dialog as appropriate  
	virtual CMsvOperation* CancelL(TRequestStatus& aStatus, const CMsvEntrySelection& aSelection);
	virtual CMsvOperation* ReplyL(TMsvId aDestination, TMsvPartList aPartlist, TRequestStatus& aCompletionStatus);
	virtual CMsvOperation* ForwardL(TMsvId aDestination, TMsvPartList aPartList, TRequestStatus& aCompletionStatus);
	virtual CMsvOperation* ConfirmSendL(TRequestStatus& aStatus, const CMsvEntrySelection& aSelection, const TSecurityInfo& aClientInfo);
	virtual void GetResourceFileName(TFileName& aFileName) const;

private:
	CSendAsTestUiMtm(CBaseMtm& aBaseMtm, CRegisteredMtmDll& aRegisteredMtmDll);
	void ConstructL();
	};

#endif	// __CSENDASTESTUIMTM_H__
