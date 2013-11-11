/*!
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
* Description:
*
*/

#ifndef DIALPADMAILBOXEVENTFILTERBASE_H
#define DIALPADMAILBOXEVENTFILTERBASE_H

#include <QObject>

class QTimer;
class Dialpad;
class DialpadSymbianWrapper;

enum err {
    DialpadErrorNone = 0,
    DialpadErrorCancel = -3
};
const int DialpadLongKeyPressTimeOut(1000);

/*!
    DialpadMailboxEventFilterBase
    Class provides voice mailbox key handling.
*/
class DialpadMailboxEventFilterBase : public QObject
{
    Q_OBJECT

public:
    explicit DialpadMailboxEventFilterBase(Dialpad* dialpad, QObject* parent = 0);
    virtual ~DialpadMailboxEventFilterBase();

protected:
    virtual bool eventFilter(QObject *watched, QEvent *event) = 0;

    /*!
        \fn bool isLongKeyPressSupported(const int key)

        Checks if key is numeric key'1' or numbersign key if it is then
        function return true, if key is something else for example
        yes/send then this returns false.
    */
    bool isLongKeyPressSupported(const int key);

    /*!
       \fn bool checkIfSendEventAndConsumeEvent(const int pressedKey, const int eventType)

       Checks that keycode is call related key if it is then handle it.
       */
    bool checkIfSendEventAndConsumeEvent(const int pressedKey, const int eventType);

    /*!
       \fn bool handleCallButtonPress()

       Check if editor has number string and if it does then createCall
       is called and funtion returns true, if there is no number then
       funtion return false.
   */
    virtual bool handleCallButtonPress() = 0;

    /*!
       \fn void clearEditor()

       Clears dialpad editor.
    */
    void clearEditor();

    /*!
       \fn createCall(const QString phoneNumber)

       Creates call request to QT Highway.
    */
    void createCall(const QString &phoneNumber, bool createVideoCall = false);

    /*!
       \fn handleMailboxOperation

       Start vmbx call or number definition procedures.
    */
    virtual void handleMailboxOperation() = 0;

    
protected slots:

    /*!
       \fn void handleLongKeyPress()
    
       Handles long key presses.
    */
    virtual void handleLongKeyPress() = 0;


protected:
    QTimer *mLongPressTimer;
    DialpadSymbianWrapper *mSymbianWrapper;
    Dialpad* mDialpad;
    int mKeyEvent;
};

#endif // DIALPADMAILBOXEVENTFILTERBASE_H
