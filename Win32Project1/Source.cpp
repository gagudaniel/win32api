#include <windows.h>
#include <time.h>

#define IDC_MAIN_EDIT1       100
#define IDC_MAIN_EDIT2       104
#define IDC_MAIN_EDIT3       105
#define IDC_MAIN_EDIT4       106
#define IDC_MAIN_BUTTON1	101			
#define IDC_MAIN_BUTTON2	102		
#define IDC_MAIN_BUTTON3	103	


void patrat(HDC hdc, int i, int j, int color);
void teren(HWND hWnd);
void getcellindex(int x, int y,int* i,int* j);
void create_lab(HWND hWnd, LPARAM lParam);
void reset(HWND hWnd);
void coordonate(HWND hWnd);
int get_start_stop(HWND hEdit);
void butoane(HWND hWnd);
int rezolva(int pas, int directiex, int directiey, HDC hdc);
int verifica(int nr, int directiex, int directiey);

int lab[10][10];
int x[100];
int y[100];
int startx, starty, stopx, stopy;
int directie[4][2] = { { 0, 1 }, { 0, -1 }, { 1, 0 }, { -1, 0 } };
HWND hEdit1, hEdit2, hEdit3, hEdit4;

LRESULT CALLBACK WinProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

int WINAPI WinMain(HINSTANCE hInst, HINSTANCE hPrevInst, LPSTR lpCmdLine, int nShowCmd)
{


	WNDCLASSEX wClass;
	ZeroMemory(&wClass, sizeof(WNDCLASSEX));
	wClass.cbClsExtra = NULL;
	wClass.cbSize = sizeof(WNDCLASSEX);
	wClass.cbWndExtra = NULL;
	wClass.hbrBackground = (HBRUSH)COLOR_WINDOW;
	wClass.hCursor = LoadCursor(NULL, IDC_ARROW);
	wClass.hIcon = NULL;
	wClass.hIconSm = NULL;
	wClass.hInstance = hInst;
	wClass.lpfnWndProc = (WNDPROC)WinProc;
	wClass.lpszClassName = "Window Class";
	wClass.lpszMenuName = NULL;
	wClass.style = CS_HREDRAW | CS_VREDRAW;

	if (!RegisterClassEx(&wClass))
	{
		int nResult = GetLastError();
		MessageBox(NULL,
			"Window class creation failed\r\n",
			"Window Class Failed",
			MB_ICONERROR);
	}

	HWND hWnd = CreateWindowEx(NULL,
		"Window Class",
		"Labirint",
		WS_OVERLAPPEDWINDOW,
		200,
		200,
		640,
		480,
		NULL,
		NULL,
		hInst,
		NULL);

	if (!hWnd)
	{
		int nResult = GetLastError();

		MessageBox(NULL,
			"Window creation failed\r\n",
			"Window Creation Failed",
			MB_ICONERROR);
	}

	ShowWindow(hWnd, nShowCmd);

	MSG msg;
	ZeroMemory(&msg, sizeof(MSG));

	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return 0;
}

LRESULT CALLBACK WinProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	
	switch (msg)
	{
		case WM_CREATE:
		{             
						  coordonate(hWnd);
						  butoane(hWnd);
					  
		}
			break;
		case WM_LBUTTONDOWN:
		{
							   
			if (!IsDlgButtonChecked(hWnd, IDC_MAIN_BUTTON2))
					create_lab(hWnd, lParam);
		}
			break;
		case WM_PAINT:
		{
			teren(hWnd);
		}
			break;
		case WM_COMMAND:
		{
				 switch(LOWORD(wParam))
						   {
								case IDC_MAIN_BUTTON3:  
									if (!IsDlgButtonChecked(hWnd, IDC_MAIN_BUTTON2)) reset(hWnd);
									break;
								case IDC_MAIN_BUTTON1:
									
										if (IsDlgButtonChecked(hWnd, IDC_MAIN_BUTTON2))
										{
											
											startx = get_start_stop(hEdit1);
											starty = get_start_stop(hEdit2);
											stopx = get_start_stop(hEdit3);
											stopy = get_start_stop(hEdit4);

											HDC hdc = GetDC(hWnd);
											if (lab[startx][starty] == 1 && lab[stopx][stopy] == 1)
											{

												patrat(hdc, startx, starty, 2);

												patrat(hdc, stopx, stopy, 3);
												x[0] = startx;
												y[0] = starty; 

												rezolva(1, startx, starty, hdc);
											}
											else{
												MessageBox(NULL,
													"Imposibil de rezolvat!"
													"Coordonate START/STOP incorecte.",
													"Eroare",
													MB_ICONERROR | MB_OK);
											}


											ReleaseDC(hWnd, hdc);
										}
									break;
								
						   }
		
		}
			break;
		case WM_DESTROY:
		{
			PostQuitMessage(0);
			return 0;
		}
			break;
	}
	return DefWindowProc(hWnd, msg, wParam, lParam);
}
void create_lab(HWND hWnd,LPARAM lParam)
{
	int xPos;
	int yPos;
	int i, j;

	xPos = LOWORD(lParam);
	yPos = HIWORD(lParam);
	getcellindex(xPos, yPos, &i, &j);
	HDC hdc = GetDC(hWnd);

	if (i!=-1)
	patrat(hdc, i, j, 1);

	ReleaseDC(hWnd, hdc);
}
void patrat(HDC hdc,int i,int j,int color )
{
	  HPEN hPen = CreatePen(PS_NULL, 1, RGB(0, 0, 0));

	  SelectObject(hdc, hPen);

	  HBRUSH hBrush1 = CreateSolidBrush(RGB(102, 51, 0));
	  HBRUSH hBrush2 = CreateSolidBrush(RGB(255, 0, 0));
	  HBRUSH hBrush3 = CreateSolidBrush(RGB(0, 255, 0));
	  HBRUSH hBrush4 = CreateSolidBrush(RGB(128, 128, 128));

	  switch (color)
	  {
	  case 0: SelectObject(hdc, hBrush1);
		  break;
	  case 1: SelectObject(hdc, hBrush4);
		  lab[i][j] = 1;
		  break;
	  case 2: SelectObject(hdc, hBrush3);
		  lab[i][j] = 1;
		  break;
	  case 3: SelectObject(hdc, hBrush2);
		  lab[i][j] = 1;
		  break;


	  }
		Rectangle(hdc, 35 + 37 * j + 2 * (j + 1), 35 + 37 * i + 2 * (i + 1), 35 + 37 * (j + 1) + 2 * (j + 1), 35 + 37 * (i + 1) + 2 * (i + 1));
		
}
void teren(HWND hWnd)
{

	LOGBRUSH brush;
	COLORREF col = RGB(0, 0, 0);
	DWORD pen_style = PS_SOLID | PS_JOIN_MITER | PS_GEOMETRIC;

	brush.lbStyle = BS_SOLID;
	brush.lbColor = col;
	brush.lbHatch = 0;

	PAINTSTRUCT ps;



	HDC hdc = BeginPaint(hWnd, &ps);

	HPEN hPen1 = ExtCreatePen(pen_style, 8, &brush, 0, NULL);
	SelectObject(hdc, hPen1);

	POINT points[5] = { { 30, 30 }, { 30, 431 }, { 431, 431 },
	{ 431, 30 }, { 30, 30 } };

	Polygon(hdc, points, 5);

	for (int i = 0; i <= 9; i++)
	{

		for (int j = 0; j <= 9; j++)
		{
			patrat(hdc, i, j, 0);
		}
	}

	EndPaint(hWnd, &ps);
}
void getcellindex(int x, int y, int* i, int* j)
{
	POINT pt = { x, y };
	RECT rc;
	rc.left = 37;
	rc.top = 37;
	rc.right = 427;
	rc.bottom = 427;

	if (PtInRect(&rc, pt))
	{
		x = pt.x - rc.left;
		y = pt.y - rc.top;

		(*i) = y / 39;
		(*j) = x / 39;
	}
	else{
		(*i) = -1;
	}
}
void reset(HWND hWnd)
{
	HDC hdc = GetDC(hWnd);

	for (int i = 0; i <= 9; i++)
	{

		for (int j = 0; j <= 9; j++)
		{
			lab[i][j] = 0;
			patrat(hdc, i, j, 0);
		}
	}

	ReleaseDC(hWnd, hdc);
}
void coordonate(HWND hWnd)
{
	char nr[2];

	CreateWindow("static", "START", WS_VISIBLE | WS_CHILD, 525, 50, 100, 24, hWnd, NULL, NULL, NULL);
	CreateWindow("static", "x", WS_VISIBLE | WS_CHILD, 515, 65, 20, 20, hWnd, NULL, NULL, NULL);
	CreateWindow("static", "y", WS_VISIBLE | WS_CHILD, 550, 65, 20, 20, hWnd, NULL, NULL, NULL);
	CreateWindow("static", "STOP", WS_VISIBLE | WS_CHILD, 528, 85, 100, 24, hWnd, NULL, NULL, NULL);
	CreateWindow("static", "x", WS_VISIBLE | WS_CHILD, 515, 100, 20, 20, hWnd, NULL, NULL, NULL);
	CreateWindow("static", "y", WS_VISIBLE | WS_CHILD, 550, 100, 20, 20, hWnd, NULL, NULL, NULL);
	for (int i = 0; i < 10; i++)
	{
		_itoa_s(i, nr, 10);
		CreateWindow("static", nr, WS_VISIBLE | WS_CHILD, 51 + 39 * i, 5, 20, 20, hWnd, NULL, NULL, NULL);
		CreateWindow("static", nr, WS_VISIBLE | WS_CHILD, 5, 46 + 39 * i, 20, 20, hWnd, NULL, NULL, NULL);
	}
}
int get_start_stop(HWND hEdit)
{
	char buffer[10];
	int index;
	SendMessage(hEdit,
		WM_GETTEXT,
		sizeof(buffer) / sizeof(buffer[0]),
		reinterpret_cast<LPARAM>(buffer));
	index = atoi(buffer);
	return index;
}
void butoane(HWND hWnd)
{
	hEdit1 = CreateWindowEx(WS_EX_CLIENTEDGE,
		"EDIT",
		"",
		WS_CHILD | WS_VISIBLE |
		ES_MULTILINE | ES_AUTOVSCROLL | ES_AUTOHSCROLL,
		525,
		65,
		20,
		20,
		hWnd,
		(HMENU)IDC_MAIN_EDIT1,
		GetModuleHandle(NULL),
		NULL);
	hEdit2 = CreateWindowEx(WS_EX_CLIENTEDGE,
		"EDIT",
		"",
		WS_CHILD | WS_VISIBLE |
		ES_MULTILINE | ES_AUTOVSCROLL | ES_AUTOHSCROLL,
		560,
		65,
		20,
		20,
		hWnd,
		(HMENU)IDC_MAIN_EDIT2,
		GetModuleHandle(NULL),
		NULL);
	hEdit3 = CreateWindowEx(WS_EX_CLIENTEDGE,
		"EDIT",
		"",
		WS_CHILD | WS_VISIBLE |
		ES_MULTILINE | ES_AUTOVSCROLL | ES_AUTOHSCROLL,
		525,
		100,
		20,
		20,
		hWnd,
		(HMENU)IDC_MAIN_EDIT3,
		GetModuleHandle(NULL),
		NULL);
	hEdit4 = CreateWindowEx(WS_EX_CLIENTEDGE,
		"EDIT",
		"",
		WS_CHILD | WS_VISIBLE |
		ES_MULTILINE | ES_AUTOVSCROLL | ES_AUTOHSCROLL,
		560,
		100,
		20,
		20,
		hWnd,
		(HMENU)IDC_MAIN_EDIT4,
		GetModuleHandle(NULL),
		NULL);
	HWND hWndButton = CreateWindowEx(NULL,
		"BUTTON",
		"GO!",
		WS_TABSTOP | WS_VISIBLE |
		WS_CHILD | BS_DEFPUSHBUTTON,
		500,
		220,
		100,
		24,
		hWnd,
		(HMENU)IDC_MAIN_BUTTON1,
		GetModuleHandle(NULL),
		NULL);


	HWND hWndButton1 = CreateWindowEx(NULL,
		"BUTTON",
		"Ready?",
		WS_TABSTOP | WS_VISIBLE |
		WS_CHILD | BS_AUTOCHECKBOX,
		500,
		180,
		100,
		24,
		hWnd,
		(HMENU)IDC_MAIN_BUTTON2,
		GetModuleHandle(NULL),
		NULL);

	HWND hWndButton2 = CreateWindowEx(NULL,
		"BUTTON",
		"RESET",
		WS_TABSTOP | WS_VISIBLE |
		WS_CHILD | BS_DEFPUSHBUTTON,
		500,
		140,
		100,
		24,
		hWnd,
		(HMENU)IDC_MAIN_BUTTON3,
		GetModuleHandle(NULL),
		NULL);
}
int rezolva(int pas, int directiex,int directiey,HDC hdc)
{
	if (directiex == stopx&&directiey == stopy)
	{
		patrat(hdc, stopx, stopy, 2);
		return 1;
	}

	for (int i = 0; i < 4; i++)
	{
		if (verifica(pas-1, directiex + directie[i][0], directiey + directie[i][1]) == 0)
		{
			x[pas] = directiex + directie[i][0];
			y[pas] = directiey + directie[i][1];
			SleepEx(500,FALSE);
				patrat(hdc, x[pas], y[pas], 2);
			if (rezolva(pas + 1, directiex + directie[i][0], directiey + directie[i][1],hdc))return 1;
			SleepEx(500, FALSE);
			patrat(hdc, x[pas], y[pas], 1);
		}
		
	}
	if (pas == 1)
	{
		MessageBox(NULL,
			"Imposibil de rezolvat!"
			"\nIncercati sa refaceti labirintul.",
			"Eroare",
			MB_ICONERROR | MB_OK);
	}
	return 0;
}
int verifica(int pas, int directiex, int directiey)
{
	if (directiex > 9 || directiey > 9 || directiex < 0 || directiey < 0)return 1;

	if (lab[directiex][directiey] == 0) return 1;

	for (int i = 0; i < pas; i++)
	{
		if (x[i] == directiex&&y[i] == directiey)return 1;
	}
	return 0;
}
