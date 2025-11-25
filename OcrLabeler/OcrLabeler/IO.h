#pragma once
#include "Header.h"

#define TYPE_STRING  0
#define TYPE_INT     1
#define TYPE_DOUBLE  2

struct option
   {
   size_t id;
   wstring name;
   wstring value;
   wstring flag;
   bool optional;
   bool available = false;
   };

class IO
   {
   private:

      vector<option> options;
      int NbArguments = 1;
      int NbOptions = 0;

      bool findByName(wstring name, option &arg)
         {
         for(auto opt : options)
            {
            if(name.compare(opt.name) == 0)
               {
               arg = opt;
               return true;
               }
            }
         return false;
         }

      bool findByFlag(wstring flag, option &arg)
         {
         for(auto &opt : options)
            {
            if(flag.compare(opt.flag) == 0)
               {
               arg = opt;
               return true;
               }
            }
         return false;
         }

      std::wstring str_tolower(std::wstring s)
         {
         std::transform(s.begin(), s.end(), s.begin(),
                        [](wchar_t c) { return std::tolower(c); }
         );
         return s;
         }

   public:

      bool isAvailable(wstring name)
         {
         for(auto &opt : options)
            {
            if(name.compare(opt.name) == 0)
               {
               return opt.available;
               }
            }
         return false;
         }

      bool isValid(wstring name)
         {
         for(auto opt : options)
            {
            if(name.compare(opt.name) == 0)
               {
               return true;
               }
            }
         return false;
         }

      bool parse(int argc, char** argv)
         {
         if(argc < NbArguments)
            {
            wcout << "Invalid input format!\n";
            wcout << "Valid Format: ";

            for(auto &opt : options)
               {
               if(!opt.flag.empty())
                  wcout << "-" << opt.flag << " ";

               wcout << opt.name << " ";
               }

            wcout << endl;

            return false;
            }

         for(size_t i = 0; i < argc - 1; i++)
            {
            if(argv[i + 1][0] != '-')
               {
               options[i].value = std::wstring(argv[i + 1], argv[i + 1] + std::strlen(argv[i + 1]));
               options[i].available = true;
               }
            else
               {
               option opt;
               wstring flag = std::wstring(argv[i + 1], argv[i + 1] + std::strlen(argv[i + 1]));
               flag = flag.substr(1);
               i++;
               if(findByFlag(flag, opt))
                  options[opt.id].value = std::wstring(argv[i + 1], argv[i + 1] + std::strlen(argv[i + 1]));
               options[opt.id].available = true;
               }
            }

         wcout << "Received elements:\n";
         for(auto &opt : options)
            {
            if(!opt.value.empty())
               wcout << " ." << opt.name << " \t: " << opt.value << endl;
            }

         wcout << endl;

         return true;
         }

      wstring getString(wstring name)
         {
         option opt;
         if(findByName(name, opt))
            return opt.value;
         else
            return L"None";
         }

      int getInt(wstring name)
         {
         option opt;
         if(findByName(name, opt))
            if(!opt.value.empty())
               return stoi(opt.value);
         return 0;
         }

      double getDouble(wstring name)
         {
         option opt;
         if(findByName(name, opt))
            return stod(opt.value);
         return -1;
         }

      void addElement(wstring name, wstring flag = L"")
         {
         option opt;
         opt.id = options.size();
         opt.flag = flag;
         opt.name = name;

         options.push_back(opt);
         if(!flag.empty()) // add the flag as input argument
            NbOptions++;
         else
            NbArguments++;

         //if(!flag.empty())
         //   {
         //   flags.insert(pair<wstring, wstring>(flag, name));
         //   NbOptions++;
         //   }
         //arguments.insert(pair<wstring, pair<wstring, int>>(name, pair<wstring, int>("value", NbArguments)));

         return;
         }

      void splitString(wstring str, vector<int> &vec, wstring delimiter = L",")
         {
         str = str_tolower(str);

         size_t pos = 0;
         wstring token;

         while((pos = str.find(delimiter)) != std::wstring::npos)
            {
            token = str.substr(0, pos);
            vec.push_back(stoi(token));
            str.erase(0, pos + delimiter.length());
            }
         vec.push_back(stoi(str));
         return;
         }
   };