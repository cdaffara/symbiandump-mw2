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
// Declaration of the CMsvDescriptionArray and TMsvDescription classes 
// 
//

#ifndef CMSV_DESCRIPTION_ARRAY_H
#define CMSV_DESCRIPTION_ARRAY_H

class TMsvDescription
/** @internalTechnology */
	{
public:
	const TMsvEntry*	iEntry;
	TPtrC				iDescription;
	};

NONSHARABLE_CLASS(CMsvDescriptionArray) : public CArrayPtrFlat<TMsvDescription>
	{
public:
	CMsvDescriptionArray();
	~CMsvDescriptionArray();
	void ReverseOrder();
	};
#endif
