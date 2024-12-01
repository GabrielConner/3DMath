#pragma once
#include "Objects.h"
#include <iostream>
#include "flowControl.h"
#include "input.h"
#include "globalVars.h"
#include <cctype>
#include "globalScripts.h"
#include <iomanip>
#include <sstream>

enum struct mathType { invalid = -1, add, minus, times, divide, };
enum struct mathFunc { invalid = -1, log, nthroot, pow, ln, sqrt, cos, sin, tan, icos, isin, itan, sec, csc, cot, isec, icsc, icot };
enum struct constant { invalid = -1, pi, e, x };

struct mathVal {
    constant val;
    float num;

    float get(float xVal) {
        if (val == constant::invalid)
            return num;

        switch (val) {
        case constant::pi:
            return _pi;
        case constant::e:
            return _e;
        case constant::x:
            return xVal;
        }
    }

    mathVal() {
        val = constant::invalid;
        num = 0;
    }
    mathVal(constant v) {
        val = v;
        num = 0;
    }
    mathVal(float n) {
        val = constant::invalid;
        num = n;
    }
    mathVal(constant v, float n) {
        val = v;
        num = n;
    }
};

struct mathStep {
    mathType type;
    mathFunc func;
    bool ABS;
    int level;
    bool group;
    bool gotHelper;

    mathVal num;
    mathVal helper;

    mathStep() {
        type = mathType::invalid;
        func = mathFunc::invalid;
        ABS = false;
        level = 0;
        group = false;
        gotHelper = false;
    }
    mathStep(mathType T, mathFunc F, bool B, int L, bool G, bool GH, mathVal N, mathVal H) {
        type = T;
        func = F;
        ABS = B;
        level = L;
        group = G;
        gotHelper = GH;
        num = N;
        helper = H;
    }

    float solveFunc(float xVal) {
        float ret = 0;
        float useNum = num.get(xVal);
        float useHelp = helper.get(xVal);

        switch (func) {
        case mathFunc::invalid:
            return useNum;
        case mathFunc::log:
            return log(useNum) / log(useHelp);
        case mathFunc::ln:
            return log(useNum);
        case mathFunc::sqrt:
            return pow(useNum, 0.5f);
        case mathFunc::cos:
            return cos(useNum);
        case mathFunc::sin:
            return sin(useNum);
        case mathFunc::tan:
            return tan(useNum);
        case mathFunc::icos:
            return acos(useNum);
        case mathFunc::isin:
            return asin(useNum);
        case mathFunc::itan:
            return atan(useNum);
        case mathFunc::sec:
            return 1 / cos(useNum);
        case mathFunc::csc:
            return 1 / sin(useNum);
        case mathFunc::cot:
            return 1 / tan(useNum);
        case mathFunc::isec:
            return acos(1 / useNum);
        case mathFunc::icsc:
            return asin(1 / useNum);
        case mathFunc::icot:
            return atan(1 / useNum);
        case mathFunc::nthroot:
            return pow(useNum, 1 / useHelp);
        case mathFunc::pow:
            return pow(useNum, useHelp);
        default:
            return useNum;
        }
    }

    float solveStep(float xVal, float curVal) {
        float stepVal = solveFunc(xVal);

        switch (type) {
        case mathType::invalid:
            return stepVal;
        case mathType::add:
            return curVal + stepVal;
        case mathType::minus:
            return curVal - stepVal;
        case mathType::times:
            return curVal * stepVal;
        case mathType::divide:
            return curVal / stepVal;
        default:
            return curVal;
        }
    }

    bool funcNeedsHelper() {
        if (gotHelper)
            return false;

        if (func >= mathFunc::log && func <= mathFunc::pow)
            return true;
        return false;
    }

    static bool lessThan(mathType a, mathType b) {
        if (a == mathType::invalid)
            return true;
        if (a < mathType::times)
            if (b >= mathType::times)
                return true;
            else
                return false;
        return false;
    }

    static float solve(std::vector<mathStep>::iterator steps, int length, float xVal, int stepMin) {
        float ret = 0;

        for (int i = 0; i < length; i++) {
            if (steps[i].group) {
                int gStart = i + 1;
                int gLength = 0;
                int add = 0;

                bool foundEnd = false;
                for (int find = gStart; find < length; find++) {
                    if (steps[find].level <= stepMin && steps[find].group) {
                        gLength = find - gStart;
                        foundEnd = true;
                        break;
                    }
                }
                if (!foundEnd)
                    gLength = length - gStart;

                steps[i].num = steps[i].ABS ? abs(solve(steps + gStart, gLength, xVal, stepMin + 1)) : solve(steps + gStart, gLength, xVal, stepMin + 1);

                if (i < length - 1 && steps[i].funcNeedsHelper() && steps[i + gLength + 1].group) {
                    add = gLength + 2;
                    foundEnd = false;
                    gStart = i + gLength + 3;
                    for (int find = gStart; find < length; find++) {
                        if (steps[find].level <= stepMin && steps[find].group) {
                            gLength = find - gStart;
                            foundEnd = true;
                            break;
                        }
                    }
                    if (!foundEnd)
                        gLength = length - gStart;

                    steps[i].helper = steps[i].ABS ? -solve(steps + gStart, gLength, xVal, stepMin + 1) : solve(steps + gStart, gLength, xVal, stepMin + 1);
                }

                if (i == 0) {
                    ret = steps[i].solveFunc(xVal);
                    if (steps[i].type == mathType::minus)
                        ret = -ret;
                }
                else if (i >= length - 1) {
                    ret = steps[i].solveStep(xVal, ret);
                }
                else if (length > i + gLength + add + 2 && lessThan(steps[i].type, steps[i + gLength + add + 2].type)) {
                    steps[i].num = steps[i + gLength + add + 2].solveStep(xVal, steps[i].num.get(xVal));
                    ret = steps[i].solveStep(xVal, ret);
                    i++;
                }
                else
                    ret = steps[i].solveStep(xVal, ret);

                i += gLength + 1 + add;
                continue;
            }

            if (i == 0) {
                ret = steps[i].solveFunc(xVal);
                if (steps[i].type == mathType::minus)
                    ret = -ret;
                continue;
            }
            if (i >= length - 1) {
                ret = steps[i].solveStep(xVal, ret);
                break;
            }

            if (lessThan(steps[i].type, steps[i + 1].type)) {
                steps[i].num = steps[i + 1].solveStep(xVal, steps[i].solveFunc(xVal));
                ret = steps[i].solveStep(xVal, ret);
                i++;
                continue;
            }

            ret = steps[i].solveStep(xVal, ret);
        }
        return ret;
    }
};

void parseEquation(std::vector<mathStep>& equation, std::string str);
void getGraphPoints(std::vector<Vec2>& points, std::vector<mathStep> graph, uint steps, float min, float max);
bool createTube(float tubeSize, float*& vertArr, uint*& indiceArr, size_t& vertSize, size_t& indiceSize, uint& triCount, uint resolution, std::vector<Vec2> points);
bool createGraphMesh(float* vertices, uint size, uint pPerCircle, uint pPerLine, uint*& indiceArr, size_t& indiceSize, uint& triCount);
std::pair<float*, uint> rotateGraph(std::vector<Vec2> points, Vec3 axis, float endAngle, uint steps);
void createGraphVolume();
void createGraphShape();