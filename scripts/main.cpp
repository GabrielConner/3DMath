#include "Main.h"
float* tubeVertices = nullptr;
uint* tubeIndices = nullptr;
size_t tubeVerticeSize = 0;
size_t tubeIndiceSize = 0;

float* volumeVertices = nullptr;
uint* volumeIndices = nullptr;
size_t volumeVerticeSize = 0;
size_t volumeIndiceSize = 0;

std::vector<Vec2> tubePoints;

extern Camera MainCamera;

float rotationSpeed = 30;

Object* refCube = nullptr;
Object* tube = nullptr;
Object* volume = nullptr;

uint tubeVBO, tubeEBO;
uint tubeTriangleCount = 0;

uint volumeVBO, volumeEBO;
uint volumeTriangleCount = 0;

uint volumeCirclePoints = 15;
uint tubeResolution = 5;
uint graphResolution = 200;

float xMin = 0;
extern float xDist;
float volumeAngle = 360;
float tubeThickness = 0.04f;

Vec3 volumeAxis = { 0, 1, 0 };

std::vector<mathStep> graph;

TextField* equationInputScr = nullptr;
Button* equationSubmitScr = nullptr;
Button* showEquationInputScr = nullptr;
Button* increMaxScr = nullptr;
Button* decreMaxScr = nullptr;
Button* increMinScr = nullptr;
Button* decreMinScr = nullptr;
TextBox* minScr = nullptr;
TextBox* maxScr = nullptr;
Button* increThickScr = nullptr;
Button* decreThickScr = nullptr;
TextBox* thickScr = nullptr;
Button* showSettingsScr = nullptr;
Button* volumeAxisChangeScr = nullptr;



Object* equationInput = nullptr;
Object* equationSubmit = nullptr;
Object* showEquationInput = nullptr;
Object* increMax = nullptr;
Object* decreMax = nullptr;
Object* increMin = nullptr;
Object* decreMin = nullptr;
Object* min = nullptr;
Object* minBox = nullptr;
Object* max = nullptr;
Object* maxBox = nullptr;
Object* increThick = nullptr;
Object* decreThick = nullptr;
Object* thick = nullptr;
Object* thickBox = nullptr;
Object* showSettings = nullptr;
Object* settingsBackground = nullptr;
Object* volumeAxisChange = nullptr;

void submitEquation() {
    parseEquation(graph, equationInputScr->text);
    createGraphVolume();
    createGraphShape();
}

void showEquation() {
    equationInput->active = !equationInput->active;
    if (equationInput->active) {
        equationSubmit->transform.position.y = -1 + 2 * equationInput->transform.scale.y + equationSubmit->transform.scale.y;
        showEquationInput->transform.position.y = -1 + 2 * equationInput->transform.scale.y + showEquationInput->transform.scale.y;
        showEquationInputScr->textScr->text = "Hide equation";
        showEquationInputScr->textScr->textUpdate();
    }
    else {
        equationSubmit->transform.position.y = -1 + equationSubmit->transform.scale.y;
        showEquationInput->transform.position.y = -1 + showEquationInput->transform.scale.y;
        showEquationInputScr->textScr->text = "Show equation";
        showEquationInputScr->textScr->textUpdate();
    }
}

void showSettingsFunc() {
    settingsBackground->active = !settingsBackground->active;
    if (settingsBackground->active) {
        showSettings->transform.position.y = settingsBackground->transform.position.y - settingsBackground->transform.scale.y - 0.1f;
        showSettingsScr->textScr->text = "Hide settings";
        showSettingsScr->textScr->textUpdate();
    }
    else {
        showSettings->transform.position.y = 1.0f - 0.1f;
        showSettingsScr->textScr->text = "Show settings";
        showSettingsScr->textScr->textUpdate();
    }
}

void increMaxFunc() {
    if (keyAction::getShift())
        xDist += 0.01f;
    else if (keyAction::keyHeld(GLFW_KEY_LEFT_CONTROL))
        xDist += 0.05f;
    else
        xDist += 0.5f;
}
void decreMaxFunc() {
    if (keyAction::getShift())
        xDist -= 0.01f;
    else if (keyAction::keyHeld(GLFW_KEY_LEFT_CONTROL))
        xDist -= 0.05f;
    else
        xDist -= 0.5f;
}
void increMinFunc() {
    if (keyAction::getShift())
        xMin += 0.01f;
    else if (keyAction::keyHeld(GLFW_KEY_LEFT_CONTROL))
        xMin += 0.05f;
    else
        xMin += 0.5f;
}
void decreMinFunc() {
    if (keyAction::getShift())
        xMin -= 0.01f;
    else if (keyAction::keyHeld(GLFW_KEY_LEFT_CONTROL))
        xMin -= 0.05f;
    else
        xMin -= 0.5f;
}
void increThickFunc() {
    if (keyAction::getShift())
        tubeThickness += 0.01f;
    else if (keyAction::keyHeld(GLFW_KEY_LEFT_CONTROL))
        tubeThickness += 0.05f;
    else
        tubeThickness += 0.5f;
}
void decreThickFunc() {
    if (keyAction::getShift())
        tubeThickness -= 0.01f;
    else if (keyAction::keyHeld(GLFW_KEY_LEFT_CONTROL))
        tubeThickness -= 0.05f;
    else
        tubeThickness -= 0.5f;
}

bool yAxis = true;
void changeAxis() {
    yAxis = !yAxis;
    if (!yAxis) {
        volumeAxis = { 1, 0, 0 };
        volumeAxisChangeScr->textScr->text = "Change to Y-Axis";
    }
    else {
        volumeAxis = { 0, 1, 0 };
        volumeAxisChangeScr->textScr->text = "Change to X-Axis";
    }

    volumeAxisChangeScr->textScr->textUpdate();

}

void startUI() {
    equationInput = createObj("square");
    equationInput->UI = true;
    equationInput->transform.scale = { _screenRatio, 0.08, 1 };
    equationInput->transform.position = { 0, -1 + 0.08, 0.9 };
    equationInput->color = { 0, 0, 0, 1 };

    equationInputScr = new TextField();
    equationInputScr->emptyText = "Enter equation...";
    equationInputScr->emptyColor = { 0.5, 0.5, 0.5, 1 };
    equationInputScr->color = { 0.9, 0.9, 0.9, 1 };

    addObjScript(equationInput, equationInputScr);
    equationInputScr->textScr->font = "CascadiaCodeLight_NORMAL";
    equationInputScr->textScr->fontSize = 1;
    equationInputScr->textScr->lineSize = 1.2;
    equationInputScr->textScr->textUpdate();

    equationSubmit = createObj("square");
    equationSubmit->UI = true;
    equationSubmit->transform.scale = { 0.1f, 0.1f, 1.0f };
    equationSubmit->transform.position = { _screenRatio - 0.1f, -1.0f + 0.26f, 0.9f };
    equationSubmit->color = { 1, 0, 0, 1 };

    equationSubmitScr = new Button();
    equationSubmitScr->onPressed = submitEquation;

    addObjScript(equationSubmit, equationSubmitScr);
    equationSubmitScr->textScr->text = "Submit";
    equationSubmitScr->textScr->fontSize = 0.4f;
    equationSubmitScr->textScr->lineSize = 0.4f;
    equationSubmitScr->textScr->mode = TEXT_CENTER_RENDER;
    equationSubmitScr->textScr->textUpdate();

    showEquationInput = createObj("square");
    showEquationInput->UI = true;
    showEquationInput->transform.scale = { 0.15f, 0.1f, 1.0f };
    showEquationInput->transform.position = { _screenRatio - equationSubmit->transform.scale.y * 2 - 0.15f, -1 + 0.1f + equationInput->transform.scale.y * 2, 0.9f };
    showEquationInput->color = { 0, 0, 0, 1 };

    showEquationInputScr = new Button;
    showEquationInputScr->onPressed = showEquation;

    addObjScript(showEquationInput, showEquationInputScr);
    showEquationInputScr->textScr->text = "Hide equation";
    showEquationInputScr->textScr->fontColor = { 1, 1, 1, 1 };
    showEquationInputScr->textScr->mode = TEXT_CENTER_RENDER;
    showEquationInputScr->textScr->fontSize = 0.4f;
    showEquationInputScr->textScr->lineSize = 0.4f;
    showEquationInputScr->textScr->textUpdate();

    settingsBackground = createObj("square");
    settingsBackground->UI = true;
    settingsBackground->transform.scale = { 0.6, 0.8, 1.0 };
    settingsBackground->transform.position = { -_screenRatio + 0.6f, 1.0 - 0.8, 0.85 };
    settingsBackground->color = { 0.6, 0.6, 0.65, 1.0 };


    showSettings = createObj("square");
    showSettings->UI = true;
    showSettings->transform.scale = { 0.15f, 0.1f, 1.0f };
    showSettings->transform.position = { -_screenRatio + 0.15f, settingsBackground->transform.position.y - settingsBackground->transform.scale.y - 0.1f, 0.9f };
    showSettings->color = { 0, 0, 0, 1 };
    
    showSettingsScr = new Button;
    showSettingsScr->onPressed = showSettingsFunc;

    addObjScript(showSettings, showSettingsScr);
    showSettingsScr->textScr->text = "Hide settings";
    showSettingsScr->textScr->fontColor = { 1, 1, 1, 1 };
    showSettingsScr->textScr->mode = TEXT_CENTER_RENDER;
    showSettingsScr->textScr->fontSize = 0.4f;
    showSettingsScr->textScr->lineSize = 0.4f;
    showSettingsScr->textScr->textUpdate();


    maxBox = createObj("square");
    max = createObj("square");
    maxBox->setParent(max);
    max->UI = true;
    maxBox->UI = true;
    max->transform.scale = { 0.2f, 0.15f, 1.0f };
    max->transform.position = { settingsBackground->transform.position.x, 1 - 0.2f, 0.9f };
    maxBox->color = { 0.4, 0.4, 0.4, 1 };
    maxBox->relativeTransform.position.z = -0.02f;
    max->setParent(settingsBackground);

    maxScr = new TextBox;
    maxScr->fontSize = 0.3f;
    maxScr->lineSize = 0.3f;
    maxScr->mode = TEXT_CENTER_RENDER;
    addObjScript(max, maxScr);

    increMax = createObj("square");
    increMax->UI = true;
    increMax->transform.scale = { 0.15f, 0.15f, 1.0f };
    increMax->transform.position = max->transform.position + Vec3{0.35f, 0, 0};
    increMax->color = { 0, 0, 0, 1 };
    increMax->setParent(settingsBackground);

    increMaxScr = new Button;
    increMaxScr->onPressed = increMaxFunc;
    addObjScript(increMax, increMaxScr);
    increMaxScr->textScr->text = "+";
    increMaxScr->textScr->mode = TEXT_CENTER_RENDER;
    increMaxScr->textScr->fontSize = 1;
    increMaxScr->textScr->lineSize = 1;
    increMaxScr->textScr->fontColor = { 1, 1, 1, 1 };
    increMaxScr->textScr->textUpdate();

    decreMax = createObj("square");
    decreMax->UI = true;
    decreMax->transform.scale = { 0.15f, 0.15f, 1.0f };
    decreMax->transform.position = max->transform.position - Vec3{ 0.35f, 0, 0 };
    decreMax->color = { 0, 0, 0, 1 };
    decreMax->setParent(settingsBackground);

    decreMaxScr = new Button;
    decreMaxScr->onPressed = decreMaxFunc;
    addObjScript(decreMax, decreMaxScr);
    decreMaxScr->textScr->text = "-";
    decreMaxScr->textScr->mode = TEXT_CENTER_RENDER;
    decreMaxScr->textScr->fontSize = 1;
    decreMaxScr->textScr->lineSize = 1;
    decreMaxScr->textScr->fontColor = { 1, 1, 1, 1 };
    decreMaxScr->textScr->textUpdate();

    minBox = createObj("square");
    min = createObj("square");
    minBox->setParent(min);
    min->UI = true;
    minBox->UI = true;
    min->transform.scale = { 0.2f, 0.15f, 1.0f };
    min->transform.position = { settingsBackground->transform.position.x, 1 - 0.55f, 0.9f };
    minBox->color = { 0.4, 0.4, 0.4, 1 };
    minBox->relativeTransform.position.z = -0.02f;
    min->setParent(settingsBackground);

    minScr = new TextBox;
    minScr->fontSize = 0.3f;
    minScr->lineSize = 0.3f;
    minScr->mode = TEXT_CENTER_RENDER;
    addObjScript(min, minScr);

    increMin = createObj("square");
    increMin->UI = true;
    increMin->transform.scale = { 0.15f, 0.15f, 1.0f };
    increMin->transform.position = min->transform.position + Vec3{ 0.35f, 0, 0 };
    increMin->color = { 0, 0, 0, 1 };
    increMin->setParent(settingsBackground);

    increMinScr = new Button;
    increMinScr->onPressed = increMinFunc;
    addObjScript(increMin, increMinScr);
    increMinScr->textScr->text = "+";
    increMinScr->textScr->mode = TEXT_CENTER_RENDER;
    increMinScr->textScr->fontSize = 1;
    increMinScr->textScr->lineSize = 1;
    increMinScr->textScr->fontColor = { 1, 1, 1, 1 };
    increMinScr->textScr->textUpdate();

    decreMin = createObj("square");
    decreMin->UI = true;
    decreMin->transform.scale = { 0.15f, 0.15f, 1.0f };
    decreMin->transform.position = min->transform.position - Vec3{ 0.35f, 0, 0 };
    decreMin->color = { 0, 0, 0, 1 };
    decreMin->setParent(settingsBackground);

    decreMinScr = new Button;
    decreMinScr->onPressed = decreMinFunc;
    addObjScript(decreMin, decreMinScr);
    decreMinScr->textScr->text = "-";
    decreMinScr->textScr->mode = TEXT_CENTER_RENDER;
    decreMinScr->textScr->fontSize = 1;
    decreMinScr->textScr->lineSize = 1;
    decreMinScr->textScr->fontColor = { 1, 1, 1, 1 };
    decreMinScr->textScr->textUpdate();

    thickBox = createObj("square");
    thick = createObj("square");
    thickBox->setParent(thick);
    thick->UI = true;
    thickBox->UI = true;
    thick->transform.scale = { 0.2f, 0.15f, 1.0f };
    thick->transform.position = { settingsBackground->transform.position.x, 1 - 0.9f, 0.9f };
    thickBox->color = { 0.4, 0.4, 0.4, 1 };
    thickBox->relativeTransform.position.z = -0.02f;
    thick->setParent(settingsBackground);

    thickScr = new TextBox;
    thickScr->fontSize = 0.2f;
    thickScr->lineSize = 0.2f;
    thickScr->mode = TEXT_CENTER_RENDER;
    addObjScript(thick, thickScr);

    increThick = createObj("square");
    increThick->UI = true;
    increThick->transform.scale = { 0.15f, 0.15f, 1.0f };
    increThick->transform.position = thick->transform.position + Vec3{ 0.35f, 0, 0 };
    increThick->color = { 0, 0, 0, 1 };
    increThick->setParent(settingsBackground);

    increThickScr = new Button;
    increThickScr->onPressed = increThickFunc;
    addObjScript(increThick, increThickScr);
    increThickScr->textScr->text = "+";
    increThickScr->textScr->mode = TEXT_CENTER_RENDER;
    increThickScr->textScr->fontSize = 1;
    increThickScr->textScr->lineSize = 1;
    increThickScr->textScr->fontColor = { 1, 1, 1, 1 };
    increThickScr->textScr->textUpdate();

    decreThick = createObj("square");
    decreThick->UI = true;
    decreThick->transform.scale = { 0.15f, 0.15f, 1.0f };
    decreThick->transform.position = thick->transform.position - Vec3{ 0.35f, 0, 0 };
    decreThick->color = { 0, 0, 0, 1 };
    decreThick->setParent(settingsBackground);

    decreThickScr = new Button;
    decreThickScr->onPressed = decreThickFunc;
    addObjScript(decreThick, decreThickScr);
    decreThickScr->textScr->text = "-";
    decreThickScr->textScr->mode = TEXT_CENTER_RENDER;
    decreThickScr->textScr->fontSize = 1;
    decreThickScr->textScr->lineSize = 1;
    decreThickScr->textScr->fontColor = { 1, 1, 1, 1 };
    decreThickScr->textScr->textUpdate();
    
    volumeAxisChange = createObj("square");
    volumeAxisChange->UI = true;
    volumeAxisChange->transform.scale = { 0.4, 0.1, 1.0 };
    volumeAxisChange->transform.position = settingsBackground->transform.position - Vec3{ 0.f, settingsBackground->transform.scale.y - 0.2f, -0.05f};
    volumeAxisChange->color = { 0.4, 0.4, 0.4, 1 };
    volumeAxisChange->setParent(settingsBackground);

    volumeAxisChangeScr = new Button;
    volumeAxisChangeScr->onPressed = changeAxis;

    addObjScript(volumeAxisChange, volumeAxisChangeScr);
    volumeAxisChangeScr->textScr->text = "Change to X-Axis";
    volumeAxisChangeScr->textScr->fontSize = 0.4f;
    volumeAxisChangeScr->textScr->lineSize = 0.4f;
    volumeAxisChangeScr->textScr->mode = TEXT_CENTER_RENDER;
    volumeAxisChangeScr->textScr->textUpdate();
    
}

float maxSaid = -1;
float minSaid = -1;
float thickSaid = -1;

void progStart() {
    MainCamera.position = { 0, 0, -5 };

    xDist = 5;

    //parseEquation(graph, "sqrt|cos(2x)|+(elog(x)(2))/pi");
    parseEquation(graph, "pow(x)(2)");

    setBgColor(1, 1, 1, 0);

    loadBufferObj("tube", &tubeVBO, &tubeEBO, &tubeTriangleCount);
    loadBufferObj("volume", &volumeVBO, &volumeEBO, &volumeTriangleCount);

    tube = createObj("tube");
    tube->color = { 0, 0, 0, 1 };
    tube->active = false;
    tube->math = true;

    volume = createObj("volume");
    volume->color = { 1, 0, 0, 1 };
    volume->active = false;
    volume->math = true;

    refCube = createObj("cube");
    refCube->transform.scale = 0.05f;
    refCube->color = { 0, 0, 0, 1 };

    createGraphVolume();
    createGraphShape();

    startUI();
}

Vec2 preMousePos;

void progMain() {
    if (_hideMouse) {
        MainCamera.addRotation(-(_mousePosY - preMousePos.y) * rotationSpeed, (_mousePosX - preMousePos.x) * rotationSpeed, 0);
    }
    preMousePos = { _mousePosX, _mousePosY };

    if (keyAction::keyHeld(GLFW_KEY_W))
        MainCamera.position += MainCamera.directionFacing() * 10.f * _deltaTime;
    if (keyAction::keyHeld(GLFW_KEY_S))
        MainCamera.position += MainCamera.directionFacing() * -10.f * _deltaTime;

    if (keyAction::keyPressed(GLFW_KEY_T))
        _hideMouse = !_hideMouse;

    if (maxSaid != xDist) {
        std::stringstream s;
        s << std::fixed << std::setprecision(2) << xDist;
        maxScr->text = "Max : " + s.str();
        maxScr->textUpdate();
    }
    if (minSaid != xMin) {
        std::stringstream s;
        s << std::fixed << std::setprecision(2) << xMin;
        minScr->text = "Min : " + s.str();
        minScr->textUpdate();
    }
    if (thickSaid != tubeThickness) {
        std::stringstream s;
        s << std::fixed << std::setprecision(2) << tubeThickness;
        thickScr->text = "Thickness : " + s.str();
        thickScr->textUpdate();
    }
}

void progEnd() {
    delete[](tubeVertices);
    delete[](tubeIndices);
    delete[](volumeVertices);
    delete[](volumeIndices);
}

void createGraphShape() {
    getGraphPoints(tubePoints, graph, graphResolution, xMin, xDist);
    tube->active = createTube(tubeThickness, tubeVertices, tubeIndices, tubeVerticeSize, tubeIndiceSize, tubeTriangleCount, tubeResolution, tubePoints);
    createBufferObj(tubeVBO, tubeEBO, tubeVertices, tubeIndices, tubeVerticeSize, tubeIndiceSize);
}

void createGraphVolume() {
    delete[](volumeVertices);
    delete[](volumeIndices);

    getGraphPoints(tubePoints, graph, graphResolution, xMin, xDist);
    auto rotatedGraph = rotateGraph(tubePoints, volumeAxis, volumeAngle, volumeCirclePoints);

    volumeVertices = rotatedGraph.first;
    volumeVerticeSize = rotatedGraph.second * sizeof(float);

    volume->active = createGraphMesh(volumeVertices, rotatedGraph.second, volumeCirclePoints, tubePoints.size(), volumeIndices, volumeIndiceSize, volumeTriangleCount);
    createBufferObj(volumeVBO, volumeEBO, volumeVertices, volumeIndices, volumeVerticeSize, volumeIndiceSize);
}

void getGraphPoints(std::vector<Vec2>&points, std::vector<mathStep> graph, uint steps, float min, float max) {
    points.clear();
    float stepSize = (max - min) / steps;
    float x = min;
    for (int i = 0; i < steps; i++) {
        points.push_back({ x, mathStep::solve(std::vector<mathStep>(graph).begin(), graph.size(), x, 0)});
        x += stepSize;
    }
    points.push_back({ max, mathStep::solve(std::vector<mathStep>(graph).begin(), graph.size(), max, 0) });
}

bool createTube(float tubeSize, float*& vertArr, uint*& indiceArr, size_t& vertSize, size_t& indiceSize, uint& triCount, uint resolution, std::vector<Vec2> points) {
    if (points.size() < 2 || resolution < 3)
        return false;

    float rotAngle = 360.f / resolution;
    float offsetAngle = rotAngle / 2;

    if (vertArr)
        delete[](vertArr);
    if (indiceArr)
        delete[](indiceArr);

    triCount = resolution * 2 * (points.size() - 1);
    vertSize = resolution * 5 * points.size();
    indiceSize = triCount * 3;

    vertArr = new float[vertSize];
    indiceArr = new uint[indiceSize];

    vertSize *= sizeof(float);
    indiceSize *= sizeof(uint);

    Vec3* sCirclePoints = new Vec3[resolution];
    Vec3* tCirclePoints = new Vec3[resolution];

    for (uint i = 0; i < resolution; i++) {
        float angle = _degToRad * i * rotAngle;
        sCirclePoints[i] = { 0, tubeSize * sin(angle), tubeSize * cos(angle) };
    }
    for (uint i = 0; i < resolution; i++) {
        float angle = _degToRad * (i * rotAngle + offsetAngle);
        tCirclePoints[i] = { 0, tubeSize * sin(angle), tubeSize * cos(angle) };
    }

    Vec2 previousPoint = points[0];
    Vec2 nextPoint = points[1];
    uint index = 0;
    for (uint point = 0; point < points.size(); point++) {
        if (point < points.size() - 1)
            nextPoint = points[point + 1];

        Vec2 lineDiff = nextPoint - previousPoint;
        double lineAngle = atan(lineDiff.y / lineDiff.x);
        Vec3* circleUse = point % 2 ? tCirclePoints : sCirclePoints;
        previousPoint = points[point];

        for (uint i = 0; i < resolution; i++) {
            Vec3 pointMoved = { -circleUse[i].y * (float)sin(lineAngle) + points[point].x, circleUse[i].y * (float)cos(lineAngle) + points[point].y, circleUse[i].z };
            vertArr[5 * ((point * resolution) + i)] = pointMoved.x;
            vertArr[(5 * ((point * resolution) + i)) + 1] = pointMoved.y;
            vertArr[(5 * ((point * resolution) + i)) + 2] = pointMoved.z;
            vertArr[(5 * ((point * resolution) + i)) + 3] = 0;
            vertArr[(5 * ((point * resolution) + i)) + 4] = 0;
        }
    }
    for (uint point = 1; point < points.size(); point++) {
        for (uint i = 0; i < resolution; i++) {

            indiceArr[6 * (resolution * (point - 1) + i)] = point * resolution + i;
            indiceArr[6 * (resolution * (point - 1) + i) + 1] = (point - 1) * resolution + i;
            if (i < resolution - 1)
                indiceArr[6 * (resolution * (point - 1) + i) + 2] = (point - 1) * resolution + i + 1;
            else
                indiceArr[6 * (resolution * (point - 1) + i) + 2] = (point - 1) * resolution;


            indiceArr[6 * (resolution * (point - 1) + i) + 3] = point * resolution + i;
            if (i < resolution - 1)
                indiceArr[6 * (resolution * (point - 1) + i) + 4] = (point - 1) * resolution + i + 1;
            else
                indiceArr[6 * (resolution * (point - 1) + i) + 4] = (point - 1) * resolution;
            if (i < resolution - 1)
                indiceArr[6 * (resolution * (point - 1) + i) + 5] = point * resolution + i + 1;
            else
                indiceArr[6 * (resolution * (point - 1) + i) + 5] = point * resolution;
        }
    }

    delete[](sCirclePoints);
    delete[](tCirclePoints);
    return true;
}

std::pair<float*, uint> rotateGraph(std::vector<Vec2> points, Vec3 axis, float endAngle, uint steps) {
    float rotAngle = endAngle / steps * _degToRad;

    float* vertices = new float[points.size() * steps * 5];

    glm::mat4 rotMatrix;

    for (int a = 0; a < steps; a++) {
        rotMatrix = glm::mat4(1);
        rotMatrix = glm::rotate(rotMatrix, a * rotAngle, axis.toGLM());

        for (int point = 0; point < points.size(); point++) {
            Vec3 newPoint = (rotMatrix * glm::vec4(points[point].x, points[point].y, 0, 1));
            vertices[5 * (a * points.size() + point)] = newPoint.x;
            vertices[5 * (a * points.size() + point) + 1] = newPoint.y;
            vertices[5 * (a * points.size() + point) + 2] = newPoint.z;
            vertices[5 * (a * points.size() + point) + 3] = 0;
            vertices[5 * (a * points.size() + point) + 4] = 0;
        }
    }

    return { vertices, points.size() * steps * 5 };
}

bool createGraphMesh(float* vertices, uint size, uint pPerCircle, uint pPerLine, uint*& indiceArr, size_t& indiceSize, uint& triCount) {
    if (pPerCircle < 3)
        return false;

    triCount = pPerCircle * (pPerLine - 1) * 2;
    indiceSize = triCount * 3;

    indiceArr = new uint[indiceSize];

    indiceSize = indiceSize * sizeof(uint);

    for (uint point = 0; point < pPerLine - 1; point++) {
        for (uint i = 0; i < pPerCircle; i++) {

            indiceArr[6 * (pPerCircle * point + i)] = i * pPerLine + point;
            if ( i < pPerCircle - 1)
                indiceArr[6 * (pPerCircle * point + i) + 1] = (i + 1) * pPerLine + point;
            else
                indiceArr[6 * (pPerCircle * point + i) + 1] = point;
            indiceArr[6 * (pPerCircle * point + i) + 2] = i * pPerLine + point + 1;


            indiceArr[6 * (pPerCircle * point + i) + 3] = i * pPerLine + point + 1;
            if (i < pPerCircle - 1)
                indiceArr[6 * (pPerCircle * point + i) + 4] = (i + 1) * pPerLine + point;
            else
                indiceArr[6 * (pPerCircle * point + i) + 4] = point;

            if (i < pPerCircle - 1)
                indiceArr[6 * (pPerCircle * point + i) + 5] = (i + 1) * pPerLine + point + 1;
            else
                indiceArr[6 * (pPerCircle * point + i) + 5] = point + 1;
        }
    }

    return true;

}

int strMath(std::string str) {
    if (str == "+")
        return 0;
    if (str == "-")
        return 0;
    if (str == "*")
        return 0;
    if (str == "/")
        return 0;

    if (str == "log")
        return 1;
    if (str == "nthroot")
        return 1;
    if (str == "^" || str == "pow")
        return 1;
    if (str == "ln")
        return 1;
    if (str == "sqrt")
        return 1;
    if (str == "cos")
        return 1;
    if (str == "sin")
        return 1;
    if (str == "tan")
        return 1;
    if (str == "icos")
        return 1;
    if (str == "isin")
        return 1;
    if (str == "itan")
        return 1;
    if (str == "sec")
        return 1;
    if (str == "csc")
        return 1;
    if (str == "cot")
        return 1;
    if (str == "isec")
        return 1;
    if (str == "icsc")
        return 1;
    if (str == "icot")
        return 1;

    if (str == "pi")
        return 2;
    if (str == "e")
        return 2;
    if (str == "x")
        return 2;

    return 3;
}

mathType parseMathType(std::string str) {
    if (str == "+")
        return mathType::add;
    if (str == "-")
        return mathType::minus;
    if (str == "*")
        return mathType::times;
    if (str == "/")
        return mathType::divide;

    return mathType::invalid;
}

mathFunc parseMathFunc(std::string str) {
    if (str == "log")
        return mathFunc::log;
    if (str == "nthroot")
        return mathFunc::nthroot;
    if (str == "^" || str == "pow")
        return mathFunc::pow;
    if (str == "ln")
        return mathFunc::ln;
    if (str == "sqrt")
        return mathFunc::sqrt;
    if (str == "cos")
        return mathFunc::cos;
    if (str == "sin")
        return mathFunc::sin;
    if (str == "tan")
        return mathFunc::tan;
    if (str == "icos")
        return mathFunc::icos;
    if (str == "isin")
        return mathFunc::isin;
    if (str == "itan")
        return mathFunc::itan;
    if (str == "sec")
        return mathFunc::sec;
    if (str == "csc")
        return mathFunc::csc;
    if (str == "cot")
        return mathFunc::cot;
    if (str == "isec")
        return mathFunc::isec;
    if (str == "icsc")
        return mathFunc::icsc;
    if (str == "icot")
        return mathFunc::icot;

    return mathFunc::invalid;
}

constant parseMathConstant(std::string str) {
    if (str == "pi")
        return constant::pi;
    if (str == "e")
        return constant::e;
    if (str == "x")
        return constant::x;

    return constant::invalid;
}

void parseEquation(std::vector<mathStep>& equation, std::string str) {
    bool onNum = false;
    std::string savedStr = "";
    equation.clear();
    mathStep tempStep = mathStep();
    int curLevel = 0;
    bool ABS = false;

    for (char c : str) {
        if (std::isdigit(c) || c == '.') {
            savedStr += c;
            onNum = true;
            continue;
        }
        if (onNum) {
            onNum = false;
            tempStep.num = mathVal(std::stof(savedStr));
            equation.push_back(tempStep);
            tempStep = mathStep();
            tempStep.level = curLevel;
            savedStr.clear();
        }

        if (c == '|') {
            ABS = !ABS;

            if (equation.size() != 0 && equation[equation.size() - 1].func != mathFunc::invalid) {
                equation[equation.size() - 1].ABS = ABS;
                continue;
            }


            if (ABS)
                curLevel++;
            else
                curLevel--;

            tempStep.ABS = true;
            tempStep.group = true;
            if (!ABS)
                tempStep.level = curLevel;
            if (equation.size() != 0 && ABS && tempStep.type == mathType::invalid)
                tempStep.type = mathType::times;

            equation.push_back(tempStep);
            tempStep = mathStep();
            tempStep.level = curLevel;
            continue;
        }

        if (c == '(' || c == '[' || c == '{') {
            if (equation.size() != 0 && equation[equation.size() - 1].func != mathFunc::invalid)
                continue;

            curLevel++;

            tempStep.group = true;
            if (equation.size() != 0 && tempStep.type == mathType::invalid)
                tempStep.type = mathType::times;

            equation.push_back(tempStep);
            tempStep = mathStep();
            tempStep.level = curLevel;
            continue;
        }

        if (c == ')' || c == ']' || c == '}') {
            curLevel--;
            tempStep.level = curLevel;

            tempStep.group = true;
            equation.push_back(tempStep);
            tempStep = mathStep();
            tempStep.level = curLevel;

            continue;
        }


        savedStr += c;
        int type = strMath(savedStr);
        if (type == 3)
            continue;

        switch (type) {
        case 0:
            tempStep.type = parseMathType(savedStr);
            break;
        case 1:
            if (equation.size() != 0 && !equation[equation.size() -1].group && tempStep.type == mathType::invalid)
                tempStep.type = mathType::times;
            tempStep.func = parseMathFunc(savedStr);
            tempStep.group = true;
            curLevel++;
            equation.push_back(tempStep);
            tempStep = mathStep();
            tempStep.level = curLevel;
            break;
        case 2:
            if (equation.size() != 0 && !equation[equation.size() - 1].group && tempStep.type == mathType::invalid)
                tempStep.type = mathType::times;
            tempStep.num = parseMathConstant(savedStr);
            equation.push_back(tempStep);
            tempStep = mathStep();
            tempStep.level = curLevel;
            break;
        default:
            continue;
            break;
        }
        savedStr.clear();
    }

    if (onNum) {
        tempStep.num = mathVal(std::stof(savedStr));
        equation.push_back(tempStep);
    }
}
