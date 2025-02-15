#include "../main.h"


extern void mc_thePlayer_jump(JNIEnv*);
extern void set_right_click_delay(JNIEnv* env, int max);

extern jclass getMinecraftClass(JNIEnv* env);
extern jclass getPlayerClass(JNIEnv* env);
extern jclass getWorldClass(JNIEnv* env);
extern jclass getPlayerCapabilitiesClass(JNIEnv* env);



extern jobject get_minecraft(JNIEnv* env);
extern jobject get_player(JNIEnv* env);
extern jobject get_world(JNIEnv* env);

extern double getX(JNIEnv* env, jobject entity);
extern double getY(JNIEnv* env, jobject entity);
extern double getZ(JNIEnv* env, jobject entity);
extern int get_block(JNIEnv* env,int x, int y, int z);

extern void set_sneaking(JNIEnv* env, bool flag);
extern void set_sprinting(JNIEnv* env, jobject entity, bool flag);


extern bool onGround(JNIEnv* env, jobject entity);
extern bool isSneaking(JNIEnv* env, jobject entity);

extern void stepHeight(JNIEnv* env,jobject entity, float newHight);

extern void Players(JNIEnv* env, jobject world);

