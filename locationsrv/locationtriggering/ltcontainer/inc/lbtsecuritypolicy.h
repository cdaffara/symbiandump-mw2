/*
* Copyright (c) 2006 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  This header file describes the LBT security policy that is 
*                used by LT container to access/modify the database
*
*/


#ifndef LBTSECURITYPOLICY
#define LBTSECURITYPOLICY

/**
 * This class provides the inteface to define security policy. As of now
 * the security policy only provides for security check based on secure id.
 * This class will be extended any future requirements for security checking
 *
 *  The inteface provides the following functionality,
 *  - Define policy for a particular process based on secure id
 *
 *  @lib lbtcontainer.lib
 *  @since S60 v4.0
 */
class TLbtSecurityPolicy
	{
	public:
		// Default constructor
		TLbtSecurityPolicy()
			{
			iOwnerSecureId = KNullUidValue;
			}
		
		// Copy constructor
		TLbtSecurityPolicy(const TLbtSecurityPolicy& aSecurityPolicy)
			{
			iOwnerSecureId = aSecurityPolicy.GetOwnerSecureId();
			}
		
		// Sets the secure id
		inline void SetOwnerSecureId(TSecureId aSecureId)
			{
			iOwnerSecureId = aSecureId;
			}
		
		// Returns the set secure id
		inline const TSecureId GetOwnerSecureId() const
			{
			return iOwnerSecureId;
			}
		
		// Checks if the security check is null
		inline TBool IsNull()
			{
			return (iOwnerSecureId == KNullUidValue);
			}
		
		// overloading comparison operator
		inline TBool operator==(TLbtSecurityPolicy& aSecurityPolicy)
			{
			return ( iOwnerSecureId == aSecurityPolicy.GetOwnerSecureId() );
			}
		
		// overloading assignment operator
		inline void operator=(TLbtSecurityPolicy& aSecurityPolicy)
			{
			iOwnerSecureId = aSecurityPolicy.GetOwnerSecureId();
			}
		
		// returns a null security policy
		static inline TLbtSecurityPolicy Null()
			{
			TLbtSecurityPolicy security;
			return security;
			}
	
	private:
		// Secure id of the trigger owner
		TSecureId iOwnerSecureId;
	};

// Null security policy.
#define KLbtNullSecurity TLbtSecurityPolicy::Null()

#endif //LBTSECURITYPOLICY

// end of file

