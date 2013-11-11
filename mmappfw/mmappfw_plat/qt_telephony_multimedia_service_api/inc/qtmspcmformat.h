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

#ifndef QTMS_PCM_FORMAT_H
#define QTMS_PCM_FORMAT_H

#include <qtms.h>
#include <qtmsformat.h>
#include <QObject>
#include "qtmswrapperexport.h"

// FORWARD DECLARATIONS
namespace TMS
{
class TMSFormat;
}

namespace QTMS
{

/**
 * QTMSPCMFormat class
 *
 * This class provides access methods to configure and query PCM-16 codec
 * format settings.
 *
 * @lib QTMSapi.lib
 *
 */
class QTMS_WRAPPER_DLL_EXPORT QTMSPCMFormat: public QObject, public QTMSFormat
{
Q_OBJECT
public:
    /**
     * Destructor
     */
    virtual ~QTMSPCMFormat();

protected:
    /**
     * Constructor
     */
    QTMSPCMFormat();
};

} //namespace QTMS

#endif //QTMS_PCM_FORMAT_H
