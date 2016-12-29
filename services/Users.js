var UserTable = class
{
    constructor(Database)
    {
      this.Database = Database;
      this.Database.run("CREATE TABLE IF NOT EXISTS users(ID INTEGER PRIMARY KEY AUTOINCREMENT, UserName TEXT)");
    }

    Insert(args)
    {
      var UserName = args[0]
      var Statement = this.Database.prepare("INSERT INTO users (UserName) VALUES (?)");
      Statement.run(UserName);
      Statement.finalize();
    }

    
};

module.exports = 
{
  GetTable:function(Database)
  {
    return new UserTable(Database);
  }
};

