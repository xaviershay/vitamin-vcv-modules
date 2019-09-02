#include "plugin.hpp"


struct Pan : Module {
	enum ParamIds {
		PATH1108_PARAM,
		NUM_PARAMS
	};
	enum InputIds {
		RECT56657_INPUT,
		RECT56657_2_INPUT,
		NUM_INPUTS
	};
	enum OutputIds {
		RECT56677_3_OUTPUT,
		RECT56677_OUTPUT,
		NUM_OUTPUTS
	};
	enum LightIds {
		NUM_LIGHTS
	};

	Pan() {
		config(NUM_PARAMS, NUM_INPUTS, NUM_OUTPUTS, NUM_LIGHTS);
		configParam(PATH1108_PARAM, 0.f, 1.f, 0.f, "");
	}

	void process(const ProcessArgs &args) override {
	}
};

struct RetroKnob : SVGKnob {
	RetroKnob() {
		box.size = Vec(20, 20);
		minAngle = -0.75*M_PI;
		maxAngle = 0.75*M_PI;
		setSVG(APP->window->loadSvg(asset::plugin(pluginInstance, "res/SmallKnob.svg")));
	}
};

struct PanWidget : ModuleWidget {
	PanWidget(Pan *module) {
		setModule(module);
		setPanel(APP->window->loadSvg(asset::plugin(pluginInstance, "res/Pan.svg")));

		addChild(createWidget<ScrewSilver>(Vec(RACK_GRID_WIDTH, 0)));
		addChild(createWidget<ScrewSilver>(Vec(box.size.x - 2 * RACK_GRID_WIDTH, 0)));
		addChild(createWidget<ScrewSilver>(Vec(RACK_GRID_WIDTH, RACK_GRID_HEIGHT - RACK_GRID_WIDTH)));
		addChild(createWidget<ScrewSilver>(Vec(box.size.x - 2 * RACK_GRID_WIDTH, RACK_GRID_HEIGHT - RACK_GRID_WIDTH)));

		addParam(createParamCentered<RetroKnob>(mm2px(Vec(12.7, 56.455)), module, Pan::PATH1108_PARAM));

		addInput(createInput<PJ301MPort>(mm2px(Vec(8.593, 31.039)), module, Pan::RECT56657_INPUT));
		addInput(createInput<PJ301MPort>(mm2px(Vec(8.593, 67.213)), module, Pan::RECT56657_2_INPUT));

		addOutput(createOutput<PJ301MPort>(mm2px(Vec(3.014, 96.819)), module, Pan::RECT56677_3_OUTPUT));
		addOutput(createOutput<PJ301MPort>(mm2px(Vec(14.173, 96.819)), module, Pan::RECT56677_OUTPUT));
	}
};


Model *modelPan = createModel<Pan, PanWidget>("Pan");