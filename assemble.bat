py -3 asm_api/assemble.py
if %errorlevel% neq 0 exit /b %errorlevel%
py -3 asmpatch.py "D:/WW/Vanilla ISOs/The Legend of Zelda - The Wind Waker.iso" "../Vanilla ISOs/WW ASM Patched"
