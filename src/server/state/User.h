#ifndef USER_H
#define USER_H
#include <msgpack.hpp>
#include <string>
class User_t
{
public:
  User_t(){}
  User_t(std::string Name):m_Name(Name){}
  std::string m_Name;
  
  
  bool operator< (User_t const& Other) const
  {
    std::hash<std::string> HashFunction;
    auto HashOwn = HashFunction(m_Name);
    auto HashOther = HashFunction(Other.m_Name);
    return HashOwn < HashOther;
  }
  MSGPACK_DEFINE(m_Name)
  
};

#endif /* USER_H */

