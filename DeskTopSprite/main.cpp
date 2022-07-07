#include "stdafx.h"
#include "desktopsprite.h"
#include <QtGui/QApplication>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);

	QTextCodec *codec = QTextCodec::codecForName("System");  
	QTextCodec::setCodecForLocale(codec);  
	QTextCodec::setCodecForCStrings(codec);  
	QTextCodec::setCodecForTr(codec); 

	DeskTopSprite w;
	w.show();
	return a.exec();
}
