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
*
*/



/**
 @file
 
 Gets Number Of Lines Of Body Text
 
 @return
 Returns Number Of Lines Of Body Text
*/
inline TInt CExpPop3MailInfo::GetNumLinesBodyText() const 
	{
	return iLinesBodyText;
	}

/** 
Sets Number Of Lines Of Body Text

@param aLinesBodyText
Count of Lines of Body Text
*/
inline void CExpPop3MailInfo::SetNumLinesBodyText(TInt aLinesBodyText) 
	{
	iLinesBodyText = aLinesBodyText;
	}

/** 
Gets Number Of Attachments

@return
Returns Number Of Attachments
*/
inline TInt CExpPop3MailInfo::GetNumAttachments() const 
	{
	return iNumAttachments;
	}
	
/** 
Sets Number Of Attachments

@param aNumAttachments
Count of Number of Attachments
*/
inline void CExpPop3MailInfo::SetNumAttachments(TInt aNumAttachments)
	{
	iNumAttachments = aNumAttachments;
	}

/** 
Gets Number Of Html Lines

@return
Returns Number Of Html Lines
*/
inline TInt CExpPop3MailInfo::GetNumLinesHtml() const 
	{
	return iNumLinesHtml;
	}

/** 
Sets Number Of Lines Of HTML

@param aLinesHtml
Count of Lines of HTML
*/
inline void CExpPop3MailInfo::SetNumLinesHtml(TInt aNumLinesHtml)
	{
	iNumLinesHtml = aNumLinesHtml;
	}

/** 
Gets Flag status for FooterExpected

@return
Returns Value of FooterExpected
*/
inline TBool CExpPop3MailInfo::GetFooterExpected() const
	{
	return iFooterExpected;
	}

/** 
Sets if Footer is Expected

@param aFooterExpected
Sets ETrue if Footer Expected
*/
inline void CExpPop3MailInfo::SetFooterExpected(TBool aFooterExpected)
	{
	iFooterExpected = aFooterExpected;
	}

/** 
Gets Size of Footer

@return
Returns Size of Footer
*/
inline TInt CExpPop3MailInfo::GetFooterSize() const
	{
	return iFooterSize;
	}

/** 
Sets Footer Size

@param aFooterSize
Sets Footer Size
*/
inline void CExpPop3MailInfo::SetFooterSize(TBool aFooterSize)
	{
	iFooterSize = aFooterSize;
	}

/** 
Gets Description

@return
Returns Description
*/
inline TDesC& CExpPop3MailInfo::GetDescription()
	{
	return iDescription;
	}

/** 
Sets Description

@param aDesc
Sets Description
*/
inline void CExpPop3MailInfo::SetDescription(TDesC& aDesc) 
	{ 
	iDescription.Copy(aDesc);
	}
