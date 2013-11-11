/*
* Copyright (c) 2005 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:   XDM errors
*
*/




#ifndef __XDMERRORS__
#define __XDMERRORS__


// Error range -32400 -> -32429 ( first and last number included)

#define XDM_ERROR_BASE -32400

enum
    {
    // General error
    KXcapErrorGeneral                           = XDM_ERROR_BASE,
    
/* Errors from the XCAP Application Usages */
    
    // Invalid element or attribute data value
    KXcapErrorIllegalDataValue                  = XDM_ERROR_BASE -1,
    
    // Mandatory element missing from document
    KXcapErrorMissingRequiredElement            = XDM_ERROR_BASE -2,
    
    // Mandatory attribute missing from element
    KXcapErrorMissingRequiredAttribute          = XDM_ERROR_BASE -3,
    
    // Restricted element occurence rule 
    KXcapErrorTooManyOccurencies                = XDM_ERROR_BASE -4,
    
    // Illegal elements, wrong element order, etc
    KXcapErrorSchemaViolation                   = XDM_ERROR_BASE -5,
    
    // Element or attribute value against enumerated values
    KXcapErrorEnumerationRule                   = XDM_ERROR_BASE -6,
    
    // Illegal root element in document
    KXcapErrorIllegalRootElement                = XDM_ERROR_BASE -7,
    
    // Mandatory value missing from element
    KXcapErrorMissingRequiredElementValue       = XDM_ERROR_BASE -8,
    
/* Errors from the XCAP Protocol */
    
    // Could not create a network connection
    KXcapErrorNetworkNotAvailabe                = XDM_ERROR_BASE -9,
    
    // General HTTP Server Error - 5xx
    KXcapErrorHttpServer                        = XDM_ERROR_BASE -10,
    
    // General HTTP Client Error - 4xx
    KXcapErrorHttpClient                        = XDM_ERROR_BASE -11,
    
    // Bad request - 400
    KXcapErrorHttpBadRequest                    = XDM_ERROR_BASE -12,
    
    // The client was not authoried to use the resource - 401
    KXcapErrorHttpUnauthorised                  = XDM_ERROR_BASE -13,
    
    // Use of the resource has been forbidden - 403
    KXcapErrorHttpForbidden                     = XDM_ERROR_BASE -14,
    
    // The requested resource could not be found - 404
    KXcapErrorHttpNotFound                      = XDM_ERROR_BASE -15,
    
    // The request timed out - 408
    KXcapErrorHttpRequestTimeout                = XDM_ERROR_BASE -16,
    
    // An HTTP conflict - 409
    KXcapErrorHttpConflict                      = XDM_ERROR_BASE -17,
    
    // Precondition failed - 412
    KXcapErrorHttpPrecondFailed                 = XDM_ERROR_BASE -18,
    
    // An unrecoverable conflict 
    KXcapErrorUnrecoverableConflict             = XDM_ERROR_BASE -19,
    
    // Authentication || user data integrity problems 
    KXcapErrorAuthentication                    = XDM_ERROR_BASE -20
    };



#endif      //__XDMERRORS__
            
// End of File
