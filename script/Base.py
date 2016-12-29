from os import environ, remove, path
import asyncio
import subprocess
from autobahn.asyncio.wamp import ApplicationSession, ApplicationRunner
from autobahn.wamp.types import PublishOptions
from autobahn.wamp import auth

import time 
import _thread
import sys
def Start(ToRun, RealmName="realm1"):
    Runner = ApplicationRunner(environ.get("AUTOBAHN_DEMO_ROUTER", u"ws://127.0.0.1:8080/ws"), RealmName)
    Runner.run(ToRun)
  
class Base(ApplicationSession):

  def StartSTC_Server(self):
    self.RemoveDB() # Need to remove existing DB in order to start with a clean Lobby.
    args = ("../build/STC_Server")
    self.popen = subprocess.Popen(args, stdout=subprocess.PIPE)
    print("Started STC_Server...Sleeping 0.3s to give the binary some start up time")
    time.sleep(1.0) #This should really be fixed. Not quite sure what to do here query WAMP router for number joined?
  
  def RemoveDB(self):
    if path.isfile('db'):
        remove('db')

  def Exit(self):
    print("COMPLETED SUCCESSFULLY... Will now raise KeyboardInterrupt to close script")
    self.popen.terminate()
    _thread.interrupt_main() #Exit with KeyboardInterrupt

  async def onJoin(self, details):
    for Test in self.GetTests():
        await Test()

    self.Exit()   

  async def onConnect(self):  
    #self.StartSTC_Server()
    self.join(u"realm1", [u"wampcra"], u"dev")

  async def onChallenge(self, challenge):
      USER_SECRET=u'dev'
      if challenge.method == u"wampcra":
         print("WAMP-CRA challenge received: {}".format(challenge))

         if u'salt' in challenge.extra:
            # salted secret
            key = auth.derive_key(USER_SECRET,
                                  challenge.extra['salt'],
                                  challenge.extra['iterations'],
                                  challenge.extra['keylen'])
         else:
            # plain, unsalted secret
            key = USER_SECRET

         # compute signature for challenge, using the key
         signature = auth.compute_wcs(key, challenge.extra['challenge'])

         # return the signature to the router for verification
         return signature

      else:
        raise Exception("Invalid authmethod {}".format(challenge.method))
