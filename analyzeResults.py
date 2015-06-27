import os

#J = 0.5 * C * V^2
#J = 0.5 * 10000E-6 * ((Vt2-Vb2)^2 - (Vt1 - Vb1)^2)
#W = J / T
#W = J / (T1 - T2)

def parseLine(line):
	parts = line.split('\t')
	#print parts
	time = long(parts[0])
	vHigh = float(parts[1])*10.0
	vLow = float(parts[2])*10.0
	return time,vHigh,vLow,vHigh-vLow

def computePower(timeA,timeB,vDiffA,vDiffB):
	return 0.5 * 10000E-6 * (vDiffB*vDiffB-vDiffA*vDiffA) / ((timeB-timeA)*1E-6)

def parseFile(fileName):
	#print fileName
	fr = open("./SolarLog/"+fileName,'r')
	fw = open("./SolarResults/wattage"+fileName,'w')
	maxPower = 0
	#Parse the first line
	for i in range(10):
		fr.readline()
	timeA,vHighA,vLowA,vDiffA = parseLine(fr.readline())
	for line in fr.readlines():
		timeB,vHighB,vLowB,vDiffB = parseLine(line)
		if((vDiffB-vDiffA) > 1.0): #Wait till there's a 1 volt difference
			power = computePower(timeA,timeB,vDiffA,vDiffB)	
			if(power > maxPower):
				maxPower = power;
			#fw.write("%i,%i,%f,%f\n"%(timeA,timeB,vDiffA,vDiffB))
			fw.write("%f\t%f\n"%((vHighB+vHighA)/2.0,power))	
			timeA = timeB
			vHighA = vHighB
			vLowA = vLowB	
			vDiffA = vDiffB
	print(fileName + "\t%f"%maxPower)
	fr.close()
	fw.close()

fileList = os.listdir("./SolarLog/")
#print fileList
for f in fileList:
	parseFile(f)
		
