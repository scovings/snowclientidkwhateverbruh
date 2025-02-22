#pragma once

#include "jni/meta_jni.hpp"
#include<string>
#include <gl/GL.h>
namespace maps
{
	BEGIN_KLASS_DEF(Object, "java/lang/Object")
	END_KLASS_DEF()

	BEGIN_KLASS_DEF(String, "java/lang/String")
		static String create(const char* str)
		{
			return String(jni::get_env()->NewStringUTF(str));
		}


		std::string to_string()
		{
			if (!object_instance) return std::string();
			jstring str_obj = (jstring)object_instance;
			jsize utf8_size = jni::get_env()->GetStringUTFLength(str_obj);
			jsize size = jni::get_env()->GetStringLength(str_obj);

			std::string str(utf8_size, '\0');
			jni::get_env()->GetStringUTFRegion(str_obj, 0, size, str.data());
			return str;
		}
	END_KLASS_DEF()

	BEGIN_KLASS_DEF(Collection, "java/util/Collection")
		jni::method<jni::array<Object>, "toArray"> toArray{ *this };
	END_KLASS_DEF()
	BEGIN_KLASS_DEF_EX(List, "java/util/List", Collection)
	END_KLASS_DEF()
	BEGIN_KLASS_DEF(URL, "java/net/URL")
		jni::constructor<String> constructor{ *this };

		jni::method<String, "toString"> toString{ *this };
	END_KLASS_DEF()


	BEGIN_KLASS_DEF(Entity, "pk")
		jni::method<String, "e_"> getName{ *this };
        jni::method<void, "bF"> Jump{ *this};


		jni::field<jdouble, "s", jni::NOT_STATIC> getX{ *this};
		jni::field<jdouble, "t", jni::NOT_STATIC> getY{ *this};
		jni::field<jdouble, "u", jni::NOT_STATIC> getZ{ *this};

		jni::field<jdouble, "v", jni::NOT_STATIC> getMotionX{ *this};
		jni::field<jdouble, "w", jni::NOT_STATIC> getMotionY{ *this};
		jni::field<jdouble, "x", jni::NOT_STATIC> getMotionZ{ *this};

        

	END_KLASS_DEF()

	BEGIN_KLASS_DEF(ActiveRenderInfo, "auz")
		jni::field<GLfloat, "b", jni::NOT_STATIC> MODELVIEW{ *this};
		jni::field<GLfloat, "c", jni::NOT_STATIC> PROJECTION{ *this};
		jni::field<GLfloat, "a", jni::NOT_STATIC> VIEWPORT{ *this};

	END_KLASS_DEF()


	BEGIN_KLASS_DEF_EX(EntityLivingBase, "pr", Entity)
		jni::method<jfloat, "bn"> getHealth{ *this };
	END_KLASS_DEF()


	BEGIN_KLASS_DEF_EX(EntityPlayer, "wn", EntityLivingBase)
	END_KLASS_DEF()

	BEGIN_KLASS_DEF_EX(EntityPlayerSP, "bew", EntityPlayer)
		jni::method<void, "e", jni::NOT_STATIC, String> sendChatMessage{ *this };
		jni::method<String, "w", jni::NOT_STATIC> getClientBrand{ *this };
	END_KLASS_DEF()


	BEGIN_KLASS_DEF(World, "adm")
		jni::field<List, "j"> playerEntities{ *this };
	END_KLASS_DEF()

	BEGIN_KLASS_DEF_EX(WorldClient, "bdb", World)
	END_KLASS_DEF()

	BEGIN_KLASS_DEF(Minecraft, "ave")
		jni::field<Minecraft, "S", jni::STATIC> theMinecraft{ *this };
		jni::field<jint, "d"> displayWidth{ *this };
		jni::field<EntityPlayerSP, "h"> thePlayer{ *this };
		jni::field<WorldClient, "f"> theWorld{ *this };
        jni::field<jint, "ap"> rightClickDelay{*this};

        
		jni::method<void, "aw", jni::NOT_STATIC> clickMouse{ *this };
		jni::method<void, "a", jni::NOT_STATIC, jint, jint> resize{ *this };
	END_KLASS_DEF()
	

	KLASS_DECLARATION(ClassLoader, "java/lang/ClassLoader");

	BEGIN_KLASS_MEMBERS(ClassLoader)
	END_KLASS_MEMBERS()
}