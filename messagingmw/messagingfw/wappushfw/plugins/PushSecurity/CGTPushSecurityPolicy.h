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
// Purpose : Defines all the plug-in classes contained within the HttpSecurity
// plug-in DLL.
// The HTTP Security plug-in provides an implementation of security
// policy for use by HTTP when configuring WTLS in the WAP stack.
// // DISCLAIMER - THE IMPLEMENTATION FILE FOR THIS CLASS CONTAINS DUMMY         //
// //              POLICIES FOR TESTING PURPOSES ONLY.  IT SHOULD NOT FORM PART  //
// //              OF FINAL DELIVERABLE SOFTWARE.  IN REALITY, DELIVERABLE       //
// //              SOFTWARE SHOULD REFLECT ACTUAL LICENSEE OR NETWORK PROVIDER   //
// //              SECURITY POLICY.                                              //
// 
//

#ifndef __CGTPUSHSECURITYPOLICY_H__
#define __CGTPUSHSECURITYPOLICY_H__

// System includes
#include <e32base.h>

// Local includes
#include "CPushSecurityPolicy.h"

// PushSecurity Plug-in data type handled
_LIT(KWapPushSecurityData,"WTLS/Security Policy");

/**
 * Class definition for CWapPushSecurity
 */
//##ModelId=3B66B89E0137
class CGTPushSecurityPolicy : public CPushSecurityPolicy
/**
@internalComponent
@released
*/
	{
public:	// Methods

	//##ModelId=3B66B89E01A7
	static CGTPushSecurityPolicy* NewL();

	//##ModelId=3B66B89E01A6
	virtual ~CGTPushSecurityPolicy();

private: // Methods inherited from CPushSecurityPolicy

	//##ModelId=3B66B89E019F
	virtual TBool AnonymousAuthenticationAllowed();

	//##ModelId=3B66B89E0196
	virtual TBool GetKeyExchangeSuiteNormal(
									RWTLS::TKeyExchangeSuite& aSuite,
									RWTLS::TIdType& aIdType,
									TDes8& aKeyId
									);

	//##ModelId=3B66B89E0192
	virtual TBool GetKeyExchangeSuiteAnon(
									RWTLS::TKeyExchangeSuite& aSuite,
									RWTLS::TIdType& aIdType,
									TDes8& aKeyId
									);

	//##ModelId=3B66B89E018A
	virtual TBool GetNumCipherSuites(TInt& aNumCipherSuites);

	//##ModelId=3B66B89E0188
	virtual TBool GetCipherSuitesL(RWTLS::CipherSuiteArray& aCipherSuites);

	//##ModelId=3B66B89E0180
	virtual TBool GetKeyRefreshRate(TUint8& aRate);

	//##ModelId=3B66B89E017E
	virtual TBool GetSequenceNumberMode(RWTLS::TSequenceNumberMode& aMode);

	//##ModelId=3B66B89E0175
	virtual TBool GetRecordLengthUsage(TBool& aUseRecordLengthFlag);

	//##ModelId=3B66B89E0173
	virtual TBool GetSharedSecretLC(HBufC8*& aSharedSecret);

	//##ModelId=3B66B89E016A
	virtual TCipherResponse GetNegotiatedCipherAction(
										RWTLS::TCipherSuite* aSuite,
										RWTLS::TSequenceNumberMode* aMode
										);

	//##ModelId=3B66B89E0162
	virtual TAny* Extend_CPushSecurityPolicy1(TAny* aArg);

	//##ModelId=3B66B89E0160
	virtual TAny* Extend_CPushSecurityPolicy2(TAny* aArg);

private: // Methods

	//##ModelId=3B66B89E015F
	CGTPushSecurityPolicy();

	//##ModelId=3B66B89E0155
	void ConstructL();

private:	// Attributes

	/** Counter for the number of times that GetKeyExchangeSuiteXxx() has been called
	 */
	//##ModelId=3B66B89E014E
	TInt	iKeyExchangeSuitesSet;

	};

#endif    // __CGTPUSHSECURITYPOLICY_H__
