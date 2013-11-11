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

#ifndef __DUMMYCAPCODEC_H__
#define __DUMMYCAPCODEC_H__

#include <e32base.h>

class TAliasAddress;

/**
 *  WAP client application capabilities interface class
 */
class CCapCodec : public CBase
	{
public:

	typedef enum
		{
		EAckHeaders =                  0x10,
		ESessionResume =               0x20,
		EPush =                        0x40,
		EConfPush =                    0x80,
		EAll =                         0xF0
		}EProtOptions;

	IMPORT_C ~CCapCodec();
	IMPORT_C void Reset();
	IMPORT_C static  CCapCodec* NewL();
	IMPORT_C static  CCapCodec* NewL(const TDesC8& aEncodeBuf);
	IMPORT_C void SetProtocolOptions(const TUint32 aProtOptions);
	IMPORT_C void GetProtocolOptions(TUint32& aProtOptions);
    
	IMPORT_C void SetClientSDUSize(const TUint32 aSize);
	IMPORT_C void ClientSDUSize(TUint32& aSize) const;
	IMPORT_C void SetServerSDUSize(const TUint32 aSize);
	IMPORT_C void GetServerSDUSize(TUint32& aSize) const;

	IMPORT_C void SetMethodMOR(const TUint8 aVal);
	IMPORT_C void MethodMOR(TUint8& aVal) const;
	IMPORT_C void SetPushMOR(const TUint8 aVal);
	IMPORT_C void GetPushMOR(TUint8& aVal) const;

	IMPORT_C TInt AddExtendedMethod(TDes8& aMethod,const TUint8 aVal);
	IMPORT_C TInt GetExtendedMethod(const TInt aIndex,TDes8& aMethod,TUint8& aVal) const;
	IMPORT_C void NumExtendedMethods(TInt& aNumExtendedMethods) const;

	IMPORT_C TInt AddCodePage(TDes8& aCodePage,const TUint8 aVal);
	IMPORT_C TInt GetCodePage(const TInt aIndex,TDes8& aCodePage,TUint8& aVal) const;
	IMPORT_C void NumCodePages(TInt& aNumCodePages) const;
	
	IMPORT_C TInt AddUnknownCap(TDes8& aName,const TDesC8& aValue);
	IMPORT_C TInt GetUnknownCap(const TInt aIndex,TDes8& aName,TDes8& aValue) const;
	IMPORT_C void NumUnknownCap(TInt& aNumUnknown) const;
	
	IMPORT_C TInt AddAliasAddress(const TAliasAddress& aAddress);
	IMPORT_C TInt GetAliasAddress(const TInt aIndex, TAliasAddress& aAddress) const;
	IMPORT_C void NumAliasAddress(TInt& aNumAddresses) const;
	
protected:
private:
	IMPORT_C TInt CapEncodeSize(void) const;
	IMPORT_C TInt Encode(TDes8& aDestCapSet) const;
	IMPORT_C TInt Decode(const TDesC8& aSrcCapSet);
	IMPORT_C void ConstructL();
	IMPORT_C void ConstructL(const TDesC8& aDecodeBuf);
private:
	TUint32 iProtOptions;
	TUint32 iClientSDUSize;
	TUint32 iServerSDUSize;
	TUint8 iMethodMOR;
	TUint8 iPushMOR;
	//code page
	//unknown cap
	//alias address
	};





/**
 * For get and set of alias addresses information
 *
 */
class TAliasAddress
{
public:
	IMPORT_C ~TAliasAddress();
	IMPORT_C TAliasAddress();
    
    // Set the port value
	IMPORT_C void SetPort(const TUint16 aPort);
	// Set the bearer value
	IMPORT_C void SetBearer(const TUint8 aBearer);
	// Set the address array
	IMPORT_C TInt SetAddress(const TDesC8& aAddress);
	// Bool's, use uchar to avoid conflict with Nokia BOOL (uchar) and Epoc TBool (Uint32) 
	IMPORT_C TUint8 HasBearer(void) const;
	IMPORT_C TUint8 HasPort(void) const;
	// Get Bearer value
	IMPORT_C TUint8 Bearer(void) const;
	// Get Port value
	IMPORT_C TUint16 Port(void) const;
	// Get the stored address
	IMPORT_C const TDesC8& Address() const;
	// Restore to constructed state
	IMPORT_C void Reset(void);

protected:
private:

	TBuf8<64> iAddress;
	TUint8 iBearer;
	TUint16 iPort;
	TUint8 iHasPort;
	TUint8 iHasBearer;
};
#endif
