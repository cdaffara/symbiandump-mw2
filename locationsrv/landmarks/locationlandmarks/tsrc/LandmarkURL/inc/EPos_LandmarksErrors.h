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
* Description:    Defines landmarks panic codes
*
*/





#ifndef LANDMARKSERRORS_H
#define LANDMARKSERRORS_H

// INCLUDES
#include <e32base.h>
#include <e32def.h>

// CONSTANTS
_LIT(KPosLandmarksClientPanic,"Landmarks Client");

/**
 * Landmarks panic codes
 *
 */
enum TPositionLandmarksClientPanic
    {
    EPosInvalidPositionFieldId                  = 0
    /** A client has specified a position field ID that is invalid for
        the landmark. */,
    EPosNoneOrMultipleLandmarkAttributeSet      = 1
    /** A client has specified none or multiple landmark attributes. */,
    EPosSpecifiedIntervalLiesOutsideIteratedSet = 2
    /** A client has specified an interval that lies partially outside the
        iterated set. */,
    EPosNaNCoordinate                           = 3
    /** A client has specified a coordinate with latitude and/or longitude set
        to NaN. */,
    EPosInvalidLandmarkAttribute                = 4
    /** A client has specified a landmark attribute that is invalid. */,
    EPosInvalidValueSpecifiedInResourceFile     = 5
    /** An invalid value has been detected in a resource file. */,
    EPosInvalidPartialReadParameters            = 6
    /** Invalid partial read parameters have been detected. */,
    EPosInvalidRequestedPositionFields          = 7
    /** Invalid requested position fields have been detected. */,
    EPosNegativeValue                           = 8
    /** A negative value has been detected. */,
    EPosInvalidOperationMode                    = 9
    /** Invalid operation mode. Caused by mixed calls to NextStep and ExecuteL
        for an CPosLmOperation object or subsequent calls to NextStep. */,
    EPosInvalidEnumValue                        = 10
    /** Invalid enum value. */,
    EPosLmProtocolBreak                         = 11
    /** The protocol of CPosLandmarkEncoder/CPosLandmarkParser is not
        followed. */,
    EPosLmInvalidArgument                       = 12
    /** A client has passed an invalid argument. */,
    EPosInvalidIndex                            = 14
    /** A client has specified an invalid index. */,
    EPosInvalidItemType                         = 15
    /** A client has specified an invalid item type. */,
    EPosSearchOperationInUse                    = 16
    /** A client has tried to set/unset display data during an ongoing search.
        */
    };

// FUNCTION PROTOTYPES
IMPORT_C GLDEF_C void Panic(const TDesC& aCategory, TInt aReason);

#endif      // LANDMARKSERRORS_H

// End of File
