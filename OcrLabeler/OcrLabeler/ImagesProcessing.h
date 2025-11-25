#pragma once
#include "FileManager.h"

class ImageProc
   {
   AIL_ID MilSystem,
      MilApplication,
      MilImage,
      MilDisplay,
      MilDispOverlay = M_NULL;

   public:

      ImageProc()
         {
         MappAllocDefault(M_DEFAULT, &MilApplication, &MilSystem, &MilDisplay, M_NULL, M_NULL);
         MdispControl(MilDisplay, M_SCALE_DISPLAY, M_ENABLE);
         MdispControl(MilDisplay, M_OVERLAY_CLEAR, M_DEFAULT);
         }

      ~ImageProc()
         {
         MappFreeDefault(MilApplication, MilSystem, MilDisplay, M_NULL, M_NULL);
         }

      AIL_ID getSystem()
         {
         return MilSystem;
         }

      AIL_ID getApp()
         {
         return MilApplication;
         }

      AIL_ID getDisplay()
         {
         return MilDisplay;
         }

      AIL_ID getDisplayOverlay()
         {
         if(MilDispOverlay == M_NULL)
            MdispInquire(MilDisplay, M_OVERLAY_ID, &MilDispOverlay);

         return MilDispOverlay;
         }

      AIL_ID AllocBuf(AIL_INT BandSize, AIL_INT SizeX, AIL_INT SizeY, AIL_INT Type = 8 + M_UNSIGNED, AIL_INT64 Att = M_IMAGE + M_PROC + M_DISP)
         {
         AIL_ID MilImage = MbufAllocColor(MilSystem, BandSize, SizeX, SizeY, 8 + M_UNSIGNED, M_IMAGE + M_PROC + M_DISP, M_NULL);
         return MilImage;
         }

      void resize(File p,
                  AIL_ID &MilResizedImg,
                  AIL_INT SizeX,
                  AIL_INT SizeY,
                  AIL_INT OffsetX = 0,
                  AIL_INT OffsetY = 0,
                  AIL_INT64 Interpolation = M_INTERPOLATE)
         {
         MbufRestore(p.getPath(), MilSystem, &MilImage);

         AIL_INT Width = 0;
         AIL_INT Height = 0;

         MbufInquire(MilImage, M_SIZE_X, &Width);
         MbufInquire(MilImage, M_SIZE_Y, &Height);

         AIL_ID MilChild = MbufChildColor2d(MilImage, M_ALL_BANDS, OffsetX / 2, OffsetY / 2, Width - OffsetX, Height - OffsetY, M_NULL);

         MbufClone(MilChild,
                   M_DEFAULT,
                   SizeX, SizeY,
                   M_DEFAULT,
                   M_DEFAULT,
                   M_DEFAULT,
                   &MilResizedImg);

         MimResize(MilChild, MilResizedImg, M_FILL_DESTINATION, M_FILL_DESTINATION, Interpolation);

         MbufFree(MilChild);
         MbufFree(MilImage);
         return;
         }
   };
