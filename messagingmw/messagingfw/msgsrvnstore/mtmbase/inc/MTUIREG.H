// Copyright (c) 1998-2009 Nokia Corporation and/or its subsidiary(-ies).
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
// MTM client-side MTMUI registry class 
// 
//

#if !defined __MTUIREG_H__
#define __MTUIREG_H__


#if !defined (__E32BASE_H__)
#include <e32base.h>		// CActive, TFileName etc.
#endif

#if !defined (__MTCLREG_H__)
#include <mtclreg.h>
#endif

class CBaseMtmUi;

/** UI MTM factory function.

A concrete User Interface MTM must implement an exported factory function of 
this type. It should return an instance of the CBaseMtmUi-derived class that 
provides the implementation. The factory function is called by the User Interface 
MTM registry when a client requests this User Interface MTM.

A CBaseMtmUi-derived class typically provides a NewL() function, which the 
factory function calls.

The factory function is called by ordinal. The ordinal of the function must 
match that recorded in the MTM's registry information. 

For example, a User Interface MTM whose concrete class was CEgMtmUi could 
define a suitable factory function as:

@code
EXPORT_C CBaseMtmUi* NewEgMtmUiL(CBaseMtm& aMtm, CRegisteredMtmDll& aRegisteredDll)
	{
	return CEgMtmUi::NewL(aMtm, aRegisteredDll);
	}@endcode

@param aBaseMtm The CBaseMtm requesting the object.
@param aRegisteredMtmDll Registration data for the MTM DLL.
@return A newly-created instance of the CBaseMtmUi-derived class 
for the User Interface MTM.
@leave Any The factory function should leave if it cannot create the object. */
typedef CBaseMtmUi* MtmUiFactoryFunctionL(CBaseMtm&, CRegisteredMtmDll&);

// Client side MTM registry

class CMtmUiRegistry : public CObserverRegistry
/** Accesses the User Interface MTM registry. 

This registry holds details of the 
all the User Interface MTMs currently available on the system. Message client 
applications use this class to get a CBaseMtmUi-derived object by which to 
access User Interface MTM functionality.

Note that the base class CMtmDllRegistry provides functions for discovering 
what MTMs are present in the registry. 
@publishedAll
@released
*/
	{
public:
	IMPORT_C static CMtmUiRegistry* NewL(CMsvSession& aMsvSession,
		TTimeIntervalMicroSeconds32 aTimeoutMicroSeconds32=TTimeIntervalMicroSeconds32(30000000));
	virtual ~CMtmUiRegistry();
	IMPORT_C CBaseMtmUi* NewMtmUiL(CBaseMtm& aMtm);
	//
protected:
	CMtmUiRegistry(CMsvSession& aMsvSession, TTimeIntervalMicroSeconds32 aTimeoutMicroSeconds32);
	//
private:
	CBaseMtmUi* NewMtmL(const RLibrary& aLib, CBaseMtm& aMtm, CRegisteredMtmDll& aReg) const;
	};



#endif // __MTUIREG_H__
