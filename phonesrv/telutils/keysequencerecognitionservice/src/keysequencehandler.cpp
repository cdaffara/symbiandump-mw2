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

#include <QCoreApplication>
#include <QDebug>
#include <xqaiwrequest.h>
#include "keysequencehandler.h"
#include "keysequencerecognitionservicelog.h"

/*!
  KeySequenceHandler::KeySequenceHandler.
 */
KeySequenceHandler::KeySequenceHandler(
    QObject* parent)
    : 
    QObject(parent),
    m_keySequenceValidator("")
{
    DPRINT_METHODENTRYEXIT;
}


/*!
  KeySequenceHandler::~KeySequenceHandler.
 */
KeySequenceHandler::~KeySequenceHandler()
{
    DPRINT_METHODENTRYEXIT;
}


/*!
  KeySequenceHandler::keySequenceValidator.
 */
QString KeySequenceHandler::keySequenceValidator() const
{
    DPRINT_METHODENTRYEXIT;
    
    return m_keySequenceValidator;
}


/*!
  KeySequenceHandler::setKeySequenceValidator.
 */
void KeySequenceHandler::setKeySequenceValidator(const QString &validator)
{
    DPRINT_METHODENTRYEXIT;
    
    m_keySequenceValidator = validator;
}
