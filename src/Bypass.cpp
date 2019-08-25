#include "plugin.hpp"

struct Bypass : Module {
	enum ParamIds {
    ENUMS(SEND_PARAM,10),
		NUM_PARAMS
	};
	enum InputIds {
    ENUMS(IN_INPUT,10),
    ENUMS(RETURN_INPUT,10),
		NUM_INPUTS
	};
	enum OutputIds {
    ENUMS(OUT_OUTPUT,10),
    ENUMS(SEND_OUTPUT,10),
		NUM_OUTPUTS
	};
	enum LightIds {
    ENUMS(SEND_LIGHT, 10),
		NUM_LIGHTS
	};

	bool state[10];
	dsp::BooleanTrigger sendTrigger[10];

	Bypass() {
		config(NUM_PARAMS, NUM_INPUTS, NUM_OUTPUTS, NUM_LIGHTS);
    for (int i = 0; i < 10; i++) {
      configParam(SEND_PARAM + i, 0.f, 1.f, 0.f, "");
    }
    onReset();
	}

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
    for (int i = 0; i < 10; i++) {
      if (sendTrigger[i].process(params[SEND_PARAM + i].getValue() > 0.f))
        state[i] ^= true;

      if (state[i]) {
        route(IN_INPUT+i, SEND_OUTPUT+i);
        route(RETURN_INPUT+i, OUT_OUTPUT+i);
      } else {
        route(IN_INPUT+i, OUT_OUTPUT+i);

        // Clear send
        if (outputs[SEND_OUTPUT+i].isConnected()) {
          outputs[SEND_OUTPUT+i].clearVoltages();
        }
      }

      lights[SEND_LIGHT+i].setBrightness(state[i] ? 0.9 : 0.0f);
    }
	}

	void onReset() override {
		for (int i = 0; i < 10; i++) {
			state[i] = false;
		}
	}

	void onRandomize() override {
		for (int i = 0; i < 10; i++) {
			state[i] = (random::uniform() < 0.5f);
		}
	}
};

template <typename BASE>
struct MuteLight : BASE {
	MuteLight() {
		this->box.size = mm2px(Vec(6.f, 6.f));
	}
};

struct BypassWidget : ModuleWidget {
	BypassWidget(Bypass *module) {
		setModule(module);
		setPanel(APP->window->loadSvg(asset::plugin(pluginInstance, "res/Bypass.svg")));

		addChild(createWidget<ScrewSilver>(Vec(RACK_GRID_WIDTH, 0)));
		addChild(createWidget<ScrewSilver>(Vec(box.size.x - 2 * RACK_GRID_WIDTH, 0)));
		addChild(createWidget<ScrewSilver>(Vec(RACK_GRID_WIDTH, RACK_GRID_HEIGHT - RACK_GRID_WIDTH)));
		addChild(createWidget<ScrewSilver>(Vec(box.size.x - 2 * RACK_GRID_WIDTH, RACK_GRID_HEIGHT - RACK_GRID_WIDTH)));

    const float yOffset = 9.95f;

    for (int i = 0; i < 10; i++) {
      addInput(createInput<PJ301MPort>(mm2px(Vec(4.055, 17.8 + (yOffset * i))), module, Bypass::IN_INPUT+i));
      addOutput(createOutput<PJ301MPort>(mm2px(Vec(15.214, 17.8 + (yOffset * i))), module, Bypass::SEND_OUTPUT+i));

      addParam(createParam<LEDBezel>(mm2px(Vec(26.673, 18.106 + (yOffset * i))), module, Bypass::SEND_PARAM+i));
      addChild(createLight<MuteLight<BlueLight>>(mm2px(Vec(27.423, 18.856 + (yOffset * i))), module, Bypass::SEND_LIGHT+i));

      addInput(createInput<PJ301MPort>(mm2px(Vec(37.533, 17.8 + (yOffset * i))), module, Bypass::RETURN_INPUT+i));
      addOutput(createOutput<PJ301MPort>(mm2px(Vec(48.692, 17.8 + (yOffset * i))), module, Bypass::OUT_OUTPUT+i));
    }
	}
};

Model *modelBypass = createModel<Bypass, BypassWidget>("Bypass");
