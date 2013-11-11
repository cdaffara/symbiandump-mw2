/*
* Copyright (c) 2002 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  CAudioStreamTestApp class member functions
*
*/



// INCLUDE FILES
#include "AudioStreamTestApp.h"
#include "OutputStreamer.h"
#include "InputStreamer.h"
#include "MediaEvent.h"


// EXTERNAL DATA STRUCTURES
//extern  ?external_data;

// EXTERNAL FUNCTION PROTOTYPES  
//extern ?external_function( ?arg_type,?arg_type );

// CONSTANTS
//const ?type ?constant_var = ?constant;

// MACROS
//#define ?macro ?macro_def

// LOCAL CONSTANTS AND MACROS
//const ?type ?constant_var = ?constant;
//#define ?macro_name ?macro_def

// MODULE DATA STRUCTURES
//enum ?declaration
//typedef ?declaration

// LOCAL FUNCTION PROTOTYPES
//?type ?function_name( ?arg_type, ?arg_type );

// FORWARD DECLARATIONS
//class ?FORWARD_CLASSNAME;

// ============================= LOCAL FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// ?function_name ?description.
// ?description
// Returns: ?value_1: ?description
//          ?value_n: ?description_line1
//                    ?description_line2
// -----------------------------------------------------------------------------
//
/*
?type ?function_name(
    ?arg_type arg,  // ?description
    ?arg_type arg)  // ?description
    {

    ?code  // ?comment

    // ?comment
    ?code
    }
*/

// ============================ MEMBER FUNCTIONS ===============================
TInt CAudioStreamTestApp::TestCaseInputNewL(CStifSectionParser */*aSection*/, TTestResult & aResult)
{
    CInputStreamer* inStream;
	TRAPD(err, inStream = CInputStreamer::NewL(iLogger, 1));
	iLogger->Log(_L("Returned from NewL: %d"), err );
	aResult.iResult = err ;

	if ( err )
	{
		iLogger->Log(_L("Test failed, error code %d"), err);
		aResult.iResultDes.Copy(GetErrorDes(err));
	}
	else 
	{
		iLogger->Log(_L("Test was successful"));
		aResult.iResultDes.Copy(KTestCaseResultSuccess());
	}

	delete inStream;
	inStream = NULL;
    
    return KErrExecuted;
}

TInt CAudioStreamTestApp::TestCaseInputNewWithParamL(CStifSectionParser *aSection, TTestResult & aResult)
{
    TBool usingDefault;
    CInputStreamer* inStream;

    // read in priority and preference
	TInt priority = GetIntL(aSection, KTagPriority, usingDefault, KDefaultPriority);
	TInt pref = GetIntL(aSection, KTagPriorityPreference, usingDefault, KDefaultPriorityPreference);

	TRAPD(err, inStream = CInputStreamer::NewL(priority, IntToPriorityPreference(pref), iLogger, 1));
	iLogger->Log(_L("Returned from NewL: %d"), err );
	aResult.iResult = err ;

	if ( err )
	{
		iLogger->Log(_L("Test failed, error code %d"), err);
		aResult.iResultDes.Copy(GetErrorDes(err));
	}
	else 
	{
		iLogger->Log(_L("Test was successful"));
		aResult.iResultDes.Copy(KTestCaseResultSuccess());
	}

	delete inStream;
	inStream = NULL;
    
    return KErrExecuted;
}

TInt CAudioStreamTestApp::TestCaseInputSetPropL(CStifSectionParser *aSection, TTestResult & aResult)
{
    TInt returnErr = KErrExecuted;
    TBool usingDefault1, usingDefault2;
    CInputStreamer* inStream;

	TRAPD(err, inStream = CInputStreamer::NewL(iLogger, 1));
	iLogger->Log(_L("Returned from NewL: %d"), err );
	aResult.iResult = err ;

	if ( !err )
	{
        // read in sample rate and channel
	    TInt rate = GetIntL(aSection, KTagSampleRate, usingDefault1, KDefaultSampleRate);
		rate = inStream->TranslateRate(rate, TMdaAudioDataSettings::ESampleRate8000Hz);
	    TInt chan = GetIntL(aSection, KTagChannelCount, usingDefault2, KDefaultChannels);
	    chan = (chan==1 ? TMdaAudioDataSettings::EChannelsMono : TMdaAudioDataSettings::EChannelsStereo);
	    
	    // if using default value, error reading data
	    if (usingDefault1 || usingDefault2)
	    {
			aResult.iResultDes.Copy(KConfigInvalid());
   			aResult.iResult = KErrNotExecuted;
			returnErr = KErrConfigInvalid;
	    }
	    else
	    {
		    inStream->SetExitAfterOpen(ETrue);
    		inStream->StartRecording();
	    	CActiveScheduler::Start();

        	TRAP(err, inStream->SetAudioPropertiesL(rate, chan));
        	aResult.iResult = err ;

        	if (err == KErrNone)
        	{
        		iLogger->Log(_L("Test was successful"));
		        aResult.iResultDes.Copy(KTestCaseResultSuccess());
        	}
        	else if (err == KErrNotSupported)
        	{
        		aResult.iResult = KErrNone;	
        		iLogger->Log(_L("KErrNotSupported is accepted, Test was successful"));
		        aResult.iResultDes.Copy(KTestCaseResultSuccess());
        	}
        	else
        	{
        		iLogger->Log(_L("Test failed, error code %d"), err);
		        aResult.iResultDes.Copy(GetErrorDes(err));
        	}
	    }
	}
	else 
	{
		iLogger->Log(_L("Test failed, error code %d"), err);
		aResult.iResultDes.Copy(GetErrorDes(err));
	}

	delete inStream;
	inStream = NULL;
    
    return returnErr;
}

TInt CAudioStreamTestApp::TestCaseInputSetGainL(CStifSectionParser *aSection, TTestResult & aResult)
{
    TInt returnErr = KErrExecuted;
    TBool usingDefault;
    CInputStreamer* inStream;
    
	TRAPD(err, inStream = CInputStreamer::NewL(iLogger, 1));
	iLogger->Log(_L("Returned from NewL: %d"), err );
	aResult.iResult = err ;

	if ( err )
	{
		iLogger->Log(_L("Test failed, error code %d"), err);
		aResult.iResultDes.Copy(GetErrorDes(err));
	}
	else 
	{
	    // read gain
	    TInt gain = GetIntL(aSection, KTagGain, usingDefault, KDefaultRelGain);

	    // if using default value, error reading data
	    if (usingDefault)
	    {
			aResult.iResultDes.Copy(KConfigInvalid());
   			aResult.iResult = KErrNotExecuted;
			returnErr = KErrConfigInvalid;
	    }
	    else
	    {
	        // validate gain
	        TInt maxGain = inStream->GetMaxGain();
			iLogger->Log(_L("Valid Gain value from 0 to %d"), maxGain);
	        //if (gain < 0)
	        //    gain = 0;
	        //else if (gain > maxGain)
	        //    gain = maxGain;
	    
        	inStream->SetGain(gain);
        	TInt rGain = inStream->GetGain(); 

        	if (gain == rGain)
        	{
        		iLogger->Log(_L("Test was successful"));
		        aResult.iResultDes.Copy(KTestCaseResultSuccess());
            	aResult.iResult = KErrNone ;
        	}
        	else
        	{
				iLogger->Log(_L("Gain is %d"), rGain);
				iLogger->Log(_L("Expected Gain is %d"), gain);
				iLogger->Log(_L("Expected Gain is different from real Gain"));
				aResult.iResultDes.Copy(KTestCaseResultFailExpectedValueDifferent());
				aResult.iResult = KErrExpectedValueDifferent;
        	}
	    }
	}

	delete inStream;
	inStream = NULL;
    
    return returnErr;
}

TInt CAudioStreamTestApp::TestCaseInputSetBalanceL(CStifSectionParser *aSection, TTestResult & aResult)
{
    TInt returnErr = KErrExecuted;
    TBool usingDefault;
    CInputStreamer* inStream;
    
	TRAPD(err, inStream = CInputStreamer::NewL(iLogger, 1));
	iLogger->Log(_L("Returned from NewL: %d"), err );
	aResult.iResult = err ;

	if ( err )
	{
		iLogger->Log(_L("Test failed, error code %d"), err);
		aResult.iResultDes.Copy(GetErrorDes(err));
	}
	else 
	{
	    // read balance
	    TInt balance = GetIntL(aSection, KTagBalance, usingDefault, KDefaultBalance);

	    // if using default value, error reading data
	    if (usingDefault)
	    {
			aResult.iResultDes.Copy(KConfigInvalid());
   			aResult.iResult = KErrNotExecuted;
			returnErr = KErrConfigInvalid;
	    }
	    else
	    {
	        // validate balance
			iLogger->Log(_L("Valid Balance value from %d to %d"), KMMFBalanceMaxLeft, KMMFBalanceMaxRight);
	        //if (balance < KMMFBalanceMaxLeft)
	        //    balance = KMMFBalanceMaxLeft;
	        //else if (balance > KMMFBalanceMaxRight)
	        //    balance = KMMFBalanceMaxRight;
	    
        	inStream->SetBalanceL(balance);
        	TInt rBalance = inStream->GetBalanceL(); 

        	if (balance == rBalance)
        	{
        		iLogger->Log(_L("Test was successful"));
		        aResult.iResultDes.Copy(KTestCaseResultSuccess());
            	aResult.iResult = KErrNone ;
        	}
        	else
        	{
				iLogger->Log(_L("Balance is %d"), rBalance);
				iLogger->Log(_L("Expected Balance is %d"), balance);
				iLogger->Log(_L("Expected Balance is different from real Balance"));
				aResult.iResultDes.Copy(KTestCaseResultFailExpectedValueDifferent());
				aResult.iResult = KErrExpectedValueDifferent;
        	}
	    }
	}

	delete inStream;
	inStream = NULL;
    
    return returnErr;
}

TInt CAudioStreamTestApp::TestCaseInputSetPriorityL(CStifSectionParser *aSection, TTestResult & aResult)
{
    TInt returnErr = KErrExecuted;
    TBool usingDefault1, usingDefault2;
    CInputStreamer* inStream;
    
	TRAPD(err, inStream = CInputStreamer::NewL(iLogger, 1));
	iLogger->Log(_L("Returned from NewL: %d"), err );
	aResult.iResult = err ;

	if ( err )
	{
		iLogger->Log(_L("Test failed, error code %d"), err);
		aResult.iResultDes.Copy(GetErrorDes(err));
	}
	else 
	{
        // read in priority and preference
    	TInt priority = GetIntL(aSection, KTagPriority, usingDefault1, KDefaultPriority);
	    TInt pref = GetIntL(aSection, KTagPriorityPreference, usingDefault2, KDefaultPriorityPreference);

	    // if using default value, error reading data
	    if (usingDefault1 || usingDefault2)
	    {
			aResult.iResultDes.Copy(KConfigInvalid());
   			aResult.iResult = KErrNotExecuted;
			returnErr = KErrConfigInvalid;
	    }
	    else
	    {
	        // validate
			iLogger->Log(_L("Valid Priority value from %d to %d"), EMdaPriorityMin, EMdaPriorityMax);
	        //if (priority < EMdaPriorityMin)
	        //    priority = EMdaPriorityMin;
	        //else if (priority > EMdaPriorityMax)
	        //    priority = EMdaPriorityMax;
	    
        	inStream->SetPriority(priority, IntToPriorityPreference(pref));

            // no way to determine if successful
       		iLogger->Log(_L("Test was successful"));
	        aResult.iResultDes.Copy(KTestCaseResultSuccess());
           	aResult.iResult = KErrNone ;
	    }
	}

	delete inStream;
	inStream = NULL;
    
    return returnErr;
}

TInt CAudioStreamTestApp::TestCaseInputSetDataTypeL(CStifSectionParser *aSection, TTestResult & aResult)
{
    TInt returnErr = KErrExecuted;
    TBool usingDefault;
    CInputStreamer* inStream;
    
	TRAPD(err, inStream = CInputStreamer::NewL(iLogger, 1));
	iLogger->Log(_L("Returned from NewL: %d"), err );
	aResult.iResult = err ;

	if ( err )
	{
		iLogger->Log(_L("Test failed, error code %d"), err);
		aResult.iResultDes.Copy(GetErrorDes(err));
	}
	else 
	{
	    // read DataType
	    TFourCC dataType(GetIntL(aSection, KTagDataType, usingDefault, KMMFFourCCCodeNULL));

	    // if using default value, error reading data
	    if (usingDefault)
	    {
			aResult.iResultDes.Copy(KConfigInvalid());
   			aResult.iResult = KErrNotExecuted;
			returnErr = KErrConfigInvalid;
	    }
	    else
	    {
		    inStream->SetExitAfterOpen(ETrue);
    		inStream->StartRecording();
	    	CActiveScheduler::Start();

        	TRAP(err, inStream->SetDataTypeL(dataType));
        	TFourCC rDataType = inStream->GetDataType();
           	aResult.iResult = err ;

        	//if ((dataType == rDataType) || (err == KErrNotSupported))
        	if (dataType == rDataType)
        	{
        		iLogger->Log(_L("Test was successful"));
		        aResult.iResultDes.Copy(KTestCaseResultSuccess());
        	}
        	else if (err == KErrNotSupported)
        	{
        		aResult.iResult = KErrNone;
        		iLogger->Log(_L("KErrNotSupported is accepted, Test was successful"));
		        aResult.iResultDes.Copy(KTestCaseResultSuccess());
        	}
        	else
        	{
				iLogger->Log(_L("DataType is %d"), rDataType.FourCC());
				iLogger->Log(_L("Expected DataType is %d"), dataType.FourCC());
				iLogger->Log(_L("Expected DataType is different from real DataType"));
				aResult.iResultDes.Copy(KTestCaseResultFailExpectedValueDifferent());
				aResult.iResult = KErrExpectedValueDifferent;
        	}
	    }
	}

	delete inStream;
	inStream = NULL;
    
    return returnErr;
}

TInt CAudioStreamTestApp::TestCaseInputOpenL(CStifSectionParser */*aSection*/, TTestResult & aResult)
{
    TInt returnErr = KErrExecuted;
    CInputStreamer* inStream;
    
	TRAPD(err, inStream = CInputStreamer::NewL(iLogger, 1));
	iLogger->Log(_L("Returned from NewL: %d"), err );
	aResult.iResult = err ;

	if ( err )
	{
		iLogger->Log(_L("Test failed, error code %d"), err);
		aResult.iResultDes.Copy(GetErrorDes(err));
	}
	else 
	{
    	TMdaAudioDataSettings *settingsIn = new(ELeave)TMdaAudioDataSettings();
	    CleanupStack::PushL(settingsIn);
    	inStream->SetGain( ( inStream->GetMaxGain()*KDefaultRelGain )/100);
	    settingsIn->iCaps |= TMdaAudioDataSettings::ESampleRateFixed;
	    settingsIn->iSampleRate = TMdaAudioDataSettings::ESampleRate8000Hz;
    	settingsIn->iChannels = TMdaAudioDataSettings::EChannelsMono;

	    inStream->SetSettings(settingsIn);
    	CleanupStack::Pop(settingsIn);
    	
	    inStream->SetExitAfterOpen(ETrue);
    	inStream->StartRecording();
	    CActiveScheduler::Start();
    
        err = inStream->GetFinalError();
       	aResult.iResult = err ;
        
       	if (!err)
       	{
       		iLogger->Log(_L("Test was successful"));
	        aResult.iResultDes.Copy(KTestCaseResultSuccess());
       	}
       	else
       	{
       		if (err == KErrNotSupported)
       		{
       			aResult.iResult = KErrNone;
        		iLogger->Log(_L("KErrNotSupported is accepted, Test was successful"));
		        aResult.iResultDes.Copy(KTestCaseResultSuccess());
		      }
		      else
		      {
       			iLogger->Log(_L("Test failed, error code %d"), err);
	        	aResult.iResultDes.Copy(GetErrorDes(err));
	      	}
       	}
	}

	delete inStream;
	inStream = NULL;
    
    return returnErr;
}

TInt CAudioStreamTestApp::TestCaseInputRecordL(CStifSectionParser *aSection, TTestResult & aResult)
{
    TInt returnErr = KErrExecuted;
    TBool usingDefault;
    CInputStreamer* inStream;
    
	TRAPD(err, inStream = CInputStreamer::NewL(iLogger, 1));
	iLogger->Log(_L("Returned from NewL: %d"), err );
	aResult.iResult = err ;

	if ( err )
	{
		iLogger->Log(_L("Test failed, error code %d"), err);
		aResult.iResultDes.Copy(GetErrorDes(err));
	}
	else 
	{
    	TMdaAudioDataSettings *settingsIn = new(ELeave)TMdaAudioDataSettings();
	    CleanupStack::PushL(settingsIn);
    	inStream->SetGain( ( inStream->GetMaxGain()*KDefaultRelGain )/100);
	    settingsIn->iCaps |= TMdaAudioDataSettings::ESampleRateFixed;
	    
		TInt sampleRate = GetIntL(aSection, KTagSampleRate, usingDefault, CStreamerBase::KSampleRate8000Hz);
	    settingsIn->iSampleRate = inStream->TranslateRate(sampleRate, TMdaAudioDataSettings::ESampleRate8000Hz);
	    
		TInt channels   = GetIntL(aSection, KTagChannelCount, usingDefault, CStreamerBase::KChannelsMono);
    	settingsIn->iChannels = (channels==1 ? TMdaAudioDataSettings::EChannelsMono : TMdaAudioDataSettings::EChannelsStereo);
    	
    	TTimeIntervalMicroSeconds32 DelayRecord = KDefaultRecordDelay;

	    inStream->SetSettings(settingsIn);
    	CleanupStack::Pop(settingsIn);

    	//Event to stop recording after DelayRec microseconds (from CActiveScheduler::Start() )
	    iLogger->Log(_L("Recording will stop after (%d) microseconds") , DelayRecord.Int());
    	CParameters *StopRecParam = new(ELeave) CParameters( CInputStreamer::KRecorderActionStop );
	    CleanupStack::PushL(StopRecParam);
    	CMediaEvent *MeStopRec = CMediaEvent::NewL(iLogger, DelayRecord, DelayRecord, inStream, 
    	                                           StopRecParam, (CActive::EPriorityStandard) );
	    CleanupStack::Pop(StopRecParam);
    	CleanupStack::PushL(MeStopRec);
    	
    	inStream->StartRecording();
	    CActiveScheduler::Start();
    
        err = inStream->GetFinalError();
       	aResult.iResult = err ;
        
       	if (!err)
       	{
       		iLogger->Log(_L("Test was successful"));
	        aResult.iResultDes.Copy(KTestCaseResultSuccess());
       	}
       	else
       	{
       		if (err == KErrNotSupported)
       		{
       		  aResult.iResult = KErrNone;
        		iLogger->Log(_L("KErrNotSupported is accepted, Test was successful"));
		        aResult.iResultDes.Copy(KTestCaseResultSuccess());
		      }
		      else
		      {
       			iLogger->Log(_L("Test failed, error code %d"), err);
	        	aResult.iResultDes.Copy(GetErrorDes(err));
	        }
       	}

    	CleanupStack::PopAndDestroy(MeStopRec);
	}

	delete inStream;
	inStream = NULL;
    
    return returnErr;
}

TInt CAudioStreamTestApp::TestCaseInputBitRateL(CStifSectionParser *aSection, TTestResult & aResult)
{
    TInt returnErr = KErrExecuted;
    TBool usingDefault;
    CInputStreamer* inStream;
    
	TRAPD(err, inStream = CInputStreamer::NewL(iLogger, 1));
	iLogger->Log(_L("Returned from NewL: %d"), err );
	aResult.iResult = err ;

	if ( err )
	{
		iLogger->Log(_L("Test failed, error code %d"), err);
		aResult.iResultDes.Copy(GetErrorDes(err));
	}
	else 
	{
    	TMdaAudioDataSettings *settingsIn = new(ELeave)TMdaAudioDataSettings();
	    CleanupStack::PushL(settingsIn);
    	inStream->SetGain( ( inStream->GetMaxGain()*KDefaultRelGain )/100);
	    settingsIn->iCaps |= TMdaAudioDataSettings::ESampleRateFixed;
	    settingsIn->iSampleRate = TMdaAudioDataSettings::ESampleRate8000Hz;
    	settingsIn->iChannels = TMdaAudioDataSettings::EChannelsMono;
    	TTimeIntervalMicroSeconds32 DelayRecord = 1500000;

	    inStream->SetSettings(settingsIn);
    	CleanupStack::Pop(settingsIn);

    	//Event to stop recording after DelayRec microseconds (from CActiveScheduler::Start() )
	    iLogger->Log(_L("Recording will stop after (%d) microseconds") , DelayRecord.Int());
    	CParameters *StopRecParam = new(ELeave) CParameters( CInputStreamer::KRecorderActionStop );
	    CleanupStack::PushL(StopRecParam);
    	CMediaEvent *MeStopRec = CMediaEvent::NewL(iLogger, DelayRecord, DelayRecord, inStream, 
    	                                           StopRecParam, (CActive::EPriorityStandard) );
	    CleanupStack::Pop(StopRecParam);
    	CleanupStack::PushL(MeStopRec);
    	
	    inStream->SetExitAfterOpen(ETrue);
    	inStream->StartRecording();
	    CActiveScheduler::Start();
        
    	CleanupStack::PopAndDestroy(MeStopRec);
    	
        // get supported bit rate
    	RArray<TInt> array;
	    TRAP(err, inStream->GetSupportedBitRatesL(array));
	
    	if ((err == KErrNone) && (array.Count() > 0))
    	{
		    iLogger->Log(_L("GetSupportedBitRatesL() returned %d result"), array.Count());
    	}
	    else if (err != KErrNone) 
	    {
    		iLogger->Log(_L("GetSupportedBitRatesL() failed with error %d"), err);
		}
	    else if (array.Count() == 0)
		{
		    iLogger->Log(_L("GetSupportedBitRatesL() returned no supported bitrates!"), err);
		}
        
        // set bit rate
        //const TInt KTestBitRate = 48000 * 16;
	    TInt rate = GetIntL(aSection, KTagSampleRate, usingDefault, KDefaultSampleRate);
	    TInt chan = GetIntL(aSection, KTagChannelCount, usingDefault, KDefaultChannels);
	    TInt sampleSize = GetIntL(aSection, KTagSampleSize, usingDefault, KDefaultSampleSize);
	    TInt bitRate = rate * chan * sampleSize;
	    
    	TRAP(err, inStream->SetBitRateL(bitRate));
       	aResult.iResult = err ;
	
	    if (err == KErrNone)
		{
            // verify bit rate
        	TInt rBitRate = 0;
	        TRAPD(err, rBitRate = inStream->BitRateL());
           	aResult.iResult = err ;

        	if (err == KErrNone)
		    {
		        if (rBitRate == bitRate)
    		    {
               		iLogger->Log(_L("Test was successful"));
	                aResult.iResultDes.Copy(KTestCaseResultSuccess());
    		    }
	    	    else
		        {
               		iLogger->Log(_L("Test failed, error code %d"), err);
	                aResult.iResultDes.Copy(GetErrorDes(err));
		        }
		    }
        	else if (err == KErrNotSupported)
        	{
        		iLogger->Log(_L("Test was successful"));
		        aResult.iResultDes.Copy(KTestCaseResultNotSupported());
        	}
        	else
		    {
	    	    iLogger->Log(_L("BitRateL() failed with error %d"), err);
                aResult.iResultDes.Copy(GetErrorDes(err));
		    }
		}
       	else if (err == KErrNotSupported)
       	{
       		aResult.iResult = KErrNone; 
       		iLogger->Log(_L("KErrNotSupported is accepted, Test was successful"));
	        aResult.iResultDes.Copy(KTestCaseResultSuccess());
       	}
	    else
		{
		    iLogger->Log(_L("SetBitRateL() failed with error %d"), err);
            aResult.iResultDes.Copy(GetErrorDes(err));
		}

    	//TTimeIntervalMicroSeconds32 delayStop = 500000;
    	//CParameters *stopParam = new(ELeave) CParameters( CInputStreamer::KRecorderActionStop );
	    //CleanupStack::PushL(stopParam);
    	//CMediaEvent *stopRec = CMediaEvent::NewL(iLogger, delayStop, delayStop,
    	//                                inStream, stopParam, (CActive::EPriorityStandard));
	    //CleanupStack::Pop(stopParam);
    	//CleanupStack::PushL(stopRec);
	    //CActiveScheduler::Start();
    	//CleanupStack::PopAndDestroy(stopRec);
	    //iLogger->Log(_L("PopAndDestroy(stopRec)"));
	}

	delete inStream;
	inStream = NULL;
    
    return returnErr;
}



//  End of File
