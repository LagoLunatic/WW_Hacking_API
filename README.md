
### About

This is a work-in-progress API for writing custom code for The Legend of Zelda: The Wind Waker (USA GameCube version).

Features:
* Writing ASM patches to edit the game's existing code (in both main.dol and REL files)
* Include C code in ASM patches (currently only supports main.dol, REL support planned)
* Creating brand new actors from scratch coded in C, and compiling them into REL files
* Replacing existing actors from the game with your new custom actors

In the future it will also allow adding as many new actors as you want into the game without having to replace existing ones.  

The C code you write can call the vanilla game's functions and use its global variables seamlessly, for example:
```c
// Check if a switch is set.
bool isSwitchSet = dSv_info_c__isSwitch(&g_dComIfG_gameInfo.mSvInfo, switchToCheck, this->parent.mCurrent.mRoomNo);
```
```c
// Load the file with index 4 from files/res/Object/Ecube.arc, and instantiate it as a 3D model.
J3DModelData* modelData = dRes_control_c__getRes("Ecube", 4, g_dComIfG_gameInfo.mResCtrl.mObjectInfo, 0x40);
this->mpModel = mDoExt_J3DModel__create(modelData, 0x80000, 0x11000000);
```

You can use any function, variable, or struct that has been documented. Not every single one in the game has been documented, but hundreds of them have, so you should be able to do quite a lot.  
Huge thanks to [Jasper](https://github.com/magcius) for documenting tons of them.  

### Requirements

Download and install the following:
* git: https://git-scm.com/downloads
* Python 3.10.7: https://www.python.org/downloads/release/python-3107/
* devkitPro: https://devkitpro.org/

### Installation

First, clone this repository to your computer by running this in a command prompt:  
`git clone --recurse-submodules https://github.com/LagoLunatic/WW_Hacking_API.git`  

Then open the WW_Hacking_API folder in a command prompt and install Python dependencies by running:  
`py -3.10 -m pip install -r requirements.txt`  

### Usage

The hacking API can be used to write code in two distinct ways: editing the vanilla game's existing code with ASM patches, or creating new actors written in C.

#### Patching existing ASM code

First, write an ASM patch file with your custom ASM code and put it in the `asm_patches` directory.  
In these patches, you can use the following pseudo instructions:
* `.open "path/here"` specifies the path of the file to edit (relative to the ISO root)
* `.org 0x1234` specifies the address (for main.dol) or the offset (for RELs) at which to write the custom code
* `.org @NextFreeSpace` specifies that the following code should be written in a chunk of free space, the location of which will be determined automatically by the assembler
* `.close` closes the currently open file
* `.include "path/here.asm"` takes the contents of another ASM patch you have written, and inserts it into this one (relative to the `asm_patches` directory)
* `.include "path/here.c"` takes the contents of a C source file you have written, compiles it, and inserts it into this ASM patch (relative to the `asm_patches` directory)

Next, run the following command to assemble all of the ASM patches in the `asm_patches` directory into diffs which will be placed in the `asm_patches/patch_diffs` directory:  
`py asm_api/assemble.py`

Finally, run this command to apply the patch diffs to the files in the ISO, and output all of the files that were modified to a folder with the proper directory structure (unchanged files will not be written):  
`py asmpatch.py [path to clean WW ISO file] [path to modified output extracted ISO folder]`

#### Creating custom C actors

You can compile a custom actor into a REL file like so:  
`py build_rel.py [path to C source file] [REL module ID number in hexadecimal] [actor profile symbol name]`

`[path to C source file]` is the path to your .c file containing your custom actor's code.  
`[REL module ID number in hexadecimal]` is the module ID to give to the new REL file. Do not confuse this with the actor ID - this is only used for linking RELs, and must be unique among all RELs in the game.  
`[actor profile symbol name]` is the variable name you gave to your custom actor's actor profile.  

The following optional arguments can also be used:  
`--rels_arc` is the path to a RELS.arc. If specified, the script will insert the new REL into this RELS.arc, replacing one of the vanilla RELs, and will also update the profile list located in RELS.arc so the new REL works properly.  
`--md5_check` is and MD5 hash. If the built REL does not have the same MD5 hash as this, an error will be thrown. This is for testing purposes; you probably don't want to use it.  

For example:
`py build_rel.py "./examples/switch_op.c" 0x58 g_profile_SwitchOperator --rels_arc "../Wind Waker Extracted/files/RELS.arc"`

As for how to choose the REL module ID, if replacing an existing REL, simply look at the first 4 bytes of the original REL in a hex editor. That's the module ID for it - so specify that ID and it will replace that actor.  
If you don't want to break anything in the game, using 0x58 is recommended - that's the module ID for the actor `hotest`, aka `d_a_rectangle.rel` - this is a useless test actor that was never placed and doesn't do anything, so replacing it has no side effects.  
If you want to add a new REL to the game without replacing an existing one (not supported by this API yet), the first unused module ID is 0x1A0.  
