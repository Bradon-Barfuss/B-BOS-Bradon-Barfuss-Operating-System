#ifndef __OS__DRIVERS__DRIVER_H
#define __OS__DRIVERS__DRIVER_H
namespace OS{
    namespace drivers{

        
        
class Driver{
public:
    Driver();
    ~Driver();
    
    virtual void Activate();
    virtual void Deactivate();
    virtual int Reset();//everytime you boot up the OS you need to reset the drivers.
    //The reset will return an int, the int is the time to wait for it to reset
};



class DriverManager{
public:
    DriverManager();
    void AddDrivers(Driver*);
    void ActivateAll();

private:
    Driver* drivers[255];
    int numDrivers;
};
    

        
    }
}


#endif
