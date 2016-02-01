import das_client
from sys import argv
import os

inputFile = argv[1] #contains a list of files that you're running over
output = "secondaryMap = {"
with open(inputFile,'r') as f:
	for x in f:
		x = x.rstrip()
		output=output+'\n    "'+x+'": ['
		x = x.split('/store',1)[-1] 
		x = '/store'+x #gets rid of any xrootd info in the filename
		das_query_string = 'das_client.py --query="parent file=%s" --limit=0' % x
		das_output = os.popen(das_query_string).read() #query DAS via bash
                xs1 = das_output.replace("\n"," ") #don't want newline in string
		xs2 = xs1.split()
                #xs2.remove('')
                #print 'xs2'
                #print len(xs2)
                #print xs2
                das_output_sum = ''
                for aod in xs2:
                    #print 'aod'
                    #print aod
                    das_query_string2 = 'das_client.py --query="parent file=%s" --limit=0' % aod
		    das_output2 = os.popen(das_query_string2).read() #query DAS via bash
		    das_output = das_output2.replace("root","root',")#commas between each secondary file
		    das_output = das_output.replace("/store","'root://cmsxrootd.fnal.gov//store") #need xrootd info in secondary filename
                    das_output = das_output.replace("\n","") #don't want newline in string
                    #print das_output
		    das_output_sum += das_output#add all secondary RAW files found!
		das_output_sum = das_output_sum[:-1]#remove last comma in list of secondary files
		output = output+das_output_sum+'],'#end list of secondary files
output=output[:-1]#remove last comma
output=output+"\n}"#end dictionary
print output
