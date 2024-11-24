#define _OBJECTS
#include "Objects.h"

bool addGlobalObj(ObjectBase*&);

static std::map<std::string, std::tuple<uint*, uint*, uint*>> objTypes;
static std::map<std::string, Object*> savedObjs;

unsigned int squareVBO, squareEBO, squareTriCount;
unsigned int triangleVBO, triangleEBO, triangleTriCount;
unsigned int cubeVBO, cubeEBO, cubeTriCount;

uint objCount = 0;
ObjectBase* globalObjects[_MAX_OBJECTS];

static glm::mat4 _transform(1);

Camera MainCamera = Camera(0, 0, 70, 0.01f, 100.f);

static Shader shader;
static uint VAO;

const float* _identityMatrix = nullptr;

bool _showUI = true;

namespace Objects {
	//Creates default objects
    void start() {
        createBufferObj(squareVBO, squareEBO, squareVertices, squareIndices, sizeof(squareVertices), sizeof(squareIndices));
        squareTriCount = 2;
        loadBufferObj("square", &squareVBO, &squareEBO, &squareTriCount);

        createBufferObj(triangleVBO, triangleEBO, triangleVertices, triangleIndices, sizeof(triangleVertices), sizeof(triangleIndices));
        triangleTriCount = 1;
        loadBufferObj("triangle", &triangleVBO, &triangleEBO, &triangleTriCount);

        createBufferObj(cubeVBO, cubeEBO, cubeVertices, cubeIndices, sizeof(cubeVertices), sizeof(cubeIndices));
        cubeTriCount = 12;
        loadBufferObj("cube", &cubeVBO, &cubeEBO, &cubeTriCount);

        //_projection = glm::perspective(90.f, _screenRatio, -50.f, 50.f);

        MainCamera.setPerspectiveView(70, _screenRatio, 0.01f, 100.f);
        MainCamera.setOrthographicView(_screenRatio, 1, 1);

        _identityMatrix = new float [16] { 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1 };
    }

	//Deletes at the end
	void end() {
		unloadBufferObj("square");
		unloadBufferObj("triangle");
		glDeleteBuffers(1, &squareVBO);
		glDeleteBuffers(1, &squareEBO);
        glDeleteBuffers(1, &triangleVBO);
        glDeleteBuffers(1, &triangleEBO);
        deleteObjMapping();

        for (auto savedObj : savedObjs) {
            deleteObj(savedObj.second, 'c');
        }
        savedObjs.clear();
        delete[](_identityMatrix);
	}
}


//---------------------OBJECT----HANDLING


bool saveObj(Object* obj, std::string objName) {
    if (savedObjs.count(objName))
        return false;

    Object* objCopy = new Object;
    *objCopy = obj;

    return savedObjs.insert(std::pair<std::string, Object*>{objName, objCopy}).second;
}

Object* instantiateObj(std::string objName) {
    if (!savedObjs.count(objName))
        return nullptr;

    Object* ret = createObj();
    *ret = savedObjs.at(objName);
    for (auto& child : ret->children) {
        addGlobalObj((ObjectBase*&)child);
    }
    startObject(ret);

    return ret;
}

//Creates the buffers for an object
void createBufferObj(uint& VBO, uint& EBO, const float vertices[], const uint indices[], const size_t vertSize, const size_t indiceSize) {
	glCreateBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, vertSize, vertices, GL_STATIC_DRAW);

	glCreateBuffers(1, &EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indiceSize, indices, GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

//Loads an object into a map
bool loadBufferObj(std::string name, uint* VBO, uint* EBO, uint* triCount) {
	return objTypes.insert({ name, {VBO, EBO, triCount} }).second;
}

//Removes a specific object type from the map
bool unloadBufferObj(std::string name) {
	return objTypes.erase(name);
}

//Deletes all object types
void deleteObjMapping() {
	objTypes.clear();
}

//Returns a created object
Object* createObj(std::string objName) {
    if (!objTypes.count(objName))
        return nullptr;
    auto& objData = objTypes.at(objName);

    Object* obj = new Object();
    ObjectBase* objBase = (ObjectBase*)obj;

    objBase->VBO = std::get<0>(objData);
    objBase->EBO = std::get<1>(objData);
    objBase->triCount = std::get<2>(objData);
    obj->objType = objName;

    if (!addGlobalObj(objBase))
    {
        delete(obj);
        return nullptr;
    }

    return obj;
}

Object* createObj() {
    Object* obj = new Object();
    ObjectBase* objBase = (ObjectBase*)obj;

    if (!addGlobalObj(objBase)) {
        delete(obj);
        return nullptr;
    }

    return obj;
}

uint findObjSlot() {
    for (uint i = 0; i < _MAX_OBJECTS; i++) {
        if (globalObjects[i] == nullptr)
            return i;
    }
    return _MAX_OBJECTS;
}

bool addGlobalObj(ObjectBase*& obj) {
    if (obj->index > -1)
        return false;
    uint objSlot = findObjSlot();
    if (objSlot == _MAX_OBJECTS)
        return false;

    globalObjects[objSlot] = obj;
    globalObjects[objSlot]->index = objSlot;

    objCount++;
    return true;
}

void deleteAll() {
    for (uint i = 0; i < _MAX_OBJECTS; i++) {
        if (globalObjects[i] == nullptr || ((Object*)globalObjects[i])->parent != nullptr)
            continue;
        clearObjScripts((Object*&)globalObjects[i]);
        clearObjChildren((Object*&)globalObjects[i]);
        delete(globalObjects[i]);
        globalObjects[i] = nullptr;
    }
    objCount = 0;
}

void deleteObj(Object*& obj, const char) {
    if (obj == nullptr)
        return;
    clearObjScripts(obj);
    clearObjChildren(obj);
    obj->removeParent();
    obj->removeDependent();

    delete(obj);
    obj = nullptr;
}

void deleteObj(Object*& obj) {
    if (obj == nullptr)
        return;
    int index = obj->getIndex();
    clearObjScripts(obj);
    clearObjChildren(obj);
    obj->removeParent();
    obj->removeDependent();

    delete(obj);
    if (index > -1) {
        globalObjects[index] = nullptr;
        objCount--;
    }
    obj = nullptr;
}

void deleteObj(Object*& obj, int) {
    if (obj == nullptr)
        return;
    uint index = obj->getIndex();
    clearObjScripts(obj);
    clearObjChildren(obj);
    obj->removeDependent();

    delete(obj);
    if (index > -1) {
        globalObjects[index] = nullptr;
        objCount--;
    }
    obj = nullptr;
}


//--------------------FOR-OBJECTS

void startObject(Object* obj) {
    for (auto scr : ((ObjectBase*)obj)->scripts) {
        if (!((scriptBase*)scr)->started) {
            createScriptObjs(((scriptBase*)scr)->objsNeeded());
            ((scriptBase*)scr)->start();
        }
    }
    for (auto child : obj->children) {
        startObject(child);
    }
}

void createScriptObjs(std::pair<std::vector<std::string>, std::vector<void*>&> pair) {
    for (auto str : pair.first) {
        pair.second.push_back(createObj(str));
        ((Object*)pair.second[pair.second.size() - 1])->scriptCreated = true;
    }
}

//Add an object script
void addObjScript(Object* obj, void* script) {
    ((ObjectBase*)obj)->scripts.push_back(script);
    scriptBase* realScr = (scriptBase*)script;
    createScriptObjs(realScr->objsNeeded());
    realScr->vThisObj = obj;
    realScr->start();
}

//Add an object script without starting it
void addObjScript(Object* obj, void* script, char c) {
    ((ObjectBase*)obj)->scripts.push_back(script);
    scriptBase* realScr = (scriptBase*)script;
    realScr->vThisObj = obj;
}

/// Remove an object script at a specific index
void removeObjScript(Object* obj, unsigned int index) {
    ((scriptBase*)((ObjectBase*)obj)->scripts[index])->end();
    delete(((ObjectBase*)obj)->scripts[index]);
    ((ObjectBase*)obj)->scripts.erase(((ObjectBase*)obj)->scripts.begin() + index);
}

/// <summary>
/// Get the index of a specific script on an object
/// </summary>
/// <returns>An unsigned int with the index</returns>
unsigned int getObjScriptIndex(Object*& obj, std::string name) {
    for (uint i = 0; i < ((ObjectBase*)obj)->scripts.size(); i++) {
        scriptBase* realScr = (scriptBase*)((ObjectBase*)obj)->scripts[i];
        if (realScr->getName() == name) {
            return i;
        }
    }
    return UINT_MAX;
}


/// <summary>
/// Updates all of an objects scripts
/// </summary>
void updateObjScripts(Object*& obj) {
    for (uint i = 0; i < ((ObjectBase*)obj)->scripts.size(); i++) {
        scriptBase* realScr = (scriptBase*)((ObjectBase*)obj)->scripts[i];
        realScr->update();
    }
}

/// <summary>
/// Deletes all the scripts from an object
/// </summary>
void clearObjScripts(Object* const& obj) {
    for (uint i = 0; i < ((ObjectBase*)obj)->scripts.size(); i++) {
        ((scriptBase*)((ObjectBase*)obj)->scripts[i])->end();
        delete(((ObjectBase*)obj)->scripts[i]);
    }
    ((ObjectBase*)obj)->scripts.clear();
}

void clearObjChildren(Object*& obj) {
    if (obj->children.size() == 0)
        return;
    for (auto child : obj->children) {
        deleteObj(child, 0);
    }
    obj->children.clear();
}

void updateObjChildren(Object*& obj) {
    if (obj->children.size() == 0)
        return;
    for (Object* child : obj->children) {
        child->setToRelative();
        updateObjChildren(child);
    }
}

glm::mat4 createObjTransform(Object* obj) {
    
    glm::mat4 ret = glm::mat4(1);
    ret = glm::translate(ret, obj->transform.position.toGLM());
    ret = glm::rotate(ret, (float)degToRad * obj->transform.rotation.x, glm::vec3(1, 0, 0));
    ret = glm::rotate(ret, (float)degToRad * obj->transform.rotation.y, glm::vec3(0, 1, 0));
    ret = glm::rotate(ret, (float)degToRad * obj->transform.rotation.z, glm::vec3(0, 0, 1));
    ret = glm::scale(ret, obj->transform.scale.toGLM());

    return ret;
}



//---------------------OBJECT-DRAWING


void drawAllObjs() {
    if (objCount == 0)
        return;
    MainCamera.setViewMatrix();


    uint objDrawn = 0;
    Object* prevDependent = nullptr;

    for (int i = 0; i < _MAX_OBJECTS; i++) {
        if (objDrawn >= objCount)
            break;
        if (globalObjects[i] == nullptr)
            continue;

        ObjectBase*& objBase = globalObjects[i];
        objBase->index = i;
        Object*& obj = (Object*&)objBase;

        if (obj->UI && !_showUI)
            continue;

        if (!Object::chainActive(obj)) {
            objDrawn++;
            continue;
        }

        if (obj->parent == nullptr)
            updateObjChildren(obj);
        updateObjScripts(obj);

        if (obj->dependent != nullptr) {
            glEnable(GL_STENCIL_TEST);
            if (obj->dependent != prevDependent) {
                glClear(GL_STENCIL_BUFFER_BIT);
                drawObjStencil(obj->dependent);
            }
            prevDependent = obj->dependent;
            glStencilFunc(GL_EQUAL, 0x01, 0xFF);
            glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);
        }
        else
            glDisable(GL_STENCIL_TEST);

        if (obj->weak) {
            glBlendEquation(GL_FUNC_ADD);
            glBlendFuncSeparate(GL_ONE_MINUS_DST_ALPHA, GL_DST_ALPHA, GL_ZERO, GL_ONE);
        }
        else {
            glBlendEquation(GL_FUNC_ADD);
            glBlendFuncSeparate(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_ONE, GL_ZERO);
        }

        _transform = createObjTransform(obj);
        if (obj->usesTexture()) {
            uint texTarget = obj->getTexture();
            shader.use(getShader("textureShader"));
            if (!obj->UI) {
                shader.setMat4("view", MainCamera.viewMatrix);
                shader.setMat4("projection", MainCamera.perspectiveView);
            }
            else {
                shader.setMat4("projection", MainCamera.orthographicView);
                shader.setMat4("view", _identityMatrix);
            }

            shader.setMat4("transform", _transform);
            shader.setVec4("color", obj->color);
            shader.setInt("texTarget", 0);
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, texTarget);
        }
        else {
            shader.use(getShader("noTextureShader"));
            if (!obj->UI) {
                shader.setMat4("view", MainCamera.viewMatrix);
                shader.setMat4("projection", MainCamera.perspectiveView);
            }
            else {
                shader.setMat4("projection", MainCamera.orthographicView);
                shader.setMat4("view", _identityMatrix);
            }

            shader.setMat4("transform", _transform);
            shader.setVec4("color", obj->color);
        }

        glBindVertexArray(VAO);
        glBindBuffer(GL_ARRAY_BUFFER, *objBase->VBO);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, *objBase->EBO);

        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
        glEnableVertexAttribArray(1);

        glDrawElements(GL_TRIANGLES, *objBase->triCount * 3, GL_UNSIGNED_INT, 0);
        objDrawn++;
    }
    glClear(GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void drawObjStencil(Object* obj) {
    glStencilFunc(GL_ALWAYS, 0x01, 0xFF);
    glStencilOp(GL_INCR, GL_INCR, GL_INCR);
    glDisable(GL_DEPTH_TEST);
    glColorMask(0, 0, 0, 0);

    ObjectBase*& objBase = (ObjectBase*&)obj;

    _transform = createObjTransform(obj);

    shader.use(getShader("noTextureShader"));
    shader.setMat4("transform", _transform);
    if (!obj->UI) {
        shader.setMat4("view", MainCamera.viewMatrix);
        shader.setMat4("projection", MainCamera.perspectiveView);
    }
    else {
        shader.setMat4("projection", MainCamera.orthographicView);
        shader.setMat4("view", _identityMatrix);
    }

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, *objBase->VBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, *objBase->EBO);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glDrawElements(GL_TRIANGLES, *objBase->triCount * 3, GL_UNSIGNED_INT, 0);

    glEnable(GL_DEPTH_TEST);
    glColorMask(1, 1, 1, 1);
}