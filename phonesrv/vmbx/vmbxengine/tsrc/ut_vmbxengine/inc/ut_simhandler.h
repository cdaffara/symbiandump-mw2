#ifndef UT_SIMHANDLER_H
#define UT_SIMHANDLER_H

#include <QObject>

class CVmbxSimHandler;

class Ut_SimHandler : public QObject
{
    Q_OBJECT
    
public:
    Ut_SimHandler();
    virtual ~Ut_SimHandler();
    
private slots:
    void testCreateAndDelete();
    //void testGetL();
    //void testSave();
    void testIsWritable();
    void testNotifyStoreEvent();        
    void testPhoneBookType();
    void testCancelAsyncRequest();
    
private:
    void createSim();
    void deleteSim();
    
private:
    CVmbxSimHandler *mSim;
};

#endif // UT_SIMHANDLER_H
