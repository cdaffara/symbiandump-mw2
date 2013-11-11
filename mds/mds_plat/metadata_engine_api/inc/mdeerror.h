/*
* Copyright (c) 2007-2009 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  Common error code definitions
*
*/


#ifndef MDEERROR_H
#define MDEERROR_H

#include <e32def.h>

/**
 * Canceled or commited object is not locked.
 */
const TInt KErrMdENotLocked = -36200;

/**
 * Property's type is different than expected.
 */
const TInt KErrMdEIncorrectType = -36201;

/**
 * Mandatory property is missing from object.
 */
const TInt KErrMdEMandatoryPropertyMissing = -36202;

/**
 * Property's value is not between allowed minimum and maximum value.
 */
const TInt KErrMdEPropertyValueNotBetweenAllowedMinMax = -36203;

/**
 * First MdE's internal error code.
 * 
 * All MdE's internal error codes (defined in mdeinternalerror.h) are be 
 * between KErrMdEFirstInternalError and KErrMdELastInternalError.
 */
const TInt KErrMdEFirstInternalError = -36240;

/**
 * Last MdE's internal error code.
 * 
 * All MdE's internal error codes (defined in mdeinternalerror.h) are be 
 * between KErrMdEFirstInternalError and KErrMdELastInternalError.
 */
const TInt KErrMdELastInternalError  = -36299;

#endif /*MDEERROR_H_*/
