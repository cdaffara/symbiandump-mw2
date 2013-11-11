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

#ifndef QTMS_FACTORY_H
#define QTMS_FACTORY_H

#include <qtms.h>
#include <QObject>
#include <QtCore/qglobal.h>
#include "qtmswrapperexport.h"

namespace QTMS
{

// FORWARD DECLARATIONS
class QTMSCall;
class QTMSFormat;
class QTMSEffect;
class QTMSBuffer;
class QTMSSource;
class QTMSSink;
class QTMSFactoryImpl;
class QTMSVer;
class QTMSGlobalRouting;
class QTMSDTMF;
class QTMSRingTone;
class QTMSInbandTone;

/**
 * QTMSFactory class
 *
 * This class serves as a factory crating all necessary QTMS components.
 *
 * Usage:
 * <code>
 * QTMSFactory* iFactory;
 * QTMSCall* iCall;
 * gint err;
 * err = QTMSFactory::CreateFactory(iFactory);
 * err = iFactory->CreateCall(QTMS_CALL_IP, iCall);
 * ...
 * err = iFactory->DeleteCall(iCall);
 * delete iFactory;
 * ...
 * </code>
 *
 * @lib QTMSapi.lib
 *
 */
class QTMS_WRAPPER_DLL_EXPORT QTMSFactory : public QObject
{
    Q_OBJECT
public:
    /**
     * Returns pointer reference to created QTMS Factory object. The specified
     * API version will allow using different QTMS API variants.
     *
     * @param  QTMSfactory
     *      Created Factory object.
     *
     * @param  ver
     *      API version which should be used to create QTMS components.
     *
     * @return
     *      QTMS_RESULT_SUCCESS if factory is created successfully.
     *      QTMS_RESULT_INSUFFICIENT_MEMORY if factory creation failed due to
     *      insufficient of memory.
     *      QTMS_RESULT_VERSION_NOT_SUPPORTED if requested API version is not
     *      supported.
     *
     */
    static gint CreateFactory(QTMSFactory*& tmsfactory, QTMSVer& ver);

    /**
     * Destructor
     *
     * Deletes the QTMS Factory.
     *
     */
    virtual ~QTMSFactory();

    /**
     * Creates QTMS Call of the given type with context ID.
     *
     * @param  ctype
     *      Call type to be created.
     *      Supported call types:
     *          QTMS_CALL_CS
     *          QTMS_CALL_ECS
     *          QTMS_CALL_IP
     *
     * @param  QTMScall
     *      Created Call object.
     *
     * @param  ctxid
     *      The Context ID to be assigned to the call. The default is
     *      DEFAULT_CALL_CONTEXT.
     *
     * @return
     *      QTMS_RESULT_SUCCESS if call is created successfully.
     *      QTMS_RESULT_INSUFFICIENT_MEMORY if call creation failed due to
     *      insufficient memory.
     *      QTMS_RESULT_CALL_TYPE_NOT_SUPPORTED if calltype is not supported.
     *      QTMS_RESULT_INVALID_ARGUMENT if the context id is not valid.
     *      QTMS_RESULT_FATAL_ERROR if there is any other error.
     *
     */
    gint CreateCall(QTMSCallType ctype, QTMSCall*& QTMScall,
        guint ctxid = DEFAULT_CALL_CONTEXT);

    /**
     * Deletes QTMS Call.
     *
     * @param  QTMSct
     *      The call to be deleted.
     *
     * @return
     *      QTMS_RESULT_SUCCESS if call is deleted successfully.
     *      QTMS_RESULT_INVALID_ARGUMENT if the call is not valid.
     *
     */
    gint DeleteCall(QTMSCall*& qtmsct);

    /**
     * Is the given calltype supported by the QTMS API.
     *
     * @param  ctype
     *      The call type to be validated.
     *
     * @param  flag
     *      Indicates wheter the call type is supported or not.
     *
     * @return
     *      QTMS_RESULT_SUCCESS if operation was successful.
     *      QTMS_RESULT_FATAL_ERROR if operation failed.
     *
     */
    gint IsCallTypeSupported(QTMSCallType ctype, gboolean& flag);

    /**
     * Returns supported codec format types.
     *
     * @param  strmtype
     *      Stream type: Uplink or Downlink
     *
     * @param  fmtlist
     *      Format vector in which the supported formats are stored
     *
     * @return
     *      QTMS_RESULT_SUCCESS if operation was successful.
     *      QTMS_RESULT_FATAL_ERROR if operation failed.
     *
     */
    gint GetSupportedFormats(const QTMSStreamType strmtype,
        FormatVector& fmtlist);

    /**
     * Creates Format object of the given format type.
     *
     * @param  fmttype
     *      Format type to be created.
     *
     * @param  QTMSfmt
     *      Crated Format object.
     *
     * @return
     *      QTMS_RESULT_SUCCESS if format is created successfully.
     *      QTMS_RESULT_INSUFFICIENT_MEMORY if format creation failed due to
     *      insufficient of memory.
     *      QTMS_RESULT_FORMAT_TYPE_NOT_SUPPORTED if format type is not
     *      supported.
     *
     */
    gint CreateFormat(QTMSFormatType fmttype, QTMSFormat*& qtmsfmt);

    /**
     * Deletes Format object.
     *
     * @param  QTMSfmt
     *      Format to be deleted.
     *
     * @return
     *      QTMS_RESULT_SUCCESS if format is deleted successfully.
     *      QTMS_RESULT_INVALID_ARGUMENT if the format is not valid.
     *
     */
    gint DeleteFormat(QTMSFormat*& qtmsfmt);

    /**
     * Creates Effect object of the given type.
     *
     * @param  QTMSeffecttype
     *      Effect type to be created.
     *
     * @param  QTMSeffect
     *      Created Effect object.
     *
     * @return
     *      QTMS_RESULT_SUCCESS if effect is created successfully.
     *      QTMS_RESULT_INSUFFICIENT_MEMORY if effect creation failed due to
     *      insufficient memory.
     *      QTMS_RESULT_EFFECT_TYPE_NOT_SUPPORTED if the effect type is not
     *      supported.
     *
     */
    gint CreateEffect(QTMSEffectType QTMSeffecttype,
        QTMSEffect*& qtmseffect);

    /**
     * Deletes Effect object.
     *
     * @param  QTMSeffect
     *      Effect object to be deleted.
     *
     * @return
     *      QTMS_RESULT_SUCCESS if effect is deleted successfully.
     *      QTMS_RESULT_INVALID_ARGUMENT if the effect is not valid.
     */
    gint DeleteEffect(QTMSEffect*& qtmseffect);

    /**
     * Creates Buffer object of the given type and size.
     * The memory is allocated on the heap.
     *
     * @param  buffertype
     *      Buffer type to be created.
     *
     * @param  size
     *      Size of the buffer to be created.
     *
     * @param  QTMSbuffer
     *      Created buffer object.
     *
     * @return
     *      QTMS_RESULT_SUCCESS if buffer is created successfully.
     *      QTMS_RESULT_INSUFFICIENT_MEMORY if buffer creation failed due to
     *      insufficient memory.
     *      QTMS_RESULT_BUFFER_TYPE_NOT_SUPPORTED if the buffer type is not
     *      supported.
     *
     */
    gint CreateBuffer(QTMSBufferType buffertype, guint size,
        QTMSBuffer*& qtmsbuffer);

    /**
     * Deletes the Buffer object.
     *
     * @param  QTMSbuffer
     *      Buffer to be deleted.
     *
     * @return
     *      QTMS_RESULT_SUCCESS if buffer is deleted successfully.
     *      QTMS_RESULT_INVALID_ARGUMENT if the buffer is not valid.
     */
    gint DeleteBuffer(QTMSBuffer*& qtmsbuffer);

    /**
     * Creates the Source object of the given type.
     *
     * @param  srctype
     *      Source type to be created.
     *
     * @param  QTMSsrc
     *      Crated source object.
     *
     * @return
     *      QTMS_RESULT_SUCCESS if source is created successfully.
     *      QTMS_RESULT_INSUFFICIENT_MEMORY if source creation failed due to
     *      lack of memory.
     *      QTMS_RESULT_SOURCE_TYPE_NOT_SUPPORTED if the source type is not
     *      supported.
     *
     */
    gint CreateSource(QTMSSourceType srctype, QTMSSource*& qtmssrc);

    /**
     * Deletes the Source object.
     *
     * @param  QTMSsrc
     *      Source to be deleted.
     *
     * @return
     *      QTMS_RESULT_SUCCESS if source is deleted successfully.
     *      QTMS_RESULT_INVALID_ARGUMENT if the source is not valid.
     *
     */
    gint DeleteSource(QTMSSource*& qtmssrc);

    /**
     * Creates the Sink object of the given QTMSSinkType.
     *
     * @param  sinktype
     *      Sink type to be created.
     *
     * @param  QTMSsink
     *      Created sink object.
     *
     * @return
     *      QTMS_RESULT_SUCCESS if sink is created successfully.
     *      QTMS_RESULT_INSUFFICIENT_MEMORY if sink creation failed due to
     *      lack of memory.
     *      QTMS_RESULT_SOURCE_TYPE_NOT_SUPPORTED if the sink type is not
     *      supported.
     *
     */
    gint CreateSink(QTMSSinkType sinktype, QTMSSink*& qtmssink);

    /**
     * Deletes the Sink object.
     *
     * @param  QTMSsink
     *      The sink to be deleted.
     *
     * @return
     *      QTMS_RESULT_SUCCESS if sink is deleted successfully.
     *      QTMS_RESULT_INVALID_ARGUMENT if the sink is not valid.
     */
    gint DeleteSink(QTMSSink*& qtmssink);

    /**
     * Creates QTMSGlobalRouting object.
     *
     * @param  globrouting
     *      Created global routing object.
     *
     * @return
     *      QTMS_RESULT_SUCCESS if global routing object is created successfully.
     *      QTMS_RESULT_INSUFFICIENT_MEMORY if global routing creation failed
     *      due to lack of memory.
     *      QTMS_RESULT_SOURCE_TYPE_NOT_SUPPORTED if global routing is not
     *      supported.
     *
     */
    gint CreateGlobalRouting(QTMSGlobalRouting*& globrouting);

    /**
     * Deletes QTMSGlobalRouting object.
     *
     * @param  globrouting
     *      Global routing object to be deleted.
     *
     * @return
     *      QTMS_RESULT_SUCCESS if global routing object is deleted successfully.
     *      QTMS_RESULT_INVALID_ARGUMENT if the global routing object is invalid.
     */
    gint DeleteGlobalRouting(QTMSGlobalRouting*& globrouting);

    /**
     * Create DTMF player object.
     *
     * @param  streamtype
     *     Stream ID to be used to create the DTMF player.
     *     QTMS_STREAM_UPLINK
     *     QTMS_STREAM_DOWNLINK
     *
     * @param  dtmf
     *      Created DTMF player.
     *
     * @return
     *      QTMS_RESULT_SUCCESS if the operation was successful.
     *      QTMS_RESULT_INSUFFICIENT_MEMORY if DTMF player creation failed due
     *      to insufficient memory.
     *      QTMS_RESULT_FATAL_ERROR if an error occured.
     *      QTMS_REASON_EMERGENCY_CALL_ONGOING if emergency call is active.
     *      QTMS_REASON_PERMISSION_DENIED if permission is denied.
     *
     */
    gint CreateDTMF(QTMSStreamType streamtype, QTMSDTMF*& dtmf);

    /**
     * Delete DTMF player object.
     *
     * @param  dtmf
     *      DTMF player to be deleted.
     *
     * @return
     *      QTMS_RESULT_SUCCESS if the operation was successful.
     *      QTMS_RESULT_INVALID_ARGUMENT if the DTMF player is not valid.
     *
     */
    gint DeleteDTMF(QTMSDTMF*& dtmf);

    /**
     * Creates QTMSRingTone player object  for audio ring tone.
     *
     * @param  rt
     *      RingTone player object to be created.
     *
     * @return
     *      QTMS_RESULT_SUCCESS when object is created successfully.
     *      QTMS_RESULT_INSUFFICIENT_MEMORY when object creation failed due to
     *      insufficient memory.
     */
    gint CreateRingTonePlayer(QTMSRingTone*& rt);

    /**
     * Deletes QTMSRingTone object.
     *
     * @param  rt
     *      RingTone player object to be deleted.
     *
     * @return
     *      QTMS_RESULT_SUCCESS when object is deleted successfully.
     *      QTMS_RESULT_INVALID_ARGUMENT if RingTone player object is invalid.
     */
    gint DeleteRingTonePlayer(QTMSRingTone*& rt);

    /**
     * Creates QTMSInbandTone player object.
     *
     * @param  inbandtone
     *      InbandTone player object to be created.
     *
     * @return
     *      QTMS_RESULT_SUCCESS when object is created successfully.
     *      QTMS_RESULT_INSUFFICIENT_MEMORY when object creation failed due to
     *      insufficient memory.
     */
    gint CreateInbandTonePlayer(QTMSInbandTone*& inbandtone);

    /**
     * Deletes QTMSInbandTone object.
     *
     * @param  inbandtone
     *      InbandTone player object to be deleted.
     *
     * @return
     *      QTMS_RESULT_SUCCESS when object is deleted successfully.
     *      QTMS_RESULT_INVALID_ARGUMENT if InbandTone player object is invalid.
     */
    gint DeleteInbandTonePlayer(QTMSInbandTone*& inbandtone);

private:
    /**
     * Constructor
     */
    QTMSFactory(QTMSVer& ver);

private:
    QTMSFactoryImpl* impl;
};

} //namespace QTMS

#endif //QTMS_FACTORY_H
