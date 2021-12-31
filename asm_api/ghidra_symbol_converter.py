
import sys
sys.path.insert(0, "./wwrando")
from asm.disassemble import get_main_symbols

import re

with open("./vanilla_defines/framework.map") as f:
  framework_symbols_addr_to_name = get_main_symbols(f.read())

framework_symbols_name_to_addr = {}
for address, name in framework_symbols_addr_to_name.items():
  framework_symbols_name_to_addr[name] = address

ghidra_symbols = {}
with open("./vanilla_defines/ww_linker.ld") as f:
  matches = re.findall(r"^(\S+) = 0x([0-9a-f]{8});$", f.read(), re.IGNORECASE | re.MULTILINE)
  for match in matches:
    name, address = match
    address = int(address, 16)
    ghidra_symbols[address] = name

ghidra_to_framework_symbols = {}
for address, name in ghidra_symbols.items():
  if address in framework_symbols_addr_to_name:
    ghidra_to_framework_symbols[name] = framework_symbols_addr_to_name[address]
