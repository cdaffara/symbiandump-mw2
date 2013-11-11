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
* Description: Audio Output Control Utility
*
*/



// INCLUDE FILES
#include "AudioOutputControlTestClass.h"
#include "debug.h"




/************************************************************************************************************/

void CAudioOutputControlTestClass::Event( MControl* aControl, TUint aEventType, TAny* aEventObject )
    {
    switch (aEventType)
        {
        case MStreamControlObserver::KStateChangedEvent:
            {
            MStreamControl* control1 = (MStreamControl*)(aControl);
            MErrorCode* evt = (MErrorCode*)aEventObject;
            RDebug::Print(_L("CEMCTestApp::Event:EStateChanged"));
            switch(control1->GetState())
                {
                case MStreamControl::CLOSED:
                    RDebug::Print(_L("CEMCTestApp::Event:EStateChanged[Closed]"));
                    //PrintMessageWindow(_L("EStateChanged[Closed]"));
                    break;
                case MStreamControl::INITIALIZED:
                    RDebug::Print(_L("CEMCTestApp::Event:EStateChanged[Opened]"));
                    //PrintMessageWindow(_L("EStateChanged[Opened]"));
                    if( evt->GetErrorCode() == KErrEof )
                        {
                        SourceMenuResetEOFFlag();
                        }

                     TInt err = iMVolumeControl->GetMaxVolume(iMaxVolume);
                    // For descriptor source set DRM Type and allowed output
                    if (iMDataBufferSource)
                        {
                        if (iDRMConfigIntfc)
                            {
                            delete iDRMConfigIntfc;
                            iDRMConfigIntfc = NULL;
                            }
                        TAny* intfc(NULL);
                        TVersion ver(KDRMConfigIntfcMajorVer1, KDRMConfigIntfcMinorVer1, KDRMConfigIntfcBuildVer1);
                        TInt status = iMDataBufferSource->GetInterface(KDRMConfigIntfc,
                                                                      ver,
                                                                      intfc);
                        if (status == KErrNone)
                            {
                            iDRMConfigIntfc = (CDRMConfigIntfc*)intfc;
                      //      RDebug::Print(_L("CEMCTestApp::Event:SetDRMType[%d]"), iDRMConfigIntfc->SetDRMType(EOMA2));
                      //      RDebug::Print(_L("CEMCTestApp::Event:AppendAllowedOutputDevice(EAudioAllowAnalog)[%d]"), iDRMConfigIntfc->AppendAllowedOutputDevice(EAudioAllowAnalog));
                       //     RDebug::Print(_L("CEMCTestApp::Event:AppendAllowedOutputDevice(EAudioAllowFMTx)[%d]"), iDRMConfigIntfc->AppendAllowedOutputDevice(EAudioAllowFMTx));
                       //     RDebug::Print(_L("CEMCTestApp::Event:AppendAllowedOutputDevice(EAudioAllowFMTx)[%d]"), iDRMConfigIntfc->AppendAllowedOutputDevice(EAudioAllowFMTx));
                       //     RDebug::Print(_L("CEMCTestApp::Event:AppendAllowedOutputDevice(EAudioAllowFMTx)[%d]"), iDRMConfigIntfc->Commit());
                           }
                        }
                    break;
                case MStreamControl::PRIMED:
                    RDebug::Print(_L("CEMCTestApp::Event:EStateChanged[Primed]"));
                    //PrintMessageWindow(_L("EStateChanged[Primed]"));
                    break;
                case MStreamControl::EXECUTING:
                    RDebug::Print(_L("CEMCTestApp::Event:EStateChanged[Playing]"));
                    //PrintMessageWindow(_L("EStateChanged[Playing]"));
                    break;
                case MStreamControl::BUFFERING:
                    RDebug::Print(_L("CEMCTestApp::Event:EStateChanged[Buffering]"));
                    //PrintMessageWindow(_L("EStateChanged[Buffering]"));
                    break;
                case MStreamControl::PAUSED:
                    RDebug::Print(_L("CEMCTestApp::Event:EStateChanged[Paused]"));
                    //PrintMessageWindow(_L("EStateChanged[Paused]"));
                    break;
                default:
                    break;
                };
             }
             break;


        /*case MSourceControlObserver::KBufferProcessedEvent:
            {
            MBufferProcessedEvent* evt = (MBufferProcessedEvent*)aEventObject;
            RDebug::Print(_L("CEMCTestApp::Event:EBufferProcessed[AudioBuffer[0x%x]Reason[%d]]") , evt->GetDataBuffer(), evt->GetErrorCode() );
            TInt index = iBuffers.Find( evt->GetDataBuffer() );
            if ( index >= 0 )
                {
                iAvailable[index] = ETrue;
                }

            if(!iBufferEmptiedEventAO->IsActive() && !iIsEOFReached)
                iBufferEmptiedEventAO->SetActive();

            if(iBufferEmptiedEventAO->IsActive())
                {
                TRequestStatus* status = &(iBufferEmptiedEventAO->iStatus);
                User::RequestComplete(status,evt->GetErrorCode());
                }

            }
            break;

        case MSourceControlObserver::KBitRateChangedEvent:
            TUint rate;
            iMFileSource->GetBitRate(rate);
            RDebug::Print(_L("CEMCTestApp::Event:BitRateChanged[%d]"),rate);
            break;

        case MStreamControlObserver::KDurationChangedEvent:
            PrintMessageWindow(_L("EDurationChanged"));
            break;*/
        default:
            break;
        };
    }






//Devsound initialization is completed.Configure devsound capabilities.
void CAudioOutputControlTestClass::InitializeComplete(TInt aError)
	{
    FTRACE(FPrint(_L("CAudioOutputControlTestClass::InitializeComplete")));

    iLog->Log(_L("InitializeComplete, return code =%d!!!"),aError);
    ProcessEvent(EInitializeComplete, aError);

	}


void CAudioOutputControlTestClass::ToneFinished(TInt aError)
   	{
 //	Panic(KErrNotSupported);
 	}

//This is called when an empty audio buffer is available.Fill the buffer with audio data.
void CAudioOutputControlTestClass::BufferToBeFilled(CMMFBuffer* aBuffer)
	{

	}

//This is called when an audio play completion is successfully played or otherwise
void CAudioOutputControlTestClass::PlayError(TInt aError)
 	{
//	SendErrorToClient(aError);
	}


//Will panic if called.Should not be called during playing
void CAudioOutputControlTestClass::BufferToBeEmptied(CMMFBuffer* aBuffer)
	 {
//	 Panic(KErrNotSupported);
	 }


//Will panic if called.Should not be called during playing
void CAudioOutputControlTestClass::RecordError(TInt aError)
	 {
  //   Panic(KErrNotSupported);
	 }


//Will panic if called.Should not be called during playing
void CAudioOutputControlTestClass::ConvertError(TInt aError)
	 {
//	 Panic(KErrNotSupported);
	 }


//Will panic if called.Should not be called during playing
void CAudioOutputControlTestClass::DeviceMessage(TUid aMessageType, const TDesC8& aMsg)
	 {
//	 Panic(KErrNotSupported);
	 }
/*
// Audio Output Stream
void CAudioOutputControlTestClass::MaoscOpenComplete(TInt aError)
	{
    FTRACE(FPrint(_L("CAudioOutputControlTestClass::MaoscOpenComplete")));
    iLog->Log(_L("MaoscOpenComplete, return code =%d!!!"),aError);
    ProcessEvent(EInitializeComplete, aError);
	}
void CAudioOutputControlTestClass::MaoscBufferCopied(TInt aError, const TDesC8& aBuffer)
	{
	}

void CAudioOutputControlTestClass::MaoscPlayComplete(TInt aError)
	{
	}*/
// End of File
