/*
* Copyright (c) 2010 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  Client-Server communication
*
*/

#include "posindicatorhelper.h"
#include "posindicatorhelperserver.h"
#include "posindhelperclientrequester.h"    
#include "posindicatorhelpererrors.h"
#include "posindinfochangeobserver.h"

#include <e32std.h>
#include <s32strm.h>
#include <s32mem.h>

#include <epos_poslandmarkserialization.h>

// ---------------------------------------------------------------------------
// RPosIndicatorHelper::RPosIndicatorHelper()
//
// Default constructor.
// ---------------------------------------------------------------------------
// 
 RPosIndicatorHelper::RPosIndicatorHelper()
     : iClientRequester(NULL),
       iLmkBuf(NULL),
       iLandmark(NULL),
       iCoordinateSettingValue( ELocCoordinatesNotHidden )
  {     
     FUNC("+ RPosIndicatorHelper::RPosIndicatorHelper");
     FUNC("- RPosIndicatorHelper::RPosIndicatorHelper");
  }

// ---------------------------------------------------------------------------
// RPosIndicatorHelper::~RPosIndicatorHelper
//
// Destructor.
// ---------------------------------------------------------------------------
// 	
 RPosIndicatorHelper::~RPosIndicatorHelper()
  {  
     FUNC("+ RPosIndicatorHelper::~RPosIndicatorHelper");

  if(iClientRequester)
      {
      delete iClientRequester;
      iClientRequester = NULL;
      }   
  if(iLmkBuf)
      {
      delete iLmkBuf;
      iLmkBuf = NULL;
      }
  if(iLandmark)
      {
      delete iLandmark;
      iLandmark = NULL;
      }
  FUNC("- RPosIndicatorHelper::~RPosIndicatorHelper");
  }


// ---------------------------------------------------------------------------
// RPosIndicatorHelper::ConstructL
//Symbian 2nd phase constructor can leave.
// ---------------------------------------------------------------------------
//     
void RPosIndicatorHelper::ConstructL()
    {  
    FUNC("+ RPosIndicatorHelper::ConstructL");
    iClientRequester = CPosIndHelperClientRequester::NewL(*this);
    iLandmark = NULL;
    CheckHideCoordinateL();
    FUNC("- RPosIndicatorHelper::ConstructL");
    }


// ---------------------------------------------------------
// RPosIndicatorHelper::Open
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
 TInt RPosIndicatorHelper::Open( RPosIndicatorHelperServer& aServer,
                MPosIndInfoChangeObserver* aPosIndInfoChangeObserver )
    {
     FUNC("+ RPosIndicatorHelper::Open");

    __ASSERT_ALWAYS( aPosIndInfoChangeObserver , User::Panic  (KPosIndHelperClientPanicCategory, EPosIndicatorHelperServerBadHandle));
     iPosIndInfoChangeObserver = aPosIndInfoChangeObserver;

    __ASSERT_ALWAYS(aServer.Handle() != 0,
		User::Panic(KPosIndHelperClientPanicCategory, EPosIndicatorHelperServerBadHandle));
		
	__ASSERT_ALWAYS(! SubSessionHandle() , User::Panic  (KPosIndHelperClientPanicCategory, EPosIndicatorHelperServerBadHandle));
	TRAPD(ret, ConstructL());
	if (ret == KErrNone)
		{
		ret = CreateSubSession(aServer, ESubSessionOpen, TIpcArgs());
		}
	return ret;     
    }

// ---------------------------------------------------------
// RPosIndicatorHelper::Close
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
 void RPosIndicatorHelper:: Close()
    {
    FUNC("+ RPosIndicatorHelper::Close");

	TBool isActive = iClientRequester->IsActive();
    TInt status = iClientRequester->iStatus.Int();
    
	if( isActive && status == KRequestPending )
	    {
	    User::Panic(KPosIndHelperClientPanicCategory, EPosIndicatorHelperRequestsNotCancelled );
	    }
	
   if(SubSessionHandle())
        {
        CloseSubSession(ESubSessionClose);
        }
   
    FUNC("- RPosIndicatorHelper::Close");
    }


// ---------------------------------------------------------------------------
// RPosIndicatorHelper::RequestPosInfo()
// ---------------------------------------------------------------------------
void RPosIndicatorHelper::RequestPosInfo()
    {
    FUNC("+ RPosIndicatorHelper::RequestPosInfo");

    __ASSERT_ALWAYS(SubSessionHandle() , User::Panic  (KPosIndHelperClientPanicCategory, EPosIndicatorHelperServerBadHandle));
    
    if(iClientRequester->IsActive())
            return;				// If a previous request for position is alive, ignore this call
    
    TPckg<TPositionSatelliteInfo> posSatPckg( iPositionInfo );
    TIpcArgs args;
    args.Set(KParamCurrentPosition, &posSatPckg);
    
    iClientRequester->iStatus = KRequestPending;    
    SendReceive(EGetCurrentPosition, args , iClientRequester->iStatus);
    iClientRequester->Start(EGetCurrentPosition);
    FUNC("- RPosIndicatorHelper::RequestPosInfo");
    }

// ---------------------------------------------------------------------------
// RPosIndicatorHelper::GetAddressInfoSizePrivate()
// ---------------------------------------------------------------------------
void RPosIndicatorHelper::GetAddressInfoSize()
    {
    FUNC("+ RPosIndicatorHelper::RPosIndicatorHelper");
    
    // Create buffer and send message to resolve address
    TPckg<TInt> posSatPckg( iSize );
    TIpcArgs args;
    args.Set(KParamCurrentAddressSize ,&posSatPckg);
    
    iClientRequester->iStatus = KRequestPending;
    SendReceive(EGetCurrentAddressInfoSize, args, iClientRequester->iStatus);
    iClientRequester->Start(EGetCurrentAddressInfoSize);
    
    FUNC("- RPosIndicatorHelper::GetAddressInfoSize");
    }

// ---------------------------------------------------------------------------
// RPosIndicatorHelper::GetAddressL()
// ---------------------------------------------------------------------------
void RPosIndicatorHelper::GetAddressL()
    {  
    FUNC("+ RPosIndicatorHelper::RPosIndicatorHelper");
    TIpcArgs args;
    FUNC("    TIpcArgs args");
    iLmkBuf = HBufC8::NewL(iSize);      // Need to check iStatus
    FUNC("    HBufC8::NewLC");

    TPtr8 bufferPtr = iLmkBuf->Des();
    FUNC("    iLmkBuf->Des()");

    args.Set( KParamCurrentAddress , &bufferPtr);
    FUNC("    args.Set");

    iClientRequester->iStatus = KRequestPending;
    SendReceive(EGetCurrentAddressInfo, args, iClientRequester->iStatus);
    iClientRequester->Start(EGetCurrentAddressInfo);  
    
    FUNC("- RPosIndicatorHelper::GetAddressL");
    
    }

// ---------------------------------------------------------------------------
// RPosIndicatorHelper::CancelCurrentRequest()
// ---------------------------------------------------------------------------
void RPosIndicatorHelper::CancelCurrentRequest()
    {
    FUNC("+ RPosIndicatorHelper::CancelCurrentRequest");
    if (iClientRequester->IsActive())
        {
        SendReceive(ECancelCurrentOperation);
        iClientRequester->Cancel();
        }
    FUNC("- RPosIndicatorHelper::CancelCurrentRequest");
    }

// ---------------------------------------------------------------------------
// RPosIndicatorHelper::NotifyServerRequestCompletion()
// ---------------------------------------------------------------------------
void RPosIndicatorHelper::NotifyServerRequestCompletion(TServiceIds aServiceId, TInt aErrCode )
    {    
    FUNC("+ RPosIndicatorHelper::NotifyServerRequestCompletion");
    TInt err = aErrCode;
    switch( aServiceId )
        {
        case EGetCurrentPosition:
            {
            FUNC("case EGetCurrentPosition");

            //delete old landmark
            if(iLandmark)
                {
                delete iLandmark;
                iLandmark = NULL;
                }
            //if error in retrieval of coordinate info, notify client 
            if( err!=KErrNone )
                {
                LOG("Error in NotifyServerRequestCompletion : %d",err);
                //notify client about change in position information
                iPosIndInfoChangeObserver->NotifyPosInfoChange( iLandmark, err );
                return;
                }
            if( iCoordinateSettingValue == ELocCoordinatesNotHidden )
                {
                //add coordinate info to landmark object
                TRAP(err,            
                iLandmark = CPosLandmark::NewL();          
                TPosition position;
                iPositionInfo.GetPosition( position );
                iLandmark->SetPositionL( position );
                );           
                
                if(err == KErrNone)
                    {
                    LOG("No error in NotifyServerRequestCompletion : %d",err);
                    //notify client about change in position information
                    iPosIndInfoChangeObserver->NotifyPosInfoChange( iLandmark, err );    
                    }
                }
			GetAddressInfoSize();
            break;
            }
        case EGetCurrentAddressInfoSize:
            {
            FUNC("case EGetCurrentAddressInfoSize");

            //delete old landmark
            if(iLandmark)
                {
                delete iLandmark;
                iLandmark = NULL;
                }
            //if error in retrieval of coordinate info, notify client 
            if( err!=KErrNone )
                {
                //notify client about change in position information
                iPosIndInfoChangeObserver->NotifyPosInfoChange( iLandmark, err );
                break;
                }
            GetAddressL();
            break;
            }
        case EGetCurrentAddressInfo:
            {
            FUNC("case EGetCurrentAddressInfo");

            //delete old landmark
            if(iLandmark)
                {
                delete iLandmark;
                iLandmark = NULL;
                }
            //if error in retrieval of coordinate info, notify client 
            if( err!=KErrNone )
                {
                //notify client about change in position information
                iPosIndInfoChangeObserver->NotifyPosInfoChange( iLandmark, err );
                }
            
            //add coordinate info to landmark object
            TRAPD(err,            
            iLandmark = CPosLandmark::NewL();
            iLandmark = PosLandmarkSerialization::UnpackL(*iLmkBuf)
            );           
            
            if(err == KErrNone)
                {
                //notify client about change in position information
                iPosIndInfoChangeObserver->NotifyPosInfoChange( iLandmark, err );
                }
            break;
            }
        default:
            {            
            break;
            }            
        }
    FUNC("- RPosIndicatorHelper::NotifyServerRequestCompletion");
    }

void RPosIndicatorHelper::CheckHideCoordinateL()
    {
    FUNC("+ RPosIndicatorHelper::CheckHideCoordinateL");
    CRepository* repository = CRepository::NewL( TUid::Uid( KCRUidLocLocalVariation) );
    CleanupStack::PushL( repository );
    User::LeaveIfError( repository->Get( KLocHideCoordinates,
                                        iCoordinateSettingValue ));  
    CleanupStack::PopAndDestroy( repository );
    FUNC("+ RPosIndicatorHelper::CheckHideCoordinateL");
    }
	
