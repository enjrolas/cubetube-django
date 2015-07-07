import subprocess
import os

accessToken="23c1dcf933408f9443606b41304fe0502c246ddf" 
deviceID="3c002f000f47343339383037"
filename="application.ino"
directory="photonListener"
command=['node', 'directoryflash.js', '%s' % accessToken, '%s' % deviceID, "photonListener"]
#command=['node', 'cloudflash.js', '%s' % accessToken, '%s' % deviceID, '%s' % filename]
print(command)
p = subprocess.Popen(command, stdout=subprocess.PIPE, stderr=subprocess.PIPE, cwd="%s%s" % ('/home/glass/cubetube-testing/media/', 'cloudware'))
output=[]
i=0
jsonResult=""
for line in p.stdout.readlines():
    line=line.replace('Device flashing started successfully: ', '')
    jsonResult="%s%s" % (jsonResult, line)
    i+=1
    
print jsonResult
