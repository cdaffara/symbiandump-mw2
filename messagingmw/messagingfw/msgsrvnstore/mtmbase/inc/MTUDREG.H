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
//

#if !defined __MTUDREG_H__
#define __MTUDREG_H__

#if !defined (__MTCLREG_H__)
#include <mtclreg.h>
#endif

class CBaseMtmUiData;

/** UI Data MTM factory function.

A concrete UI Data MTM must implement an exported factory function of this 
type. It should return an instance of the CBaseMtmUiData-derived class that 
provides the implementation. The factory function is called by the UI Data 
MTM registry when a client requests this UI Data MTM. 

A CBaseMtmUiData-derived class typically provides a NewL() function, which 
the factory function calls. 

The factory function is called by ordinal. The ordinal of the function must 
match that recorded in the MTM's registry information. 

The CRegisteredMtmDll& argument passes the registration data for the MTM DLL.

The return value is a newly-created instance of the CBaseMtmUi-derived class 
for the User Interface MTM.

The factory function should leave if it cannot create the object. 

For example, a UI Data MTM whose concrete class was CEgMtmUiData could define 
a suitable factory function as: 

@code
EXPORT_C CBaseMtmUiData* NewEgMtmL(CRegisteredMtmDll& aRegisteredMtmDll)
	{
	return CEgMtmUiData::NewL(aRegisteredMtmDll);
	}
@endcode */
typedef CBaseMtmUiData* MtmUiDataFactoryFunctionL(CRegisteredMtmDll&);

// Client side MTM UI data registry

class CMtmUiDataRegistry : public CObserverRegistry
/** Accesses the UI Data MTM registry. 

This registry holds details of the all the 
UI Data MTMs currently available on the system. Message client applications 
use this class to get a CBaseMtmUiData-derived object by which to access UI 
Data MTM functionality.

Note that the base class CMtmDllRegistry provides functions for discovering 
what MTMs are present in the registry. 
@publishedAll
@released
*/
	{
public:
	IMPORT_C static CMtmUiDataRegistry* NewL(CMsvSession& aMsvSession,
		TTimeIntervalMicroSeconds32 aTimeoutMicroSeconds32=TTimeIntervalMicroSeconds32(30000000));
	virtual ~CMtmUiDataRegistry();
	IMPORT_C CBaseMtmUiData* NewMtmUiDataLayerL(const TUid& aMtmTypeUid);
	//
protected:
	CMtmUiDataRegistry(CMsvSession& aMsvSession, TTimeIntervalMicroSeconds32 aTimeoutMicroSeconds32);
	//
private:
	CBaseMtmUiData* NewMtmL(const RLibrary& aLib, CRegisteredMtmDll& aReg) const;
	};



#endif // __MTUDREG_H__
