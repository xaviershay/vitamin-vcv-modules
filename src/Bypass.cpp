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

		addParam(createParam<LEDBezel>(mm2px(Vec(26.673, 18.106)), module, Bypass::SEND_PARAM+0));
		addParam(createParam<LEDBezel>(mm2px(Vec(26.73, 28.337)), module, Bypass::SEND_PARAM+1));
		addParam(createParam<LEDBezel>(mm2px(Vec(26.73, 38.51)), module, Bypass::SEND_PARAM+2));
		addParam(createParam<LEDBezel>(mm2px(Vec(26.73, 48.683)), module, Bypass::SEND_PARAM+3));
		addParam(createParam<LEDBezel>(mm2px(Vec(26.73, 58.855)), module, Bypass::SEND_PARAM+4));
		addParam(createParam<LEDBezel>(mm2px(Vec(26.73, 69.028)), module, Bypass::SEND_PARAM+5));
		addParam(createParam<LEDBezel>(mm2px(Vec(26.73, 79.201)), module, Bypass::SEND_PARAM+6));
		addParam(createParam<LEDBezel>(mm2px(Vec(26.73, 89.373)), module, Bypass::SEND_PARAM+7));
		addParam(createParam<LEDBezel>(mm2px(Vec(26.73, 99.546)), module, Bypass::SEND_PARAM+8));
		addParam(createParam<LEDBezel>(mm2px(Vec(26.73, 109.719)), module, Bypass::SEND_PARAM+9));

    for (int i = 0; i < 10; i++) {
      addInput(createInput<PJ301MPort>(mm2px(Vec(4.055, 17.8 + (10.173 * i))), module, Bypass::IN_INPUT+i));
      addOutput(createOutput<PJ301MPort>(mm2px(Vec(15.214, 17.8 + (10.173 * i))), module, Bypass::SEND_OUTPUT+i));
      addInput(createInput<PJ301MPort>(mm2px(Vec(37.533, 17.8 + (10.173 * i))), module, Bypass::RETURN_INPUT+i));
      addOutput(createOutput<PJ301MPort>(mm2px(Vec(48.692, 17.8 + (10.173 * i))), module, Bypass::OUT_OUTPUT+i));
    }
    /*
		addInput(createInput<PJ301MPort>(mm2px(Vec(15.214, 27.936)), module, Bypass::RECT56657_2_41_INPUT));
		addInput(createInput<PJ301MPort>(mm2px(Vec(4.055, 27.982)), module, Bypass::RECT56657_3_INPUT));
		addInput(createInput<PJ301MPort>(mm2px(Vec(15.214, 38.109)), module, Bypass::RECT56657_2_2_INPUT));
		addInput(createInput<PJ301MPort>(mm2px(Vec(4.055, 38.155)), module, Bypass::RECT56657_9_INPUT));
		addInput(createInput<PJ301MPort>(mm2px(Vec(15.214, 48.282)), module, Bypass::RECT56657_2_42_INPUT));
		addInput(createInput<PJ301MPort>(mm2px(Vec(4.055, 48.328)), module, Bypass::RECT56657_98_INPUT));
		addInput(createInput<PJ301MPort>(mm2px(Vec(15.214, 58.455)), module, Bypass::RECT56657_2_98_INPUT));
		addInput(createInput<PJ301MPort>(mm2px(Vec(4.055, 58.501)), module, Bypass::RECT56657_0_INPUT));
		addInput(createInput<PJ301MPort>(mm2px(Vec(15.214, 68.627)), module, Bypass::RECT56657_2_0_INPUT));
		addInput(createInput<PJ301MPort>(mm2px(Vec(4.055, 68.673)), module, Bypass::RECT56657_1_INPUT));
		addInput(createInput<PJ301MPort>(mm2px(Vec(15.214, 78.8)), module, Bypass::RECT56657_2_1_INPUT));
		addInput(createInput<PJ301MPort>(mm2px(Vec(4.055, 78.846)), module, Bypass::RECT56657_6_INPUT));
		addInput(createInput<PJ301MPort>(mm2px(Vec(15.214, 88.973)), module, Bypass::RECT56657_2_4_INPUT));
		addInput(createInput<PJ301MPort>(mm2px(Vec(4.055, 89.019)), module, Bypass::RECT56657_70_INPUT));
		addInput(createInput<PJ301MPort>(mm2px(Vec(15.214, 99.145)), module, Bypass::RECT56657_2_9_INPUT));
		addInput(createInput<PJ301MPort>(mm2px(Vec(4.055, 99.191)), module, Bypass::RECT56657_62_INPUT));
		addInput(createInput<PJ301MPort>(mm2px(Vec(15.214, 109.318)), module, Bypass::RECT56657_2_5_INPUT));
		addInput(createInput<PJ301MPort>(mm2px(Vec(4.055, 109.364)), module, Bypass::RECT56657_7_INPUT));

		addOutput(createOutput<PJ301MPort>(mm2px(Vec(48.692, 17.81)), module, Bypass::RECT56677_OUTPUT));
		addOutput(createOutput<PJ301MPort>(mm2px(Vec(37.533, 17.946)), module, Bypass::RECT56677_3_OUTPUT));
		addOutput(createOutput<PJ301MPort>(mm2px(Vec(48.692, 27.982)), module, Bypass::RECT56677_60_OUTPUT));
		addOutput(createOutput<PJ301MPort>(mm2px(Vec(37.533, 28.119)), module, Bypass::RECT56677_3_6_OUTPUT));
		addOutput(createOutput<PJ301MPort>(mm2px(Vec(48.692, 38.155)), module, Bypass::RECT56677_0_OUTPUT));
		addOutput(createOutput<PJ301MPort>(mm2px(Vec(37.533, 38.292)), module, Bypass::RECT56677_3_60_OUTPUT));
		addOutput(createOutput<PJ301MPort>(mm2px(Vec(48.692, 48.328)), module, Bypass::RECT56677_34_OUTPUT));
		addOutput(createOutput<PJ301MPort>(mm2px(Vec(37.533, 48.464)), module, Bypass::RECT56677_3_62_OUTPUT));
		addOutput(createOutput<PJ301MPort>(mm2px(Vec(48.692, 58.501)), module, Bypass::RECT56677_6_OUTPUT));
		addOutput(createOutput<PJ301MPort>(mm2px(Vec(37.533, 58.637)), module, Bypass::RECT56677_3_9_OUTPUT));
		addOutput(createOutput<PJ301MPort>(mm2px(Vec(48.692, 68.673)), module, Bypass::RECT56677_28_OUTPUT));
		addOutput(createOutput<PJ301MPort>(mm2px(Vec(37.533, 68.81)), module, Bypass::RECT56677_3_2_OUTPUT));
		addOutput(createOutput<PJ301MPort>(mm2px(Vec(48.692, 78.846)), module, Bypass::RECT56677_4_OUTPUT));
		addOutput(createOutput<PJ301MPort>(mm2px(Vec(37.533, 78.983)), module, Bypass::RECT56677_3_5_OUTPUT));
		addOutput(createOutput<PJ301MPort>(mm2px(Vec(48.692, 89.019)), module, Bypass::RECT56677_8_OUTPUT));
		addOutput(createOutput<PJ301MPort>(mm2px(Vec(37.533, 89.155)), module, Bypass::RECT56677_3_1_OUTPUT));
		addOutput(createOutput<PJ301MPort>(mm2px(Vec(48.692, 99.191)), module, Bypass::RECT56677_25_OUTPUT));
		addOutput(createOutput<PJ301MPort>(mm2px(Vec(37.533, 99.328)), module, Bypass::RECT56677_3_73_OUTPUT));
		addOutput(createOutput<PJ301MPort>(mm2px(Vec(48.692, 109.364)), module, Bypass::RECT56677_2_OUTPUT));
		addOutput(createOutput<PJ301MPort>(mm2px(Vec(37.533, 109.501)), module, Bypass::RECT56677_3_7_OUTPUT));
    */

		addChild(createLight<MuteLight<BlueLight>>(mm2px(Vec(27.423, 18.856)), module, Bypass::SEND_LIGHT));
		addChild(createLight<MuteLight<BlueLight>>(mm2px(Vec(27.48, 29.088)), module, Bypass::SEND_LIGHT+1));
		addChild(createLight<MuteLight<BlueLight>>(mm2px(Vec(27.48, 39.261)), module, Bypass::SEND_LIGHT+2));
		addChild(createLight<MuteLight<BlueLight>>(mm2px(Vec(27.48, 49.433)), module, Bypass::SEND_LIGHT+3));
		addChild(createLight<MuteLight<BlueLight>>(mm2px(Vec(27.48, 59.606)), module, Bypass::SEND_LIGHT+4));
		addChild(createLight<MuteLight<BlueLight>>(mm2px(Vec(27.48, 69.779)), module, Bypass::SEND_LIGHT+5));
		addChild(createLight<MuteLight<BlueLight>>(mm2px(Vec(27.48, 79.951)), module, Bypass::SEND_LIGHT+6));
		addChild(createLight<MuteLight<BlueLight>>(mm2px(Vec(27.48, 90.124)), module, Bypass::SEND_LIGHT+7));
		addChild(createLight<MuteLight<BlueLight>>(mm2px(Vec(27.48, 100.297)), module, Bypass::SEND_LIGHT+8));
		addChild(createLight<MuteLight<BlueLight>>(mm2px(Vec(27.48, 110.47)), module, Bypass::SEND_LIGHT+9));
	}
};

Model *modelBypass = createModel<Bypass, BypassWidget>("Bypass");
