import tkinter as tk
from tkinter import ttk
from tkinter import filedialog
from PIL import Image, ImageTk
import serial.tools.list_ports_windows
import serial
import time

############  Backdend ############

bmp = bytearray()

img = None

ports = [port.device for port in serial.tools.list_ports_windows.comports() ]   

ser = None

isPressed = False
isRunning = False
isDown = False

def loadImage():
    global img
    file_path = filedialog.askopenfilename(filetypes=[("PNG Images","*.png"),("All files","*.*")])
    if not file_path:
        return
    img = Image.open(file_path)
    img = img.resize((200, 200))
    tk_img = ImageTk.PhotoImage(img)
    canvas.itemconfig(img_on_canvas, image=tk_img)
    canvas.tk_img = tk_img 
    imageToBytes()

def sendContionuous(message):
    global isPressed
    if isPressed:
        send(message)
        root.after(100, sendContionuous, message)

def send(message):
    global ser
    if ser is not None and ser.is_open:
        ser.write(message)

def onPress(message):
    global isPressed
    isPressed= True
    sendContionuous(message)

def onRelease():
    global isPressed
    isPressed = False

def connect():
    global ser
    selectedPort = portVar.get()
    if selectedPort:
        print("Connecting ", selectedPort)
        ser = serial.Serial(selectedPort, 9600, timeout=1)
    else:
        print("no avaliable ports")

def toggleStart():
    global isRunning
    if not isRunning:
        send(b"S")
        start_btn.config(text="Pracuje", background="Red")
        sendBmp(bmp)
        isRunning = True
    else:
        send(b"X")
        start_btn.config(text="Start", background="Green")
        isRunning = False

def sendBmp(bmp):
    chunk_size = 32
    total_size = len(bmp)
    index = 0

    def waitReady():
        if ser.in_waiting > 0:
            line = ser.readline().decode(errors="ignore").strip()
            print("Odebrano:", line)
            if line == "READY":
                sendChunk()
            else:
                root.after(50, waitReady)
        else:
            root.after(50, waitReady)

    def sendChunk():
        nonlocal index
        if index < total_size:
            chunk = bmp[index:index+chunk_size]
            ser.write(bytearray(chunk))
            print(f"Wysłano bajty {index}–{index+len(chunk)-1}")
            index += chunk_size
            root.after(50, waitNext)
        else:
            print("Cała bitmapa wysłana!")
            waitForStopWorking()  
            
    def waitNext():
        if ser.in_waiting > 0:
            line = ser.readline().decode(errors="ignore").strip()
            if line == "NEXT":
                sendChunk()
            else:
                root.after(50, waitNext)
        else:
            root.after(50, sendChunk)
    # Start
    waitReady()

def waitForStopWorking():
    global isRunning
    while ser.in_waiting > 0:
        line = ser.readline().decode(errors="ignore").strip()
        if line == "STOP":
            start_btn.config(text="Start", background="Green")
            isRunning = False
    root.after(50, waitForStopWorking)

def toggleHead():
    global isDown
    if isDown:
        send(b"H")
        head_btn.config(text="Głowica: Podniesiona", background="green")
        isDown = False
    else:
        send(b"Z")
        head_btn.config(text="Głowica: Opuszczona", background="red")
        isDown = True
            
def imageToBytes():  # its from gpt so idk
    global img, bmp
    bmp = bytearray()
    img = img.transpose(Image.FLIP_LEFT_RIGHT)
    img = img.resize((40, 40), Image.NEAREST)
    monoImg = img.convert("1")
    pixels = monoImg.load()
    width, height = monoImg.size

    for y in range(height):
        byte = 0 
        bitsFilled = 0
        for x in range(width):

            if pixels[x,y]:
                bit = 1
            else:
                bit = 0

            byte = (byte << 1) | bit
            bitsFilled += 1

            if bitsFilled == 8:
                bmp.append(byte)
                byte = 0
                bitsFilled = 0
            
        if bitsFilled > 0:
            byte = byte << (8 -bitsFilled)
            bmp.append(byte)


########## GUI and aplication ##########

root = tk.Tk()
root.title("Obsługa CNC")
root.geometry("800x600")

# Frame left – controls
frame_left = tk.Frame(root)
frame_left.pack(side="left", fill="y", padx=10)

mainLabel = tk.Label(frame_left, text="Sterowanie maszyną CNC", font="arial")
mainLabel.pack(pady=10, padx=25)

load_btn = tk.Button(frame_left, text="Wczytaj obrazek")
load_btn.pack(pady=15)
load_btn.config(command=loadImage)

controlLabel = tk.Label(frame_left ,text="Sterowanie", foreground="blue" )
controlLabel.pack(pady=20)

# frame for buttons
control_frame = tk.Frame(frame_left)
control_frame.pack(pady=0)

# up
up_btn = tk.Button(control_frame, text="^", width=3)
up_btn.grid(row=0, column=1, padx=5, pady=5)
up_btn.bind("<ButtonPress-1>", lambda e: onPress(b"U"))
up_btn.bind("<ButtonRelease-1>", lambda e: onRelease())

# left
left_btn = tk.Button(control_frame, text="<", width=3)
left_btn.grid(row=1, column=0, padx=5, pady=5)
left_btn.bind("<ButtonPress-1>", lambda e: onPress(b"L"))
left_btn.bind("<ButtonRelease-1>", lambda e: onRelease())

# center empty
center_label = tk.Label(control_frame, text=" ", width=3)
center_label.grid(row=1, column=1)

# right
right_btn = tk.Button(control_frame, text=">", width=3)
right_btn.grid(row=1, column=2, padx=5, pady=5)
right_btn.bind("<ButtonPress-1>", lambda e: onPress(b"R"))
right_btn.bind("<ButtonRelease-1>", lambda e: onRelease())

# down
down_btn = tk.Button(control_frame, text="v", width=3)
down_btn.grid(row=2, column=1, padx=5, pady=5)
down_btn.bind("<ButtonPress-1>", lambda e: onPress(b"D"))
down_btn.bind("<ButtonRelease-1>", lambda e: onRelease())

# frame for head controls
head_frame = tk.Frame(frame_left)
head_frame.pack()

# head
head_btn = tk.Button(head_frame, text="Głowica: Podniesiona", background="green", command=toggleHead)
head_btn.grid(pady=15, row=1, column=1)

# empty label
empty_label = tk.Label(head_frame, text="", width=2)
empty_label.grid(pady=10, row=1, column=2)

# correct head height up
up_head_btn = tk.Button(head_frame, text="+", background="grey", command=lambda: send(b"V"))
up_head_btn.grid(pady=10, row=1, column=3)

# correct head height up
down_head_btn = tk.Button(head_frame, text="-", background="grey", command= lambda: send(b"N"))
down_head_btn.grid(pady=10, row=1, column=4)

frame_ports = tk.Frame(frame_left)
frame_ports.pack(pady=10)


# label for ports
portLabel = tk.Label(frame_ports, text="Wybierz odpowiedni port")
portLabel.pack(pady=20)

# chose ports 
portVar = tk.StringVar()
portWindow = ttk.Combobox(frame_ports, textvariable=portVar, values=ports)
portWindow.pack(side="left")

# connect 
connect_btn = tk.Button(frame_ports, text="Połącz", command=connect)
connect_btn.pack(padx=20, side="left")

# start
start_btn = tk.Button(frame_left, text="Start", background="green", command= lambda: toggleStart())
start_btn.pack(pady=35)

# Frame prawy – Canvas
frame_right = tk.Frame(root)
frame_right.pack(side="right", fill="both", expand=True)

canvas = tk.Canvas(frame_right, width=200, height=200, bg="gray")
canvas.pack(padx=10, pady=10)

img_on_canvas = canvas.create_image(0, 0, anchor="nw")

root.mainloop()
