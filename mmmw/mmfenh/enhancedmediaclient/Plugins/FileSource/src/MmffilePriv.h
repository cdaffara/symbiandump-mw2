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
* Description:  Header of TransferBuffer.
*
*/


#ifndef __MMFFILEPRIV_H_
#define __MMFFILEPRIV_H_

#include <f32file.h>

#include <mmf/server/mmfclip.h>
#include <mmf/server/mmfdatabuffer.h>


/**
@publishedAll
@released

  Provides the filename and path of file. 
  
    Used by CFileSource::ConstructL().
*/
class TMMFFileParams
    {
    public:
    /**
    Filename and path of file. 
        */
        TFileName iPath ; // Filename and path of file.
    private:
    /**
    This member is internal and not intended for use.
        */
        TInt iReserved1;
    };

    /**
    @internalTechnology	
*/
const TUid KFileHandleUid = {0x101FFA19};
const TUid KMMFileSourceUid = {0x101F7DA9};
const TUid KMMFileHandleSourceUid = {0x101F7DAA};
const TUid KFileSourceUid = {0x10207B89};

/**
@publishedAll
@released

  Provides a pointer to the file handle 
  
    Used by CFileSource::ConstructL().
*/
class TMMFFileHandleParams
    {
    public:
        TMMFFileHandleParams(RFile* aFile) : iUid(KFileHandleUid), iFile(aFile) {}
        TMMFFileHandleParams() : iUid(KFileHandleUid) {}
        
        TUid iUid;
        /**
        Handle of file 
        */
        RFile* iFile;
    private:
    /**
    This member is internal and not intended for use.
        */
        TInt iReserved1;
    };




    /**
    @publishedAll
    @released
*/
typedef TPckgBuf<TMMFFileParams>  TMMFFileConfig ;

/**
@publishedAll
@released
*/
typedef TPckgBuf<TMMFFileHandleParams>  TMMFFileHandleConfig ;


/**
@publishedAll
@released

  This details how big the iTransferBufferCopies array of CFileSource may
  become before we start looking to free entries.
*/
const TInt KAcceptableTransferBufferCopiesSize = 5;

/**
@publishedAll
@released

  This details how big the iTransferBufferCopies array of CFileSource may
  become before we start looking to free entries.
*/
const TInt KMaximumTransferBufferCopiesSize = 100;


/**
*  @publishedAll
*
*  Represents a copy of a KUidMmfTransferBuffer used for reading/writting to the file server
*/
class CTransferBufferCopy : public CBase
    {
    public:
        static CTransferBufferCopy* NewL(TInt aMaxLength);
        
        virtual ~CTransferBufferCopy()
            {delete iBuffer;}
        
        
        TDes8& Des() {return iBufferDes;}
        
        TInt MaxLength() {return iBufferDes.MaxLength();}
        
        void ReUse(TInt aMaxLength) {iBufferDes.Set(iBuffer,0, Min(aMaxLength, iMaxLength));}
        
        TBool InUse() {return iInUse;}
        
        void SetInUse(TBool aInUse) {iInUse=aInUse;}
        
    private:
        CTransferBufferCopy(TInt aMaxLength) : CBase(), iMaxLength(aMaxLength), iBufferDes(0,0,0), iInUse(EFalse){}
        
        void ConstructL();
        
    private:
        TUint8*	iBuffer;
        
        //Holds the original MaxLength when class constructed. 
        //May be larger than MaxLength of iBufferDes
        TInt	iMaxLength;
        
        TPtr8	iBufferDes;
        
        TBool	iInUse;
    };




    /**
    * @internalComponent
    *
    * A request is created when an external object requests or supplies data.  Calls to the File Server are
    * made asynchronously and a CReadWriteRequest created to notify the caller on completion.
    *
    * CReadWriteRequest is an abstract class.  Concrete instances are of CReadRequest & CWriteRequest.
    * Concrete instances need to know whether to call MDataSink::BufferFilledL() or MDataSource::BufferEmptiedL()
*/
class CReadWriteRequest : public CActive
    {
    public:
        CReadWriteRequest(TAny* aSinkOrSource, CMMFBuffer* aBuffer,TBool aSourceType)
            : CActive(EPriorityStandard),
            iSinkOrSource(aSinkOrSource), 
            iBuffer(aBuffer),
            iSourceType(aSourceType)
            {
            CActiveScheduler::Add( this );
            }
        
        CReadWriteRequest(TAny* aSinkOrSource, CMMFBuffer* aBuffer, CTransferBufferCopy* aOptionalDataBuffer,TBool aSourceType)
            : CActive(EPriorityStandard),
            iSinkOrSource(aSinkOrSource), 
            iBuffer(aBuffer),
            iSourceType(aSourceType),
            iTransferBufferCopy(aOptionalDataBuffer)
            {
            CActiveScheduler::Add( this );
            iTransferBufferCopy->SetInUse(ETrue);
            }
        
        CReadWriteRequest(CReadWriteRequest& aRequest)
            : CActive(EPriorityStandard),
            iSinkOrSource(aRequest.iSinkOrSource),
            iBuffer(aRequest.iBuffer),
            iSourceType(aRequest.iSourceType),
            iTransferBufferCopy(aRequest.iTransferBufferCopy)
            {
            CActiveScheduler::Add( this );
            iTransferBufferCopy->SetInUse(ETrue);
            }
        
        
        inline TBool Processing() const { return iState == EProcessing; }   
        inline TBool Completed() const { return iState == ECompleted; }
        TDes8& BufferDes() ;
        TBool SourceType();
        const TDesC8& BufferDesC() ;
        CMMFBuffer* Buffer();
        TAny* GetSinkOrSource();
        ~CReadWriteRequest() ;
        
        
        // CActive functions.
        // 
        void SetActive() ;
        void DoCancel() ;
        virtual void RunL() = 0 ;
        virtual TInt RunError( TInt aError ) ;
        
        
    protected :
        TAny* iSinkOrSource;
        CMMFBuffer* iBuffer;
        TBool iSourceType;
        CTransferBufferCopy* iTransferBufferCopy;
        
        TBool iCompleted ;
        TDes8* iBufferDes ;
        TInt iError ;
        TBool iUseTransferBuffer ;
        void SetTransferBuffer (TBool aTBuffer) ;
        TBool CanUseTransferBuffer () ;
        enum TState
            {
            EActive,
            EProcessing,
            ECompleted
            };
        
        TState iState;
    } ;

    /**
    * @internalComponent
*/
class CReadRequest : public CReadWriteRequest
    {
    public :
        CReadRequest(TAny* aParent,TAny* aSinkOrSource, CMMFBuffer* aBuffer, TUint aPosition, TUint aFileSize,TBool aSourceType)
            : CReadWriteRequest(aSinkOrSource, aBuffer,aSourceType),
            iPosition(aPosition), iFileSize(aFileSize),iParent(aParent)
            { } 
        
        CReadRequest(TAny* aParent,TAny* aSinkOrSource, CMMFBuffer* aBuffer, CTransferBufferCopy* aOptionalDataBuffer, TUint aPosition, TUint aFileSize,TBool aSourceType)
            : CReadWriteRequest(aSinkOrSource, aBuffer, aOptionalDataBuffer,aSourceType),
            iPosition(aPosition), iFileSize(aFileSize),iParent(aParent)
            { } 
        
        void RunL();
    private:
        TUint iPosition;
        TUint iFileSize;
        TAny* iParent;
    };

#endif

