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
* Description:  Data transfer object for harvest data
 *
*/


#include <harvesterdata.h>
#include <mdeobject.h>
#include <locationdatatype.h>

// CHarvestData implementation

//==========================================================================
// Constructor
//==========================================================================
//
CHarvesterData::CHarvesterData( const HBufC* aUri )
	{
	iUri = aUri;
	iClientData = NULL;
	iMdeObject = NULL;
	iLocationData = NULL;
	iIsBinary = ETrue;
	iClientId = KNullUid;
	}

//==========================================================================
// Destructor
//==========================================================================
//
CHarvesterData::~CHarvesterData()
	{
	delete iUri;
	iUri = NULL;
	delete iClientData;
	iClientData = NULL;
	delete iMdeObject;
	iMdeObject = NULL;
	delete iLocationData;
	iLocationData = NULL;
	}

//==========================================================================
// NewLC
//==========================================================================
//
EXPORT_C CHarvesterData* CHarvesterData::NewLC( const HBufC* aUri )
	{
	CHarvesterData* self = new (ELeave)CHarvesterData( aUri );
	CleanupStack::PushL( self );
	self->ConstructL();
	return self;
	}

//==========================================================================
// NewL
//==========================================================================
//
EXPORT_C CHarvesterData* CHarvesterData::NewL( const HBufC* aUri )
	{
	CHarvesterData* self=CHarvesterData::NewLC( aUri );
	CleanupStack::Pop(); // self;
	return self;
	}

//==========================================================================
// ConstructL
//==========================================================================
//
void CHarvesterData::ConstructL()
	{
	}

//==========================================================================
// Uri
//==========================================================================
//
EXPORT_C const TDesC& CHarvesterData::Uri()
	{
	return *iUri;
	}

//==========================================================================
// SetEventType
//==========================================================================
//
EXPORT_C void CHarvesterData::SetEventType( THarvesterEventType aEventType )
	{
	iEventType = aEventType;
	}

//==========================================================================
// EventType
//==========================================================================
//
EXPORT_C THarvesterEventType CHarvesterData::EventType()
	{
	return iEventType;
	}

//==========================================================================
// SetPluginObserver
//==========================================================================
//
EXPORT_C void CHarvesterData::SetPluginObserver( MHarvesterPluginObserver& aObserver )
	{
	iObserver = &aObserver;
	}

//==========================================================================
// PluginObserver
//==========================================================================
//
EXPORT_C MHarvesterPluginObserver* CHarvesterData::PluginObserver()
	{
	return iObserver;
	}

//==========================================================================
// SetOrigin
//==========================================================================
//
EXPORT_C void CHarvesterData::SetOrigin( TOrigin aOrigin )
	{
	iOrigin = aOrigin;
	}

//==========================================================================
// Origin
//==========================================================================
//
EXPORT_C TOrigin CHarvesterData::Origin()
	{
	return iOrigin;
	}

//==========================================================================
// SetTakeSnapshot
//==========================================================================
//
EXPORT_C void CHarvesterData::SetTakeSnapshot( TBool aTakeSnapshot )
	{
	iTakeSnapshot = aTakeSnapshot;
	}

//==========================================================================
// TakeSnapshot
//==========================================================================
//
EXPORT_C TBool CHarvesterData::TakeSnapshot()
	{
	return iTakeSnapshot;
	}

//==========================================================================
// SetObjectType
//==========================================================================
//
EXPORT_C void CHarvesterData::SetObjectType( THarvesterObjectType aObjectType )
	{
	iObjectType = aObjectType;
	}

//==========================================================================
// ObjectType
//==========================================================================
//
EXPORT_C THarvesterObjectType CHarvesterData::ObjectType()
	{
	return iObjectType;
	}

//==========================================================================
// SetClientData
//==========================================================================
//
EXPORT_C void CHarvesterData::SetClientData( CBase* aClientData )
	{
	iClientData = aClientData;
	}

//==========================================================================
// ClientData
//==========================================================================
//
EXPORT_C CBase* CHarvesterData::ClientData()
	{
	return iClientData;
	}

//==========================================================================
// ErrorCode
//==========================================================================
//
EXPORT_C TInt CHarvesterData::ErrorCode()
	{
	return iErrorCode;
	}

//==========================================================================
// SetErrorCode
//==========================================================================
//
EXPORT_C void CHarvesterData::SetErrorCode( TInt aErrorCode )
	{
	iErrorCode = aErrorCode;
	}

//==========================================================================
// MdeObject
//==========================================================================
//
EXPORT_C CMdEObject& CHarvesterData::MdeObject()
	{
	return *iMdeObject;
	}

//==========================================================================
// SetMdeObject
//==========================================================================
//
EXPORT_C void CHarvesterData::SetMdeObject( CMdEObject* aMdeObject )
	{
    if( iMdeObject )
        {
        delete iMdeObject;
        iMdeObject = NULL;
        }
	iMdeObject = aMdeObject;
	}

//==========================================================================
// SetLocationData
//==========================================================================
//
EXPORT_C void CHarvesterData::SetLocationData( TLocationData* aLD )
	{
	iLocationData = aLD;
	}

//==========================================================================
// LocationData
//==========================================================================
//
EXPORT_C TLocationData* CHarvesterData::LocationData()
	{
	return iLocationData;
	}

//==========================================================================
// SetClientId
//==========================================================================
//
EXPORT_C void CHarvesterData::SetClientId( TUid aClientId )
	{
	iClientId = aClientId;
	}

//==========================================================================
// ClientId
//==========================================================================
//
EXPORT_C TUid CHarvesterData::ClientId()
	{
	return iClientId;
	}

//==========================================================================
// AddLocation
//==========================================================================
//
EXPORT_C TBool CHarvesterData::AddLocation()
	{
	return iAddLocation;
	}

//==========================================================================
// SetAddLocation
//==========================================================================
//
EXPORT_C void CHarvesterData::SetAddLocation( TBool aAdd )
	{
	iAddLocation = aAdd;
	}

//==========================================================================
// SetHarvesterPluginInfo
//==========================================================================
//
EXPORT_C void CHarvesterData::SetHarvesterPluginInfo( CHarvesterPluginInfo* aPluginInfo )
	{
	iPluginInfo = aPluginInfo;
	}

//==========================================================================
// IsBinary
//==========================================================================
//
EXPORT_C TBool CHarvesterData::IsBinary()
	{
	return iIsBinary;
	}

//==========================================================================
// SetBinary
//==========================================================================
//
EXPORT_C void CHarvesterData::SetBinary( TBool aValue )
	{
	iIsBinary = aValue;
	}

//==========================================================================
// HarvesterPluginInfo
//==========================================================================
//
EXPORT_C CHarvesterPluginInfo* CHarvesterData::HarvesterPluginInfo()
	{
	return iPluginInfo;
	}
