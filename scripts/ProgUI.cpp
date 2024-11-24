#define _PROG_UI
#include "ProgUI.h"

Object* UIFPSObj = nullptr;
TextBox* UIFPSScr = nullptr;

namespace ProgUI {
    void start() {
        UIFPSObj = createObj("square");
        UIFPSScr = new TextBox;

        UIFPSObj->transform.scale = { 0.3, 0.075, 1 };
        UIFPSObj->transform.position = { _screenRatio - UIFPSObj->transform.scale.x, 1 - UIFPSObj->transform.scale.y, 1 };
        UIFPSObj->transform.position.z = 0.85f;

        UIFPSScr->text = "FPS : ";
        UIFPSScr->font = "CascadiaCode_BIG";
        UIFPSScr->fontSize = 1;
        addObjScript(UIFPSObj, UIFPSScr);

        UIFPSObj->active = false;
    }
    void update() {
        if (UIFPSObj->active) {
            UIFPSScr->text = "FPS : " + std::to_string((int)(1 / _deltaTime));
            UIFPSScr->textUpdate();
        }
    }
}

namespace progUI{
    void fps(bool val) {
        UIFPSObj->active = val;
    }
    void fps() {
        UIFPSObj->active = !UIFPSObj->active;
    }
}