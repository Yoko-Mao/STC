from twisted.internet.defer import inlineCallbacks
from Base import Base

class Lobby(Base):

    def __init__(self, RealmName):
        Base.__init__(self, RealmName)

    def Test(self):
        import ipdb
        ipdb.set_trace()
    @inlineCallbacks
    def onJoin(self, details):
        reply = yield self.call(u'STC.lobby.users.add', 'william')
        self.log.info('result: {}'.format(reply))

        reply = yield self.call(u'STC.lobby.users.get')

        for UserName in reply.results[1]:
            self.log.info('Got user: {}'.format(UserName))
        #yield self.leave()
        #self.Test()

if __name__ == '__main__':
    session = Lobby(u'realm1')
