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
* Description: Provides SAT popups info and progress notes
*
*/

#ifndef SATAPPPOPUPPROVIDER_H
#define SATAPPPOPUPPROVIDER_H

// includes
#include <QObject>

// forward declarations
class HbMessageBox;
class HbDialog;
class HbDeviceMessageBox;
class SatAppAction;

class SatAppPopupProvider : public QObject
{
    Q_OBJECT

public:
    SatAppPopupProvider(QObject *parent = 0);
    virtual ~SatAppPopupProvider();
    
public slots:

    /** displays a text note on screen */
    void displayText(SatAppAction &action);

    /** shows a simple notification about an ongoing operation */
    void notification(SatAppAction &action);
    
    /** requests any ongoing wait note to be dismissed */
    void stopShowWaitNote();

    /** show a error note for send ss command */
    void showSsErrorNote();
    
    /** requests any ongoing wait note and display text to be dismissed */
    void clearScreen();
    
    /** reset some data members */
    void resetState();
    
private:
    QString alphaId(SatAppAction &action);
    void showBIPWaitNote(SatAppAction &action);
    void showCloseChannelWaitNote(SatAppAction &action);
    void showMoSmControlNote(SatAppAction &action);
    void showSatInfoNote(SatAppAction &action);
    void showSmsWaitNote(SatAppAction &action);
    void showCallControlNote(SatAppAction &action);        
    void showDtmfWaitNote(SatAppAction &action);
    void showSsWaitNote(SatAppAction &action);

private:
    HbMessageBox *mDisplayText;
    HbDialog *mWaitDialog;
    HbDeviceMessageBox *mWaitDeviceDialog;
    SatAppAction *mAction;
    
    // for unit testing
    friend class PopupProviderTest;
};

#endif /* SATAPPPOPUPPROVIDER_H */
