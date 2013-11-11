/*
* Copyright (c) 2005-2006 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  Provides logging utility.
*
*/

#include <e32std.h>
#include <e32cmn.h>
#include <utf.H>
#include <stiflogger.h>
#include <ecom/implementationinformation.h>
#include <epos_comasuplposition.h>
#include <epos_comasuplreqasstdata.h>
#include "epos_comasuplpostesterlogger.h"
#include <epos_comasuplsetcapabilities.h>
#include <epos_comasuplPosPayload.h>
/*
-------------------------------------------------------------------------------

    Class: COMASuplPosTesterLogger

    Method: NewL

    Description: Creats an object of COMASuplPosTesterLogger.

    This NewL method accepts several parameters.

    Parameters: const TDesC& aLogFile: in: Name of Log file.

    Return Values: COMASuplPosTesterLogger: Pointer to COMASuplPosTesterLogger object.

    Errors/Exceptions:  Leaves if called COMASuplPosTesterLogger::ConstructL method fails

    Status: Proposal

-------------------------------------------------------------------------------
*/
 COMASuplPosTesterLogger* COMASuplPosTesterLogger::NewL(const TDesC& aLogFile)
	{
	COMASuplPosTesterLogger* self = new(ELeave)COMASuplPosTesterLogger;
	CleanupStack::PushL(self);
	self->ConstructL(aLogFile);
	CleanupStack::Pop(self);
	return self;
	}

/*
-------------------------------------------------------------------------------

    Class: COMASuplPosTesterLogger

    Method: ~COMASuplPosTesterLogger

    Description: Destructor of COMASuplPosTesterLogger.

    Parameters:
    
    Return Values:

    Errors/Exceptions:

    Status: Proposal

-------------------------------------------------------------------------------
*/	
 COMASuplPosTesterLogger::~COMASuplPosTesterLogger()
	{
	delete iLogFileLogger;
	}


/*
-------------------------------------------------------------------------------

    Class: COMASuplPosTesterLogger

    Method: ConvertAsciiToHex

    Description: Converts ASCII data to Hexadecimal.

    Parameters: const TDesC8& aAscii: in: Data in ASCII format.
    			HBufC8 *& aHex: out: Buffer containing data in Hexadecimal format.

    Return Values: TInt: Non Zero value if an error occurs.

    Errors/Exceptions: 

    Status: Proposal

-------------------------------------------------------------------------------
*/	
 TInt COMASuplPosTesterLogger::ConvertAsciiToHex(const TDesC8& aAscii
 														, HBufC8 *& aHex)
	{
	const TInt hexValLength = 5;

	delete aHex;

	TRAPD(err, aHex = HBufC8::NewL(aAscii.Length() * hexValLength));
	if(KErrNone != err)
		{
		return err;
		}
	TPtr8 aHexPtr = aHex->Des();
	TInt sourceLength = aAscii.Length();

	TPtr8 hexPtr = aHex->Des();
	for(TInt count = 0; count < sourceLength; count++)
		{
		if(count > 0)
			{
			hexPtr.Append(KComma);
			}
		THexVal hexVal;
		ConvertCharToHex(aAscii[count], hexVal);
		hexPtr.Append(hexVal);
		}
	return 0;
	}

TInt COMASuplPosTesterLogger::ConvertCharToHex(TUint8 aAsciiChar
													, THexVal& aHexVal)
	{
	aHexVal.Format(KHexFormat, aAsciiChar);
	return KErrNone;
	}
/*
-------------------------------------------------------------------------------

    Class: COMASuplPosTesterLogger

    Method: WriteImplementationInfoL

    Description: Logs the information about Implementation.

    Parameters: const CImplementationInformation& aImplementationInfo: in:
    			Implementation information.

    Return Values: void

    Errors/Exceptions: Leaves if memory allocation fails.

    Status: Proposal

-------------------------------------------------------------------------------
*/	
 void COMASuplPosTesterLogger::
 WriteImplementationInfoL(
 					const CImplementationInformation& aImplementationInfo)
	{
	iLogFileLogger->WriteDelimiter();
	
	HBufC8* displayName = 	CnvUtfConverter::ConvertFromUnicodeToUtf8L(
									aImplementationInfo.DisplayName());
	CleanupStack::PushL(displayName);
	TInt len = displayName->Length();
	displayName = displayName->ReAllocL(len + KMaxHexSize);
	CleanupStack::Pop(displayName);
	CleanupStack::PushL(displayName);
	displayName->Des().ZeroTerminate();
	iLogFileLogger->Log(_L8("POS Name: %S"), displayName);
	CleanupStack::PopAndDestroy(displayName);
	
	HBufC8* posUid = CnvUtfConverter::ConvertFromUnicodeToUtf8L(
							aImplementationInfo.ImplementationUid().Name());
	CleanupStack::PushL(posUid);
	posUid->Des().ZeroTerminate();
	iLogFileLogger->Log(_L8("POS Uid: %S"), posUid);
	CleanupStack::PopAndDestroy(posUid);
	
	iLogFileLogger->Log(_L8("POS Version: %d"), aImplementationInfo.Version());
	_LIT8(KTestDateTime, "Test date and time : ");
	TBuf8<KDateTimeLength> dateAndTime;
	GetCurrentDateAndTime(dateAndTime);
	
	HBufC8* dateTime = HBufC8::NewL(KMaxBufLength);
	TPtr8 dateTimeInfo = dateTime->Des();
	dateTimeInfo.Copy(KTestDateTime);
	dateTimeInfo.Append(dateAndTime);
	WriteLine(dateTimeInfo);
	delete dateTime;

	iLogFileLogger->WriteDelimiter();
	}




/*
-------------------------------------------------------------------------------

    Class: COMASuplPosTesterLogger

    Method: WritePosTesterParametersL

    Description: Logs the parameters used for testing an Implementation.

    Parameters: void.

    Return Values: void

    Errors/Exceptions:

    Status: Proposal

-------------------------------------------------------------------------------
*/	
 void COMASuplPosTesterLogger::WritePosTesterParametersL()
	{
	iLogFileLogger->Log(_L8("POS Tester Parameters.\r\n"));
	iLogFileLogger->WriteDelimiter();
	}

/*
-------------------------------------------------------------------------------

    Class: COMASuplPosTesterLogger

    Method: WriteSummaryL

    Description: Logs the summary for an Implementation tested.

    Parameters: TInt aErrors: in: Number of errors.
    			TInt aWarnings: in: Number of warnings.
    			TInt aInfo: in: Number of informations.

    Return Values: void

    Errors/Exceptions:

    Status: Proposal

-------------------------------------------------------------------------------
*/	
 void COMASuplPosTesterLogger::WriteSummaryL(TTestingStatus aTestStatus
 											, TInt aInfo, TInt aWarnings
 											, TInt aErrors)
	{
	iLogFileLogger->WriteDelimiter();
	iLogFileLogger->WriteDelimiter();
	
	HBufC8* summaryBuf = HBufC8::NewL(KMaxBufLength);
	CleanupStack::PushL(summaryBuf);
	TPtr8 summary = summaryBuf->Des();
	
	HBufC8* summaryDataBuf = HBufC8::NewL(KMaxBufLength);
	CleanupStack::PushL(summaryDataBuf);
	TPtr8 summaryData = summaryDataBuf->Des();
	
	if(ETestNotStarted == aTestStatus)
		{
		summary.Copy(KTestNotStarted);
		}
	if(ETestComplete == aTestStatus)
		{
		summary.Copy(KTestComplete);
		}
	if(ETestCancelled == aTestStatus)
		{
		summary.Copy(KTestCanceled);
		}
	if(ETestAborted == aTestStatus)
		{
		summary.Copy(KTestAborted);
		}
	
	summary.Append(KExclamation());
	_LIT8(KFormatTxt," Number of Errors = %d | Number of Warnings = %d | Number of Infos = %d");
	summaryData.Format(KFormatTxt, aErrors, aWarnings, aInfo);
	summary.Append(summaryData);
	iLogFileLogger->Log(summary);
	iLogFileLogger->WriteDelimiter();
	iLogFileLogger->WriteDelimiter();
	
	CleanupStack::PopAndDestroy(summaryDataBuf);
	CleanupStack::PopAndDestroy(summaryBuf);
	}

/*
-------------------------------------------------------------------------------

    Class: COMASuplPosTesterLogger

    Method: GetCurrentDateAndTime

    Description: Gets current system date and time.

    Parameters: TBuf8<KdateTimeLength>& aDateAndTime: out: contains date and time.
    			
    Return Values: void

    Errors/Exceptions:

    Status: Proposal

-------------------------------------------------------------------------------
*/	
 void COMASuplPosTesterLogger::GetCurrentDateAndTime(TDes8& aDateAndTime)
	{
	TTime time;
	time.HomeTime();
	TDateTime dateTime;
	
	dateTime = time.DateTime();
	_LIT8(KFormatTxt,"%d/%d/%d  %d:%d:%d");
	aDateAndTime.Format(KFormatTxt, dateTime.Year(), dateTime.Month() + 1, dateTime.Day() + 1,
									dateTime.Hour(), dateTime.Minute(), dateTime.Second());
	}

 void COMASuplPosTesterLogger::PrepareDateAndTimeString(
 						TDes8& aDateAndTime
 						, TDateTime& aDateTime)
	{
	_LIT8(KFormatTxt,"%d/%d/%d  %d:%d:%d:%d");
	aDateAndTime.Format(KFormatTxt, aDateTime.Year(), aDateTime.Month() + 1
						, aDateTime.Day() + 1, aDateTime.Hour()
						, aDateTime.Minute(), aDateTime.Second()
						, aDateTime.MicroSecond());

	}
/*
-------------------------------------------------------------------------------

    Class: COMASuplPosTesterLogger

    Method: GetLogFileLogger

    Description: Gets object of CStifLogger for log file.

    Parameters: void.
    			
    Return Values: CStifLogger: Object of CStifLogger

    Errors/Exceptions:

    Status: Proposal

-------------------------------------------------------------------------------
*/	
 CStifLogger& COMASuplPosTesterLogger::GetLogFileLogger()
	{ 
	return *iLogFileLogger;
	}


/*
-------------------------------------------------------------------------------

    Class: COMASuplPosTesterLogger

    Method: WriteLine

    Description: Writes a line to log file.

    Parameters: TDesC8& aLine: in:line to be written.
    Parameters: TDesC8& aLine: in:Session Id to be added to the line.
    			
    Return Values: void

    Errors/Exceptions:

    Status: Proposal

-------------------------------------------------------------------------------
*/

void COMASuplPosTesterLogger::WriteLine(const TDesC8& aLine, TInt aSessionId)
	{
	if(aSessionId == 0)
		{
		iLogFileLogger->Log(aLine);
		return;
		}
	else
		{
		HBufC8* line = NULL;
		TInt err = KErrGeneral;

		TRAP(err, line = HBufC8::NewL(aLine.Length() 
											+ KExtraBuffer));
		if(err == KErrNone )
			{
			TPtr8 ptr = line->Des();
			ptr.Copy(aLine);
			ptr.Append(KSessionId);
			ptr.AppendNum(aSessionId);
			iLogFileLogger->Log(*line);
			}
		else
			{
			iLogFileLogger->Log(aLine);
			return;
			}
		delete line;
		}
	}

// ---------------------------------------------------------------------------
// Facilitates logging a method with status code.
// ---------------------------------------------------------------------------
//
 void COMASuplPosTesterLogger::WriteStatus(const TDesC8& aLine, TInt aErrCode
 									, TInt aSessionId, const TDesC8* aCode )
	{
	HBufC8* dataBuf = NULL;
	TRAPD(err, dataBuf = HBufC8::NewL(KMaxBufLength));
	if (err != KErrNone)
		{
		return;
		}
	TPtr8 data = dataBuf->Des();
	data.Copy(aLine);
	data.Append(*aCode);
	data.AppendNum(aErrCode);
	if(aSessionId != 0)
		{
		data.Append(KSessionId);
		data.AppendNum(aSessionId);
		}
	iLogFileLogger->Log(data);
	delete dataBuf;
	}

// ---------------------------------------------------------------------------
// Writes a "aContainer::aVar = aData" to file.
// ---------------------------------------------------------------------------
//
void COMASuplPosTesterLogger::LogData(const TDesC8& aContainer
							, const TDesC8& aVar, TInt aData)
	{
	HBufC8* containment = NULL;
	TRAPD(err, CreateStringL(aContainer, aVar, containment));
	if(err == KErrNone)
		{
		TPtr8 ptr = containment->Des();
		ptr.Append(KEquals);
		ptr.AppendNum(aData);
		WriteLine(ptr);
		delete containment;
		}
	else
		{
		TClassName varName;
		CreateString(aContainer, aVar, varName);
		varName.AppendNum(aData);
		WriteLine(varName);
		}
	}

// ---------------------------------------------------------------------------
// Writes a "aContainer::aVar = aData" to file.
// ---------------------------------------------------------------------------
//
void COMASuplPosTesterLogger::LogData(const TDesC8& aContainer
							, const TDesC8& aVar, const TDesC8& aData)
	{
	HBufC8* buffer = NULL;
	TInt length = aContainer.Length() + aVar.Length() + aData.Length()
			 + KExtraBuffer;
	TRAPD(err, buffer = HBufC8::NewL(length));
	if(err != KErrNone)
		{
		WriteLine(KCouldNotWrite);
		return;
		}
	TPtr8 data = buffer->Des();
	data.Copy(aContainer);
	data.Append(KScope);
	data.Append(aVar);
	data.Append(KEquals);
	data.Append(aData);
	WriteLine(data);
	delete buffer;
	}

// ---------------------------------------------------------------------------
// Provides a descriptor containing "aContainer::aVar".
// ---------------------------------------------------------------------------
//
void COMASuplPosTesterLogger::CreateStringL(const TDesC8& aContainer
							,const TDesC8& aVar, HBufC8*& aString)
	{
	delete aString;
	aString = NULL;
	aString = HBufC8::NewL(aContainer.Length() 
							 + aVar.Length() + KExtraBuffer);
	TPtr8 strPtr = aString->Des();
	strPtr.Copy(aContainer);
	strPtr.Append(KScope);
	strPtr.Append(aVar);
	}

// ---------------------------------------------------------------------------
// Provides a descriptor containing "aContainer::aVar".
// ---------------------------------------------------------------------------
//
void COMASuplPosTesterLogger::CreateString(const TDesC8& aContainer
						, const TDesC8& aVar, TClassName& aString)
	{
	TInt lengthReq = aContainer.Length() + aVar.Length()
					 + 2 * (KScope().Length()) + KExtraBuffer;
	if(lengthReq > KClassNameLength)
		{
		aString.Copy(aVar);
		}
	else
		{
		aString.Copy(aContainer);
		aString.Append(KScope);
		aString.Append(aVar);
		}
	}

// ---------------------------------------------------------------------------
// Provides a descriptor containing "aParent::aClass".
// ---------------------------------------------------------------------------
//
void COMASuplPosTesterLogger::CreateContainer(const TDesC8& aParent
							, const TDesC8& aClass, TContainer& aContainer)
	{
	if(0 == aParent.Length())
		{
		aContainer.Copy(aClass);
		}
	else
		{
		aContainer.Copy(aParent);
		if(aClass.Length() > 0)
			{
			aContainer.Append(KScope);
			aContainer.Append(aClass);
			}
		}
	}

// ---------------------------------------------------------------------------
// Logs an object of COMASuplReqAsstData.
// ---------------------------------------------------------------------------
//
void COMASuplPosTesterLogger::LogSuplReqAsstData(COMASuplReqAsstData* aData)
	{
	_LIT8(KReqAsstData, "COMASuplReqAsstData");
	TBool almanacReq = EFalse;
	TBool utcModel = EFalse;
	TBool ionModel = EFalse;
    TBool dgpsCorrect = EFalse;
    TBool refeLocation = EFalse;
    TBool referenceTimeRequested = EFalse;
    TBool acquisition = EFalse;
	TBool realTime = EFalse;
	
	aData->GetReqAsstData(almanacReq, utcModel, ionModel, dgpsCorrect
						, refeLocation, referenceTimeRequested, acquisition
						, realTime);
	LogData(KReqAsstData, _L8("iAlmanacRequested"), almanacReq);
	LogData(KReqAsstData, _L8("iUtcModelRequested"), utcModel);
	LogData(KReqAsstData, _L8("iIonosphericModelRequested"), ionModel);
	LogData(KReqAsstData, _L8("iDgpsCorrectionsRequested"), dgpsCorrect);
	LogData(KReqAsstData, _L8("iReferenceLocationRequested"), refeLocation);
	LogData(KReqAsstData, _L8("iReferenceTimeRequested"), referenceTimeRequested);
	LogData(KReqAsstData, _L8("iAcquisitionAssistance"), acquisition);
	LogData(KReqAsstData, _L8("iRealTimeIntegrityRequested"), realTime);
	
    //Log Navigation Model
    TOMASuplNavigationModel navModel;
    aData->GetNavigationData(navModel);
    LogSuplNavigationModel(KReqAsstData, navModel);
	}

// ---------------------------------------------------------------------------
// Logs an object of TOMASuplNavigationModel.
// ---------------------------------------------------------------------------
//
void COMASuplPosTesterLogger::LogSuplNavigationModel(const TDesC8& aContainer
								, TOMASuplNavigationModel& aNavModel)
	{
	TContainer container;
	CreateContainer(aContainer, _L8("TOMASuplNavigationModel"), container);
	
	TInt gpsWeek = KNoValue;
	TInt gpsToe = KNoValue;
	TInt nSAT = KNoValue;
    TInt toeLimit = KNoValue;
	aNavModel.GetNavigationModel(gpsWeek, gpsToe, nSAT, toeLimit );
	LogData(container, _L8("iGPSWeek"), gpsWeek);
	LogData(container, _L8("iGPSToe"), gpsToe);
	LogData(container, _L8("iNSat"), nSAT);
	LogData(container, _L8("iToeLimit"), toeLimit);
	
	//Log SatInfo array
	RArray<TOMASuplSatelliteInfoElement> aSatInfo;
	TOMASuplSatelliteInfoElement satInfoElt;
	aNavModel.GetSatInfoElement(aSatInfo);
	TInt eltCount = aSatInfo.Count();
	for(TInt counter = 0; counter < eltCount; counter++)
		{
		aNavModel.GetSatInfoElement(satInfoElt, counter);
		LogSuplSatelliteInfoElement(container, satInfoElt);
		}
	aSatInfo.Close();
	}

// ---------------------------------------------------------------------------
// Logs an object of TOMASuplSatelliteInfoElement.
// ---------------------------------------------------------------------------
//
void COMASuplPosTesterLogger::LogSuplSatelliteInfoElement(
									const TDesC8& aContainer
									, TOMASuplSatelliteInfoElement& aSatInfo)
	{
	TContainer container;
	CreateContainer(aContainer, _L8("TOMASuplSatelliteInfoElement"), container);
	
	TInt satId = KNoValue;
	TInt iode = KNoValue;
	aSatInfo.GetSatInfoElement(satId, iode);
	LogData(container, _L8("iSatId"), satId);
	LogData(container, _L8("iIode"), iode);
	}

// ---------------------------------------------------------------------------
// Logs an object of COMASuplSETCapabilities.
// ---------------------------------------------------------------------------
//
//Capabalities
void COMASuplPosTesterLogger::LogSuplSETCapabilities(COMASuplSETCapabilities*
														 aSetCap)
	{
	_LIT8(KReqAsstData, "COMASuplSETCapabilities");
	TOMASuplPosTechnology posTechnology;
	COMASuplSETCapabilities::TOMASuplPrefMethod    prefMethod;
  	TOMASuplPosProtocol   posProtocol;

  	aSetCap->GetSETCapabilities(posTechnology, prefMethod, posProtocol);
	
	LogSuplPosTechnology(KReqAsstData, posTechnology);
	LogSuplPrefMethod(KReqAsstData, prefMethod);
	LogSuplPosProtocol(KReqAsstData, posProtocol);
	}

// ---------------------------------------------------------------------------
// Logs an object of TOMASuplPosTechnology.
// ---------------------------------------------------------------------------
//
void COMASuplPosTesterLogger::LogSuplPosTechnology(const TDesC8& aContainer
									, TOMASuplPosTechnology& aPosTech)
	{
	TContainer container;
	CreateContainer(aContainer, _L8("TOMASuplPosTechnology"), container);
	
	TBool	 gpsSETAssisted = EFalse;
	TBool	 gpsSETBased = EFalse;
	TBool	 autonomousGps = EFalse;
	TBool	 fLT = EFalse;
	TBool	 eOTD = EFalse;
	TBool	 oTDOA = EFalse;
	
	aPosTech.GetPosTechnology(gpsSETAssisted, gpsSETBased, autonomousGps, fLT
								, eOTD, oTDOA);
	LogData(container, _L8("iGpsSETAssisted"), gpsSETAssisted);
	LogData(container, _L8("iGpsSETBased"), gpsSETBased);
	LogData(container, _L8("iAutonomousGps"), autonomousGps);
	LogData(container, _L8("iFLT"), fLT);
	LogData(container, _L8("ieOTD"), eOTD);
	LogData(container, _L8("ioTDOA"), oTDOA);

	}

// ---------------------------------------------------------------------------
// Logs enum COMASuplSETCapabilities::TOMASuplPrefMethod.
// ---------------------------------------------------------------------------
//
void COMASuplPosTesterLogger::LogSuplPrefMethod(const TDesC8& aContainer
										, TInt aPrefMethod)
	{
	TContainer container;
	CreateContainer(aContainer, _L8("TOMASuplPrefMethod"), container);
	
	LogData(container, _L8("iPrefMethod"), aPrefMethod);
	}

// ---------------------------------------------------------------------------
// Logs an object of TOMASuplPosProtocol.
// ---------------------------------------------------------------------------
//
void COMASuplPosTesterLogger::LogSuplPosProtocol(const TDesC8& aContainer
										, TOMASuplPosProtocol& aProtocol)
	{
	TContainer container;
	CreateContainer(aContainer, _L8("TOMASuplPosProtocol"), container);
	
	TOMASuplPosProtocol protocol;
	TBool tIA801 = EFalse; 
	TBool rRLP = EFalse;
	TBool rRC = EFalse;

	aProtocol.GetPosProtocol(tIA801, rRLP, rRC);
	
	LogData(container, _L8("iTIA801"), tIA801);
	LogData(container, _L8("iRRLP"), rRLP);
	LogData(container, _L8("iRRC"), rRC);
	}

// ---------------------------------------------------------------------------
// Logs an object of COMASuplPosition.
// ---------------------------------------------------------------------------
//
//Position
void COMASuplPosTesterLogger::LogSuplPosition(COMASuplPosition* aPosition)
	{
	_LIT8(KReqAsstData, "COMASuplPosition");
	TOMASuplUtcTime utcTime;
	TOMASuplPositionEstimate posEstimate;
	
	aPosition->GetPosition (utcTime, posEstimate);
	LogSuplUtcTime(KReqAsstData, utcTime);
	LogSuplPositionEstimate(KReqAsstData, posEstimate);
	
	COMASuplVelocity* velocity = NULL;
	TInt err = KErrGeneral;
	err = aPosition->GetVelocity(velocity);
	if(err == KErrNone)
		{
		LogSuplVelocity(KReqAsstData, velocity);
		}
	else
		{
		WriteStatus(_L8("Could not get iVelocity."), err);
		}
	}

// ---------------------------------------------------------------------------
// Logs an object of TOMASuplUtcTime.
// ---------------------------------------------------------------------------
//
void COMASuplPosTesterLogger::LogSuplUtcTime(const TDesC8& aContainer
								, TOMASuplUtcTime& aUtcTime)
	{
	TContainer container;
	CreateContainer(aContainer, _L8("TOMASuplUtcTime"), container);
	TBuf8<KDateTimeLength> dateAndTime(0);

	TDateTime timeStamp;
	TInt zoneCode = KNoValue;
	TInt zone = KNoValue;
	aUtcTime.GetUtcTime(timeStamp, zoneCode, zone);
	PrepareDateAndTimeString(dateAndTime, timeStamp);
	LogData(container, _L8("iTimeStamp"), dateAndTime);
	LogData(container, _L8("iZoneCode"), zoneCode);
	LogData(container, _L8("iZone"), zone);
	
	}

// ---------------------------------------------------------------------------
// Logs an object of TOMASuplPositionEstimate.
// ---------------------------------------------------------------------------
//
void COMASuplPosTesterLogger::LogSuplPositionEstimate(
													const TDesC8& aContainer
								, TOMASuplPositionEstimate& aPosEstimate)
	{
	TContainer container;
	CreateContainer(aContainer, _L8("TOMASuplPositionEstimate"), container);
	
	TInt err = KErrGeneral;
	//LogUncertainty
	TOMASuplUncertainty uncertainty;
	err = aPosEstimate.GetUncertainty(uncertainty);
	if(err == KErrNone)
		{
		LogUncertainty(container, uncertainty);
		}
	else
		{
		WriteStatus(_L8("Could not get iUncertainty."), err);
		}
	
	//Get Confidence
	TInt confidence = KNoValue;
	err = aPosEstimate.GetConfidence(confidence);
	if(err == KErrNone)
		{
		LogData(container,_L8("iConfidence"), confidence);
		}
	else
		{
		LogData(container,_L8("iConfidence"), KNotSet);
		}
	//LogAltitudeInfo
	TOMASuplAltitudeInfo altInfo;
	err = aPosEstimate.GetAltitudeInfo(altInfo);
	if(err == KErrNone)
		{
		LogAltitudeInfo(container, altInfo);
		}
	else
		{
		WriteStatus(_L8("Could not get iAltitudeInfo."), err);
		}
	
	//Log latitude sign, latitude and longitude.
	TOMASuplPositionEstimate::TOMASuplLatitudeSign latSign
									 = TOMASuplPositionEstimate::ENorth;
    TInt latitude = KNoValue;
    TInt longitude = KNoValue;
    TInt latitudeSign(latSign);
	aPosEstimate.GetPositionEstimate(latSign, latitude, longitude);
	LogLatLongLatSign(container, latitudeSign, latitude, longitude);
	}

// ---------------------------------------------------------------------------
// Logs an object of TOMASuplUncertainty.
// ---------------------------------------------------------------------------
//
void COMASuplPosTesterLogger::LogUncertainty(
											const TDesC8& aContainer
								, TOMASuplUncertainty& aUncertainty)
	{
	TContainer container;
	CreateContainer(aContainer, _L8("TOMASuplUncertainty"), container);

	TInt uncertaintySemiMajor = KNoValue;
    TInt uncertaintySemiMinor = KNoValue;
    TInt orientationMajorAxis = KNoValue;
    
    aUncertainty.GetUncertainty(uncertaintySemiMajor, uncertaintySemiMinor
    							, orientationMajorAxis);

	LogData(container, _L8("iUncertaintySemiMajor"), uncertaintySemiMajor);
	LogData(container, _L8("iUncertaintySemiMinor"), uncertaintySemiMinor);
	LogData(container, _L8("iOrientationMajorAxis"), orientationMajorAxis);
	}

// ---------------------------------------------------------------------------
// Logs an object of TOMASuplAltitudeInfo.
// ---------------------------------------------------------------------------
//
void COMASuplPosTesterLogger::LogAltitudeInfo(const TDesC8& aContainer
										, TOMASuplAltitudeInfo& aAltInfo)
	{
	TContainer container;
	CreateContainer(aContainer, _L8("TOMASuplAltitudeInfo"), container);

	TOMASuplAltitudeInfo::TOMASuplAltitudeDirection altitudeDirection;
    TInt altitude = KNoValue;
    TInt altitudeUncertainty = KNoValue;
    aAltInfo.GetAltitudeInfo(altitudeDirection, altitude
    									, altitudeUncertainty);
   	LogData(container, _L8("iAltitudeDirection"), altitudeDirection);
   	LogData(container, _L8("iAltitude"), altitude);
   	LogData(container, _L8("iAltitudeUncertainty"), altitudeUncertainty);

	}

// ---------------------------------------------------------------------------
// Logs elements in an object of TOMASuplAltitudeInfo.
// ---------------------------------------------------------------------------
//
void COMASuplPosTesterLogger::LogLatLongLatSign(
								const TDesC8& aContainer, TInt& aLatitudeSign
									, TInt& aLatitude, TInt& aLongitude)
	{
	TContainer container;
	CreateContainer(aContainer, KNULL, container);
	
	LogData(container, _L8("iLatitudeSign"), aLatitudeSign);
	LogData(container, _L8("iLongitude"), aLongitude);
	LogData(container, _L8("iLatitude"), aLatitude);
	}

// ---------------------------------------------------------------------------
// Logs an object of COMASuplVelocity.
// ---------------------------------------------------------------------------
//
void COMASuplPosTesterLogger::LogSuplVelocity(const TDesC8& aContainer
									, COMASuplVelocity* aVelocity)
	{
	TContainer container;
	CreateContainer(aContainer, _L8("COMASuplVelocity"), container);
	
	COMASuplHorizVelocity* velocity = aVelocity->Velocity();
	if(NULL == velocity)
		{
		WriteLine(_L8("Velocity is NULL"));
		return;
		}
	TOMASuplVelocityType velocityType = velocity->VelType();
	
	LogSuplVelocityType(container, velocityType);
	
	switch(velocityType)
		{
		case EHorizVelocity:
			{
			LogHorizVelocity(container, velocity);
			break;
			}
			
		case EHorizAndVertVelocity:
			{
			WriteLine(_L8("EHorizAndVertVelocity ."));
			COMASuplHorizAndVertVelocity* hVVelocity
					 = static_cast<COMASuplHorizAndVertVelocity*>(velocity);
			return LogSuplHorizAndVertVelocity(container, hVVelocity);
			}
			
		case EHorizUncertVelocity:
			{
			WriteLine(_L8("EHorizUncertVelocity ."));
			COMASuplHorizUncertVelocity* hUncertVelo =
						 static_cast<COMASuplHorizUncertVelocity*>(velocity);
			return LogSuplHorizUncertVelocity(container, hUncertVelo);
			}
			
		case EHorizAndVertUncertVelocity:
			{
			WriteLine(_L8("EHorizAndVertUncertVelocity ."));
			COMASuplHorizAndVertUncertVelocity* hvUncertVelo = 
				static_cast<COMASuplHorizAndVertUncertVelocity*>(velocity);
			return LogSuplHorizAndVertUncertVelocity(container
					, hvUncertVelo);
			}
			
		default:
			{
			WriteLine(_L8("ERROR Default in Log Velocity switch ."));
			break;
			}
		}
	}

// ---------------------------------------------------------------------------
// Logs an enum TOMASuplVelocityType.
// ---------------------------------------------------------------------------
//
void COMASuplPosTesterLogger::LogSuplVelocityType(const TDesC8& aContainer
									, TOMASuplVelocityType aVelocityType)
	{
	TContainer container;
	CreateContainer(aContainer, _L8("TOMASuplVelocityType"), container);
	
	LogData(container, _L8("iVelType"), aVelocityType);
	}

// ---------------------------------------------------------------------------
// Logs an object of COMASuplHorizVelocity.
// ---------------------------------------------------------------------------
//
void COMASuplPosTesterLogger::LogHorizVelocity(const TDesC8& aContainer
						, COMASuplHorizVelocity* aVelocity)
	{
	TContainer container;
	CreateContainer(aContainer, _L8("COMASuplHorizVelocity"), container);
	
	TUint16 bearing = KNoValue16;
	TUint16  horSpeed = KNoValue16;
	aVelocity->GetHorizVel(bearing, horSpeed);
	LogData(container, _L8("iBearing"), bearing);
	LogData(container, _L8("iHorSpeed"), horSpeed);
	}

// ---------------------------------------------------------------------------
// Logs an object of COMASuplHorizAndVertVelocity.
// ---------------------------------------------------------------------------
//
void COMASuplPosTesterLogger::LogSuplHorizAndVertVelocity(
							const TDesC8& aContainer
						, COMASuplHorizAndVertVelocity* aVelocity)
	{
	TContainer container;
	CreateContainer(aContainer, _L8("COMASuplHorizAndVertVelocity")
															, container);
	
	TUint16 bearing = KNoValue16;
	TUint16 horSpeed = KNoValue16;
	TUint8 verDirect = KNoValue8;
	TUint8 verSpeed = KNoValue8;
	
	aVelocity->GetHorizAndVertVel(bearing, horSpeed, verDirect, verSpeed);
	LogData(container, _L8("iBearing"), bearing);
	LogData(container, _L8("iHorSpeed"), horSpeed);
	LogData(container, _L8("iVertDirection"), verDirect);
	LogData(container, _L8("iVertSpeed"), verSpeed);
	}

// ---------------------------------------------------------------------------
// Logs an object of COMASuplHorizUncertVelocity.
// ---------------------------------------------------------------------------
//
void COMASuplPosTesterLogger::LogSuplHorizUncertVelocity(
								const TDesC8& aContainer
								, COMASuplHorizUncertVelocity* aVelocity)
	{
	TContainer container;
	CreateContainer(aContainer, _L8("COMASuplHorizUncertVelocity")
															, container);
	TUint16 bearing = KNoValue16;
	TUint16 horSpeed = KNoValue16;
	TUint8 uncertSpeed = KNoValue8;
	
	aVelocity->GetHorizUncertVel(bearing, horSpeed, uncertSpeed);
	LogData(container, _L8("iBearing"), bearing);
	LogData(container, _L8("iHorSpeed"), horSpeed);
	LogData(container, _L8("iUncertSpeed"), uncertSpeed);
	}

// ---------------------------------------------------------------------------
// Logs an object of COMASuplHorizAndVertUncertVelocity.
// ---------------------------------------------------------------------------
//
void COMASuplPosTesterLogger::LogSuplHorizAndVertUncertVelocity(
										const TDesC8& aContainer
			, COMASuplHorizAndVertUncertVelocity* aVelocity)
	{
	TContainer container;
	CreateContainer(aContainer, _L8("COMASuplHorizAndVertUncertVelocity")
															, container);
	
	TUint8 horUncertSpeed = KNoValue8;
	TUint8 verUncertSpeed = KNoValue8;
	
	TUint16 bearing = KNoValue16;
	TUint16 horSpeed = KNoValue16;
	TUint8 verDirect = KNoValue8;
	TUint8 verSpeed = KNoValue8;
	
	aVelocity->GetHorizVertUncertVel(bearing, horSpeed, verDirect
								, verSpeed, horUncertSpeed, verUncertSpeed);

	LogData(container, _L8("iBearing"), bearing);
	LogData(container, _L8("iHorSpeed"), horSpeed);
	LogData(container, _L8("iVertDirection"), verDirect);
	LogData(container, _L8("iVertSpeed"), verSpeed);
	LogData(container, _L8("iHorizUncertSpeed"), horUncertSpeed);
	LogData(container, _L8("iVertUncertSpeed"), verUncertSpeed);
	}

// ---------------------------------------------------------------------------
// Logs an object of COMASuplPosPayload.
// ---------------------------------------------------------------------------
//
void COMASuplPosTesterLogger::LogPayload(COMASuplPosPayload* aPayload)
	{
	HBufC8 * hexPayload = NULL;
	HBufC8* payload = NULL;
	
	COMASuplPosPayload::TOMASuplPosPayloadType payloadType;
	
	aPayload->GetPosPayload(payload, payloadType);
	iLogFileLogger->Log(_L8("COMASuplPosPayload::iPayloadType = %d")
											, payloadType);
	if(NULL == payload)
		{
		LogData(_L8("COMASuplPosPayload"), _L8("iPayload"), KNotSet);
		return;
		}
	TInt err = ConvertAsciiToHex(*payload, hexPayload);
	if(KErrNone == err)
		{
		LogData(_L8("COMASuplPosPayload"), _L8("iPayload"), *hexPayload);
		delete hexPayload;
		}
	else
		{
		LogData(_L8("COMASuplPosPayload"), _L8("iPayload"), *payload);
		}
	
	}

// ---------------------------------------------------------------------------
// Logs an object of COMASuplPosPayload.
// ---------------------------------------------------------------------------
//
void COMASuplPosTesterLogger::LogPayloadSent(COMASuplPosPayload* aPayload)
	{
	HBufC8* payload = NULL;
	
	COMASuplPosPayload::TOMASuplPosPayloadType payloadType;
	
	aPayload->GetPosPayload(payload, payloadType);
	
	iLogFileLogger->Log(_L8("COMASuplPosPayload::iPayloadType = %d")
										, payloadType);
	if(NULL == payload)
		{
		LogData(_L8("COMASuplPosPayload"), _L8("iPayload"), KNotSet);
		return;
		}
	LogData(_L8("COMASuplPosPayload"), _L8("iPayload"), *payload);
	}
// ---------------------------------------------------------------------------
// Logs an object of COMASuplInfoRequestList.
// ---------------------------------------------------------------------------
//
void COMASuplPosTesterLogger::LogRequestList(COMASuplInfoRequestList&
											 aReqList)
	{
	TInt listSize = aReqList.GetCount();
	iLogFileLogger->Log(_L8("*****************Logging Request List*****************"));
	for(TInt count = 0; count < listSize; count++)
		{
		COMASuplInfoRequest *request = aReqList.GetElement(count);
		iLogFileLogger->Log(_L8("++++++++Logging request list item index: %d ++++++++"), count);
		LogRequest(request);
		WriteLine(KReqLogObjEnd);
		}
	WriteLine(KReqLogEnd);
	}

// ---------------------------------------------------------------------------
// Logs an object of COMASuplInfoRequest.
// ---------------------------------------------------------------------------
//
void COMASuplPosTesterLogger::LogRequest(COMASuplInfoRequest* aReq)
	{
	TInt reqStatus = aReq->Status();
	iLogFileLogger->Log(_L8("COMASuplInfoRequest::iReqStatus = %d")
														, reqStatus);
	
	COMASuplInfoRequest::TOMASuplInfoType reqType = aReq->Type();
	
	iLogFileLogger->Log(_L8("COMASuplInfoRequest::iType = %d")
														, reqType);
	
	switch(reqType)
		{
		case COMASuplInfoRequest::EOMASuplReqAsstData:
			{
			LogSuplReqAsstData(REINTERPRET_CAST(COMASuplReqAsstData*, aReq));
			}
			break;
   		case COMASuplInfoRequest::EOMASuplVelocity:
   			{
   			LogSuplVelocity(KNULL, REINTERPRET_CAST(COMASuplVelocity*, aReq));
   			}
   			break;
   		case COMASuplInfoRequest::EOMASuplSETCapabilities:
   			{
   			LogSuplSETCapabilities(REINTERPRET_CAST(COMASuplSETCapabilities*, aReq));
   			}
   			break;
   		case COMASuplInfoRequest::EOMASuplPosPayload:
   			{
   			LogPayload(REINTERPRET_CAST(COMASuplPosPayload*, aReq));
   			}
   			break;
   		case COMASuplInfoRequest::EOMASuplPosition:
   			{
   			LogSuplPosition(REINTERPRET_CAST(COMASuplPosition*, aReq));
   			}
   			break;
		}
	}


/*
-------------------------------------------------------------------------------

    Class: COMASuplPosTesterLogger

    Method: COMASuplPosTesterLogger

    Description: Default constructor.

    Parameters: void

    Return Values:

    Errors/Exceptions:

    Status: Proposal

-------------------------------------------------------------------------------
*/
COMASuplPosTesterLogger::COMASuplPosTesterLogger()
	{
	iSyncRequestGiven = EFalse;
	}

/*
-------------------------------------------------------------------------------

    Class: COMASuplPosTesterLogger

    Method: ConstructL

    Description: Completes the construction of an object of COMASuplPosTesterLogger.

    This ConstructL method accepts several parameters.

    Parameters: const TDesC& aLogFile: in: Name of Log file.

    Return Values: void.

    Errors/Exceptions:  Leaves if called CStifLogger::NewL method fails

    Status: Proposal

-------------------------------------------------------------------------------
*/	
void COMASuplPosTesterLogger::ConstructL(const TDesC& aLogFile)
	{
	iLogFileLogger = CStifLogger::NewL(KLoggingDir, 
											aLogFile,
											CStifLogger::ETxt,// Logger type
                                            CStifLogger::EFile,// output is file
                                            EFalse,// overwrite is not used
                                            EFalse,// Time stamp is not used
                                            ETrue,// Line break is used
                                            EFalse,// Even ranking is not used
                                            EFalse,// Thread id is not added after the log file
                                            ETrue,// Log directory will create
                                            2048);//Buffer size
	
	}

// ---------------------------------------------------------------------------
// It should be called just before issuing a Synchronous request.
// Initializes iRequestStartTime with the current time.
// ---------------------------------------------------------------------------
//
void COMASuplPosTesterLogger::GetRequestStartTime()
	{
	iSyncRequestGiven = ETrue;
	iRequestStartTime.HomeTime();
	}

// ---------------------------------------------------------------------------
// It should be called just after the Synchronous request.
// Initializes iRequestEndTime with the current time.
// ---------------------------------------------------------------------------
//
void COMASuplPosTesterLogger::RequestExecuted()
	{
	if(iSyncRequestGiven)
		{
		iSyncRequestGiven = EFalse;
		iRequestEndTime.HomeTime();
		}
	else
		{
		iLogFileLogger->Log(_L8("Request Start time was NOT recorded."));
		}
	}

// ---------------------------------------------------------------------------
// Gets the difference between time obtained by GetRequestStartTime().
// and RequestExecuted into aDiff.
// ---------------------------------------------------------------------------
//
void COMASuplPosTesterLogger::GetTimeDifference(TTime& aDiff)
	{
	TTimeIntervalMicroSeconds microSecs = 0;
	microSecs = iRequestEndTime.MicroSecondsFrom(iRequestStartTime);
	aDiff = microSecs.Int64();
	}

// ---------------------------------------------------------------------------
// Logs aTime into the log file.
// ---------------------------------------------------------------------------
//
void COMASuplPosTesterLogger::LogTime(const TDesC8& aMsg, TTime& aTime)
	{
	TDateTime timeDiff = aTime.DateTime();
	TBuf8<KTimeDiffLength> timeDiffBuffer(0);
	timeDiffBuffer.AppendNum(timeDiff.Hour());
	timeDiffBuffer.Append(KHour);
	timeDiffBuffer.AppendNum(timeDiff.Minute());
	timeDiffBuffer.Append(KMins);
	timeDiffBuffer.AppendNum(timeDiff. Second());
	timeDiffBuffer.Append(KSecs);
	timeDiffBuffer.AppendNum(timeDiff.MicroSecond());
	timeDiffBuffer.Append(KMicroSecs);
	TClassName string;
	CreateString(aMsg, timeDiffBuffer, string);
	iLogFileLogger->Log(string);
	}

// ---------------------------------------------------------------------------
// Calculates time difference between iRequestStartTime and 
// iRequestEndTime and logs the difference into a log file.
// ---------------------------------------------------------------------------
//	
void COMASuplPosTesterLogger::LogExecutionTime(const TDesC8& aMsg)
	{
	TTime timeDiff;
	RequestExecuted();
	GetTimeDifference(timeDiff);
	LogTime(aMsg, timeDiff);
	}

// ---------------------------------------------------------------------------
// Writes a test header into the log file.
// ---------------------------------------------------------------------------
//
 void COMASuplPosTesterLogger::WriteTestHeader(const TDesC8& aTestHeader
 												, TInt aSessionId)
	{
	HBufC8* line = NULL;
	TInt err = KErrGeneral;
	if(aSessionId == 0)
		{
		iLogFileLogger->Log(aTestHeader);
		return;
		}
	else
		{
		TRAP(err, line = HBufC8::NewL(aTestHeader.Length() 
											+ KExtraBuffer));
		if(err == KErrNone )
			{
			TPtr8 ptr = line->Des();
			ptr.Copy(aTestHeader);
			ptr.Append(KSessionId);
			ptr.AppendNum(aSessionId);
			iLogFileLogger->Log(*line);
			}
		else
			{
			iLogFileLogger->Log(aTestHeader);
			return;
			}
		delete line;
		}
	iLogFileLogger->WriteDelimiter();
	}
