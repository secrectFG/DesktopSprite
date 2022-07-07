#include "stdafx.h"
#include "luaScript.h"
#include "IFrame.h"
#include "hgevector.h"
extern IFrame* frame;

LuaScript gLua;




static void luaQDubug( const char* str )
{
	qDebug()<<str;
}

static void RenderPic( float x, float y, void* _pic, bool bMirror)
{
	QPixmap* pic = static_cast<QPixmap*>(_pic);
	frame->RenderPic((int)x, (int)y, pic, bMirror);
}

static void* LoadPic(const char* fn)
{
	return frame->LoadPic(fn);
}

static float GetTime()
{
	return frame->GetTime();
}

static void SetDebugMode()
{
	frame->SetDebugMode();
}

static void Resize( int x, int y )
{
	frame->Resize(x,y);
}

static int GetWinPos(LuaState* state)
{
	const QPoint& pos = frame->GetWinPos();
	state->PushInteger(pos.x());
	state->PushInteger(pos.y());
	return 2;
}

static int GetScreenWidth()
{
	return frame->GetScreenWidth();
}

static int GetScreenHeight()
{
	return frame->GetScreenHeight();
}

static int GetPicWidth(void* _pic)
{
	QPixmap* pic = static_cast<QPixmap*>(_pic);
	return pic->width();
}

static int GetPicHeight(void* _pic)
{
	QPixmap* pic = static_cast<QPixmap*>(_pic);
	return pic->height();
}

static void Move(int x, int y)
{
	frame->Move(x, y);
}

static int Random_Int(int min, int max)
{
	return frame->Random_Int(min,max);
}

static double Random_Double( double min,double max )
{
	return frame->Random_Double(min,max);
}

void RegisterFunctionToLua()
{
	gLua.RegisterCFunctionToLua(luaQDubug, "qDebug");
	gLua.RegisterCFunctionToLua(RenderPic, "RenderPic");
	gLua.RegisterCFunctionToLua(LoadPic, "LoadPic");
	gLua.RegisterCFunctionToLua(GetTime, "GetTime");
	gLua.RegisterCFunctionToLua(SetDebugMode, "SetDebugMode");
	gLua.RegisterCFunctionToLua(Resize, "Resize");
	gLua.Register(GetWinPos, "GetWinPos");
	gLua.RegisterCFunctionToLua(GetScreenWidth, "GetScreenWidth");
	gLua.RegisterCFunctionToLua(GetScreenHeight, "GetScreenHeight");
	gLua.RegisterCFunctionToLua(GetPicWidth, "GetPicWidth");
	gLua.RegisterCFunctionToLua(GetPicHeight, "GetPicHeight");
	gLua.RegisterCFunctionToLua(Move, "Move");
	gLua.RegisterCFunctionToLua(Random_Int, "Random_Int");
	gLua.RegisterCFunctionToLua(Random_Double, "Random_Double");
}

