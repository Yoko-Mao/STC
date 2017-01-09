from Base import Base, Start
from enum import Enum

async def RegisterUser(Session, UserName):
    "calling RegisterUser"
    Result = await Session.call(u'stc.database.users.create',UserName)
    "Called RegisterUser"
 
class WorkOrderResult_t(Enum):
    SUCCESS = 0
    ERROR = 1
    DUPLICATE = 2
    NOT_FOUND = 3


class State_t:
    def __init__(self):
        self.UserNames = []
        self.Session = None

    def Init(self, Session):
        self.Session = Session

    def OnUserUpdate(self,IsAdded, UserName):
        if IsAdded:
            self.UserNames.append(UserName)
        else:
            self.UserNames.remove(UserName)

    async def Subscribe(self):
        await self.Session.subscribe(self.OnUserUpdate, u'STC.State.Users.Update')

    async def AddUser(self, UserName, ExpectedResult=WorkOrderResult_t.SUCCESS):
        Result = await self.Session.call(u'STC.State.Users.Add', UserName)
        assert Result is ExpectedResult.value, "AddUser did not have expected result. Got {0} and Expected {1}".format(WorkOrderResult_t(Result), ExpectedResult.name)

    async def RemoveUser(self, UserName, ExpectedResult=WorkOrderResult_t.SUCCESS):
        Result = await self.Session.call(u'STC.State.Users.Remove', UserName)
        assert Result is ExpectedResult.value, "RemoveUser did not have expected result. Got {0} and Expected {1}".format(WorkOrderResult_t(Result), ExpectedResult.name)


class StateTest_t(Base):
   ## Used in base class to obtain list of test actions that should be ran
   def GetTests(self):
        return [self.Setup, self.Test_AddUser]
   
   async def Setup(self):
        State.Init(self)

   ## Test the functionalities to add and remove users
   #
   #  1. Start with empty server (First test)
   #  2. Add user 'william'
   #  3. Verify that one user is present in the state.
   #  4. Add a new user to the state with name 'william'. As the name is unique this should fail.
   #  5. Verify that one user is still present in the state.
   #  6. Add users 'Yoko', 'Doe', 'Rae' and 'Me'. All these calls are expected to succeed.
   #  7. Verify that five users are present in the state.
   #  8. Remove user 'william'.
   #  9. Verify that four users are present in the state.
   # 10. Remove user 'william'. This call is expected to fail. User does not exist
   # 11. Verify that four users are present in the state.
   #
   async def Test_AddUser(self):
        "MAde it here"
        await RegisterUser(self, u'william');    
        #await State.Subscribe()  
        #await State.AddUser('william')
        #assert len(State.UserNames) is 1, "One user was expected to be present"
        #await State.AddUser('william', WorkOrderResult_t.DUPLICATE)
        #assert len(State.UserNames) is 1, "One user was still expected to be present after adding duplicate"
        #
        #for User in ['Yoko', 'Doe', 'Rae', 'Me']:
        #    await State.AddUser(User)
        #    
        #assert len(State.UserNames) is 5, "Five users were expected to be present after adding duplicates"
        #await State.RemoveUser('william')
        #assert len(State.UserNames) is 4, "Four users were expected to be present after removing one"
        #await State.RemoveUser('william', WorkOrderResult_t.NOT_FOUND)
        #assert len(State.UserNames) is 4, "Four users were expected to be present since a non existing user cannot be removed"

        
if __name__ == '__main__':
    State = State_t()
    Start(StateTest_t)
