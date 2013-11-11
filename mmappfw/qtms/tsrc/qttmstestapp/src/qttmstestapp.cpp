/*
 * Copyright (c) 2009 Nokia Corporation and/or its subsidiary(-ies).
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
 * Description: Telephony Multimedia Service - QTmsTestApp
 *
 */

#include "qtmstestengine.h"
#include "ctmstestengine.h"
#include "mtmstestengine.h"
#include "qttmstestapp.h"

#define CHECKENGINE if(!iTmsEngine){QMessageBox::critical(0, tr("Error"),\
                                   tr("Engine not ready."),\
                                   QMessageBox::Ok);\
                                   return;}

TmsTestApp::TmsTestApp(QWidget *parent) :
    QMainWindow(parent)
    {
    mainContainer = new QWidget(this);
    setCentralWidget(mainContainer);

    iTmsEngine = NULL;

    statusDisplay = new QTextEdit(mainContainer);

    statusDisplay->setReadOnly(true);

    statusDisplay->setAlignment(Qt::AlignTop);

    statusDisplay->setTextColor(QColor(Qt::black));
    statusDisplay->setText("Qt TMS test app");

    QPalette p = statusDisplay->palette();
    p.setColor(QPalette::Base, QColor(Qt::white));
    statusDisplay->setPalette(p);
    QGridLayout *layout = new QGridLayout;

    layout->addWidget(statusDisplay);
    statusDisplay->setForegroundRole(QPalette::Foreground);

    mainContainer->setLayout(layout);

    createActions();
    createMenus();
    }

TmsTestApp::~TmsTestApp()
    {
    delete iTmsEngine;
    }

void TmsTestApp::createActions()
    {
    //Select engine
    qtmsengine = new QAction(tr("QT TMS"),this);
    connect(qtmsengine, SIGNAL(triggered()), this, SLOT(qtmsengineslot()));
    tmsengine  = new QAction(tr("TMS"),this);
    connect(tmsengine, SIGNAL(triggered()), this, SLOT(tmsengineslot()));

    //Call type actions
    cscall = new QAction(tr("CS"), this);
    connect(cscall, SIGNAL(triggered()), this, SLOT(cscallslot()));
    ipcall = new QAction(tr("IP"), this);
    connect(ipcall, SIGNAL(triggered()), this, SLOT(ipcallslot()));

    //Stream actions
    pcmdec = new QAction(tr("PCM-16"),this);
    connect(pcmdec, SIGNAL(triggered()), this, SLOT(pcmdecslot()));
    amrdec = new QAction(tr("AMR-NB"),this);
    connect(amrdec, SIGNAL(triggered()), this, SLOT(amrdecslot()));
    g711dec = new QAction(tr("G711"),this);
    connect(g711dec, SIGNAL(triggered()), this, SLOT(g711decslot()));
    g729dec = new QAction(tr("G729"),this);
    connect(g729dec, SIGNAL(triggered()), this, SLOT(g729decslot()));
    ilbcdec = new QAction(tr("iLBC"),this);
    connect(ilbcdec, SIGNAL(triggered()), this, SLOT(ilbcdecslot()));

    pcmenc = new QAction(tr("PCM-16"),this);
    connect(pcmenc, SIGNAL(triggered()), this, SLOT(pcmencslot()));
    amrenc = new QAction(tr("AMR-NB"),this);
    connect(amrenc, SIGNAL(triggered()), this, SLOT(amrencslot()));
    g711enc = new QAction(tr("G711"),this);
    connect(g711enc, SIGNAL(triggered()), this, SLOT(g711encslot()));
    g729enc = new QAction(tr("G729"),this);
    connect(g729enc, SIGNAL(triggered()), this, SLOT(g729encslot()));
    ilbcenc = new QAction(tr("iLBC"),this);
    connect(ilbcenc, SIGNAL(triggered()), this, SLOT(ilbcencslot()));

    downlinkformats = new QAction(tr("Get downlink formats"),this);
    connect(downlinkformats, SIGNAL(triggered()), this, SLOT(downlinkformatsslot()));
    strmdnstart = new QAction(tr("Start Dnlink"), this);
    connect(strmdnstart, SIGNAL(triggered()), this, SLOT(strmdnstartslot()));
    strmdnstop = new QAction(tr("Stop Dnlink"), this);
    connect(strmdnstop, SIGNAL(triggered()), this, SLOT(strmdnstopslot()));
    strmdnclose = new QAction(tr("Close Dnlink"), this);
    connect(strmdnclose, SIGNAL(triggered()), this, SLOT(strmdncloseslot()));

    uplinkformats = new QAction(tr("Get uplink formats"),this);
    connect(uplinkformats, SIGNAL(triggered()), this, SLOT(uplinkformatsslot()));
    strmupstart = new QAction(tr("Start Uplink"), this);
    connect(strmupstart, SIGNAL(triggered()), this, SLOT(strmupstartslot()));
    strmupstop = new QAction(tr("Stop Uplink"), this);
    connect(strmupstop, SIGNAL(triggered()), this, SLOT(strmupstopslot()));
    strmupclose = new QAction(tr("Close Uplink"), this);
    connect(strmupclose, SIGNAL(triggered()), this, SLOT(strmupcloseslot()));

    //Routing actions
    availoutputact = new QAction(tr("Available outputs"), this);
    connect(availoutputact, SIGNAL(triggered()), this, SLOT(availoutputslot()));
    currentoutputact = new QAction(tr("Current Device Output"), this);
    connect(currentoutputact, SIGNAL(triggered()), this, SLOT(currentoutputslot()));
    handsetact = new QAction(tr("Select HandSet"), this);
    connect(handsetact, SIGNAL(triggered()), this, SLOT(handsetslot()));
    loudspkract = new QAction(tr("Select Loudspkr"), this);
    connect(loudspkract, SIGNAL(triggered()), this, SLOT(loudspkrslot()));
    wiredact = new QAction(tr("Select Wired accessory"), this);
    connect(wiredact, SIGNAL(triggered()), this, SLOT(wiredslot()));
    btact = new QAction(tr("Select BlueTooth"), this);
    connect(btact, SIGNAL(triggered()), this, SLOT(btslot()));
    ttyact = new QAction(tr("Select TTY"), this);
    connect(ttyact, SIGNAL(triggered()), this, SLOT(ttyslot()));

    //Stream gain effect actions
    stmgainact = new QAction(tr("Get gain"), this);
    connect(stmgainact, SIGNAL(triggered()), this, SLOT(stmgainslot()));
    stmmaxgainact = new QAction(tr("Get max gain"), this);
    connect(stmmaxgainact, SIGNAL(triggered()), this, SLOT(stmmaxgainslot()));
    stmsetgainact = new QAction(tr("Set gain"), this);
    connect(stmsetgainact, SIGNAL(triggered()), this, SLOT(stmsetgainslot()));
    stmmutegainact = new QAction(tr("Mute mic"), this);
    connect(stmmutegainact, SIGNAL(triggered()), this, SLOT(stmmutegainslot()));

    //Stream vol effect actions
    stmvolact = new QAction(tr("Get vol"), this);
    connect(stmvolact, SIGNAL(triggered()), this, SLOT(stmvolslot()));
    stmmaxvolact = new QAction(tr("Get max vol"), this);
    connect(stmmaxvolact, SIGNAL(triggered()), this, SLOT(stmmaxvolslot()));
    stmsetvolact = new QAction(tr("Set vol"), this);
    connect(stmsetvolact, SIGNAL(triggered()), this, SLOT(stmsetvolslot()));
    stmmutevolact = new QAction(tr("Mute spkr"), this);
    connect(stmmutevolact, SIGNAL(triggered()), this, SLOT(stmmutevolslot()));

    //Global vol effects actions
    globalvolact = new QAction(tr("Get global vol"), this);
    connect(globalvolact, SIGNAL(triggered()), this, SLOT(globalvolslot()));
    globalmaxvolact = new QAction(tr("Get global max vol"), this);
    connect(globalmaxvolact, SIGNAL(triggered()), this, SLOT(globalmaxvolslot()));
    globalsetvolact = new QAction(tr("Set global vol"), this);
    connect(globalsetvolact, SIGNAL(triggered()), this, SLOT(globalsetvolslot()));

    //Global gain effects actions
    globalgainact = new QAction(tr("Get global gain"), this);
    connect(globalgainact, SIGNAL(triggered()), this, SLOT(globalgainslot()));
    globalmaxgainact = new QAction(tr("Get global max gain"), this);
    connect(globalmaxgainact, SIGNAL(triggered()), this, SLOT(globalmaxgainslot()));
    globalsetgainact = new QAction(tr("Set global gain"), this);
    connect(globalsetgainact, SIGNAL(triggered()), this, SLOT(globalsetgainslot()));

    //Decoder settings
    togglecng = new QAction(tr("ToggleCng G711/iLBC"), this);
    connect(togglecng,SIGNAL(triggered()), this ,SLOT(togglecngslot()));
    getcng = new QAction(tr("GetCng G711/iLBC"), this);
    connect(getcng,SIGNAL(triggered()), this ,SLOT(getcngslot()));
    toggleplc = new QAction(tr("Toggle PLC G711"), this);
    connect(toggleplc,SIGNAL(triggered()), this ,SLOT(toggleplcslot()));
    getplc = new QAction(tr("Get PLC G711"), this);
    connect(getplc,SIGNAL(triggered()), this ,SLOT(getplcslot()));
    setalawdecg711 = new QAction(tr("Set ALaw G711"), this);
    connect(setalawdecg711,SIGNAL(triggered()), this ,SLOT(setalawdecg711slot()));
    setulawdecg711 = new QAction(tr("Set uLaw G711"), this);
    connect(setulawdecg711,SIGNAL(triggered()), this ,SLOT(setulawdecg711slot()));
    getmodedecg711 = new QAction(tr("Get mode G711"), this);
    connect(getmodedecg711,SIGNAL(triggered()), this ,SLOT(getmodegdec711slot()));
    set20msdecilbc = new QAction(tr("Set 20ms frm iLBC"), this);
    connect(set20msdecilbc,SIGNAL(triggered()), this ,SLOT(set20msdecilbcslot()));
    set30msdecilbc = new QAction(tr("Set 30ms frm iLBC"), this);
    connect(set30msdecilbc,SIGNAL(triggered()), this ,SLOT(set30msdecilbcslot()));
    getmodedecilbc = new QAction(tr("Getmode iLBC"), this);
    connect(getmodedecilbc,SIGNAL(triggered()), this ,SLOT(getmodedecilbcslot()));

    //Encoder settings
    getsupportedbr = new QAction(tr("Get supported bitrates"), this);
    connect(getsupportedbr,SIGNAL(triggered()), this ,SLOT(getsupportedbrslot()));
    setminbr = new QAction(tr("Set min br"), this);
    connect(setminbr,SIGNAL(triggered()), this ,SLOT(setminbrslot()));
    setmaxbr = new QAction(tr("Set max br"), this);
    connect(setmaxbr,SIGNAL(triggered()), this ,SLOT(setmaxbrslot()));
    getbr = new QAction(tr("Get br"), this);
    connect(getbr,SIGNAL(triggered()), this ,SLOT(getbrslot()));
    togglevad = new QAction(tr("Toggle VAD"), this);
    connect(togglevad,SIGNAL(triggered()), this ,SLOT(togglevadslot()));
    getvad = new QAction(tr("Get VAD"), this);
    connect(getvad,SIGNAL(triggered()), this ,SLOT(getvadslot()));
    setalawencg711 = new QAction(tr("Set ALaw G711"), this);
    connect(setalawencg711,SIGNAL(triggered()), this ,SLOT(setalawencg711slot()));
    setulawencg711 = new QAction(tr("Set uLaw G711"), this);
    connect(setulawencg711,SIGNAL(triggered()), this ,SLOT(setulawencg711slot()));
    getmodeencg711 = new QAction(tr("Get mode G711"), this);
    connect(getmodeencg711,SIGNAL(triggered()), this ,SLOT(getmodeencg711slot()));
    set20msencilbc = new QAction(tr("Set 20ms fr iLBC"), this);
    connect(set20msencilbc,SIGNAL(triggered()), this ,SLOT(set20msencilbcslot()));
    set30msencilbc = new QAction(tr("Set 30ms fr iLBC"), this);
    connect(set30msencilbc,SIGNAL(triggered()), this ,SLOT(set30msencilbcslot()));
    getmodeencilbc = new QAction(tr("Get mode iLBC"), this);
    connect(getmodeencilbc,SIGNAL(triggered()), this ,SLOT(getmodeencilbcslot()));

    //DTMF actions
    dtmfdninit = new QAction(tr("Init DTMF Dnlink"), this);
    connect(dtmfdninit, SIGNAL(triggered()), this, SLOT(dtmfdninitslot()));
    dtmfdnplay = new QAction(tr("Play DTMF Dnlink"), this);
    connect(dtmfdnplay, SIGNAL(triggered()), this, SLOT(dtmfdnplayslot()));
    dtmfdnstop = new QAction(tr("Stop DTMF Dnlink"), this);
    connect(dtmfdnstop, SIGNAL(triggered()), this, SLOT(dtmfdnstopslot()));
    dtmfupinit = new QAction(tr("Init DTMF Uplink"), this);
    connect(dtmfupinit, SIGNAL(triggered()), this, SLOT(dtmfupinitslot()));
    dtmfupplay = new QAction(tr("Play DTMF Uplink"), this);
    connect(dtmfupplay, SIGNAL(triggered()), this, SLOT(dtmfupplayslot()));
    dtmfupstop = new QAction(tr("Stop DTMF Uplink"), this);
    connect(dtmfupstop, SIGNAL(triggered()), this, SLOT(dtmfupstopslot()));

    //Inband tones actions
    inbandbusy = new QAction(tr("Busy"), this);
    connect(inbandbusy,SIGNAL(triggered()), this ,SLOT(inbandbusyslot()));
    inbandpathnotaval = new QAction(tr("RadioPath Not Available"), this);
    connect(inbandpathnotaval,SIGNAL(triggered()), this ,SLOT(inbandpathnotavilslot()));
    inbandcongestion = new QAction(tr("Congestion"), this);
    connect(inbandcongestion,SIGNAL(triggered()), this ,SLOT(inbandcongestionslot()));
    inbandspecialinfo = new QAction(tr("Special Info"), this);
    connect(inbandspecialinfo,SIGNAL(triggered()), this ,SLOT(inbandspecialinfoslot()));
    inbandreorder = new QAction(tr("Reorder"), this);
    connect(inbandreorder,SIGNAL(triggered()), this ,SLOT(inbandreorderslot()));
    inbandremotealert = new QAction(tr("Remote alert"), this);
    connect(inbandremotealert,SIGNAL(triggered()), this ,SLOT(inbandremotealertslot()));
    inbandcallwait = new QAction(tr("Call waiting"), this);
    connect(inbandcallwait,SIGNAL(triggered()), this ,SLOT(inbandcallwaitslot()));
    inbanddatacall = new QAction(tr("Data Call"), this);
    connect(inbanddatacall,SIGNAL(triggered()), this ,SLOT(inbanddatacallslot()));
    inbandnoseq = new QAction(tr("No sequence"), this);
    connect(inbandnoseq,SIGNAL(triggered()), this ,SLOT(inbandnoseqslot()));
    inbandbeepseq = new QAction(tr("Beep seq"), this);
    connect(inbandbeepseq,SIGNAL(triggered()), this ,SLOT(inbandbeepseqslot()));
    inbandstop = new QAction(tr("Stop"), this);
    connect(inbandstop,SIGNAL(triggered()), this ,SLOT(inbandstopslot()));

    //Ring tone actions
    rtplay = new QAction(tr("Play"), this);
    connect(rtplay, SIGNAL(triggered()), this, SLOT(rtplayslot()));
    rtpause = new QAction(tr("Pause"), this);
    connect(rtpause, SIGNAL(triggered()), this, SLOT(rtpauseslot()));
    rtmute = new QAction(tr("Mute"), this);
    connect(rtmute, SIGNAL(triggered()), this, SLOT(rtmuteslot()));
    rtstop = new QAction(tr("Stop"), this);
    connect(rtstop, SIGNAL(triggered()), this, SLOT(rtstopslot()));
    rtdeinit = new QAction(tr("Deinit"), this);
    connect(rtdeinit, SIGNAL(triggered()), this, SLOT(rtdeinitslot()));
    rtprofile = new QAction(tr("Current Profile"), this);
    connect(rtprofile, SIGNAL(triggered()), this, SLOT(rtprofileslot()));
    rtaudiofile = new QAction(tr("Audio File"), this);
    connect(rtaudiofile, SIGNAL(triggered()), this, SLOT(rtfileslot()));
    rttts = new QAction(tr("Profile with TTS"), this);
    connect(rttts, SIGNAL(triggered()), this, SLOT(rtttsslot()));
    rtsequence = new QAction(tr("Sequence"), this);
    connect(rtsequence, SIGNAL(triggered()), this, SLOT(rtsequenceslot()));
    rtbeep = new QAction(tr("Beep"), this);
    connect(rtbeep, SIGNAL(triggered()), this, SLOT(rtbeepslot()));
    rtsilent = new QAction(tr("Silent"), this);
    connect(rtsilent, SIGNAL(triggered()), this, SLOT(rtsilentslot()));

    onetouch = new QAction(tr("One Touch Loopback"),this);
    connect(onetouch,SIGNAL(triggered()), this ,SLOT(onetouchslot()));

    exitAction = new QAction(tr("&Exit"), this);
    connect(exitAction, SIGNAL(triggered()), this, SLOT(quit()));
    }

void TmsTestApp::createMenus()
    {
    enginemenu = menuBar()->addMenu("Select Engine");
    enginemenu->addAction(qtmsengine);
    enginemenu->addAction(tmsengine);

    //Call type menu options
    QMenu* calltypemenu;
    calltypemenu = menuBar()->addMenu("CallType");
    calltypemenu->addAction(cscall);
    calltypemenu->addAction(ipcall);

    //Stream type menu options
    QMenu* strmdnmenu;
    strmdnmenu = menuBar()->addMenu("Downlink Stream");

    strmdnmenu->addAction(downlinkformats);

    QMenu* decodermenu;
    decodermenu = strmdnmenu->addMenu("Init Decoder...");

    decodermenu->addAction(pcmdec);
    decodermenu->addAction(amrdec);
    decodermenu->addAction(g711dec);
    decodermenu->addAction(g729dec);
    decodermenu->addAction(ilbcdec);

    strmdnmenu->addAction(strmdnstart);
    strmdnmenu->addAction(strmdnstop);
    strmdnmenu->addAction(strmdnclose);

    QMenu* speakermenu;
    speakermenu = strmdnmenu->addMenu("Speaker Settings");
    speakermenu->addAction(stmvolact);
    speakermenu->addAction(stmmaxvolact);
    speakermenu->addAction(stmsetvolact);
    speakermenu->addAction(stmmutevolact);
    QMenu* routingmenu;
    routingmenu = strmdnmenu->addMenu("Output device");
    routingmenu->addAction(availoutputact);
    routingmenu->addAction(currentoutputact);
    routingmenu->addAction(handsetact);
    routingmenu->addAction(loudspkract);
    routingmenu->addAction(wiredact);
    routingmenu->addAction(btact);
    routingmenu->addAction(ttyact);

    QMenu* decodersettings;
    decodersettings = strmdnmenu->addMenu("Decoder Settings");
    decodersettings->addAction(togglecng);
    decodersettings->addAction(getcng);
    decodersettings->addAction(toggleplc);
    decodersettings->addAction(getplc);
    decodersettings->addAction(setalawdecg711);
    decodersettings->addAction(setulawdecg711);
    decodersettings->addAction(getmodedecg711);
    decodersettings->addAction(set20msdecilbc);
    decodersettings->addAction(set30msdecilbc);
    decodersettings->addAction(getmodedecilbc);

    QMenu* strmupmenu;
    strmupmenu = menuBar()->addMenu("Uplink Stream");

    strmupmenu->addAction(uplinkformats);
    QMenu* encodermenu;
    encodermenu = strmupmenu->addMenu("Init Encoder...");

    strmupmenu->addAction(strmupstart);
    strmupmenu->addAction(strmupstop);
    strmupmenu->addAction(strmupclose);

    encodermenu->addAction(pcmenc);
    encodermenu->addAction(amrenc);
    encodermenu->addAction(g711enc);
    encodermenu->addAction(g729enc);
    encodermenu->addAction(ilbcenc);

    QMenu* micmenu;
    micmenu = strmupmenu->addMenu("Mic Settings");
    micmenu->addAction(stmgainact);
    micmenu->addAction(stmmaxgainact);
    micmenu->addAction(stmsetgainact);
    micmenu->addAction(stmmutegainact);

    QMenu* encodersettings;
    encodersettings = strmupmenu->addMenu("Encoder Settings");
    encodersettings->addAction(getsupportedbr);
    encodersettings->addAction(setminbr);
    encodersettings->addAction(setmaxbr);
    encodersettings->addAction(getbr);
    encodersettings->addAction(togglevad);
    encodersettings->addAction(getvad);
    encodersettings->addAction(setalawencg711);
    encodersettings->addAction(setulawencg711);
    encodersettings->addAction(getmodeencg711);
    encodersettings->addAction(set20msencilbc);
    encodersettings->addAction(set30msencilbc);
    encodersettings->addAction(getmodeencilbc);

    //DTMF tone player options
    QMenu* dtmfmenu;
    dtmfmenu =  menuBar()->addMenu("DTMF Tone player");
    dtmfmenu->addAction(dtmfdninit);
    dtmfmenu->addAction(dtmfdnplay);
    dtmfmenu->addAction(dtmfdnstop);
    dtmfmenu->addAction(dtmfupinit);
    dtmfmenu->addAction(dtmfupplay);
    dtmfmenu->addAction(dtmfupstop);

    //Inband tone options
    QMenu* inbandmenu;
    inbandmenu = menuBar()->addMenu("Inband Tones");
    inbandmenu->addAction(inbandbusy);
    inbandmenu->addAction(inbandpathnotaval);
    inbandmenu->addAction(inbandcongestion);
    inbandmenu->addAction(inbandspecialinfo);
    inbandmenu->addAction(inbandreorder);
    inbandmenu->addAction(inbandremotealert);
    inbandmenu->addAction(inbandcallwait);
    inbandmenu->addAction(inbanddatacall);
    inbandmenu->addAction(inbandnoseq);
    inbandmenu->addAction(inbandbeepseq);
    inbandmenu->addAction(inbandstop);

    QMenu* globalffectsmenu;
    globalffectsmenu = menuBar()->addMenu("Global Effects");
    globalffectsmenu->addAction(globalvolact);
    globalffectsmenu->addAction(globalmaxvolact);
    globalffectsmenu->addAction(globalsetvolact);
    globalffectsmenu->addAction(globalgainact);
    globalffectsmenu->addAction(globalmaxgainact);
    globalffectsmenu->addAction(globalsetgainact);


    QMenu* rtmenu;
    rtmenu = menuBar()->addMenu("RingTone Player");
    QMenu* rtmenuinit;
    rtmenuinit = rtmenu->addMenu("Init Player");
    rtmenuinit->addAction(rtprofile);
    rtmenuinit->addAction(rtaudiofile);
    rtmenuinit->addAction(rttts);
    rtmenuinit->addAction(rtbeep);
    rtmenuinit->addAction(rtsilent);
    rtmenu->addAction(rtplay);
    rtmenu->addAction(rtmute);
    rtmenu->addAction(rtpause);
    rtmenu->addAction(rtstop);
    rtmenu->addAction(rtdeinit);

    //One Touch Loopback
    menuBar()->addAction(onetouch);

    menuBar()->addAction(exitAction);
    }

void TmsTestApp::qtmsengineslot()
    {
   	statusDisplay->append("Using QT TMS api");
   	iTmsEngine = QTmsTestEngine::NewL(statusDisplay);

    //After an engine has been selected hide option
    enginemenu->menuAction()->setVisible(false);
    }

void TmsTestApp::tmsengineslot()
    {
    statusDisplay->append("Using openC TMS api");
    iTmsEngine = CTmsTestEngine::NewL(statusDisplay);

    //After an engine has been selected hide option
    enginemenu->menuAction()->setVisible(false);
    }

void TmsTestApp::cscallslot()
    {
    CHECKENGINE
    iTmsEngine->SetCallType(TMS_CALL_CS);
    }
void TmsTestApp::ipcallslot()
    {
    CHECKENGINE
    iTmsEngine->SetCallType(TMS_CALL_IP);
    }
void TmsTestApp::downlinkformatsslot()
    {
    CHECKENGINE
    iTmsEngine->GetSupportedDownlinkFormats(ETrue);
    }
void TmsTestApp::strmdnstartslot()
    {
    CHECKENGINE
    iTmsEngine->StartDownlink();
    }
void TmsTestApp::strmdnstopslot()
    {
    CHECKENGINE
    iTmsEngine->StopDownlink();
    }
void TmsTestApp::strmdncloseslot()
    {
    CHECKENGINE
    iTmsEngine->CloseDownlink();
    }
void TmsTestApp::uplinkformatsslot()
    {
    CHECKENGINE
    iTmsEngine->GetSupportedUplinkFormats(ETrue);
    }
void TmsTestApp::strmupstartslot()
    {
    CHECKENGINE
    iTmsEngine->StartUplink();
    }
void TmsTestApp::strmupstopslot()
    {
    CHECKENGINE
    iTmsEngine->StopUplink();
    }
void TmsTestApp::strmupcloseslot()
    {
    CHECKENGINE
    iTmsEngine->CloseUplink();
    }
void TmsTestApp::dtmfdninitslot()
    {
    CHECKENGINE
    iTmsEngine->InitDTMFTonePlayerDnlink();
    }
void TmsTestApp::dtmfdnplayslot()
    {
    CHECKENGINE
    iTmsEngine->DTMFTonePlayDnlink();
    }
void TmsTestApp::dtmfdnstopslot()
    {
    CHECKENGINE
    iTmsEngine->CloseDTMFPlayerDnlink();
    }
void TmsTestApp::dtmfupinitslot()
    {
    CHECKENGINE
    iTmsEngine->InitDTMFTonePlayerUplink();
    }
void TmsTestApp::dtmfupplayslot()
    {
    CHECKENGINE
    iTmsEngine->DTMFTonePlayUplink();
    }
void TmsTestApp::dtmfupstopslot()
    {
    CHECKENGINE
    iTmsEngine->CloseDTMFPlayerUplink();
    }
void TmsTestApp::inbandbusyslot()
    {
    CHECKENGINE
    iTmsEngine->StartInbandTone(TMS_INBAND_USER_BUSY);
    }
 void TmsTestApp::inbandpathnotavilslot()
     {
     CHECKENGINE
     iTmsEngine->StartInbandTone(TMS_INBAND_RADIO_PATH_NOT_AVAIL);
     }
 void TmsTestApp::inbandcongestionslot()
     {
     CHECKENGINE
     iTmsEngine->StartInbandTone(TMS_INBAND_CONGESTION);
     }
 void TmsTestApp::inbandspecialinfoslot()
     {
     CHECKENGINE
     iTmsEngine->StartInbandTone(TMS_INBAND_SPECIAL_INFO);
     }
 void TmsTestApp::inbandremotealertslot()
    {
     CHECKENGINE
     iTmsEngine->StartInbandTone(TMS_INBAND_REMOTE_ALEARTING);
    }
void TmsTestApp::inbanddatacallslot()
    {
    CHECKENGINE
    iTmsEngine->StartInbandTone(TMS_INBAND_DATA_CALL);
    }
void TmsTestApp::inbandcallwaitslot()
    {
    CHECKENGINE
    iTmsEngine->StartInbandTone(TMS_INBAND_CALL_WAITING);
    }
void TmsTestApp::inbandreorderslot()
    {
    CHECKENGINE
    iTmsEngine->StartInbandTone(TMS_INBAND_REORDER);
    }
void TmsTestApp::inbandnoseqslot()
    {
    CHECKENGINE
    iTmsEngine->StartInbandTone(TMS_INBAND_NO_SEQUENCE);
    }
void TmsTestApp::inbandbeepseqslot()
    {
    CHECKENGINE
    iTmsEngine->StartInbandTone(TMS_INBAND_BEEP_SEQUENCE);
    }
void TmsTestApp::inbandstopslot()
    {
    CHECKENGINE
    iTmsEngine->StopInbandTone();
    }
void TmsTestApp::globalvolslot()
    {
    CHECKENGINE
    iTmsEngine->GetGlobalVol();
    }
void TmsTestApp::globalmaxvolslot()
    {
    CHECKENGINE
    iTmsEngine->GetGlobalMaxVol();
    }
void TmsTestApp::globalsetvolslot()
    {
    CHECKENGINE
    iTmsEngine->SetGlobalVol();
    }
void TmsTestApp::globalgainslot()
    {
    CHECKENGINE
    iTmsEngine->GetGlobalGain();
    }
void TmsTestApp::globalmaxgainslot()
    {
    CHECKENGINE
    iTmsEngine->GetGlobalMaxGain();
    }
void TmsTestApp::globalsetgainslot()
    {
    CHECKENGINE
    iTmsEngine->SetGlobalGain();
    }
void TmsTestApp::stmgainslot()
    {
    CHECKENGINE
    iTmsEngine->GetGain();
    }
void TmsTestApp::stmmaxgainslot()
    {
    CHECKENGINE
    iTmsEngine->GetMaxGain();
    }
void TmsTestApp::stmsetgainslot()
    {
    CHECKENGINE
    iTmsEngine->SetMaxGain();
    }
void TmsTestApp::stmmutegainslot()
    {
    CHECKENGINE
    iTmsEngine->MuteMic();
    }
void TmsTestApp::stmvolslot()
    {
    CHECKENGINE
    iTmsEngine->GetVolume();
    }
void TmsTestApp::stmmaxvolslot()
    {
    CHECKENGINE
    iTmsEngine->GetMaxVolume();
    }
void TmsTestApp::stmsetvolslot()
    {
    CHECKENGINE
    iTmsEngine->SetMaxVolume();
    }
void TmsTestApp::stmmutevolslot()
    {
    CHECKENGINE
    iTmsEngine->MuteSpeaker();
    }
void TmsTestApp::handsetslot()
    {
    CHECKENGINE
    iTmsEngine->SetOutputDevice(TMS_AUDIO_OUTPUT_HANDSET);
    }
void TmsTestApp::loudspkrslot()
    {
    CHECKENGINE
    iTmsEngine->SetOutputDevice(TMS_AUDIO_OUTPUT_LOUDSPEAKER);
    }
void TmsTestApp::wiredslot()
    {
    CHECKENGINE
    iTmsEngine->SetOutputDevice(TMS_AUDIO_OUTPUT_WIRED_ACCESSORY);
    }
void TmsTestApp::btslot()
    {
    CHECKENGINE
    iTmsEngine->SetOutputDevice(TMS_AUDIO_OUTPUT_ACCESSORY);
    }
void TmsTestApp::ttyslot()
    {
    CHECKENGINE
    iTmsEngine->SetOutputDevice(TMS_AUDIO_OUTPUT_ETTY);
    }
void TmsTestApp::currentoutputslot()
    {
    CHECKENGINE
    iTmsEngine->GetAudioDevice();
    }
void TmsTestApp::availoutputslot()
    {
    CHECKENGINE
    iTmsEngine->GetAvailableOutput();
    }
void TmsTestApp::pcmdecslot()
    {
    CHECKENGINE
    iTmsEngine->SetDownlinkFormat(TMS_FORMAT_PCM);
    }

void TmsTestApp::amrdecslot()
    {
    CHECKENGINE
    iTmsEngine->SetDownlinkFormat(TMS_FORMAT_AMR);
    }
void TmsTestApp::g711decslot()
    {
    CHECKENGINE
    iTmsEngine->SetDownlinkFormat(TMS_FORMAT_G711);
    }
void TmsTestApp::g729decslot()
    {
    CHECKENGINE
    iTmsEngine->SetDownlinkFormat(TMS_FORMAT_G729);
    }
void TmsTestApp::ilbcdecslot()
    {
    CHECKENGINE
    iTmsEngine->SetDownlinkFormat(TMS_FORMAT_ILBC);
    }
void TmsTestApp::pcmencslot()
    {
    CHECKENGINE
    iTmsEngine->SetUplinkFormat(TMS_FORMAT_PCM);
    }
void TmsTestApp::amrencslot()
    {
    CHECKENGINE
    iTmsEngine->SetUplinkFormat(TMS_FORMAT_AMR);
    }
void TmsTestApp::g711encslot()
    {
    CHECKENGINE
    iTmsEngine->SetUplinkFormat(TMS_FORMAT_G711);
    }
void TmsTestApp::g729encslot()
    {
    CHECKENGINE
    iTmsEngine->SetUplinkFormat(TMS_FORMAT_G729);
    }
void TmsTestApp::ilbcencslot()
    {
    CHECKENGINE
    iTmsEngine->SetUplinkFormat(TMS_FORMAT_ILBC);
    }
void TmsTestApp::togglecngslot()
    {
    CHECKENGINE
    iTmsEngine->ToggleCng();
    }
void TmsTestApp::getcngslot()
    {
    CHECKENGINE
    iTmsEngine->GetCng();
    }
void TmsTestApp::toggleplcslot()
    {
    CHECKENGINE
    iTmsEngine->TogglePlc();
    }
void TmsTestApp::getplcslot()
    {
    CHECKENGINE
    iTmsEngine->GetPlc();
    }
void TmsTestApp::setalawdecg711slot()
    {
    CHECKENGINE
    iTmsEngine->SetDnLinkG711ALAW();
    }
void TmsTestApp::setulawdecg711slot()
    {
    CHECKENGINE
    iTmsEngine->SetDnLinkG711uLAW();
    }
void TmsTestApp::getmodegdec711slot()
    {
    CHECKENGINE
    iTmsEngine->GetDnLinkG711Mode();
    }
void TmsTestApp::set20msdecilbcslot()
    {
    CHECKENGINE
    iTmsEngine->SetDnLinkILBC20MS();
    }
void TmsTestApp::set30msdecilbcslot()
    {
    CHECKENGINE
    iTmsEngine->SetDnLinkILBC30MS();
    }
void TmsTestApp::getmodedecilbcslot()
    {
    CHECKENGINE
    iTmsEngine->GetDnLinkILBCMode();
    }
void TmsTestApp::getsupportedbrslot()
    {
    CHECKENGINE
    iTmsEngine->GetSupportedBitrates();
    }
void TmsTestApp::setminbrslot()
    {
    CHECKENGINE
    iTmsEngine->SelectMinBitrate();
    }
void TmsTestApp::setmaxbrslot()
    {
    CHECKENGINE
    iTmsEngine->SelectMaxBitrate();
    }
void TmsTestApp::getbrslot()
    {
    CHECKENGINE
    iTmsEngine->GetBitrate();
    }
void TmsTestApp::togglevadslot()
    {
    CHECKENGINE
    iTmsEngine->ToggleVad();
    }
void TmsTestApp::getvadslot()
    {
    CHECKENGINE
    iTmsEngine->GetVad();
    }
void TmsTestApp::setalawencg711slot()
    {
    CHECKENGINE
    iTmsEngine->SetUpLinkG711ALAW();
    }
void TmsTestApp::setulawencg711slot()
    {
    CHECKENGINE
    iTmsEngine->SetUpLinkG711uLAW();
    }
void TmsTestApp::getmodeencg711slot()
    {
    CHECKENGINE
    iTmsEngine->GetUpLinkG711Mode();
    }
void TmsTestApp::set20msencilbcslot()
    {
    CHECKENGINE
    iTmsEngine->SetUpLinkILBC20MS();
    }
void TmsTestApp::set30msencilbcslot()
    {
    CHECKENGINE
    iTmsEngine->SetUpLinkILBC30MS();
    }
void TmsTestApp::getmodeencilbcslot()
    {
    CHECKENGINE
    iTmsEngine->GetUpLinkILBCMode();
    }
void TmsTestApp::rtplayslot()
    {
    CHECKENGINE
    iTmsEngine->PlayRingTone();
    }
void TmsTestApp::rtpauseslot()
    {
    CHECKENGINE
    iTmsEngine->PauseVideoRingTone();
    }
void TmsTestApp::rtmuteslot()
    {
    CHECKENGINE
    iTmsEngine->MuteRingTone();
    }
void TmsTestApp::rtstopslot()
    {
    CHECKENGINE
    iTmsEngine->StopRingTone();
    }
void TmsTestApp::rtdeinitslot()
    {
    CHECKENGINE
    iTmsEngine->DeinitRingTonePlayer();
    }
void TmsTestApp::rtprofileslot()
    {
    CHECKENGINE
    iTmsEngine->InitRingTonePlayerFromProfiles();
    }
void TmsTestApp::rtfileslot()
    {
    CHECKENGINE
    iTmsEngine->InitRingTonePlayerFromFile();
    }
void TmsTestApp::rtttsslot()
    {
    CHECKENGINE
    iTmsEngine->InitRingToneWithTTS();
    }
void TmsTestApp::rtbeepslot()
    {
    CHECKENGINE
    iTmsEngine->InitRingToneBeepOnce();
    }
void TmsTestApp::rtsequenceslot()
    {
    CHECKENGINE
    iTmsEngine->InitRingToneSequencePlayer();
    }
void TmsTestApp::rtsilentslot()
    {
    CHECKENGINE
    iTmsEngine->InitRingToneSilent();
    }
void TmsTestApp::onetouchslot()
    {
    CHECKENGINE
    iTmsEngine->OneTouchLoopback();
    }

#include "../tmp/moc_qttmstestapp.cpp"


