import serial
import serial.tools.list_ports
import msvcrt
import datetime
import statistics

baundRate = 115200
data=[]

def main():
    allSerialPorts()
    comNumb = str(input("COM_ :"))
    print("Wybrales COM",comNumb)
    listenPort(comNumb)


def allSerialPorts():
    ports=serial.tools.list_ports.comports()
    for i in ports:
        print(i)

def listenPort(portNum):
    global data
    ser=serial.Serial("COM"+portNum,baundRate,timeout=1)
    while True:
        newRead=ser.readline()
        newRead.decode("utf-8")
        if newRead:
            data.append(newRead)
            print(data)
        
        if msvcrt.kbhit():
            button=msvcrt.getch().decode("utf-8", errors="ignore")
            if button=="c":
                print("Koniec nasluchiwania")
                saveData()
                break

def saveData(fileName="plik"):
    with open(fileName+".txt", "a", encoding="utf-8") as file:
        for line in data:
            file.write(datetime.datetime.now().strftime("[%Y:%m:%d-%H:%M:%S.%f]")[:-3] + "] --- "+line.decode("utf-8")+"\n")
    
main()