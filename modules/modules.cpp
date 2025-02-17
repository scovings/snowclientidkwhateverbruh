#include "modules.h"
#include "../functions/functions.h"

std::vector<Module*> mods;

void Fly::init() {
    enabled = false;
    subscribe("on-tick", &Fly::onClientTick, this);  // Subscribe to "on-tick"
    subscribe("96", &Fly::on_key_press, this); // Subscribe to "Key-press"
}

void Fly::toggle() {
    enabled = !enabled;
    if (enabled) {
        printf("[M] Fly Mod Enabled!\n");
        onEnable();
    } else {
        printf("[M] Fly Mod Disabled!\n");
        onDisable();
    }
}

void Fly::onEnable() {
    // Handle enabling fly
}

void Fly::onDisable() {
    // Handle disabling fly
}

void Fly::onClientTick() {
    if (enabled) {
        mc_thePlayer_jump(env);

    }
}

void Fly::on_key_press() {
    toggle();
    
    }




void Fastplace::init(){
    enabled = false;
    subscribe("on-tick", &Fastplace::onClientTick, this);  // Subscribe to "on-tick"
    subscribe("71", &Fastplace::on_key_press, this); // Subscribe to "Key-press"
}
void Fastplace::toggle(){
        enabled = !enabled;
    if (enabled) {
        onEnable();
    } else {
        onDisable();
    }
    }
void Fastplace::onEnable(){
    printf("[M] FastPlace Enabled!\n");
    }
void Fastplace::onDisable(){
    printf("[M] FastPlace Disabled-+!\n");

    }
void Fastplace::onClientTick(){
    if(enabled){
        set_right_click_delay(env, max_delay);// randomization later
  

    }
}
void Fastplace::on_key_press(){
    toggle();
}




void Eagle::init(){
    enabled = false;
    subscribe("on-tick", &Eagle::onClientTick, this);  // Subscribe to "on-tick"
    subscribe("80", &Eagle::on_key_press, this); // Subscribe to "Key-press"

}

void Eagle::toggle(){
            enabled = !enabled;
    if (enabled) {
        onEnable();
    } else {
        onDisable();
    }
}

void Eagle::onEnable(){
    printf("[M] Eagle Hack Enabled!\n");
}
void Eagle::onDisable(){
    printf("[M] Eagle Hack Disabled!\n");
}
void Eagle::onClientTick(){
    if(enabled){
        //get player position
        //get block under player
        //is block air?
        //if so, sneak
        //else: unsneak

        //additional logic for you to implement:
        //dont get in the way of the user:
        //if not on ground, dont sneak
        //if the shift key is pressed on the keyboard, dont unsneak
        //if sprinting, the user probably plans on jumping or falling, so dont prevent this by sneaking

        jobject thePlayer = get_player(env);
        double x = getX(env, thePlayer);
        double y = getY(env, thePlayer);
        double z = getZ(env, thePlayer);

        //adjust for negative coords
        if(x < 0){
		    x--;
        }
        if(z < 0){
            z--;
        }

        int block = get_block(env, x, y-0.49, z);
        std::cout << block << std::endl;
        if(block == 0){
            //sneak
            set_sneaking(env, true);
        }else{
            //dont sneak
            set_sneaking(env, false);
        }
        env->DeleteLocalRef(thePlayer);
    }
}
void Eagle::on_key_press(){
    toggle();
}

