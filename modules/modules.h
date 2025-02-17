#include "../main.h"
#include <vector>


class Module {
    public:
    std::string name;
    int key_code;
    bool enabled;
    virtual void init(){}
    virtual void toggle(){}
    virtual void onEnable(){}
    virtual void onDisable(){}
    Module(std::string name, int key){
        this->name = name;
        key_code = key;
    }
};


extern std::vector<Module*> mods;


class Fly : public Module{
    public:

    void init();
    void toggle();
    void onEnable();
    void onDisable();
    void onClientTick();
    void on_key_press();
    Fly(std::string name, int key) : Module(name, key){
        init();
    }
};





class Fastplace : public Module{
    public:

    int max_delay = 2;
    
    void init();
    void toggle();
    void onEnable();
    void onDisable();
    void onClientTick();
    void on_key_press();

    Fastplace(std::string name, int key) : Module(name, key){
        init();
    }
};


class Eagle : public  Module{
    public:

    void init();
    void toggle();
    void onEnable();
    void onDisable();
    void onClientTick();
    void on_key_press();

    Eagle(std::string name, int key) : Module(name,key){
        init();
    }

};




class Velocity : public  Module{
    public:

    double x;
    double y;
    double z;
    int hurtTimeDelay = 0;

    void init();
    void toggle();
    void onEnable();
    void onDisable();
    void onClientTick();
    void on_key_press();

    Velocity(std::string name, int key) : Module(name,key){
        init();
    }

};
