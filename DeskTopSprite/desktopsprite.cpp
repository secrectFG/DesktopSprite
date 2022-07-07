#include "stdafx.h"
#include "desktopsprite.h"
#include <Windows.h>

IFrame* frame = NULL;

DeskTopSprite::DeskTopSprite(QWidget *parent, Qt::WFlags flags)
	: QWidget(parent, flags),
	deltaTime(0),
	lastTime(0),
	press(false),
	bShowMenu(false)
{
	timer.start();
	mt = newMT(timeGetTime());
	frame = this;
	ui.setupUi(this);

	this->setAttribute(Qt::WA_TranslucentBackground);//设置背景透明
	this->setWindowTitle(tr("FGDeskTopPet"));

	reg = new QSettings("HKEY_LOCAL_MACHINE\\SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Run",QSettings::NativeFormat);
	
	bool bAutoRun = false;
	QString path = QApplication::applicationFilePath();
	if (reg->value("DeskTopSprite").toString() == path)//判断是否已经注册
		bAutoRun = true;

	trayIcon = new QSystemTrayIcon(this);//创建一个系统托盘图标
	trayIcon->setIcon(windowIcon());
	trayIcon->show();

	act_about = new QAction(tr("关于"), this);
	act_autoRun = new QAction(tr("开机启动"), this);
	act_autoRun->setCheckable(true);
	act_autoRun->setChecked(bAutoRun);
	act_exit = new QAction(tr("退出"), this);
	act_systemTray = new QAction(tr("显示系统托盘"), this);
	act_systemTray->setCheckable(true);
	act_systemTray->setChecked(trayIcon->isVisible());
	menu=new QMenu(this);
	menu->addAction(act_about);
	menu->addAction(act_autoRun);
	menu->addAction(act_systemTray);
	menu->addAction(act_exit); 
	
	connect(menu, SIGNAL(aboutToHide()), this, SLOT(AboutToHide()));
	connect(menu, SIGNAL(aboutToShow()), this, SLOT(AboutToShow()));
	connect(act_exit, SIGNAL(triggered()), this, SLOT(close()));
	connect(act_exit, SIGNAL(triggered()), this, SLOT(Close()));
	connect(act_about, SIGNAL(triggered()), this, SLOT(About()));
	connect(act_autoRun, SIGNAL(toggled (bool)), this, SLOT(AutoRunCheck(bool)));
	connect(act_systemTray, SIGNAL(toggled (bool)), this, SLOT(ShowTrayCheck(bool)));
	//获取当前路径
	std::string pathname = GetCerrentDir();
	currentDir = pathname.c_str();
	std::string luaPath = (pathname + "/script/");

	RegisterFunctionToLua();

	gLua->GetGlobals().SetString("g_currentPath", pathname.c_str());
	int ret = gLua->DoFile((luaPath+"main.luac").c_str());
	if (ret)
	{
		ret = gLua->DoFile((luaPath+"main.lua").c_str());
	}
	if(ret){
		QMessageBox::warning(this, tr("错误！"), tr("脚本加载失败！"));
		Q_ASSERT(0);
	}

	LuaFunctionVoid fun = gLua->GetGlobal("Init");
	fun(pathname.c_str());


	
	
	lastTime = GetTime();

	updateTimer = new QTimer(this);
	connect(updateTimer, SIGNAL(timeout()), this, SLOT(update()));
	updateTimer->start(1/30.f*1000.f);

	
}

DeskTopSprite::~DeskTopSprite()
{
	trayIcon->hide();
	delete trayIcon;
	delete mt;
	delete reg;
	
	LuaFunctionVoid fun = gLua->GetGlobal("Release");
	fun();

	for (size_t i = 0; i < picList.size() ;i++)
	{
		delete picList[i];
	}
}

//重写绘图事件
void DeskTopSprite::paintEvent(QPaintEvent *)
{
	float nowTime = timer.elapsed()/1000.f;
	deltaTime = nowTime - lastTime;
	FrameUpdate();
	QPainter painter(this);

	for (size_t i = 0;i< renderQueue.size();i++)
	{
		RenderData& data = renderQueue[i];
		QImage img = data.pic->toImage();
		painter.drawImage(data.x,data.y,img.mirrored(data.bMirror, false));
	}

	renderQueue.clear();

	lastTime = nowTime;
}

void DeskTopSprite::mousePressEvent(QMouseEvent *event)
{
	raise();
	if(event->button() == Qt::LeftButton)
	{
		oldPos = event->globalPos();
		press = true;
		LuaFunctionVoid fun = gLua->GetGlobal("OnHit");
		fun(event->globalX () , event->globalY (), event->x(), event->y());
	}
}

void DeskTopSprite::mouseMoveEvent(QMouseEvent *event)
{
	if (press)
	{
		oldPos = event->globalPos();
		LuaFunctionVoid fun = gLua->GetGlobal("OnMove");
		fun(event->globalX () , event->globalY ());
	}
}

void DeskTopSprite::mouseReleaseEvent(QMouseEvent *event)
{
	press = false;
	LuaFunctionVoid fun = gLua->GetGlobal("OnMouseRelease");
	fun();
}


void DeskTopSprite::RenderPic(  int x, int y, QPixmap* pic , bool bMirror )
{
	RenderData data;
	data.pic = pic;
	data.bMirror = bMirror;
	data.x = x;
	data.y = y;
	renderQueue.push_back(data);
}

void DeskTopSprite::FrameUpdate()
{
	LuaFunctionVoid fun = gLua->GetGlobal("Update");
	fun(deltaTime);
}

QPixmap* DeskTopSprite::LoadPic( const QString& fn )
{
	QPixmap *p = new QPixmap;
	if(!p->load(currentDir+"/"+fn))
	{
		QMessageBox::warning(this, tr("错误！"), tr("加载图片失败:")+fn);
		Q_ASSERT(0);
		return NULL;
	}
	picList.push_back(p);
	return p;
}

float DeskTopSprite::GetTime()const
{
	return timer.elapsed()/1000.f;
}

void DeskTopSprite::SetDebugMode()
{
	this->setWindowFlags(Qt::WindowStaysOnTopHint);
}

void DeskTopSprite::Resize( int x, int y )
{
	this->resize(x, y);
}

QPoint DeskTopSprite::GetWinPos() const
{
	return this->pos();
}

int DeskTopSprite::GetScreenWidth() const
{
	return QApplication::desktop()->width();
}

int DeskTopSprite::GetScreenHeight() const
{
	return QApplication::desktop()->height();
}

void DeskTopSprite::Move( int x, int y )
{
	this->move( x, y );
}

void DeskTopSprite::contextMenuEvent( QContextMenuEvent *event )
{
	QCursor cur=this->cursor();
	
	menu->exec(cur.pos()); //关联到光标
}

void DeskTopSprite::About()
{
	QMessageBox::about(this, tr("关于 FGDeskTopPet"), tr("版本 beta 1.02\n作者:谜の铁皮\n邮箱:fg0005823820@163.com\n微博:http://weibo.com/wtfg"));
}

void DeskTopSprite::AboutToHide()
{
	bShowMenu = false;
	updateTimer->start();
}

void DeskTopSprite::AboutToShow()
{
	bShowMenu = true;
	updateTimer->stop();
}

void DeskTopSprite::Close()
{
	QApplication::quit();
}

void DeskTopSprite::AutoRunCheck( bool bCheck )
{
	//开机自动运行
	if (bCheck)
	{
		reg->setValue("DeskTopSprite",QApplication::applicationFilePath());
	}
	else
	{
		reg->setValue("DeskTopSprite","");
	}
}

std::string DeskTopSprite::GetCerrentDir()
{
	std::string ret;
	std::string temp =  QApplication::applicationFilePath().toStdString();
	std::string::iterator it = temp.end();
	while(1)
	{
		it --;
		char c = *it;
		if (c == '/' || c == '\\')
		{
			it++;
			ret.assign(temp.begin(), it);
			break;
		}
	}
	
	return ret;
}




int DeskTopSprite::Random_Int(int min, int max)
{
	double min_=min;double max_=max;
	double r=genrand_res53(mt);
	if (min_>max_) { double t=min_; min_=max_; max_=t; }
	min_=ceil(min_);
	max_=floor(max_);
	if (min_>max_) return 0;
	r=min_+floor(r*(max_-min_+1));
	return (int)r;
}

double DeskTopSprite::Random_Double( double min,double max )
{
	double r=genrand_res53(mt);
	if (min>max) { double t=min; min=max; max=t; }
	r=min+(r*((max-1)-min+1));
	return r;
}

void DeskTopSprite::ShowTrayCheck( bool bCheck )
{
	bCheck ? trayIcon->show() : trayIcon->hide();
}
