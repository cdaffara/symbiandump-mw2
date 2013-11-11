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
* Description:    RLS and Presence XDM, This class represents Presence Action
*
*/




// Includes
#include <utf.h>
#include <XdmDocumentNode.h>
#include <XdmNodeAttribute.h>
#include <xcapappusagedef.h>

#include "cpresencexdm.h"
#include "presenceactionxdm.h"
#include "rlspresxdmlogger.h"
#include "rlspresxdmconstsint.h"

// ---------------------------------------------------------------------------
// TPresenceActionXDM::TPresenceActionXDM()
// ---------------------------------------------------------------------------
//
EXPORT_C TPresenceActionXDM::TPresenceActionXDM():iPresXDM(NULL)
    {
    OPENG_DP(D_OPENG_LIT( "TPresenceActionXDM::TPresenceActionXDM()" ) );
    }

// ---------------------------------------------------------------------------
// TPresenceActionXDM::Init()
// ---------------------------------------------------------------------------
//
EXPORT_C void TPresenceActionXDM::Init(CPresenceXDM* const aPresXDM)
    {
    OPENG_DP(D_OPENG_LIT( "TPresenceActionXDM::Init(aPresXDM=%d)"),aPresXDM );
    iPresXDM = aPresXDM;
    }

// ---------------------------------------------------------------------------
// TPresenceActionXDM::AddOrReplaceAction()
// ---------------------------------------------------------------------------
//
EXPORT_C TInt TPresenceActionXDM::AddOrReplaceActionL(const TDesC& aRuleID, 
                                                        const TDesC& aAction)
	{
    OPENG_DP(D_OPENG_LIT( "TPresenceActionXDM::AddOrReplaceActionL()" ) );
    OPENG_DP(D_OPENG_LIT( "     aRuleID = %S, aAction = %S" ),&aRuleID, &aAction);

    TInt err(KErrNone);

    CXdmDocumentNode* subHandlingNode = GetSubHandlingNodeL(aRuleID, ETrue);
    if(subHandlingNode!=NULL)
        {
        if(IsValidAction(aAction))
            {
            subHandlingNode->SetLeafNode(ETrue);
            subHandlingNode->SetLeafNodeContentL(aAction);
            err = KErrNone;			
            }
        else
            err = KErrArgument;		
        }
    else
		err = KErrNotFound;
	
    OPENG_DP(D_OPENG_LIT( "     return: %d" ),err);
    return err;
    }

// ---------------------------------------------------------------------------
// TPresenceActionXDM::RemoveAction()
// ---------------------------------------------------------------------------
//                                                                
EXPORT_C TInt TPresenceActionXDM::RemoveActionL(const TDesC& aRuleID)
	{
    OPENG_DP(D_OPENG_LIT( "TPresenceActionXDM::RemoveActionL()" ) );
    OPENG_DP(D_OPENG_LIT( "     aRuleID = %S" ),&aRuleID );
    __ASSERT_ALWAYS(iPresXDM, User::Leave(KErrNotReady));

    TInt err(KErrNone);
    CXdmDocumentNode* subHandlingNode = GetSubHandlingNodeL(aRuleID, EFalse);
    if(subHandlingNode!=NULL)
        iPresXDM->RemoveFromModelL(subHandlingNode);
    else
        err = KErrNotFound;

    OPENG_DP(D_OPENG_LIT( "     return: %d" ),err);
    return err;
	}

// ---------------------------------------------------------------------------
// TPresenceActionXDM::GetAction()
// ---------------------------------------------------------------------------
//         
EXPORT_C TInt TPresenceActionXDM::GetActionL(const TDesC& aRuleID, TDes& aAction)
	{
    OPENG_DP(D_OPENG_LIT( "TPresenceActionXDM::GetActionL()" ) );
    OPENG_DP(D_OPENG_LIT( "		aRuleID = %S" ),&aRuleID );
	
    TInt err(KErrNone);
	
	CXdmDocumentNode* subHandlingNode = GetSubHandlingNodeL(aRuleID, EFalse);
	if(subHandlingNode==NULL)
    	{
        OPENG_DP(D_OPENG_LIT( "     return: KErrNotFound" ));
    	return KErrNotFound;	
    	}
	    
	HBufC* nodeContents(NULL);

    subHandlingNode->SetLeafNode(ETrue);
    nodeContents = CnvUtfConverter::ConvertToUnicodeFromUtf8L
                                            (subHandlingNode->LeafNodeContent());
    //Check given string length
    if((nodeContents->Des()).Length() > aAction.MaxLength())
        err = KErrArgument;
    else
    aAction = nodeContents->Des();
	
	delete nodeContents;

    OPENG_DP(D_OPENG_LIT( "     return: err" ));
    return err;
	}

// ---------------------------------------------------------------------------
// TPresenceActionXDM::GetSubHandlingNode()
// ---------------------------------------------------------------------------
//    
CXdmDocumentNode* TPresenceActionXDM::GetSubHandlingNodeL(const TDesC& aRuleID,  
                                                                TBool aCreate)
    {
    OPENG_DP(D_OPENG_LIT( " TPresenceActionXDM::GetSubHandlingNodeL()" ) );
    OPENG_DP(D_OPENG_LIT( "		GetSubHandlingNodeL aRuleID = %S, aCreate = %d" ),
                                                            &aRuleID,aCreate);
                                                            
    __ASSERT_ALWAYS(iPresXDM, User::Leave(KErrNotReady));
    
    CXdmDocumentNode* actionNode = iPresXDM->GetRuleChildNodeL(aRuleID, 
                                                        KXdmActions, aCreate);
    if ( (!aCreate) && (actionNode==NULL))
        return NULL;

    RPointerArray<CXdmDocumentNode> nodes;
    CXdmDocumentNode* subHandling(NULL);

    // Finding subhandling node
	actionNode->Find(KXdmSubHandling, nodes);
	OPENG_DP(D_OPENG_LIT( "		GetSubHandlingNodeL nodesCount = %d" ),nodes.Count());
	if(nodes.Count()) // if action node exist
	    {
	    subHandling = nodes[0]; // only one sub-handling node can exist
	    }
	else if (aCreate) // if asked to create
	    {
	    subHandling = actionNode->CreateChileNodeL(KXdmSubHandling);
	    }

    nodes.Close();        
    return subHandling;   
    }

// ---------------------------------------------------------------------------
// TPresenceActionXDM::IsValidAction()
// ---------------------------------------------------------------------------
//    
TBool TPresenceActionXDM::IsValidAction(const TDesC& aAction)
    {
    OPENG_DP(D_OPENG_LIT( " TPresenceActionXDM::IsValidAction()" ) );
    if((aAction==KPresBlock)||(aAction==KPresConfirm)||(aAction==KPresPoliteBlock)||(aAction==KPresAllow))
        return ETrue;
    return EFalse;
    }
    
// end of file

