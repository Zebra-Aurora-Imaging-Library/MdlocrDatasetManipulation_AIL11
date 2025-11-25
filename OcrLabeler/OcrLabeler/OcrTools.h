#pragma once
#include "Header.h"

//// BOUNDING BOX DEFINITION ////
class BoundingBox
   {
   friend class Entry;
   friend class Dataset;

   private:
      AIL_STRING m_className;
      AIL_DOUBLE m_TLX, m_TLY, m_TRX, m_TRY, m_BLX, m_BLY, m_BRX, m_BRY;

   public:
      BoundingBox(const AIL_STRING& className = L"", AIL_DOUBLE TLX = -1.0, AIL_DOUBLE TLY = -1.0, AIL_DOUBLE TRX = -1.0, AIL_DOUBLE TRY = -1.0,
                  AIL_DOUBLE BLX = -1.0, AIL_DOUBLE BLY = -1.0, AIL_DOUBLE BRX = -1.0, AIL_DOUBLE BRY = -1.0)
         : m_className(className), m_TLX(TLX), m_TLY(TLY), m_TRX(TRX), m_TRY(TRY), m_BLX(BLX), m_BLY(BLY), m_BRX(BRX), m_BRY(BRY)
         {}

      AIL_STRING getClass() const { return m_className; }
      AIL_DOUBLE getTLX() const { return m_TLX; }
      AIL_DOUBLE getTLY() const { return m_TLY; }
      AIL_DOUBLE getTRX() const { return m_TRX; }
      AIL_DOUBLE getTRY() const { return m_TRY; }
      AIL_DOUBLE getBLX() const { return m_BLX; }
      AIL_DOUBLE getBLY() const { return m_BLY; }
      AIL_DOUBLE getBRX() const { return m_BRX; }
      AIL_DOUBLE getBRY() const { return m_BRY; }
   };

//// ENTRY DEFINITION ////
class Entry
   {
   friend class Dataset;

   private:
      AIL_INT m_W = 0; // Width of the image. These are not used at the moment.
      AIL_INT m_H = 0; // Height of the image.
      AIL_INT m_I = -1;

      AIL_STRING m_absPath;
      AIL_STRING m_localPath;

      unordered_map<AIL_INT, BoundingBox> m_boundingBoxes;
      unordered_map<AIL_INT, AIL_INT> m_textBoxLink; //This is used to link the text graph to the bounding box graph

      AIL_INT m_selectedBoundingBox = -1;

   public:
      Entry() {};
      Entry(const AIL_STRING& absPath, AIL_INT imgWidth, AIL_INT imgHeight, AIL_INT entryIndex)
         : m_W(imgWidth), m_H(imgHeight), m_I(entryIndex), m_localPath(absPath), m_selectedBoundingBox(-1){}

      // Method to return iterators for the beginning and end of the map
      std::pair<std::unordered_map<AIL_INT, BoundingBox>::iterator, std::unordered_map<AIL_INT, BoundingBox>::iterator> getBoundingBoxIterators();
      void updateSelectedBoundingBoxLabel(AIL_INT label = -1);
      void addBoundingBox(AIL_INT label, const BoundingBox& bb);
      void addBoundingBox(AIL_INT label, const AIL_STRING& className, AIL_DOUBLE TLX, AIL_DOUBLE TLY, AIL_DOUBLE TRX, AIL_DOUBLE TRY, AIL_DOUBLE BLX, AIL_DOUBLE BLY, AIL_DOUBLE BRX, AIL_DOUBLE BRY);
      void addTextBoxLink(AIL_INT boxLabel, AIL_INT textLabel);
      void assignNewBoundingBoxesMap(unordered_map<AIL_INT, BoundingBox> newMap);

   };

pair<unordered_map<AIL_INT, BoundingBox>::iterator, unordered_map<AIL_INT, BoundingBox>::iterator> Entry::getBoundingBoxIterators()
   {
   return make_pair(m_boundingBoxes.begin(), m_boundingBoxes.end());
   }

void Entry::updateSelectedBoundingBoxLabel(AIL_INT label)
   {
   if(label == -1 || label == 0)
      {
      std::wcout << L"No bounding box selected.\n";
      }
   m_selectedBoundingBox = label;
   }

void Entry::addBoundingBox(AIL_INT label, const BoundingBox& bb)
   {
   m_boundingBoxes.emplace(label, bb);
   updateSelectedBoundingBoxLabel(label);
   }

void Entry::addBoundingBox(AIL_INT label, const AIL_STRING& className, AIL_DOUBLE TLX, AIL_DOUBLE TLY, AIL_DOUBLE TRX, AIL_DOUBLE TRY, AIL_DOUBLE BLX, AIL_DOUBLE BLY, AIL_DOUBLE BRX, AIL_DOUBLE BRY)
   {
   BoundingBox bb(className, TLX, TLY, TRX, TRY, BLX, BLY,  BRX, BRY);
   m_boundingBoxes.emplace(label, bb);
   updateSelectedBoundingBoxLabel();
   }

void Entry::addTextBoxLink(AIL_INT boxLabel, AIL_INT textLabel)
   {
   m_textBoxLink.emplace(boxLabel, textLabel);
   }

void Entry::assignNewBoundingBoxesMap(unordered_map<AIL_INT, BoundingBox> newMap)
   {
   m_boundingBoxes = newMap;
   }

//// DATASET DEFINITION ////
class Dataset
   {
   private:
      AIL_INT m_curEntryIndex = -1;

   public:
      AIL_STRING m_RootDir;
      vector<Entry> m_Entries;

      Dataset() {};
      Dataset(const AIL_STRING& RootDir): m_RootDir(RootDir) {}

      void setRootDir(const AIL_STRING& RootDir);

      void addEntry(const Entry& entry);
      void addEntry(const AIL_STRING& filePath, AIL_INT imgWidth, AIL_INT imgHeight);
      void clearEntry();

      void selectEntry(AIL_INT index);
      void selectEntry(const AIL_STRING& imgPath);

      AIL_BOOL goFirstEntry();
      AIL_BOOL goNextEntry();
      AIL_BOOL goPreviousEntry();

      AIL_BOOL entryExists(const AIL_STRING& imgPath) const;
      AIL_INT getNumberOfEntries() const;
      Entry& getEntry(AIL_INT index = -1);
      Entry getEntry(const AIL_STRING& imgPath);
      AIL_INT getEntryWidth(AIL_INT entryIndex = -1) const;
      AIL_INT getEntryHeight(AIL_INT entryIndex = -1) const;

      void setEntryWidth(AIL_INT width, AIL_INT entryIndex = -1);
      void setEntryHeight(AIL_INT height, AIL_INT entryIndex = -1);

      void addBoundingBox(AIL_INT label, const BoundingBox& BoundingBox, AIL_INT entryIndex = -1);
      void addBoundingBox(AIL_INT textLabel, AIL_INT boxLabel, const BoundingBox& BoundingBox, AIL_INT entryIndex = -1);
      void removeBoundingBox(AIL_INT label = -1);
      BoundingBox getBoundingBox(AIL_INT label = -1);
      void clearBoundingBoxes();
      AIL_BOOL textLabelExists(AIL_INT boxLabel) const;
      AIL_INT getTextLabel(AIL_INT boxLabel);
      void modifyTextLabel(AIL_INT boxLabel, AIL_INT textLabel);
      void clearTextBoxLink();

      AIL_INT getNumberOfBoundingBoxes(AIL_INT index = -1) const;
      void selectBoundingBox(AIL_INT label);

      AIL_INT getSelectedBoundingBoxLabel() const;

      void updateBoundingBox(AIL_INT flag, AIL_DOUBLE value, AIL_INT label = -1);
      void updateBoundingBoxClass(const AIL_STRING& className, AIL_INT label = -1);

      void getObjectCoords(AIL_DOUBLE* TLX, AIL_DOUBLE* TLY, AIL_DOUBLE* TRX, AIL_DOUBLE* TRY, AIL_DOUBLE* BLX, AIL_DOUBLE* BLY, AIL_DOUBLE* BRX, AIL_DOUBLE* BRY, AIL_INT label = -1);

      //void exportAILdataset(AIL_STRING DestPath);
      void generateJSON(const std::wstring& destDir, AIL_BOOL isTemp = false) const;
      void importJSON(const std::wstring& srcFilePath);
   };

// Function to normalize backslashes in a wstring
std::wstring normalizeBackslashes(const std::wstring& input)
   {
   std::wstring result;
   bool lastWasBackslash = false;

   for(wchar_t ch : input)
      {
      if(ch == L'\\')
         {
         if(!lastWasBackslash)
            {
            result += ch;
            }
         lastWasBackslash = true;
         }
      else
         {
         result += ch;
         lastWasBackslash = false;
         }
      }

   return result;
   }

void Dataset::setRootDir(const AIL_STRING& RootDir)
   {
   m_RootDir = m_RootDir;
   }

void Dataset::addEntry(const Entry& entry)
   {
   m_Entries.push_back(entry);
   }

void Dataset::addEntry(const AIL_STRING& filePath, AIL_INT imgWidth, AIL_INT imgHeight)
   {
   AIL_INT nbEntries = m_Entries.size();
   Entry e(filePath, imgWidth, imgHeight, nbEntries);
   m_Entries.push_back(e);
   }

void Dataset::clearEntry()
   {
   m_Entries[m_curEntryIndex].m_boundingBoxes.clear();
   m_Entries[m_curEntryIndex].updateSelectedBoundingBoxLabel();
   }

void Dataset::selectEntry(AIL_INT index)
   {
   m_curEntryIndex = index;
   }

void Dataset::selectEntry(const AIL_STRING& imgPath)
   {
   Entry entry;
   for(int idx = 0; idx < m_Entries.size(); idx++)
      {
      if(normalizeBackslashes(m_Entries[idx].m_localPath).compare(normalizeBackslashes(imgPath)) == 0)
         {
         m_curEntryIndex = idx;
         return;
         }
      }
   wcout << "Warning:: Entry \"" << imgPath << "\" not found! \n";
   return;
   }

AIL_BOOL Dataset::goFirstEntry()
   {
   m_curEntryIndex = 0;
   if(getNumberOfEntries() == 0)
      {
      m_curEntryIndex--;
      return false;
      }
   return true;
   }

AIL_BOOL Dataset::goNextEntry()
   {
   m_curEntryIndex++;
   if(m_curEntryIndex == getNumberOfEntries())
      {
      m_curEntryIndex--;
      return false;
      }
   return true;
   }

AIL_BOOL Dataset::goPreviousEntry()
   {
   m_curEntryIndex--;
   if(m_curEntryIndex < 0)
      {
      m_curEntryIndex++;
      return false;
      }
   return true;
   }

AIL_BOOL Dataset::entryExists(const AIL_STRING& imgPath) const
   {
   for(auto& e : m_Entries)
      {
      if(normalizeBackslashes(e.m_localPath).compare(normalizeBackslashes(imgPath)) == 0)
         {
         return true;
         }
      }
   return false;
   }

AIL_INT Dataset::getNumberOfEntries() const
   {
   return m_Entries.size();
   }

Entry& Dataset::getEntry(AIL_INT index)
   {
   if(index == -1)
      {
      index = m_curEntryIndex;
      }

   if(index >= 0 && index < static_cast<AIL_INT>(m_Entries.size()))
      {
      return m_Entries[index];
      }
   throw std::out_of_range("Entry index out of range.");
   }

Entry Dataset::getEntry(const AIL_STRING& imgPath)
   {
   for(auto& e : m_Entries)
      {
      if(normalizeBackslashes(e.m_localPath).compare(normalizeBackslashes(imgPath)) == 0)
         {
         return e;
         }
      }
   wcout << "Warning:: Entry \"" << imgPath << "\" not found! \n";
   return Entry();
   }

AIL_INT Dataset::getEntryWidth(AIL_INT entryIndex) const
   {
   if(entryIndex == -1)
      entryIndex = m_curEntryIndex;

   return m_Entries[entryIndex].m_W;
   }

AIL_INT Dataset::getEntryHeight(AIL_INT entryIndex) const
   {
   if(entryIndex == -1)
      entryIndex = m_curEntryIndex;

   return m_Entries[entryIndex].m_H;
   }

void Dataset::setEntryWidth(AIL_INT width, AIL_INT entryIndex)
   {
   if(entryIndex == -1)
      entryIndex = m_curEntryIndex;

   m_Entries[entryIndex].m_W = width;
   return;
   }

void Dataset::setEntryHeight(AIL_INT height, AIL_INT entryIndex)
   {
   if(entryIndex == -1)
      entryIndex = m_curEntryIndex;

   m_Entries[entryIndex].m_H = height;
   return;
   }

void Dataset::addBoundingBox(AIL_INT label, const BoundingBox& BoundingBox, AIL_INT entryIndex)
   {
   if(entryIndex == -1)
      entryIndex = m_curEntryIndex   ;

   m_Entries[entryIndex].addBoundingBox(label, BoundingBox);
   }

void Dataset::addBoundingBox(AIL_INT TextLabel, AIL_INT BoxLabel, const BoundingBox& BoundingBox, AIL_INT entryIndex)
   {
   if(entryIndex == -1)
      entryIndex = m_curEntryIndex;

   m_Entries[entryIndex].addBoundingBox(BoxLabel, BoundingBox);
   m_Entries[entryIndex].addTextBoxLink(BoxLabel, TextLabel);
   }

BoundingBox Dataset::getBoundingBox(AIL_INT label)
   {
   if(label == -1)
      label = m_Entries[m_curEntryIndex].m_selectedBoundingBox;
   if(m_Entries[m_curEntryIndex].m_boundingBoxes.find(label) != m_Entries[m_curEntryIndex].m_boundingBoxes.end())
      {
      return m_Entries[m_curEntryIndex].m_boundingBoxes[label];
      }
   else
      {
      // Handle the case where the key is not found
      std::cout << "Label " << label << " does not exist in the boxes map." << std::endl;
      return BoundingBox();
      }
   }

void Dataset::removeBoundingBox(AIL_INT label)
   {
   if(getNumberOfBoundingBoxes() > 0)
      {
      if(label == -1)
         label = m_Entries[m_curEntryIndex   ].m_selectedBoundingBox;
	   m_Entries[m_curEntryIndex   ].m_boundingBoxes.erase(label);
	   m_Entries[m_curEntryIndex   ].m_textBoxLink.erase(label);
      m_Entries[m_curEntryIndex   ].m_selectedBoundingBox =  -1;
      }
   }

void Dataset::clearBoundingBoxes()
   {
   m_Entries[m_curEntryIndex].m_boundingBoxes.clear();
   }

AIL_BOOL Dataset::textLabelExists(AIL_INT boxLabel) const
   {
   if(boxLabel == -1)
      boxLabel = m_Entries[m_curEntryIndex].m_selectedBoundingBox;
   if(m_Entries[m_curEntryIndex].m_textBoxLink.find(boxLabel) != m_Entries[m_curEntryIndex].m_textBoxLink.end())
      {
      return true;
      }
   else
      {
      return false;
      }
   }

AIL_INT Dataset::getTextLabel(AIL_INT boxLabel)
   {
   if(boxLabel == -1)
      boxLabel = m_Entries[m_curEntryIndex].m_selectedBoundingBox;
   if(m_Entries[m_curEntryIndex].m_textBoxLink.find(boxLabel) != m_Entries[m_curEntryIndex].m_textBoxLink.end())
      {
      return m_Entries[m_curEntryIndex].m_textBoxLink[boxLabel];
      }
   else
      {
      // Handle the case where the key is not found
      std::cout << "Label " << boxLabel << " does not exist in the text-box map." << std::endl;
      return -1;
      }
   }

void Dataset::modifyTextLabel(AIL_INT boxLabel, AIL_INT textLabel)
   {
   m_Entries[m_curEntryIndex].m_textBoxLink[boxLabel] = textLabel;
   }

void Dataset::clearTextBoxLink()
   {
   m_Entries[m_curEntryIndex].m_textBoxLink.clear();
   }

AIL_INT Dataset::getNumberOfBoundingBoxes(AIL_INT index) const
   {
   if(index == -1)
      index = m_curEntryIndex;

   return m_Entries[index].m_boundingBoxes.size();
   }

void Dataset::selectBoundingBox(AIL_INT label)
{
	m_Entries[m_curEntryIndex   ].m_selectedBoundingBox = label;
}

AIL_INT Dataset::getSelectedBoundingBoxLabel() const
   {
   if(m_Entries[m_curEntryIndex].m_selectedBoundingBox == 0)
      {
      std::cout << " No selected bounding box. Selected bounding box label is 0. ";
      }
   else if(m_Entries[m_curEntryIndex].m_selectedBoundingBox == -1)
      {
      std::cout << " No selected bounding box. Selected bounding box label is -1. ";
      }
   return m_Entries[m_curEntryIndex].m_selectedBoundingBox;
   }

void Dataset::updateBoundingBoxClass(const AIL_STRING& className, AIL_INT label)
   {
   if(label == -1)
      label = m_Entries[m_curEntryIndex].m_selectedBoundingBox;

   if(m_Entries[m_curEntryIndex].m_boundingBoxes.find(label) != m_Entries[m_curEntryIndex].m_boundingBoxes.end())
      {
      m_Entries[m_curEntryIndex].m_boundingBoxes[label].m_className = className;
      }
   else
      {
      // Handle the case where the key is not found
      std::cout << "Bounding box label " << label << " does not exist in the map." << std::endl;
      }
   }

void Dataset::updateBoundingBox(AIL_INT flag, AIL_DOUBLE value, AIL_INT label)
   {
   if(label == -1)
      label = m_Entries[m_curEntryIndex].m_selectedBoundingBox;

   if(m_Entries[m_curEntryIndex].m_boundingBoxes.find(label) != m_Entries[m_curEntryIndex].m_boundingBoxes.end())
      {
      switch(flag)
         {
         case 0:
            break;
         case 1:
            m_Entries[m_curEntryIndex].m_boundingBoxes[label].m_TLX = value;
            break;
         case 2:
            m_Entries[m_curEntryIndex].m_boundingBoxes[label].m_TLY = value;
            break;
         case 3:
            m_Entries[m_curEntryIndex].m_boundingBoxes[label].m_TRX = value;
            break;
         case 4:
            m_Entries[m_curEntryIndex].m_boundingBoxes[label].m_TRY = value;
            break;
         case 5:
            m_Entries[m_curEntryIndex].m_boundingBoxes[label].m_BLX = value;
            break;
         case 6:
            m_Entries[m_curEntryIndex].m_boundingBoxes[label].m_BLY = value;
            break;
         case 7:
            m_Entries[m_curEntryIndex].m_boundingBoxes[label].m_BRX = value;
            break;
         case 8:
            m_Entries[m_curEntryIndex].m_boundingBoxes[label].m_BRY = value;
            break;
         }
      }
   }

void Dataset::getObjectCoords(AIL_DOUBLE* TLX, AIL_DOUBLE* TLY, AIL_DOUBLE* TRX, AIL_DOUBLE* TRY, AIL_DOUBLE* BLX, AIL_DOUBLE* BLY, AIL_DOUBLE* BRX, AIL_DOUBLE* BRY, AIL_INT label)
   {
   if(label == -1)
      label = m_Entries[m_curEntryIndex   ].m_selectedBoundingBox;

   if(m_Entries[m_curEntryIndex   ].m_boundingBoxes.find(label) != m_Entries[m_curEntryIndex   ].m_boundingBoxes.end())
      {
      *TLX = m_Entries[m_curEntryIndex   ].m_boundingBoxes[label].getTLX();
      *TLY = m_Entries[m_curEntryIndex   ].m_boundingBoxes[label].getTLY();
      *TRX = m_Entries[m_curEntryIndex   ].m_boundingBoxes[label].getTRX();
      *TRY = m_Entries[m_curEntryIndex   ].m_boundingBoxes[label].getTRY();
      *BLX = m_Entries[m_curEntryIndex   ].m_boundingBoxes[label].getBLX();
      *BLY = m_Entries[m_curEntryIndex   ].m_boundingBoxes[label].getBLY();
      *BRX = m_Entries[m_curEntryIndex   ].m_boundingBoxes[label].getBRX();
      *BRY = m_Entries[m_curEntryIndex   ].m_boundingBoxes[label].getBRY();
      }
   else
      {
      // Handle the case where the key is not found
      std::cout << "Label " << label << " does not exist in the map." << std::endl;
      }
   }

// TODO: complete the implementation for this if needed
//void Dataset::exportAILdataset(AIL_STRING DestPath)
//   {
//   MdlocrExport(DestPath, M_DEFAULT, AilDataset, M_DEFAULT);
//   }

// Function to normalize backslashes in a wstring so that all sequences are doubled
std::wstring normalizeDoubleBackslashes(const std::wstring& input)
   {
   std::wstring result;
   size_t length = input.length();
   size_t i = 0;

   while(i < length)
      {
      if(input[i] == L'\\')
         {
         // Add a double backslash to the result
         result += L"\\";
         result += L"\\";

         // Skip over any additional backslashes
         while(i < length && input[i] == L'\\')
            {
            ++i;
            }
         }
      else
         {
         // Add non-backslash characters to the result
         result += input[i];
         ++i;
         }
      }

   return result;
   }

void Dataset::generateJSON(const wstring& destDir, AIL_BOOL isTemp) const
   {
   wstring json = L"{\n    \"entries\": [\n";

   for(size_t i = 0; i < m_Entries.size(); ++i)
      {
      json += L"        {\n";
      json += L"            \"image_name\": \"" + normalizeDoubleBackslashes(m_Entries[i].m_localPath) + L"\",\n";
      json += L"            \"annotations\": [\n";

      for(auto it = m_Entries[i].m_boundingBoxes.begin(); it != m_Entries[i].m_boundingBoxes.end(); ++it)
         {
         const auto& character = it->second.getClass();
         const auto x1 = it->second.getTLX();
         const auto y1 = it->second.getTLY();
         const auto x2 = it->second.getBLX();
         const auto y2 = it->second.getBLY();
         const auto x3 = it->second.getBRX();
         const auto y3 = it->second.getBRY();
         const auto x4 = it->second.getTRX();
         const auto y4 = it->second.getTRY();
         
         wstring polygon = L"POLYGON((" +
            std::to_wstring(x1) + L" " + std::to_wstring(y1) + L"," +
            std::to_wstring(x2) + L" " + std::to_wstring(y2) + L"," +
            std::to_wstring(x3) + L" " + std::to_wstring(y3) + L"," +
            std::to_wstring(x4) + L" " + std::to_wstring(y4) + L"," +
            std::to_wstring(x1) + L" " + std::to_wstring(y1) + L"))";

         json += L"                {\n";
         json += L"                    \"character\": \"" + character + L"\",\n";
         json += L"                    \"bbox\": \"" + polygon + L"\"\n";
         json += L"                }";
         if(it != std::prev(m_Entries[i].m_boundingBoxes.end()))
            {
            json += L",";
            }
         json += L"\n";
         }

      json += L"            ]\n";
      json += L"        }";
      if(i < m_Entries.size() - 1)
         {
         json += L",";
         }
      json += L"\n";
      }

   json += L"    ]\n}";

   // Write the JSON data to a file
   wstring destFilePath;
   if(isTemp)
      destFilePath = destDir + L"\\temp_annotations.json";
   else
      destFilePath = destDir + L"\\annotations.json";

   wfstream file(destFilePath, std::ios::out);
   if(file.is_open())
      {
      file << json;
      file.close();
      std::cout << "JSON file generated successfully." << std::endl;
      }
   else
      {
      std::cerr << "Error opening file for writing." << std::endl;
      }
   }

void Dataset::importJSON(const std::wstring& srcFilePath)
   {
   std::wfstream file(srcFilePath, std::ios::in);
   if(!file.is_open())
      {
      std::wcerr << L"Error opening file for reading." << std::endl;
      return;
      }

   std::wstring line, json_content;
   while(std::getline(file, line))
      {
      json_content += line;
      }
   file.close();

   // A very basic and manual JSON parsing (assuming specific format)
   size_t pos = 0;
   while((pos = json_content.find(L"\"image_name\": \"", pos)) != std::wstring::npos)
      {
      Entry entry;
      pos += 15; // Move past "image_name": "
      size_t end_pos = json_content.find(L"\"", pos);
      entry.m_localPath = json_content.substr(pos, end_pos - pos);

      //size_t pos_name = entry.m_absPath.find_last_of('\\');
      //entry.m_localPath = entry.m_absPath.substr(pos_name + 1);
      //entry.m_absPath = srcFilePath.substr(0, srcFilePath.find_last_of('\\') + 1) + entry.m_absPath;

      // Find annotations
      pos = json_content.find(L"\"annotations\": [", end_pos);
      if(pos == std::wstring::npos) break;
      pos += 16;

      // Check if annotations are empty
      size_t annotations_end_pos = json_content.find(L"]", pos);
      if(annotations_end_pos == pos + 12)
         {
         addEntry(entry);
         continue; // Skip to the next image entry if annotations are empty
         }

      int bboxIndex = 1;
      while((pos = json_content.find(L"\"character\": \"", pos)) != std::wstring::npos)
         {
         BoundingBox bbox;
         pos += 14; // Move past "character": "
         end_pos = json_content.find(L"\"", pos);
         bbox.m_className = json_content.substr(pos, end_pos - pos);

         // Find bbox polygon data
         pos = json_content.find(L"\"bbox\": \"POLYGON((", end_pos);
         if(pos == std::wstring::npos) break;
         pos += 18;

         // Extract the coordinates
         int scanned = swscanf_s(json_content.c_str() + pos, L"%lf %lf,%lf %lf,%lf %lf,%lf %lf",
                 &bbox.m_TLX, &bbox.m_TLY, &bbox.m_BLX, &bbox.m_BLY,
                 &bbox.m_BRX, &bbox.m_BRY, &bbox.m_TRX, &bbox.m_TRY);

         entry.addBoundingBox(bboxIndex++, bbox); // Temporarily use incremental index as labels. Correct labels will be reassigned at drawing time.

         size_t next_entry_pos = json_content.find(L"\"image_name\": \"", pos);
         size_t next_anno_pos = json_content.find(L"},                {", pos);
         
         if(next_entry_pos < next_anno_pos) break;
         pos = next_anno_pos;
         }

      addEntry(entry);
      }

   std::wcout << L"JSON file imported successfully." << std::endl;
   }