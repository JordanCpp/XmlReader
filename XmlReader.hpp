#ifndef _XmlReader_hpp
#define _XmlReader_hpp

#include <string>
#include <iostream>
#include <fstream>

class XmlReader
{
public:
  enum
  {
    OpeningTag,
    ClosingTag,
    Value
  };
  
  enum
  {
    OnCheck,
    OffCheck
  };
  
  bool IsOpeningTag();
  bool IsClosingTag();
  bool IsValue();
  bool IsOnCheck();
  bool IsOffCheck();
  XmlReader(const std::string & Name, size_t Check = XmlReader::OnCheck);
  ~XmlReader();
  char NextChar();
  bool Next(std::string & Dest);
  bool NextOpeningTag(const std::string & Name, std::string & Dest);
  bool NextClosingTag(const std::string & Name, std::string & Dest);
  bool NextValue(std::string & Dest);
  bool NextNode(const std::string & Name, std::string & Buffer, std::string & Value);
private:
  std::ifstream mInput;
  size_t mType;
  size_t mTabs;
  size_t mLine;
  size_t mCheck;
};

#endif
