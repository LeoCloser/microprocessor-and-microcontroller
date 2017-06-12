@ECHO OFF
"C:\Program Files (x86)\Atmel\AVR Tools\AvrAssembler2\avrasm2.exe" -S "C:\microprocesadores\prac6b\labels.tmp" -fI -W+ie -o "C:\microprocesadores\prac6b\prac6b.hex" -d "C:\microprocesadores\prac6b\prac6b.obj" -e "C:\microprocesadores\prac6b\prac6b.eep" -m "C:\microprocesadores\prac6b\prac6b.map" "C:\microprocesadores\prac6b\prac6b.asm"
