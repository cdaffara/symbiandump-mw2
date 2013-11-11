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
* Description:   TNE server
*
*/



#include"HXTneserver.h"
#include"HXTneclientservercommon.h"
#include <flogger.h>
#include <f32file64.h>
#include <e32math.h> // Sqrt
#include <stdlib.h> // abs
static const TInt kDefaultStack = 0x4000;



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




const TInt KMaxPacketToDecode = 160; // Many clips begin with several seconds of logos / MPAA rating which need to be skipped

//////////////Server///////////////


CTneServer::CTneServer(CActive::TPriority aActiveObjectPriority)
    : CServer2(aActiveObjectPriority)
    {
    }



CSession2* CTneServer::NewSessionL(const TVersion& /* aVersion */,const RMessage2& 
/*aMessage*/) const 

{
	 return new (ELeave) CTneSession;
	}
	
	
	
GLDEF_C  TInt CTneServer::ThreadFunction(TAny*  aName)
{
	HBufC   *pServerName = (HBufC *) aName;
	CTrapCleanup* cleanup = NULL;
	CActiveScheduler *pA = NULL;
	CTneServer *pS = NULL;
	TInt err = KErrNoMemory;

    FLOG(_L("CTneServer::ThreadFunction ServerName=%S"), pServerName); 		
	  // get clean-up stack
	if (pServerName)
	{
    		cleanup =CTrapCleanup::New(); 
    	 	pA = new CActiveScheduler;    
    		pS = new CTneServer(EPriorityHigh);
    		
    	 }
     
    
    if (cleanup && pA && pS )
   {
   		// OK
   		CActiveScheduler::Install(pA);      
      	// Start the server
    	err =  pS->Start(*pServerName);
  	}
  	
  	RThread::Rendezvous(err);
   	
    CActiveScheduler::Start();

      // Tidy up...    
    delete pS;
    delete pA;
    delete cleanup; 
    
    
    return(err);
}


//global function

TInt  StartThread(RThread& aServerThread,  HBufC *pServerName)
    {
    TInt res=KErrNone;
    
    
       res=aServerThread.Create(*pServerName,
            CTneServer::ThreadFunction,
            kDefaultStack, &User::Heap(),            
            (TAny *)pServerName  );
            
          // The thread has been created OK so get it started - however
          // we need to make sure that it has started before we continue.
        if (res==KErrNone)
            {
            TRequestStatus rendezvousStatus;
            
            aServerThread.SetPriority(EPriorityNormal);
            aServerThread.Rendezvous(rendezvousStatus);
            aServerThread.Resume();
            User::WaitForRequest(rendezvousStatus);
            res = rendezvousStatus.Int();
             }
            
          // The thread has not been created - clearly there's been a problem.
        else
            {
            aServerThread.Close();
             }
       
    return res;
    }




	
	////////////////////////ServerSession//////////////////////////////
	
	
CTneSession::CTneSession():CActive(EPriorityStandard),
        iWidth(0),                 
        iHeight(0) ,                                            
        iDuration(0)  ,                                        
        iFrameCount(0)    ,                                     
        m_LastError(KErrNone),                                          
        m_uPacketsReceived(0)    ,                                   
        m_bDone(EFalse),                                                      
        iFs(NULL),                                                    
        iYUVBuffer(NULL),
        iClientYUVBufferPtrPtr(NULL),                         
        iReOpenFileLPending(EFalse),                          
        iGetThumbPending(EFalse),                        
        iCloseHandle(EFalse),        	                        
        ibOpenFilePending(EFalse),  
        iThumbIndex(0),
        iUtil(NULL),                                                
        m_State( ENotReady),
        m_bMetaDataReady(EFalse),
        iPosition(0)
{
    CActiveScheduler::Add(this);
    FLOG(_L("CTneSession::CTneSession()in this=%x"), this);                      
    FLOG(_L("CTneSession::CTneSession()out this=%x"), this); 
}                           
                         
                         
                                     
   CTneSession::~CTneSession() 
{                          
	 FLOG(_L("CTneSession::~CTneSession()in this=%x"), this); 
	 
	 Cancel();
	 
     if(iYUVBuffer)        
     {    	               
        User::Free(iYUVBuffer);
        iYUVBuffer = NULL; 
	 }                     
	                       
	 if (iCloseHandle)     
	 {	                   
	    iFileHandle.Close();
	 }                     
	 if (iUtil)            
	 {                     
	    delete iUtil;      
	    iUtil = NULL;      
	 }                     
	 if(iFs)               
    {                      
	    iFs->Close();      
	    delete iFs;
	    iFs = NULL;
    }
	 CompleteRequest(iClientRequest,KErrCancel);
	 CompleteRequest(iMetaDataRequest,KErrCancel); 
	 CompleteRequest(iThumbnailRequest,KErrCancel); 
	 CompleteCancelRequest();	 
	 CActiveScheduler::Stop();
   
    FLOG(_L("CTneSession::~CTneSession()out this=%x"), this);                	
}


	/**
Services a client request.
*/
void CTneSession::ServiceL(const RMessage2& aMessage)
    {
        DispatchMessageL(aMessage); 
    }


void CTneSession::DispatchMessageL(const RMessage2& aMessage)
    {

   	TBool bCompleteRequest = EFalse;
   	TInt lError = KErrNone;
    RFile* pFileHandle;	
    TFileName *pFileName;
    TInt aPosition;
    TNEThumbRequest *pThumbRequestData;
    RFile64 aFilehandle;
    
    FLOG(_L("CTneSession::DispatchMessageL in type=%d"), aMessage.Function());
    
    switch (aMessage.Function())
        {        	        
    case  EOpenFileRFmsg:
        m_State = EStartGettingMetadata;
        bCompleteRequest = EFalse;
        CompleteRequest(iClientRequest,KErrNotReady); // Any previous pending request
        iClientRequest = aMessage;
     	pFileHandle = ( RFile* ) aMessage.Ptr0(); // Handle to read Message data     
     	aPosition  = (TInt  ) aMessage.Ptr1();
     	
     	lError = iFileHandle.Duplicate(*pFileHandle);
     	iPosition = aPosition;
     	     	
     	if (lError == KErrNone)
     	{
            iCloseHandle = ETrue;
            iStatus = KRequestPending;
            SetActive();
            TRequestStatus* pStatus = &iStatus;
            User::RequestComplete(pStatus, KErrNone);          
        }
        
        break;
        
//fix me - Avoid duplicate code -common func (Future Work)
    case  EOpenFIleNamemsg:
        iFs = NULL;
        m_State = EStartGettingMetadata;
        CompleteRequest(iClientRequest,KErrNotReady); // Any previous pending request
        iClientRequest = aMessage;
     	pFileName = (TFileName* ) aMessage.Ptr0();    	      
     	aPosition  = (TInt  ) aMessage.Ptr1();
     	
     	iFs = new RFs;
     	if(iFs == NULL)
     	{
     	    lError = KErrNoMemory;
     	    bCompleteRequest = ETrue;
     	}
     	else if ( (lError  = iFs->Connect())!= KErrNone)
     	{
     	    bCompleteRequest = ETrue;
     	}
     	else if ((lError = aFilehandle.Open(*iFs,*pFileName, EFileShareReadersOnly | EFileStream | EFileRead))!= KErrNone)
     	{
     	    bCompleteRequest = ETrue;
     	}
     	else if ((lError = iFileHandle.Duplicate(aFilehandle))!= KErrNone)
     	{
     	    bCompleteRequest = ETrue;
     	    aFilehandle.Close();
     	}
     	else
     	{
     	    bCompleteRequest = EFalse;
     	    aFilehandle.Close();
     	    iStatus = KRequestPending;
     	    iCloseHandle = ETrue;
     	    SetActive();
     	    TRequestStatus* pStatus = &iStatus;
     	    User::RequestComplete(pStatus, KErrNone);
     	}

        if (lError  !=  KErrNone)
        {
            bCompleteRequest = ETrue;
        }                        
        
        break;
      case EGetMetaDatamsg:
        
        CompleteRequest(iMetaDataRequest,KErrNotReady); // Any previous pending request
        iMetaDataRequest = aMessage;
        NotifyIfGetMetaDataPending(m_LastError);       
        break;
        
      case EGetThumbmsg:
        
        CompleteRequest(iThumbnailRequest,KErrNotReady); // Any previous pending request
        iThumbnailRequest = aMessage;
        pThumbRequestData  = ( TNEThumbRequest * ) aMessage.Ptr0();
        // store thumb request parameters       	    
        iClientYUVBufferPtrPtr  = &(pThumbRequestData->iYUVBuffer);        
        iThumbIndex = pThumbRequestData->iIndex;
        if (iThumbIndex == KBestThumbIndex || iThumbIndex <= 1)
        {
            m_State = EStartGettingThumbNail;
            DoGetThumb();
        }
        else
        {
            m_State = EStartGettingThumbNailWithIndex;
			if (!ibOpenFilePending)
			{
			    lError = ReOpenFile(iFileHandle);
			    if(lError)
			    {
			        m_LastError = lError;
			        CompleteRequest(iThumbnailRequest,m_LastError);
			    }
			}
			else
			{ // Previous thumbnail generation is in progress so cancel it	
			    if(iUtil)
			    {
			        iUtil->CancelThumb();
			    }
			    // Once control returns and if m_State is EStartGettingThumbNailWithIndex call ReOpenFile
			    iReOpenFileLPending = ETrue;
			}
		}
        break;
    case ECancelThumbmsg:
        FLOG(_L("CTneSession::DispatchMessageL cancellation ++"));
        iGetThumbPending = EFalse;
        iCancelRequest = aMessage;        
        m_State = ECancelling;               
        if(iUtil)
        {
        	iUtil->CancelThumb();
        }
        // cancel any pending getthumb or openfile request.
        lError = KErrCancel;
        CompleteRequest(iClientRequest,lError); 
        CompleteRequest(iThumbnailRequest,lError); 
        
        if (!ibOpenFilePending)
        {
           CompleteCancelRequest(); 
        }
        FLOG(_L("CTneSession::DispatchMessageL cancellation --"));

        break;    
    
   default:
      //  PanicClient(aMessage,EBadRequest);
        return;
        
      }
    
    // If any error occured during file opening
    if (bCompleteRequest)
    {
        CompleteRequest(iClientRequest,lError);
    }
	FLOG(_L("CTneSession::DispatchMessageL out type=%d"), aMessage.Function());
}



TInt CTneSession::DoOpenFile(RFile &aFileHandle, TUint uStartTime)
{
      FLOG(_L(" CTneSession::DoOpenFile in"));
        
   MHXThumbnailUtilityImplObserver*	pObserver = 
				(MHXThumbnailUtilityImplObserver *) this;
	
	ibOpenFilePending = ETrue;	
	TFileName aFilename;
	aFileHandle.FullName(aFilename);	
	 FLOG(_L("aFilename=%S"),&aFilename ); 	
	TRAPD(err, iUtil = CHXThumbnailUtility::NewL(*pObserver));
	   m_bDone = EFalse;
 	   m_uPacketsReceived = 0;
	if (err == KErrNone)
	{
        TRAP(err, iUtil->OpenFileL(aFileHandle, uStartTime));
    }
       FLOG(_L(" CTneSession::DoOpenFile  out err = %d"),err);
    ibOpenFilePending = EFalse;
    return err;
}

TInt CTneSession::ReOpenFile(RFile &aFileHandle)
{
	iGetThumbPending = ETrue; 
	TInt lError = KErrNone;
    TUint uStartingTime = 0;
    lError = GetStartingTime(uStartingTime);
    if (lError == KErrNone)
    {       
    	if (iUtil)
    	{	
    	    iUtil->CancelThumb();
    		delete iUtil;
    		iUtil = NULL;    						    
    	}
    	
    	iPosition = uStartingTime;
    	
    	if(!IsActive())   		   
    	{
            iStatus = KRequestPending;
            SetActive();
            TRequestStatus* pStatus = &iStatus;
            User::RequestComplete(pStatus, KErrNone);
    	}
    }
   
    return lError; 	
}



void CTneSession::FetchBasicMetaData()
{
    TUint lCount = 0;
    TUint i=0;
    TUint lFrameRateinSec  = 0;
    
    iUtil->GetMetaDataCount(lCount);
    
    TBool bGotFrameSize = EFalse, bGotDuration = EFalse, bGotFrameRate = EFalse;
    
    for (i=0; i<lCount; i++)
    {
        HBufC *pDes = NULL;           
        HXMetaDataKeys::EHXMetaDataId id;        
        iUtil->GetMetaDataAt(i, id, pDes);
        
        if (id == HXMetaDataKeys::EHXFrameSize && pDes)
        {
            TPtr pFrameSizePtr = pDes->Des();
            _LIT(KChar_x, "x");
            TInt xLoc = pFrameSizePtr.Find(KChar_x);
            if (xLoc != KErrNotFound)
            {            	            
            TLex lexWidth(pFrameSizePtr.Mid(0, xLoc));
            TLex lexHeight(pFrameSizePtr.Mid(xLoc+1));
            lexWidth.Val(iWidth);  // Storing into local iWidth variable
            lexHeight.Val(iHeight);  // Storing into local iHeight variable  
            bGotFrameSize = ETrue;
            FLOG(_L(" iWidth=%d,iHeight=%d"), iWidth,iHeight);
            }
        }
        else if (id == HXMetaDataKeys::EHXDuration && pDes)
        {
            TLex lexDuration(pDes->Des());
            lexDuration.Val(iDuration);
            bGotDuration = ETrue;
        }        
        else if (id == HXMetaDataKeys::EHXFramesPerSecond && pDes)
        {
        	TLex lexFramesPerSecond(pDes->Des());        	
            lexFramesPerSecond.Val(lFrameRateinSec);            
        	bGotFrameRate = ETrue;
        }
        
        if (bGotDuration && bGotFrameRate && bGotFrameSize)
        {        	
        	break;	
        }
        
    } // end of for
    
    if (bGotDuration && bGotFrameRate)
    {
    	// approximate frame count
    	// iDuration is in msec.
    	iFrameCount =   ( (iDuration+500)/1000 ) * lFrameRateinSec;
    }
    
    else
    {
        iFrameCount =   ( (iDuration+500)/1000 ) * 10;
    }

    
}


void CTneSession::DoGetThumb()
{
    iGetThumbPending = ETrue;
    if(m_bDone)
	{						
        if (m_LastError != KErrNone)
        {
            if (iYUVBuffer)
            {
                User::Free(iYUVBuffer);
		    	iYUVBuffer = NULL;
            }
        }
                                
		NotifyIfGetThumbPending(m_LastError, iYUVBuffer);
	}
}

void CTneSession::MetaDataReady(TInt aError)
{

    FLOG(_L("CTneSession::MetaDataReady in callbacks from Utility aError=%d"), aError);
    if ( m_State == EStartGettingMetadata)
    {    	    	   	    	    	 
	    // extract some minimum metadata to be used for thumbnail generation
	    if (aError == KErrNone)
	    {	    		    
		    FetchBasicMetaData();		    
		    if(iYUVBuffer)
		    {    	    
		    	User::Free(iYUVBuffer);
		    	iYUVBuffer = NULL;
			}
					
		    // Allocate memory for iYUVBuffer    
		    if( iWidth > 4 && iHeight > 4 ) // for all frame sizes 
		    {	    		   
			    TInt length = iWidth * iHeight;
			    length += (length>>1);    
			    iYUVBuffer = (TUint8*)User::Alloc(length);
			        
			    if (!iYUVBuffer)
			    {
			        FLOG(_L("CTneSession::MetaDataReady Error No memory for iYUVBuffer")); 
			    	aError = KErrNoMemory;    	
			    }
		    }
		    else
		    {
		    	FLOG(_L("CTneSession::MetaDataReady Error Width/Height not found"));                	
		    	aError = KErrNotSupported;	
		    	
		    }
    	}
	    m_LastError = aError;
	    
	    // only complete the request if there is any error. If the request is not completed here then 
	    // it will be completed after the DoOpenFile() returns.	 
	    if (aError != KErrNone)
	    {
	     	CompleteRequest(iClientRequest,aError);
	    }
	   
    }
    FLOG(_L("CTneSession::MetaDataReady out aError=%d"), aError);      
    m_bMetaDataReady = ETrue;    
    
    NotifyIfGetMetaDataPending(m_LastError);       
}


void CTneSession::PacketReady(TInt aError, 
                void *pData, TUint32 aDataSize)
{	
    FLOG(_L("CTneSession::PacketReady aError=%d m_State=%d aDataSize=%d"), aError, m_State, aDataSize);
    if(aDataSize > (iWidth*iHeight*3/2 ))
    {
	aDataSize  = iWidth*iHeight*3/2 ;  //restore back to values coming from header if they differ 
    	
      }
    if (m_State == ECancelling)
    {
        FLOG(_L("CTneSession::PacketReady no op"));
        // Calling cancel thumbnail
        if(iUtil)
        {
            iUtil->CancelThumb();
        }
        
        return;
    }
    if(aDataSize < (iWidth*iHeight*3/2 ))  // check to avoid getting very low size
    {
    	if(m_uPacketsReceived == 0)
    	{
    		m_LastError = KErrNotSupported ;  // if this is the first packet then only send KErrNotSupported		
    	}
    	
    	m_bDone = ETrue;
    }
    
    if (m_LastError != KErrNone || aError != KErrNone || pData == NULL) 
    {
        FLOG(_L("CTneSession::PacketReady aError=%d m_State=%d pData=%x"), aError, m_State, pData);
        // MetadataReady might set m_LastError                    
		m_bDone = ETrue;
		
		if (m_LastError == KErrNone)
		{
		    if (aError == KErrNone)
		    {
		        m_LastError = KErrNotSupported;
		    }
		    else
		    {
		        m_LastError = aError;
		    }
		}				
	}
	else			
	{		
		m_uPacketsReceived++;
		if ( IsGoodFrame((TUint8*)pData )  && !m_bDone )
		{
			m_bDone = ETrue;
			m_LastError = KErrNone;	
	    	Mem::Copy(iYUVBuffer, pData, aDataSize);	
		}
		else if (m_uPacketsReceived == 1) // we copy the first packet
		{
		    Mem::Copy(iYUVBuffer, pData, aDataSize);
		}
		else if (m_uPacketsReceived >= KMaxPacketToDecode)
		{		
			// we decoded upto KMaxPacketToDecode and haven't found
			// a good frame yet.
			FLOG(_L("CHXTNEVideoClipInfoImp::PacketReady max count reached %d"), 
					m_uPacketsReceived);
					Mem::Copy(iYUVBuffer, pData, aDataSize);	// we  copy and use the last frame 
					m_bDone = ETrue;
					m_LastError = KErrNone;
		}
	}
    
	
	// we are done. Tell the TNUtil that no more packets are needed.
	if (m_bDone)
	{
		iUtil->CancelThumb();
	}
	
	if (iGetThumbPending)
	{		
				
		// we are done either success or failure.
		if(m_bDone)
		{						
            if (m_LastError != KErrNone)
            {
                if (iYUVBuffer)
                {
                    User::Free(iYUVBuffer);
		    	    iYUVBuffer = NULL;
                }
            }
                                    
			NotifyIfGetThumbPending(m_LastError, iYUVBuffer);
		}
	}
	
	
}

void CTneSession::EndOfPackets()
{	
	if (!m_bDone)
		{	 
			if(m_uPacketsReceived >= 1)
				{
	 				 NotifyIfGetThumbPending(KErrNone, iYUVBuffer);
				 }   
			else
				{
				  	TUint8*     pYUVBuffer = NULL;
  					NotifyIfGetThumbPending(KErrNotFound, pYUVBuffer);
				}
			// If end of packet has been received then thumbnailengine should be done
			m_bDone = ETrue;
		}        

	if(!m_bMetaDataReady)
        {
            MetaDataReady(KErrCompletion);
        }
}

/* Determines whether a frame contains sufficient data to be considered an useful frame.
 * The logic scans a set of pixels to confirm:
 * too bright or too dark pixels are not taken into account. Only mid brightness pixels will be considered
 * if less than minSamples valid samples the frame is rejected. 
 * if the mode is more frequent than maxModePrct% the frame is rejected. This is useful to reject the MPAA rating or other simple logos
 * if the contrast is too low (max - min) frame is rejected
 * if the standard deviation is too low frame is rejected.
 */

TBool CTneSession::IsGoodFrame(TUint8* aYUVDataPtr)
{
 	TInt i;
    TInt ySize = iWidth*iHeight; 
    TUint8 * resetYUVPtr = aYUVDataPtr;

    // only these luminances are taken into account
    TInt minSamples = 5;
    TInt tooDark = 30;
    TInt tooBright = 240;

    // Defines how much data we analyze from the image to analyze its validity
    TInt pixelSkips = iHeight;  // scans about 1 to 3 pixels per line depending on aspect ratio 

    // average luminance profiling
    TInt runningSum = 0;
    TInt numberOfSamples = 0;
    TInt averageValue = 0;

    // contrast profiling
    TInt minValue = 255;
    TInt maxValue = 0;
    TInt minMaxDeltaThreshold = 20;

    // mode profiling
    int mode = 0;
    int modeSamples = 0;
    int histogram[255] = {0};
    int maxModePrct = 69;

    // standard deviation profiling
    TInt minStdDeviation = 5;
    TUint32 residualsum = 0;
    TReal stdDeviation = 0;

    // Exit value
    TBool goodFrame = ETrue;

    for(i=0, numberOfSamples=0; i<ySize; i+=pixelSkips, aYUVDataPtr+=pixelSkips)
    {
        if ( (*aYUVDataPtr>tooDark) && (*aYUVDataPtr<tooBright) )
        {
            runningSum += *aYUVDataPtr;
            if(*aYUVDataPtr > maxValue)
                maxValue = *aYUVDataPtr;
            if(*aYUVDataPtr < minValue)
                minValue = *aYUVDataPtr;
            histogram[*aYUVDataPtr]++;
            numberOfSamples++;
        }
    }

    if (numberOfSamples < minSamples)
    {
        //FLOG(_L("CTneSession::IsGoodFrame too few good samples"));
        goodFrame = EFalse;
    }
    else
    {
        // Find the mode
        for (i=0; i<255; i++)
        {
            if (histogram[i] > modeSamples)
            {
                modeSamples = histogram[i];
                mode = i;
            }
        }
        // Add the mode and most immediate values, as compression may add artifacts that disperse its value
        for (i = mode-2, modeSamples = 0; i < mode+3; i++)
        {
            modeSamples += histogram[i];
        }
    
        if (modeSamples * 100 / numberOfSamples > maxModePrct)
        {
            //FLOG(_L("Mode (%d) in over %d%% of the image\n", mode, modeSamples * 100 / numberOfSamples);
            goodFrame = false;
        }
        else 
        {
            averageValue = runningSum / numberOfSamples;
            // Rescan the frame now that we the average value is known
            aYUVDataPtr = resetYUVPtr;
    
    
            // Calculate the sum of residuals: (pixel - avgpixel)^2
            for(i=0; i<ySize; i+=pixelSkips, aYUVDataPtr+=pixelSkips)
            {
                if ( (*aYUVDataPtr>tooDark) && (*aYUVDataPtr<tooBright) )
                {
                    residualsum += (*aYUVDataPtr - averageValue) * (*aYUVDataPtr - averageValue);
                }
            }
    
            // Get the standard deviation
            Math::Sqrt(stdDeviation , residualsum / numberOfSamples);
    
            if (stdDeviation < minStdDeviation)
            {
                //FLOG(_L("CTneSession::IsGoodFrame too low StdDeviation: %f"), stdDeviation);
                goodFrame = EFalse;
            }
            else if((maxValue - minValue) < minMaxDeltaThreshold)
            {
                //FLOG(_L("CTneSession::IsGoodFrame too little difference between brightest and darkest pixel"));
                goodFrame = EFalse;
            }
        }
    }

    return goodFrame;
}

TInt CTneSession::GetStartingTime(TUint &uStartingTime)
{	
	TInt err = KErrNone;
	
	if (iThumbIndex <= iFrameCount)
	{
		TReal tempDuration = iDuration;
		uStartingTime = (tempDuration/iFrameCount) * iThumbIndex;
		// thumbnail beyond the clip duration.
		if (uStartingTime >= iDuration)
		{
			uStartingTime = 0;
		}
	}
	else
	{
		err = KErrNotSupported;		
	}
	FLOG(_L("CTNEVideoClipInfo::GetStartingTime StartingTime=%d iDuration=%d iFrameCount=l:%d h:%d iThumbIndex=%d err=%d"), 
		 uStartingTime, iDuration, I64LOW(iFrameCount),I64HIGH(iFrameCount), iThumbIndex, err);
	return err;
}



void CTneSession::CompleteRequest(const RMessage2& aMessage, TInt aError)
{
     if(!aMessage.IsNull())
     {
         aMessage.Complete(aError);
     }
}




// ownership of pBitMap will be passed to Observer
void CTneSession::NotifyIfGetThumbPending(TInt aError, TUint8 *&pYUVBuffer) 
{
	if (iGetThumbPending && !iThumbnailRequest.IsNull())
	{		
		iGetThumbPending = EFalse;        
        *iClientYUVBufferPtrPtr = pYUVBuffer;
        iThumbnailRequest.Complete(aError);
    
	}	
}

void CTneSession::NotifyIfGetMetaDataPending(TInt aError)
{
    if (m_bMetaDataReady && !iMetaDataRequest.IsNull())
    {   
        TNEMetaData* pMetaData;
        pMetaData = ( TNEMetaData* ) iMetaDataRequest.Ptr0();
        pMetaData->iWidth = iWidth;
        pMetaData->iHeight = iHeight;
        pMetaData->iFrameCount = iFrameCount;
        iMetaDataRequest.Complete(aError);        
    }
    
}

void CTneSession::CompleteCancelRequest()
{
    FLOG(_L("CTneSession::CompleteCancelRequest ++"));

     if (!iCancelRequest.IsNull())
     {
      
        if (iUtil)
        {
            delete iUtil;
            iUtil = NULL;
        }
        iCancelRequest.Complete(KErrNone);
        FLOG(_L("CTneSession::CompleteCancelRequest message sent "));
     }   
     FLOG(_L("CTneSession::CompleteCancelRequest --"));

}
// Open the file
void CTneSession::RunL()
{
    TInt lError = KErrNone;
    m_bDone = EFalse;
    m_uPacketsReceived = 0;
    FLOG(_L("CTneSession::RunL ++"));

    lError = DoOpenFile(iFileHandle, iPosition);
    
    if(lError)
    {
        if(iUtil)
        {
            iUtil->CancelThumb();
            delete iUtil;
            iUtil = NULL;
        }
    }
    
    CompleteRequest(iClientRequest,lError); // Complete client request 
    ibOpenFilePending = EFalse;
    // If any cancel call is pending
    CompleteCancelRequest();
    
    // This mean App has requested thumbnail based on index when last file opening was still in progress
    if(iReOpenFileLPending)
    {
      lError = ReOpenFile(iFileHandle);
      // If some error sent notification to client
      if(lError)
      {
          m_LastError = lError;
          CompleteRequest(iThumbnailRequest,m_LastError);
      }
      iReOpenFileLPending = EFalse;
    }
    FLOG(_L("CTneSession::RunL -- "));
}

// Cancel file open and pending thumbnail 
void CTneSession::DoCancel()
{
    CompleteRequest(iClientRequest,KErrCancel); // Complete any request
    CompleteRequest(iMetaDataRequest,KErrCancel); // Complete any request 
    CompleteRequest(iThumbnailRequest,KErrCancel); // Complete any request 
    ibOpenFilePending = EFalse;
    iReOpenFileLPending = EFalse;
    iGetThumbPending = EFalse;
    // If any cancel call is pending
    CompleteCancelRequest();

}
