#pragma once

struct IFrame
{
	virtual ~IFrame(){}

	virtual QPixmap* LoadPic(const QString& fn) = 0;
	virtual void RenderPic( int x, int y, QPixmap* pic, bool bMirror) = 0;
	virtual float GetTime()const = 0;
	virtual void SetDebugMode() = 0;
	virtual void Resize(int x, int y) = 0;
	virtual  QPoint GetWinPos()const = 0;
	virtual int GetScreenWidth()const = 0;
	virtual int GetScreenHeight()const = 0;
	virtual void Move(int x, int y) = 0;
	virtual int Random_Int(int min, int max) = 0;
	virtual double Random_Double( double min,double max ) = 0;
};