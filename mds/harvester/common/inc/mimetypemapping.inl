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



template <typename T> TMimeTypeMapping<T>::TMimeTypeMapping(TPtrC aExt)
	{
	iExt.Set( aExt.Ptr(), aExt.Length() );
	}

template <typename T> TMimeTypeMapping<T>::TMimeTypeMapping(TPtrC aExt, TPtrC aMimeType, const T& aHandler) :
	iHandler( aHandler )
	{
	iExt.Set( aExt.Ptr(), aExt.Length() );
	iMimeType.Set( aMimeType.Ptr(), aMimeType.Length() );
	}

template <typename T> TInt TMimeTypeMapping<T>::CompareFunction(
		const TMimeTypeMapping<T>& aKey,
		const TMimeTypeMapping<T>& aItem)
	{
	return aKey.iExt.CompareF( aItem.iExt );
	}
