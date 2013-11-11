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

#ifndef DIALPADEMERGENCYCALLEVENTFILTER_H
#define DIALPADEMERGENCYCALLEVENTFILTER_H

#include <QObject>

class Dialpad;
class CPhCltEmergencyCall;

/*!
    DialpadEmergencyCallEventFilter
    Class provides emergency call key handling.
*/
class DialpadEmergencyCallEventFilter : public QObject
{
    Q_OBJECT

public:
    explicit DialpadEmergencyCallEventFilter(Dialpad* dialpad, QObject* parent = 0);
    virtual ~DialpadEmergencyCallEventFilter();

protected:
    bool eventFilter(QObject *watched, QEvent *event);

private:

    /*!
       \fn void isEmergencyNumber()

       Checks is string emergency number.
    */
    bool isEmergencyNumber(QString string);
    
    /*!
       \fn void editorText()

       Returns text from editor (in western digits).
    */
    QString editorText();
    
    /*!
       \fn createCall(const QString phoneNumber)

       Creates call request to QT Highway.
    */
    void createCall(const QString &phoneNumber);

protected slots:

    /*!
        \fn void editorTextChanged()
        
        This slot is called when dialpad editor text is changed.
    */
    void editorTextChanged();
    
private:
    Dialpad* mDialpad;
    CPhCltEmergencyCall* mEmergency;
};

#endif // DIALPADEMERGENCYCALLEVENTFILTER_H
