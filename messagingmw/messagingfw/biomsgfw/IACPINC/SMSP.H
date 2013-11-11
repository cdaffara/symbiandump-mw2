// Copyright (c) 1998-2009 Nokia Corporation and/or its subsidiary(-ies).
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
// SMS settings parser
// 
//

#ifndef __SMSP_H__
#define __SMSP_H__

#if !defined (__SMUTSET_H__)
#include <smutset.h>
#endif


class CSmsParser : public CBase
/**
@internalComponent
@released
*/
	{
public:
	static CSmsParser* NewLC();
	static CSmsParser* NewL();

	~CSmsParser();
	void ParseL(CParsedFieldCollection& aIacpFields);
	void ProcessL(CMsvEntry& aEntry); 
	
private:
	CSmsParser();
	void ConstructL();
	void FindServiceCentreAddressL();

private:
	CSmsSettings*	iSmsSettings;
	HBufC*			iServiceCentreName;
	HBufC*			iServiceCentreAddress;
	};
	
#endif // __SMSP_H__ 
