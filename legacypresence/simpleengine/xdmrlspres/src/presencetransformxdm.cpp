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
* Description:    RLS and Presence XDM, This class represents Presence Transform
*
*/




//Includes
#include <utf.h>
#include <XdmDocumentNode.h>
#include <XdmNodeAttribute.h>
#include <xcapappusagedef.h>

#include "cpresencexdm.h"
#include "presencetransformxdm.h"
#include "rlspresxdmlogger.h"
#include "rlspresxdmconstsint.h"



// ---------------------------------------------------------------------------
// TPresenceTransformXDM::TPresenceTransformXDM()
// ---------------------------------------------------------------------------
//
EXPORT_C TPresenceTransformXDM::TPresenceTransformXDM():iPresXDM(NULL)
    {
    OPENG_DP(D_OPENG_LIT( "TPresenceTransformXDM::TPresenceTransformXDM()" ) );
    }
    
// ---------------------------------------------------------------------------
// TPresenceTransformXDM::Init()
// ---------------------------------------------------------------------------
//
EXPORT_C void TPresenceTransformXDM::Init(CPresenceXDM* const aPresXDM)
    {
    OPENG_DP(D_OPENG_LIT( "TPresenceTransformXDM::Init(%d)"),aPresXDM);
    iPresXDM = aPresXDM;
    }
    
// ---------------------------------------------------------------------------
// TPresenceTransformXDM::~TPresenceTransformXDM()
// ---------------------------------------------------------------------------
//
EXPORT_C TInt TPresenceTransformXDM::AddTransComplexL(const TDesC& aRuleID, 
                               const TDesC& aComplexChild, 
                               const RArray<TTransDataCompo>& aTypeAndValues)
	{
    OPENG_DP(D_OPENG_LIT( "TPresenceTransformXDM::AddTransComplexL" ) );
    OPENG_DP(D_OPENG_LIT( "		aRuleID(%S), aComplexChild(%S), aTypeAndValuesCount(%d)"),
                            &aRuleID, &aComplexChild,  aTypeAndValues.Count());
	
	// check validity of input data
	if (!((aComplexChild==KXdmProvidePersons)||(aComplexChild==KXdmProvideServices)||
	                                        (aComplexChild==KXdmProvideDevices)))
	    return KErrArgument;
	    
    if(!AreNodeNamesValid(aComplexChild, aTypeAndValues))
        return KErrArgument;
	
	CXdmDocumentNode* complexChildNode = GetTransformChildNodeL(aRuleID, 
	                                                    aComplexChild, ETrue);
	if(complexChildNode==NULL)
	    return KErrNotFound;
	                                                    
    // delete the provide all if they exist
    if (IsProvideAllExistL(aRuleID, aComplexChild))
        {
    	complexChildNode->SetEmptyNode(ETrue);
    	complexChildNode->SetEmptyNode(EFalse);
        }
	 
	CXdmDocumentNode* childNode(NULL);
	TInt childCount = aTypeAndValues.Count();
	TBuf<KNodeNameMaxLength> nodeName;
	for(TInt i=0;i<childCount;i++)
	    {
	    nodeName = (aTypeAndValues[i]).nodeName;
	    childNode = complexChildNode->CreateChileNodeL(nodeName);
	    childNode->SetLeafNode(ETrue);
	    childNode->SetLeafNodeContentL((aTypeAndValues[i]).nodeContent);
        OPENG_DP(D_OPENG_LIT( "		i(%d), nodeName(%S)"), i, &nodeName);	    
	    }
	
	return KErrNone;
	}
	
// ---------------------------------------------------------------------------
// TPresenceTransformXDM::AddOrRepProvideAllTransComplexL()
// ---------------------------------------------------------------------------
//
EXPORT_C TInt TPresenceTransformXDM::AddOrRepProvideAllTransComplexL(
                    const TDesC& aRuleID, const TDesC& aComplexChild)
    {
    OPENG_DP(D_OPENG_LIT( "TPresenceTransformXDM::AddOrRepProvideAllTransComplexL" ) );
    OPENG_DP(D_OPENG_LIT( "		aRuleID(%S), aComplexChild(%S)"),
                                                    &aRuleID, &aComplexChild);

    TBuf<KNodeContentMaxLength> nodeName;
	if (aComplexChild==KXdmProvideServices)
	    nodeName = KPresAllServices;
	else if (aComplexChild==KXdmProvidePersons)
	    nodeName = KPresAllPersons;
	else if (aComplexChild==KXdmProvideDevices)
	    nodeName = KPresAllDevices;
	else if (aComplexChild==KPresProvideAllAttributes)
	    nodeName = KPresProvideAllAttributes;
	else
	    return KErrArgument;

	CXdmDocumentNode* complexChildNode = GetTransformChildNodeL(aRuleID, 
	                                                    aComplexChild, ETrue);
	if(complexChildNode==NULL)
	    return KErrNotFound;
	                                                    
	if (aComplexChild!=KPresProvideAllAttributes)
	    {
	// delete all children from this node
	complexChildNode->SetEmptyNode(ETrue);
	complexChildNode->SetEmptyNode(EFalse);
	
	CXdmDocumentNode* emptyNode = complexChildNode->CreateChileNodeL(nodeName);
	emptyNode->SetEmptyNode(ETrue);
	    }
	else
	    complexChildNode->SetEmptyNode(ETrue);
	
	return KErrNone;
	}
                               
// ---------------------------------------------------------------------------
// TPresenceTransformXDM::GetTransComplexL()
// ---------------------------------------------------------------------------
//
EXPORT_C TInt TPresenceTransformXDM::GetTransComplexL(const TDesC& aRuleID, 
                               const TDesC& aComplexChild, 
                               RArray<TTransDataCompo>& aTypeAndValues)
	{
    OPENG_DP(D_OPENG_LIT( "TPresenceTransformXDM::GetTransComplexL" ) );
    OPENG_DP(D_OPENG_LIT( "		aRuleID(%S), aComplexChild(%S)"),
                                                    &aRuleID, &aComplexChild);
	
	// check validity of input data
	if (!((aComplexChild==KXdmProvidePersons)||(aComplexChild==KXdmProvideServices)||
	                                        (aComplexChild==KXdmProvideDevices)))
	    return KErrArgument;

	CXdmDocumentNode* complexChildNode = GetTransformChildNodeL(aRuleID, 
	                                                    aComplexChild, EFalse);
	                                                    
    if (complexChildNode==NULL)
        return KErrNotFound;
    
    CXdmDocumentNode* childNode(NULL);
    TBuf<KNodeNameMaxLength> nodeName;
    HBufC* nodeContents(NULL);
    TTransDataCompo myDataCompo;
    
    TInt childCount = complexChildNode->NodeCount();
    OPENG_DP(D_OPENG_LIT( "     childCount(%d)" ),childCount );
    
    for (TInt i=0; i<childCount; i++)
        {
        childNode = complexChildNode->ChileNode(i);
        nodeName.Copy(childNode->NodeName());
        (myDataCompo.nodeName).Copy(nodeName);
        
        // get attribute if nodeType support attribute
        if ((nodeName==KPresAllDevices)||(nodeName==KPresAllPersons)||
                                                    (nodeName==KPresAllServices))
            {
            aTypeAndValues.Append(myDataCompo);
            break;
            }
       
        childNode->SetLeafNode(ETrue);
        nodeContents = CnvUtfConverter::ConvertToUnicodeFromUtf8L
                                            (childNode->LeafNodeContent());
        (myDataCompo.nodeContent).Copy(nodeContents->Des());
        aTypeAndValues.Append(myDataCompo);
        delete nodeContents;
        }
    
    return KErrNone;		
	}
	
// ---------------------------------------------------------------------------
// TPresenceTransformXDM::AddOrRepTransKnownBoolL()
// ---------------------------------------------------------------------------
//
EXPORT_C TInt TPresenceTransformXDM::AddOrRepTransKnownBoolL(const TDesC& aRuleID, 
                                const TDesC& aBoolChild, TBool aValue)
 	{
    OPENG_DP(D_OPENG_LIT( "TPresenceTransformXDM::AddOrRepTransKnownBoolL" ) );
    OPENG_DP(D_OPENG_LIT( "		aRuleID(%S), aBoolChild(%S)"),
                                                    &aRuleID, &aBoolChild);
 	if (!IsBoolChildSupported(aBoolChild))
 	    return KErrArgument;
 	
 	return AddOrRepTransBoolL(aRuleID, aBoolChild, aValue);
 	}
 	
// ---------------------------------------------------------------------------
// TPresenceTransformXDM::GetTransKnownBoolL()
// ---------------------------------------------------------------------------
//
EXPORT_C TInt TPresenceTransformXDM::GetTransKnownBoolL(const TDesC& aRuleID, 
                                        const TDesC& aBoolChild, TBool& aValue)
	{
    OPENG_DP(D_OPENG_LIT( "TPresenceTransformXDM::GetTransKnownBoolL" ) );
    OPENG_DP(D_OPENG_LIT( "		aRuleID(%S)"), &aRuleID);
 	if (!IsBoolChildSupported(aBoolChild))
 	    return KErrArgument;
 	
 	return GetTransBoolL(aRuleID, aBoolChild, aValue);
	}

// ---------------------------------------------------------------------------
// TPresenceTransformXDM::AddOrRepTransUnknownBoolL()
// ---------------------------------------------------------------------------
//
EXPORT_C TInt TPresenceTransformXDM::AddOrRepTransUnknownBoolL(
            const TDesC& aRuleID, const TDesC& aChildName, TBool aValue)
	{
    OPENG_DP(D_OPENG_LIT( "TPresenceTransformXDM::AddOrRepTransUnknownBoolL" ) );
    OPENG_DP(D_OPENG_LIT( "		aRuleID(%S), aChildName(%S)"), &aRuleID, &aChildName);
 	return AddOrRepTransBoolL(aRuleID, aChildName, aValue);
	}

// ---------------------------------------------------------------------------
// TPresenceTransformXDM::GetTransUnknownBoolL()
// ---------------------------------------------------------------------------
//
EXPORT_C TInt TPresenceTransformXDM::GetTransUnknownBoolL(const TDesC& aRuleID, 
                                            const TDesC& aChildName, TBool& aValue)
	{
    OPENG_DP(D_OPENG_LIT( "TPresenceTransformXDM::GetTransUnknownBoolL" ) );
    OPENG_DP(D_OPENG_LIT( "		aRuleID(%S), aChildName(%S)"), &aRuleID, &aChildName);
 	return GetTransBoolL(aRuleID, aChildName, aValue);
	}

// ---------------------------------------------------------------------------
// TPresenceTransformXDM::RemoveTransformL()
// ---------------------------------------------------------------------------
//
EXPORT_C TInt TPresenceTransformXDM::RemoveTransformL(const TDesC& aRuleID, 
                                                const TDesC& aChildElement)
	{
    OPENG_DP(D_OPENG_LIT( "TPresenceTransformXDM::RemoveTransformL" ) );
    OPENG_DP(D_OPENG_LIT( "		aRuleID(%S), aChildElement(%S)"), &aRuleID, 
                                                                &aChildElement);
	
	__ASSERT_ALWAYS(iPresXDM, User::Leave(KErrNotReady));
	
	CXdmDocumentNode* myNode(NULL);
	if(aChildElement==KNullDesC) // remove whole transform
	    {
	    myNode = iPresXDM->GetRuleChildNodeL(aRuleID, KXdmTransformations, EFalse);
	    }
	else
	    {
	    myNode = GetTransformChildNodeL(aRuleID, aChildElement, EFalse);
	    }
	if(myNode==NULL)
	    return KErrNotFound;
	iPresXDM->RemoveFromModelL(myNode);
	OPENG_DP(D_OPENG_LIT( " removed" ) );
	return KErrNone;
	}

// ---------------------------------------------------------------------------
// TPresenceTransformXDM::IsTransformExistL()
// ---------------------------------------------------------------------------
//
EXPORT_C TBool TPresenceTransformXDM::IsTransformExistL(const TDesC& aRuleID, 
                                                    const TDesC& aChildElement)
	{
    OPENG_DP(D_OPENG_LIT( "TPresenceTransformXDM::IsTransformExist" ) );
    OPENG_DP(D_OPENG_LIT( "		aRuleID(%S), aChildElement(%S)"), &aRuleID, 
                                                                &aChildElement);

	CXdmDocumentNode* myNode = GetTransformChildNodeL(aRuleID, aChildElement, EFalse);
	if(myNode)
	    {
	    OPENG_DP(D_OPENG_LIT( " True" ) );
	    return ETrue;
	    }
	return EFalse;		
	}

// ---------------------------------------------------------------------------
// TPresenceTransformXDM::GetTransformChildNodeL()
// ---------------------------------------------------------------------------
//
CXdmDocumentNode* TPresenceTransformXDM::GetTransformChildNodeL( const TDesC& aRuleID,
                                        const TDesC& aChild, TBool aCreate)
    {
    OPENG_DP(D_OPENG_LIT( " TPresenceTransformXDM::GetTransformChildNodeL" ) );
    OPENG_DP(D_OPENG_LIT( "		aRuleID(%S), aChild(%S), aCreate(%d)"), &aRuleID, 
                                                                &aChild, aCreate);
                                                                
    __ASSERT_ALWAYS(iPresXDM, User::Leave(KErrNotReady));
                                                                
    CXdmDocumentNode* transformNode = iPresXDM->GetRuleChildNodeL(aRuleID, 
                                                KXdmTransformations, aCreate);
    if ( (!aCreate) && (transformNode==NULL))
        return NULL;

    RPointerArray<CXdmDocumentNode> nodes;
    CXdmDocumentNode* childNode(NULL);

    // Finding transform node
	transformNode->Find(aChild, nodes);
	OPENG_DP(D_OPENG_LIT( "     nodeCount(%d)" ), nodes.Count());
	if(nodes.Count()) // if transform node exist
	    {
	    childNode = nodes[0]; // only one complex child node can exist
	    }
	else if (aCreate) // if asked to create
	    {
	    childNode = transformNode->CreateChileNodeL(aChild);
	    }

    nodes.Close();        
    return childNode;   
    }

// ---------------------------------------------------------------------------
// TPresenceTransformXDM::AreNodeNamesValid()
// ---------------------------------------------------------------------------
//
TBool TPresenceTransformXDM::AreNodeNamesValid(const TDesC& aComplexChild, 
                            const RArray<TTransDataCompo>& aTypeAndValues)
    {
    OPENG_DP(D_OPENG_LIT( " TPresenceTransformXDM::AreNodeNamesValid" ) );
    OPENG_DP(D_OPENG_LIT( "		aComplexChild(%S)"), &aComplexChild);

    TInt nodeCount = aTypeAndValues.Count();
    TBuf<KNodeNameMaxLength> myNodeName;
    TInt i(0);

    if(aComplexChild == KXdmProvideDevices)
        {
        for(i=0;i<nodeCount;i++)
            {
            myNodeName = (aTypeAndValues[i]).nodeName;
            if( (myNodeName!=KPresClass)&&(myNodeName!=KPresOccurenceID)&&
                                                    (myNodeName!=KPresDeviceID))
                return EFalse;
            }
        }
        
    else if (aComplexChild == KXdmProvidePersons)
        {
        for(i=0;i<nodeCount;i++)
            {
            myNodeName = (aTypeAndValues[i]).nodeName;
            if( (myNodeName!=KPresClass)&&(myNodeName!=KPresOccurenceID))
                return EFalse;
            }
        }
        
    else if (aComplexChild == KXdmProvideServices)
        {
        for(i=0;i<nodeCount;i++)
            {
            myNodeName = (aTypeAndValues[i]).nodeName;
            if( (myNodeName!=KPresClass)&&(myNodeName!=KPresOccurenceID)
                &&(myNodeName!=KPresServiceUri)&&(myNodeName!=KPresServiceUriScheme)
                &&(myNodeName!=KPresServiceId) )
                return EFalse;
            }
        }
        
    else
        return EFalse;
    
    OPENG_DP(D_OPENG_LIT( "		True"));
    return ETrue;
    }

// ---------------------------------------------------------------------------
// TPresenceTransformXDM::IsProvideAllExistL()
// ---------------------------------------------------------------------------
//
TBool TPresenceTransformXDM::IsProvideAllExistL(const TDesC& aRuleID,
                                        const TDesC& aComplexChild)
    {
    OPENG_DP(D_OPENG_LIT( " TPresenceTransformXDM::IsProvideAllExist" ) );
    OPENG_DP(D_OPENG_LIT( "		aRuleID(%S), aComplexChild(%S)"), 
                                                    &aRuleID, &aComplexChild);

    CXdmDocumentNode* myComplexChild = GetTransformChildNodeL(aRuleID,
                                               aComplexChild, EFalse);
    if(myComplexChild==NULL)
        return EFalse;
    
    TBuf<KNodeNameMaxLength> nodeName;
	if (aComplexChild==KXdmProvideServices)
	    nodeName = KPresAllServices;
	else if (aComplexChild==KXdmProvidePersons)
	    nodeName = KPresAllPersons;
	else if (aComplexChild==KXdmProvideDevices)
	    nodeName = KPresAllDevices;
	else
	    return EFalse;
	
	if ((myComplexChild->NodeCount()) == 1)
	    {
	    CXdmDocumentNode* provideAllNode = myComplexChild->ChileNode(0);
	    if ((provideAllNode->NodeName()) == nodeName)
	        {
	        OPENG_DP(D_OPENG_LIT( "		True"));
	        return ETrue;  
	        }
	    }
    
    return EFalse;
    }

// ---------------------------------------------------------------------------
// TPresenceTransformXDM::IsBoolChildSupported()
// ---------------------------------------------------------------------------
//
TBool TPresenceTransformXDM::IsBoolChildSupported(const TDesC& aBoolChild)
    {
    OPENG_DP(D_OPENG_LIT( " TPresenceTransformXDM::IsBoolChildSupported" ) );

    if( (aBoolChild==KPresProvideActivities)||
        (aBoolChild==KPresProvideClass)||
        (aBoolChild==KPresProvideDeviceID)||
        (aBoolChild==KPresProvideMood)||
        (aBoolChild==KPresProvidePlaceIs)||
        (aBoolChild==KPresProvidePlaceType)||
        (aBoolChild==KPresProvidePrivacy)||
        (aBoolChild==KPresProvideRelationship)||
        (aBoolChild==KPresProvideSphere)||
        (aBoolChild==KPresProvideStatusIcon)||
        (aBoolChild==KPresProvideTimeOffset)||
        (aBoolChild==KPresProvideNote)||
        (aBoolChild==KPresProvideUnknownAttribute)||
    
        (aBoolChild==KPresProvideWillingness)||
        (aBoolChild==KPresProvideNetworkAvailability)||
        (aBoolChild==KPresProvideSessionParticipation)||
        (aBoolChild==KPresProvideGeoPriv)||
        (aBoolChild==KPresProvideRegistrationState)||
        (aBoolChild==KPresProvideBarringState) )
        return ETrue;
    return EFalse;
    }

// ---------------------------------------------------------------------------
// TPresenceTransformXDM::AddOrRepTransBoolL()
// ---------------------------------------------------------------------------
//
TInt TPresenceTransformXDM::AddOrRepTransBoolL(const TDesC& aRuleID, 
                                const TDesC& aBoolChild, TBool aValue)
    {
    OPENG_DP(D_OPENG_LIT( " TPresenceTransformXDM::AddOrRepTransBoolL" ) );

	CXdmDocumentNode* childNode = GetTransformChildNodeL(aRuleID, 
	                                                    aBoolChild, ETrue);
	if(childNode==NULL)
	    return KErrNotFound;
	                                                    
	TBuf8<KPresRLSChildConstBufLen> childContents;
	if(aValue)
	    childContents = KXdmTrue;
	else
	    childContents = KXdmFalse;       
	                                   
	childNode->SetLeafNode(ETrue);
	childNode->SetLeafNodeContentL(childContents);
	
	return KErrNone;		
    }

// ---------------------------------------------------------------------------
// TPresenceTransformXDM::GetTransBoolL()
// ---------------------------------------------------------------------------
//
TInt TPresenceTransformXDM::GetTransBoolL(const TDesC& aRuleID, 
                                    const TDesC& aBoolChild, TBool& aValue)
    {
    OPENG_DP(D_OPENG_LIT( " TPresenceTransformXDM::GetTransBoolL" ) );

	CXdmDocumentNode* childNode = GetTransformChildNodeL(aRuleID, 
	                                                    aBoolChild, EFalse);
	if (childNode==NULL)
	    return KErrNotFound;                                                   
	
	TInt err(KErrNone);

    childNode->SetLeafNode(ETrue);

    TPtrC8 nodeContents = childNode->LeafNodeContent();

    if(nodeContents == KXdmTrue)
        aValue = ETrue;
    else if (nodeContents == KXdmFalse)
        aValue = EFalse;
    else
        err = KErrNotFound;
	    
	return err;		
    }
    
//end of file
