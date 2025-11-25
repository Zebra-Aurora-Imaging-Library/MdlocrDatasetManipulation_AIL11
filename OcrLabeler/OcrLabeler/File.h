#pragma once
#include "Header.h"


class File
   {
   private:

      bool validity;
      std::wstring path;
      std::wstring root;
      std::wstring localPath;
      std::wstring localRoot;
      std::wstring parent;
      std::wstring name;
      std::wstring nameNoExt;
      std::wstring ext;
      std::wstring label;
      std::wstring AugmentationType;
      std::wstring blob;

      std::list<std::wstring> valid_extensions = {
         L"jpg",
         L"jpeg",
         L"png",
         L"bmp",
         L"tif",
         L"tiff",
         L"mim"
         };

      wstring tolower(wstring str)
         {
         std::transform(str.begin(), str.end(), str.begin(), ::tolower);
         return str;
         }

      bool formatIsValid(std::wstring Extension)
         {
         auto ext = tolower(Extension);
         bool found = (std::find(valid_extensions.begin(), valid_extensions.end(), ext) != valid_extensions.end());
         return found;
         }

   public:

      File(std::wstring path, std::wstring RootDir)
         {
         //std::wcout << path << std::endl;
         this->path = path;
         this->root = RootDir;
         std::vector<std::wstring> Tokens;
         this->validity = extractTokens(RootDir, path, Tokens);

         if(validity)
            {
            this->localRoot        = Tokens[0];
            this->parent           = Tokens[1];
            this->name             = Tokens[2];
            this->nameNoExt        = Tokens[3];
            this->ext              = Tokens[4];
            this->label            = Tokens[5];
            this->blob             = Tokens[6];
            this->AugmentationType = Tokens[7];
            this->localPath        = Tokens[8];
            }
         }

      std::wstring getPath()
         {
         return this->path;
         }
      std::wstring getLocalPath()
         {
         return this->localPath;
         }
      std::wstring getRoot()
         {
         return this->root;
         }
      std::wstring getLocalRoot()
         {
         return this->localRoot;
         }
      std::wstring getParent()
         {
         return this->parent;
         }
      std::wstring getName()
         {
         return this->name;
         }
      std::wstring getLabel()
         {
         return this->label;
         }
      std::wstring getBlob()
         {
         return this->blob;
         }
      std::wstring getAugmentationType()
         {
         return this->AugmentationType;
         }
      std::wstring getExtension()
         {
         return this->ext;
         }
      std::wstring getNameNoExt()
         {
         return this->nameNoExt;
         }

      bool isValid()
         {
         return validity;
         }

      bool extractTokens(std::wstring RootDir, std::wstring FileName, std::vector<std::wstring> &Tokens)
         {
         std::wstring LocalPath = FileName.substr(RootDir.length());
         if(LocalPath.at(0) == '\\')
            LocalPath = LocalPath.substr(1);

         //std::wstring LocalPath = Path.substr(RootDir.length(), nameStartInd - RootDir.length());

         auto nameStartInd = LocalPath.find_last_of('\\') + 1;
         auto pointInd = LocalPath.find_last_of('.');

         std::wstring name = LocalPath.substr(nameStartInd);
         std::wstring nameNoExt = LocalPath.substr(nameStartInd, pointInd - nameStartInd);
         std::wstring extension = LocalPath.substr(pointInd + 1);
         std::wstring LocalDir = LocalPath.substr(0, nameStartInd - 1);
         std::wstring parent = LocalDir.substr(LocalDir.find_last_of(L"\\") + 1, LocalDir.length());

         // depends on the structure
         std::wstring label = LocalPath.substr(0, LocalPath.find_first_of(L'\\'));

         if(nameStartInd == 0)
            {
            LocalDir = L"";
            parent = L"";
            label = L"";
            }

         Tokens.push_back(LocalDir + L"\\");
         Tokens.push_back(parent);
         Tokens.push_back(name);
         Tokens.push_back(nameNoExt);
         Tokens.push_back(extension);
         Tokens.push_back(label);

         std::wstring data = LocalPath.substr(nameStartInd, pointInd - nameStartInd);

         AIL_INT BlobIndex = data.find_last_of(L"blob") + 1;
         AIL_INT PatchIndex = data.find_last_of(L"patch") + 1;

         std::wstring  BlobId = data.substr(BlobIndex, 1);
         std::wstring  PatchId = data.substr(PatchIndex, 2);
         if(PatchId.size() == 1)
            {
            //std::wcout << "Warning:: check the image name; patchid is not valid." << std::endl;
            PatchId = L"0" + PatchId;
            }

         Tokens.push_back(BlobId);
         Tokens.push_back(PatchId);
         Tokens.push_back(LocalPath);

         return formatIsValid(extension);
         }
   };

