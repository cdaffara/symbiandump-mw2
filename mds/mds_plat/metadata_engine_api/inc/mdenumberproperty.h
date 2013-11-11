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

#ifndef __MDENUMBERPROPERTY_H__
#define __MDENUMBERPROPERTY_H__

#include <mdeproperty.h>

/**
 * Template for number property compatibility wrappers to property.
 */
template< class T >
class CMdENumberProperty : public CMdEProperty
	{
public:
	/**
	 * Returns the value of the property.
	 *
	 * @return  value
	 */
	T Value() const;

	/**
	 * Sets the value of the property.
	 *
	 * @param aValue       New value.
	 */
	void SetValueL(T aValue);
	};

/* Typedefs for numeric properties. */
typedef CMdENumberProperty<TBool>   CMdEBoolProperty;
typedef CMdENumberProperty<TInt8>   CMdEInt8Property;
typedef CMdENumberProperty<TUint8>  CMdEUint8Property;
typedef CMdENumberProperty<TInt16>  CMdEInt16Property;
typedef CMdENumberProperty<TUint16> CMdEUint16Property;
typedef CMdENumberProperty<TInt32>  CMdEInt32Property;
typedef CMdENumberProperty<TUint32> CMdEUint32Property;
typedef CMdENumberProperty<TInt64>  CMdEInt64Property;
typedef CMdENumberProperty<TReal32> CMdEReal32Property;
typedef CMdENumberProperty<TReal64> CMdEReal64Property;
typedef CMdENumberProperty<TTime>   CMdETimeProperty;


#include <mdenumberproperty.inl>

#endif  // __MDENUMBERPROPERTY_H__
