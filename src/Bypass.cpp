#include "plugin.hpp"

struct Bypass : Module {
  /* START GENERATED - DO NOT EDIT: Enums */
  enum InputIds {
    IN1,
    IN2,
    IN3,
    IN4,
    IN5,
    IN6,
    IN7,
    RETURN1,
    RETURN2,
    RETURN3,
    RETURN4,
    RETURN5,
    RETURN6,
    RETURN7,
    RETURN8,

  NUM_INPUTS
};

  enum OutputIds {
    OUT1,
    OUT2,
    OUT3,
    OUT4,
    OUT5,
    OUT6,
    OUT7,
    OUT8,
    SEND1,
    SEND2,
    SEND3,
    SEND4,
    SEND5,
    SEND6,
    SEND7,

  NUM_OUTPUTS
};

  enum ParamIds {
    BUTTON1,
    BUTTON2,
    BUTTON3,
    BUTTON5,
    BUTTON7,

  NUM_PARAMS
};

  enum LightIds {
    LIGHT1,
    LIGHT2,
    LIGHT3,
    LIGHT5,
    LIGHT7,

  NUM_LIGHTS
};
  /* END GENERATED: Enums */

  Bypass() {
    config(NUM_PARAMS, NUM_INPUTS, NUM_OUTPUTS, NUM_LIGHTS);
    /* START GENERATED - PARAMS EDITABLE: Param Config */
    configSwitch(BUTTON1, 0.f, 1.f, 0.f, "#1", {"Bypass", "Send"});
    configSwitch(BUTTON2, 0.f, 1.f, 0.f, "#2", {"Bypass", "Send"});
    configSwitch(BUTTON3, 0.f, 1.f, 0.f, "#3", {"Bypass", "Send"});
    configSwitch(BUTTON5, 0.f, 1.f, 0.f, "#4", {"Bypass", "Send"});
    configSwitch(BUTTON7, 0.f, 1.f, 0.f, "#5", {"Bypass", "Send"});
    /* END GENERATED: Param Config */
    configInput(IN1, "#1");
    configInput(IN2, "#2");
    configInput(IN3, "#3 L");
    configInput(IN4, "#3 R");
    configInput(IN5, "#4 L");
    configInput(IN6, "#4 R");
    configInput(IN7, "#5");
    configOutput(SEND1, "#1");
    configOutput(SEND2, "#2");
    configOutput(SEND3, "#3 L");
    configOutput(SEND4, "#3 R");
    configOutput(SEND5, "#4 L");
    configOutput(SEND6, "#4 R");
    configOutput(SEND7, "#5");
    configInput(RETURN1, "#1 return");
    configInput(RETURN2, "#2 return");
    configInput(RETURN3, "#3 L return");
    configInput(RETURN4, "#3 R return");
    configInput(RETURN5, "#4 L return");
    configInput(RETURN6, "#4 R return");
    configInput(RETURN7, "#5 L return");
    configInput(RETURN8, "#5 R return");
    configOutput(OUT1, "#1");
    configOutput(OUT2, "#2");
    configOutput(OUT3, "#3 L");
    configOutput(OUT4, "#3 R");
    configOutput(OUT5, "#4 L");
    configOutput(OUT6, "#4 R");
    configOutput(OUT7, "#5 L");
    configOutput(OUT8, "#5 R");
    configBypass(IN1, OUT1);
    configBypass(IN2, OUT2);
    configBypass(IN3, OUT3);
    configBypass(IN4, OUT4);
    configBypass(IN5, OUT5);
    configBypass(IN6, OUT6);
    configBypass(IN7, OUT7);
    onReset();
  }

	bool state[NUM_PARAMS];

  void route(int input, int output, float multiplier = 1.0) {
    assert(input < NUM_INPUTS);
    assert(output < NUM_OUTPUTS);
    if (inputs[input].isConnected() && outputs[output].isConnected()) {
      int channels = inputs[input].getChannels();
      for (int c = 0; c < channels; c += 4) {
        simd::float_4 out = simd::float_4::load(inputs[input].getVoltages(c));

        outputs[output].setVoltageSimd(out * multiplier, c);
      }
    } else {
      if (outputs[output].isConnected()) {
        outputs[output].clearVoltages();
      }
    }
  }

  void process(const ProcessArgs &args) override {
    int NUM_RETURN_BUTTON_MAPPING = 8;
    int returnButtonMapping[8] = { 0, 1, 2, 2, 3, 3, 4, 4};

    for (int i = 0; i < NUM_PARAMS; i++) {
      assert(BUTTON1 + i < NUM_PARAMS);
      state[i] = params[BUTTON1 + i].getValue() > 0.f;

      lights[LIGHT1+i].setBrightness(state[i] ? 1.0 : 0.0f);
    }

    for (int i = 0; i < 8; i++) {
      assert(i < NUM_RETURN_BUTTON_MAPPING);
      assert(returnButtonMapping[i] < NUM_PARAMS);
      if (state[returnButtonMapping[i]]) {
        if (i < 7) {
          route(IN1+i, SEND1+i);
        }
        route(RETURN1+i, OUT1+i);
      } else {
        if (i == 6) {
          // Mono -> Stereo case
          // 0.71 = sin(45deg)
          // This keeps overall combined power of output signal constant.
          route(IN1+i, OUT1+i, 0.71);
          route(IN1+i, OUT1+i+1, 0.71);
        } else if (i == 7) {
          // Ignore, was handled by previous loop
        } else {
          route(IN1+i, OUT1+i);
        }

        // Clear send. Ignore i == 7 because handled by previous loop.
        if (i < 7) {
          assert(SEND1+i < NUM_OUTPUTS);
          if (outputs[SEND1+i].isConnected()) {
            outputs[SEND1+i].clearVoltages();
          }
        }
      }

      int channels = 1;
      if (i < 7) {
        assert(IN1+i < NUM_INPUTS);
        assert(SEND1+i < NUM_OUTPUTS);
        channels = inputs[IN1+i].getChannels();
        outputs[SEND1+i].setChannels(channels);
      } else {
        assert(IN1+6 < NUM_INPUTS);
        channels = inputs[IN1+6].getChannels();
      }

      assert(i < NUM_RETURN_BUTTON_MAPPING);
      assert(returnButtonMapping[i] < NUM_PARAMS);
      if (state[returnButtonMapping[i]]) {
        assert(RETURN1+i < NUM_INPUTS);
        assert(OUT1+i < NUM_OUTPUTS);
        channels = inputs[RETURN1+i].getChannels();
        outputs[OUT1+i].setChannels(channels);
      } else {
        assert(OUT1+i < NUM_OUTPUTS);
        outputs[OUT1+i].setChannels(channels);
      }
    }
  }

	void onReset() override {
		for (int i = 0; i < NUM_PARAMS; i++) {
			state[i] = false;
		}
	}

	void onRandomize() override {
		for (int i = 0; i < NUM_PARAMS; i++) {
			state[i] = (random::uniform() < 0.5f);
		}
	}

	json_t *dataToJson() override {
		json_t *rootJ = json_object();

		// states
		json_t *statesJ = json_array();
		for (int i = 0; i < NUM_PARAMS; i++) {
			json_t *stateJ = json_boolean(state[i]);
			json_array_append_new(statesJ, stateJ);
		}
		json_object_set_new(rootJ, "states", statesJ);

		return rootJ;
	}

	void dataFromJson(json_t *rootJ) override {
		// states
		json_t *statesJ = json_object_get(rootJ, "states");
		if (statesJ) {
			for (int i = 0; i < NUM_PARAMS; i++) {
				json_t *stateJ = json_array_get(statesJ, i);
				if (stateJ)
					state[i] = json_boolean_value(stateJ);
			}
		}
	}
};

template <typename BASE>
struct MuteLight : BASE {
	MuteLight() {
		this->box.size = mm2px(Vec(4.f, 4.f));
	}
};

static const NVGcolor RETRO_RED = nvgRGB(0xf0, 0x59, 0x2d);

struct RetroLight : app::ModuleLightWidget {
  RetroLight() {
		bgColor = nvgRGB(0xfd, 0xf0, 0xc4);
		borderColor = nvgRGB(0x9b, 0x5a, 0x49);
    addBaseColor(RETRO_RED);
	}
};

struct RetroButton : app::SvgSwitch {
	app::ModuleLightWidget* light;

  RetroButton() {
    momentary = false;
		this->box.size = mm2px(Vec(4.f, 4.f));
    addFrame(APP->window->loadSvg(asset::plugin(pluginInstance, "res/LEDBezel.svg")));
		light = new LEDBezelLight<RetroLight>;
		light->box.pos = box.size.div(2).minus(light->box.size.div(2));
		addChild(light);
  }
  app::ModuleLightWidget* getLight() { return light; }
};

struct BypassWidget : ModuleWidget {
  BypassWidget(Bypass *module) {
    setModule(module);
    setPanel(APP->window->loadSvg(asset::plugin(pluginInstance, "res/Bypass.svg")));

		addChild(createWidget<ScrewSilver>(Vec(15, 0)));
		addChild(createWidget<ScrewSilver>(Vec(box.size.x - 30, 0)));
		addChild(createWidget<ScrewSilver>(Vec(15, 365)));
		addChild(createWidget<ScrewSilver>(Vec(box.size.x - 30, 365)));

    /* START GENERATED - DO NOT EDIT: Add Components */
    addParam(createLightParamCentered<RetroButton>(mm2px(Vec(33.020000, 39.109344)), module, Bypass::BUTTON1, Bypass::LIGHT1));
    addParam(createLightParamCentered<RetroButton>(mm2px(Vec(33.020000, 49.843910)), module, Bypass::BUTTON2, Bypass::LIGHT2));
    addParam(createLightParamCentered<RetroButton>(mm2px(Vec(32.953182, 65.945671)), module, Bypass::BUTTON3, Bypass::LIGHT3));
    addParam(createLightParamCentered<RetroButton>(mm2px(Vec(32.831013, 87.414726)), module, Bypass::BUTTON5, Bypass::LIGHT5));
    addParam(createLightParamCentered<RetroButton>(mm2px(Vec(32.831013, 108.883780)), module, Bypass::BUTTON7, Bypass::LIGHT7));
    addInput(createInputCentered<PJ301MPort>(mm2px(Vec(6.237754, 39.109362)), module, Bypass::IN1));
    addInput(createInputCentered<PJ301MPort>(mm2px(Vec(6.237754, 49.843886)), module, Bypass::IN2));
    addInput(createInputCentered<PJ301MPort>(mm2px(Vec(6.237754, 60.578420)), module, Bypass::IN3));
    addInput(createInputCentered<PJ301MPort>(mm2px(Vec(6.237754, 71.312945)), module, Bypass::IN4));
    addInput(createInputCentered<PJ301MPort>(mm2px(Vec(6.237754, 82.047476)), module, Bypass::IN5));
    addInput(createInputCentered<PJ301MPort>(mm2px(Vec(6.237754, 92.782001)), module, Bypass::IN6));
    addInput(createInputCentered<PJ301MPort>(mm2px(Vec(6.237754, 103.516534)), module, Bypass::IN7));
    addOutput(createOutputCentered<PJ301MPort>(mm2px(Vec(56.812223, 39.109362)), module, Bypass::OUT1));
    addOutput(createOutputCentered<PJ301MPort>(mm2px(Vec(56.812223, 49.843886)), module, Bypass::OUT2));
    addOutput(createOutputCentered<PJ301MPort>(mm2px(Vec(56.812223, 60.578420)), module, Bypass::OUT3));
    addOutput(createOutputCentered<PJ301MPort>(mm2px(Vec(56.812223, 71.312945)), module, Bypass::OUT4));
    addOutput(createOutputCentered<PJ301MPort>(mm2px(Vec(56.812223, 82.047476)), module, Bypass::OUT5));
    addOutput(createOutputCentered<PJ301MPort>(mm2px(Vec(56.812223, 92.782001)), module, Bypass::OUT6));
    addOutput(createOutputCentered<PJ301MPort>(mm2px(Vec(56.812223, 103.516534)), module, Bypass::OUT7));
    addOutput(createOutputCentered<PJ301MPort>(mm2px(Vec(56.812223, 114.251060)), module, Bypass::OUT8));
    addInput(createInputCentered<PJ301MPort>(mm2px(Vec(43.110273, 39.109362)), module, Bypass::RETURN1));
    addInput(createInputCentered<PJ301MPort>(mm2px(Vec(43.110273, 49.843886)), module, Bypass::RETURN2));
    addInput(createInputCentered<PJ301MPort>(mm2px(Vec(43.110273, 60.578420)), module, Bypass::RETURN3));
    addInput(createInputCentered<PJ301MPort>(mm2px(Vec(43.110273, 71.312945)), module, Bypass::RETURN4));
    addInput(createInputCentered<PJ301MPort>(mm2px(Vec(43.110273, 82.047476)), module, Bypass::RETURN5));
    addInput(createInputCentered<PJ301MPort>(mm2px(Vec(43.110273, 92.782001)), module, Bypass::RETURN6));
    addInput(createInputCentered<PJ301MPort>(mm2px(Vec(43.110273, 103.516534)), module, Bypass::RETURN7));
    addInput(createInputCentered<PJ301MPort>(mm2px(Vec(43.110273, 114.251060)), module, Bypass::RETURN8));
    addOutput(createOutputCentered<PJ301MPort>(mm2px(Vec(19.939704, 39.109362)), module, Bypass::SEND1));
    addOutput(createOutputCentered<PJ301MPort>(mm2px(Vec(19.939704, 49.843886)), module, Bypass::SEND2));
    addOutput(createOutputCentered<PJ301MPort>(mm2px(Vec(19.939704, 60.578420)), module, Bypass::SEND3));
    addOutput(createOutputCentered<PJ301MPort>(mm2px(Vec(19.939704, 71.312945)), module, Bypass::SEND4));
    addOutput(createOutputCentered<PJ301MPort>(mm2px(Vec(19.939704, 82.047476)), module, Bypass::SEND5));
    addOutput(createOutputCentered<PJ301MPort>(mm2px(Vec(19.939704, 92.782001)), module, Bypass::SEND6));
    addOutput(createOutputCentered<PJ301MPort>(mm2px(Vec(19.939704, 103.516534)), module, Bypass::SEND7));
    /* END GENERATED: Add Components */
  }
};

Model *modelBypass = createModel<Bypass, BypassWidget>("Bypass");
