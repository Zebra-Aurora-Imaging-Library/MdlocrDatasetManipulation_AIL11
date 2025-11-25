#include "ImagesProcessing.h"
#include "OcrTools.h"
#include "KeyboardToClassMapping.h"
#include "IO.h"

AIL_BOOL PAUSE = true;
AIL_BOOL FINISH = false;
AIL_BOOL SAME_IMAGE = true;

AIL_DOUBLE TLX, TLY, TRX, TRY, BLX, BLY, BRX, BRY;

struct HookData
   {
   Dataset *Dataset;
   FileManager *FManager;

   AIL_ID AilSystem,
          AilDisplay,
          AilImage,
          AilGraphicsList,
          AilGraphicsContext,
          AilDlocrContext,
          AilDlocrResult;

   AIL_INT DefaultBBWidth = 50,
           DefaultBBHeight = 50,
           ArrayIndex;

   AIL_DOUBLE TLX, TLY,
              TRX, TRY,
              BLX, BLY,
              BRX, BRY;

   AIL_STRING DestDir;
   };

void PrintInstruction()
   {
   MosPrintf(AIL_TEXT("\n KEYMAP:\n=============================\n"));
   MosPrintf(AIL_TEXT(" Next image (generate temp annotation file)   :: Right arrow (->)\n"));
   MosPrintf(AIL_TEXT(" Previous image                               :: Left arrow (<-)\n"));
   MosPrintf(AIL_TEXT(" Add a bounding box                           :: Right Click\n"));
   MosPrintf(AIL_TEXT(" Use keyboard to select class                 :: Letters, Numbers and Special Characters\n"));
   MosPrintf(AIL_TEXT(" Delete the selected bounding box             :: Delete key\n"));
   MosPrintf(AIL_TEXT(" Delete all the bounding boxes in the image   :: CTL + Delete key\n"));
   MosPrintf(AIL_TEXT(" Use DL OCR module to generate bounding boxes :: CTL + P\n"));
   MosPrintf(AIL_TEXT(" Set the current size bounding box as default :: CTL + D\n"));
   MosPrintf(AIL_TEXT(" Save labeling                                :: CTL + S\n"));
   MosPrintf(AIL_TEXT(" End with saving                              :: Esc\n\n"));
   MosPrintf(AIL_TEXT(" Press <enter> to start.\n"));
   MosGetch();
   }

void HandleClassSelection(AIL_INT KeyValue, AIL_INT CombinationValue, HookData* HookDataPtr);

// Changes selected bounding boxe.
AIL_INT MFTYPE GraphicSelectionHookHandler(AIL_INT HookType, AIL_ID EventId, void *UserDataPtr)
   {
   AIL_INT BbLabel;
   HookData* HookDataPtr = (HookData*)UserDataPtr;
   MgraGetHookInfo(EventId, M_GRAPHIC_LABEL_VALUE, &BbLabel);
   
   //if Bounding Box selected
   if(BbLabel != M_NO_LABEL)
      HookDataPtr->Dataset->selectBoundingBox(BbLabel);
   return 0;
   }

// Moves bounding box and text when selected.
AIL_INT MFTYPE GraphicModificationHookHandler(AIL_INT HookType, AIL_ID EventId, void *UserDataPtr)
   {
   AIL_INT ControlType, BbLabel, GraphType;
   HookData* HookDataPtr = (HookData*)UserDataPtr;
   MgraGetHookInfo(EventId, M_GRAPHIC_CONTROL_TYPE, &ControlType);
   
   if(ControlType != M_DELETE)
      {
      MgraGetHookInfo(EventId, M_GRAPHIC_LABEL_VALUE, &BbLabel);
      MgraInquireList(HookDataPtr->AilGraphicsList, M_GRAPHIC_LABEL(BbLabel), M_DEFAULT, M_GRAPHIC_TYPE, &GraphType);
      
      if(BbLabel != -1 && GraphType != M_GRAPHIC_TYPE_TEXT)
         {
         MgraInquireList(HookDataPtr->AilGraphicsList, M_GRAPHIC_LABEL(BbLabel), M_DEFAULT, M_CORNER_TOP_LEFT_X, &HookDataPtr->TLX);
         MgraInquireList(HookDataPtr->AilGraphicsList, M_GRAPHIC_LABEL(BbLabel), M_DEFAULT, M_CORNER_TOP_LEFT_Y, &HookDataPtr->TLY);
         MgraInquireList(HookDataPtr->AilGraphicsList, M_GRAPHIC_LABEL(BbLabel), M_DEFAULT, M_CORNER_TOP_RIGHT_X, &HookDataPtr->TRX);
         MgraInquireList(HookDataPtr->AilGraphicsList, M_GRAPHIC_LABEL(BbLabel), M_DEFAULT, M_CORNER_TOP_RIGHT_Y, &HookDataPtr->TRY);
         MgraInquireList(HookDataPtr->AilGraphicsList, M_GRAPHIC_LABEL(BbLabel), M_DEFAULT, M_CORNER_BOTTOM_LEFT_X, &HookDataPtr->BLX);
         MgraInquireList(HookDataPtr->AilGraphicsList, M_GRAPHIC_LABEL(BbLabel), M_DEFAULT, M_CORNER_BOTTOM_LEFT_Y, &HookDataPtr->BLY);
         MgraInquireList(HookDataPtr->AilGraphicsList, M_GRAPHIC_LABEL(BbLabel), M_DEFAULT, M_CORNER_BOTTOM_RIGHT_X, &HookDataPtr->BRX);
         MgraInquireList(HookDataPtr->AilGraphicsList, M_GRAPHIC_LABEL(BbLabel), M_DEFAULT, M_CORNER_BOTTOM_RIGHT_Y, &HookDataPtr->BRY);
         
         if(HookDataPtr->Dataset->textLabelExists(BbLabel))
            {
            MgraControlList(HookDataPtr->AilGraphicsList, M_GRAPHIC_LABEL(HookDataPtr->Dataset->getTextLabel(BbLabel)), M_DEFAULT, M_POSITION_X, HookDataPtr->TLX);
            MgraControlList(HookDataPtr->AilGraphicsList, M_GRAPHIC_LABEL(HookDataPtr->Dataset->getTextLabel(BbLabel)), M_DEFAULT, M_POSITION_Y, HookDataPtr->BLY);
            }
         
         HookDataPtr->Dataset->updateBoundingBox(1, HookDataPtr->TLX, BbLabel);
         HookDataPtr->Dataset->updateBoundingBox(2, HookDataPtr->TLY, BbLabel);
         HookDataPtr->Dataset->updateBoundingBox(3, HookDataPtr->TRX, BbLabel);
         HookDataPtr->Dataset->updateBoundingBox(4, HookDataPtr->TRY, BbLabel);
         HookDataPtr->Dataset->updateBoundingBox(5, HookDataPtr->BLX, BbLabel);
         HookDataPtr->Dataset->updateBoundingBox(6, HookDataPtr->BLY, BbLabel);
         HookDataPtr->Dataset->updateBoundingBox(7, HookDataPtr->BRX, BbLabel);
         HookDataPtr->Dataset->updateBoundingBox(8, HookDataPtr->BRY, BbLabel);
         }
      }
   return 0;
   }

// Adds a new bounding box
AIL_INT MFTYPE RightClick(AIL_INT HookType, AIL_ID EventId, void* UserDataPtr)
   {
   if(EventId != M_NULL)
      {
      AIL_INT MousePosX, MousePosY, BbLabel, TxtLabel;
      MdispGetHookInfo(EventId,M_MOUSE_POSITION_X, &MousePosX);
      MdispGetHookInfo(EventId,M_MOUSE_POSITION_Y, &MousePosY);
      HookData* HookDataPtr = (HookData*)UserDataPtr;
      
      // Update entry with new BoundingBox
      TLX = AIL_DOUBLE (MousePosX - (HookDataPtr->DefaultBBWidth / 2));
      TLY = AIL_DOUBLE (MousePosY - (HookDataPtr->DefaultBBHeight / 2));
      TRX = AIL_DOUBLE (MousePosX + (HookDataPtr->DefaultBBWidth / 2));
      TRY = AIL_DOUBLE (MousePosY - (HookDataPtr->DefaultBBHeight / 2));
      BLX = AIL_DOUBLE (MousePosX - (HookDataPtr->DefaultBBWidth / 2));
      BLY = AIL_DOUBLE (MousePosY + (HookDataPtr->DefaultBBHeight / 2));
      BRX = AIL_DOUBLE (MousePosX + (HookDataPtr->DefaultBBWidth / 2));
      BRY = AIL_DOUBLE (MousePosY + (HookDataPtr->DefaultBBHeight / 2));
      BoundingBox Bb = BoundingBox(L" ", TLX, TLY, TRX, TRY, BLX, BLY, BRX, BRY);
      
      // Draw new bounding box
      MgraRect(HookDataPtr->AilGraphicsContext, HookDataPtr->AilGraphicsList, TLX, TLY, BRX, BRY);
      MgraInquireList(HookDataPtr->AilGraphicsList, M_LIST, M_DEFAULT, M_LAST_LABEL, &BbLabel);
      MgraControlList(HookDataPtr->AilGraphicsList, M_GRAPHIC_LABEL(BbLabel), M_DEFAULT, M_ROTATABLE, M_DISABLE);
      MgraControlList(HookDataPtr->AilGraphicsList, M_GRAPHIC_LABEL(BbLabel), M_DEFAULT, M_COLOR, M_COLOR_RED);
      
      MgraText(HookDataPtr->AilGraphicsContext, HookDataPtr->AilGraphicsList, BLX, BLY, L" ");
      MgraInquireList(HookDataPtr->AilGraphicsList, M_LIST, M_DEFAULT, M_LAST_LABEL, &TxtLabel);
      MgraControlList(HookDataPtr->AilGraphicsList, M_GRAPHIC_LABEL(TxtLabel), M_DEFAULT, M_SELECTABLE, M_DISABLE);
      MgraControlList(HookDataPtr->AilGraphicsList, M_GRAPHIC_LABEL(TxtLabel), M_DEFAULT, M_BACKGROUND_MODE, M_TRANSPARENT);
      
      HookDataPtr->Dataset->addBoundingBox(TxtLabel, BbLabel, Bb);
      }
   return 0;
   }

AIL_INT MFTYPE KeyPressed(AIL_INT HookType, AIL_ID EventId, void* UserDataPtr)
   {
   if(EventId != M_NULL)
      {
      AIL_INT KeyValue, CombinationValue;
      MdispGetHookInfo(EventId, M_KEY_VALUE, &KeyValue);
      MdispGetHookInfo(EventId, M_COMBINATION_KEYS, &CombinationValue);
      
      HookData* HookDataPtr = (HookData*)UserDataPtr;
      
      // Action depending on the pressed key
      // Change the image. 
      if(KeyValue == M_KEY_ARROW_LEFT)
         {
         HookDataPtr->ArrayIndex--;
         HookDataPtr->Dataset->goPreviousEntry();
         PAUSE = false;
         SAME_IMAGE = false;
         }
      // Change and save temporary file
      else if(KeyValue == M_KEY_ARROW_RIGHT || KeyValue == M_KEY_SPACE)
         {
         HookDataPtr->ArrayIndex++;
         HookDataPtr->Dataset->goNextEntry();
         PAUSE = false;
         SAME_IMAGE = false;
         HookDataPtr->Dataset->generateJSON(HookDataPtr->DestDir, true);
         }
      // Change default bounding box size
      else if(KeyValue == M_KEY_D && CombinationValue == M_KEY_CTRL)
         {
         HookDataPtr->DefaultBBWidth = static_cast<AIL_INT>(HookDataPtr->Dataset->getBoundingBox().getBRX() - HookDataPtr->Dataset->getBoundingBox().getBLX());
         HookDataPtr->DefaultBBHeight = static_cast<AIL_INT>(HookDataPtr->Dataset->getBoundingBox().getBLY() - HookDataPtr->Dataset->getBoundingBox().getTLY());
         }
      // Save annotations
      else if(KeyValue == M_KEY_S && CombinationValue == M_KEY_CTRL)
         {
         HookDataPtr->Dataset->generateJSON(HookDataPtr->DestDir);
         }
      // Use DLOCR to generate bounding boxes
      else if(KeyValue == M_KEY_P && CombinationValue == M_KEY_CTRL)
         {
         MdlocrRead(HookDataPtr->AilDlocrContext, HookDataPtr->AilImage, HookDataPtr->AilDlocrResult, M_READ_ALL);
         AIL_INT ReadStatus, NumberOfStringRead, StringSize;
         MdlocrGetResult(HookDataPtr->AilDlocrResult, M_GENERAL, M_DEFAULT, M_STATUS, &ReadStatus);
         if(ReadStatus == M_COMPLETE)
            {
            AIL_STRING CharName = L" ";
            MdlocrGetResult(HookDataPtr->AilDlocrResult, M_GENERAL, M_GENERAL, M_STRING_NUMBER + M_TYPE_AIL_INT,
                            &NumberOfStringRead);
            AIL_INT BbLabel, TxtLabel;
            for(AIL_INT i = 0; i < NumberOfStringRead; ++i)
               {
               MdlocrGetResult(HookDataPtr->AilDlocrResult, i, M_DEFAULT, M_STRING_CHAR_NUMBER, &StringSize);
               for(AIL_INT j = 0; j < StringSize; ++j)
                  {
                  MdlocrGetResult(HookDataPtr->AilDlocrResult, i, M_INDEX_IN_STRING(j), M_CHAR_BOX_UL_X, &TLX);
                  MdlocrGetResult(HookDataPtr->AilDlocrResult, i, M_INDEX_IN_STRING(j), M_CHAR_BOX_BL_X, &BLX);
                  MdlocrGetResult(HookDataPtr->AilDlocrResult, i, M_INDEX_IN_STRING(j), M_CHAR_BOX_BR_X, &BRX);
                  MdlocrGetResult(HookDataPtr->AilDlocrResult, i, M_INDEX_IN_STRING(j), M_CHAR_BOX_UR_X, &TRX);
                  MdlocrGetResult(HookDataPtr->AilDlocrResult, i, M_INDEX_IN_STRING(j), M_CHAR_BOX_UL_Y, &TLY);
                  MdlocrGetResult(HookDataPtr->AilDlocrResult, i, M_INDEX_IN_STRING(j), M_CHAR_BOX_BL_Y, &BLY);
                  MdlocrGetResult(HookDataPtr->AilDlocrResult, i, M_INDEX_IN_STRING(j), M_CHAR_BOX_BR_Y, &BRY);
                  MdlocrGetResult(HookDataPtr->AilDlocrResult, i, M_INDEX_IN_STRING(j), M_CHAR_BOX_UR_Y, &TRY);
                  MdlocrGetResult(HookDataPtr->AilDlocrResult, i, M_INDEX_IN_STRING(j), M_CHAR_NAME, CharName);
                  BoundingBox Bb = BoundingBox(CharName, TLX, TLY, TRX, TRY, BLX, BLY, BRX, BRY);
                  
                  // Draw Bounding Boxes
                  MgraRect(HookDataPtr->AilGraphicsContext, HookDataPtr->AilGraphicsList, TLX, TLY, BRX, BRY);
                  MgraInquireList(HookDataPtr->AilGraphicsList, M_LIST, M_DEFAULT, M_LAST_LABEL, &BbLabel);
                  MgraControlList(HookDataPtr->AilGraphicsList, M_GRAPHIC_LABEL(BbLabel), M_DEFAULT, M_ROTATABLE, M_DISABLE);
                  MgraControlList(HookDataPtr->AilGraphicsList, M_GRAPHIC_LABEL(BbLabel), M_DEFAULT, M_COLOR, M_COLOR_GREEN);
                  
                  MgraText(HookDataPtr->AilGraphicsContext, HookDataPtr->AilGraphicsList, BLX, BLY, CharName);
                  MgraInquireList(HookDataPtr->AilGraphicsList, M_LIST, M_DEFAULT, M_LAST_LABEL, &TxtLabel);
                  MgraControlList(HookDataPtr->AilGraphicsList, M_GRAPHIC_LABEL(TxtLabel), M_DEFAULT, M_SELECTABLE, M_DISABLE);
                  MgraControlList(HookDataPtr->AilGraphicsList, M_GRAPHIC_LABEL(TxtLabel), M_DEFAULT, M_COLOR, M_COLOR_GREEN);
                  MgraControlList(HookDataPtr->AilGraphicsList, M_GRAPHIC_LABEL(TxtLabel), M_DEFAULT, M_BACKGROUND_MODE, M_TRANSPARENT);
                  
                  HookDataPtr->Dataset->addBoundingBox(TxtLabel, BbLabel, Bb);
                  }
               }
            }
         else
            cout << "Reading error!";
         }
      // Remove selected bounding box
      else if(KeyValue == M_KEY_DELETE && CombinationValue != M_KEY_CTRL)
         {
         AIL_INT BbLabel = HookDataPtr->Dataset->getSelectedBoundingBoxLabel();
         if(BbLabel != 0 && BbLabel != -1 && HookDataPtr->Dataset->textLabelExists(BbLabel))
            {
            MgraControlList(HookDataPtr->AilGraphicsList, M_GRAPHIC_LABEL(HookDataPtr->Dataset->getSelectedBoundingBoxLabel()), 
                            M_DEFAULT, M_DELETE, M_DEFAULT);
            MgraControlList(HookDataPtr->AilGraphicsList, M_GRAPHIC_LABEL(HookDataPtr->Dataset->getTextLabel(BbLabel)),
                            M_DEFAULT, M_DELETE, M_DEFAULT);
            HookDataPtr->Dataset->removeBoundingBox();
            }
         else
            std::cout << "Delete cannot be performed: No bounding box selected.";
         }
      // Remove all bounding boxes from the entry
      else if(KeyValue == M_KEY_DELETE && CombinationValue == M_KEY_CTRL)
         {
         MgraControlList(HookDataPtr->AilGraphicsList, M_ALL, M_DEFAULT, M_DELETE, M_DEFAULT);
         HookDataPtr->Dataset->clearBoundingBoxes();
         HookDataPtr->Dataset->clearTextBoxLink();
         }
      // Save and Exit application 
      else if(KeyValue == M_KEY_ESC)
         {
         PAUSE = false;
         FINISH = true;
         }
      // Define the class
      else
         HandleClassSelection(KeyValue, CombinationValue, HookDataPtr);
      }
   return 0;
   }

void HandleClassSelection(AIL_INT KeyValue, AIL_INT CombinationValue, HookData* HookDataPtr)
{
   AIL_BOOL IsCapsLockOn = GetKeyState(VK_CAPITAL) & 0x0001;
   AIL_BOOL IsShiftOn = CombinationValue == M_KEY_SHIFT;
   unordered_map<AIL_INT, AIL_STRING>::iterator it;
   AIL_BOOL Found = false;
   if(IsCapsLockOn && IsShiftOn)
      {
      it = LC_SC_KEYCLASSMAP.find(KeyValue);
      if(it != LC_SC_KEYCLASSMAP.end())
         Found = true;
      }
   else if(!IsCapsLockOn && IsShiftOn)
      {
      it = UC_SC_KEYCLASSMAP.find(KeyValue);
      if(it != UC_SC_KEYCLASSMAP.end())
         Found = true;
      }
   else if(IsCapsLockOn && !IsShiftOn)
      {
      it = UC_NUM_KEYCLASSMAP.find(KeyValue);
      if(it != UC_NUM_KEYCLASSMAP.end())
         Found = true;
      }
   else
      {
      it = LC_NUM_KEYCLASSMAP.find(KeyValue);
      if(it != LC_NUM_KEYCLASSMAP.end())
         Found = true;
      }
   
   if(Found)
      {
      AIL_INT BbLabel = HookDataPtr->Dataset->getSelectedBoundingBoxLabel();
      if(BbLabel != 0 && BbLabel != -1)
         {
         AIL_STRING CurrentClass = it->second;
         std::wcout << "Class changed to: " << CurrentClass << std::endl;
         
         if(HookDataPtr->Dataset->textLabelExists(BbLabel))
            {
            AIL_INT TxtLabel = HookDataPtr->Dataset->getTextLabel(BbLabel);
            MgraControlList(HookDataPtr->AilGraphicsList, M_GRAPHIC_LABEL(BbLabel), M_DEFAULT, M_COLOR, M_COLOR_GREEN);
            MgraControlList(HookDataPtr->AilGraphicsList, M_GRAPHIC_LABEL(TxtLabel), M_DEFAULT, M_DELETE, M_DEFAULT);
            MgraText(HookDataPtr->AilGraphicsContext, HookDataPtr->AilGraphicsList, 
                     HookDataPtr->Dataset->getBoundingBox().getBLX(), 
                     HookDataPtr->Dataset->getBoundingBox().getBLY(), CurrentClass);
            HookDataPtr->Dataset->updateBoundingBoxClass(CurrentClass);
            MgraInquireList(HookDataPtr->AilGraphicsList, M_LIST, M_DEFAULT, M_LAST_LABEL, &TxtLabel);
            MgraControlList(HookDataPtr->AilGraphicsList, M_GRAPHIC_LABEL(TxtLabel), M_DEFAULT, M_COLOR, M_COLOR_GREEN);
            MgraControlList(HookDataPtr->AilGraphicsList, M_GRAPHIC_LABEL(TxtLabel), M_DEFAULT, M_SELECTABLE, M_DISABLE);
            MgraControlList(HookDataPtr->AilGraphicsList, M_GRAPHIC_LABEL(TxtLabel), M_DEFAULT, M_BACKGROUND_MODE, M_TRANSPARENT);
            
            HookDataPtr->Dataset->modifyTextLabel(BbLabel, TxtLabel);
            }
         }
      else
         std::wcout << L"Class cannot be modified: No bounding box selected.\n";
      }
   else if(CombinationValue != M_KEY_SHIFT && CombinationValue != M_KEY_CTRL)
      std::wcout << L"Pressed key < " << KeyValue << L" > is not valid!\n";
   }

int main(int argc, char** argv)
   {
   // Print program information
   std::wcout << L"OCR Labeler is a tool that facilitates the image labeling process for OCR.\n";
   std::wcout << L"The DL OCR module can be used to facilitate labeling.\n";
   std::wcout << L"The results will be saved in AIL-compatible format.\n\n";
   std::wcout << L"Note that you need to use the US English keyboard to have the correct mapping of characters.\n";
   std::wcout << L"IMPORTANT: Make sure that all bounding boxes are assigned to a class before exporting.\n\n";

   IO parser;
   parser.addElement(L"ImgDir");
   parser.addElement(L"DestDir");
   parser.addElement(L"LoadAnnotations", L"json");

   if(!parser.parse(argc, argv))
      {
      std::wcout << "\nTo run the program, the executable needs to be called from the command prompt\n";
      std::wcout << "with the following arguments:\n";
      std::wcout << "1- ImgDir: Path to the parent folder of the images to label.\n";
      std::wcout << "2- DestDir: Path where to save the annotations.\n";
      std::wcout << "3- LoadAnnotations: Path to the json file for loading and editing. (optionnal)\n";
      std::wcout << "\nPress <enter> to end.\n";
      MosGetch();
      return 0;
      }

   PrintInstruction();

   FileManager Fm;
   ImageProc Ip;
   HookData HookDataStruct;

   AIL_STRING RootDir = parser.getString(L"ImgDir");
   AIL_STRING DestDir = parser.getString(L"DestDir");

   AIL_STRING AnnotationFilePath;

   AIL_UNIQUE_BUF_ID  AilImage;

   auto AilDisplay = Ip.getDisplay();
   MdispControl(AilDisplay, M_SCALE_DISPLAY, M_ENABLE);

   vector<File> Images; 
   Fm.loadPatches(RootDir, Images);

   Dataset WorkingDataset(RootDir);

   MdispHookFunction(AilDisplay, M_KEY_DOWN, KeyPressed, &HookDataStruct);
   MdispHookFunction(AilDisplay, M_MOUSE_RIGHT_BUTTON_UP, RightClick, &HookDataStruct);
   
   if(parser.isAvailable(L"LoadAnnotations"))
      {
      AnnotationFilePath = parser.getString(L"LoadAnnotations");
      std::wcout << L"Annotation File Path: " << AnnotationFilePath << "\n";
      WorkingDataset.importJSON(AnnotationFilePath);
      }

   // For Preannotation
   AIL_UNIQUE_DLOCR_ID AilDlocrContext = MdlocrAlloc(Ip.getSystem(), M_OCR_NET1_BALANCED_V1, M_DEFAULT, M_UNIQUE_ID);
   AIL_UNIQUE_DLOCR_ID AilDlocrResult = MdlocrAllocResult(Ip.getSystem(), M_OCR_NET1_READ_RESULT, M_DEFAULT, M_UNIQUE_ID);
   
   AIL_INT SizeX, SizeY;
   MbufDiskInquire(Images[0].getPath(), M_SIZE_X, &SizeX);
   MbufDiskInquire(Images[0].getPath(), M_SIZE_Y, &SizeY);
   MdlocrControl(AilDlocrContext, M_TARGET_MAX_SIZE_X, SizeX);
   MdlocrControl(AilDlocrContext, M_TARGET_MAX_SIZE_Y, SizeY);
   MdlocrPreprocess(AilDlocrContext, M_DEFAULT);
   
   AIL_UNIQUE_GRA_ID AilGraphicsList = MgraAllocList(Ip.getSystem(), M_DEFAULT, M_UNIQUE_ID);
   AIL_UNIQUE_GRA_ID AilGraphicsContext = MgraAlloc(Ip.getSystem(), M_UNIQUE_ID);
   MgraControlList(AilGraphicsList, M_LIST, M_DEFAULT, M_SELECTION_RADIUS, 10);
   MdispControl(AilDisplay, M_ASSOCIATED_GRAPHIC_LIST_ID, AilGraphicsList);

   Fm.createDirs(DestDir);
   AIL_INT ArrayIndex = 0;

   std::wcout << L"Labeling ...\n";
   while(!FINISH)
      {
      auto& Img = Images[ArrayIndex];
      std::wcout << "Image " << "(" << (ArrayIndex + 1) << " of " << (Images.size()) << ") :: " << Img.getLocalPath()  << endl;
      if(ArrayIndex == 0)
         cout << "You are at the begining of your data." << endl;
      if(ArrayIndex == Images.size() - 1)
         cout << "You have reached the end of your data." << endl;

      AIL_INT SizeX, SizeY;
      MbufDiskInquire(Img.getPath(), M_SIZE_X, &SizeX);
      MbufDiskInquire(Img.getPath(), M_SIZE_Y, &SizeY);
      SAME_IMAGE = true;

      // Add the image to the m_Entries. 
      if(!WorkingDataset.entryExists(Img.getLocalPath()))
         {
         WorkingDataset.addEntry(Img.getLocalPath(), SizeX, SizeY);
         WorkingDataset.selectEntry(Img.getLocalPath());
         }
      else 
         {
         WorkingDataset.selectEntry(Img.getLocalPath());
         }
      
      // Load and display the image.
      // Reallocate for each image to make sure it supports different images sizes. 
      AilImage = MbufAllocColor(Ip.getSystem(), 1, SizeX, SizeY, 8 + M_UNSIGNED, M_IMAGE + M_DISP + M_PROC, M_UNIQUE_ID);
      MbufImport(Img.getPath(), M_DEFAULT, M_LOAD, Ip.getSystem(), &AilImage);
      MdispSelect(AilDisplay, AilImage);

      MgraClear(AilGraphicsContext, AilGraphicsList);
      
      //If entry is already populated, restore the bounding boxes
      if (WorkingDataset.getNumberOfBoundingBoxes() > 0)
         {
         unordered_map<AIL_INT, BoundingBox> TempBB;
         AIL_INT BbLabel, TxtLabel;
         AIL_DOUBLE Othr;
         WorkingDataset.clearTextBoxLink();
         auto [beginIt, endIt] = WorkingDataset.getEntry().getBoundingBoxIterators();
      
         for(auto it = beginIt; it != endIt; ++it)
            {
            AIL_INT OldBbLabel = it->first;
            BoundingBox Bb = it->second;
            WorkingDataset.getObjectCoords(&TLX, &TLY, &Othr, &Othr, &Othr, &Othr, &BRX, &BRY, OldBbLabel);
            MgraRect(AilGraphicsContext, AilGraphicsList, TLX, TLY, BRX, BRY);
            MgraInquireList(AilGraphicsList, M_LIST, M_DEFAULT, M_LAST_LABEL, &BbLabel);
            MgraControlList(AilGraphicsList, M_GRAPHIC_LABEL(BbLabel), M_DEFAULT, M_ROTATABLE, M_DISABLE);
            MgraText(AilGraphicsContext, AilGraphicsList, TLX, BRY, Bb.getClass());
            MgraInquireList(AilGraphicsList, M_LIST, M_DEFAULT, M_LAST_LABEL, &TxtLabel);
            MgraControlList(AilGraphicsList, M_GRAPHIC_LABEL(TxtLabel), M_DEFAULT, M_SELECTABLE, M_DISABLE);
         
            WorkingDataset.getEntry().addTextBoxLink(BbLabel, TxtLabel);
            TempBB.emplace(BbLabel, Bb); // Prepare new map for the bounding boxes with the new graphic labels.
            if(!TempBB[BbLabel].getClass().compare(L" "))
               {
               MgraControlList(AilGraphicsList, M_GRAPHIC_LABEL(TxtLabel), M_DEFAULT, M_BACKGROUND_MODE, M_TRANSPARENT);
               MgraControlList(AilGraphicsList, M_GRAPHIC_LABEL(BbLabel), M_DEFAULT, M_COLOR, M_COLOR_RED);
               }
            else
               {
               MgraControlList(AilGraphicsList, M_GRAPHIC_LABEL(BbLabel), M_DEFAULT, M_COLOR, M_COLOR_GREEN);
               MgraControlList(AilGraphicsList, M_GRAPHIC_LABEL(TxtLabel), M_DEFAULT, M_COLOR, M_COLOR_GREEN);
               MgraControlList(AilGraphicsList, M_GRAPHIC_LABEL(TxtLabel), M_DEFAULT, M_BACKGROUND_MODE, M_TRANSPARENT);
               }
            }
         WorkingDataset.clearBoundingBoxes();
         WorkingDataset.getEntry().assignNewBoundingBoxesMap(TempBB);
         WorkingDataset.getEntry().updateSelectedBoundingBoxLabel(-1);
         std::wcout << WorkingDataset.getNumberOfBoundingBoxes() << " bounding boxes loaded." << "\n";
         }

      MdispControl(AilDisplay, M_GRAPHIC_LIST_INTERACTIVE, M_ENABLE);

      // Set hook function.
      HookDataStruct.AilSystem = Ip.getSystem();
      HookDataStruct.AilDisplay = AilDisplay;
      HookDataStruct.AilImage = AilImage;
      HookDataStruct.Dataset = &WorkingDataset;
      HookDataStruct.ArrayIndex = ArrayIndex;
      HookDataStruct.DestDir = DestDir;
      HookDataStruct.AilGraphicsList = AilGraphicsList;
      HookDataStruct.AilGraphicsContext = AilGraphicsContext;
      HookDataStruct.AilDlocrContext = AilDlocrContext;
      HookDataStruct.AilDlocrResult = AilDlocrResult;
      
      MgraHookFunction(AilGraphicsList, M_GRAPHIC_SELECTION_MODIFIED, GraphicSelectionHookHandler, &HookDataStruct);
      MgraHookFunction(AilGraphicsList, M_GRAPHIC_MODIFIED, GraphicModificationHookHandler, &HookDataStruct);
      
      
      while (SAME_IMAGE and !FINISH)
      	Sleep(100);
      
      MgraHookFunction(AilGraphicsList, M_GRAPHIC_SELECTION_MODIFIED + M_UNHOOK, GraphicSelectionHookHandler, &HookDataStruct);
      MgraHookFunction(AilGraphicsList, M_GRAPHIC_MODIFIED + M_UNHOOK, GraphicModificationHookHandler, &HookDataStruct);
      
      ArrayIndex = max(0, HookDataStruct.ArrayIndex);
      ArrayIndex = min(ArrayIndex, static_cast<AIL_INT>(Images.size()) - 1);
      }

   MdispHookFunction(AilDisplay, M_KEY_DOWN + M_UNHOOK, KeyPressed, &HookDataStruct);
   MdispHookFunction(AilDisplay, M_MOUSE_RIGHT_BUTTON_UP + M_UNHOOK, RightClick, &HookDataStruct);

   WorkingDataset.generateJSON(DestDir);

   std::wcout << "\nDone!\n\n";
   }