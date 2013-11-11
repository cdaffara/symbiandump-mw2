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



#ifndef _CHXCTneServer_H_
#define _CHXCTneServer_H_

#include"HXTneclientservercommon.h"
#include <hxtnutil.h>
#include <hxtnutil_obs.h>

#include <TNEVideoClipInfo.h>
#include <e32base.h>
#include <e32svr.h>
#include <e32uid.h>
#include <e32std.h>
#include <coemain.h>
#include <ecom/ecom.h>


#include "yuv2rgb12.h"
#include "yuv2rgb16.h"
#include "yuv2rgb24.h"


//////////////////////Server class/////////////////
class CTneServer : public CServer2
{

public:
CSession2* NewSessionL(const TVersion& aVersion,const RMessage2& aMessage) const;
public :
      // The thread function executed by the server
    static TInt ThreadFunction(TAny* aStarted);
     
protected:
    CTneServer(CActive::TPriority aActiveObjectPriority);
    };


/////////////////Session class///////////////////////////

class CTneSession : public CSession2,public CActive, public MHXThumbnailUtilityImplObserver
    
    {
public:
    CTneSession();
    ~CTneSession();

      //service request
    void ServiceL(const RMessage2& aMessage);
    void DispatchMessageL(const RMessage2& aMessage);
	// void InitL();  we are not using this rite now
	
	 // observer interface from MHXThumbnailUtilityImplObserver
    void MetaDataReady(TInt aError);
    void PacketReady(TInt aError, void *pData, TUint32 aDataSize);  
    void EndOfPackets();
   
    // CActive methods
     virtual void RunL();
     virtual void DoCancel();
     
protected:
     
    void    CompleteRequest(const RMessage2& aMessage, TInt aError);    // Completes the RMessage corresponding to Getthumb
    void    FetchBasicMetaData();               // Extract metadata from Util
    void    DoGetThumb();                           // Calls Notification of Thumbpending to client on Clients request to Get thumb
    TBool   IsGoodFrame(TUint8* aYUVDataPtr);  // checks on the value of each frame wrt  width and height 
    void    StopServer();
    
    void NotifyIfGetThumbPending(TInt aError, TUint8 *&pYUVBuffer); // Notifies client of thumb pending 
    void NotifyIfGetMetaDataPending(TInt aError); // Notifies client of metadata pending 
    TInt ReOpenFile(RFile &aFileHandle); // Called in case the clip is required to be open not from 0 index but from middle of clip
    TInt DoOpenFile(RFile &aFileHandle, TUint uStartTime);
    TInt GetStartingTime(TUint &uStartingTime);
    void CompleteCancelRequest(); // Completes the Rmessage corresponding to GetCancel
    
private:
    
        enum EThumbnailUtilState 
        {
                EStartGettingMetadata = 0,        
                EStartGettingThumbNail,
                EStartGettingThumbNailWithIndex,        
                ECancelling,
                ENotReady
           };
        
        TInt                                         iWidth;
        TInt                                         iHeight;    
        TUint                                        iDuration;
        TInt64 			                             iFrameCount;
                                               
      //   Variable to handle error during the state transitions in server
        TInt				                         m_LastError;
        TUint				                         m_uPacketsReceived;
                                               
                                               
        // will be set to true if got the good thumb or an error has occured.
        TBool 			                             m_bDone;
        //file server                          
        RFs*	                                     iFs;       
                                               
                                               
        // Concatenated YUV data for decoded frame
        TUint8* 		                             iYUVBuffer;      
        TUint8**                                     iClientYUVBufferPtrPtr;
        TBool		                                 iReOpenFileLPending;
        TBool                                        iGetThumbPending;
                                               
        TBool                                        iCloseHandle;
        TBool                                        ibOpenFilePending;
        TInt                                         iThumbIndex;
                                              
        RMessage2	                                 iClientRequest;   // This message will handle Open related request. 
        RMessage2	                                 iCancelRequest;   // This message will handle Cancel request
        RMessage2                                    iMetaDataRequest; // This message will handle MetaData Request
        RMessage2                                    iThumbnailRequest;// This message will handle thumbnail message

        // helix thumbnail utility	           
        CHXThumbnailUtility*         	             iUtil;
        EThumbnailUtilState                          m_State;  
        RFile                                        iFileHandle;

        TBool                                        m_bMetaDataReady;
        TUint                                        iPosition;

    };                                         
    
#endif // _CHXCTneServer_H_
