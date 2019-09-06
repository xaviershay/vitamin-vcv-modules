#include "plugin.hpp"


struct Bypass2 : Module {
	enum ParamIds {
		BUTTON_PARAM,
		NUM_PARAMS
	};
	enum InputIds {
		INAAAA_INPUT,
		SEND_INPUT,
		NUM_INPUTS
	};
	enum OutputIds {
		RETURN_OUTPUT,
		OUT_OUTPUT,
		NUM_OUTPUTS
	};
	enum LightIds {
		LIGHT_LIGHT,
		NUM_LIGHTS
	};

	Bypass2() {
		config(NUM_PARAMS, NUM_INPUTS, NUM_OUTPUTS, NUM_LIGHTS);
		configParam(BUTTON_PARAM, 0.f, 1.f, 0.f, "");
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

struct RetroButton : app::SvgSwitch {
	RetroButton() {
    momentary = true;
		addFrame(APP->window->loadSvg(asset::plugin(pluginInstance, "res/LEDBezel.svg")));
	}
};

struct Bypass2Widget : ModuleWidget {
	Bypass2Widget(Bypass2 *module) {
		setModule(module);
		setPanel(APP->window->loadSvg(asset::plugin(pluginInstance, "res/Bypass2.svg")));

		addChild(createWidget<ScrewSilver>(Vec(RACK_GRID_WIDTH, 0)));
		addChild(createWidget<ScrewSilver>(Vec(box.size.x - 2 * RACK_GRID_WIDTH, 0)));
		addChild(createWidget<ScrewSilver>(Vec(RACK_GRID_WIDTH, RACK_GRID_HEIGHT - RACK_GRID_WIDTH)));
		addChild(createWidget<ScrewSilver>(Vec(box.size.x - 2 * RACK_GRID_WIDTH, RACK_GRID_HEIGHT - RACK_GRID_WIDTH)));

		addParam(createParamCentered<RetroButton>(mm2px(Vec(32.583, 39.109)), module, Bypass2::BUTTON_PARAM));

		addInput(createInputCentered<PJ301MPort>(mm2px(Vec(6.238, 39.109)), module, Bypass2::INAAAA_INPUT));
		addInput(createInputCentered<PJ301MPort>(mm2px(Vec(18.881, 39.109)), module, Bypass2::SEND_INPUT));

		addOutput(createOutputCentered<PJ301MPort>(mm2px(Vec(43.11, 39.109)), module, Bypass2::RETURN_OUTPUT));
		addOutput(createOutputCentered<PJ301MPort>(mm2px(Vec(56.812, 39.109)), module, Bypass2::OUT_OUTPUT));

		addChild(createLightCentered<MuteLight<RedLight>>(mm2px(Vec(32.583, 39.109)), module, Bypass2::LIGHT_LIGHT));
	}
};


Model *modelBypass2 = createModel<Bypass2, Bypass2Widget>("Bypass2");
