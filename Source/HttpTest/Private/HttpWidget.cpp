// Fill out your copyright notice in the Description page of Project Settings.


#include "HttpWidget.h"
#include "Components/Button.h"
#include "HttpActor.h"
#include "Components/TextBlock.h"
#include "Components/EditableText.h"
#include "JsonParseLib.h"
#include "Engine/Texture2D.h"
#include "Components/Image.h"
#include "JsonObjectConverter.h"

void UHttpWidget::NativeConstruct()
{
	Super::NativeConstruct();
	// ��ư�� �����ϰ�ʹ�.
	ButtonPostSend->OnClicked.AddDynamic(this, &UHttpWidget::OnMyClickSendPost);

	ButtonGetWebImage->OnClicked.AddDynamic(this, &UHttpWidget::OnMyClickGetWebImage);
}

void UHttpWidget::OnMyClickSendPost()
{
	//�����ͺ� textblock���� �������� ������� ����
    FString json = HttpActor->Login_Convert_StructToJson(UserID->GetText().ToString(), UserPW->GetText().ToString());
	//Login��û
	HttpActor->ReqPostTest(ServerURL, json);
}

void UHttpWidget::OnMyClickGetWebImage()
{
	HttpActor->ReqGetWebImage(WebImageURL);
}

void UHttpWidget::SetWebImage(UTexture2D* newTexture)
{
	ImageWeb->SetBrushFromTexture(newTexture);
}

void UHttpWidget::SetTextLog(FString log)
{
	TextLog->SetText(FText::FromString(log));
}

void UHttpWidget::SetHttpActor(AHttpActor* actor)
{
	HttpActor = actor;
}
