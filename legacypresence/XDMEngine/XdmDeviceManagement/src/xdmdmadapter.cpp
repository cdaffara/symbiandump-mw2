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
* Description:    DM OMA XDM Settings Adapter
 *
*/




#include <implementationproxy.h>
#include <utf.h>
#include <nsmldmuri.h>
#include <XdmSettingsApi.h>
#include <XdmSettingsCollection.h>
#include <XdmSettingsProperty.h>
#include "xdmdmadapter.h"

#ifdef _DEBUG
    #include <flogger.h>
#endif



// -----------------------------------------------------------------------------
// CXdmDMAdapter::NewL( )
// -----------------------------------------------------------------------------
//
CXdmDMAdapter* CXdmDMAdapter::NewL( MSmlDmCallback* aDmCallback )
    {
    CXdmDMAdapter* self = new (ELeave) CXdmDMAdapter( aDmCallback );
#ifdef _DEBUG
    WriteToLog(_L8("CXdmDMAdapter::NewL(): end") );
#endif
    return self;
    }
    
// -----------------------------------------------------------------------------
// CXdmDMAdapter::CXdmDMAdapter()
// -----------------------------------------------------------------------------
//
CXdmDMAdapter::CXdmDMAdapter( MSmlDmCallback* aDmCallback ) : 
CSmlDmAdapter( aDmCallback )
    {
#ifdef _DEBUG
    WriteToLog(_L8("CXdmDMAdapter::CXdmDMAdapter()") );
#endif
    }

// -----------------------------------------------------------------------------
// CXdmDMAdapter::~CXdmDMAdapter()
// -----------------------------------------------------------------------------
//
CXdmDMAdapter::~CXdmDMAdapter()
    {
#ifdef _DEBUG
    WriteToLog(_L8("CXdmDMAdapter::~CXdmDMAdapter()") );
#endif
    }

// -----------------------------------------------------------------------------
//  CXdmDMAdapter::DDFVersionL
// -----------------------------------------------------------------------------
//
void CXdmDMAdapter::DDFVersionL( CBufBase& aDDFVersion )
    {   
    aDDFVersion.InsertL( 0, KXdmDDFVersion );
#ifdef _DEBUG
    WriteToLog(_L8("CXdmDMAdapter::DDFVersionL(TDes& aDDFVersion)") );
#endif
    }

// -----------------------------------------------------------------------------
//  CXdmDMAdapter::DDFStructureL
// -----------------------------------------------------------------------------
//
void CXdmDMAdapter::DDFStructureL( MSmlDmDDFObject& aDDF )
    {
#ifdef _DEBUG
    WriteToLog(_L8("CXdmDMAdapter::DDFStructureL(): begin") );
#endif    

    // For OMA_XDM 
    TSmlDmAccessTypes accTypeGet;
    accTypeGet.SetGet();
     
    // For OMA_XDM/<X> 
    TSmlDmAccessTypes accTypeAll;
    accTypeAll.SetAdd();
    accTypeAll.SetGet();
    accTypeAll.SetReplace();
    accTypeAll.SetDelete();
    
    // For leaf nodes
    TSmlDmAccessTypes accTypeNoDelete;
    accTypeNoDelete.SetAdd();
    accTypeNoDelete.SetGet();
    accTypeNoDelete.SetReplace();

    // ./OMA_XDM
    MSmlDmDDFObject& omaXdmNode = aDDF.AddChildObjectL( KXdmDmNodeName );
    FillNodeInfoL( omaXdmNode, accTypeGet, MSmlDmDDFObject::EOne,
                  MSmlDmDDFObject::EPermanent, MSmlDmDDFObject::ENode,
                  KXdmDmNodeNameDescription );
                  
    // ./OMA_XDM/<X> 
    MSmlDmDDFObject& rtNode = omaXdmNode.AddChildObjectGroupL();
    FillNodeInfoL( rtNode, accTypeAll, MSmlDmDDFObject::EOneOrMore,
                  MSmlDmDDFObject::EDynamic, MSmlDmDDFObject::ENode,
                  KXdmDmDynamicDescription );

    // ./OMA_XDM/<X>/APPID            
    MSmlDmDDFObject& appIdNode = rtNode.AddChildObjectL( KXdmDmAppId );
    FillNodeInfoL( appIdNode, accTypeNoDelete, MSmlDmDDFObject::EZeroOrOne,
                  MSmlDmDDFObject::EDynamic, MSmlDmDDFObject::EChr,
                  KXdmDmAppIdDescription );
                  
    // ./OMA_XDM/<X>/NAME            
    MSmlDmDDFObject& nameNode = rtNode.AddChildObjectL( KXdmDmName );
    FillNodeInfoL( nameNode, accTypeNoDelete, MSmlDmDDFObject::EZeroOrOne,
                  MSmlDmDDFObject::EDynamic, MSmlDmDDFObject::EChr,
                  KXdmDmNameDescription );
            
    // ./OMA_XDM/<X>/PROVIDER-ID           
    MSmlDmDDFObject& provIdNode = rtNode.AddChildObjectL( KXdmDmProviderId );
    FillNodeInfoL( provIdNode, accTypeNoDelete,MSmlDmDDFObject::EZeroOrOne,
                  MSmlDmDDFObject::EDynamic, MSmlDmDDFObject::EChr,
                  KXdmDmProviderIdDescription );    
                               
    // ./OMA_XDM/<X>/ToConRef
    MSmlDmDDFObject& toConRefNode = rtNode.AddChildObjectL( KXdmDmToConRef );
    FillNodeInfoL( toConRefNode, accTypeNoDelete, MSmlDmDDFObject::EZeroOrOne,
                  MSmlDmDDFObject::EDynamic, MSmlDmDDFObject::ENode,
                  KXdmDmToConRefDescription);
                  
    // ./OMA_XDM/<X>/ToConRef/<X> (TO-NAPID)          
    MSmlDmDDFObject& napIdNode = toConRefNode.AddChildObjectL( KXdmDmToNapId );
    FillNodeInfoL( napIdNode, accTypeNoDelete, MSmlDmDDFObject::EZeroOrOne, 
                  MSmlDmDDFObject::EDynamic, MSmlDmDDFObject::ENode,
                  KXdmDmToNapIdDescription );                 
                                   
    // ./OMA_XDM/<X>/ToConRef/TO-NAPID/ConRef                                             
    MSmlDmDDFObject& napIdRefNode = napIdNode.AddChildObjectL( KXdmDmConRef );
    FillNodeInfoL( napIdRefNode, accTypeNoDelete, MSmlDmDDFObject::EZeroOrOne, 
                  MSmlDmDDFObject::EDynamic, MSmlDmDDFObject::EChr,
                  KXdmDmConRefDescription );
                  
    // ./OMA_XDM/<X>/ToConRef/<X> (SIP)          
    MSmlDmDDFObject& sipNode = toConRefNode.AddChildObjectL( KXdmDmSip );
    FillNodeInfoL( sipNode, accTypeNoDelete, MSmlDmDDFObject::EZeroOrOne, 
                  MSmlDmDDFObject::EDynamic, MSmlDmDDFObject::ENode,
                  KXdmDmSipDescription );                 
                                   
    // ./OMA_XDM/<X>/ToConRef/SIP/ConRef                                             
    MSmlDmDDFObject& sipRefNode = sipNode.AddChildObjectL( KXdmDmConRef );
    FillNodeInfoL( sipRefNode, accTypeNoDelete, MSmlDmDDFObject::EZeroOrOne, 
                  MSmlDmDDFObject::EDynamic, MSmlDmDDFObject::EChr,
                  KXdmDmConRefDescription );
    
    // ./OMA_XDM/<X>/URI          
    MSmlDmDDFObject& uriNode = rtNode.AddChildObjectL( KXdmDmUri );
    FillNodeInfoL( uriNode, accTypeNoDelete, MSmlDmDDFObject::EOne,
                  MSmlDmDDFObject::EDynamic, MSmlDmDDFObject::EChr,
                  KXdmDmUriDescription );  
                  
    // ./OMA_XDM/<X>/AAUTHNAME         
    MSmlDmDDFObject& userIdNode = rtNode.AddChildObjectL( KXdmDmAAuthName );
    FillNodeInfoL( userIdNode, accTypeNoDelete, MSmlDmDDFObject::EZeroOrOne,
                  MSmlDmDDFObject::EDynamic, MSmlDmDDFObject::EChr,
                  KXdmDmAAuthNameDescription );     
                  
    // ./OMA_XDM/<X>/AAUTHSECRET         
    MSmlDmDDFObject& pwdNode = rtNode.AddChildObjectL( KXdmDmAAuthSecret );
    FillNodeInfoL( pwdNode, accTypeNoDelete, MSmlDmDDFObject::EZeroOrOne,
                  MSmlDmDDFObject::EDynamic,MSmlDmDDFObject::EChr,
                  KXdmDmAAuthSecretDescription ); 
                    
    // ./OMA_XDM/<X>/AAUTHTYPE         
    MSmlDmDDFObject& authTypeNode = rtNode.AddChildObjectL( KXdmDmAAuthType );
    FillNodeInfoL( authTypeNode, accTypeNoDelete, MSmlDmDDFObject::EZeroOrOne,
                  MSmlDmDDFObject::EDynamic,MSmlDmDDFObject::EChr,
                  KXdmDmAAuthTypeDescription );
  
#ifdef _DEBUG
    WriteToLog(_L8("CXdmDMAdapter::DDFStructureL(): end") );
#endif 
    }
    
// -----------------------------------------------------------------------------
// CXdmDMAdapter::ChildURIListL
// -----------------------------------------------------------------------------
//
void CXdmDMAdapter::ChildURIListL( const TDesC8& aUri, 
                                   const TDesC8& /*aLUID*/, 
                                   const CArrayFix<TSmlDmMappingInfo>& 
                                       /*aPreviousURISegmentList*/, 
                                   const TInt aResultsRef, 
                                   const TInt aStatusRef )
    {
#ifdef _DEBUG
    WriteToLog(_L8("CXdmDMAdapter::ChildURIListL(): begin") );
#endif    
    
    CSmlDmAdapter::TError retValue = CSmlDmAdapter::EOk;

    CBufBase* currentList = CBufFlat::NewL( 1 );
    CleanupStack::PushL( currentList ); // << currentList
    
    const TPtrC8 lastUriSeg = NSmlDmURI::LastURISeg( aUri );
    TBuf8<KSmlMaxURISegLen> segmentName;
 
    // ./OMA_XDM
    if( Match( lastUriSeg, KXdmDmNodeName ) ) 
        {
        RArray<TInt> settingIds;
        CleanupClosePushL( settingIds );    // << settingIds
        CDesCArray* names = TXdmSettingsApi::CollectionNamesLC( settingIds ); // << names
        TInt idCount = settingIds.Count();
        for( TInt i(0); i < idCount; i++ )
            {
            segmentName.Copy( KNullDesC8 ); 
            segmentName.AppendNum( settingIds[i] );
            currentList->InsertL( currentList->Size(), segmentName );
            currentList->InsertL( currentList->Size(), KXdmDmSeparator );
            }
        Callback().SetResultsL( aResultsRef, *currentList, KNullDesC8 );
        Callback().SetStatusL( aStatusRef, retValue );
        CleanupStack::PopAndDestroy( 3, currentList ); // >>> settingIds, names, currentList
        return;
        }   

    // ./OMA_XDM/X   
    if( NSmlDmURI::NumOfURISegs( aUri ) == KXdmDmLevel )   
        {
        segmentName.Copy( KXdmDmAppId );
        currentList->InsertL( currentList->Size(), segmentName );
        currentList->InsertL( currentList->Size(), KXdmDmSeparator );
        
        segmentName.Copy( KXdmDmName );
        currentList->InsertL( currentList->Size(), segmentName );
        currentList->InsertL( currentList->Size(), KXdmDmSeparator );
        
        segmentName.Copy( KXdmDmProviderId );
        currentList->InsertL( currentList->Size(), segmentName );
        currentList->InsertL( currentList->Size(), KXdmDmSeparator );
        
        segmentName.Copy( KXdmDmToConRef );
        currentList->InsertL( currentList->Size(), segmentName );
        currentList->InsertL( currentList->Size(), KXdmDmSeparator );
                
        segmentName.Copy( KXdmDmUri );
        currentList->InsertL( currentList->Size(), segmentName );
        currentList->InsertL( currentList->Size(), KXdmDmSeparator );
        
        segmentName.Copy( KXdmDmAAuthName );
        currentList->InsertL( currentList->Size(), segmentName );
        currentList->InsertL( currentList->Size(), KXdmDmSeparator );
        
        segmentName.Copy( KXdmDmAAuthSecret );
        currentList->InsertL( currentList->Size(), segmentName );
        currentList->InsertL( currentList->Size(), KXdmDmSeparator );
        
        segmentName.Copy( KXdmDmAAuthType );
        currentList->InsertL( currentList->Size(), segmentName );
        currentList->InsertL( currentList->Size(), KXdmDmSeparator );
        }
        
    // ./OMA_XDM/X/ToConRef   
    else if ( Match( lastUriSeg, KXdmDmToConRef ) ) 
        {
        segmentName.Copy( KXdmDmSip );
        currentList->InsertL( currentList->Size(), segmentName );
        currentList->InsertL( currentList->Size(), KXdmDmSeparator );
        
        segmentName.Copy( KXdmDmToNapId );
        currentList->InsertL( currentList->Size(), segmentName );
        currentList->InsertL( currentList->Size(), KXdmDmSeparator );
        }
        
    // ./OMA_XDM/X/ToConRef/SIP
    else if( Match( lastUriSeg, KXdmDmSip ) ) 
        {
        segmentName.Copy( KXdmDmConRef );
        currentList->InsertL( currentList->Size(), segmentName );
        currentList->InsertL( currentList->Size(), KXdmDmSeparator );
        }
        
    // ./OMA_XDM/X/ToConRef/TO-NAPID
    else if( Match ( lastUriSeg, KXdmDmToNapId ) ) 
        {
        segmentName.Copy( KXdmDmConRef );
        currentList->InsertL( currentList->Size(), segmentName );
        currentList->InsertL( currentList->Size(), KXdmDmSeparator );
        }
    else
        {
        // if none of asked nodes found return error.
        retValue = CSmlDmAdapter::ENotFound;
        }
       
    Callback().SetResultsL( aResultsRef, *currentList, KNullDesC8 );
    Callback().SetStatusL( aStatusRef, retValue );
    CleanupStack::PopAndDestroy( currentList ); // >>> currentList  
#ifdef _DEBUG
    WriteToLog(_L8("CXdmDMAdapter::ChildURIListL(): end") );
#endif
    }    
 
// -----------------------------------------------------------------------------
//  CXdmDMAdapter::FetchLeafObjectL
// -----------------------------------------------------------------------------
//
void CXdmDMAdapter::FetchLeafObjectL( const TDesC8& aUri, 
                                      const TDesC8& aLUID, 
                                      const TDesC8& aType, 
                                      const TInt aResultsRef, 
                                      const TInt aStatusRef )
    {
#ifdef _DEBUG
    WriteToLog(_L8("CXdmDMAdapter::FetchLeafObjectL(): begin") );
#endif  

    TInt settingsId = FindSettingsIdL( aLUID, aUri );

    CBufBase* result = CBufFlat::NewL( KXdmDefaultResultSize );
    CleanupStack::PushL( result );    // << result
    CSmlDmAdapter::TError status = FetchObjectL( aUri, *result, settingsId );
    if( status == CSmlDmAdapter::EOk )
        {
        Callback().SetResultsL( aResultsRef, *result, aType );
        }
    Callback().SetStatusL( aStatusRef, status );
    CleanupStack::PopAndDestroy( result );  // >>> result
    
#ifdef _DEBUG
    WriteToLog(_L8("CXdmDMAdapter::FetchLeafObjectL(): end") );
#endif
    }
    
// -----------------------------------------------------------------------------
// CSmlDmAdapter::TError FetchObjectL
// -----------------------------------------------------------------------------
//
CSmlDmAdapter::TError CXdmDMAdapter::FetchObjectL( const TDesC8& aUri, 
                                                   CBufBase& aObject,
                                                   TInt aSettingsId )
    {
#ifdef _DEBUG
    WriteToLog(_L8("CXdmDMAdapter::FetchObjectL(): begin") );
#endif    
    CSmlDmAdapter::TError status = CSmlDmAdapter::EOk;
      
    TXdmSettingsProperty property ( EXdmPropNotFound );
    TPtrC8 lastUriSeg = NSmlDmURI::LastURISeg( aUri );
    // APPID
    if( Match( lastUriSeg, KXdmDmAppId ) )
        {
        property = EXdmPropAppId;                          
        }
    // NAME
    else if( Match( lastUriSeg, KXdmDmName ) )
        {
        property = EXdmPropName;  
        }   
    // PROVIDER-ID  
    else if( Match( lastUriSeg, KXdmDmProviderId ) )
        {
        property = EXdmPropProvId;  
        }
    // URI
    else if( Match( lastUriSeg, KXdmDmUri) )
        {
        property = EXdmPropUri;  
        }
    // AAUTHNAME
    else if( Match( lastUriSeg, KXdmDmAAuthName) )
        {
        property = EXdmPropAuthName;  
        }
    // AAUTHSECRET
    else if( Match( lastUriSeg, KXdmDmAAuthSecret) )
        {
        property = EXdmPropAuthSecret;  
        }
    // AAUTHTYPE
    else if( Match( lastUriSeg, KXdmDmAAuthType ) )
        {
        property = EXdmPropAuthType;  
        }       
    // ToConRef/SIP/ConRef
    // ToConRef/TO-NAPID/ConRef     
    else if( Match( lastUriSeg, KXdmDmConRef) )
        {
        TPtrC8 conRefUri = NSmlDmURI::RemoveLastSeg( aUri );
        TPtrC8 lastConRefSeg = NSmlDmURI::LastURISeg( conRefUri );
        
        if( Match( lastConRefSeg, KXdmDmSip ) )
            {
            return FetchSipConRefL( aSettingsId, aObject );
            }  
        else if( Match( lastConRefSeg, KXdmDmToNapId ) )
            {
            HBufC* value = NULL;
            TInt error( KErrNone );
            TRAP( error, ( value = TXdmSettingsApi::PropertyL( aSettingsId, EXdmPropToNapId ) ) );
            if ( error == KErrNone )
                {
                CleanupStack::PushL( value );   // << value
                TInt iapId = DesToInt( *value ); 
                CleanupStack::PopAndDestroy( value );   // >>> value
                HBufC8* uri = URIFromIapIdL( iapId );
                if( uri )
                    {
                    CleanupStack::PushL( uri ); // << uri
                    aObject.InsertL( 0, uri->Des() );
                    status = CSmlDmAdapter::EOk;
                    CleanupStack::PopAndDestroy( uri ); // uri
                    }
                else
                    {
                    status = CSmlDmAdapter::ENotFound;
                    }
                return status;
                }
            }
        }
    else
        {
        status = CSmlDmAdapter::ENotFound;
        }
    // known property
    if ( status == CSmlDmAdapter::EOk )
        {
        status = GetPropertyL( aSettingsId, property, aObject ); 
        }

#ifdef _DEBUG
    WriteToLog(_L8("CXdmDMAdapter::FetchObjectL(): end") );
#endif    
    return status;
    }  
         
// -----------------------------------------------------------------------------
// CXdmDMAdapter::FetchLeafObjectSizeL
// -----------------------------------------------------------------------------
//
void CXdmDMAdapter::FetchLeafObjectSizeL( const TDesC8& aUri, 
                                          const TDesC8& aLUID, 
                                          const TDesC8& aType, 
                                          const TInt aResultsRef, 
                                          const TInt aStatusRef )
    {
#ifdef _DEBUG
    WriteToLog(_L8("CXdmDMAdapter::FetchLeafObjectSizeL(): begin") );
#endif

    TInt settingsId = FindSettingsIdL( aLUID, aUri );
    
    CBufBase* result = CBufFlat::NewL( KXdmDefaultResultSize );
    CleanupStack::PushL( result );
    CSmlDmAdapter::TError status = FetchObjectL( aUri, *result, settingsId );
    
    if( status == CSmlDmAdapter::EOk )
        {
        TInt objSizeInBytes = result->Size();
        
        TBuf8<KXdmDmObjectSizeWidth> stringObjSizeInBytes;
        stringObjSizeInBytes.Num( objSizeInBytes );
        
        result->Reset();
        result->InsertL( 0, stringObjSizeInBytes );
        
        Callback().SetResultsL( aResultsRef, *result, aType );
        }   
        
    Callback().SetStatusL( aStatusRef, status );
    CleanupStack::PopAndDestroy( result );  // >>> result
#ifdef _DEBUG
    WriteToLog(_L8("CXdmDMAdapter::FetchLeafObjectSizeL(): end") );
 #endif
    }

// -----------------------------------------------------------------------------
// CXdmDMAdapter::UpdateLeafObjectL()
// -----------------------------------------------------------------------------
//
void CXdmDMAdapter::UpdateLeafObjectL( const TDesC8& aUri, 
                                       const TDesC8& aLUID, 
                                       const TDesC8& aObject, 
                                       const TDesC8& /*aType*/, 
                                       const TInt aStatusRef )
    {
#ifdef _DEBUG
    WriteToLog(_L8("CXdmDMAdapter::UpdateLeafObjectL(): begin") );
#endif   
    CSmlDmAdapter::TError status = CSmlDmAdapter::EOk;

    TInt settingsId = FindSettingsIdL( aLUID, aUri );
    
    TXdmSettingsProperty property;
    TPtrC8 lastUriSeg = NSmlDmURI::LastURISeg( aUri );
    // APPID
    if( Match( lastUriSeg, KXdmDmAppId ) )
        {
        property = EXdmPropAppId;                          
        }
    // NAME
    else if( Match( lastUriSeg, KXdmDmName ) )
        {
        // first check if the new value is same as earlier
        HBufC* current = NULL;
        TInt error( KErrNone );
        TRAP( error, ( current = TXdmSettingsApi::PropertyL( settingsId, EXdmPropName ) ) );
        if ( error == KErrNone )
            {
            TBool same ( EFalse );
            CleanupStack::PushL( current );   // << current
            HBufC8* current8 = ConvertLC( *current ); // << current8
            if ( Match( current8->Des(), aObject ) )
                {
                same = ETrue;
                }
            CleanupStack::PopAndDestroy( 2, current8 );  // >>> current, current8
            if ( same )
                {
                Callback().SetStatusL( aStatusRef, status );
                return;
                }
            }
        // if the name is already in use, new name with index is created
        HBufC* value = CheckExistingNamesLC( aObject );     // << value
        TRAP( error, TXdmSettingsApi::UpdatePropertyL( settingsId, *value, EXdmPropName ) );
        CleanupStack::PopAndDestroy( value );                      // >>> value
        if ( error != KErrNone )
            {
            status = CSmlDmAdapter::ENotFound;
            }
        Callback().SetStatusL( aStatusRef, status );
        return;  
        }   
    // PROVIDER-ID  
    else if( Match( lastUriSeg, KXdmDmProviderId ) )
        {
        property = EXdmPropProvId;  
        }
    // URI
    else if( Match( lastUriSeg, KXdmDmUri) )
        {
        property = EXdmPropUri;  
        }
    // AAUTHNAME
    else if( Match( lastUriSeg, KXdmDmAAuthName) )
        {
        property = EXdmPropAuthName;  
        }
    // AAUTHSECRET
    else if( Match( lastUriSeg, KXdmDmAAuthSecret) )
        {
        property = EXdmPropAuthSecret;  
        }
    // AAUTHTYPE
    else if( Match( lastUriSeg, KXdmDmAAuthType ) )
        {
        property = EXdmPropAuthType;  
        }           
    // ToConRef/TO-NAPID/ConRef
    // ToConRef/SIP/ConRef
    else if( Match( lastUriSeg, KXdmDmConRef ) )
        {
        TPtrC8 conRefUri = NSmlDmURI::RemoveLastSeg( aUri );
        TPtrC8 lastConRefSeg = NSmlDmURI::LastURISeg( conRefUri );
        if( Match( lastConRefSeg, KXdmDmToNapId ) )
            {
            TInt iapId = IapIdFromURIL( aObject );
            HBufC8* idBuf = IntToDes8LC( iapId ); // << idBuf
            status = UpdatePropertyL( settingsId, EXdmPropToNapId, *idBuf ); 
            CleanupStack::PopAndDestroy( idBuf );        // >>> idBuf
            Callback().SetStatusL( aStatusRef, status ); 
 #ifdef _DEBUG
    WriteToLog(_L8("CXdmDMAdapter::UpdateLeafObjectL(): TO_NAPID/ConRef end") );
 #endif
            return;
            } 
        else if( Match( lastConRefSeg, KXdmDmSip ) )
            {
            TInt sipId = GetSipIdL( aObject );
            HBufC8* idBuf = IntToDes8LC( sipId ); // << idBuf
            status = UpdatePropertyL( settingsId, EXdmPropToAppRef, *idBuf ); 
            CleanupStack::PopAndDestroy( idBuf );        // >>> idBuf
            Callback().SetStatusL( aStatusRef, status ); 
 #ifdef _DEBUG
    WriteToLog(_L8("CXdmDMAdapter::UpdateLeafObjectL(): SIP/ConRef end") );
 #endif
            return;
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
    // known property
    if ( status == CSmlDmAdapter::EOk )
        {
        status = UpdatePropertyL( settingsId, property, aObject ); 
        }

    Callback().SetStatusL( aStatusRef, status ); 
        
#ifdef _DEBUG
    WriteToLog(_L8("CXdmDMAdapter::UpdateLeafObjectL(): end") );
 #endif
    }

// -----------------------------------------------------------------------------
// CXdmDMAdapter::AddNodeObjectL
// -----------------------------------------------------------------------------
//
void CXdmDMAdapter::AddNodeObjectL( const TDesC8& aUri, 
                                    const TDesC8& /*aParentLUID*/, 
                                    const TInt aStatusRef )
    {
#ifdef _DEBUG
    WriteToLog(_L8("CXdmDMAdapter::AddNodeObjectL(): begin") );
#endif    
  
    CSmlDmAdapter::TError status = CSmlDmAdapter::EError;
    if ( Match( NSmlDmURI::RemoveLastSeg( aUri ), KXdmDmNodeName ) )
        {
        // create new set
        CXdmSettingsCollection* collection = new ( ELeave ) CXdmSettingsCollection;
        CleanupStack::PushL( collection );  // << collection
        collection->AppendL( KXdmDefaultId, EXdmPropSettingsId );
        TInt id = TXdmSettingsApi::CreateCollectionL( *collection );
        HBufC8* luid = IntToDes8LC( id );   // << luid
        Callback().SetMappingL( aUri, *luid );
        CleanupStack::PopAndDestroy( 2, luid );   // >>> collection, luid  
        status =  CSmlDmAdapter::EOk;  
#ifdef _DEBUG
    WriteToLog(_L8("CXdmDMAdapter::AddNodeObjectL(): Settings created id=%d"), id );
#endif
        }
    Callback().SetStatusL( aStatusRef, status );
    
#ifdef _DEBUG
    WriteToLog(_L8("CXdmDMAdapter::AddNodeObjectL(): end") );
#endif
    }
    
// -----------------------------------------------------------------------------
// CXdmDMAdapter::DeleteObjectL
// -----------------------------------------------------------------------------
//
void CXdmDMAdapter::DeleteObjectL( const TDesC8& aUri, 
                                   const TDesC8& aLUID, 
                                   const TInt aStatusRef )
    {
#ifdef _DEBUG
    WriteToLog(_L8("CXdmDMAdapter::DeleteObjectL( ): begin") );
#endif

    CSmlDmAdapter::TError status = CSmlDmAdapter::EError;
    if ( Match( NSmlDmURI::RemoveLastSeg( aUri ), KXdmDmNodeName ) )
        {
        TInt id = FindSettingsIdL( aLUID, aUri );
        TInt error (KErrNone );
        TRAP( error, TXdmSettingsApi::RemoveCollectionL( id ) ); 
        if ( error == KErrNone )
            {
            status = CSmlDmAdapter::EOk;
            }
#ifdef _DEBUG
    WriteToLog(_L8("CXdmDMAdapter::DeleteObjectL(): Settings deleted id=%d"), id );
#endif
        }
    Callback().SetStatusL( aStatusRef, status );
    
#ifdef _DEBUG
    WriteToLog(_L8("CXdmDMAdapter::DeleteObjectL( ): end") );
#endif
    }
    
// -----------------------------------------------------------------------------
// CXdmDMAdapter::UpdateLeafObjectL
// -----------------------------------------------------------------------------
//
void CXdmDMAdapter::UpdateLeafObjectL( const TDesC8& /*aUri*/, 
                                       const TDesC8& /*aLUID*/, 
                                       RWriteStream*& /*aStream*/, 
                                       const TDesC8& /*aType*/, 
                                       const TInt aStatusRef )
    {  
    // Update from stream not used
    Callback().SetStatusL( aStatusRef, CSmlDmAdapter::EError );
    }
    
// -----------------------------------------------------------------------------
// CXdmDMAdapter::ExecuteCommandL()
// -----------------------------------------------------------------------------
//
void CXdmDMAdapter::ExecuteCommandL( const TDesC8& /*aUri*/, 
                                     const TDesC8& /*aLUID*/, 
                                     const TDesC8& /*aArgument*/, 
                                     const TDesC8& /*aType*/, 
                                     const TInt aStatusRef )
    {
    // Not supported 
    Callback().SetStatusL( aStatusRef, CSmlDmAdapter::EError );
    }

// -----------------------------------------------------------------------------
// CXdmDMAdapter::ExecuteCommandL()
// -----------------------------------------------------------------------------
//
void CXdmDMAdapter::ExecuteCommandL( const TDesC8& /*aUri*/, 
                                     const TDesC8& /*aLUID*/, 
                                     RWriteStream*& /*aStream*/, 
                                     const TDesC8& /*aType*/, 
                                     const TInt aStatusRef )
    {
    // Not supported
    Callback().SetStatusL( aStatusRef, CSmlDmAdapter::EError );
    }

// -----------------------------------------------------------------------------
// CXdmDMAdapter::CopyCommandL()
// -----------------------------------------------------------------------------
//
void CXdmDMAdapter::CopyCommandL( const TDesC8& /*aTargetURI*/, 
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
// CXdmDMAdapter::StartAtomicL()
// -----------------------------------------------------------------------------
//
void CXdmDMAdapter::StartAtomicL()
    {
    // Not supported
    }

// -----------------------------------------------------------------------------
// CXdmDMAdapter::CommitAtomicL()
// -----------------------------------------------------------------------------
//
void CXdmDMAdapter::CommitAtomicL()
    {
    // Not supported
    }

// -----------------------------------------------------------------------------
// CXdmDMAdapter::RollbackAtomicL()
// -----------------------------------------------------------------------------
//
void CXdmDMAdapter::RollbackAtomicL()
    {
    // Not supported
    }

// -----------------------------------------------------------------------------
// CXdmDMAdapter::StreamingSupport()
// -----------------------------------------------------------------------------
//
TBool CXdmDMAdapter::StreamingSupport( TInt& /*aItemSize*/ )
    {
    return EFalse;
    }

// -----------------------------------------------------------------------------
// CXdmDMAdapter::StreamCommittedL()
// -----------------------------------------------------------------------------
//
void CXdmDMAdapter::StreamCommittedL()
    {
    // Not supported
    }
    
// -----------------------------------------------------------------------------
// CXdmDMAdapter::CompleteOutstandingCmdsL()
// -----------------------------------------------------------------------------
//
void CXdmDMAdapter::CompleteOutstandingCmdsL()
    {
#ifdef _DEBUG
    WriteToLog(_L8("CXdmDMAdapter::CompleteOutstandingCmdsL()") );  
#endif
    }

// -----------------------------------------------------------------------------
// CXdmDMAdapter::GetPropertyL
// -----------------------------------------------------------------------------
//
CSmlDmAdapter::TError CXdmDMAdapter::GetPropertyL( TInt aSettingsId, 
                                                   TXdmSettingsProperty aProperty, 
                                                   CBufBase& aObject )
    {
    HBufC* value = NULL;
    TInt error( KErrNone );
    TRAP( error, ( value = TXdmSettingsApi::PropertyL( aSettingsId, aProperty ) ) );
    if ( error == KErrNone )
        {
        CleanupStack::PushL( value );           // << value
        HBufC8* utfValue = ConvertLC( *value ); // << utfValue
        aObject.InsertL( 0, *utfValue );
        CleanupStack::PopAndDestroy( 2, utfValue );       // >>> value, utfValue
        return CSmlDmAdapter::EOk;
        }
    return CSmlDmAdapter::ENotFound;
    }
    

// -----------------------------------------------------------------------------
// CXdmDMAdapter::UpdatePropertyL
// -----------------------------------------------------------------------------
//
CSmlDmAdapter::TError CXdmDMAdapter::UpdatePropertyL( TInt aSettingsId, 
                                                      TXdmSettingsProperty aProperty, 
                                                      const TDesC8& aObject )
    {
    HBufC* value = ConvertLC( aObject ); // << value
    TInt error( KErrNone );
    TRAP( error, TXdmSettingsApi::UpdatePropertyL( aSettingsId, *value, aProperty )  );
    CleanupStack::PopAndDestroy( value );     // >>> value

    if ( error == KErrNone )
        {
        return CSmlDmAdapter::EOk;
        }
    return CSmlDmAdapter::ENotFound;
    }
          
 
// -----------------------------------------------------------------------------
// CXdmDMAdapter::FillNodeInfoL
// ------------------------------------------------------------------------------
//
void CXdmDMAdapter::FillNodeInfoL( MSmlDmDDFObject& aNode,
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
    
    if( aFormat != MSmlDmDDFObject::ENode )
        {
        aNode.AddDFTypeMimeTypeL( KXdmDmTextPlain );
        }
    aNode.SetDescriptionL( aDescription );
    }
    

// ----------------------------------------------------------------------------
// CXdmDMAdapter::IapIdFromURIL
// ----------------------------------------------------------------------------
//
TInt CXdmDMAdapter::IapIdFromURIL( const TDesC8& aUri )
    {
    TInt id( KErrNotFound );
    MSmlDmAdapter::TError status( MSmlDmAdapter::EError );
    CBufBase* result = CBufFlat::NewL(1);
    CleanupStack::PushL( result );  // << result
       
    TPtrC8 uri = NSmlDmURI::RemoveDotSlash( aUri ); 
    // Request the identifier 
    Callback().FetchLinkL( uri, *result, status );
    
    if( status == MSmlDmAdapter::EOk )
        {
        HBufC8* luid = Callback().GetLuidAllocL( uri ); 
        if ( luid && luid->Length() )
            {
            id = DesToInt( *luid );
            } 
        delete luid;
        luid = NULL;
        }

    CleanupStack::PopAndDestroy( result );  // >>> result
    return id;
    } 

// ----------------------------------------------------------------------------
// CXdmDMAdapter::URIFromIapIdL
// ----------------------------------------------------------------------------
//
HBufC8* CXdmDMAdapter::URIFromIapIdL( TInt aId )
    {
    CBufBase *allIds = CBufFlat::NewL(KXdmDmIdTableSize);
    CleanupStack::PushL( allIds );  // << allIds
    MSmlDmAdapter::TError status;
    // Fetch all IAP ids
    Callback().FetchLinkL( KXdmDmAP, *allIds, status );

    if ( status == MSmlDmAdapter::EOk )
        {   
        TInt index(0);
        TInt segStart(0);
        while ( index != KErrNotFound )
            { 
            // Get pointer from segStart            
            TPtrC8 allIdsPtr = allIds->Ptr(segStart).Mid(0);
            // Find the end of the segment
            index = allIdsPtr.Locate('/');
            HBufC8* uriSeg = NULL;
            // This is the last one
            if ( index == KErrNotFound )
                {
                TPtrC8 uriSeg8Ptr = allIds->Ptr(segStart);
                uriSeg = uriSeg8Ptr.AllocLC();  // << uriSeg
                }
            else
                {
                TPtrC8 uriSeg8Ptr = allIds->Ptr(segStart).Mid( 0, index );
                uriSeg = uriSeg8Ptr.AllocLC();  // << uriSeg8Ptr
                }
            // Construct the uri
            HBufC8* uri = HBufC8::NewLC( KXdmDmAP().Length() 
                                         + KXdmDmSeparator().Length() 
                                         + uriSeg->Length() ); // << uri
            TPtr8 uriPtr = uri->Des();
            uriPtr.Format( KXdmDmAP );
            uriPtr.Append( KXdmDmSeparator );
            uriPtr.Append( *uriSeg );
            // Get the luid from the uri
            HBufC8* luid = Callback().GetLuidAllocL( uriPtr );
            if ( luid && luid->Length() > 0 )
                {
                // No need to push luid to the CleanupStack
                TInt id = DesToInt( luid->Des() );
                delete luid;
                luid = NULL;
                if ( id == aId )
                    {
                    // The correct one found
                    CleanupStack::Pop();  // >>> uri
                    CleanupStack::PopAndDestroy( 2, allIds );  // >>> uriSeg, allIds
                    return uri;
                    }
                }
            // This was wrong, delete and get the next one
            CleanupStack::PopAndDestroy( 2, uriSeg ); // >>> uri, uriSeg
            segStart += index + 1;
            }        
        }
    CleanupStack::PopAndDestroy( allIds ); // >>> allIds
    return NULL;
    }  
// -----------------------------------------------------------------------------
// CXdmDMAdapter::GetSipIdL
// -----------------------------------------------------------------------------
//
TInt CXdmDMAdapter::GetSipIdL( const TDesC8& aUri )
    {

    CSmlDmAdapter::TError status = EOk;
    CBufBase* result = CBufFlat::NewL(1);
    CleanupStack::PushL( result );    // << result

    TPtrC8 uri = NSmlDmURI::RemoveDotSlash( aUri );
    // Request the sip settings identifier
    Callback().FetchLinkL( uri, *result, status);
        
    if( status == EOk )
        {
        TUint32 id( 0 );
        TPtrC8 hexIndex = uri.Right( KXdmDmHexLength );
        TLex8 lexer( hexIndex );
        lexer.Val( id, EHex );
        CleanupStack::PopAndDestroy( result );   // >>> result   
        return id;       
        }
    CleanupStack::PopAndDestroy( result );   // >>> result   
    return KErrNotFound;       
    }
// -----------------------------------------------------------------------------
// CXdmDMAdapter::FetchSipConRefL
// -----------------------------------------------------------------------------
//
CSmlDmAdapter::TError CXdmDMAdapter::FetchSipConRefL( TInt aSettingsId,
                                                      CBufBase& aObject)
    {

    CSmlDmAdapter::TError status = CSmlDmAdapter::EOk;
    TInt sipSettingsId( KErrNotFound );
    
    HBufC* value = NULL;
    TInt error( KErrNone );
    TRAP( error, ( value = TXdmSettingsApi::PropertyL( aSettingsId, EXdmPropToAppRef ) ) );
    if ( error == KErrNone )
        {
        sipSettingsId = DesToInt( *value );
        delete value;
        value = NULL;
        }
    
    if( sipSettingsId != KErrNotFound )
        {
        CBufBase* result = CBufFlat::NewL(1);
        CleanupStack::PushL(result);    // << result
        
        // Request all the sip settings identifiers 
        Callback().FetchLinkL( KXdmDmSip, *result, status);
        
        if( status == EOk )
            {
            TPtr8 uriSeg8Ptr = result->Ptr(0);
            
            HBufC8* uriSegBuffer = uriSeg8Ptr.AllocL();
            CleanupStack::PushL( uriSegBuffer );  // << uriSegBuffer
            
            TPtr8 uriSegBufferPtr = uriSegBuffer->Des();
            
            TInt numOfUriSegs = NSmlDmURI::NumOfURISegs(uriSeg8Ptr);
            
            // Check if the SIP settings id match with one of the found
            // SIP settings identifiers
            while( numOfUriSegs > 1)
                {
                uriSegBufferPtr = NSmlDmURI::RemoveLastSeg(uriSegBufferPtr);
                
                TPtrC8 hexIndex = uriSegBufferPtr.Right(KXdmDmHexLength);
                
                TUint32 id(0);
                TLex8 lexer(hexIndex);
                if( lexer.Val(id, EHex) == KErrNone )
                    {
                    if( id == sipSettingsId )
                        {
                        TPtrC8 lastUriSegSip = NSmlDmURI::LastURISeg(uriSegBufferPtr);
                        
                        // Total length of the sip settings link where one is for
                        // the separator
                        TInt sipLinkLength = KXdmDmSip().Length() + 
                                             KXdmDmSeparator().Length() +
                                             lastUriSegSip.Length();
                        HBufC8* sipLink = HBufC8::NewLC( sipLinkLength );   // <<< sipLink
                        
                        TPtr8 sipLinkPtr = sipLink->Des();
                        sipLinkPtr.Append( KXdmDmSip );
                        sipLinkPtr.Append( KXdmDmSeparator );
                        sipLinkPtr.Append( lastUriSegSip );
                        
                        aObject.InsertL( 0, sipLinkPtr );
                        
                        CleanupStack::PopAndDestroy( sipLink );   // >>> sipLink
                        break;
                        }
                    }
                
                numOfUriSegs -= 1;  
                }
            
            CleanupStack::PopAndDestroy( uriSegBuffer );    // >>> uriSegBuffer
            }
        
        CleanupStack::PopAndDestroy( result );    // >>> result
        }
    else
        {
        status = CSmlDmAdapter::ENotFound;
        }
    
    return status;
    }   

// -----------------------------------------------------------------------------
// CXdmDMAdapter::FindSettingsIdL
// -----------------------------------------------------------------------------
//      
TInt CXdmDMAdapter::FindSettingsIdL( const TDesC8& aLUID, const TDesC8& aUri )
    {
    TInt settingsId(0);
    if ( aLUID.Length() > 0 )
        {
        settingsId = DesToInt( aLUID );
        return settingsId;
        }
    else
        {
        TPtrC8 name = NSmlDmURI:: URISeg( aUri, KXdmDmLevel );
        settingsId = DesToInt( name );
        HBufC8* luid = IntToDes8LC( settingsId ); // << luid
        Callback().SetMappingL( aUri, *luid );
        CleanupStack::PopAndDestroy( luid ); // luid 
        return settingsId;
        }
    }

// -----------------------------------------------------------------------------
// CXdmDMAdapter::CheckExistingNamesLC()
// -----------------------------------------------------------------------------
// 
HBufC* CXdmDMAdapter::CheckExistingNamesLC( const TDesC8& aName )
    {    
    TBool ready( EFalse );  
    RArray<TInt> settingIds;
    CleanupClosePushL( settingIds );                // << settingIds
    CDesCArray* settingNames = TXdmSettingsApi::CollectionNamesLC( settingIds ); // << settingNames
    TInt index( 0 );
    TBuf<512> tempName;
    while ( !ready )
        {
        TBool found ( EFalse );
        TInt count = settingNames->MdcaCount();
        tempName.Copy( aName );
        if ( index > 0 )
            {
            tempName.Append( KXdmDmStartBracket );
            tempName.AppendNumFixedWidth( index, EDecimal, 2 );
            tempName.Append( KXdmDmCloseBracket );
            }
        for ( TInt i(0); i < count && !found; i++ )
            {
            if ( !settingNames->MdcaPoint(i).CompareF( tempName ) )
                {
                found = ETrue;
                index++;
                }
            }
        if ( !found )
            {
            ready = ETrue;
            }
        }
    CleanupStack::PopAndDestroy( 2 );   // >>> settingNames, settingIds
    HBufC* newName = tempName.AllocLC();    // << newName
    return newName;
    }
    
// -----------------------------------------------------------------------------
// CXdmDMAdapter::Match
// -----------------------------------------------------------------------------
//
TBool CXdmDMAdapter::Match( const TDesC8& aLeft, const TDesC8& aRight )
    {
    if ( !aLeft.Compare( aRight ) )
        {
        return ETrue;
        }
    return EFalse;
    }
    
// -----------------------------------------------------------------------------
// CXdmDMAdapter::ConvertLC
// -----------------------------------------------------------------------------
//  
HBufC8* CXdmDMAdapter::ConvertLC( const TDesC& aSource )
    {   
    HBufC8* destination = HBufC8::NewLC( aSource.Length() ); // << destination
    TPtr8 bufferPtr = destination->Des();
    CnvUtfConverter::ConvertFromUnicodeToUtf8( bufferPtr, aSource );
    return destination;
    }   
    
// -----------------------------------------------------------------------------
// CXdmDMAdapter::ConvertLC
// -----------------------------------------------------------------------------
//  
HBufC*  CXdmDMAdapter::ConvertLC( const TDesC8& aSource )
    {   
    HBufC* destination = HBufC::NewLC( aSource.Length() );  // << destination
    TPtr bufferPtr = destination->Des();
    CnvUtfConverter::ConvertToUnicodeFromUtf8( bufferPtr, aSource );
    return destination;
    }
// ----------------------------------------------------------------------------
// CXdmDMAdapter::DesToInt
// ----------------------------------------------------------------------------
TInt CXdmDMAdapter::DesToInt( const TDesC& aSource ) const
    {
    TLex16 lex( aSource );
    TInt value ( KErrNotFound );
    lex.Val( value );
    return value;
    }

// ----------------------------------------------------------------------------
// CXdmDMAdapter::DesToInt
// ----------------------------------------------------------------------------
TInt CXdmDMAdapter::DesToInt( const TDesC8& aSource ) const
    {
    TLex8 lex( aSource );
    TInt value ( KErrNotFound );
    lex.Val( value );
    return value;
    }

// ----------------------------------------------------------------------------
// CXdmDMAdapter::IntToDes8LC
// ----------------------------------------------------------------------------
HBufC8* CXdmDMAdapter::IntToDes8LC( const TInt aSource )
    {
    // 10 = max length of 32bit integer
    HBufC8* buf = HBufC8::NewLC( KXdmDmMaxIntLength ); // << buf
    TPtr8 ptrBuf = buf->Des();
    ptrBuf.Num( aSource );
    return buf;
    }

// ----------------------------------------------------
// CXdmDMAdapter::WriteToLog
// DEBUG only
// ----------------------------------------------------
//
#ifdef _DEBUG
void CXdmDMAdapter::WriteToLog( TRefByValue<const TDesC8> aFmt,... )                                 
    {
    VA_LIST list;
    VA_START( list, aFmt );
    TBuf8<KXdmDmLogBufferMaxSize> buf;
    buf.FormatList( aFmt, list );
    RFileLogger::Write( KXdmDmLogDir, KXdmDmLogFile, EFileLoggingModeAppend, buf );
    }                           
#endif                              
                                
// End of File



