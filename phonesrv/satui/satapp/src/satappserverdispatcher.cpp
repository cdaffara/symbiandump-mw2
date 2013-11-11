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

// qt
#include <QStringList>
#include <qglobal.h>
#include <QCoreApplication>
// symbian
#include <centralrepository.h>
#include <ProfileEngineSDKCRKeys.h>

// sat
#include <msatuiadapter.h>
#include "satappserverdispatcher.h"
#include "satappaction.h"
#include "satappconstant.h"


// ----------------------------------------------------------------------------
// SatAppServerDispatcher::SatAppServerDispatcher
// ----------------------------------------------------------------------------
//
SatAppServerDispatcher::SatAppServerDispatcher(QObject *parent) :
    QObject(parent)
{
    qDebug("SATAPP: SatAppServerDispatcher::SatAppServerDispatcher");
    QT_TRAP_THROWING(ConnectSatSessionL());
}

// ----------------------------------------------------------------------------
// SatAppServerDispatcher::~SatAppServerDispatcher
// ----------------------------------------------------------------------------
//
SatAppServerDispatcher::~SatAppServerDispatcher()
{
    qDebug("SATAPP: SatAppServerDispatcher::~SatAppServerDispatcher");
    DisconnectSatSession();
}

// ----------------------------------------------------------------------------
// SatAppServerDispatcher::menuSelection
// ----------------------------------------------------------------------------
//
void SatAppServerDispatcher::menuSelection(SatAppAction* action)
{
    qDebug("SATAPP: SatAppServerDispatcher::menuSelection >");
    if ( ESatSuccess == action->response() ) {
        // user selected item from menu
        int menuItem = action->value(KeySelection).toInt();
        int helpRequested = action->value(KeyHelpRequested).toBool();
        qDebug("SATAPP: SatAppServerDispatcher::MenuSelection item=%d",
            menuItem);
        iSat.Adapter()->MenuSelection(menuItem, helpRequested);
    } else if (ESatSessionTerminatedByUser == action->response()) {
        // session terminated while executing the menu action
        qDebug("SATAPP: SatAppServerDispatcher::SessionTerminated");
        iSat.Adapter()->SessionTerminated(EEndKeyUsed);
    }
    delete action;
    qDebug("SATAPP: SatAppServerDispatcher::menuSelection <");
}

// ----------------------------------------------------------------------------
// SatAppServerDispatcher::userCancelResponse
// ----------------------------------------------------------------------------
//
void SatAppServerDispatcher::userCancelSession(SatAppAction* action)
{
    qDebug("SATAPP: SatAppServerDispatcher::userCancelSession");
    if (action && ESatBackwardModeRequestedByUser == action->response()){
        iSat.Adapter()->SessionTerminated(ESessionCancel);
    }    
    delete action;
    action = 0;
}


// ****************************************************************************
// * ************************************************************************ *
// * *                          SYMBIAN PART                                * *
// * ************************************************************************ *
// ****************************************************************************

// ----------------------------------------------------------------------------
// SatAppServerDispatcher::ConnectSatSessionL
// ----------------------------------------------------------------------------
//
void SatAppServerDispatcher::ConnectSatSessionL()
{
    qDebug("SATAPP: SatAppServerDispatcher::ConnectSatSessionL>");
    //Register to Server as subsession
    //If Server is not up, this function call may take time
    iSatSession.ConnectL();
    iSat.RegisterL(iSatSession, this);
    if (!iSat.Adapter())
        User::Leave(KErrNotFound);
    qDebug("SATAPP: SatAppServerDispatcher::ConnectSatSessionL<");
}

// ----------------------------------------------------------------------------
// SatAppServerDispatcher::DisconnectSatSession
// ----------------------------------------------------------------------------
//
void SatAppServerDispatcher::DisconnectSatSession()
{
    qDebug("SATAPP: SatAppServerDispatcher::DisconnectSatSession>");
    iSat.Close();
    iSatSession.Close();
    qDebug("SATAPP: SatAppServerDispatcher::DisconnectSatSession<");
}

// ----------------------------------------------------------------------------
// My own quick string -> descriptor conversion function :-)
// ----------------------------------------------------------------------------
//
void s2d(const QString str, TDes& desc)
{
    desc.Copy(reinterpret_cast<const TUint16*>(str.utf16()), str.length());
}

// ----------------------------------------------------------------------------
// SatAppServerDispatcher::DisplayTextL
// ----------------------------------------------------------------------------
//
TSatUiResponse SatAppServerDispatcher::DisplayTextL( const TDesC& aText,
    const TDesC& aSimApplicationName,
    const TSatIconId& aIconId,
    TBool& aRequestedIconDisplayed,
    const TBool aSustainedText,
    const TTimeIntervalSeconds aDuration,
    const TBool aWaitUserToClear )
{
    qDebug("SATAPP: SatAppServerDispatcher::DisplayTextL >");
    Q_UNUSED(aIconId);
    SatAppAction action(ESatDisplayTextAction);
    // validate
    if (aText.Length()== 0 || aText.Length()>RSat::KTextToDisplayMaxSize){
        qDebug("SATAPP: SatAppServerDispatcher::DisplayText no text");
        aRequestedIconDisplayed = EFalse;
        return ESatFailure;
    }

    QT_TRYCATCH_LEAVING(
        // ensure state
        emit stopShowWaitNote();

        // call
        action.set(KeyText,aText);
        action.set(KeyApplicationName, aSimApplicationName);
        action.set(KeySustainedText, aSustainedText);
        action.set(KeyDuration, const_cast<TTimeIntervalSeconds &>(aDuration));
        action.set(KeyWaitUserToClear, aWaitUserToClear);

        emit displayText(action);
        action.waitUntilCompleted();
    );

    // return
    qDebug("SATAPP: SatAppServerDispatcher::DisplayTextL action.response() %x<", 
        action.response());
    return action.response();
}

// ----------------------------------------------------------------------------
// SatAppServerDispatcher::GetInkeyL
// ----------------------------------------------------------------------------
//
TSatUiResponse SatAppServerDispatcher::GetInkeyL(
    const TDesC& aText,
    const TSatCharacterSet aCharacterSet,
    TChar& aInput,
    const TBool aHelpIsAvailable,
    const TSatIconId& aIconId,
    TBool& aRequestedIconDisplayed,
    TUint& aDuration,
    const TBool aImmediateDigitResponse )
{
    Q_UNUSED(aHelpIsAvailable);
    Q_UNUSED(aIconId);
    
    qDebug("SATAPP: SatAppServerDispatcher::GetInkeyL >");

    // validate
    aRequestedIconDisplayed = EFalse;
    if (ESatYesNo == aCharacterSet && aText.Length() == 0) {
        qDebug("SATAPP: CSatUiObserver::GetInkey return ESatFailure");
        return ESatFailure;
    }

    SatAppAction action(ESatGetInkeyAction);
    QT_TRYCATCH_LEAVING(
        // ensure state
        emit stopShowWaitNote();

        // call
        action.set(KeyText,aText);
        action.set(KeyCharacterSet,(int)aCharacterSet);
        action.set(KeyInKey,(int)aInput);
        // convert from SAT time units(?) to milliseconds
        action.set(KeyDuration,(int)aDuration*KSymbianTimeConvertQtTime);
        action.set(KeyImmediate,(bool)aImmediateDigitResponse);
        emit getInkey(action);
        action.waitUntilCompleted();
        // convert from milliseconds to SAT time units(?)
        aDuration = 
                action.value(KeyDuration).toInt()/KSymbianTimeConvertQtTime;
        aInput = action.value(KeyInKey).toInt();
    );

    // return
    qDebug("SATAPP: SatAppServerDispatcher::GetInkeyL <");
    return action.response();
}

// ----------------------------------------------------------------------------
// SatAppServerDispatcher::GetInputL
// ----------------------------------------------------------------------------
//
TSatUiResponse SatAppServerDispatcher::GetInputL(
    const TDesC& aText,
    const TSatCharacterSet aCharacterSet,
    TDes& aInput,
    const TInt aMinLength,
    const TInt aMaxLength,
    const TBool aHideInput,
    const TBool aHelpIsAvailable,
    const TSatIconId& aIconId,
    TBool& aRequestedIconDisplayed )
{
    Q_UNUSED(aHelpIsAvailable);
    Q_UNUSED(aIconId);
    qDebug("SATAPP: SatAppServerDispatcher::GetInputL >");

    // validate
    aRequestedIconDisplayed = EFalse;
    if ((aCharacterSet == ESatCharSmsDefaultAlphabet ||
         aCharacterSet == ESatCharUcs2Alphabet) &&
         aHideInput) {
        qDebug("SATAPP: SatAppServerDispatcher::GetInput return ESatFailure");
        return ESatFailure;
    }

    SatAppAction action(ESatGetInputAction);
    QT_TRYCATCH_LEAVING(
        // ensure state
        emit stopShowWaitNote();
    
        // call
        action.set(KeyText,aText);
        action.set(KeyCharacterSet,(int)aCharacterSet);
        action.set(KeyInputString,aInput);
        action.set(KeyMinLength,aMinLength);
        action.set(KeyMaxLength,aMaxLength);
        action.set(KeyHideInput,(bool)aHideInput);
        emit getInput(action);
        action.waitUntilCompleted();
        s2d(action.value(KeyInputString).toString(), aInput);
    )

    // return
    qDebug("SATAPP: SatAppServerDispatcher::GetInputL <");
    return action.response();
}

// ----------------------------------------------------------------------------
// SatAppServerDispatcher::SetUpMenuL
// ----------------------------------------------------------------------------
//
TSatUiResponse SatAppServerDispatcher::SetUpMenuL(
    const TDesC& aText,
    const MDesCArray& aMenuItems,
    const CArrayFixFlat<TSatAction>* aMenuItemNextActions,
    const TBool aHelpIsAvailable,
    const TSatIconId& aIconId,
    const CArrayFixFlat<TInt>* aMenuIcons,
    const enum TSatIconQualifier aIconListQualifier,
    const enum TSatSelectionPreference aSelectionPreference )
{
    Q_UNUSED(aMenuItemNextActions);
    Q_UNUSED(aHelpIsAvailable);
    Q_UNUSED(aIconId);
    Q_UNUSED(aMenuIcons);
    Q_UNUSED(aIconListQualifier);
    Q_UNUSED(aSelectionPreference);
    qDebug("SATAPP: SatAppServerDispatcher::SetUpMenuL >");

    QT_TRYCATCH_LEAVING(

        // validate
        if (!aMenuItems.MdcaCount()) {
            emit clearScreen();
            QCoreApplication::instance()->quit();
            return ESatSuccess;
        }

        // ensure state
        emit stopShowWaitNote();
    
        // call
        SatAppAction* action = new SatAppAction(ESatSetUpMenuAction, this);
        action->set(KeyText,aText);
        action->set(KeyMenu,aMenuItems);
        // connect for asynchronous menu selection
        connect(
            action,SIGNAL(actionCompleted(SatAppAction*)),
            this,SLOT(menuSelection(SatAppAction*)));
        
        emit setUpMenu(*action);
    )

    // return
    qDebug("SATAPP: SatAppServerDispatcher::SetUpMenuL <");
    return ESatSuccess;
}

// ----------------------------------------------------------------------------
// SatAppServerDispatcher::SelectItemL
// ----------------------------------------------------------------------------
//
TSatUiResponse SatAppServerDispatcher::SelectItemL(
    const TDesC& aText,
    const MDesCArray& aMenuItems,
    const CArrayFixFlat<TSatAction>* aMenuItemNextActions,
    const TInt aDefaultItem,
    TUint8& aSelection,
    const TBool aHelpIsAvailable,
    const TSatIconId& aIconId,
    const CArrayFixFlat<TInt>* aMenuIcons,
    const enum TSatIconQualifier aIconListQualifier,
    TBool& aRequestedIconDisplayed,
    const enum TSatSelectionPreference aSelectionPreference )
{
    Q_UNUSED(aMenuItemNextActions);
    Q_UNUSED(aHelpIsAvailable);
    Q_UNUSED(aIconId);
    Q_UNUSED(aMenuIcons);
    Q_UNUSED(aIconListQualifier);
    Q_UNUSED(aRequestedIconDisplayed);
    Q_UNUSED(aSelectionPreference);
    TSatUiResponse resp(ESatFailure);
    SatAppAction action(ESatSelectItemAction);
    qDebug("SATAPP: SatAppServerDispatcher::SelectItemL >");

    QT_TRYCATCH_LEAVING(
        // ensure state
        emit stopShowWaitNote();
    
        // call
        action.set(KeyText,aText);
        action.set(KeyMenu,aMenuItems);
        action.set(KeyDefault,aDefaultItem);
        emit selectItem(action);
        action.waitUntilCompleted();
        resp = action.response();
        if ( ESatSuccess == resp ) {
            aSelection = static_cast<TUint8>(
                    action.value(KeySelection).toInt());
        }
    )

    // return
    qDebug("SATAPP: SatAppServerDispatcher::SelectItemL <");
    return resp;
}

// ----------------------------------------------------------------------------
// SatAppServerDispatcher::PlayTone
// ----------------------------------------------------------------------------
//
TSatUiResponse SatAppServerDispatcher::PlayTone(
    const TDesC& aText,
    const TSatTone aTone,
    const TTimeIntervalMicroSeconds aDuration,
    const TSatIconId& aIconId,
    TBool& aRequestedIconDisplayed )
{
    Q_UNUSED(aIconId);
    aRequestedIconDisplayed = EFalse;
    TSatUiResponse resp(ESatSuccess);
    qDebug("SATAPP: SatAppServerDispatcher::PlayTone >");
    SatAppAction action(ESatPlayToneAction);
    int err(KErrNone);
    QT_TRYCATCH_ERROR(
        err,
        // ensure state
        emit stopShowWaitNote();
    
        // call
        action.set(KeyText,aText);
        action.set(KeyToneId,aTone);
        action.set(KeyDuration, const_cast<TTimeIntervalMicroSeconds &>(aDuration));
        emit playTone(action);
        action.waitUntilCompleted();
        resp = action.response();
    )

    // return
    qDebug("SATAPP: SatAppServerDispatcher::PlayTone err = %d<", err);
    return resp;
}


// ----------------------------------------------------------------------------
// SatAppServerDispatcher::ConfirmCommand
// ----------------------------------------------------------------------------
//
void SatAppServerDispatcher::ConfirmCommand(
    const TSatSQueryCommand aCommandId,
    const TSatAlphaIdStatus aAlphaIdStatus,
    const TDesC& aText,
    const TDesC& aAdditionalText,
    TBool& aActionAccepted,
    const TSatIconId& aIconId,
    TBool& aRequestedIconDisplayed,
    TBool& aTerminatedByUser )
{
    qDebug("SATAPP: SatAppServerDispatcher::ConfirmCommand >");
    Q_UNUSED(aIconId);
    aRequestedIconDisplayed = EFalse;
    aTerminatedByUser = EFalse;
    //ConfirmAction
    SatAppAction action(ESatConfirmAction);
    qDebug("SATAPP: SatAppServerDispatcher::ConfirmCommand");
    int err(KErrNone);
    QT_TRYCATCH_ERROR(
        err,
        emit stopShowWaitNote();
    
        action.set(KeyQueryCommandId, aCommandId);
        action.set(KeyAlphaIdStatus, aAlphaIdStatus);
        action.set(KeyText, aText);
        action.set(KeyAdditionalText, aAdditionalText);
        action.set(KeyActionAccepted, aActionAccepted);

        emit confirmCommand(action);
        action.waitUntilCompleted();
        if (ESatSuccess == action.response() ) {
            aActionAccepted = ETrue;
        }
    )
    qDebug("SATAPP: SatAppServerDispatcher::ConfirmCommand err = %d <", err);
}

// ----------------------------------------------------------------------------
// SatAppServerDispatcher::Notification
// ----------------------------------------------------------------------------
//
TSatUiResponse SatAppServerDispatcher::Notification(
    const TSatSNotifyCommand aCommandId,
    const TSatAlphaIdStatus aAlphaIdStatus,
    const TDesC& aText,
    const TSatIconId& aIconId,
    TBool& aRequestedIconDisplayed,
    const TSatControlResult aControlResult )
{
    Q_UNUSED(aIconId);
    Q_UNUSED(aRequestedIconDisplayed);
    qDebug("SATAPP: SatAppServerDispatcher::Notification command id=%d",
        aCommandId);

    TSatUiResponse res(ESatFailure);
    int err(KErrNone);
    switch (aCommandId)
    {
        // With cancel
        case ESatSSendDataNotify:    // fall through
        case ESatSReceiveDataNotify: // fall through
        case ESatSSendDtmfNotify:    // fall through
        {
            QT_TRYCATCH_ERROR(
                err,
                emit stopShowWaitNote();
                SatAppAction *action = new SatAppAction(ESatNotifyAction, this);
                action->set(KeyText, aText);
                action->set(KeyCommandId, aCommandId);
                action->set(KeyAlphaIdStatus, aAlphaIdStatus);
                action->set(KeyControlResult, aControlResult);
                // connect for asynchronous menu selection
                connect(
                    action,SIGNAL(actionCompleted(SatAppAction*)),
                    this,SLOT(userCancelSession(SatAppAction*)));
                emit notification(*action);
                res = ESatSuccess;
            )
            break;
        }
        // Without cancel
        case ESatSMoSmControlNotify:  // fall through
        case ESatSCallControlNotify:  // fall through 
        case ESatSSendUssdNotify:     // fall through 
        case ESatSSendSsNotify:       // fall through
        case ESatSSendSmsNotify:      // fall through
        case ESatSCloseChannelNotify: // fall through
        {
            QT_TRYCATCH_ERROR(
                err,
                emit stopShowWaitNote();
                SatAppAction a(ESatNotifyAction);
                a.set(KeyText, aText);
                a.set(KeyCommandId, aCommandId);
                a.set(KeyAlphaIdStatus, aAlphaIdStatus);
                a.set(KeyControlResult, aControlResult);
                emit notification(a);
                res = ESatSuccess;
            )
            break;
        }
        default:
        {
            break;
        }
    }
    qDebug("SATAPP: SatAppServerDispatcher::Notification < ret=%d", err);
    return res;
}

// ----------------------------------------------------------------------------
// SatAppServerDispatcher::EventNotification
// ----------------------------------------------------------------------------
//
void SatAppServerDispatcher::EventNotification(
    const TSatSEvent aEventId,
    const TSatSEventStatus aEventStatus,
    const TInt aError )
{
    qDebug( "SATAPP: SatAppServerDispatcher::EventNotification aEventId %d", 
        aEventId );
    Q_UNUSED(aEventStatus);
    Q_UNUSED(aError);
    int err(KErrNone);

    QT_TRYCATCH_ERROR( err,
    switch ( aEventId )
        {
        case ESatSSmEndEvent:
        case ESatSsEndEvent:
        case ESatSDtmfEndEvent:
            {
            emit stopShowWaitNote();
            break;
            }
        case ESatSClearScreenEvent:
            {
            qDebug("SATAPP: ClearScreen event");
            emit clearScreen();
            break;
            }
       case ESatSsErrorEvent:
            {
            qDebug("SATAPP: Notifying Ss error" );
            // If error occurred (and Alpha ID provided), notify user
            emit showSsErrorNote();
            break;
            }
        case ESatSCloseSatUiAppEvent:
            {
            qDebug(" Close UI event" );
            emit clearScreen();
            QCoreApplication::instance()->quit();
            break;
            }
        default:
            {
            qDebug("SATAPP: Unknown event occured: %i", aEventId);
            break;
            }
        }
    ) // end QT_TRYCATCH_ERROR
    qDebug("SATAPP: SatAppServerDispatcher::EventNotification err = %d<", err);
}

//End of file
