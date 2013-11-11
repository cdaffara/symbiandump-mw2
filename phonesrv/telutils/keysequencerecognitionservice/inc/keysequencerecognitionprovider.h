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
* Description: Implements Qt Highway provider for key sequence handling.
*
*/

#ifndef KEYSEQUENCERECOGNITIONPROVIDER_H
#define KEYSEQUENCERECOGNITIONPROVIDER_H

#include <xqserviceprovider.h>

class KeySequenceHandler;

class KeySequenceRecognitionProvider : public XQServiceProvider
{
    Q_OBJECT

public:

    KeySequenceRecognitionProvider(QObject *parent = 0);
    ~KeySequenceRecognitionProvider();
    
public slots:
    
    /*!
        \fn keySequenceValidator()
        
        Returns regular expression for key sequence validation. Client must 
        validate sequence before requesting provider to execute it. Regular
        expression is in escaped format.
        
        \return Escaped regular expression.
        
        Usage example:
        XQAiwRequest *request = mAiwMgr.create(
            "com.nokia.symbian.IKeySequenceRecognition",
            "keySequenceValidator()", 
            true);
        
        QVariant keySequenceValidator;
        bool requestOk = request->send(keySequenceValidator);
        if (requestOk && keySequenceValidator.toString().size()) {
            QString expression = keySequenceValidator.toString();
            mValidator = new QRegExp(expression);
        }
    */
    QString keySequenceValidator();
    
    /*!
        \fn executeKeySequence(const QString& keySequence)
        
        Client can use this method to execute key sequence.
        Key sequence may contain for example product codes etc. that
        may cause lower layers to do any specific things.
        
        \param  keySequence          Key sequence to execute.
        \return True if key sequence was processed, false otherwise.
        
        Usage example:
        XQAiwRequest *request = mAiwMgr.create(
            "com.nokia.symbian.IKeySequenceRecognition",
            "executeKeySequence(QString)", 
            true);
        
        QList<QVariant> arguments;
        arguments << "*#0000#";
        request->setArguments(arguments);
        
        QVariant keySequenceProcessed;
        bool requestOk = request.send(keySequenceProcessed);
    */
    bool executeKeySequence(const QString &keySequence);

private:
    void setupLocalization();
    void constructKeySequenceHandlers();
    void constructKeySequenceValidator();
    
private:
    QString m_keySequenceValidator;
    QList<KeySequenceHandler*> m_handlers;
};

#endif // KEYSEQUENCERECOGNITIONPROVIDER_H
