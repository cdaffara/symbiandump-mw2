// Copyright (c) 2001-2009 Nokia Corporation and/or its subsidiary(-ies).
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
// Implements all the WapPushSecurity plugin classes contained within
// this plugin DLL
// // DISCLAIMER - THIS SOURCE FILE CONTAINS DUMMY POLICIES FOR TESTING PURPOSES //
// //              ONLY.  IT SHOULD NOT FORM PART OF FINAL DELIVERABLE SOFTWARE. //
// //              IN REALITY, DELIVERABLE SOFTWARE SHOULD REFLECT ACTUAL        //
// //              LICENSEE OR NETWORK PROVIDER SECURITY POLICY.                 //
// 
//

// Local includes
#include "CGTPushSecurityPolicy.h"

// Constants
_LIT(KReserved, "Reserved");

TAny* CGTPushSecurityPolicy::Extend_CPushSecurityPolicy1(TAny* aArg)
	{
	User::Panic(KReserved, KErrNotSupported);
	return aArg;
	}

TAny* CGTPushSecurityPolicy::Extend_CPushSecurityPolicy2(TAny* aArg)
	{
	User::Panic(KReserved, KErrNotSupported);
	return aArg;
	}

/**
 * Implementation of class CGTPushSecurityPolicy
 */

/**
 * Standard plugin constructor
 * Return				: nothing
 * Error Behaviour		: none, cannot fail.
 * Post Conditions		: constructed class with partial initialisation.
*/
CGTPushSecurityPolicy::CGTPushSecurityPolicy()
: CPushSecurityPolicy()
	{
	}

/**
 * Complete the initialisation of the plugin
 * May leave if any part of initialisation fails
 * Post-Condition : Fully initialised class when successful.
 */
void CGTPushSecurityPolicy::ConstructL()
	{
	}

/**
 * Create an instance of a CGTPushSecurityPolicy on the heap
 * Leaving if the instance cannot be fully initialised.
 * Nothing is left on the cleanup stack
 *
 * Rtn: a new CHTTPManager object, by ptr. Ownership is transferred to the
 *      caller.
 */
CGTPushSecurityPolicy* CGTPushSecurityPolicy::NewL()
	{
	CGTPushSecurityPolicy* self = new (ELeave) CGTPushSecurityPolicy;
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop(self);
	return self;
	}

// Default d'tor. Removes this object and releases memory held by it
CGTPushSecurityPolicy::~CGTPushSecurityPolicy()
	{
	}


/**
 * Methods inherited from CSecurityPolicy
 */

/**
 * Is anonymous authentication allowed?
 *
 * Rtn: ETrue if anonymous authentication can be allowed
 */
TBool CGTPushSecurityPolicy::AnonymousAuthenticationAllowed()
	{
	// This GT Plug-in doesn't allow anonymous authentication.
	//__LOG_ENTER(_L("CGTPushSecurityPolicy::AnonymousAuthenticationAllowed"));
	return ETrue;
	}

/**
 * Obtain the key exchange suite for WTLS, for a non-anonymous session (ie.
 * on in which the server-certificate is obtained and authenticated)
 *
 * Out:
 *  aSuite	- the key exchange suite desired for WTLS
 *  aIdType	- the ID type desired for WTLS
 *  aKeyId	- the key ID desired for WTLS
 *
 * Rtn: ETrue if the default is to be overridden
 */
TBool CGTPushSecurityPolicy::GetKeyExchangeSuiteNormal(
								RWTLS::TKeyExchangeSuite& aSuite,
								RWTLS::TIdType& aIdType,
								TDes8& aKeyId
								)
	{
	//
	// NB DISCLAIMER - THIS IS A DUMMY POLICY FOR TESTING PURPOSES ONLY.   //
	//                 IN REALITY, DELIVERABLE SOFTWARE SHOULD REFLECT     //
	//                 ACTUAL LICENSEE/NETWORK PROVIDER POLICY.            //
	//
	//__LOG_ENTER(_L("CGTPushSecurityPolicy::GetKeyExchangeSuiteNormal"));
	TBool retVal = EFalse;
	switch (iKeyExchangeSuitesSet)
		{
		case 0:
			{
			aSuite = RWTLS::ERsa512;
			aIdType = RWTLS::EIdNull;
			aKeyId.Copy(KNullDesC8);
			retVal = ETrue;
			iKeyExchangeSuitesSet++;
			} break;
		case 1:
			{
			aSuite = RWTLS::ERsa768;
			aIdType = RWTLS::EIdNull;
			aKeyId.Copy(KNullDesC8);
			retVal = ETrue;
			iKeyExchangeSuitesSet++;
			} break;
		case 2:
			{
			// No more key exchange suites to be set...
			retVal = EFalse;
			}
		}
	//__LOG_RETURN;
	return retVal;
	}

/**
 * Obtain the key exchange suite for WTLS, for an anonymous session (ie.
 * on in which no server-certificate is obtained)
 *
 * Out:
 *  aSuite	- the key exchange suite desired for WTLS
 *  aIdType	- the ID type desired for WTLS
 *  aKeyId	- the key ID desired for WTLS
 *
 * Rtn: ETrue if the default is to be overridden
 */
TBool CGTPushSecurityPolicy::GetKeyExchangeSuiteAnon(
								RWTLS::TKeyExchangeSuite& aSuite,
								RWTLS::TIdType& aIdType,
								TDes8& aKeyId
								)
	{
	//
	// NB DISCLAIMER - THIS IS A DUMMY POLICY FOR TESTING PURPOSES ONLY.   //
	//                 IN REALITY, DELIVERABLE SOFTWARE SHOULD REFLECT     //
	//                 ACTUAL LICENSEE/NETWORK PROVIDER POLICY.            //
	//
	//__LOG_ENTER(_L("CGTPushSecurityPolicy::GetKeyExchangeSuiteAnon"));
	TBool retVal = EFalse;
	switch (iKeyExchangeSuitesSet)
		{
		case 0:
			{
			aSuite = RWTLS::ERsaAnon512;
			aIdType = RWTLS::EIdNull;
			aKeyId.Copy(KNullDesC8);
			retVal = ETrue;
			iKeyExchangeSuitesSet++;
			} break;
		case 1:
			{
			aSuite = RWTLS::ERsaAnon768;
			aIdType = RWTLS::EIdNull;
			aKeyId.Copy(KNullDesC8);
			retVal = ETrue;
			iKeyExchangeSuitesSet++;
			} break;
		case 2:
			{
			// No more key exchange suites to be set...
			retVal = EFalse;
			}
		}
	//__LOG_RETURN;
	return retVal;
	}

/**
 * Obtain the number of cipher suites desired for WTLS
 *
 * Out:
 *  aNumCipherSuites - the number of cipher suites to be requested
 *
 * Rtn: ETrue if the default is to be overridden
 */
TBool CGTPushSecurityPolicy::GetNumCipherSuites(TInt& aNumCipherSuites)
	{
	//
	// NB DISCLAIMER - THIS IS A DUMMY POLICY FOR TESTING PURPOSES ONLY.   //
	//                 IN REALITY, DELIVERABLE SOFTWARE SHOULD REFLECT     //
	//                 ACTUAL LICENSEE/NETWORK PROVIDER POLICY.            //
	//
	//__LOG_ENTER(_L("CGTPushSecurityPolicy::GetNumCipherSuites"));
	// We support four different suites (see next method)
	aNumCipherSuites = 4;
	//__LOG_RETURN;
	return ETrue;
	}

/**
 * Obtain the cipher suites desired for WTLS - an array of TCipherSuite
 *
 * Out:
 *  aCipherSuites - the cipher suites to be requested
 *
 * Rtn: ETrue if the default is to be overridden
 */
TBool CGTPushSecurityPolicy::GetCipherSuitesL(RWTLS::CipherSuiteArray& aCipherSuites)
	{
	//
	// NB DISCLAIMER - THIS IS A DUMMY POLICY FOR TESTING PURPOSES ONLY.   //
	//                 IN REALITY, DELIVERABLE SOFTWARE SHOULD REFLECT     //
	//                 ACTUAL LICENSEE/NETWORK PROVIDER POLICY.            //
	//
	//__LOG_ENTER(_L("CGTPushSecurityPolicy::GetCipherSuitesL"));
	// Add the four suites we support to the supplied array
	RWTLS::TCipherSuite suite1 = {RWTLS::ERc5_cbc_56, RWTLS::ESha};
	aCipherSuites.AppendL(suite1);
	RWTLS::TCipherSuite suite2 = {RWTLS::ERc5_cbc_56, RWTLS::ESha_80};
	aCipherSuites.AppendL(suite2);
	RWTLS::TCipherSuite suite3 = {RWTLS::ERc5_cbc_56, RWTLS::ESha_40};
	aCipherSuites.AppendL(suite3);
	RWTLS::TCipherSuite suite4 = {RWTLS::ERc5_cbc_40, RWTLS::ESha};
	aCipherSuites.AppendL(suite4);
	//__LOG_RETURN;
	return ETrue;
	}

/**
 * Obtain the key refresh rate for WTLS
 *
 * Out:
 *  aRate - the rate value desired. Refreshing occurs every 2^aRate msgs
 *
 * Rtn: ETrue if the default is to be overridden
 */
TBool CGTPushSecurityPolicy::GetKeyRefreshRate(TUint8& aRate)
	{
	//
	// NB DISCLAIMER - THIS IS A DUMMY POLICY FOR TESTING PURPOSES ONLY.   //
	//                 IN REALITY, DELIVERABLE SOFTWARE SHOULD REFLECT     //
	//                 ACTUAL LICENSEE/NETWORK PROVIDER POLICY.            //
	//
	//__LOG_ENTER(_L("CGTPushSecurityPolicy::GetKeyRefreshRate"));
	// Set a rate of 3 - ie. keys are refreshed after every 2^3 messages
	aRate = 3;
	//__LOG_RETURN;
	return ETrue;
	}

/**
 * Obtain the sequence number mode desired for WTLS
 *
 * Out:
 *  aMode - the desired mode
 *
 * Rtn: ETrue if the default is to be overridden
 */
TBool CGTPushSecurityPolicy::GetSequenceNumberMode(RWTLS::TSequenceNumberMode& aMode)
	{
	//
	// NB DISCLAIMER - THIS IS A DUMMY POLICY FOR TESTING PURPOSES ONLY.   //
	//                 IN REALITY, DELIVERABLE SOFTWARE SHOULD REFLECT     //
	//                 ACTUAL LICENSEE/NETWORK PROVIDER POLICY.            //
	//
	//__LOG_ENTER(_L("CGTPushSecurityPolicy::GetSequenceNumberMode"));
	// Sequence number mode is not used
	aMode = RWTLS::ENotUsed;
	//__LOG_RETURN;
	return ETrue;
	}
/**
 * Obtain the record length usage flag for WTLS
 *
 * Out:
 *  aUseRecordLengthFlag : ETrue if record length usage is desired for WTLS
 *
 * Rtn: ETrue if the default is to be overridden
 */
TBool CGTPushSecurityPolicy::GetRecordLengthUsage(TBool& aUseRecordLengthFlag)
	{
	//
	// NB DISCLAIMER - THIS IS A DUMMY POLICY FOR TESTING PURPOSES ONLY.   //
	//                 IN REALITY, DELIVERABLE SOFTWARE SHOULD REFLECT     //
	//                 ACTUAL LICENSEE/NETWORK PROVIDER POLICY.            //
	//
	//__LOG_ENTER(_L("CGTPushSecurityPolicy::GetRecordLengthUsage"));
	// Record length is not used
	aUseRecordLengthFlag = EFalse;
	//__LOG_RETURN;
	return ETrue;
	}

/**
 * Obtain the shared secret used for WTLS
 *
 * Out:
 *  aSharedSecret - the shared secret data
 *
 * Rtn: ETrue if the default is to be overridden
 */
TBool CGTPushSecurityPolicy::GetSharedSecretLC(HBufC8*& aSharedSecret)
	{
	//
	// NB DISCLAIMER - THIS IS A DUMMY POLICY FOR TESTING PURPOSES ONLY.   //
	//                 IN REALITY, DELIVERABLE SOFTWARE SHOULD REFLECT     //
	//                 ACTUAL LICENSEE/NETWORK PROVIDER POLICY.            //
	//
	//__LOG_ENTER(_L("CGTPushSecurityPolicy::GetSharedSecretLC"));
	// Shared secret is not used
	aSharedSecret = NULL;
	//__LOG_RETURN;
	return EFalse;
	}
/**
 * Find out what action to take with the actual ciphers negotiated with
 * the stack
 *
 * In:
 *  aSuite	- the negotiated cipher suite, or NULL if none was reported by
 *            the stack
 *  aMode	- the negotiated mode, or NULL if none was reported by the stack
 *
 * Rtn: TCipherResponse enumerates the response to be taken
 */
CPushSecurityPolicy::TCipherResponse
CGTPushSecurityPolicy::GetNegotiatedCipherAction(
									RWTLS::TCipherSuite* aSuite,
									RWTLS::TSequenceNumberMode* aMode
									)
	{
	//
	// NB DISCLAIMER - THIS IS A DUMMY POLICY FOR TESTING PURPOSES ONLY.   //
	//                 IN REALITY IT SHOULD CHECK THE ACTUAL CIPHER VALUES //
	//                 AGAINST THOSE REQUESTED TO DETERMINE THEIR          //
	//                 ACCEPTABILITY.                                      //
	//

	// Check the values negotiated with the stack. Check for the worst case
	// first
	if ((aSuite == NULL) && (aMode == NULL))
		{
		// Nothing negotiated at all - never complete the connection
		return CPushSecurityPolicy::ENeverConnect;
		}
	else if ((aSuite == NULL) || (aMode == NULL))
		{
		// One thing was negotiated out of two - ask the user
		return CPushSecurityPolicy::EAskToConnect;
		}
	else
		{
		// Both things negotiated ok - connect unconditionally
		return CPushSecurityPolicy::EAlwaysConnect;
		}
	}
