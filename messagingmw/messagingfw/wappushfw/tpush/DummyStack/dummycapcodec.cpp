// Copyright (c) 2000-2009 Nokia Corporation and/or its subsidiary(-ies).
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

#include "dummycapcodec.h"


EXPORT_C CCapCodec::~CCapCodec()
	{
	}

/**
 * Clear class members to defaults and release memory
 * 
 * Uses Nokia defaults specified in 
 * '\wapstack\wapstksrv\include\cap_codec.h'
 */
EXPORT_C void CCapCodec::Reset()
	{
	iProtOptions=0x0004;	//WPS_CAP_ProtocolOptions
	iClientSDUSize=1400;	//WPS_CAP_DEFAULT_ClientSDUSize
	iServerSDUSize=1400;	//WPS_CAP_DEFAULT_ServerSDUSize
	iMethodMOR=1;			//WPS_CAP_DEFAULT_MethodMOR
	iPushMOR=1;				//WPS_CAP_DEFAULT_PushMOR
	}
    
/** 
 * Client application calls to construct a new capabilities class
 */
EXPORT_C CCapCodec* CCapCodec::NewL()
	{
	CCapCodec* self=new (ELeave) CCapCodec();
	CleanupStack::PushL(self);	
	self->ConstructL();
	CleanupStack::Pop();
	return self;
	}

/** 
 * Overloaded to take an encoded capabilities buffer retrieved from the client API
 * using a RWSPCOConn::GetSessionData() call
 */
EXPORT_C CCapCodec* CCapCodec::NewL(const TDesC8& /*aEncodeBuf*/)
	{
	return static_cast<CCapCodec*>(NULL);
	}
    
/** 
 * Set protocol options
 */
EXPORT_C void CCapCodec::SetProtocolOptions(const  TUint32 aProtOptions)
	{
	iProtOptions=aProtOptions;
	}
/**
 * Get protocol options
 */
EXPORT_C void CCapCodec::GetProtocolOptions(TUint32& aProtOptions)
	{
	aProtOptions=iProtOptions;
	}
    
/**
 * Set Client SDU size
 */
EXPORT_C void CCapCodec::SetClientSDUSize(const TUint32 aSize)
	{
	iClientSDUSize=aSize;
	}

/**
 * Get Client SDU size
 */
EXPORT_C void CCapCodec::ClientSDUSize(TUint32& aSize) const
	{
	aSize=iClientSDUSize;
	}

/**
 * Set Server SDU size
 */
EXPORT_C void CCapCodec::SetServerSDUSize(const TUint32 aSize)
	{
	iServerSDUSize=aSize;
	}

/**
 * Get Server SDU size
 */
EXPORT_C void CCapCodec::GetServerSDUSize(TUint32& aSize) const
	{
	aSize=iServerSDUSize;
	}

/**
 * Set method Maximum Outstanding Requests
 *
 */
EXPORT_C void CCapCodec::SetMethodMOR(const TUint8 aVal)
	{
	iMethodMOR=aVal;
	}

/**
 * Get method Maximum Outstanding Requests
 *
 */
EXPORT_C void CCapCodec::MethodMOR(TUint8& aVal) const
	{
	aVal=iMethodMOR;
	}

/**
 * Set push Maximum Outstanding Requests
 *
 */
EXPORT_C void CCapCodec::SetPushMOR(const TUint8 aVal)
	{
	iPushMOR=aVal;
	}

/**
 * Get for Maximum Outstanding Requests
 *
 */
EXPORT_C void CCapCodec::GetPushMOR(TUint8& aVal) const
	{
	aVal=iPushMOR;
	}

/** 
 * Set and Get for class extended methods
 * returns Nokia defined code , 0 = No Error
 */
EXPORT_C TInt CCapCodec::AddExtendedMethod(TDes8& /*aMethod*/,const TUint8 /*aVal*/)
	{
	return KErrNone;
	}

/**
 * Call NumExtendedMethods() first
 * aIndex is between (0 to (aNumExtendedMethods - 1)
 * Return KErrNone or KErrTooBig if aIndex is out of range or
 * the descriptor is too small. 
 */	
EXPORT_C TInt CCapCodec::GetExtendedMethod(const TInt /*aIndex*/,TDes8& /*aMethod*/,TUint8& /*aVal*/) const
	{
	return KErrNone;
	}

EXPORT_C void CCapCodec::NumExtendedMethods(TInt& /*aNumExtendedMethods*/) const
	{
	}

/** 
 * Set and Get for class code pages
 * returns Nokia defined code , 0 = No Error
 */
EXPORT_C TInt CCapCodec::AddCodePage(TDes8& /*aCodePage*/,const TUint8 /*aVal*/)
	{	
	return KErrNone;
	}

/**
 * Call NumCodePages() first
 * aIndex is between (0 to (aNumCodePages - 1)
 * Return KErrNone or KErrTooBig if aIndex is out of range or
 * the descriptor is too small.
 */
EXPORT_C TInt CCapCodec::GetCodePage(const TInt /*aIndex*/,TDes8& /*aCodePage*/,TUint8& /*aVal*/) const
	{
	return KErrNone;
	}

EXPORT_C void CCapCodec::NumCodePages(TInt& /*aNumCodePages*/) const
	{
	}

/**
 * Set and Get for Unknown/Other capabilities
 * returns Nokia defined code , 0 = No Error
 */
EXPORT_C TInt CCapCodec::AddUnknownCap(TDes8& /*aName*/,const TDesC8& /*aValue*/)
	{
	return KErrNone;
	}


/** 
 * Call NumUnknownCap first
 * aIndex is between (0 to (aNumUnknown - 1)
 * Return KErrNone or KErrTooBig if aIndex is out of range or one or more
 * of the descriptors is too small.
 */
EXPORT_C TInt CCapCodec::GetUnknownCap(const TInt /*aIndex*/,TDes8& /*aName*/,TDes8& /*aValue*/) const
	{
	return KErrNone;
	}

/** 
 * Set and Get for class aliases
 * Requires the use of an Alias class 
 * Returns Nokia defined code, 0 = no error
 */
EXPORT_C void CCapCodec::NumUnknownCap(TInt& /*aNumUnknown*/) const
	{
	}

EXPORT_C TInt CCapCodec::AddAliasAddress(const TAliasAddress& /*aAddress*/)
	{
	return KErrNone;
	}

/** 
 * Call NumAliasAddress() first
 * aIndex is between (0 to (aNumAddresses - 1)
 * Return KErrNone or KErrTooBig if aIndex is out of range 
 */
EXPORT_C TInt CCapCodec::GetAliasAddress(const TInt /*aIndex*/, TAliasAddress& /*aAddress*/) const
	{
	return KErrNone;
	}

EXPORT_C void CCapCodec::NumAliasAddress(TInt& /*aNumAddresses*/) const
	{
	}


EXPORT_C TInt CCapCodec::CapEncodeSize(void) const
	{
	return KErrNone;
	}

EXPORT_C TInt CCapCodec::Encode(TDes8& /*aDestCapSet*/) const
	{
	return KErrNone;
	}

EXPORT_C TInt CCapCodec::Decode(const TDesC8& /*aSrcCapSet*/)
	{
	return KErrNone;
	}

EXPORT_C void CCapCodec::ConstructL()
	{
	}

EXPORT_C void CCapCodec::ConstructL(const TDesC8& /*aDecodeBuf*/)
	{
	}









/**
 * For Get and Set of Alias Address
 *
 */
EXPORT_C TAliasAddress::~TAliasAddress()
	{
	}
	
EXPORT_C TAliasAddress::TAliasAddress()
	{
	iHasPort = FALSE;
	iHasBearer = FALSE;
	}
    
EXPORT_C void TAliasAddress::SetPort(const TUint16 aPort)
	{
	iPort = aPort;
	iHasPort = TRUE;
	}

EXPORT_C void TAliasAddress::SetBearer(const TUint8 aBearer)
	{
	iBearer = aBearer;
	iHasBearer = TRUE;
	}
	
EXPORT_C TInt TAliasAddress::SetAddress(const TDesC8& aAddress)
	{
	if(aAddress.Length() >iAddress.MaxLength())
		return KErrTooBig;
	iAddress = aAddress;
	return KErrNone;
	}

EXPORT_C TUint8 TAliasAddress::HasBearer(void) const
	{
	return iHasBearer;
	}
	
EXPORT_C TUint8 TAliasAddress::HasPort(void) const
	{
	return iHasPort;
	}

EXPORT_C TUint8 TAliasAddress::Bearer(void) const
	{
	return iBearer;
	}
	
EXPORT_C TUint16 TAliasAddress::Port(void) const
	{
	return iPort;
	}
	
EXPORT_C const TDesC8& TAliasAddress::Address() const
	{
	return iAddress;
	}
	
EXPORT_C void TAliasAddress::Reset(void)
	{
	iHasPort = FALSE;
	iHasBearer = FALSE;
	iAddress.SetLength(0);
	}
