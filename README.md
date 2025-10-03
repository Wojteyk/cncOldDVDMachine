# CNC Machine from Recycled DVD Drives

This project demonstrates a **DIY CNC machine** built from recycled DVD drive mechanisms, controlled by an **Arduino Uno**, own designed **motor controler** and connected to a **Python desktop application** over a serial port.  
The setup is capable of converting png images into bitmapsent from the application to the microcontroller, which then translates them into movement commands for the CNC hardware.

## ✨ Key Features
- **Hardware Control**: Arduino Uno drives stepper motors salvaged from DVD drives  
- **Custom Protocol**: Communication between the Python app and Arduino over serial  
- **Bitmap to Motion Conversion**: Python app encodes bitmap images and streams them to the Arduino for execution  
- **Low-cost CNC Prototype**: Built entirely from recycled hardware components  

## 🛠️ Technologies
- **Own designed motor driver** selfmade for universty project using l298n H-bridge
- **Arduino C++** for real-time motor control and protocol parsing  
- **Python (PySerial, PIL)** for image processing and serial communication  
- **DIY Mechanical Assembly** using DVD drive mechanics and a custom frame  

## 🎯 Purpose
This project was created as a **practical demonstration of hardware-software integration**, showcasing:  
- Embedded programming on microcontrollers  
- Serial communication between devices  
- Application of image processing in real-world automation  
- Hands-on mechanical prototyping with limited resources

