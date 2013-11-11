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
* Description:   COMASuplDMAdapter is an ecom plugin that implements the interface CSmlDmAdapter to handle SUPL SyncML DM settings
*
*/




#ifndef C_COMASUPLDMADAPTER_H
#define C_COMASUPLDMADAPTER_H

#include <smldmadapter.h>
#include <centralrepository.h>
#include <s32file.h>

#include "epos_tomasuplsettingsmap.h"
#include "epos_comasupldmparam.h"


typedef RPointerArray<COMASuplDMParam> RSuplDMParams;

class COMASuplTrace;
class COMASuplDMRepository;

/**
 * COMASuplDMAdapter is an ecom plugin that implements the interface
 * CSmlDmAdapter to handle SUPL SyncML DM settings
 * @since S60 v3.1
 */
class COMASuplDMAdapter : public CSmlDmAdapter
    {

public:

    static COMASuplDMAdapter* NewL(MSmlDmCallback* aDmCallback);

    ~COMASuplDMAdapter();

    void DDFVersionL( CBufBase& aVersion );

    void DDFStructureL( MSmlDmDDFObject& aDDF );

    void UpdateLeafObjectL( const TDesC8& aURI, 
                                const TDesC8& aLUID,
                                const TDesC8& aObject,
                                const TDesC8& aType,
                                TInt aStatusRef );

    void UpdateLeafObjectL( const TDesC8& aURI,
                                const TDesC8& aLUID,
                                RWriteStream*& aStream, 
                                const TDesC8& aType,
                                TInt aStatusRef );

    void DeleteObjectL( const TDesC8& aURI, 
                                const TDesC8& aLUID,
                                TInt aStatusRef );

    void FetchLeafObjectL( const TDesC8& aURI,
                                const TDesC8& aLUID,
                                const TDesC8& aType, 
                                TInt aResultsRef,
                                TInt aStatusRef );

    void FetchLeafObjectSizeL( const TDesC8& aURI, 
                                const TDesC8& aLUID,
                                const TDesC8& aType, 
                                TInt aResultsRef,
                                TInt aStatusRef );

    void ChildURIListL( const TDesC8& aURI, 
                            const TDesC8& aLUID,
                            const CArrayFix<TSmlDmMappingInfo>& aPreviousURISegmentList,
                            TInt aResultsRef, 
                            TInt aStatusRef );

    void AddNodeObjectL( const TDesC8& aURI, 
                            const TDesC8& aParentLUID,
                            TInt aStatusRef );

    void ExecuteCommandL( const TDesC8& aURI, 
                            const TDesC8& aLUID,
                            const TDesC8& aArgument, 
                            const TDesC8& aType,
                            TInt aStatusRef );

    void ExecuteCommandL( const TDesC8& aURI, 
                            const TDesC8& aLUID,
                            RWriteStream*& aStream, 
                            const TDesC8& aType,
                            TInt aStatusRef );

    void CopyCommandL( const TDesC8& aTargetURI, 
                        const TDesC8& aTargetLUID,
                        const TDesC8& aSourceURI, 
                        const TDesC8& aSourceLUID,
                        const TDesC8& aType, 
                        TInt aStatusRef );

    void StartAtomicL();


    void CommitAtomicL();


    void RollbackAtomicL();


    TBool StreamingSupport( TInt& aItemSize );


    void StreamCommittedL();


    void CompleteOutstandingCmdsL();

private:

    typedef enum
        {
        ENone,
        EProcessing,
        EStarting,
        ECommitting,
        ECommitted,
        EAborted	
        } TSuplSmlDMAdapterState;

    COMASuplDMAdapter( MSmlDmCallback* aDmCallback );

    void ConstructL();

    void FillNodeInfoL( MSmlDmDDFObject& aDDFObject, 
                            TSmlDmAccessTypes& aAccessTypes, 
                            MSmlDmDDFObject::TScope aScope,
                            MSmlDmDDFObject::TDFFormat aFormat, 
                            MSmlDmDDFObject::TOccurence aOccurence,
                            const TDesC8& aMIMEType );

    TInt ProcessL( const TDesC8& aURI, 
                    const TDesC8& aLUID,
                    const TDesC8& aObject,
                    const TDesC8& aType,
                    TInt aStatusRef );		

    TInt GetIapIdL( TInt& aVal, const TDesC8& aToNapId );

    TInt GetIapStrL( TInt& aVal, TDes8& aToNapId );

    TInt GetLeafObjectLC( const TDesC8& aURI,
                            const TDesC8& aLUID,
                            const TDesC8& aType, 
                            TDesC8*& aValue,
                            TInt aResultsRef,
                            TInt aStatusRef );
                            
    void SetVariation();

private: // data

    COMASuplDMRepository*   iRepository;

    TOMASuplSettingsMap 	iSuplSettingsMap;

    RSuplDMParams 			iSuplParams;

    TBool       			iIsAtomic;

    TSuplSmlDMAdapterState 	iSuplSmlDMAdapterState;

    RFs 					iFs;

    RFileWriteStream 		iFileWriteStream;

    TDesC8* 				iFileName;

    COMASuplDMParam* 		iHelperStreaming;

    TInt 					iLastIndex;

    /**
     * Handle to an instance of COMASuplTrace used for tracing
     */ 
    COMASuplTrace*			iTrace;
    
    /**
     * Flag value specifying the local variation settings
     */
    TBool                   iVariantEnabled;
    
    };

#endif  // C_COMASUPLDMADAPTER_H
