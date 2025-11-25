#pragma once
#include "File.h"
#include "Util.h"

#define PATCH_EXTRACTION 1
static vector<wstring> DEFAULT_VECTOR;

class FileManager
   {
   private:
      std::vector<File> Patches;
      std::vector<std::vector<File>> Images;

   public:

      void saveMilBuf(wstring RootDir, wstring Filename, AIL_ID Buffer)
         {
         fs::create_directories(RootDir);
         MbufExport(RootDir + Filename, M_USE_EXTENSION, Buffer);
         return;
         }

      void createDirs(wstring RootDir)
         {
         fs::create_directories(RootDir);
         return;
         }

      void loadPatches(std::wstring RootDir, std::vector<File> &Images, std::vector<AIL_INT> *Indexes = NULL, int ImagesPerIndex = 1)
         {
         // Load Images
         // Folders
         AIL_INT ind = 0;
         AIL_INT imgs = 0;
         RootDir += L"\\";
         for(auto & p : fs::recursive_directory_iterator(RootDir))
            {
            if(!p.is_directory())
               {
               File tempFile(p.path().wstring(), RootDir);
               if(tempFile.isValid())
                  {
                  Images.push_back(tempFile);
                  imgs++;
                  if(tempFile.getAugmentationType().compare(NO_AUGMENT_FLAG) == 0 && Indexes != NULL)
                     {
                     Indexes->push_back(ind);
                     ind++;
                     if(imgs != ImagesPerIndex)
                        wcout << "Warning : Incorrect number of images in a batch! " << imgs << " != " << ImagesPerIndex << "\n";
                     imgs = 0;
                     }
                  }
               else
                  std::wcout << L"Invalid Image Format! :: " << p.path().wstring() << std::endl;
               }
            }
         return;
         }

      void importCSV(std::wstring path, vector<vector<wstring>> &csvFile, vector<wstring>& header,wstring delim = L",")
         {
         bool readHeader = false;

         wifstream csvStream;
         csvStream.open(path);

         wstring line;
         vector<wstring> lineVector;
         while(getline(csvStream, line))
            {
            splitString(line, lineVector, delim);
            if(!readHeader)
               {
               header = lineVector;
               readHeader = true;
               }
            else
               {
               csvFile.push_back(lineVector);
               }
            lineVector.clear();
            }
         return;
         }

      void exportCSV(std::wstring path, vector<vector<wstring>> &csvData, vector<wstring> &header = DEFAULT_VECTOR, wstring delim = L",")
         {
         wofstream csvFile;
         csvFile.open(path);

         if(header.size() > 0)
            {
            for(auto h : header)
               {
               csvFile << h << delim << " ";
               }
            csvFile << endl;
            }

         for(auto row : csvData)
            {
            for(auto data : row)
               {
               csvFile << data << delim << " ";
               }
            csvFile << endl;
            }

         csvFile.close();
         return;
         }

      void writeToFile(wstring path, wstring str)
         {
         std::wofstream fileStream(path, std::ofstream::out);
         fileStream << str;
         }
   };

