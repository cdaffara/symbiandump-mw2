/*
* Copyright (c) 2005 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:   Implementation of COMASuplDMParam
*
*/




#include <commdb.h>
#include <utf.h>

#include "epos_omasuplconfigurationkeys.h"
#include "epos_comasupldmadapter.h"
#include "epos_tomasuplsettingsmap.h"
#include "epos_comasupldmrepository.h"
#include "epos_comasupltrace.h"


_LIT8( KTextType,"text/plain" );
_LIT8( KIntType,"int" );
_LIT8( KSlash, "/" );
_LIT( KTraceFileName, "epos_dmmanager.cpp" );
_LIT8( KSuplDMAdapterVersion, "1.0" );


static const TUid KSUPLVarRepositoryUid = { 0x102750A0 };
static const TUint KMaxSuplSettinglength = 1024;
static const TUint KStreamingSize = 512;


// ======== MEMBER FUNCTIONS ========

// -----------------------------------------------------------------------------
// COMASuplDMAdapter::NewL()
// COMASuplDMAdapter instantiation method.
// -----------------------------------------------------------------------------
//
COMASuplDMAdapter* COMASuplDMAdapter::NewL( MSmlDmCallback* aDmCallback )
    {
    COMASuplDMAdapter* self = new ( ELeave ) COMASuplDMAdapter( aDmCallback );
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop( self );
    return self;	
    }


// -----------------------------------------------------------------------------
// COMASuplDMAdapter::~COMASuplDMAdapter()
// Destructor
// -----------------------------------------------------------------------------
//
COMASuplDMAdapter::~COMASuplDMAdapter()
    {
    if(iTrace)
       	{
    	iTrace->Trace( _L("COMASuplDMAdapter::~COMASuplDMAdapter"), 
                                        KTraceFileName, __LINE__ );	
    	}
    if ( iRepository )
        {
        delete iRepository;    
        }
    
    if ( iTrace )
        {
        delete iTrace;
        }
    iSuplParams.ResetAndDestroy();
    iSuplParams.Close();
    }


// -----------------------------------------------------------------------------
// COMASuplDMAdapter::DDFVersionL()
// Returns the version information
// -----------------------------------------------------------------------------
//
void COMASuplDMAdapter::DDFVersionL( CBufBase& aVersion )
    {
    if( !iVariantEnabled )
        {
        return;
        }
    
    iTrace->Trace( _L("COMASuplDMAdapter::DDFVersionL"), 
                                        KTraceFileName, __LINE__ );
    aVersion.InsertL( 0, KSuplDMAdapterVersion );
    }


// -----------------------------------------------------------------------------
// COMASuplDMAdapter::DDFStructureL()
// Returns the tree structure of the SUPL DM settings
// -----------------------------------------------------------------------------
//
void COMASuplDMAdapter::DDFStructureL( MSmlDmDDFObject& aDDF )
    {
    if( !iVariantEnabled )
        {
        return;
        }
    
    iTrace->Trace( _L("COMASuplDMAdapter::DDFStructureL - Start"), 
                                        KTraceFileName, __LINE__ );	
    TSmlDmAccessTypes accessTypes;

    // create root node 
    MSmlDmDDFObject& root = aDDF.AddChildObjectL( KRootNode );
    // add  settings
    accessTypes.Reset();
    accessTypes.SetGet();
    FillNodeInfoL( root, accessTypes, MSmlDmDDFObject::EPermanent, 
                   MSmlDmDDFObject::ENode, MSmlDmDDFObject::EOne, KTextType );

    // create nodes to store settings and add to root node
    MSmlDmDDFObject& slpaddr = root.AddChildObjectL(KSlpAddr);
    // add  settings
    accessTypes.Reset();
    accessTypes.SetGet();	
    accessTypes.SetReplace();
    FillNodeInfoL( slpaddr, accessTypes, MSmlDmDDFObject::EDynamic, 
                MSmlDmDDFObject::EChr, MSmlDmDDFObject::EZeroOrOne, KTextType );

    MSmlDmDDFObject& napid = root.AddChildObjectL( KIapIdName );
    // add  settings
    accessTypes.Reset();
    accessTypes.SetGet();	
    accessTypes.SetReplace();
    FillNodeInfoL( napid, accessTypes, MSmlDmDDFObject::EDynamic, 
                MSmlDmDDFObject::EChr, MSmlDmDDFObject::EZeroOrOne, KTextType );
    
    MSmlDmDDFObject& nslpname = root.AddChildObjectL( KName );
    // add  settings
    accessTypes.Reset();
    accessTypes.SetGet();   
    accessTypes.SetReplace();  
    FillNodeInfoL( nslpname, accessTypes, MSmlDmDDFObject::EDynamic, 
            MSmlDmDDFObject::EChr, MSmlDmDDFObject::EZeroOrOne, KTextType );

    MSmlDmDDFObject& nproviderid = root.AddChildObjectL( KProviderId );
    // add  settings
    accessTypes.Reset();
    accessTypes.SetGet();   
    accessTypes.SetReplace(); 
    FillNodeInfoL( nproviderid, accessTypes, MSmlDmDDFObject::EDynamic, 
            MSmlDmDDFObject::EChr, MSmlDmDDFObject::EZeroOrOne, KTextType );
    
    MSmlDmDDFObject& ntoconref = root.AddChildObjectL( KToConRef );
    // add  settings
    accessTypes.Reset();
    accessTypes.SetGet();   
    accessTypes.SetReplace();  
    FillNodeInfoL( ntoconref, accessTypes, MSmlDmDDFObject::EDynamic, 
            MSmlDmDDFObject::EChr, MSmlDmDDFObject::EZeroOrOne, KTextType );
    
    MSmlDmDDFObject& nconref = root.AddChildObjectL( KConRef );
    // add  settings
    accessTypes.Reset();
    accessTypes.SetGet();   
    accessTypes.SetReplace();  
    FillNodeInfoL( nconref, accessTypes, MSmlDmDDFObject::EDynamic, 
            MSmlDmDDFObject::EChr, MSmlDmDDFObject::EZeroOrOne, KTextType );

    MSmlDmDDFObject& naddrtype = root.AddChildObjectL( KAddrType );
    // add  settings
    accessTypes.Reset();
    accessTypes.SetGet();   
    accessTypes.SetReplace();
    FillNodeInfoL( naddrtype, accessTypes, MSmlDmDDFObject::EDynamic, 
            MSmlDmDDFObject::EChr, MSmlDmDDFObject::EZeroOrOne, KTextType );
       
    iTrace->Trace( _L("COMASuplDMAdapter::DDFStructureL - End"), 
                                                    KTraceFileName, __LINE__ );	
    }


// -----------------------------------------------------------------------------
// COMASuplDMAdapter::UpdateLeafObjectL()
// Updates the SUPL DM settings
// -----------------------------------------------------------------------------
//
void COMASuplDMAdapter::UpdateLeafObjectL(  const TDesC8& aURI, 
                                                const TDesC8& aLUID,
                                                const TDesC8& aObject,
                                                const TDesC8& aType,
                                                TInt aStatusRef )
    {
    iTrace->Trace( _L("COMASuplDMAdapter::UpdateLeafObjectL - Start"), 
                                                    KTraceFileName, __LINE__ );
    if( !iIsAtomic )  // not atomic
        {
        iTrace->Trace( _L("COMASuplDMAdapter::UpdateLeafObjectL Non Atomic"), 
                                                    KTraceFileName, __LINE__ );
        ProcessL( aURI, aLUID, aObject, aType, aStatusRef );

        iTrace->Trace( _L("COMASuplDMAdapter::UpdateLeafObjectL Non Atomic End"), 
                                                    KTraceFileName, __LINE__ );		
        return;
        }
    else            // atomic
        {
        iTrace->Trace( _L("COMASuplDMAdapter::UpdateLeafObjectL Atomic"), 
                                                    KTraceFileName, __LINE__ );

        COMASuplDMParam* suplDMParam = COMASuplDMParam::NewL( aURI, aLUID, 
                    &aObject, aType, aStatusRef, 0, COMASuplDMParam::EReplace );
        CleanupStack::PushL( suplDMParam );
        iSuplParams.AppendL( suplDMParam );
        CleanupStack::Pop();		

        // no callback --- as it will be handled in CompleteOutstandingCmdsL
        iSuplSmlDMAdapterState = EProcessing;
        iTrace->Trace( _L("COMASuplDMAdapter::UpdateLeafObjectL Atomic End"), 
                                                    KTraceFileName, __LINE__ );		
        } 
    }


// -----------------------------------------------------------------------------
// COMASuplDMAdapter::UpdateLeafObjectL()
// Updates the SUPL DM settings - Stream support
// -----------------------------------------------------------------------------
//
void COMASuplDMAdapter::UpdateLeafObjectL( const TDesC8& aURI, const TDesC8& aLUID, 
                                                    RWriteStream*& aStream, 
                                                    const TDesC8& aType, 
                                                    TInt aStatusRef )
    {
    iTrace->Trace( _L("COMASuplDMAdapter::UpdateLeafObjectL(Stream) Start"), 
                                                    KTraceFileName, __LINE__ );
    const TDesC* fileName = NULL;
    TInt rc = TOMASuplSettingsMap::FileName( aURI, fileName );
    if( rc != KErrNone )
        {
        Callback().SetStatusL( aStatusRef, ENotFound );
        return;
        }

    rc = iFs.Connect();
    if(rc != KErrNone)
    	{
    	Callback().SetStatusL( aStatusRef, EError );
      return;
    	}
   	rc = iFileWriteStream.Replace( iFs, *fileName, EFileWrite );
    if( rc != KErrNone )
        {
        Callback().SetStatusL( aStatusRef, EError );
        return;
        }

    iHelperStreaming = COMASuplDMParam::NewL( aURI, aLUID, NULL, aType, 
                                    aStatusRef, 0, COMASuplDMParam::EReplace );
    aStream = &iFileWriteStream;

    iTrace->Trace( _L("COMASuplDMAdapter::UpdateLeafObjectL End"), 
                                                    KTraceFileName, __LINE__ );
    }


// -----------------------------------------------------------------------------
// COMASuplDMAdapter::DeleteObjectL()
// 
// -----------------------------------------------------------------------------
//
void COMASuplDMAdapter::DeleteObjectL( const TDesC8& /*aURI*/, 
                                        const TDesC8& /*aLUID*/,
                                        TInt aStatusRef )
    {
    iTrace->Trace( _L("COMASuplDMAdapter::DeleteObjectL"), 
                                                    KTraceFileName, __LINE__ );
    Callback().SetStatusL( aStatusRef, EError );
    }


// -----------------------------------------------------------------------------
// COMASuplDMAdapter::FetchLeafObjectL()
// Retrieves the data in the leaf object
// -----------------------------------------------------------------------------
//
void COMASuplDMAdapter::FetchLeafObjectL( const TDesC8& aURI,
                                            const TDesC8& aLUID,
                                            const TDesC8& aType, 
                                            TInt aResultsRef,
                                            TInt aStatusRef )
    {
    iTrace->Trace( _L("COMASuplDMAdapter::FetchLeafObjectL"), KTraceFileName, __LINE__ );
    TDesC8* value = NULL;

    TInt rc = GetLeafObjectLC( aURI, aLUID, aType, value, aResultsRef, aStatusRef );
    if( rc != KErrNone )
        {
        Callback().SetStatusL( aStatusRef, EError );
        return;
        }		

    if( value->Length() <= KStreamingSize )
        {
        CBufBase* buf = CBufFlat::NewL( value->Length() );
        CleanupStack::PushL( buf );
        buf->InsertL( 0, *value );
        Callback().SetResultsL( aResultsRef, *buf, KTextType );	
        CleanupStack::PopAndDestroy( 2 );
        }
    else
        {
        RFileWriteStream wstream;
        const TDesC* fileName = NULL;

        TInt retC = TOMASuplSettingsMap::FileName( aURI, fileName );
        if( retC != KErrNone )
            {
            CleanupStack::PopAndDestroy();
            Callback().SetStatusL( aStatusRef, EError );
            return;
            }

        TInt rc = wstream.Replace( iFs, *fileName, EFileRead|EFileWrite );
        if(rc != KErrNone)	
            {
            CleanupStack::PopAndDestroy();
            Callback().SetStatusL( aStatusRef, EError );
            return;
            }

        wstream.WriteL( *value );
        wstream.Close();

        RFileReadStream rstream;
        rc = rstream.Open( iFs, *fileName, EFileRead|EFileWrite );
        if( rc != KErrNone )
            {
            CleanupStack::PopAndDestroy();		
            // handle error
            Callback().SetStatusL( aStatusRef, EError );
            return;
            }

        RReadStream* stream = &rstream;
        Callback().SetResultsL( aResultsRef, stream, KTextType );
        rstream.Close();	
        CleanupStack::PopAndDestroy();
        } // else

    Callback().SetStatusL( aStatusRef, EOk );

    iTrace->Trace( _L("COMASuplDMAdapter::FetchLeafObjectL End"), 
                                                KTraceFileName, __LINE__ );
    }


// -----------------------------------------------------------------------------
// COMASuplDMAdapter::FetchLeafObjectSizeL()
// Retrieves the size of the data in the leaf object
// -----------------------------------------------------------------------------
//
void COMASuplDMAdapter::FetchLeafObjectSizeL( const TDesC8& aURI, 
                                                const TDesC8& aLUID,
                                                const TDesC8& aType, 
                                                TInt aResultsRef,
                                                TInt aStatusRef )
    {
    iTrace->Trace( _L("COMASuplDMAdapter::FetchLeafObjectSizeL"), 
                                                KTraceFileName, __LINE__ );
    TDesC8* value = NULL;

    TInt rc = GetLeafObjectLC( aURI, aLUID, aType, value, aResultsRef, aStatusRef );
    if( rc != KErrNone )
        {
        CleanupStack::PopAndDestroy();
        Callback().SetStatusL( aStatusRef, EError );
        return;
        }

    TBuf8<KSizeOfSettingId> buf;
    CBufBase* bufBase = CBufFlat::NewL( KSizeOfSettingId );
    CleanupStack::PushL( bufBase );
    buf.AppendNum( value->Length() );
    bufBase->InsertL( 0, *value );

    Callback().SetResultsL( aResultsRef, *bufBase, KIntType );

    CleanupStack::PopAndDestroy( 2 );
    Callback().SetStatusL( aStatusRef, EOk );

    iTrace->Trace( _L("COMASuplDMAdapter::FetchLeafObjectSizeL End"), 
                                                KTraceFileName, __LINE__ );
    }


// -----------------------------------------------------------------------------
// COMASuplDMAdapter::ChildURIListL()
// Retrieves the child URI list of the root node
// -----------------------------------------------------------------------------
//
void COMASuplDMAdapter::ChildURIListL( const TDesC8& aURI, 
                const TDesC8& aLUID,
                const CArrayFix<TSmlDmMappingInfo>& /*aPreviousURISegmentList*/,
                TInt aResultsRef, 
                TInt aStatusRef )
    {
    iTrace->Trace( _L("COMASuplDMAdapter::ChildURIListL"), KTraceFileName, __LINE__ );				

    RChildList list;
    RArray<TPtrC8> children;

    TInt rc = TOMASuplSettingsMap::Get( aURI, list, children );
    if( rc != KErrNone )
        {
        list.Close();
        children.Close();		
        Callback().SetStatusL( aStatusRef, EError );
        return;
        }

    for( TInt i = 0; i < list.Count(); i++ )
        {
        TSettingMap& info = list[i];
        Callback().SetMappingL( info.iSettingName, aLUID );
        }

    list.Close();

    // set child list
    TInt resultSize = 0;
    for( TInt i = 0; i < children.Count(); i++ )
        {
        resultSize += children[i].Length();
        }

    if( resultSize && resultSize <= KStreamingSize )
        {
        CBufBase* currList = 
        CBufFlat::NewL( resultSize + children.Count() * sizeof(TChar) );
        CleanupStack::PushL( currList );
        for( TInt i = 0; i < children.Count(); i++ )
            {
            TInt startFromIndex = currList->Size();
            currList->InsertL(startFromIndex, children[i]);
            if( i != children.Count() - 1 )
                {
                startFromIndex = currList->Size();				
                currList->InsertL( startFromIndex, KSlash );
                }
            }	

        children.Close();		
        Callback().SetResultsL( aResultsRef, *currList, KNullDesC8 );
        CleanupStack::PopAndDestroy();
        }	
    else if( resultSize && resultSize > KStreamingSize )
        {
        RFileWriteStream wstream;

        TInt rc = wstream.Replace( iFs, KSuplFile, EFileRead|EFileWrite );
        if(rc != KErrNone)	
            {
            children.Close();
            Callback().SetStatusL( aStatusRef, EError );
            return;
            }

        for( TInt i = 0; i < children.Count(); i++ )
            {
            wstream.WriteL( children[i] );
            if( i != children.Count() - 1 )
                {
                wstream.WriteL( KSlash );
                }
            }	

        // writing to stream is over
        children.Close();		

        wstream.Close();
        RFileReadStream rstream;

        rc = rstream.Open( iFs, KSuplFile, EFileRead|EFileWrite );
        if( rc != KErrNone )
            {
            Callback().SetStatusL( aStatusRef, EError );
            return;
            }

        RReadStream* stream = &rstream;
        Callback().SetResultsL( aResultsRef, stream, KTextType );
        rstream.Close();	
        }
    else
        {
        children.Close();		
        Callback().SetStatusL( aStatusRef, EError );
        return;
        }

    Callback().SetStatusL( aStatusRef, EOk );
    iTrace->Trace( _L("COMASuplDMAdapter::ChildURIListL End"), KTraceFileName, __LINE__ );
    }


// -----------------------------------------------------------------------------
// COMASuplDMAdapter::AddNodeObjectL()
//
// -----------------------------------------------------------------------------
//
void COMASuplDMAdapter::AddNodeObjectL( const TDesC8& /*aURI*/, 
                                            const TDesC8& /*aParentLUID*/,
                                            TInt aStatusRef )
    {
    iTrace->Trace( _L("COMASuplDMAdapter::AddNodeObjectL"), KTraceFileName, __LINE__ );				
    Callback().SetStatusL( aStatusRef, EError );
    }


// -----------------------------------------------------------------------------
// COMASuplDMAdapter::ExecuteCommandL()
//
// -----------------------------------------------------------------------------
//
void COMASuplDMAdapter::ExecuteCommandL( const TDesC8& /*aURI*/, 
                                            const TDesC8& /*aLUID*/,
                                            const TDesC8& /*aArgument*/, 
                                            const TDesC8& /*aType*/,
                                            TInt aStatusRef )
    {
    iTrace->Trace( _L("COMASuplDMAdapter::ExecuteCommandL"), KTraceFileName, __LINE__ );				
    Callback().SetStatusL( aStatusRef, EError );
    }


// -----------------------------------------------------------------------------
// COMASuplDMAdapter::ExecuteCommandL()
//
// -----------------------------------------------------------------------------
//
void COMASuplDMAdapter::ExecuteCommandL( const TDesC8& /*aURI*/, 
                                            const TDesC8& /*aLUID*/,
                                            RWriteStream*& /*aStream*/, 
                                            const TDesC8& /*aType*/,
                                            TInt aStatusRef )
    {
    iTrace->Trace( _L("COMASuplDMAdapter::ExecuteCommandL"), KTraceFileName, __LINE__ );				
    Callback().SetStatusL( aStatusRef, EError );
    }


// -----------------------------------------------------------------------------
// COMASuplDMAdapter::CopyCommandL()
//
// -----------------------------------------------------------------------------
//
void COMASuplDMAdapter::CopyCommandL( const TDesC8& /*aTargetURI*/, 
                                        const TDesC8& /*aTargetLUID*/,
                                        const TDesC8& /*aSourceURI*/, 
                                        const TDesC8& /*aSourceLUID*/,
                                        const TDesC8& /*aType*/, 
                                        TInt aStatusRef )
    {
    iTrace->Trace( _L("COMASuplDMAdapter::CopyCommandL"), KTraceFileName, __LINE__ );
    Callback().SetStatusL( aStatusRef, EError );
    }


// -----------------------------------------------------------------------------
// COMASuplDMAdapter::StartAtomicL()
// Indicates the start of an atomic command
// -----------------------------------------------------------------------------
//
void COMASuplDMAdapter::StartAtomicL()
    {
    iTrace->Trace( _L("COMASuplDMAdapter::StartAtomicL"), KTraceFileName, __LINE__ );
    iIsAtomic = ETrue;
    }


// -----------------------------------------------------------------------------
// COMASuplDMAdapter::CommitAtomicL()
// Indicates the successful completion of an atomic command
// -----------------------------------------------------------------------------
//
void COMASuplDMAdapter::CommitAtomicL()
    {
    iTrace->Trace( _L("COMASuplDMAdapter::CommitAtomicL"), KTraceFileName, __LINE__ );
    if( iIsAtomic )
        {
        if( iSuplSmlDMAdapterState == EStarting )
            {
            iSuplSmlDMAdapterState = ECommitting;
            iRepository->CommitTransactionL();
            iSuplSmlDMAdapterState = ECommitted;
            }
        iIsAtomic = EFalse;
        }
    }


// -----------------------------------------------------------------------------
// COMASuplDMAdapter::RollbackAtomicL()
// Rolls back changes issued between StartAtomicL and CommitAtomicL
// -----------------------------------------------------------------------------
//
void COMASuplDMAdapter::RollbackAtomicL()
    {
    iTrace->Trace( _L("COMASuplDMAdapter::RollbackAtomicL"), KTraceFileName, __LINE__ );
    if( iIsAtomic && iSuplSmlDMAdapterState == EStarting )
        {
        iRepository->RollbackTransactionL();
        iIsAtomic = EFalse;	
        }
    }


// -----------------------------------------------------------------------------
// COMASuplDMAdapter::StreamingSupport()
// Returns ETrue if streaming is supported
// -----------------------------------------------------------------------------
//
TBool COMASuplDMAdapter::StreamingSupport( TInt& aItemSize )
    {
    if ( !iVariantEnabled )
        {
        return EFalse;
        }
    
    iTrace->Trace( _L("COMASuplDMAdapter::StreamingSupport Start"), KTraceFileName, __LINE__ );
    TBool retCD = ETrue;
    TInt rc = iFs.Connect();
    rc != KErrNone ? retCD = EFalse : aItemSize = KStreamingSize;

    iTrace->Trace( _L("COMASuplDMAdapter::StreamingSupport End"), KTraceFileName, __LINE__ );
    return retCD;
    }


// -----------------------------------------------------------------------------
// COMASuplDMAdapter::StreamCommittedL()
//
// -----------------------------------------------------------------------------
//
void COMASuplDMAdapter::StreamCommittedL()
    {
    iTrace->Trace( _L("COMASuplDMAdapter::StreamCommittedL"), KTraceFileName, __LINE__ );				
    TBuf8<KMaxSuplSettinglength> result;
    RFileReadStream rstream;
    HBufC8* URI = NULL;
    HBufC8* LUID = NULL;
    HBufC8* object = NULL;	
    HBufC8* type = NULL;
    TInt 	statusRef;
    TInt	resultRef;
    COMASuplDMParam::TCommand command;
    const TDesC* fileName = NULL;

    iHelperStreaming->Get( URI, LUID, object, type, statusRef, resultRef, command );

    iFileWriteStream.Close();

    TInt rc = TOMASuplSettingsMap::FileName( *URI, fileName );
    if( rc != KErrNone )
        {
        Callback().SetStatusL( statusRef, ENotFound );
        return;
        }

    rc = rstream.Open( iFs, *fileName, EFileRead );
    if( rc != KErrNone )
        {
        Callback().SetStatusL( statusRef, EError );
        return;
        }

    rstream.ReadL( result );
    rstream.Close();

    if( !iIsAtomic )
        {			
        ProcessL( *URI, *LUID, result, *type, statusRef );
        } // if not atomic
    else
        {
        object = result.AllocL();
        iHelperStreaming->Set( URI, LUID, object, type, statusRef, resultRef, command );
        iSuplParams.AppendL( iHelperStreaming );
        iHelperStreaming = NULL;
        iSuplSmlDMAdapterState = EProcessing;
        } // atomic
    }


// -----------------------------------------------------------------------------
// COMASuplDMAdapter::CompleteOutstandingCmdsL()
// 
// -----------------------------------------------------------------------------
//
void COMASuplDMAdapter::CompleteOutstandingCmdsL()
    {
    iTrace->Trace( _L("COMASuplDMAdapter::CompleteOutstandingCmdsL Start"), KTraceFileName, __LINE__ );
    if( iIsAtomic )
        {
        iTrace->Trace( _L("COMASuplDMAdapter::CompleteOutstandingCmdsL Atomic Start"), KTraceFileName, __LINE__ );					
        TInt parameterCount = iSuplParams.Count();

        if( parameterCount == 0 )
            {
            User::Leave( KErrNotFound ); 
            }

        iRepository->StartTransactionL();
        for( TInt loop = 0; loop < parameterCount; loop++ )
            {
            HBufC8* URI = NULL;
            HBufC8* LUID = NULL;
            HBufC8* object = NULL;	
            HBufC8* type = NULL;
            TInt 	statusRef;
            TInt	resultRef;
            COMASuplDMParam::TCommand command;

            iSuplParams[loop]->Get( URI, LUID, object, type, statusRef, resultRef, command );

            if( command == COMASuplDMParam::EReplace )
                {
                ProcessL( *URI, *LUID, *object, *type, statusRef );
                }

            }

        iTrace->Trace( _L("COMASuplDMAdapter::CompleteOutstandingCmdsL Atomic End"), KTraceFileName, __LINE__ );
        } 
    iTrace->Trace( _L("COMASuplDMAdapter::CompleteOutstandingCmdsL End"), KTraceFileName, __LINE__ );
    }


// -----------------------------------------------------------------------------
// COMASuplDMAdapter::COMASuplDMAdapter()
// C++ Constructor
// -----------------------------------------------------------------------------
//
COMASuplDMAdapter::COMASuplDMAdapter( MSmlDmCallback* aDmCallback ) : 
                                        CSmlDmAdapter( aDmCallback )
    {
    iRepository = NULL;	
    iSuplSmlDMAdapterState = ENone;		
    iIsAtomic = EFalse;
    iLastIndex = 0;
    }


// -----------------------------------------------------------------------------
// COMASuplDMAdapter::ConstructL()
// Symbian 2nd phase constructor
// -----------------------------------------------------------------------------
//
void COMASuplDMAdapter::ConstructL()
    {
    SetVariation();
    
    if( !iVariantEnabled )
        {
        return;
        }
     
    iTrace = COMASuplTrace::NewL();
    
    iTrace->Trace( _L("COMASuplDMAdapter::ConstructL"), 
                                        KTraceFileName, __LINE__ );	
    iRepository = COMASuplDMRepository::NewL();
    }
    
    
// -----------------------------------------------------------------------------
// COMASuplDMAdapter::FillNodeInfoL()
// Updates node information
// -----------------------------------------------------------------------------
//
void COMASuplDMAdapter::FillNodeInfoL(MSmlDmDDFObject& aDDFObject, 
                                        TSmlDmAccessTypes& aAccessTypes, 
                                        MSmlDmDDFObject::TScope aScope,
                                        MSmlDmDDFObject::TDFFormat aFormat, 
                                        MSmlDmDDFObject::TOccurence aOccurence,
                                        const TDesC8& aMIMEType)
    {
    iTrace->Trace( _L("COMASuplDMAdapter::FillNodeInfoL Start"), KTraceFileName, __LINE__ );
    aDDFObject.SetAccessTypesL( aAccessTypes );
    aDDFObject.SetScopeL( aScope );
    aDDFObject.SetOccurenceL( aOccurence );
    aDDFObject.SetDFFormatL( aFormat );
    if( aFormat != MSmlDmDDFObject::ENode )
        {
        aDDFObject.AddDFTypeMimeTypeL( aMIMEType );
        }
    iTrace->Trace( _L("COMASuplDMAdapter::FillNodeInfoL End"), KTraceFileName, __LINE__ );
    }


// -----------------------------------------------------------------------------
// COMASuplDMAdapter::ProcessL()
// 
// -----------------------------------------------------------------------------
//
TInt COMASuplDMAdapter::ProcessL( const TDesC8& aURI, 
                                    const TDesC8& /*aLUID*/,
                                    const TDesC8& aObject,
                                    const TDesC8& /*aType*/,
                                    TInt aStatusRef )
    {
    iTrace->Trace( _L("COMASuplDMAdapter::ProcessL"), KTraceFileName, __LINE__ );

    TOMASuplSettingsMap::TSuplSettingType suplSettingType = TOMASuplSettingsMap::ENone ;

    TInt err = TOMASuplSettingsMap::Map( aURI, suplSettingType );
    if( err != KErrNone )
        {
        iTrace->Trace( _L("COMASuplDMAdapter::UpdateLeafObjectL Uri Not Found"), 
                                                KTraceFileName, __LINE__ );
        Callback().SetStatusL( aStatusRef, ENotFound );
        return err;
        }

    HBufC* setting = NULL;
    TRAP( err, setting = HBufC::NewL( aObject.Size() ) );
    if ( err != KErrNone )
        {
        iTrace->Trace( _L("Out of Memory"), KTraceFileName, __LINE__ );
        Callback().SetStatusL( aStatusRef, ENoMemory );
        return err;
        }
    CleanupStack::PushL( setting );
    
    // Convert to unicode
    TPtr settingPtr = setting->Des();
    err = CnvUtfConverter::ConvertToUnicodeFromUtf8( settingPtr, aObject );
    if ( err != KErrNone )
        {
        iTrace->Trace( _L("Data Conversion Error from Utf8 to Unicode"), 
                                        KTraceFileName, __LINE__ );
        Callback().SetStatusL( aStatusRef, EError );
        CleanupStack::PopAndDestroy(1); //setting
        return err;
        }

   
    TRAP( err, iRepository->SaveL(settingPtr,suplSettingType ,iIsAtomic ) );
    if ( err != KErrNone )
        {
        iTrace->Trace( _L("Error saving SUPL settings"), 
                KTraceFileName, __LINE__ );
        Callback().SetStatusL( aStatusRef, EError );
        CleanupStack::PopAndDestroy(1); //setting
        return err;
        }
    else
        {
        Callback().SetStatusL( aStatusRef, EOk );
        }
        
    CleanupStack::PopAndDestroy(1); //setting
    return err;
    }


// -----------------------------------------------------------------------------
// COMASuplDMAdapter::GetLeafObjectLC()
// 
// -----------------------------------------------------------------------------
//
TInt COMASuplDMAdapter::GetLeafObjectLC( const TDesC8& aURI,
                                            const TDesC8& /*aLUID*/,
                                            const TDesC8& /*aType*/, 
                                            TDesC8*& aValue,
                                            TInt /*aResultsRef*/,
                                            TInt /*aStatusRef*/ )
    {
    iTrace->Trace( _L("COMASuplDMAdapter::GetLeafObjectLC"), KTraceFileName, __LINE__ );
    TOMASuplSettingsMap::TSuplSettingType suplSettingType = TOMASuplSettingsMap::ENone;

    aValue = NULL;

    TInt rc = TOMASuplSettingsMap::Map( aURI, suplSettingType );
    if(rc == KErrNone)
        {
        switch(suplSettingType)
            {
            case TOMASuplSettingsMap::EName :
                iRepository->GetHSLPAddressLC( aValue );
                break;

            case TOMASuplSettingsMap::EPrefConRef :
                iRepository->GetIAPNameLC( aValue );
                break;
            case TOMASuplSettingsMap::ESlp :
            case TOMASuplSettingsMap::EProviderId :
            case TOMASuplSettingsMap::EToConRef:
            case TOMASuplSettingsMap::EConRef:
            case TOMASuplSettingsMap::EAddrType:
                iRepository->GetExtnParamsLC(aValue,suplSettingType);                
                break;
            default :
                return KErrNotFound;
            } // switch
        }
        else 
            {
            return rc;
            }

    return KErrNone;
    }


// -----------------------------------------------------------------------------
// COMASuplProvHandler::SetVariation
// Initializes the local variation flag
// -----------------------------------------------------------------------------
//
void COMASuplDMAdapter::SetVariation()
    {
    iVariantEnabled = ETrue;
    CRepository *repository = NULL;
    TRAPD( err, repository = CRepository::NewL( KSUPLVarRepositoryUid ) );
    if ( KErrNone != err )
        {
        return;        
        }
    TInt val;
    TInt rc = repository->Get( KOMASuplSyncMLDMProvHandlerVariant, val );
    if ( KErrNone == rc && val == 0 )
        {
        iVariantEnabled = EFalse;
        }
    delete repository;
    }
