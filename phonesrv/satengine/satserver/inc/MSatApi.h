/*
* Copyright (c) 2002-2007 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  Interface for USat API.
*
*/



#ifndef MSATAPI_H
#define MSATAPI_H

//  INCLUDES
#include <e32std.h>
#include <etelsat.h>

// CLASS DECLARATION

/**
*  Interface for USat api.
*
*  @lib SatEngine.lib
*  @since Series 60 3.0
*/
class MSatApi
    {
    protected:  // Constructors and destructor

        /**
        * C++ default constructor.
        */
        MSatApi() {};

        /**
        * Destructor.
        */
        virtual ~MSatApi() {};

    public: // New functions

        /**
        * Issues SetUpMenu request.
        * @param aStatus Request status.
        * @param aPCmd Command identifier and other relevant information
        */
        virtual void NotifySetUpMenu(
            TRequestStatus& aStatus,
            TDes8& aPCmd ) = 0;

        /**
        * Cancels the setupmenu request.
        */
        virtual void NotifySetUpMenuCancel() = 0;

        /**
        * Issues SetUpCall request
        * @param aStatus Request status.
        * @param aPCmd Command identifier and other relevant information
        */
        virtual void NotifySetUpCall(
            TRequestStatus& aStatus,
            TDes8& aPCmd ) = 0;

        /**
        * Cancels the SetUpCall request
        */
        virtual void NotifySetUpCallCancel() = 0;

        /**
        * Issues LangNotify request
        * @param aStatus Request status.
        * @param aPCmd Command identifier and other relevant information
        */
        virtual void NotifyLanguageNotification(
            TRequestStatus& aStatus,
            TDes8& aPCmd ) = 0;

        /**
        * Cancels the LangNotify request
        */
        virtual void NotifyLanguageNotificationCancel() = 0;

        /**
        * Issues the SelectItem request.
        * @param aStatus Request status.
        * @param aPCmd Command identifier and other relevant information
        */
        virtual void NotifySelectItem(
            TRequestStatus& aStatus,
            TDes8& aPCmd ) = 0;

        /**
        * Cancels the SelectItem request
        */
        virtual void NotifySelectItemCancel() = 0;

        /**
        * Issues the SendSm request.
        * @param aStatus Request status.
        * @param aPCmd Command identifier and other relevant information
        */
        virtual void NotifySendSm(
            TRequestStatus& aStatus,
            TDes8& aPCmd ) = 0;

        /**
        * Cancels the SendSm request.
        */
        virtual void NotifySendSmCancel() = 0;

        /**
        * Sends sm message.
        * @param aStatus Request status
        * @aData Data to be sent
        * @aMsgReference Message reference.
        */
        virtual void SendMessageNoLogging(
            TRequestStatus& aStatus,
            TDes8& aData,
            TUint16& aMsgReference ) = 0;

        /**
        * Cancels the send sm request.
        */
        virtual void SendMessageNoLoggingCancel() = 0;

        /**
        * Issues the request
        * @param aStatus Request status.
        * @param aPCmd Command identifier and other relevant information
        */
        virtual void NotifyLaunchBrowser(
            TRequestStatus& aStatus,
            TDes8& aPCmd ) = 0;

        /**
        * Cancels the request
        */
        virtual void NotifyLaunchBrowserCancel() = 0;

        /**
        * Issues the request
        * @param aStatus Request status.
        * @param aPCmd Command identifier and other relevant information
        */
        virtual void NotifyDisplayText(
            TRequestStatus& aStatus,
            TDes8& aPCmd ) = 0;

        /**
        * Cancels the request
        */
        virtual void NotifyDisplayTextCancel() = 0;

        /**
        * Issues the request
        * @param aStatus Request status.
        * @param aPCmd Command identifier and other relevant information
        */
        virtual void NotifyGetInput(
            TRequestStatus& aStatus,
            TDes8& aPCmd ) = 0;

        /**
        * Cancels the request
        */
        virtual void NotifyGetInputCancel() = 0;

        /**
        * Issues the request
        * @param aStatus Request status.
        * @param aPCmd Command identifier and other relevant information
        */
        virtual void NotifyGetInkey(
            TRequestStatus& aStatus,
            TDes8& aPCmd ) = 0;

        /**
        * Cancels the request
        */
        virtual void NotifyGetInkeyCancel() = 0;

        /**
        * Issues the request
        * @param aStatus Request status.
        * @param aPCmd Command identifier and other relevant information
        */
        virtual void NotifyCallControl(
            TRequestStatus& aStatus,
            TDes8& aPCmd ) = 0;

        /**
        * Cancels the request
        */
        virtual void NotifyCallControlCancel() = 0;

        /**
        * Issues the request
        * @param aStatus Request status.
        * @param aPCmd Command identifier and other relevant information
        */
        virtual void NotifyMoSmControl(
            TRequestStatus& aStatus,
            TDes8& aPCmd ) = 0;

        /**
        * Cancels the request
        */
        virtual void NotifyMoSmControlCancel() = 0;

        /**
        * Issues the request
        * @param aStatus Request status.
        * @param aPCmd Command identifier and other relevant information
        */
        virtual void NotifySetUpEventList(
            TRequestStatus& aStatus,
            TDes8& aPCmd ) = 0;

        /**
        * Cancels the request
        */
        virtual void NotifySetUpEventListCancel() = 0;

        /**
        * Issues the request
        * @param aStatus Request status.
        * @param aPCmd Command identifier and other relevant information
        */
        virtual void NotifySetUpIdleModeText(
            TRequestStatus& aStatus,
            TDes8& aPCmd ) = 0;

        /**
        * Cancels the request
        */
        virtual void NotifySetUpIdleModeTextCancel() = 0;

        /**
        * Issues the request
        * @param aStatus Request status.
        * @param aPCmd Command identifier and other relevant information
        */
        virtual void NotifySendDtmf(
            TRequestStatus& aStatus,
            TDes8& aPCmd ) = 0;

        /**
        * Cancels the request
        */
        virtual void NotifySendDtmfCancel() = 0;

        /**
        * Issues the request
        * @param aStatus Request status.
        * @param aPCmd Command identifier and other relevant information
        */
        virtual void NotifySendSs(
            TRequestStatus& aStatus,
            TDes8& aPCmd ) = 0;

        /**
        * Cancels the request
        */
        virtual void NotifySendSsCancel() = 0;

        /**
        * Issues the request
        * @param aStatus Request status.
        * @param aPCmd Command identifier and other relevant information
        */
        virtual void NotifySendUssd(
            TRequestStatus& aStatus,
            TDes8& aPCmd ) = 0;

        /**
        * Cancels the request
        */
        virtual void NotifySendUssdCancel() = 0;

        /**
        * Issues the request
        * @param aStatus Request status.
        * @param aPCmd Command identifier and other relevant information
        */
        virtual void NotifyPlayTone(
            TRequestStatus& aStatus,
            TDes8& aPCmd ) = 0;

        /**
        * Cancels the request
        */
        virtual void NotifyPlayToneCancel() = 0;

        /**
        * Issues the request
        * @param aStatus Request status.
        * @param aPCmd Command identifier and other relevant information
        */
        virtual void NotifyRefreshRequired(
            TRequestStatus& aStatus ,
            TDes8& aPCmd ) = 0;

        /**
        * Cancels the request
        */
        virtual void NotifyRefreshRequiredCancel() = 0;

        /**
        * Respond to NotifyRefreshRequired. Replaces Terminal Rsp here.
        * @param aPCkg Refresh required response package
        */
        virtual void RefreshAllowed( const TDesC8& aPCkg ) = 0;

        /**
        * Issues the request
        * @param aStatus Request status.
        * @param aPCmd Command identifier and other relevant information
        */
        virtual void NotifyRefresh(
            TRequestStatus& aStatus,
            TDes8& aPCmd ) = 0;

        /**
        * Cancels the request
        */
        virtual void NotifyRefreshCancel() = 0;

        /**
        * Issues the request
        * @param aStatus Request status.
        */
        virtual void NotifySimSessionEnd( TRequestStatus& aStatus ) = 0;

        /**
        * Cancels the SimSessionEnd request
        */
        virtual void NotifySimSessionEndCancel() = 0;

        /**
        * Issues the request
        * @param aStatus Request status.
        * @param aPCmd Command identifier and other relevant information
        */
        virtual void NotifyLocalInfo(
            TRequestStatus& aStatus,
            TDes8& aPCmd ) = 0;

        /**
        * Cancels the request
        */
        virtual void NotifyLocalInfoCancel() = 0;

        /**
        * Issues the request
        * @param aStatus Request status.
        * @param aPCmd Command identifier and other relevant information
        */
        virtual void NotifyOpenChannel(
            TRequestStatus& aStatus,
            TDes8& aPCmd ) = 0;

        /**
        * Cancels the request
        */
        virtual void NotifyOpenChannelCancel() = 0;

        /**
        * Issues the request
        * @param aStatus Request status.
        * @param aPCmd Command identifier and other relevant information
        */
        virtual void NotifyCloseChannel(
            TRequestStatus& aStatus,
            TDes8& aPCmd ) = 0;

        /**
        * Cancels the request
        */
        virtual void NotifyCloseChannelCancel() = 0;

        /**
        * Issues the request
        * @param aStatus Request status.
        * @param aPCmd Command identifier and other relevant information
        */
        virtual void NotifyGetChannelStatus(
            TRequestStatus& aStatus,
            TDes8& aPCmd ) = 0;

        /**
        * Cancels the request
        */
        virtual void NotifyGetChannelStatusCancel() = 0;

        /**
        * Issues the request
        * @param aStatus Request status.
        * @param aPCmd Command identifier and other relevant information
        */
        virtual void NotifySendData(
            TRequestStatus& aStatus,
            TDes8& aPCmd ) = 0;

        /**
        * Cancels the request
        */
        virtual void NotifySendDataCancel() = 0;

        /**
        * Issues the request
        * @param aStatus Request status.
        * @param aPCmd Command identifier and other relevant information
        */
        virtual void NotifyReceiveData(
            TRequestStatus& aStatus,
            TDes8& aPCmd ) = 0;

        /**
        * Cancels the request
        */
        virtual void NotifyReceiveDataCancel() = 0;

        /**
        * Sends menu selection to SIM.
        * @param aMenuSelection Selected menu item
        */
        virtual void SendMenuSelection( const TDes8& aMenuSelection ) = 0;

        /**
        * Sends terminal response to SIM.
        * @param aPCmd Proactive command, which is sending the Terminal Rsp.
        * @param aPckg Packed data of terminal rsp.
        */
        virtual void TerminalRsp(
            RSat::TPCmd aPCmd,
            const TDesC8& aPckg ) = 0;

        /**
        * Gets the icon info from SIM.
        * @param aReqStatus Request status.
        * @param aRecordNumber Specifies the EF_IMG in SIM.
        * @param aIconEf Contains the icon informations from SIM in returning
        *        of the method.
        */
        virtual void GetIcon(
            TRequestStatus& aReqStatus,
            TUint8 aRecordNumber,
            RSat::TIcon& aIconEf ) = 0;

        /**
        * Gets the icon image.
        * @param aReqStatus Request status.
        * @param aInstanceInfo Informatio of the icon, which data is
        *        fetched from SIM.
        * @param aInstance Contains the icon data from SIM in returning of the
        *        method.
        */
        virtual void GetImageInstance(
            TRequestStatus& aReqStatus,
            const TDesC8& aInstanceInfo,
            TDes8& aInstance ) = 0;

        /**
        * Gets color lookup table of the icon.
        * @param aReqStatus Request status.
        * @param aInstanceNumber Image instance number.
        * @param aOffset Offset of the clut.
        * @param aClut Contains the Clut of the icon in returning of the
        *        method.
        */
        virtual void GetClut(
            TRequestStatus& aReqStatus,
            TUint aInstanceNumber,
            TUint aOffset,
            TDes8& aClut ) = 0;

        /**
        * Sends notify to SIM about some event using EventDownload.
        * @param aReqStatus Request status.
        * @param aSingleEvent Single event reported to SIM.
        * @param aEventInfo Contains information of an event.
        */
        virtual void EventDownload(
            TRequestStatus& aReqStatus,
            RSat::TEventList aSingleEvent,
            const TDesC8& aEventInfo ) = 0;

        /**
        * Indicates is RSat connected
        * @return ETrue if RSat is connected
        */
        virtual TBool IsRSatConnected() const = 0;
        
        /**
        * Called when all the supported notifies are sent to TSY
        * @return TInt from RSat
        */
        virtual TInt UsatClientReadyIndication() = 0;

    private:

        // Prohibit copy constructor if not deriving from CBase.
        MSatApi( const MSatApi& );

        // Prohibit assigment operator if not deriving from CBase.
        MSatApi& operator=( const MSatApi& );

    };

#endif      // MSATAPI_H

// End of File
