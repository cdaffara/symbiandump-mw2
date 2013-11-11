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

#include <QCoreApplication>
#include <QTimer>
#include <QTranslator>
#include <QLocale>
#include <tstasksettings.h>
#include "keysequencerecognitionprovider.h"
#include "manufacturerkeysequencehandler.h"
#include "imeikeysequencehandler.h"
#include "lifetimerkeysequencehandler.h"
#include "simcontrolkeysequencehandler.h"
#include "bluetoothkeysequencehandler.h"
#include "keysequencerecognitionservicelog.h"

/*!
  KeySequenceRecognitionProvider::KeySequenceRecognitionProvider.
 */
KeySequenceRecognitionProvider::KeySequenceRecognitionProvider(
    QObject* parent)
    : 
    XQServiceProvider(QLatin1String(
        "keysequencerecognitionservice.com.nokia.symbian.IKeySequenceRecognition"),
        parent),
    m_keySequenceValidator("")
{
    DPRINT_METHODENTRYEXIT;
    
    publishAll();
    
    setupLocalization();
    
    // One should not call anything exception generating after handlers are
    // created because memory for handlers will be leaked on an exception 
    // while being in c++ constructor.
    constructKeySequenceHandlers();

    // Keysequencerecognitionprovider to be invisible in taskswitcher 
    TsTaskSettings taskSettings;
    taskSettings.setVisibility(false);
}

/*!
  KeySequenceRecognitionProvider::~KeySequenceRecognitionProvider.
 */
KeySequenceRecognitionProvider::~KeySequenceRecognitionProvider()
{
    DPRINT_METHODENTRYEXIT;
}

/*!
  KeySequenceRecognitionProvider::keySequenceValidator.
 */
QString KeySequenceRecognitionProvider::keySequenceValidator()
{
    DPRINT_METHODENTRYEXIT;
    
    if (m_keySequenceValidator.isEmpty()) {
        constructKeySequenceValidator();
    }
    
    return m_keySequenceValidator;
}

/*!
  KeySequenceRecognitionProvider::executeKeySequence.
 */
bool KeySequenceRecognitionProvider::executeKeySequence(
    const QString &keySequence)
{
    DPRINT_METHODENTRYEXIT;
    
    bool handled = false;
    
    QList<KeySequenceHandler*>::const_iterator it = m_handlers.constBegin();
    for (;(it != m_handlers.constEnd()) && (!handled); ++it) {
        handled = (*it)->executeKeySequence(keySequence);
    }
    
    return handled;
}

/*!
  KeySequenceRecognitionProvider::setupLocalization.
 */
void KeySequenceRecognitionProvider::setupLocalization()
{
    DPRINT_METHODENTRYEXIT;
    
    QScopedPointer<QTranslator> translator(new QTranslator(this));
    
    QString locale = QLocale::system().name();
    QString path = QString("z:/resource/qt/translations/");
    bool translatorLoaded = 
        translator->load(QString(path + "telephone_" + locale));
    
    if (translatorLoaded) {
        qApp->installTranslator(translator.data());
        translator.take();
    }
}

/*!
  KeySequenceRecognitionProvider::constructKeySequenceHandlers.
 */
void KeySequenceRecognitionProvider::constructKeySequenceHandlers()
{
    DPRINT_METHODENTRYEXIT;
    
    QScopedPointer<KeySequenceHandler> manufacturerHandler( 
        new ManufacturerKeySequenceHandler(this));
    m_handlers.append(manufacturerHandler.data());
    manufacturerHandler.take();
    
    QScopedPointer<KeySequenceHandler> imeiHandler( 
        new ImeiKeySequenceHandler(this));
    m_handlers.append(imeiHandler.data());
    imeiHandler.take();

    QScopedPointer<KeySequenceHandler> lifeTimerHandler( 
        new LifeTimerKeySequenceHandler(this));
    m_handlers.append(lifeTimerHandler.data());
    lifeTimerHandler.take();
    
    QScopedPointer<KeySequenceHandler> simControlHandler( 
        new SimControlKeySequenceHandler(this));
    m_handlers.append(simControlHandler.data());
    simControlHandler.take();

    QScopedPointer<KeySequenceHandler> bluetoothHandler( 
        new BluetoothKeySequenceHandler(this));
    m_handlers.append(bluetoothHandler.data());
    bluetoothHandler.take();
}

/*!
  KeySequenceRecognitionProvider::constructKeySequenceValidator.
 */
void KeySequenceRecognitionProvider::constructKeySequenceValidator()
{
    DPRINT_METHODENTRYEXIT;
    
    QList<KeySequenceHandler*>::const_iterator it = m_handlers.constBegin();
    for (;it != m_handlers.constEnd(); ++it) {
        if (!m_keySequenceValidator.isEmpty()) {
            m_keySequenceValidator.append("|");
        }
        
        QString validator = (*it)->keySequenceValidator();
        m_keySequenceValidator.append(validator);
    }
}
