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
* Description:    RLS and Presence XDM, This class represents those parts of 
*                 presence identity which are not present in any other class.
*
*/




// Includes
#include <XdmDocumentNode.h>
#include <XdmNodeAttribute.h>
#include <xcapappusagedef.h>
#include <utf.h>

#include "prescondmisc.h"
#include "cpresencexdm.h"
#include "rlspresxdmlogger.h"
#include "rlspresxdmconstsint.h"


// ---------------------------------------------------------------------------
// TPresCondMisc::TPresCondMisc()
// ---------------------------------------------------------------------------
//
EXPORT_C TPresCondMisc::TPresCondMisc(): iPresXDM(NULL)
    {
    OPENG_DP(D_OPENG_LIT( "TPresCondMisc::TPresCondMisc()" ));
    }

// ---------------------------------------------------------------------------
// TPresCondMisc::Init()
// ---------------------------------------------------------------------------
//
EXPORT_C void TPresCondMisc::Init(CPresenceXDM* const aPresXDM)
    {
    OPENG_DP(D_OPENG_LIT( "TPresCondMisc::Init(%d)" ), aPresXDM);
    iPresXDM = aPresXDM;
    }

// ---------------------------------------------------------------------------
// TPresCondMisc::AddOtherIdentityL()
// ---------------------------------------------------------------------------
//
EXPORT_C TInt TPresCondMisc::AddOtherIdentityL(const TDesC& aRuleId)
    {
    OPENG_DP(D_OPENG_LIT( "TPresCondMisc::AddOtherIdentityL(%S)"), &aRuleId);
	
    __ASSERT_ALWAYS(iPresXDM, User::Leave(KErrNotReady));
    
    TInt ret(KErrNotFound);
	
	CXdmDocumentNode* otherIdentityNode = iPresXDM->GetConditionChildNodeL(aRuleId, 
	                                                KXdmOtherIdentity, ETrue);
	if(otherIdentityNode)
	    ret = KErrNone;
	
    OPENG_DP(D_OPENG_LIT( "     return: %d"),ret);
    return ret;
    }

// ---------------------------------------------------------------------------
// TPresCondMisc::IsOtherIdentityExistL()
// ---------------------------------------------------------------------------
//
EXPORT_C TBool TPresCondMisc::IsOtherIdentityExistL(const TDesC& aRuleId)
    {
    OPENG_DP(D_OPENG_LIT( "TPresCondMisc::IsOtherIdentityExistL(%S)"), &aRuleId);
	
    __ASSERT_ALWAYS(iPresXDM, User::Leave(KErrNotReady));
    
    TBool ret(EFalse);
	
	CXdmDocumentNode* otherIdentityNode = iPresXDM->GetConditionChildNodeL(aRuleId, 
	                                                KXdmOtherIdentity, EFalse);
	if(otherIdentityNode)
	    ret = ETrue;
	
    OPENG_DP(D_OPENG_LIT( "     return: %d"),ret);
    return ret;
    }

// ---------------------------------------------------------------------------
// TPresCondMisc::RemoveOtherIdentityL()
// ---------------------------------------------------------------------------
//
EXPORT_C TInt TPresCondMisc::RemoveOtherIdentityL(const TDesC& aRuleId)
    {
    OPENG_DP(D_OPENG_LIT( "TPresCondMisc::RemoveOtherIdentityL(%S)"), &aRuleId);
	
    __ASSERT_ALWAYS(iPresXDM, User::Leave(KErrNotReady));
    
    TInt ret(KErrNotFound);
	
	CXdmDocumentNode* otherIdentityNode = iPresXDM->GetConditionChildNodeL(aRuleId, 
	                                                KXdmOtherIdentity,EFalse);
	if(otherIdentityNode)
	    {
	    iPresXDM->RemoveFromModelL(otherIdentityNode);
	    ret = KErrNone;
	    }
	    
    OPENG_DP(D_OPENG_LIT( "     return: %d"),ret);
    return ret;
    }

// ---------------------------------------------------------------------------
// TPresCondMisc::AddAnonymousRequestL()
// ---------------------------------------------------------------------------
//
EXPORT_C TInt TPresCondMisc::AddAnonymousRequestL(const TDesC& aRuleId)
    {
    OPENG_DP(D_OPENG_LIT( "TPresCondMisc::AddAnonymousRequestL(%S)"), &aRuleId);
	
    __ASSERT_ALWAYS(iPresXDM, User::Leave(KErrNotReady));
    
    TInt ret(KErrNotFound);
	
	CXdmDocumentNode* anonymousReqNode = iPresXDM->GetConditionChildNodeL(aRuleId, 
	                                                KXdmAnonymousRequest, ETrue);
	if(anonymousReqNode)
	    ret = KErrNone;
	
    OPENG_DP(D_OPENG_LIT( "     return: %d"),ret);
    return ret;
    }

// ---------------------------------------------------------------------------
// TPresCondMisc::IsAnonymousRequestExistL()
// ---------------------------------------------------------------------------
//
EXPORT_C TBool TPresCondMisc::IsAnonymousRequestExistL(const TDesC& aRuleId)
    {
    OPENG_DP(D_OPENG_LIT( "TPresCondMisc::IsAnonymousRequestExistL(%S)"), &aRuleId);
	
    __ASSERT_ALWAYS(iPresXDM, User::Leave(KErrNotReady));
    
    TBool ret(EFalse);
	
	CXdmDocumentNode* anonymousReqNode = iPresXDM->GetConditionChildNodeL(aRuleId, 
	                                                KXdmAnonymousRequest, EFalse);
	if(anonymousReqNode)
	    ret = ETrue;
	
    OPENG_DP(D_OPENG_LIT( "     return: %d"),ret);
    return ret;
    }

// ---------------------------------------------------------------------------
// TPresCondMisc::RemoveAnonymousRequestL()
// ---------------------------------------------------------------------------
//
EXPORT_C TInt TPresCondMisc::RemoveAnonymousRequestL(const TDesC& aRuleId)
    {
    OPENG_DP(D_OPENG_LIT( "TPresCondMisc::RemoveAnonymousRequestL(%S)"), &aRuleId);
	
    __ASSERT_ALWAYS(iPresXDM, User::Leave(KErrNotReady));
    
    TInt ret(KErrNotFound);
	
	CXdmDocumentNode* anonymousReqNode = iPresXDM->GetConditionChildNodeL(aRuleId, 
	                                                KXdmAnonymousRequest,EFalse);
	if(anonymousReqNode)
	    {
	    iPresXDM->RemoveFromModelL(anonymousReqNode);
	    ret = KErrNone;
	    }
	    
    OPENG_DP(D_OPENG_LIT( "     return: %d"),ret);
    return ret;
    }

// ---------------------------------------------------------------------------
// TPresCondMisc::AddExternListL()
// ---------------------------------------------------------------------------
//
EXPORT_C TInt TPresCondMisc::AddExternListL(const TDesC& aRuleId, 
                                                            const TDesC& aUri)
    {
    OPENG_DP(D_OPENG_LIT( "TPresCondMisc::AddExternListL()" ) );
    OPENG_DP(D_OPENG_LIT( "     aRuleId = %S, aUri = %S"),&aRuleId, &aUri);
	
    __ASSERT_ALWAYS(iPresXDM, User::Leave(KErrNotReady));
	
	CXdmDocumentNode* extListNode = iPresXDM->GetConditionChildNodeL(aRuleId, 
	                                                KXdmExternalList, ETrue);
	if(extListNode==NULL)
	    return KErrNotFound;
	
	if(IsExternListExistL(aRuleId,aUri))
	    return KErrAlreadyExists;
	
    CXdmDocumentNode* entryNode(NULL);
    CXdmNodeAttribute* ancAttr(NULL);
   
    entryNode = extListNode->CreateChileNodeL(KXdmEntry);
    ancAttr = entryNode->CreateAttributeL(KXdmAnc);
    ancAttr->SetAttributeValueL(aUri);
    
    OPENG_DP(D_OPENG_LIT( "     return: KErrNone"));
    return KErrNone;
    }
    
// ---------------------------------------------------------------------------
// TPresCondMisc::IsExternListExistL()
// ---------------------------------------------------------------------------
//
EXPORT_C TBool TPresCondMisc::IsExternListExistL(const TDesC& aRuleId,
                                                            const TDesC& aUri)
    {
    OPENG_DP(D_OPENG_LIT( "TPresCondMisc::IsExternListExistL()" ) );
    OPENG_DP(D_OPENG_LIT( "     aRuleId = %S, aUri = %S"),&aRuleId, &aUri);

    __ASSERT_ALWAYS(iPresXDM, User::Leave(KErrNotReady));

	CXdmDocumentNode* extListNode = iPresXDM->GetConditionChildNodeL(aRuleId, 
	                                                KXdmExternalList, EFalse);
	if(extListNode==NULL)
	    return EFalse;

	RPointerArray<CXdmDocumentNode> nodes;
    CXdmNodeAttribute* ancAttr(NULL);
	
	// finding entry nodes
    TInt err = extListNode->Find(KXdmEntry, nodes);
    
    // extracting lists from entry nodes
    TInt entriesCountInNode = nodes.Count();
    OPENG_DP(D_OPENG_LIT( "     nodesCount = %d" ),entriesCountInNode);
    
    for (TInt i=0;i<entriesCountInNode;i++)
        { // get the anc attribute
        ancAttr = (nodes[i])->Attribute(KXdmAnc);
        if (aUri==(ancAttr->AttributeValue()))
            {
            OPENG_DP(D_OPENG_LIT( "     ETrue"));
            nodes.Close();
            return ETrue;               
            }
        }

    nodes.Close();    
    return EFalse;
    }

// ---------------------------------------------------------------------------
// TPresCondMisc::GetExternalListsL()
// ---------------------------------------------------------------------------
//
EXPORT_C void TPresCondMisc::GetExternalListsL(const TDesC& aRuleId, 
                                                            CDesCArray& aUris)
    {
    OPENG_DP(D_OPENG_LIT( "TPresCondMisc::GetExternalListsL()" ) );
    OPENG_DP(D_OPENG_LIT( "     aRuleId = %S" ),&aRuleId);
    
    __ASSERT_ALWAYS(iPresXDM, User::Leave(KErrNotReady));
    
	CXdmDocumentNode* extListNode = iPresXDM->GetConditionChildNodeL(aRuleId, 
	                                                KXdmExternalList, EFalse);
	if(extListNode==NULL)
	    return;
	
	RPointerArray<CXdmDocumentNode> nodes;
    CXdmNodeAttribute* ancAttr(NULL);
	
	// finding entry nodes
    TInt err = extListNode->Find(KXdmEntry, nodes);
    OPENG_DP(D_OPENG_LIT( "     nodesCount = %d" ),nodes.Count());

    // extracting lists from entry nodes
    TInt entriesCountInNode = nodes.Count();
    for (TInt i=0;i<entriesCountInNode;i++)
        { // get the anc attribute
        ancAttr = (nodes[i])->Attribute(KXdmAnc);
        // push it to given discriptor array
        aUris.AppendL(ancAttr->AttributeValue());
        }

    nodes.Close();    
    return;
    }                                            

// ---------------------------------------------------------------------------
// TPresCondMisc::DeleteExternalListL()
// ---------------------------------------------------------------------------
//
EXPORT_C TInt TPresCondMisc::DeleteExternalListL(const TDesC& aRuleId, 
                                                            const TDesC& aUri)
    {
    OPENG_DP(D_OPENG_LIT( "TPresCondMisc::DeleteExternalListL()" ) );
    OPENG_DP(D_OPENG_LIT( "     aRuleId = %S, aUri = %S"),&aRuleId, &aUri);
	
    __ASSERT_ALWAYS(iPresXDM, User::Leave(KErrNotReady));
	
	TInt ret(KErrNotFound);
	
	CXdmDocumentNode* extListNode = iPresXDM->GetConditionChildNodeL(aRuleId, 
	                                                KXdmExternalList, EFalse);
	if(extListNode==NULL)
	    return ret;

	RPointerArray<CXdmDocumentNode> nodes;
    CXdmNodeAttribute* ancAttr(NULL);
	
	// finding entry nodes
    if ( (extListNode->Find(KXdmEntry, nodes))==KErrNone)
        {
        // extracting lists from entry nodes
        TInt entriesCountInNode = nodes.Count();
        OPENG_DP(D_OPENG_LIT( "     nodesCount = %d" ),entriesCountInNode);
       
        for (TInt i=0;i<entriesCountInNode;i++)
            { // get the anc attribute
            ancAttr = (nodes[i])->Attribute(KXdmAnc);
            if (aUri==(ancAttr->AttributeValue()))
                {
                OPENG_DP(D_OPENG_LIT( "     RemoveFromModelL: i=%d" ),i);
                iPresXDM->RemoveFromModelL(nodes[i]);
                ret = KErrNone;
                break;               
                }
            }
        }
    nodes.Close();    
    return ret;
    }                                            

// ---------------------------------------------------------------------------
// TPresCondMisc::DeleteAllExternalListsL()
// ---------------------------------------------------------------------------
//
EXPORT_C void TPresCondMisc::DeleteAllExternalListsL(const TDesC& aRuleId)
    {
    OPENG_DP(D_OPENG_LIT( "TPresCondMisc::DeleteAllExternalListsL()" ) );
    OPENG_DP(D_OPENG_LIT( "     aRuleId = %S"),&aRuleId);
	
    __ASSERT_ALWAYS(iPresXDM, User::Leave(KErrNotReady));
	
	CXdmDocumentNode* extListNode = iPresXDM->GetConditionChildNodeL(aRuleId, 
	                                                KXdmExternalList, EFalse);
	if(extListNode==NULL)
	    return;
	
	RPointerArray<CXdmDocumentNode> nodes;
	
	// finding entry nodes
    TInt err = extListNode->Find(KXdmEntry, nodes);
    
    // extracting lists from entry nodes
    TInt entriesCountInNode = nodes.Count();
    OPENG_DP(D_OPENG_LIT( "     nodesCount = %d" ),entriesCountInNode);
    
    for (TInt i=0;i<entriesCountInNode;i++)
        {
        iPresXDM->RemoveFromModelL(nodes[i]);
        }

    nodes.Close();    
    return;
    }

// ---------------------------------------------------------------------------
// TPresCondMisc::DeleteAllExternalListsL()
// ---------------------------------------------------------------------------
//
EXPORT_C void TPresCondMisc::DeleteExternalListsL(const TDesC& aRuleId,
                                                    const MDesCArray& aUris)
    {
    OPENG_DP(D_OPENG_LIT( "TPresCondMisc::DeleteExternalListsL()" ) );
    OPENG_DP(D_OPENG_LIT( "     aRuleId = %S, aUrisCount = %d" ),&aRuleId, 
                                                        aUris.MdcaCount() );
	
    __ASSERT_ALWAYS(iPresXDM, User::Leave(KErrNotReady));                                                    
	
	CXdmDocumentNode* extListNode = iPresXDM->GetConditionChildNodeL(aRuleId, 
	                                                KXdmExternalList, EFalse);
	if(extListNode==NULL)
	    return;
	
	RPointerArray<CXdmDocumentNode> nodes;
    CXdmNodeAttribute* ancAttr(NULL);
	
	// finding entry nodes
    TInt err = extListNode->Find(KXdmEntry, nodes);

    // extracting lists from entry nodes
    TInt entriesCountInNode = nodes.Count();
    OPENG_DP(D_OPENG_LIT( "     nodesCount = %d" ),entriesCountInNode);

    TInt entitiesCount = aUris.MdcaCount();
    for (TInt i=0;i<entriesCountInNode;i++)
        { // get the anc attribute
        ancAttr = (nodes[i])->Attribute(KXdmAnc);
        // match it with all given entries
        for (TInt j=0;j<entitiesCount ; j++)
            {
            if ((aUris.MdcaPoint(j))==(ancAttr->AttributeValue()))
                {
                OPENG_DP(D_OPENG_LIT( "     RemoveFromModelL i=%d,j=%d" ),i,j);
                iPresXDM->RemoveFromModelL(nodes[i]);
                break;               
                }
            }
        }

    nodes.Close();    
    return;
    }
