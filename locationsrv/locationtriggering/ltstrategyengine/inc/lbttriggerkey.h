/*
* Copyright (c) 2006, 2007 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  Class definition of Trigger Key
*
*/



#ifndef T_LBTTRIGGERKEY_H
#define T_LBTTRIGGERKEY_H


#include <s32btree.h>


/**
 * TTriggerKey is a class used to create and order keys to be used for entries 
 * in the Btree
 */
class TLbtTriggerKey : public MBtreeKey

    {
    
public:     // from base class MBtreeKey

    /**
     * Retrieves the key value for a particular entry
     */
    virtual const TAny* Key( const TAny* anEntry ) const;
    
    /**
     * Orders two keys 
     */
    virtual TInt Compare( const TAny *aLeft, const TAny *aRight ) const;
    
    /**
     * Determines the midpoint between two keys
     */
    virtual void Between( const TAny *aLeft, const TAny *aRight, 
                                            TBtreePivot &aPivot ) const;
    
    };

#endif  // T_LBTTRIGGERKEY_H
