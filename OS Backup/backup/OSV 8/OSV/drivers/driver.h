#ifndef __DRIVER_H
#define __DRIVER_H

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
        Driver* drivers[265];
        int numDrivers;
    };


#endif
