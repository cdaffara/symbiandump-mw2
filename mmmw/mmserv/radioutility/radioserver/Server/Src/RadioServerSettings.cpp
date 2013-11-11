/*
* Copyright (c) 2002-2004 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  This class maintains all the current radio settings. It's also
*				 responsible for sending spontaneous events using Publish & Subscribe.
*
*/



// INCLUDE FILES
#include    "RadioServerSettings.h"
#include 	"RadioClientServer.h"


// CONSTANTS
_LIT_SECURITY_POLICY_PASS(KRadioServerReadPolicy);
_LIT_SECURITY_POLICY_C1(KRadioServerWritePolicy, ECapabilityWriteUserData);

const TInt KFMDefaultFreq 			= 87500000;	// 87.5 MHz
const TInt KJapaneseFMDefaultFreq 	= 76000000;	// 76.0 MHz


// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CRadioServerSettings::CRadioServerSettings
// C++ default constructor can NOT contain any code, that
// might leave.
// -----------------------------------------------------------------------------
//
CRadioServerSettings::CRadioServerSettings()
	:	iMonitorOn(EFalse),
		iRadioOn(EFalse),
		iAntennaAttached(ETrue),
		iOfflineMode(EFalse),
		iTransmitterActive(EFalse),
		iRange(ERsRangeFmEuroAmerica),
		iMinFreq(0),
		iMaxFreq(0),
		iFrequency(KFMDefaultFreq),
		iForceMonoEnabled(EFalse),
		iSquelchEnabled(ETrue),
		iVolume(0),
		iMute(EFalse),
		iBalanceLeft(KRadioServerBalanceMax),
		iBalanceRight(KRadioServerBalanceMax),
		iAutoSwitch(EFalse)
    {
    }

// -----------------------------------------------------------------------------
// CRadioServerSettings::ConstructL
// Symbian 2nd phase constructor can leave.
// -----------------------------------------------------------------------------
//
void CRadioServerSettings::ConstructL()
    {
	TInt err = RProperty::Define(KRadioServPsMonitorState, RProperty::EInt,
								 KRadioServerReadPolicy, KRadioServerWritePolicy);
	if ( err != KErrAlreadyExists)
		{
		User::LeaveIfError(err);
		}

	err = RProperty::Define(ERadioServPsPlayerState, RProperty::EByteArray,
								 KRadioServerReadPolicy, KRadioServerWritePolicy);
	if ( err != KErrAlreadyExists)
		{
		User::LeaveIfError(err);
		}

    err = RProperty::Define(ERadioServPsAntennaStatus, RProperty::EInt,
							KRadioServerReadPolicy, KRadioServerWritePolicy);
	if ( err != KErrAlreadyExists)
		{
		User::LeaveIfError(err);
		}

	err = RProperty::Define(ERadioServPsOfflineMode, RProperty::EInt,
							KRadioServerReadPolicy, KRadioServerWritePolicy);
	if ( err != KErrAlreadyExists)
		{
		User::LeaveIfError(err);
		}

	err = RProperty::Define(ERadioServPsTransmitterStatus, RProperty::EInt,
							KRadioServerReadPolicy, KRadioServerWritePolicy);
	if ( err != KErrAlreadyExists)
		{
		User::LeaveIfError(err);
		}

   	err = RProperty::Define(ERadioServPsFrequency, RProperty::EInt,
							KRadioServerReadPolicy, KRadioServerWritePolicy);
	if ( err != KErrAlreadyExists)
		{
		User::LeaveIfError(err);
		}

   	err = RProperty::Define(ERadioServPsFrequencyRange, RProperty::EInt,
							KRadioServerReadPolicy, KRadioServerWritePolicy);
	if ( err != KErrAlreadyExists)
		{
		User::LeaveIfError(err);
		}

	err = RProperty::Define(ERadioServPsForceMonoReception, RProperty::EInt,
							KRadioServerReadPolicy, KRadioServerWritePolicy);
	if ( err != KErrAlreadyExists)
		{
		User::LeaveIfError(err);
		}

	err = RProperty::Define(ERadioServPsSquelch, RProperty::EInt,
							KRadioServerReadPolicy, KRadioServerWritePolicy);
	if ( err != KErrAlreadyExists)
		{
		User::LeaveIfError(err);
		}

	err = RProperty::Define(ERadioServPsVolume, RProperty::EInt,
							KRadioServerReadPolicy, KRadioServerWritePolicy);
	if ( err != KErrAlreadyExists)
		{
		User::LeaveIfError(err);
		}

	err = RProperty::Define(ERadioServPsMuteStatus, RProperty::EInt,
							KRadioServerReadPolicy, KRadioServerWritePolicy);
	if ( err != KErrAlreadyExists)
		{
		User::LeaveIfError(err);
		}

	err = RProperty::Define(ERadioServPsBalance, RProperty::EByteArray,
							KRadioServerReadPolicy, KRadioServerWritePolicy);
	if ( err != KErrAlreadyExists)
		{
		User::LeaveIfError(err);
		}

	err = RProperty::Define(ERadioServPsRdsSignalStatus, RProperty::EInt,
							KRadioServerReadPolicy, KRadioServerWritePolicy);
	if ( err != KErrAlreadyExists)
		{
		User::LeaveIfError(err);
		}

	err = RProperty::Define(ERadioServPsAutoSwitchStatus, RProperty::EInt,
							KRadioServerReadPolicy, KRadioServerWritePolicy);
	if ( err != KErrAlreadyExists)
		{
		User::LeaveIfError(err);
		}

	err = RProperty::Define(ERadioServPsProgrammeIdentification, RProperty::EInt,
							KRadioServerReadPolicy, KRadioServerWritePolicy);
	if ( err != KErrAlreadyExists)
		{
		User::LeaveIfError(err);
		}

	err = RProperty::Define(ERadioServPsProgrammeType, RProperty::EInt,
							 KRadioServerReadPolicy, KRadioServerWritePolicy);
	if ( err != KErrAlreadyExists)
		{
		User::LeaveIfError(err);
		}

	err = RProperty::Define(ERadioServPsProgrammeService, RProperty::EText,
							 KRadioServerReadPolicy, KRadioServerWritePolicy);
	if ( err != KErrAlreadyExists)
		{
		User::LeaveIfError(err);
		}

	err = RProperty::Define(ERadioServPsRadioText, RProperty::EText,
							 KRadioServerReadPolicy, KRadioServerWritePolicy);
	if ( err != KErrAlreadyExists)
		{
		User::LeaveIfError(err);
		}

  err = RProperty::Define(ERadioServPsRadioTextPlusObjects, RProperty::EByteArray,
               KRadioServerReadPolicy, KRadioServerWritePolicy);
  if ( err != KErrAlreadyExists)
    {
    User::LeaveIfError(err);
    }

	err = RProperty::Define(ERadioServPsClockTime, RProperty::EByteArray,
							 KRadioServerReadPolicy, KRadioServerWritePolicy);
	if ( err != KErrAlreadyExists)
		{
		User::LeaveIfError(err);
		}

	err = RProperty::Define(ERadioServPsTrafficAnnouncementStatus, RProperty::EInt,
							 KRadioServerReadPolicy, KRadioServerWritePolicy);
	if ( err != KErrAlreadyExists)
		{
		User::LeaveIfError(err);
		}

    err = RProperty::Define(ERadioServPsAfSearchBegin, RProperty::EInt,
							 KRadioServerReadPolicy, KRadioServerWritePolicy);
	if ( err != KErrAlreadyExists)
		{
		User::LeaveIfError(err);
		}

    err = RProperty::Define(ERadioServPsAfSearchEnd, RProperty::EByteArray,
							 KRadioServerReadPolicy, KRadioServerWritePolicy);
	if ( err != KErrAlreadyExists)
		{
		User::LeaveIfError(err);
		}
	}

// -----------------------------------------------------------------------------
// CRadioServerSettings::NewL
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
CRadioServerSettings* CRadioServerSettings::NewL()
    {
    CRadioServerSettings* self = new( ELeave ) CRadioServerSettings();
    CleanupStack::PushL(self);
    self->ConstructL();
    CleanupStack::Pop(self);
    return self;
    }

// Destructor
CRadioServerSettings::~CRadioServerSettings()
    {
	RProperty::Delete(KRadioServerPropertyCategory, KRadioServPsMonitorState);
	RProperty::Delete(KRadioServerPropertyCategory, ERadioServPsPlayerState);
	RProperty::Delete(KRadioServerPropertyCategory, ERadioServPsAntennaStatus);
	RProperty::Delete(KRadioServerPropertyCategory, ERadioServPsOfflineMode);
	RProperty::Delete(KRadioServerPropertyCategory, ERadioServPsTransmitterStatus);
	RProperty::Delete(KRadioServerPropertyCategory, ERadioServPsFrequency);
	RProperty::Delete(KRadioServerPropertyCategory, ERadioServPsFrequencyRange);
	RProperty::Delete(KRadioServerPropertyCategory, ERadioServPsForceMonoReception);
	RProperty::Delete(KRadioServerPropertyCategory, ERadioServPsSquelch);
	RProperty::Delete(KRadioServerPropertyCategory, ERadioServPsVolume);
	RProperty::Delete(KRadioServerPropertyCategory, ERadioServPsMuteStatus);
	RProperty::Delete(KRadioServerPropertyCategory, ERadioServPsBalance);
	RProperty::Delete(KRadioServerPropertyCategory, ERadioServPsRdsSignalStatus);
	RProperty::Delete(KRadioServerPropertyCategory, ERadioServPsAutoSwitchStatus);
	RProperty::Delete(KRadioServerPropertyCategory, ERadioServPsProgrammeIdentification);
	RProperty::Delete(KRadioServerPropertyCategory, ERadioServPsProgrammeType);
	RProperty::Delete(KRadioServerPropertyCategory, ERadioServPsProgrammeService);
	RProperty::Delete(KRadioServerPropertyCategory, ERadioServPsRadioText);
	RProperty::Delete(KRadioServerPropertyCategory, ERadioServPsRadioTextPlusObjects);
	RProperty::Delete(KRadioServerPropertyCategory, ERadioServPsClockTime);
	RProperty::Delete(KRadioServerPropertyCategory, ERadioServPsTrafficAnnouncementStatus);
	RProperty::Delete(KRadioServerPropertyCategory, ERadioServPsAfSearchBegin);
	RProperty::Delete(KRadioServerPropertyCategory, ERadioServPsAfSearchEnd);
    }

// -----------------------------------------------------------------------------
// CRadioServerSettings::SetRadioMonitorStatus
// -----------------------------------------------------------------------------
//
void CRadioServerSettings::SetRadioMonitorStatus(
	TBool aMonitorOn,
	TBool aPublish )
    {
    if ( iMonitorOn != aMonitorOn )
		{
		iMonitorOn = aMonitorOn;
		if ( aPublish)
			{
			RProperty::Set(KRadioServerPropertyCategory,
						   KRadioServPsMonitorState,
						   iMonitorOn);
			}
		}
    }

// -----------------------------------------------------------------------------
// CRadioServerSettings::SetRadioOn
// -----------------------------------------------------------------------------
//
void CRadioServerSettings::SetRadioOn(
	TBool aPublish )
    {
    if ( !iRadioOn )
		{
		iRadioOn = ETrue;
		if ( aPublish)
			{
			TPckgBuf<TRsSettingsData> data;
			data().iError = KErrNone;
			data().iData1 = ETrue;
			RProperty::Set(KRadioServerPropertyCategory,
						   ERadioServPsPlayerState,
						   data);
			}
		}
    }

// -----------------------------------------------------------------------------
// CRadioServerSettings::SetRadioOff
// -----------------------------------------------------------------------------
//
void CRadioServerSettings::SetRadioOff(
	TInt aError,
	TBool aPublish )
    {
    if ( iRadioOn )
		{
		iRadioOn = EFalse;
		if ( aPublish)
			{
			TPckgBuf<TRsSettingsData> data;
			data().iError = aError;
			data().iData1 = EFalse;
			RProperty::Set(KRadioServerPropertyCategory,
						   ERadioServPsPlayerState,
						   data);
			}
		}
    }

// -----------------------------------------------------------------------------
// CRadioServerSettings::IsRadioOn
// -----------------------------------------------------------------------------
//
TBool CRadioServerSettings::IsRadioOn() const
    {
	return iRadioOn;
    }

// -----------------------------------------------------------------------------
// CRadioServerSettings::SetAntennaStatus
// -----------------------------------------------------------------------------
//
void CRadioServerSettings::SetAntennaStatus(
	TBool aAttached,
	TBool aPublish )
    {
    if ( iAntennaAttached != aAttached )
		{
		iAntennaAttached = aAttached;
		if ( aPublish)
			{
			RProperty::Set(KRadioServerPropertyCategory,
		        	       ERadioServPsAntennaStatus,
		        	       iAntennaAttached);
			}
		}
    }

// -----------------------------------------------------------------------------
// CRadioServerSettings::IsAntennaAttached
// -----------------------------------------------------------------------------
//
TBool CRadioServerSettings::IsAntennaAttached() const
    {
	return iAntennaAttached;
    }

// -----------------------------------------------------------------------------
// CRadioServerSettings::SetOfflineModeStatus
// -----------------------------------------------------------------------------
//
void CRadioServerSettings::SetOfflineModeStatus(
	TBool aOfflineMode,
	TBool aPublish )
    {
    if ( iOfflineMode != aOfflineMode )
		{
		iOfflineMode = aOfflineMode;
		if ( aPublish)
			{
			RProperty::Set(KRadioServerPropertyCategory,
						   ERadioServPsOfflineMode,
						   iOfflineMode);
			}
		}
    }

// -----------------------------------------------------------------------------
// CRadioServerSettings::IsOfflineModeEnabled
// -----------------------------------------------------------------------------
//
TBool CRadioServerSettings::IsOfflineModeEnabled() const
    {
	return iOfflineMode;
    }

// -----------------------------------------------------------------------------
// CRadioServerSettings::SetTransmitterStatus
// -----------------------------------------------------------------------------
//
void CRadioServerSettings::SetTransmitterStatus(
	TBool aActive,
	TBool aPublish )
    {
    if ( iTransmitterActive != aActive )
		{
		iTransmitterActive = aActive;
		if ( aPublish)
			{
			RProperty::Set(KRadioServerPropertyCategory,
						   ERadioServPsTransmitterStatus,
						   iTransmitterActive);
			}
		}
    }

// -----------------------------------------------------------------------------
// CRadioServerSettings::IsTransmitterActive
// -----------------------------------------------------------------------------
//
TBool CRadioServerSettings::IsTransmitterActive() const
    {
	return iTransmitterActive;
    }

// -----------------------------------------------------------------------------
// CRadioServerSettings::SetFrequency
// -----------------------------------------------------------------------------
//
void CRadioServerSettings::SetFrequency(
	TInt aFrequency,
	TBool aPublish )
	{
    if ( iFrequency != aFrequency )
		{
    	iFrequency = aFrequency;
		if ( aPublish)
			{
			RProperty::Set(KRadioServerPropertyCategory,
						   ERadioServPsFrequency,
						   iFrequency);
			}
    	}
    }

// -----------------------------------------------------------------------------
// CRadioServerSettings::Frequency
// -----------------------------------------------------------------------------
//
TInt CRadioServerSettings::Frequency() const
    {
	return iFrequency;
    }

// -----------------------------------------------------------------------------
// CRadioServerSettings::SetFrequencyRange
// -----------------------------------------------------------------------------
//
void CRadioServerSettings::SetFrequencyRange(
	TRsFrequencyRange aRange,
	TInt aMinFreq,
	TInt aMaxFreq,
	TBool aPublish )
    {
	iMinFreq = aMinFreq;
	iMaxFreq = aMaxFreq;
    if( iRange != aRange )
    	{
    	iRange = aRange;
	    if ( aRange == ERsRangeFmEuroAmerica )
	    	{
			iFrequency = KFMDefaultFreq;
			}
		else if ( aRange == ERsRangeFmJapan )
			{
			iFrequency = KJapaneseFMDefaultFreq;
			}
		if ( aPublish)
			{
			RProperty::Set(KRadioServerPropertyCategory,
		              	   ERadioServPsFrequencyRange,
		                   iRange);
			}
    	}
    }

// -----------------------------------------------------------------------------
// CRadioServerSettings::FrequencyRange
// -----------------------------------------------------------------------------
//
TRsFrequencyRange CRadioServerSettings::FrequencyRange() const
    {
	return iRange;
    }

// -----------------------------------------------------------------------------
// CRadioServerSettings::GetFrequencyRange
// -----------------------------------------------------------------------------
//
TInt CRadioServerSettings::GetFrequencyRange(
	TRsFrequencyRange& aRange,
	TInt& aMinFreq,
	TInt& aMaxFreq ) const
	{
	TInt err = KErrNone;
	if ( iMinFreq != 0 )
		{
		aRange = iRange;
		aMinFreq = iMinFreq;
		aMaxFreq = iMaxFreq;
		}
	else
		{
		err = KErrNotFound;
		}
	return err;
	}

// -----------------------------------------------------------------------------
// CRadioServerSettings::SetForcedMonoStatus
// -----------------------------------------------------------------------------
//
void CRadioServerSettings::SetForcedMonoStatus(
	TBool aForceMono,
	TBool aPublish )
	{
    if ( iForceMonoEnabled != aForceMono )
		{
    	iForceMonoEnabled = aForceMono;
		if ( aPublish)
			{
			RProperty::Set(KRadioServerPropertyCategory,
						   ERadioServPsForceMonoReception,
						   iForceMonoEnabled);
			}
    	}
    }

// -----------------------------------------------------------------------------
// CRadioServerSettings::IsForceMonoEnabled
// -----------------------------------------------------------------------------
//
TBool CRadioServerSettings::IsForceMonoEnabled() const
    {
	return iForceMonoEnabled;
    }

// -----------------------------------------------------------------------------
// CRadioServerSettings::SetSquelch
// -----------------------------------------------------------------------------
//
void CRadioServerSettings::SetSquelch(
	TBool aEnabled,
	TBool aPublish )
	{
    if ( iSquelchEnabled != aEnabled )
		{
    	iSquelchEnabled = aEnabled;
		if ( aPublish)
			{
			RProperty::Set(KRadioServerPropertyCategory,
						   ERadioServPsSquelch,
						   iSquelchEnabled);
			}
    	}
    }

// -----------------------------------------------------------------------------
// CRadioServerSettings::IsSquelchEnabled
// -----------------------------------------------------------------------------
//
TBool CRadioServerSettings::IsSquelchEnabled() const
    {
	return iSquelchEnabled;
    }

// -----------------------------------------------------------------------------
// CRadioServerSettings::SetVolume
// -----------------------------------------------------------------------------
//
void CRadioServerSettings::SetVolume(
	TInt aVolume,
	TBool aPublish )
    {
    if ( iVolume != aVolume )
		{
    	iVolume = aVolume;
		if ( aPublish)
			{
			RProperty::Set(KRadioServerPropertyCategory,
						   ERadioServPsVolume,
						   iVolume);
			}
    	}
    }
// -----------------------------------------------------------------------------
// CRadioServerSettings::Volume
// -----------------------------------------------------------------------------
//
TInt CRadioServerSettings::Volume() const
    {
	return iVolume;
    }

// -----------------------------------------------------------------------------
// CRadioServerSettings::SetMuteStatus
// -----------------------------------------------------------------------------
//
void CRadioServerSettings::SetMuteStatus(
	TBool aMute,
	TBool aPublish )
    {
    if ( iMute != aMute )
		{
    	iMute = aMute;
		if ( aPublish)
			{
			RProperty::Set(KRadioServerPropertyCategory,
						   ERadioServPsMuteStatus,
						   iMute);
			}
    	}
    }

// -----------------------------------------------------------------------------
// CRadioServerSettings::IsMute
// -----------------------------------------------------------------------------
//
TBool CRadioServerSettings::IsMute() const
    {
	return iMute;
    }

// -----------------------------------------------------------------------------
// CRadioServerSettings::SetBalance
// -----------------------------------------------------------------------------
//
void CRadioServerSettings::SetBalance(
	TInt aLeft,
	TInt aRight,
	TBool aPublish )
    {
    if ( (iBalanceLeft != aLeft) || (iBalanceRight != aRight) )
		{
		iBalanceLeft = aLeft;
		iBalanceRight = aRight;
		if ( aPublish)
			{
			TPckgBuf<TRsSettingsData> data;
			data().iData1 = iBalanceLeft;
			data().iData2 = iBalanceRight;
			RProperty::Set(KRadioServerPropertyCategory,
						   ERadioServPsBalance,
						   data);
			}
		}
    }

// -----------------------------------------------------------------------------
// CRadioServerSettings::GetBalance
// -----------------------------------------------------------------------------
//
void CRadioServerSettings::GetBalance(
	TInt& aLeft,
	TInt& aRight )
    {
    aLeft = iBalanceLeft;
    aRight = iBalanceRight;
    }

// -----------------------------------------------------------------------------
// CRadioServerSettings::SetRdsSignalChange
// -----------------------------------------------------------------------------
//
void CRadioServerSettings::SetRdsSignalChange(
	TBool aSignal )
	{
 	RProperty::Set(KRadioServerPropertyCategory,
 	               ERadioServPsRdsSignalStatus,
 	               aSignal);
	}

// -----------------------------------------------------------------------------
// CRadioServerSettings::SetAutoSwitchStatus
// -----------------------------------------------------------------------------
//
void CRadioServerSettings::SetAutoSwitchStatus(
	TBool aAuto )
	{
    if ( iAutoSwitch != aAuto )
		{
		iAutoSwitch = aAuto;
		RProperty::Set(KRadioServerPropertyCategory,
	        	       ERadioServPsAutoSwitchStatus,
	        	       iAutoSwitch);
		}
    }

// -----------------------------------------------------------------------------
// CRadioServerSettings::AutoSwitch
// -----------------------------------------------------------------------------
//
TBool CRadioServerSettings::AutoSwitch() const
    {
	return iAutoSwitch;
    }

// -----------------------------------------------------------------------------
// CRadioServerSettings::SetProgrammeIdentification
// -----------------------------------------------------------------------------
//
void CRadioServerSettings::SetProgrammeIdentification(
	TInt aPi )
	{
    RProperty::Set(KRadioServerPropertyCategory,
    			   ERadioServPsProgrammeIdentification,
    			   aPi);
	}

// -----------------------------------------------------------------------------
// CRadioServerSettings::SetProgrammeType
// -----------------------------------------------------------------------------
//
void CRadioServerSettings::SetProgrammeType(
	TRdsProgrammeType aPty )
	{
	RProperty::Set(KRadioServerPropertyCategory,
	               ERadioServPsProgrammeType,
	               aPty);
	}

// -----------------------------------------------------------------------------
// CRadioServerSettings::SetProgrammeService
// -----------------------------------------------------------------------------
//
void CRadioServerSettings::SetProgrammeService(
	TRdsPSName& aPs )
	{
	TRsRdsPSName ps;
	ps.Copy(aPs);
	RProperty::Set(KRadioServerPropertyCategory,
	               ERadioServPsProgrammeService,
	               ps);
	}

// -----------------------------------------------------------------------------
// CRadioServerSettings::SetRadioText
// -----------------------------------------------------------------------------
//
void CRadioServerSettings::SetRadioText(
	TRdsRadioText& aRt )
	{
	TRsRdsRadioText rt;
	CRadioServerSettings::SetRDSCharConv( aRt, rt );
	RProperty::Set(KRadioServerPropertyCategory,
	               ERadioServPsRadioText,
	               rt);
	}
	
// -----------------------------------------------------------------------------
// CRadioServerSettings::SetRadioTextPlusObjects
// -----------------------------------------------------------------------------
//
void CRadioServerSettings::SetRadioTextPlusObjects(
  TRdsRadioText& aRtPlusObject1, TUint32 aContentType1,
  TRdsRadioText& aRtPlusObject2, TUint32 aContentType2)
  {
  TPckgBuf<TRsRdsRTPlusObjectData> objData;
  TRsRdsRadioText rt_obj1;
  TRsRdsRadioText rt_obj2;
  CRadioServerSettings::SetRDSCharConv( aRtPlusObject1, rt_obj1 );
  CRadioServerSettings::SetRDSCharConv( aRtPlusObject2, rt_obj2 );
  rt_obj1.Copy(aRtPlusObject1);
  rt_obj2.Copy(aRtPlusObject2);
  objData().aRtObj1 = rt_obj1;
  objData().iContentType1 = aContentType1;
  objData().aRtObj2 = rt_obj2;
  objData().iContentType2 = aContentType2;  
  RProperty::Set(KRadioServerPropertyCategory,
                 ERadioServPsRadioTextPlusObjects,
                 objData);
  }

// -----------------------------------------------------------------------------
// CRadioServerSettings::SetRDSCharConv
// -----------------------------------------------------------------------------
//
void CRadioServerSettings::SetRDSCharConv( TRdsRadioText& aTRdsStrSource, TRsRdsRadioText& aTRsRdsStrDest )
    {
    TUint8 loop_cntr;
    TUint8 str_len = aTRdsStrSource.Length();
    
    for( loop_cntr = 0 ; loop_cntr < str_len ; loop_cntr++ )
        {
        aTRsRdsStrDest.Append( &TRsRdsCharConvData[ (TUint8)aTRdsStrSource[ loop_cntr ] ], 1 );
        }
    }

// -----------------------------------------------------------------------------
// CRadioServerSettings::SetClockTime
// -----------------------------------------------------------------------------
//
void CRadioServerSettings::SetClockTime(
	TDateTime& aCt )
	{
	TPckgBuf<TDateTime> ct(aCt);
	RProperty::Set(KRadioServerPropertyCategory,
				   ERadioServPsClockTime,
				   ct );
	}

// -----------------------------------------------------------------------------
// CRadioServerSettings::SetTrafficAnnouncementStatus
// -----------------------------------------------------------------------------
//
void CRadioServerSettings::SetTrafficAnnouncementStatus(
	TBool aTaOn )
	{
	RProperty::Set(KRadioServerPropertyCategory,
				   ERadioServPsTrafficAnnouncementStatus,
				   aTaOn);
	}

// -----------------------------------------------------------------------------
// CRadioServerSettings::SetRdsBeginAF
// -----------------------------------------------------------------------------
//
void CRadioServerSettings::SetRdsBeginAF()
	{
	RProperty::Set(KRadioServerPropertyCategory,
				   ERadioServPsAfSearchBegin,
				   ETrue);
	}

// -----------------------------------------------------------------------------
// CRadioServerSettings::SetRdsEndAF
// -----------------------------------------------------------------------------
//
void CRadioServerSettings::SetRdsEndAF(
	TInt aError,
	TInt aFrequency )
	{
    TPckgBuf<TRsSettingsData> afData;
    afData().iError = aError;
    afData().iData1 = aFrequency;

  	RProperty::Set(KRadioServerPropertyCategory,
				   ERadioServPsAfSearchEnd,
				   afData);

    if (aError == KErrNone)
    	{
    	SetFrequency(aFrequency);
    	}
	}

// -----------------------------------------------------------------------------
// CRadioServerSettings::Reset
// -----------------------------------------------------------------------------
//
void CRadioServerSettings::Reset()
    {
	iMonitorOn = EFalse;
	iRadioOn = EFalse;
	iAntennaAttached = ETrue;
	iOfflineMode = EFalse;
	iTransmitterActive = EFalse;
	iRange = ERsRangeFmEuroAmerica;
	iMinFreq = 0,
	iMaxFreq = 0,
	iFrequency = KFMDefaultFreq;
	iForceMonoEnabled = EFalse;
	iSquelchEnabled = ETrue;
	iVolume = 0;
	iMute = EFalse;
	iBalanceLeft = KRadioServerBalanceMax;
	iBalanceRight = KRadioServerBalanceMax;
	iAutoSwitch = EFalse;
	}

//  End of File
