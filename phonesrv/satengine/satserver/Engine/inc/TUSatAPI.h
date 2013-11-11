/*
* Copyright (c) 2002-2008 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  Wrapper for USAT api.
*
*/



#ifndef TUSATAPI_H
#define TUSATAPI_H

//  INCLUDES
#include <etelsat.h>
#include "MSatApi.h"

// FORWARD DECLARATION
class MSatMultiModeApi;
class CSatSActiveWrapper;

// CLASS DECLARATION

/**
*  Wrapper for USAT API.
*
*  @lib SatEngine.lib
*  @since Series 60 3.0
*/
class TUSatAPI : public MSatApi
    {
    public:  // Constructors and destructor

        /**
        * C++ default constructor.
        */
        TUSatAPI();

    public: // New functions

        /**
        * Connects to USat API.
        * @param aPhone Reference to MSatMultiModeApi
        * @return Error code indicating connection status
        */
        TInt Connect( MSatMultiModeApi& aPhone );

        /**
        * Closes the connection to USAT API.
        */
        void Close();

    public: // From base classes

        /**
        * From MSatApi Issues notification for SetUpMenu command from USAT api.
        * @param aStatus Request status
        * @param aPCmd Command identifier
        */
        void NotifySetUpMenu(
            TRequestStatus& aStatus,
            TDes8& aPCmd );

        /**
        * From MSatApi Cancels SetUpMenu notification.
        */
        void NotifySetUpMenuCancel();

        /**
        * From MSatApi Issues notification for LanguageNotification command from USAT api.
        * @param aStatus Request status
        * @param aPCmd Command identifier
        */
        void NotifyLanguageNotification(
            TRequestStatus& aStatus,
            TDes8& aPCmd );

        /**
        * From MSatApi Cancels LanguageNotification notification.
        */
        void NotifyLanguageNotificationCancel();

        /**
        * From MSatApi Issues SetUpCall request
        * @param aStatus Request status
        * @param aPCmd Command identifier
        */
        void NotifySetUpCall(
            TRequestStatus& aStatus,
            TDes8& aPCmd );

        /**
        * From MSatApi Cancels the SetUpCall request
        */
        void NotifySetUpCallCancel();

        /**
        * From MSatApi Issues the SelectItem request.
        * @param aStatus Request status
        * @param aPCmd Command identifier
        */
        void NotifySelectItem(
            TRequestStatus& aStatus,
            TDes8& aPCmd );

        /**
        * From MSatApi Cancels the SelectItem request
        */
        void NotifySelectItemCancel();

        /**
        * From MSatApi.
        * @param aStatus Request status
        * @param aPCmd Command identifier
        */
        void NotifySendSm(
            TRequestStatus& aStatus,
            TDes8& aPCmd );

        /**
        * From MSatApi.
        */
        void NotifySendSmCancel();

        /**
        * From MSatApi
        * @param aStatus Request status
        * @param aData Data to be sent
        * @param aMsgReference Reference to message
        */
        void SendMessageNoLogging(
            TRequestStatus& aStatus,
            TDes8& aData,
            TUint16& aMsgReference ) ;

        /**
        * From MSatApi
        */
        void SendMessageNoLoggingCancel();

        /**
        * From MSatApi
        * @param aStatus Request status
        * @param aData Command identifier
        */
        void NotifyLaunchBrowser(
            TRequestStatus& aStatus,
            TDes8& aData );

        /**
        * From MSatApi
        */
        void NotifyLaunchBrowserCancel();

        /**
        * From MSatApi
        * @param aStatus Request status
        * @param aData Command identifier
        */
        void NotifyDisplayText(
            TRequestStatus& aStatus,
            TDes8& aData );

        /**
        * From MSatApi
        */
        void NotifyDisplayTextCancel();

        /**
        * From MSatApi
        * @param aStatus Request status
        * @param aData Command identifier
        */
        void NotifyGetInput(
            TRequestStatus& aStatus,
            TDes8& aData );

        /**
        * From MSatApi
        */
        void NotifyGetInputCancel();

        /**
        * From MSatApi
        * @param aStatus Request status
        * @param aData Command identifier
        */
        void NotifyGetInkey(
            TRequestStatus& aStatus,
            TDes8& aData );

        /**
        * From MSatApi
        */
        void NotifyGetInkeyCancel();

        /**
        * From MSatApi
        * @param aStatus Request status
        * @param aData Command identifier
        */
        void NotifyCallControl(
            TRequestStatus& aStatus,
            TDes8& aData );

        /**
        * From MSatApi
        */
        void NotifyCallControlCancel();

        /**
        * From MSatApi
        * @param aStatus Request status
        * @param aData Command identifier
        */
        void NotifyMoSmControl(
            TRequestStatus& aStatus,
            TDes8& aData );

        /**
        * From MSatApi
        */
        void NotifyMoSmControlCancel();

        /**
        * From MSatApi
        * @param aStatus Request status
        * @param aData Command identifier
        */
        void NotifySetUpEventList(
            TRequestStatus& aStatus,
            TDes8& aData );

        /**
        * From MSatApi
        */
        void NotifySetUpEventListCancel();

        /**
        * From MSatApi
        * @param aStatus Request status
        * @param aData Command identifier
        */
        void NotifySetUpIdleModeText(
            TRequestStatus& aStatus,
            TDes8& aData );

        /**
        * From MSatApi
        */
        void NotifySetUpIdleModeTextCancel();

        /**
        * From MSatApi
        * @param aStatus Request status
        * @param aData Command identifier
        */
        void NotifySendDtmf(
            TRequestStatus& aStatus,
            TDes8& aData );

        /**
        * From MSatApi
        */
        void NotifySendDtmfCancel();

        /**
        * From MSatApi
        * @param aStatus Request status
        * @param aData Command identifier
        */
        void NotifySendSs(
            TRequestStatus& aStatus,
            TDes8& aData );

        /**
        * From MSatApi
        */
        void NotifySendSsCancel();

        /**
        * From MSatApi
        * @param aStatus Request status
        * @param aData Command identifier
        */
        void NotifySendUssd(
            TRequestStatus& aStatus,
            TDes8& aData );

        /**
        * From MSatApi
        */
        void NotifySendUssdCancel();

        /**
        * From MSatApi
        * @param aStatus Request status
        * @param aData Command identifier
        */
        void NotifyPlayTone(
            TRequestStatus& aStatus,
            TDes8& aData );

        /**
        * From MSatApi
        */
        void NotifyPlayToneCancel();

        /**
        * From MSatApi
        * @param aStatus Request status
        * @param aData Command identifier
        */
        void NotifyRefreshRequired(
            TRequestStatus& aStatus,
            TDes8& aData );

        /**
        * From MSatApi
        */
        void NotifyRefreshRequiredCancel();

        /**
        * From MSatApi
        * @param aPCkg Refresh required response package
        */
        void RefreshAllowed( const TDesC8& aPCkg );

        /**
        * From MSatApi
        * @param aStatus Request status
        * @param aData Command identifier
        */
        void NotifyRefresh(
            TRequestStatus& aStatus,
            TDes8& aData );

        /**
        * From MSatApi
        */
        void NotifyRefreshCancel();

        /**
        * From MSatApi Issues the SimSessionEnd request
        * @param aStatus Request status
        */
        void NotifySimSessionEnd( TRequestStatus& aStatus );

        /**
        * From MSatApi Cancels the SimSessionEnd request
        */
        void NotifySimSessionEndCancel();

        /**
        * From MSatApi
        * @param aStatus Request status
        * @param aData Command identifier
        */
        void NotifyLocalInfo(
            TRequestStatus& aStatus,
            TDes8& aData );

        /**
        * From MSatApi
        */
        void NotifyLocalInfoCancel();

        /**
        * Issues the request
        * @param aStatus Request status.
        * @param aPCmd Command identifier and other relevant information
        */
        void NotifyOpenChannel(
            TRequestStatus& aStatus,
            TDes8& aPCmd );

        /**
        * Cancels the request
        */
        void NotifyOpenChannelCancel();

        /**
        * Issues the request
        * @param aStatus Request status.
        * @param aPCmd Command identifier and other relevant information
        */
        void NotifyCloseChannel(
            TRequestStatus& aStatus,
            TDes8& aPCmd );

        /**
        * Cancels the request
        */
        void NotifyCloseChannelCancel();

        /**
        * Issues the request
        * @param aStatus Request status.
        * @param aPCmd Command identifier and other relevant information
        */
        void NotifyGetChannelStatus(
            TRequestStatus& aStatus,
            TDes8& aPCmd );

        /**
        * Cancels the request
        */
        void NotifyGetChannelStatusCancel();

        /**
        * Issues the request
        * @param aStatus Request status.
        * @param aPCmd Command identifier and other relevant information
        */
        void NotifySendData(
            TRequestStatus& aStatus,
            TDes8& aPCmd );

        /**
        * Cancels the request
        */
        void NotifySendDataCancel();

        /**
        * Issues the request
        * @param aStatus Request status.
        * @param aPCmd Command identifier and other relevant information
        */
        void NotifyReceiveData(
            TRequestStatus& aStatus,
            TDes8& aPCmd );

        /**
        * Cancels the request
        */
        void NotifyReceiveDataCancel();

        /**
        * From MSatApi Sends menu selection to SIM.
        * @param aMenuSelection Selected menu item
        */
        void SendMenuSelection( const TDes8& aMenuSelection );

        /**
        * From MSatApi Sends terminal rsp to SIM.
        * @param aPCmd Command identifier
        * @param aPCkg Terminal response package
        */
        void TerminalRsp(
            RSat::TPCmd aPCmd,
            const TDesC8& aPCkg );

        /**
        * From MSatApi. Gets the icon info from SIM.
        * @param aReqStatus Request status.
        * @param aRecordNumber Specifies the EF_IMG in SIM.
        * @param aIconEf Contains the icon informations from SIM in returning
        * of the method.
        */
        void GetIcon(
            TRequestStatus& aReqStatus,
            TUint8 aRecordNumber,
            RSat::TIcon& aIconEf );

        /**
        * From MSatApi. Gets the icon image.
        * @param aReqStatus Request status.
        * @param aInstanceInfo Informatio of the icon, which data is
        * fetched from SIM.
        * @param aInstance Contains the icon data from SIM in returning of the
        * method.
        */
        void GetImageInstance(
            TRequestStatus& aReqStatus,
            const TDesC8& aInstanceInfo,
            TDes8& aInstance );

        /**
        * From MSatApi. Gets color lookup table of the icon.
        * @param aReqStatus Request status.
        * @param aInstanceNumber Image instance number.
        * @param aOffset Offset of the clut.
        * @param aClut Contains the Clut of the icon in returning of the
        * method.
        */
        void GetClut(
            TRequestStatus& aReqStatus,
            TUint aInstanceNumber,
            TUint aOffset,
            TDes8& aClut );

        /**
        * From MSatApi Sends notify to SIM about some event using EventDownload.
        * @param aReqStatus Request status.
        * @param aSingleEvent Single event reported to SIM.
        * @param aEventInfo Contains information of an event.
        */
        void EventDownload(
            TRequestStatus& aReqStatus,
            RSat::TEventList aSingleEvent,
            const TDesC8& aEventInfo );

        /**
        * From MSatApi Indicates is RSat connected
        * @return ETrue if RSat is connected
        */
        TBool IsRSatConnected() const;
        
        /**
        * From MSatApi Called when all supported notifies are sent
        * @return error code from RSat::UsatClientReadyIndication()
        */
        TInt UsatClientReadyIndication();
        
        /**
        * From CActive
        */
        void RunL();
        
        /**
        * From CActive
        */
        void DoCancel();
        
    private:

        // Prohibit copy constructor if not deriving from CBase.
        TUSatAPI( const TUSatAPI& );

        // Prohibit assigment operator if not deriving from CBase.
        TUSatAPI& operator=( const TUSatAPI& );

    private:    // Data

        // (U)SAT interface handler
        RSat iSat;
        // Indicates is RSat connected or not
        TBool iRSatConnected;
        // Active object wrapper for wait loops and asynhcronous function calls
        CSatSActiveWrapper* iWrapper;

    };

#endif      // TUSATAPI_H

// End of File
