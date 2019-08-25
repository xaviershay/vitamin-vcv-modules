#include "plugin.hpp"


struct Bypass : Module {
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
		NUM_LIGHTS
	};

	Bypass() {
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
	}

	void process(const ProcessArgs &args) override {
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

		addParam(createParam<RoundBlackKnob>(mm2px(Vec(27.154, 18.165)), module, Bypass::RECT56094_PARAM));
		addParam(createParam<RoundBlackKnob>(mm2px(Vec(27.154, 18.165)), module, Bypass::RECT56094_4_PARAM));
		addParam(createParam<RoundBlackKnob>(mm2px(Vec(27.154, 18.165)), module, Bypass::RECT56094_1_PARAM));
		addParam(createParam<RoundBlackKnob>(mm2px(Vec(27.154, 18.165)), module, Bypass::RECT56094_5_PARAM));
		addParam(createParam<RoundBlackKnob>(mm2px(Vec(27.154, 18.165)), module, Bypass::RECT56094_6_PARAM));
		addParam(createParam<RoundBlackKnob>(mm2px(Vec(27.154, 18.165)), module, Bypass::RECT56094_3_PARAM));
		addParam(createParam<RoundBlackKnob>(mm2px(Vec(27.154, 18.165)), module, Bypass::RECT56094_9_PARAM));
		addParam(createParam<RoundBlackKnob>(mm2px(Vec(27.154, 18.165)), module, Bypass::RECT56094_49_PARAM));
		addParam(createParam<RoundBlackKnob>(mm2px(Vec(27.154, 18.165)), module, Bypass::RECT56094_50_PARAM));
		addParam(createParam<RoundBlackKnob>(mm2px(Vec(27.154, 18.165)), module, Bypass::RECT56094_17_PARAM));

		addInput(createInput<PJ301MPort>(mm2px(Vec(15.214, 17.764)), module, Bypass::RECT56657_2_INPUT));
		addInput(createInput<PJ301MPort>(mm2px(Vec(4.055, 17.81)), module, Bypass::RECT56657_INPUT));
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

		addChild(createLight<MediumLight<RedLight>>(mm2px(Vec(27.903, 18.915)), module, Bypass::RECT56697_LIGHT));
		addChild(createLight<MediumLight<RedLight>>(mm2px(Vec(27.903, 18.915)), module, Bypass::RECT56697_0_LIGHT));
		addChild(createLight<MediumLight<RedLight>>(mm2px(Vec(27.903, 18.915)), module, Bypass::RECT56697_5_LIGHT));
		addChild(createLight<MediumLight<RedLight>>(mm2px(Vec(27.903, 18.915)), module, Bypass::RECT56697_3_LIGHT));
		addChild(createLight<MediumLight<RedLight>>(mm2px(Vec(27.903, 18.915)), module, Bypass::RECT56697_4_LIGHT));
		addChild(createLight<MediumLight<RedLight>>(mm2px(Vec(27.903, 18.915)), module, Bypass::RECT56697_2_LIGHT));
		addChild(createLight<MediumLight<RedLight>>(mm2px(Vec(27.903, 18.915)), module, Bypass::RECT56697_28_LIGHT));
		addChild(createLight<MediumLight<RedLight>>(mm2px(Vec(27.903, 18.915)), module, Bypass::RECT56697_6_LIGHT));
		addChild(createLight<MediumLight<RedLight>>(mm2px(Vec(27.903, 18.915)), module, Bypass::RECT56697_1_LIGHT));
		addChild(createLight<MediumLight<RedLight>>(mm2px(Vec(27.903, 18.915)), module, Bypass::RECT56697_57_LIGHT));
	}
};


Model *modelBypass = createModel<Bypass, BypassWidget>("Bypass");