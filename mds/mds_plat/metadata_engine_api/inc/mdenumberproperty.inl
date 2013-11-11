/*
* Copyright (c) 2008-2009 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  CMdENumberPropertys' compatible wrapper to CMdEProperty
*
*/


#include <mdepropertydef.h>

#define MDE_DEFINE_NUMBER_PROPERTY(PropertyType) \
template<> inline T##PropertyType CMdENumberProperty< T##PropertyType >::Value() const \
    { \
    T##PropertyType value = NULL; \
    TRAPD( err, value = PropertyType##ValueL() ); \
	if( err != KErrNone ) \
		{ \
		_LIT( KCMdEPropertyGetValuePanic, "CMdE##PropertyType##Property::Value" ); \
		User::Panic( KCMdEPropertyGetValuePanic, KErrNotSupported ); \
		} \
    return value; \
    } \
    template<> inline void CMdENumberProperty<T##PropertyType>::SetValueL(T##PropertyType aValue) \
    { \
    TRAPD( err, Set##PropertyType##ValueL( aValue ) ); \
	if( err != KErrNone ) \
		{ \
		_LIT( KCMdEPropertySetValuePanic, "CMdE##PropertyType##Property::SetValueL" ); \
		User::Panic( KCMdEPropertySetValuePanic, KErrNotSupported ); \
		} \
    }

MDE_DEFINE_NUMBER_PROPERTY(Bool)
MDE_DEFINE_NUMBER_PROPERTY(Int8)
MDE_DEFINE_NUMBER_PROPERTY(Uint8)
MDE_DEFINE_NUMBER_PROPERTY(Int16)
MDE_DEFINE_NUMBER_PROPERTY(Uint16)
MDE_DEFINE_NUMBER_PROPERTY(Int32)
MDE_DEFINE_NUMBER_PROPERTY(Uint32)
MDE_DEFINE_NUMBER_PROPERTY(Int64)
MDE_DEFINE_NUMBER_PROPERTY(Real32)
MDE_DEFINE_NUMBER_PROPERTY(Real64)
MDE_DEFINE_NUMBER_PROPERTY(Time)

#undef MDE_DEFINE_NUMBER_PROPERTY
