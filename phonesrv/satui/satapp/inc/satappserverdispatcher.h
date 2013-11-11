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
* Description: Receives UI commands from SAT server and converts to QT
*
*/

#ifndef SATAPPSERVERDISPATCHER_H
#define SATAPPSERVERDISPATCHER_H

#include <QObject>
#include <msatuiobserver.h>
#include <rsatuisession.h>
#include <rsatsession.h>

// forward declarations
class SatAppAction;

class SatAppServerDispatcher: public QObject, public MSatUiObserver
{
    Q_OBJECT

public:

    SatAppServerDispatcher(QObject *parent = 0);

    virtual ~SatAppServerDispatcher();

signals:

    /** signal request to display a menu */
    void setUpMenu(SatAppAction &action);

    /** requests a list selection menu to be displayed */
    void selectItem(SatAppAction &action);

    /** Requtes a text to be displayed to the user*/
    void displayText(SatAppAction &action);

    /** requests a key input to be displayed */
    void getInkey(SatAppAction &action);

    /** requests a string input to be displayed */
    void getInput(SatAppAction &action);

    /** Requests tone to be played */
    void playTone(SatAppAction &action);

    /** requests a confirm string to be displayed */
    void confirmCommand(SatAppAction &action);

    /** Requests an info notification or wait/status note */
    void notification(SatAppAction &action);
  
    /** requests any ongoing wait note to be dismissed */
    void stopShowWaitNote();

    /** requests all ongoing activities to be cleared on SAT UI */
    void clearScreen();

    /** requests to show ss error note*/
    void showSsErrorNote();

public slots:

     /**
     * Response to the Set Up Menu command.
     * @param aMenuItem The selected menu item.
     * @param aHelpRequested Indicates whether help was requested.
     */
    void menuSelection(SatAppAction* action);

    /**
    * User cancel response, Send DTMF, Send Data, Receive Data
    */
    void userCancelSession(SatAppAction* action);
    
    
    // ****************************************************************************
    // * ************************************************************************ *
    // * *                          SYMBIAN PART                                * *
    // * ************************************************************************ *
    // ****************************************************************************

private:
    
    void ConnectSatSessionL();
    void DisconnectSatSession();
    
public: // from MSatUiObserver

    /**
     * see msatuiobserver.h
     */
    TSatUiResponse DisplayTextL( const TDesC& aText,
        const TDesC& aSimApplicationName,
        const TSatIconId& aIconId,
        TBool& aRequestedIconDisplayed,
        const TBool aSustainedText,
        const TTimeIntervalSeconds aDuration,
        const TBool aWaitUserToClear );

    /**
     * see msatuiobserver.h
     */
    TSatUiResponse GetInkeyL( const TDesC& aText,
        const TSatCharacterSet aCharacterSet, TChar& aInput,
        const TBool aHelpIsAvailable,
        const TSatIconId& aIconId,
        TBool& aRequestedIconDisplayed,
        TUint& aDuration,
        const TBool aImmediateDigitResponse );

    /**
     * see msatuiobserver.h
     */
    TSatUiResponse GetInputL( const TDesC& aText,
        const TSatCharacterSet aCharacterSet, TDes& aInput,
        const TInt aMinLength, const TInt aMaxLength,
        const TBool aHideInput,
        const TBool aHelpIsAvailable,
        const TSatIconId& aIconId,
        TBool& aRequestedIconDisplayed );

    /**
     * see msatuiobserver.h
     */
    TSatUiResponse SetUpMenuL( const TDesC& aText,
        const MDesCArray& aMenuItems,
        const CArrayFixFlat<TSatAction>* aMenuItemNextActions,
        const TBool aHelpIsAvailable,
        const TSatIconId& aIconId,
        const CArrayFixFlat<TInt>* aMenuIcons,
        const enum TSatIconQualifier aIconListQualifier,
        const enum TSatSelectionPreference aSelectionPreference );

    /**
     * see msatuiobserver.h
     */
    TSatUiResponse SelectItemL( const TDesC& aText,
        const MDesCArray& aMenuItems,
        const CArrayFixFlat<TSatAction>* aMenuItemNextActions,
        const TInt aDefaultItem, TUint8& aSelection,
        const TBool aHelpIsAvailable,
        const TSatIconId& aIconId,
        const CArrayFixFlat<TInt>* aMenuIcons,
        const enum TSatIconQualifier aIconListQualifier,
        TBool& aRequestedIconDisplayed,
        const enum TSatSelectionPreference aSelectionPreference );

    /**
     * see msatuiobserver.h
     */
    TSatUiResponse PlayTone( const TDesC& aText,
        const TSatTone aTone,
        const TTimeIntervalMicroSeconds aDuration,
        const TSatIconId& aIconId,
        TBool& aRequestedIconDisplayed );

    /**
     * see msatuiobserver.h
     */
    void ConfirmCommand(
        const TSatSQueryCommand aCommandId,
        const TSatAlphaIdStatus aAlphaIdStatus,
        const TDesC& aText,
        const TDesC& aAdditionalText,
        TBool& aActionAccepted,
        const TSatIconId& aIconId,
        TBool& aRequestedIconDisplayed,
        TBool& aTerminatedByUser );

   /**
    * see msatuiobserver.h
    */
    TSatUiResponse Notification(
        const TSatSNotifyCommand aCommandId,
        const TSatAlphaIdStatus aAlphaIdStatus,
        const TDesC& aText,
        const TSatIconId& aIconId,
        TBool& aRequestedIconDisplayed,
        const TSatControlResult aControlResult );

    /**
     * see msatuiobserver.h
     */
    void EventNotification(
        const TSatSEvent aEventId,
        const TSatSEventStatus aEventStatus,
        const TInt aError );

private:
    
    RSatUiSession iSat;
    RSatSession iSatSession;

};

#endif //SATAPPSERVERDISPATCHER_H
