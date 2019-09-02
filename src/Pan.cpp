#include "plugin.hpp"


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
		configParam(PAN_PARAM, 0.f, 1.f, 0.5f, "L/R Pan", "", 0.f, 2.0f, -1.0f);
	}

	void process(const ProcessArgs &args) override {
		float in[16] = {};
		float l_out[16] = {};
		float r_out[16] = {};
    float cv[16] = {};

    int channels = 1;
    int cvChannels = 0;

    if (inputs[IN_INPUT].isConnected()) {
      channels = inputs[IN_INPUT].getChannels();
      inputs[IN_INPUT].readVoltages(in);

      if (inputs[CV_INPUT].isConnected()) {
        cvChannels = inputs[CV_INPUT].getChannels();
        inputs[CV_INPUT].readVoltages(cv);
      };

      float knobCv = params[PAN_PARAM].getValue();

      // TODO: Apply pan law
      for (int c = 0; c < channels; c++) {
        float panAmount;

        if (c < cvChannels) {
          panAmount = cv[c] / 10.0f;
        } else {
          panAmount = knobCv;
        }

        l_out[c] = in[c] * (1.0f - panAmount);
        r_out[c] = in[c] * panAmount;
      }

      if (outputs[LEFT_OUTPUT].isConnected()) {
        outputs[LEFT_OUTPUT].setChannels(channels);
        outputs[LEFT_OUTPUT].writeVoltages(l_out);
      }

      if (outputs[RIGHT_OUTPUT].isConnected()) {
        outputs[RIGHT_OUTPUT].setChannels(channels);
        outputs[RIGHT_OUTPUT].writeVoltages(r_out);
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

		addParam(createParamCentered<RetroKnob>(mm2px(Vec(12.7, 56.455)), module, Pan::PAN_PARAM));

		addInput(createInput<PJ301MPort>(mm2px(Vec(8.593, 31.039)), module, Pan::IN_INPUT));
		addInput(createInput<PJ301MPort>(mm2px(Vec(8.593, 67.213)), module, Pan::CV_INPUT));

		addOutput(createOutput<PJ301MPort>(mm2px(Vec(3.014, 96.819)), module, Pan::LEFT_OUTPUT));
		addOutput(createOutput<PJ301MPort>(mm2px(Vec(14.173, 96.819)), module, Pan::RIGHT_OUTPUT));
	}
};


Model *modelPan = createModel<Pan, PanWidget>("Pan");
