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



inline TPtrC CMtmDllInfo::HumanReadableName() const
/** Gets the descriptive name of the MTM component for which the object holds registration 
data.

@return Descriptor holding the descriptive name of the MTM component */
	{
	return iHumanReadableName->Des();
	}

inline TUid CMtmGroupData::MtmTypeUid() const
/** Gets the group UID.

@return UID that uniquely identifies the MTM group */
	{
	return iMtmTypeUid;
	}

inline TUid CMtmGroupData::TechnologyTypeUid() const
/** Gets the technology UID.

Technology types can be used to conceptually group different MTMs (e.g. as 
providing Internet email).

@return UID that indicates the messaging technology that the MTM group implements */
	{
	return iTechnologyTypeUid;
	}


inline TUid CRegisteredMtmDll::MtmTypeUid() const
/** Gets the MTM type UID.

@return MTM type UID */
	{
	return iMtmTypeUid;
	}

inline TUid CRegisteredMtmDll::TechnologyTypeUid() const
/** Gets the technology type UID.

@return UID that indicates the messaging technology that the MTM group implements */
	{
	return iTechnologyTypeUid;
	}

inline const CMtmDllInfo& CRegisteredMtmDll::MtmDllInfo() const
/** Gets the full registration data for the MTM.

@return Object encapsulating full registration data for the MTM */
	{
	return *iMtmDllInfo;
	}

inline TInt CRegisteredMtmDll::MtmDllRefCount() const
/** Gets the number of MTM objects instantiated from the registry.

@return Number of current instances of the MTM */
	{
	return iMtmDllRefCount;
	}

inline TInt CMtmDllRegistry::NumRegisteredMtmDlls() const
/** Gets the number of registered MTMs.

@return Number of registered MTMs */
	{
	return iRegisteredMtmDllArray.Count();
	}

inline TBool CMtmDllRegistry::IsPresent(TUid aMtmTypeUid) const
/** Tests if the MTM with the specified UID is registered.

@param aMtmTypeUid UID of MTM to check 
@return ETrue if the specified MTM in registered, else EFalse */
	{
	return MtmTypeUidToIndex(aMtmTypeUid)<iRegisteredMtmDllArray.Count();
	}

