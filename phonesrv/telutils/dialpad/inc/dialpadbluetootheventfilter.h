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

#ifndef DIALPADBLUETOOTHEVENTFILTER_H
#define DIALPADBLUETOOTHEVENTFILTER_H

#include <QObject>

class QTimer;
class Dialpad;

/*!
    DialpadBluetoothEventFilter
    Class provides bluetooth on/off key handling.
*/
class DialpadBluetoothEventFilter : public QObject
{
    Q_OBJECT

public:
    explicit DialpadBluetoothEventFilter(Dialpad* dialpad, QObject* parent = 0);
    virtual ~DialpadBluetoothEventFilter();

protected:
    bool eventFilter(QObject *watched, QEvent *event);

private slots:
    /*!
       \fn void toggleBluetooth()

       Toggles bluetooth on or off.
    */
    void toggleBluetooth();


private:
    QTimer *mLongPressTimer;
    Dialpad* mDialpad;
};

#endif // DIALPADBLUETOOTHEVENTFILTER_H
