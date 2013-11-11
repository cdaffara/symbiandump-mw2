// Copyright (c) 1998-2009 Nokia Corporation and/or its subsidiary(-ies).
// All rights reserved.
// This component and the accompanying materials are made available
// under the terms of "Eclipse Public License v1.0"
// which accompanies this distribution, and is available
// at the URL "http://www.eclipse.org/legal/epl-v10.html".
//
// Initial Contributors:
// Nokia Corporation - initial contribution.
//
// Contributors:
//
// Description:
//

#include <e32std.h>
#include <bacline.h>
#include <d32comm.h>
#include <e32test.h>
#include <f32file.h>

RTest test(_L("T_SERIALLOG"));

const TInt KSerialLogBufferSize=1024;
const TInt KSerialLogDefaultSerialPort=0;
const TInt KSerialLogTransmitTimeout=1000000;
const TInt KSerialLogErrorDisplayWait=5000000;
_LIT8(KSerialLogStartDelimiterFormat,"[=====> START %S]\n");
_LIT8(KSerialLogStopDelimiterFormat, "[=====> STOP %S]\n");

LOCAL_C void ReportError(TInt aError)
	{
	test.Console()->Printf(_L("\n\n"));
	switch (aError)
		{
		case KErrArgument:
			test.Console()->Printf(_L("Usage: t_seriallog logfilename [serial port]\n"));
			break;
		case KErrTimedOut:
			test.Console()->Printf(_L("ERROR: Timeout during serial write\n"));
			break;
		case KErrNotFound:
			test.Console()->Printf(_L("ERROR: File/Port not found\n"));
			break;
		case KErrNotSupported:
			test.Console()->Printf(_L("ERROR: Port not found/supported\n"));
			break;
		default:
			test.Console()->Printf(_L("ERROR: %d\n"),aError);
			break;
		}
	if (aError)
		User::After(KSerialLogErrorDisplayWait);
	}

LOCAL_C TInt SetSerialConfiguration(RBusDevComm& aSerial)
	{
	TCommConfigV01 theConfig;
	TCommConfig config(theConfig);
	aSerial.Config(config);
	config().iRate=EBps115200;
	config().iParity=EParityNone;
	config().iDataBits=EData8;
	config().iStopBits=EStop1;
	return(aSerial.SetConfig(config));
	}

LOCAL_C TInt SendSerialData(RBusDevComm& aSerial,const TDesC8& aData)
	{
	TRequestStatus serStat;
	TRequestStatus timStat;

	RTimer timer;
	TInt error=timer.CreateLocal();
	if (!error)
		{
		timer.After(timStat,KSerialLogTransmitTimeout);
		aSerial.Write(serStat,aData);
		User::WaitForRequest(serStat,timStat);
		if (timStat.Int()==KErrNone)
			{
			aSerial.WriteCancel();
			error=KErrTimedOut;
			}
		else if (serStat.Int()!=KErrNone)
			{
			timer.Cancel();
			error=serStat.Int();
			}
		else
			timer.Cancel();
		timer.Close();
		}
	return(error);
	}

LOCAL_C TInt SendDelimiterL(RBusDevComm& aSerial,const TDesC8& aFormatter,const TPtrC& aLogFile)
	{
	TBuf8<KSerialLogBufferSize> aLogFileDes8;
	for (TInt i=0;i<aLogFile.Length();i++)
		aLogFileDes8.Append((TChar)aLogFile[i]);
	TBuf8<KSerialLogBufferSize> buffer;
	buffer.Format(aFormatter,&aLogFileDes8);
	return(SendSerialData(aSerial,buffer));
	}

LOCAL_C void doOpenSerialL(RBusDevComm& aSerial, TInt aPort)
	{
	TInt error;
#if defined (__WINS__)
	error=User::LoadPhysicalDevice(_L("ECDRV"));
#else
	error=User::LoadPhysicalDevice(_L("EUART1"));
	if (error==KErrNone||error==KErrAlreadyExists)
		error=User::LoadPhysicalDevice(_L("EUART2"));
	if (error==KErrNone||error==KErrAlreadyExists)
		error=User::LoadPhysicalDevice(_L("EUART3"));
	if (error==KErrNone||error==KErrAlreadyExists)
		error=User::LoadPhysicalDevice(_L("EUART4"));
#endif
	if (error==KErrNone||error==KErrAlreadyExists||error==KErrNotFound)
		error=User::LoadLogicalDevice(_L("ECOMM"));
	if (error==KErrAlreadyExists)
		error=KErrNone;
	User::LeaveIfError(error);

	test.Printf(_L("Loaded serial device drivers.\n"));

	// Open serial port.
	User::LeaveIfError(aSerial.Open(aPort));
	User::LeaveIfError(SetSerialConfiguration(aSerial));

	test.Printf(_L("Opened serial port.\n"));
	}

LOCAL_C void doSendTerminateL(TInt aPort)
	{
	// Open serial port.
	RBusDevComm serial;
	doOpenSerialL(serial,aPort);
	SendSerialData(serial, _L8("!TERMINATE_MSG_LOGGING!"));
	// Close serial port.
	serial.Close();
	}

LOCAL_C void doSendSerialL(const TPtrC& aLogFile, TInt aPort)
	{
	TInt error;
	RFs aFs;
	User::LeaveIfError(aFs.Connect());

	// Open logfile.
	RFile logFile;
	test.Printf(_L("\nSending file: %S.\n"),&aLogFile);
	User::LeaveIfError(logFile.Open(aFs,aLogFile,EFileRead));

	// Open serial port.
	RBusDevComm serial;
	doOpenSerialL(serial, aPort);

	// Send the start delimiter.
	TBuf8<KSerialLogBufferSize> buffer;
	error=SendDelimiterL(serial,KSerialLogStartDelimiterFormat,aLogFile);
	
	// Send the logfile.
	while (!error)
		{
		error=logFile.Read(buffer);
		if (!error && buffer.Length())
			error=SendSerialData(serial,buffer);
		if (!buffer.Length())
			error=KErrEof;
		}

	// Any errors?
	if (error==KErrEof)
		error=KErrNone;

	// Send the stop delimiter.
	if (!error)
		error=SendDelimiterL(serial,KSerialLogStopDelimiterFormat,aLogFile);
	User::After(1000000);

	// Close serial port.
	serial.Close();
	// Close logfile.
	logFile.Close();

	User::LeaveIfError(error);
	}

LOCAL_C void doMainL()
	{
	// Get command line.
	CCommandLineArguments* parsed=CCommandLineArguments::NewLC();
	// Check for numerical arguments.
	TInt error=KErrNone;
	if (parsed->Count()>=2)
		{
		//  Argument 1 = log file name [Argument 2 = serial port (defaults to 0)]
		TInt serialPort=KSerialLogDefaultSerialPort;
		if (parsed->Count()==3)
			{
			TLex convArg(parsed->Arg(2));
			TInt parsedSerialPort;
			if (convArg.Val(parsedSerialPort)==KErrNone)
				serialPort=parsedSerialPort;
			else
				error=KErrArgument;
			}
		if (parsed->Count()>3)
			error=KErrArgument;
		if (!error)
			{
			if (parsed->Arg(1).Compare(_L("TERMINATE")) == 0)
				doSendTerminateL(serialPort);
			else
				doSendSerialL(parsed->Arg(1),serialPort);
			}
		}
	else
		{
		error=KErrArgument;
		}

	CleanupStack::PopAndDestroy(parsed);
	User::LeaveIfError(error);
	}

GLDEF_C TInt E32Main()
	{	
	__UHEAP_MARK;
//	HAL::SetAutoSwitchOffBehavior(ESwitchOffDisabled);	// Can't do this anymore :<
	User::ResetInactivityTime();
	test.Title();
	CTrapCleanup* theCleanup=CTrapCleanup::New();
	TRAPD(result,doMainL()); 
	ReportError(result);
	delete theCleanup;      
	test.Close();
	__UHEAP_MARKEND;
	return(KErrNone);
	}
