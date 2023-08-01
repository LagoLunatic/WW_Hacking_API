#!/usr/bin/python3.10

import os
from subprocess import call
import sys
import elf2rel
import argparse
import hashlib


parser = argparse.ArgumentParser(description="Compile an actor written in C to a GameCube REL file.")
parser.add_argument(
  "c_src_path",
  type=str,
  help="path to the C source file",
)
parser.add_argument(
  "module_id",
  type=str,
  help="the REL module ID in hexadecimal",
)
parser.add_argument(
  "profile",
  type=str,
  help="the symbol name of the actor's profile",
)
parser.add_argument(
  "--rels_arc",
  type=str,
  help="path to the RELS.arc to insert the REL into",
)
parser.add_argument(
  "--md5_check",
  type=str,
  help="the MD5 hash to ensure the REL matches once built (for testing; use after updating Ghidra exports)",
)
args = parser.parse_args()

c_src_path = args.c_src_path
rel_id = int(args.module_id, 16)
actor_profile_name = args.profile
rels_arc_path = args.rels_arc
md5_check = args.md5_check


if sys.platform == "win32":
  DEVKIT_BIN_PATH = r"C:\devkitPro\devkitPPC\bin"
else:
  if not "DEVKITPPC" in os.environ:
    raise Exception(r"Could not find devkitPPC. Path to devkitPPC should be in the DEVKITPPC env var")
  DEVKIT_BIN_PATH = os.environ.get("DEVKITPPC") + "/bin"

def get_bin(name):
  if not sys.platform == "win32":
    return os.path.join(DEVKIT_BIN_PATH, name)
  return os.path.join(DEVKIT_BIN_PATH, name + ".exe")

if not os.path.isfile(get_bin("powerpc-eabi-gcc")):
  raise Exception(r"Failed to assemble code: Could not find devkitPPC. devkitPPC should be installed to: C:\devkitPro\devkitPPC")


build_dir = "./build"
basename = os.path.basename(c_src_path)
basename_no_ext = os.path.splitext(basename)[0]
linker_script_path = "./vanilla_defines/ww_linker.ld"

if not os.path.isdir(build_dir):
  os.makedirs(build_dir)


elf_path = os.path.join(build_dir, basename_no_ext + ".o")
command = [
  get_bin("powerpc-eabi-gcc"),
  "-mcpu=750",
  "-fno-inline",
  "-Wall",
  "-Og",
  "-g",
  "-fshort-enums",
  # -fno-jump-tables disables a compiler optimization where GCC would turn
  # switch statements into lists of relocated pointers to offsets within
  # the function to jump to. Although the GameCube supports these just fine
  # and the vanilla game used them plenty, we can't use them here because
  # GCC chooses the type of relocation for the jump table to use as
  # R_PPC_REL32 (32-bit relative offset relocation), which is not supported.
  # The closest we have is R_PPC_ADDR32 (32-bit absolute address relocation)
  # but there is no clear way to convert between these.
  "-fno-jump-tables",
  "-c", c_src_path,
  "-o", elf_path,
]
print(" ".join(command))
print()
result = call(command)
if result != 0:
  raise Exception("Compiler call failed")


linked_elf_path = os.path.join(build_dir, basename_no_ext + "_linked.o")
command = [
  get_bin("powerpc-eabi-ld"),
  "--relocatable",
  "-T", linker_script_path,
  "-o", linked_elf_path,
  elf_path,
]
print(" ".join(command))
print()
result = call(command)
if result != 0:
  raise Exception("Linker call failed")


disassembled_elf_path = os.path.join(build_dir, basename_no_ext + "_disassembled_elf.asm")
command = [
  get_bin("powerpc-eabi-objdump"),
  "-m", "powerpc",
  "-D",
  "-EB",
  "--disassemble-zeroes",
  "--reloc",
  linked_elf_path,
]
print(" ".join(command))
print()
with open(disassembled_elf_path, "w") as f:
  result = call(command, stdout=f)
if result != 0:
  raise Exception("Objdump call failed")


out_rel_path = os.path.join(build_dir, "d_a_%s.rel" % basename_no_ext)
elf2rel.convert_elf_to_rel(linked_elf_path, out_rel_path, rel_id, actor_profile_name, rels_arc_path)


disassembled_rel_path = os.path.join(build_dir, basename_no_ext + "_disassembled_rel.asm")
command = [
  get_bin("powerpc-eabi-objdump"),
  "-m", "powerpc",
  "-D",
  "-EB",
  "--disassemble-zeroes",
  "-b", "binary",
  out_rel_path,
]
print(" ".join(command))
print()
with open(disassembled_rel_path, "w") as f:
  result = call(command, stdout=f)
if result != 0:
  raise Exception("Objdump call failed")


md5 = hashlib.md5()
with open(out_rel_path, "rb") as f:
  md5.update(f.read())
print("MD5 hash of the built REL file: %s" % md5.hexdigest())
if md5_check is not None:
  if md5.hexdigest() == md5_check:
    print("MD5 hash matched.")
  else:
    raise Exception("MD5 hash did not match! Expected: %s" % md5_check)

