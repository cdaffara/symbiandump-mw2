/**
* Copyright (c) 2004-2009 Nokia Corporation and/or its subsidiary(-ies).
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
*
*/



/**
 @file
*/

#ifndef CEXPMAILINFO_H_INCLUDED
#define CEXPMAILINFO_H_INCLUDED

class CExpPop3MailInfo : public CBase
	{

	
	public:


		CExpPop3MailInfo() { }

		virtual ~CExpPop3MailInfo() { }
		
		TInt GetNumLinesBodyText() const { return iLinesBodyText; }
		void SetNumLinesBodyText( TInt aLinesBodyText ) { iLinesBodyText = aLinesBodyText; }

		TInt GetNumAttachments() const { return iNumAttachments; }
		void SetNumAttachments( TInt aNumAttachments ) { iNumAttachments = aNumAttachments; }

		TInt GetNumLinesHtml() const { return iNumLinesHtml; }
		void SetNumLinesHtml( TInt aNumLinesHtml ) { iNumLinesHtml = aNumLinesHtml; }

		TBool GetFooterExpected() const { return iFooterExpected; }
		void SetFooterExpected( TBool aFooterExpected ) { iFooterExpected = aFooterExpected; }

		TInt GetFooterSize() const { return iFooterSize; }
		void SetFooterSize( TBool aFooterSize ) { iFooterSize = aFooterSize; }

		TDesC& GetDescription() { return iDescription; }
		void SetDescription( TDesC& aDesc ) { iDescription.Copy( aDesc ); }

	private:

		TInt						iLinesBodyText;
		TInt						iNumAttachments;
		TInt						iNumLinesHtml;
		TBool						iFooterExpected;
		TBuf<64>					iDescription;
		TInt						iFooterSize;





	};




#endif // CEXPMAILINFO_H_INCLUDED
