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
* Description:  Common settings for LBT Source plugin
*
*/

#ifndef COMMON_H_
#define COMMON_H_

_LIT( KMyRequestor , "Requestor" );

_LIT(KDefaultValue,"0");

_LIT( KDefaultContextSourceName, "Location" );


_LIT(KContextType, "Location.Trigger");

// constant to define max length for trigger name & set name
// limit as specified by LBT engine
const TInt KMaxNameLength =256;
// defines length of real number
const TInt KRealWidth = 50;
// ascii value for point '.'
const TInt KRealPoint = 46;
// used for formatting the latitude & longitude
const TInt KCoordinateDecimalPlaces = 6;
// constants for checking the range of the coordinates
const TReal KMinLatitude = -90.0;
const TReal KMaxLatitude = 90.0;
const TReal KMinLongitude = -180.0;
const TReal KMaxLongitude = 180.0;
const TReal KMinRadius = 100.0;

// constant defining the permissible region value
_LIT( KRegionValue, "circle" );
_LIT( KDirectionValueOnEntry, "onentry" );
_LIT( KDirectionValueOnExit, "onexit" );
_LIT( KCurrentStateOn, "on" );
_LIT( KCurrentStateOff, "off" );




typedef struct 
 {
    TLbtTriggerId iTriggerId;
    TPtrC contextName;
    TPtrC triggerName;
 }TriggerList;
         
#endif /* COMMON_H_ */
