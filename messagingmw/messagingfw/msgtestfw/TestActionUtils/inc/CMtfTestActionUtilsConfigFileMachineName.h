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

#ifndef __CMTFTESTACTIONUTILSCONFIGFILEMACHINENAME_H__
#define __CMTFTESTACTIONUTILSCONFIGFILEMACHINENAME_H__

#include <e32base.h>

class CMtfTestActionUtilsConfigFileMachineName : public CBase
	{
public:
	static CMtfTestActionUtilsConfigFileMachineName* NewL(const TDesC& aPtrEmailAdd);
	static CMtfTestActionUtilsConfigFileMachineName* NewLC(const TDesC& aPtrEmailAdd);
   ~CMtfTestActionUtilsConfigFileMachineName();

public:
	const TDesC8& MachineName();
	const TDesC& MachineNameEmail();
private:
	CMtfTestActionUtilsConfigFileMachineName();
	void ConstructL(const TDesC& aPtrEmailAdd); 

private:
	HBufC8* iMachineName;
	HBufC* iMachineNameEmail;
	};

#endif
