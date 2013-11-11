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
* Description: Base class for key sequence handlers.
*
*/

#ifndef KEYSEQUENCEHANDLER_H
#define KEYSEQUENCEHANDLER_H

#include <QObject>
#include <QString>

class KeySequenceHandler : public QObject
{
    Q_OBJECT
    
public:

    KeySequenceHandler(QObject *parent = 0);
    ~KeySequenceHandler();
    
    QString keySequenceValidator() const;
    virtual bool executeKeySequence(const QString &keySequence) = 0;

protected:
    void setKeySequenceValidator(const QString &validator);
    
private:
    QString m_keySequenceValidator;
};

#endif // KEYSEQUENCEHANDLER_H
