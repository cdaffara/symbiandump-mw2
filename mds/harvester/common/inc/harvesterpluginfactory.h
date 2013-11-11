/*
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

#ifndef __CHARVESTERPLUGINFACTORY_H__
#define __CHARVESTERPLUGINFACTORY_H__

#include <e32base.h>

// FORWARD DECLARATION 
class CHarvesterPluginInfo;
class CHarvesterPlugin;
class CHarvesterData;
class CHarvesterBlacklist;
class CHarvesterEventManager;

NONSHARABLE_CLASS( CHarvesterPluginFactory ) : public CBase
	{
	public:

		IMPORT_C static CHarvesterPluginFactory* NewL();
		virtual ~CHarvesterPluginFactory();
		
		IMPORT_C void GetObjectDefL( CHarvesterData& aHD, TDes& aObjectDef );
		IMPORT_C void GetMimeType(const TDesC& aUri, TDes& aMimeType);
		IMPORT_C TInt HarvestL( CHarvesterData* aHD );
		IMPORT_C RPointerArray<CHarvesterPluginInfo>& GetPluginInfos();
		IMPORT_C void SetBlacklist( CHarvesterBlacklist& aBlacklist );
		IMPORT_C TBool IsSupportedFileExtension( const TDesC& aFileName );
		IMPORT_C TBool IsContainerFileL( const TDesC& aURI );
		
		IMPORT_C void SendHarvestingStatusEventL( TBool aStarted );
		IMPORT_C void PauseHarvester( TBool aPaused );
		
		IMPORT_C void GetObjectDefL( CHarvesterData* aHD, TDes& aObjectDef );

	private:
		CHarvesterPluginFactory();
		void ConstructL();
		void SetupHarvesterPluginInfoL();
		void AddNewPluginL( const TDesC8& aType, const TDesC8& aOpaque, TUid aPluginUid );
		void GetSupportedPluginsL( RPointerArray<CHarvesterPluginInfo>& aSupportedPlugins, const TDesC& aExt );
		void SetPluginInfo( CHarvesterData* aHD );
		
	private:
		RPointerArray<CHarvesterPluginInfo> iHarvesterPluginInfoArray;
		CHarvesterBlacklist* iBlacklist;
		  
	    TBool iHarvesting;
		CHarvesterEventManager* iHarvesterEventManager;
		
		HBufC* iLastConfirmedContainerExt;
		HBufC* iLastConfirmedSupportedExt;
	};

#endif

