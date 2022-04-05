// Fill out your copyright notice in the Description page of Project Settings.


#include "GISRasterfile.h"

// #include "UnrealGDAL.h"
// #include "GDALHelpers.h"

#include <Runtime/ImageWrapper/Public/IImageWrapper.h>
#include <Runtime/ImageWrapper/Public/IImageWrapperModule.h>

TArray<int> UGISRasterfile::GetHeight(FString path, FVector2D& size) 
{
    TArray<int> height; 

    IImageWrapperModule& ImageWrapperModule = FModuleManager::LoadModuleChecked<IImageWrapperModule>(FName("ImageWrapper"));
    TSharedPtr<IImageWrapper> ImageWrapper = ImageWrapperModule.CreateImageWrapper(EImageFormat::PNG);

    TArray64<uint8> RawFileData; 

    if (FFileHelper::LoadFileToArray(RawFileData, *path))
    {
        if (ImageWrapper.IsValid() && ImageWrapper->SetCompressed(RawFileData.GetData(), RawFileData.Num()))
        {
            TArray<uint8> UncompressedBGRA;
            const ERGBFormat format = ERGBFormat::BGRA; 
            int32 inBitDepth = 8; 
            bool success = ImageWrapper->GetRaw(format, inBitDepth, UncompressedBGRA); 
            if (success)
            {
                size = FVector2D(ImageWrapper->GetWidth(), ImageWrapper->GetHeight());

                //Get only blue value
                for (int32 x = 0; x < UncompressedBGRA.Num(); x = x + 4)
                {
                    height.Add(UncompressedBGRA[x]); 
                }
            }
        }
    }
    return height; 
}