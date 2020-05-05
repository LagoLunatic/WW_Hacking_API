
import re
from collections import OrderedDict
import yaml


with open("overloaded_funcs.txt", "r") as f:
  overloaded_funcs = yaml.safe_load(f)

NAMESPACES_TO_NOT_QUALIFY = [
  "Global",
  "d_com_inf_game",
  "f_pc_leaf",
  "f_op_actor",
  "f_op_actor_mng",
  "mtx",
  "d_kankyo",
  "d_bg_s",
  "m_Do_ext",
  "m_Do_printf",
  "m_Do_mtx",
]

NAMESPACES_TO_NOT_QUALIFY_FOR_FUNCS = NAMESPACES_TO_NOT_QUALIFY + [
  "d_npc",
  "SComponent",
]

def clean_symbol_name(symbol_name):
  symbol_name = symbol_name.replace("new[]", "new_array")
  symbol_name = symbol_name.replace("delete[]", "delete_array")
  symbol_name = symbol_name.replace("operator_()", "operator_parentheses")
  symbol_name = symbol_name.replace("operator_==", "operator_equal_to")
  symbol_name = symbol_name.replace("operator_!=", "operator_not_equal_to")
  symbol_name = symbol_name.replace("operator_=", "operator_set")
  symbol_name = symbol_name.replace("operator_+=", "operator_plus_set")
  symbol_name = symbol_name.replace("operator_-=", "operator_minus_set")
  symbol_name = symbol_name.replace("operator*=", "operator_times_set")
  symbol_name = symbol_name.replace("operator_/=", "operator_divide_set")
  symbol_name = symbol_name.replace("operator_+", "operator_plus")
  symbol_name = symbol_name.replace("operator_-", "operator_minus")
  symbol_name = symbol_name.replace("operator*", "operator_times")
  symbol_name = symbol_name.replace("operator_/", "operator_divide")
  symbol_name = re.sub(r"[@:\.,\-]", "_", symbol_name)
  #symbol_name = re.sub(r"[@:\.,\-%\"!&()|]", "_", symbol_name)
  symbol_name = re.sub(r"<", "__", symbol_name)
  symbol_name = re.sub(r">", "__", symbol_name)
  symbol_name = symbol_name.replace("*", "_star")
  return symbol_name


# Clean up structs exported from Ghidra.

with open("./ghidra_exports/ww_structs_from_ghidra.h") as f:
  input_str = f.read()

output_str = "\n"
output_str += "struct cXyz { float x; float y; float z; };\n\n"
output_str += "struct csXyz { short x; short y; short z; };\n\n"
current_enum_name = None
current_struct_name = None
for line in input_str.splitlines():
  comment_match = re.search(r"//|/\*", line)
  if comment_match:
    comment_start_index = comment_match.start()
    comment = line[comment_start_index:]
    line = line[:comment_start_index]
  else:
    comment = ""
  
  line = re.sub(r"[@:\?!]", "_", line)
  line = re.sub(r"\+", "Plus", line)
  
  if line == "typedef struct TVec3<float> TVec3<float>, *PTVec3<float>;":
    line = "//" + line
  elif line == "typedef struct TVec3<short> TVec3<short>, *PTVec3<short>;":
    line = "//" + line
  elif line == "typedef dword ...;":
    line = "//" + line
  else:
    line = re.sub(r"TVec3<float>( [^{])", "cXyz\\1", line)
    line = re.sub(r"TVec3<short>( [^{])", "csXyz\\1", line)
    #line = re.sub(r"(\S+)<([^<>]+)>", "\\1__\\2", line)
    line = re.sub(r"<", "__", line)
    line = re.sub(r">", "__", line)
  
  line = re.sub(r"pointer ~;", "pointer destructor;", line)
  #line = re.sub(r"pointer ~([^;]+);", "pointer \\1_destructor;", line)
  line = re.sub(r"pointer ~([^;]+);", "pointer destructor;", line)
  
  if line == "typedef struct struct struct, *Pstruct;":
    line = "//" + line
  
  if line in ["typedef ulong size_t;", "typedef struct _IO_FILE __FILE;", "typedef short    wchar_t;", "typedef long __off_t;"]:
    line = "//" + line
  
  if line.startswith("typedef struct ") or line.startswith("struct "):
    line = re.sub(r"\*", "_star", line)
    line = re.sub(r",(\S)", "_\\1", line)
  if current_enum_name is not None:
    line = re.sub(r"[()]", "_", line)
  
  enum_def_match = re.search(r"^typedef enum (\S+) {", line)
  if enum_def_match:
    current_enum_name = enum_def_match.group(1)
  
  struct_def_match = re.search(r"^struct (\S+) {", line)
  if struct_def_match:
    current_struct_name = struct_def_match.group(1)
  
  if current_enum_name is not None and "__" in current_enum_name:
    enum_value_match = re.search(r"^    ([^=]+)=([^,]+)(,?)$", line)
    if enum_value_match:
      # We have to add the enum name as a qualifier to the beginning of each enum value to prevent duplicate identifier issues.
      enum_value_name = enum_value_match.group(1)
      enum_value = enum_value_match.group(2)
      maybe_comma = enum_value_match.group(3)
      enum_value_name = "%s__%s" % (current_enum_name, enum_value_name)
      line = "    %s=%s%s" % (enum_value_name, enum_value, maybe_comma)
  
  if current_struct_name in ["profile_method_class", "profile_leaf_method_class"]:
    # Fix the actor methods to take any argument type.
    line = re.sub(r"\(void \*\);$", "();", line)
  
  if current_struct_name in ["cXyz", "csXyz"]:
    # We added these to the beginning so we don't want them duplicated.
    pass
  elif current_struct_name in ["struct", "_IO_FILE", "_IO_marker"]:
    line_with_comment = line + comment
    output_str += ("// " + line_with_comment + "\n")
  else:
    line_with_comment = line + comment
    output_str += (line_with_comment + "\n")
  
  if current_enum_name is not None and line == "} %s;" % current_enum_name:
    current_enum_name = None
  if current_struct_name is not None and line == "};":
    current_struct_name = None

with open("./vanilla_defines/ww_structs.h", "w") as f:
  f.write(output_str)



# Clean up function signatures exported from Ghidra.

with open("./ghidra_exports/ww_functions_from_ghidra.csv") as f:
  input_str = f.read()

# First parse the CSV.
func_datas = []
func_name_to_namespaces = OrderedDict()
for line in input_str.splitlines()[1:]:
  line = line[1:-1] # Remove first and last quotation mark since split won't get these
  func_name, address, func_signature, func_size, namespace = line.split("\",\"")
  address = int(address, 16)
  func_size = int(func_size, 16)
  
  if address >= 0x80338680:
    # Not in main.dol
    continue
  
  func_sign_match = re.search(r"^(\S+(?: \*)?) (.+)$", func_signature)
  return_value = func_sign_match.group(1)
  rest_of_func_sign = func_sign_match.group(2)
  assert rest_of_func_sign.startswith(func_name)
  arguments = rest_of_func_sign[len(func_name):]
  
  if address in overloaded_funcs:
    # Manually rename some overloaded functions to prevent conflicts because C does not support overloading.
    func_name = overloaded_funcs[address]
  
  if func_name not in func_name_to_namespaces:
    func_name_to_namespaces[func_name] = []
  
  if namespace not in func_name_to_namespaces[func_name]:
    func_name_to_namespaces[func_name].append(namespace)
  
  func_datas.append((func_name, address, return_value, arguments, func_size, namespace))

#namespaces_that_need_qualification = []
#for func_name, namespaces in func_name_to_namespaces.items():
#  if len(namespaces) > 1:
#    print(func_name)
#    #for namespace in namespaces:
#    #  if namespace not in namespaces_that_need_qualification:
#    #    if namespace == "f_op_actor_mng":
#    #      print(func_name)
#    #      input()
#    #    namespaces_that_need_qualification.append(namespace)
##print(namespaces_that_need_qualification)

output_str = "\n"
linker_str = "\n"
seen_symbol_names = []
for func_name, address, return_value, arguments, func_size, namespace in func_datas:
  symbol_name = func_name
  if symbol_name.startswith("~"):
    symbol_name = symbol_name[1:] + "_destructor"
  
  # Add the namespace to the symbol name.
  if namespace not in NAMESPACES_TO_NOT_QUALIFY_FOR_FUNCS:
    symbol_name = "%s__%s" % (namespace, symbol_name)
  
  # Clean up the symbol name.
  symbol_name = clean_symbol_name(symbol_name)
  
  # Clean up the arguments.
  arguments = re.sub(r"[@:]", "_", arguments)
  arguments = re.sub(r"TVec3<float>( [^{])", "cXyz\\1", arguments)
  arguments = re.sub(r"TVec3<short>( [^{])", "csXyz\\1", arguments)
  def fix_type_arg(match):
    return "__%s__" % match.group(1).replace("*", "_star").replace(",", "_")
  while "<" in arguments:
    arguments = re.sub(r"<([^<>]+)>", fix_type_arg, arguments)
  # Remove argument name from ... variable number of argument indicators
  arguments = re.sub(r"\.\.\. [^()\s,]+", "...", arguments)
  # Function argument definitions don't get exported from Ghidra, so we need to convert these manually.
  # Manually replace some specific known function argument definitions.
  arguments = re.sub(r"cPhs__Handler \* ([^()\s,]+)", "int \\1(void * )", arguments)
  # For the others, just make them able to take any type of function here.
  arguments = re.sub(r"FuncDef\d+ \*", "void *", arguments)
  # Fix array arguments. e.g. "float[6] * param_2" -> "float param_2[6]"
  def fix_array_arg(match):
    data_type = match.group(1)
    array_length = match.group(2)
    arg_name = match.group(3)
    return "%s %s[%s]" % (data_type, arg_name, array_length)
  arguments = re.sub(r"([^\s,()\[\]]+)\[(\d+)\] \* ([^\s,()\[\]]+)", fix_array_arg, arguments)
  
  func_signature = "%s %s%s" % (return_value, symbol_name, arguments)
  
  should_comment_out = False
  if symbol_name in seen_symbol_names:
    # Duplicate name. Comment it out for now.
    should_comment_out = True
  
  if symbol_name == "MSL_C_PPCEABI_bare_H__fwide":
    # Causes issues for some reason, just comment it out since it's unimportant
    should_comment_out = True
  
  if should_comment_out:
    output_str += "// "
    linker_str += "/* "
  
  output_str += "%s;" % func_signature
  linker_str += "%s = 0x%08X;" % (symbol_name, address)
  
  if should_comment_out:
    linker_str += " */"
  
  output_str += "\n"
  linker_str += "\n"
  
  seen_symbol_names.append(symbol_name)

with open("./vanilla_defines/ww_functions.h", "w") as f:
  f.write(output_str)



# Clean up variables exported from Ghidra.

with open("./ghidra_exports/ww_variables_from_ghidra.csv") as f:
  input_str = f.read()


# First parse the CSV.
var_datas = []
for line in input_str.splitlines()[1:]:
  line = line[1:-1] # Remove first and last quotation mark since split won't get these
  var_name, address, data_type, data_size, namespace = line.split("\",\"")
  address = int(address, 16)
  data_size = int(data_size, 16)
  
  if address >= 0x803FCF20:
    # Not in main.dol
    continue
  
  if var_name == "":
    # No name
    continue
  
  base_data_type = data_type.split(" ")[0]
  if base_data_type in ["float", "double", "string", "TerminatedCString", "unicode"]:
    # Ghidra puts the value in the variable name for these, and they're not useful to have anyway.
    continue
  
  var_datas.append((var_name, address, data_type, data_size, namespace))

output_str = "\n"
linker_str += "\n"
for var_name, address, data_type, data_size, namespace in var_datas:
  symbol_name = var_name
  
  if symbol_name.startswith("~"):
    symbol_name = symbol_name[1:] + "_destructor"
  
  # Add the namespace to the symbol name.
  if namespace not in NAMESPACES_TO_NOT_QUALIFY:
    symbol_name = "%s__%s" % (namespace, symbol_name)
  
  # Clean up the symbol name.
  symbol_name = clean_symbol_name(symbol_name)
  
  if symbol_name in seen_symbol_names:
    # Duplicate name. Comment it out for now.
    output_str += "// "
    linker_str += "/* "
  
  if data_type == "TVec3<float>":
    data_type = "cXyz"
  elif data_type == "TVec3<short>":
    data_type = "csXyz"
  
  # Array declarations have to be moved to after the variable name.
  array_dec_match = re.search(r"([^\[\]]+)(\[\d+\])", data_type)
  if array_dec_match:
    data_type = array_dec_match.group(1)
    array_declaration = array_dec_match.group(2)
  else:
    array_declaration = ""
  
  output_str += "extern %s %s%s;" % (data_type, symbol_name, array_declaration)
  linker_str += "%s = 0x%08X;" % (symbol_name, address)
  
  if symbol_name in seen_symbol_names:
    linker_str += " */"
  
  output_str += "\n"
  linker_str += "\n"
  
  seen_symbol_names.append(symbol_name)
  

with open("./vanilla_defines/ww_variables.h", "w") as f:
  f.write(output_str)

with open("./vanilla_defines/ww_linker.ld", "w") as f:
  f.write(linker_str)
