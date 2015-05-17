#Voltage from the solar cells is cut by 1/4. So multiply all results by 4
import urllib2
import datetime
#import urllib.request
scaler = 4.0
v1, v2, v3 = -1
v1 = scaler*float(urllib2.urlopen("http://myArduino.local/arduino/4").read())
#How do I know if it's safe to turn on the lower ones? By wattage? (300W at 0.5ohms, 100W at 1.5ohms, 30W at 5ohms)
if ((v1 * v1 / 0.5) < 100):
	v2 = scaler*float(urllib2.urlopen("http://myArduino.local/arduino/5").read())
if ((v1 * v1 / 1.5) < 30):
	v3 = scaler*float(urllib2.urlopen("http://myArduino.local/arduino/6").read())

w1 = v1*v1/0.5
w2 = v2*v2/1.5
w3 = v3*v3/5.0

f = open("SolarLog.txt",'a')
s = "%f\t%f\t%f\t%f\t%f\t%f\t%s\n"%(w1,w2,w3,v1,v2,v3,str(dattime.datetime.now())
f.write(s)
f.close()
