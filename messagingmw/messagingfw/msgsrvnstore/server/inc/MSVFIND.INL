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
// CMsvFindOperation
// 
//

inline const CMsvFindResultSelection& CMsvFindOperation::GetFindResult() const
/** Returns the result of the search operation.

If the search operation is still in progress, then the results returned are 
those that have been found at the time of this call.

@return A reference to the search operation results. */
	{
	return *iFindResultSel;
	}
