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
* Description:  NPP PSY internal Central Repository keys
*
*/


#ifndef NPPPSYINTERNALCRKEYS_H
#define NPPPSYINTERNALCRKEYS_H

/**
* @file npppsyinternalcrkeys.h
*
* Network Positioning Proxy Configuration API instructs the licensees on how
* to configure Network Positioning Proxy to have licensees' own PSY that 
* implements the network-based positioning technology to be used by Network 
* Positioning Proxy. 
* This API is a type of setting API which
* defines Central Repository UID and keys.
*
* @since S60 3.1
*/


/**
* Network Positioning Configuration API CenRep UID.
*/
const TUid KCRUidNppPsy = { 0x10206915 };

/**
* Hidden PSY list key.
*
* PSY list is a string which contains uids of hidden PSYs behind NPP PSY. For 
* example "12345678 87654321". Maximun length of this string is 128.
*
*/
const TUint32 KNppPsyPsyList = 0x10000001;

/*
* Time to Next Fix.
*
* Name of key which holds time to next fix.
* By default the value will be 180 sec.
*
*/
const TUint32 KPSYTimeToNextFix = 0x00000005;

/**
* Maximun length of PSY List string.
*/
const TInt KNppPsyPsyListMaxLength = 128;

/**
* PSY list frozen status.
*/
enum TNppPsyPsyListFrozenStatus
    {
    /**
    * PSY List is unfrozen. In this mode, if a hidden PSY fails to return a 
    * location estimation, the hidden PSY will be moved to the end of the 
    * list. The next hidden PSY on the PSY list will be tried. If all the 
    * hidden PSYs on the PSY list are tried, Network Positioning Proxy 
    * returns the information received from the last PSY to Location FW.
    */
    ENppPsyPsyListUnFrozen = 0,
    /**
    * PSY list is frozen with fallback. Network Positioning Proxy will not
    * modify the PSY list, but if the first PSY fails, there will be a fall
    * back of the Network Positioning Proxy to the next PSY (if available)
    * on the list, until a tried PSY gives a fix or all listed PSYs have 
    * been tried.
    */
    ENppPsyPsyListFrozenFallback = 1,
    /**
    * PSY list is frozen without fallback. Network Positioning Proxy will
    * not modify the PSY list, and it uses only the first PSY on the list. 
    * If the first PSY fails, the Network Positioning Proxy will not use 
    * any other PSYs.
    */
    ENppPsyPsyListFrozenNoFallback = 2
    };


#endif      // NPPPSYINTERNALCRKEYS_H

// End of file