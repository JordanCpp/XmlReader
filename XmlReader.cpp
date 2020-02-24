#include "XmlReader.hpp"

bool XmlReader::IsOpeningTag()
{
  return mType == XmlReader::OpeningTag;
}

bool XmlReader::IsClosingTag()
{
  return mType == XmlReader::ClosingTag;
}

bool XmlReader::IsValue()
{
  return mType == XmlReader::Value;
}

bool XmlReader::IsOnCheck()
{
  return mCheck == XmlReader::OnCheck;
}

bool XmlReader::IsOffCheck()
{
  return mCheck == XmlReader::OffCheck;
}

XmlReader::XmlReader(const std::string & Name, size_t Check)
{
  mInput.open(Name.c_str());

  if (mInput.is_open())
  {
    mLine = 1;
    mTabs = 0;

    mCheck = Check;
  }
  else
  {
    std::cout << "Not open File:" + Name << std::endl;
    std::terminate();
  }
}

XmlReader::~XmlReader()
{
  mInput.close();
}

char XmlReader::NextChar()
{
  char ch;

  mInput.get(ch);
  mTabs++;

  if (ch == '\n')
  {
    mTabs = 1;
    mLine++;
  }

  return ch;
}

bool XmlReader::Next(std::string & Dest)
{
  Dest.clear();

  char ch = NextChar();

  while (isspace(ch))
  {
    ch = NextChar();
  }

  bool Result = false;
  
  if (mInput.eof())
  {
    Result = false;
  }
  else if (ch == '<')
  {
    ch = NextChar();

    if (ch == '/')
    {
      mType = XmlReader::ClosingTag;
      ch = NextChar();
    }
    else
    {
      mType = XmlReader::OpeningTag;
    }

    while ((ch != '>') && (ch != '\n'))
    {
      Dest.push_back(ch);
      ch = NextChar();
    }

    Result = true;
  }
  else
  {
    mType = XmlReader::Value;

    while ((ch != '<') && (ch != '\n'))
    {
      Dest.push_back(ch);
      ch = NextChar();
    }

    if (ch == '<')
    {
      mInput.unget();
    }

    Result = true;
  }

  return Result;
}

bool XmlReader::NextOpeningTag(const std::string & Name, std::string & Dest)
{
  bool Result = false;

  if (Next(Dest) && IsOpeningTag() && Dest == Name)
  {
    Result = true;
  }
  else
  {
    Result = false;
  }

  if (IsOnCheck() == true && Result == false)
  {
    std::cout << "Not found opening tag:" << Name << std::endl;
    std::terminate();
  }

  return Result;
}

bool XmlReader::NextClosingTag(const std::string & Name, std::string & Dest)
{
  bool Result = false;

  if (Next(Dest) && IsClosingTag() && Dest == Name)
  {
    Result = true;
  }
  else
  {
    Result = false;
  }

  if (IsOnCheck() == true && Result == false)
  {
    std::cout << "Not found closing tag:" << Name << std::endl;
    std::terminate();
  }

  return Result;
}

bool XmlReader::NextValue(std::string & Dest)
{
  bool Result = false;

  if (Next(Dest) && IsValue())
  {
    Result = true;
  }
  else
  {
    Result = false;
  }

  if (IsOnCheck() == true && Result == false)
  {
    std::cout << "Not found value" << std::endl;
    std::terminate();
  }

  return Result;
}

bool XmlReader::NextNode(const std::string & Name, std::string & Buffer, std::string & Value)
{
  Value.clear();

  bool Result = false;

  if (NextOpeningTag(Name, Buffer))
  {
    if (NextValue(Buffer))
    {
      Value.append(Buffer);

      if (NextClosingTag(Name, Buffer))
      {
        Result = true;
      }
    }
  }

  if (IsOnCheck() == true && Result == false)
  {
    std::cout << "Not found node:" << Name << std::endl;
    std::terminate();
  }

  return Result;
}
