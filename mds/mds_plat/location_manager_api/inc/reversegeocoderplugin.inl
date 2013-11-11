/*
* Copyright (c) 2010 Nokia Corporation and/or its subsidiary(-ies).
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
* Description: This is partial interface implementation which is 
*              used by plugins. 
* 
*/



// INCLUDE FILES

#include <ecom/ecom.h>       // declares E-com framework classes


// -----------------------------------------------------------------------------
// CReverseGeoCoderPlugin::~CReverseGeoCoderPlugin()
// Destroys the plugin implementation
// -----------------------------------------------------------------------------
//
inline CReverseGeoCoderPlugin::~CReverseGeoCoderPlugin()
{
    REComSession::DestroyedImplementation( iDtorKey );
}


// -----------------------------------------------------------------------------
// CReverseGeoCoderPlugin::NewL()
// Creates the instance of CReverseGeoCoderPlugin concrete implementation.
// -----------------------------------------------------------------------------
//
inline CReverseGeoCoderPlugin* CReverseGeoCoderPlugin::NewL()
{
		RImplInfoPtrArray aImplInfoArray;
		REComSession::ListImplementationsL(KReversegeocodeInterfaceUid, aImplInfoArray);
		
		TInt count = aImplInfoArray.Count();
		
		if(!count) 
			{
			/* No implementations found for the reverse geocoding functionality */
			User::Leave(KErrNotFound);
			}
			
		/* One or more implementations found. Choose the first one */
		const CImplementationInformation* plugin = aImplInfoArray[0];
		TUid  pluginId = plugin->ImplementationUid();
		
		CReverseGeoCoderPlugin* self = reinterpret_cast<CReverseGeoCoderPlugin*>(
	       		REComSession::CreateImplementationL(
	           pluginId, _FOFF( CReverseGeoCoderPlugin,iDtorKey ) ) );
	
	    return self;
}

// End of file
