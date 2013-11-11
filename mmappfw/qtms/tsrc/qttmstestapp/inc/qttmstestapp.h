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
 * Description: Telephony Multimedia Service - TestApp
 *
 */

#ifndef QTTMSTESTAPP_H
#define QTTMSTESTAPP_H

#include <QObject>
#include <QWidget>
#include <QtGui/QMainWindow>
#include <QtCore>
#include <QtGui>
#include <QTextEdit>
#include <QAction>
#include <QMenu>
#include <QMenubar>
//#include "mtmstestengine.h"

// Forward declaration
class MTmsTestEngine;
//
// Class declaration
//
class TmsTestApp : public QMainWindow
    {
    Q_OBJECT
public:
    TmsTestApp(QWidget *parent = 0);

    ~TmsTestApp();

protected:

private slots:

    void qtmsengineslot();
    void tmsengineslot();

    void cscallslot();
    void ipcallslot();

    void strmdnstartslot();
    void strmdnstopslot();
    void strmdncloseslot();
    void strmupstartslot();
    void strmupstopslot();
    void strmupcloseslot();


    void globalvolslot();
    void globalmaxvolslot();
    void globalsetvolslot();

    void globalgainslot();
    void globalmaxgainslot();
    void globalsetgainslot();

    void stmgainslot();
    void stmmaxgainslot();
    void stmsetgainslot();
    void stmmutegainslot();

    void stmvolslot();
    void stmmaxvolslot();
    void stmsetvolslot();
    void stmmutevolslot();

    void handsetslot();
    void loudspkrslot();
    void wiredslot();
    void btslot();
    void ttyslot();
    void currentoutputslot();
    void availoutputslot();

    void dtmfdninitslot();
    void dtmfdnplayslot();
    void dtmfdnstopslot();
    void dtmfupinitslot();
    void dtmfupplayslot();
    void dtmfupstopslot();

    void inbandbusyslot();
    void inbandpathnotavilslot();
    void inbandcongestionslot();
    void inbandspecialinfoslot();
    void inbandremotealertslot();
    void inbanddatacallslot();
    void inbandcallwaitslot();
    void inbandreorderslot();
    void inbandnoseqslot();
    void inbandbeepseqslot();
    void inbandstopslot();

    void downlinkformatsslot();
    void pcmdecslot();
    void amrdecslot();
    void g711decslot();
    void g729decslot();
    void ilbcdecslot();

    void uplinkformatsslot();
    void pcmencslot();
    void amrencslot();
    void g711encslot();
    void g729encslot();
    void ilbcencslot();


    void togglecngslot();
    void getcngslot();
    void toggleplcslot();
    void getplcslot();
    void setalawdecg711slot();
    void setulawdecg711slot();
    void getmodegdec711slot();
    void set20msdecilbcslot();
    void set30msdecilbcslot();
    void getmodedecilbcslot();

     //Encoder settings

    void getsupportedbrslot();
    void setminbrslot();
    void setmaxbrslot();
    void getbrslot();
    void togglevadslot();
    void getvadslot();
    void setalawencg711slot();
    void setulawencg711slot();
    void getmodeencg711slot();
    void set20msencilbcslot();
    void set30msencilbcslot();
    void getmodeencilbcslot();


    void rtplayslot();
    void rtpauseslot();
    void rtmuteslot();
    void rtstopslot();
    void rtdeinitslot();
    void rtprofileslot();
    void rtttsslot();
    void rtfileslot();
    void rtbeepslot();
    void rtsequenceslot();
    void rtsilentslot();

    void onetouchslot();

private:
    void createActions();
    void createMenus();

private:
    QWidget *mainContainer;
    QMenu *audioMenu;

    QAction *qtmsengine;
    QAction *tmsengine;
    QMenu* enginemenu;

    QAction *cscall;
    QAction *ipcall;
    QAction *exitAction;

    QAction *dtmfdninit;
    QAction *dtmfdnplay;
    QAction *dtmfdnstop;

    QAction *dtmfupinit;
    QAction *dtmfupplay;
    QAction *dtmfupstop;

    QAction *inbandbusy;
    QAction *inbandpathnotaval;
    QAction *inbandcongestion;
    QAction *inbandspecialinfo;
    QAction *inbandreorder;
    QAction *inbandremotealert;
    QAction *inbandcallwait;
    QAction *inbanddatacall;
    QAction *inbandnoseq;
    QAction *inbandbeepseq;
    QAction *inbandstop;

    QAction *downlinkformats;
    QAction *initdecoder;
    QAction *strmdnstart;
    QAction *strmdnstop;
    QAction *strmdnclose;
    QAction *pcmdec;
    QAction *amrdec;
    QAction *g711dec;
    QAction *g729dec;
    QAction *ilbcdec;

    QAction *uplinkformats;
    QAction *initencoder;
    QAction *strmupstart;
    QAction *strmupstop;
    QAction *strmupclose;

    QAction *pcmenc;
    QAction *amrenc;
    QAction *g711enc;
    QAction *g729enc;
    QAction *ilbcenc;

    QAction *availoutputact;
    QAction *currentoutputact;
    QAction *handsetact;
    QAction *loudspkract;
    QAction *wiredact;
    QAction *btact;
    QAction *ttyact;

    QAction *globalvolact;
    QAction *globalmaxvolact;
    QAction *globalsetvolact;

    QAction *globalgainact;
    QAction *globalmaxgainact;
    QAction *globalsetgainact;

    QAction *stmgainact;
    QAction *stmmaxgainact;
    QAction *stmsetgainact;
    QAction *stmmutegainact;

    QAction *stmvolact;
    QAction *stmmaxvolact;
    QAction *stmsetvolact;
    QAction *stmmutevolact;

    QAction *togglecng;
    QAction *getcng;
    QAction *toggleplc;
    QAction *getplc;
    QAction *setalawdecg711;
    QAction *setulawdecg711;
    QAction *getmodedecg711;
    QAction *set20msdecilbc;
    QAction *set30msdecilbc;
    QAction *getmodedecilbc;

    QAction *getsupportedbr;
    QAction *setminbr;
    QAction *setmaxbr;
    QAction *getbr;
    QAction *togglevad;
    QAction *getvad;
    QAction *setalawencg711;
    QAction *setulawencg711;
    QAction *getmodeencg711;
    QAction *set20msencilbc;
    QAction *set30msencilbc;
    QAction * getmodeencilbc;

    QAction *rtplay;
    QAction *rtpause;
    QAction *rtmute;
    QAction *rtstop;
    QAction *rtdeinit;
    QAction *rtprofile;
    QAction *rttts;
    QAction *rtaudiofile;
    QAction *rtbeep;
    QAction *rtsequence;
    QAction *rtsilent;

    QAction *onetouch;

    QTextEdit *statusDisplay;

    //TMS members
    MTmsTestEngine *iTmsEngine;

    };
#endif
