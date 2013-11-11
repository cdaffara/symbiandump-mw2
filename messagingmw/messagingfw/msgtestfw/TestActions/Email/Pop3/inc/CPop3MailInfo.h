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

#ifndef __CPOP3_MAIL_INFO_H__
#define __CPOP3_MAIL_INFO_H__

#include <popcmtm.h>




class CPop3MailInfo : public CBase
{
	
	public:
		TImPop3PopulateOptions& MailInfo()						{ return iMailInfo; } 
	private:
		TImPop3PopulateOptions iMailInfo;



};

#endif // __CPOP3_MAIL_INFO_H__
