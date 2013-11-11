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
* Description:    RLS and Presence XDM, This is primary class for Presence XDM
*
*/




// Includes
#include <XdmProtocolInfo.h>
#include <XdmEngine.h>
#include <XdmDocument.h>
#include <XdmDocumentNode.h>
#include <XdmNodeAttribute.h>
#include <e32math.h>
#include <xcapappusagedef.h>

#include "cpresencexdm.h"
#include "mrlspresxdmasynchandler.h"
#include "rlspresxdmlogger.h"
#include "rlspresxdmconstsint.h"

// ---------------------------------------------------------------------------
// CPresenceXDM::NewL()
// ---------------------------------------------------------------------------
//
EXPORT_C CPresenceXDM* CPresenceXDM::NewL( const TInt aSettingId )
	{
    OPENG_DP(D_OPENG_LIT( "CPresenceXDM::NewL" ) );
	CPresenceXDM* self = CPresenceXDM::NewLC(aSettingId);
	CleanupStack::Pop(self);
	return self;		
	}

// ---------------------------------------------------------------------------
// CPresenceXDM::NewLC()
// ---------------------------------------------------------------------------
//
EXPORT_C CPresenceXDM* CPresenceXDM::NewLC( const TInt aSettingId )
    {
    OPENG_DP(D_OPENG_LIT( "CPresenceXDM::NewLC" ) );
    CPresenceXDM* self = new (ELeave) CPresenceXDM();
    CleanupStack::PushL( self );
    self->ConstructL( aSettingId );
    return self;
    }

// ---------------------------------------------------------------------------
// CPresenceXDM::ConstructL()
// ---------------------------------------------------------------------------
//    
void CPresenceXDM::ConstructL(const TInt aSettingId )
    {
    OPENG_DP(D_OPENG_LIT( "CPresenceXDM::ConstructL" ) );
    iXDMProtocolInfo = CXdmProtocolInfo::NewL(aSettingId);
    iXDMEngine = CXdmEngine::NewL( *iXDMProtocolInfo);
    iPresDocument = iXDMEngine->CreateDocumentModelL(KPresDocumentName,
                                                    EXdmOmaPresenceRules);
                                                    
    // put this to some other function
    CreateRootIfNeededL();
    
    CActiveScheduler::Add( this );
    }

// ---------------------------------------------------------------------------
// CPresenceXDM::CPresenceXDM()
// ---------------------------------------------------------------------------
//
CPresenceXDM::CPresenceXDM() : CActive( EPriorityStandard ),
                                            iAsyncReq(EPresXDMNoRequestMade) 
    {
    OPENG_DP(D_OPENG_LIT( "CPresenceXDM::CPresenceXDM" ) );
    }

// ---------------------------------------------------------------------------
// CPresenceXDM::~CPresenceXDM()
// ---------------------------------------------------------------------------
//    
CPresenceXDM::~CPresenceXDM()
    {
    OPENG_DP(D_OPENG_LIT( "CPresenceXDM::~CPresenceXDM" ) );
    if(IsActive())
        Cancel();
    delete iPresDocument;    
    delete iXDMEngine;
    delete iXDMProtocolInfo;
    iPresDocument = NULL;
    }


// ---------------------------------------------------------------------------
// CPresenceXDM::CreateRootIfNeededL()
// ---------------------------------------------------------------------------
//    
void CPresenceXDM::CreateRootIfNeededL()
    {
    OPENG_DP(D_OPENG_LIT( "CPresenceXDM::CreateRootIfNeededL" ) );
    CXdmDocumentNode* myRootNode = iPresDocument->DocumentRoot();
    if (myRootNode)
        {
        myRootNode->SetEmptyNode(EFalse);// allow new data to be created, for xdmengine bug
        return;
        }
    myRootNode = iPresDocument->CreateRootL();
    myRootNode->SetNameL( KXdmRuleset );
    }

// ---------------------------------------------------------------------------
// CPresenceXDM::UpdateToServerL()
// ---------------------------------------------------------------------------
//
EXPORT_C TInt CPresenceXDM::UpdateToServerL
                                    (MRLSPresXDMAsyncHandler* const aHandler)   
	{
    OPENG_DP(D_OPENG_LIT( "CPresenceXDM::UpdateToServerL(%d)" ),aHandler );
    __ASSERT_ALWAYS(aHandler, User::Leave(KErrArgument));
    if (IsActive())
        return KErrAlreadyExists;
    iAsyncReq = EPresXDMUpdateToServer;
    iAsyncHandler = aHandler;
    
    iPresDocument->AppendL();
    iXDMEngine->UpdateL(iPresDocument, iStatus);
    SetActive();
    return KErrNone;
	}

// ---------------------------------------------------------------------------
// CPresenceXDM::UpdateAllFromServerL()
// ---------------------------------------------------------------------------
//	
EXPORT_C TInt CPresenceXDM::UpdateAllFromServerL
                                    (MRLSPresXDMAsyncHandler* const aHandler)
    {
    OPENG_DP(D_OPENG_LIT( "CPresenceXDM::UpdateAllFromServerL(%d)" ),aHandler );
    __ASSERT_ALWAYS(aHandler, User::Leave(KErrArgument));
    if (IsActive())
        return KErrAlreadyExists;
    iPresDocument->ResetContents();
    iPresDocument->FetchDataL();
    iAsyncReq = EPresXDMUpdateFromServer;
    iAsyncHandler = aHandler;
    iXDMEngine->UpdateL(iPresDocument, iStatus);
    SetActive();
    return KErrNone;
    }
    
// ---------------------------------------------------------------------------
// CPresenceXDM::CancelUpdateL()
// ---------------------------------------------------------------------------
//	
EXPORT_C void CPresenceXDM::CancelUpdateL()
    {
    OPENG_DP(D_OPENG_LIT( "CPresenceXDM::CancelUpdateL" ));
    if (IsActive())
        {
        DoCancel();
        }
    }    

// ---------------------------------------------------------------------------
// CPresenceXDM::GetAllRulesL()
// ---------------------------------------------------------------------------
//
EXPORT_C void CPresenceXDM::GetAllRulesL(CDesCArray& aRuleIds)
	{
    OPENG_DP(D_OPENG_LIT( "CPresenceXDM::GetAllRulesL" ) );
    CXdmDocumentNode* myRootNode = iPresDocument->DocumentRoot();
    if (!myRootNode)
        return;
    
    RPointerArray<CXdmDocumentNode> nodes;
    CXdmNodeAttribute* idAttribute(NULL);
    
    // Find all rules under root
    if ((myRootNode->Find(KXdmRule, nodes)) == KErrNone)
        { // go through all found rules
        TInt ruleCountInNode = nodes.Count();
        OPENG_DP(D_OPENG_LIT( "     ruleCountInNode = %d" ),ruleCountInNode );
        for (TInt i=0;i<ruleCountInNode;i++)
            { // get the id attribute
            idAttribute = (nodes[i])->Attribute(KXdmId);
            // push it to given discriptor array
            aRuleIds.AppendL(idAttribute->AttributeValue());
            }
        }
    nodes.Close();
    return;
	}

// ---------------------------------------------------------------------------
// CPresenceXDM::DeleteRuleL()
// ---------------------------------------------------------------------------
//
EXPORT_C TInt CPresenceXDM::DeleteRuleL(const TDesC& aRuleId)
	{
    OPENG_DP(D_OPENG_LIT( "CPresenceXDM::DeleteRuleL(%S)" ),&aRuleId );
	CXdmDocumentNode* ruleNode = IsRuleExistInt(aRuleId);
    if (ruleNode)
        {
        iPresDocument->RemoveFromModelL(ruleNode);
        OPENG_DP(D_OPENG_LIT( "     removed" ) );
        return KErrNone;
        }
    return KErrNotFound;
	}

// ---------------------------------------------------------------------------
// CPresenceXDM::CreateNewRuleL()
// ---------------------------------------------------------------------------
//
EXPORT_C TInt CPresenceXDM::CreateNewRuleL(TDes& aRuleId)
	{
    OPENG_DP(D_OPENG_LIT( "CPresenceXDM::CreateNewRuleL(%S)" ),&aRuleId );
	CreateRootIfNeededL();
	
    if(aRuleId==KNullDesC) // create rule id if needed
        GetUniqueRuleId(aRuleId);
	else if(IsRuleExist(aRuleId)) // if rule id already exist, check it
	    return KErrAlreadyExists;
	
    CXdmDocumentNode* myRootNode = iPresDocument->DocumentRoot();
    if (myRootNode)
        {
        CXdmDocumentNode* ruleNode = myRootNode->CreateChileNodeL(KXdmRule);
        CXdmNodeAttribute* attributeRuleId = ruleNode->CreateAttributeL(KXdmId);
        attributeRuleId->SetAttributeValueL(aRuleId);
        return KErrNone;
        }
    return KErrNotFound;
	}

// ---------------------------------------------------------------------------
// CPresenceXDM::IsRuleExist()
// ---------------------------------------------------------------------------
//
EXPORT_C TBool CPresenceXDM::IsRuleExist(const TDesC& aRuleId)
	{
	OPENG_DP(D_OPENG_LIT( "CPresenceXDM::IsRuleExist(%S)" ),&aRuleId );
	CXdmDocumentNode* ruleNode = IsRuleExistInt(aRuleId);
	if(ruleNode)
	    {
	    OPENG_DP(D_OPENG_LIT( "     True"));
	    return ETrue;
	    }
	return EFalse;
	}

// ---------------------------------------------------------------------------
// CPresenceXDM::DeleteAllRules()
// ---------------------------------------------------------------------------
//    
EXPORT_C void CPresenceXDM::DeleteAllRules()
    {
	OPENG_DP(D_OPENG_LIT( "CPresenceXDM::DeleteAllRules" ));
    CXdmDocumentNode* myRootNode = iPresDocument->DocumentRoot();
    if (!myRootNode)
        return; // return if no rule exists
    
    myRootNode->SetEmptyNode(ETrue); // delete all data
    myRootNode->SetEmptyNode(EFalse);// allow new data to be created
    
    }

// ---------------------------------------------------------------------------
// CPresenceXDM::DeleteEmptyRules()
// ---------------------------------------------------------------------------
//    
EXPORT_C void CPresenceXDM::DeleteEmptyRulesL()
    {
	OPENG_DP(D_OPENG_LIT( "CPresenceXDM::DeleteEmptyRules" ));
    CXdmDocumentNode* myRootNode = iPresDocument->DocumentRoot();
    if (!myRootNode)
        return; // return if no rule exists
        
    RPointerArray<CXdmDocumentNode> nodes;

    // Find all rules under  root Node
    if ((myRootNode->Find(KXdmRule, nodes)) == KErrNone)
        { // go through all found rules
        TInt ruleCountInNode = nodes.Count();
        OPENG_DP(D_OPENG_LIT( "     ruleCountInNode = %d" ),ruleCountInNode );
        
        for (TInt i=0;i<ruleCountInNode;i++)
            {
            if ( ((nodes[i])->NodeCount()) == 0)// if rule is empty
                {
                iPresDocument->RemoveFromModelL(nodes[i]);
                OPENG_DP(D_OPENG_LIT( "     delete at i = %d" ), i);
                }
            }
        }
    nodes.Close();
    }
	
// ---------------------------------------------------------------------------
// CPresenceXDM::IsRuleExistInt()
// ---------------------------------------------------------------------------
//	
CXdmDocumentNode* CPresenceXDM::IsRuleExistInt(const TDesC& aRuleId)
    {
	OPENG_DP(D_OPENG_LIT( " CPresenceXDM::IsRuleExistInt(%S)" ),&aRuleId );
    CXdmDocumentNode* myRootNode = iPresDocument->DocumentRoot();
    if (!myRootNode)
        return NULL;
    
    RPointerArray<CXdmDocumentNode> nodes;
    CXdmDocumentNode* desiredNode(NULL);
    CXdmNodeAttribute* idAttribute(NULL);
    
    // Find all rules under  root Node
    if ((myRootNode->Find(KXdmRule, nodes)) == KErrNone)
        { // go through all found rules
        TInt ruleCountInNode = nodes.Count();
        OPENG_DP(D_OPENG_LIT( "     ruleCountInNode = %d" ),ruleCountInNode );
        for (TInt k=0;k<ruleCountInNode;k++)
            { // get the id attribute
            idAttribute = (nodes[k])->Attribute(KXdmId);
            // match it with given attr
            if (idAttribute->AttributeValue() == aRuleId)
                {
                desiredNode = nodes[k];
                break;
                }
            }
        }
    nodes.Close();
    return desiredNode;        
    }

// ---------------------------------------------------------------------------
// CPresenceXDM::GetRuleChildNodeL()
// ---------------------------------------------------------------------------
//
CXdmDocumentNode* CPresenceXDM::GetRuleChildNodeL(const TDesC& aRuleId, 
                                        const TDesC& aRuleChild, TBool aCreate)
    {
    OPENG_DP(D_OPENG_LIT( " CPresenceXDM::GetRuleChildNodeL()" ) );
    OPENG_DP(D_OPENG_LIT( "     aRuleId = %S, aRuleChild = %S, aCreate = %d"),
                                               &aRuleId, &aRuleChild, aCreate);
    
	CXdmDocumentNode* myRuleNode = IsRuleExistInt(aRuleId);
	if (!myRuleNode)
	    return NULL;
	
    RPointerArray<CXdmDocumentNode> nodes;
    CXdmDocumentNode* ruleChildNode(NULL);

    // Finding rule child node
	myRuleNode->Find(aRuleChild, nodes);
	TInt childCount = myRuleNode->NodeCount();
	TInt position;
    OPENG_DP(D_OPENG_LIT( "     GetruleChildNodeL nodeCount = %d"),nodes.Count());
	if(nodes.Count()) // if rule child node exist
	    {
	    ruleChildNode = nodes[0]; // only one rule child node can exist
	    }
	else if (aCreate) // if asked to create
	    {
	    ruleChildNode = iXDMEngine->CreateDocumentNodeL();
	    ruleChildNode->SetNameL(aRuleChild);
	    
	    if(aRuleChild==KXdmConditions) // if we need to add conditions
            {                          // add always to the first position         
            myRuleNode->InsertChileNodeL(0, ruleChildNode);
            position = 0;
            }
	        
	    else if(aRuleChild==KXdmActions) // if we need to add actions
	        {
	        myRuleNode->Find(KXdmConditions, nodes);
	        if (nodes.Count())
	            {
	            myRuleNode->InsertChileNodeL(1, ruleChildNode);//if conditions exists add after it
	            position = 1;
	            }
	        else
	           {
	           myRuleNode->InsertChileNodeL(0, ruleChildNode);
	           position = 0;  
	           }
	        }

	    else // to add transformations
	        {
	        myRuleNode->InsertChileNodeL(childCount, ruleChildNode);
	        position = childCount;
	        }
	    delete ruleChildNode;
	    ruleChildNode = myRuleNode->ChileNode(position);
	    OPENG_DP(D_OPENG_LIT( "      position = %d"),position);
	    }

    nodes.Close();    
    return ruleChildNode;    
    }
    
// ---------------------------------------------------------------------------
// CPresenceXDM::GetConditionChildNodeL()
// ---------------------------------------------------------------------------
//   
CXdmDocumentNode* CPresenceXDM::GetConditionChildNodeL(const TDesC& aRuleId,
                                     const TDesC& aCondChild, TBool aCreate)
    {
    OPENG_DP(D_OPENG_LIT( " CPresenceXDM::GetConditionChildNodeL()" ) );
    OPENG_DP(D_OPENG_LIT( "      aRuleId = %S, aCondChild = %S, aCreate = %d"),
                                               &aRuleId, &aCondChild, aCreate);
    
    CXdmDocumentNode* conditionNode = GetRuleChildNodeL(aRuleId, KXdmConditions, 
                                                                    aCreate);
    if (!conditionNode)
        return NULL;
    
    RPointerArray<CXdmDocumentNode> nodes;
    CXdmDocumentNode* condChildNode(NULL);


    // Finding condition child node
	conditionNode->Find(aCondChild, nodes);
    OPENG_DP(D_OPENG_LIT( "     GetcondChildNodeL nodeCount = %d"),nodes.Count());
	if(nodes.Count()) // if condition child node exist
	    {
	    condChildNode = nodes[0]; // only one condition child node can exist
	    }
	else if (aCreate) // if asked to create
	    {
	    condChildNode = conditionNode->CreateChileNodeL(aCondChild);
	    }

    nodes.Close();        
    return condChildNode;
    }
    
// ---------------------------------------------------------------------------
// CPresenceXDM::GetUniqueRuleId()
// ---------------------------------------------------------------------------
//    
void CPresenceXDM::GetUniqueRuleId(TDes& aRuleId)
    {
	OPENG_DP(D_OPENG_LIT( " CPresenceXDM::GetUniqueRuleId" ));
    aRuleId.Copy(KPresRuleIDPrefix);
    TInt myNumber;

    // make sure that newly created rule id doesnt exist in the document
    while (IsRuleExist(aRuleId))
        {
        aRuleId.Delete(KPresRuleIDPrefixLength, KPresRuleIDNumLength);
        myNumber = Math::Random() - KPresRuleIdCalc;
        aRuleId.AppendNumFixedWidth(myNumber,EDecimal,KPresRuleIDNumLength);
        }
    OPENG_DP(D_OPENG_LIT( "     aRuleId = %S" ), &aRuleId);
    }


// ---------------------------------------------------------------------------
// CPresenceXDM::DoCancel()
// ---------------------------------------------------------------------------
//
void CPresenceXDM::DoCancel()
    {
	OPENG_DP(D_OPENG_LIT( " CPresenceXDM::DoCancel" ));
    iXDMEngine->CancelUpdate(iPresDocument);
    iAsyncReq = EPresXDMRequestCancelling;      
    }

// ---------------------------------------------------------------------------
// CPresenceXDM::RunL()
// ---------------------------------------------------------------------------
//        
void CPresenceXDM::RunL()
    {
    OPENG_DP(D_OPENG_LIT( " CPresenceXDM::RunL" ));
    OPENG_DP(D_OPENG_LIT( " iAsyncReq = %d, iStatus.Int() = %d" ),
                                                    iAsyncReq,iStatus.Int());
    TInt orig = iAsyncReq;   
    iAsyncReq = EPresXDMNoRequestMade;      

    switch (orig)
        {
        case EPresXDMUpdateToServer:
            iAsyncHandler->HandlePresUpdateDocumentL(iStatus.Int());
            break;
        case EPresXDMUpdateFromServer:
            CreateRootIfNeededL();
            iAsyncHandler->HandlePresUpdateDocumentL(iStatus.Int());
            break;
        case EPresXDMRequestCancelling:
            iAsyncHandler->HandlePresUpdateCancelL(iStatus.Int());
            break;
        case EPresXDMNoRequestMade:
        default:
            break;
        }
    }
    
// ---------------------------------------------------------------------------
// CPresenceXDM::RunError()
// ---------------------------------------------------------------------------
//    
TInt CPresenceXDM::RunError(TInt aError)
    {
    OPENG_DP(D_OPENG_LIT( " CPresenceXDM::RunError" ));
    OPENG_DP(D_OPENG_LIT( "     RunError iAsyncReq(%d), aError(%d)" ),iAsyncReq,
                                                                 aError);
    return KErrNone;
    }    

// ---------------------------------------------------------------------------
// CPresenceXDM::RemoveFromModelL()
// ---------------------------------------------------------------------------
//    
void CPresenceXDM::RemoveFromModelL( CXdmDocumentNode* aNode )
    {
    OPENG_DP(D_OPENG_LIT( " CPresenceXDM::RemoveFromModelL(%d)" ),aNode);
    iPresDocument->RemoveFromModelL(aNode);   
    }
    
// end of file
