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
* Description:  This header file describes the class that handles the storage
*                of triggers in RAM Structures.
*
*/


#ifndef C_LBTCONTAINER_RAMTRIGGERID_TREE_H
#define C_LBTCONTAINER_RAMTRIGGERID_TREE_H

// system includes
#include <e32base.h>
#include <e32cmn.h>
#include "lbtcontainertriggerentry.h"
#include "lbttriggerkey.h"






/**
 *  This class consists of the RAM structures that store the triggers.
 *
 *  It internally utilizes RAM memory and stores the trigger information
 *  in a efficient data structure.
 */
NONSHARABLE_CLASS( CLbtRamTriggerIdTree ) : public CBase
                                        
    {
public:
    /**
     * The Symbian 2 phase constructor.
     * This method creates a new instance of CLbtRamTriggersManager
     *
     * @return An instance of this class
     */
    static CLbtRamTriggerIdTree* NewL();

    /**
     * Destructor.
     */
    virtual ~CLbtRamTriggerIdTree();


	/**
     *
     */    
    void AddToTreeL(CLbtContainerTriggerEntry* aEntry );
    
    /**
     *
     */    
    void DeleteFromTreeL( TLbtTriggerId aKey );
    
    
    /**
     * Delete all the current entries in the tree and eset the tree.
     */    
    void ResetL();
    
    void SetIteratorL();
    
    /**
     * Checks if the tree is entry
     */
    TBool IsEmpty();
    
    CLbtContainerTriggerEntry* GetNextEntryL();
    
    CLbtContainerTriggerEntry* FindEntryInTreeL(TLbtTriggerId aKey );
    
protected: 
   

private:
    /**
     * C++ constructor
     */
    CLbtRamTriggerIdTree();

    /**
     * Symbian 2nd phase of construction
     */
    void ConstructL();

    /**
     * C++ Copy Constructor
     * The private copy constructor prevents the usage of copy constructors
     * with this class.
     */
    CLbtRamTriggerIdTree(const CLbtRamTriggerIdTree& aRamTriggersStore);
    
    /**
     * Overload equality operator
     * The private overload of the equality operator prevents the usage of
     * equality operator with this class.
     */
    CLbtRamTriggerIdTree& operator =(const CLbtRamTriggerIdTree& aRamTriggersStore);
    
    /**
     * Instance of TLbtTriggerKey to manage key values to access trigger 
     * information stored in the Btree
     */
    TLbtTriggerKey                                  iKey;
    
    /**
     * Allocates memory to be used by the BTree to store trigger information
     */
    CMemPagePool*                                   iPool;
    
    /**
     * An instance of TBtreeFix to contain the list of enabled and valid 
     * triggers in the system
     */
    TBtreeFix< CLbtContainerTriggerEntry*, TLbtTriggerId >*           iTriggerList;
    
    TBtreeMark iMark;
    TBool iNext;
    /**
     * To gain access to trigger information structure
     */
    friend class TLbtTriggerKey; 
    };


#endif // C_LBTCONTAINER_RAMTRIGGERID_TREE_H
