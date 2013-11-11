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
* Description: Implements simcontrol key sequence handling.
*
*/

#include <qsysteminfo.h>
#include <qmobilityglobal.h>
#include <secui.h>
#include <secuimanualsecuritysettings.h>
#include "keysequencerecognitionservicedefs.h"
#include "keysequencerecognitionservicelog.h"
#include "simcontrolkeysequencehandler.h"


//QTM_USE_NAMESPACE

/*!
  SimControlKeySequenceHandler::SimControlKeySequenceHandler.
 */
SimControlKeySequenceHandler::SimControlKeySequenceHandler(
    QObject* parent)
    :
    KeySequenceHandler(parent),
    m_securityModel(0)
{
    DPRINT_METHODENTRYEXIT;
    
    setKeySequenceValidator(KCodeChangePin1 + "|" +
              KCodeChangePin2 + "|" +
              KCodeUnblockPin1 + "|" +
              KCodeUnblockPin2);
                                    
    QT_TRAP_THROWING(TSecUi::InitializeLibL());
    QT_TRAP_THROWING( m_securityModel = CManualSecuritySettings::NewL() );
}


/*!
  SimControlKeySequenceHandler::~SimControlKeySequenceHandler.
 */
SimControlKeySequenceHandler::~SimControlKeySequenceHandler()
{
    DPRINT_METHODENTRYEXIT;
    
    delete m_securityModel;    
    TSecUi::UnInitializeLib();
}


/*!
  SimControlKeySequenceHandler::executeKeySequence.
 */
bool SimControlKeySequenceHandler::executeKeySequence(
    const QString &keySequence)
{
    DPRINT_METHODENTRYEXIT;
   
    bool handled = parseString( keySequence );

    return handled;
}


/*!
  SimControlKeySequenceHandler::parseString.
 */
bool SimControlKeySequenceHandler::parseString(const QString &keySequence)
{
    DPRINT_METHODENTRYEXIT;
    
    const QString KChangePin1("**04*");
    const QString KChangePin2("**042");
    const QString KUnblockPin1("**05*");
    const QString KUnblockPin2("**052");
    
    QString oldPin;
    QString newPin;
    QString verifyNewPin;
    QString puk;
    
    SimOperation operation = None;
    bool handled = false;
   
    QString keySequencePrefix (keySequence);
    
    //Get first 5 chars from keysequence string
    keySequencePrefix.chop(keySequencePrefix.length()-5);
    QRegExp expression(QRegExp::escape(keySequencePrefix));
    
    QString parsedKeySequence(keySequence);
 
    //remove '#' from end 
    parsedKeySequence.chop(1);

    QStringList pins;
    
    if (expression.exactMatch(KChangePin1) || expression.exactMatch(KChangePin2))
            {
            if (expression.exactMatch(KChangePin1))
                  {
                  parsedKeySequence.remove(0, 5);
                  operation = Pin1;
                  }
            
            if (expression.exactMatch(KChangePin2))
                  { 
                  parsedKeySequence.remove(0, 6);
                  operation = Pin2;
                 }
            pins = parsedKeySequence.split("*");
            oldPin= pins.value(0);
            newPin = pins.value(1);
            verifyNewPin = pins.value(2);
            handled = true;
            processChangePin(operation, oldPin, newPin, verifyNewPin);
            }
    
    if (expression.exactMatch(KUnblockPin1) || expression.exactMatch(KUnblockPin2))
            {
            if ( expression.exactMatch(KUnblockPin1))
                  {
                  parsedKeySequence.remove(0, 5);
                  operation = Pin1;
                  }
            
            if (expression.exactMatch(KUnblockPin2))
                  { 
                  parsedKeySequence.remove(0, 6);
                  operation = Pin2;
                 }
            
            pins = parsedKeySequence.split("*");
            puk = pins.value(0);
            newPin = pins.value(1);
            verifyNewPin = pins.value(2);
            handled = true;
            processUnblockPin(operation, puk, newPin, verifyNewPin);
            }  
    
    return handled;
}

/*!
  SimControlKeySequenceHandler::processChangePin.
 */
void SimControlKeySequenceHandler::processChangePin(SimOperation operation, const QString &oldPin,
                                                   const QString &newPin, const QString &verifyNew)
    {
    CManualSecuritySettings::TPin pin;
    
        if(operation == Pin1)
            {
            pin = CManualSecuritySettings::EPin1;
            }
        else
            {
            pin = CManualSecuritySettings::EPin2;
            }
       
       TBuf<200> oldPinBuf(oldPin.utf16());
       TBuf<200> newPinBuf(newPin.utf16());                
       TBuf<200> verifyNewBuf(verifyNew.utf16()); 
          
       QT_TRAP_THROWING(m_securityModel->ChangePinL(pin, oldPinBuf, newPinBuf, verifyNewBuf));
    }

/*!
  SimControlKeySequenceHandler::processUnblockPin.
 */
void SimControlKeySequenceHandler::processUnblockPin(SimOperation operation, const QString &puk, 
                                                   const QString &newPin, const QString &verifyNew)
    {
    CManualSecuritySettings::TPin pin;
    
    if(operation == Pin1)
         {
          pin = CManualSecuritySettings::EPin1;
         }
     else
         {
         pin= CManualSecuritySettings::EPin2;
         }
    
    TBuf<200> pukBuf(puk.utf16());
    TBuf<200> newPinBuf(newPin.utf16());                
    TBuf<200> verifyNewBuf(verifyNew.utf16()); 
          
    
    QT_TRAP_THROWING(m_securityModel->UnblockPinL(pin, pukBuf, newPinBuf, verifyNewBuf));
    }
