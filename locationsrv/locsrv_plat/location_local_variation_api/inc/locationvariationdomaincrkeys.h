/*
* Copyright (c) 2006 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  Central Repository keys for Location Local Variation.
*                These keys defines the local variation in location domain
*
*/


#ifndef LOCATIONVARIATIONDOMAINCRKEYS_H
#define LOCATIONVARIATIONDOMAINCRKEYS_H

/**
 * UID value for the Central Repository Key used for Location Local Variation.
 */
const TInt KCRUidLocLocalVariation = 0x20022D83;

/**
 * Central repository Key identifer value for Hiding the Coordinates in the location applications.
 * This is a binary value
 */
const TInt KLocHideCoordinates = 0x00000001;

/**
 * Central repository Key identifer value for Hiding the Help in the location applications.
 * This is a binary value
 */
const TInt KLocHideHelp = 0x00000002;

/**
 * Enumeration list of values for KLocHideCoordinates
 * 
 * Defines the Hiding behaviour of the Location coordinates.
 */
enum TLocHideCoordinatesValues
    {
    /**
     * This value indicates that the coordinates in the location applications
	 * should NOT be hidden.
     */  
    ELocCoordinatesNotHidden    = 0x00000000,
	
    /**
     * This value indicates that the coordinates in the location applications
	 * should be hidden.
     */
    ELocCoordinatesHidden      = 0x00000001
    };

/**
 * Enumeration list of values for KLocHideHelp
 * 
 * Defines the Hiding behaviour of the help in Location applications.
 */
enum TLocHideHelpValues
    {
    /**
     * This value indicates that the help in the location applications
	 * should NOT be hidden.
     */  
    ELocHelpNotHidden    = 0x00000000,
	
    /**
     * This value indicates that the help in the location applications
	 * should be hidden.
     */
    ELocHelpHidden      = 0x00000001
    };
#endif      // LOCATIONVARIATIONDOMAINCRKEYS_H