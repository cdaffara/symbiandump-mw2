/*
* Copyright (c) 2002 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  MPXUser testing implementation (mpxuser.h)
*
*/


#include <s32mem.h>

#include <ctsydomainpskeys.h>
#include <data_caging_path_literals.hrh>

#include "commontestclass.h"
#include "mpxuser.h"

#include <coeaui.h>
#include "EdwinTestControl.h"
#include <EIKENV.H>

// Begin MPXUser testing implementation (mpxuser.h)=======================================
// -----------------------------------------------------------------------------
// CCommonTestClass::CompareUids()
// Returns: Symbian OS errors.
// -----------------------------------------------------------------------------
TInt CCommonTestClass::CompareUids()
    {
	FTRACE(FPrint(_L("CCommonTestClass::CompareUids testing MPXUser::CompareUids() begin")));
    iLog->Log(_L("CCommonTestClass::CompareUids testing MPXUser::CompareUids() begin"));
    TInt err = KErrNone;
    const TUid KMPXCollectionServerUid3={0x101FFC31};  
    const TUid KPodcastCollection = {0x101FFB31};
        
    TInt notequal = MPXUser::CompareUids(KMPXCollectionServerUid3, KPodcastCollection);
    TInt equal = MPXUser::CompareUids(KMPXCollectionServerUid3, KMPXCollectionServerUid3);
    
    if ( !notequal || equal )
        {
        err = KErrUnexpectedValue;
        }
    
	FTRACE(FPrint(_L("CCommonTestClass::CompareUids testing MPXUser::CompareUids() end err=%d"), err));
    iLog->Log(_L("CCommonTestClass::CompareUids testing MPXUser::CompareUids() end err=%d"), err);
	return err;
    }

// -----------------------------------------------------------------------------
// CCommonTestClass::IsCallOngoing()
// Returns: Symbian OS errors.
// -----------------------------------------------------------------------------
TInt CCommonTestClass::IsCallOngoing()
    {
	FTRACE(FPrint(_L("CCommonTestClass::IsCallOngoing testing MPXUser::IsCallOngoing() begin")));
    iLog->Log(_L("CCommonTestClass::IsCallOngoing testing MPXUser::IsCallOngoing() begin"));
    TInt err = KErrNone;

    MPXUser::IsCallOngoing(EPSCTsyCallTypeH324Multimedia);
    
	FTRACE(FPrint(_L("CCommonTestClass::IsCallOngoing testing MPXUser::IsCallOngoing() end err=%d"), err));
    iLog->Log(_L("CCommonTestClass::IsCallOngoing testing MPXUser::IsCallOngoing() end err=%d"), err);
	return err;
    }


// -----------------------------------------------------------------------------
// CCommonTestClass::CompleteWithDllPath()
// Returns: Symbian OS errors.
// -----------------------------------------------------------------------------
TInt CCommonTestClass::CompleteWithDllPath()
    {
	FTRACE(FPrint(_L("CCommonTestClass::CompleteWithDllPath testing MPXUser::CompleteWithDllPath() begin")));
    iLog->Log(_L("CCommonTestClass::CompleteWithDllPath testing MPXUser::CompleteWithDllPath() begin"));
    TInt err = KErrNone;

    TParse parse;
    _LIT(KMPXCollectionDbResourceFile,  "mpxcollectiondbres.rsc");
    parse.Set( KMPXCollectionDbResourceFile, &KDC_APP_RESOURCE_DIR, NULL );
    TFileName resFile(parse.FullName());
    User::LeaveIfError(MPXUser::CompleteWithDllPath(resFile));
    
	FTRACE(FPrint(_L("CCommonTestClass::CompleteWithDllPath testing MPXUser::CompleteWithDllPath() end err=%d"), err));
    iLog->Log(_L("CCommonTestClass::CompleteWithDllPath testing MPXUser::CompleteWithDllPath() end err=%d"), err);
	return err;
    }

// -----------------------------------------------------------------------------
// CCommonTestClass::Alloc8L()
// Returns: Symbian OS errors.
// -----------------------------------------------------------------------------
TInt CCommonTestClass::Alloc8L()
    {
	FTRACE(FPrint(_L("CCommonTestClass::Alloc8L testing MPXUser::Alloc8L() begin")));
    iLog->Log(_L("CCommonTestClass::Alloc8L testing MPXUser::Alloc8L() begin"));
    TInt err = KErrNone;

    TParse parse;
    _LIT(descriptor,  "memory to be allocated");
    HBufC8* mem = MPXUser::Alloc8L(descriptor);
    delete mem;
	FTRACE(FPrint(_L("CCommonTestClass::Alloc8L testing MPXUser::Alloc8L() end err=%d"), err));
    iLog->Log(_L("CCommonTestClass::Alloc8L testing MPXUser::Alloc8L() end err=%d"), err);
	return err;
    }

// -----------------------------------------------------------------------------
// CCommonTestClass::AllocL()
// Returns: Symbian OS errors.
// -----------------------------------------------------------------------------
TInt CCommonTestClass::AllocL()
    {
	FTRACE(FPrint(_L("CCommonTestClass::AllocL testing MPXUser::AllocL() begin")));
    iLog->Log(_L("CCommonTestClass::AllocL testing MPXUser::AllocL() begin"));
    TInt err = KErrNone;

    TParse parse;
    _LIT8(descriptor,  "memory to be allocated");
    HBufC* mem = MPXUser::AllocL(descriptor);
    delete mem;
	FTRACE(FPrint(_L("CCommonTestClass::AllocL testing MPXUser::AllocL() end err=%d"), err));
    iLog->Log(_L("CCommonTestClass::AllocL testing MPXUser::AllocL() end err=%d"), err);
	return err;
    }


// -----------------------------------------------------------------------------
// CCommonTestClass::Ptr()
// Returns: Symbian OS errors.
// -----------------------------------------------------------------------------
TInt CCommonTestClass::Ptr()
    {
	FTRACE(FPrint(_L("CCommonTestClass::Ptr testing MPXUser::Ptr(const TDesC& aDes) begin")));
    iLog->Log(_L("CCommonTestClass::Ptr testing MPXUser::Ptr(const TDesC& aDes) begin"));
    TInt err = KErrNone;

    TBufC8<40> descriptor8(_L8("xxx"));
    TPtrC mem = MPXUser::Ptr(descriptor8);
	FTRACE(FPrint(_L("CCommonTestClass::Ptr testing MPXUser::Ptr(const TDesC& aDes) end err=%d"), err));
    iLog->Log(_L("CCommonTestClass::Ptr testing MPXUser::Ptr(const TDesC& aDes) end err=%d"), err);
	return err;
    }

    
// -----------------------------------------------------------------------------
// CCommonTestClass::Ptr8()
// Returns: Symbian OS errors.
// -----------------------------------------------------------------------------
TInt CCommonTestClass::Ptr8()
    {
	FTRACE(FPrint(_L("CCommonTestClass::Ptr8 testing MPXUser::Ptr(const TDesC& aDes) begin")));
    iLog->Log(_L("CCommonTestClass::Ptr8 testing MPXUser::Ptr(const TDesC& aDes) begin"));
    TInt err = KErrNone;

    TBufC<40> descriptor16(_L("xxx"));
    TPtrC8 mem = MPXUser::Ptr(descriptor16);
	FTRACE(FPrint(_L("CCommonTestClass::Ptr8 testing MPXUser::Ptr(const TDesC& aDes) end err=%d"), err));
    iLog->Log(_L("CCommonTestClass::Ptr8 testing MPXUser::Ptr(const TDesC& aDes) end err=%d"), err);
	return err;
    }


// -----------------------------------------------------------------------------
// CCommonTestClass::CreateBufferLC()
// Returns: Symbian OS errors.
// -----------------------------------------------------------------------------
TInt CCommonTestClass::CreateBufferLC()
    {
	FTRACE(FPrint(_L("CCommonTestClass::CreateBufferLC testing MPXUser::CreateBufferLC() begin")));
    iLog->Log(_L("CCommonTestClass::CreateBufferLC testing MPXUser::CreateBufferLC() begin"));
    TInt err = KErrNone;

    TBufC<40> descriptor(_L("xxx"));
    CBufBase* buf( MPXUser::CreateBufferLC( descriptor.Size() ));
    CleanupStack::Pop( buf );
    
	FTRACE(FPrint(_L("CCommonTestClass::CreateBufferLC testing MPXUser::CreateBufferLC(s) end err=%d"), err));
    iLog->Log(_L("CCommonTestClass::CreateBufferLC testing MPXUser::CreateBufferLC() end err=%d"), err);
	return err;
    }


// -----------------------------------------------------------------------------
// CCommonTestClass::CreateBufferDesCL()
// Returns: Symbian OS errors.
// -----------------------------------------------------------------------------
TInt CCommonTestClass::CreateBufferDesCL()
    {
	FTRACE(FPrint(_L("CCommonTestClass::CreateBufferDesCL testing MPXUser::CreateBufferL() begin")));
    iLog->Log(_L("CCommonTestClass::CreateBufferDesCL testing MPXUser::CreateBufferL() begin"));
    TInt err = KErrNone;

    TBufC<40> descriptor(_L("xxx"));
    CBufBase* buffer(NULL);
    MPXUser::CreateBufferL( descriptor, buffer );
    delete buffer;    
	FTRACE(FPrint(_L("CCommonTestClass::CreateBufferDesCL testing MPXUser::CreateBufferL(s) end err=%d"), err));
    iLog->Log(_L("CCommonTestClass::CreateBufferDesCL testing MPXUser::CreateBufferL() end err=%d"), err);
	return err;
    }


// -----------------------------------------------------------------------------
// CCommonTestClass::CreateBufferDesCArrayL()
// Returns: Symbian OS errors.
// -----------------------------------------------------------------------------
TInt CCommonTestClass::CreateBufferDesCArrayL()
    {
	FTRACE(FPrint(_L("CCommonTestClass::CreateBufferDesCArrayL testing MPXUser::CreateBufferL() begin")));
    iLog->Log(_L("CCommonTestClass::CreateBufferDesCArrayL testing MPXUser::CreateBufferL() begin"));
    TInt err = KErrNone;

    CDesCArray* array = new(ELeave)CDesCArrayFlat(10);    
    array->AppendL(_L("test"));
    CBufBase* buffer(NULL);
    MPXUser::CreateBufferL( array, buffer );
    delete buffer;    
    delete array;
	FTRACE(FPrint(_L("CCommonTestClass::CreateBufferDesCArrayL testing MPXUser::CreateBufferL(s) end err=%d"), err));
    iLog->Log(_L("CCommonTestClass::CreateBufferDesCArrayL testing MPXUser::CreateBufferL() end err=%d"), err);
	return err;
    }

// -----------------------------------------------------------------------------
// CCommonTestClass::CreateBufferMsgL()
// Returns: Symbian OS errors.
// Could not create an instance of RMessage2 properly, ignore this test case. 
// -----------------------------------------------------------------------------
TInt CCommonTestClass::CreateBufferMsgL()
    {//TODO need to update
	FTRACE(FPrint(_L("CCommonTestClass::CreateBufferMsgL testing MPXUser::CreateBufferL() begin")));
    iLog->Log(_L("CCommonTestClass::CreateBufferMsgL testing MPXUser::CreateBufferL() begin"));
    TInt err = KErrNone;
//
//    RMessage2* msg = new(ELeave)RMessage2();
       
//    HBufC* msg1 = HBufC::New( 20 );
//    HBufC* msg2 = HBufC::New( 20 );
//    TUint16 msgContent = (TUint16)1;
//    msg1->Des().Copy( &msgContent );
//    msg2->Des().Copy( &msgContent );
//
//    TPtr msgPtr1 = msg1->Des();
//    TPtr msgPtr2 = msg2->Des();
////    
////    TDes8* msgTDes(NULL);
//
//    msg->ReadL( 0,msgPtr1 );
//    msg->ReadL( 1,msgPtr2 );
    
//    msg.Write( 1,_L("Message") );
//    const TRequestStatus* statusConst = msg->ClientStatus();
//    TRequestStatus* status = const_cast<TRequestStatus*>( statusConst );
//    TInt statusValue= status->operator =( 2 );
//    TBuf8<2> msgContent;
//    TUint8 msgNum = (TUint8)1;
//    msgContent = &msgNum;
//    msg.Read( 0,msgContent );
//    CBufBase* buffer(NULL);
//    MPXUser::CreateBufferL( *msg,1,buffer );
//    delete buffer;    

	FTRACE(FPrint(_L("CCommonTestClass::CreateBufferMsgL testing MPXUser::CreateBufferL(s) end err=%d"), err));
    iLog->Log(_L("CCommonTestClass::CreateBufferMsgL testing MPXUser::CreateBufferL() end err=%d"), err);
	return err;
    }



// -----------------------------------------------------------------------------
// CCommonTestClass::ExternalizeL()
// Returns: Symbian OS errors.
// -----------------------------------------------------------------------------
TInt CCommonTestClass::ExternalizeL()
    {
	FTRACE(FPrint(_L("CCommonTestClass::ExternalizeL testing MPXUser::ExternalizeL() begin")));
    iLog->Log(_L("CCommonTestClass::ExternalizeL testing MPXUser::ExternalizeL() begin"));

	TInt err = KErrNone;
    
    CBufBase* buffer = CBufFlat::NewL( 50 );
    CleanupStack::PushL( buffer );
    RBufWriteStream writeStream( *buffer );
    CleanupClosePushL( writeStream );
    CDesCArray* array = new(ELeave)CDesCArrayFlat(10);    
    array->AppendL(_L("test"));
    MPXUser::ExternalizeL( array, writeStream );
    writeStream.CommitL();
    buffer->Compress();
    CleanupStack::PopAndDestroy( &writeStream );
    delete array;
    CleanupStack::PopAndDestroy( buffer );
    
	FTRACE(FPrint(_L("CCommonTestClass::ExternalizeL testing MPXUser::ExternalizeL() end err=%d"), err));
    iLog->Log(_L("CCommonTestClass::ExternalizeL testing MPXUser::ExternalizeL() end err=%d"), err);
	return err;
    }

// -----------------------------------------------------------------------------
// CCommonTestClass::ExternalizeArrayL()
// Returns: Symbian OS errors.
// -----------------------------------------------------------------------------
TInt CCommonTestClass::ExternalizeArrayL()
    {
	FTRACE(FPrint(_L("CCommonTestClass::ExternalizeArrayL testing MPXUser::ExternalizeL() begin")));
    iLog->Log(_L("CCommonTestClass::ExternalizeArrayL testing MPXUser::ExternalizeL() begin"));

	TInt err = KErrNone;
    
    CBufBase* buffer = CBufFlat::NewL( 50 );
    CleanupStack::PushL( buffer );
    RBufWriteStream writeStream( *buffer );
    CleanupClosePushL( writeStream );
    CDesCArray* array = new(ELeave)CDesCArrayFlat(10);    
    array->AppendL(_L("test"));
    RArray<TMPXItemId> items;
    CleanupClosePushL(items);
    MPXUser::ExternalizeL( items.Array(), writeStream );
    writeStream.CommitL();
    buffer->Compress();
    CleanupStack::PopAndDestroy( &items );
    CleanupStack::PopAndDestroy( &writeStream );
    delete array;
    CleanupStack::PopAndDestroy( buffer );
    
	FTRACE(FPrint(_L("CCommonTestClass::ExternalizeArrayL testing MPXUser::ExternalizeL() end err=%d"), err));
    iLog->Log(_L("CCommonTestClass::ExternalizeArrayL testing MPXUser::ExternalizeL() end err=%d"), err);
	return err;
    }


// -----------------------------------------------------------------------------
// CCommonTestClass::InternalizeArrayL()
// Returns: Symbian OS errors.
// -----------------------------------------------------------------------------



// -----------------------------------------------------------------------------
// CCommonTestClass::CopyArrayL()
// Returns: Symbian OS errors.
// -----------------------------------------------------------------------------
TInt CCommonTestClass::CopyArrayL()
    {
	FTRACE(FPrint(_L("CCommonTestClass::CopyArrayL testing MPXUser::CopyArrayL() begin")));
    iLog->Log(_L("CCommonTestClass::CopyArrayL testing MPXUser::CopyArrayL() begin"));
    TInt err = KErrNone;

    CDesCArray* src = new(ELeave)CDesCArrayFlat(10);    
    CDesCArray* dest = new(ELeave)CDesCArrayFlat(10);    
    src->AppendL(_L("test"));
    MPXUser::CopyArrayL( *src, *dest );
    delete src;    
    delete dest;
	FTRACE(FPrint(_L("CCommonTestClass::CopyArrayL testing MPXUser::CopyArrayL(s) end err=%d"), err));
    iLog->Log(_L("CCommonTestClass::CopyArrayL testing MPXUser::CopyArrayL() end err=%d"), err);
	return err;
    }


// -----------------------------------------------------------------------------
// CCommonTestClass::MergeArray()
// Returns: Symbian OS errors.
// -----------------------------------------------------------------------------
TInt CCommonTestClass::MergeArray()
    {
	FTRACE(FPrint(_L("CCommonTestClass::MergeArray testing MPXUser::MergeArray() begin")));
    iLog->Log(_L("CCommonTestClass::MergeArray testing MPXUser::MergeArray() begin"));
    TInt err = KErrNone;

    CDesCArray* src( NULL );
    TRAP(err,src = new(ELeave)CDesCArrayFlat(10));    
    CDesCArray* dest( NULL );
    TRAP(err,dest = new(ELeave)CDesCArrayFlat(20));    
    TRAP(err , src->AppendL(_L("test")));
    MPXUser::MergeArray( *src, *dest );
    delete src;    
    delete dest;
	FTRACE(FPrint(_L("CCommonTestClass::MergeArray testing MPXUser::MergeArray(s) end err=%d"), err));
    iLog->Log(_L("CCommonTestClass::MergeArray testing MPXUser::MergeArray() end err=%d"), err);
	return err;
    }
    
    
// -----------------------------------------------------------------------------
// CCommonTestClass::MergeAttributeL()
// Returns: Symbian OS errors.
// -----------------------------------------------------------------------------
TInt CCommonTestClass::MergeAttributeL()
    {
	FTRACE(FPrint(_L("CCommonTestClass::MergeAttributeL testing MPXUser::MergeAttributeL() begin")));
    iLog->Log(_L("CCommonTestClass::MergeAttributeL testing MPXUser::MergeAttributeL() begin"));
    TInt err = KErrNone;

    RArray<TMPXAttribute> atts1;
    RArray<TMPXAttribute> atts2;
    CleanupClosePushL(atts1);
    CleanupClosePushL(atts2);

    TInt contentId = 0;
    TUint attId = (TUint)0;
    TMPXAttribute att(contentId, attId);
    atts1.Append(att);
    
    MPXUser::MergeAttributeL( atts1.Array(), atts2 );

    CleanupStack::PopAndDestroy( &atts2 );
    CleanupStack::PopAndDestroy( &atts1 );

	FTRACE(FPrint(_L("CCommonTestClass::MergeAttributeL testing MPXUser::MergeAttributeL(s) end err=%d"), err));
    iLog->Log(_L("CCommonTestClass::MergeAttributeL testing MPXUser::MergeAttributeL() end err=%d"), err);
	return err;
    }


// -----------------------------------------------------------------------------
// CCommonTestClass::ProcessIdL()
// Returns: Symbian OS errors.
// -----------------------------------------------------------------------------
TInt CCommonTestClass::ProcessIdL()
    {
	FTRACE(FPrint(_L("CCommonTestClass::ProcessIdL testing MPXUser::ProcessIdL() begin")));
    iLog->Log(_L("CCommonTestClass::ProcessIdL testing MPXUser::ProcessIdL() begin"));
    TInt err = KErrNone;

    TThreadId threadId = RThread().Id();
    MPXUser::ProcessIdL(threadId);

	FTRACE(FPrint(_L("CCommonTestClass::ProcessIdL testing MPXUser::ProcessIdL(s) end err=%d"), err));
    iLog->Log(_L("CCommonTestClass::ProcessIdL testing MPXUser::ProcessIdL() end err=%d"), err);
	return err;
    }

// -----------------------------------------------------------------------------
// CCommonTestClass::CompareOrderedUidArrays()
// Returns: Symbian OS errors.
// -----------------------------------------------------------------------------
TInt CCommonTestClass::CompareOrderedUidArrays()
    {
	FTRACE(FPrint(_L("CCommonTestClass::CompareOrderedUidArrays testing MPXUser::CompareOrderedUidArrays() begin")));
    iLog->Log(_L("CCommonTestClass::CompareOrderedUidArrays testing MPXUser::CompareOrderedUidArrays() begin"));
    TInt err = KErrNone;

    RArray<TUid> atts1;
    RArray<TUid> atts2;
    CleanupClosePushL(atts1);
    CleanupClosePushL(atts2);

    const TUid KMPXCollectionServerUid3={0x101FFC31};  
    const TUid KPodcastCollection = {0x101FFB31};
    atts1.Append(KMPXCollectionServerUid3);
    atts1.Append(KPodcastCollection);
    
    MPXUser::CompareOrderedUidArrays( atts1.Array(), atts2.Array() );

    CleanupStack::PopAndDestroy( &atts2 );
    CleanupStack::PopAndDestroy( &atts1 );

	FTRACE(FPrint(_L("CCommonTestClass::CompareOrderedUidArrays testing MPXUser::CompareOrderedUidArrays(s) end err=%d"), err));
    iLog->Log(_L("CCommonTestClass::CompareOrderedUidArrays testing MPXUser::CompareOrderedUidArrays() end err=%d"), err);
	return err;
    }
//add function 



// -----------------------------------------------------------------------------
// CCommonTestClass::CreateFromBufferL()
// Returns: Symbian OS errors.
// -----------------------------------------------------------------------------
TInt CCommonTestClass::CreateFromBufferL()
    {
	FTRACE(FPrint(_L("CCommonTestClass::CreateFromBufferL testing MPXUser::CreateBufferL() begin")));
    iLog->Log(_L("CCommonTestClass::CreateFromBufferL testing MPXUser::CreateBufferL() begin"));

	TInt err = KErrNone;
	CDesCArray* array = new(ELeave)CDesCArrayFlat(10);    
    array->AppendL(_L("test"));
    CBufBase* buffer(NULL);
    MPXUser::CreateBufferL( array, buffer );
	
    MPXUser::CreateFromBufferL( *buffer, array );
    delete array;
    delete buffer;
    
	FTRACE(FPrint(_L("CCommonTestClass::CreateFromBufferL testing MPXUser::CreateBufferL(s) end err=%d"), err));
    iLog->Log(_L("CCommonTestClass::CreateFromBufferL testing MPXUser::CreateBufferL() end err=%d"), err);
	return err;
    }
// -----------------------------------------------------------------------------
// CCommonTestClass::InternalizeArrayL()
// Returns: Symbian OS errors.
// -----------------------------------------------------------------------------
TInt CCommonTestClass::InternalizeArrayL()
    {
	FTRACE(FPrint(_L("CCommonTestClass::InternalizeArrayL testing MPXUser::InternalizeL() begin")));
    iLog->Log(_L("CCommonTestClass::InternalizeArrayL testing MPXUser::InternalizeL() begin"));

	TInt err = KErrNone;
	
    CBufBase* buffer = CBufFlat::NewL( 50 );
    CleanupStack::PushL( buffer );
    RBufWriteStream writeStream( *buffer );
    CleanupClosePushL( writeStream );
    writeStream.WriteInt32L( 1 );
    writeStream.WriteUint32L( 1000 );
    writeStream.WriteUint32L( 1001 );
    writeStream.CommitL();
    //buffer->Compress();
    RBufReadStream readStream( *buffer );
    CleanupClosePushL( readStream );

    //create items
    RArray<TMPXItemId> items;
    CleanupClosePushL( items );
    
    MPXUser::InternalizeL( items, readStream );
    CleanupStack::PopAndDestroy( 4 );

	FTRACE(FPrint(_L("CCommonTestClass::InternalizeArrayL testing MPXUser::InternalizeL() end err=%d"), err));
    iLog->Log(_L("CCommonTestClass::InternalizeArrayL testing MPXUser::InternalizeL() end err=%d"), err);
	return err;
    }

// -----------------------------------------------------------------------------
// CCommonTestClass::InternalizeDesCArrayL()
// Returns: Symbian OS errors.
// -----------------------------------------------------------------------------
TInt CCommonTestClass::InternalizeDesCArrayL()
    {
	FTRACE(FPrint(_L("CCommonTestClass::InternalizeDesCArrayL testing MPXUser::InternalizeL() begin")));
    iLog->Log(_L("CCommonTestClass::InternalizeDesCArrayL testing MPXUser::InternalizeL() begin"));

	TInt err = KErrNone;
	
    CBufBase* buffer = CBufFlat::NewL( 50 );
    CleanupStack::PushL( buffer );
    RBufWriteStream writeStream( *buffer );
    CleanupClosePushL( writeStream );
    writeStream.WriteInt32L( 0 );
//    writeStream.WriteUint32L( 1000 );
//    writeStream.WriteL( _L("ArrayItem") );
    writeStream.CommitL();
    //buffer->Compress();
    RBufReadStream readStream( *buffer );
    CleanupClosePushL( readStream );
    
    CDesCArray* array = new(ELeave)CDesCArrayFlat(10);    
    array->AppendL(_L("test"));

    
    MPXUser::InternalizeL( array, readStream );
    buffer->Compress();
    delete array;
    CleanupStack::PopAndDestroy( 3 );
     
	FTRACE(FPrint(_L("CCommonTestClass::InternalizeDesCArrayL testing MPXUser::InternalizeL() end err=%d"), err));
    iLog->Log(_L("CCommonTestClass::InternalizeDesCArrayL testing MPXUser::InternalizeL() end err=%d"), err);
	return err;
    }


// -----------------------------------------------------------------------------
// CCommonTestClass::FindWindowGroupIdL()
// Returns: Symbian OS errors.
// -----------------------------------------------------------------------------
TInt CCommonTestClass::FindWindowGroupIdL()
    {
    FTRACE(FPrint(_L("CCommonTestClass::FindWindowGroupIdL testing MPXUser::FindWindowGroupIdL() begin")));
    iLog->Log(_L("CCommonTestClass::FindWindowGroupIdL testing MPXUser::FindWindowGroupIdL() begin"));
    
    TInt err = KErrNone;
    TProcessId aProcId;
  
    TThreadId threadId = RThread().Id();
    aProcId= MPXUser::ProcessIdL(threadId);
   
    RWsSession wsSession=CEikonEnv::Static()->WsSession();

    MPXUser::FindWindowGroupIdL(aProcId,wsSession );
  
    FTRACE(FPrint(_L("CCommonTestClass::FindWindowGroupIdL testing MPXUser::FindWindowGroupIdL() end err=%d"), err));
    iLog->Log(_L("CCommonTestClass::FindWindowGroupIdL testing MPXUser::FindWindowGroupIdL() end err=%d"), err);
   
   	return err; 
    }
// -----------------------------------------------------------------------------
// CCommonTestClass::CompleteWithDllPathOL()
// Returns: Symbian OS errors.
// -----------------------------------------------------------------------------
TInt CCommonTestClass::CompleteWithDllPathOL()
    {
    FTRACE(FPrint(_L("CCommonTestClass::RecreateBufferL testing MPXUser::RecreateBufferL() begin")));
    iLog->Log(_L("CCommonTestClass::RecreateBufferL testing MPXUser::RecreateBufferL() begin"));
  
    TInt err = KErrNone;
    TParse parse;
    _LIT(KMPXCollectionDbResourceFile,  "mpxcollectiondbres.rsc");
    parse.Set( KMPXCollectionDbResourceFile, &KDC_APP_RESOURCE_DIR, NULL );
    TFileName resFile(parse.FullName());
    _LIT(KmpxDllName,"mpxcollectiondbres.dll");
    const TDesC& aDllName = KmpxDllName;
    MPXUser::CompleteWithDllPath(aDllName,resFile);
    
    FTRACE(FPrint(_L("CCommonTestClass::RecreateBufferL testing MPXUser::RecreateBufferL() end err=%d"), err));
    iLog->Log(_L("CCommonTestClass::RecreateBufferL testing MPXUser::RecreateBufferL() end err=%d"), err);

	return err;
    }

// -----------------------------------------------------------------------------
// CCommonTestClass::RecreateBufferL()
// Returns: Symbian OS errors.
// -----------------------------------------------------------------------------
TInt CCommonTestClass::RecreateBufferL()
    {
    FTRACE(FPrint(_L("CCommonTestClass::RecreateBufferL testing MPXUser::RecreateBufferL() begin")));
    iLog->Log(_L("CCommonTestClass::RecreateBufferL testing MPXUser::RecreateBufferL() begin"));

    TInt err = KErrNone;
    TInt aSize = 4;
    CBufBase* buf=CBufFlat::NewL(aSize);
    MPXUser::RecreateBufferL( 4 , buf );
    
    FTRACE(FPrint(_L("CCommonTestClass::RecreateBufferL testing MPXUser::RecreateBufferL() end err=%d"), err));
    iLog->Log(_L("CCommonTestClass::RecreateBufferL testing MPXUser::RecreateBufferL() end err=%d"), err);
	return err;
    }
// -----------------------------------------------------------------------------
// CCommonTestClass::Alloc8ZL()
// Returns: Symbian OS errors.
// -----------------------------------------------------------------------------
TInt CCommonTestClass::Alloc8ZL()
    {
      FTRACE(FPrint(_L("CCommonTestClass::Alloc8ZL testing MPXUser::Alloc8ZL() begin")));
	  iLog->Log(_L("CCommonTestClass::Alloc8ZL testing MPXUser::Alloc8ZL() begin"));
	  TInt err = KErrNone;
	  TParse parse;
      _LIT(descriptor,  "memory to be allocated");
      HBufC8* mem = MPXUser::Alloc8ZL(descriptor);
      delete mem;
      FTRACE(FPrint(_L("CCommonTestClass::Alloc8ZL testing MPXUser::Alloc8ZL() end err=%d"), err));
      iLog->Log(_L("CCommonTestClass::Alloc8ZL testing MPXUser::Alloc8ZL() end err=%d"), err);
      return err;
    }
    
// -----------------------------------------------------------------------------
// CCommonTestClass::CreateBufferRMessage()
// Returns: Symbian OS errors.
// -----------------------------------------------------------------------------

