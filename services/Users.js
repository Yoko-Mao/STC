
var UserTable = class
{
    constructor(Database)
    {
      this.Database = Database;
      this.Database.run("CREATE TABLE IF NOT EXISTS users(ID INTEGER PRIMARY KEY AUTOINCREMENT, UserName TEXT)");
    }

    Create(args)
    {
      //var UserName = args[0]
      //var Statement = this.Database.prepare("INSERT INTO users (UserName) VALUES (?)");
      //Statement.run(UserName);
      //Statement.finalize();
      Console.log("Inserted??");
      return 1337;
    } 
    
    Read(args)
    {
      var UserName = args[0]
      this.Database.each("SELECT (ID, UserName) FROM users WHERE (UserName) IS (?)",function(err,row){
        return [row.ID, row.UserName];
      });
   }  
};

module.exports = 
{
  GetTable:function(Database)
  {
    return new UserTable(Database);
  }
};

