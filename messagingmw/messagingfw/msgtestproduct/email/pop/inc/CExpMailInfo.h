/**
* Copyright (c) 2006-2009 Nokia Corporation and/or its subsidiary(-ies).
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
* This contains the class for CExpPop3MailInfo 
* 
*
*/



/**
 @file
*/

#ifndef __CEXPMAILINFO_H__
#define __CEXPMAILINFO_H__


// Epoc include
#include <e32base.h>

/**
This class holds the information of number of lines, description etc... of a message.
*/
class CExpPop3MailInfo : public CBase
	{
public:
	CExpPop3MailInfo() { }

	virtual ~CExpPop3MailInfo() { }
	
	inline TInt GetNumLinesBodyText() const;
	inline void SetNumLinesBodyText(TInt aLinesBodyText);

	inline TInt GetNumAttachments() const;
	inline void SetNumAttachments(TInt aNumAttachments);

	inline TInt GetNumLinesHtml() const;
	inline void SetNumLinesHtml(TInt aNumLinesHtml);

	inline TBool GetFooterExpected() const;
	inline void SetFooterExpected(TBool aFooterExpected);

	inline TInt GetFooterSize() const;
	inline void SetFooterSize(TBool aFooterSize);

	inline TDesC& GetDescription();
	inline void SetDescription(TDesC& aDesc);

private:
	TInt						iLinesBodyText;
	TInt						iNumAttachments;
	TInt						iNumLinesHtml;
	TBool						iFooterExpected;
	TBuf<64>					iDescription;
	TInt						iFooterSize;
	};


#include "CExpMailInfo.inl"

#endif // __CEXPMAILINFO_H__
