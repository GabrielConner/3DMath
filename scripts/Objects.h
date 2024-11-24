#pragma once
#if defined(_OBJECTS) || defined(_FLOW_CONTROL)
#include <map>
#include <stdexcept>
#include "Constants.h"
#include <algorithm>
#include "IntializeShader.h"
#include "OpenGL.h"
#include "GlobalVars.h"
namespace Objects {
	void start();
	void end();
}
#endif

#include "ObjScripts.h"
#include "TypeDefs.h"
#include "Vector.h"
#include <Set>
#include <string>
#include <Vector>
#include <limits>
#include "Shapes.h"
#include <iostream>
#include "MathConstants.h"
#include "Camera.h"

struct ObjectBase {
	int index = -1;
	uint *texTarget = nullptr;
	uint *VBO = nullptr;
	uint *EBO = nullptr;
	uint *triCount = nullptr;
	std::vector<void*> scripts;
	ObjectBase() : index(-1), texTarget(nullptr), VBO(nullptr), EBO(nullptr), triCount(nullptr), scripts() {}
};

struct Transform {
	Vec3 position = 0;
	Vec3 scale = 1;
	Vec3 rotation = 0;

	Transform() : position(0), scale(1), rotation(0) {}
};

struct Object : private ObjectBase { 
	Transform transform;
	Transform relativeTransform;
	std::set<Object*> children;
    std::set<Object*> dependencies;
    Object* dependent = nullptr;
	Object* parent = nullptr;
	Vec4 color = 1;
	bool active = true;
    std::string objType;
    bool scriptCreated = false;
    bool UI = false;
    bool weak = false;
    

    Object& operator=(Object* obj) {
        this->transform = obj->transform;

        void addObjScript(Object * obj, void* script, char c);
        for (auto& scrV : ((ObjectBase*)obj)->scripts) {
            void* newScr = ((scriptBase*)scrV)->getNew();
            addObjScript(this, newScr, 'c');
        }

        for (auto child : obj->children) {
            if (child->scriptCreated)
                continue;
            Object* temp = new Object;
            *temp = child;
            temp->setParent(this);
        }

        this->color = obj->color;
        this->active = obj->active;
        this->objType = obj->objType;
        this->active = obj->active;

        ObjectBase* base = (ObjectBase*)this;
        ObjectBase* objBase = (ObjectBase*)obj;

        base->texTarget = objBase->texTarget;
        base->VBO = objBase->VBO;
        base->EBO = objBase->EBO;
        base->triCount = objBase->triCount;

        return *this;
    }


	//-------------------Constructor
    //-------------------Destructor

	Object() : transform(), relativeTransform(), children(), dependencies(), parent(nullptr), color(1), active(true), scriptCreated(false), UI(false) {}
    ~Object() {}

	//--------------------------getter functions

	uint getIndex() const {
		return index;
	}
	uint getVBO() const {
		return *VBO;
	}
	uint getEBO() const {
		return *EBO;
	}
	uint getTriCount() const {
		return *triCount;
	}
	uint getTexture() {
		return *texTarget;
	}
    
    template <typename T>
    T* getScript() {
        for (auto scrV : ((ObjectBase*)this)->scripts) {
            scriptBase* scr = (scriptBase*)scrV;
            if (scr->getName() == T::name)
                return (T*)scrV;
        }
    }
    

	//-------------------------Setter functions

	void setTexture(uint& texture) {
		texTarget = &texture;
	}
	void removeTexture() {
		texTarget = nullptr;
	}
	

	//-------------------------Parenting functions

	void setParent(Object* obj) {
		if (parent != nullptr)
			parent->children.erase(this);

        obj->children.insert(this);
		parent = obj;
        this->UI = parent->UI;
        relativeTransform.position = (transform.position - parent->transform.position) / parent->transform.scale;
		relativeTransform.scale = transform.scale / parent->transform.scale;
		relativeTransform.rotation = transform.rotation - parent->transform.rotation;
	}

	void setToRelative() {
		if (parent == nullptr)
			return;

        this->UI = parent->UI;

        glm::mat4 Translation(1);
        Translation = glm::rotate(Translation, glm::radians<float>(parent->transform.rotation.x), glm::vec3(1, 0, 0));
        Translation = glm::rotate(Translation, glm::radians<float>(parent->transform.rotation.y), glm::vec3(0, 1, 0));
        Translation = glm::rotate(Translation, glm::radians<float>(parent->transform.rotation.z), glm::vec3(0, 0, 1));
        Translation = glm::translate(Translation, (relativeTransform.position * parent->transform.scale + parent->transform.position).toGLM());

        transform.position = Translation * glm::vec4(0, 0, 0, 1);
        transform.rotation = parent->transform.rotation;
        transform.scale = parent->transform.scale * relativeTransform.scale;
	}
	void removeParent() {
		if (parent == nullptr)
			return;
		parent->children.erase(this);

		parent = nullptr;
	}


    //Dependent object stuff

    void setDependent(Object* obj) {
        removeDependent();

        dependent = obj;
        obj->dependencies.insert(this);
    }

    void removeDependent() {
        if (dependent == nullptr)
            return;
        dependent->dependencies.erase(this);
        dependent = nullptr;
    }



	//------------------------------Checking functions

	bool usesTexture() {
		return texTarget == nullptr ? false : true;
	}
	static bool chainActive(Object*& obj) {
        if (!obj->active)
            return false;
		if (obj->parent == nullptr)
			return true;
		if (!obj->parent->active)
			return false;
		return chainActive(obj->parent);
	}
};

Object* createObj(std::string objName);
bool unloadBufferObj(std::string name);
bool loadBufferObj(std::string name, uint* VBO, uint* EBO, uint* triCount);
void deleteObjMapping();
void createBufferObj(uint& VBO, uint& EBO, const float vertices[], const uint indices[], const size_t vertSize, const size_t indiceSize);
void addObjScript(Object* obj, void* script);
void removeObjScript(Object* obj, unsigned int index);
unsigned int getObjScriptIndex(Object*& obj, std::string name);
void updateObjScripts(Object*& obj);
void clearObjScripts(Object* const& obj);
void updateObjChildren(Object*& obj);
void clearObjChildren(Object*& obj);
void deleteObj(Object*& obj);
void deleteObj(Object*& obj, int);
void deleteAll();
void drawObjStencil(Object* obj);
void drawAllObjs();
uint findObjSlot();
bool addGlobalObj(ObjectBase*& obj);
Object* createObj();
Object* instantiateObj(std::string objName);
bool saveObj(Object* obj, std::string objName);
void addObjScript(Object* obj, void* script, char c);
void deleteObj(Object*& obj, const char);
void startObject(Object* obj);
void createScriptObjs(std::pair<std::vector<std::string>, std::vector<void*>&> pair);
glm::mat4 createObjTransform(Object* obj);

extern ObjectBase* globalObjects[];
extern uint objCount;