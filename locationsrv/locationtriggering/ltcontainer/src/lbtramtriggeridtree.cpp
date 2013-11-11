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


#include "lbtramtriggeridtree.h"
#include "lbtlogger.h"


// ======== MEMBER FUNCTIONS ========

// ---------------------------------------------------------------------------
// The Symbian 2 phase constructor
// ---------------------------------------------------------------------------
//
CLbtRamTriggerIdTree* CLbtRamTriggerIdTree::NewL()
    {
    CLbtRamTriggerIdTree* self = new( ELeave ) CLbtRamTriggerIdTree;
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop( self );
    return self;
    }
    

// ---------------------------------------------------------------------------
// Destructor
// ---------------------------------------------------------------------------
//
CLbtRamTriggerIdTree::~CLbtRamTriggerIdTree()
    {
    FUNC_ENTER("CLbtRamTriggerIdTree::~CLbtRamTriggerIdTree");
    TRAP_IGNORE( ResetL() );
    delete iTriggerList;
    delete iPool;
    }

// ---------------------------------------------------------------------------
// CLbtRamTriggerIdTree::AddToTreeL
// ---------------------------------------------------------------------------
//
void CLbtRamTriggerIdTree::AddToTreeL(  CLbtContainerTriggerEntry* aEntry )
	{
	FUNC_ENTER("CLbtRamTriggerIdTree::AddToTreeL");
	TBtreePos pos;
    iTriggerList->InsertL( pos, aEntry );
 	}

// ---------------------------------------------------------------------------
// CLbtRamTriggerIdTree::FindEntryInTreeL
// ---------------------------------------------------------------------------
// 	
CLbtContainerTriggerEntry* CLbtRamTriggerIdTree::FindEntryInTreeL(TLbtTriggerId aKey )
    {
    FUNC_ENTER("CLbtRamTriggerIdTree::FindEntryInTreeL");
    TBtreePos pos;
    CLbtContainerTriggerEntry* tEntry = NULL;    
    
    TInt ret = iTriggerList->FindL(pos,aKey);
    if(ret)
        {        
        iTriggerList->ExtractAtL(pos, tEntry);        
        }
    return tEntry;
    }

// ---------------------------------------------------------------------------
// CLbtRamTriggerIdTree::DeleteFromTreeL
// ---------------------------------------------------------------------------
// 	
void  CLbtRamTriggerIdTree::DeleteFromTreeL( TLbtTriggerId aKey )
	{
	FUNC_ENTER("CLbtRamTriggerIdTree::DeleteFromTreeL");
    TBtreePos pos;
    TInt isPresent = iTriggerList->FindL(pos,aKey);
    if(!isPresent)
        {
        User::Leave(KErrNotFound);
        }
    CLbtContainerTriggerEntry* tEntry;
    iTriggerList->ExtractAtL(pos,tEntry);
    if(tEntry == NULL)
        {
        User::Leave(KErrNotFound);
        }
    
	iTriggerList->DeleteL(aKey);
	delete tEntry;
	}


// ---------------------------------------------------------------------------
// CLbtRamTriggerIdTree::ResetL
// ---------------------------------------------------------------------------
//
void CLbtRamTriggerIdTree::ResetL()
    {
    FUNC_ENTER("CLbtRamTriggerIdTree::ResetL");
    TBtreeMark index;
    if (  iTriggerList )
        {
        // Check if the TriggerList is valid. Otherwise don't execute this
        // operation.
        TBool next = iTriggerList->ResetL( index );
        while( next ) 
            {
            CLbtContainerTriggerEntry* tEntry=NULL;
            iTriggerList->ExtractAtL( index, tEntry );
            delete tEntry;
            next = iTriggerList->NextL( index );
            }
        }
    }

// ---------------------------------------------------------------------------
// CLbtRamTriggerIdTree::SetIteratorL
// ---------------------------------------------------------------------------
//
void CLbtRamTriggerIdTree::SetIteratorL()
    {
    FUNC_ENTER("CLbtRamTriggerIdTree::SetIteratorL");
    iNext=iTriggerList->ResetL(iMark);
    }

// ---------------------------------------------------------------------------
// CLbtRamTriggerIdTree::SetIteratorL
// ---------------------------------------------------------------------------
//
TBool CLbtRamTriggerIdTree::IsEmpty()
	{
	return iTriggerList->IsEmpty();
	}

// ---------------------------------------------------------------------------
// CLbtRamTriggerIdTree::GetNextEntryL
// ---------------------------------------------------------------------------
//
CLbtContainerTriggerEntry* CLbtRamTriggerIdTree::GetNextEntryL()
    {
    FUNC_ENTER("CLbtRamTriggerIdTree::GetNextEntryL");
    CLbtContainerTriggerEntry* tEntry=NULL;
    if(iNext)
        {
        iTriggerList->ExtractAtL( iMark, tEntry );
        iNext=iTriggerList->NextL( iMark );
        }
    return tEntry;        
    }
    
// ---------------------------------------------------------------------------
// Constructor
// ---------------------------------------------------------------------------
//
CLbtRamTriggerIdTree::CLbtRamTriggerIdTree()
    :iPool(NULL),
     iTriggerList(NULL)
    {
    }


// ---------------------------------------------------------------------------
// The 2nd phase Symbian Constructor
// ---------------------------------------------------------------------------
//
void CLbtRamTriggerIdTree::ConstructL()
    {
    FUNC_ENTER("CLbtRamTriggerIdTree::ConstructL");
    iTriggerList = new ( ELeave ) 
                            TBtreeFix< CLbtContainerTriggerEntry*, TLbtTriggerId >( EBtreeFast );
    iPool = CMemPagePool::NewL();
    iTriggerList->Connect( iPool, &iKey );
    }
