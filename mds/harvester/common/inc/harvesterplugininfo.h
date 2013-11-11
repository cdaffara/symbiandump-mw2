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
* Description:  Harvester plugin information struct*
*/

#ifndef __HARVESTERPLUGININFO_H__
#define __HARVESTERPLUGININFO_H__

#include "harvesterplugin.h"

class CHarvesterData;

class CHarvesterPluginInfo : public CBase
	{
public:
	virtual ~CHarvesterPluginInfo()
	{
		delete iPlugin;
		iPlugin = NULL;
		iExtensions.ResetAndDestroy();
		iExtensions.Close();
		iObjectTypes.ResetAndDestroy();
		iObjectTypes.Close();
		iQueue.ResetAndDestroy();
		iQueue.Close();
	}
	
	CHarvesterPluginInfo() : CBase(), iPlugin( NULL ), iPluginUid( KNullUid )
	{
	}
	
	/** @var 	CHarvesterPlugin Plugin
	 *  @brief 	Pointer to the loaded plugin. */
	CHarvesterPlugin* iPlugin;

	/** @var 	RPointerArray<TDesC> iExtensions
	 *  @brief 	Supported extensions of the plugin. */
    RPointerArray<TDesC> iExtensions;
    
    /** @var 	RPointerArray<TDesC> iObjectTypes
	 *  @brief 	Describes plugin - returned by CImplementationInformation->DataType(). */
    RPointerArray<TDesC> iObjectTypes;

    /** @var 	TUid iPluginUid;
	 *  @brief 	Harvester plugin UID */	    
    TUid iPluginUid;

	/** */	
	RPointerArray<CHarvesterData> iQueue;
	
private:
	};

#endif

