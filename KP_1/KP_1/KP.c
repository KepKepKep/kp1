// ³���������� ���� ���������� ����� "�' � "�"
// ����������: ���������\���������\��������

#include <windows.h>		// ϳ��������� ����������� �������
#define _USE_MATH_DEFINES	// ϳ��������� ������������ ��������
#include <math.h>

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);	// �������� ������� ����(���� �����)
LPCWSTR szProgName = L"Progname";						// ��'� ��������
char szText[] = "";										// ����� ��� ��������� ������������
			/*	����� �� ����������� ������ ����� ������� ���� ��������. 
				� ������ ������� - �� ����� ��������� � �� ��������� ������� ���� � */
POINT	tz1[16] = { -50,50,0,50,0,10,20,10,30,8,40,2,47,-9,50,-20, 47,-30, // ������� ������ �
					43,-40,32,-48,20,-50,-25,-50,-25,30,-50,30,-50,50 },	
	tz2[8] = { 0,-10,15,-10,22,-13,25,-20, 22,-27,15,-30,0,-30,0,-10 },		// �������� ������ �
	bz1[16] = { -35,50,35,50,35,30,-10,30,-10,10,10,10,20,8,30,2,38,-9,40,	// ������� ������ �
				-20,38,-30,33,-40,23,-48,10,-50,-35,-50,-35,50 },
	bz2[8] = {-10,-10,3,-10,13,-13,15,-20,13,-27,3,-30,-10,-30,-10,-10};	// �������� ������ �
POINT	z1[16], z2[8];
double alpha = 0.0, k=1.0;	// alpha - ��� ������ �����, �������� ����
							// k - ���������� ����������
				/* ��� - ������� �������� (������� Main ��� Win32) */
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	HWND hWnd;			// ������������� ����
	MSG lpMsg;			// ������������� �����������
	WNDCLASS w;			// ��������� ���������� ��������� WNDCLASS
						// � ��� - ���������� ����� �������� ���������
	w.lpszClassName = (LPCWSTR)szProgName;	// ��'� ��������
	w.hInstance = hInstance;		// ������������� ��������� �������
	w.lpfnWndProc = WndProc;		// �������� �� ������� ����
	w.hCursor = LoadCursor(NULL, IDC_ARROW);
	w.hIcon = LoadIcon(hInstance, IDI_APPLICATION);
	w.lpszMenuName = 0;				//���� ������ �� �����
	w.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);	// ���� ���� ����
	w.style = CS_HREDRAW | CS_VREDRAW;		//����� - ���������������� �� � � �� �
	w.cbClsExtra = 0;
	w.cbWndExtra = 0;

	RegisterClass(&w);						// ��������� ���������� ������ ���� 
											// ��� �������� �� ����� WNDCLASS
		/* ��������� ����� ���� � ���'��, ���������� ��������� ������� CreateWindow */
	hWnd = CreateWindow(szProgName,			// ��'� ��������
		L"�������� ���� ��������� - 1.0", // ��������� ����
		WS_OVERLAPPEDWINDOW,				// ����� ���� - ������������
		20,									// ��������� ���� �� ����� �� �� �
		20,									// �� �
		800,								// ������ (�� �� �)
		600,								// ������ (�� �)
		(HWND)NULL,							// ������������� ������������ ����
		(HMENU)NULL,						// ������������� ����
		(HINSTANCE)hInstance,				// ������������� ���������� ��������
		(HINSTANCE)NULL);					// ��� ���������� ���������
						  //��������� ���� � ���'�� �� �����
	ShowWindow(hWnd, nCmdShow);
	
	UpdateWindow(hWnd);		//��������� ����� ����

							//���� ������� ����������
	while (GetMessage(&lpMsg, NULL, 0, 0))
	{										// ��������� ����������� � �����
		TranslateMessage(&lpMsg);			// ������������ ����������� ����� � �������
		DispatchMessage(&lpMsg);			// ����������� ����������� �������� ������� ����
	}
	return(lpMsg.wParam);
}		/* ���������� ������� �������*/

/*=======  ������� ����, �������� ��� ��������� � ����� 8  == */
LRESULT CALLBACK WndProc(HWND hWnd, UINT messg,	WPARAM wParam, LPARAM lParam)
{
	HPEN hpen1;						//��'��� ��'���� ���� (������)
	HGDIOBJ hbrushOld, hpenOld;
	HBRUSH hbrush;					//��'��� ����� (��� ������)
	HDC hdc;						//��������� �������������� ��������� ��������
	PAINTSTRUCT ps;					//��������� ���������� ��������� ���������� ������
	int x = 800, y = 600, z=0,		// ������ ��������� ����
	bx, by;							// ������ ����
	unsigned int key = 20;			// ����� ��������� ���� ������
			/*	���� ������� ����������	*/
	for (int i = 0;i < 16;i++)	// ���������� ���������� ��������� ����� ������� �����
	{
		z1[i].x = (long)(250 + tz1[i].x*cos(alpha) + tz1[i].y*sin(alpha));	//����������
		z1[i].y = (long)(250 - tz1[i].y*cos(alpha) + tz1[i].x*sin(alpha));
	}
	for (int i = 0;i < 8;i++)
	{
		z2[i].x = (long)(250 + tz2[i].x*cos(alpha) + tz2[i].y*sin(alpha));	//�����������
		z2[i].y = (long)(250 - tz2[i].y*cos(alpha) + tz2[i].x*sin(alpha));
	}
	switch (messg)		/*	������� ������ �� ��, ���� ���� ������ ����� messg	*/
	{
		case WM_PAINT:						// ����������� ���������		
			hdc = BeginPaint(hWnd, &ps);	// ������ ���������
			SetBkMode(hdc, TRANSPARENT);	// ����� ����������������
			hpen1 = CreatePen(PS_SOLID, 1, RGB(0, 0, 0));	//���� �� - ������
			hpenOld = (HPEN)SelectObject(hdc, hpen1);
			/* ===================== � � � � � � �  ��Ϊ� ������� =================*/
			/* �������� ���� ��� ������������ ����� ������ ��������:*/							
			hbrush = CreateSolidBrush(RGB(150, 0, 150));	// ���� (�������������) ������� 
			hbrushOld = (HBRUSH)SelectObject(hdc, hbrush);	// ��������� "������"
			Polygon(hdc, z1, 16);							// ������������ ������������
			hbrush = CreateSolidBrush(RGB(255, 255, 255));	// ���� ������� ���� 
			hbrushOld = (HBRUSH)SelectObject(hdc, hbrush);	// ��������� "������"
			Polygon(hdc, z2, 8);							// "��������" ������. �������
			hpen1 = CreatePen(PS_SOLID, 3, RGB(0, 0, 0));	// ������� �� 3 ���.
			hpenOld = (HPEN)SelectObject(hdc, hpen1);		// ������ ���� ��� ��������� ��
			/*  �������� ����� ������� � ��� ��������:	*/
			TextOut(hdc, 600, 30, L"���� �����   : � ��� �" , 25);
			TextOut(hdc, 600, 60, L"���������    :", 15);	
			MoveToEx(hdc, 720, 75, NULL);
			LineTo(hdc, 720, 55);							// ���������� 
			LineTo(hdc, 715, 63);							// ��������
			LineTo(hdc, 725, 63);							// �����
			LineTo(hdc, 720, 55);
			TextOut(hdc, 600, 90, L"���������     :", 15);
			MoveToEx(hdc, 720,105, NULL);
			LineTo(hdc, 715, 98);							// ���������� 
			LineTo(hdc, 725, 98);							// ��������
			LineTo(hdc, 720, 105);							// ����
			LineTo(hdc, 720, 85);
			TextOut(hdc, 590 , 120, L"������� ����    :", 18);
			MoveToEx(hdc, 715, 130, NULL);
			LineTo(hdc, 721, 125);							// ���������� 
			LineTo(hdc, 721, 135);							// ��������
			LineTo(hdc, 715, 130);
			LineTo(hdc, 735, 130);
			TextOut(hdc, 590, 150, L"������� ������ :", 16);
			MoveToEx(hdc, 715, 160, NULL);
			LineTo(hdc, 735, 160);							// ���������� 
			LineTo(hdc, 729, 155);							// ��������
			LineTo(hdc, 729, 165);							// ������
			LineTo(hdc, 735, 160);
			TextOut(hdc, 600, 190, L"� � � � �           :  'Esc'", 29);
			
		case WM_KEYDOWN:
			key = wParam;					//	�������� ��� ��������� ������
			_itow_s(key, szText, 4, 10);	//	������������ � ���������� ����� - ���������:
											// 1-���; 2-�������. ������������; 
											// 3-����� ����� � ����� ��� �������; 
											// 4-������� �������� 
			hdc = GetDC(hWnd);				// ��������� ��������� ���������
			switch (key)					// ��������� ������:
			{	
				case 188:						// ���� ����� "�"
					for (int i = 0;i < 16;i++)	// ���������� ������ ����� ����� �
					{
						bx = tz1[i].x; tz1[i].x = bz1[i].x; bz1[i].x = bx;
						by = tz1[i].y; tz1[i].y = bz1[i].y; bz1[i].y = by;
					};
					for (int i = 0;i < 8;i++)
					{
						bx = tz2[i].x; tz2[i].x = bz2[i].x; bz2[i].x = bx;
						by = tz2[i].y; tz2[i].y = bz2[i].y; bz2[i].y = by;
					};
					break;
				case 221:						// ˳����"�"
					for (int i = 0;i < 16;i++)	// ���������� ������ ����� ����� �
					{
						bx = tz1[i].x; tz1[i].x = bz1[i].x; bz1[i].x = bx;
						by = tz1[i].y; tz1[i].y = bz1[i].y; bz1[i].y = by;
					};
					for (int i = 0;i < 8;i++)
					{
						int bx = tz2[i].x; tz2[i].x = bz2[i].x; bz2[i].x = bx;
						int by = tz2[i].y; tz2[i].y = bz2[i].y; bz2[i].y = by;
					};
					break;
				case 39:						// ����� ������ ��������� (������ ����)
					alpha =alpha+ M_PI / 6.0;	// ��� ������ �������� �� 30 �������
					break;
				case 38:						// ��������� (������ �����)
					k *= 1.2;					// �� 20%
					break;
				case 37:						// �� ������� ��������� (������ ������)
					alpha = alpha- M_PI / 6.0;	// ��� ������ �������� �� 30 �������
					break;						
				case 40:						// ��������� (������ ����)
					k *= 0.85;					// �� 15%
					break;
				case 27:						// ����� (Esc)
					TextOut(hdc, 150, 630, L"����� �� �����.   �����������!", 31);
					Sleep(1500);
					PostQuitMessage(0);			// ������ ����� � �������� "ʲ����"
					break;
			}
				/*  "�������" ����� - ��������� ��������� ������� ��� ����������  */
				hpen1 = CreatePen(PS_SOLID, 1, RGB(255, 255, 255));	//���� �� - ����
				hpenOld = (HPEN)SelectObject(hdc, hpen1);
				hbrush = CreateSolidBrush(RGB(255, 255, 255));	// ������� ������ (����)
				hbrushOld = (HBRUSH)SelectObject(hdc, hbrush);	// ��������� "������"
				Rectangle(hdc, 0, 0, 590, 600);					// �������� ���� ���������
				for (int i = 0;i < 16;i++)	// ���������� ������ ��������� ����� ������� �����
				{
					z1[i].x = (long)(250 + (tz1[i].x*cos(alpha) + tz1[i].y*sin(alpha))*k);
					z1[i].y = (long)(250 - (tz1[i].y*cos(alpha) - tz1[i].x*sin(alpha))*k);
				}
				for (int i = 0;i < 8;i++)
				{
					z2[i].x = (long)(250 + (tz2[i].x*cos(alpha) + tz2[i].y*sin(alpha))*k);
					z2[i].y = (long)(250 - (tz2[i].y*cos(alpha) - tz2[i].x*sin(alpha))*k);
				}
				hpen1 = CreatePen(PS_SOLID, 1, RGB(0, 0, 0));	//���� �� - ������
				hpenOld = (HPEN)SelectObject(hdc, hpen1);		//���������� ������� ��� ���������

				hbrush = CreateSolidBrush(RGB(150, 0, 150));	// ���� (�������������) ������� 
				hbrushOld = (HBRUSH)SelectObject(hdc, hbrush);	// ��������� "������"
				Polygon(hdc, z1, 16);
				hbrush = CreateSolidBrush(RGB(255, 255, 255));	// ���� ���� 
				hbrushOld = (HBRUSH)SelectObject(hdc, hbrush);	// ��������� "������"
				Polygon(hdc, z2, 8);			
		
			/*===    � � � � � �   "��Ϊ�" �������    ==============*/
			ValidateRect(hWnd, NULL);		// ���������� ����	
			EndPaint(hWnd, &ps);			// ��������� ���������
			break;
	
		case WM_DESTROY:
			PostQuitMessage(0); //��������� ����������� ������ � ����� 0 - ��������� ����������
			break;
		default:				//��������� ����� ������� �� ������������ ������
			return(DefWindowProc(hWnd, messg, wParam, lParam)); 
			
		}	
	 return 0;
}
