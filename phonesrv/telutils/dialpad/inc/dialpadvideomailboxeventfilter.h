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

#ifndef DIALPADVIDEOMAILBOXEVENTFILTER_H
#define DIALPADVIDEOMAILBOXEVENTFILTER_H

#include "dialpadmailboxeventfilterbase.h"

class QTimer;
class Dialpad;
class DialpadSymbianWrapper;

/*!
    DialpadVideoMailboxEventFilter
    Class provides video mailbox key handling.
*/
class DialpadVideoMailboxEventFilter : public DialpadMailboxEventFilterBase
{
    Q_OBJECT

public:
    explicit DialpadVideoMailboxEventFilter(Dialpad* dialpad, QObject* parent = 0);
    virtual ~DialpadVideoMailboxEventFilter();

protected:
    bool eventFilter(QObject *watched, QEvent *event);

    /*!
       \fn bool handleCallButtonPress()

       Check if editor has number string and if it does then createVideoCall
       is called and funtion returns true, if there is no number then
       funtion return false.
   */
    bool handleCallButtonPress();

    /*!
       \fn handleMailboxOperation

       Start vmbx call or number definition procedures.
    */
    void handleMailboxOperation();

protected slots:

    /*!
       \fn void handleLongKeyPress()
    
       Handles long key presses.
    */
    void handleLongKeyPress();

private:
    bool isVideoMbxSupported();
};

#endif // DIALPADVIDEOMAILBOXEVENTFILTER_H
