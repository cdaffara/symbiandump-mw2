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


#ifndef C_COMASUPLPOSTESTERLOGGER_H
#define C_COMASUPLPOSTESTERLOGGER_H

#include <e32base.h>
#include <e32des8.h>
#include <epos_comasuplvelocity.h>
#include "epos_comasuplpostestercategory.h"
class CImplementationInformation;
class CStifLogger;
class TTime;
class COMASuplReqAsstData;
class TOMASuplNavigationModel;
class COMASuplSETCapabilities;
class TOMASuplPosTechnology;   
class TOMASuplPrefMethod;
class TOMASuplPosProtocol;
class TOMASuplSatelliteInfoElement;
class COMASuplPosition;
class TOMASuplUtcTime;
class TOMASuplPositionEstimate;
class COMASuplVelocity;
class COMASuplHorizAndVertVelocity;
class COMASuplHorizUncertVelocity;
class COMASuplHorizAndVertUncertVelocity;
class COMASuplPosPayload;
class COMASuplInfoRequestList;
class TOMASuplUncertainty;
class TOMASuplAltitudeInfo;

/** Maximum length of the buffer holding date and time. */
const TInt KDateTimeLength = 32;

/** Maximum length of the buffer holding time difference. */
const TInt KTimeDiffLength = 64;

/** Maximum length of the buffer on stack. */
const TInt KMaxBufLength = 512;

/** Maximum length of the buffer holding class name. */
const TInt KClassNameLength = 512;

/** Maximum length of the buffer that can be used as extra buffer */
const TInt KExtraBuffer = 32;

/** Value user to initialize integers */
const TUint16 KNoValue16 = 9999;
const TInt KNoValue = -99999;
const TUint8 KNoValue8 = 127;

/** Maximum length of the buffer holding a hexadecimal value for an ASCII */
const TInt KMaxHexSize = 6;

/** Provides a buffer to hold class name. */
typedef TBuf8<KClassNameLength> TClassName;

/** Provides a buffer to hold list of all parent classes in the hierarchy. */
typedef TBuf8<KMaxBufLength> TContainer;

/** Provides a buffer to hold hexadecimal equivalent of an ASCII*/
typedef TBuf8<KMaxHexSize> THexVal;

/** Path for log file.*/
_LIT(KLoggingDir, "c:\\logs\\postester\\");

/** Path for log folder relative to c:\logs.*/
_LIT(KDirName, "postester");

/** Separator between two objects logged in the file. */
_LIT8(KReqLogObjEnd, "+++++++++++++++++++++++++++++++++++++++++++++++++++");

/** End of request list logged in the file. */
_LIT8(KReqLogEnd, "***************************************************");

/** Constant used for logging*/
_LIT8(KSendingPosMsg, "Info: Sending POS Message to Message plugin.");

/** Constant used for logging*/
_LIT8(KSessionInitialized, "Info: POS Session Initialized.");

/** Constant used for logging*/
_LIT8(KInitializingSession, "Info: Initializing POS Session.");

/** Constant used for logging*/
_LIT8(KInitializingHandler, "Info: Initializing POS Handler.");

_LIT8(KInitializedHandler, "Info: PosHandler Initialized.");

/** Constant used for logging*/
_LIT8(KSessionInitializeFail, "Error: Could not initialize POS Session.");

/** Constant used for logging*/
_LIT8(KHandlerCreateFail, "Error: Could not create POS Handler.");

/** Constant used for logging*/
_LIT8(KHandlerInitFail, "Error: Could not initialize POS Handler.");

/** Constant used for logging*/
_LIT8(KSessionCreated, "Info: POS Session Created.");

/** Constant used for logging*/
_LIT8(KCreatingSession, "Info: Creating POS Session.");

/** Constant used for logging*/
_LIT8(KCreatingSessionFail,  "Error: Could not Create New Session." );

/** Constant used for logging*/
_LIT8(KSpace, " ");

/** Constant used for logging*/
_LIT8(KComma, ",");

/** Constant used formatting ASCII to hexadecimal*/
_LIT8(KHexFormat, "%03x");

/** Constant used for logging*/
_LIT8(KNotSet, " Not Set");

/** Constant used for logging*/
_LIT8(KErrCode, " Code = ");

/** Constant used for logging*/
_LIT8(KTrue, " True");

/** Constant used for logging*/
_LIT8(KFalse, " False");

/** Constant used for logging*/
_LIT8(KScope, "::");

/** Constant used for logging*/
_LIT8(KEquals, " = ");

/** Constant used for logging*/
_LIT8(KHour, " Hours, ");

/** Constant used for logging*/
_LIT8(KMins, " Mins, ");

/** Constant used for logging*/
_LIT8(KSecs, " Secs, ");

/** Constant used for logging*/
_LIT8(KMicroSecs, " MicroSecs ");

/** Constant used for logging*/
_LIT8(KNULL, "");

/** Constant used for logging*/
_LIT8(KExclamation, "!");

/** Constant used for logging*/
_LIT8(KUserCancelsTest, "Info: Testing canceled by user.");

/** Constant used for logging*/
_LIT8(KExecHandlerNewL, "Info: Execution Time of COMASuplPosHandlerBase::NewL()");

/** Constant used for logging*/
_LIT8(KExecCreateNewSessionL, "Info: Execution Time of COMASuplPosHandlerBase::CreateNewSessionL()");

/** Constant used for logging*/
_LIT8(KTestAbored, "Test Aborted.");

/** Constant used for logging*/
_LIT8(KSessionEnd, "Info: Ending POS Session.");

/** Constant used for logging status of test*/
_LIT8(KTestNotStarted, "Test Not Started");

/** Constant used for logging information about test*/
_LIT8(KInitializeErrStatus, "Error: COMASuplPosSessionBase::InitializeL completed with ");

/** Constant used for logging status of test*/
_LIT8(KTestComplete, "Testing Complete");

/** Constant used for logging status of test*/
_LIT8(KTestCanceled, "Testing Canceled");

/** Constant used for logging status of test*/
_LIT8(KTestAborted, "Error: Testing Aborted");

/** Constant used for logging.*/
_LIT8(KPosSessionInActive, "Warning: POS Session Inactive.");

/** Constant used for logging.*/
_LIT8(KMsgFileMsgExhausted, "Error: Messages in the Message file exhausted.");

/** Constant used for logging.*/
_LIT8(KEndingSession, "Info: Ending POS Session.");

/** Constant used for logging.*/
_LIT8(KCouldNotWrite, "Could not write data.");

/** Constant used for logging.*/
_LIT8(KLeaveInHandlerCreation,"Error: Leave occured in POS Handler creation.\
 Leave");

/** Constant used for logging.*/
_LIT8(KLeaveInHandlerInit,"Error: Leave occured in POS Handler Initialization\
. Leave");

/** Constant used for logging.*/
_LIT8(KLeaveInCreateNewSession,"Error: Leave occured in POS Session creation\
. Leave");

/** Constant used for logging.*/
_LIT8(KLeaveOccured,"Error: Leave occured. Leave");

/** Constant used for logging.*/
_LIT8(KLeaveRequestListCreation,"Error: Leave occured while creating request\
 list. Leave");
 
 /** Constant used for logging.*/
_LIT8(KLeaveResettingPosition,"Error: Leave occured while resetting Position\
. Leave");

/** Constant used for logging.*/
_LIT8(KLeaveInSessionInit,"Error: Leave occured in POS Session \
Initialization. Leave");

/** Constant used for logging.*/
_LIT8(KLeaveInGetSuplInfo,"Error: Leave occured in GetSuplInfoL(). Leave");

/** Constant used for logging.*/
_LIT8(KLeaveInGetPosition,"Error: Leave occured in GetPositionL(). Leave");

/** Constant used for logging.*/
_LIT8(KReqCompWith,"Request completed with code: ");

/** Constant used for logging.*/
_LIT8(KSessionInitResult, "Session Initialization completed with ");

/** Constant used for logging.*/
_LIT8(KInfo,"Info: ");

/** Constant used for logging.*/
_LIT8(KWarning,"Warning: ");

/** Constant used for logging.*/
_LIT8(KError,"Error: ");

/** Constant used for logging.*/
_LIT8(KSessionId,"--SessionId: ");
/**
 *  Provides logging facility.
 *
 *  Provides functionality to log parameters, messages and result.
 *
 *  @lib epos_comasuplpostestermodulecfg.lib
 *  @since S60 v3,1u
 */
class COMASuplPosTesterLogger: public CBase
	{
	public:	// Constructors and destructor

		 static COMASuplPosTesterLogger* NewL(const TDesC& aLogFile);
		 virtual ~COMASuplPosTesterLogger();
		
	public:		//New functions

		/**
	     * Converts 8 bit ASCII information to Hexadecimal.
	     *
	     * @since S60 v3.1u
	     * @param aAscii Data in ASCII.
	     * @param aHex Data in Hexadecimal.
	     * @return TInt Error code.
	     */

		 TInt ConvertAsciiToHex(const TDesC8& aAscii, HBufC8*& aHex);
		 
		/**
	     * Converts 8 bit ASCII character to Hexadecimal.
	     *
	     * @since S60 v3.1u
	     * @param aAsciiChar Character in ASCII.
	     * @param aHexVal  Hexadecimal value of aAsciiChar.
	     * @return TInt Error code.
	     */
         TInt ConvertCharToHex(TUint8 aAsciiChar, THexVal& aHexVal);

		/**
	     * Writes DisplayName, Uid & Version of implementation into the logfile.
	     *
	     * @since S60 v3.1u
	     * @param aImplementationInfo An implementation to be logged.
	     * @return void
	     */
		 void WriteImplementationInfoL(
		 			const CImplementationInformation& aImplementationInfo);
		
		/**	
	     * Writes information in resource file into logfile .
	     *
	     * @since S60 v3.1u
	     * @return void
	     */
         void WritePosTesterParametersL();
        
		/**
	     * Writes summary of testing into the logfile.
	     *
	     * @since S60 v3.1u
	     * @param aErrors Number of errors messages.
	     * @param aWarnings Number of warnings messages.
	     * @param aInfo Number of info messages.
	     * @return void
	     */
		 void WriteSummaryL(TTestingStatus aTestStatus, TInt aInfo
		 					, TInt aWarnings, TInt aErrors);
		
		/**
	     * Gets current date & time into the argument.
	     *
	     * @since S60 v3.1u
	     * @param aDateAndTime Gets current date an time.
	     * @return void
	     */
         void GetCurrentDateAndTime(TDes8& aDateAndTime);

        /**
	     * Converts TDateTime object into a descriptor.
	     *
	     * @since S60 v3.1u
	     * @param aDateAndTime Gets aDateTime as a string.
	     * @param aDateAndTime TDateTime object whose data is required as a string.
	     * @return void
	     */
         void PrepareDateAndTimeString(TDes8& aDateAndTime
 										, TDateTime& aDateTime);
        
		/**
	     * Gets LogFile logger.
	     *
	     * @since S60 v3.1u
	     * @return CStifLogger& Reference to StifLogger.
	     */
         CStifLogger& GetLogFileLogger();
        
		/**
	     * Writes a line to file. If enough spcce is provided, appends
	     * aSessionId to aLine.
	     *
	     * @since S60 v3.1u
	     * @param aLine Line to be written in the log file.
	     * @param aSessionId Session number. Used for multiple session test.
	     * @return void
	     */
         void WriteLine(const TDesC8& aLine, TInt aSessionId = 0);
         
        /**
	     * Writes a "aLine Error code = ErrCode" to file.
	     *
	     * @since S60 v3.1u
	     * @param aLine Line to be written in the log file.
	     * @param aErrCode Error code to be logged.
	     * @return void
	     */
         void WriteStatus(const TDesC8& aLine, TInt aErrCode
         							, TInt aSessionId = 0
         							, const TDesC8* aCode = &KErrCode());
         
        /**
	     * Writes a "aContainer::aVar = aData" to file.
	     *
	     * @since S60 v3.1u
	     * @param aContainer Descriptor to be written in the log file.
	     * @param aVar Name of the variable to be written in the log file.
	     * @param aData Integer value of the variable.
	     * @return void
	     */
         void LogData(const TDesC8& aContainer
         							, const TDesC8& aVar, TInt aData);

        /**
	     * Writes a "aContainer::aVar = aData" to file.
	     *
	     * @since S60 v3.1u
	     * @param aContainer Descriptor to be written in the log file.
	     * @param aVar Name of the variable to be written in the log file.
	     * @param aData Descriptor value to be written in the log file.
	     * @return void
	     */
		 void LogData(const TDesC8& aContainer, const TDesC8& aVar
					, const TDesC8& aData);

		/**
	     * Provides a descriptor containing "aContainer::aVar".
	     *
	     * @since S60 v3.1u
	     * @param aContainer Descriptor to be written in the log file.
	     * @param aVar Name of the variable to be written in the log file.
	     * @param aString Descriptor containing "aContainer::aVar".
	     * @return void
	     */
		 void CreateStringL(const TDesC8& aContainer
									, const TDesC8& aVar, HBufC8*& aString);

		/**
	     * Provides a descriptor containing "aContainer::aVar".
	     *
	     * @since S60 v3.1u
	     * @param aContainer Descriptor to be written in the log file.
	     * @param aVar Name of the variable to be written in the log file.
	     * @param aString Descriptor containing "aContainer::aVar". If the
	     * length of the descriptor is less than length(aContainer + aVar + 4)
	     * it only contains aVar.
	     * @return void
	     */
		 void CreateString(const TDesC8& aContainer
							, const TDesC8& aVar, TClassName& aString);

		/**
	     * Provides a descriptor containing "aParent::aClass".
	     *
	     * @since S60 v3.1u
	     * @param aParent Descriptor to be written in the log file.
	     * @param aClass Name of the variable to be written in the log file.
	     * @param aContainer Descriptor containing "aParent::aClass".
	     * @return void
	     */
		 void CreateContainer(const TDesC8& aParent
							, const TDesC8& aClass, TContainer& aContainer);
         
         /**
	     * Logs an object of COMASuplReqAsstData.
	     *
	     * @since S60 v3.1u
	     * @param aData Object to be written in the log file.
	     * @return void
	     */
         void LogSuplReqAsstData(COMASuplReqAsstData* aData);

		/**
	     * Logs an object of TOMASuplNavigationModel.
	     *
	     * @since S60 v3.1u
	     * @param aContainer Name of object containing aNavModel.
	     * @param aNavModel Object to be written in the log file.
	     * @return void
	     */
         void LogSuplNavigationModel(const TDesC8& aContainer
         							, TOMASuplNavigationModel& aNavModel);

		/**
	     * Logs an object of TOMASuplSatelliteInfoElement.
	     *
	     * @since S60 v3.1u
	     * @param aContainer Name of object containing aSatInfo.
	     * @param aSatInfo Object to be written in the log file.
	     * @return void
	     */
         void LogSuplSatelliteInfoElement(const TDesC8& aContainer
         						, TOMASuplSatelliteInfoElement& aSatInfo);

        /**
	     * Logs an object of COMASuplSETCapabilities.
	     *
	     * @since S60 v3.1u
	     * @param aSetCap Object to be written in the log file.
	     * @return void
	     */
         void LogSuplSETCapabilities(COMASuplSETCapabilities* aSetCap);

		/**
	     * Logs an object of TOMASuplPosTechnology.
	     *
	     * @since S60 v3.1u
	     * @param aContainer Name of object containing aPosTech.
	     * @param aPosTech Object to be written in the log file.
	     * @return void
	     */
         void LogSuplPosTechnology(const TDesC8& aContainer
         							, TOMASuplPosTechnology& aPosTech);

		/**
	     * Logs enum COMASuplSETCapabilities::TOMASuplPrefMethod.
	     *
	     * @since S60 v3.1u
	     * @param aContainer Name of object containing aPosTech.
	     * @param aPrefMethod Enum value to be written in the log file.
	     * @return void
	     */
         void LogSuplPrefMethod(const TDesC8& aContainer
         		, TInt aPrefMethod);

		/**
	     * Logs an object of TOMASuplPosProtocol.
	     *
	     * @since S60 v3.1u
	     * @param aContainer Name of object containing aProtocol.
	     * @param aProtocol Object to be written in the log file.
	     * @return void
	     */
         void LogSuplPosProtocol(const TDesC8& aContainer
         						, TOMASuplPosProtocol& aProtocol);
         
        /**
	     * Logs an object of COMASuplPosition.
	     *
	     * @since S60 v3.1u
	     * @param aPosition Object to be written in the log file.
	     * @return void
	     */
         void LogSuplPosition(COMASuplPosition* aPosition);
        
        /**
	     * Logs an object of TOMASuplUtcTime.
	     *
	     * @since S60 v3.1u
	     * @param aContainer Name of object containing aUtcTime.
	     * @param aUtcTime Object to be written in the log file.
	     * @return void
	     */
         void LogSuplUtcTime(const TDesC8& aContainer
         					, TOMASuplUtcTime& aUtcTime);

        /**
	     * Logs an object of TOMASuplPositionEstimate.
	     *
	     * @since S60 v3.1u
	     * @param aContainer Name of object containing aPosEstimate.
	     * @param aPosEstimate Object to be written in the log file.
	     * @return void
	     */
         void LogSuplPositionEstimate(const TDesC8& aContainer
         				, TOMASuplPositionEstimate& aPosEstimate);

		/**
	     * Logs an object of TOMASuplUncertainty.
	     *
	     * @since S60 v3.1u
	     * @param aContainer Name of object containing aUncertainty.
	     * @param aUncertainty Object to be written in the log file.
	     * @return void
	     */
         void LogUncertainty(const TDesC8& aContainer
								, TOMASuplUncertainty& aUncertainty);

		/**
	     * Logs an object of TOMASuplAltitudeInfo.
	     *
	     * @since S60 v3.1u
	     * @param aContainer Name of object containing aAltInfo.
	     * @param aAltInfo Object to be written in the log file.
	     * @return void
	     */
		void LogAltitudeInfo(const TDesC8& aContainer
								, TOMASuplAltitudeInfo& aAltInfo);

		/**
	     * Logs elements in an object of TOMASuplAltitudeInfo.
	     *
	     * @since S60 v3.1u
	     * @param aContainer Name of object containing aAltInfo.
	     * @param aLatitudeSign Value is either 0 or 1.
	     * @param aLatitude Value of latitude.
	     * @param aLongitude Value of longitude.
	     * @return void
	     */
		void LogLatLongLatSign(const TDesC8& aContainer, TInt& aLatitudeSign
									, TInt& aLatitude, TInt& aLongitude);

		/**
	     * Logs an object of COMASuplVelocity.
	     *
	     * @since S60 v3.1u
	     * @param aContainer Name of object containing aVelocity.
	     * @param aVelocity Object to be written in the log file.
	     * @return void
	     */
         void LogSuplVelocity(const TDesC8& aContainer
         							, COMASuplVelocity* aVelocity);
         
        /**
	     * Logs an enum TOMASuplVelocityType.
	     *
	     * @since S60 v3.1u
	     * @param aContainer Name of object containing aVelocityType.
	     * @param aVelocityType Object to be written in the log file.
	     * @return void
	     */
         void LogSuplVelocityType(const TDesC8& aContainer
         				, TOMASuplVelocityType aVelocityType);

        /**
	     * Logs an object of COMASuplHorizVelocity.
	     *
	     * @since S60 v3.1u
	     * @param aContainer Name of object containing aVelocity.
	     * @param aVelocity Object to be written in the log file.
	     * @return void
	     */ 				
         void LogHorizVelocity(const TDesC8& aContainer
								, COMASuplHorizVelocity* aVelocity);

		/**
	     * Logs an object of COMASuplHorizAndVertVelocity.
	     *
	     * @since S60 v3.1u
	     * @param aContainer Name of object containing aVelocity.
	     * @param aVelocity Object to be written in the log file.
	     * @return void
	     */
         void LogSuplHorizAndVertVelocity(const TDesC8& aContainer
         		, COMASuplHorizAndVertVelocity* aVelocity);

		/**
	     * Logs an object of COMASuplHorizUncertVelocity.
	     *
	     * @since S60 v3.1u
	     * @param aContainer Name of object containing aVelocity.
	     * @param aVelocity Object to be written in the log file.
	     * @return void
	     */
         void LogSuplHorizUncertVelocity(const TDesC8& aContainer
         						, COMASuplHorizUncertVelocity* aVelocity);

		/**
	     * Logs an object of COMASuplHorizAndVertUncertVelocity.
	     *
	     * @since S60 v3.1u
	     * @param aContainer Name of object containing aVelocity.
	     * @param aVelocity Object to be written in the log file.
	     * @return void
	     */
         void LogSuplHorizAndVertUncertVelocity(const TDesC8& aContainer
         				, COMASuplHorizAndVertUncertVelocity* aVelocity);
         
        /**
	     * Logs an object of COMASuplPosPayload, after converting 
	     * it to Hexadecimal.
	     *
	     * @since S60 v3.1u
	     * @param aPayload Object to be written in the log file.
	     * @return void
	     */
         void LogPayload(COMASuplPosPayload* aPayload);

        /**
	     * Logs an object of COMASuplPosPayload.
	     *
	     * @since S60 v3.1u
	     * @param aPayload Object to be written in the log file.
	     * @return void
	     */
         void LogPayloadSent(COMASuplPosPayload* aPayload);


        /**
	     * Logs an object of COMASuplInfoRequestList.
	     *
	     * @since S60 v3.1u
	     * @param aPayload Object to be written in the log file.
	     * @return void
	     */ 
         void LogRequestList(COMASuplInfoRequestList& aReqList);

        /**
	     * Logs an object of COMASuplInfoRequest.
	     *
	     * @since S60 v3.1u
	     * @param aPayload Object to be written in the log file.
	     * @return void
	     */ 
         void LogRequest(COMASuplInfoRequest* aReq);

        
	private:
	
        COMASuplPosTesterLogger();
        void ConstructL(const TDesC& aLogFile);
    public:
    	/**
	     * It should be called just before issuing a Synchronous request.
	     * Initializes iRequestStartTime with the current time.
	     *
	     * @since S60 v3.1u
	     * @return void
	     */
        void GetRequestStartTime();
        
        /**
	     * It should be called just after the Synchronous request.
	     * Initializes iRequestEndTime with the current time.
	     *
	     * @since S60 v3.1u
	     * @return void
	     */
        void RequestExecuted();
        
        /**
	     * Gets the difference between time obtained by GetRequestStartTime().
	     * and RequestExecuted into aDiff.
	     * @since S60 v3.1u
	     * @param aLine Line to be written in the log file.
	     * @return void
	     */
        void GetTimeDifference(TTime& aDiff);
        
        /**
	     * Logs aTime into the log file.
	     *
	     * @since S60 v3.1u
	     * @param aLine Line to be written in the log file.
	     * @return void
	     */
        void LogTime(const TDesC8& aMsg, TTime& aTime);
        
        /**
	     * Calculates time difference between iRequestStartTime and 
	     * iRequestEndTime and logs the difference into a log file.
	     *
	     * @since S60 v3.1u
	     * @param aLine Line to be written in the log file.
	     * @return void
	     */
        void LogExecutionTime(const TDesC8& aMsg);
        
        /**
	     * Writes a test header into the log file.
	     *
	     * @since S60 v3.1u
	     * @param aLine Line to be written in the log file.
	     * @return void
	     */
        void WriteTestHeader(const TDesC8& aTestHeader, TInt aSessionId = 0);

	private:
		/**
         * Object for Logfile operations.
         * Own.
         */
        CStifLogger* iLogFileLogger;
        
        /**
     	* ETrue if Start time is recorded.
     	*/
        TBool iSyncRequestGiven;
        
        /**
     	* Start time of request.
     	*/
        TTime iRequestStartTime;
        
        /**
     	* End time of request.
     	*/
        TTime iRequestEndTime;

	};
#endif