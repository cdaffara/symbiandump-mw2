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

#ifndef QTMS_SPEAKER_SINK_H
#define QTMS_SPEAKER_SINK_H

#include <qtms.h>
#include <qtmssink.h>
#include <QObject>
#include "qtmswrapperexport.h"

// FORWARD DECLARATIONS
namespace TMS
{
class TMSSink;
}

namespace QTMS
{

/**
 * QTMSSpeakerSink class
 *
 * This class instantiates QTMS speaker sink object.
 *
 * @lib QTMSapi.lib
 *
 */
class QTMS_WRAPPER_DLL_EXPORT QTMSSpeakerSink : public QObject,
                                                public QTMSSink
{
    Q_OBJECT
public:
    /**
     * Destructor
     */
    virtual ~QTMSSpeakerSink();

    /**
     * Return sink type.
     *
     * This function can be called at any time.
     *
     * @param  sinktype
     *      The type of sink object (QTMS_SINK_SPEAKER).
     *
     * @return
     *      QTMS_RESULT_SUCCESS if the operation was successful.
     *
     */
    virtual gint GetType(QTMSSinkType& sinktype);

protected:
    /**
     * Constructor
     */
    QTMSSpeakerSink();

protected:
    TMS::TMSSink *iSink;
};

} //namespace QTMS

#endif //QTMS_SPEAKER_SINK_H
// End of file

