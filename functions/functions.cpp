#include "functions.h"
#include <fstream>
#include "../mappings.hpp"
void mc_thePlayer_jump(JNIEnv* env){
    jclass minecraftClass = env->FindClass("ave");
    jmethodID getMinecraft = env->GetStaticMethodID(minecraftClass, "A", "()Lave;");
    jobject mc_instance = env->CallStaticObjectMethod(minecraftClass, getMinecraft);

    //get player
    jfieldID playerField = env->GetFieldID(minecraftClass, "h", "Lbew;");
    jobject player = env->GetObjectField(mc_instance, playerField);
    if(player == NULL){
        //clsprintf("Error: player is null, are you in game?\n");
        return;
    }
    jclass player_class = env->GetObjectClass(player);

    //call player.jump()
    jmethodID jumpMethod = env->GetMethodID(player_class, "bF", "()V");
    env->CallVoidMethod(player, jumpMethod);

    //free mem:
    env->DeleteLocalRef(minecraftClass);
    env->DeleteLocalRef(mc_instance);
    env->DeleteLocalRef(player);
    env->DeleteLocalRef(player_class);
}



void set_right_click_delay(int max){
	//if the delay timer is above the max entered, set it to the second var
	//mc.rightClickDelayTimer = 0; - the var is found in the minecraft main class
	//obf name = ap, is an int

   maps::Minecraft minecraft{};
   maps::Minecraft theMinecraft = minecraft.theMinecraft.get();

   theMinecraft.rightClickDelay = max;

   
}



jclass getMinecraftClass(JNIEnv* env){
    jclass result = env->FindClass("ave");
    return result;
}


jclass getPlayerClass(JNIEnv* env){
    jclass result = env->FindClass("bew");
    return result;
}



jclass getWorldClass(JNIEnv* env){
    jclass result = env->FindClass("bdb");
    return result;
}


jclass getPlayerCapabilitiesClass(JNIEnv* env){

    jclass result = env->FindClass("wl");
    return result;
}


jobject get_minecraft(JNIEnv* env) {
	jclass minecraftClass = env->FindClass("ave");
	jmethodID minecraftField = env->GetStaticMethodID(minecraftClass, "A", "()Lave;");
	jobject result = env->CallStaticObjectMethod(minecraftClass, minecraftField);
	env->DeleteLocalRef(minecraftClass);
	return result;
}
jobject get_player(JNIEnv* env) {
	jclass minecraftClass = env->FindClass("ave");
    jobject mc_obj = get_minecraft(env);
	jfieldID playerField = env->GetFieldID(minecraftClass, "h", "Lbew;");
	jobject result = env->GetObjectField(mc_obj, playerField);

	env->DeleteLocalRef(minecraftClass);
	env->DeleteLocalRef(mc_obj);
	return result;
}
jobject get_world(JNIEnv* env){
	jclass minecraftClass = env->FindClass("ave");
    jobject mc_obj = get_minecraft(env);
	jfieldID worldField = env->GetFieldID(minecraftClass, "f", "Lbdb;");
	jobject result = env->GetObjectField(mc_obj, worldField);

	env->DeleteLocalRef(minecraftClass);
	env->DeleteLocalRef(mc_obj);
	return result;
}



double getX(){
    maps::Minecraft Minecraft{};
    maps::EntityPlayerSP EntityPlayerSP{};

    maps::Minecraft theMinecraft = Minecraft.theMinecraft.get();
    maps::Minecraft g_theMinecraft = maps::Minecraft(theMinecraft, true);

    maps::EntityPlayerSP thePlayer = theMinecraft.thePlayer.get();

    return thePlayer.getX.get();
}
double getY(){
    maps::Minecraft Minecraft{};
    maps::EntityPlayerSP EntityPlayerSP{};

    maps::Minecraft theMinecraft = Minecraft.theMinecraft.get();
    maps::Minecraft g_theMinecraft = maps::Minecraft(theMinecraft, true);

    maps::EntityPlayerSP thePlayer = theMinecraft.thePlayer.get();

    return thePlayer.getY.get();
}
double getZ(){
    maps::Minecraft Minecraft{};
    maps::EntityPlayerSP EntityPlayerSP{};

    maps::Minecraft theMinecraft = Minecraft.theMinecraft.get();
    maps::Minecraft g_theMinecraft = maps::Minecraft(theMinecraft, true);

    maps::EntityPlayerSP thePlayer = theMinecraft.thePlayer.get();

    return thePlayer.getZ.get();
}


void setMotion(JNIEnv* env, jobject entity, double x, double y, double z){
    jclass clazz = env->GetObjectClass(entity);

    jfieldID motionX = env->GetFieldID(clazz, "v", "D");
    jfieldID motionY = env->GetFieldID(clazz, "w", "D");
    jfieldID motionZ = env->GetFieldID(clazz, "x", "D");


    env->SetDoubleField(entity, motionX, x);
    env->SetDoubleField(entity, motionY, y);
    env->SetDoubleField(entity, motionZ, z);




}


double getMotionX(){
    maps::Minecraft Minecraft{};
    maps::EntityPlayerSP EntityPlayerSP{};

    maps::Minecraft theMinecraft = Minecraft.theMinecraft.get();
    maps::Minecraft g_theMinecraft = maps::Minecraft(theMinecraft, true);

    maps::EntityPlayerSP thePlayer = theMinecraft.thePlayer.get();

    return thePlayer.getMotionX.get();
}
double getMotionY(){
    maps::Minecraft Minecraft{};
    maps::EntityPlayerSP EntityPlayerSP{};

    maps::Minecraft theMinecraft = Minecraft.theMinecraft.get();
    maps::Minecraft g_theMinecraft = maps::Minecraft(theMinecraft, true);

    maps::EntityPlayerSP thePlayer = theMinecraft.thePlayer.get();

    return thePlayer.getMotionY.get();
}
double getMotionZ(){
    maps::Minecraft Minecraft{};
    maps::EntityPlayerSP EntityPlayerSP{};

    maps::Minecraft theMinecraft = Minecraft.theMinecraft.get();
    maps::Minecraft g_theMinecraft = maps::Minecraft(theMinecraft, true);

    maps::EntityPlayerSP thePlayer = theMinecraft.thePlayer.get();

    return thePlayer.getMotionZ.get();
}



int getHurtTime(JNIEnv* env, jobject entity){
    if(entity == NULL){
		return 0;
	}
	jclass klass = env->GetObjectClass(entity);
    jfieldID field = env->GetFieldID(klass, "Z", "I");
	int result = env->GetIntField(entity, field);
	env->DeleteLocalRef(klass);
	return result;
}



int get_block(JNIEnv* env, int x, int y, int z) {
    // Open log file for appending
    std::ofstream logFile("C:\\Users\\snowdev\\Music\\log.txt", std::ios::app);
    if (!logFile.is_open()) {
        std::cerr << "Error: Unable to open log file for writing." << std::endl;
        return 0;
    }

    logFile << "get_block called with coordinates: (" << x << ", " << y << ", " << z << ")\n";  // Log input coordinates

    // Log the start of getting the world object
    logFile << "Step 1: Attempting to get world object...\n";
    jobject worldObj = get_world(env);
    if (worldObj == NULL) {
        logFile << "Error: null world object\n";  // Log when world object is NULL
        logFile.close();
        return 0;
    }

    // Log step for creating block position object
    logFile << "Step 2: Attempting to create block position object...\n";
    jclass blockPosClass = env->FindClass("cj");
    if (blockPosClass == NULL) {
        logFile << "Error: Unable to find block position class\n";  // Log if class is not found
        env->DeleteGlobalRef(worldObj);
        logFile.close();
        return 0;
    }
    jmethodID constructor = env->GetMethodID(blockPosClass, "<init>", "(III)V");
    jobject blockPos = env->NewObject(blockPosClass, constructor, x, y, z);
    if (blockPos == NULL) {
        logFile << "Error: failed to create block position object\n";  // Log error when blockPos creation fails
        env->DeleteGlobalRef(worldObj);
        logFile.close();
        return 0;
    }

    // Log step for getting block state
    logFile << "Step 3: Attempting to get block state...\n";
    jclass world_class = env->FindClass("adm");
    if (world_class == NULL) {
        logFile << "Error: Unable to find world class\n";  // Log if class is not found
        env->DeleteGlobalRef(worldObj);
        env->DeleteGlobalRef(blockPos);
        logFile.close();
        return 0;
    }
    jmethodID getBlockState = env->GetMethodID(world_class, "p", "(Lcj;)Lalz;");
    jobject iBlockState = env->CallObjectMethod(worldObj, getBlockState, blockPos);
    if (iBlockState == NULL) {
        logFile << "Error: failed to get block state\n";  // Log error when block state retrieval fails
        env->DeleteGlobalRef(worldObj);
        env->DeleteGlobalRef(blockPos);
        logFile.close();
        return 0;
    }

    // Log step for getting block from block state
    logFile << "Step 4: Attempting to get block from block state...\n";
    jclass iBlockStateClass = env->FindClass("alz");
    if (iBlockStateClass == NULL) {
        logFile << "Error: Unable to find block state class\n";  // Log if class is not found
        env->DeleteGlobalRef(worldObj);
        env->DeleteGlobalRef(blockPos);
        env->DeleteGlobalRef(iBlockState);
        logFile.close();
        return 0;
    }
    jmethodID getBlockFromBlockState = env->GetMethodID(iBlockStateClass, "c", "()Lafh;");
    jobject block = env->CallObjectMethod(iBlockState, getBlockFromBlockState);
    if (block == NULL) {
        logFile << "Error: failed to get block from block state\n";  // Log error when block retrieval fails
        env->DeleteGlobalRef(worldObj);
        env->DeleteGlobalRef(blockPos);
        env->DeleteGlobalRef(iBlockState);
        logFile.close();
        return 0;
    }

    // Log step for getting block ID
    logFile << "Step 5: Attempting to get block ID...\n";
    jclass blockClass = env->FindClass("afh");
    if (blockClass == NULL) {
        logFile << "Error: Unable to find block class\n";  // Log if class is not found
        env->DeleteGlobalRef(worldObj);
        env->DeleteGlobalRef(blockPos);
        env->DeleteGlobalRef(iBlockState);
        env->DeleteGlobalRef(block);
        logFile.close();
        return 0;
    }
    jmethodID getIDFromBlock = env->GetStaticMethodID(blockClass, "a", "(Lafh;)I");
    if (getIDFromBlock == NULL) {
        logFile << "Error: Unable to find getIDFromBlock method\n";  // Log if method is not found
        env->DeleteGlobalRef(worldObj);
        env->DeleteGlobalRef(blockPos);
        env->DeleteGlobalRef(iBlockState);
        env->DeleteGlobalRef(block);
        logFile.close();
        return 0;
    }
    int blockID = env->CallIntMethod(blockClass, getIDFromBlock, block);
    logFile << "Retrieved block ID: " << blockID << "\n";  // Log the retrieved block ID

    // Free memory and clean up
    logFile << "Step 6: Freeing allocated memory...\n";
    env->DeleteLocalRef(blockPosClass);
    env->DeleteLocalRef(world_class);
    env->DeleteLocalRef(iBlockStateClass);
    env->DeleteLocalRef(blockClass);

    env->DeleteLocalRef(worldObj);
    env->DeleteLocalRef(blockPos);
    env->DeleteLocalRef(iBlockState);
    env->DeleteLocalRef(block);

    logFile.close();  // Close the log file before returning
    return blockID;
}







void set_sneaking(JNIEnv* env, bool flag){

	jclass klass = env->FindClass("ave");
    jobject mc_obj = get_minecraft(env);
	jfieldID field = env->GetFieldID(klass, "t", "Lavh;");
	jobject gameSettingsObj = env->GetObjectField(mc_obj, field);
	if(gameSettingsObj == NULL){
		printf("gameSettingsObj is null\n");
		env->DeleteLocalRef(klass);
		env->DeleteLocalRef(mc_obj);
		return;
	}

	jclass klass2 = env->FindClass("avh");
	jfieldID field2 = env->GetFieldID(klass2, "ad", "Lavb;");
	jobject keyBindSneakObj = env->GetObjectField(gameSettingsObj, field2);
	if(keyBindSneakObj == NULL){
		printf("keyBindSneakObj is null\n");
		env->DeleteLocalRef(klass);
		env->DeleteLocalRef(mc_obj);
		env->DeleteLocalRef(gameSettingsObj);
		env->DeleteLocalRef(klass2);
		return;
	}

	jclass klass3 = env->FindClass("avb");
	jfieldID field3 = env->GetFieldID(klass3, "h", "Z");
	jboolean sneaking = env->GetBooleanField(keyBindSneakObj, field3);
	env->SetBooleanField(keyBindSneakObj, field3, flag);

	env->DeleteLocalRef(klass);
	env->DeleteLocalRef(klass2);
	env->DeleteLocalRef(klass3);
	env->DeleteLocalRef(mc_obj);
	env->DeleteLocalRef(gameSettingsObj);
	env->DeleteLocalRef(keyBindSneakObj);
}



bool onGround(JNIEnv* env, jobject entity){
	if(entity == NULL){
		return 0;
	}
	jclass klass = env->GetObjectClass(entity);
	jfieldID field = env->GetFieldID(klass, "C", "Z");
	bool result = env->GetBooleanField(entity, field);
	env->DeleteLocalRef(klass);
	return result;
	
}

bool isSneaking(JNIEnv* env, jobject entity){
	if(entity == NULL){
		return 0;
	}
	jclass klass = env->GetObjectClass(entity);
	jfieldID field = env->GetFieldID(klass, "av", "()Z");
	bool result = env->GetBooleanField(entity, field);
	env->DeleteLocalRef(klass);
	return result;
	
}


bool isBurning(JNIEnv* env, jobject entity){
	if(entity == NULL){
		return 0;
	}
	jclass klass = env->GetObjectClass(entity);
	jfieldID field = env->GetFieldID(klass, "at", "()Z");
	bool result = env->GetBooleanField(entity, field);
	env->DeleteLocalRef(klass);
	return result;
	
}



bool isSprinting(JNIEnv* env, jobject entity){
	if(entity == NULL){
		return 0;
	}
	jclass klass = env->GetObjectClass(entity);
	jfieldID field = env->GetFieldID(klass, "aw", "()Z");
	bool result = env->GetBooleanField(entity, field);
	env->DeleteLocalRef(klass);
	return result;
	
}


bool isInGui(JNIEnv* env, jobject minecraft){
	if(minecraft == NULL){
		return 0;
	}

    
	jclass klass = env->GetObjectClass(minecraft);
	jfieldID field = env->GetFieldID(klass, "m", "()Z");
	bool result = env->GetBooleanField(minecraft, field);
	env->DeleteLocalRef(klass);
	return result;
	
}

void stepHeight(JNIEnv* env,jobject entity, float newHeight){



	jclass klass = env->GetObjectClass(entity);
	jfieldID field = env->GetFieldID(klass, "t", "F");
	env->SetFloatField(entity, field, newHeight);


}



void Players(JNIEnv* env, jobject world){



	jclass klass = env->GetObjectClass(world);
	jfieldID field = env->GetFieldID(klass, "j", "Ljava/util/List;");
	std::cout << env->GetObjectField(klass, field) << std::endl;

}


void set_sprinting(JNIEnv* env,jobject entity, bool flag)
{
        jclass klass = env->FindClass("bew");
        jmethodID setSprintingMethod = env->GetMethodID(klass, "d", "(Z)V");
        env->CallBooleanMethod(entity, setSprintingMethod, flag);

        env->DeleteLocalRef(klass);
}

