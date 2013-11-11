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
* Description:   TNE Video Clip Implementation
*
*/




#include "HXTNEVideoClipInfoImp.h"
#include <e32base.h>


#include <fbs.h>
#include <coemain.h>
#include <ecom/ecom.h>


#include "yuv2rgb12.h"
#include "yuv2rgb16.h"
#include "yuv2rgb24.h"

#include <flogger.h>

// PRINT macro
#ifdef _DEBUG
#include <e32svr.h>
#define PRINT(x) RDebug::Print x
#else
#define PRINT(x)
#endif

#ifdef _DEBUG
#define ENABLE_S60_TNE_LOGGING 1
#endif

//implementation
#ifdef ENABLE_S60_TNE_LOGGING
#define FLOG FileLog
inline static void FileLog(TRefByValue<const TDesC16> aFmt,...)
{
    VA_LIST  args;
    VA_START (args, aFmt);
    RFileLogger::WriteFormat(_L("tne"), _L("S60tne.log"), 
                        EFileLoggingModeAppend, aFmt, args);
}

#else // logging is disabled
#define FLOG LogFNull
inline static void LogFNull(TRefByValue<const TDesC16> aFmt,...){}

#endif // enable logging

//server details
const TUint kDefaultMessageSlots=4;



// -----------------------------------------------------------------------------
// CTNEVideoClipInfo::NewL
// API exposed by TNE to 
// TNEVideoClipInfo using RFile handle
// -----------------------------------------------------------------------------
EXPORT_C CTNEVideoClipInfo* CTNEVideoClipInfo::NewL(const RFile& aFileHandle,
                                                    MTNEVideoClipInfoObserver& aObserver)
{
    FLOG(_L("CTNEVideoClipInfo::NewL"));

     CHXTNEVideoClipInfoImp* self = ( CHXTNEVideoClipInfoImp*)NewLC(aFileHandle, aObserver);


    FLOG(_L("CTNEVideoClipInfo::NewL out"));
    return self;
}

// -----------------------------------------------------------------------------
// CTNEVideoClipInfo::NewL
// API exposed by TNE to create TNEVideoClipInfo using file name
// -----------------------------------------------------------------------------
EXPORT_C CTNEVideoClipInfo* CTNEVideoClipInfo::NewL(const TDesC& aFileName,
                                                    MTNEVideoClipInfoObserver& aObserver)
{
    FLOG(_L("CTNEVideoClipInfo::NewL"));

     CHXTNEVideoClipInfoImp* self = ( CHXTNEVideoClipInfoImp*)NewLC(aFileName, aObserver);


    FLOG(_L("CTNEVideoClipInfo::NewL out"));
    return self;
}

EXPORT_C CTNEVideoClipInfo* CTNEVideoClipInfo::NewLC(const RFile& aFileHandle,
                                                     MTNEVideoClipInfoObserver& aObserver)
{
     	
    CHXTNEVideoClipInfoImp* self = new (ELeave)  CHXTNEVideoClipInfoImp();
    CleanupStack::PushL(self);
    self->ConstructL(aFileHandle, aObserver);   
    CleanupStack::Pop();
    return self;
}

    
EXPORT_C CTNEVideoClipInfo* CTNEVideoClipInfo::NewLC(const TDesC& aFileName,
                                                     MTNEVideoClipInfoObserver& aObserver)
{
    
     CHXTNEVideoClipInfoImp* self = new (ELeave)  CHXTNEVideoClipInfoImp();
    CleanupStack::PushL(self);
    self->ConstructL(aFileName, aObserver);   
     CleanupStack::Pop();
    return self;
}




CTNEVideoClipInfo*  CHXTNEVideoClipInfoImp::NewL(const RFile& aFileHandle, 
                                              MTNEVideoClipInfoObserver& aObserver)
{
     CHXTNEVideoClipInfoImp* self = new (ELeave)  CHXTNEVideoClipInfoImp();
    CleanupStack::PushL(self);
    self->ConstructL(aFileHandle, aObserver);
    CleanupStack::Pop(self);
    return self;
}

//constructor

 CHXTNEVideoClipInfoImp::CHXTNEVideoClipInfoImp()
 {
 	//empty constructor
 
 }


//destructor


 CHXTNEVideoClipInfoImp::~CHXTNEVideoClipInfoImp()
{
    FLOG(_L("CTNEVideoClipInfo::~ CHXTNEVideoClipInfoImp in"));
    
        
    
    RSessionBase::Close();
    iServerThread.Close(); 
   
    if (iServerName)
   {
    	delete iServerName;
    	iServerName = NULL;
    }
    if (iObserver)
    {
        delete iObserver;
        iObserver = NULL;
    }
    
    if (iCloseHandle)
    {    	
        iCloseHandle = EFalse;
    	iFileHandle.Close();	
    }
        		           		    
    if (iOutBitmap)
    {
        delete iOutBitmap;
        iOutBitmap = 0;
    }
            
    
    if(iFs)
    {
	    iFs->Close();
	    delete iFs;
	    iFs = NULL;
    }
    
    if (iEnhancer)
    {
        delete iEnhancer;
        iEnhancer = NULL;
    }
    
    FLOG(_L("CTNEVideoClipInfo::~ CHXTNEVideoClipInfoImp out"));    
}

void  CHXTNEVideoClipInfoImp::ConstructL(const RFile& aFileHandle,
                                      MTNEVideoClipInfoObserver& aObserver)
{
    FLOG(_L("CTNEVideoClipInfo::ConstructL in this=%x"), this);
    iObserver = CTNEClientObserver::NewL(this);
	iClipInfoObserver = &aObserver;

		
	User::LeaveIfError(CreateServer());
	iFileHandle.Duplicate(aFileHandle);
	iCloseHandle = ETrue;
	User::LeaveIfError(OpenFile(iFileHandle, 0));
    
    FLOG(_L("CTNEVideoClipInfo::ConstructL out this=%x"), this);
    
 }

void  CHXTNEVideoClipInfoImp::ConstructL(const TDesC& aFileName,
                                      MTNEVideoClipInfoObserver& aObserver)
{
    FLOG(_L("CTNEVideoClipInfo::ConstructL in this=%x"), this);
    iObserver = CTNEClientObserver::NewL(this);
   	iClipInfoObserver = &aObserver;
   	iFileName.Copy(aFileName);
   
    User::LeaveIfError(CreateServer());
	
    User::LeaveIfError(OpenFile(0));
   
    
    FLOG(_L("CTNEVideoClipInfo::ConstructL out this=%x"), this);
}

TInt  CHXTNEVideoClipInfoImp::CreateServer()
{
	TInt err = KErrNoMemory;	
	FLOG(_L("CHXTNEVideoClipInfoImp::CreateServer in"));    
	 TRAP(err,iServerName = HBufC::NewL(KMaxFileName));
	
	if (iServerName && err == KErrNone)
	{ 
		 TPtr ptr = iServerName->Des();
    ptr.Copy(KTneServerName);
    ptr.AppendNum(Math::Random(), EHex);
	
		err = StartThread(iServerThread, iServerName);
	}		
	
    if ( err == KErrNone)
    {
       //server name FIXME
       err = CreateSession(*iServerName, Version(), kDefaultMessageSlots);
    }
                     
    FLOG(_L("CHXTNEVideoClipInfoImp::CreateServer out err=%d"), err);   
    return(err); 
  
}

TVersion CHXTNEVideoClipInfoImp::Version(void) const
    {
    return(TVersion(KTNEServMajorVersionNumber,KTNEServMinorVersionNumber,KTNEServBuildVersionNumber));
    }


//SendRecieve() commands to server

TInt CHXTNEVideoClipInfoImp::OpenFile(const RFile& aFileHandle, TInt aPosition)

{
   FLOG(_L("CHXTNEVideoClipInfoImp::OpenFile in"));
   c_State = EStartGettingMetadata;
   iObserver->Activate();
   SendReceive(EOpenFileRFmsg,TIpcArgs(&aFileHandle,aPosition), iObserver->iStatus);    
   FLOG(_L("CHXTNEVideoClipInfoImp::OpenFile out"));
   return KErrNone;
 }

TInt CHXTNEVideoClipInfoImp::OpenFile(TInt aPosition)

{
   FLOG(_L("CHXTNEVideoClipInfoImp::OpenFile in"));
   c_State = EStartGettingMetadata;
   iObserver->Activate();
   SendReceive(EOpenFIleNamemsg,TIpcArgs(&iFileName,aPosition), iObserver->iStatus);    
   FLOG(_L("CHXTNEVideoClipInfoImp::OpenFile out"));
   return KErrNone;
 }

void CHXTNEVideoClipInfoImp::GetThumbL(MTNEVideoClipThumbObserver& aObserver, 
                                     TInt aIndex,
                                     TSize* const aResolution,
                                     TDisplayMode aDisplayMode,
                                     TBool aEnhance,
                                     TInt aPriority)
                                     
{
        FLOG(_L("CHXTNEVideoClipInfoImp::GetThumbL in"));
        
        iThumbObserver               = &aObserver;
        iThumbIndex                    = aIndex;
        iThumbEnhance               = aEnhance;	
        iThumbDisplayMode         = aDisplayMode; 
        iPriority                            = aPriority;                
        iThumbRequestData.iIndex =  aIndex;
        
        if (aResolution)
        {    	    
            iOutputThumbResolution = *aResolution;
        }
        else
        {    	                            	
            iOutputThumbResolution.iHeight = iMetaData.iHeight;
            iOutputThumbResolution.iWidth = iMetaData.iWidth;         
        }
        
        iThumbRequestData.iYUVBuffer = NULL;                
        c_State = EStartGettingThumbNail;
        iObserver->Activate();
        SendReceive(EGetThumbmsg, TIpcArgs(&iThumbRequestData), iObserver->iStatus);    
        FLOG(_L("CHXTNEVideoClipInfoImp::GetThumbL out"));
 }
  

void CHXTNEVideoClipInfoImp::CancelThumb()
{
    FLOG(_L("CHXTNEVideoClipInfoImp::CancelThumb in"));
    c_State = ECancelling;
    SendReceive(ECancelThumbmsg);    
    FLOG(_L("CHXTNEVideoClipInfoImp::CancelThumb out"));
}
  
  TPtrC CHXTNEVideoClipInfoImp::FileName() const
{
    TFileName fileName;
	if (iCloseHandle)
	{
		iFileHandle.FullName(fileName);
	}

    return fileName;
}

TInt CHXTNEVideoClipInfoImp::VideoFrameCount() const
{
	return (TInt)iMetaData.iFrameCount ;

}
  

// Called from Observer after a Complete() from Server

void  CHXTNEVideoClipInfoImp::ServiceCompleted(TInt aError)
{
	FLOG(_L("CHXTNEVideoClipInfoImp::ServiceCompleted in aError=%d c_State=%d"),
	            aError, c_State);
    switch(c_State)
    {
        case EStartGettingMetadata:			                        
            FetchAndSendMetaData(aError);            
            break;
        case EStartGettingThumbNail:
            if (aError == KErrNone)
            {
                GenerateThumbNail();
            }
            else
            {
                if (iOutBitmap)
                {
                    delete iOutBitmap;
                    iOutBitmap = NULL;
                }
                m_LastError = aError;
            }
            
			NotifyIfGetThumbPending(m_LastError, iOutBitmap);
			break;
		case ECancelling:
        default:
            break;
    }
    
    FLOG(_L("CHXTNEVideoClipInfoImp::ServiceCompleted out"));
    
}

void CHXTNEVideoClipInfoImp::FetchAndSendMetaData(TInt aError)
{
    FLOG(_L("CHXTNEVideoClipInfoImp::FetchAndSendMetaData in"));
    if (aError == KErrNone)
    {
        aError = SendReceive(EGetMetaDatamsg, TIpcArgs(&iMetaData));
    }
    if(c_State== EStartGettingMetadata)
    {
    CTNEVideoClipInfo *pInfo = (CTNEVideoClipInfo *) this;
    iClipInfoObserver->NotifyVideoClipInfoReady(*pInfo, aError);  
    }
    FLOG(_L("CHXTNEVideoClipInfoImp::FetchAndSendMetaData out and state = %d"), c_State);

}

// ownership of pBitMap will be passed to Observer
void CHXTNEVideoClipInfoImp::NotifyIfGetThumbPending(TInt aError, CFbsBitmap *&pBitMap) 
{
    FLOG(_L("CHXTNEVideoClipInfoImp::NotifyIfGetThumbPending in"));
	if (iThumbObserver)
	{				
        CFbsBitmap* pTmpBitmap = pBitMap;
        pBitMap = NULL;
		iThumbObserver->NotifyVideoClipThumbCompleted(*this, aError, pTmpBitmap);
	}
	FLOG(_L("CHXTNEVideoClipInfoImp::NotifyIfGetThumbPending out"));
}

void CHXTNEVideoClipInfoImp::GenerateThumbNail()
{
	FLOG(_L("CHXTNEVideoClipInfoImp::GenerateThumbNail in"));
    TInt bytesPerPixel = 0;           
    TInt error = KErrNone;
    
    if ( !iRgbBuf )
    {
        //we only support even frame width & height. Ignore and return error
        if ( (iMetaData.iWidth & 0x01) || (iMetaData.iHeight & 0x01) )
        {
             FLOG(_L("CHXTNEVideoClipInfoImp::GenerateThumbNail non-even frame size returning error iMetaData.iWidth=%d iMetaData.iHeight=%d "),iMetaData.iWidth,iMetaData.iHeight);
             HandleThumbnailError(KErrNotSupported);
             return;
        }
        
        TSize inputFrameResolution(iMetaData.iWidth,iMetaData.iHeight);
                       
        // rgb specs
        TUint thumbLength = iMetaData.iWidth * iMetaData.iHeight; 
        TUint thumbUVLength = thumbLength>>2;	
        
       // VPASSERT(iYuvBuf);
        // assign yuv pointers 
        TUint8* yBuf = iThumbRequestData.iYUVBuffer;
        TUint8* uBuf = yBuf + thumbLength;
        TUint8* vBuf = uBuf + thumbUVLength;	
      
  	   // convert to no enhabnce if 64K is selected as thumb display mode
  	 	if(iThumbDisplayMode == EColor64K)
  	    iThumbEnhance = EFalse;
  	   
        // check validity of thumbnail and associated operation
        if(iThumbEnhance)		// for saving to file
        {			
                iThumbDisplayMode = EColor16M;				// 24-bit color image for enhancement
          
            // no enhancements if clip is too small.
            if (iOutputThumbResolution.iWidth < 16 || iOutputThumbResolution.iHeight < 16)
            {
            	iThumbEnhance = EFalse;            	
            }
        }
        else								// for screen display
        {
            if(iThumbDisplayMode == ENone)					// if no preference
                iThumbDisplayMode = EColor64K;				// 16-bit image
        }
        
        // determine proper bit depth for the bitmap
        if(iThumbDisplayMode == EColor16M)
            bytesPerPixel = 3; // 24-bit rgb takes 3 bytes, stored as bbbbbbbb gggggggg rrrrrrrr
        else if(iThumbDisplayMode == EColor64K || iThumbDisplayMode == EColor4K)
            bytesPerPixel = 2; // 12-bit rgb takes 2 bytes, stored as ggggbbbb xxxxrrrr                            
        else
        {
            HandleThumbnailError(KErrNotSupported);
            return;					// support for 12-, 16- and 24-bit color images only
        }
        
        // create output rgb buffer
        TRAP(error, iRgbBuf = (TUint8*) User::AllocL(thumbLength * bytesPerPixel));
        if (HandleThumbnailError(error))
            return;                                
        
        TInt scanLineLength;
        
        // convert yuv to rgb
        switch (iThumbDisplayMode)
        {
            
        case EColor4K:
            {
                TInt error;
                CYuv2Rgb12* yuvConverter = NULL; 
                TRAP(error, yuvConverter = new(ELeave) CYuv2Rgb12);
                if (HandleThumbnailError(error))
                    return;
                scanLineLength = inputFrameResolution.iWidth * bytesPerPixel;                 
                TRAP(error, yuvConverter->ConstructL(inputFrameResolution.iWidth, inputFrameResolution.iHeight, inputFrameResolution.iWidth, inputFrameResolution.iHeight));
                if (HandleThumbnailError(error))
                    return;
                yuvConverter->Convert(yBuf, uBuf, vBuf, inputFrameResolution.iWidth, inputFrameResolution.iHeight, iRgbBuf, scanLineLength);                
                delete yuvConverter;		
                yuvConverter=0;         
            }
            break;
            
        default:
        case EColor64K:
            {
                TInt error;
                CYuv2Rgb16* yuvConverter = NULL; 
                TRAP(error, yuvConverter = new(ELeave) CYuv2Rgb16);
                if (HandleThumbnailError(error))
                    return;                
                scanLineLength = inputFrameResolution.iWidth * bytesPerPixel; 
                TRAP(error, yuvConverter->ConstructL(inputFrameResolution.iWidth, inputFrameResolution.iHeight, inputFrameResolution.iWidth, inputFrameResolution.iHeight);)
                if (HandleThumbnailError(error))
                    return;
                yuvConverter->Convert(yBuf, uBuf, vBuf, inputFrameResolution.iWidth, inputFrameResolution.iHeight, iRgbBuf, scanLineLength);                
                delete yuvConverter;		
                yuvConverter=0; 
            }
            break;
            
        case EColor16M:
            {
                TInt error;
                CYuv2Rgb24* yuvConverter = NULL; 
                TRAP(error, yuvConverter = new(ELeave) CYuv2Rgb24);
                if (HandleThumbnailError(error))
                    return;                                
                scanLineLength = inputFrameResolution.iWidth * bytesPerPixel;                 
                TRAP(error, yuvConverter->ConstructL(inputFrameResolution.iWidth, inputFrameResolution.iHeight, inputFrameResolution.iWidth, inputFrameResolution.iHeight))
                if (HandleThumbnailError(error))
                    return;                
                yuvConverter->Convert(yBuf, uBuf, vBuf, inputFrameResolution.iWidth, inputFrameResolution.iHeight, iRgbBuf, scanLineLength);                
                delete yuvConverter;		
                yuvConverter=0; 
            }
            break;
        }        
    }

	if(!iThumbEnhance)
	{
        TSize inputFrameResolution(iMetaData.iWidth,iMetaData.iHeight);
		    
		/* Pre-calculate pixel indices for horizontal scaling. */
		// inputFrameResolution is the resolution of the image read from video clip.
		// iOutputThumbResolution is the final resolution desired by the caller.
		
		TInt OutW = (iOutputThumbResolution.iWidth + 3 )/4 * 4;
		TInt OutH = (iOutputThumbResolution.iHeight + 3 )/4 * 4;
		
		TSize roundedsize = TSize (OutW,OutH);
		
		TInt xIncrement = inputFrameResolution.iWidth * OutW;
		TInt xBoundary =  OutW * OutW;
		
		TInt* xIndices = 0;
		TRAPD(xIndicesErr, xIndices = new (ELeave) TInt[OutW]);
		if (xIndicesErr == KErrNone)
		{
			TInt xDecision = xIncrement / bytesPerPixel; // looks like they changed here - orig was /2
			TInt sourceIndex = 0;
			for (TInt x = 0; x < OutW; x++)
			{
				while (xDecision > xBoundary)
				{
					xDecision -= xBoundary;
					sourceIndex += bytesPerPixel;
				}
				
				xIndices[x] = sourceIndex;
				xDecision += xIncrement;
			}
		}
		else
		{		    
		    HandleThumbnailError(xIndicesErr);
		    return;
		}
		
		/* Initialize bitmap. */
		TRAPD(bitmapErr, iOutBitmap = new (ELeave) CFbsBitmap);
		if ((xIndicesErr == KErrNone) && (bitmapErr == KErrNone))
		{
			bitmapErr = iOutBitmap->Create(roundedsize, iThumbDisplayMode/*EColor64K*/);
			if (bitmapErr == KErrNone)
			{
                // Lock the heap to prevent the FBS server from invalidating the address
                iOutBitmap->LockHeap();

				/* Scale to desired iOutputThumbResolution and copy to bitmap. */
				TUint8* dataAddress = (TUint8*)iOutBitmap->DataAddress(); // fix
				
				TInt yIncrement = inputFrameResolution.iHeight * OutH;
				TInt yBoundary =  OutH * OutH;
				
				TInt targetIndex = 0;
				TInt sourceRowIndex = 0;
				TInt yDecision = yIncrement / 2; 
				for (TInt y = 0; y < OutH; y++)
				{
					while (yDecision > yBoundary)
					{
						yDecision -= yBoundary;
						sourceRowIndex += (inputFrameResolution.iWidth * bytesPerPixel); 
					}
					yDecision += yIncrement;


					for (TInt x = 0; x < OutW; x++)					
					{
                        for (TInt i = 0; i < bytesPerPixel; ++i)
                        {
                            const TInt firstPixelSourceIndex = sourceRowIndex + xIndices[x] + i;
                            dataAddress[targetIndex] = iRgbBuf[firstPixelSourceIndex];
                            targetIndex++;
                        }
                    }
				}
                iOutBitmap->UnlockHeap();
                
                // resing to original if odd values were input
               if(roundedsize != iOutputThumbResolution)
               {
               
               	TInt err ;
               	err = iOutBitmap->Resize(iOutputThumbResolution);
               	if(err!= KErrNone)
               		HandleThumbnailError(err);	
               	
               }
                
			} 
			
			else
			{			    
				delete iOutBitmap; iOutBitmap = 0;
				HandleThumbnailError(bitmapErr);
				return;
			}
		}
		else
		{
		    HandleThumbnailError(bitmapErr);
		    delete[] xIndices; xIndices = 0;
		    return;
		}
		
		delete[] xIndices;
		xIndices = 0;
	}
	else		// enhance
	{
		TInt i,j;
		// create input bitmap and buffer
		CFbsBitmap* inBitmap = 0;
		TRAPD(inBitmapErr, inBitmap = new (ELeave) CFbsBitmap);
		if( inBitmapErr == KErrNone )
        {
		    // create bitmaps
		    TSize originalResolution(iMetaData.iWidth, iMetaData.iHeight);
		    inBitmapErr = inBitmap->Create(originalResolution, iThumbDisplayMode/*EColor16M*/); 
		
            if( inBitmapErr == KErrNone )
            {
		        // fill image from rgb buffer to input bitmap buffer 
		        TPtr8 linePtr(0,0); 
        		TInt lineLength = inBitmap->ScanLineLength(originalResolution.iWidth, iThumbDisplayMode); 
		        for(j=0, i=0; j<originalResolution.iHeight; j++, i+=lineLength)
		        {
        			linePtr.Set(iRgbBuf+i, lineLength, lineLength);
		        	inBitmap->SetScanLine((TDes8&)linePtr,j); 
		        }
		
        		// create output bitmap 
		        TRAPD(outBitmapErr, iOutBitmap = new (ELeave) CFbsBitmap);
                if( outBitmapErr == KErrNone )
                {
		            outBitmapErr = iOutBitmap->Create(iOutputThumbResolution, iThumbDisplayMode/*EColor16M*/); // same size as input frame
		
                    if( outBitmapErr == KErrNone )
                    {
		                // post-processing enhancement 
		                TRAP(outBitmapErr, EnhanceThumbnailL((const CFbsBitmap*)inBitmap, (CFbsBitmap*)iOutBitmap));

                    }
                    else
                    {
                        delete inBitmap; inBitmap = 0;   
                        delete iOutBitmap; iOutBitmap = 0;
                        HandleThumbnailError(outBitmapErr);
                        return;
                    }
                }
                else
                {
                     delete inBitmap; inBitmap = 0;
                     HandleThumbnailError(outBitmapErr);
                     return;
                }
            }
            else
            {
                delete inBitmap; inBitmap = 0;
                HandleThumbnailError(inBitmapErr);
                return;                
            }
		
		    // delete input bitmap 
		    delete inBitmap;
		    inBitmap = 0;
        }
        else
        {
            HandleThumbnailError(inBitmapErr);
            return;
        }
	}

    delete iRgbBuf;
    iRgbBuf = 0;
    FLOG(_L("CHXTNEVideoClipInfoImp::GenerateThumbNail out"));
}


TBool CHXTNEVideoClipInfoImp::HandleThumbnailError(TInt aError)
{
    if (aError != KErrNone)
    {
    	m_LastError = aError;
   
        return ETrue;
    }
       
    return EFalse;    
}


void CHXTNEVideoClipInfoImp::EnhanceThumbnailL(const CFbsBitmap* aInBitmap, 
											CFbsBitmap* aTargetBitmap) 
{
    FLOG(_L("CHXTNEVideoClipInfoImp::EnhanceThumbnailL in"));
  // create enhancement object
	if(!iEnhancer)
    iEnhancer = (CDisplayChain*) CDisplayChain::NewL();

	// enhance image
	iEnhancer->ProcessL(aInBitmap, aTargetBitmap); 

	// clear enhancement object
	delete iEnhancer;
	iEnhancer=0;
	FLOG(_L("CHXTNEVideoClipInfoImp::EnhanceThumbnailL out"));

}
