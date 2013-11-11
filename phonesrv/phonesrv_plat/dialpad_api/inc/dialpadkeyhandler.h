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
* Description: Dialpad key handler.
*
*/

#ifndef DIALPADKEYHANDLER_H
#define DIALPADKEYHANDLER_H

#include <QObject>

class Dialpad;
class DialpadVoiceMailboxEventFilter;
class DialpadVideoMailboxEventFilter;
class DialpadBluetoothEventFilter;
class DialpadKeySequenceEventFilter;
class DialpadEmergencyCallEventFilter;
class DialpadHashEventFilter;
#ifdef _DEBUG
 #ifdef __WINSCW__
  class DialpadInternalEventFilter;
 #endif
#endif
class HbMainWindow;

#ifdef BUILD_DIALPADKEYHANDLER
#define DIALPADKEYHANDLER_EXPORT Q_DECL_EXPORT
#else
#define DIALPADKEYHANDLER_EXPORT Q_DECL_IMPORT
#endif

/*!
    DialpadKeyHandler
    Class provides key handling for dialpad component.

    @code
    Dialpad *dialpad = new Dialpad();
    DialpadKeyHandler *keyhandler = new DialpadKeyHandler(dialpad, this);
    @endcode
*/
class DIALPADKEYHANDLER_EXPORT DialpadKeyHandler : public QObject
{
    Q_OBJECT

public:
    /*! Declares possible key event filters which can be used with the 
     * dialpad. */
    enum DialpadKeyEventFilter
    {
        /*! Enables calling to voice mailbox with a long '1' key press. */
        VoiceMailbox    = 0x0001,
        /*! Enables calling to video mailbox with a long '2' key press. */
        VideoMailBox    = 0x0002,
        /*! Enables switching bluetooth on/off with a long '*' key press. */
        Bluetooth       = 0x0004,
        /*! Enables handling of key sequences like *#06# for showing IMEI. */
        KeySequence     = 0x0008,
        /*! Forced handling for emergency call. */
        EmergencyCall   = 0x0010,
        /*! Enables and disables silent mode with a long '#' key press. */
        Hash    = 0x23
    };
    Q_DECLARE_FLAGS(DialpadKeyEventFilters, DialpadKeyEventFilter)
    
public:
    /*! \deprecated DialpadKeyHandler(Dialpad*, HbMainWindow&, QObject*) is 
     * deprecated. 
     * Please use 
     *     DialpadKeyHandler(
     *         Dialpad*, 
     *         QFlags<DialpadKeyHandler::DialpadKeyEventFilter>, 
     *         QObject*) 
     * instead. */
    explicit DialpadKeyHandler(
        Dialpad *dialPad, 
        HbMainWindow& mainWindow, 
        QObject *parent = 0);
    
    explicit DialpadKeyHandler(
        Dialpad *dialPad, 
        DialpadKeyHandler::DialpadKeyEventFilters filters,
        QObject *parent = 0);
    virtual ~DialpadKeyHandler();
    
private:
    HbMainWindow& mMainWindow;
    QScopedPointer<DialpadVoiceMailboxEventFilter> mVmbxFilter;
    QScopedPointer<DialpadVideoMailboxEventFilter> mVideoVmbxFilter;
    QScopedPointer<DialpadBluetoothEventFilter> mBtFilter;
    QScopedPointer<DialpadKeySequenceEventFilter> mKeySequenceFilter;
    QScopedPointer<DialpadEmergencyCallEventFilter> mEmergencyCallFilter;
    QScopedPointer<DialpadHashEventFilter> mHashFilter;
#ifdef _DEBUG
 #ifdef __WINSCW__
    QScopedPointer<DialpadInternalEventFilter> mInternalFilter;  
 #endif
#endif
};

#endif // DIALPADKEYHANDLER_H
