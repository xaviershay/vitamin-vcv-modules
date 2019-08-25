#include "plugin.hpp"

struct MyModule : Module {
	bool state[10];
	dsp::BooleanTrigger muteTrigger[10];


	void onReset() override {
		for (int i = 0; i < 10; i++) {
			state[i] = true;
		}
	}
	void onRandomize() override {
		for (int i = 0; i < 10; i++) {
			state[i] = (random::uniform() < 0.5f);
		}
	}

	json_t *dataToJson() override {
		json_t *rootJ = json_object();

		// states
		json_t *statesJ = json_array();
		for (int i = 0; i < 10; i++) {
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
			for (int i = 0; i < 10; i++) {
				json_t *stateJ = json_array_get(statesJ, i);
				if (stateJ)
					state[i] = json_boolean_value(stateJ);
			}
		}
	}

	enum ParamIds {
		RECT56094_PARAM,
		RECT56094_4_PARAM,
		RECT56094_1_PARAM,
		RECT56094_5_PARAM,
		RECT56094_6_PARAM,
		RECT56094_3_PARAM,
		RECT56094_9_PARAM,
		RECT56094_49_PARAM,
		RECT56094_50_PARAM,
		RECT56094_17_PARAM,
		RECT56094_7_PARAM,
		NUM_PARAMS
	};
	enum InputIds {
		RECT56657_2_INPUT,
		RECT56657_INPUT,
		RECT56657_2_41_INPUT,
		RECT56657_3_INPUT,
		RECT56657_2_2_INPUT,
		RECT56657_9_INPUT,
		RECT56657_2_42_INPUT,
		RECT56657_98_INPUT,
		RECT56657_2_98_INPUT,
		RECT56657_0_INPUT,
		RECT56657_2_0_INPUT,
		RECT56657_1_INPUT,
		RECT56657_2_1_INPUT,
		RECT56657_6_INPUT,
		RECT56657_2_4_INPUT,
		RECT56657_70_INPUT,
		RECT56657_2_9_INPUT,
		RECT56657_62_INPUT,
		RECT56657_2_5_INPUT,
		RECT56657_7_INPUT,
		RECT56657_2_03_INPUT,
		RECT56657_05_INPUT,
		NUM_INPUTS
	};
	enum OutputIds {
		RECT56677_OUTPUT,
		RECT56677_3_OUTPUT,
		RECT56677_60_OUTPUT,
		RECT56677_3_6_OUTPUT,
		RECT56677_0_OUTPUT,
		RECT56677_3_60_OUTPUT,
		RECT56677_34_OUTPUT,
		RECT56677_3_62_OUTPUT,
		RECT56677_6_OUTPUT,
		RECT56677_3_9_OUTPUT,
		RECT56677_28_OUTPUT,
		RECT56677_3_2_OUTPUT,
		RECT56677_4_OUTPUT,
		RECT56677_3_5_OUTPUT,
		RECT56677_8_OUTPUT,
		RECT56677_3_1_OUTPUT,
		RECT56677_25_OUTPUT,
		RECT56677_3_73_OUTPUT,
		RECT56677_2_OUTPUT,
		RECT56677_3_7_OUTPUT,
		RECT56677_29_OUTPUT,
		RECT56677_3_12_OUTPUT,
		NUM_OUTPUTS
	};
	enum LightIds {
		RECT56697_LIGHT,
		RECT56697_0_LIGHT,
		RECT56697_5_LIGHT,
		RECT56697_3_LIGHT,
		RECT56697_4_LIGHT,
		RECT56697_2_LIGHT,
		RECT56697_28_LIGHT,
		RECT56697_6_LIGHT,
		RECT56697_1_LIGHT,
		RECT56697_57_LIGHT,
		RECT56697_68_LIGHT,
		NUM_LIGHTS
	};

	MyModule() {
		config(NUM_PARAMS, NUM_INPUTS, NUM_OUTPUTS, NUM_LIGHTS);
		configParam(RECT56094_PARAM, 0.f, 1.f, 0.f, "");
		configParam(RECT56094_4_PARAM, 0.f, 1.f, 0.f, "");
		configParam(RECT56094_1_PARAM, 0.f, 1.f, 0.f, "");
		configParam(RECT56094_5_PARAM, 0.f, 1.f, 0.f, "");
		configParam(RECT56094_6_PARAM, 0.f, 1.f, 0.f, "");
		configParam(RECT56094_3_PARAM, 0.f, 1.f, 0.f, "");
		configParam(RECT56094_9_PARAM, 0.f, 1.f, 0.f, "");
		configParam(RECT56094_49_PARAM, 0.f, 1.f, 0.f, "");
		configParam(RECT56094_50_PARAM, 0.f, 1.f, 0.f, "");
		configParam(RECT56094_17_PARAM, 0.f, 1.f, 0.f, "");
		configParam(RECT56094_7_PARAM, 0.f, 1.f, 0.f, "");
    onReset();
	}

	void process(const ProcessArgs &args) override {
		const float zero[16] = {};
		float out[16] = {};
		int channels = 1;

		// Iterate rows
		for (int i = 0; i < 10; i++) {
			// Process trigger
			if (muteTrigger[i].process(params[RECT56094_PARAM + i].getValue() > 0.f))
				state[i] ^= true;
      /*

			// Get input
			// Inputs are normalized to the input above it, so only set if connected
			if (inputs[IN_INPUT + i].isConnected()) {
				channels = inputs[IN_INPUT + i].getChannels();
				inputs[IN_INPUT + i].readVoltages(out);
			}

			// Set output
			if (outputs[OUT_OUTPUT + i].isConnected()) {
				outputs[OUT_OUTPUT + i].setChannels(channels);
				outputs[OUT_OUTPUT + i].writeVoltages(state[i] ? out : zero);
			}
      */

			// Set light
			lights[RECT56697_LIGHT + i].setBrightness(state[i] ? 0.9f : 0.f);
		}
	}
};

template <typename BASE>
struct MuteLight : BASE {
	MuteLight() {
		this->box.size = mm2px(Vec(6.f, 6.f));
	}
};

struct MyModuleWidget : ModuleWidget {
	MyModuleWidget(MyModule *module) {
		setModule(module);
		setPanel(APP->window->loadSvg(asset::plugin(pluginInstance, "res/MyModule.svg")));

		addChild(createWidget<ScrewSilver>(Vec(RACK_GRID_WIDTH, 0)));
		addChild(createWidget<ScrewSilver>(Vec(box.size.x - 2 * RACK_GRID_WIDTH, 0)));
		addChild(createWidget<ScrewSilver>(Vec(RACK_GRID_WIDTH, RACK_GRID_HEIGHT - RACK_GRID_WIDTH)));
		addChild(createWidget<ScrewSilver>(Vec(box.size.x - 2 * RACK_GRID_WIDTH, RACK_GRID_HEIGHT - RACK_GRID_WIDTH)));

		addParam(createParam<LEDBezel>(mm2px(Vec(27.154, 18.165)), module, MyModule::RECT56094_PARAM));
		addParam(createParam<LEDBezel>(mm2px(Vec(27.154, 18.165)), module, MyModule::RECT56094_4_PARAM));
		addParam(createParam<LEDBezel>(mm2px(Vec(27.154, 18.165)), module, MyModule::RECT56094_1_PARAM));
		addParam(createParam<LEDBezel>(mm2px(Vec(27.154, 18.165)), module, MyModule::RECT56094_5_PARAM));
		addParam(createParam<LEDBezel>(mm2px(Vec(27.154, 18.165)), module, MyModule::RECT56094_6_PARAM));
		addParam(createParam<LEDBezel>(mm2px(Vec(27.154, 18.165)), module, MyModule::RECT56094_3_PARAM));
		addParam(createParam<LEDBezel>(mm2px(Vec(27.154, 18.165)), module, MyModule::RECT56094_9_PARAM));
		addParam(createParam<LEDBezel>(mm2px(Vec(27.154, 18.165)), module, MyModule::RECT56094_49_PARAM));
		addParam(createParam<LEDBezel>(mm2px(Vec(27.154, 18.165)), module, MyModule::RECT56094_50_PARAM));
		addParam(createParam<LEDBezel>(mm2px(Vec(27.154, 18.165)), module, MyModule::RECT56094_17_PARAM));
		addParam(createParam<LEDBezel>(mm2px(Vec(27.154, 18.165)), module, MyModule::RECT56094_7_PARAM));

		addInput(createInput<PJ301MPort>(mm2px(Vec(15.214, 17.764)), module, MyModule::RECT56657_2_INPUT));
		addInput(createInput<PJ301MPort>(mm2px(Vec(4.055, 17.81)), module, MyModule::RECT56657_INPUT));
		addInput(createInput<PJ301MPort>(mm2px(Vec(15.214, 26.761)), module, MyModule::RECT56657_2_41_INPUT));
		addInput(createInput<PJ301MPort>(mm2px(Vec(4.055, 26.807)), module, MyModule::RECT56657_3_INPUT));
		addInput(createInput<PJ301MPort>(mm2px(Vec(15.214, 35.757)), module, MyModule::RECT56657_2_2_INPUT));
		addInput(createInput<PJ301MPort>(mm2px(Vec(4.055, 35.803)), module, MyModule::RECT56657_9_INPUT));
		addInput(createInput<PJ301MPort>(mm2px(Vec(15.214, 44.754)), module, MyModule::RECT56657_2_42_INPUT));
		addInput(createInput<PJ301MPort>(mm2px(Vec(4.055, 44.8)), module, MyModule::RECT56657_98_INPUT));
		addInput(createInput<PJ301MPort>(mm2px(Vec(15.214, 53.751)), module, MyModule::RECT56657_2_98_INPUT));
		addInput(createInput<PJ301MPort>(mm2px(Vec(4.055, 53.797)), module, MyModule::RECT56657_0_INPUT));
		addInput(createInput<PJ301MPort>(mm2px(Vec(15.214, 62.748)), module, MyModule::RECT56657_2_0_INPUT));
		addInput(createInput<PJ301MPort>(mm2px(Vec(4.055, 62.794)), module, MyModule::RECT56657_1_INPUT));
		addInput(createInput<PJ301MPort>(mm2px(Vec(15.214, 71.744)), module, MyModule::RECT56657_2_1_INPUT));
		addInput(createInput<PJ301MPort>(mm2px(Vec(4.055, 71.79)), module, MyModule::RECT56657_6_INPUT));
		addInput(createInput<PJ301MPort>(mm2px(Vec(15.214, 80.741)), module, MyModule::RECT56657_2_4_INPUT));
		addInput(createInput<PJ301MPort>(mm2px(Vec(4.055, 80.787)), module, MyModule::RECT56657_70_INPUT));
		addInput(createInput<PJ301MPort>(mm2px(Vec(15.214, 89.738)), module, MyModule::RECT56657_2_9_INPUT));
		addInput(createInput<PJ301MPort>(mm2px(Vec(4.055, 89.784)), module, MyModule::RECT56657_62_INPUT));
		addInput(createInput<PJ301MPort>(mm2px(Vec(15.214, 98.735)), module, MyModule::RECT56657_2_5_INPUT));
		addInput(createInput<PJ301MPort>(mm2px(Vec(4.055, 98.781)), module, MyModule::RECT56657_7_INPUT));
		addInput(createInput<PJ301MPort>(mm2px(Vec(15.214, 107.731)), module, MyModule::RECT56657_2_03_INPUT));
		addInput(createInput<PJ301MPort>(mm2px(Vec(4.055, 107.777)), module, MyModule::RECT56657_05_INPUT));

		addOutput(createOutput<PJ301MPort>(mm2px(Vec(48.692, 17.81)), module, MyModule::RECT56677_OUTPUT));
		addOutput(createOutput<PJ301MPort>(mm2px(Vec(37.533, 17.946)), module, MyModule::RECT56677_3_OUTPUT));
		addOutput(createOutput<PJ301MPort>(mm2px(Vec(48.692, 26.807)), module, MyModule::RECT56677_60_OUTPUT));
		addOutput(createOutput<PJ301MPort>(mm2px(Vec(37.533, 26.943)), module, MyModule::RECT56677_3_6_OUTPUT));
		addOutput(createOutput<PJ301MPort>(mm2px(Vec(48.692, 35.803)), module, MyModule::RECT56677_0_OUTPUT));
		addOutput(createOutput<PJ301MPort>(mm2px(Vec(37.533, 35.94)), module, MyModule::RECT56677_3_60_OUTPUT));
		addOutput(createOutput<PJ301MPort>(mm2px(Vec(48.692, 44.8)), module, MyModule::RECT56677_34_OUTPUT));
		addOutput(createOutput<PJ301MPort>(mm2px(Vec(37.533, 44.937)), module, MyModule::RECT56677_3_62_OUTPUT));
		addOutput(createOutput<PJ301MPort>(mm2px(Vec(48.692, 53.797)), module, MyModule::RECT56677_6_OUTPUT));
		addOutput(createOutput<PJ301MPort>(mm2px(Vec(37.533, 53.933)), module, MyModule::RECT56677_3_9_OUTPUT));
		addOutput(createOutput<PJ301MPort>(mm2px(Vec(48.692, 62.794)), module, MyModule::RECT56677_28_OUTPUT));
		addOutput(createOutput<PJ301MPort>(mm2px(Vec(37.533, 62.93)), module, MyModule::RECT56677_3_2_OUTPUT));
		addOutput(createOutput<PJ301MPort>(mm2px(Vec(48.692, 71.79)), module, MyModule::RECT56677_4_OUTPUT));
		addOutput(createOutput<PJ301MPort>(mm2px(Vec(37.533, 71.927)), module, MyModule::RECT56677_3_5_OUTPUT));
		addOutput(createOutput<PJ301MPort>(mm2px(Vec(48.692, 80.787)), module, MyModule::RECT56677_8_OUTPUT));
		addOutput(createOutput<PJ301MPort>(mm2px(Vec(37.533, 80.924)), module, MyModule::RECT56677_3_1_OUTPUT));
		addOutput(createOutput<PJ301MPort>(mm2px(Vec(48.692, 89.784)), module, MyModule::RECT56677_25_OUTPUT));
		addOutput(createOutput<PJ301MPort>(mm2px(Vec(37.533, 89.921)), module, MyModule::RECT56677_3_73_OUTPUT));
		addOutput(createOutput<PJ301MPort>(mm2px(Vec(48.692, 98.781)), module, MyModule::RECT56677_2_OUTPUT));
		addOutput(createOutput<PJ301MPort>(mm2px(Vec(37.533, 98.917)), module, MyModule::RECT56677_3_7_OUTPUT));
		addOutput(createOutput<PJ301MPort>(mm2px(Vec(48.692, 107.777)), module, MyModule::RECT56677_29_OUTPUT));
		addOutput(createOutput<PJ301MPort>(mm2px(Vec(37.533, 107.914)), module, MyModule::RECT56677_3_12_OUTPUT));

		addChild(createLight<MuteLight<BlueLight>>(mm2px(Vec(27.903, 18.915)), module, MyModule::RECT56697_LIGHT));
		addChild(createLight<MuteLight<BlueLight>>(mm2px(Vec(27.903, 18.915)), module, MyModule::RECT56697_0_LIGHT));
		addChild(createLight<MuteLight<BlueLight>>(mm2px(Vec(27.903, 18.915)), module, MyModule::RECT56697_5_LIGHT));
		addChild(createLight<MuteLight<BlueLight>>(mm2px(Vec(27.903, 18.915)), module, MyModule::RECT56697_3_LIGHT));
		addChild(createLight<MuteLight<BlueLight>>(mm2px(Vec(27.903, 18.915)), module, MyModule::RECT56697_4_LIGHT));
		addChild(createLight<MuteLight<BlueLight>>(mm2px(Vec(27.903, 18.915)), module, MyModule::RECT56697_2_LIGHT));
		addChild(createLight<MuteLight<BlueLight>>(mm2px(Vec(27.903, 18.915)), module, MyModule::RECT56697_28_LIGHT));
		addChild(createLight<MuteLight<BlueLight>>(mm2px(Vec(27.903, 18.915)), module, MyModule::RECT56697_6_LIGHT));
		addChild(createLight<MuteLight<BlueLight>>(mm2px(Vec(27.903, 18.915)), module, MyModule::RECT56697_1_LIGHT));
		addChild(createLight<MuteLight<BlueLight>>(mm2px(Vec(27.903, 18.915)), module, MyModule::RECT56697_57_LIGHT));
		addChild(createLight<MuteLight<BlueLight>>(mm2px(Vec(27.903, 18.915)), module, MyModule::RECT56697_68_LIGHT));
	}
};


Model *modelMyModule = createModel<MyModule, MyModuleWidget>("MyModule");
