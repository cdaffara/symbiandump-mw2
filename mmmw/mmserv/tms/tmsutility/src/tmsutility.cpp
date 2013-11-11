/*
 * Copyright (c) 2009-2010 Nokia Corporation and/or its subsidiary(-ies).
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
 * Description: Telephony Multimedia Service
 *
 */

#include <e32cmn.h>
#include <mmffourcc.h>
#include <mmcccodecinformation.h>
#include <etel.h>
#include "tmsutility.h"

using namespace TMS;

// -----------------------------------------------------------------------------
// TMSUtility::TMSResult
//
// -----------------------------------------------------------------------------
//
EXPORT_C gint TMSUtility::TMSResult(const gint aError)
    {
    gint result(TMS_RESULT_SUCCESS);

    if (aError >= TMS_RESULT_SUCCESS)
        {
        return aError; //return TMS error
        }

    switch (aError)
        {
        case KErrNotFound: //-1
            result = TMS_RESULT_DOES_NOT_EXIST;
            break;
        case KErrGeneral: //-2
            result = TMS_RESULT_GENERAL_ERROR;
            break;
        case KErrCancel: //-3
            result = TMS_RESULT_OPERATION_CANCELLED;
            break;
        case KErrNoMemory: //-4
            result = TMS_RESULT_INSUFFICIENT_MEMORY;
            break;
        case KErrNotSupported: //-5
            result = TMS_RESULT_ILLEGAL_OPERATION;
            break;
        case KErrArgument: //-6
            result = TMS_RESULT_INVALID_ARGUMENT;
            break;
        case KErrTotalLossOfPrecision: //-7
            result = TMS_RESULT_GENERAL_ERROR;
            break;
        case KErrBadHandle: //-8
            result = TMS_RESULT_INVALID_ARGUMENT;
            break;
        case KErrOverflow: //-9
        case KErrUnderflow: //-10
            result = TMS_RESULT_BUFFER_LATENCY_ERROR;
            break;
        case KErrAlreadyExists: //-11
            result = TMS_RESULT_ALREADY_EXIST;
            break;
        case KErrPathNotFound: //-12
            result = TMS_RESULT_GENERAL_ERROR;
            break;
        case KErrDied: //-13
        case KErrServerTerminated: //-15
            result = TMS_RESULT_FATAL_ERROR;
            break;
        case KErrInUse: //-14
        case KErrServerBusy: //-16
        case KErrCompletion: //-17
        case KErrNotReady: //-18
        case KErrUnknown: //-19
        case KErrCorrupt: //-20
        case KErrAccessDenied: //-21
        case KErrLocked: //-22
        case KErrWrite: //-23
        case KErrDisMounted: //-24
        case KErrEof: //-25
        case KErrDiskFull: //-26
        case KErrBadDriver: //-27
        case KErrBadName: //-28
        case KErrCommsLineFail: //-29
        case KErrCommsFrame: //-30
        case KErrCommsOverrun: //-31
        case KErrCommsParity: //-32
        case KErrTimedOut: //-33
        case KErrCouldNotConnect: //-34
        case KErrCouldNotDisconnect: //-35
        case KErrDisconnected: //-36
        case KErrBadLibraryEntryPoint: //-37
        case KErrBadDescriptor: //-38
        case KErrAbort: //-39
        case KErrTooBig: //-40
        case KErrDivideByZero: //-41
        case KErrBadPower: //-42
        case KErrDirFull: //-43
        case KErrHardwareNotAvailable: //-44
        case KErrSessionClosed: //-45
        case KErrPermissionDenied: //-46
        case KErrExtensionNotSupported: //-47
        case KErrCommsBreak: //-48
        case KErrNoSecureTime: //-49
        default:
            result = TMS_RESULT_GENERAL_ERROR;
            break;
        }
    return result;
    }

// -----------------------------------------------------------------------------
// TMSUtility::SymbianResult
//
// -----------------------------------------------------------------------------
//
EXPORT_C TInt TMSUtility::SymbianResult(const gint aError)
    {
    TInt result(KErrNone);

    if (aError <= TMS_RESULT_SUCCESS)
        {
        return aError; //return Symbian error
        }

    switch (aError)
        {
        case TMS_RESULT_DOES_NOT_EXIST:
            result = KErrNotFound;
            break;
        case TMS_RESULT_OPERATION_CANCELLED:
            result = KErrCancel;
            break;
        case TMS_RESULT_INSUFFICIENT_MEMORY:
            result = KErrNoMemory;
            break;
        case TMS_RESULT_ILLEGAL_OPERATION:
            result = KErrNotSupported;
            break;
        case TMS_RESULT_INVALID_ARGUMENT:
            result = KErrArgument;
            break;
        case TMS_RESULT_BUFFER_LATENCY_ERROR:
            result = KErrUnderflow;
            break;
        case TMS_RESULT_ALREADY_EXIST:
            result = KErrAlreadyExists;
            break;
        case TMS_RESULT_FATAL_ERROR:
        case TMS_RESULT_GENERAL_ERROR:
        default:
            result = KErrGeneral;
            break;
        }
    return result;
    }

// -----------------------------------------------------------------------------
// TMSUtility::EtelToTMSResult
//
// -----------------------------------------------------------------------------
//
EXPORT_C gint TMSUtility::EtelToTMSResult(const gint aError)
    {
    gint result(TMS_RESULT_SUCCESS);

    if (aError >= TMS_RESULT_SUCCESS)
        {
        return aError; //return TMS error
        }

    switch(aError)
        {
        case KErrArgument: //-6
            result = TMS_RESULT_INVALID_ARGUMENT;
            break;
        case KErrEtelCallNotActive: //-2007
            result = TMS_REASON_CALL_NOT_ACTIVE;//add TMS_RESULT_CALL_NOT_ACTIVE
            break;
        //case KErrEtelCoreBase: //-2000
        case KErrEtelNotCallOwner: //-2000
        case KErrEtelDuplicatePhoneName: //-2001
        case KErrEtelAlreadyCallOwner://-2002
        case KErrEtelNoCarrier://-2003
        case KErrEtelBusyDetected://-2004
        case KErrEtelNoClientInterestedInThisCall://-2005
        case KErrEtelInitialisationFailure://-2006
        //case KErrEtelCallNotActive: //-2007
        case KErrEtelNoAnswer://-2008
        case KErrEtelNoDialTone://-2009
        case KErrEtelConfigPortFailure://-2010
        case KErrEtelFaxChunkNotCreated://-2011
        case KErrEtelNotFaxOwner://-2012
        case KErrEtelPortNotLoanedToClient://-2013
        case KErrEtelWrongModemType://-2014
        case KErrEtelUnknownModemCapability://-2015
        case KErrEtelAnswerAlreadyOutstanding://-2016
        case KErrEtelModemNotDetected://-2017
        case KErrEtelModemSettingsCorrupt://-2018
        case KErrEtelPortAlreadyLoaned://-2019
        case KErrEtelCallAlreadyActive: //-2020
        default:
            result = TMS_RESULT_GENERAL_ERROR; //add TMS_DTMF_GENERAL_ERROR/FAILED
            break;
        }

    return result;
    }

// -----------------------------------------------------------------------------
// TMSUtility::ToTMSOutput
//
// -----------------------------------------------------------------------------
//
EXPORT_C TMSAudioOutput TMSUtility::ToTMSOutput(
        CTelephonyAudioRouting::TAudioOutput taroutput)
    {
    TMSAudioOutput output = TMS_AUDIO_OUTPUT_NONE;
    switch (taroutput)
        {
        case CTelephonyAudioRouting::ENotActive:
        case CTelephonyAudioRouting::ENone:
            output = TMS_AUDIO_OUTPUT_NONE;
            break;
        case CTelephonyAudioRouting::EHandset:
            output = TMS_AUDIO_OUTPUT_HANDSET;
            break;
        case CTelephonyAudioRouting::ELoudspeaker:
            output = TMS_AUDIO_OUTPUT_LOUDSPEAKER;
            break;
        case CTelephonyAudioRouting::EWiredAudioAccessory:
            output = TMS_AUDIO_OUTPUT_WIRED_ACCESSORY;
            break;
        case CTelephonyAudioRouting::EBTAudioAccessory:
            output = TMS_AUDIO_OUTPUT_ACCESSORY;
            break;
        case CTelephonyAudioRouting::ETTY:
            output = TMS_AUDIO_OUTPUT_ETTY;
            break;
        default:
            break;
        }
    return output;
    }

// -----------------------------------------------------------------------------
// TMSUtility::ToTAROutput
//
// -----------------------------------------------------------------------------
//
EXPORT_C CTelephonyAudioRouting::TAudioOutput TMSUtility::ToTAROutput(
        TMSAudioOutput output)
    {
    CTelephonyAudioRouting::TAudioOutput taroutput =
            CTelephonyAudioRouting::ENone;
    switch (output)
        {
        case TMS_AUDIO_OUTPUT_NONE:
            taroutput = CTelephonyAudioRouting::ENone;
            break;
        case TMS_AUDIO_OUTPUT_HANDSET:
            taroutput = CTelephonyAudioRouting::EHandset;
            break;
        case TMS_AUDIO_OUTPUT_LOUDSPEAKER:
            taroutput = CTelephonyAudioRouting::ELoudspeaker;
            break;
        case TMS_AUDIO_OUTPUT_WIRED_ACCESSORY:
            taroutput = CTelephonyAudioRouting::EWiredAudioAccessory;
            break;
        case TMS_AUDIO_OUTPUT_ACCESSORY:
            taroutput = CTelephonyAudioRouting::EBTAudioAccessory;
            break;
        case TMS_AUDIO_OUTPUT_ETTY:
            taroutput = CTelephonyAudioRouting::ETTY;
            break;
        default:
            break;
        }
    return taroutput;
    }

// -----------------------------------------------------------------------------
// TMSUtility::ToFourCC
//
// -----------------------------------------------------------------------------
//
EXPORT_C TUint32 TMSUtility::ToFourCC(TMSFormatType frmtType)
    {
    TUint32 fourCC(NULL);
    switch (frmtType)
        {
        case TMS_FORMAT_PCM:
            fourCC = KMMFFourCCCodePCM16;
            break;
        case TMS_FORMAT_AMR:
            fourCC = KMccFourCCIdAMRNB;
            break;
        case TMS_FORMAT_G711:
            fourCC = KMccFourCCIdG711;
            break;
        case TMS_FORMAT_G729:
            fourCC = KMccFourCCIdG729;
            break;
        case TMS_FORMAT_ILBC:
            fourCC = KMccFourCCIdILBC;
            break;
        default:
            break;
        }
    return fourCC;
    }

// -----------------------------------------------------------------------------
// TMSUtility::FourCCToTMSFormat
//
// -----------------------------------------------------------------------------
//
EXPORT_C TMSFormatType TMSUtility::FourCCToTMSFormat(TUint32 fourcc)
    {
    TMSFormatType format = NULL;
    switch (fourcc)
        {
        case KMMFFourCCCodePCM16:   // = 0x36315020
            format = TMS_FORMAT_PCM;
            break;
        case KMccFourCCIdAMRNB:     //= 0x524d4120;
            format = TMS_FORMAT_AMR;
            break;
        case KMccFourCCIdG711:      // = 0x31313747;
            format = TMS_FORMAT_G711;
            break;
        case KMccFourCCIdG729:      // = 0x39323747;
            format = TMS_FORMAT_G729;
            break;
        case KMccFourCCIdILBC:      // = 0x43424c49;
            format = TMS_FORMAT_ILBC;
            break;
        default:
            break;
        }
    return format;
    }
