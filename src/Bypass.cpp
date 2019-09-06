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
    IN8,
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
    SEND8,

  NUM_OUTPUTS
};

  enum ParamIds {
    BUTTON1,
    BUTTON2,
    BUTTON3,
    BUTTON4,
    BUTTON5,
    BUTTON6,
    BUTTON7,
    BUTTON8,

  NUM_PARAMS
};

  enum LightIds {
    LIGHT1,
    LIGHT2,
    LIGHT3,
    LIGHT4,
    LIGHT5,
    LIGHT6,
    LIGHT7,
    LIGHT8,

  NUM_LIGHTS
};
  /* END GENERATED: Enums */

  Bypass() {
    config(NUM_PARAMS, NUM_INPUTS, NUM_OUTPUTS, NUM_LIGHTS);
    /* START GENERATED - PARAMS EDITABLE: Param Config */
    configParam(BUTTON1, 0.f, 1.f, 0.f, "Channel 1 Send");
    configParam(BUTTON2, 0.f, 1.f, 0.f, "Channel 2 Send");
    configParam(BUTTON3, 0.f, 1.f, 0.f, "Channel 3 Send");
    configParam(BUTTON4, 0.f, 1.f, 0.f, "Channel 4 Send");
    configParam(BUTTON5, 0.f, 1.f, 0.f, "Channel 5 Send");
    configParam(BUTTON6, 0.f, 1.f, 0.f, "Channel 6 Send");
    configParam(BUTTON7, 0.f, 1.f, 0.f, "Channel 7 Send");
    configParam(BUTTON8, 0.f, 1.f, 0.f, "Channel 8 Send");
    /* END GENERATED: Param Config */
    onReset();
  }

	bool state[NUM_PARAMS];
	dsp::BooleanTrigger sendTrigger[NUM_PARAMS];

  void route(int input, int output) {
		float out[16] = {};
		int channels = 1;

    if (inputs[input].isConnected()) {
      channels = inputs[input].getChannels();
      inputs[input].readVoltages(out);

      if (outputs[output].isConnected()) {
        outputs[output].setChannels(channels);
        outputs[output].writeVoltages(out);
      }
    } else {
      if (outputs[output].isConnected()) {
        outputs[output].clearVoltages();
      }
    }
  }

  void process(const ProcessArgs &args) override {
    for (int i = 0; i < NUM_PARAMS; i++) {
      if (sendTrigger[i].process(params[BUTTON1 + i].getValue() > 0.f))
        state[i] ^= true;

      if (state[i]) {
        route(IN1+i, SEND1+i);
        route(RETURN1+i, OUT1+i);
      } else {
        route(IN1+i, OUT1+i);

        // Clear send
        if (outputs[SEND1+i].isConnected()) {
          outputs[SEND1+i].clearVoltages();
        }
      }

      lights[LIGHT1+i].setBrightness(state[i] ? 0.9 : 0.0f);
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
  RetroButton() {
    momentary = true;
		this->box.size = mm2px(Vec(4.f, 4.f));
    addFrame(APP->window->loadSvg(asset::plugin(pluginInstance, "res/LEDBezel.svg")));
  }
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
    addParam(createParamCentered<RetroButton>(mm2px(Vec(33.020000, 39.109344)), module, Bypass::BUTTON1));
    addParam(createParamCentered<RetroButton>(mm2px(Vec(33.020000, 49.843910)), module, Bypass::BUTTON2));
    addParam(createParamCentered<RetroButton>(mm2px(Vec(33.020000, 60.578442)), module, Bypass::BUTTON3));
    addParam(createParamCentered<RetroButton>(mm2px(Vec(33.020000, 71.312965)), module, Bypass::BUTTON4));
    addParam(createParamCentered<RetroButton>(mm2px(Vec(33.020000, 82.047501)), module, Bypass::BUTTON5));
    addParam(createParamCentered<RetroButton>(mm2px(Vec(33.020000, 92.782021)), module, Bypass::BUTTON6));
    addParam(createParamCentered<RetroButton>(mm2px(Vec(33.020000, 103.516560)), module, Bypass::BUTTON7));
    addParam(createParamCentered<RetroButton>(mm2px(Vec(33.020000, 114.251080)), module, Bypass::BUTTON8));
    addInput(createInputCentered<PJ301MPort>(mm2px(Vec(6.237754, 39.109362)), module, Bypass::IN1));
    addInput(createInputCentered<PJ301MPort>(mm2px(Vec(6.237754, 49.843886)), module, Bypass::IN2));
    addInput(createInputCentered<PJ301MPort>(mm2px(Vec(6.237754, 60.578420)), module, Bypass::IN3));
    addInput(createInputCentered<PJ301MPort>(mm2px(Vec(6.237754, 71.312945)), module, Bypass::IN4));
    addInput(createInputCentered<PJ301MPort>(mm2px(Vec(6.237754, 82.047476)), module, Bypass::IN5));
    addInput(createInputCentered<PJ301MPort>(mm2px(Vec(6.237754, 92.782001)), module, Bypass::IN6));
    addInput(createInputCentered<PJ301MPort>(mm2px(Vec(6.237754, 103.516534)), module, Bypass::IN7));
    addInput(createInputCentered<PJ301MPort>(mm2px(Vec(6.237754, 114.251060)), module, Bypass::IN8));
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
    addOutput(createOutputCentered<PJ301MPort>(mm2px(Vec(19.939704, 114.251060)), module, Bypass::SEND8));
    addChild(createLightCentered<MuteLight<RetroLight>>(mm2px(Vec(33.020000, 39.109344)), module, Bypass::LIGHT1));
    addChild(createLightCentered<MuteLight<RetroLight>>(mm2px(Vec(33.020000, 49.843910)), module, Bypass::LIGHT2));
    addChild(createLightCentered<MuteLight<RetroLight>>(mm2px(Vec(33.020000, 60.578442)), module, Bypass::LIGHT3));
    addChild(createLightCentered<MuteLight<RetroLight>>(mm2px(Vec(33.020000, 71.312965)), module, Bypass::LIGHT4));
    addChild(createLightCentered<MuteLight<RetroLight>>(mm2px(Vec(33.020000, 82.047501)), module, Bypass::LIGHT5));
    addChild(createLightCentered<MuteLight<RetroLight>>(mm2px(Vec(33.020000, 92.782021)), module, Bypass::LIGHT6));
    addChild(createLightCentered<MuteLight<RetroLight>>(mm2px(Vec(33.020000, 103.516560)), module, Bypass::LIGHT7));
    addChild(createLightCentered<MuteLight<RetroLight>>(mm2px(Vec(33.020000, 114.251080)), module, Bypass::LIGHT8));
    /* END GENERATED: Add Components */
  }
};

Model *modelBypass = createModel<Bypass, BypassWidget>("Bypass");
