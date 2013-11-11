// Copyright (c) 1999-2009 Nokia Corporation and/or its subsidiary(-ies).
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
// CMsvFindText
// 
//

inline TBool CMsvFindText::FoundText() const
/** Returns the result of the asynchronous search for a matching text string.

@return ETrue if there is a match, otherwise EFalse. */
	{
	return iFoundText;
	}
