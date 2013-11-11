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
* Description:    RLS and Presence XDM, This is class for RLS XDM
*
*/




// Includes
#include <utf.h>
#include <XdmProtocolInfo.h>
#include <XdmEngine.h>
#include <XdmDocument.h>
#include <XdmDocumentNode.h>
#include <XdmNodeAttribute.h>
#include <XdmErrors.h>
#include <xcapappusagedef.h>

#include "crlsxdm.h"
#include "mrlspresxdmasynchandler.h"
#include "rlspresxdmlogger.h"
#include "rlspresxdmconstsint.h"


// ---------------------------------------------------------------------------
// CRLSXDM::NewL()
// ---------------------------------------------------------------------------
//
EXPORT_C CRLSXDM* CRLSXDM::NewL( const TInt aSettingId )
	{
  OPENG_DP(D_OPENG_LIT( "CRLSXDM::NewL" ));
	CRLSXDM* self = CRLSXDM::NewLC(aSettingId);
	CleanupStack::Pop(self);
	return self;		
	}
	
// ---------------------------------------------------------------------------
// CRLSXDM::NewLC()
// ---------------------------------------------------------------------------
//
EXPORT_C CRLSXDM* CRLSXDM::NewLC( const TInt aSettingId )
	{
    OPENG_DP(D_OPENG_LIT( "CRLSXDM::NewLC" ));
    CRLSXDM* self = new (ELeave) CRLSXDM();
    CleanupStack::PushL( self );
    self->ConstructL( aSettingId );
    return self;
	}

// ---------------------------------------------------------------------------
// CRLSXDM::ConstructL()
// ---------------------------------------------------------------------------
//	
void CRLSXDM::ConstructL(const TInt aSettingId)
    {
    OPENG_DP(D_OPENG_LIT( "CRLSXDM::ConstructL(%d)" ), aSettingId);
    iXDMProtocolInfo = CXdmProtocolInfo::NewL(aSettingId);
    iXDMEngine = CXdmEngine::NewL( *iXDMProtocolInfo);
    iRLSDocument = iXDMEngine->CreateDocumentModelL(KRLSDocumentName,
                                                    EXdmRlsServices);
                                                    
    CreateRootIfNeededL();
    
    iListPath = new(ELeave)CDesCArraySeg(KPresRLSDesArrayInitlVal);
    CActiveScheduler::Add( this );
    }

// ---------------------------------------------------------------------------
// CRLSXDM::CreateRootIfNeededL()
// ---------------------------------------------------------------------------
//    
void CRLSXDM::CreateRootIfNeededL()
    {
    OPENG_DP(D_OPENG_LIT( "CRLSXDM::CreateRootIfNeededL" ));
    CXdmDocumentNode* myRootNode = iRLSDocument->DocumentRoot();
    if (myRootNode)
        {
        myRootNode->SetEmptyNode(EFalse);// allow new data to be created, for xdmengine bug
        return;
        }
    myRootNode = iRLSDocument->CreateRootL();
    myRootNode->SetNameL( KXdmRlsServices );
    }

// ---------------------------------------------------------------------------
// CRLSXDM::CRLSXDM()
// ---------------------------------------------------------------------------
//    
CRLSXDM::CRLSXDM() : CActive( EPriorityStandard ), 
                                            iAsyncReq(ERLSXDMNoRequestMade)
    {
    OPENG_DP(D_OPENG_LIT( "CRLSXDM::CRLSXDM" ));
    }
    
// ---------------------------------------------------------------------------
// CRLSXDM::~CRLSXDM()
// ---------------------------------------------------------------------------
//
CRLSXDM::~CRLSXDM()
    {
    OPENG_DP(D_OPENG_LIT( "CRLSXDM::~CRLSXDM()" ));
    if(IsActive())
        Cancel();
    
    delete iRLSDocument;
    delete iXDMEngine;
    delete iXDMProtocolInfo;
    iRLSDocument = NULL;
    
    if(iListPath)
        iListPath->Reset();
    delete iListPath;
    
    iListPointerPath.Reset();
    
    delete iNegotiatedServiceUri;
    }

// ---------------------------------------------------------------------------
// CRLSXDM::UpdateToServerL()
// ---------------------------------------------------------------------------
//    
EXPORT_C TInt CRLSXDM::UpdateToServerL(MRLSPresXDMAsyncHandler* const aHandler)
	{
    OPENG_DP(D_OPENG_LIT( "CRLSXDM::UpdateToServerL(%d)" ), aHandler);
    __ASSERT_ALWAYS(aHandler, User::Leave(KErrArgument));
    if (IsActive())
        return KErrAlreadyExists;
    iAsyncReq = ERLSXDMUpdateToServer;
    iAsyncHandler = aHandler;
    iRLSDocument->AppendL();
    iXDMEngine->UpdateL(iRLSDocument, iStatus);
    SetActive();
    return KErrNone;
	}

// ---------------------------------------------------------------------------
// CRLSXDM::CancelUpdateL()
// ---------------------------------------------------------------------------
//	
EXPORT_C void CRLSXDM::CancelUpdateL()
    {
    OPENG_DP(D_OPENG_LIT( "CRLSXDM::CancelUpdateL" ));
    if (IsActive())
        {
        DoCancel();
        }
    }

// ---------------------------------------------------------------------------
// CRLSXDM::UpdateAllFromServerL()
// ---------------------------------------------------------------------------
//
EXPORT_C TInt CRLSXDM::UpdateAllFromServerL(MRLSPresXDMAsyncHandler* const aHandler)
    {
    OPENG_DP(D_OPENG_LIT( "CRLSXDM::UpdateAllFromServerL(%d)" ));
    __ASSERT_ALWAYS(aHandler, User::Leave(KErrArgument));
    if (IsActive())
        return KErrAlreadyExists;
    
    iAsyncHandler = aHandler;
    iRLSDocument->ResetContents();
    iRLSDocument->FetchDataL();
    iAsyncReq = ERLSXDMUpdateFromServer;
    
    iXDMEngine->UpdateL(iRLSDocument, iStatus);
   
    SetActive();
    return KErrNone;
    }

// ---------------------------------------------------------------------------
// CRLSXDM::DeleteAll()
// ---------------------------------------------------------------------------
//    
EXPORT_C void CRLSXDM::DeleteAll()
    {
    OPENG_DP(D_OPENG_LIT( "CRLSXDM::DeleteAllL"));
    
    // resetting current path
    iListPath->Reset();
    iListPointerPath.Reset();

    CXdmDocumentNode* myRootNode = iRLSDocument->DocumentRoot();
    if (myRootNode==NULL)
        return;
    
    myRootNode->SetEmptyNode(ETrue); // delete all data
    myRootNode->SetEmptyNode(EFalse);// allow new data to be created
    }

// ---------------------------------------------------------------------------
// CRLSXDM::AddServiceURIL()
// ---------------------------------------------------------------------------
//
EXPORT_C TInt CRLSXDM::AddServiceURIL(const TDesC& aServiceURI)
	{
    OPENG_DP(D_OPENG_LIT( "CRLSXDM::AddServiceURIL(%S)" ),&aServiceURI);
    CXdmDocumentNode* myRootNode = iRLSDocument->DocumentRoot();
    if (myRootNode==NULL)
        return KErrNotFound;
    
    //check if already exist    
    if (IsServiceUriExist(aServiceURI, myRootNode) != NULL)
        return KErrAlreadyExists;
    
    //workaround
    myRootNode->SetEmptyNode(EFalse);// allow new data to be created
    
    CXdmDocumentNode* listServNode = myRootNode->CreateChileNodeL(KXdmService);
    CXdmNodeAttribute* attributeUri = listServNode->CreateAttributeL(KXdmUri);
    attributeUri->SetAttributeValueL(aServiceURI);
    
    // now add the packages
    CXdmDocumentNode* packagesNode = listServNode->CreateChileNodeL(KXdmPackages);
    CXdmDocumentNode* packageNode = packagesNode->CreateChileNodeL(KXdmPackage);
    packageNode->SetLeafNode(ETrue);
    packageNode->SetLeafNodeContentL(KXdmPresence);
    return KErrNone;
    }

// ---------------------------------------------------------------------------
// CRLSXDM::RemoveServiceURIL()
// ---------------------------------------------------------------------------
//
EXPORT_C TInt CRLSXDM::RemoveServiceURIL(const TDesC& aServiceURI)
	{
    OPENG_DP(D_OPENG_LIT( "CRLSXDM::RemoveServiceURIL(%S)" ),&aServiceURI);
    CXdmDocumentNode* myRootNode = iRLSDocument->DocumentRoot();
    TInt err(KErrNotFound);
    if (myRootNode!=NULL)
        {
        CXdmDocumentNode* serviceNode = IsServiceUriExist(aServiceURI, myRootNode);
        if(serviceNode) // if exist
            {
            iRLSDocument->RemoveFromModelL(serviceNode);
            err = KErrNone;
            }
        }
    // check valdity of path if operation is successful
    if(iListPath->Count() && (err==KErrNone))
        {
        if( (iListPath->MdcaPoint(0)) == aServiceURI)
            {
            // resetting current path
            iListPath->Reset();
            iListPointerPath.Reset();
            }
        }
        
    OPENG_DP(D_OPENG_LIT( "     returned: %d" ),err);
    return err;
	}

// ---------------------------------------------------------------------------
// CRLSXDM::IsExistServiceURI()
// ---------------------------------------------------------------------------
//
EXPORT_C TBool CRLSXDM::IsExistServiceURI(const TDesC& aServiceURI)
	{
    OPENG_DP(D_OPENG_LIT( "CRLSXDM::IsExistServiceURI(%S)" ),&aServiceURI);
    CXdmDocumentNode* myRootNode = iRLSDocument->DocumentRoot();
    TBool isExist(EFalse);
    if (myRootNode!=NULL)
        {
        CXdmDocumentNode* serviceNode = IsServiceUriExist(aServiceURI, myRootNode);
        if(serviceNode) // if exist
            {
            isExist = ETrue;
            }
        }

    OPENG_DP(D_OPENG_LIT( "     returned: %d" ),isExist);
    return isExist;
	}

// ---------------------------------------------------------------------------
// CRLSXDM::GetServiceUris()
// ---------------------------------------------------------------------------
//
EXPORT_C void CRLSXDM::GetServiceUrisL(CDesCArray& aServiceUris)
    {
    OPENG_DP(D_OPENG_LIT( "CRLSXDM::GetServiceUris" ));
    CXdmDocumentNode* myRootNode = iRLSDocument->DocumentRoot();
    RPointerArray<CXdmDocumentNode> nodes;
    CXdmNodeAttribute* uriAttribute(NULL);

    if (myRootNode!=NULL)
        {
        // Find all Service Uris under root Node
        if ((myRootNode->Find(KXdmService, nodes)) == KErrNone)
            { // go through all found Uris
            TInt uriCountInNode = nodes.Count();
            OPENG_DP(D_OPENG_LIT( "     uriCountInNode(%d)" ),uriCountInNode);
            for (TInt k=0;k<uriCountInNode;k++)
                { // get the uri attribute
                uriAttribute = (nodes[k])->Attribute(KXdmUri);
                // push it to given discriptor array
                aServiceUris.AppendL(uriAttribute->AttributeValue());
                }
            }
        }
    nodes.Close();
    }

// ---------------------------------------------------------------------------
// CRLSXDM::DeleteAllEmptyServiceUrisL()
// ---------------------------------------------------------------------------
//	
EXPORT_C void CRLSXDM::DeleteAllEmptyServiceUrisL()
    {
    OPENG_DP(D_OPENG_LIT( "CRLSXDM::DeleteAllEmptyServiceUrisL" ));
    RPointerArray<CXdmDocumentNode> uriNodes;
    
    // resetting current path
    iListPath->Reset();
    iListPointerPath.Reset();

    TInt found = iRLSDocument->Find( KXdmService, uriNodes);
    OPENG_DP(D_OPENG_LIT( "     total Uris: %d" ), found);
    
    for (TInt i=0;i<found;i++)
        {
        if ( ((uriNodes[i])->NodeCount()) <= 1)// if Uri have only package node
            {
            iRLSDocument->RemoveFromModelL(uriNodes[i]);
            OPENG_DP(D_OPENG_LIT( "     delete at i = %d" ), i);
            }
        }
    uriNodes.Close();    
    }

// ---------------------------------------------------------------------------
// CRLSXDM::RenameListL()
// ---------------------------------------------------------------------------
//	
EXPORT_C TInt CRLSXDM::RenameListL(const TDesC& aListName, 
                                                    const TDesC& aListNewName)
	{
	OPENG_DP(D_OPENG_LIT( "CRLSXDM::RenameListL(%S)" ),&aListName);
	TInt err(KErrNotFound);
	TInt count = iListPointerPath.Count();
	if (count)
	    {
	    CXdmDocumentNode* list = GetNonLeafChild(KXdmList,aListName,
                                                    iListPointerPath[count-1]);
	    if (list!=NULL) // if found
            { // if new name already exist
            CXdmDocumentNode* newList = GetNonLeafChild(KXdmList, aListNewName,
                                                        iListPointerPath[count-1]);
            if (newList)
                err = KErrAlreadyExists;
            else
                {
         	    CXdmNodeAttribute* myAttribute = list->Attribute(KXdmName);
        	    myAttribute->SetAttributeValueL(aListNewName);
        	    err = KErrNone;
                }
            }
        }
    OPENG_DP(D_OPENG_LIT( "     returned: %d" ),err);
    return err;
	}

// ---------------------------------------------------------------------------
// CRLSXDM::DeleteAllEmptyLists()
// ---------------------------------------------------------------------------
//
EXPORT_C TInt CRLSXDM::DeleteAllEmptyLists(MRLSPresXDMAsyncHandler* const aHandler)
    {
    OPENG_DP(D_OPENG_LIT( "CRLSXDM::DeleteAllEmptyListsL(%d)" ), aHandler);

    if(aHandler==NULL)
        return KErrArgument;
    
    if (IsActive())
        return KErrAlreadyExists;
    
    // resetting current path
    iListPath->Reset();
    iListPointerPath.Reset();

    iAsyncReq = ERLSXDMDeletingEmptyLists;
    iAsyncHandler = aHandler;
    TRequestStatus* status = &iStatus;
    User::RequestComplete(status,KErrNone);
    SetActive();
    return KErrNone;
    }

// ---------------------------------------------------------------------------
// CRLSXDM::AddElementL()
// ---------------------------------------------------------------------------
//
EXPORT_C TInt CRLSXDM::AddElementL(const TDesC& aElementType,
                        const TDesC& aData, const TDesC& aDisplayName)
    {
	OPENG_DP(D_OPENG_LIT( "CRLSXDM::AddElementL" ));
	OPENG_DP(D_OPENG_LIT( "     aElementType(%S), aData(%S), aDisplayName(%S)"),
	                                &aElementType, &aData, &aDisplayName);
	TInt err(KErrNone);
    TInt count = iListPointerPath.Count();

	if(aElementType==KXdmResourceList)
	    {
    	if (count) // should be atleast 1, so that resource-list reside inside serviceUri
    	    {
    	    CXdmDocumentNode* list = GetLeafChild(aElementType, aData,
                                                        iListPointerPath[count-1]);
    	    if (!list) // if not found
	            {
        	    if(count==1)// if the package on same level
        	        RemovePackageL();
                AddLeafElementL(KXdmResourceList, aData, iListPointerPath[count-1]);	    
                if(count==1)// if the package on same level
        	        AddPackageL(); // so that package is the last element on service-uri
        	    err = KErrNone;
	            }
	        else
                err = KErrAlreadyExists;
            }
        else
            err = KErrPermissionDenied; // path is not suitable
	    }
	else if(IsNonLeafElementValid(aElementType))
	    {
	    if( (count>1) || ((aElementType==KXdmList)&&(count)) )
	        {
	        CXdmDocumentNode* element = GetNonLeafChild(aElementType, aData,
                                                        iListPointerPath[count-1]);
            if(!element) // if element doesnt exist
	            {
        	    if(count==1)// if the package on same level
        	        RemovePackageL();
                AddNonLeafElementL(aElementType, aData, aDisplayName, iListPointerPath[count-1]);	    
                if(count==1)// if the package on same level
        	        AddPackageL(); // so that package is the last element on service-uri
        	    err = KErrNone;
	            }
            else
                err = KErrAlreadyExists;
	        }
        else
            err = KErrPermissionDenied; // path is not suitable
	    }
	else
	    err = KErrArgument;
	
	OPENG_DP(D_OPENG_LIT( " return: err(%d)" ), err);
	return err;
    }

// ---------------------------------------------------------------------------
// CRLSXDM::GetDisplayName()
// ---------------------------------------------------------------------------
//
EXPORT_C TInt CRLSXDM::GetDisplayName(const TDesC& aElementType,
                                const TDesC& aData, TPtrC8& aDisplayName)
    {
	OPENG_DP(D_OPENG_LIT( "CRLSXDM::GetDisplayName" ));
	OPENG_DP(D_OPENG_LIT( "     aElementType(%S), aData(%S)"),
	                                    &aElementType, &aData);
	
	TInt err(KErrNotFound);
	
	TInt count = iListPointerPath.Count();
	if (count) // if there is atleast service uri
	    {
	    if(IsNonLeafElementValid(aElementType))
	        {
	        err = GetDisplayNameInt(aElementType, aData, aDisplayName,
                                            iListPointerPath[count-1]);
	        }
	    else
	        err = KErrArgument;
        }
    return err;     
    }

// ---------------------------------------------------------------------------
// CRLSXDM::UpdateDisplayNameL()
// ---------------------------------------------------------------------------
//
EXPORT_C TInt CRLSXDM::UpdateDisplayNameL(const TDesC& aElementType,  
                        const TDesC& aData, const TDesC& aDisplayName)
    {
	OPENG_DP(D_OPENG_LIT( "CRLSXDM::UpdateDisplayNameL" ));
	OPENG_DP(D_OPENG_LIT( "     aElementType(%S), aData(%S), aDisplayName(%S)"),
	                                    &aElementType, &aData, &aDisplayName);
	
	TInt err(KErrNotFound);
	
	TInt count = iListPointerPath.Count();
	if (count) // if there is atleast service uri
	    {
	    if(IsNonLeafElementValid(aElementType))
	        {
	        err = UpdateDisplayNameIntL(aElementType, aData, aDisplayName,
                                            iListPointerPath[count-1]);
	        }
	    else
	        err = KErrArgument;
        }
    return err;     
    }

// ---------------------------------------------------------------------------
// CRLSXDM::GetElementsL()
// ---------------------------------------------------------------------------
//
EXPORT_C void CRLSXDM::GetElementsL(const TDesC& aElementType, 
                                                    CDesCArray& aValues)
    {
	OPENG_DP(D_OPENG_LIT( "CRLSXDM::GetElementsL" ));
	OPENG_DP(D_OPENG_LIT( "     aElementType(%S)"),&aElementType);
	
	TInt count = iListPointerPath.Count();
	if (count) // if there is atleast service uri
	    {
	    if(IsNonLeafElementValid(aElementType))
	        GetNonLeafElementsL(aElementType, aValues, iListPointerPath[count-1]);
	    else if(aElementType==KXdmResourceList)
	        GetLeafElementsL(aElementType, aValues, iListPointerPath[count-1]);
        }
    }
            
// ---------------------------------------------------------------------------
// CRLSXDM::RemoveElementL()
// ---------------------------------------------------------------------------
//
EXPORT_C TInt CRLSXDM::RemoveElementL(const TDesC& aElementType, 
                                                    const TDesC& aData)
    {
	OPENG_DP(D_OPENG_LIT( "CRLSXDM::RemoveElementL" ));
	OPENG_DP(D_OPENG_LIT( "     aElementType(%S), aData(%S)"),
	                                            &aElementType, &aData);
	TInt err(KErrNotFound);
    TInt count = iListPointerPath.Count();
    CXdmDocumentNode* myNode(NULL);
    
    if(count)
        {
      	if(aElementType==KXdmResourceList)
      	    myNode = GetLeafChild(aElementType,aData,iListPointerPath[count-1]);
      	else if(IsNonLeafElementValid(aElementType))
      	    myNode = GetNonLeafChild(aElementType,aData,iListPointerPath[count-1]);
      	else
      	    err = KErrArgument;
        }

    if (myNode) // if found
        {
        iRLSDocument->RemoveFromModelL(myNode);
        err = KErrNone;
        }
	
	OPENG_DP(D_OPENG_LIT( " return: err(%d)" ), err);
	return err;
    }

// ---------------------------------------------------------------------------
// CRLSXDM::IsElementExist()
// ---------------------------------------------------------------------------
//
EXPORT_C TBool CRLSXDM::IsElementExist(const TDesC& aElementType, 
                                                    const TDesC& aData)
    {
	OPENG_DP(D_OPENG_LIT( "CRLSXDM::IsElementExist" ));
	OPENG_DP(D_OPENG_LIT( "     aElementType(%S), aData(%S)"),
	                                            &aElementType, &aData);
	TBool ret(EFalse);
    TInt count = iListPointerPath.Count();
    CXdmDocumentNode* myNode(NULL);
    
    if(count)
        {
      	if(aElementType==KXdmResourceList)
      	    myNode = GetLeafChild(aElementType,aData,iListPointerPath[count-1]);
      	else if(IsNonLeafElementValid(aElementType))
      	    myNode = GetNonLeafChild(aElementType,aData,iListPointerPath[count-1]);
        }

    if (myNode) // if found
        {
        ret = ETrue;
        }
	
	OPENG_DP(D_OPENG_LIT( " return: err(%d)" ), ret);
	return ret;
    }
	
// ---------------------------------------------------------------------------
// CRLSXDM::SwitchToServiceURI()
// ---------------------------------------------------------------------------
//	
EXPORT_C TInt CRLSXDM::SwitchToServiceURIL(const TDesC& aServiceURI)
	{
	OPENG_DP(D_OPENG_LIT( "CRLSXDM::SwitchToServiceURI(%S)" ),&aServiceURI);
	TInt err(KErrNotFound);
    CXdmDocumentNode* myRootNode = iRLSDocument->DocumentRoot();
    if (myRootNode==NULL)
        return err;
    
    CXdmDocumentNode* serviceUriNode = IsServiceUriExist( aServiceURI, myRootNode);
    if (serviceUriNode==NULL)
        return KErrNotFound;
    
    iListPointerPath.Reset();
    iListPath->Reset();
    
    iListPointerPath.Append(serviceUriNode); // append pointer to node
    iListPath->AppendL(aServiceURI); // append new list path
    return KErrNone;		
	}

// ---------------------------------------------------------------------------
// CRLSXDM::SwitchToList()
// ---------------------------------------------------------------------------
//	
EXPORT_C TInt CRLSXDM::SwitchToListL(const TDesC& aListName)
	{
	OPENG_DP(D_OPENG_LIT( "CRLSXDM::SwitchToList(%S)" ),&aListName);
	TInt err(KErrNotFound);
	TInt count = iListPointerPath.Count();
	if (count)
	    {
	    CXdmDocumentNode* list = GetNonLeafChild(KXdmList,aListName,
                                                    iListPointerPath[count-1]);
	    if (list) // if found
	        {
    	    iListPointerPath.Append(list); // append pointer to node
            iListPath->AppendL(aListName); // append new list path
            err = KErrNone;
	        }
        }
    OPENG_DP(D_OPENG_LIT( "     returned: %d" ),err);    
    return err;        
	}

// ---------------------------------------------------------------------------
// CRLSXDM::SwitchOutFromList()
// ---------------------------------------------------------------------------
//	
EXPORT_C void CRLSXDM::SwitchOutFromList()
	{
	OPENG_DP(D_OPENG_LIT( "CRLSXDM::SwitchOutFromList" ));
	TInt count = iListPath->Count();
	if(count) // if some path exist
        {
        iListPath->Delete(count-1); // remove last list name
        iListPointerPath.Remove(count-1); // remove last node pointer
        OPENG_DP(D_OPENG_LIT( "     switched out" ));
        }
	}

// ---------------------------------------------------------------------------
// CRLSXDM::GetCurrentPathL()
// ---------------------------------------------------------------------------
//	
EXPORT_C void CRLSXDM::GetCurrentPathL(CDesCArray& aPath)
	{
	OPENG_DP(D_OPENG_LIT( "CRLSXDM::GetCurrentPath" ));
	aPath.Reset();
	TInt count = iListPath->Count();
    for (TInt i=0;i<count;i++)
        {
        aPath.AppendL(iListPath->MdcaPoint(i));
        }
    OPENG_DP(D_OPENG_LIT( "     aPathCount = %d" ),aPath.Count());
	}
	
// ---------------------------------------------------------------------------
// CRLSXDM::SetCurrentpath()
// ---------------------------------------------------------------------------
//
EXPORT_C TInt CRLSXDM::SetCurrentpathL(const MDesCArray& aLists)
	{
	OPENG_DP(D_OPENG_LIT( "CRLSXDM::SetCurrentpath" ));
    CXdmDocumentNode* myRootNode = iRLSDocument->DocumentRoot();
    if (myRootNode==NULL)
        return KErrNotFound;
    
    CXdmDocumentNode* serviceUriNode(NULL);
    
    TInt count = aLists.MdcaCount();

    if(count) // if there is atleast one element(service-uri) in path info
        {
        serviceUriNode = IsServiceUriExist( aLists.MdcaPoint(0), myRootNode);
        if (serviceUriNode==NULL)
            {
            OPENG_DP(D_OPENG_LIT( "     service-uri doesnt exist:%d" ),KErrNotFound);
            return KErrNotFound;
            }
        }
    
    RPointerArray<CXdmDocumentNode> confirmedNodes;
    
    CXdmDocumentNode* currentNode = serviceUriNode;
    confirmedNodes.Append(currentNode);
    
    if(count>1) // if some list(s) exist in path info
        {
        for (TInt i=1; i<count; i++)
            {
            currentNode = GetNonLeafChild(KXdmList, aLists.MdcaPoint(i),
                                                                     currentNode);
            if (currentNode==NULL)
                {
                confirmedNodes.Close();
                OPENG_DP(D_OPENG_LIT( "     path incorrect at i:%d" ),i);
                return KErrNotFound;
                }
            confirmedNodes.Append(currentNode);
            }
        }
    
    // since path is verified, update local path variables
    OPENG_DP(D_OPENG_LIT( "     pathFound" ));
    iListPath->Reset();
    iListPointerPath.Reset();
    for (TInt j=0;j<count;j++)
        {
        iListPath->AppendL(aLists.MdcaPoint(j));
        iListPointerPath.Append(confirmedNodes[j]);
        }
    
    confirmedNodes.Close();    
    return KErrNone;
	}

// ---------------------------------------------------------------------------
// CRLSXDM::IsServiceUriExist()
// ---------------------------------------------------------------------------
//	
CXdmDocumentNode* CRLSXDM::IsServiceUriExist(const TDesC& uriAttr,
                                            CXdmDocumentNode* aRootNode)
    {
    OPENG_DP(D_OPENG_LIT( " CRLSXDM::IsServiceUriExist" ));
    // Find all service nodes in this root
    RPointerArray<CXdmDocumentNode> serviceNodes;
    TInt err= aRootNode->Find( KXdmService, serviceNodes);
    if (err != KErrNone) // return if no service nodes found
        {
        serviceNodes.Close();
        return NULL;    
        }
        
    TInt serviceNodesCounts = serviceNodes.Count();
    CXdmDocumentNode* desiredNode(NULL);
    // Go through all service nodes
    for(TInt i=0; i<serviceNodesCounts;i++)
        {
        CXdmNodeAttribute* UriAttr = (serviceNodes[i])->Attribute(KXdmUri);
        // if URI value is same
        if (UriAttr->AttributeValue() == uriAttr)
            {
            desiredNode = serviceNodes[i];
            OPENG_DP(D_OPENG_LIT( " CRLSXDM: Found" ));              
            break;
            }
        }
    serviceNodes.Close();
    return desiredNode;
    }
    
// ---------------------------------------------------------------------------
// CRLSXDM::GetNonLeafChild()
// ---------------------------------------------------------------------------
//
CXdmDocumentNode* CRLSXDM::GetNonLeafChild(const TDesC& aChildName,
                                    const TDesC& aAttrValue,
                                    CXdmDocumentNode* const aTargetNode)
    {
    OPENG_DP(D_OPENG_LIT( " CRLSXDM::GetNonLeafChild" ));
    OPENG_DP(D_OPENG_LIT( "  aChildName:%S, aAttrValue:%S"),
                                       &aChildName, &aAttrValue);
    CXdmDocumentNode* currentNode = aTargetNode;
    RPointerArray<CXdmDocumentNode> nodes;
    CXdmDocumentNode* desiredNode(NULL);
    CXdmNodeAttribute* attr(NULL);
    TBuf<KPresRLSElemAttrNameLen> attrName;
    
    GetNonLeafElementAttrName(aChildName, attrName);
    
    // Find all childs under currentNode
    if ((currentNode->Find(aChildName, nodes)) == KErrNone)
        { // go through all found childs
        TInt givenChildCountInNode = nodes.Count();
        OPENG_DP(D_OPENG_LIT( " GetNonLeafChild: givenChildCountInNode = %d" ),
                                                         givenChildCountInNode);
        for (TInt k=0;k<givenChildCountInNode;k++)
            { // get the attribute
            attr = (nodes[k])->Attribute(attrName);
            // match it with given attr
            if (attr->AttributeValue() == aAttrValue)
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
// CRLSXDM::GetLeafChild()
// ---------------------------------------------------------------------------
//    
CXdmDocumentNode* CRLSXDM::GetLeafChild(const TDesC& aChildName,
                const TDesC& aValue, CXdmDocumentNode* const aTargetNode)
    {
    OPENG_DP(D_OPENG_LIT( " CRLSXDM::IsResListExist" ));
    OPENG_DP(D_OPENG_LIT( " aChildName:%S, aValue:%S" ), &aChildName, &aValue);
    CXdmDocumentNode* currentNode = aTargetNode;
    RPointerArray<CXdmDocumentNode> nodes;
    CXdmDocumentNode* desiredNode(NULL);
    HBufC* nodeContents(NULL);
    
    // Find all leaf children under currentNode
    if ((currentNode->Find(aChildName, nodes)) == KErrNone)
        { // go through all found children
        TInt childCountInNode = nodes.Count();
        OPENG_DP(D_OPENG_LIT( " GetLeafChild: childCountInNode = %d" ), 
                                                        childCountInNode);
        for (TInt k=0;k<childCountInNode;k++)
            { // get the leaf node content
            nodeContents = CnvUtfConverter::ConvertToUnicodeFromUtf8L
                                            ((nodes[k])->LeafNodeContent());
            // match it with given contents
            if (nodeContents->Des() == aValue)
                {
                desiredNode = nodes[k];
                delete nodeContents;
                break;
                }
            delete nodeContents;    
            }
        }
    nodes.Close();
    return desiredNode;        
    }
    
// ---------------------------------------------------------------------------
// CRLSXDM::AddNonLeafElementL()
// ---------------------------------------------------------------------------
// 
void CRLSXDM::AddNonLeafElementL(const TDesC& aElementType, 
                        const TDesC& aData, const TDesC& aDisplayName,
                                CXdmDocumentNode* const aTargetNode)
    {
	OPENG_DP(D_OPENG_LIT( " CRLSXDM::AddNonLeafElementL" ));
	OPENG_DP(D_OPENG_LIT( "     aElementType(%S), aData(%S), aDisplayName(%S)"),
	                                &aElementType,&aData,&aDisplayName);
	                                
    TBuf<KPresRLSElemAttrNameLen> attrName;
    
    GetNonLeafElementAttrName(aElementType, attrName);
                                
	
    CXdmDocumentNode* myNode = aTargetNode->CreateChileNodeL(aElementType);
    CXdmNodeAttribute* myAttribute = myNode->CreateAttributeL(attrName);
    myAttribute->SetAttributeValueL(aData);
    
    if (aDisplayName!=KNullDesC) // if a display name is provided
        {
        CXdmDocumentNode* myDispName = myNode->CreateChileNodeL(KXdmDisplayName);
        myDispName->SetLeafNode(ETrue);
        myDispName->SetLeafNodeContentL(aDisplayName);
        }
    OPENG_DP(D_OPENG_LIT( "     NonLeafElement added" ));
    }
   
// ---------------------------------------------------------------------------
// CRLSXDM::AddLeafElementL()
// ---------------------------------------------------------------------------
// 
void CRLSXDM::AddLeafElementL(const TDesC& aElementType, const TDesC& aData,
                                    CXdmDocumentNode* const aTargetNode)
    {
	OPENG_DP(D_OPENG_LIT( " CRLSXDM::AddLeafElementL" ));
	OPENG_DP(D_OPENG_LIT( "     aElementType(%S), aData(%S)"),
	                                            &aElementType, &aData);
	
    CXdmDocumentNode* myNode = aTargetNode->CreateChileNodeL(aElementType);
    myNode->SetLeafNode(ETrue);
    myNode->SetLeafNodeContentL(aData);
    OPENG_DP(D_OPENG_LIT( "     AddLeafElementL added" ));
    }

// ---------------------------------------------------------------------------
// CRLSXDM::GetDisplayNameInt()
// ---------------------------------------------------------------------------
// 
TInt CRLSXDM::GetDisplayNameInt(const TDesC& aElementType,
                        const TDesC& aData, TPtrC8& aDisplayName,
                                CXdmDocumentNode* const aTargetNode)
    {
	OPENG_DP(D_OPENG_LIT( " CRLSXDM::GetDisplayName" ));
	OPENG_DP(D_OPENG_LIT( "     aElementType(%S),aData(%S)" )
	                                    ,&aElementType, &aData);
	
	TInt err(KErrNotFound);
	
	RPointerArray<CXdmDocumentNode> nodes;
    TBuf<KPresRLSElemAttrNameLen> attrName;
    
    GetNonLeafElementAttrName(aElementType, attrName);

    CXdmDocumentNode* myNode = GetNonLeafChild(aElementType, aData,
                                                            aTargetNode);
    if (myNode) // if found
        {
        // Find display name node
        if ( (myNode->Find(KXdmDisplayName, nodes)) == KErrNone)
            {
            TInt nodeCountInNode = nodes.Count();
            OPENG_DP(D_OPENG_LIT( "     nodeCountInNode(%d)" ),nodeCountInNode);
            if(nodeCountInNode==1)// should be one node that of dispname
                {
                CXdmDocumentNode* dispNameNode = myNode->ChileNode(0);
                dispNameNode->SetLeafNode(ETrue);
                aDisplayName.Set(dispNameNode->LeafNodeContent());
                err = KErrNone;
                }    
            }
        }    
    
    nodes.Close();
    return err;     
    }

// ---------------------------------------------------------------------------
// CRLSXDM::UpdateDisplayNameIntL()
// ---------------------------------------------------------------------------
// 
TInt CRLSXDM::UpdateDisplayNameIntL(const TDesC& aElementType, 
                        const TDesC& aData, const TDesC& aDisplayName,
                                CXdmDocumentNode* const aTargetNode)
    {
	OPENG_DP(D_OPENG_LIT( " CRLSXDM::UpdateDisplayNameL" ));
	OPENG_DP(D_OPENG_LIT( "     aElementType(%S),aData(%S),aDisplayName(%S)" )
	                                    ,&aElementType,&aData, &aDisplayName);
	                                    
    TBuf<KPresRLSElemAttrNameLen> attrName;
    
    GetNonLeafElementAttrName(aElementType, attrName);

    CXdmDocumentNode* myNode = GetNonLeafChild(aElementType, aData, aTargetNode);
    if (myNode==NULL) // if found
        return KErrNotFound;
    
    
    myNode->SetEmptyNode(ETrue); // delete all children from this
    myNode->SetEmptyNode(EFalse);
    CXdmDocumentNode* myDispName = myNode->CreateChileNodeL(KXdmDisplayName);
    myDispName->SetLeafNode(ETrue);
    myDispName->SetLeafNodeContentL(aDisplayName);
    OPENG_DP(D_OPENG_LIT( "     updated" ));
    return KErrNone;
    }

// ---------------------------------------------------------------------------
// CRLSXDM::GetNonLeafElementsL()
// ---------------------------------------------------------------------------
// 
void CRLSXDM::GetNonLeafElementsL(const TDesC& aElementType,
            CDesCArray& aValues, CXdmDocumentNode* const aTargetNode)
    {
	OPENG_DP(D_OPENG_LIT( " CRLSXDM::GetNonLeafElementsL" ));
	OPENG_DP(D_OPENG_LIT( "     aElementType(%S)"),&aElementType);

    RPointerArray<CXdmDocumentNode> nodes;
    CXdmNodeAttribute* attribute(NULL);
    
    TBuf<KPresRLSElemAttrNameLen> attrName;
    GetNonLeafElementAttrName(aElementType, attrName);
    
    // Find all elements under currentNode
    if ((aTargetNode->Find(aElementType, nodes)) == KErrNone)
        { // go through all found nodes
        TInt nodeCountInNode = nodes.Count();
        OPENG_DP(D_OPENG_LIT( "     nodeCountInNode(%d)" ),nodeCountInNode);
        for (TInt k=0;k<nodeCountInNode;k++)
            { // get the aAttr attribute
            attribute = (nodes[k])->Attribute(attrName);
            // push it to given discriptor array
            aValues.AppendL(attribute->AttributeValue());
            }
        }
        
    nodes.Close();
    }
            
// ---------------------------------------------------------------------------
// CRLSXDM::GetLeafElementsL()
// ---------------------------------------------------------------------------
// 
void CRLSXDM::GetLeafElementsL(const TDesC& aElementType, CDesCArray& aValues, 
                                CXdmDocumentNode* const aTargetNode)
    {
    OPENG_DP(D_OPENG_LIT( " CRLSXDM::GetLeafElementsL(%S)" ), &aElementType);

    RPointerArray<CXdmDocumentNode> nodes;
    HBufC* nodeContents(NULL);
    
    // Find all leaf children under currentNode
    if ((aTargetNode->Find(aElementType, nodes)) == KErrNone)
        { // go through all found children
        TInt childCountInNode = nodes.Count();
        OPENG_DP(D_OPENG_LIT( " GetLeafElementsL: childCountInNode = %d" ), 
                                                        childCountInNode);
        for (TInt k=0;k<childCountInNode;k++)
            { // get the leaf node content
            nodeContents = CnvUtfConverter::ConvertToUnicodeFromUtf8L
                                            ((nodes[k])->LeafNodeContent());
            CleanupStack::PushL(nodeContents);
            aValues.AppendL(nodeContents->Des());
            CleanupStack::PopAndDestroy(nodeContents);                                
            }
        }
    nodes.Close();
    }
    
    
// ---------------------------------------------------------------------------
// CRLSXDM::ValidateAndGetAttrName()
// ---------------------------------------------------------------------------
//    
void CRLSXDM::GetNonLeafElementAttrName(const TDesC& aElementName,
                                                            TDes& aAttrName)
    {
    OPENG_DP(D_OPENG_LIT( " CRLSXDM::ValidateAndGetAttrName" ));
    OPENG_DP(D_OPENG_LIT( " aElementName:%S, aAttrName:%S" ), 
                                                    &aElementName, &aAttrName);
    if (aElementName == KPresList)
        {
        aAttrName.Copy(KXdmName);
        }
    else if (aElementName == KPresEntry)
        {
        aAttrName.Copy(KXdmUri);
        }
    else if (aElementName == KPresExternal)
        {
        aAttrName.Copy(KXdmAnchor);
        }
    else if (aElementName == KPresEntryRef)
        {
        aAttrName.Copy(KXdmRef);
        }
    }
    
// ---------------------------------------------------------------------------
// CRLSXDM::IsNonLeafElementValid()
// ---------------------------------------------------------------------------
//    
TBool CRLSXDM::IsNonLeafElementValid(const TDesC& aElementName)
    {
    OPENG_DP(D_OPENG_LIT( " CRLSXDM::IsNonLeafElementValid" ));
    OPENG_DP(D_OPENG_LIT( " aElementName:%S"), &aElementName);
    TBool err(EFalse);
    
    if ( (aElementName == KPresList)||(aElementName == KPresEntry)||
            (aElementName == KPresExternal)||(aElementName == KPresEntryRef))
        err = ETrue;

    OPENG_DP(D_OPENG_LIT( "     return: %d" ), err);
    return err;    
    }
    
// ---------------------------------------------------------------------------
// CRLSXDM::AddPackageL()
// ---------------------------------------------------------------------------
//    
void CRLSXDM::AddPackageL()
    {
    OPENG_DP(D_OPENG_LIT( " CRLSXDM::AddPackage()" ));
    RPointerArray<CXdmDocumentNode> nodes;
	TInt count = iListPointerPath.Count();
	if (count)
	    {
        // now add the packages
        CXdmDocumentNode* packagesNode = iListPointerPath[0]->CreateChileNodeL(KXdmPackages);
        CXdmDocumentNode* packageNode = packagesNode->CreateChileNodeL(KXdmPackage);
        packageNode->SetLeafNode(ETrue);
        packageNode->SetLeafNodeContentL(KXdmPresence);
        }
    }

// ---------------------------------------------------------------------------
// CRLSXDM::RemovePackageL()
// ---------------------------------------------------------------------------
//    
void CRLSXDM::RemovePackageL()
    {
    OPENG_DP(D_OPENG_LIT( " CRLSXDM::RemovePackage()" ));
    RPointerArray<CXdmDocumentNode> nodes;
	TInt count = iListPointerPath.Count();
	if (count)
	    {
        if (((iListPointerPath[0])->Find(KXdmPackages, nodes)) == KErrNone)
            {
            TInt nodeCount = nodes.Count();
            OPENG_DP(D_OPENG_LIT( "     RemovePackage: nodeCount = %d" ), nodeCount);
            if(nodeCount) //if package found
                {
                iRLSDocument->RemoveFromModelL(nodes[0]);
                OPENG_DP(D_OPENG_LIT( "     removed" ));                
                }
            }
        }
    nodes.Close();
    }
    
// ---------------------------------------------------------------------------
// CRLSXDM::DeleteEmptyListsL()
// ---------------------------------------------------------------------------
//    
void CRLSXDM::DeleteEmptyListsL()
    {
    OPENG_DP(D_OPENG_LIT( " CRLSXDM::DeleteEmptyLists" ));
    RPointerArray<CXdmDocumentNode> listNodes;
    
    TInt found = iRLSDocument->Find( KXdmList, listNodes);
    OPENG_DP(D_OPENG_LIT( "     total Uris: %d" ), found);
    for (TInt i=0;i<found;i++)
        {
        if ( ((listNodes[i])->NodeCount()) < 2)// one child can be of display-name
                                               // otherwise list is empty
            {
            if ( ((listNodes[i])->NodeCount()) == 1) // if possibly disp-name present
                {
                if (((listNodes[i])->ChileNode(0)->NodeName()) != KXdmDisplayName)
                    continue; // if the one found node is not display name
                              // then the list is not empty
                }
            iRLSDocument->RemoveFromModelL(listNodes[i]);
            OPENG_DP(D_OPENG_LIT( "     delete at i = %d" ), i);
            listNodes.Close();
            return; // we are deleting one by one because we dont know
                    // the hirarchy
            }
        }
    listNodes.Close();    
    iAsyncReq = ERLSXDMDeletedEmptyLists;
    }

// ---------------------------------------------------------------------------
// CRLSXDM::DoCancel()
// ---------------------------------------------------------------------------
//    
void CRLSXDM::DoCancel()
    {
    OPENG_DP(D_OPENG_LIT( " CRLSXDM::DoCancel" ));
    iXDMEngine->CancelUpdate(iRLSDocument);
    iAsyncReq = ERLSXDMRequestCancelling;      
    }

// ---------------------------------------------------------------------------
// CRLSXDM::RunL()
// ---------------------------------------------------------------------------
//    
void CRLSXDM::RunL()
    {
    OPENG_DP(D_OPENG_LIT( " CRLSXDM::RunL" ));
    OPENG_DP(D_OPENG_LIT( "     RunL iAsyncReq(%d), iStatus(%d)" ),iAsyncReq,
                                                                 iStatus.Int());
                                                                                     
    TRequestStatus* status = &iStatus;
    TInt origStatus = iStatus.Int();
                                                                 
    switch (iAsyncReq)
        {
        case ERLSXDMUpdateToServer:
            
            if ( origStatus == KXcapErrorHttpConflict )
                {
                TRAP_IGNORE( DoParseNegotiatedServiceUriL( iRLSDocument->ErrorRoot() ));
                }
            else
                {
                delete iNegotiatedServiceUri;
                iNegotiatedServiceUri = NULL;
                }                           
                        
            iAsyncReq = ERLSXDMNoRequestMade;                        
            TRAP_IGNORE( iAsyncHandler->HandleRLSUpdateDocumentL( origStatus ));
            break;
        case ERLSXDMUpdateFromServer:
            CreateRootIfNeededL();
            iAsyncReq = ERLSXDMNoRequestMade;               
            if( origStatus == KXcapErrorHttpNotFound) // if doc not on server
                {                                
                TRAP_IGNORE (iAsyncHandler->HandleRLSUpdateDocumentL(KErrNotFound));
                }
            else
                {                                
                TRAP_IGNORE( iAsyncHandler->HandleRLSUpdateDocumentL( origStatus ));
                }
            break;
            
        case ERLSXDMDeletingEmptyLists:
            DeleteEmptyListsL();
            User::RequestComplete(status,KErrNone);
            SetActive();
            break;
                
        case ERLSXDMDeletedEmptyLists:
            iAsyncReq = ERLSXDMNoRequestMade;        
            TRAP_IGNORE( iAsyncHandler->HandleRLSDeleteAllEmptyListsL( origStatus ));
            break;
            
        case ERLSXDMRequestCancelling:
            iAsyncReq = ERLSXDMNoRequestMade;         
            TRAP_IGNORE( iAsyncHandler->HandleRLSUpdateCancelL( origStatus ));  
            break;

        case ERLSXDMNoRequestMade:
        default:
            break;
        }
    }

// ---------------------------------------------------------------------------
// CRLSXDM::RunError()
// ---------------------------------------------------------------------------
//    
TInt CRLSXDM::RunError(TInt aError)
    {
    OPENG_DP(D_OPENG_LIT( " CRLSXDM::RunError" ));
    OPENG_DP(D_OPENG_LIT( "     RunError iAsyncReq(%d), aError(%d)" ),iAsyncReq,
                                                                        aError);
    // A show must go on, we coomplete the failed current client request. 
    TRequestStatus* status = &iStatus;                                                                       
    iAsyncReq = ERLSXDMNoRequestMade;                                                                        
    User::RequestComplete( status, aError );                                                                        
    
    return KErrNone;
    }
    
// ---------------------------------------------------------------------------
// CRLSXDM::NegotiatedServiceUri()
// ---------------------------------------------------------------------------
//     
EXPORT_C TPtrC CRLSXDM::NegotiatedServiceUri()
    {
    return iNegotiatedServiceUri ? iNegotiatedServiceUri->Des() : TPtrC();
    }    
	
// ---------------------------------------------------------------------------
// CRLSXDM::DoParseNegotiatedServiceUriL
// ---------------------------------------------------------------------------
//	
void CRLSXDM::DoParseNegotiatedServiceUriL( CXdmDocumentNode* aErrorBody )
    {
    _LIT( KMyXcapError, "xcap-error");        
    _LIT( KMyUniquenessFailure, "uniqueness-failure");
    _LIT( KMyExists, "exists");  
    _LIT( KMyField, "field"); 
    _LIT( KMyAltValue, "alt-value");  
        
    delete iNegotiatedServiceUri;
    iNegotiatedServiceUri = NULL;

    if ( !aErrorBody )
        {
        return;
        } 
        
    // Parse the content
    // For example
    // ------------    
    /*
    <?xml version="1.0" encoding="UTF-8"?>
    <xcap-error xmlns="urn:oma:xml:xdm:xcap-error">
        <uniqueness-failure>
            <exists field="rls-services/service%5b1%5d/@uri">
                <alt-value>sip:tu30@poc.nokia.com;omapresencer1rls=5425</alt-value>
            </exists>
        </uniqueness-failure>
    </xcap-error>
    // ------------  
    */         
        
    if ( aErrorBody->NodeName().Compare( KMyXcapError ))
        {
        return;
        }
    
    RPointerArray<CXdmDocumentNode> resultArray;
    CleanupClosePushL( resultArray );           // <<< resultArray
            
    CXdmDocumentNode* currNode = aErrorBody;
    resultArray.Reset();
    
    resultArray.Reset();        
    currNode->Find( KMyUniquenessFailure, resultArray );
    if ( resultArray.Count() > 0)
        {
        currNode = resultArray[0];            
        }
    else
        {
        User::Leave( KErrNotFound );
        }     
            
    resultArray.Reset();        
    currNode->Find( KMyExists, resultArray );
    if ( resultArray.Count() > 0)
        {
        currNode = resultArray[0];
        if ( !(currNode->HasAttribute( KMyField ) )) 
            {
            // notice: We do not check the content of "field" attribute, 
            // we trust that it is ok and refers to the last modified data.
            User::Leave( KErrNotFound );
            }             
        }
    else
        {
        User::Leave( KErrNotFound );
        }    
        
    resultArray.Reset();        
    currNode->Find( KMyAltValue, resultArray );
    if ( resultArray.Count() > 0)
        {
        currNode = resultArray[0];    
        }
    else
        {
        User::Leave( KErrNotFound );
        }     
       
    TPtrC8 pCon = currNode->LeafNodeContent();
       
    // Convert <alt-value> value to Unicode.
    iNegotiatedServiceUri = CnvUtfConverter::ConvertToUnicodeFromUtf8L( pCon );           

    CleanupStack::PopAndDestroy( &resultArray );   // >>> resultArray   
                
    }    

// end of file
