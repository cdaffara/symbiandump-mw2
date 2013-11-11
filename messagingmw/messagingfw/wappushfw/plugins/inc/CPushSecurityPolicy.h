// Copyright (c) 1997-2009 Nokia Corporation and/or its subsidiary(-ies).
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
// The HTTP Security plug-in provides an implementation of security policy for use by HTTP
// when configuring WTLS in the WAP stack.
// 
//

/**
 @file CPushSecurityPolicy.h
*/

#ifndef __CPUSHSECURITYPOLICY_H__
#define __CPUSHSECURITYPOLICY_H__

// System includes
//
#include <e32base.h>

namespace RWTLS
	{
	enum TBulkEncryptionAlgorithm
		{
		EBulkNull     = 0x00,
		ERc5_cbc_40   = 0x01,
		ERc5_cbc_56   = 0x02,
		ERc5_cbc      = 0x03,
		EDes_cbc_40   = 0x04,
		EDes_cbc      = 0x05,
		E3Des_cbc_ede = 0x06,
		EIdea_cbc_40  = 0x07,
		EIdea_cbc_56  = 0x08,
		EIdea_cbc     = 0x09
		};
	
	enum TMacAlgorithm
		{
		ESha_0         = 0x00,
		ESha_40        = 0x01,
		ESha_80        = 0x02,
		ESha           = 0x03,
		ESha_xor_40    = 0x04,
		EMd5_40        = 0x05,
		EMd5_80        = 0x06,
		EMd5           = 0x07
		};

	enum TIdType 
		{
		EIdNull,
		EText, 
		EBinary, 
		EKeyHashSha = 254, 
		EX509Name = 255
		};

	enum TKeyExchangeSuite 
		{    
		ESharedSecret = 1,
		EDHAnon,
		EDHAnon512,
		EDHAnon768,
		ERsaAnon,
		ERsaAnon512,
		ERsaAnon768,
		ERsa,
		ERsa512,
		ERsa768,
		EEcdhAnon,
		EEcdhAnon113,
		EEcdhAnon131,
		EEcdhEcdsa
		};

	struct TCipherSuite
		{
		TBulkEncryptionAlgorithm iBulkEncryptionAlgorithm;
		TMacAlgorithm            iMacAlgorithm;
		};

	typedef CArrayFixFlat<TCipherSuite> CipherSuiteArray;
					
	enum TSequenceNumberMode 
		{
		ENotUsed = 0, 
		EImplicit, 
		EExplicit
		};
	}


// Constant definitions used by this class
//
// Push Security Plug-in data type handled
//
_LIT(KPushSecurityData,"WTLS/SecurityPolicy");

// PushSecurity Plug-in UID matched to by the plugin server
//
const TUid KUidPushSecurityPolicy = { 0x101F3E59 };

// Class definition for CPushSecurityPolicy
//
//##ModelId=3B66B89D0232
class CPushSecurityPolicy : public CBase
/**
@internalComponent
@released
*/
	{
public: // Types

/**
	@enum	TCipherResponse
	An enumeration of actions to be taken in response to the negotiated	cipher, 
	as obtained from the WTLS layer of the stack. In circumstances where a lower
	cipher strength was obtained than requested, the policy may choose to abandon
	connection or to ask the user if they wish to continue.
 
@internalComponent
@released
*/
	enum TCipherResponse
		{
		/** Specifies that connection should be abandoned */
		ENeverConnect	= 0,
		/** Specifies that the user should be asked if the they wish to continue connecting */
		EAskToConnect	= 1,
		/** Specifies that connection should continue */
		EAlwaysConnect	= 2
		};

public: // Methods

/**
	Intended Usage	:	Static c'tor that uses ECom to instantiate a CPushSecurityPolicy
	derived class that matches the data in aMatchData.
	@since			6.0
	@param			aMatchData	A descriptor with the matching data
	@return			A pointer to the created CPushSecurityPolicy derived object that can
	matches the matching data.
 */
	//##ModelId=3B66B89D02DB
	inline static CPushSecurityPolicy* NewL(const TDesC& aMatchData);

/**
	Intended Usage	:	D'tor. Uses ECom to ensure that the DLL containing the 
	used interface implementation is unloaded at the appropriate time.
	@since			6.0
 */
	//##ModelId=3B66B89D02D4
	inline virtual ~CPushSecurityPolicy();

/**
	Intended Usage	:	Queries to see if anonymous authentication allowed.
	@since			6.0
	@return			A boolean value of ETrue if anonymous authentication can be 
	allowed, otherwise EFalse if given.
 */
	//##ModelId=3B66B89D02D3
	virtual TBool AnonymousAuthenticationAllowed() =0;

/**
	Intended Usage	:	Obtain the key exchange suite for WTLS, for a non-anonymous
	session (ie on in which the server-certificate is obtained and authenticated).
	@since			6.0
	@param			aSuite		An output argument with the key exchange suite desired for WTLS
	@param			aIdType		An output argument with the ID type desired for WTLS
	@param			aKeyId		An output argument with the key ID desired for WTLS
	@return			A boolean value of ETrue if the default is to be overridden.
 */
	//##ModelId=3B66B89D02CA
	virtual TBool GetKeyExchangeSuiteNormal(
										RWTLS::TKeyExchangeSuite& aSuite, 
										RWTLS::TIdType& aIdType, 
										TDes8& aKeyId
										) =0;
/**
	Intended Usage	:	Obtain the key exchange suite for WTLS for an anonymous
	session (ie on in which no server-certificate is obtained).
	@since			6.0
	@param			aSuite		An output argument with the key exchange suite desired for WTLS
	@param			aIdType		An output argument with the ID type desired for WTLS
	@param			aKeyId		An output argument with the key ID desired for WTLS
	@return			A boolean value of ETrue if the default is to be overridden.
 */
	//##ModelId=3B66B89D02BF
	virtual TBool GetKeyExchangeSuiteAnon(
									RWTLS::TKeyExchangeSuite& aSuite,
									RWTLS::TIdType& aIdType,
									TDes8& aKeyId
									) =0;

/**
	Intended Usage	:	Obtain the number of cipher suites desired for WTLS.
	@since			6.0
	@param			aNumCipherSuites	An output argument with the number of 
	cipher suites to be requested
	@return			A boolean value of ETrue if the default is to be overridden.
 */
	//##ModelId=3B66B89D02BD
	virtual TBool GetNumCipherSuites(TInt& aNumCipherSuites) =0;

/**
	Intended Usage	:	Obtain the cipher suites desired for WTLS - an array of 
	TCipherSuite
	@since			6.0
	@param			aCipherSuites	An output argument with the cipher suites to 
	be requested
	@return			A boolean value of ETrue if the default is to be overridden.
 */
	//##ModelId=3B66B89D02B5
	virtual TBool GetCipherSuitesL(RWTLS::CipherSuiteArray& aCipherSuites) =0;

/**
	Intended Usage	:	Obtain the key refresh rate for WTLS.
	@since			6.0
	@param			aRate	An output argument with the rate value desired. 
	Refreshing occurs every 2^aRate messages
	@return			A boolean value of ETrue if the default is to be overridden.
	@todo			Add the pre and post conditions.
	@pre 			None
	@post			Unspecified
 */
	//##ModelId=3B66B89D02B3
	virtual TBool GetKeyRefreshRate(TUint8& aRate) =0;

/**
	Intended Usage	:	Obtain the sequence number mode desired for WTLS.
	@since			6.0
	@param			aMode	An output argument with the desired mode
	@return			A boolean value of ETrue if the default is to be overridden.
 */
	//##ModelId=3B66B89D02AB
	virtual TBool GetSequenceNumberMode(RWTLS::TSequenceNumberMode& aMode) =0;

/**
	Intended Usage	:	Obtain the record length usage flag for WTLS.
	@since			6.0
	@param			aUseRecordLengthFlag	An output argument which has a value
	of ETrue if record length usage is desired for WTLS
	@return			A boolean value of ETrue if the default is to be overridden
 */
	//##ModelId=3B66B89D02A9
	virtual TBool GetRecordLengthUsage(TBool& aUseRecordLengthFlag) =0;

/**
	Intended Usage	:	Obtain the shared secret used for WTLS.
	@since			6.0
	@param			aSharedSecret	An output argument with the shared secret data
	@return			A boolean value of ETrue if the default is to be overridden
 */
	//##ModelId=3B66B89D02A0
	virtual TBool GetSharedSecretLC(HBufC8*& aSharedSecret) =0;

/**
	Intended Usage	:	Finds out what action to take with the actual ciphers 
	negotiated with the stack.
	@since			6.0
	@param			aSuite	The negotiated cipher suite, or NULL if none was 
	reported by the stack
	@param			aMode	The negotiated mode, or NULL if none was reported 
	by the stack
	@return			A TCipherResponse value that specifies the response to be taken.
 */
	//##ModelId=3B66B89D0297
	virtual TCipherResponse GetNegotiatedCipherAction(RWTLS::TCipherSuite* aSuite, RWTLS::TSequenceNumberMode* aMode) =0;

protected: // Methods

/**
	Intended Usage	:	C'tor.
	@since			6.0
 */
	//##ModelId=3B66B89D0296
	inline CPushSecurityPolicy();

private:	// Methods
	
/**
	Intended Usage	:	Reserved for future expansion. Reserves space in export 
	table to maintain binary compatibility.
	@since			6.0
	@param			aArg	An argument
	@return			A pointer to a generic data structure.
 */
	//##ModelId=3B66B89D0294
	virtual TAny* Extend_CPushSecurityPolicy1(TAny* aArg) =0;

/**
	Intended Usage	:	Reserved for future expansion. Reserves space in export 
	table to maintain binary compatibility.
	@since			6.0
	@param			aArg	An argument
	@return			A pointer to a generic data structure.
 */
	//##ModelId=3B66B89D028A
	virtual TAny* Extend_CPushSecurityPolicy2(TAny* aArg) =0;

private:

	/** A unique UID used in interface destruction 
	 */
	//##ModelId=3B66B89D0282
	TUid	iDtor_ID_Key;

	};

#include "CPushSecurityPolicy.inl"

#endif	// __CPUSHSECURITYPOLICY_H__




