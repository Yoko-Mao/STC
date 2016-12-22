from Base import Base, Start
from enum import Enum

class WorkOrderResult_t(Enum):
    SUCCESS = 0
    ERROR = 1
    DUPLICATE = 2

class Lobby_t:
    def __init__(self):
        self.UserNames = []
        self.Session = None

    def Init(self, Session):
        self.Session = Session

    def OnUserUpdate(self,IsAdded, UserName):
        if IsAdded:
            self.UserNames.append(UserName)

    async def Subscribe(self):
        await self.Session.subscribe(self.OnUserUpdate, u'STC.Lobby.Users.Update')

    async def AddUser(self, UserName, ExpectedResult=WorkOrderResult_t.SUCCESS.value):
        Result = await self.Session.call(u'STC.Lobby.Users.Add', UserName)
        assert Result is ExpectedResult, "AddUser did not have expected result. Got {0} and Expected {1}".format(Result, ExpectedResult)

class LobbyTest_t(Base):
   ## Used in base class to obtain list of test actions that should be ran
   def GetTests(self):
        return [self.Setup, self.Test_AddUser]
   
   async def Setup(self):
        Lobby.Init(self)
 
   ## Test the functionalities to add and remove users
   #
   #  1. Start with empty server (First test)
   #  2. Add user 'william'
   #  3. Verify that one user is present in the lobby.
   #  4. Add a new user to the lobby with name 'william'. As the name is unique this should fail.
   #  5. Verify that one user is still present in the lobby.
   #  6. Add users 'Yoko', 'Doe', 'Rae' and 'Me'. All these calls are expected to succeed.
   #  7. Verify that five users are present in the lobby.
   async def Test_AddUser(self):
        await Lobby.Subscribe()  
        await Lobby.AddUser('william')
        assert len(Lobby.UserNames) is 1, "One user was expected to be present"
        await Lobby.AddUser('william', WorkOrderResult_t.DUPLICATE.value)
        assert len(Lobby.UserNames) is 1, "One user was still expected to be present after adding duplicate"
        
        for User in ['Yoko', 'Doe', 'Rae', 'Me']:
            await Lobby.AddUser(User)
            
        assert len(Lobby.UserNames) is 5, "Five users were expected to be present after adding duplicates"
        
if __name__ == '__main__':
    Lobby = Lobby_t()
    Start(LobbyTest_t)
