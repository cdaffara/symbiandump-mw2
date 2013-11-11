/*
* Copyright (c) 2002 Nokia Corporation and/or its subsidiary(-ies).
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
* Description: CAlwaysOnlineEComInterface
*
*/


#ifndef __AlwaysOnlineEComInterface_H__
#define __AlwaysOnlineEComInterface_H__

#include <e32base.h>
#include <msvapi.h>

#include <ecom/ecom.h>

class MAlwaysOnlineStatusQueryInterface;

/**
	@public
	@class CAlwaysOnlineEComInterface
	@since 7.0
	@dependencies REComSession, CActive
	An AlwaysOnlineManager abstract class being representative of the
	concrete class which the client wishes to use.
	It acts as a base, for a real class to provide all the 
	functionality that a client requires.  
	It supplies instantiation & destruction by using
	the ECom framework, and functional services
	by using the methods of the actual class.
 */
class CAlwaysOnlineEComInterface  : public CBase
{
public:
	// The interface for passing initialisation parameters
	// to the derived class constructor.
	typedef struct _CEComInterfaceInitParams
		{
		TInt integer;
		const TDesC* descriptor;
		} TAlwaysOnlineManagerInterfaceInitParams;

	//The function which instantiates an object of this type 
	//(eg NewL for a CBase derived object).
	static CAlwaysOnlineEComInterface* NewL( TUid aId );

	//The function which instantiates an object of this type 
	// using the MatchString as the resolver parameters.
	//(eg NewL for a CBase derived object).
	static CAlwaysOnlineEComInterface* CAlwaysOnlineEComInterface::NewL(const TDesC8& aMatchString);

	//Standardised destructor.
	inline virtual ~CAlwaysOnlineEComInterface();

	//Request a list of all available implementations which 
	//satisfy this given interface.
	static void ListImplementationsL(RImplInfoPtrArray& aImplInfoArray);
	static void ListAllImplementationsL(RImplInfoPtrArray& aImplInfoArray);

	
    //Pure interface method
    //Commands from alwaysonlinemanager to plugin
    //OPEN: is return value really needed?
    virtual TAny* HandleServerCommandL( TInt aCommand, TDesC8* aParameters = NULL ) = 0;

    inline void SetStatusQueryObject( MAlwaysOnlineStatusQueryInterface* aStatusQueryObject );
    
    inline TUid InstanceUid();

protected:
	//Default c'tor
	inline CAlwaysOnlineEComInterface();

//Attributes
private:
	TUid iDtor_ID_Key;

protected:

    MAlwaysOnlineStatusQueryInterface* iStatusQueryObject;

	// Declare the Test bed state accessor as a friend
	friend class TCEComInterface_StateAccessor;

};

#include <AlwaysOnlineEComInterface.inl>

#endif /* __CAlwaysOnlineEComInterface_H__ */

//EOF
