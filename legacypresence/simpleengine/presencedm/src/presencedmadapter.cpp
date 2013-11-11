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
* Description:    OMA Presence Settings Device Management Adapter's source file 
*
*/



 
#include <implementationproxy.h>
#include <nsmldmuri.h>
#include <utf.h>
#include <badesca.h>
#include <StringLoader.h>

#include "presencedmadapter.h"
#include "presencedmlogger.h"

// -----------------------------------------------------------------------------
// CPresenceDMAdapter::NewL()
// -----------------------------------------------------------------------------
//
CPresenceDMAdapter* CPresenceDMAdapter::NewL(MSmlDmCallback* aDmCallback )
    {
    PRES_DM_LOG(LOG_LIT("NewL start"));    
    CPresenceDMAdapter* self = new (ELeave) CPresenceDMAdapter( aDmCallback );
    PRES_DM_LOG(LOG_LIT(" NewL end"));    
    return self;
    }
    
// -----------------------------------------------------------------------------
// CPresenceDMAdapter::CPresenceDMAdapter()
// -----------------------------------------------------------------------------
//
CPresenceDMAdapter::CPresenceDMAdapter(MSmlDmCallback* aDmCallback) : 
    CSmlDmAdapter( aDmCallback ), iGetSetID(KErrNotFound)
    {
    PRES_DM_LOG(LOG_LIT("CPresenceDMAdapter")); 
    }

// -----------------------------------------------------------------------------
// CPresenceDMAdapter::~CPresenceDMAdapter()
// -----------------------------------------------------------------------------
//
CPresenceDMAdapter::~CPresenceDMAdapter()
    {
    PRES_DM_LOG(LOG_LIT("~CPresenceDMAdapter")); 
    }

// -----------------------------------------------------------------------------
//  CPresenceDMAdapter::DDFVersionL
// -----------------------------------------------------------------------------
//
void CPresenceDMAdapter::DDFVersionL(CBufBase& aDDFVersion)
    {
    PRES_DM_LOG(LOG_LIT("DDFVersionL"));   
    aDDFVersion.InsertL( 0, KPresDMDDFVersion );    
    }

// -----------------------------------------------------------------------------
//  CPresenceDMAdapter::DDFStructureL
// -----------------------------------------------------------------------------
//
void CPresenceDMAdapter::DDFStructureL( MSmlDmDDFObject& aDDF )
    {
    PRES_DM_LOG(LOG_LIT("DDFStructureL"));    

    // For OMA_PRESENCE 
    TSmlDmAccessTypes accessTypeGet;
    accessTypeGet.SetGet();
     
    // For OMA_PRESENCE/<X> 
    TSmlDmAccessTypes accessTypeAll;
    accessTypeAll.SetAdd();
    accessTypeAll.SetGet();
    accessTypeAll.SetReplace();
    accessTypeAll.SetDelete();
    
    // For leaf nodes
    TSmlDmAccessTypes accessTypeNoDelete;
    accessTypeNoDelete.SetAdd();
    accessTypeNoDelete.SetGet();
    accessTypeNoDelete.SetReplace();

    // ./OMA_PRESENCE
    MSmlDmDDFObject& omaPresNode = aDDF.AddChildObjectL( KPresDMNodeName );
    FillNodeInfoL( omaPresNode, accessTypeGet, MSmlDmDDFObject::EOne,
                  MSmlDmDDFObject::EPermanent, MSmlDmDDFObject::ENode,
                  KPresDMNodeNameDescription );
                  
    // ./OMA_PRESENCE/<X> 
    MSmlDmDDFObject& runTimeNode = omaPresNode.AddChildObjectGroupL();
    FillNodeInfoL( runTimeNode, accessTypeAll, MSmlDmDDFObject::EZeroOrMore,
                  MSmlDmDDFObject::EDynamic, MSmlDmDDFObject::ENode,
                  KPresDmDynamicDescription );
    
    // ./OMA_PRESENCE/<X>/NAME            
    MSmlDmDDFObject& nameNode = runTimeNode.AddChildObjectL( KPresDMName );
    FillNodeInfoL( nameNode, accessTypeNoDelete, MSmlDmDDFObject::EZeroOrOne,
                  MSmlDmDDFObject::EDynamic, MSmlDmDDFObject::EChr,
                  KPresDMNameDescription );
            
    // ./OMA_PRESENCE/<X>/PROVIDER-ID           
    MSmlDmDDFObject& providerIdNode = runTimeNode.AddChildObjectL( KPresDMProviderId );
    FillNodeInfoL( providerIdNode, accessTypeNoDelete,MSmlDmDDFObject::EZeroOrOne,
                  MSmlDmDDFObject::EDynamic, MSmlDmDDFObject::EChr,
                  KPresDMProviderIdDescription );    
                               
    // ./OMA_PRESENCE/<X>/ToConRef
    MSmlDmDDFObject& toConRefNode = runTimeNode.AddChildObjectL( KPresDMToConRef );
    FillNodeInfoL( toConRefNode, accessTypeNoDelete, MSmlDmDDFObject::EZeroOrOne,
                  MSmlDmDDFObject::EDynamic, MSmlDmDDFObject::ENode,
                  KPresDMToConRefDescription);
                  
        // ./OMA_PRESENCE/<X>/ToConRef/SIP-PROFILE (Towards SIP ID)                                         
        MSmlDmDDFObject& napIdSIPNode = toConRefNode.AddChildObjectL( KPresDMSIPProfile );
        FillNodeInfoL( napIdSIPNode, accessTypeNoDelete, MSmlDmDDFObject::EZeroOrOne, 
                      MSmlDmDDFObject::EDynamic, MSmlDmDDFObject::ENode,
                      KPresDMSIPProfileDescription );
                  
            // ./OMA_PRESENCE/<X>/ToConRef/SIP-PROFILE/ConRef (The actual SIP ID)                                    
            MSmlDmDDFObject& conRefSipNode = napIdSIPNode.AddChildObjectL( KPresDMConRef );
            FillNodeInfoL( conRefSipNode, accessTypeNoDelete, MSmlDmDDFObject::EZeroOrOne, 
                          MSmlDmDDFObject::EDynamic, MSmlDmDDFObject::EChr,
                          KPresDMConRefDescription );
                  
        // ./OMA_PRESENCE/<X>/ToConRef/XDM-SET (To XDM-ID) (Towards XDM ID)                                         
        MSmlDmDDFObject& napIdToXDMNode = toConRefNode.AddChildObjectL( KPresDMXDMSet );
        FillNodeInfoL( napIdToXDMNode, accessTypeNoDelete, MSmlDmDDFObject::EZeroOrOne, 
                      MSmlDmDDFObject::EDynamic, MSmlDmDDFObject::ENode,
                      KPresDMXDMSetDescription );
                  
            // ./OMA_PRESENCE/<X>/ToConRef/XDM-SET/ConRef (actual XDM node)       
            MSmlDmDDFObject& conRefXDMNode = napIdToXDMNode.AddChildObjectL( KPresDMConRef );
            FillNodeInfoL( conRefXDMNode, accessTypeNoDelete, MSmlDmDDFObject::EZeroOrOne, 
                          MSmlDmDDFObject::EDynamic, MSmlDmDDFObject::EChr,
                          KPresDMConRefDescription );                 
                                   
    // ./OMA_PRESENCE/<X>/CLIENT-DATA-OBJ-LIMIT          
    MSmlDmDDFObject& dataObjNode = runTimeNode.AddChildObjectL( KPresDMClientObjDataLim );
    FillNodeInfoL( dataObjNode, accessTypeNoDelete, MSmlDmDDFObject::EOne,
                  MSmlDmDDFObject::EDynamic, MSmlDmDDFObject::EInt,
                  KPresDMClientObjDataLimDescription );
                  
    // ./OMA_PRESENCE/<X>/CONTENT-SERVER-URI
    MSmlDmDDFObject& ContentSrvNode = runTimeNode.AddChildObjectL( KPresDMContentSrvUri );
    FillNodeInfoL( ContentSrvNode, accessTypeNoDelete, MSmlDmDDFObject::EZeroOrOne,
                  MSmlDmDDFObject::EDynamic,MSmlDmDDFObject::EChr,
                  KPresDMContentSrvUriDescription );            
                  
    // ./OMA_PRESENCE/<X>/SOURCE-THROTTLE-PUBLISH   
    MSmlDmDDFObject& srcThrotNode = runTimeNode.AddChildObjectL( KPresDMSrcThPublish );
    FillNodeInfoL( srcThrotNode, accessTypeNoDelete, MSmlDmDDFObject::EZeroOrOne,
                  MSmlDmDDFObject::EDynamic, MSmlDmDDFObject::EInt,
                  KPresDMSrcThPublishDescription );     
                  
        // ./OMA_PRESENCE/<X>/MAX-NUMBER-OF-PRESENCE-SUBSCRIPTIONS  
    MSmlDmDDFObject& maxSubsNode = runTimeNode.AddChildObjectL( KPresDMMaxPresSubs );
    FillNodeInfoL( maxSubsNode, accessTypeNoDelete, MSmlDmDDFObject::EZeroOrOne,
                  MSmlDmDDFObject::EDynamic,MSmlDmDDFObject::EInt,
                  KPresDMMaxPresSubsDescription ); 
                    
    // ./OMA_PRESENCE/<X>/MAX-NUMBER-OF-SUBSCRIPTIONS-IN-PRESENCE-LIST   
    MSmlDmDDFObject& maxSubsListNode = runTimeNode.AddChildObjectL( KPresDMMaxPresSubsInLists );
    FillNodeInfoL( maxSubsListNode, accessTypeNoDelete, MSmlDmDDFObject::EZeroOrOne,
                  MSmlDmDDFObject::EDynamic,MSmlDmDDFObject::EInt,
                  KPresDMMaxPresSubsInListsDescription );
                  
    // ./OMA_PRESENCE/<X>/SERVICE-URI-TEMPLATE
    MSmlDmDDFObject& serUriTempNode = runTimeNode.AddChildObjectL( KPresDMServiceUriTemp );
    FillNodeInfoL( serUriTempNode, accessTypeNoDelete, MSmlDmDDFObject::EOne,
                  MSmlDmDDFObject::EDynamic,MSmlDmDDFObject::EChr,
                  KPresDMServiceUriTempDescription );
  
    PRES_DM_LOG(LOG_LIT(" DDFStructureL end"));  
    }
    
// -----------------------------------------------------------------------------
// CPresenceDMAdapter::ChildURIListL
// -----------------------------------------------------------------------------
//
void CPresenceDMAdapter::ChildURIListL( const TDesC8& aUri, 
                                       const TDesC8& /*aLUID*/, 
                                       const CArrayFix<TSmlDmMappingInfo>& 
                                       /*aPreviousURISegmentList*/, 
                                       const TInt aResultsRef, 
                                       const TInt aStatusRef )
    {
    PRES_DM_LOG(LOG_LIT8("ChildURIListL(%S): begin"), &aUri);
    const TInt KPresDMSegmentNameLen = 50;
    
    CSmlDmAdapter::TError retValue = CSmlDmAdapter::EOk;

    CBufBase* currentList = CBufFlat::NewL(1);
    CleanupStack::PushL( currentList ); // << currentList
    
    const TPtrC8 lastUriSeg = NSmlDmURI::LastURISeg( aUri );
    TBuf8<KPresDMSegmentNameLen> segmentName;
    
    // ./OMA_PRESENCE
    if( !lastUriSeg.Compare( KPresDMNodeName ) ) 
        {
        PRES_DM_LOG(LOG_LIT8("  ./OMA_PRESENCE"));
        RArray<TInt> settingIds;
        CleanupClosePushL( settingIds );    // << settingIds
        CDesCArray* names = PresSettingsApi::GetAllSetsNamesLC(settingIds);
        TInt IdCount = settingIds.Count();
        
        PRES_DM_LOG(LOG_LIT("   IdCount = %d"), IdCount);
            
        for( TInt i(0); i < IdCount; i++ )
            {
            TInt id = settingIds[i];
            segmentName.Copy( KNullDesC8 );
            segmentName.AppendNum( id );
            currentList->InsertL( currentList->Size(), segmentName);
            currentList->InsertL( currentList->Size(), KPresDMSeparator );
            }
        CleanupStack::PopAndDestroy(names);
        CleanupStack::PopAndDestroy(1); // settingIds, 
        
        Callback().SetResultsL( aResultsRef, *currentList, KNullDesC8 );
        Callback().SetStatusL( aStatusRef, retValue );
        CleanupStack::PopAndDestroy( currentList ); // >>> currentList
        currentList = NULL;
        return;
        }   

    // ./OMA_PRESENCE/X
    if( NSmlDmURI::NumOfURISegs( aUri ) == KPresDMLevel ) // if Uri segments are 2
                                               // we are certainly at x
                                               // level   
        {
        PRES_DM_LOG(LOG_LIT8("  ./OMA_PRESENCE/X"));
        
        segmentName.Copy( KPresDMName );
        currentList->InsertL( currentList->Size(), segmentName );
        currentList->InsertL( currentList->Size(), KPresDMSeparator );
        
        segmentName.Copy( KPresDMProviderId );
        currentList->InsertL( currentList->Size(), segmentName );
        currentList->InsertL( currentList->Size(), KPresDMSeparator );
        
        segmentName.Copy( KPresDMToConRef );
        currentList->InsertL( currentList->Size(), segmentName );
        currentList->InsertL( currentList->Size(), KPresDMSeparator );
                
        segmentName.Copy( KPresDMClientObjDataLim );
        currentList->InsertL( currentList->Size(), segmentName );
        currentList->InsertL( currentList->Size(), KPresDMSeparator );
        
        segmentName.Copy( KPresDMContentSrvUri );
        currentList->InsertL( currentList->Size(), segmentName );
        currentList->InsertL( currentList->Size(), KPresDMSeparator );
        
        segmentName.Copy( KPresDMSrcThPublish );
        currentList->InsertL( currentList->Size(), segmentName );
        currentList->InsertL( currentList->Size(), KPresDMSeparator );
        
        segmentName.Copy( KPresDMMaxPresSubs );
        currentList->InsertL( currentList->Size(), segmentName );
        currentList->InsertL( currentList->Size(), KPresDMSeparator );
        
        segmentName.Copy( KPresDMMaxPresSubsInLists );
        currentList->InsertL( currentList->Size(), segmentName );
        currentList->InsertL( currentList->Size(), KPresDMSeparator );
        
        segmentName.Copy( KPresDMServiceUriTemp );
        currentList->InsertL( currentList->Size(), segmentName );
        currentList->InsertL( currentList->Size(), KPresDMSeparator );
        }
        
    // ./OMA_PRESENCE/X/ToConRef   
    else if ( !lastUriSeg.Compare( KPresDMToConRef ) ) 
        {
        PRES_DM_LOG(LOG_LIT8("  ./OMA_PRESENCE/X/ToConRef"));
        segmentName.Copy( KPresDMSIPProfile );
        currentList->InsertL( currentList->Size(), segmentName );
        currentList->InsertL( currentList->Size(), KPresDMSeparator );
        
        segmentName.Copy( KPresDMXDMSet );
        currentList->InsertL( currentList->Size(), segmentName );
        currentList->InsertL( currentList->Size(), KPresDMSeparator );
        }
    
    // ./OMA_PRESENCE/X/ToConRef/SIP-ID    
    else if ( !lastUriSeg.Compare( KPresDMSIPProfile ) ) 
        {
        PRES_DM_LOG(LOG_LIT8("  ./OMA_PRESENCE/X/ToConRef/SIP-ID"));
        segmentName.Copy( KPresDMConRef );
        currentList->InsertL( currentList->Size(), segmentName );
        currentList->InsertL( currentList->Size(), KPresDMSeparator );
        }
    
    // ./OMA_PRESENCE/X/ToConRef/XDM-ID      
    else if ( !lastUriSeg.Compare( KPresDMXDMSet ) )
        {
        PRES_DM_LOG(LOG_LIT8("  ./OMA_PRESENCE/X/ToConRef/XDM-ID"));
        segmentName.Copy( KPresDMConRef );
        currentList->InsertL( currentList->Size(), segmentName );
        currentList->InsertL( currentList->Size(), KPresDMSeparator );
        }
        
    else
        {
        // if none of asked nodes found return error.
        retValue = CSmlDmAdapter::ENotFound;
        }
       
    Callback().SetResultsL( aResultsRef, *currentList, KNullDesC8 );
    Callback().SetStatusL( aStatusRef, retValue );
    CleanupStack::PopAndDestroy( currentList ); // >>> currentList  
    PRES_DM_LOG(LOG_LIT(" ChildURIListL(): end") );
    }    
 
// -----------------------------------------------------------------------------
//  CPresenceDMAdapter::FetchLeafObjectL
// -----------------------------------------------------------------------------
//
void CPresenceDMAdapter::FetchLeafObjectL( const TDesC8& aUri, 
                                      const TDesC8& /*aLUID*/, 
                                      const TDesC8& aType, 
                                      const TInt aResultsRef, 
                                      const TInt aStatusRef )
    {
    PRES_DM_LOG(LOG_LIT8("FetchLeafObjectL(%S): start"), &aUri );
    CBufBase* result = CBufFlat::NewL( KPresDMDefaultResultSize );
    CleanupStack::PushL( result );    // << result
    CSmlDmAdapter::TError status = FetchObjectL( aUri, *result );
    if( status == CSmlDmAdapter::EOk )
        {
        Callback().SetResultsL( aResultsRef,*result,aType );
        }
    Callback().SetStatusL(aStatusRef, status);
    CleanupStack::PopAndDestroy(result);  // >>> result
    
    PRES_DM_LOG(LOG_LIT(" FetchLeafObjectL(): end") );
    }
    
// -----------------------------------------------------------------------------
// CSmlDmAdapter::TError FetchObjectL
// -----------------------------------------------------------------------------
//
CSmlDmAdapter::TError CPresenceDMAdapter::FetchObjectL( const TDesC8& aUri, 
                                                   CBufBase& aObject)
    {
    PRES_DM_LOG(LOG_LIT8("FetchObjectL(%S): start"), &aUri);
    CSmlDmAdapter::TError status = CSmlDmAdapter::EOk;
   
    TPresSettingsSet tempSet;
    TPresSettingsProperty property;

    HBufC8* luid = Callback().GetLuidAllocL( aUri );
    TInt settingsId = DesToInt( *luid );
    
    PRES_DM_LOG(LOG_LIT("   settingsId after callback:%d"), settingsId);
    
    // check whether above found id is valid or not
    if (!IsPresIDValidL(settingsId))
        {
        //try to find it another way
        TPtrC8 removedLastSeg = NSmlDmURI::RemoveLastSeg(aUri);
        TPtrC8 idSegment = NSmlDmURI::LastURISeg(removedLastSeg);
        TInt settingsId = DesToInt(idSegment);
        if (!IsPresIDValidL(settingsId))
            status = CSmlDmAdapter::ENotFound;
        PRES_DM_LOG(LOG_LIT("   settingsId found local way:%d"), settingsId);
        }
    
    // if previous settings id processing was ok
    if (status == CSmlDmAdapter::EOk)
        {
        TPtrC8 lastUriSeg = NSmlDmURI::LastURISeg( aUri );
        // NAME
        if( Match(lastUriSeg, KPresDMName ) )
            {
            property = EPresPropSetName;  
            }
        // PROVIDER-ID  
        else if( Match(lastUriSeg, KPresDMProviderId ) )
            {
            property = EPresPropProviderID;
            }
        // Client object data limit 
        else if( Match(lastUriSeg, KPresDMClientObjDataLim) )
            {
            property = EPresPropObjectSize;  
            }
        // content server Uri
        else if( Match(lastUriSeg, KPresDMContentSrvUri) )
            {
            property = EPresPropContSrvAdd;  
            }
        // Source throttle publish
        else if( Match(lastUriSeg, KPresDMSrcThPublish) )
            {
            property = EPresPropPubInterval;  
            }
        // Max pres subs
        else if( Match(lastUriSeg, KPresDMMaxPresSubs ) )
            {
            property = EPresPropMaxSubs;  
            }
        // max subs in lists    
        else if( Match(lastUriSeg, KPresDMMaxPresSubsInLists ) )
            {
            property = EPresPropMaxCntInList;  
            }
        // service uri template     
        else if( Match(lastUriSeg, KPresDMServiceUriTemp ) )
            {
            property = EPresPropDomainSyntax;  
            }       
            
        // ConRef/TO-SIP
        // ConRef/TO-XDM        
        else if( Match( lastUriSeg, KPresDMConRef) )
            {
            TPtrC8 conRefUri = NSmlDmURI::RemoveLastSeg( aUri );
            TPtrC8 lastConRefSeg = NSmlDmURI::LastURISeg( conRefUri );
            
            if( Match( lastConRefSeg, KPresDMSIPProfile ) )
                {
                property = EPresPropSIPProfile;
                }  
            else if( Match( lastConRefSeg, KPresDMXDMSet ) )
                {
                property = EPresPropXDMSet;
                }  
            else
                {
                status = CSmlDmAdapter::ENotFound;
                }
            }
        else
            {
            status = CSmlDmAdapter::ENotFound;
            }
        }
    
    // known property
    if ( status == CSmlDmAdapter::EOk )
        {
        status = GetPropertyL( settingsId, property, aObject ) ;
        PRES_DM_LOG(LOG_LIT("   GetPropertyL status: %d"), status); 
        }
        
    PRES_DM_LOG(LOG_LIT(" FetchObjectL: ends"));    
    return status;
    }  
         
// -----------------------------------------------------------------------------
// CPresenceDMAdapter::FetchLeafObjectSizeL
// -----------------------------------------------------------------------------
//
void CPresenceDMAdapter::FetchLeafObjectSizeL( const TDesC8& aUri, 
                                              const TDesC8& /*aLUID*/, 
                                              const TDesC8& aType, 
                                              const TInt aResultsRef, 
                                              const TInt aStatusRef )
    {
    PRES_DM_LOG(LOG_LIT8("FetchLeafObjectSizeL(%S): begins"), &aUri); 
    CBufBase* result = CBufFlat::NewL( KPresDMDefaultResultSize );
    CleanupStack::PushL( result );
    CSmlDmAdapter::TError status = FetchObjectL( aUri, *result );
    PRES_DM_LOG(LOG_LIT("   FetchObjectL: status = %d"), status); 
    if( status == CSmlDmAdapter::EOk )
        {
        TInt objSizeInBytes = result->Size();
        PRES_DM_LOG(LOG_LIT("   objSizeInBytes = %d"), objSizeInBytes); 
        
        TBuf8<KPresDMObjectSizeWidth> stringObjSizeInBytes;
        stringObjSizeInBytes.Num( objSizeInBytes );
        
        result->Reset();
        result->InsertL( 0, stringObjSizeInBytes );
        
        Callback().SetResultsL( aResultsRef, *result, aType );
        }   
        
    Callback().SetStatusL( aStatusRef, status) ;
    CleanupStack::PopAndDestroy(result);    
    PRES_DM_LOG(LOG_LIT(" FetchLeafObjectSizeL: ends")); 
    }

// -----------------------------------------------------------------------------
// CPresenceDMAdapter::UpdateLeafObjectL()
// -----------------------------------------------------------------------------
//
void CPresenceDMAdapter::UpdateLeafObjectL(const TDesC8& aUri, 
                                         const TDesC8& /*aLUID*/, 
                                         const TDesC8& aObject, 
                                         const TDesC8& /*aType*/, 
                                         const TInt aStatusRef)
    {
    PRES_DM_LOG(LOG_LIT8("UpdateLeafObjectL(%S): starts"), &aUri); 
    CSmlDmAdapter::TError status = CSmlDmAdapter::EOk;
    
    TPresSettingsSet tempSet;
    TPresSettingsProperty property;

    HBufC8* luid = Callback().GetLuidAllocL( aUri );
    TInt settingsId = DesToInt( *luid );
    
    PRES_DM_LOG(LOG_LIT("   settingsId after callback:%d"), settingsId);
    
    // check whether above found id is valid or not
    if (!IsPresIDValidL(settingsId))
        {
        //try to find it another way
        TPtrC8 removedLastSeg = NSmlDmURI::RemoveLastSeg(aUri);
        TPtrC8 idSegment = NSmlDmURI::LastURISeg(removedLastSeg);
        TInt settingsId = DesToInt(idSegment);
        if (!IsPresIDValidL(settingsId))
            status = CSmlDmAdapter::ENotFound; 
        PRES_DM_LOG(LOG_LIT("   settingsId found local way:%d"), settingsId);
        }
    
    // if previous settings id processing was ok
    if (status == CSmlDmAdapter::EOk)
        {
        TPtrC8 lastUriSeg = NSmlDmURI::LastURISeg( aUri );

        // NAME
        if( Match(lastUriSeg, KPresDMName ) )
            {
            property = EPresPropSetName;  
            }
        // PROVIDER-ID  
        else if( Match(lastUriSeg, KPresDMProviderId ) )
            {
            property = EPresPropProviderID;  
            }
        // Client object data limit 
        else if( Match(lastUriSeg, KPresDMClientObjDataLim) )
            {
            property = EPresPropObjectSize;  
            }
        // content server Uri
        else if( Match(lastUriSeg, KPresDMContentSrvUri) )
            {
            property = EPresPropContSrvAdd;  
            }
        // Source throttle publish
        else if( Match(lastUriSeg, KPresDMSrcThPublish) )
            {
            property = EPresPropPubInterval;  
            }
        // Max pres subs
        else if( Match(lastUriSeg, KPresDMMaxPresSubs ) )
            {
            property = EPresPropMaxSubs;  
            }
        // max subs in lists    
        else if( Match(lastUriSeg, KPresDMMaxPresSubsInLists ) )
            {
            property = EPresPropMaxCntInList;  
            }
        // service uri template     
        else if( Match(lastUriSeg, KPresDMServiceUriTemp ) )
            {
            property = EPresPropDomainSyntax;  
            }       
            
        // ToConRef/TO-SIP/ConRef
        // ToConRef/TO-XDM/ConRef       
        else if( Match( lastUriSeg, KPresDMConRef) )
            {
            TPtrC8 conRefUri = NSmlDmURI::RemoveLastSeg( aUri );
            TPtrC8 lastConRefSeg = NSmlDmURI::LastURISeg( conRefUri );
            
            if( Match( lastConRefSeg, KPresDMSIPProfile ) )
                {
                property = EPresPropSIPProfile;
                }  
            else if( Match( lastConRefSeg, KPresDMXDMSet ) )
                {
                property = EPresPropXDMSet;
                }  
            else
                {
                status = CSmlDmAdapter::ENotFound;
                }
            }
        else
            {
            status = CSmlDmAdapter::ENotFound;
            }
        }
        
    else // if id is not valid
        {
        status = CSmlDmAdapter::ENotFound;    
        }

    // known property and id is ok
    if ( status == CSmlDmAdapter::EOk )
        {
        status = UpdatePropertyL( settingsId, property, aObject ); 
        PRES_DM_LOG(LOG_LIT("   UpdatePropertyL status: %d"), status); 
        }

    Callback().SetStatusL(aStatusRef, status); 
        
    PRES_DM_LOG(LOG_LIT(" UpdateLeafObjectL: ends")); 
    }

// -----------------------------------------------------------------------------
// CPresenceDMAdapter::AddNodeObjectL
// -----------------------------------------------------------------------------
//
void CPresenceDMAdapter::AddNodeObjectL( const TDesC8& aUri, 
                                    const TDesC8& /*aParentLUID*/, 
                                    const TInt aStatusRef )
    {
    PRES_DM_LOG(LOG_LIT8("AddNodeObjectL(%S): begins"), &aUri);   
  
    CSmlDmAdapter::TError status = CSmlDmAdapter::EError;
    HBufC* lastUriSegment;
    
    if (!NSmlDmURI::RemoveLastSeg( aUri ).Compare( KPresDMNodeName ))
        {
        // Both the Name and provider id of new set, we are setting from
        // lastUrisegment, they will expectedly changed by the next calls
        // to this function by the framework
        lastUriSegment = ConvertLC(NSmlDmURI::LastURISeg(aUri));
        TPresSettingsSet mySet; // New set to be created
        
        // Processing for set name
        if (lastUriSegment->Length() > KPresSetNameMaxLength)
            (mySet.iSetName).Copy(lastUriSegment->Left(KPresSetNameMaxLength));
        else
            (mySet.iSetName).Copy(lastUriSegment->Des());
        CheckDuplicateNameL(mySet.iSetName, 0);
        
        // Processing for provider ID
        if (lastUriSegment->Length() > KPresProviderIDMaxLength)
            (mySet.iProviderID).Copy(lastUriSegment->Left(KPresProviderIDMaxLength));
        else
            (mySet.iProviderID).Copy(lastUriSegment->Des());
        
        TInt id = PresSettingsApi::CreateSetL(mySet);
        PRES_DM_LOG(LOG_LIT("   New set id(%d)"), id);  
        HBufC8* luid = IntToDes8LC( id ); // << luid
        Callback().SetMappingL( aUri, *luid );
        CleanupStack::PopAndDestroy(luid); // luid
        CleanupStack::PopAndDestroy(lastUriSegment); //lastUriSegment
        

        if(id>0) // if set is created successfully
            {
            status =  CSmlDmAdapter::EOk;            
            }
        }
    Callback().SetStatusL( aStatusRef, status );
    PRES_DM_LOG(LOG_LIT("   status(%d)"), status);  
    PRES_DM_LOG(LOG_LIT(" AddNodeObjectL: ends"));
    }
    
// -----------------------------------------------------------------------------
// CPresenceDMAdapter::DeleteObjectL
// -----------------------------------------------------------------------------
//
void CPresenceDMAdapter::DeleteObjectL( const TDesC8& aUri, 
                                   const TDesC8& aLUID, 
                                   const TInt aStatusRef )
    {
    PRES_DM_LOG(LOG_LIT8("DeleteObjectL(%S): begins"), &aUri);   
    //only node object deletion is possible

    CSmlDmAdapter::TError status = CSmlDmAdapter::EError;
    if ( !NSmlDmURI::RemoveLastSeg( aUri ).Compare( KPresDMNodeName ) )
        {
        TInt id = DesToInt( aLUID );
        TInt error = PresSettingsApi::RemoveSetL(id); 
        if ( error == KErrNone )
            {
            status = CSmlDmAdapter::EOk;
            }
    PRES_DM_LOG(LOG_LIT("   id(%d), error(%d), satus(%d)"), id, error, status);       
        }
    Callback().SetStatusL( aStatusRef, status );
    PRES_DM_LOG(LOG_LIT(" DeleteObjectL: ends"));   
    }
    
// -----------------------------------------------------------------------------
// CPresenceDMAdapter::UpdateLeafObjectL
// -----------------------------------------------------------------------------
//
void CPresenceDMAdapter::UpdateLeafObjectL( const TDesC8& /*aUri*/, 
                                           const TDesC8& /*aLUID*/, 
                                           RWriteStream*& /*aStream*/, 
                                           const TDesC8& /*aType*/, 
                                           const TInt aStatusRef )
    {
    PRES_DM_LOG(LOG_LIT("UpdateLeafObjectL: begins"));      
    // Update from stream not used
    Callback().SetStatusL(aStatusRef, CSmlDmAdapter::EError);
    
    PRES_DM_LOG(LOG_LIT(" UpdateLeafObjectL(stream): end"));      
    }
    
// -----------------------------------------------------------------------------
// CPresenceDMAdapter::ExecuteCommandL()
// -----------------------------------------------------------------------------
//
void CPresenceDMAdapter::ExecuteCommandL( const TDesC8& /*aUri*/, 
                                         const TDesC8& /*aLUID*/, 
                                         const TDesC8& /*aArgument*/, 
                                         const TDesC8& /*aType*/, 
                                         const TInt aStatusRef )
    {
    // Not supported 
    Callback().SetStatusL( aStatusRef, CSmlDmAdapter::EError );
    }

// -----------------------------------------------------------------------------
// CPresenceDMAdapter::ExecuteCommandL()
// -----------------------------------------------------------------------------
//
void CPresenceDMAdapter::ExecuteCommandL( const TDesC8& /*aUri*/, 
                                         const TDesC8& /*aLUID*/, 
                                         RWriteStream*& /*aStream*/, 
                                         const TDesC8& /*aType*/, 
                                         const TInt aStatusRef )
    {
    // Not supported
    Callback().SetStatusL( aStatusRef, CSmlDmAdapter::EError );
    }

// -----------------------------------------------------------------------------
// CPresenceDMAdapter::CopyCommandL()
// -----------------------------------------------------------------------------
//
void CPresenceDMAdapter::CopyCommandL( const TDesC8& /*aTargetURI*/, 
                                      const TDesC8& /*aTargetLUID*/, 
                                      const TDesC8& /*aSourceURI*/, 
                                      const TDesC8& /*aSourceLUID*/, 
                                      const TDesC8& /*aType*/, 
                                      TInt aStatusRef )
    {
    // Not supported
    Callback().SetStatusL( aStatusRef, CSmlDmAdapter::EError );
    }

// -----------------------------------------------------------------------------
// CPresenceDMAdapter::StartAtomicL()
// -----------------------------------------------------------------------------
//
void CPresenceDMAdapter::StartAtomicL()
    {
    // Not supported
    }

// -----------------------------------------------------------------------------
// CPresenceDMAdapter::CommitAtomicL()
// -----------------------------------------------------------------------------
//
void CPresenceDMAdapter::CommitAtomicL()
    {
    // Not supported
    }

// -----------------------------------------------------------------------------
// CPresenceDMAdapter::RollbackAtomicL()
// -----------------------------------------------------------------------------
//
void CPresenceDMAdapter::RollbackAtomicL()
    {
    // Not supported
    }

// -----------------------------------------------------------------------------
// CPresenceDMAdapter::StreamingSupport()
// -----------------------------------------------------------------------------
//
TBool CPresenceDMAdapter::StreamingSupport( TInt& /*aItemSize*/ )
    {
    return EFalse;
    }

// -----------------------------------------------------------------------------
// CPresenceDMAdapter::StreamCommittedL()
// -----------------------------------------------------------------------------
//
void CPresenceDMAdapter::StreamCommittedL()
    {
    // Not supported
    }
    
// -----------------------------------------------------------------------------
// CPresenceDMAdapter::CompleteOutstandingCmdsL()
// -----------------------------------------------------------------------------
//
void CPresenceDMAdapter::CompleteOutstandingCmdsL()
    {
    PRES_DM_LOG(LOG_LIT("CompleteOutstandingCmdsL"));      
    }

// -----------------------------------------------------------------------------
// CPresenceDMAdapter::GetPropertyL
// -----------------------------------------------------------------------------
//
CSmlDmAdapter::TError CPresenceDMAdapter::GetPropertyL( TInt aSetId, 
                                                  TPresSettingsProperty aProperty, 
                                                  CBufBase& aObject )
    {
    PRES_DM_LOG(LOG_LIT(" GetPropertyL(%d, %d)"),aSetId,aProperty);      
    
    HBufC* value = NULL;
    TInt error( KErrNone );
    
    
    // Get the set only if we dont have it in internal buffer
    if(iGetSetID != aSetId)
        {
        error = PresSettingsApi:: SettingsSetL(aSetId, iGetSet);
        if(error==KErrNone)
            iGetSetID = aSetId;
        PRES_DM_LOG(LOG_LIT("   error(%d)"),error);  
        }
        
    if (error == KErrNone)
        {
        switch (aProperty)
            {
            case EPresPropSetName:
                value = (iGetSet.iSetName).AllocLC();                    
                break;
            case EPresPropProviderID:
                value = (iGetSet.iProviderID).AllocLC();
                break;
            case EPresPropSIPProfile:
                return GetConRefFromSipIDL(iGetSet.iSipProfile, aObject);
            case EPresPropXDMSet:
                return GetConRefFromXdmIDL(iGetSet.iXDMSetting, aObject);
            case EPresPropObjectSize:
                value = IntToDesLC(iGetSet.iObjectSize);
                break;
            case EPresPropContSrvAdd:
                value = (iGetSet.iContSrvrAdd).AllocLC();
                break;
            case EPresPropPubInterval:
                value = IntToDesLC(iGetSet.iPublicationInt);
                break;
            case EPresPropMaxSubs:
                value = IntToDesLC(iGetSet.iMaxSubscriptions);
                break;
            case EPresPropMaxCntInList:
                value = IntToDesLC(iGetSet.iMaxContactsInList);
                break;
            case EPresPropDomainSyntax:
                value = (iGetSet.iDomainSyntax).AllocLC();
                break;
            default:
                error = KErrNotFound;
                break;
            }
        }
        
    PRES_DM_LOG(LOG_LIT("   value = %S"), &(*value));       
        
    if ( error == KErrNone )
        {
        HBufC8* utfValue = ConvertLC( *value ); // << utfValue
        aObject.InsertL( 0, *utfValue );
        CleanupStack::PopAndDestroy(utfValue);  // >>> utfValue
        CleanupStack::PopAndDestroy(value);  // >>> value
        PRES_DM_LOG(LOG_LIT(" return(%d)"),CSmlDmAdapter::EOk);  
        return CSmlDmAdapter::EOk;
        }
    PRES_DM_LOG(LOG_LIT(" return(%d)"),CSmlDmAdapter::ENotFound);      
    return CSmlDmAdapter::ENotFound;
    }
    

// -----------------------------------------------------------------------------
// CPresenceDMAdapter::UpdatePropertyL
// -----------------------------------------------------------------------------
//
CSmlDmAdapter::TError CPresenceDMAdapter::UpdatePropertyL( TInt aSetId, 
                                                     TPresSettingsProperty aProperty, 
                                                     const TDesC8& aObject )
    {
    PRES_DM_LOG(LOG_LIT(" UpdatePropertyL(%d, %d)"),aSetId,aProperty);     
    
    TInt valueInt(KErrNotFound);
    HBufC* value = ConvertLC(aObject); // value
    TPtr ptr = value->Des();
    CSmlDmAdapter::TError result = CSmlDmAdapter::EError;
    TInt error(KErrNone);
    
    // we handle SIP and XDM references in a different way
    if(aProperty == EPresPropSIPProfile)
        {
        valueInt = GetSipIdFromConRefL(aObject);
        }
        
    else if(aProperty == EPresPropXDMSet)
        {
        valueInt = GetXdmIdFromConRefL(aObject);
        }
    
    // for all other properties use normal way to get value    
    else
        {
        valueInt = DesToInt(aObject); 
        }
        
    // for str properties
    if(PresSettingsApi::IsStr(aProperty))
        {
        if (SetProperLength(aProperty, ptr))
            {
            if(aProperty == EPresPropSetName)
                {
                CheckDuplicateNameL(ptr, aSetId);                
                }
            PRES_DM_LOG(LOG_LIT("   value = %S"),&(*value));      
            error = PresSettingsApi::UpdatePropertyL( aSetId, aProperty, *value);
            if(error == KErrNone)
                result = CSmlDmAdapter::EOk;    
            
            }
        else
            result = CSmlDmAdapter::EInvalidObject;        
        }
    // for int properties    
    else if(valueInt > KErrNotFound) // if valid value
        {
        PRES_DM_LOG(LOG_LIT("   valueInt = %d"), valueInt);
        error = PresSettingsApi::UpdatePropertyL( aSetId, aProperty, valueInt);
        if(error == KErrNone)
            result = CSmlDmAdapter::EOk;
        }
    else
        result = CSmlDmAdapter::ENotFound;
    
    CleanupStack::PopAndDestroy(value);
    
    
    PRES_DM_LOG(LOG_LIT("   error(%d) , result(%d)"), error, result);
    return result;
    }

// -----------------------------------------------------------------------------
// CPresenceDMAdapter::Match
// -----------------------------------------------------------------------------
//
TBool CPresenceDMAdapter::Match( const TDesC8& aLeft, const TDesC8& aRight )
    {
    if ( !aLeft.Compare( aRight ) )
        {
        return ETrue;
        }
    return EFalse;
    }         
 
 // -----------------------------------------------------------------------------
// CPresenceDMAdapter::FillNodeInfoL
// ------------------------------------------------------------------------------
//
void CPresenceDMAdapter::FillNodeInfoL( MSmlDmDDFObject& aNode,
                                       TSmlDmAccessTypes aAccTypes,
                                       MSmlDmDDFObject::TOccurence aOccurrence, 
                                       MSmlDmDDFObject::TScope aScope, 
                                       MSmlDmDDFObject::TDFFormat aFormat,
                                       const TDesC8& aDescription)
    {
    aNode.SetAccessTypesL( aAccTypes );
    aNode.SetOccurenceL( aOccurrence );
    aNode.SetScopeL( aScope );
    aNode.SetDFFormatL( aFormat );
    
    if( aFormat!= MSmlDmDDFObject::ENode )
        {
        aNode.AddDFTypeMimeTypeL( KPresDMTextPlain );
        }
    aNode.SetDescriptionL( aDescription );
    }
    
// -----------------------------------------------------------------------------
// CPresenceDMAdapter::ConvertLC
// -----------------------------------------------------------------------------
//  
HBufC8* CPresenceDMAdapter::ConvertLC( const TDesC& aSource ) const
    {   
    HBufC8* destination = HBufC8::NewLC( aSource.Length() );
    TPtr8 bufferPtr = destination->Des();
    CnvUtfConverter::ConvertFromUnicodeToUtf8( bufferPtr, aSource );
    return destination;
    }   
    
// -----------------------------------------------------------------------------
// CPresenceDMAdapter::ConvertLC
// -----------------------------------------------------------------------------
//  
HBufC*  CPresenceDMAdapter::ConvertLC( const TDesC8& aSource ) const 
    {   
    HBufC* destination = HBufC::NewLC(aSource.Length());
    TPtr bufferPtr = destination->Des();
    CnvUtfConverter::ConvertToUnicodeFromUtf8( bufferPtr, aSource );
    return destination;
    }

// ----------------------------------------------------------------------------
// CPresenceDMAdapter::DesToInt
// ----------------------------------------------------------------------------
TInt CPresenceDMAdapter::DesToInt( const TDesC8& aLuid ) const
    {
    TLex8 lex( aLuid );
    TInt value ( KErrNotFound );
    lex.Val( value );
    return value;
    }

// ----------------------------------------------------------------------------
// CPresenceDMAdapter::IntToDes8LC
// ----------------------------------------------------------------------------
HBufC8* CPresenceDMAdapter::IntToDes8LC( const TInt aLuid ) const
    {
    // 10 = max length of 32bit integer
    HBufC8* buf = HBufC8::NewLC( KPresDMMaxIntLength ); // << buf
    TPtr8 ptrBuf = buf->Des();
    ptrBuf.Num( aLuid );
    return buf;
    }

// ----------------------------------------------------------------------------
// CPresenceDMAdapter::IntToDesLC
// ----------------------------------------------------------------------------
HBufC* CPresenceDMAdapter::IntToDesLC( const TInt aLuid ) const
    {
    // 10 = max length of 32bit integer
    HBufC* buf = HBufC::NewLC( KPresDMMaxIntLength ); // << buf
    TPtr ptrBuf = buf->Des();
    ptrBuf.Num( aLuid );
    return buf;
    }
    
// -----------------------------------------------------------------------------
// CPresenceDMAdapter::GetConRefFromSipIDL
// -----------------------------------------------------------------------------
//
CSmlDmAdapter::TError CPresenceDMAdapter::GetConRefFromSipIDL( TInt aSipID,
                                                      CBufBase& aObject)
    {
    PRES_DM_LOG(LOG_LIT(" GetConRefFromSipIDL(%d)"),aSipID);

    CSmlDmAdapter::TError status = CSmlDmAdapter::EOk;
    
    TBool found(EFalse);
    
    if( aSipID != KErrNotFound )
        {
        CBufBase* result = CBufFlat::NewL(1);
        CleanupStack::PushL(result);    // << result
        
        // Request all the sip settings identifiers 
        Callback().FetchLinkL( KPresDMSipDMNode, *result, status);
        
        if( status == EOk )
            {
            TPtr8 uriSeg8Ptr = result->Ptr(0);
            
            HBufC8* uriSegBuffer = uriSeg8Ptr.AllocLC();
            
            TPtr8 uriSegBufferPtr = uriSegBuffer->Des();
            
            TInt numOfUriSegs = NSmlDmURI::NumOfURISegs(uriSeg8Ptr);
            
            TBuf8<100> idLinkBuffer;
            
            TInt foundId(KErrNotFound);
            
                     
            // Check if given SIP set id match with any of found
            // SIP settings identifiers
            while( numOfUriSegs > 1)
                {
                idLinkBuffer = NSmlDmURI::LastURISeg(uriSegBufferPtr);
                uriSegBufferPtr = NSmlDmURI::RemoveLastSeg(uriSegBufferPtr);
                
                idLinkBuffer.Insert(0, KPresDMSipDMNode);
                idLinkBuffer.Insert(3, KPresDMSeparator);
                idLinkBuffer.Append(KPresDMSeparator);
                idLinkBuffer.Append(KPresDMSipIdPrefix);
                
                TPtrC8 finalLink = idLinkBuffer;
                        
                //Reqest the ID
                result->Reset();
                Callback().FetchLinkL( finalLink, *result, status);
                        
                foundId = DesToInt(result->Ptr(0));
                        
                if (foundId == aSipID)
                    {
                    found = ETrue;
                    aObject.InsertL(0, NSmlDmURI::RemoveLastSeg(finalLink));
                        break;
                        }
                
                numOfUriSegs -= 1;  
                }
            CleanupStack::PopAndDestroy( uriSegBuffer );    // >>> uriSegBuffer
            }
        
        CleanupStack::PopAndDestroy( result );    // >>> result
        }
    if(found == EFalse)
        {
        status = CSmlDmAdapter::ENotFound;
        }
        
    PRES_DM_LOG(LOG_LIT(" GetConRefFromSipIDL(%d): ends"), status);    
    
    return status;
    
    }
    
// -----------------------------------------------------------------------------
// CPresenceDMAdapter::GetConRefFromXdmIDL
// -----------------------------------------------------------------------------
//
CSmlDmAdapter::TError CPresenceDMAdapter::GetConRefFromXdmIDL( TInt aXdmID,
                                                      CBufBase& aObject)
    {
    PRES_DM_LOG(LOG_LIT(" GetConRefFromXdmIDL(%d)"),aXdmID);

    CSmlDmAdapter::TError status = CSmlDmAdapter::ENotFound;
    
    if( aXdmID != KErrNotFound )
        {
        CBufBase* result = CBufFlat::NewL(1);
        CleanupStack::PushL(result);    // << result
        
        // Request all the XDM settings identifiers 
        Callback().FetchLinkL( KPresDMXdmDMNode, *result, status);
        TInt id(KErrNotFound);
        
        TPtrC8 lastUriSegXdm;
        
        if( status == EOk )
            {
            TPtr8 uriSeg8Ptr = result->Ptr(0);
            
            HBufC8* uriSegBuffer = uriSeg8Ptr.AllocLC();
            
            TPtr8 uriSegBufferPtr = uriSegBuffer->Des();
            
            TInt numOfUriSegs = NSmlDmURI::NumOfURISegs(uriSeg8Ptr);
            
            // Check if the Presence settings XDM settings id match with one of the found
            // XDM settings identifiers
            while( numOfUriSegs > 0)
                {
                lastUriSegXdm.Set(NSmlDmURI::LastURISeg(uriSegBufferPtr));
                uriSegBufferPtr = NSmlDmURI::RemoveLastSeg(uriSegBufferPtr);
                id = DesToInt(lastUriSegXdm);

                if(id == aXdmID)
                    {
                    // Total length of the sip settings link where one is for
                    // the separator
                    TInt XDMLinkLength = KPresDMXdmDMNode().Length() + 
                                         KPresDMSeparator().Length() +
                                         lastUriSegXdm.Length();
                    HBufC8* XDMLink = HBufC8::NewLC( XDMLinkLength );   // <<< XDMLink
                    
                    TPtr8 XDMLinkPtr = XDMLink->Des();
                    
                    XDMLinkPtr.Append( KPresDMXdmDMNode );
                    XDMLinkPtr.Append( KPresDMSeparator );
                    XDMLinkPtr.Append( lastUriSegXdm );
                    
                    aObject.InsertL( 0, XDMLinkPtr );
                    
                    CleanupStack::PopAndDestroy( XDMLink );   // >>> XDMLink
                    status = CSmlDmAdapter::EOk;
                    break;
                    }
                numOfUriSegs--;
                }
            CleanupStack::PopAndDestroy( uriSegBuffer );    // >>> uriSegBuffer
            }
        CleanupStack::PopAndDestroy( result );    // >>> result
        }
        
    PRES_DM_LOG(LOG_LIT(" GetConRefFromXdmIDL(%d): ends"), status);   
    return status;
    } 
    
// -----------------------------------------------------------------------------
// CPresenceDMAdapter::IsPresIDValidL
// -----------------------------------------------------------------------------
//
TBool CPresenceDMAdapter::IsPresIDValidL(TInt aSetId)
    {
    TPresSettingsSet tempSet;
    if ((PresSettingsApi::SettingsSetL(aSetId,tempSet)) == KErrNone)
        return ETrue;
    return EFalse;   
    }
    
// -----------------------------------------------------------------------------
// CPresenceDMAdapter::GetSipIdFromConRefL
// -----------------------------------------------------------------------------
//
TInt CPresenceDMAdapter::GetSipIdFromConRefL(const TDesC8& aUri )
    {
    PRES_DM_LOG(LOG_LIT8(" GetSipIdFromConRefL(%S)"),&aUri);
    
    CSmlDmAdapter::TError status = EOk;
    TInt returnId(KErrNotFound);
    
    CBufBase* result = CBufFlat::NewL(1);
    CleanupStack::PushL(result);    // << result

    // make all SIP profiles known
    Callback().FetchLinkL( KPresDMSipDMNode, *result, status );
    
    //TBuf8<300> resultBuf;
    //result->Read(0, resultBuf,result->Size());
    
    TPtrC8 uri = NSmlDmURI::RemoveDotSlash( aUri );
    TBuf8<50> object = uri;
        
    object.Append( KPresDMSeparator );
    object.Append( KPresDMSipIdPrefix );
    
    result->Reset();
    Callback().FetchLinkL( object, *result, status);
    
    //resultBuf.Zero();
    //result->Read(0, resultBuf,result->Size());
        
    if( status == EOk )
        {
        TUint32 id(NULL);
        id = DesToInt( result->Ptr( 0 ) );
        returnId = (TInt)id;       
        }
    CleanupStack::PopAndDestroy( result );   // >>> result   
    
    PRES_DM_LOG(LOG_LIT("   status(%d), returnId(%d): end"), status, returnId);
  
    return returnId;       
    }
        
// -----------------------------------------------------------------------------
// CPresenceDMAdapter::GetXdmIdFromConRefL
// -----------------------------------------------------------------------------
//
TInt CPresenceDMAdapter::GetXdmIdFromConRefL(const TDesC8& aUri )
    {
    PRES_DM_LOG(LOG_LIT8(" GetXdmIdFromConRefL(%S)"),&aUri);
    const TInt KPresDMIdDesSize = 10;
    
    CSmlDmAdapter::TError status = EOk;
    TInt id( KErrNotFound );
    TBuf8<KPresDMIdDesSize> idDes;
    
    CBufBase* result = CBufFlat::NewL(1);
    CleanupStack::PushL(result);    // << result

    TPtrC8 uri = NSmlDmURI::RemoveDotSlash( aUri );
    // Request the xdm settings identifier
    Callback().FetchLinkL( uri, *result, status);
        
    if( status == EOk ) // if the link is valid. Wont use result
        {
        TPtrC8 idSegment = NSmlDmURI::LastURISeg(aUri);
        id = DesToInt(idSegment);
        }
    CleanupStack::PopAndDestroy( result );   // >>> result   
    
    PRES_DM_LOG(LOG_LIT("   status(%d), id(%d): end"), status, id);
    return id;       
    }
    
// ---------------------------------------------------------------------------
// CPresenceDMAdapter::GetPrefixL()
// ---------------------------------------------------------------------------
// 
HBufC* CPresenceDMAdapter::GetPrefixL( HBufC* aName )
    {
    TPtrC prefix = aName->Des();
    TInt lastBrace = aName->LocateReverse('(');
    if ( lastBrace != KErrNotFound )
        {
        // aName looks like "<prefix><brace><something>".
        // See if <something> is an integer number and
        // <something> ends to a brace.
        TPtrC num = aName->Right( aName->Length() - lastBrace - 1 );
        TInt val;
        TLex lex( num );
        if ( lex.Val( val ) == KErrNone && num.Locate(')') == num.Length() - 1 )
            {
            // Yes, the trailer is an integer.
            prefix.Set( aName->Left( lastBrace ) );
            }
        }
    return prefix.AllocL();
    }    
    
// -----------------------------------------------------------------------------
// CPresenceDMAdapter::CheckDuplicateNameL
// Checks if duplicate named Presence Settings. Rename if same.
// -----------------------------------------------------------------------------
void CPresenceDMAdapter::CheckDuplicateNameL( TDes& aSetName, const TInt& aSetId)
    {
    PRES_DM_LOG(LOG_LIT(" CheckDuplicateNameL(%S, %d)"),&aSetName, aSetId);
    
	_LIT( KPresDMNumbering, "(%N)" );
    const TInt KPresDMFormatExtraLen = 6;
    
    HBufC* newName = HBufC::NewLC(KPresSetNameMaxLength);
    newName->Des().Copy(aSetName);

	RArray<TInt> setIds;
	CleanupClosePushL( setIds );
	CDesCArray* names = PresSettingsApi::GetAllSetsNamesLC(setIds);
	TInt count(setIds.Count());
	
    newName = newName->ReAlloc( 
        newName->Length() + KPresDMFormatExtraLen );// enough for formatting

    HBufC* tempSetName = NULL;
    TInt pos = 0;// used as dummy
    TInt found = 0;
    TInt index = 1;
    TPtr ptr = newName->Des();
    
    // do the actual checking of name
    found = names->Find( *newName, pos );
    while ( found == 0 )
        {
        if ( !tempSetName )
            {
            tempSetName = GetPrefixL( newName );
            tempSetName = tempSetName->ReAlloc( 
                tempSetName->Length() + KPresDMFormatExtraLen );// enough for formatting
            CleanupStack::PushL( tempSetName );
            tempSetName->Des().Append( KPresDMNumbering );
            }
        StringLoader::Format( ptr, *tempSetName, -1, index );
        found = names->Find( *newName, pos );
        index++;
        }

    // cleanup
    if ( tempSetName )
        {
        CleanupStack::PopAndDestroy( tempSetName ); //tempSetName
        }
    
    CleanupStack::PopAndDestroy(names); // names
    CleanupStack::PopAndDestroy(1); // setIds 

    aSetName.Copy(newName->Des());
    CleanupStack::PopAndDestroy( newName );
    
    PRES_DM_LOG(LOG_LIT(" CheckDuplicateNameL(%S): end"),&aSetName);
    }
    
// -----------------------------------------------------------------------------
// CPresenceDMAdapter::SetProperLength
// -----------------------------------------------------------------------------
TBool CPresenceDMAdapter::SetProperLength(TPresSettingsProperty aProp, TPtr& aValue)
    {
    PRES_DM_LOG(LOG_LIT(" SetProperLength(%d)"),aProp);
    
    TBool result(ETrue);
    switch (aProp)
        {
        case EPresPropSetName:
            if(aValue.Length() > KPresSetNameMaxLength)
                aValue = aValue.Left(KPresSetNameMaxLength);
            break;
        
        case EPresPropContSrvAdd:
            if(aValue.Length() > KContentSrvrAddMaxLength)
                result = EFalse;            
            break;
            
        case EPresPropDomainSyntax:
            if(aValue.Length() > KPresDomainSyntxMaxLength)
                result = EFalse;
            break;
        
        case EPresPropProviderID:
            if(aValue.Length() > KPresProviderIDMaxLength)
                aValue = aValue.Left(KPresProviderIDMaxLength);
            break;
        default:
            break;
        }
    PRES_DM_LOG(LOG_LIT(" SetProperLength(%d): end"),result);
    return result;   
    }

// End of File



