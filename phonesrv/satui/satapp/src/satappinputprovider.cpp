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
* Description: Provides input methods like query dialogs
*
*/

// qt
#include <QTimer>
#include <QtDebug>
// hb
#include <hbglobal.h>
#include <hbinputdialog.h>
#include <hbinputeditorinterface.h>
#include <hbinputstandardfilters.h>
#include <hbmessagebox.h>
#include <hblabel.h>
#include <hbaction.h>
// satapp
#include "satappinputprovider.h"
#include "satappconstant.h"
#include "satappaction.h"


// ----------------------------------------------------------------------------
// SatAppInputProvider::SatAppInputProvider
// ----------------------------------------------------------------------------
//
SatAppInputProvider::SatAppInputProvider(QObject *parent)
    : QObject(parent), mCurrentAction(0), mCurrentDialog(0)
{
    qDebug("SATAPP: SatAppInputProvider::SatAppInputProvider >");
    mDurationTimer = new QTimer(this);
    qDebug("SATAPP: SatAppInputProvider::SatAppInputProvider <");
}

// ----------------------------------------------------------------------------
// SatAppInputProvider::~SatAppInputProvider
// ----------------------------------------------------------------------------
//
SatAppInputProvider::~SatAppInputProvider()
{
    qDebug("SATAPP: SatAppInputProvider::~SatAppInputProvider >");
    delete mCurrentDialog;
    mCurrentDialog = 0;
    qDebug("SATAPP: SatAppInputProvider::~SatAppInputProvider <");
}

// ----------------------------------------------------------------------------
// SatAppInputProvider::getInkey
// requests user to enter a SINGLE key, character or digit
// ----------------------------------------------------------------------------
//
void SatAppInputProvider::getInkey(SatAppAction& action)
{
    qDebug("SATAPP: SatAppInputProvider::getInkey >");
    
    delete mCurrentDialog;
    mCurrentDialog=0;
    
    QString heading = action.value(KeyText).toString();
    qDebug() << "SATAPP: getInkey: heading=" << heading;
    
    TSatCharacterSet characterSet = 
        static_cast<TSatCharacterSet>(action.value(KeyCharacterSet).toInt());

    qDebug("SATAPP: SatAppInputProvider::getInkey char set %d", characterSet);

    // yes no query must have text.
    if ((0 == heading.length()) && (ESatYesNo == characterSet)) {
        action.completeWithFailure();
        return;
    }
    
    // duration
    int duration = action.value(KeyDuration).toInt();
    if (duration == 0) {
        duration = KDefaultSelectionTimeoutMseconds;
    }
    qDebug("SATAPP: getInkey: duration=%d", duration);
    mInputTimer.start();

    if (heading.isEmpty()) {
        if (ESatDigitOnly == characterSet) {
            heading = hbTrId("txt_simatk_dialog_entern1_digit_09");
        } else {
            // ESatCharSmsDefaultAlphabet or ESatCharUcs2Alphabet
            heading = hbTrId("txt_simatk_dialog_entern1_character");
        }
    }
    qDebug() << "SATAPP: getInkey: heading=" << heading;

    mCurrentAction = &action;
    bool immediate = action.value(KeyImmediate).toBool();
    qDebug() << "SATAPP: getInkey: immediate=" << immediate;
    
    if (characterSet == ESatYesNo) {
        //Yes no imediate will be combine with YES no.
        showGetYesNoQuery(heading, duration);
    }
    else if (immediate) {
        // we only have digit immediate
        showGetImmediateQuery(heading, duration);
    }
    else {
        // Reminder, there is no default inkey in get inkey
        // proactive command.
        showGetInkeyQuery(heading, characterSet, duration);
    }
    SAT_ASSERT(connect(mCurrentAction, SIGNAL(actionCompleted(SatAppAction*)),
        this, SLOT(clearDurationTimer())));
    qDebug("SATAPP: SatAppInputProvider::getInkey <");
}

// ----------------------------------------------------------------------------
// SatAppInputProvider::getInput
// requests user to enter a STRING of free text, number or password etc.
// ----------------------------------------------------------------------------
//
void SatAppInputProvider::getInput(SatAppAction& action)
{
    qDebug("SATAPP: SatAppInputProvider::getInput >");
 
    delete mCurrentDialog;
    mCurrentDialog=0;
    // character set
    TSatCharacterSet characterSet = 
        static_cast<TSatCharacterSet>(action.value(KeyCharacterSet).toInt());

    qDebug("SATAPP: SatAppInputProvider::getInput char set %d", characterSet);

    // heading
    QString heading = action.value(KeyText).toString();
    qDebug() << "SATAPP: getInput: heading=" << heading;
    if (heading.isEmpty()) {
        if (ESatDigitOnly == characterSet) {
            heading = hbTrId("txt_simatk_dialog_entern1_digit_09");
        } else {
            // ESatCharSmsDefaultAlphabet or ESatCharUcs2Alphabet
            heading = hbTrId("txt_simatk_dialog_entern1_character");
        }
    }
    qDebug() << "SATAPP: getInput: heading=" << heading;

    // min length
    int minLength = action.value(KeyMinLength).toInt();
    if (minLength<0) {
        minLength=0;
    }
    if (minLength>KGetInputStringMaxSize){
        minLength=KGetInputStringMaxSize;
    }

    // max length
    int maxLength = action.value(KeyMaxLength).toInt();
    if (maxLength<minLength) {
        maxLength=minLength;
    }
    if (maxLength>KGetInputStringMaxSize) {
        maxLength=KGetInputStringMaxSize;
    }
    qDebug("SATAPP: SatAppInputProvider::getInput min %d", minLength);
    qDebug("SATAPP: SatAppInputProvider::getInput max %d", maxLength);

    // default input text
    QString inputText = action.value(KeyInputString).toString();
    if (inputText.length() < minLength || inputText.length() > maxLength){
        inputText = "";
    }
    qDebug() << "SATAPP: getInput: inputText=" << inputText;

    // hide input
    bool hideInput = action.value(KeyHideInput).toBool();
    qDebug("SATAPP: SatAppInputProvider::getInput hide %d", hideInput);

    qDebug("SATAPP: getInput: min=%d,max=%d,hide=%d",
        minLength, maxLength, hideInput);
    
    mCurrentAction = &action;
    showGetInputQuery( heading, inputText,
            characterSet, minLength, maxLength, hideInput);
 
    SAT_ASSERT(connect(mCurrentAction, SIGNAL(actionCompleted(SatAppAction*)),
        this, SLOT(clearDurationTimer())));
    qDebug("SATAPP: SatAppInputProvider::getInput <");
}

// ----------------------------------------------------------------------------
// SatAppInputProvider::showGetInkeyQuery
// ----------------------------------------------------------------------------
//
void SatAppInputProvider::showGetInkeyQuery(
    const QString &heading,
    TSatCharacterSet characterSet,
    int duration)
{
    qDebug("SATAPP: SatAppInputProvider::showGetInkeyQuery >");

    HbInputDialog* inputDialog = new HbInputDialog();
    mCurrentDialog = inputDialog;
    // Set PromptText
    inputDialog->setPromptText(heading);
    // Set ContentText
    HbEditorInterface inputMode(inputDialog->lineEdit());
    inputDialog->lineEdit()->setFocus();
    if (ESatDigitOnly == characterSet) {
        // digit mode, digits only (0 9, *, #, and +)
        inputMode.setFilter(HbPhoneNumberFilter::instance());
        inputMode.setMode(HbInputModeNumeric);
        inputMode.setInputConstraints(HbEditorConstraintFixedInputMode);
    } else {
        // char mode
        inputMode.setUpAsLatinAlphabetOnlyEditor();
    }
    composeDialog(inputDialog, GetInkey);
    mCurrentAction->set(KeyMinLength, 1); 
    inputDialog->lineEdit()->setMaxLength(1);

    // when input changed, enable/disable OK button
    SAT_ASSERT(connect(inputDialog->lineEdit(), SIGNAL(textChanged(QString)),
        this, SLOT(updateQueryAction(QString))));
    // when OK pressed, fill in data to action and complete it
    SAT_ASSERT(connect(inputDialog->actions().at(0), SIGNAL(triggered()),
        this, SLOT(transmitInkeyFromDialog())));
    // when cancel pressed, complete action with ESatBackwardModeRequestedByUser
    SAT_ASSERT(connect(inputDialog->actions().at(1), SIGNAL(triggered()),
        mCurrentAction, SLOT(completeWithBackRequested())));
    // after timeout, reset the provider
    startDurationTimer(duration);
    // open the dialog. When the dialog closes, reset everything
    inputDialog->open();

    qDebug("SATAPP: SatAppInputProvider::showGetInkeyQuery <");
}

// ----------------------------------------------------------------------------
// SatAppInputProvider::showGetImmediateQuery
// ----------------------------------------------------------------------------
//
void SatAppInputProvider::showGetImmediateQuery(
    const QString &heading,
    int duration)
{
    qDebug("SATAPP: SatAppInputProvider::showGetImmediateQuery >");

    HbInputDialog* inputDialog = new HbInputDialog();
    mCurrentDialog = inputDialog;
    // Set PromptText
    inputDialog->setPromptText(heading);
    // Set ContentText
    HbEditorInterface inputMode(inputDialog->lineEdit());
    inputDialog->lineEdit()->setFocus();

    inputMode.setFilter(HbPhoneNumberFilter::instance());
    inputMode.setMode(HbInputModeNumeric);
    inputMode.setInputConstraints(HbEditorConstraintFixedInputMode);
    
    composeDialog(inputDialog, GetImmediate);

    // when receiving first input character, send it immediately
    SAT_ASSERT(connect(inputDialog->lineEdit(), SIGNAL(textChanged(QString)),
        this, SLOT(transmitInkeyImmediately(QString))));
    // when cancel pressed, complete action with ESatBackwardModeRequestedByUser
    SAT_ASSERT(connect(inputDialog->actions().at(0), SIGNAL(triggered()),
        mCurrentAction, SLOT(completeWithBackRequested())));
    // after timeout, reset the provider
    startDurationTimer(duration);
    // open the dialog. When the dialog closes, reset everything
    inputDialog->open();

    qDebug("SATAPP: SatAppInputProvider::showGetImmediateQuery <");
}

// ----------------------------------------------------------------------------
// SatAppInputProvider::showGetYesNoQuery
// ----------------------------------------------------------------------------
//
void SatAppInputProvider::showGetYesNoQuery(
    const QString &text,
    int duration)
{
    qDebug("SATAPP: SatAppInputProvider::showGetYesNoQuery >");
    HbMessageBox* messageBox = new HbMessageBox(HbMessageBox::MessageTypeInformation);
    mCurrentDialog = messageBox;

    // Set the label as heading widget
    HbLabel *lHeading = new HbLabel(text, messageBox);
    messageBox->setHeadingWidget(lHeading);
    messageBox->setIconVisible(false);

    composeDialog(messageBox, GetYesNo);

    // when YES pressed, complete action with ESatSuccess, inkey set to 1
    SAT_ASSERT(connect(messageBox->actions().at(0), SIGNAL(triggered()),
        this, SLOT(transmitInkeyTrue())));
    // when NO pressed, complete action with ESatSuccess, inkey set to 0
    SAT_ASSERT(connect(messageBox->actions().at(1), SIGNAL(triggered()),
        this, SLOT(transmitInkeyFalse())));
 
    startDurationTimer(duration);     
    // show dialog.
    messageBox->open();

    qDebug("SATAPP: SatAppInputProvider::showGetYesNoQuery <");
}

// ----------------------------------------------------------------------------
// SatAppInputProvider::showGetInputQuery
// ----------------------------------------------------------------------------
//
void SatAppInputProvider::showGetInputQuery(
    const QString& heading,
    const QString& content,
    TSatCharacterSet characterSet,
    int minLength,
    int maxLength,
    bool hideInput)
{
    Q_UNUSED(minLength);
    qDebug("SATAPP: SatAppInputProvider::showGetInputQuery >");

    HbInputDialog* inputQuery = new HbInputDialog();
    mCurrentDialog = inputQuery;
    inputQuery->setPromptText(heading);
    inputQuery->setValue(QVariant(content));
    HbEditorInterface inputMode(inputQuery->lineEdit());
    inputQuery->lineEdit()->setFocus();
    if (ESatDigitOnly == characterSet) {
        // digit mode, digits only (0 9, *, #, and +)
        inputMode.setFilter(HbPhoneNumberFilter::instance());
        inputMode.setMode(HbInputModeNumeric);
        inputMode.setInputConstraints(HbEditorConstraintFixedInputMode);
    } else {
        // char mode
        inputMode.setUpAsLatinAlphabetOnlyEditor();
    }

    inputQuery->lineEdit()->setMaxLength(maxLength);

    composeDialog(inputQuery, GetInput);
    // initially set the button states
    updateQueryAction(inputQuery->value().toString());

    if (hideInput) {
        inputQuery->lineEdit()->setEchoMode(HbLineEdit::Password);
        qDebug("SATAPP: showGetInputQuery: hiding input");
    }

    // when dialog content changes, dim ok button accordingly, use
    // use contentChanged signal from the base class of the editor
    // to get the signal when in password mode
    SAT_ASSERT(connect(inputQuery->lineEdit(), SIGNAL(contentsChanged()), 
        this, SLOT(updateQueryAction())));
    // transmit the text on OK press
    SAT_ASSERT(connect(inputQuery->actions().at(0), SIGNAL(triggered()),
        this, SLOT(transmitStringFromDialog())));
    // on Cancel key complete action with ESatBackwardModeRequestedByUser
    SAT_ASSERT(connect(inputQuery->actions().at(1), SIGNAL(triggered()),
        mCurrentAction, SLOT(completeWithBackRequested())));
    // after timeout, reset the provider
    startDurationTimer(KDefaultSelectionTimeoutMseconds);
    // open dialog. When it closes, reset everything
    inputQuery->open();

    qDebug("SATAPP: SatAppInputProvider::showGetInputQuery <");
}

// ----------------------------------------------------------------------------
// SatAppInputProvider::resetState
// ----------------------------------------------------------------------------
//
void SatAppInputProvider::resetState()
{
    qDebug("SATAPP: SatAppInputProvider::resetState >");
    if (mCurrentAction) {
        qDebug("SATAPP: SatAppInputProvider::resetState action");        
        mCurrentAction->completeWithNoResponse();
        mCurrentAction = 0;
    }

    qDebug("SATAPP: SatAppInputProvider::resetState action done");

    delete mCurrentDialog;
    mCurrentDialog = 0;
    qDebug("SATAPP: SatAppInputProvider::resetState <");
}

// ----------------------------------------------------------------------------
// SatAppInputProvider::composeDialog
// make sure dlg has 2 buttons
// ----------------------------------------------------------------------------
//
void SatAppInputProvider::composeDialog(
    HbDialog *dlg,
    DialogMode mode)
{
    qDebug("SATAPP: SatAppInputProvider::composeDialog >");
    Q_ASSERT(mCurrentAction);

    dlg->setModal(true);
    dlg->setDismissPolicy(HbDialog::NoDismiss);

    dlg->clearActions();
    HbAction *primaryAction = new HbAction(dlg);
    dlg->addAction(primaryAction);
    
    HbAction *secondaryAction = new HbAction(dlg);
    dlg->addAction(secondaryAction);
    
    switch(mode){
        case GetYesNo:
        {
            primaryAction->setText(hbTrId("txt_common_button_yes"));
            secondaryAction->setText(hbTrId("txt_common_button_no"));
            break;
        }
        case GetImmediate:
        {
            dlg->removeAction(primaryAction);
            secondaryAction->setText(hbTrId("txt_common_button_cancel"));
            break;
        }
        default:
        {
            primaryAction->setText(hbTrId("txt_common_button_ok"));
            primaryAction->setEnabled(false);
            secondaryAction->setText(hbTrId("txt_common_button_cancel"));
            break;
        }
    }
    qDebug("SATAPP: SatAppInputProvider::composeDialog <");
}

// ----------------------------------------------------------------------------
// SatAppInputProvider::updateQueryAction
// Updates dialog's OK button according to input text length limits
// ----------------------------------------------------------------------------
//
void SatAppInputProvider::updateQueryAction()
{
    qDebug("SATAPP: SatAppInputProvider::updateQueryAction >");
    Q_ASSERT(mCurrentDialog);
    QString content = 
        qobject_cast<HbInputDialog*>(mCurrentDialog)->value().toString();
    qDebug() << "SATAPP: updateQueryAction: content=" << content;
    updateQueryAction(content);
    qDebug("SATAPP: SatAppInputProvider::updateQueryAction >");
}

// ----------------------------------------------------------------------------
// SatAppInputProvider::updateQueryAction
// Updates dialog's OK button according to input text length limits
// ----------------------------------------------------------------------------
//
void SatAppInputProvider::updateQueryAction(QString text)
{
    qDebug("SATAPP: SatAppInputProvider::updateQueryAction 2>");
    Q_ASSERT(mCurrentDialog);
    qDebug() << "SATAPP: updateQueryAction 2: text=" << text;
    int min=0;
    if (mCurrentAction->hasValue(KeyMinLength))
        min=mCurrentAction->value(KeyMinLength).toInt();
    int max=10;
    if (mCurrentAction->hasValue(KeyMaxLength))
        max=mCurrentAction->value(KeyMaxLength).toInt();
    if (mCurrentDialog->actions().at(0)) {
        if (text.length() >= min && text.length() <= max) {
            mCurrentDialog->actions().at(0)->setEnabled(true);
        } else {
            mCurrentDialog->actions().at(0)->setEnabled(false);
        }
    }
    qDebug("SATAPP: SatAppInputProvider::updateQueryAction 2<");
}

// ----------------------------------------------------------------------------
// SatAppInputProvider::transmitInkeyFromDialog
// ----------------------------------------------------------------------------
//
void SatAppInputProvider::transmitInkeyFromDialog()
{
    qDebug("SATAPP: SatAppInputProvider::transmitInkeyFromDialog >");
    Q_ASSERT(mCurrentDialog);
    Q_ASSERT(mCurrentAction->action()==ESatGetInkeyAction);
    // grab the input string
    HbInputDialog* inkeyQuery = static_cast<HbInputDialog*>(mCurrentDialog);
    QString inputString = inkeyQuery->value().toString();
    qDebug() << "SATAPP: transmitInkeyFromDialog: input=" << inputString;
    transmitInkeyImmediately(inputString);
    qDebug("SATAPP: SatAppInputProvider::transmitInkeyFromDialog <");
}

// ----------------------------------------------------------------------------
// SatAppInputProvider::transmitInkeyImmediately
// ----------------------------------------------------------------------------
//
void SatAppInputProvider::transmitInkeyImmediately(QString text)
{
    qDebug("SATAPP: SatAppInputProvider::transmitInkeyImmediately >");
    transmitInkey((int)text.utf16()[0]);
    qDebug("SATAPP: SatAppInputProvider::transmitInkeyImmediately <");
}


// ----------------------------------------------------------------------------
// SatAppInputProvider::transmitInkeyTrue
// ----------------------------------------------------------------------------
//
void SatAppInputProvider::transmitInkeyTrue()
{
    qDebug("SATAPP: SatAppInputProvider::transmitInkeyTrue >");
    Q_ASSERT(mCurrentAction);
    Q_ASSERT(mCurrentAction->action()==ESatGetInkeyAction);
    Q_ASSERT(mCurrentAction->value(KeyCharacterSet).toInt()==ESatYesNo);
    transmitInkey(1);
    qDebug("SATAPP: SatAppInputProvider::transmitInkeyTrue <");
}

// ----------------------------------------------------------------------------
// SatAppInputProvider::transmitInkeyFalse
// ----------------------------------------------------------------------------
//
void SatAppInputProvider::transmitInkeyFalse()
{
    qDebug("SATAPP: SatAppInputProvider::transmitInkeyFalse >");
    Q_ASSERT(mCurrentAction);
    Q_ASSERT(mCurrentAction->action()==ESatGetInkeyAction);
    Q_ASSERT(mCurrentAction->value(KeyCharacterSet).toInt()==ESatYesNo);
    transmitInkey(0);
    qDebug("SATAPP: SatAppInputProvider::transmitInkeyFalse >");
}

// ----------------------------------------------------------------------------
// SatAppInputProvider::transmit
// ----------------------------------------------------------------------------
//
void SatAppInputProvider::transmitInkey(int key)
{
    qDebug("SATAPP: SatAppInputProvider::transmitInkey >");
    qDebug("SATAPP: SatAppInputProvider::transmitInkey %d",key);
    Q_ASSERT(mCurrentAction);
    Q_ASSERT(mCurrentAction->action()==ESatGetInkeyAction);
    // write inkey
    mCurrentAction->set(KeyInKey,key);
    // overwrite timer duration
    if (mCurrentAction->hasValue(KeyDuration)) {
        int newDuration = mInputTimer.elapsed();
        qDebug("SATAPP: SatAppInputProvider::transmitInkey dur ", newDuration);
        mCurrentAction->set(KeyDuration, newDuration);
    }
    bool immediate = mCurrentAction->value(KeyImmediate).toBool();
    if(immediate) {
        mCurrentDialog->close();
    }
    mCurrentAction->completeWithSuccess();
    mCurrentAction = 0;
    qDebug("SATAPP: SatAppInputProvider::transmitInkey >");
}

// ----------------------------------------------------------------------------
// SatAppInputProvider::transmitStringFromDialog
// ----------------------------------------------------------------------------
//
void SatAppInputProvider::transmitStringFromDialog()
{
    qDebug("SATAPP: SatAppInputProvider::transmitStringFromDialog >");
    Q_ASSERT(mCurrentDialog);
    Q_ASSERT(mCurrentAction);
    Q_ASSERT(mCurrentAction->action()==ESatGetInputAction);
    // write input string
    HbInputDialog* inputQuery = static_cast<HbInputDialog*>(mCurrentDialog);
    QString inputString = inputQuery->value().toString();
    qDebug() << "SATAPP: transmitStringFromDialog: string=" << inputString;

    mCurrentAction->set(KeyInputString,inputString);
    // overwrite timer duration
    if (mCurrentAction->hasValue(KeyDuration)) {
        int newDuration = mInputTimer.elapsed();
        qDebug("SATAPP: SatAppInputProvider::transmitStringFromDialog dur ",
            newDuration);
        mCurrentAction->set(KeyDuration, newDuration);
    }
    mCurrentAction->completeWithSuccess();
    mCurrentAction = 0;
    qDebug("SATAPP: SatAppInputProvider::transmitStringFromDialog >");
}

// ----------------------------------------------------------------------------
// SatAppInputProvider::startDurationTimer
// ----------------------------------------------------------------------------
//
void SatAppInputProvider::startDurationTimer(int duration)
{
    qDebug("SATAPP: SatAppInputProvider::startDurationTimer %d>", duration);
    Q_ASSERT(mDurationTimer);    
    if (duration) {
        mDurationTimer->setSingleShot(true);
        SAT_ASSERT(connect(mDurationTimer, SIGNAL(timeout()),
            this, SLOT(resetState())));
        mDurationTimer->start(duration);
    }
    qDebug("SATAPP: SatAppInputProvider::startDurationTimer <");    
}

// ----------------------------------------------------------------------------
// SatAppInputProvider::clearDurationTimer
// ----------------------------------------------------------------------------
//
void SatAppInputProvider::clearDurationTimer()
{
    qDebug("SATAPP: SatAppInputProvider::clearDurationTimer >");    
    Q_ASSERT(mDurationTimer);
    Q_ASSERT(mCurrentAction);

    bool res = disconnect(mCurrentAction, SIGNAL(actionCompleted(SatAppAction*)),
        this, SLOT(clearDurationTimer()));
    qDebug("SATAPP: SatAppInputProvider::clearDurationTimer act %d>", res);
    res = disconnect(mDurationTimer, SIGNAL(timeout()), 
        this, SLOT(resetState()));
    qDebug("SATAPP: SatAppInputProvider::clearDurationTimer dur %d>", res);
    
    mDurationTimer->stop();
    qDebug("SATAPP: SatAppInputProvider::clearDurationTimer <");    
}

//End of file
