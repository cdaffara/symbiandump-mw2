/*
* Copyright (c) 2009 Nokia Corporation and/or its subsidiary(-ies). 
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
#ifndef MIMETYPEMAPPING_H_
#define MIMETYPEMAPPING_H_

//
// TMimeTypeMapping
// 
// Helper class for mapping file extensions 
// to MIME types and harvesting method.
//
template <typename T> class TMimeTypeMapping
	{
	public:
		TPtrC iExt;
		TPtrC iMimeType;
		T iHandler;
		
		TMimeTypeMapping(TPtrC aExt);

		TMimeTypeMapping(TPtrC aExt, TPtrC aMimeType, const T& aHandler);
		
		static TInt CompareFunction(const TMimeTypeMapping& aKey, 
				const TMimeTypeMapping& aItem);
	};

#include "mimetypemapping.inl"

#endif /*MIMETYPEMAPPING_H_*/
