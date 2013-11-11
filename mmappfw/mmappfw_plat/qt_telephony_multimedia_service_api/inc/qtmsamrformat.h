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

#ifndef QTMS_AMR_FORMAT_H
#define QTMS_AMR_FORMAT_H

#include <QObject>
#include <qtmsformat.h>
#include "qtmswrapperexport.h"

// FORWARD DECLARATIONS
namespace TMS
{
class TMSFormat;
}

namespace QTMS
{

/**
 * QTMSAMRFormat class
 *
 * This class provides access methods to configure and query AMR-NB codec
 * format settings.
 *
 * This format class can be used for both the encoder and decoder.
 *
 * @lib QTMSapi.lib
 *
 */
class QTMS_WRAPPER_DLL_EXPORT QTMSAMRFormat: public QObject, public QTMSFormat
{
Q_OBJECT
public:
    /**
     * Destructor
     */
    virtual ~QTMSAMRFormat();

protected:
    /**
     * Constructor
     */
    QTMSAMRFormat();
};

} //namespace QTMS

#endif //QTMS_AMR_FORMAT_H
