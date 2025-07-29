// #include "C:/Wichtig/System/Static/Library/WindowEngine1.0.h"
// #include "C:/Wichtig/System/Static/Library/Victor2.h"
// #include "C:/Wichtig/System/Static/Library/Vdctor2.h"
// #include "C:/Wichtig/System/Static/Library/Complex.h"
// #include "C:/Wichtig/System/Static/Library/TransformedView.h"
// #include "C:\Wichtig\System\Static\Container\Vector.h"

#include "/home/codeleaded/System/Static/Library/WindowEngine1.0.h"
#include "/home/codeleaded/System/Static/Library/Victor2.h"
#include "/home/codeleaded/System/Static/Library/Vdctor2.h"
#include "/home/codeleaded/System/Static/Library/Complex2.h"
#include "/home/codeleaded/System/Static/Library/TransformedView.h"
#include "/home/codeleaded/System/Static/Container/Vector.h"


TransformedViewD tv;
int nMode = 0;
int nIterations = 1;
float Curving = 1.0f;

void RenderSierpinskiTriangle(Vec2 p,Vec2 d,int Iteration,int MaxIteration,Pixel first,Pixel second){
	if(Iteration<MaxIteration){
		Vec2 sp1 = { p.x - d.x * 0.5f,p.y + d.y };
		Vec2 sp2 = { p.x + d.x * 0.5f,p.y + d.y };
		RenderTriangle(p,sp1,sp2,Pixel_Inter(first,second,(float)Iteration / (float)MaxIteration));

		Vec2 mp1 = { p.x,				p.y + d.y };
		Vec2 mp2 = { p.x - d.x * 0.25f,	p.y + d.y * 0.5f };
		Vec2 mp3 = { p.x + d.x * 0.25f,	p.y + d.y * 0.5f };
		RenderTriangle(mp1,mp2,mp3,Pixel_Inter(first,second,(float)(Iteration+1) / (float)MaxIteration));
		
		RenderSierpinskiTriangle(p,(Vec2){ d.x * 0.5f,d.y * 0.5f },Iteration+1,MaxIteration,first,second);
		RenderSierpinskiTriangle(mp2,(Vec2){ d.x * 0.5f,d.y * 0.5f },Iteration+1,MaxIteration,first,second);
		RenderSierpinskiTriangle(mp3,(Vec2){ d.x * 0.5f,d.y * 0.5f },Iteration+1,MaxIteration,first,second);
	}
}

void Setup(AlxWindow* w){
	tv = TransformedViewD_New((Vdc2){ GetWidth(),GetHeight() });
	TransformedViewD_Offset(&tv,(Vdc2){ -0.5,0.0 });

	nIterations = 5;

	//nMode = 5;

	ResizeAlxFont(16,16);
}

void Update(AlxWindow* w){
	tv.ZoomSpeed = (float)w->ElapsedTime;
	TransformedViewD_HandlePanZoom(&tv,window.Strokes,(Vdc2){ GetMouse().x,GetMouse().y });

	if (Stroke(ALX_KEY_UP).PRESSED) 	nIterations += 1;
	if (Stroke(ALX_KEY_DOWN).PRESSED) 	nIterations -= 1;
	if (nIterations < 1) 				nIterations = 1;

	if (Stroke(ALX_KEY_LEFT).DOWN) 	Curving *= 0.99f;
	if (Stroke(ALX_KEY_RIGHT).DOWN) Curving *= 1.01f;

	Timepoint tp1 = Time_Nano();

	Clear(BLACK);

	Vdc2 p = TransformedViewD_WorldScreenPos(&tv,(Vdc2){ 0.0f,0.0f });
	Vdc2 d = TransformedViewD_WorldScreenLength(&tv,(Vdc2){ Curving,1.0f });
	RenderSierpinskiTriangle((Vec2){ p.x,p.y },(Vec2){ d.x,d.y },0,nIterations,BLUE,GREEN);
	
	Timepoint tp2 = Time_Nano();
	double elapsedTime = Time_Elapsed(tp1,tp2);

	String str = String_Format("| Offset: %f,%f - Zoom: %f,%f |",tv.Offset.x,tv.Offset.y,tv.Scale.x,tv.Scale.y);
	RenderCStrSize(str.Memory,str.size,0,0,WHITE);
	String_Free(&str);

	str = String_Format("| w->ElapsedTime Taken: %fs - Iterations: %d |",elapsedTime,nIterations);
	RenderCStrSize(str.Memory,str.size,0,GetAlxFont()->CharSizeY + 10,WHITE);
	String_Free(&str);
}

void Delete(AlxWindow* w){
    
}

int main(){
    if(Create("Trifractal",1200,1200,1,1,Setup,Update,Delete))
        Start();
    return 0;
}