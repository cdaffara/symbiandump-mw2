/*
* Copyright (c) 2002 Nokia Corporation and/or its subsidiary(-ies).
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
* Description: Inline methods for AlwaysOnlineEComInterface.
*
*/


#include "AlwaysOnlineEComInterface.h"
#include "alwaysonlinemanagercommon.h"

_LIT8(KAlwaysOnlineManagerImplementationType,"text/*");

inline CAlwaysOnlineEComInterface::CAlwaysOnlineEComInterface( )
	{
	// Do nothing
	}

inline CAlwaysOnlineEComInterface::~CAlwaysOnlineEComInterface()
	{
	// Destroy any instance variables and then
	// inform the framework that this specific 
	// instance of the interface has been destroyed.
	REComSession::DestroyedImplementation(iDtor_ID_Key);
	}

inline CAlwaysOnlineEComInterface* CAlwaysOnlineEComInterface::NewL( TUid aId )
	{
    return REINTERPRET_CAST(CAlwaysOnlineEComInterface*, 
        REComSession::CreateImplementationL( aId, _FOFF(CAlwaysOnlineEComInterface,iDtor_ID_Key) ));
	}

inline CAlwaysOnlineEComInterface* CAlwaysOnlineEComInterface::NewL(const TDesC8& aMatchString)
	{
	// Set up the interface find for the default resolver.
	TEComResolverParams ResolverParams;
	ResolverParams.SetDataType(aMatchString);
	ResolverParams.SetWildcardMatch(ETrue);		// Allow wildcard matching
	TUid resolverUid = {0x10009D91};

	// Set up some empty initialisation parameters
	TAlwaysOnlineManagerInterfaceInitParams initParams;
	initParams.integer		= 0;
	initParams.descriptor	= NULL;

	return REINTERPRET_CAST(CAlwaysOnlineEComInterface*, 
							REComSession::CreateImplementationL(KCEComInterfaceUid, 
															   _FOFF(CAlwaysOnlineEComInterface,iDtor_ID_Key), 
															   &initParams,
															   ResolverParams,
															   resolverUid));
	}

inline void CAlwaysOnlineEComInterface::ListImplementationsL(RImplInfoPtrArray& aImplInfoArray)
	{
	// Set up the interface find for the default resolver.
	TEComResolverParams ResolverParams;
	ResolverParams.SetDataType(KAlwaysOnlineManagerImplementationType());
	ResolverParams.SetWildcardMatch(ETrue);		// Allow wildcard matching
	TUid resolverUid = {0x10009D91};

	REComSession::ListImplementationsL(KCEComInterfaceUid,
									   ResolverParams,
									   resolverUid,
									   aImplInfoArray);
	}

inline void CAlwaysOnlineEComInterface::ListAllImplementationsL(RImplInfoPtrArray& aImplInfoArray)
	{
	REComSession::ListImplementationsL(KCEComInterfaceUid, aImplInfoArray);
	}


inline TAny* CAlwaysOnlineEComInterface::HandleServerCommandL( TInt /*aCommand*/, TDesC8* /*aParameters*/ )
    {
    return (TAny*)KErrNotSupported;
    }

inline void CAlwaysOnlineEComInterface::SetStatusQueryObject( MAlwaysOnlineStatusQueryInterface* aStatusQueryObject )
    {
    iStatusQueryObject = aStatusQueryObject;
    }

inline TUid CAlwaysOnlineEComInterface::InstanceUid()
    {
    return iDtor_ID_Key;
    }
