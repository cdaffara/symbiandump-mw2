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
* Description:    RLS and Presence XDM, This class represents sphere in a 
*                 Presence Condition
*
*/




// Includes
#include <XdmDocumentNode.h>
#include <XdmNodeAttribute.h>
#include <xcapappusagedef.h>
#include <utf.h>

#include "prescondsphere.h"
#include "cpresencexdm.h"
#include "rlspresxdmlogger.h"
#include "rlspresxdmconstsint.h"


// ---------------------------------------------------------------------------
// TPresCondSphere::TPresCondSphere()
// ---------------------------------------------------------------------------
//	
EXPORT_C TPresCondSphere::TPresCondSphere(): iPresXDM(NULL)
    {
    OPENG_DP(D_OPENG_LIT( "TPresCondSphere::TPresCondSphere()" ));
    }

// ---------------------------------------------------------------------------
// TPresCondSphere::Init()
// ---------------------------------------------------------------------------
//    
EXPORT_C void TPresCondSphere::Init(CPresenceXDM* const aPresXDM)
    {
    OPENG_DP(D_OPENG_LIT( "TPresCondSphere::Init(%d)" ), aPresXDM);
    iPresXDM = aPresXDM;
    }

// ---------------------------------------------------------------------------
// TPresCondSphere::AddOrReplaceSphereL()
// ---------------------------------------------------------------------------
//	
EXPORT_C TInt TPresCondSphere::AddOrReplaceSphereL(const TDesC& aRuleID, 
                                                        const TDesC& aSphere)
	{
    OPENG_DP(D_OPENG_LIT( "TPresCondSphere::AddOrReplaceSphereL()" ) );
    OPENG_DP(D_OPENG_LIT( "     aRuleID = %S, aSphere = %S"),&aRuleID, &aSphere);
	
	CXdmDocumentNode* sphereNode = GetSphereNodeL(aRuleID, ETrue);
	if(sphereNode==NULL)
	    return KErrNotFound;
	
	CXdmNodeAttribute* valueAttr(NULL);
	valueAttr = sphereNode->Attribute(KXdmValue);
	// if Value attribute is not found
	if (valueAttr==NULL)
	    {
	    valueAttr = sphereNode->CreateAttributeL(KXdmValue);
	    }
	    
	valueAttr->SetAttributeValueL(aSphere);
	return KErrNone;
	}

// ---------------------------------------------------------------------------
// TPresCondSphere::GetSphereL()
// ---------------------------------------------------------------------------
//	
EXPORT_C TInt TPresCondSphere::GetSphereL(const TDesC& aRuleID, 
                                                        TPtrC& aSphere)
	{
    OPENG_DP(D_OPENG_LIT( "TPresCondSphere::GetSphere()" ) );
    OPENG_DP(D_OPENG_LIT( "     aRuleID = %S"),&aRuleID);
	
	CXdmDocumentNode* sphereNode = GetSphereNodeL(aRuleID, EFalse);
	if(sphereNode==NULL)
	    return KErrNotFound;
	
	CXdmNodeAttribute* valueAttr = sphereNode->Attribute(KXdmValue);
	
	if(valueAttr)
	    {
	    aSphere.Set(valueAttr->AttributeValue());
	    return KErrNone;
	    }
	    
	return KErrNotFound;
	}

// ---------------------------------------------------------------------------
// TPresCondSphere::DeleteSphereL()
// ---------------------------------------------------------------------------
//	
EXPORT_C TInt TPresCondSphere::DeleteSphereL(const TDesC& aRuleID)
	{
    OPENG_DP(D_OPENG_LIT( "TPresCondSphere::DeleteSphereL()" ) );
    OPENG_DP(D_OPENG_LIT( "     aRuleID = %S"),&aRuleID);

    __ASSERT_ALWAYS(iPresXDM, User::Leave(KErrNotReady));

	CXdmDocumentNode* sphereNode = GetSphereNodeL(aRuleID, EFalse);
	if(sphereNode==NULL)
	    return KErrNotFound;
	
	iPresXDM->RemoveFromModelL(sphereNode);
	
	OPENG_DP(D_OPENG_LIT( "     removed"));	
	return KErrNone;
	}

// ---------------------------------------------------------------------------
// TPresCondSphere::GetSphereNode()
// ---------------------------------------------------------------------------
//    
CXdmDocumentNode* TPresCondSphere::GetSphereNodeL(const TDesC& aRuleID,
                                                        TBool aCreate)
    {
    OPENG_DP(D_OPENG_LIT( " TPresCondSphere::GetSphereNodeL()" ) );
    OPENG_DP(D_OPENG_LIT( "     GetSphereNodeL aRuleID = %S, aCreate = %d"),
                                                            &aRuleID, aCreate);
    
    __ASSERT_ALWAYS(iPresXDM, User::Leave(KErrNotReady));                                                        
    
    CXdmDocumentNode* conditionNode = iPresXDM->GetRuleChildNodeL(aRuleID, 
                                                    KXdmConditions, aCreate);
    if (conditionNode==NULL)
        return conditionNode;

    RPointerArray<CXdmDocumentNode> nodes;
    CXdmDocumentNode* sphereNode(NULL);

    // Finding sphere node
	conditionNode->Find(KXdmSphere, nodes);
    OPENG_DP(D_OPENG_LIT( "     GetSphereNodeL nodeCount = %d"),nodes.Count());
	if(nodes.Count()) // if identity node exist
	    {
	    sphereNode = nodes[0]; // only one sphere node can exist
	    }
	else if (aCreate) // if asked to create
	    {
	    sphereNode = conditionNode->CreateChileNodeL(KXdmSphere);
	    }

    nodes.Close();        
    return sphereNode;   
    }

// end of file

