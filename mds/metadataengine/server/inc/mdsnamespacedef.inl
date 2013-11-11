/*
* Copyright (c) 2009 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  Class to hold description about one namespace and every objects,
*                properties and events it holds.
*
*/


inline CMdsNamespaceDef::CMdsNamespaceDef( TDefId aId, TBool aReadOnly, TUint32 aVendorId )
   : iVendorId(aVendorId), iFlags(ENamespaceDefFlagsFirstRead)
   	{
   	SetId( aId );
   	if (aReadOnly)
   		{
   		iFlags |= ENamespaceDefFlagsReadOnly;
   		}
   	}

inline TBool CMdsNamespaceDef::GetReadOnly() const
	{
	return iFlags&ENamespaceDefFlagsReadOnly ? ETrue : EFalse;
	}

inline TBool CMdsNamespaceDef::GetFirstRead() const
	{
	return iFlags&ENamespaceDefFlagsFirstRead ? ETrue : EFalse;
	}

inline void CMdsNamespaceDef::UnsetFirstRead()
	{
	iFlags &= ~ENamespaceDefFlagsFirstRead;
	}

inline void CMdsNamespaceDef::SetBaseObject( CMdsObjectDef* const aBaseObject )
	{
	iBaseObject = aBaseObject;
	}

inline TInt32 CMdsNamespaceDef::GetVendorId() const
	{
	return iVendorId;
	}

inline void CMdsNamespaceDef::SetVendorId(TInt32 aVendorId)
	{
	iVendorId = aVendorId;
	}
