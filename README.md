
### About

This is a work-in-progress API for writing custom for the The Legend of Zelda: The Wind Waker (USA GameCube version).

Currently, it allows you to create brand new actors from scratch, compiling them into custom REL files.
It can also insert those REL files into the game, replacing an existing actor.
In the future it will also allow adding as many new actors as you want into the game without having to replace existing ones.

### Requirements

Download and install the following:
* Python 3.8.2: https://www.python.org/downloads/release/python-382/
* devkitPro: https://devkitpro.org/

### Usage

You can compile a custom actor into a REL file like so:
`py build.py [path to C source file] [REL module ID number in hexadecimal] [actor profile symbol name] [optional: path to RELS.arc to insert the REL into]`

`[path to C source file]` is the path to your .c file containing your custom actor's code.
`[REL module ID number in hexadecimal]` is the module ID to give to the new REL file. Do not confuse this with the actor ID - this is only used for linking RELs, and must be unique among all RELs in the game.
`[actor profile symbol name]` is the variable name you gave to your custom actor's actor profile.
`[optional: path to RELS.arc to insert the REL into]` is the path to a RELS.arc. If specified, the script will insert the new REL into this RELS.arc, replacing one of the vanilla RELs, and will also update the profile list located in RELS.arc so the new REL works properly.

For example:
`py build_rel.py "./examples/switch_op.c" 0x58 g_profile_SwitchOperator "../Wind Waker Extracted/files/RELS.arc"`

For choosing the REL module ID, if replacing an existing REL, simply look at the first 4 bytes of the original REL. That's the module ID for it - so specify that ID and it will replace that actor.
If you don't want to break anything in the game, using 0x58 is recommended - that's the module ID for the actor `hotest`, aka `d_a_rectangle.rel` - this is a useless test actor that was never placed and doesn't do anything, so replacing it has no side effects.
If you want to add a new REL to the game without replacing an existing one (not supported by this API yet), any module ID 0x1A0 or higher was unused in the vanilla game.
