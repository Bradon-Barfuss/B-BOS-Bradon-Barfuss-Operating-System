#include <drivers/driver.h>

using namespace OS::drivers;

Driver::Driver(){
    
}

Driver::~Driver(){
    
}

void Driver::Activate(){
    
}

void Driver::Deactivate(){
    
}


int Driver::Reset(){
    return 0;
}

DriverManager::DriverManager(){
    numDrivers = 0;
}

void DriverManager::AddDrivers(Driver* drv){
    drivers[numDrivers] = drv;
    numDrivers++;
}

void DriverManager::ActivateAll(){
    for(int i = 0; i < 2; i++){
        drivers[i]->Activate();
    }
    
    
}
