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


#include "lbtramtriggerclientsidtree.h"




// ======== MEMBER FUNCTIONS ========

// ---------------------------------------------------------------------------
// The Symbian 2 phase constructor
// ---------------------------------------------------------------------------
//
CLbtRamTriggersClientSIDTree* CLbtRamTriggersClientSIDTree::NewL()
    {
    CLbtRamTriggersClientSIDTree* self = new( ELeave ) CLbtRamTriggersClientSIDTree;
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop( self );
    return self;
    }
    

// ---------------------------------------------------------------------------
// Destructor
// ---------------------------------------------------------------------------
//
CLbtRamTriggersClientSIDTree::~CLbtRamTriggersClientSIDTree()
    {
    TRAPD(err, ResetL());
    if( KErrNone != err )
        {
        // TODO: Log warning message that entries could not be deleted
        }
    delete iTriggerList;
    delete iPool;
    }

// ---------------------------------------------------------------------------
// Add an entry to the tree
// ---------------------------------------------------------------------------
//
void CLbtRamTriggersClientSIDTree::AddToTreeL(TTriggerTreeBasedOnSID* aEntry )
	{
	TBtreePos pos;
    iTriggerList->InsertL( pos, aEntry );
	}


// ---------------------------------------------------------------------------
// Update an entry in the tree
// ---------------------------------------------------------------------------
//

void CLbtRamTriggersClientSIDTree::UpdateEntry( TSecureId aSid,CLbtContainerTriggerEntry* aEntry)
{
	TBtreePos pos;
	TRAP_IGNORE( iTriggerList->FindL(pos,aSid); );
	TTriggerTreeBasedOnSID* tEntry=NULL;
	TRAP_IGNORE( iTriggerList->ExtractAtL( pos, tEntry ) );
	tEntry->trigArray.Append(aEntry);
}


// ---------------------------------------------------------------------------
// ResetL
// ---------------------------------------------------------------------------
//
void CLbtRamTriggersClientSIDTree::ResetL()
    {
    if (  iTriggerList )
        {
        // Check if the TriggerList is valid. Otherwise don't execute this
        // operation.
        TBtreeMark index;
        TBool next = iTriggerList->ResetL( index );
    
        while( next )
            {
            TTriggerTreeBasedOnSID* tEntry=NULL;
            iTriggerList->ExtractAtL( index, tEntry );
            delete tEntry;
            next = iTriggerList->NextL( index );
            }
        }
    }

// ---------------------------------------------------------------------------
// IsEntryPresent
// ---------------------------------------------------------------------------
//
TBool CLbtRamTriggersClientSIDTree::IsEntryPresent(TSecureId aSid)
{
	TBtreePos pos;
	TBool present = EFalse;
	TRAP_IGNORE( present = iTriggerList->FindL(pos,aSid) );
	return present;
}

void CLbtRamTriggersClientSIDTree::SetIteratorL()
    {
    iNext=iTriggerList->ResetL(iMark);
    }

CLbtRamTriggersClientSIDTree::TTriggerTreeBasedOnSID* CLbtRamTriggersClientSIDTree::GetNextEntryL()
    {
    TTriggerTreeBasedOnSID* tEntry=NULL;
    if(iNext)
        {
        iTriggerList->ExtractAtL( iMark, tEntry );
        iNext=iTriggerList->NextL( iMark );
        }
    return tEntry;    
        
    }
CLbtRamTriggersClientSIDTree::TTriggerTreeBasedOnSID* CLbtRamTriggersClientSIDTree::FindEntry(TSecureId aSid)
    {
    TBtreePos pos;
    
    TTriggerTreeBasedOnSID* tEntry=NULL;
    
    TBool find ;
    
    TRAPD(err,find = iTriggerList->FindL(pos,aSid) );
    if(err != KErrNone)
     return NULL;
    
    if(find)
    	{
    	TRAP(err,iTriggerList->ExtractAtL( pos, tEntry ));
	    if(err != KErrNone)
	    	{
	    	return NULL;
	    	}
	    		
    	}
	    
	    
	    
    return tEntry;
    
    }
void  CLbtRamTriggersClientSIDTree::DeleteFromTreeL( TSecureId aSid )
	{
	
    
    iTriggerList->DeleteL(aSid);
	
	}    
	
void  CLbtRamTriggersClientSIDTree::RemoveTriggerFromSIDTreeL( TSecureId aSid , TLbtTriggerId aId)
    {
    TBtreePos pos;
    iTriggerList->FindL(pos,aSid);
    TTriggerTreeBasedOnSID* tEntry=NULL;
    iTriggerList->ExtractAtL( pos, tEntry );
    for(TInt j=tEntry->trigArray.Count()-1;j>=0;j--)
        {
         /* Container Trigger Entry stored in the tree */
                    
            CLbtContainerTriggerEntry* contEntry=tEntry->trigArray[j];
            
            /* Trigger Entry in Container Trigger Entry */
            
            CLbtTriggerEntry* trigEntry=contEntry->TriggerEntry();
            
            if(trigEntry->Id()==aId)
                {
                tEntry->trigArray.Remove(j);
                }
            
        }
        
    if(tEntry->trigArray.Count() == 0)
        {
        DeleteFromTreeL(aSid);
        }
    }
// ---------------------------------------------------------------------------
// Constructor
// ---------------------------------------------------------------------------
//
CLbtRamTriggersClientSIDTree::CLbtRamTriggersClientSIDTree()
    :iPool(NULL),
     iTriggerList(NULL)
    {
    // Nothing to do now
    }


// ---------------------------------------------------------------------------
// The 2nd phase Symbian Constructor
// ---------------------------------------------------------------------------
//
void CLbtRamTriggersClientSIDTree::ConstructL()
    {
    iTriggerList = new ( ELeave ) 
                            TBtreeFix< TTriggerTreeBasedOnSID*, TSecureId >( EBtreeFast );
    iPool = CMemPagePool::NewL();
    iTriggerList->Connect( iPool, &iKey );
    
     
    }
