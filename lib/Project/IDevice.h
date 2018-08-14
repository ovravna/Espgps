#ifndef IDevice_H
#define IDevice_H


class IDevice {


public:

    virtual bool begin() = 0; 

    virtual bool handle() = 0; 
};


#endif /* IDevice_H */