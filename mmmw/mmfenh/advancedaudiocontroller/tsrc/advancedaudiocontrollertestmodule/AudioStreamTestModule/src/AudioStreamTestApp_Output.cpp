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

TInt CAudioStreamTestApp::TestCaseOutputNewL(CStifSectionParser */*aSection*/, TTestResult & aResult)
{
    COutputStreamer* outStream;
	TRAPD(err, outStream = COutputStreamer::NewL(iLogger, 1));
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

	delete outStream;
	outStream = NULL;
    
    return KErrExecuted;
}

TInt CAudioStreamTestApp::TestCaseOutputNewWithParamL(CStifSectionParser *aSection, TTestResult & aResult)
{
    TBool usingDefault;
    COutputStreamer* outStream;

    // read in priority and preference
	TInt priority = GetIntL(aSection, KTagPriority, usingDefault, KDefaultPriority);
	TInt pref = GetIntL(aSection, KTagPriorityPreference, usingDefault, KDefaultPriorityPreference);

	TRAPD(err, outStream = COutputStreamer::NewL(priority, IntToPriorityPreference(pref), iLogger, 1));
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

	delete outStream;
	outStream = NULL;
    
    return KErrExecuted;
}

TInt CAudioStreamTestApp::TestCaseOutputSetPropL(CStifSectionParser *aSection, TTestResult & aResult)
{
    TInt returnErr = KErrExecuted;
    TBool usingDefault1, usingDefault2;
    COutputStreamer* outStream;

	TRAPD(err, outStream = COutputStreamer::NewL(iLogger, 1));
	iLogger->Log(_L("Returned from NewL: %d"), err );
	aResult.iResult = err ;

	if ( !err )
	{
        // read in sample rate and channel
	    TInt rate = GetIntL(aSection, KTagSampleRate, usingDefault1, KDefaultSampleRate);
		rate = outStream->TranslateRate(rate, TMdaAudioDataSettings::ESampleRate8000Hz);
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
			outStream->StartPlayback(NULL);
		    outStream->SetExitAfterOpen(ETrue);
			CActiveScheduler::Start();
	    
        	TRAP(err, outStream->SetAudioPropertiesL(rate, chan));
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

	delete outStream;
	outStream = NULL;
    
    return returnErr;
}

TInt CAudioStreamTestApp::TestCaseOutputOpenL(CStifSectionParser */*aSection*/, TTestResult & aResult)
{
    TInt returnErr = KErrExecuted;
    COutputStreamer* outStream;
    
	TRAPD(err, outStream = COutputStreamer::NewL(iLogger, 1));
	iLogger->Log(_L("Returned from NewL: %d"), err );
	aResult.iResult = err ;

	if ( err )
	{
		iLogger->Log(_L("Test failed, error code %d"), err);
		aResult.iResultDes.Copy(GetErrorDes(err));
	}
	else 
	{
		TMdaAudioDataSettings settings;
		settings.Query();
		settings.iSampleRate = TMdaAudioDataSettings::ESampleRate8000Hz;
		settings.iChannels = TMdaAudioDataSettings::EChannelsStereo;
		settings.iFlags = TMdaAudioDataSettings::ENoNetworkRouting;
		settings.iVolume = 0;

		outStream->StartPlayback(&settings);
	    outStream->SetExitAfterOpen(ETrue);
		CActiveScheduler::Start();

        err = outStream->GetFinalError();
       	aResult.iResult = err ;
		if (err)
		{
       		iLogger->Log(_L("Test failed, error code %d"), err);
	        aResult.iResultDes.Copy(GetErrorDes(err));
		}
		else 
		{
       		iLogger->Log(_L("Test was successful"));
	        aResult.iResultDes.Copy(KTestCaseResultSuccess());
		}
	}

	delete outStream;
	outStream = NULL;
    
    return returnErr;
}

TInt CAudioStreamTestApp::TestCaseOutputSetVolumeL(CStifSectionParser *aSection, TTestResult & aResult)
{
	TBool usingDefault = EFalse;
    TInt returnErr = KErrExecuted;
    COutputStreamer* outStream;
    
	TRAPD(err, outStream = COutputStreamer::NewL(iLogger, 1));
	iLogger->Log(_L("Returned from NewL: %d"), err );
	aResult.iResult = err ;

	if ( err )
	{
		iLogger->Log(_L("Test failed, error code %d"), err);
		aResult.iResultDes.Copy(GetErrorDes(err));
	}
	else 
	{
		TInt maxVol = outStream->GetMaxVolume();
	    TInt vol = GetIntL(aSection, KTagVolume, usingDefault, maxVol);

		// convert relative volume to actual volume
		vol = (maxVol*vol)/100;
   		iLogger->Log(_L("Volume: 0 to %d"), maxVol);
   		
	    // if using default value, error reading data
	    //if (usingDefault || (vol < 0) || (vol > maxVol))
	    if (usingDefault)
	    {
			aResult.iResultDes.Copy(KConfigInvalid());
   			aResult.iResult = KErrNotExecuted;
			returnErr = KErrConfigInvalid;
	    }
	    else
	    {
			///set the volume
			outStream->SetVolume(vol);

			///get the volume
			TInt rvol = outStream->GetVolume();
			if (vol != rvol ) 
			{
	       		iLogger->Log(_L("Expected volume: %d"), vol);
	       		iLogger->Log(_L("Actual volume  : %d"), rvol);
	       		iLogger->Log(_L("Test failed, expected volume is different from actual volume"));
		        aResult.iResultDes.Copy(KTestCaseResultFailExpectedValueDifferent());
				aResult.iResult = KErrExpectedValueDifferent;
			}
			else 
			{
    	   		iLogger->Log(_L("Test was successful"));
		        aResult.iResultDes.Copy(KTestCaseResultSuccess());
            	aResult.iResult = KErrNone ;
			}

		}
	}

	delete outStream;
	outStream = NULL;
    
    return returnErr;
}

TInt CAudioStreamTestApp::TestCaseOutputSetPriorityL(CStifSectionParser *aSection, TTestResult & aResult)
{
    TInt returnErr = KErrExecuted;
    TBool usingDefault1, usingDefault2;
    COutputStreamer* outStream;
    
	TRAPD(err, outStream = COutputStreamer::NewL(iLogger, 1));
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
	    
        	outStream->SetPriority(priority, IntToPriorityPreference(pref));

            // no way to determine if successful
       		iLogger->Log(_L("Test was successful"));
	        aResult.iResultDes.Copy(KTestCaseResultSuccess());
           	aResult.iResult = KErrNone ;
	    }
	}

	delete outStream;
	outStream = NULL;
    
    return returnErr;
}

TInt CAudioStreamTestApp::TestCaseOutputPlayL(CStifSectionParser *aSection, TTestResult & aResult)
{
	TBool usingDefault;
    TInt returnErr = KErrExecuted;
	TInt err = KErrNone;
    COutputStreamer* outStream;
    
	TRAP(err, outStream = COutputStreamer::NewL(iLogger, 1));
	iLogger->Log(_L("Returned from NewL: %d"), err );
	aResult.iResult = err ;

	if ( err )
	{
		iLogger->Log(_L("Test failed, error code %d"), err);
		aResult.iResultDes.Copy(GetErrorDes(err));
		return returnErr;
	}

	// read in file name
	TPtrC fileNamePtr;
	TBool haveFileName = !aSection->GetLine(KTagSoundDes, fileNamePtr, ENoTag) ;
	if (!haveFileName)
	{
		aResult.iResult = KErrNotExecuted;
		aResult.iResultDes.Copy(KConfigInvalid());
		iLogger->Log(KTestCaseResultFinished() , &KConfigInvalid() );
		returnErr = KErrConfigInvalid;
	}
	else
	{
		TFileName fileName = fileNamePtr;
		err = outStream->LoadAudioFileL(fileName);

		if (err)
		{
	   		aResult.iResult = err ;
   			iLogger->Log(_L("Test failed, error code %d"), err);
	        aResult.iResultDes.Copy(GetErrorDes(err));
		}
		else
		{
	    	// read in priority and preference
		  	TInt priority = GetIntL(aSection, KTagPriority, usingDefault, KDefaultPriority);
    		TInt pref = GetIntL(aSection, KTagPriorityPreference, usingDefault, KDefaultPriorityPreference);
		   	outStream->SetPriority(priority, IntToPriorityPreference(pref));

			//Get settings
			TInt maxVol = outStream->GetMaxVolume();
			TInt volOut        = GetIntL(aSection, KTagVolume, usingDefault, maxVol/2);
			TInt sampleRateOut = GetIntL(aSection, KTagSampleRate, usingDefault, CStreamerBase::KSampleRate8000Hz);
			TInt channelsOut   = GetIntL(aSection, KTagChannelCount, usingDefault, CStreamerBase::KChannelsMono);
	
			// convert relative volume to actual volume
			volOut = (maxVol*volOut)/100;
			if (volOut < KRelVolumeMin) {volOut=KRelVolumeMin;}
			if (volOut > maxVol) {volOut=maxVol;}

			TMdaAudioDataSettings *settingsOut = new(ELeave)TMdaAudioDataSettings;
			CleanupStack::PushL(settingsOut);

			settingsOut->iVolume = volOut;
			settingsOut->iCaps |= TMdaAudioDataSettings::ESampleRateFixed;

			//Setup the sample rate bit according to the integer read
			settingsOut->iSampleRate = outStream->TranslateRate(sampleRateOut, TMdaAudioDataSettings::ESampleRate8000Hz);
			settingsOut->iChannels = (channelsOut==1 ? TMdaAudioDataSettings::EChannelsMono : TMdaAudioDataSettings::EChannelsStereo);

			outStream->StartPlayback(settingsOut);
			CActiveScheduler::Start();

			// Pause 1 sec to let player finish playing last buffer.
			TTimeIntervalMicroSeconds32 anInterval=1000000;
			User::After(anInterval);

			CleanupStack::PopAndDestroy(settingsOut);
		
			err = outStream->GetFinalError();
   			aResult.iResult = err ;
			if (err)
			{
   				iLogger->Log(_L("Test failed, error code %d"), err);
	        	aResult.iResultDes.Copy(GetErrorDes(err));
			}
			else 
			{
    			iLogger->Log(_L("Test was successful"));
			    aResult.iResultDes.Copy(KTestCaseResultSuccess());
			}
		}
	}
	
	delete outStream;
	outStream = NULL;

	return returnErr;
}

TInt CAudioStreamTestApp::TestCaseOutputSetBalanceL(CStifSectionParser *aSection, TTestResult & aResult)
{
    TInt returnErr = KErrExecuted;
    TBool usingDefault;
    COutputStreamer* outStream;
    
	TRAPD(err, outStream = COutputStreamer::NewL(iLogger, 1));
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
	    
        	TRAPD(err1, outStream->SetBalanceL(balance));
        	TInt rBalance;
        	TRAPD(err2, rBalance = outStream->GetBalanceL());

        	if ( err1 || err2 || (balance != rBalance) )
        	{
				iLogger->Log(_L("Balance is %d"), rBalance);
				iLogger->Log(_L("Expected Balance is %d"), balance);
				iLogger->Log(_L("Expected Balance is different from real Balance"));
				aResult.iResultDes.Copy(KTestCaseResultFailExpectedValueDifferent());
				aResult.iResult = KErrExpectedValueDifferent;
        	}
        	else
        	{
        		iLogger->Log(_L("Test was successful"));
		        aResult.iResultDes.Copy(KTestCaseResultSuccess());
            	aResult.iResult = KErrNone ;
        	}
	    }
	}

	delete outStream;
	outStream = NULL;
    
    return returnErr;
}

TInt CAudioStreamTestApp::TestCaseOutputSetDataTypeL(CStifSectionParser *aSection, TTestResult & aResult)
{
    TInt returnErr = KErrExecuted;
    TBool usingDefault;
    COutputStreamer* outStream;
    
	TRAPD(err, outStream = COutputStreamer::NewL(iLogger, 1));
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
			outStream->StartPlayback(NULL);
		    outStream->SetExitAfterOpen(ETrue);
			CActiveScheduler::Start();

        	TRAP(err, outStream->SetDataTypeL(dataType));
           	aResult.iResult = err ;
        	TFourCC rDataType = outStream->GetDataType();

        	//if ((dataType == rDataType) || (err == KErrNotSupported))
        	if (dataType == rDataType)
        	{
        		iLogger->Log(_L("Test was successful"));
		        aResult.iResultDes.Copy(KTestCaseResultSuccess());
        	}
        	else if (err == KErrNotSupported)
        	{
        		aResult.iResult = KErrNone;
        		iLogger->Log(_L("KErrNotSupported is accepted, Test was successful"));   //Sue
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

	delete outStream;
	outStream = NULL;
    
    return returnErr;
}

TInt CAudioStreamTestApp::TestCaseOutputStopL(CStifSectionParser *aSection, TTestResult & aResult)
{
	TBool usingDefault;
    TInt returnErr = KErrExecuted;
	TInt err = KErrNone;
    COutputStreamer* outStream;
    
	TRAP(err, outStream = COutputStreamer::NewL(iLogger, 1));
	iLogger->Log(_L("Returned from NewL: %d"), err );
	aResult.iResult = err ;

	if ( err )
	{
		iLogger->Log(_L("Test failed, error code %d"), err);
		aResult.iResultDes.Copy(GetErrorDes(err));
		return returnErr;
	}

	// read in file name
	TPtrC fileNamePtr;
	TBool haveFileName = !aSection->GetLine(KTagSoundDes, fileNamePtr, ENoTag) ;
	if (!haveFileName)
	{
		aResult.iResult = KErrNotExecuted;
		aResult.iResultDes.Copy(KConfigInvalid());
		iLogger->Log(KTestCaseResultFinished() , &KConfigInvalid() );
		returnErr = KErrConfigInvalid;
	}
	else
	{
		TFileName fileName = fileNamePtr;
		err = outStream->LoadAudioFileL(fileName);

		if (err)
		{
	   		aResult.iResult = err ;
   			iLogger->Log(_L("Test failed, error code %d"), err);
	        aResult.iResultDes.Copy(GetErrorDes(err));
		}
		else
		{
	    	// read in priority and preference
		  	TInt priority = GetIntL(aSection, KTagPriority, usingDefault, KDefaultPriority);
    		TInt pref = GetIntL(aSection, KTagPriorityPreference, usingDefault, KDefaultPriorityPreference);
		   	outStream->SetPriority(priority, IntToPriorityPreference(pref));

			//Get settings
			TInt maxVol = outStream->GetMaxVolume();
			TInt volOut        = GetIntL(aSection, KTagVolume, usingDefault, maxVol/2);
			TInt sampleRateOut = GetIntL(aSection, KTagSampleRate, usingDefault, CStreamerBase::KSampleRate8000Hz);
			TInt channelsOut   = GetIntL(aSection, KTagChannelCount, usingDefault, CStreamerBase::KChannelsMono);

			// convert relative volume to actual volume
			volOut = (maxVol*volOut)/100;
			if (volOut < KRelVolumeMin) {volOut=KRelVolumeMin;}
			if (volOut > maxVol) {volOut=maxVol;}

			TMdaAudioDataSettings *settingsOut = new(ELeave)TMdaAudioDataSettings;
			CleanupStack::PushL(settingsOut);

			settingsOut->iVolume = volOut;
			settingsOut->iCaps |= TMdaAudioDataSettings::ESampleRateFixed;

			//Setup the sample rate bit according to the integer read
			settingsOut->iSampleRate = outStream->TranslateRate(sampleRateOut, TMdaAudioDataSettings::ESampleRate8000Hz);
			settingsOut->iChannels = (channelsOut==1 ? TMdaAudioDataSettings::EChannelsMono : TMdaAudioDataSettings::EChannelsStereo);

    		//Event to stop playback after StopDelay microseconds (from CActiveScheduler::Start() )
			TInt stopDelay = GetIntL(aSection, KTagStopDelay, usingDefault, KDefaultStopDelay);
		    iLogger->Log(_L("Playback will stop after (%d) microseconds") , stopDelay);
    		CParameters *StopPlayParam = new(ELeave) CParameters( COutputStreamer::KPlayerActionStop );
		    CleanupStack::PushL(StopPlayParam);
    		CMediaEvent *MeStopPlay = CMediaEvent::NewL(iLogger, stopDelay, stopDelay, outStream, 
    		                                            StopPlayParam, (CActive::EPriorityStandard) );
		    CleanupStack::Pop(StopPlayParam);
    		CleanupStack::PushL(MeStopPlay);

			outStream->StartPlayback(settingsOut);
			CActiveScheduler::Start();

    		CleanupStack::PopAndDestroy(MeStopPlay);
			CleanupStack::PopAndDestroy(settingsOut);
		
			err = outStream->GetFinalError();
   			aResult.iResult = err ;
			if (err)
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
			else 
			{
	    		iLogger->Log(_L("Test was successful"));
			    aResult.iResultDes.Copy(KTestCaseResultSuccess());
			}
		}
	}
	
	delete outStream;
	outStream = NULL;

	return returnErr;
}

//  End of File
