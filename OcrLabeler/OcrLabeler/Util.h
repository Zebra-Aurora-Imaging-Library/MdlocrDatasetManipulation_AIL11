#pragma once
#include "Header.h"

static void splitString(std::wstring str, std::vector<wstring> &vec, std::wstring delimiter = L",")
   {
   size_t pos = 0;
   wstring token;

   while((pos = str.find(delimiter)) != std::wstring::npos)
      {
      token = str.substr(0, pos);
      vec.push_back(token);
      str.erase(0, pos + delimiter.length());
      }
   if(str.size() > 0)
      vec.push_back(str);
   return;
   }