from os import environ
import asyncio
import subprocess
from autobahn.asyncio.wamp import ApplicationSession, ApplicationRunner
import time 
import _thread
import sys
def Start(ToRun, RealmName="realm1"):
    Runner = ApplicationRunner(environ.get("AUTOBAHN_DEMO_ROUTER", u"ws://127.0.0.1:8080/ws"), RealmName)
    Runner.run(ToRun)
  
class Base(ApplicationSession):
  def StartSTC_Server(self):
    args = ("../build/STC_Server")
    self.popen = subprocess.Popen(args, stdout=subprocess.PIPE)
    print("Started STC_Server...Sleeping 0.1s to give the binary some start up time")
    time.sleep(0.1) #This should really be fixed. Not quite sure what to do here query WAMP router for number joined?
  
  def Exit(self):
    print("COMPLETED SUCCESSFULLY... Will now raise KeyboardInterrupt to close script")
    self.popen.terminate()
    _thread.interrupt_main() #Exit with KeyboardInterrupt

  async def onJoin(self, details):
    self.StartSTC_Server()
    for Test in self.GetTests():
        await Test()

    self.Exit()   

