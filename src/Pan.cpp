#include "plugin.hpp"

// Polyphonic panning module that applies constant power pan law.
//
// Reference:
//
// * Section 2.2 of http://www.cs.cmu.edu/~music/icm-online/readings/panlaws/panlaws.pdf
struct Pan : Module {
	enum ParamIds {
		PAN_PARAM,
		NUM_PARAMS
	};
	enum InputIds {
		IN_INPUT,
		CV_INPUT,
		NUM_INPUTS
	};
	enum OutputIds {
		LEFT_OUTPUT,
		RIGHT_OUTPUT,
		NUM_OUTPUTS
	};
	enum LightIds {
		NUM_LIGHTS
	};

	Pan() {
		config(NUM_PARAMS, NUM_INPUTS, NUM_OUTPUTS, NUM_LIGHTS);
		configParam(PAN_PARAM, -5.f, 5.f, 0.f, "L/R Pan", "%", 0, 20);
	}

	void process(const ProcessArgs &args) override {
    simd::float_4 in[4];
    simd::float_4 cv[4];
    simd::float_4 left[4];
    simd::float_4 right[4];

    if (inputs[IN_INPUT].isConnected()) {
      int channels = inputs[IN_INPUT].getChannels();
      float knobCv = params[PAN_PARAM].getValue();

      for (int c = 0; c < channels; c += 4) {
        int i = c / 4;

        in[i] = simd::float_4::load(inputs[IN_INPUT].getVoltages(c));
        cv[i] = simd::float_4::load(inputs[CV_INPUT].getVoltages(c));

        cv[i] = simd::clamp(
          (cv[i] + knobCv + 5.0f) / 10.0f * M_PI / 2,
          0.0,
          M_PI/2
        );

        left[i]  = in[i] * simd::cos(cv[i]);
        right[i] = in[i] * simd::sin(cv[i]);

        outputs[LEFT_OUTPUT].setVoltageSimd(left[i], c);
        outputs[RIGHT_OUTPUT].setVoltageSimd(right[i], c);
        outputs[LEFT_OUTPUT].setChannels(channels);
        outputs[RIGHT_OUTPUT].setChannels(channels);
      }
    } else {
      outputs[LEFT_OUTPUT].clearVoltages();
      outputs[RIGHT_OUTPUT].clearVoltages();
    }
	}
};

struct RetroKnob : SVGKnob {
	RetroKnob() {
		box.size = Vec(20, 20);
		minAngle = -0.75*M_PI;
		maxAngle = 0.75*M_PI;
		setSvg(APP->window->loadSvg(asset::plugin(pluginInstance, "res/SmallKnob.svg")));
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

		addParam(createParamCentered<RetroKnob>(mm2px(Vec(12.7, 56.455)), module, Pan::PAN_PARAM));

		addInput(createInput<PJ301MPort>(mm2px(Vec(8.593, 31.039)), module, Pan::IN_INPUT));
		addInput(createInput<PJ301MPort>(mm2px(Vec(8.593, 67.213)), module, Pan::CV_INPUT));

		addOutput(createOutput<PJ301MPort>(mm2px(Vec(3.014, 96.819)), module, Pan::LEFT_OUTPUT));
		addOutput(createOutput<PJ301MPort>(mm2px(Vec(14.173, 96.819)), module, Pan::RIGHT_OUTPUT));
	}
};

Model *modelPan = createModel<Pan, PanWidget>("Pan");
