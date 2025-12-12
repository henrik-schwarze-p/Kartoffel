#!/usr/bin/env ruby

def functions(app_name = "")
  [{ name: "name", type: "const char*", params: "", body: "{return PSTR(\"#{app_name}\");}" },
   { name: "cardinality", type: "int", params: "", body: "{return APP_CARDINALITY_UNDEFINED;}" },
   { name: "registerVars", type: "void", params: "", body: "{}" },
   { name: "whenCreated", type: "void", params: "", body: "{}" },
   { name: "monitor", type: "void", params: "int x,int y,int w,int h", body: "{}" },
   { name: "timeSlice", type: "void", params: "int fg, frequency fr", body: "{}" },
   { name: "actionsDef", type: "const char*", params: "int kind", body: "{return 0;}" },
   { name: "conditionsDef", type: "const char*", params: "int kind2", body: "{return 0;}" },
   { name: "setConditionParameter", type: "void", params: "int kind, int param", body: "{}" },
   { name: "setActionParameter", type: "void", params: "int kind, int param", body: "{}" },
   { name: "printConditionParameter", type: "void", params: "int a, int b, uint16_t c", body: "{print(c);}" },
   { name: "printActionParameter", type: "void", params: "int a, int b, uint16_t c", body: "{print(c);}" },
   { name: "whenPowered", type: "void", params: "", body: "{}" },
   { name: "startScreen", type: "void", params: "", body: "{}" },
   { name: "whenDestroyed", type: "void", params: "", body: "{}" },
   { name: "getLabelForPort", type: "const char*", params: "int a, int b", body: "{return 0;}" },
   { name: "evalCondition", type: "int", params: "int a, uint16_t* array", body: "{return 0;}" },
   { name: "performAction", type: "void", params: "int a, uint16_t* array", body: "{}" },
   { name: "hasMonitor", type: "int", params: "", body: "" }].collect do |fn|
    { name: fn[:name], type: fn[:type], params: fn[:params], body: fn[:body],
      params_with_comma: fn[:params] != "" ? "," + fn[:params] : fn[:params],
      name_uppercase: fn[:name][0].upcase + fn[:name][1..],
      calling_params: fn[:params].split(",").map { |param_definition| param_definition.split(" ").last }.join(",") }
  end
end

def found_in_code(fn, code)
  searched = (fn[:type] + " " + fn[:name] + "(").gsub(/\*/, "_").gsub(/\s+/, "") # intid(
  code.split("\n").collect { |l| l.gsub(/\*/, "_").gsub(/\s+/, "") }.each do |line|
    return true if line.start_with?(searched)
  end
  false
end

source_dir = ARGV.size > 0 ? ARGV[0] : "."
target_dir = ARGV.size > 1 ? ARGV[1] : "."
start = Time.now

files = Dir.glob("#{source_dir}/App[A-Z]*").select { |f| File.file? f }.collect do |file|
  l = source_dir.length + 4
  { name: file[l..-5], file: file, prefix: file[l..-5][0].downcase + file[l + 1..-5], code: File.read(file) }
end

puts source_dir
puts target_dir

help = ""
namespaces = ""
files.each_with_index do |file, index|
  help += " * #{index}: #{file[:prefix]}\n"
  namespaces << "namespace #{file[:prefix]} {\n"
  monitor_exists = 0
  # name of the file is in the first line in a comment, if not, we take the file name
  first_line = file[:code].split("\n").first.strip
  if first_line.start_with?("// ")
    app_name = first_line[3..].strip
  else
    app_name = file[:name].sub(/^App/, "").sub(/\.cpp$/, "").gsub(/([a-z])([A-Z])/, '\1 \2')
  end
  functions(app_name).each do |function|
    function_name, function_type, function_params, function_body = function[:name], function[:type], function[:params], function[:body]
    if found_in_code(function, file[:code])
      monitor_exists = 1 if function_name == "monitor"
      namespaces << "   #{function_type} #{function_name}(#{function_params});\n"
    else
      if function_name == "hasMonitor"
        namespaces += "   #{function_type} #{function_name}(#{function_params}) {return #{monitor_exists};}\n"
      else
        namespaces += "   #{function_type} #{function_name}(#{function_params}) #{function_body}\n"
      end
    end
  end
  namespaces << "}\n"
end

functions.each do |function|
  function_name, function_type, function_params = function[:name], function[:type], function[:params]
  function_type_or_screen = function_name == "startScreen" ? "screen" : function_type
  namespaces += "#{function_type_or_screen} callGenerated#{function[:name_uppercase]}ForType(int type#{function[:params_with_comma]}) {\n"
  files.each_with_index do |file, index|
    namespaces += "    if (type==#{index}) "
    return_type = function_type != "void" || function_name == "startScreen" ? "return " : ""
    sentence = "#{return_type}#{file[:prefix]}::#{function_name}(#{function[:calling_params]});\n"
    namespaces += sentence.gsub("startScreen()", "startScreen")
  end
  namespaces += "    return 0;\n" if function_type != "void" || function_name == "startScreen"
  namespaces += "}\n#{function_type_or_screen} callGenerated#{function[:name_uppercase]}(int instance#{function[:params_with_comma]}) {\n"
  return_sentence = function_type != "void" ? "return" : ""
  coma = function[:calling_params] != "" ? "," : ""
  return_sentence = "return" if function_name == "startScreen"
  namespaces += "    #{return_sentence} callGenerated#{function[:name_uppercase]}ForType(typeForInstance(instance)#{coma}#{function[:calling_params]});\n}\n"
end

if files.size > 0
  help = "\n/*\n#{help} */\n"
  File.open("#{target_dir}/Glue.inc.h", "w") { |f| f.write(namespaces) }
  File.open("#{target_dir}/NumberOfDescriptors.inc.h", "w") { |f| f.write("#define NUMBER_OF_DESCRIPTORS #{files.size}#{help}") }
end

puts "Glued #{files.size} apps in #{(1000.0 * (Time.now - start)).to_i}ms"
