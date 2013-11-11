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
* Description:    RLS and Presence XDM, This class represents 'one' identity part
*                 of a Presence Condition in a rule
*
*/




// Includes
#include <XdmDocumentNode.h>
#include <XdmNodeAttribute.h>
#include <xcapappusagedef.h>
#include <utf.h>

#include "prescondidentityone.h"
#include "cpresencexdm.h"
#include "rlspresxdmlogger.h"
#include "rlspresxdmconstsint.h"


// ---------------------------------------------------------------------------
// TPresCondIdentityOne::TPresCondIdentityOne()
// ---------------------------------------------------------------------------
//	
EXPORT_C TPresCondIdentityOne::TPresCondIdentityOne(): iPresXDM(NULL)
    {
    OPENG_DP(D_OPENG_LIT( "TPresCondIdentityOne::TPresCondIdentityOne()" ));
    }

// ---------------------------------------------------------------------------
// TPresCondIdentityOne::Init()
// ---------------------------------------------------------------------------
//    
EXPORT_C void TPresCondIdentityOne::Init(CPresenceXDM* const aPresXDM)
    {
    OPENG_DP(D_OPENG_LIT( "TPresCondIdentityOne::Init(%d)" ), aPresXDM);
    iPresXDM = aPresXDM;
    }

// ---------------------------------------------------------------------------
// TPresCondIdentityOne::AddIdentitiesL()
// ---------------------------------------------------------------------------
//
EXPORT_C TInt TPresCondIdentityOne::AddIdentitiesL(const TDesC& aRuleID,
                                                const MDesCArray& aIds)
	{
    OPENG_DP(D_OPENG_LIT( "TPresCondIdentityOne::AddIdentitiesL()" ) );
    OPENG_DP(D_OPENG_LIT( "     aRuleID = %S, aIdsCount = %d" ),&aRuleID, 
                                                        aIds.MdcaCount() );
    
    __ASSERT_ALWAYS(iPresXDM, User::Leave(KErrNotReady));                                                    
    
	CXdmDocumentNode* identityNode = iPresXDM->GetConditionChildNodeL(aRuleID, 
	                                                KXdmIdentity, ETrue);
	if(identityNode==NULL)
	    {
	    OPENG_DP(D_OPENG_LIT( "     return: KErrNotFound" ));
	    return KErrNotFound;
	    }
	    
    CXdmDocumentNode* oneNode(NULL);
    CXdmNodeAttribute* idAttr(NULL);
  	RPointerArray<CXdmDocumentNode> nodes;
  	
	// finding 'one' nodes
    TInt err = identityNode->Find(KXdmOne, nodes);
    OPENG_DP(D_OPENG_LIT( "     nodesCount = %d" ),nodes.Count());

    // putting ids to identity node
    TInt entitiesCount = aIds.MdcaCount();
    for(TInt i=0;i<entitiesCount;i++)
        {
        // find whether its already exist
        if(IsIdExistInNodeArray(nodes, aIds.MdcaPoint(i)) )
            continue;    
        oneNode = identityNode->CreateChileNodeL(KXdmOne);
        idAttr = oneNode->CreateAttributeL(KXdmId);
        idAttr->SetAttributeValueL(aIds.MdcaPoint(i));
        }
    
    nodes.Close();
    OPENG_DP(D_OPENG_LIT( "     return: KErrNone" ));    
    return KErrNone;
	}

// ---------------------------------------------------------------------------
// TPresCondIdentityOne::GetIdentitiesL()
// ---------------------------------------------------------------------------
//	
EXPORT_C void TPresCondIdentityOne::GetIdentitiesL(const TDesC& aRuleID, 
                                                    CDesCArray& aIds)
	{
    OPENG_DP(D_OPENG_LIT( "TPresCondIdentityOne::GetIdentitiesL()" ) );
    OPENG_DP(D_OPENG_LIT( "     aRuleID = %S" ),&aRuleID);
    
    __ASSERT_ALWAYS(iPresXDM, User::Leave(KErrNotReady));
    
	CXdmDocumentNode* identityNode = iPresXDM->GetConditionChildNodeL(aRuleID, 
	                                                KXdmIdentity, EFalse);
	if(identityNode==NULL)
	    return;
	
	RPointerArray<CXdmDocumentNode> nodes;
    CXdmNodeAttribute* idAttr(NULL);
	
	// finding 'one' nodes
    TInt err = identityNode->Find(KXdmOne, nodes);
    OPENG_DP(D_OPENG_LIT( "     nodesCount = %d" ),nodes.Count());

    // extracting ids from ones nodes
    TInt idsCountInNode = nodes.Count();
    for (TInt i=0;i<idsCountInNode;i++)
        { // get the id attribute
        idAttr = (nodes[i])->Attribute(KXdmId);
        // push it to given discriptor array
        aIds.AppendL(idAttr->AttributeValue());
        }

    nodes.Close();    
    return;
	}

// ---------------------------------------------------------------------------
// TPresCondIdentityOne::DeleteIdentitiesL()
// ---------------------------------------------------------------------------
//	
EXPORT_C void TPresCondIdentityOne::DeleteIdentitiesL(const TDesC& aRuleID,
                                                    const MDesCArray& aIds)
	{
    OPENG_DP(D_OPENG_LIT( "TPresCondIdentityOne::DeleteIdentitiesL()" ) );
    OPENG_DP(D_OPENG_LIT( "     aRuleID = %S, aIdsCount = %d" ),&aRuleID, 
                                                        aIds.MdcaCount() );
	
    __ASSERT_ALWAYS(iPresXDM, User::Leave(KErrNotReady));                                                    
	
	CXdmDocumentNode* identityNode = iPresXDM->GetConditionChildNodeL(aRuleID, 
	                                                KXdmIdentity, EFalse);
	if(identityNode==NULL)
	    return;
	
	RPointerArray<CXdmDocumentNode> nodes;
    CXdmNodeAttribute* idAttr(NULL);
	
	// finding 'one' nodes
    TInt err = identityNode->Find(KXdmOne, nodes);

    // extracting ids from ones nodes
    TInt idsCountInNode = nodes.Count();
    OPENG_DP(D_OPENG_LIT( "     nodesCount = %d" ),idsCountInNode);

    TInt entitiesCount = aIds.MdcaCount();
    for (TInt i=0;i<idsCountInNode;i++)
        { // get the id attribute
        idAttr = (nodes[i])->Attribute(KXdmId);
        // match it with all given entries
        for (TInt j=0;j<entitiesCount ; j++)
            {
            if ((aIds.MdcaPoint(j))==(idAttr->AttributeValue()))
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

// ---------------------------------------------------------------------------
// TPresCondIdentityOne::DeleteAllIdentitiesL()
// ---------------------------------------------------------------------------
//
EXPORT_C void TPresCondIdentityOne::DeleteAllIdentitiesL(const TDesC& aRuleID)
	{
    OPENG_DP(D_OPENG_LIT( "TPresCondIdentityOne::DeleteAllIdentitiesL()" ) );
    OPENG_DP(D_OPENG_LIT( "     aRuleID = %S"),&aRuleID);
	
    __ASSERT_ALWAYS(iPresXDM, User::Leave(KErrNotReady));
	
	CXdmDocumentNode* identityNode = iPresXDM->GetConditionChildNodeL(aRuleID, 
	                                                KXdmIdentity, EFalse);
	if(identityNode==NULL)
	    return;
	
	RPointerArray<CXdmDocumentNode> nodes;
	
	// finding 'one' nodes
    TInt err = identityNode->Find(KXdmOne, nodes);
    
    // extracting ids from ones nodes
    TInt idsCountInNode = nodes.Count();
    OPENG_DP(D_OPENG_LIT( "     nodesCount = %d" ),idsCountInNode);
    
    for (TInt i=0;i<idsCountInNode;i++)
        {
        iPresXDM->RemoveFromModelL(nodes[i]);
        }

    nodes.Close();    
    return;
	}

// ---------------------------------------------------------------------------
// TPresCondIdentityOne::AddIdentityL()
// ---------------------------------------------------------------------------
//	
EXPORT_C TInt TPresCondIdentityOne::AddIdentityL(const TDesC& aRuleID, 
                                                        const TDesC& aId)
	{
    OPENG_DP(D_OPENG_LIT( "TPresCondIdentityOne::AddIdentityL()" ) );
    OPENG_DP(D_OPENG_LIT( "     aRuleID = %S, aId = %S"),&aRuleID, &aId);
	
    __ASSERT_ALWAYS(iPresXDM, User::Leave(KErrNotReady));
	
	CXdmDocumentNode* identityNode = iPresXDM->GetConditionChildNodeL(aRuleID, 
	                                                KXdmIdentity, ETrue);
	if(identityNode==NULL)
	    return KErrNotFound;
	
	if(IsIdentityExistL(aRuleID,aId))
	    return KErrAlreadyExists;
	
    CXdmDocumentNode* oneNode(NULL);
    CXdmNodeAttribute* idAttr(NULL);
   
    oneNode = identityNode->CreateChileNodeL(KXdmOne);
    idAttr = oneNode->CreateAttributeL(KXdmId);
    idAttr->SetAttributeValueL(aId);
    
    OPENG_DP(D_OPENG_LIT( "     return: KErrNone"));
    return KErrNone;
	}

// ---------------------------------------------------------------------------
// TPresCondIdentityOne::DeleteIdentityL()
// ---------------------------------------------------------------------------
//	
EXPORT_C TInt TPresCondIdentityOne::DeleteIdentityL(const TDesC& aRuleID, 
                                                        const TDesC& aId)
	{
    OPENG_DP(D_OPENG_LIT( "TPresCondIdentityOne::DeleteIdentityL()" ) );
    OPENG_DP(D_OPENG_LIT( "     aRuleID = %S, aId = %S"),&aRuleID, &aId);
	
    __ASSERT_ALWAYS(iPresXDM, User::Leave(KErrNotReady));
	
	TInt ret(KErrNotFound);
	
	CXdmDocumentNode* identityNode = iPresXDM->GetConditionChildNodeL(aRuleID, 
	                                                KXdmIdentity, EFalse);
	if(identityNode==NULL)
	    return ret;

	RPointerArray<CXdmDocumentNode> nodes;
    CXdmNodeAttribute* idAttr(NULL);
	
	// finding 'one' nodes
    if ( (identityNode->Find(KXdmOne, nodes))==KErrNone)
        {
        // extracting ids from ones nodes
        TInt idsCountInNode = nodes.Count();
        OPENG_DP(D_OPENG_LIT( "     nodesCount = %d" ),idsCountInNode);
       
        for (TInt i=0;i<idsCountInNode;i++)
            { // get the id attribute
            idAttr = (nodes[i])->Attribute(KXdmId);
            if (aId==(idAttr->AttributeValue()))
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
// TPresCondIdentityOne::IsIdentityExistL()
// ---------------------------------------------------------------------------
//	
EXPORT_C TBool TPresCondIdentityOne::IsIdentityExistL(const TDesC& aRuleID, 
                                                    const TDesC& aId)
	{
    OPENG_DP(D_OPENG_LIT( "TPresCondIdentityOne::IsIdentityExist()" ) );
    OPENG_DP(D_OPENG_LIT( "     aRuleID = %S, aId = %S"),&aRuleID, &aId);

    __ASSERT_ALWAYS(iPresXDM, User::Leave(KErrNotReady));

	CXdmDocumentNode* identityNode = iPresXDM->GetConditionChildNodeL(aRuleID, 
	                                                KXdmIdentity, EFalse);
	if(identityNode==NULL)
	    return EFalse;

	RPointerArray<CXdmDocumentNode> nodes;
    CXdmNodeAttribute* idAttr(NULL);
	
	// finding 'one' nodes
    TInt err = identityNode->Find(KXdmOne, nodes);
    
    // extracting ids from ones nodes
    TInt idsCountInNode = nodes.Count();
    OPENG_DP(D_OPENG_LIT( "     nodesCount = %d" ),idsCountInNode);
    
    for (TInt i=0;i<idsCountInNode;i++)
        { // get the id attribute
        idAttr = (nodes[i])->Attribute(KXdmId);
        if (aId==(idAttr->AttributeValue()))
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
// TPresCondIdentityOne::IsIdExistInNodeArray()
// ---------------------------------------------------------------------------
//
TBool TPresCondIdentityOne::IsIdExistInNodeArray(
                        const RPointerArray<CXdmDocumentNode>& nodes, 
                                                    const TDesC& aId)
    {
    OPENG_DP(D_OPENG_LIT( " TPresCondIdentityOne::IsIdExistInNodeArray()" ) );
    OPENG_DP(D_OPENG_LIT( "     IsIdExistInNodeArray aId = %S"), &aId);

    TInt idsCountInNode = nodes.Count();
    CXdmNodeAttribute* idAttr(NULL);
    for (TInt i=0;i<idsCountInNode;i++)
        { // get the id attribute
        idAttr = (nodes[i])->Attribute(KXdmId);
        if (aId==(idAttr->AttributeValue()))
            {
            OPENG_DP(D_OPENG_LIT( "     IsIdExistInNodeArray True"));
            return ETrue;               
            }
        }
    return EFalse;  
    }
    
// end of file

