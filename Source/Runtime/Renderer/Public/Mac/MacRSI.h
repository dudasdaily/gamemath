#pragma once
#include <vector>
#include <string>

namespace CK
{
class MacRSI : public RendererInterface
{
public:
	MacRSI() = default;
	~MacRSI();

public:
	virtual bool Init(const ScreenPoint& InScreenSize) override;
	virtual void Shutdown() override;
	virtual bool IsInitialized() const override { return _Initialized; }

	virtual void Clear(const LinearColor& InClearColor) override;
	virtual void BeginFrame() override;
	virtual void EndFrame() override;

	virtual void DrawPoint(const Vector2& InVectorPos, const LinearColor& InColor) override;
	virtual void DrawPoint(const ScreenPoint& InScreenPos, const LinearColor& InColor) override;
	virtual void DrawLine(const Vector2& InStartPos, const Vector2& InEndPos, const LinearColor& InColor) override;
	virtual void DrawLine(const Vector4& InStartPos, const Vector4& InEndPos, const LinearColor& InColor) override;

	virtual float GetDepthBufferValue(const ScreenPoint& InPos) const override;
	virtual void SetDepthBufferValue(const ScreenPoint& InPos, float InDepthValue) override;

	virtual void DrawFullVerticalLine(int InX, const LinearColor& InColor) override;
	virtual void DrawFullHorizontalLine(int InY, const LinearColor& InColor) override;

	virtual void PushStatisticText(std::string && InText) override;
	virtual void PushStatisticTexts(std::vector<std::string> && InTexts) override;

protected:
	FORCEINLINE bool IsInScreen(const ScreenPoint& InPos) const;
	FORCEINLINE int GetScreenBufferIndex(const ScreenPoint& InPos) const;
	FORCEINLINE void SetPixelOpaque(const ScreenPoint& InPos, const LinearColor& InColor);
	FORCEINLINE void SetPixel(const ScreenPoint& InPos, const LinearColor& InColor);

private:
	int TestRegion(const Vector2& InVectorPos, const Vector2& InMinPos, const Vector2& InMaxPos);
	bool CohenSutherlandLineClip(Vector2& InOutStartPos, Vector2& InOutEndPos, const Vector2& InMinPos, const Vector2& InMaxPos);

protected:
	bool _Initialized = false;
	Color32* _ScreenBuffer = nullptr;
	float* _DepthBuffer = nullptr;
	ScreenPoint _ScreenSize;
	std::vector<std::string> _StatisticTexts;
};

FORCEINLINE bool MacRSI::IsInScreen(const ScreenPoint& InPos) const
{
	if ((InPos.X < 0 || InPos.X >= _ScreenSize.X) || (InPos.Y < 0 || InPos.Y >= _ScreenSize.Y))
	{
		return false;
	}
	return true;
}

FORCEINLINE int MacRSI::GetScreenBufferIndex(const ScreenPoint& InPos) const
{
	return InPos.Y * _ScreenSize.X + InPos.X;
}

FORCEINLINE void MacRSI::SetPixelOpaque(const ScreenPoint& InPos, const LinearColor& InColor)
{
	if (!IsInScreen(InPos)) return;
	_ScreenBuffer[GetScreenBufferIndex(InPos)] = InColor.ToColor32();
}

FORCEINLINE void MacRSI::SetPixel(const ScreenPoint& InPos, const LinearColor& InColor)
{
	SetPixelOpaque(InPos, InColor);
}

}
