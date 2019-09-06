#include "plugin.hpp"

struct Bypass2 : Module {
  /* START GENERATED - DO NOT EDIT: Enums */
  enum InputIds {
    IN1,
    RET1,

    NUM_INPUTS
  };

  enum OutputIds {
    OUT1,
    SEND1,

    NUM_OUTPUTS
  };

  enum ParamIds {
    BYPASS1,

    NUM_PARAMS
  };

  enum LightIds {
    LIGHT1,

    NUM_LIGHTS
  };
  /* END GENERATED: Enums */

  Bypass2() {
    config(NUM_PARAMS, NUM_INPUTS, NUM_OUTPUTS, NUM_LIGHTS);
    /* START GENERATED - PARAMS EDITABLE: Param Config */
    configParam(BYPASS1, 0.f, 1.f, 0.f, "");
    /* END GENERATED: Param Config */
  }

  void process(const ProcessArgs &args) override {
  }
};

template <typename BASE>
struct MuteLight : BASE {
	MuteLight() {
		this->box.size = mm2px(Vec(6.f, 6.f));
	}
};

struct Bypass2Widget : ModuleWidget {
  Bypass2Widget(Bypass2 *module) {
    setModule(module);
    setPanel(APP->window->loadSvg(asset::plugin(pluginInstance, "res/Bypass2.svg")));

    /* START GENERATED - DO NOT EDIT: Add Components */
    addInput(createInputCentered<PJ301MPort>(mm2px(Vec(6.237754, 39.109354)), module, Bypass2::IN1));
    addInput(createInputCentered<PJ301MPort>(mm2px(Vec(43.110273, 39.109354)), module, Bypass2::RET1));
    addOutput(createOutputCentered<PJ301MPort>(mm2px(Vec(56.812223, 39.109354)), module, Bypass2::OUT1));
    addOutput(createOutputCentered<PJ301MPort>(mm2px(Vec(18.881370, 39.109354)), module, Bypass2::SEND1));
    addParam(createParamCentered<LEDBezel>(mm2px(Vec(32.583322, 39.109354)), module, Bypass2::BYPASS1));
    addChild(createLightCentered<MuteLight<RedLight>>(mm2px(Vec(32.583324, 39.109337)), module, Bypass2::LIGHT1));
    /* END GENERATED: Add Components */
  }
};

Model *modelBypass2 = createModel<Bypass2, Bypass2Widget>("Bypass2");
