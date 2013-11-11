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

#ifndef QTMSVER_H
#define QTMSVER_H

// Include files
#include <qtms.h>

namespace QTMS {

/**
 * TMSVer class
 *
 * Contains TMS version information.
 *
 */
class QTMSVer
    {
public:

    /*
     * Constructor
     */
    QTMSVer();

    /*
     * Constructor
     *
     * @param  mjr
     *      Major version number.
     *
     * @param  mnr
     *      Minor version number.
     *
     * @param  bld
     *      Build version number.
     *
     */
    QTMSVer(guint mjr, guint mnr, guint bld) :
        ver_major(mjr),
        ver_minor(mnr),
        ver_build(bld) {}

public:
    guint ver_major;
    guint ver_minor;
    guint ver_build;
    };

} //namespace QTMS

#endif //QTMSVER_H

