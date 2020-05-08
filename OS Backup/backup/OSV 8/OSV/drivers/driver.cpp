#include "driver.h"

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
    for(int i = 0; i < numDrivers; i++){
        drivers[i]->Activate();
    }
    
    
}
