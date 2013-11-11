// Copyright (c) 2002-2009 Nokia Corporation and/or its subsidiary(-ies).
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
// MMSSettingProxyBase.h
// Header file for base MMS Settings Proxy
// 
//

#ifndef __MMSSETTINGSPROXYBASE_H__
#define __MMSSETTINGSPROXYBASE_H__

#include <e32base.h>
#include <ecom/ecom.h>
#include <msventry.h>

const TUid KMMSSettingsProxyBaseUid = {0x101F9421};

_LIT(KMMSUrl, "MMSUrl");
_LIT(KCommDbRecordId, "CommDbRecordId");
_LIT(KMsvEntry, "CMsvEntry");
const TInt KMMSSettingsProxyVersionSupported 	= 2;
const TUid KMMSSettingsProxyInterfaceUid 		= {0x101f9420};

class TMMSSettingsPair
/**
Encapsulates an MMS settings value.

A setting is specified as a name-value pair.  

@see CMMSSettingsProxyBase
@publishedPartner
@released
*/
	{
public:
		/** Setting name. */
		TPtrC iName;
		/** Setting value. This must be cast to the correct type for the value. */
		TAny* iValue;
		/** Enables the object to form part of a linked list. */
		TSglQueLink iLink;
	};

class CMMSSettingsProxyBase : public CBase
/**
ECom interface for a plug-in to make MMS settings.

The BIO WAP Access Point Parser can receive settings for the MMS over-the-air. If it does so, 
it attempts to find a plug-in on the phone that implements this interface, and passes the 
settings to the plug-in. The plug-in should then update the MMS settings in whatever way 
is suitable for the MMS implementation on the phone.
 
@publishedPartner
@released
*/
{

public:
	// delegates construction to an ECom static method and casts return value to the correct type. 
	/** Constructs the MMS settings plug-in.

	@param aInterfaceImplUid Plug-in's implementation UID. The BIO WAP Access Point Parser expects the
	implementation to have the UID KMMSSettingsProxyBaseUid (0x101F9421).
	@return The MMS settings plug-in
	*/
	static CMMSSettingsProxyBase* NewL(TUid aInterfaceImplUid);	
	/**
	Destructor.
	*/
	virtual ~CMMSSettingsProxyBase();	
	/** Passes the MMS settings to the plug-in.

	The settings are given as a linked list of name-value pairs. Currently three settings are 
	contained in this list and an implementation may use any or all of them.  They are: 
	
	- MMSUrl - A TPtrC* containing the URL of the MMS centre
	- CommDbRecordId - A TUint32* containing the CommDb record ID of the WAP ISP.
	- CMsvServerEntry - A CMsvServerEntry* used to navigate the message store

	@param aMMSSettingsList Linked list of MMS settings
	*/
	virtual void SetMMSValuesL(TSglQue<TMMSSettingsPair>& aMMSSettingsList) = 0;

private:
	TUid iDtor_ID_Key;	// required for destruction identification within the ECom framework
};

#include <mmssettingsproxybase.inl>

#endif //__MMSSETTINGSPROXYBASE_H__
