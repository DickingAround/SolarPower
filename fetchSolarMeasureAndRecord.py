#Voltage from the solar cells is cut by 1/4. So multiply all results by 4
import urllib2
import datetime
#import urllib.request

con = urllib2.urlopen("http://servo2.local/arduino/0")
s = con.readlines()
con.close
f = open("./production/SolarPower/SolarLog/SolarLog_%s.txt"%str(datetime.datetime.now()),'w')
for line in s:
	f.write(line)
f.close()
