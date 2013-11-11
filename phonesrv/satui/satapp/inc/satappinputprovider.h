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

#ifndef SATAPPINPUTPROVIDER_H
#define SATAPPINPUTPROVIDER_H

// includes
#include <QObject>
#include <QTime>
#include <msatuiobserver.h>

// forward declarations
class HbDialog;
class SatAppAction;
class QTimer;

class SatAppInputProvider : public QObject
{
    Q_OBJECT

public:
    SatAppInputProvider(QObject *parent = 0);
    virtual ~SatAppInputProvider();
    
public slots:

    /** requests user to enter a SINGLE key, character or digit */
    void getInkey(SatAppAction& action);

    /** requests user to enter a STRING of free text, number or password etc. */
    void getInput(SatAppAction& action);

    /** Resets the UI state commits pending server requests if any */
    void resetState();
    
private slots:

    /** update the actions when content in getinput changed */
    void updateQueryAction();
    /** update the actions according the text */
    void updateQueryAction(QString text);
    /** get the input key in get inkey */
    void transmitInkeyFromDialog();
    /** get the input key in get inkey immediate */
    void transmitInkeyImmediately(QString text);
    /** get the input key in get inkey yesno query */
    void transmitInkeyTrue();
    /** get the input key in get inkey yesno query */
    void transmitInkeyFalse();
    /** get the get input string */
    void transmitStringFromDialog();
    /** clear the query timeout timer */
    void clearDurationTimer();

private: // private implementation

    /** show get inkey query */
    void showGetInkeyQuery(const QString &aHeading, 
        TSatCharacterSet aCharacterSet, int aDuration);
    /** show get inkey immediate query */    
    void showGetImmediateQuery(const QString &aHeading, int aDuration);    
    /** show get inkey yes no query */
   void showGetYesNoQuery(const QString &aText, int aDuration);
    
   /** show get input query */
    void showGetInputQuery(const QString &heading, const QString &content,
            TSatCharacterSet characterSet, int minLength, int maxLength,
            bool aHideInput);
    /** dialog type */
    enum DialogMode {
        GetInkey,
        GetYesNo,
        GetImmediate,
        GetInput};
    
    /** update dialog according type */
    void composeDialog(HbDialog *dlg, DialogMode mode);
    /** set the get inkey result to action */
    void transmitInkey(int key);
    /** start dialog timeout timer */
    void startDurationTimer(int duration);
    
private:
    /** not own */
    SatAppAction* mCurrentAction;
    /** own */
    HbDialog* mCurrentDialog;
    /** own */
    QTime mInputTimer;
    /** own */
    QTimer* mDurationTimer;

    /**for unit testing */
    friend class InputProviderTest;
};

#endif /* SATAPPINPUTPROVIDER_H */
