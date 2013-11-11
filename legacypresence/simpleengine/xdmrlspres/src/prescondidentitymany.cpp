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
* Description:    RLS and Presence XDM, This class represents 'many' identity 
*                 part of a Presence Condition in a rule
*
*/




// Includes
#include <XdmDocumentNode.h>
#include <XdmNodeAttribute.h>
#include <xcapappusagedef.h>
#include <utf.h>

#include "prescondidentitymany.h"
#include "cpresencexdm.h"
#include "rlspresxdmlogger.h"
#include "rlspresxdmconstsint.h"


// ---------------------------------------------------------------------------
// TPresCondIdentityMany::TPresCondIdentityMany()
// ---------------------------------------------------------------------------
//	
EXPORT_C TPresCondIdentityMany::TPresCondIdentityMany(): iPresXDM(NULL)
    {
    OPENG_DP(D_OPENG_LIT( "TPresCondIdentityMany::TPresCondIdentityMany()" ));
    }

// ---------------------------------------------------------------------------
// TPresCondIdentityMany::Init()
// ---------------------------------------------------------------------------
//    
EXPORT_C void TPresCondIdentityMany::Init(CPresenceXDM* const aPresXDM)
    {
    OPENG_DP(D_OPENG_LIT( "TPresCondIdentityMany::Init(%d)" ), aPresXDM);
    iPresXDM = aPresXDM;
    }

// ---------------------------------------------------------------------------
// TPresCondIdentityMany::AddManyIdentityL()
// ---------------------------------------------------------------------------
//    
EXPORT_C TInt TPresCondIdentityMany::AddManyIdentityL(const TDesC& aRuleID, 
                                            const TDesC& aDomain)
    {
    OPENG_DP(D_OPENG_LIT( "TPresCondIdentityMany::AddManyIdentityL()" ) );
    OPENG_DP(D_OPENG_LIT( "     aRuleID = %S, aDomain = %S"),&aRuleID, &aDomain);
    
    TInt err(KErrAlreadyExists);
    
    if(!(IsManyExistsL(aRuleID,aDomain)))
        {
    	CXdmDocumentNode* manyNode = GetManyNodeL(aRuleID, ETrue, aDomain);
    	if(manyNode==NULL)
    	    err = KErrNotFound;
    	else
    	    err = KErrNone;
        }
	
    OPENG_DP(D_OPENG_LIT( "     return: %d"),err);
    return err;
    }
        
// ---------------------------------------------------------------------------
// TPresCondIdentityMany::IsManyExistsL()
// ---------------------------------------------------------------------------
//    
EXPORT_C TBool TPresCondIdentityMany::IsManyExistsL(const TDesC& aRuleID, 
                                            const TDesC& aDomain)
    {
    OPENG_DP(D_OPENG_LIT( "TPresCondIdentityMany::IsManyExists()" ) );
    OPENG_DP(D_OPENG_LIT( "     aRuleID = %S, aDomain = %S"),&aRuleID, &aDomain);
    
    TBool ret(ETrue);
	
	CXdmDocumentNode* manyNode = GetManyNodeL(aRuleID, EFalse, aDomain);
	if(manyNode==NULL)
	    ret = EFalse;
	
	OPENG_DP(D_OPENG_LIT( "     return: %d"),ret);
	return ret;
    }
        
// ---------------------------------------------------------------------------
// TPresCondIdentityMany::RemoveManyIdentityL()
// ---------------------------------------------------------------------------
//    
EXPORT_C TInt TPresCondIdentityMany::RemoveManyIdentityL(const TDesC& aRuleID, 
                                            const TDesC& aDomain)
    {
    OPENG_DP(D_OPENG_LIT( "TPresCondIdentityMany::RemoveManyIdentityL()" ) );
    OPENG_DP(D_OPENG_LIT( "     aRuleID = %S, aDomain = %S"),&aRuleID, &aDomain);
    
    __ASSERT_ALWAYS(iPresXDM, User::Leave(KErrNotReady));
    
    TInt ret(KErrNotFound);
	
	CXdmDocumentNode* manyNode = GetManyNodeL(aRuleID, EFalse, aDomain);
	if(manyNode!=NULL)
	    {
        OPENG_DP(D_OPENG_LIT( "     Removed"));
        iPresXDM->RemoveFromModelL(manyNode);
        ret = KErrNone;     
	    }
	
	OPENG_DP(D_OPENG_LIT( "     return: %d"),ret);
	return ret;
    }
                                            
// ---------------------------------------------------------------------------
// TPresCondIdentityMany::GetManyIdentityElementsL()
// ---------------------------------------------------------------------------
//    
EXPORT_C TInt TPresCondIdentityMany::GetManyIdentityElementsL(const TDesC& aRuleID, 
                                                    CDesCArray& aDomains)
    {
    OPENG_DP(D_OPENG_LIT( "TPresCondIdentityMany::GetManyIdentityElementsL()" ) );
    OPENG_DP(D_OPENG_LIT( "     aRuleID = %S" ),&aRuleID);
    
    __ASSERT_ALWAYS(iPresXDM, User::Leave(KErrNotReady));
    
	CXdmDocumentNode* identityNode = iPresXDM->GetConditionChildNodeL(aRuleID, 
	                                                KXdmIdentity, EFalse);
	if(identityNode==NULL)
	    return 0;
	
	RPointerArray<CXdmDocumentNode> nodes;
    CXdmNodeAttribute* domainAttr(NULL);
	
	// finding 'many' nodes
    TInt err = identityNode->Find(KXdmMany, nodes);

    // extracting entities from id nodes
    TInt manyCountInNode = nodes.Count();

    OPENG_DP(D_OPENG_LIT( "     manyCountInNode = %d" ),manyCountInNode);

    for (TInt i=0;i<manyCountInNode;i++)
        { // get the domain attribute
        domainAttr = (nodes[i])->Attribute(KXdmDomain);
        if(domainAttr)
            // push it to given discriptor array
            aDomains.AppendL(domainAttr->AttributeValue());
        }

    nodes.Close();    
    return manyCountInNode;
    }
        
// ---------------------------------------------------------------------------
// TPresCondIdentityMany::AddIdentityInManyExceptL()
// ---------------------------------------------------------------------------
//    
EXPORT_C TInt TPresCondIdentityMany::AddIdentityInManyExceptL(const TDesC& aRuleID,
                    const TDesC& aId, const TDesC& aDomain)
    {
    OPENG_DP(D_OPENG_LIT( "TPresCondIdentityMany::AddIdentityInManyExceptL()" ) );
    OPENG_DP(D_OPENG_LIT( "     aRuleID:%S,aDomain:%S,aId:%S"),
                                                    &aRuleID, &aDomain, &aId);
    
    TInt ret(KErrAlreadyExists);

	RPointerArray<CXdmDocumentNode> nodes;
	CXdmDocumentNode* exceptNode(NULL);
	CXdmNodeAttribute* idattr(NULL);
    
    
	CXdmDocumentNode* manyNode = GetManyNodeL(aRuleID, EFalse, aDomain);
	if(manyNode!=NULL)
	    {
        // if id doesnt exists in except
        if (!(IsIdentityExistsInManyExceptL(aRuleID, aId, aDomain)))
            {
            exceptNode = manyNode->CreateChileNodeL(KXdmExcept);
            idattr = exceptNode->CreateAttributeL(KXdmId);
            idattr->SetAttributeValueL(aId);
            ret = KErrNone;
            }
	    }
	else
	    ret = KErrNotFound;
	
	OPENG_DP(D_OPENG_LIT( "     return: %d"),ret);
	nodes.Close();
	return ret;
    }
                    
// ---------------------------------------------------------------------------
// TPresCondIdentityMany::AddIdentitiesInManyExceptL()
// ---------------------------------------------------------------------------
//    
EXPORT_C TInt TPresCondIdentityMany::AddIdentitiesInManyExceptL(const TDesC& aRuleID,
                const CDesCArray& aIds, const TDesC& aDomain)
    {
    OPENG_DP(D_OPENG_LIT( "TPresCondIdentityMany::AddIdentitiesInManyExceptL()" ) );
    OPENG_DP(D_OPENG_LIT( "     aRuleID:%S,aDomain:%S"),
                                                    &aRuleID, &aDomain);
    
    TInt ret(KErrNotFound);

	RPointerArray<CXdmDocumentNode> nodes;
	CXdmDocumentNode* exceptNode(NULL);
	CXdmNodeAttribute* idattr(NULL);
    
	CXdmDocumentNode* manyNode = GetManyNodeL(aRuleID, EFalse, aDomain);
	TInt entitiesCount = aIds.MdcaCount();
	if(manyNode!=NULL)
	    {
        for (TInt i = 0; i<entitiesCount; i++)
            {
            // if id doesnt exists in except
            if (!(IsIdentityExistsInManyExceptL(aRuleID, aIds.MdcaPoint(i), aDomain)))
                {
                exceptNode = manyNode->CreateChileNodeL(KXdmExcept);
                idattr = exceptNode->CreateAttributeL(KXdmId);
                idattr->SetAttributeValueL(aIds.MdcaPoint(i));
                ret = KErrNone;
                }
            }
        ret = KErrNone;
	    }

	OPENG_DP(D_OPENG_LIT( "     return: %d"),ret);
	nodes.Close();
	return ret;
    }
                                        
// ---------------------------------------------------------------------------
// TPresCondIdentityMany::GetIdentitiesFromManyExceptL()
// ---------------------------------------------------------------------------
//    
EXPORT_C TInt TPresCondIdentityMany::GetIdentitiesFromManyExceptL(const TDesC& aRuleID,
                    CDesCArray& aIds, const TDesC& aDomain)
    {
    OPENG_DP(D_OPENG_LIT( "TPresCondIdentityMany::GetIdentitiesFromManyExceptL()" ) );
    OPENG_DP(D_OPENG_LIT( "     aRuleID:%S,aDomain:%S"),&aRuleID, &aDomain);
    
    TInt ret(KErrNotFound);

	RPointerArray<CXdmDocumentNode> nodes;
	CXdmNodeAttribute* attrId(NULL);
    
	CXdmDocumentNode* manyNode = GetManyNodeL(aRuleID, EFalse, aDomain);
	if(manyNode!=NULL)
	    {
        TInt err = manyNode->Find(KXdmExcept, nodes);
        TInt exceptNodeCount = nodes.Count();
        
        // go through all 'except' nodes
        for(TInt i=0;i<exceptNodeCount;i++)
            {
            attrId = (nodes[i])->Attribute(KXdmId);
            
            // for now we are supposing that except has only 1 id
            if(attrId!=NULL)
                {
                aIds.AppendL(attrId->AttributeValue());
                }
            attrId = NULL;
            }
        ret = KErrNone;
	    }

	OPENG_DP(D_OPENG_LIT( "     return: %d"),ret);
	nodes.Close();
	return ret;
    }
    
// ---------------------------------------------------------------------------
// TPresCondIdentityMany::RemoveIdentityFromManyExceptL()
// ---------------------------------------------------------------------------
//    
EXPORT_C TInt TPresCondIdentityMany::RemoveIdentityFromManyExceptL(
        const TDesC& aRuleID, const TDesC& aId, const TDesC& aDomain)
    {
    OPENG_DP(D_OPENG_LIT( "TPresCondIdentityMany::RemoveIdentityFromManyExceptL()" ) );
    OPENG_DP(D_OPENG_LIT( "     aRuleID:%S,aDomain:%S,aId:%S"),
                                                    &aRuleID, &aDomain, &aId);
    
    __ASSERT_ALWAYS(iPresXDM, User::Leave(KErrNotReady));                                                
    
    TInt ret(KErrNotFound);

	RPointerArray<CXdmDocumentNode> nodes;
    CXdmNodeAttribute* attrId(NULL);
    
    
	CXdmDocumentNode* manyNode = GetManyNodeL(aRuleID, EFalse, aDomain);
	if(manyNode!=NULL)
	    {
        TInt err = manyNode->Find(KXdmExcept, nodes);
        TInt exceptNodeCount = nodes.Count();
        
        //go through all except nodes
        for (TInt i=0;i<exceptNodeCount;i++)
            {
            attrId = (nodes[i])->Attribute(KXdmId);
            
            // for now we are supposing that except has only 1 id
            if(attrId!=NULL)
                {
                if (aId==(attrId->AttributeValue()))
                    {
                    OPENG_DP(D_OPENG_LIT( "     RemoveFromModelL: i=%d" ),i);
                    iPresXDM->RemoveFromModelL(attrId);
                    
                    //if except node is empty after this
                    if( (nodes[i])->AttributeCount() == 0)
                        {
                        // remove except node also
                        iPresXDM->RemoveFromModelL(nodes[i]);
                        }
                    ret = KErrNone;
                    break;               
                    }
                }
            attrId = NULL;
            }
	    }
	
	OPENG_DP(D_OPENG_LIT( "     return: %d"),ret);
	nodes.Close();
	return ret;
    }
    
                                        
// ---------------------------------------------------------------------------
// TPresCondIdentityMany::DeleteAllIdentitiesFromManyExceptL()
// ---------------------------------------------------------------------------
//    
EXPORT_C TInt TPresCondIdentityMany::DeleteAllIdentitiesFromManyExceptL(
                                const TDesC& aRuleID, const TDesC& aDomain)
    {
    OPENG_DP(D_OPENG_LIT( "TPresCondIdentityMany::DeleteAllIdentitiesFromManyExceptL()" ) );
    OPENG_DP(D_OPENG_LIT( "     aRuleID:%S,aDomain:%S"),&aRuleID, &aDomain);
    
    __ASSERT_ALWAYS(iPresXDM, User::Leave(KErrNotReady));
    
    TInt ret(KErrNotFound);

	RPointerArray<CXdmDocumentNode> nodes;
	CXdmNodeAttribute* attrId(NULL);
    
	CXdmDocumentNode* manyNode = GetManyNodeL(aRuleID, EFalse, aDomain);
	if(manyNode!=NULL)
	    {
        TInt err = manyNode->Find(KXdmExcept, nodes);
        TInt exceptNodeCount = nodes.Count();
        
        //go through all except nodes
        for (TInt i=0;i<exceptNodeCount;i++)
            {
            attrId = (nodes[i])->Attribute(KXdmId);
            
            // for now we are supposing that except has only 1 id
            if(attrId!=NULL)
                {
                OPENG_DP(D_OPENG_LIT( "     RemoveFromModelL: i=%d" ),i);
                iPresXDM->RemoveFromModelL(attrId);
                    
                //if except node is empty after this
                if( (nodes[i])->AttributeCount() == 0)
                    {
                    // remove except node also
                    iPresXDM->RemoveFromModelL(nodes[i]);
                    }
                }
            attrId = NULL;
            }
        ret = KErrNone;
	    }

	OPENG_DP(D_OPENG_LIT( "     return: %d"),ret);
	nodes.Close();
	return ret;
    }
                                               
// ---------------------------------------------------------------------------
// TPresCondIdentityMany::IsIdentityExistsInManyExceptL()
// ---------------------------------------------------------------------------
//    
EXPORT_C TBool TPresCondIdentityMany::IsIdentityExistsInManyExceptL(const TDesC& aRuleID, 
                                        const TDesC& aId, const TDesC& aDomain)
    {
    OPENG_DP(D_OPENG_LIT( "TPresCondIdentityMany::IsIdentityExistsInManyExcept()" ) );
    OPENG_DP(D_OPENG_LIT( "     aRuleID:%S,aDomain:%S,aId:%S"),
                                                    &aRuleID, &aDomain, &aId);
    
    TBool ret(EFalse);
	RPointerArray<CXdmDocumentNode> nodes;
    CXdmNodeAttribute* attrId(NULL);
	
	CXdmDocumentNode* manyNode = GetManyNodeL(aRuleID, EFalse, aDomain);
	if(manyNode!=NULL)
	    {
        TInt err = manyNode->Find(KXdmExcept, nodes);
        TInt exceptNodeCount = nodes.Count();
        
        //go through all except nodes
        for (TInt i=0;i<exceptNodeCount;i++)
            {
            attrId = (nodes[i])->Attribute(KXdmId);
            
            // for now we are supposing that except has only 1 id
            if(attrId!=NULL)
                {
                if (aId==(attrId->AttributeValue()))
                    {
                    OPENG_DP(D_OPENG_LIT( "     Found: i=%d" ),i);
                    ret = ETrue;
                    break;               
                    }
                }
            attrId = NULL;
            }
	    }

    nodes.Close();    
    return ret;
    }
	
// ---------------------------------------------------------------------------
// TPresCondIdentityMany::GetManyNodeL()
// ---------------------------------------------------------------------------
//   
CXdmDocumentNode* TPresCondIdentityMany::GetManyNodeL(const TDesC& aRuleID,  
                                    TBool aCreate, const TDesC& aDomain)
    {
    OPENG_DP(D_OPENG_LIT( " TPresCondIdentityMany::GetManyNodeL()" ) );
    OPENG_DP(D_OPENG_LIT( "     GetManyNodeL aRuleID:%S, aDomain:%S, aCreate:%d"),
                                                     &aRuleID, &aDomain, aCreate);
                                                     
    __ASSERT_ALWAYS(iPresXDM, User::Leave(KErrNotReady));                                                 
                                                     
    CXdmDocumentNode* identityNode = iPresXDM->GetConditionChildNodeL(aRuleID, 
	                                                KXdmIdentity, aCreate);
    if (identityNode==NULL)
        return NULL;
    
    RPointerArray<CXdmDocumentNode> nodes;
    CXdmDocumentNode* manyNode(NULL);
    CXdmNodeAttribute* domainNameAttr(NULL);

    // Finding many node
	identityNode->Find(KXdmMany, nodes);
	TInt manyNodesCount = nodes.Count();
    OPENG_DP(D_OPENG_LIT( "     manyNodesCount = %d"), manyNodesCount);
    TBool found(EFalse);
	if(manyNodesCount) // if some nodes exists
	    {
	    for (TInt i=0;i<manyNodesCount;i++)
	        {
	        domainNameAttr = (nodes[i])->Attribute(KXdmDomain);
	        // for the case when no domain specified
	        if (aDomain==KNullDesC)
	            {
	            if(domainNameAttr == NULL)
	                {
	                found = ETrue;
	                manyNode = nodes[i];
	                break;
	                }
	            }
	        // for the case when domain is specified
	        else if(domainNameAttr!=NULL)
	            {
	            if( (domainNameAttr->AttributeValue()) == aDomain)
	                {
	                found = ETrue;
	                manyNode = nodes[i];
	                break;
	                }
	            }
	        }
	    }
	
	if (aCreate && (!found)) // if asked to create and still doesnt found
	    {
	    manyNode = identityNode->CreateChileNodeL(KXdmMany);
	    if(aDomain!=KNullDesC)
	        {
            domainNameAttr = manyNode->CreateAttributeL(KXdmDomain);
            domainNameAttr->SetAttributeValueL(aDomain);
	        }
	    }

    nodes.Close();        
    return manyNode;   
    }
    
// ---------------------------------------------------------------------------
// TPresCondIdentityMany::IsEntityExistInNodeArray()
// ---------------------------------------------------------------------------
//
TBool TPresCondIdentityMany::IsEntityExistInNodeArray(
                        const RPointerArray<CXdmDocumentNode>& nodes, 
                                                    const TDesC& aId)
    {
    OPENG_DP(D_OPENG_LIT( " TPresCondIdentityMany::IsEntityExistInNodeArray()" ) );
    OPENG_DP(D_OPENG_LIT( "     IsEntityExistInNodeArray aId = %S"), &aId);

    TInt idsCountInNode = nodes.Count();
    CXdmNodeAttribute* idAttr(NULL);
    for (TInt i=0;i<idsCountInNode;i++)
        { // get the id attribute
        idAttr = (nodes[i])->Attribute(KXdmEntity);
        if (aId==(idAttr->AttributeValue()))
            {
            OPENG_DP(D_OPENG_LIT( "     IsEntityExistInNodeArray True"));
            return ETrue;               
            }
        }
    return EFalse;  
    }
    

// end of file

