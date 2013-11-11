// Copyright (c) 2007-2009 Nokia Corporation and/or its subsidiary(-ies).
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

#include <e32std.h>
#include <ecom/ecom.h>
#include "CUriListLookup.h"


CUriListLookup::CUriListLookup()
/**
	Constructor.
*/
	{
	}
	
CUriListLookup::~CUriListLookup()
/**
	Destructor.
*/
	{
	REComSession::DestroyedImplementation(iEcomDtorID);
	}

MUriListLookup* CUriListLookup::NewL()
/**
	Factory constructor. 
	@return		A pointer to a fully constructed and initialised object.
*/
	{
	CUriListLookup* self = new(ELeave) CUriListLookup();
	return (static_cast<MUriListLookup*>(self));
	}

/**
	Retrieves the list type of a given URI. 
	@param	aServerAddress	The source URI.
	@param	aListType		On return, contains the list type of the source URI.
	@return KErrUriNotFound. The URI is not found in the list.
*/
TInt CUriListLookup::GetListType(const TDesC8& aServerAddress, InetUriList::TListType& aListType)
	{
	RInetUriList inetUriList;
	TRAPD(err, inetUriList.OpenL());
	if(err == KErrNone)
		{
		err = inetUriList.GetListType(aServerAddress, InetUriList::EWapPush, aListType);
		}
	inetUriList.Close();
	return err;			
	}	


