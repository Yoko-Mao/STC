var sqlite3 = require('sqlite3').verbose();
var Database = new sqlite3.Database("db");
var Autobahn = require('autobahn');
var Users = require('./Users');
var CurrentSession = null;
var UserTable = Users.GetTable(Database);

function RegisterRPC(Name, Functor)
{
  CurrentSession.register(Name, Functor).then(
     function (reg) 
     {
        console.log("Registered "+ Name +" RPC.");
     },
     function (err) 
     {
        console.log("Failed registration of "+ Name +" RPC: " + err);
     }
  );
}

function onchallenge (session, method, extra) 
{
   if (method === "wampcra") 
   {
      console.log("authenticating via '" + method + "' and challenge '" + extra.challenge + "' using pw '"+ process.argv[2] +"'");

      return autobahn.auth_cra.sign(process.argv[2], extra.challenge);
   }
   else if (method === "ticket")
   {
      return process.argv[2];
   }    
   else 
   {
      throw "don't know how to authenticate using '" + method + "'";
   }
}


var Connection = new Autobahn.Connection({
  url: 'ws://127.0.0.1:8080/database',
  realm: 'realm1',
  authid: 'database',
  authmethods: ["ticket"],
  onchallenge: onchallenge
});

Connection.onopen = function (Session) 
{
  CurrentSession = Session; 
  RegisterRPC("stc.database.users.insert",UserTable.Insert);
};

Connection.open();
process.on('beforeExit', function() { Database.close(); Connection.close();});
