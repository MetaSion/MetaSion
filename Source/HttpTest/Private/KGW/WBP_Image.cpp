// Fill out your copyright notice in the Description page of Project Settings.


#include "KGW/WBP_Image.h"
#include "HttpActor.h"

#include "Components/Image.h"
#include "Components/TextBlock.h"


void UWBP_Image::PlayShowImageAnimation()
{
    if (ShowImage)
    {
        PlayAnimation(ShowImage, 0.0f, 1, EUMGSequencePlayMode::Forward ,3.0f, true );
        UE_LOG(LogTemp, Log, TEXT("ShowImage Animation is playing"));
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("ShowImage Animation is nullptr"));
    }
}

void UWBP_Image::SetImage(UTexture2D* newTexture)
{
    ImageWeb->SetBrushFromTexture(newTexture);

}

void UWBP_Image::GetImage()
{
    HttpActor->ReqGetWebImage(WebImageURL);

}

// void UWBP_Image::SetTextLog(FString explain)
// {
//     Text_Explain->SetText(FText::FromString(explain));
// 
// }

void UWBP_Image::SendTextPost()
{
 TMap<FString,FString> UserData;
 FLogin LoginData;

 FString TextID =  LoginData.userId;
 UserData.Add("userId", TextID);

 FString json = UJsonParseLib::MakeJson(UserData);

 HttpActor->ReqPostTest(TextURL,json);

}



