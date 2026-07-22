import serial
import serial.tools.list_ports
import msvcrt
import datetime
import statistics

baundRate = 115200
data=[]
dataTime=[]

def main():
    printAllSerialPorts()
    
    listenPort()


def printAllSerialPorts():
    ports=serial.tools.list_ports.comports()
    for i in ports:
        print(i)

def listenPort():
    global data
    global dataTime
    
    while(True):
        
        portNumb = str(input("COM_ :"))
        print("Wybrales COM",portNumb)
        
        try:
            ser=serial.Serial("COM"+portNumb,baundRate,timeout=1)
            break
        except:
            print("Nie mozna otworzyc wybranego portu COM!\n")
            
    while True:
        newRead=ser.readline()
        newRead.decode("utf-8")
        if newRead:
            data.append(newRead)
            dataTime.append(datetime.datetime.now().strftime("[%Y:%m:%d-%H:%M:%S.%f]")[:-3] + "] --- ")
            print(data)
        
        if msvcrt.kbhit():
            button=msvcrt.getch().decode("utf-8", errors="ignore")
            if button=="c":
                print("Koniec nasluchiwania")
                saveData()
                break

def saveData(fileName="pc_app\\reads\\"):
    fileName=fileName+datetime.datetime.now().strftime("%Y-%m-%d--%H-%M")
    with open(fileName+".txt", "a", encoding="utf-8") as file:
        for line1, line2 in zip(data,dataTime):
            file.write(line2+line1.decode("utf-8")+"\n")
    file.close()
main()