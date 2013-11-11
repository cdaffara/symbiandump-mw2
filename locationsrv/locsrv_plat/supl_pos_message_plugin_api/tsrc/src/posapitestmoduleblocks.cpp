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
* Description:  Implementation of the class CPosAPITestModule
*
*/



// INCLUDE FILES
#include <e32svr.h>
#include <StifParser.h> 
#include <Stiftestinterface.h>
#include <ecom/ecom.h> 

#include <epos_comasuplposhandlerbase.h>
#include <epos_comasuplpospayload.h>
#include <epos_comasuplinforequestlist.h>
#include <epos_comasuplposition.h>
#include <epos_comasuplvelocity.h>
#include <epos_comasuplsetcapabilities.h>
#include <epos_comasuplreqasstdata.h>
#include <epos_eomasuplposerrors.h>

#include "posapiasynchrequestor.h"
#include "PosAPITestModule.h"

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CPosAPITestModule::Delete
// Delete here all resources allocated and opened from test methods. 
// Called from destructor. 
// -----------------------------------------------------------------------------
//
void CPosAPITestModule::Delete() 
    {
		REComSession::FinalClose();
    }

// -----------------------------------------------------------------------------
// CPosAPITestModule::RunMethodL
// Run specified method. Contains also table of test mothods and their names.
// -----------------------------------------------------------------------------
//
TInt CPosAPITestModule::RunMethodL( 
    CStifItemParser& aItem ) 
    {

    static TStifFunctionInfo const KFunctions[] =
        {  
        // Copy this line for every implemented function.
        // First string is the function name used in TestScripter script file.
        // Second is the actual implementation member function. 
        ENTRY( "COMASuplInfoRequestList_NewL", CPosAPITestModule::COMASuplInfoRequestList_NewL ),
        ENTRY( "COMASuplInfoRequestList_Append", CPosAPITestModule::COMASuplInfoRequestList_AppendL ),
        ENTRY( "COMASuplInfoRequestList_GetCount", CPosAPITestModule::COMASuplInfoRequestList_GetCountL ),
        ENTRY( "COMASuplInfoRequestList_GetElement", CPosAPITestModule::COMASuplInfoRequestList_GetElementL ),
        
        ENTRY( "COMASuplInfoRequest_NewL", CPosAPITestModule::COMASuplInfoRequest_NewL),
        ENTRY( "COMASuplInfoRequest_Type", CPosAPITestModule::COMASuplInfoRequest_TypeL),
        ENTRY( "COMASuplInfoRequest_Status", CPosAPITestModule::COMASuplInfoRequest_StatusL),
        ENTRY( "COMASuplInfoRequest_CloneL", CPosAPITestModule::COMASuplInfoRequest_CloneL),
        ENTRY( "TOMASuplAltitudeInfo_Set_Get",CPosAPITestModule::TOMASuplAltitudeInfo_Set_Get),
        ENTRY( "TOMASuplUncertainty_Set_Get",CPosAPITestModule::TOMASuplUncertainty_Set_Get),
        ENTRY( "TOMASuplPositionEstimate_Set_Get",CPosAPITestModule::TOMASuplPositionEstimate_Set_Get),
        ENTRY( "TOMASuplPositionEstimate_Set_Get_Confidance_AltitudeInfo",CPosAPITestModule::TOMASuplPositionEstimate_Set_Get_Confidance_AltitudeInfo),
        ENTRY( "TOMASuplUtcTime_Set_Get",CPosAPITestModule::TOMASuplUtcTime_Set_Get),
        ENTRY( "COMASuplPosition_Set_Get",CPosAPITestModule::COMASuplPosition_Set_GetL),
        ENTRY( "COMASuplPosition_SetSuplVelocity",CPosAPITestModule::COMASuplPosition_SetSuplVelocityL),
        ENTRY( "COMASuplPosition_CloneL",CPosAPITestModule::COMASuplPosition_CloneL),
        ENTRY( "COMASuplPosPayload_SuplPayloadType",CPosAPITestModule::COMASuplPosPayload_SuplPayloadTypeL),
        ENTRY( "COMASuplPosPayload_CloneL",CPosAPITestModule::COMASuplPosPayload_CloneL),
        ENTRY( "COMASuplSetCapabilities_Set_Get",CPosAPITestModule::COMASuplSetCapabilities_Set_GetL),
        ENTRY( "COMASuplSetCapabilities_CloneL",CPosAPITestModule::COMASuplSetCapabilities_CloneL),
        
        ENTRY( "COMASuplVelocity_Set_Get",CPosAPITestModule::COMASuplVelocity_Set_GetL),
        ENTRY( "COMASuplVelocity_CloneL",CPosAPITestModule::COMASuplVelocity_CloneL),
        ENTRY( "COMASuplHorizVelocity_Get_Set_HorizVel",CPosAPITestModule::COMASuplHorizVelocity_Get_Set_HorizVelL),
        ENTRY( "COMASuplHorizAndVertVelocity_Set_GetL",CPosAPITestModule::COMASuplHorizAndVertVelocity_Set_GetL),
        ENTRY( "COMASuplHorizUncertVelocity_Set_GetL",CPosAPITestModule::COMASuplHorizUncertVelocity_Set_GetL),
        ENTRY( "COMASuplHorizAndVertUncertVelocity_Set_GetL",CPosAPITestModule::COMASuplHorizAndVertUncertVelocity_Set_GetL),
        
        ENTRY( "TOMASuplSatelliteInfoElement_Set_Get",CPosAPITestModule::TOMASuplSatelliteInfoElement_Set_Get),
        ENTRY( "TOMASuplNavigationModel_Set_GetSatInfoElement_AllElements",CPosAPITestModule::TOMASuplNavigationModel_Set_GetSatInfoElement_AllElements),
        ENTRY( "TOMASuplNavigationModel_Set_GetSatInfoElement_SpecificElement",CPosAPITestModule::TOMASuplNavigationModel_Set_GetSatInfoElement_SpecificElement),
        ENTRY( "TOMASuplNavigationModel_Set_GetNavigationModel",CPosAPITestModule::TOMASuplNavigationModel_Set_GetNavigationModel),
        ENTRY( "COMASuplReqAsstData_Set_GetNavigationDataL",CPosAPITestModule::COMASuplReqAsstData_Set_GetNavigationDataL),
        ENTRY( "COMASuplReqAsstData_Set_GetReqAsstDataL",CPosAPITestModule::COMASuplReqAsstData_Set_GetReqAsstDataL),
        ENTRY( "COMASuplReqAsstData_CloneL",CPosAPITestModule::COMASuplReqAsstData_CloneL),
        
		ENTRY( "COMASuplPosHandlerBase_InitializeL",CPosAPITestModule::COMASuplPosHandlerBase_InitializeL),
		ENTRY( "COMASuplPosHandlerBase_CancelInitializeL",CPosAPITestModule::COMASuplPosHandlerBase_CancelInitializeL),
		
		ENTRY( "COMASuplPosHandlerBase_CreateNewSessionL",CPosAPITestModule::COMASuplPosHandlerBase_CreateNewSessionL),
		ENTRY( "COMASuplPosHandlerBase_VersionL",CPosAPITestModule::COMASuplPosHandlerBase_VersionL),
		ENTRY( "COMASuplPosHandlerBase_ImplementationUID",CPosAPITestModule::COMASuplPosHandlerBase_ImplementationUIDL),
		
		
		ENTRY( "COMASuplPosSessionBase_InitializeL",CPosAPITestModule::COMASuplPosSessionBase_InitializeL),
		ENTRY( "COMASuplPosSessionBase_CancelInitializeL",CPosAPITestModule::COMASuplPosSessionBase_CancelInitializeL),
		ENTRY( "COMASuplPosSessionBase_HandleSuplPosMessageL",CPosAPITestModule::COMASuplPosSessionBase_HandleSuplPosMessageL),
		ENTRY( "COMASuplPosSessionBase_GetSuplInfoL",CPosAPITestModule::COMASuplPosSessionBase_GetSuplInfoL),
		ENTRY( "COMASuplPosSessionBase_CancelSuplInfoRequestL",CPosAPITestModule::COMASuplPosSessionBase_CancelSuplInfoRequestL),
		ENTRY( "COMASuplPosSessionBase_GetPositionL",CPosAPITestModule::COMASuplPosSessionBase_GetPositionL),
		ENTRY( "COMASuplPosSessionBase_CancelGetPositionL",CPosAPITestModule::COMASuplPosSessionBase_CancelGetPositionL),
		
		
		ENTRY( "COMASuplPosSessionBase_SuplStateL",CPosAPITestModule::COMASuplPosSessionBase_SuplStateL),
		ENTRY( "COMASuplPosSessionBase_SessionEndL",CPosAPITestModule::COMASuplPosSessionBase_SessionEndL),
		ENTRY( "COMASuplPosHandlerBase_CreateNewSession_SuplObserverL",CPosAPITestModule::COMASuplPosHandlerBase_CreateNewSession_SuplObserverL),
				
		ENTRY( "ExampleL",CPosAPITestModule::ExampleL),        
        };

    const TInt count = sizeof( KFunctions ) / 
                        sizeof( TStifFunctionInfo );

    return RunInternalL( KFunctions, count, aItem );

    }
    

// -----------------------------------------------------------------------------
// CPosAPITestModule::COMASuplInfoRequest_NewL
// 
// -----------------------------------------------------------------------------
//
TInt CPosAPITestModule::COMASuplInfoRequest_NewL( CStifItemParser& /*aItem*/ )
	{
		COMASuplInfoRequest* request = COMASuplInfoRequest::NewL();
		delete request;
		return KErrNone;
	}
	
// -----------------------------------------------------------------------------
// CPosAPITestModule::COMASuplInfoRequest_Type
// 
// -----------------------------------------------------------------------------
//
TInt CPosAPITestModule::COMASuplInfoRequest_TypeL( CStifItemParser& /*aItem*/ )
	{
		TInt err;
		COMASuplInfoRequest::TOMASuplInfoType infoType = COMASuplInfoRequest::EOMASuplReqAsstData;
		COMASuplInfoRequest* request = COMASuplInfoRequest::NewL();
		request->SetType(infoType);
		if(request->Type() == COMASuplInfoRequest::EOMASuplReqAsstData)
			{
				err = KErrNone;
			}
		else
			{
				err = KErrGeneral;
			}	
			
		delete request;
		return err;
	}
	
// -----------------------------------------------------------------------------
// CPosAPITestModule::COMASuplInfoRequest_Status
// 
// -----------------------------------------------------------------------------
//
TInt CPosAPITestModule::COMASuplInfoRequest_StatusL( CStifItemParser& /*aItem*/ )
	{
		TInt err;
		COMASuplInfoRequest* request = COMASuplInfoRequest::NewL();
		request->SetStatus(KErrOMASuplParamNotSet);
		if(request->Status() == KErrOMASuplParamNotSet)
			{
				err = KErrNone;
			}
		else
			{
				err = KErrGeneral;
			}	
			
		delete request;
		return err;
		
	}
	
// -----------------------------------------------------------------------------
// CPosAPITestModule::COMASuplInfoRequest_CloneL
// 
// -----------------------------------------------------------------------------
//
TInt CPosAPITestModule::COMASuplInfoRequest_CloneL( CStifItemParser& /*aItem*/ )
	{
		TInt err;
		COMASuplInfoRequest::TOMASuplInfoType infoType = COMASuplInfoRequest::EOMASuplReqAsstData;
		COMASuplInfoRequest* request = COMASuplInfoRequest::NewL();
		request->SetStatus(KErrOMASuplParamNotSet);
		request->SetType(infoType);

		COMASuplInfoRequest* cloneLRequest = (COMASuplInfoRequest*)request->CloneL();
		
		if(cloneLRequest->Status() == KErrOMASuplParamNotSet && cloneLRequest->Type() == COMASuplInfoRequest::EOMASuplReqAsstData)
			{
				err = KErrNone;
			}
		else
			{
				err = KErrGeneral;
			}	
			
		delete cloneLRequest;	
		delete request;
		return err;
		

	}

// -----------------------------------------------------------------------------
// CPosAPITestModule::ExampleL
// Example test method function.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TInt CPosAPITestModule::ExampleL( CStifItemParser& /*aItem*/ )
    {
    return KErrNone;
    }

// -----------------------------------------------------------------------------
// CPosAPITestModule::COMASuplInfoRequestList_NewL
// 
// -----------------------------------------------------------------------------
//
COMASuplInfoRequestList* CPosAPITestModule::COMASuplInfoRequestList_NewL()
	{
			return COMASuplInfoRequestList::NewL();
	}
	
// -----------------------------------------------------------------------------
// CPosAPITestModule::COMASuplSETCapabilities_NewL
// 
// -----------------------------------------------------------------------------
//
COMASuplSETCapabilities* CPosAPITestModule::COMASuplSETCapabilities_NewL()
	{
			COMASuplSETCapabilities* capabilitSet = COMASuplSETCapabilities::NewL();

			//Set the Postechnology
			TOMASuplPosTechnology postechnology;
			postechnology.SetPosTechnology(ETrue,ETrue,ETrue,ETrue,ETrue,ETrue);
			//Set the pos protocol
			TOMASuplPosProtocol posprotocol;	
			posprotocol.SetPosProtocol(ETrue,ETrue,ETrue);					
			//set the capability of SET
			capabilitSet->SetSETCapabilities(postechnology,
										COMASuplSETCapabilities::EOMANoPreferred,
										posprotocol);
			return 	capabilitSet;									
	}

// -----------------------------------------------------------------------------
// CPosAPITestModule::COMASuplPosition_NewL
// 
// -----------------------------------------------------------------------------
//
COMASuplPosition* CPosAPITestModule::COMASuplPosition_NewL()
	{
			COMASuplPosition* position = COMASuplPosition::NewL(); 
									       
			// Fill up aPosition with Position information
			TDateTime dateTime(2005,EDecember,9,21,57,43,1234);
			TOMASuplUtcTime utcTime;
			TInt zoneCode = 0;
			TInt xone = 23;
			TInt retValue=utcTime.SetUtcTime(dateTime, zoneCode, xone);
			TOMASuplPositionEstimate posEstimate;
			TInt latitude = 100;
			TInt longitude = 100;
			posEstimate.SetPositionEstimate(TOMASuplPositionEstimate::ENorth,
											 latitude, longitude);
			position->SetPosition(utcTime, posEstimate);	
			
			return position;

	}
	
// -----------------------------------------------------------------------------
// CPosAPITestModule::COMASuplInfoRequestList_NewL
// 
// -----------------------------------------------------------------------------
//
TInt CPosAPITestModule::COMASuplInfoRequestList_NewL( CStifItemParser& /*aItem*/ )
	{
			TInt err = KErrNone;
			COMASuplInfoRequestList* OMASuplInfoRequestList = COMASuplInfoRequestList_NewL();
			if(OMASuplInfoRequestList)
				{
					err = KErrNone;
				}
			else
				{
					err = KErrGeneral;	
				}
			
			delete 	OMASuplInfoRequestList;
			return err;			
	}
	
	
// -----------------------------------------------------------------------------
// CPosAPITestModule::COMASuplInfoRequestList_Append
// 
// -----------------------------------------------------------------------------
//
TInt CPosAPITestModule::COMASuplInfoRequestList_AppendL( CStifItemParser& /*aItem*/ )
	{
			COMASuplSETCapabilities* capabilitSet = COMASuplSETCapabilities_NewL();
			COMASuplPosition* position = COMASuplPosition_NewL();

			COMASuplInfoRequestList* OMASuplInfoRequestList = COMASuplInfoRequestList_NewL();
			OMASuplInfoRequestList->Append(capabilitSet);
			OMASuplInfoRequestList->Append(position);
			delete OMASuplInfoRequestList;
			return KErrNone;
	}


// -----------------------------------------------------------------------------
// CPosAPITestModule::COMASuplInfoRequestList_GetCount
// 
// -----------------------------------------------------------------------------
//
TInt CPosAPITestModule::COMASuplInfoRequestList_GetCountL( CStifItemParser& /*aItem*/ )
	{
			TInt err = KErrNone;
			COMASuplSETCapabilities* capabilitSet = COMASuplSETCapabilities_NewL();
			COMASuplPosition* position = COMASuplPosition_NewL();

			COMASuplInfoRequestList* OMASuplInfoRequestList = COMASuplInfoRequestList_NewL();
			OMASuplInfoRequestList->Append(capabilitSet);
			OMASuplInfoRequestList->Append(position);
			
			TInt cnt = OMASuplInfoRequestList->GetCount();
			if(cnt == 2)
				err = KErrNone;
			else
				err = KErrGeneral;
			
			delete OMASuplInfoRequestList;
			return err;
	}
	

// -----------------------------------------------------------------------------
// CPosAPITestModule::COMASuplInfoRequestList_GetElement
// 
// -----------------------------------------------------------------------------
//
TInt CPosAPITestModule::COMASuplInfoRequestList_GetElementL( CStifItemParser& /*aItem*/ )
	{
			TInt err = KErrNone;
			
			COMASuplSETCapabilities* capabilitSet = COMASuplSETCapabilities_NewL();
			COMASuplPosition* position = COMASuplPosition_NewL();

			COMASuplInfoRequestList* OMASuplInfoRequestList = COMASuplInfoRequestList_NewL();
			OMASuplInfoRequestList->Append(capabilitSet);
			OMASuplInfoRequestList->Append(position);
			
			COMASuplInfoRequest *reqElement0 = OMASuplInfoRequestList->GetElement(0);
			COMASuplInfoRequest *reqElement1 = OMASuplInfoRequestList->GetElement(1);
			COMASuplInfoRequest *reqElement4 = OMASuplInfoRequestList->GetElement(4); //Not Exist
			
			if(reqElement0 == capabilitSet && reqElement1 == position && reqElement4 == NULL)
				err = KErrNone;
			else
				err = KErrGeneral;
			
			delete OMASuplInfoRequestList;
			return err;
	}


// -----------------------------------------------------------------------------
// CPosAPITestModule::TOMASuplAltitudeInfo_Set_Get
// 
// -----------------------------------------------------------------------------
//
TInt CPosAPITestModule::TOMASuplAltitudeInfo_Set_Get( CStifItemParser& aItem )
	{

      TInt altitude= 123;		//Valid range is 0 to 32767
      TInt altUncertainty = 56; //Valid range is 0 to 127.
      
	  aItem.GetNextInt(altitude);
	  aItem.GetNextInt(altUncertainty);
	
	  // SetAltitudeInfo sets values for Altitude Information	  
	  TOMASuplAltitudeInfo suplAltitudeInfo;	  
      TInt retAltitudeInfo = suplAltitudeInfo.SetAltitudeInfo(TOMASuplAltitudeInfo::EHeight,altitude,altUncertainty);
      if(retAltitudeInfo!=KErrNone)
      	{
		  	return KErrArgument;
      	}
      	
      TOMASuplAltitudeInfo::TOMASuplAltitudeDirection recAltitudeDirection;
      TInt recAltitude;
      TInt recAltUncertainty;
      suplAltitudeInfo.GetAltitudeInfo(recAltitudeDirection,recAltitude,recAltUncertainty);

	if(recAltitudeDirection == TOMASuplAltitudeInfo::EHeight && altitude ==  recAltitude && altUncertainty ==recAltUncertainty)
		return KErrNone;
	else
		return KErrGeneral;

	}

//#include <epos_comasuplposition.h>

// -----------------------------------------------------------------------------
// CPosAPITestModule::TOMASuplUncertainty_Set_Get
// 
// -----------------------------------------------------------------------------
//
TInt CPosAPITestModule::TOMASuplUncertainty_Set_Get( CStifItemParser& aItem )
	{

	  //SetUncertainty sets values for Uncertainty	  
	  TOMASuplUncertainty uncertainty;
	  TInt uncertaintySemiMajor; //Valid range is 0 to 127
	  TInt uncertaintySemiMinor; //Valid range is 0 to 127
	  TInt orientationMajorAxis; //Valid range is 0 to 180
	  
  	  aItem.GetNextInt(uncertaintySemiMajor);
	  aItem.GetNextInt(uncertaintySemiMinor);
	  aItem.GetNextInt(orientationMajorAxis);
	  
	  TInt retUncertainty = uncertainty.SetUncertainty(uncertaintySemiMajor,
	  									uncertaintySemiMinor,orientationMajorAxis);
	  if(retUncertainty!=KErrNone)
	  	{	  	 
	  	 	return KErrArgument;
	  	}
	
		  	
	TInt recUncertaintySemiMajor;
    TInt recUncertaintySemiMinor;
	TInt recOrientationMajorAxis;
	
	uncertainty.GetUncertainty(recUncertaintySemiMajor,recUncertaintySemiMinor,recOrientationMajorAxis);
	
	if(uncertaintySemiMajor == recUncertaintySemiMajor && uncertaintySemiMinor ==  recUncertaintySemiMinor && orientationMajorAxis == recOrientationMajorAxis)
		return KErrNone;
	else
		return KErrGeneral;

	}

//#include <epos_comasuplposition.h>
// -----------------------------------------------------------------------------
// CPosAPITestModule::TOMASuplPositionEstimate_Set_Get
// 
// -----------------------------------------------------------------------------
//
TInt CPosAPITestModule::TOMASuplPositionEstimate_Set_Get( CStifItemParser& aItem )
	{

	  //SetUncertainty sets values for Uncertainty	  
	  TOMASuplPositionEstimate posestimate;
	  TOMASuplUncertainty uncertainty;
	  TInt uncertaintySemiMajor; //Valid range is 0 to 127
	  TInt uncertaintySemiMinor; //Valid range is 0 to 127
	  TInt orientationMajorAxis; //Valid range is 0 to 180

  	  aItem.GetNextInt(uncertaintySemiMajor);
	  aItem.GetNextInt(uncertaintySemiMinor);
	  aItem.GetNextInt(orientationMajorAxis);
	  
	  TInt retUncertainty = uncertainty.SetUncertainty(uncertaintySemiMajor,
	  									uncertaintySemiMinor,orientationMajorAxis);
	  if(retUncertainty!=KErrNone)
	  	{	  	 
	  	 	return KErrArgument;
	  	}
	

	TInt retSetUncertainty = posestimate.SetUncertainty(uncertainty);		  	
	if(retSetUncertainty!=KErrNone)
	{
		return KErrArgument;
	}


	TOMASuplUncertainty recuncertainty;
	
	TInt recUncertaintySemiMajor;
    TInt recUncertaintySemiMinor;
	TInt recOrientationMajorAxis;
	
	TInt err = posestimate.GetUncertainty(recuncertainty);
	
	if(err != KErrNone )
		{
			return KErrGeneral;
		}
	
	uncertainty.GetUncertainty(recUncertaintySemiMajor,recUncertaintySemiMinor,recOrientationMajorAxis);
	
	if(uncertaintySemiMajor == recUncertaintySemiMajor && uncertaintySemiMinor ==  recUncertaintySemiMinor && orientationMajorAxis == recOrientationMajorAxis)
		return KErrNone;
	else
		return KErrGeneral;

	}

//#include <epos_comasuplposition.h>

// -----------------------------------------------------------------------------
// CPosAPITestModule::TOMASuplPositionEstimate_Set_Get_Confidance_AltitudeInfo
// 
// -----------------------------------------------------------------------------
//
TInt CPosAPITestModule::TOMASuplPositionEstimate_Set_Get_Confidance_AltitudeInfo( CStifItemParser& aItem )
	{
	 
	  TOMASuplPositionEstimate posEstimate;
	  TInt confidence ;//valid range is 0 to 100.
	  aItem.GetNextInt(confidence);
	  TInt retSetConfidence = posEstimate.SetConfidence(confidence);
	  if(retSetConfidence!=KErrNone)
	  	{
	
	  	return KErrArgument;
	  	}
	  	
   	  // SetAltitudeInfo sets values for Altitude Information	  
	  TOMASuplAltitudeInfo suplAltitudeInfo;	  
      TInt altitude;		//Valid range is 0 to 32767
      TInt altUncertainty ; //Valid range is 0 to 127.
      
      aItem.GetNextInt(altitude);
      aItem.GetNextInt(altUncertainty);
      
      TInt retAltitudeInfo = suplAltitudeInfo.SetAltitudeInfo(TOMASuplAltitudeInfo::EHeight,altitude,altUncertainty);
      if(retAltitudeInfo!=KErrNone)
      	{
		  	return KErrArgument;
      	}
	  
      TInt retFinalAltitudeInfo = posEstimate.SetAltitudeInfo(suplAltitudeInfo);
      
      if(retFinalAltitudeInfo!=KErrNone)
      	{
		  	return KErrGeneral;
      	} 
      	
	TOMASuplAltitudeInfo recsuplAltitudeInfo;	  
	retFinalAltitudeInfo = posEstimate.GetAltitudeInfo(recsuplAltitudeInfo);
      
      if(retFinalAltitudeInfo!=KErrNone)
      	{
		  	return KErrGeneral;
      	} 
		
      TOMASuplAltitudeInfo::TOMASuplAltitudeDirection recAltitudeDirection;
      TInt recAltitude;
      TInt recAltUncertainty;
      recsuplAltitudeInfo.GetAltitudeInfo(recAltitudeDirection,recAltitude,recAltUncertainty);
	  TInt retConfidance;
	  posEstimate.GetConfidence(retConfidance);
	if(recAltitudeDirection == TOMASuplAltitudeInfo::EHeight && altitude ==  recAltitude && altUncertainty ==recAltUncertainty
	   	&& retConfidance == confidence)
		return KErrNone;
	else
		return KErrGeneral;
		
	}

//#include <epos_comasuplposition.h>
// -----------------------------------------------------------------------------
// CPosAPITestModule::TOMASuplUtcTime_Set_Get
// 
// -----------------------------------------------------------------------------
//
TInt CPosAPITestModule::TOMASuplUtcTime_Set_Get( CStifItemParser& aItem )
	{
	
      TOMASuplUtcTime utcTime;
	  TDateTime dateTime(2005,EDecember,5,2,11,13,16);
	  TInt zoneCode;
	  TInt xone;
	  TTime setTime(dateTime);
	  
	  aItem.GetNextInt(zoneCode);
	  aItem.GetNextInt(xone);
	  
	  TInt err = utcTime.SetUtcTime(dateTime, zoneCode, xone); 
	  
      if(err!=KErrNone)
      	{
		  	return KErrArgument;
      	} 

	  TDateTime recdateTime;
	  TInt reczoneCode;
	  TInt recxone;
	  utcTime.GetUtcTime(recdateTime,reczoneCode,recxone);
	  TTime recTime(recdateTime);
	  
	  if( setTime == recTime && zoneCode == reczoneCode && xone == recxone)
		return KErrNone;
	else
		return KErrGeneral;
	}


//#include <epos_comasuplposition.h>
// -----------------------------------------------------------------------------
// CPosAPITestModule::COMASuplPosition_Set_Get
// 
// -----------------------------------------------------------------------------
//
TInt CPosAPITestModule::COMASuplPosition_Set_GetL( CStifItemParser& aItem )
	{
	
	COMASuplPosition*  position  = COMASuplPosition::NewL();
	
	TOMASuplUtcTime utcTime;
	TOMASuplPositionEstimate posEstimate;
	
  	TDateTime dateTime(2005,EDecember,9,21,57,43,1234);
  	TTime setTime(dateTime);
  	TInt zoneCode ;
  	TInt xone ;
  	
	aItem.GetNextInt(zoneCode);
	aItem.GetNextInt(xone);

	TInt retValue=utcTime.SetUtcTime(dateTime, zoneCode, xone);
	
	TInt latitude; 
	TInt longitude; 
	
	aItem.GetNextInt(latitude);
	aItem.GetNextInt(longitude);
	
	
	posEstimate.SetPositionEstimate(TOMASuplPositionEstimate::ENorth,latitude,longitude);
	position->SetPosition(utcTime, posEstimate);	
	
	TOMASuplUtcTime recutcTime;
	TOMASuplPositionEstimate recposEstimate;
	position->GetPosition(recutcTime, recposEstimate);	
	TInt reclatitude;
	TInt reclongitude;
	
	TOMASuplPositionEstimate::TOMASuplLatitudeSign recposestimate;
	
	recposEstimate.GetPositionEstimate(recposestimate,reclatitude,reclongitude);
   
   
   TDateTime recdateTime;
   TInt reczoneCode;
   TInt recxone;
   recutcTime.GetUtcTime(recdateTime,reczoneCode,recxone);
   TTime recTime(recdateTime);
	
	delete position;
	if( recTime ==  setTime && recxone == xone &&  zoneCode == reczoneCode && recposestimate == TOMASuplPositionEstimate::ENorth 
							&& latitude == reclatitude && longitude ==  reclongitude)		
		return KErrNone;
	else
		return KErrGeneral;

	}


//#include <epos_comasuplposition.h>
//#include <epos_comasuplvelocity.h>

// -----------------------------------------------------------------------------
// CPosAPITestModule::COMASuplPosition_SetSuplVelocity
// 
// -----------------------------------------------------------------------------
//
TInt CPosAPITestModule::COMASuplPosition_SetSuplVelocityL( CStifItemParser& aItem )
	{
	
	COMASuplPosition*  position  = COMASuplPosition::NewL();
	COMASuplVelocity* velocity = COMASuplVelocity::NewL();
	COMASuplHorizAndVertVelocity* horizonverticalVelocity = COMASuplHorizAndVertVelocity::NewL();
	
	TInt bearingHVV; //	= 34;
	TInt horSpeedHVV;	//  = 35;
	TInt  verDirectHVV; //	= 56;
	TInt  verSpeedHVV; //	= 57;
	
	aItem.GetNextInt(bearingHVV);
	aItem.GetNextInt(horSpeedHVV);
	aItem.GetNextInt(verDirectHVV);
	aItem.GetNextInt(verSpeedHVV);
	
	horizonverticalVelocity->SetHorizAndVertVel(bearingHVV,horSpeedHVV,verDirectHVV,verSpeedHVV);
	velocity->SetVelocity(horizonverticalVelocity);
	position->SetSuplVelocity(velocity);
		
	COMASuplVelocity* recVelocity = COMASuplVelocity::NewL();
	position->GetVelocity(recVelocity);
	COMASuplHorizVelocity* suplHorivelocity =  recVelocity->Velocity();
	COMASuplHorizAndVertVelocity* recHorizonverticalVelocity = (COMASuplHorizAndVertVelocity*)suplHorivelocity;
	
	TUint16 recBearing;
	TUint16 recHorSpeed;
	TUint8 recVerDirect;
	TUint8 recVerSpeed;
	
	recHorizonverticalVelocity->GetHorizAndVertVel(recBearing,recHorSpeed,recVerDirect,recVerSpeed);
	delete position;	
	
	if( bearingHVV ==  recBearing && horSpeedHVV == recHorSpeed &&  verDirectHVV == recVerDirect && verSpeedHVV == recVerSpeed)		
		return KErrNone;
	else
		return KErrGeneral;
	}

//#include <epos_comasuplposition.h>
//#include <epos_comasuplvelocity.h>

// -----------------------------------------------------------------------------
// CPosAPITestModule::COMASuplPosition_CloneL
// 
// -----------------------------------------------------------------------------
//
TInt CPosAPITestModule::COMASuplPosition_CloneL( CStifItemParser& aItem )
	{
	
	COMASuplPosition*  position  = COMASuplPosition::NewL();
	COMASuplVelocity* velocity = COMASuplVelocity::NewL();
	COMASuplHorizAndVertVelocity* horizonverticalVelocity = COMASuplHorizAndVertVelocity::NewL();
	
	TInt bearingHVV;// 	= 34;
	TInt horSpeedHVV;//	= 35;
	TInt verDirectHVV;//	= 56;
	TInt verSpeedHVV; //	= 57;

	aItem.GetNextInt(bearingHVV);
	aItem.GetNextInt(horSpeedHVV);
	aItem.GetNextInt(verDirectHVV);
	aItem.GetNextInt(verSpeedHVV);
	
	horizonverticalVelocity->SetHorizAndVertVel(bearingHVV,horSpeedHVV,verDirectHVV,verSpeedHVV);
	velocity->SetVelocity(horizonverticalVelocity);
	position->SetSuplVelocity(velocity);
		

	COMASuplPosition*  ClonePosition  = (COMASuplPosition*)position->CloneL();

	COMASuplVelocity* recVelocity = COMASuplVelocity::NewL();
	ClonePosition->GetVelocity(recVelocity);
	COMASuplHorizVelocity* suplHorivelocity =  recVelocity->Velocity();
	COMASuplHorizAndVertVelocity* recHorizonverticalVelocity = (COMASuplHorizAndVertVelocity*)suplHorivelocity;
	
	TUint16 recBearing;
	TUint16 recHorSpeed;
	TUint8 recVerDirect;
	TUint8 recVerSpeed;
	
	recHorizonverticalVelocity->GetHorizAndVertVel(recBearing,recHorSpeed,recVerDirect,recVerSpeed);
	
	delete position;	
	delete ClonePosition;
	
	if( bearingHVV ==  recBearing && horSpeedHVV == recHorSpeed &&  verDirectHVV == recVerDirect && verSpeedHVV == recVerSpeed)		
		return KErrNone;
	else
		return KErrGeneral;
	}


//#include <epos_comasuplpospayload.h>
// -----------------------------------------------------------------------------
// CPosAPITestModule::COMASuplPosPayload_SuplPayloadType
// 
// -----------------------------------------------------------------------------
//
TInt CPosAPITestModule::COMASuplPosPayload_SuplPayloadTypeL( CStifItemParser& /*aItem*/ )
	{
		TInt err;
		_LIT8(KPayLoad,"This is POS Pay Load Data");
		TBufC8<200> bufPayLoadData(KPayLoad);
		COMASuplPosPayload::TOMASuplPosPayloadType posPayloadType = COMASuplPosPayload::ETIA801;
		COMASuplPosPayload::TOMASuplPosPayloadType recPosPayloadType;
		
		COMASuplPosPayload* posPayload = COMASuplPosPayload::NewL();
		HBufC8* ptrPayLoadData = bufPayLoadData.AllocL();
		posPayload->SetPosPayload(ptrPayLoadData,posPayloadType);
		
		HBufC8* recPtrPayLoadData = HBufC8::NewL(200);
		posPayload->GetPosPayload(recPtrPayLoadData,recPosPayloadType);


		if( posPayloadType ==  recPosPayloadType &&  *recPtrPayLoadData == bufPayLoadData )		
			err = KErrNone;
		else
			err = KErrGeneral;
		
		if(err != KErrNone)
			{
				delete posPayload;		
				return err;	
			}
		
		COMASuplPosPayload::TOMASuplPosPayloadType posPayloadType1 = COMASuplPosPayload::ERRLP;
		COMASuplPosPayload::TOMASuplPosPayloadType recPosPayloadType1;
		posPayload->SetSuplPayloadType(posPayloadType1);	//Test only SetSuplPayloadType
		recPosPayloadType1 = posPayload->SuplPayloadType();
		
		if(posPayloadType1 ==  recPosPayloadType1)
			err = KErrNone;
		else
			err = KErrGeneral;
		   
		delete posPayload;		
		return err;
	}

//#include <epos_comasuplpospayload.h>
// -----------------------------------------------------------------------------
// CPosAPITestModule::COMASuplPosPayload_CloneL
// 
// -----------------------------------------------------------------------------
//
TInt CPosAPITestModule::COMASuplPosPayload_CloneL( CStifItemParser& /*aItem*/ )
	{
		 TInt err;
		_LIT8(KPayLoad,"This is POS Pay Load Data");
		TBufC8<200> bufPayLoadData(KPayLoad);
		COMASuplPosPayload::TOMASuplPosPayloadType posPayloadType = COMASuplPosPayload::ETIA801;
		COMASuplPosPayload::TOMASuplPosPayloadType recPosPayloadType;
		
		COMASuplPosPayload* posPayload = COMASuplPosPayload::NewL();
		HBufC8* ptrPayLoadData = bufPayLoadData.AllocL();
		posPayload->SetPosPayload(ptrPayLoadData,posPayloadType);
		
		
		COMASuplPosPayload* ClonePosPayload = (COMASuplPosPayload*)posPayload->CloneL();
		
		HBufC8* recPtrPayLoadData = HBufC8::NewL(200);
		ClonePosPayload->GetPosPayload(recPtrPayLoadData,recPosPayloadType);

		if( posPayloadType ==  recPosPayloadType &&  *recPtrPayLoadData == bufPayLoadData )		
			err = KErrNone;
		else
			err = KErrGeneral;
		
		delete posPayload;		
		delete ClonePosPayload;
		return err;
	}

//#include <epos_comasuplsetcapabilities.h>
// -----------------------------------------------------------------------------
// CPosAPITestModule::COMASuplSetCapabilities_Set_Get
// 
// -----------------------------------------------------------------------------
//
TInt CPosAPITestModule::COMASuplSetCapabilities_Set_GetL( CStifItemParser& aItem )
	{
		//ETrue ETrue ETrue ETrue ETrue EFalse ETrue EFalse EFalse		
		TBool setGpsSETAssisted,setGpsSETBased,setAutonomousGps,setaFLT,setAeOTD,setOTDOA;		
		TBool setTIA801,setRRC,setRRLP;

		setGpsSETAssisted = setGpsSETBased = setAutonomousGps = setaFLT = setAeOTD = setOTDOA = EFalse;		// To Fix Warning
		setTIA801 = setRRC = setRRLP = EFalse; // To Fix Warning
		
		TOMASuplPosTechnology postechnology;
		TOMASuplPosProtocol posprotocol;	
		TInt value;

		for(TInt i = 1; i <= 6; i++)
			{
				aItem.GetNextInt(value);
				switch(i)
					{
						case 1: setGpsSETAssisted = value; break;
						case 2: setGpsSETBased = value; break;
						case 3: setAutonomousGps = value; break;
						case 4: setaFLT = value; break;
						case 5: setAeOTD = value; break;
						case 6: setOTDOA = value; break;
						default : return KErrArgument;
					}
			}
			
		aItem.GetNextInt(value);	
		setTIA801 = value;
		aItem.GetNextInt(value);	
		setRRLP = value;
		aItem.GetNextInt(value);	
		setRRC = value;
				
		COMASuplSETCapabilities* capabilitSet = COMASuplSETCapabilities::NewL();

		//Set the Postechnology
		postechnology.SetPosTechnology(setGpsSETAssisted,setGpsSETBased,setAutonomousGps,setaFLT,setAeOTD,setOTDOA);
		//Set the pos protocol
		posprotocol.SetPosProtocol(setTIA801,setRRLP,setRRC);					
		//set the capability of SET
		capabilitSet->SetSETCapabilities(postechnology,
									COMASuplSETCapabilities::EOMANoPreferred,
									posprotocol);


		TBool recGpsSETAssisted,recGpsSETBased,recAutonomousGps,recaFLT,recAeOTD,recOTDOA;		
		TBool recTIA801,recRRC,recRRLP;
		TOMASuplPosTechnology RecPosTechnology;
		COMASuplSETCapabilities::TOMASuplPrefMethod    RecPrefMethod;
		TOMASuplPosProtocol   RecPosProtocol;

		capabilitSet->GetSETCapabilities(RecPosTechnology,RecPrefMethod,RecPosProtocol);
		RecPosTechnology.GetPosTechnology(recGpsSETAssisted,recGpsSETBased,recAutonomousGps,recaFLT,recAeOTD,recOTDOA);
		RecPosProtocol.GetPosProtocol(recTIA801,recRRLP,recRRC);
		
		delete capabilitSet;
		
		if(recGpsSETAssisted == setGpsSETAssisted  && recGpsSETBased == setGpsSETBased && recAutonomousGps == setAutonomousGps  && recaFLT == setaFLT && recAeOTD == setAeOTD && recOTDOA == setOTDOA && 
		   recTIA801 == setTIA801 && setRRC == recRRC && setRRLP == recRRLP && RecPrefMethod == COMASuplSETCapabilities::EOMANoPreferred)
			{
				return KErrNone;
			}
		else
			{
				return KErrGeneral;
			}	
	}

//#include <epos_comasuplsetcapabilities.h>
// -----------------------------------------------------------------------------
// CPosAPITestModule::COMASuplSetCapabilities_CloneL
// 
// -----------------------------------------------------------------------------
//
TInt CPosAPITestModule::COMASuplSetCapabilities_CloneL( CStifItemParser& aItem )
	{

		TBool setGpsSETAssisted,setGpsSETBased,setAutonomousGps,setaFLT,setAeOTD,setOTDOA;		
		TBool setTIA801,setRRC,setRRLP;
		setGpsSETAssisted = setGpsSETBased = setAutonomousGps = setaFLT = setAeOTD = setOTDOA = EFalse;		// To Fix Warning
		setTIA801 = setRRC = setRRLP = EFalse; // To Fix Warning

		TOMASuplPosTechnology postechnology;
		TOMASuplPosProtocol posprotocol;	
		TInt value;

		for(TInt i = 1; i <= 6; i++)
			{
				aItem.GetNextInt(value);
				switch(i)
					{
						case 1: setGpsSETAssisted = value; break;
						case 2: setGpsSETBased = value; break;
						case 3: setAutonomousGps = value; break;
						case 4: setaFLT = value; break;
						case 5: setAeOTD = value; break;
						case 6: setOTDOA = value; break;
						default : return KErrArgument;
					}
			}
			
		aItem.GetNextInt(value);	
		setTIA801 = value;
		aItem.GetNextInt(value);	
		setRRLP = value;
		aItem.GetNextInt(value);	
		setRRC = value;
				
		COMASuplSETCapabilities* capabilitSet = COMASuplSETCapabilities::NewL();

		//Set the Postechnology
		postechnology.SetPosTechnology(setGpsSETAssisted,setGpsSETBased,setAutonomousGps,setaFLT,setAeOTD,setOTDOA);
		//Set the pos protocol
		posprotocol.SetPosProtocol(setTIA801,setRRLP,setRRC);					
		//set the capability of SET
		capabilitSet->SetSETCapabilities(postechnology,
									COMASuplSETCapabilities::EOMANoPreferred,
									posprotocol);


		COMASuplSETCapabilities* CloneCapabilitSet = (COMASuplSETCapabilities*)capabilitSet->CloneL();
		
		TBool recGpsSETAssisted,recGpsSETBased,recAutonomousGps,recaFLT,recAeOTD,recOTDOA;		
		TBool recTIA801,recRRC,recRRLP;
		TOMASuplPosTechnology RecPosTechnology;
		COMASuplSETCapabilities::TOMASuplPrefMethod    RecPrefMethod;
		TOMASuplPosProtocol   RecPosProtocol;

		CloneCapabilitSet->GetSETCapabilities(RecPosTechnology,RecPrefMethod,RecPosProtocol);
		RecPosTechnology.GetPosTechnology(recGpsSETAssisted,recGpsSETBased,recAutonomousGps,recaFLT,recAeOTD,recOTDOA);
		RecPosProtocol.GetPosProtocol(recTIA801,recRRLP,recRRC);
		
		delete capabilitSet;
		delete CloneCapabilitSet;
		
		if(recGpsSETAssisted == setGpsSETAssisted  && recGpsSETBased == setGpsSETBased && recAutonomousGps == setAutonomousGps  && recaFLT == setaFLT && recAeOTD == setAeOTD && recOTDOA == setOTDOA && 
		   recTIA801 == setTIA801 && setRRC == recRRC && setRRLP == recRRLP && RecPrefMethod == COMASuplSETCapabilities::EOMANoPreferred)
			{
				return KErrNone;
			}
		else
			{
				return KErrGeneral;
			}	
	}


//#include <epos_comasuplvelocity.h>
// -----------------------------------------------------------------------------
// CPosAPITestModule::COMASuplVelocity_Set_Get
// 
// -----------------------------------------------------------------------------
//
TInt CPosAPITestModule::COMASuplVelocity_Set_GetL( CStifItemParser& aItem )
	{
	
	COMASuplVelocity* velocity = COMASuplVelocity::NewL();
	COMASuplHorizVelocity* horizonVelocity = COMASuplHorizVelocity::NewL();
	
	TInt bearingHVV; 
	TInt horSpeedHVV;
	
	aItem.GetNextInt(bearingHVV);	
	aItem.GetNextInt(horSpeedHVV);	
	
	
	horizonVelocity->SetHorizVel(bearingHVV,horSpeedHVV);
	velocity->SetVelocity(horizonVelocity);

	TUint16 recBearingHVV; 
	TUint16 recHorSpeedHVV;
	
	COMASuplHorizVelocity* recVelocity = velocity->Velocity();
	recVelocity->GetHorizVel(recBearingHVV,recHorSpeedHVV);

			if(recBearingHVV == bearingHVV && horSpeedHVV == recHorSpeedHVV )
			{
				return KErrNone;
			}
		else
			{
				return KErrGeneral;
			}	

	}
	
//#include <epos_comasuplvelocity.h>
// -----------------------------------------------------------------------------
// CPosAPITestModule::COMASuplVelocity_CloneL
// 
// -----------------------------------------------------------------------------
//
TInt CPosAPITestModule::COMASuplVelocity_CloneL( CStifItemParser& aItem )
	{
	
	COMASuplVelocity* velocity = COMASuplVelocity::NewL();
	COMASuplHorizVelocity* horizonVelocity = COMASuplHorizVelocity::NewL();
	
	TInt bearingHVV; 
	TInt horSpeedHVV;
	
	aItem.GetNextInt(bearingHVV);	
	aItem.GetNextInt(horSpeedHVV);	
	
	horizonVelocity->SetHorizVel(bearingHVV,horSpeedHVV);
	velocity->SetVelocity(horizonVelocity);
	
	TUint16 recBearingHVV; 
	TUint16 recHorSpeedHVV;
	
	COMASuplVelocity* CloneLVelocity =  (COMASuplVelocity*)velocity->CloneL();
	COMASuplHorizVelocity* recVelocity = CloneLVelocity->Velocity();
	recVelocity->GetHorizVel(recBearingHVV,recHorSpeedHVV);
	TOMASuplVelocityType  velType = velocity->VelType();
	
	delete CloneLVelocity;
	delete velocity;

		if(recBearingHVV == bearingHVV && horSpeedHVV == recHorSpeedHVV && velType == EHorizVelocity)
		{
			return KErrNone;
		}
		else
		{
			return KErrGeneral;
		}	

	}


//#include <epos_comasuplvelocity.h>
// -----------------------------------------------------------------------------
// CPosAPITestModule::COMASuplHorizVelocity_Get_Set_HorizVel
// 
// -----------------------------------------------------------------------------
//
TInt CPosAPITestModule::COMASuplHorizVelocity_Get_Set_HorizVelL( CStifItemParser& aItem )
	{
	
	COMASuplHorizVelocity* horizonVelocity = COMASuplHorizVelocity::NewL();
	
	TInt bearingHVV; 
	TInt horSpeedHVV;
	
	aItem.GetNextInt(bearingHVV);	
	aItem.GetNextInt(horSpeedHVV);	
	
	horizonVelocity->SetHorizVel(bearingHVV,horSpeedHVV);
	
	TUint16 recBearingHVV; 
	TUint16 recHorSpeedHVV;
	
	horizonVelocity->GetHorizVel(recBearingHVV,recHorSpeedHVV);
	
	delete horizonVelocity;

		if(recBearingHVV == bearingHVV && horSpeedHVV == recHorSpeedHVV )
		{
			return KErrNone;
		}
		else
		{
			return KErrGeneral;
		}	

	}

//#include <epos_comasuplvelocity.h>
// -----------------------------------------------------------------------------
// CPosAPITestModule::COMASuplHorizAndVertVelocity
// 
// -----------------------------------------------------------------------------
//
TInt CPosAPITestModule::COMASuplHorizAndVertVelocity_Set_GetL( CStifItemParser& aItem )
	{
		COMASuplHorizAndVertVelocity* horizonverticalVelocity = COMASuplHorizAndVertVelocity::NewL();
		
		TInt bearingHVV; //	= 34;
		TInt horSpeedHVV;	//  = 35;
		TInt verDirectHVV; //	= 56;
		TInt verSpeedHVV; //	= 57;
		
		aItem.GetNextInt(bearingHVV);
		aItem.GetNextInt(horSpeedHVV);
		aItem.GetNextInt(verDirectHVV);
		aItem.GetNextInt(verSpeedHVV);
		
		horizonverticalVelocity->SetHorizAndVertVel(bearingHVV,horSpeedHVV,verDirectHVV,verSpeedHVV);
		
		TUint16 recBearing;
		TUint16 recHorSpeed;
		TUint8  recVerDirect;
		TUint8  recVerSpeed;
		
		horizonverticalVelocity->GetHorizAndVertVel(recBearing,recHorSpeed,recVerDirect,recVerSpeed);
		delete horizonverticalVelocity;
		
		if( bearingHVV ==  recBearing && horSpeedHVV == recHorSpeed &&  verDirectHVV == recVerDirect && verSpeedHVV == recVerSpeed)		
			return KErrNone;
		else
			return KErrGeneral;
	}

//#include <epos_comasuplvelocity.h>
// -----------------------------------------------------------------------------
// CPosAPITestModule::COMASuplHorizAndVertVelocity
// 
// -----------------------------------------------------------------------------
//
TInt CPosAPITestModule::COMASuplHorizAndVertUncertVelocity_Set_GetL( CStifItemParser& aItem )
	{
		COMASuplHorizAndVertUncertVelocity* horizAndVertUncertVelocity = COMASuplHorizAndVertUncertVelocity::NewL();
		
		TInt bearing; //	= 34;
		TInt horSpeed;	//  = 35;
		TInt verDirect; //	= 56;
		TInt verSpeed; //	= 57;
		TInt horUncertSpeed; //20
		TInt verUncertSpeed; //40
		
		aItem.GetNextInt(bearing);
		aItem.GetNextInt(horSpeed);
		aItem.GetNextInt(verDirect);
		aItem.GetNextInt(verSpeed);
		aItem.GetNextInt(horUncertSpeed);
		aItem.GetNextInt(verUncertSpeed);
		
        horizAndVertUncertVelocity->SetHorizVertUncertVel(bearing,horSpeed,verDirect,verSpeed,horUncertSpeed,verUncertSpeed);
		
		TUint16 recBearing;
		TUint16 recHorSpeed;
		TUint8  recverDirect; 
		TUint8  recVerSpeed;
		TUint8  rechorUncertSpeed;
		TUint8  recverUncertSpeed;

		horizAndVertUncertVelocity->GetHorizVertUncertVel(recBearing,recHorSpeed,recverDirect,recVerSpeed,rechorUncertSpeed,recverUncertSpeed);
		TOMASuplVelocityType  velType = horizAndVertUncertVelocity->VelType();
		
		delete horizAndVertUncertVelocity;
		
		if( bearing ==  recBearing && horSpeed == recHorSpeed &&  verDirect == recverDirect && verSpeed == recVerSpeed 
								   && horUncertSpeed == rechorUncertSpeed && verUncertSpeed == recverUncertSpeed 
								   && velType == EHorizAndVertUncertVelocity)		
			return KErrNone;
		else
			return KErrGeneral;
	}


//#include <epos_comasuplvelocity.h>
// -----------------------------------------------------------------------------
// CPosAPITestModule::COMASuplHorizAndVertVelocity
// 
// -----------------------------------------------------------------------------
//
TInt CPosAPITestModule::COMASuplHorizUncertVelocity_Set_GetL( CStifItemParser& aItem )
	{
		COMASuplHorizUncertVelocity* horizUncertVelocity = COMASuplHorizUncertVelocity::NewL();
		
		TInt bearing; //	= 34;
		TInt horSpeed;	//  = 35;
		TInt UncertSpeed; //	= 56;
		
		aItem.GetNextInt(bearing);
		aItem.GetNextInt(horSpeed);
		aItem.GetNextInt(UncertSpeed);
		
		horizUncertVelocity->SetHorizUncertVel(bearing,horSpeed,UncertSpeed);
		TOMASuplVelocityType  velType = horizUncertVelocity->VelType();
		
		TUint16 recBearing;
		TUint16 recHorSpeed;
		TUint8  recUncertSpeed;
		
		horizUncertVelocity->GetHorizUncertVel(recBearing,recHorSpeed,recUncertSpeed);
		delete horizUncertVelocity;
		
		if( bearing ==  recBearing && horSpeed == recHorSpeed &&  UncertSpeed == recUncertSpeed && velType == EHorizUncertVelocity)		
			return KErrNone;
		else
			return KErrGeneral;
	}
	
	
//#include <epos_comasuplreqasstdata.h>
// -----------------------------------------------------------------------------
// CPosAPITestModule::TOMASuplSatelliteInfoElement
// 
// -----------------------------------------------------------------------------
//

TOMASuplSatelliteInfoElement CPosAPITestModule::TOMASuplSatelliteInfoElement_SatElement( TInt aSatId, TInt aIode, TInt& aError)
	{
		TOMASuplSatelliteInfoElement  satelliteElement;
		aError = satelliteElement.SetSatInfoElement(aSatId,aIode); 
		return satelliteElement;
	}

//#include <epos_comasuplreqasstdata.h>
// -----------------------------------------------------------------------------
// CPosAPITestModule::TOMASuplSatelliteInfoElement
// 
// -----------------------------------------------------------------------------
//
TInt CPosAPITestModule::TOMASuplSatelliteInfoElement_Set_Get( CStifItemParser& aItem )
	{
		TInt SatId; //0 to 63
		TInt Iode; //0 to 255
		TInt err;

		aItem.GetNextInt(SatId);	
		aItem.GetNextInt(Iode);	
		
		TOMASuplSatelliteInfoElement  satelliteElement = TOMASuplSatelliteInfoElement_SatElement(SatId,Iode,err);
		if(err == KErrNone)
			{
				TInt recSatId; 
				TInt recIode;
				satelliteElement.GetSatInfoElement(recSatId,recIode);
				if(recSatId == SatId && recIode == Iode)
					{
						return KErrNone;
					}
				else
					{
						return KErrGeneral;
					}	
			}	
			else
			{
				return err;
			}
	}

//#include <epos_comasuplreqasstdata.h>
// -----------------------------------------------------------------------------
// CPosAPITestModule::TOMASuplNavigationModel_Set_GetSatInfoElement_AllElements
// 
// -----------------------------------------------------------------------------
//
TInt CPosAPITestModule::TOMASuplNavigationModel_Set_GetSatInfoElement_AllElements( CStifItemParser& aItem )
	{
	
		RArray<TOMASuplSatelliteInfoElement> satInfo;
		
		RArray<TInt> arrSatId;
		RArray<TInt> arrIode;
		
		
		TInt SatId; //0 to 63
		TInt Iode; //0 to 255
		TInt err;
		TInt count;
		
		aItem.GetNextInt(count);	
		
		TInt TotalElements = count;
		
		while(count)
			{
				aItem.GetNextInt(SatId);	
				aItem.GetNextInt(Iode);	
				
				arrSatId.Append(SatId);
				arrIode.Append(Iode);
				
				TOMASuplSatelliteInfoElement  satelliteElement = TOMASuplSatelliteInfoElement_SatElement(SatId,Iode,err);
				if(err == KErrNone)
				{
					satInfo.Append(satelliteElement);
				}
				else
				{
					satInfo.Close();
					arrIode.Close();
					arrSatId.Close();
					return err;	
				}
				--count;
			}
		
		TOMASuplNavigationModel navigationModel; 
		navigationModel.SetSatInfoElement(satInfo);
		
		TInt recSatId; 
		TInt recIode;
		
		
		TOMASuplSatelliteInfoElement elementAtSpecificIndex;
		navigationModel.GetSatInfoElement(elementAtSpecificIndex,0);
		elementAtSpecificIndex.GetSatInfoElement(recSatId,recIode);
		if(!(recSatId == arrSatId[0] && recIode == arrIode[0] ))
			{
				satInfo.Close();
				arrIode.Close();
				arrSatId.Close();
				return 	KErrGeneral;
			}
		
		
		RArray<TOMASuplSatelliteInfoElement> recSatInfo;
		err = navigationModel.GetSatInfoElement(recSatInfo);
		count = 0;
		if(err == KErrNone || recSatInfo.Count() == TotalElements )
			{
				while(count < TotalElements)
					{
						TOMASuplSatelliteInfoElement satelliteElement = recSatInfo[count];
						satelliteElement.GetSatInfoElement(recSatId,recIode);
						if(!(recSatId == arrSatId[count] && recIode == arrIode[count]))
							{
								satInfo.Close();
								arrIode.Close();
								arrSatId.Close();
								recSatInfo.Close();
								return KErrArgument;
							}	
							count++;
					}
			}
		else
			{
			satInfo.Close();
			arrIode.Close();
			arrSatId.Close();
			recSatInfo.Close();
			return err;
			}

	satInfo.Close();
	arrIode.Close();
	arrSatId.Close();
	recSatInfo.Close();
	return KErrNone;			
	
	}

//#include <epos_comasuplreqasstdata.h>
// -----------------------------------------------------------------------------
// CPosAPITestModule::TOMASuplNavigationModel_Set_GetSatInfoElement_SpecificElement
// 
// -----------------------------------------------------------------------------
//
TInt CPosAPITestModule::TOMASuplNavigationModel_Set_GetSatInfoElement_SpecificElement( CStifItemParser& aItem )
	{
		// Insert the all the elements...
		RArray<TOMASuplSatelliteInfoElement> satInfo;
		
		RArray<TInt> arrSatId;
		RArray<TInt> arrIode;
		
		TInt SatId; //0 to 63
		TInt Iode; //0 to 255
		TInt err;
		TInt count;
		
		TInt insSatId; //0 to 63
		TInt insIode; //0 to 255
		TInt pos;


		TInt recSatId; 
		TInt recIode;

		aItem.GetNextInt(count);	
		TInt TotalElements = count;
		while(count)
			{
				aItem.GetNextInt(SatId);	
				aItem.GetNextInt(Iode);	
				
				arrSatId.Append(SatId);
				arrIode.Append(Iode);
				
				TOMASuplSatelliteInfoElement  satelliteElement = TOMASuplSatelliteInfoElement_SatElement(SatId,Iode,err);
				if(err == KErrNone)
				{
					satInfo.Append(satelliteElement);
				}
				else
				{
					return err;	
				}
				--count;
			}
		
		TOMASuplNavigationModel navigationModel; 
		navigationModel.SetSatInfoElement(satInfo);  //Set Elements.....
		
		aItem.GetNextInt(insSatId);	  // Elements to be inserted at pos
		aItem.GetNextInt(insIode);	
		
		aItem.GetNextInt(pos);	 
		
		TOMASuplSatelliteInfoElement insertSatelliteElement;//Element to insert
		err = insertSatelliteElement.SetSatInfoElement(insSatId,insIode); 
		err = navigationModel.SetSatInfoElement(insertSatelliteElement,pos);
		 if(err != KErrNone)
		 	{
				satInfo.Close();
				arrIode.Close();
				arrSatId.Close();
		 		return KErrArgument;
		 	}
		
		RArray<TOMASuplSatelliteInfoElement> recSatInfo;
		err = navigationModel.GetSatInfoElement(recSatInfo);
		
		if(err == KErrNone || recSatInfo.Count() == TotalElements + 1 )
			{
						TOMASuplSatelliteInfoElement recSatelliteElement = recSatInfo[pos];
						recSatelliteElement.GetSatInfoElement(recSatId,recIode);
						satInfo.Close();
						arrIode.Close();
						arrSatId.Close();
						recSatInfo.Close();
						if(recSatId == insSatId && recIode == insIode)
							{
								return KErrNone;
							}	
						else
							{
								return KErrArgument;
							}	
			}
		else
			{
				satInfo.Close();
				arrIode.Close();
				arrSatId.Close();
				recSatInfo.Close();
				return KErrArgument;
			}
	}

//#include <epos_comasuplreqasstdata.h>
// -----------------------------------------------------------------------------
// CPosAPITestModule::TOMASuplNavigationModel_Set_GetNavigationModel
// 
// -----------------------------------------------------------------------------
//
TInt CPosAPITestModule::TOMASuplNavigationModel_Set_GetNavigationModel( CStifItemParser& aItem )
	{
		TOMASuplNavigationModel navigationalModel;
		
    	TInt GPSWeek; 	// 0 to 1023
    	TInt GPSToe; 	// 0 to 167
    	TInt NSat; 		// 0 to 31
    	TInt ToeLimit;  // 0 to 10
		
		aItem.GetNextInt(GPSWeek);
		aItem.GetNextInt(GPSToe);
		aItem.GetNextInt(NSat);
		aItem.GetNextInt(ToeLimit);
		
		TInt err = navigationalModel.SetNavigationModel(GPSWeek,GPSToe,NSat,ToeLimit);
		if(err == KErrNone)
			{
				TInt recGPSWeek;
				TInt recGPSToe;
				TInt recNSat;
				TInt recToeLimit;
				navigationalModel.GetNavigationModel(recGPSWeek,recGPSToe,recNSat,recToeLimit);
				if( GPSWeek ==  recGPSWeek && GPSToe == recGPSToe &&  NSat == recNSat && ToeLimit == recToeLimit )		
					return KErrNone;
				else
					return KErrGeneral;
			}
		else
			{
				return err;
			}
	}

//#include <epos_comasuplreqasstdata.h>
// -----------------------------------------------------------------------------
// CPosAPITestModule::COMASuplReqAsstData_Set_GetNavigationData
// 
// -----------------------------------------------------------------------------
//
TInt CPosAPITestModule::COMASuplReqAsstData_Set_GetNavigationDataL( CStifItemParser& aItem )
	{
	
		COMASuplReqAsstData* reqAsstData = COMASuplReqAsstData::NewL();
		
		RArray<TOMASuplSatelliteInfoElement> satInfo;
		
		RArray<TInt> arrSatId;
		RArray<TInt> arrIode;
		
		TInt SatId; //0 to 63
		TInt Iode; //0 to 255
		TInt err;
		TInt count;
		
		aItem.GetNextInt(count);	
		
		TInt TotalElements = count;
		
		while(count)
			{
				aItem.GetNextInt(SatId);	
				aItem.GetNextInt(Iode);	
				
				arrSatId.Append(SatId);
				arrIode.Append(Iode);
				
				TOMASuplSatelliteInfoElement  satelliteElement = TOMASuplSatelliteInfoElement_SatElement(SatId,Iode,err);
				if(err == KErrNone)
				{
					satInfo.Append(satelliteElement);
				}
				else
				{	
				
					satInfo.Close();
					arrIode.Close();
					arrSatId.Close();
					return err;	
				}
				--count;
			}
		
		TOMASuplNavigationModel navigationModel; 
		navigationModel.SetSatInfoElement(satInfo);
		
		reqAsstData->SetNavigationData(navigationModel); //SET to Assistance data...
		
		TOMASuplNavigationModel recNavigationModel; 
		reqAsstData->GetNavigationData(recNavigationModel); //REtriving Assistance data...
		
		RArray<TOMASuplSatelliteInfoElement> recSatInfo;
		err = recNavigationModel.GetSatInfoElement(recSatInfo);
		
		TInt recSatId; 
		TInt recIode;
		count = 0;
		if(err == KErrNone || recSatInfo.Count() == TotalElements )
			{
				while(count < TotalElements)
					{
						TOMASuplSatelliteInfoElement satelliteElement = recSatInfo[count];
						satelliteElement.GetSatInfoElement(recSatId,recIode);
						if(!(recSatId == arrSatId[count] && recIode == arrIode[count]))
							{
								satInfo.Close();
								arrIode.Close();
								arrSatId.Close();
								recSatInfo.Close();
								delete reqAsstData;
								return KErrArgument;
							}	
							count++;
					}
			}
		else
			{
				satInfo.Close();
				arrIode.Close();
				arrSatId.Close();
				recSatInfo.Close();
				delete reqAsstData;
				return err;
			}
			
	satInfo.Close();
	arrIode.Close();
	arrSatId.Close();
	recSatInfo.Close();
	delete reqAsstData;		
	return KErrNone;			
	
	}

//#include <epos_comasuplreqasstdata.h>
// -----------------------------------------------------------------------------
// CPosAPITestModule::COMASuplReqAsstData_Set_GetNavigationData
// 
// -----------------------------------------------------------------------------
//
TInt CPosAPITestModule::COMASuplReqAsstData_Set_GetReqAsstDataL ( CStifItemParser& aItem )
	{
		
		TBool AlmanacReq,UtcModel,IonModel,DgpsCorrect,RefeLocation,ReferenceTimeRequested;
		TBool Acquisition,RealTime;
		
		AlmanacReq = UtcModel = IonModel = DgpsCorrect = RefeLocation = ReferenceTimeRequested = EFalse; //To Fix Warning
		Acquisition = RealTime = EFalse;

		TInt value;
		for(TInt i = 1; i <= 8; i++)
			{
				aItem.GetNextInt(value);
				switch(i)
					{
						case 1: AlmanacReq = value; break;
						case 2: UtcModel = value; break;
						case 3: IonModel = value; break;
						case 4: DgpsCorrect = value; break;
						case 5: RefeLocation = value; break;
						case 6: ReferenceTimeRequested = value; break;
						case 7: Acquisition = value; break;
						case 8: RealTime = value; break;
						default : return KErrArgument;
					}
			}

		COMASuplReqAsstData* reqAsstData = COMASuplReqAsstData::NewL();
        reqAsstData->SetReqAsstData(AlmanacReq,UtcModel,IonModel,DgpsCorrect,RefeLocation,ReferenceTimeRequested,Acquisition,RealTime);
        
   		TBool recAlmanacReq,recUtcModel,recIonModel,recDgpsCorrect,recRefeLocation,recReferenceTimeRequested;
		TBool recAcquisition,recRealTime;
        
        reqAsstData->GetReqAsstData(recAlmanacReq,recUtcModel,recIonModel,recDgpsCorrect,recRefeLocation,recReferenceTimeRequested,recAcquisition,recRealTime);
        delete reqAsstData;
		
		if(AlmanacReq == recAlmanacReq && UtcModel == recUtcModel && IonModel == recIonModel && 
		   recDgpsCorrect == DgpsCorrect && recRefeLocation== RefeLocation && recReferenceTimeRequested == ReferenceTimeRequested && 
		   recAcquisition == Acquisition && RealTime == recRealTime)
			return KErrNone;
		else
			return KErrGeneral;
	}
	
//#include <epos_comasuplreqasstdata.h>
// -----------------------------------------------------------------------------
// CPosAPITestModule::COMASuplReqAsstData_CloneL
// 
// -----------------------------------------------------------------------------
//
TInt CPosAPITestModule::COMASuplReqAsstData_CloneL(CStifItemParser& aItem )
	{
		TBool AlmanacReq,UtcModel,IonModel,DgpsCorrect,RefeLocation,ReferenceTimeRequested;
		TBool Acquisition,RealTime;
		
		AlmanacReq = UtcModel = IonModel = DgpsCorrect = RefeLocation = ReferenceTimeRequested = EFalse; //To Fix Warning
		Acquisition = RealTime = EFalse;
		
		TInt value;
		for(TInt i = 1; i <= 8; i++)
			{
				aItem.GetNextInt(value);
				switch(i)
					{
						case 1: AlmanacReq = value; break;
						case 2: UtcModel = value; break;
						case 3: IonModel = value; break;
						case 4: DgpsCorrect = value; break;
						case 5: RefeLocation = value; break;
						case 6: ReferenceTimeRequested = value; break;
						case 7: Acquisition = value; break;
						case 8: RealTime = value; break;
						default : return KErrArgument;
					}
			}

		COMASuplReqAsstData* reqAsstData = COMASuplReqAsstData::NewL();
        reqAsstData->SetReqAsstData(AlmanacReq,UtcModel,IonModel,DgpsCorrect,RefeLocation,ReferenceTimeRequested,Acquisition,RealTime);
        
        COMASuplReqAsstData* cloneAsstData = (COMASuplReqAsstData*)reqAsstData->CloneL();
        
   		TBool recAlmanacReq,recUtcModel,recIonModel,recDgpsCorrect,recRefeLocation,recReferenceTimeRequested;
		TBool recAcquisition,recRealTime;
        
        cloneAsstData->GetReqAsstData(recAlmanacReq,recUtcModel,recIonModel,recDgpsCorrect,recRefeLocation,recReferenceTimeRequested,recAcquisition,recRealTime);
        delete reqAsstData;
        delete cloneAsstData;
		
		if(AlmanacReq == recAlmanacReq && UtcModel == recUtcModel && IonModel == recIonModel && 
		   recDgpsCorrect == DgpsCorrect && recRefeLocation== RefeLocation && recReferenceTimeRequested == ReferenceTimeRequested && 
		   recAcquisition == Acquisition && RealTime == recRealTime)
			return KErrNone;
		else
			return KErrGeneral;
	}


// -----------------------------------------------------------------------------
// CPosAPITestModule::COMASuplPosHandlerBase_InitializeL
// 
// -----------------------------------------------------------------------------
//
TInt CPosAPITestModule::COMASuplPosHandlerBase_InitializeL(CStifItemParser& /*aItem*/ )
	{
		CPosApiAsynchRequestor* requestor = CPosApiAsynchRequestor::NewL(iLog,NULL);
		requestor->COMASuplPosHandlerBase_InitializeL();
		CActiveScheduler::Start();
		TInt err = requestor->GetResult();
		delete requestor;
		return err;
	}

// -----------------------------------------------------------------------------
// CPosAPITestModule::COMASuplPosHandlerBase_CreateNewSessionL
// 
// -----------------------------------------------------------------------------
//
TInt CPosAPITestModule::COMASuplPosHandlerBase_CreateNewSessionL(CStifItemParser& /*aItem*/)
	{
		CPosApiAsynchRequestor* requestor = CPosApiAsynchRequestor::NewL(iLog,NULL);
		requestor->COMASuplPosHandlerBase_InitializeL();
		CActiveScheduler::Start();
		requestor->COMASuplPosHandlerBase_CreateNewSessionL();
		TInt err = requestor->GetResult();
		delete requestor;
		return err;
	}


// -----------------------------------------------------------------------------
// CPosAPITestModule::COMASuplPosHandlerBase_CancelInitializeL
// 
// -----------------------------------------------------------------------------
//
TInt CPosAPITestModule::COMASuplPosHandlerBase_CancelInitializeL(CStifItemParser& /*aItem*/)
	{
		CPosApiAsynchRequestor* requestor = CPosApiAsynchRequestor::NewL(iLog,NULL);
		requestor->COMASuplPosHandlerBase_InitializeL();
		requestor->COMASuplPosHandlerBase_CancelInitializeL();
		delete requestor;
		return KErrNone;
	}
	
// -----------------------------------------------------------------------------
// CPosAPITestModule::COMASuplPosHandlerBase_VersionL
// 
// -----------------------------------------------------------------------------
//
TInt CPosAPITestModule::COMASuplPosHandlerBase_VersionL(CStifItemParser& /*aItem*/)
	{
		CPosApiAsynchRequestor* requestor = CPosApiAsynchRequestor::NewL(iLog,NULL);
		TInt err = requestor->COMASuplPosHandlerBase_VersionL();
		delete requestor;
		return err;
	}

// -----------------------------------------------------------------------------
// CPosAPITestModule::COMASuplPosHandlerBase_ImplementationUIDL
// 
// -----------------------------------------------------------------------------
//
TInt CPosAPITestModule::COMASuplPosHandlerBase_ImplementationUIDL(CStifItemParser& /*aItem*/)
	{
		CPosApiAsynchRequestor* requestor = CPosApiAsynchRequestor::NewL(iLog,NULL);
		TInt err = requestor->COMASuplPosHandlerBase_ImplementationUID();
		delete requestor;
		return err;
	}


// -----------------------------------------------------------------------------
// CPosAPITestModule::COMASuplPosSessionBase_InitializeL
// 
// -----------------------------------------------------------------------------
//
TInt CPosAPITestModule::COMASuplPosSessionBase_InitializeL(CStifItemParser& /*aItem*/)
	{
		CPosApiAsynchRequestor* requestor = CPosApiAsynchRequestor::NewL(iLog,NULL);
		requestor->COMASuplPosHandlerBase_InitializeL();
		CActiveScheduler::Start();
		TInt err = requestor->GetResult();
		if(err == KErrNone)
			{
				requestor->COMASuplPosHandlerBase_CreateNewSessionL();
				requestor->COMASuplPosSessionBase_InitializeL();
				CActiveScheduler::Start();
				err = requestor->GetResult();
			}
		delete requestor;
		return err;

	}
	
// -----------------------------------------------------------------------------
// CPosAPITestModule::COMASuplPosSessionBase_CancelInitializeL
// 
// -----------------------------------------------------------------------------
//
TInt CPosAPITestModule::COMASuplPosSessionBase_CancelInitializeL(CStifItemParser& /*aItem*/)
	{
		CPosApiAsynchRequestor* requestor = CPosApiAsynchRequestor::NewL(iLog,NULL);
		requestor->COMASuplPosHandlerBase_InitializeL();
		CActiveScheduler::Start();
		TInt err = requestor->GetResult();
		if(err == KErrNone)
			{
				requestor->COMASuplPosHandlerBase_CreateNewSessionL();
				requestor->COMASuplPosSessionBase_InitializeL();
				requestor->COMASuplPosSessionBase_CancelInitializeL();
				
			}
		delete requestor;
		return err;
		
	}

// -----------------------------------------------------------------------------
// CPosAPITestModule::COMASuplPosSessionBase_HandleSuplPosMessageL
// 
// -----------------------------------------------------------------------------
//
TInt CPosAPITestModule::COMASuplPosSessionBase_HandleSuplPosMessageL(CStifItemParser& /*aItem*/)
	{
		CPosApiAsynchRequestor* requestor = CPosApiAsynchRequestor::NewL(iLog,NULL);
		requestor->COMASuplPosHandlerBase_InitializeL();
		CActiveScheduler::Start();
		TInt err = requestor->GetResult();
		if(err == KErrNone)
			{
				requestor->COMASuplPosHandlerBase_CreateNewSessionL();
				requestor->COMASuplPosSessionBase_InitializeL();
				CActiveScheduler::Start();
				requestor->COMASuplPosSessionBase_HandleSuplPosMessageL();
			}
		delete requestor;
		return err;

	}

// -----------------------------------------------------------------------------
// CPosAPITestModule::COMASuplPosSessionBase_GetSuplInfoL
// 
// -----------------------------------------------------------------------------
//
TInt CPosAPITestModule::COMASuplPosSessionBase_GetSuplInfoL(CStifItemParser& /*aItem*/)
	{
		CPosApiAsynchRequestor* requestor = CPosApiAsynchRequestor::NewL(iLog,NULL);
		requestor->COMASuplPosHandlerBase_InitializeL();
		CActiveScheduler::Start();
		TInt err = requestor->GetResult();
		if(err == KErrNone)
			{
				requestor->COMASuplPosHandlerBase_CreateNewSessionL();
				requestor->COMASuplPosSessionBase_InitializeL();
				CActiveScheduler::Start();
				requestor->COMASuplPosSessionBase_GetSuplInfoL();
				CActiveScheduler::Start();
				err = requestor->GetResult();
			}
		delete requestor;
		return err;
	}

// -----------------------------------------------------------------------------
// CPosAPITestModule::COMASuplPosSessionBase_CancelSuplInfoRequestL
// 
// -----------------------------------------------------------------------------
//
TInt CPosAPITestModule::COMASuplPosSessionBase_CancelSuplInfoRequestL(CStifItemParser& /*aItem*/)
	{
		CPosApiAsynchRequestor* requestor = CPosApiAsynchRequestor::NewL(iLog,NULL);
		requestor->COMASuplPosHandlerBase_InitializeL();
		CActiveScheduler::Start();
		TInt err = requestor->GetResult();
		if(err == KErrNone)
			{
				requestor->COMASuplPosHandlerBase_CreateNewSessionL();
				requestor->COMASuplPosSessionBase_InitializeL();
				CActiveScheduler::Start();
				requestor->COMASuplPosSessionBase_GetSuplInfoL();
				requestor->COMASuplPosSessionBase_CancelSuplInfoRequest();
				err = requestor->GetResult();
			}
		delete requestor;
		return err;
	}

// -----------------------------------------------------------------------------
// CPosAPITestModule::COMASuplPosSessionBase_GetPositionL
// 
// -----------------------------------------------------------------------------
//
TInt CPosAPITestModule::COMASuplPosSessionBase_GetPositionL(CStifItemParser& /*aItem*/)
	{
		CPosApiAsynchRequestor* requestor = CPosApiAsynchRequestor::NewL(iLog,NULL);
		requestor->COMASuplPosHandlerBase_InitializeL();
		CActiveScheduler::Start();
		TInt err = requestor->GetResult();
		if(err == KErrNone)
			{
				requestor->COMASuplPosHandlerBase_CreateNewSessionL();
				requestor->COMASuplPosSessionBase_InitializeL();
				CActiveScheduler::Start();
				requestor->COMASuplPosSessionBase_GetPositionL();
				CActiveScheduler::Start();
				err = requestor->GetResult();
			}
		delete requestor;
		return err;
	
	}

// -----------------------------------------------------------------------------
// CPosAPITestModule::COMASuplPosSessionBase_CancelGetPositionL
// 
// -----------------------------------------------------------------------------
//
TInt CPosAPITestModule::COMASuplPosSessionBase_CancelGetPositionL(CStifItemParser& /*aItem*/)
	{
		CPosApiAsynchRequestor* requestor = CPosApiAsynchRequestor::NewL(iLog,NULL);
		requestor->COMASuplPosHandlerBase_InitializeL();
		CActiveScheduler::Start();
		TInt err = requestor->GetResult();
		if(err == KErrNone)
			{
				requestor->COMASuplPosHandlerBase_CreateNewSessionL();
				requestor->COMASuplPosSessionBase_InitializeL();
				CActiveScheduler::Start();
				requestor->COMASuplPosSessionBase_GetPositionL();
				requestor->COMASuplPosSessionBase_CancelGetPosition();				
				err = requestor->GetResult();
			}
		delete requestor;
		return err;

	}

// -----------------------------------------------------------------------------
// CPosAPITestModule::COMASuplPosSessionBase_SessionEndL
// 
// -----------------------------------------------------------------------------
//
TInt CPosAPITestModule::COMASuplPosSessionBase_SessionEndL(CStifItemParser& /*aItem*/)
	{
		CPosApiAsynchRequestor* requestor = CPosApiAsynchRequestor::NewL(iLog,NULL);
		requestor->COMASuplPosHandlerBase_InitializeL();
		CActiveScheduler::Start();
		TInt err = requestor->GetResult();
		if(err == KErrNone)
			{
				requestor->COMASuplPosHandlerBase_CreateNewSessionL();
				requestor->COMASuplPosSessionBase_SessionEndL();
				err = requestor->GetResult();
			}
		delete requestor;
		return err;
	}
	
// -----------------------------------------------------------------------------
// CPosAPITestModule::COMASuplPosSessionBase_SuplStateL
// 
// -----------------------------------------------------------------------------
//
TInt CPosAPITestModule::COMASuplPosSessionBase_SuplStateL(CStifItemParser& /*aItem*/)
	{
		CPosApiAsynchRequestor* requestor = CPosApiAsynchRequestor::NewL(iLog,NULL);
		requestor->COMASuplPosHandlerBase_InitializeL();
		CActiveScheduler::Start();
		TInt err = requestor->GetResult();
		if(err == KErrNone)
			{
				requestor->COMASuplPosHandlerBase_CreateNewSessionL();
				requestor->COMASuplPosSessionBase_SuplStateL();
				err = requestor->GetResult();
			}
		delete requestor;
		return err;

	}

// -----------------------------------------------------------------------------
// CPosAPITestModule::COMASuplPosHandlerBase_CreateNewSession_SuplObserverL
// 
// -----------------------------------------------------------------------------
//
TInt CPosAPITestModule::COMASuplPosHandlerBase_CreateNewSession_SuplObserverL(CStifItemParser& /*aItem*/)
	{
		CPosApiAsynchRequestor* requestor = CPosApiAsynchRequestor::NewL(iLog,NULL);
		requestor->COMASuplPosHandlerBase_InitializeL();
		CActiveScheduler::Start();
		requestor->COMASuplPosHandlerBase_CreateNewSession_SuplObserverL();
		TInt err = requestor->GetResult();
		delete requestor;
		return err;
	}
	
// ========================== OTHER EXPORTED FUNCTIONS =========================
// None

//  End of File
