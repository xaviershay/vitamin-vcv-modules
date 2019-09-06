require 'bundler/setup'

require 'nokogiri'
require 'pp'
require 'pry'

Component = Struct.new(:name, :klass, :x, :y) do
  def self.to_enum_cpp(components)
    <<-CPP
  enum #{self.name}Ids {
#{components.select {|x| self === x }.map {|x| "    %s,\n" % x.name }.sort.join}
  NUM_#{self.name.upcase}S
};
    CPP
  end

  def order
    0
  end
end

class Input < Component
def to_cpp(module_name)
  "addInput(createInputCentered<%s>(mm2px(Vec(%f, %f)), module, %s::%s));" % [
    klass || "PJ301MPort", x, y, module_name, name
  ]
end

def self.namespace; "INPUT"; end
end

class Output < Component
  def to_cpp(module_name)
    "addOutput(createOutputCentered<%s>(mm2px(Vec(%f, %f)), module, %s::%s));" % [
      klass || "PJ301MPort", x, y, module_name, name
    ]
  end

  def self.namespace; "OUTPUT"; end
end

class Light < Component
  def to_cpp(module_name)
    "addChild(createLightCentered<%s>(mm2px(Vec(%f, %f)), module, %s::%s));" % [
      klass || "MediumLight<RedLight>", x, y, module_name, name
    ]
  end

  def order
    1
  end

  def self.namespace; "LIGHT"; end
end

class Param < Component
  def to_cpp(module_name)
    "addParam(createParamCentered<%s>(mm2px(Vec(%f, %f)), module, %s::%s));" % [
      klass || "RoundBlackKnob", x, y, module_name, name
    ]
  end

  def to_config_cpp
    %|configParam(%s, 0.f, 1.f, 0.f, "");| % name
  end

  def self.namespace; "PARAM"; end

  # Place params below lights by default
  def order
    -1
  end
end



module_name = ARGV[0] || "First argument must be module name"

svg_file = "res/#{module_name}.svg"
cpp_file = "src/#{module_name}.cpp"

svg_data = File.read(svg_file)

doc = Nokogiri::XML(svg_data)

layer = doc.at_xpath("//svg:g[@inkscape:label='components']") || raise("No components layer found")

components = []

ALL_COMPONENT_TYPES = [Input, Output, Param, Light]
ALL_COMPONENT_TYPES.each do |component_type|
  cs = layer.xpath("//svg:circle[starts-with(@inkscape:label,'#{component_type.namespace}')]")
  cs.each do |c|
    name = c['id'].to_s
    klass = c['inkscape:label'].split("|")[1]
    x = c['cx'].to_f
    y = c['cy'].to_f
    components << component_type.new(name, klass, x, y)
  end

  gs = layer.xpath("//svg:g[starts-with(@inkscape:label,'#{component_type.namespace}')]")
  gs.each do |g|
    transform = g['transform']
    offset_x, offset_y =
      if transform
        matches = transform.match(/translate\((.*),(.*)\)/)
        if matches
          [matches[1].to_f, matches[2].to_f]
        end
      end || [0, 0]
    xs = []
    ys = []
    g.css('circle').each do |circle|
      xs << circle['cx'].to_f + offset_x
      ys << circle['cy'].to_f + offset_y
    end
    x = xs.reduce(:+) / xs.length
    y = ys.reduce(:+) / ys.length
    name = g['id'].to_s
    klass = g['inkscape:label'].split("|")[1]

    if name.length == 0
      raise "No ID on component"
    end
    components << component_type.new(name, klass, x, y)
  end
end

src = <<-CPP
#include "plugin.hpp"

struct #{module_name} : Module {
  /* START GENERATED - DO NOT EDIT: Enums */
  /* END GENERATED: Enums */

  #{module_name}() {
    config(NUM_PARAMS, NUM_INPUTS, NUM_OUTPUTS, NUM_LIGHTS);
    /* START GENERATED - PARAMS EDITABLE: Param Config */
    /* END GENERATED: Param Config */
  }

  void process(const ProcessArgs &args) override {
  }
};

struct #{module_name}Widget : ModuleWidget {
  #{module_name}Widget(#{module_name} *module) {
    setModule(module);
    setPanel(APP->window->loadSvg(asset::plugin(pluginInstance, "res/#{module_name}.svg")));

		addChild(createWidget<ScrewSilver>(Vec(15, 0)));
		addChild(createWidget<ScrewSilver>(Vec(box.size.x - 30, 0)));
		addChild(createWidget<ScrewSilver>(Vec(15, 365)));
		addChild(createWidget<ScrewSilver>(Vec(box.size.x - 30, 365)));

    /* START GENERATED - DO NOT EDIT: Add Components */
    /* END GENERATED: Add Components */
  }
};

Model *model#{module_name} = createModel<#{module_name}, #{module_name}Widget>("#{module_name}");
CPP

def replace_generated(src, label, &block)
  idx_lines = src.lines.each.with_index
  idx_start = idx_lines.detect {|x, _| x.include?("START GENERATED") && x.include?(label) }[1]
  idx_end = idx_lines.detect {|x, _| x.include?("END GENERATED") && x.include?(label) }[1]

  lines = src.lines
  existing = lines.slice!(idx_start+1..idx_end-1)
  replace = block.call(existing)

  lines = lines.insert(idx_start + 1, *replace.lines)
  lines.join
end

def update_src(module_name, src, components)
  src = replace_generated(src, "Enums") do |existing|
    ALL_COMPONENT_TYPES.map do |type|
      type.to_enum_cpp(components)
    end.join("\n")
  end

  src = replace_generated(src, "Add Components") do |existing|
    components.
      sort_by {|x| [x.order, x.name] }.
      map {|c| "    %s\n" % c.to_cpp(module_name) }.
      join
  end

  src = replace_generated(src, "Param Config") do |existing|
    existing.join +
      components.select {|x| Param === x }.map do |c|
        next if existing.detect {|x| x.include?(%|configParam(%s| % c.name) }

        "    %s\n" % c.to_config_cpp
      end.compact.sort.join
  end
end

unless File.exists?(cpp_file)
  File.write(cpp_file, src)
end

src = File.read(cpp_file)
File.write(cpp_file, update_src(module_name, src, components))
