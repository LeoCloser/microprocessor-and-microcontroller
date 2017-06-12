@ECHO OFF
"C:\Program Files (x86)\Atmel\AVR Tools\AvrAssembler2\avrasm2.exe" -S "C:\microprocesadores\prac7\labels.tmp" -fI -W+ie -o "C:\microprocesadores\prac7\prac7.hex" -d "C:\microprocesadores\prac7\prac7.obj" -e "C:\microprocesadores\prac7\prac7.eep" -m "C:\microprocesadores\prac7\prac7.map" "C:\microprocesadores\prac7\prac7.asm"
