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
* Description:  CMdETextProperty's compatible wrapper to CMdEProperty
*
*/


#ifndef __MDETEXTPROPERTY_H__
#define __MDETEXTPROPERTY_H__

#include <mdeproperty.h>

/**
 * Text property compatibility wrapper to property.
 */
NONSHARABLE_CLASS(CMdETextProperty) : public CMdEProperty
	{
public:
	/**
	 * Returns the text value of the property.
	 *
	 * @return  value
	 */
	const TDesC& Value() const;

	/**
	 * Sets the text value of the property.
	 *
	 * @param aValue       New value.
	 */
	void SetValueL(const TDesC& aValue);
	};

#include <mdetextproperty.inl>
	
#endif  // __MDETEXTPROPERTY_H__

