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
 * Description: QT Bindings for TMS
 *
 */

#ifndef QTMS_EFFECT_H
#define QTMS_EFFECT_H

#include <qtms.h>
#include <QObject>

namespace QTMS
{

/**
 * QTMSEffect class
 *
 * Base class for all effect object classes.
 * This is an abstract class.
 *
 * @lib QTMSapi.lib
 *
 */
class QTMSEffect
{
public:
    /**
     * Return effect type.
     *
     * @param  effecttype
     *      Type of the effect object.
     *
     * @return
     *      The status of the operation.
     *
     */
    virtual gint GetType(QTMSEffectType& effecttype) = 0;
};

} //namespace QTMS

#endif //QTMS_EFFECT_H
// End of file
