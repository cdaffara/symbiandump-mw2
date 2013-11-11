/*
* Copyright (c) 2002-2009 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  S60 SUPL Terminal Initiation API
*
*/

/*! \file epos_suplterminal.h
    \brief S60 SUPL Terminal Initiation API
*/

#ifndef __EPOS_SUPLTERMINAL_H__
#define __EPOS_SUPLTERMINAL_H__

#include <e32std.h>
#include <lbspositioninfo.h>
#include <epos_suplterminalqop.h>
#include <epos_suplterminaltrigger.h>
#include <epos_suplparameters.h>


// Forward declarations
class CSuplSubSessnPtrHolder;
class CSuplSettings;
class CSuplTerminalParams;


/**
 * This class is used to establish connection and to close connection
 * to terminal resident SUPL Server.  This class contains methods for 
 * connecting to SUPL Server, to close connection to SUPL Server and 
 * get SUPL Terminal Initiation API version number. 
 * After the connection has been established, its handle is passed as a 
 * parameter to the Open method of RSuplTerminalSubSession to create 
 * a "sub-session".
 * Client should close connection to SUPL Server when connection is no longer
 * required.
 */
class RSuplTerminalServer : public RSessionBase
	{
public:
    /**
     * Constructor for RSuplTerminalServer
     */
	IMPORT_C RSuplTerminalServer();

    /**
     * Creates a connection with  SUPL Server. If there is no other
     * connection when this is called, the method will start the server.
     * If client tries to connect again with the same instance before 
     * closing previous connection, it panics with panic code 
     * ESuplServerHandleNotClosed.
     *
     * @return following error codes 
     *
     *      - KErrNone when a link has been successfully established with SUPL Server.
     *      - KErrNotFound if server executable is not found to start a SUPL Server
     *      - KErrNotSupported if version of client API is not compatible 
     *		  with SUPL Server or if SUPL feature is not enabled in the terminal.
     *      - KErrPermissionDenied if client does not have CommDD capability
     */
	IMPORT_C TInt Connect();

    /**
     * Closes a connection with the SUPL Server. A client must 
     * ensure that all sub-sessions are closed and all outstanding requests 
     * are cancelled before the connection to SUPL Server is closed.
     */
	IMPORT_C void Close();

    /**
     * Obtains the current version number of the SUPL Terminal Initiation API.
     *
     * @return the version of the client API.
     */
	IMPORT_C TVersion Version() const;


	
    /**
     * Asynchronous method to creates a connection with SUPL Server. If there is no other
     * connection when this is called, the method will start the server.
     * If client tries to connect again with the same instance before 
     * closing previous connection, it panics with panic code 
     * ESuplServerHandleNotClosed.
     *
     * This method can complete the request issued by client with the following error codes 
     *      - KErrNone when a link has been successfully established with SUPL Server.
     *      - KErrNotFound if server executable is not found to start a SUPL Server
     *      - KErrNotSupported if version of client API is not compatible 
     *		  with SUPL Server or if SUPL feature is not enabled in the terminal.
     *      - KErrPermissionDenied if client does not have CommDD capability
     */

	
	IMPORT_C TInt Connect(TRequestStatus& aStatus);
private:
	void ConstructL();
	void Destruct();
	static TInt StartServer();

private:
	/** Unused variable for future expansion. */
	TAny* iReserved;
	};

/**
 * This class is used to make request to determine the position information, 
 * to request additional position information such as assistance data for A-GPS 
 * and to retrieve position information. Client can optionally specify Quality
 * of Position (QoP) while requesting for position determination. 
 * Before using the class, a connection must have already been established with the 
 * SUPL Server. This class also has methods to open subsession, 
 * to cancel outstanding position determination request and to close 
 * sub-session to SUPL Server.
 * All methods in this class, except Close, panics with ESuplServerBadHandle if
 * connection to SUPL Server is not established.
 *
 * @see RSuplTerminalServer
 */
class RSuplTerminalSubSession : public RSubSessionBase
	{
	
public :
	/**
	 * For differentiating request between Supl Service Versions 
	 *
	 */
	enum TSuplServiceType
	    {
	    ESUPL_1_0 = 1,
	    ESUPL_2_0
	    }; 
   
public:
    /**
     * Constructor for RSuplTerminalSubSession
     *
     */
	IMPORT_C RSuplTerminalSubSession();

    /**
     * Opens a sub-session to the SUPL Server. 
     * Client can request for position determination, can request for procedure to
     * get assistance data and retrieve position information through sub-session.
     * Panic ESuplServerHandleNotClosed occurs if sub-session is already opened 
     * @param [IN] aSuplServer is a connected session with SUPL Server.
     * @param [IN] aSuplService is a supl service version to be used 
     * @return Following error codes:
     *      -  KErrNone if subsession creation is sucessful.
     *      -  KErrServerBusy if SUPL Server has taken too long to accept a request
     *      -  KErrPermissionDenied if client does not have CommDD capability
     *      -  KErrNoMemory if there is no memory
     * 
     */
	IMPORT_C TInt Open(
					RSuplTerminalServer& aSuplServer, 
					TSuplServiceType aSuplService=ESUPL_1_0);

    /**
     * This is an asynchronous request to SUPL Server to determine position 
     * information or to determine additional positioning information such as
     * assistance data.
     * Client can have only one outstanding asynchronous request per sub-session.  
     *
     * Parameter aQop is of type @ref TSuplTerminalQop and can be used by the Terminal 
     * Initiation API client to specify the desired quality of position. Client can specify
     * horizontal accuracy, vertical accuracy, maximum location age and response time using this
     * parameter. 
     *
     * Parameter aSETCaps can be used to restrict the positioning technologies 
     * to be used in subsequent positioning determination request by the current client.
     * Parameter aSETCaps takes constant values defined in epos_suplterminalconstants.h
     * and additional constants can be introduced for positioning technologies not in 
     * mentioned in the list. This parameter has a default value of zero in 
     * which case any of the position technology can be used to determine the position
     * informatioin.
     * Following are the currently defined constant values:
     *      -  KGpsSETAssisted	=   0x0001,
     *      -  KGpsSETBased  	=  	0x0002,
     *      -  KAutonomousGps 	=  	0x0004,
     *      -  KAFLT 			=  	0x0008,
     *      -  KECID   		    =  	0x0010,
     *      -  KEOTD  			= 	0x0020,
     *      -  KOTDOA 			=   0x0040,
     *      -  KCID 			=   0x0080,
     *
     * 
     * Client can use more than one option from the above table using logical
     * 'OR' operator.
     *
     * Parameter aRequestId can be used by the Terminal Initiation API client 
     * to deliver a request identifier to the tunneled positioning protocol 
     * handler if needed. This is used by the client which is requesting for additional 
     * position information such as assistance data for A-GPS.  Tunneled protocol 
     * can use this value for example to match different assistance data requests 
     * and response received from SUPL Server. By default, the value of this parameter
     * is set to zero. It is client's responsibility to generate and keep track of 
     * this request identifier, if used.
     *
     * Parameter aFirstReq can be used by the Terminal Initiation API client to indicate 
     * whether the request is the first request in the subsession or not. By default, this
     * is set to ETrue to indicate that the request is the first one.  For 
     * second request onwards this parameter should be set to EFalse to indicate that the 
     * request is not the first one. 
     *
     * Since this is an asynchronous call, the client has to wait for status on 
     * aStatus parameter to know the result of position determination.
     *
     * Client can cancel this request using RSuplTerminalSubSession::CancelRunSession.
     * Before a client closes a sub-session, it must ensure that all 
     * outstanding requests have been cancelled. In particular, after the client 
     * has invoked the CancelRunSession() method, it must wait until SUPL Server has 
     * reported that the request has indeed been cancelled.
     * Panic ESuplDuplicateRequest occurs if there is already an outstanding request
     * for the sub session. 
     *
     * @param [OUT] aStatus to return result code 
     * @param [IN] aQop Desired quality of position 
     * @param [IN] aSETCaps will hold positioning technologies than can be used
     * @param [IN] aRequestId will hold request identifier to match assistance data request/response 
     * @param [IN] aFirstReq will indicate whether the request is the first one or not 
     * @return aStatus will have 
     *         - KErrNone if session is completed sucessfully.  
     *         - KErrCancel if the request was successfully cancelled
     *         - KErrTimedOut in case of SUPL session timeout
     *         - KErrNoMemory if there is no memory
     *         - KErrArgument if there is error in the arguments passed
     *         - KErrCompletion if position determination cannot be completed successfuly
     *         - KErrNotReady if SUPL server is not ready to determine position information, 
     *           client can retry after sometime.
     *         - KErrServerTerminated if server got terminated due to some error
     *         - KSuplPositionPresent if position is determined but SUPL Session could not be
     *					 completed successfully
     *         - KErrGeneral for all other errors
     */
	IMPORT_C void RunSession(
					TRequestStatus& aStatus, 
					TSuplTerminalQop& aQop, 
					TInt aSETCaps=0, 
					TInt aRequestId=0, 
					TBool aFirstReq=ETrue);

    /**
     * This is an synchronous request to SUPL server to determine position 
     * information or to determine additional positioning information.
     *
     * Parameter aQop is of type @ref TSuplTerminalQop and can be used by the Terminal 
     * Initiation API client to specify the desired quality of position. Client can specify
     * horizontal accuracy, vertical accuracy, maximum location age and response time using this
     * parameter. 
     *
     * Parameter aSETCaps can be used to restrict the positioning technologies 
     * to be used in subsequent positioning determination requests by the current client.
     * Parameter aSETCaps takes constant values defined in epos_suplterminalconstants.h 
     * and additional constants can be introduced for positioning technologies not in 
     * mentioned in the list. This parameter has a default value of zero in 
     * which case any of the position technology can be used to determine the position
     * informatioin.
     * Following are the currently defined constant values:
     *      -  KGpsSETAssisted	=   0x0001,
     *      -  KGpsSETBased  	=  	0x0002,
     *      -  KAutonomousGps 	=  	0x0004,
     *      -  KAFLT 			=  	0x0008,
     *      -  KECID   		    =  	0x0010,
     *      -  KEOTD  			= 	0x0020,
     *      -  KOTDOA 			=   0x0040,
     *      -  KCID 			=   0x0080,
     *
     * 
     * Client can use more than one option from the above table using logical
     * 'OR' operator.
     *
     * Parameter aRequestId can be used by the Terminal Initiation API client 
     * to deliver a request identifier to the tunneled positioning protocol 
     * handler if needed. This is used by the client which is requesting for additional 
     * position information such as assistance data for A-GPS. Tunneled protocol 
     * can use this value for example to match different assistance data requests 
     * and response received from SUPL Server. By default, value of this parameter
     * is set to zero. It is client's responsibility to generate and keep track of 
     * this request identifier, if used.
     *
     * Parameter aFirstReq can be used by the Terminal Initiation API client to indicate 
     * whether the request is the first request in the subsession or not. By default, this
     * is set to ETrue to indicate that the request is the first one.  For 
     * second request onwards this parameter should be set to EFalse to indicate that the 
     * request is not the first one. 
     *
     * @param [IN] aQop Desired quality of position 
     * @param [IN] aSETCaps will hold positioning technologies than can be used
     * @param [IN] aRequestId will hold request identifier to match assistance data request/response 
     * @param [IN] aFirstReq will indicate whether the request is the first one or not 
     * @return one of the following error codes: 
     *         - KErrNone if session is completed sucessfully.  
     *         - KErrTimedOut in case of SUPL session timeout
     *         - KErrNoMemory if there is no memory
     *         - KErrArgument if there is error in the arguments passed
     *         - KErrCompletion if position determination cannot be completed successfuly
     *         - KErrNotReady if SUPL server is not ready to determine position information
     *         - KErrServerTerminated if server got terminated due to some error
     *         - KSuplPositionPresent if position is determined but SUPL Session could not be
     *					 completed successfully
     *		   - KErrNotSupported if SUPL is disabled
     *         - KErrGeneral for all other errors
     */
	IMPORT_C TInt RunSession(
					TSuplTerminalQop& aQop, 
					TInt aSETCaps=0, 
					TInt aRequestId=0, 
					TBool aFirstReq=ETrue);

    /**
     * This is an asynchronous request to SUPL Server to determine position 
     * information or to determine additional positioning information such as
     * assistance data.
     * Client can have only one outstanding asynchronous request per sub-session.  
     *
     * Parameter aSETCaps can be used to restrict the positioning technologies 
     * to be used in subsequent positioning determination request by the current client.
     * Parameter aSETCaps takes constant values defined in epos_suplterminalconstants.h
     * and additional constants can be introduced for positioning technologies not
     * mentioned in the list. This parameter has a default value of zero in 
     * which case any of the position technology can be used to determine the position
     * informatioin.
     * Following are the currently defined constant values:
     *      -  KGpsSETAssisted	=   0x0001,
     *      -  KGpsSETBased  	=  	0x0002,
     *      -  KAutonomousGps 	=  	0x0004,
     *      -  KAFLT 			=  	0x0008,
     *      -  KECID   		    =  	0x0010,
     *      -  KEOTD  			= 	0x0020,
     *      -  KOTDOA 			=   0x0040,
     *      -  KCID 			=   0x0080,
     *
     * 
     * Client can use more than one option from the above table using logical
     * 'OR' operator.
     *
     * Parameter aRequestId can be used by the Terminal Initiation API client 
     * to deliver a request identifier to the tunneled positioning protocol 
     * handler if needed. This is used by the client which is requesting for additional 
     * position information such as assistance data for A-GPS. Tunneled protocol 
     * can use this value for example to match different assistance data requests 
     * and response received from SUPL Server. By default, value of this parameter
     * is set to zero. It is client's responsibility to generate and keep track of 
     * this request identifier, if used.
     *
     *
     * Parameter aFirstReq can be used by the Terminal Initiation API client to indicate 
     * whether the request is the first request in the subsession or not. By default, this
     * is set to ETrue to indicate that the request is the first one.  For 
     * second request onwards this parameter should be set to EFalse to indicate that the 
     * request is not the first one. 
     *
     * Since this is an asynchronous call, client has to wait for status on 
     * aStatus parameter to know the result of position determination.
     *
     * Client can cancel this request using RSuplTerminalSubSession::CancelRunSession.
     * Before a client closes a sub-session, it must ensure that all 
     * outstanding requests have been cancelled. In particular, after the client 
     * has invoked the CancelRunSession() method, it must wait until SUPL Server has 
     * reported that the request has indeed been cancelled.
     * Panic ESuplDuplicateRequest occurs if there is already an outstanding request
     * for the sub session. 
     *
     * @param [OUT] aStatus to return result code 
     * @param [IN] aSETCaps will hold positioning technologies than can be used
     * @param [IN] aRequestId will hold request identifier to match assistance data request/response 
     * @param [IN] aFirstReq will indicate whether the request is the first one or not 
     * @return aStatus will have 
     *         - KErrNone if session is completed sucessfully.  
     *         - KErrCancel if the request was successfully cancelled
     *         - KErrTimedOut in case of SUPL session timeout
     *         - KErrNoMemory if there is no memory
     *         - KErrArgument if there is error in the arguments passed
     *         - KErrCompletion if position determination cannot be completed successfuly
     *         - KErrNotReady if SUPL Server is not ready to determine position information, 
     *         	 client can retry after sometime.
     *         - KErrServerTerminated if server got terminated due to some error
     *         - KSuplPositionPresent if position is determined but SUPL Session could not be
     *			 completed successfully
     *         - KErrGeneral for all other errors
     */
	IMPORT_C void RunSession(
					TRequestStatus& aStatus, 
					TInt aSETCaps=0, 
					TInt aRequestId=0, 
					TBool aFirstReq=ETrue);

    /**
     * This is an synchronous request to SUPL Server to determine position 
     * information or to determine additional positioning information.
     *
     * Parameter aSETCaps can be used to restrict the positioning technologies 
     * to be used in subsequent positioning determination requests by the current client.
     * Parameter aSETCaps takes constant values defined in epos_suplterminalconstants.h 
     * and additional constants can be introduced for positioning technologies not in 
     * mentioned in the list. This parameter has a default value of zero in 
     * which case any of the position technology can be used to determine the position
     * informatioin.
     * Following are the currently defined constant values:
     *      -  KGpsSETAssisted	=   0x0001,
     *      -  KGpsSETBased  	=  	0x0002,
     *      -  KAutonomousGps 	=  	0x0004,
     *      -  KAFLT 			=  	0x0008,
     *      -  KECID   		    =  	0x0010,
     *      -  KEOTD  			= 	0x0020,
     *      -  KOTDOA 			=   0x0040,
     *      -  KCID 			=   0x0080,
     *
     * 
     * Client can use more than one option from the above table using logical
     * 'OR' operator.
     *
     * Parameter aRequestId can be used by the Terminal Initiation API client 
     * to deliver a request identifier to the tunneled positioning protocol 
     * handler if needed. This is used by the client which is requesting for additional 
     * position information such as assistance data for A-GPS. Tunneled protocol 
     * can use this value for example to match different assistance data requests 
     * and response received from SUPL server. By default, value of this parameter
     * is set to zero. It is client's responsibility to generate and keep track of 
     * this request identifier, if used.
     *
     * Parameter aFirstReq can be used by the Terminal Initiation API client to indicate 
     * whether the request is the first request in the subsession or not. By default, this
     * is set to ETrue to indicate that the request is the first one.  For 
     * second request onwards this parameter should be set to EFalse to indicate that the 
     * request is not the first one. 
     *
     * @param [IN] aSETCaps will hold positioning technologies than can be used
     * @param [IN] aRequestId will hold request identifier to match assistance data request/response 
     * @param [IN] aFirstReq will indicate whether the request is the first one or not 
     * @return one of the following error codes: 
     *         - KErrNone if session is completed sucessfully.  
     *         - KErrTimedOut in case of SUPL session timeout
     *         - KErrNoMemory if there is no memory
     *         - KErrArgument if there is error in the arguments passed
     *         - KErrCompletion if position determination cannot be completed successfuly
     *         - KErrNotReady if SUPL Server is not ready to determine position information
     *         - KErrServerTerminated if server got terminated due to some error
     *         - KSuplPositionPresent if position is determined but SUPL Session could not be
     *					completed successfully
     *         - KErrGeneral for all other errors
     */
	IMPORT_C TInt RunSession(
					TInt aSETCaps=0, 
					TInt aRequestId=0, 
					TBool aFirstReq=ETrue);

    /**
     * This is an synchronous method for retrieving position information  
     * determined during RSuplTerminalSubSession::RunSession(). This method should 
     * be called after RunSession is completed with KErrNone or with KSuplPositionPresent.
     * The parameter aPositionInfo is of type TPositionInfoBase defined by 
     * Location Acquisition API. It is possible to pass any class that is 
     * derived from TPositionInfoBase. But class types supported are 
     * TPositionInfo and HPositionGenericInfo defined in Location Acquisition API.
     * Using HPositionGenericInfo client can specify and retrieve extended 
     * position information. Client can specify field it wants to retrieve by setting 
     * identifiers of the requested fields in the HPositionGenericInfo object using 
     * HPositionGenericInfo::SetRequestedField method. 
     * Fields currently supported are:
     *      - EPositionFieldHorizontalSpeed
     *      - EPositionFieldHorizontalSpeedError
     *      - EPositionFieldVerticalSpeed
     *      - EPositionFieldVerticalSpeedError
     *
     * If the position determined during @ref RunSession() does not meet the quality criteria 
     * specified by the client, @p KSuplPositionQualityLoss is returned for @ref GetPosition().
     * But client will still be able to get the position information if @ref RunSession()
     * is completed with @p KErrNone. Quality of position in @ref RunSession()
     * is optional and returning @p KSuplPositionQualityLoss error code is applicable only
     * if client has mentioned QoP when invoking @ref RunSession().
     *
     * @param [OUT] aPositionInfo will hold, on successful completion, position information.
     * @return following error codes:
     *      -  KErrNone if position information is present
     *      -  KErrNotFound for following reasons: 
     *          - Position information is not available 
     *          - GetPosition is called before RunSession() 
     *          - RunSession() is not completed with KErrNone.
     *			- GetPosition is called after StartSuplTriggerSession()
     *      -  KErrPositionBufferOverflow if memory allocated by client for
     *      HPositionGenericInfo object is not enough to hold requested fields.
     *      -  KErrNotSupported if the field specified with HPositionGenericInfo 
     *      is not supported or if any other class derived from TPositionInfoBase, 
     *      other than TPositionInfo and HPositionGenericInfo, is used with this method.
     *      - KSuplPositionQualityLoss if the position information does not meet the quality 
     *      criteria specified by the client when invoking @ref RunSession() 
     *
     */
	IMPORT_C TInt GetPosition(TPositionInfoBase& aPositionInfo);

    /**
     * Closes a sub-session with the SUPL Server. This must be called when 
     * the RSuplTerminalSubSession sub-session is no longer required. 
     * All the outstanding requests must be cancelled before closing
     * sub-session.
     *
     */
	IMPORT_C void Close();

    /**
     * Cancels an outstanding asynchronous position determination, RunSession(), 
     * request. Canceling requests is typically attempted when an client 
     * is closing down.
     *
     */
	IMPORT_C void CancelRunSession();

    /**
     * Retrieves list of SUPL server propertis configured. Client can use the retrieved SUPL
     * server properties to specify the server address @ref RunSession() 
     * @param [OUT] aSlpList  
     * @return Following error codes:
     *      -  KErrNone if server list is retrieved successfully 
     *      -  KErrPermissionDenied if client does not have CommDD capability
     *      -  KErrNoMemory if there is no memory
     *      -  KErrNotFound if none of the server is configured
     * 
     */
    IMPORT_C TInt GetSlpList(RPointerArray<CSuplTerminalParams> &aParamValues);

    /**
     * This is an asynchronous request to SUPL Server to determine position 
     * information or to determine additional positioning information such as
     * assistance data.
     * Client can have only one outstanding asynchronous request per sub-session.  
     *
     * Parameter aQop is of type @ref TSuplTerminalQop and can be used by the Terminal 
     * Initiation API client to specify the desired quality of position. Client can specify
     * horizontal accuracy, vertical accuracy, maximum location age and response time using this
     * parameter. 
     *
     * Parameter aHslpAddress is used to specify SUPL server to be used.  The list of configured SUPL
     * server addresses can be obtained using @ref GetSlpList().  The SUPL server address is one of
     * the parameter in the the server properties obtained.
     *
     * Parameter aFallBack can be used to specify if the fallback to other configured server is allowed
     * if SUPL session with the server address specified fails.
     *
     * Parameter aSETCaps can be used to restrict the positioning technologies 
     * to be used in subsequent positioning determination request by the current client.
     * Parameter aSETCaps takes constant values defined in epos_suplterminalconstants.h
     * and additional constants can be introduced for positioning technologies not in 
     * mentioned in the list. This parameter has a default value of zero in 
     * which case any of the position technology can be used to determine the position
     * informatioin.
     * Following are the currently defined constant values:
     *      -  KGpsSETAssisted	=   0x0001,
     *      -  KGpsSETBased  	=  	0x0002,
     *      -  KAutonomousGps 	=  	0x0004,
     *      -  KAFLT 			=  	0x0008,
     *      -  KECID   		    =  	0x0010,
     *      -  KEOTD  			= 	0x0020,
     *      -  KOTDOA 			=   0x0040,
     *      -  KCID 			=   0x0080,
     *
     * 
     * Client can use more than one option from the above table using logical
     * 'OR' operator.
     *
     * Parameter aRequestId can be used by the Terminal Initiation API client 
     * to deliver a request identifier to the tunneled positioning protocol 
     * handler if needed. This is used by the client which is requesting for additional 
     * position information such as assistance data for A-GPS.  Tunneled protocol 
     * can use this value for example to match different assistance data requests 
     * and response received from SUPL Server. By default, the value of this parameter
     * is set to zero. It is client's responsibility to generate and keep track of 
     * this request identifier, if used.
     *
     * Parameter aFirstReq can be used by the Terminal Initiation API client to indicate 
     * whether the request is the first request in the subsession or not. By default, this
     * is set to ETrue to indicate that the request is the first one.  For 
     * second request onwards this parameter should be set to EFalse to indicate that the 
     * request is not the first one. 
     *
     * Since this is an asynchronous call, the client has to wait for status on 
     * aStatus parameter to know the result of position determination.
     *
     * Client can cancel this request using RSuplTerminalSubSession::CancelRunSession.
     * Before a client closes a sub-session, it must ensure that all 
     * outstanding requests have been cancelled. In particular, after the client 
     * has invoked the CancelRunSession() method, it must wait until SUPL Server has 
     * reported that the request has indeed been cancelled.
     * Panic ESuplDuplicateRequest occurs if there is already an outstanding request
     * for the sub session. 
     *
     * @param [OUT] aStatus to return result code 
     * @param [IN] aQop Desired quality of position 
     * @param [IN] aHslpAddress Desired SLP to be used 
     * @param [IN] aFallBack will indicate whether the fallback mechanism can be used or not 
     * 			   if SUPL session with the server specified fails.  
     * @param [IN] aSETCaps will hold positioning technologies than can be used
     * @param [IN] aRequestId will hold request identifier to match assistance data request/response 
     * @param [IN] aFirstReq will indicate whether the request is the first one or not 
     * @return aStatus will have 
     *         - KErrNone if session is completed sucessfully.  
     *         - KErrCancel if the request was successfully cancelled
     *         - KErrTimedOut in case of SUPL session timeout
     *         - KErrNoMemory if there is no memory
     *         - KErrArgument if there is error in the arguments passed
     *         - KErrCompletion if position determination cannot be completed successfuly
     *         - KErrNotReady if SUPL server is not ready to determine position information, 
     *         			 client can retry after sometime.
     *         - KErrServerTerminated if server got terminated due to some error
     *         - KSuplPositionPresent if position is determined but SUPL Session could not be
     *					 completed successfully
     *         - KErrGeneral for all other errors
     *
     */
	IMPORT_C void RunSession(
					TRequestStatus& aStatus, 
					TSuplTerminalQop& aQop, 
					const TDesC& aHslpAddress, 
					TBool aFallBack= EFalse, 
					TInt aSETCaps=0, 
					TInt aRequestId=0, 
					TBool aFirstReq=ETrue);

    /**
     * This is an synchronous request to SUPL server to determine position 
     * information or to determine additional positioning information.
     *
     * Parameter aQop is of type @ref TSuplTerminalQop and can be used by the Terminal 
     * Initiation API client to specify the desired quality of position. Client can specify
     * horizontal accuracy, vertical accuracy, maximum location age and response time using this
     * parameter. 
     *
     * Parameter aHslpAddress is used to specify SUPL server to be used.  The list of configured SUPL
     * server addresses can be obtained using @ref GetSlpList().  The SUPL server address is one of
     * the parameter in the the server properties obtained.
     *
     * Parameter aFallBack can be used to specify if the fallback to other configured server is allowed
     * if SUPL session with the server address specified fails.
     *
     * Parameter aSETCaps can be used to restrict the positioning technologies 
     * to be used in subsequent positioning determination requests by the current client.
     * Parameter aSETCaps takes constant values defined in epos_suplterminalconstants.h 
     * and additional constants can be introduced for positioning technologies not in 
     * mentioned in the list. This parameter has a default value of zero in 
     * which case any of the position technology can be used to determine the position
     * informatioin.
     * Following are the currently defined constant values:
     *      -  KGpsSETAssisted	=   0x0001,
     *      -  KGpsSETBased  	=  	0x0002,
     *      -  KAutonomousGps 	=  	0x0004,
     *      -  KAFLT 			=  	0x0008,
     *      -  KECID   		    =  	0x0010,
     *      -  KEOTD  			= 	0x0020,
     *      -  KOTDOA 			=   0x0040,
     *      -  KCID 			=   0x0080,
     *
     * 
     * Client can use more than one option from the above table using logical
     * 'OR' operator.
     *
     * Parameter aRequestId can be used by the Terminal Initiation API client 
     * to deliver a request identifier to the tunneled positioning protocol 
     * handler if needed. This is used by the client which is requesting for additional 
     * position information such as assistance data for A-GPS. Tunneled protocol 
     * can use this value for example to match different assistance data requests 
     * and response received from SUPL Server. By default, value of this parameter
     * is set to zero. It is client's responsibility to generate and keep track of 
     * this request identifier, if used.
     *
     * Parameter aFirstReq can be used by the Terminal Initiation API client to indicate 
     * whether the request is the first request in the subsession or not. By default, this
     * is set to ETrue to indicate that the request is the first one.  For 
     * second request onwards this parameter should be set to EFalse to indicate that the 
     * request is not the first one. 
     *
     * @param [IN] aQop Desired quality of position 
     * @param [IN] aHslpAddress Desired SLP to be used 
     * @param [IN] aFallBack will indicate whether the fallback mechanism can be used or not 
     * 			   if SUPL session with the server specified fails.  
     * @param [IN] aSETCaps will hold positioning technologies than can be used
     * @param [IN] aRequestId will hold request identifier to match assistance data request/response 
     * @param [IN] aFirstReq will indicate whether the request is the first one or not 
     * @return one of the following error codes: 
     *         - KErrNone if session is completed sucessfully.  
     *         - KErrTimedOut in case of SUPL session timeout
     *         - KErrNoMemory if there is no memory
     *         - KErrArgument if there is error in the arguments passed
     *         - KErrCompletion if position determination cannot be completed successfuly
     *         - KErrNotReady if SUPL server is not ready to determine position information
     *         - KErrServerTerminated if server got terminated due to some error
     *         - KSuplPositionPresent if position is determined but SUPL Session could not be
     *					 completed successfully
     *		   - KErrNotSupported if SUPL is disabled
     *         - KErrGeneral for all other errors
     */
	IMPORT_C TInt RunSession(
					TSuplTerminalQop& aQop, 
					const TDesC& aHslpAddress, 
					TBool aFallBack= EFalse, 
					TInt aSETCaps=0, 
					TInt aRequestId=0, 
					TBool aFirstReq=ETrue);

    /**
     * This is an asynchronous request to SUPL Server to determine position 
     * information or to determine additional positioning information such as
     * assistance data.
     * Client can have only one outstanding asynchronous request per sub-session.  
     *
     * Parameter aHslpAddress is used to specify SUPL server to be used.  The list of configured SUPL
     * server addresses can be obtained using @ref GetSlpList().  The SUPL server address is one of
     * the parameter in the the server properties obtained.
     *
     * Parameter aFallBack can be used to specify if the fallback to other configured server is allowed
     * if SUPL session with the server address specified fails.
     *
     * Parameter aSETCaps can be used to restrict the positioning technologies 
     * to be used in subsequent positioning determination request by the current client.
     * Parameter aSETCaps takes constant values defined in epos_suplterminalconstants.h
     * and additional constants can be introduced for positioning technologies not
     * mentioned in the list. This parameter has a default value of zero in 
     * which case any of the position technology can be used to determine the position
     * informatioin.
     * Following are the currently defined constant values:
     *      -  KGpsSETAssisted	=   0x0001,
     *      -  KGpsSETBased  	=  	0x0002,
     *      -  KAutonomousGps 	=  	0x0004,
     *      -  KAFLT 			=  	0x0008,
     *      -  KECID   		    =  	0x0010,
     *      -  KEOTD  			= 	0x0020,
     *      -  KOTDOA 			=   0x0040,
     *      -  KCID 			=   0x0080,
     *
     * 
     * Client can use more than one option from the above table using logical
     * 'OR' operator.
     *
     * Parameter aRequestId can be used by the Terminal Initiation API client 
     * to deliver a request identifier to the tunneled positioning protocol 
     * handler if needed. This is used by the client which is requesting for additional 
     * position information such as assistance data for A-GPS. Tunneled protocol 
     * can use this value for example to match different assistance data requests 
     * and response received from SUPL Server. By default, value of this parameter
     * is set to zero. It is client's responsibility to generate and keep track of 
     * this request identifier, if used.
     *
     *
     * Parameter aFirstReq can be used by the Terminal Initiation API client to indicate 
     * whether the request is the first request in the subsession or not. By default, this
     * is set to ETrue to indicate that the request is the first one.  For 
     * second request onwards this parameter should be set to EFalse to indicate that the 
     * request is not the first one. 
     *
     * Since this is an asynchronous call, client has to wait for status on 
     * aStatus parameter to know the result of position determination.
     *
     * Client can cancel this request using RSuplTerminalSubSession::CancelRunSession.
     * Before a client closes a sub-session, it must ensure that all 
     * outstanding requests have been cancelled. In particular, after the client 
     * has invoked the CancelRunSession() method, it must wait until SUPL Server has 
     * reported that the request has indeed been cancelled.
     * Panic ESuplDuplicateRequest occurs if there is already an outstanding request
     * for the sub session. 
     *
     * @param [OUT] aStatus to return result code 
     * @param [IN] aHslpAddress Desired SLP to be used 
     * @param [IN] aFallBack will indicate whether the fallback mechanism can be used or not 
     * 			   if SUPL session with the server specified fails.  
     * @param [IN] aSETCaps will hold positioning technologies than can be used
     * @param [IN] aRequestId will hold request identifier to match assistance data request/response 
     * @param [IN] aFirstReq will indicate whether the request is the first one or not 
     * @return aStatus will have 
     *         - KErrNone if session is completed sucessfully.  
     *         - KErrCancel if the request was successfully cancelled
     *         - KErrTimedOut in case of SUPL session timeout
     *         - KErrNoMemory if there is no memory
     *         - KErrArgument if there is error in the arguments passed
     *         - KErrCompletion if position determination cannot be completed successfuly
     *         - KErrNotReady if SUPL Server is not ready to determine position information, 
     *           	client can retry after sometime.
     *         - KErrServerTerminated if server got terminated due to some error
     *         - KSuplPositionPresent if position is determined but SUPL Session could not be
     *			  	completed successfully
     *         - KErrGeneral for all other errors
     *
     */
	IMPORT_C void RunSession(
					TRequestStatus& aStatus, 
					const TDesC& aHslpAddress, 
					TBool aFallBack= EFalse, 
					TInt aSETCaps=0, 
					TInt aRequestId=0, 
					TBool aFirstReq=ETrue);

    /**
     * This is an synchronous request to SUPL Server to determine position 
     * information or to determine additional positioning information.
     *
     * Parameter aHslpAddress is used to specify SUPL server to be used.  The list of configured SUPL
     * server addresses can be obtained using @ref GetSlpList().  The SUPL server address is one of
     * the parameter in the the server properties obtained.
     *
     * Parameter aFallBack can be used to specify if the fallback to other configured server is allowed
     * if SUPL session with the server address specified fails.
     *
     * Parameter aSETCaps can be used to restrict the positioning technologies 
     * to be used in subsequent positioning determination requests by the current client.
     * Parameter aSETCaps takes constant values defined in epos_suplterminalconstants.h 
     * and additional constants can be introduced for positioning technologies not in 
     * mentioned in the list. This parameter has a default value of zero in 
     * which case any of the position technology can be used to determine the position
     * informatioin.
     * Following are the currently defined constant values:
     *      -  KGpsSETAssisted	=   0x0001,
     *      -  KGpsSETBased  	=  	0x0002,
     *      -  KAutonomousGps 	=  	0x0004,
     *      -  KAFLT 			=  	0x0008,
     *      -  KECID   		    =  	0x0010,
     *      -  KEOTD  			= 	0x0020,
     *      -  KOTDOA 			=   0x0040,
     *      -  KCID 			=   0x0080,
     *
     * 
     * Client can use more than one option from the above table using logical
     * 'OR' operator.
     *
     * Parameter aRequestId can be used by the Terminal Initiation API client 
     * to deliver a request identifier to the tunneled positioning protocol 
     * handler if needed. This is used by the client which is requesting for additional 
     * position information such as assistance data for A-GPS. Tunneled protocol 
     * can use this value for example to match different assistance data requests 
     * and response received from SUPL server. By default, value of this parameter
     * is set to zero. It is client's responsibility to generate and keep track of 
     * this request identifier, if used.
     *
     * Parameter aFirstReq can be used by the Terminal Initiation API client to indicate 
     * whether the request is the first request in the subsession or not. By default, this
     * is set to ETrue to indicate that the request is the first one.  For 
     * second request onwards this parameter should be set to EFalse to indicate that the 
     * request is not the first one. 
     *
     * @param [IN] aHslpAddress Desired SLP to be used 
     * @param [IN] aFallBack will indicate whether the fallback mechanism can be used or not 
     * 			   if SUPL session with the server specified fails.  
     * @param [IN] aSETCaps will hold positioning technologies than can be used
     * @param [IN] aRequestId will hold request identifier to match assistance data request/response 
     * @param [IN] aFirstReq will indicate whether the request is the first one or not 
     * @return one of the following error codes: 
     *         - KErrNone if session is completed sucessfully.  
     *         - KErrTimedOut in case of SUPL session timeout
     *         - KErrNoMemory if there is no memory
     *         - KErrArgument if there is error in the arguments passed
     *         - KErrCompletion if position determination cannot be completed successfuly
     *         - KErrNotReady if SUPL Server is not ready to determine position information
     *         - KErrServerTerminated if server got terminated due to some error
     *         - KSuplPositionPresent if position is determined but SUPL Session could not be
     *					 completed successfully
     *         - KErrGeneral for all other errors
     */
	IMPORT_C TInt RunSession(
					const TDesC& aHslpAddress, 
					TBool aFallBack= EFalse, 
					TInt aSETCaps=0, 
					TInt aRequestId=0, 
					TBool aFirstReq=ETrue);

    /**
     * This is an synchronous method for retrieving information  
     * on the server used during RSuplTerminalSubSession::RunSession(). This method should 
     * be called after RunSession is completed. It is client's responsibility to allocate 
     * enough memory to hold the server address and server address can have maximum size
     * of 256 characters.
     *
     * @param [OUT] aServerAdd will hold, on successful completion, server address 
     * @return following error codes:
     *      -  KErrNone if server address is present
     *      -  KErrOverflow if the descriptor is too small to receive the value
     *      -  KErrNotFound for following reasons: 
     *          - server address is not available 
     *          - this method is called before RunSession() 
     */
	IMPORT_C TInt GetServerAddress(TDes& aHslpAddress);

    /**
     * This is an asynchronous request to SUPL Server to start periodic triggering 
     * to determine position information or to determine additional positioning information 
     * such as assistance data at given interval of time subsequently.
     * Client can have only one outstanding asynchronous request per sub-session.  
     *
     * Parameter aSETCaps can be used to restrict the positioning technologies 
     * to be used in subsequent positioning determination requests by the current client.
     * Parameter aSETCaps takes constant values defined in epos_suplterminalconstants.h 
     * and additional constants can be introduced for positioning technologies not in 
     * mentioned in the list. This parameter has a default value of zero in 
     * which case any of the position technology can be used to determine the position
     * informatioin.
     * Following are the currently defined constant values:
     *      -  KGpsSETAssisted	=   0x0001,
     *      -  KGpsSETBased  	=  	0x0002,
     *      -  KAutonomousGps 	=  	0x0004,
     *      -  KAFLT 			=  	0x0008,
     *      -  KECID   		    =  	0x0010,
     *      -  KEOTD  			= 	0x0020,
     *      -  KOTDOA 			=   0x0040,
     *      -  KCID 			=   0x0080,
     *
     * 
     * Client can use more than one option from the above table using logical
     * 'OR' operator.
     *
     * Parameter aRequestId can be used by the Terminal Initiation API client 
     * to deliver a request identifier to the tunneled positioning protocol 
     * handler if needed. This is used by the client which is requesting for additional 
     * position information such as assistance data for A-GPS. Tunneled protocol 
     * can use this value for example to match different assistance data requests 
     * and response received from SUPL server. By default, value of this parameter
     * is set to zero. It is client's responsibility to generate and keep track of 
     * this request identifier, if used.
	 *
     * FallBack mechanism is enabled as default to allow other servers
     * if SUPL session with the server address specified fails.
     *
     * Since this is an asynchronous call, client has to wait for status on 
     * aStatus parameter to know the result of starting the triggering session.
     *
     * Client can cancel this request using RSuplTerminalSubSession::StopSuplTriggerSession.
     * Before a client closes a sub-session, it must ensure that all 
     * outstanding requests have been cancelled. In particular, after the client 
     * has invoked the StopSuplTriggerSession() method, it must wait until SUPL Server has 
     * reported that the request has indeed been cancelled.
     * Panic ESuplDuplicateRequest occurs if there is already an outstanding request
     * for the sub session. 
     *
     * @param [OUT] aStatus to return result code 
     * @param [IN] aPeriodicTrigger Desired Periodic Trigger info
     * @param [IN] aSETCaps will hold positioning technologies than can be used
     * @param [IN] aRequestId will hold request identifier to match assistance data request/response 
     * @return aStatus will have 
     *         - KErrNone if session is completed sucessfully.  
     *         - KErrCancel if the request was successfully cancelled
     *         - KErrTimedOut in case of SUPL session timeout
     *         - KErrNoMemory if there is no memory
     *         - KErrArgument if there is error in the arguments passed
     *         - KErrCompletion if position determination cannot be completed successfuly
     *         - KErrNotReady if SUPL Server is not ready to start triggering
     *         - KErrServerTerminated if server got terminated due to some error
     *         - KErrGeneral for all other errors
     *		   - KErrNotSupported if called when Session is Opened with SUPL_1_0 service
     *
     * 
     */
	IMPORT_C void StartSuplTriggerSession(
                    TRequestStatus& aStatus,
					TSuplTerminalPeriodicTrigger& aPeriodicTrigger,
					TInt aSETCaps=0,
					TInt aRequestId=0);
	
    /**
     * This is an asynchronous request to SUPL Server to start periodic triggering 
     * to determine position information or to determine additional positioning information 
     * such as assistance data at given interval of time subsequently.
     * Client can have only one outstanding asynchronous request per sub-session.  
     *
     * Parameter aHslpAddress is used to specify SUPL server to be used.  The list of configured SUPL
     * server addresses can be obtained using @ref GetSlpList().  The SUPL server address is one of
     * the parameter in the the server properties obtained.
     *
     * Parameter aFallBack can be used to specify if the fallback to other configured server is allowed
     * if SUPL session with the server address specified fails.
     *
     * Parameter aSETCaps can be used to restrict the positioning technologies 
     * to be used in subsequent positioning determination requests by the current client.
     * Parameter aSETCaps takes constant values defined in epos_suplterminalconstants.h 
     * and additional constants can be introduced for positioning technologies not in 
     * mentioned in the list. This parameter has a default value of zero in 
     * which case any of the position technology can be used to determine the position
     * informatioin.
     * Following are the currently defined constant values:
     *      -  KGpsSETAssisted	=   0x0001,
     *      -  KGpsSETBased  	=  	0x0002,
     *      -  KAutonomousGps 	=  	0x0004,
     *      -  KAFLT 			=  	0x0008,
     *      -  KECID   		    =  	0x0010,
     *      -  KEOTD  			= 	0x0020,
     *      -  KOTDOA 			=   0x0040,
     *      -  KCID 			=   0x0080,
     *
     * 
     * Client can use more than one option from the above table using logical
     * 'OR' operator.
     *
     * Parameter aRequestId can be used by the Terminal Initiation API client 
     * to deliver a request identifier to the tunneled positioning protocol 
     * handler if needed. This is used by the client which is requesting for additional 
     * position information such as assistance data for A-GPS. Tunneled protocol 
     * can use this value for example to match different assistance data requests 
     * and response received from SUPL server. By default, value of this parameter
     * is set to zero. It is client's responsibility to generate and keep track of 
     * this request identifier, if used.
     *
     * Since this is an asynchronous call, client has to wait for status on 
     * aStatus parameter to know the result of starting the triggering session.
     *
     * Client can cancel this request using RSuplTerminalSubSession::StopSuplTriggerSession.
     * Before a client closes a sub-session, it must ensure that all 
     * outstanding requests have been cancelled. In particular, after the client 
     * has invoked the StopSuplTriggerSession() method, it must wait until SUPL Server has 
     * reported that the request has indeed been cancelled.
     * Panic ESuplDuplicateRequest occurs if there is already an outstanding request
     * for the sub session. 
     *
     * @param [OUT] aStatus to return result code 
     * @param [IN] aPeriodicTrigger Desired Periodic Trigger info
     * @param [IN] aHslpAddress Desired SLP to be used 
     * @param [IN] aFallBack will indicate whether the fallback mechanism can be used or not 
     * 			   if SUPL session with the server specified fails.  
     * @param [IN] aSETCaps will hold positioning technologies than can be used
     * @param [IN] aRequestId will hold request identifier to match assistance data request/response 
     * @return aStatus will have 
     *         - KErrNone if session is completed sucessfully.  
     *         - KErrCancel if the request was successfully cancelled
     *         - KErrTimedOut in case of SUPL session timeout
     *         - KErrNoMemory if there is no memory
     *         - KErrArgument if there is error in the arguments passed
     *         - KErrCompletion if position determination cannot be completed successfuly
     *         - KErrNotReady if SUPL Server is not ready to determine position information
     *         - KErrServerTerminated if server got terminated due to some error
     *         - KErrGeneral for all other errors
     *		   - KErrNotSupported if called when Session is Opened with SUPL_1_0 service
     *
     *
     */
	IMPORT_C void StartSuplTriggerSession(
                    TRequestStatus& aStatus,
					TSuplTerminalPeriodicTrigger& aPeriodicTrigger,
					const TDesC& aHslpAddress, 
					TBool aFallBack= EFalse, 
					TInt aSETCaps=0,
					TInt aRequestId=0);

   	/**
     * Listens for the event if any trigger is fired. 
     *
     * Client application must call StartSuplTriggerSession() 
     * before calling NotifyTriggerFired().
	 *
     * Client applications can use this method to get notified 
     * when a periodic trigger is fired. The firing information is returned 
     * to the client application. 
     * If more that one trigger is fired, Supl Trigger Server will complete 
     * the request and  the first fired trigger is returned. 
     * Client application shall call this method again to get next 
     * trigger firing.      
     *
     * A client application will get Trigger firing response that are created by itself.
     * 
     * This method requires @p Location capability. 
	 *
     * Since this is an asynchronous call, client has to wait for status on 
     * aStatus parameter to know the result of position determination.
     *
     * Client can cancel this request using RSuplTerminalSubSession::StopSuplTriggerSession.
     * Before a client closes a sub-session, it must ensure that all 
     * outstanding requests have been cancelled. In particular, after the client 
     * has invoked the StopSuplTriggerSession() method, it must wait until SUPL Server has 
     * reported that the request has indeed been cancelled.
     * Panic ESuplDuplicateRequest occurs if there is already an outstanding request
     * for the sub session. 
     *
     * @panic ESuplServerBadHandle If the subsession is not opened.
     * already an outstanding NotifyTriggerFired() request.
     *
     * @param[out] 	aFireInfo On return contains the fired  
     * 				trigger's firing information.
     * @param[out] 	aStatus Will be completed 
     *
     * @return aStatus will have 
     *         - KErrNone if an position info is acheived successfully at given interval time
     *         - KErrCancel if the request was successfully cancelled
     *         - KErrTimedOut in case of SUPL session timeout
     *         - KErrNotReady if SUPL Server is not ready to determine position information
     *         - KErrServerTerminated if server got terminated due to some error
     *         - KSuplPositionPresent if position is determined but SUPL Session could not be
     *			 completed successfully
     *         - KErrGeneral for all other errors
     *		   - KErrNotSupported if called when Session is Opened with SUPL_1_0 service
     *			or Called after RunSession
     *
     */
    IMPORT_C void NotifyTriggerFired(
    				TRequestStatus& aStatus,
        			TSuplTriggerFireInfo& aFireInfo);

    /**
     * Stops an outstanding asynchronous request used for Triggering. 
	 * This method is used to cancel both StartSuplTriggerSession() and NotifyTriggerFired() 
	 * which ever is outstanding at that time.     
	 *
     * Stopping requests is typically attempted when an client 
     * is closing down.
     *
     */
	IMPORT_C void StopSuplTriggerSession();
	
protected:

	IMPORT_C virtual void ConstructL();
	
	IMPORT_C virtual void Destruct();

	/**
	 * Reserved for future expansion - derived classes should see documentation
	 * on how this is to be used. 
	 *
	 */
	IMPORT_C virtual TAny* ExtendedInterface(
						TInt aFunctionNumber, 
						TAny* aPtr1, 
						TAny* aPtr2);

protected:
	/**
	 * Checks if server  is present in the database or not
	 * @ return codes
	 *  		- KErrNone if server is present
	 *  		- KErrNotFound if server is not present
	 *  		- KErrGeneral
	 */	
	TInt CheckIfSlpExists(const TDesC& aHslpAddress);
	
	/**
	 * Checks if server  is present in the database or not
	 * @ return codes
	 *  		- KErrNone if server is present
	 *  		- KErrNotFound if server is not present
	 *  		- KErrGeneral
	 */	
	TBool CheckSuplTriggerServiceStatus();


protected:

    /** 
     * A pointer to a container that holds pointer descriptors, needed to point
	 * to the clients request data during asynchronous requests 
	 */
	CSuplSubSessnPtrHolder* iPtrHolder;
	
	//Supl service - supl 1.0 or 2.0
	TSuplServiceType iSuplService;	
	

private:

   
	//Member variable for holding Qop parameters from Client
	TSuplTerminalQop iQoP;
	
	/** Unused variable for future expansion. */
	TAny* iReserved;
	
	//Member variable for holding parameters from client
	TSuplParameters iSuplParameters;
	
	//Member variable for holding periodic trigger parameter's object
    TSuplTerminalPeriodicTrigger iPeriodicTrigger;
	
	//CSuplSettings object to call GetAllSlp
	CSuplSettings* iSuplStorageSettings;

	//To check Triggering is use or not
	TBool isTriggeringUsed;
	
	//To count NotifyTriggerFired Request
	TInt iNotifyRequestCtr;
	};

#endif // __EPOS_SUPLTERMINAL_H__
